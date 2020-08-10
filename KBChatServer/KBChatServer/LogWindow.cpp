// LogWindow.cpp: 구현 파일
//

#include "stdafx.h"
#include "KBChatServer.h"
#include "LogWindow.h"
#include "afxdialogex.h"
#include "CApiAgent.h"
#define TIMER_API_COUNT 1000
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
	ON_WM_TIMER()
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
	if(m_lbLogBox.GetCount() > 500)
		m_lbLogBox.ResetContent();
	CString strTime;
	CTime t = CTime::GetCurrentTime();
	strTime.Format(_T("[%04d-%02d-%02d %02d:%02d:%02d]"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	CString strLogMsg;
	strLogMsg.Format(_T("%s %s %s"), strTime, GetLogTypeStr(typeLog), strMsg);
	m_lbLogBox.SetCurSel(m_lbLogBox.AddString(strLogMsg));
	m_lbLogBox.Invalidate();

}

BOOL LogWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetTimer(TIMER_API_COUNT, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void LogWindow::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == TIMER_API_COUNT)
	{
		CString sCount;
		sCount.Format(_T("%d"), g_pApiAgentDlg->m_listWebApiCall.GetCount());
		SetDlgItemTextW(IDC_STATIC_API_COUNT, sCount);
		((CWnd*)GetDlgItem(IDC_STATIC_API_COUNT))->Invalidate();
	}
	CDialogEx::OnTimer(nIDEvent);
}
