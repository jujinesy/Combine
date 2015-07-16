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
/// \brief 변경된 파일 시스템에 대한 정보.
///
/// 변경된 파일은 일반 파일일 수도 있고, 디렉토리일 수도 있다. 이름만으로는 
/// 변경된 것이 파일인지 디렉토리인지 구분할 수 없으므로, GetFileAttributes
/// 함수를 이용해서 구분하도록 한다.
////////////////////////////////////////////////////////////////////////////////

class cFileSystemEvent
{
public:
	enum ChangeType 
	{
		CREATED, ///< 파일 생성
		CHANGED, ///< 파일 내용 변경
		DELETED, ///< 파일 삭제
		RENAMED, ///< 파일 이름 변경
		MAX,     ///< 배열을 위한 맥스값
	};


private:
	ChangeType  m_ChangeType;   ///< 변경의 종류
	std::string m_Directory;    ///< 디렉토리
	std::string m_FileName;     ///< 원래 파일 이름
	std::string m_NewFileName;  ///< 변경된 파일 이름


public:
	/// \brief 생성자
	cFileSystemEvent(ChangeType type, const std::string& directory, 
		const std::string& fileName);

	/// \brief 생성자
	cFileSystemEvent(const std::string& directory, 
		const std::string& oldFileName, const std::string& newFileName);

	/// \brief 소멸자
	~cFileSystemEvent();


public:
	/// \brief 디렉토리까지 포함한 파일 경로를 반환한다.
	std::string GetFullPath() const;

	/// \brief 디렉토리까지 포함한 이전 파일 경로를 반환한다.
	std::string GetOldFullPath() const;

	/// \brief 디렉토리까지 포함한 새로운 파일 경로를 반환한다.
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
/// \brief 파일 시스템 변경 모니터링을 위한 클래스.
///
/// 클래스 인스턴스를 생성하고, Open 함수를 통해 모니터링할 디렉토리를 지정한 
/// 다음, 메인 루프 어딘가에서 Heartbeat() 함수를 지속적으로 호출해줘야 한다.
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

	std::string       m_Path;                                ///< 디렉토리 경로 문자열
	HANDLE            m_hDirectory;                          ///< 디렉토리 핸들
	MONITOR*          m_Monitor;                             ///< OVERLAPPED 구조체
	PFN_EVENT_HANDLER m_EventHandler[cFileSystemEvent::MAX]; ///< 이벤트 핸들러


public:
	/// \brief 생성자
	cFileSystemWatcher();

	/// \brief 생성자 + Open
	cFileSystemWatcher(const std::string& directory);

	/// \brief 소멸자
	virtual ~cFileSystemWatcher();


public:
	/// \brief 모니터링을 시작한다.
	bool Open(const std::string& directory);

	/// \brief 모니터링을 중단한다.
	void Close();

	/// \brief 객체의 상태가 정상적인지의 여부를 반환한다.
	bool IsGood() const;

	/// \brief 폴링 함수
	void Heartbeat(DWORD period = 1);

	/// \brief 이벤트 핸들러를 설정한다.
	void SetEventHandler(
		cFileSystemEvent::ChangeType type, PFN_EVENT_HANDLER handler);

	/// \brief 모니터링 중인 디렉토리를 반환한다.
	const std::string& GetPath() const { return m_Path; }


private:
	/// \brief 변경 사항 요청을 포스트한다.
	bool PostChangeNotificationRequest(int operation);

	/// \brief 파일 생성시 외부에서 호출하기 위한 함수
	void OnCreate(const std::string& fileName);

	/// \brief 파일 변경시 외부에서 호출하기 위한 함수
	void OnChange(const std::string& fileName);

	/// \brief 파일 삭제시 외부에서 호출하기 위한 함수
	void OnDelete(const std::string& fileName);

	/// \brief 파일 이름 변경시 외부에서 호출하기 위한 함수
	void OnRename(const std::string& oldFileName, const std::string& newFileName);

	/// \brief 완료 통지 함수
	static void CALLBACK FileSystemWatcherCallback(DWORD, DWORD, LPOVERLAPPED);
};