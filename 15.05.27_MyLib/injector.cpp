/**----------------------------------------------------------------------------
 * injector.cpp
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by somma (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * 26:8:2011   15:31 created
**---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "injector.h"
#include "Win32Utils.h"

bool 
my_create_remote_thread(
	_In_ HANDLE process_handle, 
	_In_ LPTHREAD_START_ROUTINE thread_start_routine, 
	_In_ LPVOID thread_param
	);

HANDLE bCreateUserThread(HANDLE hHandle, LPVOID loadLibAddr, LPVOID dllPathAddr);

typedef LONG NTSTATUS;

struct NtCreateThreadExBuffer {
	ULONG Size;
	ULONG Unknown1;
	ULONG Unknown2;
	PULONG Unknown3;
	ULONG Unknown4;
	ULONG Unknown5;
	ULONG Unknown6;
	PULONG Unknown7;
	ULONG Unknown8;
	}; 

typedef NTSTATUS (WINAPI *pNtCreateThreadEx) (
	OUT PHANDLE hThread,
	IN ACCESS_MASK DesiredAccess,
	IN LPVOID ObjectAttributes,
	IN HANDLE ProcessHandle,
	IN LPTHREAD_START_ROUTINE lpStartAddress,
	IN LPVOID lpParameter,
	IN BOOL CreateSuspended,
	IN ULONG StackZeroBits,
	IN ULONG SizeOfStackCommit,
	IN ULONG SizeOfStackReserve,
	OUT LPVOID lpBytesBuffer
);


/**
* @brief	
* @param	
* @see		
* @remarks	
* @code		
* @endcode	
* @return	
*/
bool inject_dll(_In_ DWORD pid, _In_z_ const char* dll_path)
{
	// Ÿ�� ���μ��� ����		
	DWORD rights = PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ;
	HANDLE process_handle = privileged_open_process(pid, rights, true);
	if (NULL==process_handle)
	{
		//log_err "privileged_open_process(pid=%u) failed", pid log_end
		return false;
	}

	// ��� ���μ��� �޸𸮿� dll ��θ� ��ŭ�� ���� �Ҵ� (�Ķ���� ����)
	unsigned int buffer_size = (unsigned int)strlen(dll_path) + sizeof(char);
	char* buffer = (char*) VirtualAllocEx(
								process_handle, 
								NULL, 
								buffer_size, 
								MEM_COMMIT, 
								PAGE_READWRITE
								);	
	if (NULL == buffer)
	{
		CloseHandle(process_handle);
		return false;
	}

	// dll ��θ� (���ڿ�) ����
	SIZE_T cbWritten=0;
	if (TRUE != WriteProcessMemory(
								process_handle, 
								buffer, 
								dll_path, 
								(DWORD)strlen(dll_path), 
								&cbWritten))
	{
		log_err "WriteProcessMemory() failed. gle=0x%08x", GetLastError() log_end

		VirtualFreeEx(process_handle, buffer, buffer_size, MEM_DECOMMIT);
		CloseHandle(process_handle);
		return false;
	}

	// LoadLibraryA �ּ� ���ϱ�
	HMODULE kernel32_handle = GetModuleHandleW(L"kernel32.dll");
	LPTHREAD_START_ROUTINE load_library_ptr = 
		(LPTHREAD_START_ROUTINE) GetProcAddress(kernel32_handle, "LoadLibraryA");

	// create remote-thread 
	bool ret = my_create_remote_thread(
								process_handle, 
								load_library_ptr, 
								buffer
								);
	VirtualFreeEx(process_handle, buffer, buffer_size, MEM_DECOMMIT);
	CloseHandle(process_handle);
	
	
	if (true != ret)
	{
		log_err "my_create_remote_thread( pid = %u ) failed.", pid log_end		
		return false;
	}
	else
	{
		return true;
	}
}


/**
 * @brief	
 * @param	
 * @see		
 * @remarks		�̽¿� å�� �ڵ� (05_64��Ʈ_&_Windows_Kernel_6\43_DLL_Injection_in_Kernel_6\src\InjectDll_new\InjectDll_new.cpp)
				�� debug ��忡���� ����, release ��忡���� ������
				������ �𸣰ڳ�... �ϴ� �ٻڴ� NtCreateThreadEx ����ϴ°� ���߿� ����.
				RtlCreateUserThread �� ������

 * @code		
 * @endcode	
 * @return	
**/
bool 
my_create_remote_thread(
	_In_ HANDLE process_handle, 
	_In_ LPTHREAD_START_ROUTINE thread_start_routine, 
	_In_ LPVOID thread_param
	)
{
	HANDLE thread_handle = NULL;
	
	/*
	if (true == is_vista_later(OsVersion()))
	{
		struct NtCreateThreadExBuffer ntbuffer = {0};
		DWORD temp1 = 0; 
		DWORD temp2 = 0; 

		ntbuffer.Size = sizeof(struct NtCreateThreadExBuffer);
		ntbuffer.Unknown1 = 0x10003;
		ntbuffer.Unknown2 = 0x8;
		ntbuffer.Unknown3 = &temp2;
		ntbuffer.Unknown4 = 0;
		ntbuffer.Unknown5 = 0x10004;
		ntbuffer.Unknown6 = 4;
		ntbuffer.Unknown7 = &temp1;
		ntbuffer.Unknown8 = 0;
		
		pNtCreateThreadEx func = (pNtCreateThreadEx)GetProcAddress(
														GetModuleHandleW(L"ntdll.dll"), 
														"NtCreateThreadEx");
		if(NULL == func)
		{
			log_err "GetProcAddress( NtCreateThreadEx ) failed. gle = %u", GetLastError() log_end
			return false;
		}
		
		NTSTATUS status = func(
							&thread_handle, 
							THREAD_ALL_ACCESS,
							NULL,
							process_handle,
							thread_start_routine,
							thread_param,
							FALSE,
							NULL,
							NULL,
							NULL,
							&ntbuffer
							);
		if (NULL == thread_handle)
		{
			log_err "NtCreateThreadEx() failed. status = %u, gle = %u", status, GetLastError() log_end
			return false;
		}
	}
	else
	{   
	
		thread_handle = CreateRemoteThread(
									process_handle, 
									NULL, 
									0, 
									thread_start_routine, 
									thread_param, 
									0, 
									NULL
									);
        if( thread_handle == NULL )
        {
            //log_err "CreateRemoteThread() failed. gle = %u", GetLastError() log_end
            return false;
        }
	
	}
	*/

	thread_handle = bCreateUserThread(process_handle, thread_start_routine, thread_param);
	if (thread_handle  == NULL) return false;

	WaitForSingleObject(thread_handle, INFINITE);		// dll loading �� �Ϸ�� �� ���� ���
	return true;
}


/**
 * @brief	
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
typedef struct _CLIENT_ID {
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID;
typedef CLIENT_ID *PCLIENT_ID;

typedef long (WINAPI *LPFUN_RtlCreateUserThread)(
		HANDLE,					// ProcessHandle
	    PSECURITY_DESCRIPTOR,	// SecurityDescriptor (OPTIONAL)
	    BOOLEAN,				// CreateSuspended
		ULONG,					// StackZeroBits
	    PULONG,					// StackReserved
		PULONG,					// StackCommit
	    PVOID,					// StartAddress
		PVOID,					// StartParameter (OPTIONAL)
	    PHANDLE,				// ThreadHandle
		PCLIENT_ID				// ClientID
);

HANDLE bCreateUserThread(HANDLE hHandle, LPVOID loadLibAddr, LPVOID dllPathAddr) {
	/*
		Provided help
			http://syprog.blogspot.com/2012/05/createremotethread-bypass-windows.html?showComment=1338375764336#c4138436235159645886
			http://undocumented.ntinternals.net/UserMode/Undocumented%20Functions/Executable%20Images/RtlCreateUserThread.html
			http://www.rohitab.com/discuss/topic/39493-using-rtlcreateuserthread/
	*/


	HANDLE hRemoteThread = NULL;
	LPVOID rtlCreateUserAddr = NULL;
	
	CLIENT_ID cid;
	
	rtlCreateUserAddr = GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), "RtlCreateUserThread");

	if( rtlCreateUserAddr ) 
	{
	
		LPFUN_RtlCreateUserThread funRtlCreateUserThread = (LPFUN_RtlCreateUserThread)rtlCreateUserAddr;
		funRtlCreateUserThread(
					hHandle,			// ProcessHandle
					NULL,				// SecurityDescriptor (OPTIONAL)
					FALSE,				// CreateSuspended
					0,					// StackZeroBits
					0,					// StackReserved
					0,					// StackCommit
					(PVOID) loadLibAddr,// StartAddress
					(PVOID) dllPathAddr,// StartParameter (OPTIONAL)
					&hRemoteThread,		// ThreadHandle
					&cid				// ClientID
				);
		
		if (hRemoteThread == NULL) 
		{			
			log_err "RtlCreateUserThread() failed. gle = %u", GetLastError() log_end
			return NULL;
		} 
		else 
		{
			return hRemoteThread;
		}
	} 
	else 
	{
		log_err "Could not find RtlCreateUserThread" log_end
	}

	return NULL;

}