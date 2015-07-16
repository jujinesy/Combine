////////////////////////////////////////////////////////////////////////////////
/// \file FileSystemWatcher.cpp
/// \author excel96
/// \date 2006.10.26
////////////////////////////////////////////////////////////////////////////////

//#include "PCH.h"
#include "FileSystemWatcher.h"

namespace
{
	/*
	const DWORD FILTER = 
	FILE_NOTIFY_CHANGE_SECURITY |
	FILE_NOTIFY_CHANGE_CREATION |
	FILE_NOTIFY_CHANGE_LAST_ACCESS |
	FILE_NOTIFY_CHANGE_LAST_WRITE |
	FILE_NOTIFY_CHANGE_SIZE |
	FILE_NOTIFY_CHANGE_ATTRIBUTES |
	FILE_NOTIFY_CHANGE_DIR_NAME |
	FILE_NOTIFY_CHANGE_FILE_NAME;

	FILE_NOTIFY_CHANGE_CREATION | 
	FILE_NOTIFY_CHANGE_LAST_WRITE |
	FILE_NOTIFY_CHANGE_SIZE |
	FILE_NOTIFY_CHANGE_DIR_NAME |
	FILE_NOTIFY_CHANGE_FILE_NAME;
	*/

	/// �۾��� ����
	enum Operation
	{
		CONTINUE_MONITORING,  ///< ����͸��� ��� �Ѵ�.
		STOP_MONITORING,      ///< ����͸��� �ߴ��Ѵ�.
	};

	/// ����͸��� �̺�Ʈ�� ����
	const DWORD FILTER = 
	FILE_NOTIFY_CHANGE_CREATION | 
	FILE_NOTIFY_CHANGE_LAST_WRITE |
	FILE_NOTIFY_CHANGE_SIZE |
	FILE_NOTIFY_CHANGE_DIR_NAME |
	FILE_NOTIFY_CHANGE_FILE_NAME;

	/// MONITOR ������ ũ��
	const size_t BUFFER_SIZE = sizeof(FILE_NOTIFY_INFORMATION) * 1024;

	/// FILE_NOTIFY_INFORMATION ����ü �ȿ� ���̵� ���ڿ��� std::string ���·� 
	/// ��ȯ�ؼ� ��ȯ�Ѵ�.
	std::string ExtractFileName(const FILE_NOTIFY_INFORMATION* info)
	{
		char fileName[1024] = {0, };
		int count = WideCharToMultiByte(
			CP_ACP, 0, info->FileName, info->FileNameLength / sizeof(WCHAR), 
			fileName, _ARRAYSIZE(fileName) - 1, NULL, NULL);
		fileName[count] = '\0';
		return std::string(fileName);
	}

	/// ��� ������ �齽������ �߰��� ��θ� ��ȯ�Ѵ�.
	std::string AddBackslash(const std::string& path)
	{
		if (path.empty()) return std::string("");
		return path[path.size()-1] != '\\' ? path + std::string("\\") : path;
	}

	/// ���丮 + ���� �̸��� ���ļ� ��ü ��θ� �����Ѵ�.
	std::string Combine(const std::string& dir, const std::string& file)
	{
		return AddBackslash(dir) + file;
	}

	/// ���丮�� �����ϴ����� ���θ� ��ȯ�Ѵ�.
	bool IsDirectoryExist(const std::string& path)
	{
		DWORD attr = GetFileAttributes((CString)path.c_str());
		return (attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY);
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ������
/// \param type ������ ����
/// \param directory ���丮
/// \param fileName ���� �̸�
////////////////////////////////////////////////////////////////////////////////
cFileSystemEvent::cFileSystemEvent(ChangeType type, 
								   const std::string& directory, 
								   const std::string& fileName)
								   : m_ChangeType(type), 
								   m_Directory(directory), 
								   m_FileName(fileName)
{
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ������
/// \param directory ���丮
/// \param oldFileName ���� �̸�
/// \param newFileName ����� �̸�
////////////////////////////////////////////////////////////////////////////////
cFileSystemEvent::cFileSystemEvent(const std::string& directory, 
								   const std::string& oldFileName, 
								   const std::string& newFileName)
								   : m_ChangeType(RENAMED),
								   m_Directory(directory), 
								   m_FileName(oldFileName),
								   m_NewFileName(newFileName)
{
}

////////////////////////////////////////////////////////////////////////////////
/// \brief �Ҹ���
////////////////////////////////////////////////////////////////////////////////
cFileSystemEvent::~cFileSystemEvent()
{
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ���丮���� ������ ���� ��θ� ��ȯ�Ѵ�.
/// \return std::string ��� ���ڿ�
////////////////////////////////////////////////////////////////////////////////
std::string cFileSystemEvent::GetFullPath() const
{
	return Combine(m_Directory, m_FileName);
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ���丮���� ������ ���� ���� ��θ� ��ȯ�Ѵ�.
/// \return std::string ��� ���ڿ�
////////////////////////////////////////////////////////////////////////////////
std::string cFileSystemEvent::GetOldFullPath() const
{
	return Combine(m_Directory, m_FileName);
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ���丮���� ������ ���ο� ���� ��θ� ��ȯ�Ѵ�.
/// \return std::string ��� ���ڿ�
////////////////////////////////////////////////////////////////////////////////
std::string cFileSystemEvent::GetNewFullPath() const
{
	return Combine(m_Directory, m_NewFileName);
}


////////////////////////////////////////////////////////////////////////////////
/// \struct cFileSystemWatcher::MONITOR
/// \brief �񵿱� �Լ� ȣ���� ���� OVERLAPPED ����ü
////////////////////////////////////////////////////////////////////////////////

struct cFileSystemWatcher::MONITOR : public OVERLAPPED
{
	BYTE                Buffer[BUFFER_SIZE]; ///< ����
	int                 Operation;           ///< �۾��� ����
	cFileSystemWatcher* Watcher;             ///< ��ó ��ü
};

////////////////////////////////////////////////////////////////////////////////
/// \brief ������
////////////////////////////////////////////////////////////////////////////////
cFileSystemWatcher::cFileSystemWatcher()
	: m_hDirectory(INVALID_HANDLE_VALUE), m_Monitor(NULL)
{
	ZeroMemory(m_EventHandler, sizeof(PFN_EVENT_HANDLER) * cFileSystemEvent::MAX);
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ������ + Open
/// \param directory ����͸��� ���丮
////////////////////////////////////////////////////////////////////////////////
cFileSystemWatcher::cFileSystemWatcher(const std::string& directory)
	: m_hDirectory(INVALID_HANDLE_VALUE), m_Monitor(NULL)
{
	ZeroMemory(m_EventHandler, sizeof(PFN_EVENT_HANDLER) * cFileSystemEvent::MAX);

	if (!Open(directory))
		Close();
}

////////////////////////////////////////////////////////////////////////////////
/// \brief �Ҹ���
////////////////////////////////////////////////////////////////////////////////
cFileSystemWatcher::~cFileSystemWatcher()
{
	Close();
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ����͸��� �����Ѵ�.
/// \param directory ����͸��� ���丮
/// \return bool ������ ����͸��� �����ߴٸ� true, ���� ������ ����ٸ� false
////////////////////////////////////////////////////////////////////////////////
bool cFileSystemWatcher::Open(const std::string& directory)
{
	//Close();

	bool success = false;

	do 
	{
		m_Path = AddBackslash(directory);
		if (!IsDirectoryExist(m_Path))
			break;

		m_hDirectory = CreateFile((CString)m_Path.c_str(),
			FILE_LIST_DIRECTORY,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
			NULL
			);

		if (m_hDirectory == INVALID_HANDLE_VALUE)
			break;

		m_Monitor = reinterpret_cast<MONITOR*>(HeapAlloc(GetProcessHeap(), 0, sizeof(MONITOR)));
		if (!m_Monitor)
			break;

		ZeroMemory(m_Monitor, sizeof(MONITOR));
		m_Monitor->Watcher = this;

		if (!PostChangeNotificationRequest(CONTINUE_MONITORING))
			break;

		success = true;
	} while(0);

	if (!success)
		Close();

	return success;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ����͸��� �����Ѵ�.
////////////////////////////////////////////////////////////////////////////////
void cFileSystemWatcher::Close()
{
	m_Path = "";

	if (m_hDirectory != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hDirectory);
		m_hDirectory = INVALID_HANDLE_VALUE;
	}

	if (m_Monitor)
	{
		HeapFree(GetProcessHeap(), 0, m_Monitor);
		m_Monitor = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ��ü�� ���°� ������������ ���θ� ��ȯ�Ѵ�.
/// \return bool �������̶�� true
////////////////////////////////////////////////////////////////////////////////
bool cFileSystemWatcher::IsGood() const
{
	return m_hDirectory != INVALID_HANDLE_VALUE && m_Monitor != NULL;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ���� �Լ�
/// \param period �񵿱� �Լ� ȣ�� ����� ó���� �ð�
////////////////////////////////////////////////////////////////////////////////
void cFileSystemWatcher::Heartbeat(DWORD period)
{
	if (IsGood())
	{
		SleepEx(period, TRUE); // make thread alertable
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \brief �̺�Ʈ �ڵ鷯�� �����Ѵ�.
/// \param type �̺�Ʈ�� ����
/// \param handler �ڵ鷯
////////////////////////////////////////////////////////////////////////////////
void cFileSystemWatcher::SetEventHandler(
	cFileSystemEvent::ChangeType type, cFileSystemWatcher::PFN_EVENT_HANDLER handler)
{
	if (0 <= type && type < cFileSystemEvent::MAX)
		m_EventHandler[type] = handler;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ���� ���� ��û�� ����Ʈ�Ѵ�.
/// \param operation �۾��� ����
/// \return bool ������ �����ߴٸ� true ���� ������ ����ٸ� false
////////////////////////////////////////////////////////////////////////////////
bool cFileSystemWatcher::PostChangeNotificationRequest(int operation)
{
	if (!IsGood()) return false;

	m_Monitor->Operation = operation;

	DWORD bytesReturned = 0;
	DWORD bufferSize = static_cast<DWORD>(BUFFER_SIZE);

	if (ReadDirectoryChangesW(m_hDirectory, m_Monitor->Buffer, bufferSize, 
		TRUE, FILTER, &bytesReturned, m_Monitor, FileSystemWatcherCallback))
	{
		return true;
	}
	else
	{
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ���� ������ �ܺο��� ȣ���ϱ� ���� �Լ�
/// \param fileName ������ ���� �̸�
////////////////////////////////////////////////////////////////////////////////
void cFileSystemWatcher::OnCreate(const std::string& fileName)
{
	if (m_EventHandler[cFileSystemEvent::CREATED])
	{
		cFileSystemEvent evt(cFileSystemEvent::CREATED, m_Path, fileName);
		m_EventHandler[cFileSystemEvent::CREATED](evt);
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ���� ����� �ܺο��� ȣ���ϱ� ���� �Լ�
/// \param fileName ����� ���� �̸�
////////////////////////////////////////////////////////////////////////////////
void cFileSystemWatcher::OnChange(const std::string& fileName)
{
	if (m_EventHandler[cFileSystemEvent::CHANGED])
	{
		cFileSystemEvent evt(cFileSystemEvent::CHANGED, m_Path, fileName);
		m_EventHandler[cFileSystemEvent::CHANGED](evt);
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ���� ������ �ܺο��� ȣ���ϱ� ���� �Լ�
/// \param fileName ������ ���� �̸�
////////////////////////////////////////////////////////////////////////////////
void cFileSystemWatcher::OnDelete(const std::string& fileName)
{
	if (m_EventHandler[cFileSystemEvent::DELETED])
	{
		cFileSystemEvent evt(cFileSystemEvent::DELETED, m_Path, fileName);
		m_EventHandler[cFileSystemEvent::DELETED](evt);
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ���� �̸� ����� �ܺο��� ȣ���ϱ� ���� �Լ�
/// \param oldFileName ���� ���� �̸�
/// \param newFileName ����� ���� �̸�
////////////////////////////////////////////////////////////////////////////////
void cFileSystemWatcher::OnRename(const std::string& oldFileName, const std::string& newFileName)
{
	if (m_EventHandler[cFileSystemEvent::RENAMED])
	{
		cFileSystemEvent evt(m_Path, oldFileName, newFileName);
		m_EventHandler[cFileSystemEvent::RENAMED](evt);
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \brief �Ϸ� ���� �Լ�
/// \param errorCode ���� �ڵ�
/// \param bytesTransferred ������ ó���� ��
/// \param overlapped ReadDirectoryChangesW �Լ����� �ѱ� �������� ����ü
////////////////////////////////////////////////////////////////////////////////
void CALLBACK cFileSystemWatcher::FileSystemWatcherCallback(
	DWORD errorCode, DWORD bytesTransferred, LPOVERLAPPED overlapped)
{
	cFileSystemWatcher::MONITOR* monitor = 
		reinterpret_cast<cFileSystemWatcher::MONITOR*>(overlapped);
	cFileSystemWatcher* watcher = monitor->Watcher;

	if (errorCode == ERROR_SUCCESS)
	{
		PFILE_NOTIFY_INFORMATION info = NULL;
		size_t offset = 0;
		std::string fileName, oldFileName, newFileName;

		do
		{
			info = reinterpret_cast<PFILE_NOTIFY_INFORMATION>(&monitor->Buffer[offset]);
			offset += info->NextEntryOffset;

			if (info->Action == FILE_ACTION_ADDED)
			{
				fileName = ExtractFileName(info);
				watcher->OnCreate(fileName);
			}
			else if (info->Action == FILE_ACTION_REMOVED)
			{
				fileName = ExtractFileName(info);
				watcher->OnDelete(fileName);
			}
			else if (info->Action == FILE_ACTION_MODIFIED)
			{
				fileName = ExtractFileName(info);
				watcher->OnChange(fileName);
			}
			else if (info->Action == FILE_ACTION_RENAMED_OLD_NAME)
			{
				oldFileName = ExtractFileName(info);
			}
			else if (info->Action == FILE_ACTION_RENAMED_NEW_NAME)
			{
				newFileName = ExtractFileName(info);
				watcher->OnRename(oldFileName, newFileName);
			}
		} while (info->NextEntryOffset != 0);
	}

	if (monitor->Operation == CONTINUE_MONITORING)
		watcher->PostChangeNotificationRequest(CONTINUE_MONITORING);
}