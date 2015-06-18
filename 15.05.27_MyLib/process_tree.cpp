/**----------------------------------------------------------------------------
 * process_tree.cpp
 *-----------------------------------------------------------------------------
 * module that manage running process
 *-----------------------------------------------------------------------------
 * All rights reserved by Noh,Yonghwan (fixbrain@gmail.com)
 *-----------------------------------------------------------------------------
 * 2014:6:15 22:23 created
**---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "process_tree.h"

/**
 * @brief	
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
bool process::kill(_In_ DWORD exit_code)
{
	_ASSERTE(true != _killed);	
	if (true == _killed) return true;

	set_privilege(SE_DEBUG_NAME, TRUE);
	
	HANDLE h = NULL;
	do 
	{
		h = OpenProcess(PROCESS_TERMINATE, FALSE, _pid);
		if (NULL == h)
		{
			log_err 
				"OpenProcess() failed, pid = %u, gle = %u", 
				_pid,
				GetLastError()
			log_end
			break;
		}
	
		if (!TerminateProcess(h, exit_code))
		{
			log_err 
				"TerminateProcess() failed, pid = %u, gle = %u", 
				_pid,
				GetLastError()
			log_end
			break;			
		}
	

		_killed = true;	
		log_dbg "pid = %u, %ws terminated", _pid, _process_name.c_str() log_end
	} while (false);
	
	if (NULL!=h) 
	{
		CloseHandle(h); // TerminateProcess() is asynchronous, so must call CloseHandle()
	}

	set_privilege(SE_DEBUG_NAME, FALSE);

	return (true == _killed) ? true : false;
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
bool 
cprocess_tree::build_process_tree()
{
	_proc_map.clear();

	bool ret = false;

	PROCESSENTRY32W proc_entry = {0};
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snap == INVALID_HANDLE_VALUE)
	{
		log_err "CreateToolhelp32Snapshot() failed, gle = %u", GetLastError() log_end
		return false;
	}

	if (true != set_privilege(SE_DEBUG_NAME, TRUE))
	{
		log_info "set_privilege(SE_DEBUG_NAME) failed." log_end
		// just info
	}

	do
	{
		proc_entry.dwSize = sizeof(PROCESSENTRY32W);
		if (!Process32First(snap, &proc_entry))
		{
			log_err "CreateToolhelp32Snapshot() failed, gle = %u", GetLastError() log_end
			break;
		}

		do
		{
			FILETIME create_time={0};
			HANDLE process_handle = OpenProcess(
										PROCESS_QUERY_INFORMATION, 
										FALSE, 
										proc_entry.th32ProcessID
										);
			if(NULL == process_handle)
			{
				// too much logs.
				//log_err 
				//	"OpenProcess() failed, pid = %u, proc = %s, gle = %u", 
				//	proc_entry.th32ProcessID, 
				//	WcsToMbsEx(proc_entry.szExeFile).c_str(),
				//	GetLastError() 
				//log_end

				// use create time 0!
			}
			else
			{
				FILETIME dummy_time;
				if (!GetProcessTimes(process_handle, &create_time, &dummy_time, &dummy_time, &dummy_time))
				{
					log_err "GetProcessTimes() failed, gle = %u", GetLastError() log_end
					// use create time 0!
				}

				CloseHandle(process_handle); process_handle = NULL;
			}			
						
			add_process(proc_entry.th32ParentProcessID, proc_entry.th32ProcessID, create_time, proc_entry.szExeFile);

		} while (Process32Next(snap, &proc_entry));

	} while (false);


	CloseHandle(snap);
	set_privilege(SE_DEBUG_NAME, FALSE);

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
DWORD cprocess_tree::find_process(_In_ const wchar_t* process_name)
{
	_ASSERTE(NULL != process_name);
	if (NULL == process_name) return false;

	process_map::iterator it = _proc_map.begin();
	process_map::iterator ite = _proc_map.end();
	for(; it != ite; ++it)
	{
		if (0 == it->second.process_name().compare(process_name)) 
		{
			// found
			return it->second.pid();
		}
	}

	return 0;
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
const wchar_t* cprocess_tree::get_process_name(_In_ DWORD pid)
{
	process_map::iterator it = _proc_map.find(pid);
	if (it == _proc_map.end()) return NULL;

	return it->second.process_name().c_str();

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
DWORD cprocess_tree::get_parent_pid(_In_ DWORD child_pid)
{
	process_map::iterator it = _proc_map.find(child_pid);
	if (it == _proc_map.end()) return 0;

	process prcs = it->second;
	return prcs.ppid();
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
const wchar_t* cprocess_tree::get_parent_name(_In_ DWORD child_pid)
{
	DWORD ppid = get_parent_pid(child_pid);
	if (0 == ppid) return NULL;

	process_map::iterator it = _proc_map.find(ppid);
	if (it == _proc_map.end()) return NULL;

	return it->second.process_name().c_str();
}

/**
 * @brief	��� ���μ����� �ݹ� �Լ��� ���� �����Ѵ�.
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
void cprocess_tree::iterate_process(_In_ fnproc_tree_callback callback, _In_ DWORD_PTR callback_tag)
{
	_ASSERTE(NULL != callback);		
	if (NULL == callback) return;
	
	process_map::iterator its = _proc_map.begin();
	process_map::iterator ite= _proc_map.end();
	for(; its != ite; ++its)
	{
		if (true != callback(its->second, callback_tag)) break;
	}
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
void cprocess_tree::iterate_process_tree(_In_ DWORD root_pid, _In_ fnproc_tree_callback callback, _In_ DWORD_PTR callback_tag)
{
	_ASSERTE(NULL != callback);		
	if (NULL == callback) return;

	process_map::iterator it = _proc_map.find(root_pid);
	if (it == _proc_map.end()) return;
	process root = it->second;

	iterate_process_tree(root, callback, callback_tag);
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
void cprocess_tree::print_process_tree(_In_ DWORD root_pid)
{
	process_map::iterator it = _proc_map.find(root_pid);
	if (it != _proc_map.end())
	{
		DWORD depth = 0;
		print_process_tree(it->second, depth);
		_ASSERTE(0 == depth);
	}
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
void cprocess_tree::print_process_tree(_In_ const wchar_t* root_process_name)
{
	_ASSERTE(NULL != root_process_name);
	if (NULL == root_process_name) { return; }

	DWORD pid = find_process(root_process_name);
	if (0 != pid)
	{
		print_process_tree(pid);
	}
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
bool cprocess_tree::kill_process(_In_ DWORD pid)
{
	if (pid == 0 || pid == 4) return false;

	process_map::iterator it = _proc_map.find(pid);
	if (it == _proc_map.end()) return true;
	process prcs = it->second;

	return prcs.kill(0);
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
bool cprocess_tree::kill_process(_In_ const wchar_t* process_name)
{
	_ASSERTE(NULL != process_name);
	if (NULL == process_name) return false;
	
	DWORD pid = find_process(process_name);
	return kill_process(pid);
}

/**
 * @brief	root_pid �� �� �ڽ� ���μ����� ��� �����Ѵ�. 
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
bool cprocess_tree::kill_process_tree(_In_ DWORD root_pid)
{
	if (root_pid == 0 || root_pid == 4) return false;

	process_map::iterator it = _proc_map.find(root_pid);
	if (it == _proc_map.end()) return true;
	process root = it->second;

	// check process is already killed.
	if (true == root.killed()) 
	{
		log_info "already killed. pid = %u, %ws", root.pid(), root.process_name().c_str() log_end
		return true;
	}

	// kill process tree include root.
	kill_process_tree(root);
	
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
void 
cprocess_tree::add_process(
	_In_ DWORD ppid, 
	_In_ DWORD pid, 
	_In_ FILETIME& creation_time, 
	_In_ const wchar_t* process_name
	)
{
	process p(process_name, ppid, pid, *(uint64_t*)&creation_time, false);
	_proc_map.insert( std::make_pair(pid, p) );
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
void cprocess_tree::print_process_tree(_In_ process& p, _In_ DWORD& depth)
{
	std::wstringstream prefix;
	for(DWORD i = 0; i < depth; ++i)
	{
		prefix << "    ";
	}

	log_info 
		"%spid = %u (ppid = %u), %ws ", prefix.str().c_str(), 
		p.pid(), 
		p.ppid(), 
		p.process_name().c_str() 
	log_end

	// p._pid �� ppid �� ���� item �� ã��
	process_map::iterator it = _proc_map.begin();
	process_map::iterator ite= _proc_map.end();
	for(; it != ite; ++it)
	{
		// ppid �� ���� ���������� ppid ���μ����� �̹� ����ǰ�, ���ο� ���μ����� �����ǰ�, ppid �� �Ҵ���� ��찡 
		// �߻��� �� �ִ�. ���� ppid ���� ������ ��� ppid �� ���� ���μ����� ���� �ð��� pid �� �����ð� ���� �� Ŀ�� �Ѵ�. 
		if ( it->second.ppid() == p.pid() && 
			(uint64_t)it->second.creation_time() > (uint64_t)p.creation_time())
		{
			print_process_tree(it->second, ++depth);
			--depth;
		}			
	}
}

/**
 * @brief	�ڽ��� �ڽ��� �ڽı���... ��������� ���̰�, ���� �״´�. :-)
 
			�߰��� ���� ��������� ���� ������ �ִ� ���μ����� ���� ���� �ִ�. 
			�����̴� ���� ���ΰ�, ���� �� �ִ� ���� �� ���̷���, ���ϰ��� void �� ����
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
void cprocess_tree::kill_process_tree(_In_ process& root)
{
	// terminate child processes first if exists.
	process_map::iterator its = _proc_map.begin();
	process_map::iterator ite= _proc_map.end();
	for(; its != ite; ++its)
	{
		// ppid �� ���� ���������� ppid ���μ����� �̹� ����ǰ�, ���ο� ���μ����� �����ǰ�, ppid �� �Ҵ���� ��찡 
		// �߻��� �� �ִ�. ���� ppid ���� ������ ��� ppid �� ���� ���μ����� ���� �ð��� pid �� �����ð� ���� �� Ŀ�� �Ѵ�. 
		if ( its->second.ppid() == root.pid() && 
			 its->second.creation_time() > root.creation_time())
		{
			kill_process_tree(its->second);
		}
	}

	// terminate parent process
	root.kill(0);
}


/**
 * @brief	process map �� ��ȸ�ϸ鼭 �ݹ��Լ��� ȣ���Ѵ�. 
			�ݹ��Լ��� false �� �����ϸ� ��ȸ�� ��� �����.
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
void cprocess_tree::iterate_process_tree(_In_ process& root, _In_ fnproc_tree_callback callback, _In_ DWORD_PTR callback_tag)
{
	// parent first
	if (true != callback(root, callback_tag)) return;

	// childs
	process_map::iterator its = _proc_map.begin();
	process_map::iterator ite= _proc_map.end();
	for(; its != ite; ++its)
	{
		// ppid �� ���� ���������� ppid ���μ����� �̹� ����ǰ�, ���ο� ���μ����� �����ǰ�, ppid �� �Ҵ���� ��찡 
		// �߻��� �� �ִ�. ���� ppid ���� ������ ��� ppid �� ���� ���μ����� ���� �ð��� pid �� �����ð� ���� �� Ŀ�� �Ѵ�. 
		if ( its->second.ppid() == root.pid() && 
			 its->second.creation_time() > root.creation_time())
		{
			iterate_process_tree(its->second, callback, callback_tag);
		}
	}
}
