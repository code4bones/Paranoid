
// ParanoidDlg.h : файл заголовка
//

#pragma once

typedef struct Message {
  CString message;
  int timeout;
  Message(const rjson::Object &item) {
	std::string tmp = (std::string)item.at("message");
	message = utf8_decode(tmp).c_str();
	timeout = item.at("timeout");
  }
} Message;

#include <vector>
typedef std::vector<Message*> VMessage;
typedef VMessage::iterator VMessageI;


// диалоговое окно CParanoidDlg
class CParanoidDlg : public CDialogEx
{
// Создание
public:
	CParanoidDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_PARANOID_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;
	BOOL  m_bSilent;
	BOOL  m_bHidden;
	VMessage m_messages;
	VMessageI m_iMessage;
	CMenuTrayIcon m_tray;
	CString m_strTitle;
	BOOL m_bVisible;
	int m_nTimeout;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnRemoveApp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowApp(WPARAM wParam, LPARAM lParam);
public:
  afx_msg BOOL OnQueryEndSession();
  BOOL m_bAutoRun;
  afx_msg void OnBnClickedOk();
  BOOL ReadConfig();
  afx_msg void OnContextmenuShowapp();
  afx_msg void OnContextmenuExitapp();
  afx_msg void OnUpdateContextmenuShowapp(CCmdUI *pCmdUI);
  BOOL SetAutoRun(BOOL fSet);
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
};


static const UINT WM_REMOVE_APP = ::RegisterWindowMessage(_T("RemoveThisApp"));
static const UINT WM_SHOW_APP = ::RegisterWindowMessage(_T("ShowThisApp"));
