/*-----------------------------------------------------------------------------
 * LeakWatcher.h
 *-----------------------------------------------------------------------------
 * from http://serious-code.net/moin.cgi/MemoryLeakDetection
 *

 �˻��ϰ��� �ϴ� ��� cpp ������ ������ include �� �Ʒ��� �Ʒ��� 
 �ڵ带 �߰��Ѵ�. 
	
	#include "LeakWatcher.h"

	#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#endif

 �ʿ��� �� _CrtDumpMemoryLeaks()  �Լ��� ȣ���Ѵ�.

 �޸� ���� ����Ǵ� ��� �Ʒ� �Լ��� ȣ���� �θ� �ڵ����� bp �� �ɸ�
 _CrtSetBreakAlloc(45) 

 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by Noh Yong Hwan (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * Revision History:
 * Date					Who					What
 * ----------------		----------------	----------------
 * 22.10.2007			Noh Yong Hwan		birth
**---------------------------------------------------------------------------*/

#ifndef IMWATCHINGYOULEAK
#define IMWATCHINGYOULEAK

#include <crtdbg.h>

#ifdef _DEBUG
#define THIS_FILE __FILE__

inline void* __cdecl operator new(size_t size, const char* filename, int line)
{
    return ::operator new(size, 1, filename, line);
}

inline void __cdecl operator delete(void* ptr, const char *filename, int line)
{
    ::operator delete(ptr, _NORMAL_BLOCK, filename, line);
}

#define DEBUG_NEW new(THIS_FILE, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, THIS_FILE, __LINE__)
#define calloc(c, s) _calloc_dbg(c, s, _NORMAL_BLOCK, THIS_FILE, __LINE__)
#define realloc(p, s) _realloc_dbg(p, s, _NORMAL_BLOCK, THIS_FILE, __LINE__)
#define _expand(p, s) _expand_dbg(p, s, _NORMAL_BLOCK, THIS_FILE, __LINE__)
#define free(p) _free_dbg(p, _NORMAL_BLOCK)
#define _msize(p) _msize_dbg(p, _NORMAL_BLOCK)

#endif // _DEBUG

#endif // IMWATCHINGYOULEAK