
// Sensor_AgentDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "framework.h"
#include "Sensor_Agent.h"
#include "Sensor_AgentDlg.h"
#include "afxdialogex.h"
#include "HostWnd.h"
#include <tlhelp32.h>
#include <NTSecAPI.h>
#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_SEND_INTERVAL 1000
#define TIMER_KCOLLECTION_CHECK 1
#define TIMER_START_EVENT_MANAGER 2
#define TIMER_START_RES_MANAGER 3


DWORD	g_platform_id;
DWORD	g_version_major;
DWORD	g_version_minor;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSensorAgentDlg 대화 상자

CSensorAgentDlg* g_pMainDlg = NULL;

CSensorAgentDlg::CSensorAgentDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SENSOR_AGENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_pCurShopInfo = NULL;
	m_dwFrequencyIndex = 0.0;
	m_nProfile = 0;
	m_typeAgent = AGENT_SENSOR;
}

void CSensorAgentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SHOP, m_listShopCtrl);
	DDX_Control(pDX, IDC_LIST_SHOP_SEND, m_listShopSendCtrl);
	DDX_Control(pDX, IDC_LIST_LOG, m_lbLogBox);
}

BEGIN_MESSAGE_MAP(CSensorAgentDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_APPLY, &CSensorAgentDlg::OnBnClickedBtnApply)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_SHOP, &CSensorAgentDlg::OnLvnGetdispinfoListShop)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SHOP, &CSensorAgentDlg::OnNMClickListShop)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_SHOP, &CSensorAgentDlg::OnLvnColumnclickListShop)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CSensorAgentDlg::OnHdnItemclickListShop)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_SHOP_SEND, &CSensorAgentDlg::OnLvnGetdispinfoListShopSend)
	ON_BN_CLICKED(IDC_BTN_ADD, &CSensorAgentDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DLL, &CSensorAgentDlg::OnBnClickedBtnDll)
	ON_BN_CLICKED(IDC_BTN_SEND_START, &CSensorAgentDlg::OnBnClickedBtnSendStart)
	ON_BN_CLICKED(IDC_BTN_SEND_STOP, &CSensorAgentDlg::OnBnClickedBtnSendStop)
	ON_COMMAND(IDC_MENU_TRAY_EXIT, &CSensorAgentDlg::OnMenuTrayExit)
	ON_COMMAND(IDC_MENU_TRAY_VIEW_SHOW, &CSensorAgentDlg::OnMenuTrayShow)
	ON_COMMAND(IDC_MENU_TRAY_VIEW_HIDE, &CSensorAgentDlg::OnMenuTrayHide)
END_MESSAGE_MAP()


// CSensorAgentDlg 메시지 처리기

BOOL CSensorAgentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	g_pMainDlg = this;
	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	OSVERSIONINFO osversioninfo;
	osversioninfo.dwOSVersionInfoSize = sizeof(osversioninfo);

	// Get the current OS version
	if (!GetVersionEx(&osversioninfo))
		g_platform_id = 0;
	g_platform_id = osversioninfo.dwPlatformId;
	g_version_major = osversioninfo.dwMajorVersion;
	g_version_minor = osversioninfo.dwMinorVersion;


	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	CString strSockClassName = _T("");
	for (int i = 0; i < SOCKET_COUNT; i++)
	{
		strSockClassName.Format(_T("Sensor Agent SOCK %d"), i + 1);
		m_pHostWnd[i].CreateEx(0, AfxRegisterWndClass(NULL, 0, 0, 0), strSockClassName, 0, -100, -100, 10, 10, NULL, NULL);
	}

	m_listShopCtrl.InsertColumn(0, _T("지역명"), LVCFMT_LEFT, 120);
	m_listShopCtrl.InsertColumn(1, _T("mac"), LVCFMT_LEFT, 100);
	m_listShopCtrl.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_listShopCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_listShopSendCtrl.InsertColumn(0, _T("지역명"), LVCFMT_LEFT, 120);
	m_listShopSendCtrl.InsertColumn(1, _T("mac"), LVCFMT_LEFT, 100);
	m_listShopSendCtrl.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_listShopSendCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	CString strPort;
	strPort.Format(_T("%d"), PORT_S);
	SetDlgItemTextW(IDC_EDT_TIMER_VALUE, _T("1000"));
	SetDlgItemTextW(IDC_EDT_SERVER_IP, SERVER_IP_ADDRESS);
	SetDlgItemTextW(IDC_EDT_SERVER_PORT, strPort);
	SetDlgItemTextW(IDC_EDT_PROFILE, _T("0"));
	
	//LoadConfig();
	LoadAgentConfig();

	((CButton*)GetDlgItem(IDC_RADIO_TCP))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_OPCUA))->SetCheck(FALSE);

	HICON hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON1));  // Icon to use
	if (!m_TrayIcon.Create(
		NULL,                            // Let icon deal with its own messages
		WM_ICON_NOTIFY,                  // Icon notify message to use
		_T(""),  // tooltip
		hIcon,
		IDR_MENU_SYSTEM_TRAY,                  // ID of tray icon
		FALSE,
		_T(""), // balloon tip
		_T(""),               // balloon title
		NIIF_WARNING,                    // balloon icon
		20))                            // balloon timeout
	{
		return FALSE;
	}
	m_TrayIcon.SetMenuDefaultItem(0, TRUE);

	
	UINT nStyle = 0;
	nStyle |= WS_MAXIMIZEBOX;
	nStyle |= WS_MINIMIZEBOX;
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	
	MoveWindow(0, 0, 0, 0);
	
	if (m_typeAgent == AGENT_SENSOR)
	{
		SetTimer(TIMER_KCOLLECTION_CHECK, 1000, NULL);
		SetTimer(TIMER_START_EVENT_MANAGER, 1000, NULL);
	}
	else
	{
		SetTimer(TIMER_START_RES_MANAGER, 1000, NULL);
	}
	SetAutoStart(TRUE);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSensorAgentDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSensorAgentDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSensorAgentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


CString CSensorAgentDlg::GetProgramPathW()
{
	USES_CONVERSION;
	TCHAR path_program[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];

	TCHAR strModulePath[_MAX_PATH];
	ZeroMemory(strModulePath, _MAX_PATH);
	::GetModuleFileNameW(::AfxGetApp()->m_hInstance, strModulePath, _MAX_PATH);
	_wsplitpath_s(strModulePath, drive, dir, fname, ext);
	_wmakepath_s(path_program, drive, dir, _T(""), _T(""));
	CString strPath = _T("");
	strPath.Format(_T("%s"), path_program);

	return strPath.Mid(0, strPath.GetLength() - 1);
}

void CSensorAgentDlg::LoadConfig()
{
	USES_CONVERSION;
	CString strConfig;
	strConfig.Format(_T("%s\\config.ini"), GetProgramPathW());
	CString strKey = _T("INFO");
	CString strValue;
	char sValue[1024];
	::GetPrivateProfileString(strKey, _T("agent_type"), _T("1"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
	m_typeAgent = (AGENT_TYPE)_wtoi(strValue);
}


void CSensorAgentDlg::LoadAgentConfig()
{

	USES_CONVERSION;
	CString strConfig;
	strConfig.Format(_T("%s\\config_agent.ini"), GetProgramPathW());
	CString strKey = _T("INFO");
	CString strValue;
	char sValue[1024];

	::GetPrivateProfileString(strKey, _T("agent_type"), _T("1"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
	m_typeAgent = (AGENT_TYPE)_wtoi(strValue);

	::GetPrivateProfileString(strKey, _T("count"), _T("6"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
	int nCount = _wtoi(strValue);
	CString sNum;
	CString sMac;
	CString locationname;
	for (int k = 0; k < nCount; k++)
	{
		strKey.Format(_T("INFO_%d"), k + 1);
		::GetPrivateProfileString(strKey, _T("locationname"), _T("111"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
		locationname.Format(_T("%s"), strValue);
		::GetPrivateProfileString(strKey, _T("shopcount"), _T("1"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
		int nShopCount = _wtoi(strValue);

		for (int i = 0; i < nShopCount; i++)
		{
			CShopInfo* pShopInfo = new CShopInfo();
			sNum.Format(_T("%02d"), i + 1);
			::GetPrivateProfileString(strKey, sNum, _T(""), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
			pShopInfo->strShopName.Format(_T("%s_%s"), locationname, strValue);
			sMac.Format(_T("%02d_mac"), i + 1);
			::GetPrivateProfileString(strKey, sMac, _T(""), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
			pShopInfo->m_objData.mac = strValue;
			m_listShop.AddTail(pShopInfo);
			if (i == 0)
				m_pCurShopInfo = pShopInfo;

			pShopInfo->m_objData.setData(pShopInfo->m_objData.mac, "01", "00", '0', '0', "25.1", "26.1", "27.5", "28.5", "30.5", "31.5", "32.5", "33.5");

			int nIndex = m_listShopCtrl.GetItemCount();
			m_listShopCtrl.InsertItem(LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE, m_listShopCtrl.GetItemCount(), LPSTR_TEXTCALLBACK, 0, 0, I_IMAGECALLBACK, (LPARAM)pShopInfo);

		}
	}
	m_listShopCtrl.UpdateWindow();
}

void CSensorAgentDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == TIMER_KCOLLECTION_CHECK)
	{
		KillTimer(TIMER_KCOLLECTION_CHECK);

		if (isSetupProgramInstall())
		{
			return;
		}
		//StartCollectionServer();
		SetTimer(TIMER_KCOLLECTION_CHECK,2000,NULL);
	}
	else if (nIDEvent == TIMER_START_EVENT_MANAGER)
	{
		KillTimer(TIMER_START_EVENT_MANAGER);

		StartEventManager();

		SetTimer(TIMER_START_EVENT_MANAGER, 30000, NULL);
	}
	else if (nIDEvent == TIMER_START_RES_MANAGER)
	{
		KillTimer(TIMER_START_RES_MANAGER);

		StartResManager();

		SetTimer(TIMER_START_RES_MANAGER, 10000, NULL);
	}
	else if (nIDEvent == TIMER_SEND_INTERVAL)
	{
		CTime t = CTime::GetCurrentTime();
		m_dwFrequencyIndex += 1;
		SendShopData();
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CSensorAgentDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	for (int i = 0; i < SOCKET_COUNT; i++)
	{
		m_pHostWnd[i].DestroyWindow();
	}
	while (!m_listShop.IsEmpty())
	{
		CShopInfo* pShopInfo = (CShopInfo*)m_listShop.RemoveHead();
		delete pShopInfo;
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CSensorAgentDlg::OnBnClickedBtnApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	USES_CONVERSION;
	CString strMenuItem;
	POSITION pos = m_listShopCtrl.GetFirstSelectedItemPosition();
	if (pos) {
		int iSel = m_listShopCtrl.GetNextSelectedItem(pos);
		CShopInfo* pShopInfo = (CShopInfo*)m_listShopCtrl.GetItemData(iSel);
		if (!pShopInfo)
			return;
		GetDlgItemTextW(IDC_EDT_MAC, pShopInfo->m_objData.mac);
		CString strValue;
		GetDlgItemTextW(IDC_EDT_SENSOR_STATE, strValue);
		pShopInfo->m_objData.sensor_state = _wtoi(strValue);
		GetDlgItemTextW(IDC_EDT_LEAK_1_VALUE, strValue);
		pShopInfo->m_objData.leak_1_value = _wtoi(strValue);
		GetDlgItemTextW(IDC_EDT_LEAK_2_VALUE, strValue);
		pShopInfo->m_objData.leak_2_value = _wtoi(strValue);

		GetDlgItemTextW(IDC_EDT_TEMP_1_VALUE, strValue);
		pShopInfo->m_objData.temp_1_value = _wtof(strValue);
		GetDlgItemTextW(IDC_EDT_TEMP_2_VALUE, strValue);
		pShopInfo->m_objData.temp_2_value = _wtof(strValue);
		GetDlgItemTextW(IDC_EDT_TEMP_3_VALUE, strValue);
		pShopInfo->m_objData.temp_3_value = _wtof(strValue);
		GetDlgItemTextW(IDC_EDT_TEMP_4_VALUE, strValue);
		pShopInfo->m_objData.temp_4_value = _wtof(strValue);

		GetDlgItemTextW(IDC_EDT_HUMI_1_VALUE, strValue);
		pShopInfo->m_objData.humi_1_value = _wtof(strValue);
		GetDlgItemTextW(IDC_EDT_HUMI_2_VALUE, strValue);
		pShopInfo->m_objData.humi_2_value = _wtof(strValue);
		GetDlgItemTextW(IDC_EDT_HUMI_3_VALUE, strValue);
		pShopInfo->m_objData.humi_3_value = _wtof(strValue);
		GetDlgItemTextW(IDC_EDT_HUMI_4_VALUE, strValue);
		pShopInfo->m_objData.humi_4_value = _wtof(strValue);
		GetDlgItemTextW(IDC_EDT_PROFILE, strValue);
		m_nProfile = _wtoi(strValue);
		
		SetDlgItemTextW(IDC_EDT_SEND_PACKET, pShopInfo->m_objData.getPacket());
	}
	
}


void CSensorAgentDlg::OnLvnGetdispinfoListShop(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	USES_CONVERSION;
	// TODO: Add your control notification handler code here
	CShopInfo* pShopInfo = (CShopInfo*)pDispInfo->item.lParam;
	if (!pShopInfo)
		return;
	switch (pDispInfo->item.iSubItem)
	{
	case 0:
	{
		wsprintf(pDispInfo->item.pszText, _T("%s"), pShopInfo->strShopName);
	}
	break;
	case 1:
	{
		wsprintf(pDispInfo->item.pszText, _T("%s"), pShopInfo->m_objData.mac);
	}
	break;
	}
	*pResult = 0;
}


void CSensorAgentDlg::OnNMClickListShop(NMHDR* pNMHDR, LRESULT* pResult)
{
	USES_CONVERSION;
	CString strMenuItem;
	POSITION pos = m_listShopCtrl.GetFirstSelectedItemPosition();
	if (pos) {
		int iSel = m_listShopCtrl.GetNextSelectedItem(pos);
		CShopInfo* pShopInfo = (CShopInfo*)m_listShopCtrl.GetItemData(iSel);
		if (!pShopInfo)
			return;
		SetDlgItemTextW(IDC_LB_LOCATION_NAME, pShopInfo->strShopName);
		CString sDeviceNo;
		sDeviceNo.Format(_T("%02d"), pShopInfo->m_objData.device_no);
		SetDlgItemTextW(IDC_LB_GATE_NUM, sDeviceNo);
		SetDlgItemTextW(IDC_EDT_MAC, pShopInfo->m_objData.mac);
		CString sSensorState;
		sSensorState.Format(_T("%02d"), pShopInfo->m_objData.sensor_state);
		SetDlgItemTextW(IDC_EDT_SENSOR_STATE, sSensorState);
		SetDlgItemInt(IDC_EDT_LEAK_1_VALUE, pShopInfo->m_objData.leak_1_value);
		SetDlgItemInt(IDC_EDT_LEAK_2_VALUE, pShopInfo->m_objData.leak_2_value);
		CString sValue;
		sValue.Format(_T("%.1f"), pShopInfo->m_objData.temp_1_value);
		SetDlgItemTextW(IDC_EDT_TEMP_1_VALUE, sValue);
		sValue.Format(_T("%.1f"), pShopInfo->m_objData.temp_2_value);
		SetDlgItemTextW(IDC_EDT_TEMP_2_VALUE, sValue);
		sValue.Format(_T("%.1f"), pShopInfo->m_objData.temp_3_value);
		SetDlgItemTextW(IDC_EDT_TEMP_3_VALUE, sValue);
		sValue.Format(_T("%.1f"), pShopInfo->m_objData.temp_4_value);
		SetDlgItemTextW(IDC_EDT_TEMP_4_VALUE, sValue);
		
		sValue.Format(_T("%.1f"), pShopInfo->m_objData.humi_1_value);
		SetDlgItemTextW(IDC_EDT_HUMI_1_VALUE, sValue);
		sValue.Format(_T("%.1f"), pShopInfo->m_objData.humi_2_value);
		SetDlgItemTextW(IDC_EDT_HUMI_2_VALUE, sValue);
		sValue.Format(_T("%.1f"), pShopInfo->m_objData.humi_3_value);
		SetDlgItemTextW(IDC_EDT_HUMI_3_VALUE, sValue);
		sValue.Format(_T("%.1f"), pShopInfo->m_objData.humi_4_value);
		SetDlgItemTextW(IDC_EDT_HUMI_4_VALUE, sValue);
		SetDlgItemTextW(IDC_EDT_SEND_PACKET, pShopInfo->m_objData.getPacket());
	}
	*pResult = 0;
}


void CSensorAgentDlg::OnLvnColumnclickListShop(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CSensorAgentDlg::OnHdnItemclickListShop(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	*pResult = 0;
}


void CSensorAgentDlg::OnLvnGetdispinfoListShopSend(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	USES_CONVERSION;
	// TODO: Add your control notification handler code here
	CShopInfo* pShopInfo = (CShopInfo*)pDispInfo->item.lParam;
	if (!pShopInfo)
		return;
	switch (pDispInfo->item.iSubItem)
	{
	case 0:
	{
		wsprintf(pDispInfo->item.pszText, _T("%s"), pShopInfo->strShopName);
	}
	break;
	case 1:
	{
		wsprintf(pDispInfo->item.pszText, _T("%s"), pShopInfo->m_objData.mac);
	}
	break;
	}
	*pResult = 0;
}


void CSensorAgentDlg::OnBnClickedBtnAdd()
{
	USES_CONVERSION;
	CString strMenuItem;
	POSITION pos = m_listShopCtrl.GetFirstSelectedItemPosition();
	while (pos) 
	{
		int iSel = m_listShopCtrl.GetNextSelectedItem(pos);
		CShopInfo* pShopInfo = (CShopInfo*)m_listShopCtrl.GetItemData(iSel);
		if (!pShopInfo)
			return;
		if (IsExistShopSendList(pShopInfo->m_objData.mac))
			return;
		int nIndex = m_listShopSendCtrl.GetItemCount();
		m_listShopSendCtrl.InsertItem(LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE, m_listShopCtrl.GetItemCount(), LPSTR_TEXTCALLBACK, 0, 0, I_IMAGECALLBACK, (LPARAM)pShopInfo);
	}
	m_listShopSendCtrl.UpdateWindow();
}

BOOL CSensorAgentDlg::IsExistShopSendList(CString mac)
{
	for (int i = 0; i < m_listShopSendCtrl.GetItemCount(); i++)
	{
		CShopInfo* pShopInfo = (CShopInfo*)m_listShopSendCtrl.GetItemData(i);
		if (pShopInfo != NULL)
		{
			if (pShopInfo->m_objData.mac == mac)
				return TRUE;
		}
	}
	return FALSE;
}

void CSensorAgentDlg::OnBnClickedBtnDll()
{
	USES_CONVERSION;
	CString strMenuItem;
	POSITION pos = m_listShopSendCtrl.GetFirstSelectedItemPosition();
	while (pos) 
	{
		int iSel = m_listShopSendCtrl.GetNextSelectedItem(pos);
		CShopInfo* pShopInfo = (CShopInfo*)m_listShopSendCtrl.GetItemData(iSel);
		if (!pShopInfo)
			return;
		m_listShopSendCtrl.DeleteItem(iSel);
		m_listShopSendCtrl.UpdateWindow();
	}
}

void CSensorAgentDlg::SendShopData()
{
	if (getStateProcess(RUN_APP_NAME))
	{
		BOOL bCheck = ((CButton*)GetDlgItem(IDC_RADIO_TCP))->GetCheck();

		if (bCheck == TRUE)
		{
			CString sServerIp;
			CString sPort;
			CString strValue;
			int nPort = 8080;
			GetDlgItem(IDC_EDT_SERVER_IP)->GetWindowTextW(sServerIp);
			GetDlgItem(IDC_EDT_SERVER_PORT)->GetWindowTextW(sPort);
			nPort = _wtoi(sPort);
			int nCount = m_listShopSendCtrl.GetItemCount();
			for (int i = 0; i < nCount; i++)
			{
				CShopInfo* pShopInfo = (CShopInfo*)m_listShopSendCtrl.GetItemData(i);
				m_pHostWnd[i].SendShopData(pShopInfo, sServerIp, nPort);
				strValue.Format(_T("%s"), pShopInfo->m_objData.getPacket());
				LogMessage(LOG_SEND, strValue);
			}
		}
		else
		{
			USES_CONVERSION;
			CString strConfig;
			strConfig.Format(_T("%s\\opcua_s.ini"), GetProgramPathW());
			CString strKey = _T("INFO");
			CString strValue;
			CString strSubKey = _T("");
			char sValue[1024];

			int nCount = m_listShopSendCtrl.GetItemCount();
			strValue.Format(_T("%d"), nCount);
			::WritePrivateProfileString(strKey, _T("count"), strValue, strConfig);
			for (int i = 0; i < nCount; i++)
			{
				CShopInfo* pShopInfo = (CShopInfo*)m_listShopSendCtrl.GetItemData(i);

				pShopInfo->m_objData.temp_1_value = m_pHostWnd[i].GetSendValue(pShopInfo->m_objData.temp_1_value,1);
				pShopInfo->m_objData.temp_2_value = m_pHostWnd[i].GetSendValue(pShopInfo->m_objData.temp_2_value,2);
				pShopInfo->m_objData.temp_3_value = m_pHostWnd[i].GetSendValue(pShopInfo->m_objData.temp_3_value,3);
				pShopInfo->m_objData.temp_4_value = m_pHostWnd[i].GetSendValue(pShopInfo->m_objData.temp_4_value,4);

				pShopInfo->m_objData.humi_1_value = m_pHostWnd[i].GetSendValue(pShopInfo->m_objData.humi_1_value,5);
				pShopInfo->m_objData.humi_2_value = m_pHostWnd[i].GetSendValue(pShopInfo->m_objData.humi_2_value,6);
				pShopInfo->m_objData.humi_3_value = m_pHostWnd[i].GetSendValue(pShopInfo->m_objData.humi_3_value,7);
				pShopInfo->m_objData.humi_4_value = m_pHostWnd[i].GetSendValue(pShopInfo->m_objData.humi_4_value,8);
				strSubKey.Format(_T("sensor_%d"), i + 1);

				strValue.Format(_T("%s"), pShopInfo->m_objData.getData());
				::WritePrivateProfileString(strKey, strSubKey, strValue, strConfig);
				LogMessage(LOG_SEND, strValue);
			}
		}
	}
}

void CSensorAgentDlg::OnBnClickedBtnSendStart()
{
	CString strTimerInterval;
	GetDlgItem(IDC_EDT_TIMER_VALUE)->GetWindowTextW(strTimerInterval);
	SetTimer(TIMER_SEND_INTERVAL, _wtoi(strTimerInterval), NULL);
}


void CSensorAgentDlg::OnBnClickedBtnSendStop()
{
	KillTimer(TIMER_SEND_INTERVAL);
}




CString CSensorAgentDlg::GetLogTypeStr(LOGTYPE typeLog)
{
	switch (typeLog)
	{
	case LOG_CONNECT:  return _T("LOG_CONNECT");
	case LOG_LOGIN:  return _T("LOG_LOGIN");
	case LOG_LEAVE:  return _T("LOG_LEAVE");
	case LOG_SOCKCLOSE:  return _T("LOG_SOCKCLOSE");
	case LOG_CHANNELMAKE:  return _T("LOG_CHANNELMAKE");
	case LOG_JOIN:  return _T("LOG_JOIN");
	case LOG_SEND:  return _T("SEND DATA");
	}
	return _T("UNKNOWN");
}

void CSensorAgentDlg::LogMessage(LOGTYPE typeLog, CString strMsg)
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

void CSensorAgentDlg::StartCollectionServer()
{
	USES_CONVERSION;
	if(!getStateProcess(RUN_APP_NAME))
		WinExec(W2A(RUN_APP_NAME), SW_SHOW);
}

void CSensorAgentDlg::StartEventManager()
{
	USES_CONVERSION;

	CString sRunCmd;
	sRunCmd.Format(_T("%s\\python.exe %s"), GetProgramPathW(), RUN_EVENT_PYC_NAME);
	WinExec(W2A(sRunCmd), SW_HIDE);
}

void CSensorAgentDlg::StartResManager()
{
	USES_CONVERSION;

	CString sRunCmd;
	sRunCmd.Format(_T("%s\\python.exe %s"), GetProgramPathW(), RUN_RES_PYC_NAME);
	WinExec(W2A(sRunCmd), SW_HIDE);
}

typedef NTSTATUS(NTAPI* _NtQueryInformationProcess)(
	HANDLE ProcessHandle,
	DWORD ProcessInformationClass,
	PVOID ProcessInformation,
	DWORD ProcessInformationLength,
	PDWORD ReturnLength
	);

typedef struct _UNICODE_STRING2
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} UNICODE_STRING2, * PUNICODE_STRING2;

typedef struct _PROCESS_BASIC_INFORMATION
{
	LONG ExitStatus;
	PVOID PebBaseAddress;
	ULONG_PTR AffinityMask;
	LONG BasePriority;
	ULONG_PTR UniqueProcessId;
	ULONG_PTR ParentProcessId;
} PROCESS_BASIC_INFORMATION, * PPROCESS_BASIC_INFORMATION;


PVOID GetPebAddress(HANDLE ProcessHandle)
{
	_NtQueryInformationProcess NtQueryInformationProcess =
		(_NtQueryInformationProcess)GetProcAddress(
			GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
	PROCESS_BASIC_INFORMATION pbi;


	NtQueryInformationProcess(ProcessHandle, 0, &pbi, sizeof(pbi), NULL);

	return pbi.PebBaseAddress;
}

BOOL CSensorAgentDlg::IsRunningProcess(const TCHAR* processname, TCHAR* param)
{
	USES_CONVERSION;
	HINSTANCE      hInstLib = NULL;
	HANDLE         hSnapShot = NULL;
	LPDWORD        lpdwPIDs = NULL;
	PROCESSENTRY32 procentry;
	BOOL           bFlag;
	DWORD          dwSize;
	DWORD          dwSize2;
	DWORD          dwIndex;
	HMODULE        hMod;
	HANDLE         hProcess;
	TCHAR          szFileName[MAX_PATH];
	BOOL		   bRet = FALSE;
	DWORD		   nTerminateProcessName;
	DWORD		   nEnumProcessName;
	PVOID pebAddress;
	PVOID rtlUserProcParamsAddress;
	UNICODE_STRING commandLine;
	WCHAR* commandLineContents;


	// ToolHelp Function Pointers.
	HANDLE(WINAPI * lpfCreateToolhelp32Snapshot)(DWORD, DWORD);
	BOOL(WINAPI * lpfProcess32First)(HANDLE, LPPROCESSENTRY32);
	BOOL(WINAPI * lpfProcess32Next)(HANDLE, LPPROCESSENTRY32);

	// PSAPI Function Pointers.
	BOOL(WINAPI * lpfEnumProcesses)(DWORD*, DWORD, DWORD*);
	BOOL(WINAPI * lpfEnumProcessModules)(HANDLE, HMODULE*, DWORD, LPDWORD);
	DWORD(WINAPI * lpfGetModuleBaseName)(HANDLE, HMODULE, LPTSTR, DWORD);



	// IF Windows NT 4.0
	if (g_platform_id == VER_PLATFORM_WIN32_NT && g_version_major == 4)
	{
		__try
		{
			// Get the procedure addresses explicitly. We do
			// this so we don't have to worry about modules
			// failing to load under OSes other than Windows NT 4.0 
			// because references to PSAPI.DLL can't be resolved.
			hInstLib = LoadLibraryA("psapi.DLL");
			if (hInstLib == NULL)
			{
				__leave;
			}

			// Get procedure addresses.
			lpfEnumProcesses = (BOOL(WINAPI*)(DWORD*, DWORD, DWORD*))
				GetProcAddress(hInstLib, "EnumProcesses");

			lpfEnumProcessModules = (BOOL(WINAPI*)(HANDLE, HMODULE*, DWORD, LPDWORD))
				GetProcAddress(hInstLib, "EnumProcessModules");

			lpfGetModuleBaseName = (DWORD(WINAPI*)(HANDLE, HMODULE, LPTSTR, DWORD))
				GetProcAddress(hInstLib, "GetModuleBaseNameA");

			if (lpfEnumProcesses == NULL || lpfEnumProcessModules == NULL
				|| lpfGetModuleBaseName == NULL)
			{
				__leave;
			}

			// 
			// Call the PSAPI function EnumProcesses to get all of the
			// ProcID's currently in the system.
			// 
			// NOTE: In the documentation, the third parameter of
			// EnumProcesses is named cbNeeded, which implies that you
			// can call the function once to find out how much space to
			// allocate for a buffer and again to fill the buffer.
			// This is not the case. The cbNeeded parameter returns
			// the number of PIDs returned, so if your buffer size is
			// zero cbNeeded returns zero.
			// 
			// NOTE: The "HeapAlloc" loop here ensures that we
			// actually allocate a buffer large enough for all the
			// PIDs in the system.
			// 
			dwSize2 = 256 * sizeof(DWORD);
			do
			{
				if (lpdwPIDs)
				{
					HeapFree(GetProcessHeap(), 0, lpdwPIDs);
					dwSize2 *= 2;
				}

				lpdwPIDs = (LPDWORD)HeapAlloc(GetProcessHeap(), 0, dwSize2);
				if (lpdwPIDs == NULL)
				{
					__leave;
				}

				if (!lpfEnumProcesses(lpdwPIDs, dwSize2, &dwSize))
				{
					__leave;
				}
			} while (dwSize == dwSize2);

			// How many ProcID's did we get?
			dwSize /= sizeof(DWORD);

			// Loop through each ProcID.
			for (dwIndex = 0; dwIndex < dwSize; dwIndex++)
			{
				szFileName[0] = 0;

				// Open the process (if we can... security does not
				// permit every process in the system to be opened).
				hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, lpdwPIDs[dwIndex]);
				if (hProcess != NULL)
				{
					// Here we call EnumProcessModules to get only the
					// first module in the process. This will be the 
					// EXE module for which we will retrieve the name.
					if (lpfEnumProcessModules(hProcess, &hMod, sizeof(hMod), &dwSize2))
					{
						// Get the module name
						if (!lpfGetModuleBaseName(hProcess, hMod, szFileName, sizeof(szFileName)))
						{
							szFileName[0] = 0;
						}

						nTerminateProcessName = _tcslen(processname);
						nEnumProcessName = _tcslen(szFileName);

						// 같은지 뒤에서부터 비교
						while (nTerminateProcessName != 0 && nEnumProcessName != 0)
						{
							if (tolower(szFileName[nEnumProcessName - 1]) !=
								tolower(processname[nTerminateProcessName - 1]))
							{
								break;
							}
							nTerminateProcessName--;
							nEnumProcessName--;
						}

						// terminate process that has same process name
						if (nTerminateProcessName == 0)
						{
							if (param != NULL)
							{
								pebAddress = GetPebAddress(hProcess);

								/* get the address of ProcessParameters */
								if (!ReadProcessMemory(hProcess, (PCHAR)pebAddress + 0x10,
									&rtlUserProcParamsAddress, sizeof(PVOID), NULL))
								{
									printf("Could not read the address of ProcessParameters!\n");
									continue;
								}
								/* read the CommandLine UNICODE_STRING structure */
								if (!ReadProcessMemory(hProcess, (PCHAR)rtlUserProcParamsAddress + 0x40,
									&commandLine, sizeof(commandLine), NULL))
								{
									printf("Could not read CommandLine!\n");
									continue;
								}
								/* allocate memory to hold the command line */
								commandLineContents = (WCHAR*)malloc(commandLine.Length);
								/* read the command line */
								if (!ReadProcessMemory(hProcess, commandLine.Buffer,
									commandLineContents, commandLine.Length, NULL))
								{
									printf("Could not read the command line string!\n");
									continue;
								}

								if (strcmp(W2A(commandLineContents), W2A(param)) != NULL)
								{
									bRet = TRUE;
								}

								free(commandLineContents);
							}
							else
							{
								bRet = TRUE;
							}
						}
					}
					CloseHandle(hProcess);

					if (bRet == TRUE) return TRUE;
				}
			}
		}
		__finally
		{
			if (hInstLib)
			{
				FreeLibrary(hInstLib);
			}

			if (lpdwPIDs)
			{
				HeapFree(GetProcessHeap(), 0, lpdwPIDs);
			}
		}
	}
	// If any OS other than Windows NT 4.0.
	else if (g_platform_id == VER_PLATFORM_WIN32_WINDOWS
		|| (g_platform_id == VER_PLATFORM_WIN32_NT && g_version_major > 4))
	{
		__try
		{
			hInstLib = LoadLibrary(_T("Kernel32.DLL"));
			if (hInstLib == NULL)
			{
				__leave;
			}

			// Get procedure addresses. We are linking to 
			// these functions explicitly, because a module using
			// this code would fail to load under Windows NT,
			// which does not have the Toolhelp32
			// functions in KERNEL32.DLL.
			lpfCreateToolhelp32Snapshot = (HANDLE(WINAPI*)(DWORD, DWORD))
				GetProcAddress(hInstLib, "CreateToolhelp32Snapshot");

			lpfProcess32First = (BOOL(WINAPI*)(HANDLE, LPPROCESSENTRY32))
				GetProcAddress(hInstLib, "Process32First");

			lpfProcess32Next = (BOOL(WINAPI*)(HANDLE, LPPROCESSENTRY32))
				GetProcAddress(hInstLib, "Process32Next");

			if (lpfProcess32Next == NULL || lpfProcess32First == NULL
				|| lpfCreateToolhelp32Snapshot == NULL)
			{
				__leave;
			}

			// Get a handle to a Toolhelp snapshot of all processes.
			hSnapShot = lpfCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (hSnapShot == INVALID_HANDLE_VALUE)
			{
				FreeLibrary(hInstLib);
				return FALSE;
			}

			// Get the first process' information.
			procentry.dwSize = sizeof(PROCESSENTRY32);
			bFlag = lpfProcess32First(hSnapShot, &procentry);

			// While there are processes, keep looping.
			while (bFlag)
			{
				nTerminateProcessName = _tcslen(processname);
				nEnumProcessName = _tcslen(procentry.szExeFile);

				// 같은지 뒤에서부터 비교
				while (nTerminateProcessName != 0 && nEnumProcessName != 0)
				{
					if (tolower(procentry.szExeFile[nEnumProcessName - 1]) !=
						tolower(processname[nTerminateProcessName - 1]))
					{
						break;
					}
					nTerminateProcessName--;
					nEnumProcessName--;
				}

				// terminate process
				if (nTerminateProcessName == 0)
				{
					HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procentry.th32ProcessID);

					if (param != NULL)
					{
						pebAddress = GetPebAddress(hProcess);

						/* get the address of ProcessParameters */
						if (!ReadProcessMemory(hProcess, (PCHAR)pebAddress + 0x10,
							&rtlUserProcParamsAddress, sizeof(PVOID), NULL))
						{
							printf("Could not read the address of ProcessParameters!\n");
							goto GO_NEXT;
						}
						/* read the CommandLine UNICODE_STRING structure */
						if (!ReadProcessMemory(hProcess, (PCHAR)rtlUserProcParamsAddress + 0x40,
							&commandLine, sizeof(commandLine), NULL))
						{
							printf("Could not read CommandLine!\n");
							goto GO_NEXT;
						}
						/* allocate memory to hold the command line */
						commandLineContents = (WCHAR*)malloc(commandLine.Length);
						/* read the command line */
						if (!ReadProcessMemory(hProcess, commandLine.Buffer,
							commandLineContents, commandLine.Length, NULL))
						{
							printf("Could not read the command line string!\n");
							goto GO_NEXT;
						}

						if (strcmp(W2A(commandLineContents), W2A(param)) != NULL)
						{
							bRet = TRUE;
						}

						free(commandLineContents);
					}
					else
					{
						bRet = TRUE;
					}
					CloseHandle(hProcess);

					if (bRet == TRUE) return TRUE;
				}

			GO_NEXT:
				procentry.dwSize = sizeof(PROCESSENTRY32);
				bFlag = lpfProcess32Next(hSnapShot, &procentry);
			}
		}
		__finally
		{
			if (hInstLib)
			{
				FreeLibrary(hInstLib);
			}
		}
	}

	return bRet;
}

BOOL CSensorAgentDlg::getStateProcess(CString name)
{
	// 실행중인 모든 프로세스 찾기
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 processEntry32;

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		exit(EXIT_FAILURE);
	}

	processEntry32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &processEntry32))
	{
		CloseHandle(hProcessSnap);
		exit(EXIT_FAILURE);
	}

	while (Process32Next(hProcessSnap, &processEntry32))
	{
		// 실행중인 프로세스 비교
		if (name.Compare(processEntry32.szExeFile) == 0)
		{
			// 실행중인 프로세스가 있다면
			return TRUE;
		}
	}
	// 실행중인 프로세스 중에 찾는 프로세스가 없다면
	return FALSE;
}

void CSensorAgentDlg::OnMenuTrayExit()
{
	EndDialog(IDOK);
}

void CSensorAgentDlg::OnMenuTrayShow()
{
	if (m_typeAgent == AGENT_SENSOR)
	{
		MoveWindow(CRect(0, 0, 1100, 550), TRUE);
		ShowWindow(SW_SHOW);
	}
}

void CSensorAgentDlg::OnMenuTrayHide()
{
	MoveWindow(0,0,0,0);
	ShowWindow(SW_HIDE);
}


static BOOL IsFindFile(CString sFile)
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile(sFile);
	return bWorking;
}

BOOL CSensorAgentDlg::isSetupProgramInstall()
{
	USES_CONVERSION;
	CString strConfig;
	strConfig.Format(_T("%s\\config.ini"), GetProgramPathW());
	CString strKey = _T("INFO");
	CString strValue;
	char sValue[1024];
	::GetPrivateProfileString(strKey, _T("update_completed"), _T("0"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
	int update_completed = _wtoi(strValue);
	CString sFindFile;
	sFindFile.Format(_T("%s\\%s"), GetProgramPathW(), RUN_INSTALL_NAME);
	if (update_completed == 1 
		&& IsFindFile(sFindFile))
	{
		strValue.Format(_T("%d"), 0);
		::WritePrivateProfileString(strKey, _T("update_completed"), strValue, strConfig);
		WinExec(W2A(RUN_INSTALL_NAME), SW_SHOW);
		return TRUE;
	}
	return FALSE;
}

void CSensorAgentDlg::SetAutoStart(BOOL bStart)
{
	USES_CONVERSION;
	HKEY hKey;
	CString strUpdateExePath;
	if (bStart)
		strUpdateExePath.Format(_T("%s\\Sensor_Agent.exe"), GetProgramPathW());
	else
		strUpdateExePath.Format(_T(""));
	LONG lnRes = RegOpenKeyEx(
		HKEY_CURRENT_USER,
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
		0L, KEY_WRITE,
		&hKey
	);
	if (ERROR_SUCCESS == lnRes)
	{
		lnRes = RegSetValueExW(hKey,
			_T("Sensor_Agent"), // key name
			0,
			REG_SZ,
			(LPBYTE)(LPCTSTR)strUpdateExePath,   // 실행할 어플의 경로
			strlen(W2A(strUpdateExePath)) * 2 + 1);
	}

	RegCloseKey(hKey);
}
