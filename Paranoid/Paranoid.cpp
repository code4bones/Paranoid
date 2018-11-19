
// Paranoid.cpp : Определяет поведение классов для приложения.
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


// создание CParanoidApp

CParanoidApp::CParanoidApp()
  :m_nExitCode(0)
{
	// TODO: добавьте код создания,
	// Размещает весь важный код инициализации в InitInstance
}


// Единственный объект CParanoidApp

CParanoidApp theApp;


// инициализация CParanoidApp

BOOL CParanoidApp::InitInstance()
{
	// InitCommonControlsEx() требуется для Windows XP, если манифест
	// приложения использует ComCtl32.dll версии 6 или более поздней версии для включения
	// стилей отображения. В противном случае будет возникать сбой при создании любого окна.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Выберите этот параметр для включения всех общих классов управления, которые необходимо использовать
	// в вашем приложении.
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
		  CMsgBox(bRemove ? _T("Удаление") : _T("Настройки")).
			msg(_T("Программа не установленна...")).error();
		}
		m_nExitCode = -1;
		return FALSE;
	  }
	}
	
	//
	CParanoidDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	// Поскольку диалоговое окно закрыто, возвратите значение FALSE, чтобы можно было выйти из
	//  приложения вместо запуска генератора сообщений приложения.
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
  HWND hWnd = FindWindow(_T("#32770"), _T("Напоминалка"));
  return hWnd;
}


int CParanoidApp::ExitInstance()
{
  CWinApp::ExitInstance();
  // TODO: Add your specialized code here and/or call the base class

  return m_nExitCode;
}
