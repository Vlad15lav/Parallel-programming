#include "pch.h"
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <algorithm>
#include <iomanip>

// Блочное умножение матрицы на вектор

using namespace std;

HANDLE *MutexArray;
int **Matrix;
int *Vector;
int *Result;

DWORD WINAPI ThreadMultiplication(LPVOID);
int** GetMatrix(int, int);
int* GetVector(int);
void Print(int *, int);
void Print(int **, int, int);
void DeleteMatrix(int **, int);

#define MAX_THREADS q * q
#define BUF_SIZE 255

struct Data
{
	int i;
	int j;
	int row;
	int col;
};

int _tmain()
{
	srand(GetTickCount());

	int m, n, q;
	cin >> m >> n >> q;

	Matrix = GetMatrix(m, n);
	cout << "Matrix:" << endl;
	Print(Matrix, m, n);

	Vector = GetVector(n);
	cout << "Vector:" << endl;
	Print(Vector, n);

	Result = new int[m] {0};

	MutexArray = new HANDLE[q];
	for (int i = 0; i < q; i++)
		MutexArray[i] = CreateMutex(NULL, FALSE, NULL);

	Data *DataArray = new Data[MAX_THREADS];
	HANDLE *ThreadArray = new HANDLE[MAX_THREADS];

	for (int i = 0, k = 0; i < m; i += m / q)
		for (int j = 0; j < n; j += n / q, k++)
		{
			DataArray[k].i = i;
			DataArray[k].j = j;
			DataArray[k].row = m / q;
			DataArray[k].col = n / q;
			ThreadArray[k] = CreateThread(NULL, 0, ThreadMultiplication, DataArray + k, 0, 0);
		}

	WaitForMultipleObjects(MAX_THREADS, ThreadArray, TRUE, INFINITE);
	for (int i = 0; i < MAX_THREADS; i++)
		CloseHandle(ThreadArray[i]);
	for (int i = 0; i < q; i++)
		CloseHandle(MutexArray[i]);

	cout << "Result:" << endl;
	Print(Result, m);

	delete[] DataArray;
	delete[] ThreadArray;
	delete[] MutexArray;
	delete[] Vector;
	delete[] Result;
	DeleteMatrix(Matrix, m);
	return 0;
}

DWORD WINAPI ThreadMultiplication(LPVOID lpParam)
{
	TCHAR msgBuf[BUF_SIZE];
	size_t curBufSize;
	DWORD dwChars;

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE)
		return 1;

	Data *Argument = (Data*)lpParam;
	int index_i = Argument->i, index_j = Argument->j, row = Argument->row, col = Argument->col;
	int* temp_vector = new int[row] {0};

	for (int i = index_i; i < index_i + row; i++)
		for (int j = index_j; j < index_j + col; j++)
			temp_vector[i - index_i] += Matrix[i][j] * Vector[j];

	StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Block processed: (%d, %d) ("), index_i / row, index_j / col);
	StringCchLength(msgBuf, BUF_SIZE, &curBufSize);
	for (int i = 0; i < row; i++)
	{
		StringCchPrintf(msgBuf + curBufSize, BUF_SIZE - curBufSize, TEXT("%d, "), temp_vector[i]);
		StringCchLength(msgBuf, BUF_SIZE, &curBufSize);
	}
	StringCchPrintf(msgBuf + curBufSize - 2, BUF_SIZE - curBufSize, TEXT(")\n"));
	StringCchLength(msgBuf, BUF_SIZE, &curBufSize);
	
	WriteConsole(hStdout, msgBuf, (DWORD)curBufSize, &dwChars, NULL);

	int i_mutex = index_i / row;
	WaitForSingleObject(MutexArray[i_mutex], INFINITE);

	for (int i = index_i; i < index_i + row; i++)
		Result[i] += temp_vector[i - index_i];

	ReleaseMutex(MutexArray[i_mutex]);

	delete[] temp_vector;
	return 0;
}

int** GetMatrix(int n, int m)
{
	int** matrix = new int*[n];
	for (int i = 0; i < n; i++)
		matrix[i] = new int[m];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			matrix[i][j] = rand() % 10;
	return matrix;
}

int* GetVector(int n)
{
	int* vector = new int[n];
	for (int i = 0; i < n; i++)
		vector[i] = rand() % 10;
	return vector;
}

void Print(int *mass, int size)
{
	for (int i = 0; i < size; i++)
		cout << setw(4) << mass[i];
	cout << endl;
}

void Print(int **mass, int n, int m)
{
	for (int i = 0; i < n; i++, cout << endl)
		for (int j = 0; j < m; j++)
			cout << setw(4) << mass[i][j];
}

void DeleteMatrix(int** matrix, int n)
{
	for (int i = 0; i < n; i++)
		delete[] matrix[i];
	delete[] matrix;
}