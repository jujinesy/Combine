/*-----------------------------------------------------------------------------
 * Trampoline.cpp
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by somma (fixbrain@gmail.com, unsorted@msn.com)
**---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "TinyDetour.h"
#include "Trampoline.h"

/**	---------------------------------------------------------------------------
	\brief	

	\param	
	\return			
	\code
	\endcode		
-----------------------------------------------------------------------------*/
//warning C4100: 'dwProcessId' : �������� ���� ���� �Ű� �����Դϴ�.
#pragma warning(disable: 4100)

__declspec(naked) 
int  
__stdcall 
MessageBoxA_Trampoline(
	HWND hWnd,
	LPCSTR lpText,
	LPCSTR lpCaption,
	UINT uType
	)
{
	__asm

	{
		// ���� opcode �� �����ϱ� ���� ���� (12 byte)
		nop
		nop
		nop
		nop		
		nop
		nop
		nop
		nop				
		nop
		nop
		nop
		nop				
		
		
		_emit	0xCC
		_emit	0xCC
		_emit	0xBB
		_emit	0xCC
		_emit	0xDD
		_emit	0xCC
		_emit	0xCC
		_emit	0xCC
		_emit	0xCC
	}
}
#pragma warning(default: 4100)

/**	---------------------------------------------------------------------------
	\brief	

	\param	
	\return			
	\code
	\endcode		
-----------------------------------------------------------------------------*/
int 
__stdcall 
MessageBoxA_Detour(
	HWND hWnd,
    LPCSTR lpText,
    LPCSTR lpCaption,
	UINT uType
	)
{
	return MessageBoxA_Trampoline(hWnd, lpText, "___this_is_hooked_caption___", 0);
}





