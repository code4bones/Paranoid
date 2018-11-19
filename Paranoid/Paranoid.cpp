
// Paranoid.cpp : ���������� ��������� ������� ��� ����������.
//

#include "stdafx.h"
#include "Paranoid.h"
#include "ParanoidDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

TCHAR **__wargv = 0;
int __argc = 0;

// CParanoidApp

BEGIN_MESSAGE_MAP(CParanoidApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// �������� CParanoidApp

CParanoidApp::CParanoidApp()
  :m_nExitCode(0)
{
	// TODO: �������� ��� ��������,
	// ��������� ���� ������ ��� ������������� � InitInstance
}


// ������������ ������ CParanoidApp

CParanoidApp theApp;


// ������������� CParanoidApp

BOOL CParanoidApp::InitInstance()
{
	// InitCommonControlsEx() ��������� ��� Windows XP, ���� ��������
	// ���������� ���������� ComCtl32.dll ������ 6 ��� ����� ������� ������ ��� ���������
	// ������ �����������. � ��������� ������ ����� ��������� ���� ��� �������� ������ ����.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// �������� ���� �������� ��� ��������� ���� ����� ������� ����������, ������� ���������� ������������
	// � ����� ����������.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	BOOL bRemove = GetOpt(_T("remove"));
	BOOL bShow = GetOpt(_T("show"));
	BOOL bSilent = GetOpt(_T("silent"));

	HWND hWnd = FindInstance();
	if (hWnd != NULL) {
	  if (bRemove) 
		m_nExitCode = ::SendMessage(hWnd, WM_REMOVE_APP, bSilent, 0);
	  else if (bShow) {
		m_nExitCode = ::SendMessage(hWnd, WM_SHOW_APP, 0, 0);
		::SetForegroundWindow(hWnd);
	  }
	  return FALSE;
	}
	else {
	  if (bRemove || bShow) {
		if (!bSilent) {
		  CMsgBox(bRemove ? _T("��������") : _T("���������")).
			msg(_T("��������� �� ������������...")).error();
		}
		m_nExitCode = -1;
		return FALSE;
	  }
	}
	
	//
	CParanoidDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	// ��������� ���������� ���� �������, ���������� �������� FALSE, ����� ����� ���� ����� ��
	//  ���������� ������ ������� ���������� ��������� ����������.
	return FALSE;
}



BOOL CParanoidApp::GetOpt(LPTSTR pszOpt)
{
  LPTSTR pCmdLine = GetCommandLine();
#ifdef _UNICODE
  if (wcsstr(pCmdLine, pszOpt) != 0) {
	return TRUE;
  }
#else
  if (strstr(pCmdLine, pszOpt) != 0) {
	return TRUE;
  }
#endif
  return FALSE;
}


HWND CParanoidApp::FindInstance()
{
  HWND hWnd = FindWindow(_T("#32770"), _T("�����������"));
  return hWnd;
}


int CParanoidApp::ExitInstance()
{
  CWinApp::ExitInstance();
  // TODO: Add your specialized code here and/or call the base class

  return m_nExitCode;
}
