
// Paranoid.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CParanoidApp:
// � ���������� ������� ������ ��. Paranoid.cpp
//

class CParanoidApp : public CWinApp
{
public:
	CParanoidApp();

// ���������������
public:
	virtual BOOL InitInstance();
	BOOL GetOpt(LPTSTR pszOpt);
	int m_nExitCode;
// ����������

	DECLARE_MESSAGE_MAP()
	HWND FindInstance();
	virtual int ExitInstance();
};

extern CParanoidApp theApp;