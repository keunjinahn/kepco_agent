#pragma once


// LogWindow 대화 상자

class LogWindow : public CDialogEx
{
	DECLARE_DYNAMIC(LogWindow)

public:
	LogWindow(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~LogWindow();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGWINDOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLogExit();
	void LogMessage(LOGTYPE typeLog, CString strMsg);
	CListBox	m_lbLogBox;
	CString GetLogTypeStr(LOGTYPE typeLog);
};
