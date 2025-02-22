#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <queue>

using namespace std;

DWORD WINAPI BarberThread(LPVOID);
DWORD WINAPI ClientThread(LPVOID);

HANDLE MWaitingTable;
HANDLE MWorkingTable;

HANDLE SFreeСhair;
HANDLE SClientReady;
HANDLE SClientLeave;
HANDLE *SHaircutIsOver;

queue<int> WaitingRoomTable;
queue<int> WorkingRoomTable;

struct Data
{
	int i;
	int count;
};

int _tmain()
{
	int n, p, m, w;
	cin >> n >> p >> m >> w;
	int MaxPlaces = m + w;

	for (int i = 0; i < MaxPlaces; i++)
		WaitingRoomTable.push(i);

	HANDLE *hBarbers = new HANDLE[m];
	HANDLE *hClients = new HANDLE[n];
	Data *DataArray = new Data[n];
	SHaircutIsOver = new HANDLE[MaxPlaces];

	MWaitingTable = CreateMutex(NULL, FALSE, NULL);
	MWorkingTable = CreateMutex(NULL, FALSE, NULL);

	SFreeСhair = CreateSemaphore(NULL, m, m, NULL);
	SClientReady = CreateSemaphore(NULL, 0, m, NULL);
	SClientLeave = CreateSemaphore(NULL, 0, m, NULL);
	for (int i = 0; i < MaxPlaces; i++)
		SHaircutIsOver[i] = CreateSemaphore(NULL, 0, 1, NULL);

	for (int i = 0; i < m; i++)
		hBarbers[i] = CreateThread(NULL, 0, BarberThread, (LPVOID)i, 0, 0);
	for (int i = 0; i < n; i++)
	{
		DataArray[i].i = i;
		DataArray[i].count = p;
		hClients[i] = CreateThread(NULL, 0, ClientThread, DataArray + i, 0, 0);
	}

	WaitForMultipleObjects(n, hClients, TRUE, INFINITE);

	for (int i = 0; i < m; i++)
		CloseHandle(hBarbers[i]);
	for (int i = 0; i < n; i++)
		CloseHandle(hClients[i]);
	for (int i = 0; i < MaxPlaces; i++)
		CloseHandle(SHaircutIsOver[i]);
	
	CloseHandle(MWaitingTable);
	CloseHandle(MWorkingTable);

	CloseHandle(SFreeСhair);
	CloseHandle(SClientReady);
	CloseHandle(SClientLeave);

	delete[] hBarbers;
	delete[] hClients;
	delete[] DataArray;
	delete[] SHaircutIsOver;
	return 0;
}

DWORD WINAPI BarberThread(LPVOID lpParam)
{
	int i_Barber = (int)lpParam;
	int num_Client = 0;
	srand(GetTickCount() + i_Barber);

	while (true)
	{
		WaitForSingleObject(SClientReady, INFINITE);

		WaitForSingleObject(MWorkingTable, INFINITE);
		num_Client = WorkingRoomTable.front(), WorkingRoomTable.pop();
		ReleaseMutex(MWorkingTable);
		printf("Barber %d cuting client hair with number %d...\n", i_Barber, num_Client);
		Sleep((rand() % 3 + 1) * 1000);

		ReleaseSemaphore(SHaircutIsOver[num_Client], 1, NULL);

		WaitForSingleObject(SClientLeave, INFINITE);

		WaitForSingleObject(MWaitingTable, INFINITE);
		WaitingRoomTable.push(num_Client);
		ReleaseMutex(MWaitingTable);
		printf("Barber %d is ready to accept the next client\n", i_Barber);
		ReleaseSemaphore(SFreeСhair, 1, NULL);
	}

	return 0;
}

DWORD WINAPI ClientThread(LPVOID lpParam)
{
	Data* Argument = (Data*)lpParam;
	int i_Client = Argument->i;
	int count = Argument->count;
	int number = 0;
	srand(GetTickCount() + i_Client);

	int step = 0;
	while (step < count)
	{
		WaitForSingleObject(MWaitingTable, INFINITE);
		if (WaitingRoomTable.empty())
		{
			ReleaseMutex(MWaitingTable);
			printf("There are no free seats. Client %d leaving the barbershop...\n", i_Client);
			Sleep((rand() % 5 + 1) * 1000);
			continue;
		}
		number = WaitingRoomTable.front(), WaitingRoomTable.pop();
		ReleaseMutex(MWaitingTable);
		printf("Client %d went to the barbershop and took a number %d\n", i_Client, number);

		WaitForSingleObject(SFreeСhair, INFINITE);

		WaitForSingleObject(MWorkingTable, INFINITE);
		WorkingRoomTable.push(number);
		ReleaseMutex(MWorkingTable);
		printf("Client %d siting in the barber's chair\n", i_Client);
		ReleaseSemaphore(SClientReady, 1, NULL);

		WaitForSingleObject(SHaircutIsOver[number], INFINITE);

		ReleaseSemaphore(SClientLeave, 1, NULL);
		printf("Client %d gets a haircut and leaving the barbershop...\n", i_Client);
		Sleep((rand() % 5 + 1) * 1000);

		step++;
	}

	return 0;
}