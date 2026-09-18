#include <Windows.h>
#include <locale>
#include "Mutex.hpp"
#include "Application.hpp"

// Code for "Fix my text" by Teslev Dmitry Sergeevich 2023 - 2026
int WINAPI wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE instancePrevious, _In_ wchar_t* commandLine, _In_ int showCommand)
{
	// Перехват исключений
	try
	{
		// Создание мьютекса
		Mutex mutex;

		// Установка русской локали для всей программы
		std::locale::global(std::locale(""));

		// Заупск приложения
		Application application(instance);
		application.Run();
	}
	catch (const std::exception& exception)
	{
		MessageBoxA(nullptr, exception.what(), "Fix My Text ERROR", MB_ICONERROR);
		return 1;
	}
	return 0;
}