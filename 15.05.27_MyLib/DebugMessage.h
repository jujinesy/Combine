/*-----------------------------------------------------------------------------
 * DebugMessage.h
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by somma (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * - 06.06.2010 created
**---------------------------------------------------------------------------*/

#ifndef _debug_message
#define _debug_message

#if defined(_slogger_included)

#error DebugMessage.h �� slogger �� ���ÿ� ����� �� �����ϴ�. slogger �� ����ϼ���.

#else

#pragma message("[WARNING] "__FILE__" �� ���̻� �������� �ʽ��ϴ�. �����ϸ� _slogger ����� ����ϼ���.")
#define DBG_OPN     DebugMessage(__FUNCTION__, __LINE__,
#define DBG_END     );
#define	DBG_ERR		DebugMessage(__FUNCTION__, __LINE__, "[ERR ]", 
#define DBG_WARN	DebugMessage(__FUNCTION__, __LINE__, "[WARN]",
#define DBG_INFO	DebugMessage(__FUNCTION__, __LINE__, "[INFO]",

#endif//DBG_XXX

void 
DebugMessage(
    const char* Function,
    int Line,
    const char* LogPrefix,    
    const char* fmt, 
    ...
    );


#endif//_debug_message