
// iat_hook_sample.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// Ciat_hook_sampleApp:
// �� Ŭ������ ������ ���ؼ��� iat_hook_sample.cpp�� �����Ͻʽÿ�.
//

class Ciat_hook_sampleApp : public CWinApp
{
public:
	Ciat_hook_sampleApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern Ciat_hook_sampleApp theApp;