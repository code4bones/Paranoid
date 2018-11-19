
// ParanoidDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "Paranoid.h"
#include "ParanoidDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// диалоговое окно CParanoidDlg

#define KEY _T("Software\\Microsoft\\Windows\\CurrentVersion")
#define SECTION _T("Run")
#define APP _T("Paranoid.exe")
#define MESSAGE_TIMER 0x1221



CParanoidDlg::CParanoidDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CParanoidDlg::IDD, pParent)
  , m_bAutoRun(TRUE)
  , m_bVisible(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CParanoidDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Check(pDX, IDC_AUTORUN, m_bAutoRun);
}

BEGIN_MESSAGE_MAP(CParanoidDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_REGISTERED_MESSAGE(WM_REMOVE_APP, &CParanoidDlg::OnRemoveApp)
  ON_REGISTERED_MESSAGE(WM_SHOW_APP, &CParanoidDlg::OnShowApp)
  ON_WM_QUERYENDSESSION()
  ON_BN_CLICKED(IDOK, &CParanoidDlg::OnBnClickedOk)
  ON_COMMAND(ID_CONTEXTMENU_SHOWAPP, &CParanoidDlg::OnContextmenuShowapp)
  ON_COMMAND(ID_CONTEXTMENU_EXITAPP, &CParanoidDlg::OnContextmenuExitapp)
  ON_UPDATE_COMMAND_UI(ID_CONTEXTMENU_SHOWAPP, &CParanoidDlg::OnUpdateContextmenuShowapp)
  ON_WM_SYSCOMMAND()
  ON_WM_WINDOWPOSCHANGING()
  ON_WM_TIMER()
END_MESSAGE_MAP()


// обработчики сообщений CParanoidDlg

BOOL CParanoidDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна. Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	//m_strTitle.LoadString(IDS_TITLE);

	if (!ReadConfig()) {
	  CDialog::OnOK();
	}

	// from json config
	SetWindowText(m_strTitle);

	if (!m_bHidden)
	  m_tray.Create(m_strTitle, AfxGetApp()->LoadIcon(IDR_MAINFRAME), IDR_TRAY_MENU, ID_CONTEXTMENU_SHOWAPP, FALSE);

	SetAutoRun(TRUE);
	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}



afx_msg LRESULT CParanoidDlg::OnRemoveApp(WPARAM wParam, LPARAM lParam)
{
  SetAutoRun(FALSE); 
  if (!wParam) {
	CMsgBox(m_strTitle).msg(_T("Приложение деактивированно")).warn();
  }
  CDialog::OnOK();
  return 0;
}


afx_msg LRESULT CParanoidDlg::OnShowApp(WPARAM wParam, LPARAM lParam)
{
  m_bVisible = TRUE;
  ShowWindow(SW_RESTORE);
  return 0;
}


BOOL CParanoidDlg::OnQueryEndSession()
{
  m_iMessage = m_messages.begin();
  CString str((*m_iMessage)->message);
  if (ShutdownBlockReasonCreate(AfxGetMainWnd()->m_hWnd, CStringW(str))) {
	m_nTimeout = (*m_iMessage)->timeout;
	SetTimer(MESSAGE_TIMER, 1000, 0);
  }
  return FALSE;
}

void CParanoidDlg::OnTimer(UINT_PTR nIDEvent)
{
  if (nIDEvent == MESSAGE_TIMER) {
	if (m_nTimeout <= 0) {
	  m_iMessage++;
	  if (m_iMessage == m_messages.end()) {
		ShutdownBlockReasonDestroy(AfxGetMainWnd()->m_hWnd);
		KillTimer(0x100);
		CDialog::OnOK();
		return;
	  }

	  m_nTimeout = (*m_iMessage)->timeout;
	  CStringW str((*m_iMessage)->message);
	  ShutdownBlockReasonCreate(AfxGetMainWnd()->m_hWnd, str);
	}
	else
	  m_nTimeout--;

  }
  CDialogEx::OnTimer(nIDEvent);
}


void CParanoidDlg::OnBnClickedOk()
{
  UpdateData();
  CRegistry reg(_T("SOFTWARE\\Code4Bones\\Paranoid"));
  if (!reg.WriteInt(_T("CFG"), _T("m_bAutoRun"), (DWORD&)m_bAutoRun)) {
	CMsgBox(_T("Ошибка сохранения данных"), MB_ICONERROR | MB_OK);
  }
  if (!m_bAutoRun) {
	if (CMsgBox(m_strTitle).msg(_T("Автозапуск отменен,приложение будет закрыто")).yes()) {
	  SetAutoRun(m_bAutoRun);
	  CDialog::OnOK();
	}
  }
  else {
	m_bVisible = FALSE;
	SetAutoRun(m_bAutoRun);
	ShowWindow(SW_HIDE);
  }
}




void CParanoidDlg::OnContextmenuShowapp()
{
  OnShowApp(0,0);
}


void CParanoidDlg::OnContextmenuExitapp()
{
  CDialog::OnOK();
}


void CParanoidDlg::OnUpdateContextmenuShowapp(CCmdUI *pCmdUI)
{
 // pCmdUI->Enable(FALSE);
 // pCmdUI->m_pMenu->EnableMenuItem(ID_CONTEXTMENU_SHOWAPP, BY_POS);
}


BOOL CParanoidDlg::SetAutoRun(BOOL fSet)
{
  CString strPath;

  strPath.Format(_T("\"%s\\%s\""), get_exe_path(), APP);
  if (m_bHidden)
	strPath.Append(_T("--hidden"));

  CRegistry reg(KEY);
  if (fSet) {
	LPTSTR pApp = 0;
	if (!reg.ReadString(SECTION, APP, pApp)) {
	  int res = IDYES;
	  if (!m_bSilent)
		res = CMsgBox(_T("%s - Первый запуск"), m_strTitle)
		.msg(_T("Ключ автозапуска не найден,\nиспользовать текущее расположение ?\n%s"), strPath).ask(MB_ICONQUESTION | MB_YESNO);
	  if (res == IDYES) {
		reg.WriteString(SECTION, APP, strPath);
	  }
	  else {
		CMsgBox(_T("%s - Первый запуск"), m_strTitle)
		  .msg(_T("Приложение будет закрыто, скопируйте файл в нужную директорию и запустите заново"))
		  .info();
		CDialog::OnOK();
	  }
	}
	else { // AutoRun key found
	  
	  //UpdateData(FALSE);
	}
  }
  else {
	reg.RemoveValue(SECTION, APP);
  }
  return 0;
}


BOOL CParanoidDlg::ReadConfig()
{
  try {
#ifdef _UNICODE
	std::wstring confName = get_exe_path();
#else
	std::string confName = get_exe_path();
#endif
	confName += _T("\\Paranoid.conf");
	std::ifstream ifs(confName);
	rjson::Value init;
	ifs >> init;

	rjson::Object json = init;

	m_bSilent = theApp.GetOpt(_T("silent"));
	m_bHidden = (bool)json.at("hidden");

	std::string tmp = (std::string)json.at("title");
	m_strTitle = utf8_decode(tmp).c_str();
	const rjson::Array &jsMessages = json["messages"];
	if (!jsMessages.size())
	  throw_runtime("Нет сообщений !");
	for (rjson::Array::const_iterator ival = jsMessages.begin(); ival != jsMessages.end(); ival++) {
	  const rjson::Object &item = *ival;
	  Message *pMsg = new Message(item);
	  m_messages.push_back(pMsg);
	}
  }
  catch (std::exception &e) {
	CMsgBox("Ошибка конфигурации").msg(e.what()).error();
	return FALSE;
  }
  return TRUE;
}


// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок. Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CParanoidDlg::OnPaint()
{
  if (IsIconic())
  {
	CPaintDC dc(this); // контекст устройства для рисования

	SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

	// Выравнивание значка по центру клиентского прямоугольника
	int cxIcon = GetSystemMetrics(SM_CXICON);
	int cyIcon = GetSystemMetrics(SM_CYICON);
	CRect rect;
	GetClientRect(&rect);
	int x = (rect.Width() - cxIcon + 1) / 2;
	int y = (rect.Height() - cyIcon + 1) / 2;

	// Нарисуйте значок
	dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
	CDialogEx::OnPaint();
  }
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CParanoidDlg::OnQueryDragIcon()
{
  return static_cast<HCURSOR>(m_hIcon);
}



void CParanoidDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if ((nID & 0xFFF0) == SC_CLOSE) {
	ShowWindow(SW_HIDE);
  }
  else
  {
	CDialogEx::OnSysCommand(nID, lParam);
  }
}


void CParanoidDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
  CDialogEx::OnWindowPosChanging(lpwndpos);
  if ((lpwndpos->flags & SWP_SHOWWINDOW) == SWP_SHOWWINDOW) {
	if (!m_bVisible )
	  lpwndpos->flags &= ~SWP_SHOWWINDOW;
  }

  // TODO: Add your message handler code here
}


