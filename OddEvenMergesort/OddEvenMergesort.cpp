#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <algorithm>

using namespace std;

DWORD WINAPI ComparatorThread(LPVOID);
int* GetMass(int);
void Print(int*, int);

struct Data
{
	int *x;
	int *y;
};

int _tmain()
{
	srand(GetTickCount());

	int n;
	cin >> n;

	int *mass = GetMass(n);
	Print(mass, n);

	HANDLE *ThreadArray = new HANDLE[n / 2];
	Data *DataArray = new Data[n / 2];

	int stage = 1;
	while (stage < n)
	{
		int merger = stage * 2;
		
		for (int len_comp = stage; len_comp > 0; len_comp /= 2) 
		{
			int pair = 0;
			for (int i = len_comp % stage; i + len_comp < n; i += len_comp * 2) // Pass through Comparators on one straight line
				if (i / merger == (i + len_comp) / merger) // Checking whether the input wires are in the same network MERGE[ ]
					for (int line = 0; line < len_comp; line++) // Straight lines without intersecting Comparators at a parallel step (depth)
						if (i + line + len_comp < n) // Checking redundant devices
						{
							DataArray[pair].x = mass + i + line;
							DataArray[pair].y = mass + i + line + len_comp;
							ThreadArray[pair] = CreateThread(NULL, 0, ComparatorThread, DataArray + pair, 0, 0);

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

DWORD WINAPI ComparatorThread(LPVOID lpParam)
{
	Data* Argument = (Data*)lpParam;
	int *x = Argument->x;
	int *y = Argument->y;

	if (*x > *y)
		swap(*x, *y);
	
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