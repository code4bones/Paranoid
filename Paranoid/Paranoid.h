
// Paranoid.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CParanoidApp:
// О реализации данного класса см. Paranoid.cpp
//

class CParanoidApp : public CWinApp
{
public:
	CParanoidApp();

// Переопределение
public:
	virtual BOOL InitInstance();
	BOOL GetOpt(LPTSTR pszOpt);
	int m_nExitCode;
// Реализация

	DECLARE_MESSAGE_MAP()
	HWND FindInstance();
	virtual int ExitInstance();
};

extern CParanoidApp theApp;