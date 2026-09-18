#include "ModuleManager.hpp"
#include <format>
#include <fstream>
#include "Application.hpp"
#include "SwapLayoutModule.hpp"
#include "InvertCaseModule.hpp"
#include "WebSearchModule.hpp"
#include "WebTranslateModule.hpp"
#include "ApiTranslateModule.hpp"
#include "Clipboard.hpp"
#include "Logger.hpp"
#include "KeyboardTools.hpp"

// Конструктор и деструктор
ModuleManager::ModuleManager(const Application* application) : application(application)
{
	// Получение пути к файлу настроек
	settingsPath = std::format(L"{}\\{}", *application->GetAppDataPath(), MODULES_FILE);

	// Создание модулей
	HWND window = application->GetWindow();
	modules.push_back(new SwapLayoutModule{ SWAP_LAYOUT_HOTKEY, true });
	modules.push_back(new InvertCaseModule{ INVERT_CASE_HOTKEY, true });
	modules.push_back(new WebSearchModule{ WEB_SEARCH_HOTKEY, true });
	modules.push_back(new WebTranslateModule{ WEB_TRANSLATE_HOTKEY, true });
	modules.push_back(new ApiTranslateModule{ API_TRANSLATE_HOTKEY });

	// Открытие файла на чтение
	std::wifstream file(settingsPath);

	// Инициализация настроек
	for (Module* module : modules)
	{
		// Чтение настроек
		module->ReadSettings(&file);

		// Регистрация хоткеев
		if (module->IsEnable())
			module->Register(window);
	}	
}
ModuleManager::~ModuleManager()
{
	// Открытие файла на запись
	std::wofstream file(settingsPath);

	// Удаление всех модулей
	HWND window = application->GetWindow();
	for (Module* module : modules)
	{
		// Отмена регистрации хоткея
		module->Unregister(window);
		
		// Запись данных в файл
		module->WriteSettings(&file);
		file << L'\n';

		// Удаление из кучи
		delete module;
	}
}

// Методы обработки запросов
void ModuleManager::HotkeyProcessing(ModuleType type)
{
	// Получение переменных
	const Logger* logger = application->GetLogger();
	HWND window = application->GetWindow();
	Module* module = modules[static_cast<BYTE>(type) - 1];
	Clipboard original{}, selected{};

	// Уведомление о запуске обработки
	logger->Log(L"Запуск обработки", LogType::COMMENT);

	// Отпускание проблемных клавиш
	EmulateInput({ VK_LMENU, VK_RMENU, VK_LCONTROL, VK_RCONTROL, VK_LSHIFT, VK_RSHIFT, VK_LWIN, VK_RWIN, module->GetHotkey().GetKey() }, true);

	// Получение данных из буфера обмена на сохранение
	original.Read(window);
	original.Log(logger, L"[GET] Получение данных на сохранение");

	// Очистка буфера обмена и сброс привязки
	original.Clear();

	// Эмуляция копирования через Ctrl + C
	EmulateInput({ VK_CONTROL, 'C' }, false);
	EmulateInput({ VK_CONTROL, 'C' }, true);
	for (BYTE i = 0; i != CLIPBOARD_COPY_ATTEMPTS_COUNT; i++)
	{
		// Проверка на наличие в буфере текста
		if (IsClipboardFormatAvailable(CF_UNICODETEXT))
			break;

		// Небольшая пауза перед повторной попыткой
		Sleep(USER_TIMER_MINIMUM);
	}

	// Получение данных из буфера обмена для работы
	selected.Read(window);
	selected.Log(logger, L"[GET] Получение данных для работы");

	// Проверки полученной информации
	if (selected.IsEmpty())
	{
		// Уведомляем об отсутстивии выделенного текста
		MessageBeep(MB_ICONWARNING);
		logger->Log(L"Ничего не выделено", LogType::ACCEPTABLE_ERROR);
	}
	else if (selected.GetFormat() != CF_UNICODETEXT)
	{
		// Уведомляем об отсутстивии выделенного текста
		MessageBeep(MB_ICONWARNING);
		logger->Log(L"Полученный тип данных - не текст", LogType::ACCEPTABLE_ERROR);
	}
	else
	{
		// Установка курсора ожидания
		SetCursor(LoadCursorW(nullptr, IDC_WAIT));

		// Отправка текст в модуль
		std::wstring result = module->Execute(selected.GetText());

		// При наличии результата вставка в буфер обмена
		if (!result.empty())
		{
			selected.SetText(result);
			selected.Log(logger, L"[SET] Вставка изменнёных рабочих данных");
			selected.Write(window);
	
			// Эмуляция вставки через Ctrl + V
			EmulateInput({ VK_CONTROL, 'V' }, false);
			EmulateInput({ VK_CONTROL, 'V' }, true);
			Sleep(CLIPBOARD_PASTE_TIMEOUT);
		}
	}

	// Вставка изначальной информации в буфер обмена
	original.Log(logger, L"[SET] Вставка сохранённых данных");
	original.Write(window);

	// Возвращение обычного
	SetCursor(LoadCursorW(nullptr, IDC_ARROW));

	// Уведомление о завершении обработки
	logger->Log(L"Завершение обработки", LogType::COMMENT, 1);
}

// Приватные методы класса
void ModuleManager::EmulateInput(std::initializer_list<WORD> keys, bool isKeysUp)
{
	// Эмуляция клавиш
	if (KeyboardTools::EmulateInput(keys, isKeysUp))
		application->GetLogger()->Log(L"Ошибка при эмуляции клавиш", LogType::ACCEPTABLE_ERROR);
}