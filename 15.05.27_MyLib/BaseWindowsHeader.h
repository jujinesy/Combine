/**----------------------------------------------------------------------------
 * BaseWindowsHeader.h
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by somma (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * 26:12:2011   17:53 created
**---------------------------------------------------------------------------*/
#pragma once

// std::string �� strsafe.h ���� ���� ��������� ������ ����
// 
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <sal.h>

#ifdef _DEBUG
#include <crtdbg.h>
#endif//_DEBUG