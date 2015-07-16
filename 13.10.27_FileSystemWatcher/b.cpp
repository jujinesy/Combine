#include <windows.h>
#include <stdio.h>



DWORD FolderWatcherThread(TCHAR *Folders[], DWORD NumFolders)
{
	DWORD NumFolderWatches = 0;
	TCHAR **FolderNames;
	HANDLE *Events;
	DWORD dwWait;
	DWORD Index;

	Events = LocalAlloc(LMEM_FIXED, NumFolders * sizeof( HANDLE ) );
	FolderNames = LocalAlloc(LMEM_FIXED, NumFolders * sizeof( TCHAR * ) );

	for (Index = 0; Index < NumFolders; Index++)
	{
		Events[NumFolderWatches] = FindFirstChangeNotification(Folders[Index], FALSE,
			FILE_NOTIFY_CHANGE_CEGETINFO |
			FILE_NOTIFY_CHANGE_FILE_NAME |
			FILE_NOTIFY_CHANGE_DIR_NAME |
			FILE_NOTIFY_CHANGE_SIZE |
			FILE_NOTIFY_CHANGE_LAST_WRITE|
			FILE_NOTIFY_CHANGE_ATTRIBUTES|
			FILE_NOTIFY_CHANGE_LAST_ACCESS|
			FILE_NOTIFY_CHANGE_CREATION|
			FILE_NOTIFY_CHANGE_SECURITY);
		if (INVALID_HANDLE_VALUE != Events[NumFolderWatches])
		{
			FolderNames[NumFolderWatches] = Folders[Index];
			NumFolderWatches++;
		}
	}



	if (NumFolderWatches == 0)
		return 0;
	while (TRUE)
	{
		dwWait = WaitForMultipleObjects(NumFolderWatches, Events, FALSE, INFINITE);
		if (NumFolderWatches > dwWait)
		{
			DWORD RequiredBufferSize = 0;
			LPBYTE NotificationData = NULL;
			FILE_NOTIFY_INFORMATION * pNotifyData;
			DWORD FNIOffset;
			if (!CeGetFileNotificationInfo(Events[dwWait],
				0, NULL, 0, NULL, &RequiredBufferSize))
			{
				continue;
			}

			NotificationData = (LPBYTE) LocalAlloc(LMEM_FIXED, RequiredBufferSize);
			if(NotificationData == NULL)
				break;
			if(!CeGetFileNotificationInfo(Events[dwWait],
				0, NotificationData, RequiredBufferSize, NULL, NULL))
			{
				LocalFree(NotificationData);
				continue;
			}
			RETAILMSG( 1, (TEXT("Folder: %s\n"), FolderNames[dwWait]));
			pNotifyData = NULL;
			FNIOffset = 0;
			do
			{
				pNotifyData = (FILE_NOTIFY_INFORMATION *) (NotificationData+FNIOffset);
				RETAILMSG( 1, (TEXT("NextEntryOffset %d\n"), pNotifyData->NextEntryOffset)); 
				RETAILMSG( 1, (TEXT("Action %d\n"), pNotifyData->Action)); 
				RETAILMSG( 1, (TEXT("FileNameLength %d\n"), pNotifyData->FileNameLength)); 
				RETAILMSG( 1, (TEXT("FileName %s\n"), pNotifyData->FileName)); 
				switch (pNotifyData->Action)
				{
				case FILE_ACTION_CHANGE_COMPLETED:
					RETAILMSG( 1, (TEXT("File change complete %s\n"), pNotifyData->FileName ));
					break;
				case FILE_ACTION_RENAMED_NEW_NAME:
					RETAILMSG( 1, (TEXT("File name changed %s\n"), pNotifyData->FileName ));
					break;
				case FILE_ACTION_REMOVED:
					RETAILMSG( 1, (TEXT("File removed %s\n"), pNotifyData->FileName ));
					break;
				case FILE_ACTION_RENAMED_OLD_NAME:
					RETAILMSG( 1, (TEXT("File remaned, old name %s\n"), pNotifyData->FileName ));
					break;
				case FILE_ACTION_ADDED:
					RETAILMSG( 1, (TEXT("File Added %s\n"), pNotifyData->FileName ));
					break;
				case FILE_ACTION_MODIFIED:
					RETAILMSG( 1, (TEXT("File modified %s\n"), pNotifyData->FileName ));
					break;
				default:
					RETAILMSG( 1, (TEXT("File unkown Action %X\n"), pNotifyData->Action ));
					break;
				}
				FNIOffset += pNotifyData->NextEntryOffset;
			} while(pNotifyData->NextEntryOffset);
			LocalFree(NotificationData);
			FindNextChangeNotification( Events[dwWait]);
		}
		else
		{
			break;
		}
	}
	for (Index = 0; Index < NumFolderWatches; Index++)

	{
		FindCloseChangeNotification( Events[Index]);
	}
	LocalFree(Events);
	LocalFree( FolderNames );


	return 0;

}


DWORD DiskWatcher()

{

	TCHAR *Folders[] = { TEXT("\\Hard Disk"), TEXT("\\Storage Card"), TEXT("\\Storage Card\\Data") };



	return FolderWatcherThread( Folders, sizeof(Folders)/sizeof(*Folders));

}