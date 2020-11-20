
// Sensor_AgentDlg.h: 헤더 파일
//

#pragma once

#include "HostWnd.h"
#include "SystemTray.h"
// CSensorAgentDlg 대화 상자
class CSensorAgentDlg : public CDialogEx
{
// 생성입니다.
public:
	CSensorAgentDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENSOR_AGENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public :
	CString GetProgramPathW();
	void LoadConfig();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnApply();
	CPtrList m_listShop;
	CShopInfo* m_pCurShopInfo;
	CListCtrl m_listShopCtrl;
	afx_msg void OnLvnGetdispinfoListShop(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickListShop(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnColumnclickListShop(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHdnItemclickListShop(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnGetdispinfoListShopSend(NMHDR* pNMHDR, LRESULT* pResult);
	CListCtrl m_listShopSendCtrl;
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDll();
	void SendShopData();
	BOOL IsExistShopSendList(CString mac);
	CHostWnd m_pHostWnd[SOCKET_COUNT];
	afx_msg void OnBnClickedBtnSendStart();
	afx_msg void OnBnClickedBtnSendStop();
	void LogMessage(LOGTYPE typeLog, CString strMsg);
	CListBox	m_lbLogBox;
	CString GetLogTypeStr(LOGTYPE typeLog);
	void StartCollectionServer();
	BOOL IsRunningProcess(const TCHAR* processname, TCHAR* param);
	CSystemTray m_TrayIcon;
	afx_msg void OnMenuTrayExit();
	afx_msg void OnMenuTrayShow();
	afx_msg void OnMenuTrayHide();
	BOOL getStateProcess(CString name);
	BOOL isSetupProgramInstall();
	float m_dwFrequencyIndex;
	int m_nProfile;
	void StartEventManager();
	void SetAutoStart(BOOL bStart);
	AGENT_TYPE m_typeAgent;
	void LoadAgentConfig();
	void StartResManager();
	DWORD m_dwServiceCheckCount;
	void RestartMySQLService();
};

extern CSensorAgentDlg* g_pMainDlg;