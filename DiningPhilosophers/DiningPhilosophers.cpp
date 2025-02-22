#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

using namespace std;

DWORD WINAPI PhilosopherThread(LPVOID);
HANDLE hSemaphore;
HANDLE *MutexArray;

struct Data
{
	int i;
	int phiCount;
	int rounds;
};

int _tmain()
{
	int n, p;
	cin >> n >> p;

	HANDLE *ThreadArray = new HANDLE[n];
	Data *DataArray = new Data[n];

	MutexArray = new HANDLE[n];

	hSemaphore = CreateSemaphore(NULL, n - 1, n - 1, NULL);

	for (int i = 0; i < n; i++)
		MutexArray[i] = CreateMutex(NULL, FALSE, NULL);

	for (int i = 0; i < n; i++)
	{
		DataArray[i].i = i;
		DataArray[i].phiCount = n;
		DataArray[i].rounds = p;
		ThreadArray[i] = CreateThread(NULL, 0, PhilosopherThread, DataArray + i, 0, 0);
	}

	WaitForMultipleObjects(n, ThreadArray, TRUE, INFINITE);

	for (int i = 0; i < n; i++)
	{
		CloseHandle(ThreadArray[i]);
		CloseHandle(MutexArray[i]);
	}
	CloseHandle(hSemaphore);

	delete[] DataArray;
	delete[] ThreadArray;
	delete[] MutexArray;
	return 0;
}

DWORD WINAPI PhilosopherThread(LPVOID lpParam)
{
	Data* Argument = (Data*)lpParam;
	int i_Philosopher = Argument->i;
	int phiСount = Argument->phiCount;
	int rounds = Argument->rounds;
	srand(GetTickCount() + i_Philosopher);

	int left_stick = i_Philosopher;
	int right_stick = (i_Philosopher + 1) % phiСount;

	for (int i = 0; i < rounds; i++)
	{
		printf("Philosopher %d is thinking...\n", i_Philosopher);
		Sleep((rand() % 5 + 1) * 1000);

		WaitForSingleObject(hSemaphore, INFINITE);

		WaitForSingleObject(MutexArray[left_stick], INFINITE);
		printf("Philosopher %d took left stick\n", i_Philosopher);

		WaitForSingleObject(MutexArray[right_stick], INFINITE);
		printf("Philosopher %d took right stick\n", i_Philosopher);

		printf("Philosopher %d is dining...\n", i_Philosopher);
		Sleep((rand() % 3 + 1) * 1000);

		ReleaseMutex(MutexArray[left_stick]);

		ReleaseMutex(MutexArray[right_stick]);

		ReleaseSemaphore(hSemaphore, 1, NULL);
	}

	return 0;
}