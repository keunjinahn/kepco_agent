// LogWindow.cpp: 구현 파일
//

#include "stdafx.h"
#include "KBChatServer.h"
#include "LogWindow.h"
#include "afxdialogex.h"


// LogWindow 대화 상자

IMPLEMENT_DYNAMIC(LogWindow, CDialogEx)

LogWindow::LogWindow(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGWINDOW, pParent)
{

}

LogWindow::~LogWindow()
{
}

void LogWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOGBOX, m_lbLogBox);
}


BEGIN_MESSAGE_MAP(LogWindow, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOG_EXIT, &LogWindow::OnBnClickedBtnLogExit)
END_MESSAGE_MAP()


// LogWindow 메시지 처리기


void LogWindow::OnBnClickedBtnLogExit()
{
	ShowWindow(SW_HIDE);
}




CString LogWindow::GetLogTypeStr(LOGTYPE typeLog)
{
	switch (typeLog)
	{
	case LOG_CONNECT:  return _T("LOG_CONNECT");
	case LOG_LOGIN:  return _T("LOG_LOGIN");
	case LOG_LEAVE:  return _T("LOG_LEAVE");
	case LOG_SOCKCLOSE:  return _T("LOG_SOCKCLOSE");
	case LOG_CHANNELMAKE:  return _T("LOG_CHANNELMAKE");
	case LOG_JOIN:  return _T("LOG_JOIN");
	}
	return _T("UNKNOWN");
}

void LogWindow::LogMessage(LOGTYPE typeLog, CString strMsg)
{
	USES_CONVERSION;
	CString strTime;
	CTime t = CTime::GetCurrentTime();
	strTime.Format(_T("[%04d-%02d-%02d %02d:%02d:%02d]"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	CString strLogMsg;
	strLogMsg.Format(_T("%s %s %s"), strTime, GetLogTypeStr(typeLog), strMsg);
	m_lbLogBox.SetCurSel(m_lbLogBox.AddString(strLogMsg));
	m_lbLogBox.Invalidate();

}