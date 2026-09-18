#include "Tray.hpp"
#include "Application.hpp"
#include "ResourceDefines.h"

// Конструктор и деструктор
Tray::Tray(const Application* application) : application(application), dataID(0)
{
    // Загрузка иконки приложения
	icon = reinterpret_cast<HICON>(LoadImageW(application->GetInstance(), MAKEINTRESOURCEW(ICON_FMT), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE));
    baloonIcon = reinterpret_cast<HICON>(LoadImageW(application->GetInstance(), MAKEINTRESOURCEW(ICON_FMT_NEW_VERSION), IMAGE_ICON, 256, 256, LR_DEFAULTSIZE));
    
    // Регестрация сообщения перезагрузки таскбара
    taskbarRestartMessage = RegisterWindowMessageW(L"TaskbarCreated");

    // Создание уведомления
    Restart();
}
Tray::~Tray()
{
    // Создание объекта уведомления
    NOTIFYICONDATAW data{};
    CreateData(&data);

    // Удаление уведомления
    Shell_NotifyIconW(NIM_DELETE, &data);

    // Удаление иконок
    DestroyIcon(baloonIcon);
    DestroyIcon(icon);
}

// Методы класса
void Tray::AddNewVersion()
{
    // Замена иконки
    DestroyIcon(icon);
    icon = reinterpret_cast<HICON>(LoadImageW(application->GetInstance(), MAKEINTRESOURCEW(ICON_FMT_NEW_VERSION), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE));

    // Обновление уведомления
    Restart();
}
void Tray::Restart()
{
    // Создание объекта уведомления
    NOTIFYICONDATAW data{};
    CreateData(&data);

    // Умное добавление уведомления
    if (!Shell_NotifyIconW(NIM_MODIFY, &data))
    {
        data.uID = 0;
        if (Shell_NotifyIconW(NIM_ADD, &data))
        {
            Shell_NotifyIconW(NIM_SETVERSION, &data);
            dataID = data.uID;
        }
    }
}

// Методы класса
void Tray::Show() const
{
    // Создание меню
    HMENU menu = CreatePopupMenu();
    if (!menu)
        return;

    // Кнопка новой версии
    if (application->IsNewVersionAvailable())
    {
        AddButton(menu, 0, static_cast<unsigned>(TrayButtons::NEW_VERSION), L"Доступно обновление", 0);
        AddButton(menu, MF_SEPARATOR, 0, nullptr, 0);
    }

    // Кнопка настроек
    AddButton(menu, 0, static_cast<unsigned>(TrayButtons::SETTINGS), L"Настройки", BITMAP_SETTINGS);
    AddButton(menu, MF_SEPARATOR, 0, nullptr, 0);

    // Кнопка выхода
    AddButton(menu, 0, static_cast<unsigned>(TrayButtons::EXIT), L"Выход", BITMAP_EXIT);

    // Открытие меню по центру мыши 
    POINT point{};
    GetCursorPos(&point);
    SetForegroundWindow(application->GetWindow());
    TrackPopupMenuEx(menu, TPM_BOTTOMALIGN | TPM_CENTERALIGN, point.x, point.y, application->GetWindow(), nullptr);
    DestroyMenu(menu);
}
void Tray::OpenGithub() const { ShellExecuteW(application->GetWindow(), nullptr, GITHUB_RELEASE_URL, nullptr, nullptr, SW_SHOWNORMAL); }

// Методы обработки запросов
void Tray::TrayProcessing(LPARAM lParam)
{
    switch (lParam)
    {
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    {
        Show();
        break;
    }
    case WM_LBUTTONUP:
    {
        application->OpenSettings();
        break;
    }
    case NIN_BALLOONUSERCLICK:
    {
        OpenGithub();
        break;
    }
    default: break;
    }
}
void Tray::MenuProcessing(TrayButtons button)
{
    switch (button)
    {
    case TrayButtons::SETTINGS:
    {
        application->OpenSettings();
        break;
    }
    case TrayButtons::EXIT:
    {
        DestroyWindow(application->GetWindow());
        break;
    }
    case TrayButtons::NEW_VERSION:
    {
        OpenGithub();
        break;
    }
    default: break;
    }
}
void Tray::TaskbarProcessing(unsigned message)
{
    // При перезапуске таксбара - перезапуск иконки в трее
    if (message == taskbarRestartMessage)
        Restart();
}

// Приватные методы класса
void Tray::AddButton(HMENU menu, unsigned flags, unsigned id, const wchar_t* text, unsigned bitmapId) const
{
    // Добавление кнопки
    AppendMenuW(menu, flags, id, text);

    // Установка изображения
    if (bitmapId)
        SetMenuItemBitmaps(menu, id, MF_BYCOMMAND, reinterpret_cast<HBITMAP>(LoadImageW(application->GetInstance(), MAKEINTRESOURCEW(bitmapId), IMAGE_BITMAP, 0, 0, LR_SHARED)), nullptr);
}
 void Tray::CreateData(NOTIFYICONDATAW* data) const
{
    // Заполнение данных значка в трее
    data->cbSize = sizeof(NOTIFYICONDATAW);
    data->hWnd = application->GetWindow();
    data->uID = dataID;
    data->uVersion = NOTIFYICON_VERSION_4;
    data->uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    data->uCallbackMessage = WM_USER;
    data->hIcon = icon;
    lstrcpyW(data->szTip, TRAY_TIP);

    // Заполнение данных о новой версии
    if (application->IsNewVersionAvailable())
    {
        // Добавление флага о "Balloon" уведомлении
        data->uFlags |= NIF_INFO;

        // Заполнение данных уведомления
        data->hBalloonIcon = baloonIcon;
        data->dwInfoFlags = NIIF_USER | NIIF_LARGE_ICON | NIIF_RESPECT_QUIET_TIME;
        lstrcpyW(data->szInfoTitle, TRAY_BALLOON_TITLE);
        lstrcpyW(data->szInfo, TRAY_BALLOON_BODY);
    }
}
