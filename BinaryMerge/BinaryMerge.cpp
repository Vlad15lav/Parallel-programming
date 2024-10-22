#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <algorithm>

using namespace std;

DWORD WINAPI ThreadMerge(LPVOID lpParam);
void Print(int *mass, int size);
int BinarySearch(int *a, int n, int key);

#define MAX_THREADS n + m

struct Data
{
	int *mass;
	int size;
	int key;
	int i;
	int *result;
};

int _tmain()
{
	int n, m;
	cin >> n >> m;

	int *T = new int[MAX_THREADS];
	for (int i = 0; i < MAX_THREADS; i++)
		T[i] = i + 1;
	srand(GetTickCount());
	random_shuffle(T, T + MAX_THREADS);

	int *A;
	int *B;
	A = T;
	B = T + n;

	int *C;
	C = new int[MAX_THREADS];

	sort(A, A + n);
	sort(B, B + m);
	cout << "A - ";
	Print(A, n);
	cout << "B - ";
	Print(B, m);
	
	Data *DataInput = new Data[MAX_THREADS];
	HANDLE *ThreadArray = new HANDLE[MAX_THREADS];
	for (int i = 0; i < n; i++)
	{
		DataInput[i].i = i;
		DataInput[i].mass = B;
		DataInput[i].size = m;
		DataInput[i].key = A[i];
		DataInput[i].result = C;
		ThreadArray[i] = CreateThread(NULL, 0, ThreadMerge, &DataInput[i], 0, 0);
	}
	for (int i = 0; i < m; i++)
	{
		DataInput[n + i].i = i;
		DataInput[n + i].mass = A;
		DataInput[n + i].size = n;
		DataInput[n + i].key = B[i];
		DataInput[i].result = C;
		ThreadArray[n + i] = CreateThread(NULL, 0, ThreadMerge, &DataInput[n + i], 0, 0);
	}

	WaitForMultipleObjects(MAX_THREADS, ThreadArray, TRUE, INFINITE);
	for (int i = 0; i < MAX_THREADS; i++)
		CloseHandle(ThreadArray[i]);

	cout << "C - ";
	Print(C, MAX_THREADS);

	delete[] T;
	delete[] C;
	delete[] ThreadArray;
	delete[] DataInput;
	return 0;
}

DWORD WINAPI ThreadMerge(LPVOID lpParam)
{
	Data *Argument = (Data*)lpParam;
	int k = BinarySearch(Argument->mass, Argument->size, Argument->key);
	Argument->result[Argument->i + k] = Argument->key;
	return 0;
}

int BinarySearch(int *mass, int size, int key)
{
	int left = 0;
	int right = size;
	while (left != right)
	{
		int k = (left + right) / 2;
		if (key < mass[k])
			right = k;
		else
			left = k + 1;
	}
	return right;
}

void Print(int *mass, int size)
{
	for (int i = 0; i < size; i++)
		cout << mass[i] << ' ';
	cout << endl;
}