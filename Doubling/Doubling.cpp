#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <algorithm>

using namespace std;

DWORD WINAPI ThreadSum(LPVOID lpParam);
void Print(int *mass, int size);
int* GetMass(int size);

struct Data
{
	int *left;
	int *right;
};

int _tmain()
{
	srand(GetTickCount());

	int n;
	cin >> n;

	int* mass = GetMass(n);
	Print(mass, n);

	HANDLE *ThreadArray = new HANDLE[n / 2];
	Data *DataInput = new Data[n / 2];

	int len = n;
	while (len > 1)
	{
		int pair = len / 2;
		len -= pair;

		for (int i = 0; i < pair; i++)
		{
			DataInput[i].left = mass + i;
			DataInput[i].right = mass + i + len;
			ThreadArray[i] = CreateThread(NULL, 0, ThreadSum, DataInput + i, 0, 0);
		}

		WaitForMultipleObjects(pair, ThreadArray, TRUE, INFINITE);
		for (int i = 0; i < pair; i++)
			CloseHandle(ThreadArray[i]);
	}

	Print(mass, n);

	delete[] ThreadArray;
	delete[] DataInput;
	delete[] mass;
}

DWORD WINAPI ThreadSum(LPVOID lpParam)
{
	Data *Argument = (Data*)lpParam;
	*Argument->left += *Argument->right;
	return 0;
}

int* GetMass(int size)
{
	int *mass = new int[size];
	for (int i = 0; i < size; i++)
		mass[i] = rand() % 10;
	return mass;
}

void Print(int* mass, int size)
{
	for (int i = 0; i < size; i++)
		cout << mass[i] << ' ';
	cout << endl;
}