#pragma once


// CApiAgent 대화 상자

class CApiAgent : public CDialogEx
{
	DECLARE_DYNAMIC(CApiAgent)

public:
	CApiAgent(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CApiAgent();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_APIAGENT_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CPtrList m_listWebApiCall;
	void AddAPI(APICALLDATA* pParamMap);
	HANDLE m_hThWebApiEvent;
	unsigned m_idThrWebApiEvent;
	static unsigned __stdcall ThreadWebApiCall(void* pParam);
	void call_API(int cmd, CString sApiPath, CString sFormData,CString sMethod);
	CString GetHttpQuery(CString strIP, int nPort, CString strUrl, CString strParams, CString method);
	void parse_ReturnData(int cmd, Json::Value root);
	BOOL makeJsonData(APICALLDATA* pParamMap);
	CString makeGetParam(APICALLDATA* pParamMap);
	CString makePostParam(APICALLDATA* pParamMap);
	void parse_Check_Config(Json::Value root);
	void DownloadFileFromURL(APICALLDATA* pData, CString sServer, int nPort);
};

extern CApiAgent* g_pApiAgentDlg;