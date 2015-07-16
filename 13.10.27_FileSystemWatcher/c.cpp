#include <windows.h>
#include <stdio.h>
#include <tchar.h> 
#define DUMMY_DIR               _T("C:\\temp")
#define FILE_SPY_BUFFSIZE      (sizeof(FILE_NOTIFY_INFORMATION) + (sizeof(wchar_t) * MAX_PATH))
#define MAX_FILE_SPY_COUNT     1024
#define MAX_FILE_SPY_BUFFSIZE  (FILE_SPY_BUFFSIZE * MAX_FILE_SPY_COUNT)
#define MAX_WAIT_TIMESEC       1000 * 10
#define FILE_SPY_FILTER       (FILE_NOTIFY_CHANGE_FILE_NAME  | \
	FILE_NOTIFY_CHANGE_DIR_NAME   | \
	FILE_NOTIFY_CHANGE_ATTRIBUTES | \
	FILE_NOTIFY_CHANGE_SIZE       | \
	FILE_NOTIFY_CHANGE_LAST_WRITE | \
	FILE_NOTIFY_CHANGE_LAST_ACCESS| \
	FILE_NOTIFY_CHANGE_CREATION   | \ 
	FILE_NOTIFY_CHANGE_SECURITY) 

typedef struct FileSpyData{    
	OVERLAPPED m_overlapped;    /// 비동기식 처리를 하기 위해서    
	BYTE *m_pBuff;              /// 파일 모니터링 버퍼
} FileSpyData; 

FileSpyData *CreateFileSpyData()
{    
	FileSpyData *pSpy = new FileSpyData;     
	ZeroMemory(pSpy, sizeof(FileSpyData));    
	pSpy->m_pBuff = new BYTE[MAX_FILE_SPY_BUFFSIZE];    
	ZeroMemory(pSpy->m_pBuff, MAX_FILE_SPY_BUFFSIZE);     

	return pSpy;
} 

void DestoryFileSpyData(FileSpyData *pSpy)
{    
	delete []pSpy->m_pBuff;    
	delete pSpy;
} 

/**    @brief      동기식 디렉토리 모니터링 예제*/

void DirSpy()
{    
	HANDLE hDirSpy = NULL;     
	hDirSpy = FindFirstChangeNotification(DUMMY_DIR, TRUE, FILE_NOTIFY_CHANGE_FILE_NAME);     
	if ( NULL == hDirSpy )    
	{        
		_tprintf(_T("fail to FindFirstChangeNotification\n"));        
		return;    
	}     

	do     
	{        
		if ( WAIT_OBJECT_0 == WaitForSingleObject(hDirSpy, INFINITE) )        
		{            
			_tprintf(_T("some file is changed..\n"));        
		}    
	} while ( FindNextChangeNotification(hDirSpy) );     
	FindCloseChangeNotification(hDirSpy);

} 

void OnChangeDir(BYTE *pBuff)
{   
	DWORD dwOffset = 0;    
	PFILE_NOTIFY_INFORMATION pFileInfo = NULL;        
	do    
	{        
		pFileInfo = reinterpret_cast<PFILE_NOTIFY_INFORMATION>(pBuff + dwOffset);        
		dwOffset += pFileInfo->NextEntryOffset;         

		if (pFileInfo->Action == FILE_ACTION_ADDED)        
		{            
			_tprintf(_T("create name : %s\n"), pFileInfo->FileName);        }        
		else if (pFileInfo->Action == FILE_ACTION_REMOVED)        
		{            
			_tprintf(_T("remove name : %s\n"), pFileInfo->FileName);        }        
		else if (pFileInfo->Action == FILE_ACTION_MODIFIED)        
		{            
			_tprintf(_T("change name : %s\n"), pFileInfo->FileName);        }        
		else if (pFileInfo->Action == FILE_ACTION_RENAMED_OLD_NAME)        
		{            
			_tprintf(_T("old name : %s\n"), pFileInfo->FileName);        
		}        
		else if (pFileInfo->Action == FILE_ACTION_RENAMED_NEW_NAME)       
		{            
			_tprintf(_T("new name : %s\n"), pFileInfo->FileName);        
		}    
	} while ( pFileInfo->NextEntryOffset != 0 );     

	_tprintf(_T("=================================================\n"));
} 

/**    @brief      WaitForSingleObject 함수를 사용한 비 동기식 디렉토리 모니터링 예제 */

void AsyncDirSpy()
{    
	BOOL bRet = FALSE;   
	DWORD dwTransfer = 0;    
	FileSpyData *pSpy = CreateFileSpyData();    
	HANDLE hDir = CreateFile(DUMMY_DIR,                              
		FILE_LIST_DIRECTORY,                 
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,                             
		NULL,                              
		OPEN_EXISTING,                              
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,   
		NULL);     

	if (NULL == hDir)        
		return;     

	do    
	{        
		dwTransfer = 0;                
		ZeroMemory(pSpy->m_pBuff, MAX_FILE_SPY_BUFFSIZE);        
		bRet = ReadDirectoryChangesW(hDir, 
			pSpy->m_pBuff,             
			MAX_FILE_SPY_BUFFSIZE,    
			TRUE,                       
			FILE_SPY_FILTER,             
			&dwTransfer,                     
			(LPOVERLAPPED)pSpy,          
			NULL);                

		if ( bRet && WaitForSingleObject(hDir, MAX_WAIT_TIMESEC) == WAIT_OBJECT_0 )        
		{           
			OnChangeDir(pSpy->m_pBuff);        
		}    
	} while ( TRUE == bRet );     

	DestoryFileSpyData(pSpy);    
	CloseHandle(hDir);
} 

void CALLBACK SpyCallbackFunc(DWORD dwErr, DWORD dwTransfer, LPOVERLAPPED pOverlapped)
{    
	FileSpyData *pSpy = reinterpret_cast<FileSpyData *>(pOverlapped);        
	if ( NULL == pSpy )        
		return;     

	OnChangeDir(pSpy->m_pBuff);
} 

/**    @brief      콜백함수를 사용한 비 동기식 디렉토리 모니터링 예제 */

void CallbackDirSpy()
{    
	BOOL bRet = FALSE;    
	DWORD dwTransfer = 0;    
	FileSpyData *pSpy = CreateFileSpyData();    
	HANDLE hDir = CreateFile(DUMMY_DIR,                              
		FILE_LIST_DIRECTORY,                              
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,  
		NULL,                           
		OPEN_EXISTING,                          
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,      
		NULL);    

	if (NULL == hDir)        
		return;     
	do   
	{     
		dwTransfer = 0;  
		ZeroMemory(pSpy->m_pBuff, MAX_FILE_SPY_BUFFSIZE);        
		bRet = ReadDirectoryChangesW(hDir,
			pSpy->m_pBuff,
			MAX_FILE_SPY_BUFFSIZE,
			TRUE,
			FILE_SPY_FILTER,
			&dwTransfer,
			(LPOVERLAPPED)pSpy,
			SpyCallbackFunc);               
		if ( bRet )           
			SleepEx(MAX_WAIT_TIMESEC, TRUE);     
	} while ( TRUE == bRet );     

	DestoryFileSpyData(pSpy);    
	CloseHandle(hDir);
} 

int main(int argc, char **argv)
{    
	DirSpy();    
	AsyncDirSpy();    
	CallbackDirSpy();    
	return 0;
} 