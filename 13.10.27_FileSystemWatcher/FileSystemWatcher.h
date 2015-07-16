////////////////////////////////////////////////////////////////////////////////
/// \file FileSystemWatcher.h
/// \author excel96
/// \date 2006.10.26
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <atlstr.h>
#include <process.h>
#include <Windows.h>
//#include <afxmt.h>
//#include <AFXPRIV.H>
////////////////////////////////////////////////////////////////////////////////
/// \class cFileSystemEvent
/// \brief ����� ���� �ý��ۿ� ���� ����.
///
/// ����� ������ �Ϲ� ������ ���� �ְ�, ���丮�� ���� �ִ�. �̸������δ� 
/// ����� ���� �������� ���丮���� ������ �� �����Ƿ�, GetFileAttributes
/// �Լ��� �̿��ؼ� �����ϵ��� �Ѵ�.
////////////////////////////////////////////////////////////////////////////////

class cFileSystemEvent
{
public:
	enum ChangeType 
	{
		CREATED, ///< ���� ����
		CHANGED, ///< ���� ���� ����
		DELETED, ///< ���� ����
		RENAMED, ///< ���� �̸� ����
		MAX,     ///< �迭�� ���� �ƽ���
	};


private:
	ChangeType  m_ChangeType;   ///< ������ ����
	std::string m_Directory;    ///< ���丮
	std::string m_FileName;     ///< ���� ���� �̸�
	std::string m_NewFileName;  ///< ����� ���� �̸�


public:
	/// \brief ������
	cFileSystemEvent(ChangeType type, const std::string& directory, 
		const std::string& fileName);

	/// \brief ������
	cFileSystemEvent(const std::string& directory, 
		const std::string& oldFileName, const std::string& newFileName);

	/// \brief �Ҹ���
	~cFileSystemEvent();


public:
	/// \brief ���丮���� ������ ���� ��θ� ��ȯ�Ѵ�.
	std::string GetFullPath() const;

	/// \brief ���丮���� ������ ���� ���� ��θ� ��ȯ�Ѵ�.
	std::string GetOldFullPath() const;

	/// \brief ���丮���� ������ ���ο� ���� ��θ� ��ȯ�Ѵ�.
	std::string GetNewFullPath() const;


public:
	/// \name Simple getter/setter
	/// \{ 
	ChangeType GetChangeType() const { return m_ChangeType; }
	const std::string& GetDirectory() const { return m_Directory; }
	const std::string& GetFileName() const { return m_FileName; }
	const std::string& GetOldFileName() const { return m_FileName; }
	const std::string& GetNewFileName() const { return m_NewFileName; }
	/// \} 
};


////////////////////////////////////////////////////////////////////////////////
/// \class cFileSystemWatcher
/// \brief ���� �ý��� ���� ����͸��� ���� Ŭ����.
///
/// Ŭ���� �ν��Ͻ��� �����ϰ�, Open �Լ��� ���� ����͸��� ���丮�� ������ 
/// ����, ���� ���� ��򰡿��� Heartbeat() �Լ��� ���������� ȣ������� �Ѵ�.
///
/// <pre>
///
/// cFileSystemWatcher watcher("C:\\Program Files");
/// watcher.SetEventHandler(cFileSystemEvent::CREATED, MyCreateHandler);
/// watcher.SetEventHandler(cFileSystemEvent::CHANGED, MyChangeHandler);
/// watcher.SetEventHandler(cFileSystemEvent::DELETED, MyDeleteHandler);
/// watcher.SetEventHandler(cFileSystemEvent::RENAMED, MyRenameHandler);
/// ...
/// while (true)
/// {
///     ...
///     watcher.Heartbeat();
/// }
/// </pre>
////////////////////////////////////////////////////////////////////////////////

class cFileSystemWatcher
{
public:
	typedef void (*PFN_EVENT_HANDLER)(const cFileSystemEvent& args);


private:
	struct MONITOR;

	std::string       m_Path;                                ///< ���丮 ��� ���ڿ�
	HANDLE            m_hDirectory;                          ///< ���丮 �ڵ�
	MONITOR*          m_Monitor;                             ///< OVERLAPPED ����ü
	PFN_EVENT_HANDLER m_EventHandler[cFileSystemEvent::MAX]; ///< �̺�Ʈ �ڵ鷯


public:
	/// \brief ������
	cFileSystemWatcher();

	/// \brief ������ + Open
	cFileSystemWatcher(const std::string& directory);

	/// \brief �Ҹ���
	virtual ~cFileSystemWatcher();


public:
	/// \brief ����͸��� �����Ѵ�.
	bool Open(const std::string& directory);

	/// \brief ����͸��� �ߴ��Ѵ�.
	void Close();

	/// \brief ��ü�� ���°� ������������ ���θ� ��ȯ�Ѵ�.
	bool IsGood() const;

	/// \brief ���� �Լ�
	void Heartbeat(DWORD period = 1);

	/// \brief �̺�Ʈ �ڵ鷯�� �����Ѵ�.
	void SetEventHandler(
		cFileSystemEvent::ChangeType type, PFN_EVENT_HANDLER handler);

	/// \brief ����͸� ���� ���丮�� ��ȯ�Ѵ�.
	const std::string& GetPath() const { return m_Path; }


private:
	/// \brief ���� ���� ��û�� ����Ʈ�Ѵ�.
	bool PostChangeNotificationRequest(int operation);

	/// \brief ���� ������ �ܺο��� ȣ���ϱ� ���� �Լ�
	void OnCreate(const std::string& fileName);

	/// \brief ���� ����� �ܺο��� ȣ���ϱ� ���� �Լ�
	void OnChange(const std::string& fileName);

	/// \brief ���� ������ �ܺο��� ȣ���ϱ� ���� �Լ�
	void OnDelete(const std::string& fileName);

	/// \brief ���� �̸� ����� �ܺο��� ȣ���ϱ� ���� �Լ�
	void OnRename(const std::string& oldFileName, const std::string& newFileName);

	/// \brief �Ϸ� ���� �Լ�
	static void CALLBACK FileSystemWatcherCallback(DWORD, DWORD, LPOVERLAPPED);
};