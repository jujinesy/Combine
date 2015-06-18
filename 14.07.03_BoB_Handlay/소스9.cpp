#include <Windows.h>
#include <stdio.h>
#include <TlHelp32.h>

typedef struct _THREAD_PARAM
{
	unsigned int     base;
	unsigned int   data;
	unsigned int   *argv;
	char pad1[0x20];
	DWORD pad2;
	DWORD x, y;
} THREAD_PARAM, *PTHREAD_PARAM;

DWORD arr_x[668], arr_y[668];
int      Height, Width, Mine;

typedef int(*FuncType)(unsigned int *argv);

typedef HMODULE(WINAPI *PFLOADLIBRARYA)
(
LPCSTR lpLibFileName
);

typedef FARPROC(WINAPI *PFGETPROCADDRESS)
(
HMODULE hModule,
LPCSTR lpProcName
);

typedef int (WINAPI *PFMESSAGEBOXA)
(
HWND hWnd,
LPCSTR lpText,
LPCSTR lpCaption,
UINT uType
);

int baseaddress(DWORD pID){
	DWORD addr = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
	MODULEENTRY32 module;
	module.dwSize = sizeof(MODULEENTRY32);
	Module32First(snapshot, &module);
	addr = (int)module.modBaseAddr;
	CloseHandle(snapshot);
	return addr;
}

LPVOID *getecx(unsigned long base, DWORD pid)
{
	HANDLE hProc = NULL;
	LPVOID buf[8] = { 0x0, };
	hProc = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	ReadProcessMemory(hProc, (LPCVOID)(base), buf, 8, NULL);
	CloseHandle(hProc);
	return buf;
}
LPVOID *get1byte(unsigned long base, DWORD pid)
{
	HANDLE hProc = NULL;
	LPVOID buf[1] = { 0x0, };
	hProc = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	ReadProcessMemory(hProc, (LPCVOID)(base), buf, 1, NULL);
	CloseHandle(hProc);
	return buf;
}
void search_mine(DWORD dwPID)
{
	unsigned long base = 0;
	unsigned long ecx1 = 0;
	int cel = 0;
	int i = 0;
	base = baseaddress(dwPID);

	printf("base : %p\n", base);
	for (int y = 0; y < Height; y++){
		for (int x = 0; x < Width; x++){
			ecx1 = (unsigned long)*getecx(base + 0xaaa38, dwPID) + 0x18;
			unsigned int ecx2 = (unsigned int)*getecx(ecx1, dwPID);
			unsigned int cel = ecx2 + 0x58;

			cel = (unsigned long)*getecx(cel, dwPID) + 0x10;
			cel = (unsigned long)*getecx(cel, dwPID) + (x * 8);
			cel = (unsigned long)*getecx(cel, dwPID) + 0x10;
			cel = (unsigned long)*getecx(cel, dwPID) + y;
			cel = (unsigned long)*get1byte(cel, dwPID);
			if (cel == 1){
				arr_x[i] = x;
				arr_y[i] = y;
				i++;
			}
		}
	}
}
DWORD WINAPI ThreadProc(LPVOID lParam)
{
	PTHREAD_PARAM pParam = (PTHREAD_PARAM)lParam;

	FuncType _ShowMine = (FuncType)(pParam->base);
	pParam->argv = &pParam->data;
	_ShowMine(pParam->argv);
	return 0;
}

BOOL InjectCode(DWORD dwPID)
{
	HMODULE hMod = NULL;
	THREAD_PARAM param = { 0, };
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	LPVOID pRemoteBuf[2] = { 0, };
	DWORD dwSize = 0;
	int i = 0;

	param.base = (unsigned int)baseaddress(dwPID);      // 프로그램의 base address를 구한다.
	param.data = param.base + 0x27c32;               // 지뢰찾기에서 handle값을 받아온다.
	param.base += 0x36c50;                        // showmine() offset + base address
	search_mine(dwPID);                           // mine위치를 탐색한다.
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwPID);

	for (i = 0; i < Mine + 1; i++){                     // Mine개수만큼 반복한다.
		param.x = arr_x[i];
		param.y = arr_y[i];
		printf("%d, %d\n", arr_x[i], arr_y[i]);
		dwSize = sizeof(THREAD_PARAM);               // 구조체 할당 & 메모리 적재
		pRemoteBuf[0] = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
		WriteProcessMemory(hProcess, pRemoteBuf[0], (LPVOID)&param, dwSize, NULL);

		dwSize = (DWORD)InjectCode - (DWORD)ThreadProc;   // 스레드 할당
		pRemoteBuf[1] = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		WriteProcessMemory(hProcess, pRemoteBuf[1], (LPVOID)ThreadProc, dwSize, NULL);

		hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pRemoteBuf[1], pRemoteBuf[0], 0, NULL);
	}
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);         // thread 닫기
	CloseHandle(hProcess);         // Process 닫기



	return TRUE;
}


BOOL SetPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	HANDLE hToken;
	LUID luid;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken))
	{
		printf("OpenProcessToken error: %u\n", GetLastError());
		return FALSE;
	}

	if (!LookupPrivilegeValue(NULL,           // lookup privilege on local system
		lpszPrivilege,  // privilege to lookup 
		&luid))        // receives LUID of privilege
	{
		printf("LookupPrivilegeValue error: %u\n", GetLastError());
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.
	if (!AdjustTokenPrivileges(hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		printf("AdjustTokenPrivileges error: %u\n", GetLastError());
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		printf("The token does not have the specified privilege. \n");
		return FALSE;
	}

	return TRUE;
}



int main(int argc, char *argv[])
{
	DWORD dwPID = 0;
	if (argc != 2){
		printf("%s [pid]\n", argv[0]);
		return 0;
	}
	// change privilege
	if (!SetPrivilege(SE_DEBUG_NAME, TRUE))
		return 1;
	printf("높이(9-24)(H)   :: ");
	scanf_s("%d", &Height);
	printf("너비(9-30)(W)   :: ");
	scanf_s("%d", &Width);
	printf("지뢰(10-668)(M) :: ");
	scanf_s("%d", &Mine);
	dwPID = (DWORD)atol(argv[1]);
	InjectCode(dwPID);
}