//spo lab1

#include <iostream>
#include <stdio.h>
#define WINVER 0x0A00
#include <windows.h>

#define BUFSIZE MAX_PATH
#define FILESYSNAMEBUFSIZE MAX_PATH
#define MAX_KEY_LENGTH 255

using namespace std;

#define INFO_BUFFER_SIZE 32767

//p.2 Измерение производистельности ЦП, замер рабочей частоты f ЦП
double PCFreq = 0.0;
__int64 CounterStart = 0;


void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		cout << "Function QueryPerformanceFrequency() failed!\n";

	PCFreq = double(li.QuadPart);

	printf("\nCPU frequency: %u  Hz\n", li);
	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}

//подсчет количества таков ЦП, 
//которое занимает выполнение программо1 пункта 1)


double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double((1000000 * (li.QuadPart - CounterStart)) / PCFreq);
}

int main()
{
	//p1.1. Версия ОС
	DWORD dwVersion = 0;
	DWORD dwMajorVersion = 0;
	DWORD dwMinorVersion = 0;
	DWORD dwBuild = 0;

	dwVersion = GetVersion();

	dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

	// номер сборки

	if (dwVersion < 0x80000000)
		dwBuild = (DWORD)(HIWORD(dwVersion));

	printf("Version of OS is %d.%d (%d)\n",
		dwMajorVersion,
		dwMinorVersion,
		dwBuild);

	//p1.2
	TCHAR szPath[_MAX_PATH] = { 0 };
	UINT iRet = GetSystemDirectory(szPath, _MAX_PATH);
	printf("Result  GetSystemDirectory %d\n", iRet);

	//p1.3
	char buffer[256];
	DWORD size = 256;

	GetComputerNameA(buffer, &size);
	printf("Name of computer : %s\n", buffer);

	GetUserNameA(buffer, &size);
	printf("Name of user : %s\n", buffer);

	//p1.4
	char buffer2[MAX_PATH + 1];
	DWORD size2 = MAX_PATH;
	char buffer3[MAX_PATH + 1];
	DWORD  CBufLen = MAX_PATH;
	__int64 total, available, free;

	HANDLE firstVolume = FindFirstVolumeA(buffer2, size2);
	printf("\n      %s", buffer2);

	do {
		printf("\n%s", buffer2);
		CBufLen = MAX_PATH;
		GetVolumePathNamesForVolumeNameA(buffer2, buffer3, CBufLen, &CBufLen);
		char* path = buffer3;
		printf("\npath: %s", path);
		GetDiskFreeSpaceExA(buffer2, (PULARGE_INTEGER)&available, (PULARGE_INTEGER)&total, (PULARGE_INTEGER)&free);
		printf("\nsize : %I64d  bytes ", total);
		printf("\nFree space : %I64d  bytes\n", available);



	} while (FindNextVolumeA(firstVolume, buffer2, BUFSIZE));
	FindVolumeClose(firstVolume);

	//p1.5
	DWORD dwSize;
	TCHAR szName[MAX_KEY_LENGTH];
	HKEY hKey;
	DWORD dwIndex = 0;
	DWORD retCode;
	DWORD BufferSize = 8192;
	PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK)malloc(BufferSize);
	DWORD cbData = BufferSize;

	if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		0, KEY_ALL_ACCESS, &hKey) == !ERROR_SUCCESS)
	{
		printf("Function RegOpenKeyEx() failed!\n");
	}
	else printf("\nStartup commands : \n");

	while (1) {
		dwSize = sizeof(szName);
		retCode = RegEnumValue(hKey, dwIndex, szName, &dwSize, NULL, NULL, NULL, NULL);

		if (retCode == ERROR_SUCCESS)
		{
			RegQueryValueExA(hKey, (LPCSTR)&szName, NULL, NULL, (LPBYTE)PerfData, &cbData);
			printf("      %d: %s:  %s\n", dwIndex + 1, szName, PerfData);
			dwIndex++;
		}
		else break;
	}


	//RegCloseKey(hKey);
	


	//p2
	StartCounter();
	cout << "CPU clock count: " << GetCounter() << "  us \n";
	return 0;
}