/**----------------------------------------------------------------------------
 * ResourceHelper.h
 *-----------------------------------------------------------------------------
 * �ڿ� �ڵ� ������ ���� ��� ����
 *-----------------------------------------------------------------------------
 * All rights reserved by somma (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * 22:9:2011   11:17 created
**---------------------------------------------------------------------------*/
#pragma once

#include "Win32Utils.h"

// 2012.12.20
// ���̻� ������� ����, shared_ptr ������ �������...�ϳ��� �ϳ��� ������
//

// ���� ������ ���̽� Ŭ����
//  - member �� ��� ���� �Ҹ��ڸ� ���� �ʿ䰡 ����.
//
class UnCopyable
{
protected:
	UnCopyable(){};
	~UnCopyable(){};
private:
	UnCopyable(const UnCopyable& rhs);
	UnCopyable& operator= (const UnCopyable& rhs);
};

// handle wrapper
//
class SmrtHandle: public UnCopyable
{
public:
	SmrtHandle(HANDLE handle) : m_handle(handle) {}
	~SmrtHandle(){ CloseHandle(m_handle); m_handle = INVALID_HANDLE_VALUE; }
private:
	HANDLE m_handle;
};

class SmrtObject: public UnCopyable
{
public:
	SmrtObject(PVOID object) : m_object(object) {}
	~SmrtObject(){ if (NULL != m_object) delete m_object; m_object = NULL;}
protected:
private:
	PVOID m_object;
};

// simple smart pointer
//
template <typename T>
class SmrtPtr: public UnCopyable
{
public:
	SmrtPtr(T& Ptr): m_ptr(Ptr) {};
	~SmrtPtr(){ free_and_nil(m_ptr);};
private:
	T& m_ptr;
};

class SmrtView: public UnCopyable
{
public:
	SmrtView(LPVOID hView) : m_view(hView) {}
	~SmrtView(){ UnmapViewOfFile(m_view); m_view = NULL;}
private:
	LPVOID m_view;
};
//
//class SmrtSvchandle: public UnCopyable
//{
//public:
//	SmrtSvchandle(SC_HANDLE h) : m_h(h) {}
//	~SmrtSvchandle(){ CloseServiceHandle(m_h); m_h=NULL;}
//private:
//	SC_HANDLE m_h;
//};
