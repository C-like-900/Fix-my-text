#include "Mutex.hpp"
#include <stdexcept>

// Конструктор и деструктор
Mutex::Mutex() : mutexHandle(nullptr)
{
	// Создание мьютекста
	mutexHandle = CreateMutexW(nullptr, TRUE, MUTEX_NAME);

	// Проверка на уже запущенный экземпляр
	if (!mutexHandle || GetLastError() == ERROR_ALREADY_EXISTS)
		throw std::runtime_error("The program is already running");
}
Mutex::~Mutex()
{
	// Удаление мьютекста
	if (mutexHandle)
	{
		ReleaseMutex(mutexHandle);
		CloseHandle(mutexHandle);
	}
}