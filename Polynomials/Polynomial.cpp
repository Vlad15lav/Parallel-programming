#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <ctime>

using namespace std;

DWORD WINAPI ThreadForMultiplication(LPVOID lpParam);
void FillPolynom(int *pol, int deg);
void Print(int *pol, int deg);

int *A;
int *B;
int *C;

int N, M;

#define MAX_THREADS N + M + 1

int _tmain()
{
	srand(time(NULL));
	cout << "Degree 1 Polynomial - ";
	cin >> N;
	cout << "Degree 2 Polynomial - ";
	cin >> M;

	HANDLE *ThreadArray = new HANDLE[MAX_THREADS];

	A = new int[N + 1];
	B = new int[M + 1];
	C = new int[MAX_THREADS];

	FillPolynom(A, N);
	FillPolynom(B, M);

	Print(A, N);
	Print(B, M);

	for (int i = 0; i < MAX_THREADS; i++)
		ThreadArray[i] = CreateThread(NULL, 0, ThreadForMultiplication, (LPVOID)i, 0, 0);

	WaitForMultipleObjects(MAX_THREADS, ThreadArray, TRUE, INFINITE);

	for (int i = 0; i < MAX_THREADS; i++)
		CloseHandle(ThreadArray[i]);

	Print(C, N + M);

	delete[] A;
	delete[] B;
	delete[] C;
	delete[] ThreadArray;
	return 0;
}


DWORD WINAPI ThreadForMultiplication(LPVOID lpParam)
{
	int k = (int)lpParam;
	C[k] = 0;
	for (int i = max(0, k - N), end = min(M, k); i <= end; i++)
		C[k] += A[k - i] * B[i];
	return 0;
}

void FillPolynom(int *pol, int deg)
{
	for (int i = 0; i <= deg; i++)
		pol[i] = rand() % 10 + 1;
}

void Print(int *pol, int deg)
{
	for (int i = 0; i < deg; i++)
		cout << pol[i] << "X^" << deg - i << " + ";
	cout << pol[deg] << endl;
}