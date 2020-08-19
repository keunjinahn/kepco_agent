
// KBChatServerDlg.h : 헤더 파일
//

#pragma once
#include "mysql.h"
#include "mariadb_version.h"
#include "BsButton.h"
#include "LogWindow.h"
#include "CApiAgent.h"
#include "SystemTray.h"
// CKBChatServerDlg 대화 상자
class CKBChatServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CKBChatServerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_KBCHATSERVER_DIALOG };

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
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void LogMessage(LOGTYPE typeLog,CString strMsg);

	afx_msg void OnDestroy();
	void OnBnClickedBtnExit();
	MYSQL m_Mysql;
	MYSQL_RES* m_Res;
	MYSQL_ROW m_Row;
	void SensorDataInsert(KBPKT_DATA_OBJ* pObj);
	CxImage m_imgBgMain;
	CBsButton m_btnProfile,m_btnLog,m_btnOpcua;
	void InitControl();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnBsButtonMsg(WPARAM wParam, LPARAM lParam);
	CString GetProgramPathW();
	void LoadConfig();
	CConfigInfo m_ConfigInfo;
	CFont m_fontLocation, m_fontShop,m_fontSensorValue, m_fontLeftShopName, m_fontSensorValue_detail;
	LogWindow* m_pLogWindow;
	void ShowLoginDlg();
	CShopInfo* GetShopInfo(CString srcMac);
	CShopInfo* m_pCurShopInfo;
	CFont* DrawSensorInfo(CMemDC_* pDC);
	void CheckValueDC(int nValue, SENSORTYPE type, CMemDC_* pDC, CRect* pRect);
	CRect m_rectSensor[15];
	CFont* DrawItemList(CDC* pDC, CPtrList* pItemList);
	CBrush m_blushActive, m_blushInActive;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CApiAgent* m_pAPIAgentDlg;
	afx_msg void OnBnClickedBtnLog();
	void SendSensorData();
	void LoadOpcUaData();
	void SendCheckConfig();
	CHECKRESULT CheckIncidentDevice(CStringList* pValueList, SENSORTYPE type, float* returValue);
	void CheckIncident();
	CSystemTray m_TrayIcon;
	afx_msg void OnMenuTrayExit();
	afx_msg void OnMenuTrayLog();
	void UpdateDownloadCompleteToIni(int nResult);
	CShopInfo* IsNotAllocShop(CString srcMac);
	BOOL m_bWasServerConnected;
	CString GetIpAddress();
	CPtrList m_listArea;
	CAreaInfo* GetAreaInfo(CString srcMac);
	void CheckResource();
	void Init_Cpu();
	double getCpuCurrentValue();
	void getRAMStatus(int& availableMem, int& physicalMem);
};

extern CKBChatServerDlg *g_pServerDlg;