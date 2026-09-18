#include "Graphics.hpp"
#include <dwmapi.h>
#include <stdexcept>
#include "RootClass.hpp"
#include "MouseEvents.h"

// Инициализация статчексих переменных
Graphics* Graphics::graphics = 0;

// Конструктор и деструкотр
Graphics::Graphics(HINSTANCE instance, D2Size windowSize, D2Size minWindowSize, const ColorRGBA& bgColor) : instance(instance), minWindowSizeLogical(minWindowSize),
	factory(nullptr), writeFactory(nullptr), imageFactory(nullptr), deviceContext(nullptr), swapChain(nullptr),	targetBitmap(nullptr), root(nullptr),
	mouseHover(nullptr), mousePress(nullptr), activeMousePress(nullptr), mouseScroll(nullptr), window(nullptr), brush(nullptr), penStyle(nullptr),
	timer(0), needRebuild(false), needBlockUpdate(false), needHoverUpdate(false)
{	
	// Установка общего объекта
	graphics = this;

	// Получение соотношения DPI
	dpiRatio = static_cast<float>(GetDpiForSystem()) / USER_DEFAULT_SCREEN_DPI;

	// Регистрация класса окна настроек
	WNDCLASS windowClass;
	ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.lpfnWndProc = WindowCallback;
	windowClass.hInstance = instance;
	windowClass.lpszClassName = SETTINGS_WINDOW_CLASS;
	RegisterClassW(&windowClass);
	
	// Получение частоты дисплея
	DEVMODEW devMode{};
	devMode.dmSize = sizeof(devMode);
	EnumDisplaySettingsW(0, ENUM_CURRENT_SETTINGS, &devMode);
	displayFrequencyMs = 1000 / devMode.dmDisplayFrequency;

	// Получение частоты таймера
	TIME timerFrequencyS;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&timerFrequencyS));
	timerFrequencyMs = timerFrequencyS / 1000.f;

	// Установка корня
	BlockClass::count = 0;
	root = new RootClass{ bgColor };

	// Установка минимальных размеров окна
	SetMinWindowSizePhysical(minWindowSize);

	// Создание окна
	AdjustDPISize(&windowSize);
	AdjustWindowSize(&windowSize);
	CreateWindowExW(0, SETTINGS_WINDOW_CLASS, SETTINGS_WINDOW_NAME, WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX | WS_TABSTOP | WS_THICKFRAME,
		(GetSystemMetrics(SM_CXSCREEN) - windowSize.width) / 2, (GetSystemMetrics(SM_CYSCREEN) - windowSize.height) / 2,
		windowSize.width, windowSize.height, nullptr, nullptr, instance, nullptr);
}
Graphics::~Graphics()
{
	// Удаление всех блоков
	DestroyBlockProcessing(root);

	// Удаление всех анимаций
	if (timer)
		timeKillEvent(timer);

	// Освобождение ресурсов Direct2D
	if (penStyle) penStyle->Release();
	if (brush) brush->Release();
	if (targetBitmap) targetBitmap->Release();
	if (swapChain) swapChain->Release();
	if (deviceContext) deviceContext->Release();
	if (imageFactory) imageFactory->Release();
	if (writeFactory) writeFactory->Release();
	if (factory) factory->Release();
	CoUninitialize();

	// Удаление класса окна настроек
	UnregisterClassW(SETTINGS_WINDOW_CLASS, instance);

	// Сброс общего объекта
	graphics = nullptr;
}

// Цикл обработки сообщений (временно)
void Graphics::MessageLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (GetMessageW(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

// Методы отправки запросов
void Graphics::DestroyBlockRequest(Block block) { PostGraphicMessage(GM_DESTROY_BLOCK, reinterpret_cast<WPARAM>(block)); }
void Graphics::RebuildRequest()
{
	// Попытка отправки сообщения на обновление
	TryPostUpdateMessage();
	
	// Установка графике флага вызова перестройки
	needRebuild = true;
}
void Graphics::UpdateRequest(Block block)
{
	// Установка блоку флага обновления 
	(block ? block : root)->needUpdate = true;

	// Попытка отправки сообщения на обновление
	TryPostUpdateMessage();

	// Установка графике флага вызова обновления блока
	needBlockUpdate = true;
}
void Graphics::HoverUpdateRequest()
{
	// Попытка отправки сообщения на обновление
	TryPostUpdateMessage();

	// Установка графике флага вызова обновления наведения курсора
	needHoverUpdate = true;
} 
void Graphics::CreateAnimationRequest(Animation animation) { PostGraphicMessage(GM_CREATE_ANIMATION, reinterpret_cast<WPARAM>(animation)); }

// Методы класса
float Graphics::GetDpiRatio() const { return dpiRatio; }
POINT Graphics::GetCursorPoint() const
{
	// Получение позиции курсора в клиентской области окна
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(window, &pt);

	// Коррекция позиции в зависимости от DPI 
	pt = { static_cast<LONG>(pt.x / dpiRatio), static_cast<LONG>(pt.y / dpiRatio) };
	return pt;
}
HWND Graphics::GetWindow() const { return window; }
DWFactory Graphics::GetWriteFactory() const { return writeFactory; }
DIFactory Graphics::GetImageFactory() const { return imageFactory; }
D2DeviceContext Graphics::GetDeviceContext() const { return deviceContext; }
D2Brush Graphics::GetBrush() const { return brush; }
D2PenStyle Graphics::GetPenStyle() const { return penStyle; }
Root Graphics::GetRoot() const { return root; }
void Graphics::SetCursorImage(LPCWSTR name) { SetCursor((HCURSOR)LoadImageW(0, name, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED)); }

// Методы обработки запросов
void Graphics::CreateWindowProcessing(HWND window)
{
	// Установка окна
	this->window = window;

	// Включение масштабирования DPI для неклиентской области
	EnableNonClientDpiScaling(window);

	// Установка темной темы окна
	BOOL isDarkMode = true;
	DwmSetWindowAttribute(window, DWMWA_USE_IMMERSIVE_DARK_MODE, &isDarkMode, sizeof(isDarkMode));

	// Инициализация Direct фабрик
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), nullptr, reinterpret_cast<void**>(&factory))))
		throw std::runtime_error("CreateFactory ERROR");
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,	__uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&writeFactory))))
		throw std::runtime_error("CreateWriteFactory ERROR");
	if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)) || FAILED(CoCreateInstance(CLSID_WICImagingFactory, 0, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&imageFactory))))
		throw std::runtime_error("CreateImageFactory ERROR");

	// Создание D3D11 устройства и контекста
	D3Device d3Device = nullptr;
	D3DeviceContext d3Context = nullptr;
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0, D3D11_SDK_VERSION, &d3Device, nullptr, &d3Context)))
		throw std::runtime_error("CreateDevice ERROR");
	d3Context->Release();
	
	// Получение DXGI устройства
	DXDevice dxDevice = nullptr; 
	if (FAILED(d3Device->QueryInterface(IID_PPV_ARGS(&dxDevice))))
		throw std::runtime_error("QueryInterface ERROR");

	// Получение адаптера
	DXAdapter dxAdapter = nullptr;
	if (FAILED(dxDevice->GetAdapter(&dxAdapter)))
		throw std::runtime_error("GetAdapter ERROR");
	
	// Получение фабрики цепочки обмена
	DXFactory dxFactory = nullptr;
	if (FAILED(dxAdapter->GetParent(IID_PPV_ARGS(&dxFactory))))
		throw std::runtime_error("GetParent ERROR");
	dxAdapter->Release();

	// Создание устройства и контекста Direct2D
	D2Device d2Device = nullptr;
	if (FAILED(factory->CreateDevice(dxDevice, &d2Device)))
		throw std::runtime_error("CreateDevice ERROR");
	dxDevice->Release();
	if (FAILED(d2Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &deviceContext)))
		throw std::runtime_error("CreateDeviceContext ERROR");
	d2Device->Release();

	// Создание цепочки обмена
	DXSwapChainDescription swapChainDescription{};
	swapChainDescription.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDescription.SampleDesc.Count = 1;
	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescription.BufferCount = 2;
	if (FAILED(dxFactory->CreateSwapChainForHwnd(d3Device, window, &swapChainDescription, nullptr, nullptr, &swapChain)))
		throw std::runtime_error("CreateSwapChain ERROR");
	dxFactory->Release();
	d3Device->Release();

	// Обновление целевого битмапа
	UpdateTargetBitmap();

	// Создание кисти и стиля пера
	deviceContext->CreateSolidColorBrush(ColorF(0), &brush);
	deviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
	factory->CreateStrokeStyle(StrokeStyleProperties(D2D1_CAP_STYLE_ROUND, D2D1_CAP_STYLE_ROUND, D2D1_CAP_STYLE_ROUND), 0, 0, &penStyle);

	// Вызов перестройки для добавления корня
	RebuildRequest();
}
void Graphics::PaintProcessing()
{
	// Начало отрисовки
	PAINTSTRUCT ps;
	BeginPaint(window, &ps);
	deviceContext->BeginDraw();

	// Отрислвка всех узлов из вектора
	for (Block& node : blockVector)
	{
		// Отмена обрезки если узел - метка
		if (!node)
		{
			deviceContext->PopAxisAlignedClip();
			continue;
		}

		// Обновление узла
		node->Paint();

		// Обрезка детей (Оставление метки для отмены обрезки)
		if (node->isCropper)
			deviceContext->PushAxisAlignedClip(node->rect.GetRect(), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	}

	// Конец отрисовки
	deviceContext->EndDraw();
	swapChain->Present(1, 0);
	EndPaint(window, &ps);
}
void Graphics::DestroyBlockProcessing(Block block)
{
	// Проверка на пустой блок
	if (!block)
		return;

	// Рекурсивное удаление блока и всех его детей
	DeleteBlockTree(block);

	// Вызов перестройки вектора блоков, если корень ещё существует
	if (root && window)
	{
		blockVector.clear();
		RebuildRequest();
	}
}
void Graphics::UpdateProcessing()
{
	// Перестройка вектора блоков
	if (needRebuild)
	{
		// Очистка предыдущих значений
		blockVector.clear();

		// Добавление корня в стэк
		BlockStack blockStack;
		blockStack.push(root);

		// Основной цикл просчёта вектора
		Block node;
		while (!blockStack.empty())
		{
			// Получение узла
			node = blockStack.top();
			blockStack.pop();

			// Добаление узла в итоговый вектор
			blockVector.push_back(node);

			// Проверка на метку (для отмены обрезки)
			if (!node)
				continue;

			// Оставление метки для отмены обрезки
			if (node->isCropper)
				blockStack.push(nullptr);

			// Установка ссылки на обрезающий блок
			if(node->parent)
				node->cropper = node->parent->isCropper ? node->parent : node->parent->cropper;

			// Добавление детей в стэк
			if (node->children)
				for (BlockVector::reverse_iterator child = node->children->rbegin(); child != node->children->rend(); child++)
					blockStack.push(*child);
		}

		// Сброс флага
		needRebuild = false;
		
		// Запрос на обновление узлов
		UpdateRequest();
	}

	// Обновление узлов
	if (needBlockUpdate)
	{
		// Обновление всех узлов из вектора
		for (Block& node : blockVector)
			if (node && node->needUpdate)
			{
				// Обновление узла
				node->Update();

				// Маркировка детей
				if (node->children)
					for (Block& child : *node->children)
						child->needUpdate = true;

				// Сброс флага
				node->needUpdate = false;
			}

		// Сброс флага
		needBlockUpdate = false;

		// Запрос на обновление блока наведения курсора
		HoverUpdateRequest();
	}

	// Обновление блока наведения курсора
	if (needHoverUpdate)
	{
		// Установка курсора по умолчанию (сброс)
		SetCursorImage();

		// Сброс блоков наведения
		Block oldMouseHover = mouseHover;
		mouseHover = nullptr;
		mousePress = nullptr;
		mouseScroll = nullptr;

		// Поиск верхнего блока
		Block node;
		bool mouseInside = false;
		POINT cursor = GetCursorPoint();
		for (BlockVector::reverse_iterator it = blockVector.rbegin(); it != blockVector.rend(); it++)
		{
			// Получение узла
			node = *it;

			// Проверка на существование
			if (!node)
				continue;

			// Проверка на попадание курсора в хитбокс узла
			mouseInside = node->hitbox && node->hitbox->IsPointInside(cursor);
			if (!mouseInside)
				continue;

			// Установка курсора если узел обрабатывает наведение
			if (!mouseHover && node->handlesHover) mouseHover = node;
			if (!mousePress && node->handlesPress) mousePress = node;
			if (!mouseScroll && node->handlesScroll) mouseScroll = node;

			// Выход из цикла если всё найдено
			if (mouseHover && mousePress && mouseScroll)
				break;
		}

		// Обработка событий наведения
		if (mouseHover != oldMouseHover)
		{
			// Событие выхода со старого блока
			if (oldMouseHover)
				oldMouseHover->ExecuteEvent(MouseEventType::EXIT);

			// Событие входа в новый блок
			if (mouseHover)
				mouseHover->ExecuteEvent(MouseEventType::ENTER);
		}

		// Сброс флага
		needHoverUpdate = false;
	}

	// Вызов отрисовки
	InvalidateRect(window, 0, 0);
}
void Graphics::CreateAnimationProcessing(Animation animation)
{
	// Добаление анимации в один из векторов
	bool isAdded = false;
	if (animation->GetProperties()->GetDelay())
	{
		delayedVector.push_back(animation);
		isAdded = true;
	}
	else isAdded = AddAnimation(animation);

	// Вызов сообщения анимации
	if (isAdded)
		PostGraphicMessage(GM_ANIMATION);
}
void Graphics::AnimationProcessing()
{
	// Определение текущего времени
	TIME time = 0;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&time));

	// Обработка отложенных анимаций
	Animation node = nullptr;
	for (AnimationVector::iterator it = delayedVector.begin(); it != delayedVector.end();)
	{
		// Получение анимации
		node = *it;

		// Установка начала анимации
		if (!node->GetLaunch())
			node->SetLaunch(time + static_cast<TIME>(node->GetProperties()->GetDelay() * timerFrequencyMs));
		else if (node->GetLaunch() <= time) 		
		{
			// Добаление начавшейся анимации
			node->SetLaunch(0);
			AddAnimation(node);
			it = delayedVector.erase(it);
			continue;
		}

		// Инкрементируем итератор
		it++;
	}

	// Обработка запущенных анимаций
	float progress = 0;
	for (AnimationVector::iterator it = animateVector.begin(); it != animateVector.end();)
	{
		// Получение анимации
		node = *it;

		// Установка начала анимации
		if (!node->GetLaunch())
		{
			node->SetLaunch(time);
			node->SetStart();
		}

		// Определение прогресса анимации
		progress = static_cast<float>(time - node->GetLaunch()) / (node->GetProperties()->GetDuration() * timerFrequencyMs);
		if (progress > ANIMATION_PROGRESS_END)
			progress = ANIMATION_PROGRESS_END;

		// Пересчёт значения по кривой анимации
		node->Calculate(node->GetProperties()->CurveCalculate(progress));

		// Действия при окончании анимации
		if (progress == ANIMATION_PROGRESS_END)
		{
			// Вызов события окончания анимации
			node->ExecuteOnEndEvent();

			// Удаление закончившейся анимации
			delete node;
			it = animateVector.erase(it);
			continue;
		}
		
		// Инкрементируем итератор
		it++;
	}

	// Работа с таймером
	if (delayedVector.empty() && animateVector.empty())
	{
		timeKillEvent(timer);
		timer = 0;
	}
	else if (!timer)
		timer = timeSetEvent(displayFrequencyMs, 0, TimerCallback, 0, TIME_PERIODIC);

	// Отрисовка результата анимации
	InvalidateRect(graphics->window, 0, 0);
}

// Функции обратного вызова
LRESULT Graphics::WindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Обработка сообщений
	switch (message)
	{
	// Системные сообщения
	case WM_CREATE:
	{
		// Вызов создания окна
		graphics->CreateWindowProcessing(window);
		break;
	}
	case WM_SIZE:
	{
		// Игнорирование сворачивания окна
		if (wParam == SIZE_MINIMIZED)
			break;

		// Обновление размера отрисовки
		graphics->UpdateTargetBitmap(LOWORD(lParam), HIWORD(lParam));

		// Отправка запроса на обновление всех блоков
		graphics->UpdateRequest();
		break;
	}
	case WM_DPICHANGED:
	{
		// Обновление DPI
		float dpi = static_cast<float>(HIWORD(wParam));
		graphics->dpiRatio = dpi / USER_DEFAULT_SCREEN_DPI;

		// Установка нового DPI для рендера
		graphics->deviceContext->SetDpi(dpi, dpi);

		// Установка нового размера окна
		const RECT* newRect = reinterpret_cast<RECT*>(lParam);
		SetWindowPos(window, 0, newRect->left, newRect->top, newRect->right - newRect->left, newRect->bottom - newRect->top, SWP_NOZORDER | SWP_NOACTIVATE);

		// Коррекция минимального размера окна
		graphics->SetMinWindowSizePhysical(graphics->minWindowSizeLogical);

		// Отправка запроса на обновление всех блоков
		graphics->UpdateRequest();
		break;
	}
	case WM_MOUSEMOVE:
	{
		// Обработка удержания курсора
		if (graphics->activeMousePress)
			graphics->activeMousePress->ExecuteEvent(MouseEventType::HOLD, wParam, lParam);

		// Отправка запроса на обновление блока наведения курсора
		graphics->HoverUpdateRequest();
		break;
	}
	case WM_LBUTTONDOWN:
	{
		// Обработка нажатия курсора
		graphics->activeMousePress = graphics->mousePress;
		if (graphics->activeMousePress)
		{
			SetCapture(window);
			graphics->activeMousePress->ExecuteEvent(MouseEventType::PRESS, wParam, lParam);
		}
		break;
	}
	case WM_LBUTTONUP:
	{
		// Обработка отжатия курсора
		if (graphics->activeMousePress)
		{
			graphics->activeMousePress->ExecuteEvent(MouseEventType::RELEASE, wParam, lParam);
			graphics->ResetActiveMousePress();
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		// Обработка прокрутки колёсика мыши
		if (graphics->mouseScroll)
			graphics->mouseScroll->ExecuteEvent(MouseEventType::SCROLL, wParam, lParam);
		break;
	}
	case WM_PAINT:
	{
		// Вызов отрисовки
		graphics->PaintProcessing();
		break;
	}
	case WM_GETMINMAXINFO:
	{
		// Установка минимальных размеров окна
		reinterpret_cast<LPMINMAXINFO>(lParam)->ptMinTrackSize =
		{
			static_cast<LONG>(graphics->minWindowSizePhysical.width),
			static_cast<LONG>(graphics->minWindowSizePhysical.height)
		};
		break;
	}
	case WM_DESTROY:
	{
		// Закрытие настроек
		delete graphics;
		break;
	}
	case GM_DESTROY_BLOCK:
	{
		// Вызов удаления блока
		graphics->DestroyBlockProcessing(reinterpret_cast<Block>(wParam));
		break;
	}
	case GM_UPDATE:
	{
		// Вызов обновления
		graphics->UpdateProcessing();
		break;
	}
	case GM_CREATE_ANIMATION:
	{
		// Вызов создания анимации
		graphics->CreateAnimationProcessing(reinterpret_cast<Animation>(wParam));
		break;
	}
	case GM_ANIMATION:
	{
		// Вызов обработки анимации
		graphics->AnimationProcessing();
		break;
	}
	default: break;
	}
	
	// Обработка по умолчанию
	return DefWindowProcW(window, message, wParam, lParam);
}
void Graphics::TimerCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2) { graphics->PostGraphicMessage(GM_ANIMATION); }

// Приватные методы класса
void Graphics::AdjustDPISize(D2Size* size)
{
	size->width = static_cast<UINT>(size->width * dpiRatio);
	size->height = static_cast<UINT>(size->height * dpiRatio);
}
void Graphics::AdjustWindowSize(D2Size* size)
{
	// Получение размера окна с учётом рамки и заголовка
	RECT rect{ 0, 0, static_cast<long>(size->width), static_cast<long>(size->height) };
	
	// Корректировка размера окна
	AdjustWindowRectExForDpi(&rect, WS_CAPTION, 0, 0, static_cast<UINT>(dpiRatio * USER_DEFAULT_SCREEN_DPI));
	size->width = static_cast<UINT>(rect.right - rect.left);
	size->height = static_cast<UINT>(rect.bottom - rect.top);
}
void Graphics::UpdateTargetBitmap(UINT width, UINT height)
{
	// Сброс места отрисовки
	if (graphics->targetBitmap)
	{
		graphics->deviceContext->SetTarget(nullptr);
		graphics->targetBitmap->Release();
	}

	// Изменение размера цепочки обмена
	graphics->swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_B8G8R8A8_UNORM, 0);

	// Получение поверхности
	DXSurface surface = nullptr;
	if (FAILED(swapChain->GetBuffer(0, IID_PPV_ARGS(&surface))))
		throw std::runtime_error("GetBuffer ERROR");

	// Создание места для отрисовки
	float dpi = dpiRatio * USER_DEFAULT_SCREEN_DPI;
	deviceContext->CreateBitmapFromDxgiSurface
	(
		surface,	
		BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW, PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED), dpi, dpi),
		&targetBitmap
	);
	surface->Release();

	// Установка места отрисовки
	deviceContext->SetTarget(targetBitmap);
	deviceContext->SetDpi(dpi, dpi);
}
void Graphics::PostGraphicMessage(GraphicMessages msg, WPARAM wparam, LPARAM lparam) { PostMessageW(window, msg, wparam, lparam); }
void Graphics::TryPostUpdateMessage()
{
	if (!needRebuild && !needBlockUpdate && !needHoverUpdate)
		PostGraphicMessage(GM_UPDATE);
}
void Graphics::ResetActiveMousePress()
{
	// Сброс блока нажатия
	activeMousePress = nullptr;

	// Сброс захвата мыши
	ReleaseCapture();
}
bool Graphics::AddAnimation(Animation animation)
{
	// Замена существующий анимации
	for (AnimationVector::iterator it = animateVector.begin(); it != animateVector.end(); ++it)
		if ((*it)->GetReference() == animation->GetReference())
		{
			// Очистка динамической памяти
			delete *it;
			animateVector.erase(it);
			break;
		}

	// Игнорирование мгновенной анимации
	if (!animation->GetProperties()->GetDuration())
	{
		animation->SetStart();
		animation->Calculate(ANIMATION_PROGRESS_END);
		delete animation;
		return false;
	}

	// Добавление анимации в вектор
	animateVector.push_back(animation);
	return true;
}
void Graphics::DeleteAllAnimations(Block block)
{
	// Удаление отложенных анимаций
	for (AnimationVector::iterator it = delayedVector.begin(); it != delayedVector.end();)
	{
		if ((*it)->GetSponsor() == block)
		{
			// Очистка динамической памяти
			delete* it;
			it = delayedVector.erase(it);
			continue;
		}
		++it;
	}

	// Удаление запущенных анимаций
	for (AnimationVector::iterator it = animateVector.begin(); it != animateVector.end();)
	{
		if ((*it)->GetSponsor() == block)
		{
			// Очистка динамической памяти
			delete* it;
			it = animateVector.erase(it);
			continue;
		}
		++it;
	}
}
void Graphics::ResetMouseBlocks(Block block)
{
	// Сброс блока наведения
	if (mouseHover == block) mouseHover = nullptr;
	if (mousePress == block) mousePress = nullptr;
	if (activeMousePress == block) ResetActiveMousePress();
	if (mouseScroll == block) mouseScroll = nullptr;
}
void Graphics::DeleteMention(Block block)
{
	// Удаление всех анимаций где присутствует блок-спонсор
	DeleteAllAnimations(block);

	// Сброс указателей мыши
	ResetMouseBlocks(block);
}
void Graphics::DeleteBlockTree(Block block)
{
	// Удаление записи у родителя
	if (block->parent && block->parent->children)
	{
		// Поиск блока в векторе
		BlockVector::iterator it = std::find(block->parent->children->begin(), block->parent->children->end(), block);
		if (it == block->parent->children->end())
			throw std::runtime_error("The child does not exist");

		// Удаление блока из вектора
		block->parent->children->erase(it);
	}

	// Обход дерева без рекурсии
	BlockVector deleteOrder;
	BlockStack blockStack;
	blockStack.push(block);
	while (!blockStack.empty())
	{
		// Получение блока
		Block node = blockStack.top();
		blockStack.pop();

		// Сохранение блока для удаления
		DeleteMention(node);
		deleteOrder.push_back(node);

		// Добавление детей в стек
		if (node->children)
			for (Block child : *node->children)
				blockStack.push(child);
	}

	// Сброс корня перед удалением
	if (block == root)
		root = nullptr;

	// Удаление блоков от детей к родителю
	for (BlockVector::reverse_iterator it = deleteOrder.rbegin(); it != deleteOrder.rend(); ++it)
		delete *it;
}
void Graphics::SetMinWindowSizePhysical(D2Size size)
{
	// Коррекция по DPI
	graphics->AdjustDPISize(&size);

	// Коррекция по неклиентской области окна
	graphics->AdjustWindowSize(&size);

	// Установка минимального размера окна
	minWindowSizePhysical = size;
}
