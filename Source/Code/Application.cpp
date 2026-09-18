#include "Application.hpp"
#include "Logger.hpp"
#include "Tray.hpp"
#include "UrlTools.hpp"
#include "ModuleManager.hpp"
#include "Settings.hpp"

// Инициализация статчексих переменных
static Application* application = nullptr;

// Конструктор и деструктор
Application::Application(HINSTANCE instance) : instance(instance), isNewVersionAvailable(false)
{
    // Установка общего объекта
    application = this;

    // Получение полного пути папки приложения
    wchar_t appdataPath[MAX_PATH];
    ExpandEnvironmentStringsW(APPDATA_ENVIRONMENT, appdataPath, MAX_PATH);
    CreateDirectoryW(appdataPath, nullptr);
    this->appdataPath = appdataPath;

    // Создание объекта работы с журналом
    logger = new Logger{ this };
    logger->Log(L"Инициализация приложения", LogType::ACTION);

    // Создание класса окна приложения
    WNDCLASSW trayClass{};
    trayClass.lpfnWndProc = WindowCallback;
    trayClass.hInstance = instance;
    trayClass.lpszClassName = TRAY_WINDOW_CLASS;
    RegisterClassW(&trayClass);

    // Создание окна
    window = CreateWindowW(TRAY_WINDOW_CLASS, TRAY_WINDOW_CLASS, 0, 0, 0, 0, 0, nullptr, nullptr, instance, nullptr);

    // Создание объекта работы с треем
    tray = new Tray{ this };

    // Проверка на новую версию
    if (CheckNewVersion())
    {
        isNewVersionAvailable = true;
        tray->AddNewVersion();
        logger->Log(L"Вышла новая версия", LogType::COMMENT);
    }
    else SetTimer(window, static_cast<unsigned>(TimerID::CHECK_NEW_VERSION), VERSION_CHECK_INTERVAL, nullptr);

    // Инициализация всех модулей
    moduleManager = new ModuleManager{ this };
}
Application::~Application()
{
    // Уведомляем о завершении
    logger->Log(L"Завершение приложения", LogType::ACTION, 3);

    // Остановка таймера
    if(!isNewVersionAvailable)
        KillTimer(window, static_cast<UINT_PTR>(TimerID::CHECK_NEW_VERSION));

    // Удаление класса окна приложения
    UnregisterClassW(TRAY_WINDOW_CLASS, instance);

    // Очистка динамической памяти
    delete moduleManager;
    delete tray;
    delete logger;
}

// Методы класса
HINSTANCE Application::GetInstance() const { return instance; }
const std::wstring* Application::GetAppDataPath() const { return &appdataPath; }
HWND Application::GetWindow() const { return window; }
bool Application::IsNewVersionAvailable() const { return isNewVersionAvailable; }
const Logger* Application::GetLogger() const { return logger; }
void Application::OpenSettings() const { Settings::Open(instance); }
void Application::Run()
{
    // Обработка сообщений
    MSG message{};
    while (GetMessageW(&message, nullptr, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
}

// Методы обработки запросов
LRESULT Application::ApplicationProcessing(HWND window, unsigned message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_HOTKEY:
    {
        // Обработка хоткеев
        moduleManager->HotkeyProcessing(static_cast<ModuleType>(wParam));
        return 0;
    }
    case WM_TIMER:
    {
        // Обработка таймеров
        switch (static_cast<TimerID>(wParam))
        {
        case TimerID::CHECK_NEW_VERSION:
        {
            if (CheckNewVersion())
            {
                isNewVersionAvailable = true;
                tray->AddNewVersion();
                logger->Log(L"Вышла новая версия", LogType::COMMENT);
                KillTimer(window, wParam);
            }
            break;
        }
        default: break;
        }
        return 0;
    }
    case WM_USER:
    {
        // Обработка сообщений иконки
        tray->TrayProcessing(lParam);
        return 0;
    }
    case WM_COMMAND:
    {
        // Обработка кнопок меню
        tray->MenuProcessing(static_cast<TrayButtons>(LOWORD(wParam)));
        return 0;
    }
    case WM_QUERYENDSESSION:
        return TRUE;
    case WM_ENDSESSION:
    {
        // Проверка на подтверждение завершении сессии
        if (wParam)
            DestroyWindow(window);
        break;
    }
    case WM_DESTROY:
    {
        // Запрос на остановку обработки сообщений
        PostQuitMessage(0);
        break;
    }
    default:
    {
        // При перезапуске таксбара - перезапуск иконки в трее
        if (tray)
            tray->TaskbarProcessing(message);
        break;
    }  
    }
    return DefWindowProcW(window, message, wParam, lParam);
}

// Функции обратного вызова
LRESULT Application::WindowCallback(HWND window, unsigned message, WPARAM wParam, LPARAM lParam) { return application->ApplicationProcessing(window, message, wParam, lParam); }

// Приватные методы класса
bool Application::CheckNewVersion()
{
    // Получение версии приложения
    std::wstring githubJSON = UrlTools::Request(GITHUB_RELEASE_URL, L"Accept: application/json\r\n");
    if (githubJSON.empty())
        return false;

    // Получение версии по тэгу
    std::wstring githubVersion = UrlTools::FindTagValue(githubJSON, L"tag_name");
    if (githubVersion.empty())
        return false;
    
    // Подготовка версии
    githubVersion.erase(0, 1);
    std::wstring currentVersion = VERSION;

    // Сравнение версий
    int githubVersionID = 0;
    int currentVersionID = 0;
    while (!githubVersion.empty() || !currentVersion.empty())
    {
        githubVersionID = FindVersionComponent(githubVersion);
        currentVersionID = FindVersionComponent(currentVersion);
        if (githubVersionID > currentVersionID)
            return true;
        else if (githubVersionID != currentVersionID)
            return false;
    }
    return false;
}
int Application::FindVersionComponent(std::wstring& version)
{
    // Возврат нуля при отсутствии компонента
    if (version.empty())
        return 0;

    // Получение текущего компонента
    int result = 0;
    std::size_t dot = version.find(L'.');
    if (dot != std::wstring::npos)
    {
        result = std::stoi(version.substr(0, dot));
        version.erase(0, dot + 1);
    }
    else
    {
        result = std::stoi(version);
        version.clear();
    }

    // Возрат результата
    return result;
}