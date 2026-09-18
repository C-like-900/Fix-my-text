#pragma once
#include "Graphics.h"
#include <Windows.h>
#include "D2Defines.h"
#include "BlockClass.h"
#include "RootClass.h"
#include "AnimationClass.hpp"
#include "ColorRGBA.hpp"

// Определение класса
class Graphics
{
public:
	// Общая переменая
	static Graphics* graphics;

	// Конструктор и деструктор
	Graphics(HINSTANCE instance, D2Size windowSize, D2Size minWindowSize, const ColorRGBA& bgColor);
	~Graphics();

	// Цикл обработки сообщений TODO (временно)
	void MessageLoop();
	
	// Методы отправки запросов
	void DestroyBlockRequest(Block block);
	void RebuildRequest();
	void UpdateRequest(Block block = nullptr);
	void HoverUpdateRequest();
	void CreateAnimationRequest(Animation animation);

	// Методы класса
	float GetDpiRatio() const;
	POINT GetCursorPoint() const;
	HWND GetWindow() const;
	DWFactory GetWriteFactory() const;
	DIFactory GetImageFactory() const;
	D2DeviceContext GetDeviceContext() const;
	D2Brush GetBrush() const;
	D2PenStyle GetPenStyle() const;
	Root GetRoot() const;
	void SetCursorImage(LPCWSTR name = IDC_ARROW);

private:
	// Поля класса
	HINSTANCE		instance;
	HWND			window;
	D2Size			minWindowSizeLogical;
	D2Size			minWindowSizePhysical;
	float			dpiRatio;
	D2Factory		factory;
	DWFactory		writeFactory;
	DIFactory		imageFactory;
	D2DeviceContext	deviceContext;
	DXSwapChain		swapChain;
	D2TargetBitmap	targetBitmap;
	D2Brush			brush;
	D2PenStyle		penStyle;
	Root			root;
	BlockVector		blockVector;
	Block			mouseHover;
	Block			mousePress;
	Block			activeMousePress;
	Block			mouseScroll;
	AnimationVector animateVector;
	AnimationVector delayedVector;
	UINT			displayFrequencyMs;
	float			timerFrequencyMs;
	MMRESULT		timer;
	bool			needRebuild;
	bool			needBlockUpdate;
	bool			needHoverUpdate;

	// Методы обработки запросов
	void CreateWindowProcessing(HWND window);
	void PaintProcessing();
	void DestroyBlockProcessing(Block block = nullptr);
	void UpdateProcessing();
	void CreateAnimationProcessing(Animation animation);
	void AnimationProcessing();

	// Функции обратного вызова
	static LRESULT CALLBACK WindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
	static void CALLBACK TimerCallback(UINT timerID, UINT message, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

	// Приватные методы класса
	void AdjustDPISize(D2Size* size);
	void AdjustWindowSize(D2Size* size);
	void UpdateTargetBitmap(UINT width = 0, UINT height = 0);
	void PostGraphicMessage(GraphicMessages msg, WPARAM wparam = 0, LPARAM lparam = 0);
	void TryPostUpdateMessage();
	void ResetActiveMousePress();
	bool AddAnimation(Animation animation);
	void DeleteAllAnimations(Block block);
	void ResetMouseBlocks(Block block);
	void DeleteMention(Block block);
	void DeleteBlockTree(Block block);
	void SetMinWindowSizePhysical(D2Size size);
};
