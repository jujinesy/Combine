
// iat_hook_sampleDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"


// Ciat_hook_sampleDlg ��ȭ ����
class Ciat_hook_sampleDlg : public CDialogEx
{
// �����Դϴ�.
public:
	Ciat_hook_sampleDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_IAT_HOOK_SAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CEdit m_editbox;
	afx_msg void OnBnClickedButton1();
};
