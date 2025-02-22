#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <algorithm>

using namespace std;

DWORD WINAPI SortThread(LPVOID);
DWORD WINAPI MergingComparatorThread(LPVOID);
int* GetMass(int);
void Print(int*, int);

struct Data
{
	int *x;
	int *y;
	int LenBlockX;
	int LenBlockY;
};

int _tmain()
{
	srand(GetTickCount());

	int n, p;
	cin >> n >> p;

	int *mass = GetMass(n);
	Print(mass, n);

	int BlockCount = 2 * p;
	int q;
	int t;
	if (n % BlockCount != 0)
	{
		q = n / BlockCount + 1;
		if (n % q == 0)
			BlockCount = n / q, t = q;
		else
			BlockCount = n / q + 1, t = n % q;
	}
	else
		q = n / BlockCount, t = q;
	int LenBlock = q;

	cout << "Blocks - " << BlockCount << "\nLength Block - " << LenBlock << "\nLength Last Block - " << t << endl;

	HANDLE *ThreadArray = new HANDLE[BlockCount / 2];
	Data *DataArray = new Data[BlockCount / 2];

	for (int i = 0, index = 0; i < BlockCount - 1; i += 2, index++)
	{
		DataArray[index].x = mass + i * LenBlock;
		DataArray[index].LenBlockX = LenBlock;
		DataArray[index].y = mass + (i + 1) * LenBlock;
		DataArray[index].LenBlockY = (i + 1) * LenBlock + LenBlock < n ? LenBlock : t;
		ThreadArray[index] = CreateThread(NULL, 0, SortThread, DataArray + index, 0, 0);
	}

	if (BlockCount % 2 != 0) sort(mass + n - t, mass + n);

	WaitForMultipleObjects(BlockCount / 2, ThreadArray, TRUE, INFINITE);
	for (int i = 0; i < BlockCount / 2; i++)
		CloseHandle(ThreadArray[i]);

	Print(mass, n);
	
	int stage = 1;
	while (stage < BlockCount)
	{
		int merger = stage * 2;

		for (int len_comp = stage; len_comp > 0; len_comp /= 2)
		{
			int pair = 0;

			for (int i = len_comp % stage; i + len_comp < BlockCount; i += len_comp * 2)
				if (i / merger == (i + len_comp) / merger)
					for (int line = 0; line < len_comp; line++)
						if (i + line + len_comp < BlockCount)
						{
							DataArray[pair].x = mass + (i + line) * LenBlock;
							DataArray[pair].y = mass + (i + line + len_comp) * LenBlock;
							DataArray[pair].LenBlockX = LenBlock;
							DataArray[pair].LenBlockY = (i + line + len_comp) * LenBlock + LenBlock < n ? LenBlock : t;
							ThreadArray[pair] = CreateThread(NULL, 0, MergingComparatorThread, DataArray + pair, 0, 0);

							pair++;
						}

			WaitForMultipleObjects(pair, ThreadArray, TRUE, INFINITE);
			for (int i = 0; i < pair; i++)
				CloseHandle(ThreadArray[i]);
		}

		stage = merger;
	}
	
	Print(mass, n);
	
	delete[] mass;
	delete[] ThreadArray;
	delete[] DataArray;
	
	return 0;
}

DWORD WINAPI SortThread(LPVOID lpParam)
{
	Data* Argument = (Data*)lpParam;
	int *x = Argument->x;
	int *y = Argument->y;
	int LenBlockX = Argument->LenBlockX;
	int LenBlockY = Argument->LenBlockY;

	sort(x, x + LenBlockX);
	sort(y, y + LenBlockY);

	return 0;
}

DWORD WINAPI MergingComparatorThread(LPVOID lpParam)
{
	Data* Argument = (Data*)lpParam;
	int *x = Argument->x;
	int *y = Argument->y;
	int LenBlockX = Argument->LenBlockX;
	int LenBlockY = Argument->LenBlockY;

	int *buffer = new int[LenBlockX + LenBlockY];

	int i = 0, j = 0, k = 0;
	while (i < LenBlockX && j < LenBlockY)
		if (x[i] < y[j])
			buffer[k++] = x[i++];
		else
			buffer[k++] = y[j++];
	while (i < LenBlockX) buffer[k++] = x[i++];
	while (j < LenBlockY) buffer[k++] = y[j++];

	for (int index = 0; index < LenBlockX; index++)
		x[index] = buffer[index];
	for (int index = 0; index < LenBlockY; index++)
		y[index] = buffer[index + LenBlockX];

	delete[] buffer;
	return 0;
}

int* GetMass(int size)
{
	int* a = new int[size];
	for (int i = 0; i < size; i++)
		a[i] = rand() % 10;
	return a;
}

void Print(int* a, int size)
{
	for (int i = 0; i < size; i++)
		cout << a[i] << ' ';
	cout << endl;
}