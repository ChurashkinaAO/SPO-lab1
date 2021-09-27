//spo lab1

#include <iostream>
#include <stdio.h>
#define WINVER 0x0A00
#include <windows.h>

using namespace std;

void Punct1()
{
	OSVERSIONINFO osvi;
	BOOL bIsWindowsXPorLater;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);
}



int main()
{
	//punct 1.1. Версия ОС
	OSVERSIONINFO osvi;
	BOOL bIsWindowsXPorLater;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);

	cout << "Version of OS: " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion << endl;
	cout << "Build: " << osvi.dwBuildNumber << endl;

	//punct 1.2
	int INFO_BUFFER_SIZE = 256;
	TCHAR infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	GetSystemDirectoryA(infoBuf, INFO_BUFFER_SIZE);
	printf("\nResult  GetSystemDirectory: %s\n", infoBuf);

	//punct 1.3
	char lpBuffer[INFO_BUFFER_SIZE];
	DWORD pcbBuffer = INFO_BUFFER_SIZE;
	GetUserNameA(lpBuffer, &pcbBuffer);
	printf("\nName of user : %s", lpBuffer);
	GetComputerNameA(lpBuffer, &pcbBuffer);
	printf("      Name of computer : %s", lpBuffer);

	//punct 1.4
	char buffer[MAX_PATH * 4];
	char PathName[MAX_PATH];
	DWORD lpcchReturnLength = MAX_PATH * 4;
	_ULARGE_INTEGER total, available, free;

	HANDLE search = FindFirstVolumeA(buffer, sizeof(buffer));
	printf("\n1.4");

	do {
		FindNextVolumeA(search, buffer, sizeof(buffer));
		GetVolumePathNamesForVolumeNameA(buffer, PathName, sizeof(buffer), &lpcchReturnLength);
		GetDiskFreeSpaceExA(buffer, &total, &available, &free);
		printf("\nService Name: %s", buffer);
		printf("\nPath Name: %s", PathName);
		printf("\nDisk Available space: %llu\nVolume disk space: %llu\n", available.QuadPart, total.QuadPart);

	} while (FindNextVolume(search, buffer, sizeof(buffer)));

	if (GetLastError() != ERROR_NO_MORE_FILES) {
		printf("\nA system error has occurred");
	}
	FindVolumeClose(search);

	//p1.5
	HKEY hKey;
	LPCSTR lpSubKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	DWORD  ulOptions;
	REGSAM samDesired;
	PHKEY  phkResult;
	DWORD count = 0;

	char Progbuffer[MAX_PATH * 4];
	DWORD Pblen = 32767;

	RegOpenKeyExA(HKEY_CURRENT_USER, lpSubKey, 0, KEY_ALL_ACCESS, &hKey);

	while (RegEnumValueA(hKey, count, Progbuffer, &Pblen, 0, NULL, NULL, NULL) == ERROR_SUCCESS)
	{
		RegEnumValueA(hKey, count, Progbuffer, &Pblen, 0, NULL, NULL, NULL);
		printf("\n%s", Progbuffer);
		count++;
	}



	//punct 2.1
	LARGE_INTEGER Freq;
	QueryPerformanceFrequency(&Freq);
	cout << "\nCPU frequency : " << Freq.QuadPart << " HZ";

	//punct 2.2
	LARGE_INTEGER t0, t;
	QueryPerformanceCounter(&t0);
	Punct1();
	QueryPerformanceCounter(&t);
	double ticks = t.QuadPart - t0.QuadPart;
	double ticks_per_sec = Freq.QuadPart;
	double usec_per_sec = 1e6;
	double usec = usec_per_sec * ticks / ticks_per_sec;
	printf("\nDuration point 1.1 complete=%f.3 usec\n", usec);

	return 0;
}
