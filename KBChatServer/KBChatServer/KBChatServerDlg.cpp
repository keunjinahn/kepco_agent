
// KBChatServerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "KBChatServer.h"
#include "KBChatServerDlg.h"
#include "afxdialogex.h"
#include "MsgrSvr.h"
#include "pdh.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRect g_rectListItemInfo[SHOP_INFO_COUNT];
CRect g_rectInfoArea = CRect(20, 180, 170, 657);
CRect g_rectSensorInfoArea = CRect(242, 155, 1213, 799);
#define TIMER_ACTIVE_LAMP 1000
#define TIMER_SENSOR_DATA_SEND 2000
#define TIMER_OPCUA_LOAD 2001
#define TIMER_CHECK_CONFIG 2002
#define TIMER_INCIDENT 2003
#define TIMER_RESOURCE 2004
#define TIMER_RESTART 2005
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.


static PDH_HQUERY cpuQuery;
static PDH_HCOUNTER cpuTotal;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CKBChatServerDlg 대화 상자



CKBChatServerDlg *g_pServerDlg = NULL;
CKBChatServerDlg::CKBChatServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKBChatServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
	m_fontLocation.CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, _T("굴림체"));
	m_fontShop.CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, _T("굴림체"));
	m_fontSensorValue.CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, _T("굴림체"));
	m_fontLeftShopName.CreateFont(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, _T("굴림체"));
	m_fontSensorValue_detail.CreateFont(12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, _T("굴림체"));
	m_blushActive.CreateSolidBrush(RGB(255,255,0));
	m_blushInActive.CreateSolidBrush(RGB(58, 58, 58));
	m_pLogWindow = NULL;
	m_pCurShopInfo = NULL;
	m_pAPIAgentDlg = NULL;

	m_rectSensor[0] = CRect(363, 213, 413, 233);
	m_rectSensor[1] = CRect(558, 213, 603, 233);
	m_rectSensor[2] = CRect(750, 213, 794, 233);
	m_rectSensor[3] = CRect(944, 213, 988, 233);
	m_rectSensor[4] = CRect(0, 0, 0, 0);
	m_rectSensor[5] = CRect(363, 431, 413, 453);
	m_rectSensor[6] = CRect(556, 431, 603, 453);
	m_rectSensor[7] = CRect(748, 431, 794, 453);
	m_rectSensor[8] = CRect(944, 431, 988, 453);
	m_rectSensor[9] = CRect(0, 0, 0, 0);
	m_rectSensor[10] = CRect(363, 645, 403, 666);
	m_rectSensor[11] = CRect(557, 645, 597, 666);
	m_rectSensor[12] = CRect(0, 0, 0, 0);
	m_rectSensor[13] = CRect(0, 0, 0, 0);
	m_rectSensor[14] = CRect(0, 0, 0, 0);


	int nLeft = 23;
	int nTop = 183;
	int nWidth = 143;
	int nHeight = 45;
	for (int i = 0; i < SHOP_INFO_COUNT; i++)
	{
		g_rectListItemInfo[i] = CRect(nLeft, nTop, nLeft + nWidth, nTop + nHeight);
		nTop += nHeight + 4;
	}
	m_bWasServerConnected = FALSE;
}

void CKBChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CKBChatServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_MYCOMMAND, OnBsButtonMsg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_LOG, &CKBChatServerDlg::OnBnClickedBtnLog)
	ON_COMMAND(IDC_MENU_TRAY_EXIT, &CKBChatServerDlg::OnMenuTrayExit)
	ON_COMMAND(IDC_MENU_TRAY_LOG, &CKBChatServerDlg::OnMenuTrayLog)
END_MESSAGE_MAP()


// CKBChatServerDlg 메시지 처리기


BOOL CKBChatServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	g_pServerDlg = this;
	
	/*
	int num = 64302;
	char hex[20];
	strcpy(hex, dec2Hex(num));
	int dec = Hex2Com(hex);
	*/

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	WSADATA wsaData;
    if (WSAStartup (MAKEWORD(2, 0), &wsaData) != SOCKET_ERROR)
	{
		g_MsgrSvr.Start();
	}
	LoadConfig();
	InitControl();
	MoveWindow(CRect(0, 0, 1283, 869));
	CenterWindow();
	SetTimer(TIMER_ACTIVE_LAMP, 500, NULL);
	SetTimer(TIMER_SENSOR_DATA_SEND, m_ConfigInfo.data_upload_second, NULL);
	SetTimer(TIMER_CHECK_CONFIG, 1000, NULL);
	SetTimer(TIMER_INCIDENT, 1000, NULL);
	SetTimer(TIMER_RESTART, 1000 * 60 * 60 * 6,NULL);
	
	HICON hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON2));  // Icon to use
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

	init_localdb();

	SetWindowText(_T("KConnectionServer"));

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CKBChatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CKBChatServerDlg::OnPaint()
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
		CPaintDC dcOrigin(this); // device context for painting
		CMemDC_ dc(&dcOrigin);
		CRect rect;
		GetClientRect(&rect);
		m_imgBgMain.Draw(dc, rect);

		CFont* pOldFont = dc.SelectObject(&m_fontLocation);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(255, 255, 255));
		CString strLocation;
		strLocation.Format(_T("%s 수집서버"), m_ConfigInfo.locationname);
		dc.DrawText(strLocation, CRect(194,17,1200,57), DT_CENTER | DT_TOP | DT_SINGLELINE);
		pOldFont = dc.SelectObject(&m_fontShop);
		if (m_pCurShopInfo != NULL )
		{
			CAreaInfo* pAreaInfo = (CAreaInfo*)GetAreaInfo(m_pCurShopInfo->m_objData.mac);
			if (pAreaInfo != NULL)
			{
				CString strShop;
				strShop.Format(_T("%s"), pAreaInfo->strShopName);
				dc.DrawText(strShop, CRect(839, 104, 1107, 131), DT_CENTER | DT_TOP | DT_SINGLELINE);

			}
		}

		CString strTitle;
		strTitle.Format(_T("%s"), _T("D-TRS 수집서버"));
		//strTitle.Format(_T("%s"), _T("호서대 스마트MC"));
		dc.DrawText(strTitle, CRect(17, 17, 175, 44), DT_CENTER | DT_TOP | DT_SINGLELINE);

		pOldFont = dc.SelectObject(&m_fontSensorValue_detail);
		//strTitle.Format(_T("%s"), _T("OPC UA 사용"));
		//dc.DrawText(strTitle, CRect(1169, 803, 1264, 820), DT_CENTER | DT_TOP | DT_SINGLELINE);


		pOldFont = DrawSensorInfo(&dc);
		pOldFont = DrawItemList(&dc, &m_listArea);
		dc.SelectObject(pOldFont);

		

		ReleaseDC(dc);
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CKBChatServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CKBChatServerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if ((pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN)))
	{	
		return TRUE;
	}
	else if ((pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE)))
	{	
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CKBChatServerDlg::init_localdb()
{
	USES_CONVERSION;

	char* host = "127.0.0.1";
	char* userId = "dbadmin";
	char* passwd = "kepco12!@";
	char* dbName = "kcollectiondb";
	int port = 3306;
	mysql_init(&m_Mysql);
	if (mysql_real_connect(&m_Mysql, host, userId, passwd, dbName, port, NULL, 0) == NULL)
	{
		return;
	}
	else {
		mysql_query(&m_Mysql, "set names euckr"); // 한글 깨짐 보완
	}
}

void CKBChatServerDlg::SensorDataInsert(KBPKT_DATA_OBJ *pObj)
{
	USES_CONVERSION;

	/*
	char* host = "127.0.0.1";
	char* userId = "dbadmin";
	char* passwd = "kepco12!@";
	char* dbName = "kcollectiondb";
	int port = 3306;
	mysql_init(&m_Mysql);
	if (mysql_real_connect(&m_Mysql, host, userId, passwd, dbName, port, NULL, 0) == NULL)
	{
		return;
	}
	else {
		mysql_query(&m_Mysql, "set names euckr"); // 한글 깨짐 보완
	}
	*/

	SYSTEMTIME current_time;
	GetLocalTime(&current_time);
	CString sensor_date = _T("");
	sensor_date.Format(_T("%.4ld/%.2ld/%.2ld %.2ld:%.2ld:%.2ld"), current_time.wYear, current_time.wMonth, current_time.wDay,
		current_time.wHour, current_time.wMinute, current_time.wSecond);

	CString strQuery = _T("");
	strQuery.Format(_T("insert into tb_collection_hist(mac,device_no, sensor_state,leak_1_value,leak_2_value,temp_1_value,temp_2_value,temp_3_value,temp_4_value \
		,humi_1_value, humi_2_value, humi_3_value, humi_4_value,sensor_date) values('%s','%d', '%d', '%d', '%d', '%.1f', '%.1f', '%.1f', '%.1f', '%.1f', '%.1f', '%.1f', '%.1f','%s')")
		, pObj->mac, pObj->device_no, pObj->sensor_state, pObj->leak_1_value, pObj->leak_2_value, pObj->temp_1_value, pObj->temp_2_value, pObj->temp_3_value, pObj->temp_4_value
		, pObj->humi_1_value, pObj->humi_2_value, pObj->humi_3_value, pObj->humi_4_value, sensor_date);

	if (mysql_query(&m_Mysql, W2A(strQuery))) {
		return;
	}

	int check = _CrtDumpMemoryLeaks();



	CString sMac = _T("");
	sMac.Format(_T("%s"), pObj->mac);

	CShopInfo* pShopInfo = GetShopInfo(sMac);

	if (pShopInfo == NULL)
	{
		pShopInfo = new CShopInfo();
		pShopInfo->strShopName.Format(_T("미할당"));
		pShopInfo->m_objData.mac = pObj->mac;
		m_ConfigInfo.m_pShowList->AddTail(pShopInfo);
	}

	CAreaInfo* pAreaInfo = GetAreaInfo(pObj->mac);
	if (pAreaInfo != NULL)
		pAreaInfo->m_bActive = TRUE;

	pShopInfo->m_objData.device_no = pObj->device_no;
	pShopInfo->m_objData.sensor_state = pObj->sensor_state;
	pShopInfo->m_objData.leak_1_value = pObj->leak_1_value;
	pShopInfo->m_objData.leak_2_value = pObj->leak_2_value;

	pShopInfo->m_objData.temp_1_value = pObj->temp_1_value;
	pShopInfo->m_objData.temp_1_value_total += pObj->temp_1_value;
	if (pObj->temp_1_value > pShopInfo->m_objData.temp_1_value_max)
		pShopInfo->m_objData.temp_1_value_max = pObj->temp_1_value;
	if (pShopInfo->m_objData.temp_1_value_min == 0 
		|| pObj->temp_1_value < pShopInfo->m_objData.temp_1_value_min)
		pShopInfo->m_objData.temp_1_value_min = pObj->temp_1_value;

	pShopInfo->m_objData.temp_2_value = pObj->temp_2_value;
	pShopInfo->m_objData.temp_2_value_total += pObj->temp_2_value;
	if (pObj->temp_2_value > pShopInfo->m_objData.temp_2_value_max)
		pShopInfo->m_objData.temp_2_value_max = pObj->temp_2_value;
	if (pShopInfo->m_objData.temp_2_value_min == 0
		|| pObj->temp_2_value < pShopInfo->m_objData.temp_2_value_min)
		pShopInfo->m_objData.temp_2_value_min = pObj->temp_2_value;

	pShopInfo->m_objData.temp_3_value = pObj->temp_3_value;
	pShopInfo->m_objData.temp_3_value_total += pObj->temp_3_value;
	if (pObj->temp_3_value > pShopInfo->m_objData.temp_3_value_max)
		pShopInfo->m_objData.temp_3_value_max = pObj->temp_3_value;
	if (pShopInfo->m_objData.temp_3_value_min == 0
		|| pObj->temp_3_value < pShopInfo->m_objData.temp_3_value_min)
		pShopInfo->m_objData.temp_3_value_min = pObj->temp_3_value;

	pShopInfo->m_objData.temp_4_value = pObj->temp_4_value;
	pShopInfo->m_objData.temp_4_value_total += pObj->temp_4_value;
	if (pObj->temp_4_value > pShopInfo->m_objData.temp_4_value_max)
		pShopInfo->m_objData.temp_4_value_max = pObj->temp_4_value;
	if (pShopInfo->m_objData.temp_4_value_min == 0
		|| pObj->temp_4_value < pShopInfo->m_objData.temp_4_value_min)
		pShopInfo->m_objData.temp_4_value_min = pObj->temp_4_value;

	/// <summary>
	pShopInfo->m_objData.humi_1_value = pObj->humi_1_value;
	pShopInfo->m_objData.humi_1_value_total += pObj->humi_1_value;
	if (pObj->humi_1_value > pShopInfo->m_objData.humi_1_value_max)
		pShopInfo->m_objData.humi_1_value_max = pObj->humi_1_value;
	if (pShopInfo->m_objData.humi_1_value_min == 0
		|| pObj->humi_1_value < pShopInfo->m_objData.humi_1_value_min)
		pShopInfo->m_objData.humi_1_value_min = pObj->humi_1_value;

	pShopInfo->m_objData.humi_2_value = pObj->humi_2_value;
	pShopInfo->m_objData.humi_2_value_total += pObj->humi_2_value;
	if (pObj->humi_2_value > pShopInfo->m_objData.humi_2_value_max)
		pShopInfo->m_objData.humi_2_value_max = pObj->humi_2_value;
	if (pShopInfo->m_objData.humi_2_value_min == 0
		|| pObj->humi_2_value < pShopInfo->m_objData.humi_2_value_min)
		pShopInfo->m_objData.humi_2_value_min = pObj->humi_2_value;

	pShopInfo->m_objData.humi_3_value = pObj->humi_3_value;
	pShopInfo->m_objData.humi_3_value_total += pObj->humi_3_value;
	if (pObj->humi_3_value > pShopInfo->m_objData.humi_3_value_max)
		pShopInfo->m_objData.humi_3_value_max = pObj->humi_3_value;
	if (pShopInfo->m_objData.humi_3_value_min == 0
		|| pObj->humi_3_value < pShopInfo->m_objData.humi_3_value_min)
		pShopInfo->m_objData.humi_3_value_min = pObj->humi_3_value;

	pShopInfo->m_objData.humi_4_value = pObj->humi_4_value;
	pShopInfo->m_objData.humi_4_value_total += pObj->humi_4_value;
	if (pObj->humi_4_value > pShopInfo->m_objData.humi_4_value_max)
		pShopInfo->m_objData.humi_4_value_max = pObj->humi_4_value;
	if (pShopInfo->m_objData.humi_4_value_min == 0
		|| pObj->humi_4_value < pShopInfo->m_objData.humi_4_value_min)
		pShopInfo->m_objData.humi_4_value_min = pObj->humi_4_value;

	pShopInfo->m_objData.sensor_date.Format(_T("%s"), sensor_date);

	CTime t = CTime::GetCurrentTime();
	CString sTime = _T("");
	sTime.Format(_T("%02d%02d%02d%02d%02d"), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	pShopInfo->m_current_date = _wtol(sTime);
	pShopInfo->m_currentTic = ::GetTickCount();
	pShopInfo->m_objData.dwCount++;

	if (pShopInfo == m_pCurShopInfo)
		InvalidateRect(g_rectSensorInfoArea,FALSE);
	for (int i = 0; i < SHOP_INFO_COUNT; i++)
		InvalidateRect(g_rectListItemInfo[i], FALSE);

}


CShopInfo* CKBChatServerDlg::GetShopInfo(CString srcMac)
{
	USES_CONVERSION;
	POSITION pos = m_ConfigInfo.m_pShowList->GetHeadPosition();
	while (pos)
	{
		CShopInfo* pShopInfo = (CShopInfo*)m_ConfigInfo.m_pShowList->GetNext(pos);
		
		CString destMac;
		destMac.Format(_T("%s"), pShopInfo->m_objData.mac);
		/*
		CString strMsg;
		strMsg.Format(_T("destMac mac : %s,src mac : %s",), destMac.TrimLeft(), srcMac.TrimLeft());
		theApp.AppLog.Write(_T("%s"), strMsg);
		*/
		if (destMac.TrimLeft() == srcMac.TrimLeft())
			return pShopInfo;
	}
	return NULL;
}

CAreaInfo* CKBChatServerDlg::GetAreaInfo(CString srcMac)
{
	USES_CONVERSION;
	POSITION pos = m_listArea.GetHeadPosition();
	while (pos)
	{
		CAreaInfo* pAreaInfo = (CAreaInfo*)m_listArea.GetNext(pos);
		CString destMac;
		destMac.Format(_T("%s"), pAreaInfo->mac);
		/*
		CString strMsg;
		strMsg.Format(_T("destMac mac : %s,src mac : %s",), destMac.TrimLeft(), srcMac.TrimLeft());
		theApp.AppLog.Write(_T("%s"), strMsg);
		*/
		if (destMac == srcMac)
			return pAreaInfo;
	}

	return NULL;
}



CShopInfo* CKBChatServerDlg::IsNotAllocShop(CString srcMac)
{
	USES_CONVERSION;
	POSITION pos = m_ConfigInfo.m_pShowList->GetHeadPosition();
	while (pos)
	{
		CShopInfo* pShopInfo = (CShopInfo*)m_ConfigInfo.m_pShowList->GetNext(pos);
		if (pShopInfo->m_objData.mac.GetLength() < 1)
			return pShopInfo;
	}
	return NULL;
}


void CKBChatServerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	

	mysql_close(&m_Mysql);

	g_MsgrSvr.Stop();
	WSACleanup();

	if (m_pLogWindow != NULL)
	{
		delete m_pLogWindow;
		m_pLogWindow = NULL;
	}

	while (!m_ConfigInfo.m_pShowList->IsEmpty())
	{
		CShopInfo* pShopInfo = (CShopInfo*)m_ConfigInfo.m_pShowList->RemoveHead();
		delete pShopInfo;
	}

	while (!m_listArea.IsEmpty())
	{
		CAreaInfo* pAreaInfo = (CAreaInfo*)m_listArea.RemoveHead();
		delete pAreaInfo;
	}
	
	if (m_pAPIAgentDlg != NULL)
	{
		delete m_pAPIAgentDlg;
		m_pAPIAgentDlg = NULL;
	}
	
	// TODO: Add your message handler code here
}


void CKBChatServerDlg::OnBnClickedBtnExit()
{
	if(AfxMessageBox(L"프로그램을 종료하시겠습니까?", MB_YESNO) == IDYES)
	{
		EndDialog(IDOK);
	}
}

void CKBChatServerDlg::InitControl()
{
	
	m_imgBgMain.LoadResource(FindResource(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BG_MAIN), RT_BITMAP), CXIMAGE_FORMAT_BMP, AfxGetApp()->m_hInstance);

	CxImage ximg;
	ximg.LoadResource(FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_PROFILE), RT_BITMAP), CXIMAGE_FORMAT_BMP, AfxGetInstanceHandle());
	m_btnProfile.Create(CRect(1209, 0, 1281, 58), this, IDC_BTN_EXIT, ximg, 4);
	m_btnProfile.ShowWindow(SW_SHOW);
	m_btnProfile.Invalidate();

	ximg.LoadResource(FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_LOG), RT_BITMAP), CXIMAGE_FORMAT_BMP, AfxGetInstanceHandle());
	m_btnLog.Create(CRect(1147, 97, 1186, 135), this, IDC_BTN_LOG, ximg, 4);
	m_btnLog.ShowWindow(SW_SHOW);
	m_btnLog.Invalidate();
	
	ximg.LoadResource(FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BMP_OPCUA), RT_BITMAP), CXIMAGE_FORMAT_BMP, AfxGetInstanceHandle());
	m_btnOpcua.Create(CRect(1165, 825, 1266, 860), this, IDC_BTN_OPCUA, ximg, 4,CBsButton::TOGGLE_BUTTON);
	m_btnOpcua.ShowWindow(SW_HIDE);
	m_btnOpcua.Invalidate();
	

	m_pLogWindow = new LogWindow();
	m_pLogWindow->Create(IDD_LOGWINDOW, this);
	m_pLogWindow->ShowWindow(SW_HIDE);

	m_pAPIAgentDlg = new CApiAgent();
	m_pAPIAgentDlg->Create(IDD_APIAGENT_DLG, this);
	m_pAPIAgentDlg->ShowWindow(SW_HIDE);
	/*
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
		return;
	}
	m_TrayIcon.SetMenuDefaultItem(1, TRUE);

	m_Tip.Create(this);
	*/
}


void CKBChatServerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (point.y < 91)
	{
		POINT pt;
		GetCursorPos(&pt);
		SendMessage(WM_LBUTTONUP, 0, MAKELPARAM(point.x, point.y));
		SendMessage(WM_SYSCOMMAND, 0xf012, MAKELPARAM(pt.x, pt.y));
	}

	if (g_rectInfoArea.PtInRect(point))
	{
		for (int i = 0; i< SHOP_INFO_COUNT;i++)
		{
			if (g_rectListItemInfo[i].PtInRect(point) && i < (m_listArea.GetCount()))
			{
				CAreaInfo* pAreaInfo = (CAreaInfo*)m_listArea.GetAt(m_listArea.FindIndex(i));
				if (pAreaInfo != NULL)
				{
					CShopInfo* pShopInfo = GetShopInfo(pAreaInfo->mac);
					if(pShopInfo != NULL)
						m_pCurShopInfo = pShopInfo;
				}
				Invalidate(FALSE);
			}
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

LRESULT CKBChatServerDlg::OnBsButtonMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDC_BTN_EXIT:
		OnBnClickedBtnExit();
		break;
	case IDC_BTN_LOG:
		ShowLoginDlg();
		break;
	case IDC_BTN_OPCUA:
		BOOL bCheck = m_btnOpcua.IsCheck();
		if (bCheck == TRUE)
			SetTimer(TIMER_OPCUA_LOAD, m_ConfigInfo.data_upload_second, NULL);
		else
			KillTimer(TIMER_OPCUA_LOAD);
		break;
	}

	return 0;
}

CString CKBChatServerDlg::GetProgramPathW()
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

void CKBChatServerDlg::LoadConfig()
{
	USES_CONVERSION;

	
	CString strConfig;
	strConfig.Format(_T("%s\\config.ini"), GetProgramPathW());
	CString strKey = _T("INFO");
	CString strValue;
	char sValue[1024];
	::GetPrivateProfileString(strKey, _T("server_ip"), _T("localhost"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
	m_ConfigInfo.server_ip.Format(_T("%s"), strValue);
	::GetPrivateProfileString(strKey, _T("port"), _T("5000"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
	m_ConfigInfo.port = _wtoi(strValue);
	::GetPrivateProfileString(strKey, _T("web_port"), _T("443"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
	m_ConfigInfo.web_port = _wtoi(strValue);
	::GetPrivateProfileString(strKey, _T("agent_type"), _T("1"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
	m_ConfigInfo.agent_type = (AGENT_TYPE)_wtoi(strValue);
	


	strKey = GetIpAddress();

#ifdef _DEBUG
		//strKey = _T("70.112.1.187");
#endif

	if (m_ConfigInfo.agent_type == AGENT_SENSOR)
	{
		::GetPrivateProfileString(strKey, _T("locationname"), _T("UNKNOWN"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
		m_ConfigInfo.locationname.Format(_T("%s"), strValue);
		if (m_ConfigInfo.locationname == _T("UNKNOWN"))
		{
			AfxMessageBox(_T("미인식지역 전처리서버입니다 관리자에게 문의하여주세요!!!"));
			EndDialog(IDOK);
		}
		::GetPrivateProfileString(strKey, _T("locationcode"), _T("1"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
		m_ConfigInfo.locationcode.Format(_T("%s"), strValue);

		::GetPrivateProfileString(strKey, _T("agent_ver"), _T("1.0.0"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
		m_ConfigInfo.agent_ver.Format(_T("%s"), strValue);
		::GetPrivateProfileString(strKey, _T("update_completed"), _T("0"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
		m_ConfigInfo.update_completed = _wtoi(strValue);

		::GetPrivateProfileString(strKey, _T("shopcount"), _T("1"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
		int nShopCount = _wtoi(strValue);
	}
	/*
	CString sNum;
	CString sMac;
	for (int i = 0; i < nShopCount; i++)
	{
		CShopInfo* pShopInfo = new CShopInfo();
		sNum.Format(_T("%02d"), i + 1);
		::GetPrivateProfileString(strKey, sNum, _T(""), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
		m_ConfigInfo.listShop.AddTail(strValue);
		pShopInfo->strShopName = strValue;
		sMac.Format(_T("%02d_mac"), i + 1);
		::GetPrivateProfileString(strKey, sMac, _T(""), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
		m_ConfigInfo.listShopMac.AddTail(strValue);
		pShopInfo->m_objData.mac = strValue;
		m_ConfigInfo.m_pShowList->AddTail(pShopInfo);
		if (i == 0)
			m_pCurShopInfo = pShopInfo;
	}
	*/
	strValue.Format(_T("%s"), AGENT_VERSION);
	::WritePrivateProfileString(strKey, _T("agent_ver"), strValue, strConfig);
}

void CKBChatServerDlg::UpdateDownloadCompleteToIni(int nResult)
{
	USES_CONVERSION;
	CString strConfig;
	strConfig.Format(_T("%s\\config.ini"), GetProgramPathW());
	CString strKey = _T("INFO");

	CString strValue;
	char sValue[1024];
	strValue.Format(_T("%d"),nResult);
	::WritePrivateProfileString(strKey, _T("update_completed"), strValue, strConfig);


}


void CKBChatServerDlg::ShowLoginDlg()
{
	/*
	if (m_pCurShopInfo)
	{
		CString sCmd;
		sCmd.Format(_T("%d"), API_SENSOR_REGISTER);
		APICALLDATA* pData = new APICALLDATA;
		pData->insert(APICALLDATA::value_type(KEY_CMD, sCmd));
		CString mac;
		mac.Format(_T("%s"), m_pCurShopInfo->m_objData.mac);
		pData->insert(APICALLDATA::value_type(_mac, mac));
		CString location_code;
		location_code.Format(_T("%s"), m_ConfigInfo.locationcode);
		pData->insert(APICALLDATA::value_type(_location_code, location_code));
		g_pApiAgentDlg->AddAPI(pData);
	}
	*/
	if (m_pLogWindow)
	{
		m_pLogWindow->ShowWindow(SW_SHOW);
	}
}

void CKBChatServerDlg::SendSensorData()
{

	if (m_ConfigInfo.enable_upload != 1)
		return;

	CString sCmd, mac, location_code, device_no, sensor_state, leak_1_value, leak_2_value
		, temp_1_value, temp_2_value, temp_3_value, temp_4_value, humi_1_value, humi_2_value, humi_3_value, humi_4_value;

	CString strMerge = _T("");
	for (int i = 0; i < m_ConfigInfo.m_pShowList->GetCount(); i++)
	{
		CShopInfo* pShopInfo = (CShopInfo*)m_ConfigInfo.m_pShowList->GetAt(m_ConfigInfo.m_pShowList->FindIndex(i));
		DWORD dwTic = ::GetTickCount() - pShopInfo->m_currentTic;
		if (pShopInfo != NULL 
			&& pShopInfo->m_objData.device_no != 0  
			&& dwTic < 1000 * 10) //센서데이터에 값이 들어 있으면 보냄 , 10초동안 값이 안들어오면 보내지않음
		{
			mac.Format(_T("%s"), pShopInfo->m_objData.mac);


			strMerge += mac + _T(",");
			device_no.Format(_T("%d"), pShopInfo->m_objData.device_no);
			strMerge += device_no + _T(",");
			leak_1_value.Format(_T("%d"), pShopInfo->m_objData.leak_1_value);
			strMerge += leak_1_value + _T(",");
			leak_2_value.Format(_T("%d"), pShopInfo->m_objData.leak_2_value);
			strMerge += leak_2_value + _T(",");
			temp_1_value.Format(_T("%.1f"), pShopInfo->m_objData.temp_1_value);
			strMerge += temp_1_value + _T(",");
			temp_2_value.Format(_T("%.1f"), pShopInfo->m_objData.temp_2_value);
			strMerge += temp_2_value + _T(",");
			temp_3_value.Format(_T("%.1f"), pShopInfo->m_objData.temp_3_value);
			strMerge += temp_3_value + _T(",");
			temp_4_value.Format(_T("%.1f"), pShopInfo->m_objData.temp_4_value);
			strMerge += temp_4_value + _T(",");
			humi_1_value.Format(_T("%.1f"), pShopInfo->m_objData.humi_1_value);
			strMerge += humi_1_value + _T(",");
			humi_2_value.Format(_T("%.1f"), pShopInfo->m_objData.humi_2_value);
			strMerge += humi_2_value + _T(",");
			humi_3_value.Format(_T("%.1f"), pShopInfo->m_objData.humi_3_value);
			strMerge += humi_3_value + _T(",");
			humi_4_value.Format(_T("%.1f"), pShopInfo->m_objData.humi_4_value);
			strMerge += humi_4_value + _T(",");
			strMerge += pShopInfo->m_objData.sensor_date + _T(",");
			if(i != m_ConfigInfo.m_pShowList->GetCount()-1)
				strMerge += _T("|");
			/*
			sCmd.Format(_T("%d"), API_SENSOR_DATA);
			APICALLDATA* pData = new APICALLDATA;
			pData->insert(APICALLDATA::value_type(KEY_CMD, sCmd));
			mac.Format(_T("%s"), pShopInfo->m_objData.mac);
			pData->insert(APICALLDATA::value_type(_mac, mac));
			location_code.Format(_T("%s"), m_ConfigInfo.locationcode);
			pData->insert(APICALLDATA::value_type(_location_code, location_code));
			device_no.Format(_T("%d"), pShopInfo->m_objData.device_no);
			pData->insert(APICALLDATA::value_type(_device_no, device_no));
			sensor_state.Format(_T("%d"), pShopInfo->m_objData.sensor_state);
			pData->insert(APICALLDATA::value_type(_sensor_state, sensor_state));
			leak_1_value.Format(_T("%d"), pShopInfo->m_objData.leak_1_value);
			pData->insert(APICALLDATA::value_type(_leak_1_value, leak_1_value));
			leak_2_value.Format(_T("%d"), pShopInfo->m_objData.leak_2_value);
			pData->insert(APICALLDATA::value_type(_leak_2_value, leak_2_value));
			temp_1_value.Format(_T("%.1f"), pShopInfo->m_objData.temp_1_value);
			pData->insert(APICALLDATA::value_type(_temp_1_value, temp_1_value));
			temp_2_value.Format(_T("%.1f"), pShopInfo->m_objData.temp_2_value);
			pData->insert(APICALLDATA::value_type(_temp_2_value, temp_2_value));
			temp_3_value.Format(_T("%.1f"), pShopInfo->m_objData.temp_3_value);
			pData->insert(APICALLDATA::value_type(_temp_3_value, temp_3_value));
			temp_4_value.Format(_T("%.1f"), pShopInfo->m_objData.temp_4_value);
			pData->insert(APICALLDATA::value_type(_temp_4_value, temp_4_value));
			humi_1_value.Format(_T("%.1f"), pShopInfo->m_objData.humi_1_value);
			pData->insert(APICALLDATA::value_type(_humi_1_value, humi_1_value));
			humi_2_value.Format(_T("%.1f"), pShopInfo->m_objData.humi_2_value);
			pData->insert(APICALLDATA::value_type(_humi_2_value, humi_2_value));
			humi_3_value.Format(_T("%.1f"), pShopInfo->m_objData.humi_3_value);
			pData->insert(APICALLDATA::value_type(_humi_3_value, humi_3_value));
			humi_4_value.Format(_T("%.1f"), pShopInfo->m_objData.humi_4_value);
			pData->insert(APICALLDATA::value_type(_humi_4_value, humi_4_value));
			g_pApiAgentDlg->AddAPI(pData);
			*/
		}
	}
	if (strMerge != _T(""))
	{
		sCmd.Format(_T("%d"), API_SENSOR_DATA);
		APICALLDATA* pData = new APICALLDATA;
		pData->insert(APICALLDATA::value_type(KEY_CMD, sCmd));
		location_code.Format(_T("%s"), m_ConfigInfo.locationcode);
		pData->insert(APICALLDATA::value_type(_location_code, location_code));
		pData->insert(APICALLDATA::value_type(_sensor_msg, strMerge));
		g_pApiAgentDlg->AddAPI(pData);
	}
}

void CKBChatServerDlg::SendCheckConfig()
{
	CString sCmd, location_code;
	sCmd.Format(_T("%d"), API_CHECK_CONFIG);
	APICALLDATA* pData = new APICALLDATA;
	pData->insert(APICALLDATA::value_type(KEY_CMD, sCmd));
	/*
	if (m_ConfigInfo.agent_type == AGENT_RESOURCE)
		location_code.Format(_T("%s"), _T("00"));
	else
		location_code.Format(_T("%s"), m_ConfigInfo.locationcode);
	*/
	location_code.Format(_T("%s"), m_ConfigInfo.locationcode);
	pData->insert(APICALLDATA::value_type(_location_code, location_code));
	g_pApiAgentDlg->AddAPI(pData);
}

void CKBChatServerDlg::LogMessage(LOGTYPE typeLog, CString strMsg)
{
	if (m_pLogWindow)
	{
		m_pLogWindow->LogMessage(typeLog,strMsg);
	}
}

void CKBChatServerDlg::CheckIncident()
{
	if (m_ConfigInfo.enable_upload != 1)
		return;
	for (int i = 0; i < m_ConfigInfo.m_pShowList->GetCount(); i++)
	{
		float resultValue = 0;
		CShopInfo* pShopInfo = (CShopInfo*)m_ConfigInfo.m_pShowList->GetAt(m_ConfigInfo.m_pShowList->FindIndex(i));
		if (pShopInfo != NULL)
		{
			CString sCmd, mac, sensor_value, error_code, incident_type, level;
			sCmd.Format(_T("%d"), API_INCIDENT_DATA);
			CStringList valueList;
			CString strValue;
			strValue.Format(_T("%d"), pShopInfo->m_objData.leak_1_value);
			valueList.AddTail(strValue);
			strValue.Format(_T("%d"), pShopInfo->m_objData.leak_2_value);
			valueList.AddTail(strValue);
			CHECKRESULT _checkResult_1 = CheckIncidentDevice(&valueList, SENSOR_LEAK,&resultValue);
			if (_checkResult_1 != CHECK_NORMAL)
			{
				APICALLDATA* pData = new APICALLDATA;
				pData->insert(APICALLDATA::value_type(KEY_CMD, sCmd));
				mac.Format(_T("%s"), pShopInfo->m_objData.mac);
				pData->insert(APICALLDATA::value_type(_mac, mac));
				sensor_value.Format(_T("%f"), resultValue);
				pData->insert(APICALLDATA::value_type(_sensor_value, sensor_value));
				error_code.Format(_T("%s"), _T("L01"));
				pData->insert(APICALLDATA::value_type(_error_code, error_code));
				incident_type.Format(_T("%d"), INCIDENT_THRESHOLD);
				pData->insert(APICALLDATA::value_type(_incident_type, incident_type));
				level.Format(_T("%d"), _checkResult_1);
				pData->insert(APICALLDATA::value_type(_level, level));
				g_pApiAgentDlg->AddAPI(pData);
			}
			valueList.RemoveAll();
			strValue.Format(_T("%.1f"), pShopInfo->m_objData.temp_1_value);
			valueList.AddTail(strValue);
			strValue.Format(_T("%.1f"), pShopInfo->m_objData.temp_2_value);
			valueList.AddTail(strValue);
			strValue.Format(_T("%.1f"), pShopInfo->m_objData.temp_3_value);
			valueList.AddTail(strValue);
			strValue.Format(_T("%.1f"), pShopInfo->m_objData.temp_4_value);
			valueList.AddTail(strValue);
			CHECKRESULT _checkResult_2 = CheckIncidentDevice(&valueList, SENSOR_TEMP, &resultValue);
			if (_checkResult_2 != CHECK_NORMAL && _checkResult_1 != CHECK_ERROR)
			{
				APICALLDATA* pData = new APICALLDATA;
				pData->insert(APICALLDATA::value_type(KEY_CMD, sCmd));
				mac.Format(_T("%s"), pShopInfo->m_objData.mac);
				pData->insert(APICALLDATA::value_type(_mac, mac));
				sensor_value.Format(_T("%.1f"), resultValue);
				pData->insert(APICALLDATA::value_type(_sensor_value, sensor_value));
				error_code.Format(_T("%s"), _T("T01"));
				pData->insert(APICALLDATA::value_type(_error_code, error_code));
				incident_type.Format(_T("%d"), INCIDENT_THRESHOLD);
				pData->insert(APICALLDATA::value_type(_incident_type, incident_type));
				level.Format(_T("%d"), _checkResult_2);
				pData->insert(APICALLDATA::value_type(_level, level));
				g_pApiAgentDlg->AddAPI(pData);
			}
			valueList.RemoveAll();
			strValue.Format(_T("%f"), pShopInfo->m_objData.humi_1_value);
			valueList.AddTail(strValue);
			strValue.Format(_T("%f"), pShopInfo->m_objData.humi_2_value);
			valueList.AddTail(strValue);
			strValue.Format(_T("%f"), pShopInfo->m_objData.humi_3_value);
			valueList.AddTail(strValue);
			strValue.Format(_T("%f"), pShopInfo->m_objData.humi_4_value);
			valueList.AddTail(strValue);
			CHECKRESULT _checkResult_3 = CheckIncidentDevice(&valueList, SENSOR_HUMI, &resultValue);
			if (_checkResult_3 != CHECK_NORMAL && _checkResult_1 != CHECK_ERROR && _checkResult_2 != CHECK_ERROR)
			{
				APICALLDATA* pData = new APICALLDATA;
				pData->insert(APICALLDATA::value_type(KEY_CMD, sCmd));
				mac.Format(_T("%s"), pShopInfo->m_objData.mac);
				pData->insert(APICALLDATA::value_type(_mac, mac));
				sensor_value.Format(_T("%.1f"), resultValue);
				pData->insert(APICALLDATA::value_type(_sensor_value, sensor_value));
				error_code.Format(_T("%s"), _T("H01"));
				pData->insert(APICALLDATA::value_type(_error_code, error_code));
				incident_type.Format(_T("%d"), INCIDENT_THRESHOLD);
				pData->insert(APICALLDATA::value_type(_incident_type, incident_type));
				level.Format(_T("%d"), _checkResult_3);
				pData->insert(APICALLDATA::value_type(_level, level));
				g_pApiAgentDlg->AddAPI(pData);
			}
			/*
			CHECKRESULT _checkResult_4 = CHECK_NORMAL;

			DWORD dwDiffSecond = pShopInfo->m_current_date - pShopInfo->m_last_updated_date;
			if (dwDiffSecond > (30))
			{
				_checkResult_4 = CHECK_WARRING;
				APICALLDATA* pData = new APICALLDATA;
				pData->insert(APICALLDATA::value_type(KEY_CMD, sCmd));
				mac.Format(_T("%s"), pShopInfo->m_objData.mac);
				pData->insert(APICALLDATA::value_type(_mac, mac));
				sensor_value.Format(_T("%.1f"), 0.0);
				pData->insert(APICALLDATA::value_type(_sensor_value, sensor_value));
				error_code.Format(_T("%s"), _T("D01"));
				pData->insert(APICALLDATA::value_type(_error_code, error_code));
				incident_type.Format(_T("%d"), INCIDENT_THRESHOLD);
				pData->insert(APICALLDATA::value_type(_incident_type, incident_type));
				level.Format(_T("%d"), 0);
				pData->insert(APICALLDATA::value_type(_level, level));
				g_pApiAgentDlg->AddAPI(pData);
			}

			if (_checkResult_1 != CHECK_NORMAL
				|| _checkResult_2 != CHECK_NORMAL
				|| _checkResult_3 != CHECK_NORMAL
				)
			{
				pShopInfo->m_bIncident = TRUE;
			}
			else {
				if (pShopInfo->m_bIncident == TRUE)
				{
					APICALLDATA* pData = new APICALLDATA;
					pData->insert(APICALLDATA::value_type(KEY_CMD, sCmd));
					mac.Format(_T("%s"), pShopInfo->m_objData.mac);
					pData->insert(APICALLDATA::value_type(_mac, mac));
					sensor_value.Format(_T("%.1f"), 0.0);
					pData->insert(APICALLDATA::value_type(_sensor_value, sensor_value));
					error_code.Format(_T("%s"), _T("RESOLVED"));
					pData->insert(APICALLDATA::value_type(_error_code, error_code));
					incident_type.Format(_T("%d"), INCIDENT_THRESHOLD);
					pData->insert(APICALLDATA::value_type(_incident_type, incident_type));
					level.Format(_T("%d"), 0);
					pData->insert(APICALLDATA::value_type(_level, level));
					g_pApiAgentDlg->AddAPI(pData);
					pShopInfo->m_bIncident = FALSE;
				}
			}
			*/
		}
	}
}

CHECKRESULT CKBChatServerDlg::CheckIncidentDevice(CStringList *pValueList, SENSORTYPE type,float *returValue)
{
	BOOL bWarring = FALSE;
	BOOL bError = FALSE;
	float nValue = 0;
	switch (type)
	{
	case SENSOR_LEAK:
	{
		for (int i = 0; i < pValueList->GetCount(); i++)
		{
			nValue = _wtof(pValueList->GetAt(pValueList->FindIndex(i)));
			if (g_pServerDlg->m_ConfigInfo.leak_R01_enable
				&& nValue == 1)
			{
				bError = TRUE;
				(*returValue) = nValue;
			}
		}
		break;
	}
	case SENSOR_TEMP:
	{

		for (int i = 0; i < pValueList->GetCount(); i++)
		{
			nValue = _wtof(pValueList->GetAt(pValueList->FindIndex(i)));
			if (g_pServerDlg->m_ConfigInfo.temp_R01_enable
				&& nValue >= g_pServerDlg->m_ConfigInfo.temp_R01_warning
				&& nValue < g_pServerDlg->m_ConfigInfo.temp_R01_error)
			{
				bWarring = TRUE;
				(*returValue) = nValue;
			}
			if (g_pServerDlg->m_ConfigInfo.temp_R01_enable
				&& nValue >= g_pServerDlg->m_ConfigInfo.temp_R01_error)
			{
				bError = TRUE;
				(*returValue) = nValue;
			}
		}
		break;

	}
	case SENSOR_HUMI:
	{
		for (int i = 0; i < pValueList->GetCount(); i++)
		{
			nValue = _wtof(pValueList->GetAt(pValueList->FindIndex(i)));
			if (g_pServerDlg->m_ConfigInfo.humi_R01_enable
				&& nValue >= g_pServerDlg->m_ConfigInfo.humi_R01_warning
				&& nValue < g_pServerDlg->m_ConfigInfo.humi_R01_error)
			{
				bWarring = TRUE;
				(*returValue) = nValue;
			}
			if (g_pServerDlg->m_ConfigInfo.humi_R01_enable
				&& nValue >= g_pServerDlg->m_ConfigInfo.humi_R01_error)
			{
				bError = TRUE;
				(*returValue) = nValue;
			}
		}
		break;
	}
	default:
		bWarring = FALSE;
		bError = FALSE;
	}
	if (bError == TRUE)
		return CHECK_ERROR;
	if (bWarring == TRUE)
		return CHECK_WARRING;
	return CHECK_NORMAL;
}

void CKBChatServerDlg::CheckValueDC(int nValue, SENSORTYPE type, CMemDC_* pDC,CRect *pRect)
{
	pDC->SetTextColor(RGB(200, 200, 200));
	BOOL bWarring = FALSE;
	BOOL bError = FALSE;
	switch (type)
	{
	case SENSOR_LEAK: 
	{
		if (g_pServerDlg->m_ConfigInfo.leak_R01_enable
			&& nValue >= g_pServerDlg->m_ConfigInfo.leak_R01_warning
			&& nValue < g_pServerDlg->m_ConfigInfo.leak_R01_error)
			bWarring = TRUE;
		if (g_pServerDlg->m_ConfigInfo.leak_R01_enable
			&& nValue >= g_pServerDlg->m_ConfigInfo.leak_R01_warning)
			bError = TRUE;
		break;
	}
	case SENSOR_TEMP:
	{
		if (g_pServerDlg->m_ConfigInfo.temp_R01_enable
			&& nValue >= g_pServerDlg->m_ConfigInfo.temp_R01_warning
			&& nValue < g_pServerDlg->m_ConfigInfo.temp_R01_error)
			bWarring = TRUE;
		if (g_pServerDlg->m_ConfigInfo.temp_R01_enable
			&& nValue >= g_pServerDlg->m_ConfigInfo.temp_R01_error)
			bError = TRUE;
		break;
	}
	case SENSOR_HUMI:
	{
		if (g_pServerDlg->m_ConfigInfo.humi_R01_enable
			&& nValue >= g_pServerDlg->m_ConfigInfo.humi_R01_warning
			&& nValue < g_pServerDlg->m_ConfigInfo.humi_R01_error)
			bWarring = TRUE;
		if (g_pServerDlg->m_ConfigInfo.humi_R01_enable
			&& nValue >= g_pServerDlg->m_ConfigInfo.humi_R01_error)
			bError = TRUE;
		break;
	}
	default:
		bWarring = FALSE;
		bError = FALSE;
	}

	if (bWarring == TRUE)
	{
		pDC->SetTextColor(RGB(255, 255, 0));
		CPen pen(PS_SOLID, 0, RGB(255, 255, 0));
		CPen* pOldPen = pDC->SelectObject(&pen);
		CPoint pt;
		pt.x = pRect->left - 120;
		pt.y = pRect->top - 58;
		pDC->MoveTo(pt);
		pDC->LineTo(CPoint(pt.x + 192, pt.y));
		pDC->LineTo(CPoint(pt.x + 192, pt.y + 208));
		pDC->LineTo(CPoint(pt.x, pt.y + 208));
		pDC->LineTo(CPoint(pt.x, pt.y));
	}
	if (bError == TRUE)
	{
		pDC->SetTextColor(RGB(255, 0, 0));
		CPen pen(PS_SOLID, 0, RGB(255, 0, 0));
		CPen* pOldPen = pDC->SelectObject(&pen);
		CPoint pt;
		pt.x = pRect->left - 120;
		pt.y = pRect->top - 58;
		pDC->MoveTo(pt);
		pDC->LineTo(CPoint(pt.x + 192, pt.y));
		pDC->LineTo(CPoint(pt.x + 192, pt.y + 208));
		pDC->LineTo(CPoint(pt.x, pt.y + 208));
		pDC->LineTo(CPoint(pt.x, pt.y));
	}
}

CString converValueSensor(float fValue)
{
	CString sValue;
	if (fValue == -999)
		return _T("");
	sValue.Format(_T("%.1f"), fValue);
	return sValue;
}

CFont* CKBChatServerDlg::DrawSensorInfo(CMemDC_ *pDC)
{
	//pDC->DrawText(_T("111"), CRect(194, 37, 1200, 57), DT_CENTER | DT_TOP | DT_SINGLELINE);
	CFont* pOldFont = pDC->SelectObject(&m_fontSensorValue);
	if (m_pCurShopInfo != NULL)
	{
		//pDC->DrawText(_T("222"), CRect(194, 57, 1200, 57), DT_CENTER | DT_TOP | DT_SINGLELINE);
		pDC->SetTextColor(RGB(200, 200, 200));
		CString leak_1_value;
		leak_1_value.Format(_T("%d"), m_pCurShopInfo->m_objData.leak_1_value);
		CheckValueDC(m_pCurShopInfo->m_objData.leak_1_value, SENSOR_LEAK, pDC, &m_rectSensor[10]);
		pDC->DrawText(leak_1_value, m_rectSensor[10], DT_CENTER | DT_TOP | DT_SINGLELINE);

		CString leak_2value;
		leak_2value.Format(_T("%d"), m_pCurShopInfo->m_objData.leak_2_value);
		CheckValueDC(m_pCurShopInfo->m_objData.leak_2_value, SENSOR_LEAK, pDC, &m_rectSensor[11]);
		pDC->DrawText(leak_2value, m_rectSensor[11], DT_CENTER | DT_TOP | DT_SINGLELINE);
		
		CString temp_1_value;
		temp_1_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.temp_1_value));
		CheckValueDC(m_pCurShopInfo->m_objData.temp_1_value, SENSOR_TEMP, pDC, &m_rectSensor[0]);
		pDC->DrawText(temp_1_value, m_rectSensor[0], DT_CENTER | DT_TOP | DT_SINGLELINE);
		//pDC->DrawText(temp_1_value, CRect(666, 17, 1200, 57), DT_CENTER | DT_TOP | DT_SINGLELINE);
		//pDC->DrawText(temp_1_value, CRect(363, 213, 413, 233), DT_CENTER | DT_TOP | DT_SINGLELINE);
		CString temp_2_value;
		temp_2_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.temp_2_value));
		CheckValueDC(m_pCurShopInfo->m_objData.temp_2_value, SENSOR_TEMP, pDC, &m_rectSensor[1]);
		pDC->DrawText(temp_2_value, m_rectSensor[1], DT_CENTER | DT_TOP | DT_SINGLELINE);
		CString temp_3_value;
		temp_3_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.temp_3_value));
		CheckValueDC(m_pCurShopInfo->m_objData.temp_3_value, SENSOR_TEMP, pDC, &m_rectSensor[2]);
		pDC->DrawText(temp_3_value, m_rectSensor[2], DT_CENTER | DT_TOP | DT_SINGLELINE);
		CString temp_4_value;
		temp_4_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.temp_4_value));
		CheckValueDC(m_pCurShopInfo->m_objData.temp_4_value, SENSOR_TEMP, pDC, &m_rectSensor[3]);
		pDC->DrawText(temp_4_value, m_rectSensor[3], DT_CENTER | DT_TOP | DT_SINGLELINE);


		CString humi_1_value;
		humi_1_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.humi_1_value));
		CheckValueDC(m_pCurShopInfo->m_objData.humi_1_value, SENSOR_HUMI, pDC, &m_rectSensor[5]);
		pDC->DrawText(humi_1_value, m_rectSensor[5], DT_CENTER | DT_TOP | DT_SINGLELINE);
		CString humi_2_value;
		humi_2_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.humi_2_value));
		CheckValueDC(m_pCurShopInfo->m_objData.humi_2_value, SENSOR_HUMI, pDC, &m_rectSensor[6]);
		pDC->DrawText(humi_2_value, m_rectSensor[6], DT_CENTER | DT_TOP | DT_SINGLELINE);
		CString humi_3_value;
		humi_3_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.humi_3_value));
		CheckValueDC(m_pCurShopInfo->m_objData.humi_3_value, SENSOR_HUMI, pDC, &m_rectSensor[7]);
		pDC->DrawText(humi_3_value, m_rectSensor[7], DT_CENTER | DT_TOP | DT_SINGLELINE);
		CString humi_4_value;
		humi_4_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.humi_4_value));
		CheckValueDC(m_pCurShopInfo->m_objData.humi_4_value, SENSOR_HUMI, pDC, &m_rectSensor[8]);
		pDC->DrawText(humi_4_value, m_rectSensor[8], DT_CENTER | DT_TOP | DT_SINGLELINE);
	}
	pOldFont = pDC->SelectObject(&m_fontSensorValue_detail);
	if (m_pCurShopInfo != NULL && m_pCurShopInfo->m_objData.dwCount > 0)
	{
		pDC->SetTextColor(RGB(200, 200, 200));

		CString temp_1_value;
		temp_1_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.temp_1_value_total / m_pCurShopInfo->m_objData.dwCount));
		pDC->DrawText(temp_1_value, CRect(m_rectSensor[0].left - 98, m_rectSensor[0].top + 95, m_rectSensor[0].left - 98 + 40, m_rectSensor[0].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);
		temp_1_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.temp_1_value_max));
		pDC->DrawText(temp_1_value, CRect(m_rectSensor[0].left - 46, m_rectSensor[0].top + 95, m_rectSensor[0].left - 46 + 40, m_rectSensor[0].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);
		temp_1_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.temp_1_value_min));
		pDC->DrawText(temp_1_value, CRect(m_rectSensor[0].left + 10, m_rectSensor[0].top + 95, m_rectSensor[0].left + 10 + 40, m_rectSensor[0].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);


		CString temp_2_value;
		temp_2_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.temp_2_value_total / m_pCurShopInfo->m_objData.dwCount));
		pDC->DrawText(temp_2_value, CRect(m_rectSensor[1].left - 98, m_rectSensor[1].top + 95, m_rectSensor[1].left - 98 + 40, m_rectSensor[1].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);
		temp_2_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.temp_2_value_max));
		pDC->DrawText(temp_2_value, CRect(m_rectSensor[1].left - 46, m_rectSensor[1].top + 95, m_rectSensor[1].left - 46 + 40, m_rectSensor[1].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);
		temp_2_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.temp_2_value_min));
		pDC->DrawText(temp_2_value, CRect(m_rectSensor[1].left + 10, m_rectSensor[1].top + 95, m_rectSensor[1].left + 10 + 40, m_rectSensor[1].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);

		CString temp_3_value;
		temp_3_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.temp_3_value_total / m_pCurShopInfo->m_objData.dwCount));
		pDC->DrawText(temp_3_value, CRect(m_rectSensor[2].left - 98, m_rectSensor[2].top + 95, m_rectSensor[2].left - 98 + 40, m_rectSensor[2].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);
		temp_3_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.temp_3_value_max));
		pDC->DrawText(temp_3_value, CRect(m_rectSensor[2].left - 46, m_rectSensor[2].top + 95, m_rectSensor[2].left - 46 + 40, m_rectSensor[2].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);
		temp_3_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.temp_3_value_min));
		pDC->DrawText(temp_3_value, CRect(m_rectSensor[2].left + 10, m_rectSensor[2].top + 95, m_rectSensor[2].left + 10 + 40, m_rectSensor[2].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);


		CString temp_4_value;
		temp_4_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.temp_4_value_total / m_pCurShopInfo->m_objData.dwCount));
		pDC->DrawText(temp_4_value, CRect(m_rectSensor[3].left - 98, m_rectSensor[3].top + 95, m_rectSensor[3].left - 98 + 40, m_rectSensor[3].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);
		temp_4_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.temp_4_value_max));
		pDC->DrawText(temp_4_value, CRect(m_rectSensor[3].left - 46, m_rectSensor[3].top + 95, m_rectSensor[3].left - 46 + 40, m_rectSensor[3].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);
		temp_4_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.temp_4_value_min));
		pDC->DrawText(temp_4_value, CRect(m_rectSensor[3].left + 10, m_rectSensor[3].top + 95, m_rectSensor[3].left + 10 + 40, m_rectSensor[3].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);

		///////////

		CString humi_1_value;
		humi_1_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.humi_1_value_total / m_pCurShopInfo->m_objData.dwCount));
		pDC->DrawText(humi_1_value, CRect(m_rectSensor[5].left - 98, m_rectSensor[5].top + 95, m_rectSensor[5].left - 98 + 40, m_rectSensor[5].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);
		humi_1_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.humi_1_value_max));
		pDC->DrawText(humi_1_value, CRect(m_rectSensor[5].left - 46, m_rectSensor[5].top + 95, m_rectSensor[5].left - 46 + 40, m_rectSensor[5].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);
		humi_1_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.humi_1_value_min));
		pDC->DrawText(humi_1_value, CRect(m_rectSensor[5].left + 10, m_rectSensor[5].top + 95, m_rectSensor[5].left + 10 + 40, m_rectSensor[5].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);


		CString humi_2_value;
		humi_2_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.humi_2_value_total / m_pCurShopInfo->m_objData.dwCount));
		pDC->DrawText(humi_2_value, CRect(m_rectSensor[6].left - 98, m_rectSensor[6].top + 95, m_rectSensor[6].left - 98 + 40, m_rectSensor[6].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);
		humi_2_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.humi_2_value_max));
		pDC->DrawText(humi_2_value, CRect(m_rectSensor[6].left - 46, m_rectSensor[6].top + 95, m_rectSensor[6].left - 46 + 40, m_rectSensor[6].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);
		humi_2_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.humi_2_value_min));
		pDC->DrawText(humi_2_value, CRect(m_rectSensor[6].left + 10, m_rectSensor[6].top + 95, m_rectSensor[6].left + 10 + 40, m_rectSensor[6].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);

		CString humi_3_value;
		humi_3_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.humi_3_value_total / m_pCurShopInfo->m_objData.dwCount));
		pDC->DrawText(humi_3_value, CRect(m_rectSensor[7].left - 98, m_rectSensor[7].top + 95, m_rectSensor[7].left - 98 + 40, m_rectSensor[7].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);
		humi_3_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.humi_3_value_max));
		pDC->DrawText(humi_3_value, CRect(m_rectSensor[7].left - 46, m_rectSensor[7].top + 95, m_rectSensor[7].left - 46 + 40, m_rectSensor[7].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);
		humi_3_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.humi_3_value_min));
		pDC->DrawText(humi_3_value, CRect(m_rectSensor[7].left + 10, m_rectSensor[7].top + 95, m_rectSensor[7].left + 10 + 40, m_rectSensor[7].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);


		CString humi_4_value;
		humi_4_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.humi_4_value_total / m_pCurShopInfo->m_objData.dwCount));
		pDC->DrawText(humi_4_value, CRect(m_rectSensor[8].left - 98, m_rectSensor[8].top + 95, m_rectSensor[8].left - 98 + 40, m_rectSensor[8].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);
		humi_4_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.humi_4_value_max));
		pDC->DrawText(humi_4_value, CRect(m_rectSensor[8].left - 46, m_rectSensor[8].top + 95, m_rectSensor[8].left - 46 + 40, m_rectSensor[8].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);
		humi_4_value.Format(_T("%s"), converValueSensor(m_pCurShopInfo->m_objData.humi_4_value_min));
		pDC->DrawText(humi_4_value, CRect(m_rectSensor[8].left + 10, m_rectSensor[8].top + 95, m_rectSensor[8].left + 10 + 40, m_rectSensor[8].top + 95 + 14), DT_CENTER | DT_TOP | DT_SINGLELINE);


	}
	return pOldFont;
}

CFont* CKBChatServerDlg::DrawItemList(CDC* pDC, CPtrList* pItemList)
{
	CFont* pOldFont = pDC->SelectObject(&m_fontLeftShopName);

	CBrush* oldBrush;
	oldBrush = pDC->SelectObject(&m_blushInActive);
	int nCur = 0;
	for (int i = 0; i < pItemList->GetCount(); i++)
	{
		CAreaInfo* pAreaInfo = (CAreaInfo*)pItemList->GetAt(pItemList->FindIndex(i));
		if (i > 9)
			break;
		if (pAreaInfo != NULL)
		{
			pDC->FillSolidRect(CRect(g_rectListItemInfo[i].left-1, g_rectListItemInfo[i].top - 1, g_rectListItemInfo[i].right+1, g_rectListItemInfo[i].bottom+1), RGB(28, 33, 39));
			if (m_pCurShopInfo != NULL)
			{
				if (GetAreaInfo(m_pCurShopInfo->m_objData.mac) == pAreaInfo)
					pDC->FillSolidRect(g_rectListItemInfo[i], RGB(42, 44, 65));
				else
					pDC->FillSolidRect(g_rectListItemInfo[i], RGB(60, 65, 85));
			}
			pDC->SetTextColor(RGB(255, 255, 255));
			CRect rectDate = CRect(g_rectListItemInfo[i].left + 4, g_rectListItemInfo[i].top + 6, g_rectListItemInfo[i].right - 2, g_rectListItemInfo[i].bottom - 18);
			pDC->DrawText(pAreaInfo->strShopName, -1, rectDate, DT_CENTER | DT_WORDBREAK);
			if (pAreaInfo->m_bActive == TRUE)
			{
				oldBrush = pDC->SelectObject(&m_blushActive);
				pDC->Ellipse(g_rectListItemInfo[i].left + 65, g_rectListItemInfo[i].bottom - 20, g_rectListItemInfo[i].left + 65 + 15, g_rectListItemInfo[i].bottom - 5);
			}
			else
			{
				oldBrush = pDC->SelectObject(&m_blushInActive);
				pDC->Ellipse(g_rectListItemInfo[i].left + 65, g_rectListItemInfo[i].bottom - 20, g_rectListItemInfo[i].left + 65 + 15, g_rectListItemInfo[i].bottom - 5);
				
			}
			//pDC->Ellipse(g_rectListItemInfo[i].left + 85, g_rectListItemInfo[i].bottom - 20, g_rectListItemInfo[i].left + 85 + 15, g_rectListItemInfo[i].bottom - 5);
				
		}
		nCur += 1;
	}
	pDC->SelectObject(oldBrush);

	return pOldFont;
}

void CKBChatServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == TIMER_ACTIVE_LAMP)
	{
		for(int i=0;i<SHOP_INFO_COUNT;i++)
			InvalidateRect(g_rectListItemInfo[i], FALSE);
		for (int i = 0; i < m_listArea.GetCount(); i++)
		{
			CAreaInfo* pAreaInfo = (CAreaInfo*)m_listArea.GetAt(m_listArea .FindIndex(i));
			if (pAreaInfo != NULL)
				pAreaInfo->m_bActive = FALSE;
		}
	}
	else if (nIDEvent == TIMER_SENSOR_DATA_SEND)
	{
		KillTimer(TIMER_SENSOR_DATA_SEND);
		SendSensorData();
		SetTimer(TIMER_SENSOR_DATA_SEND, m_ConfigInfo.data_upload_second, NULL);
	}
	else if (nIDEvent == TIMER_OPCUA_LOAD)
	{
		LoadOpcUaData();
	}
	else if (nIDEvent == TIMER_CHECK_CONFIG)
	{
		KillTimer(TIMER_CHECK_CONFIG);
		SendCheckConfig();
		SetTimer(TIMER_CHECK_CONFIG, m_ConfigInfo.config_check_second, NULL);
	}
	else if (nIDEvent == TIMER_INCIDENT)
	{
		KillTimer(TIMER_INCIDENT);
		CheckIncident();
		SetTimer(TIMER_INCIDENT, 30000, NULL);
	}
	else if (nIDEvent == TIMER_RESOURCE)
	{
		/*
		KillTimer(TIMER_RESOURCE);
		CheckResource();
		SetTimer(TIMER_RESOURCE, m_ConfigInfo.data_upload_second, NULL);
		*/
	}
	else if (nIDEvent == TIMER_RESTART)
	{
		EndDialog(IDOK);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CKBChatServerDlg::OnBnClickedBtnLog()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CKBChatServerDlg::LoadOpcUaData()
{
	USES_CONVERSION;
	CString strConfig;
	strConfig.Format(_T("%s\\opcua_c.ini"), GetProgramPathW());
	CString strKey = _T("INFO");
	CString strValue;
	char sValue[1024];
	::GetPrivateProfileString(strKey, _T("count"), _T("1"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
	int nShopCount = _wtoi(strValue);
	CString sNum;
	CString sMac;
	CString sData;
	for (int i = 0; i < nShopCount; i++)
	{
		sNum.Format(_T("sensor_%d"), i + 1);
		::GetPrivateProfileString(strKey, sNum, _T(""), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
		CString strData = _T("");
		strData.Format(_T("%s"), strValue);
		KBPKT_DATA_OBJ dataObj;
		dataObj.setDataParser(strData);
		SensorDataInsert(&dataObj);
	}
	Invalidate(FALSE);
}

void CKBChatServerDlg::OnMenuTrayExit()
{
	EndDialog(IDOK);
}

void CKBChatServerDlg::OnMenuTrayLog()
{
	ShowLoginDlg();
}

#define DESIRED_WINSOCK_VERSION        0x0101
#define MINIMUM_WINSOCK_VERSION        0x0001
CString CKBChatServerDlg::GetIpAddress()
{
	WSADATA wsadata;
	CString strIP; // 이 변수에 IP주소가 저장된다.
	strIP = "";

	if (!WSAStartup(DESIRED_WINSOCK_VERSION, &wsadata))
	{
		if (wsadata.wVersion >= MINIMUM_WINSOCK_VERSION)
		{
			HOSTENT* p_host_info;
			IN_ADDR in;
			char host_name[128] = { 0, };

			gethostname(host_name, 128);
			p_host_info = gethostbyname(host_name);

			if (p_host_info != NULL)
			{
				for (int i = 0; p_host_info->h_addr_list[i]; i++)
				{
					memcpy(&in, p_host_info->h_addr_list[i], 4);
					strIP = inet_ntoa(in);
				}
			}
		}
		WSACleanup();
	}
	return strIP;

}

void CKBChatServerDlg::Init_Cpu() {
	PdhOpenQuery(NULL, NULL, &cpuQuery);
	PdhAddCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
	PdhCollectQueryData(cpuQuery);
}

double CKBChatServerDlg::getCpuCurrentValue() {
	PDH_FMT_COUNTERVALUE counterVal;

	PdhCollectQueryData(cpuQuery);
	PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
	return counterVal.doubleValue;
}


void CKBChatServerDlg::CheckResource()
{
	CString sCmd, ipaddress, cpu_value, ram_avail_value, ram_phys_value;
	int availableMem , physicalMem = 0;
	getRAMStatus(availableMem, physicalMem);
	sCmd.Format(_T("%d"), API_RESOURCE);
	ipaddress = GetIpAddress();
	APICALLDATA* pData = new APICALLDATA;
	pData->insert(APICALLDATA::value_type(KEY_CMD, sCmd));
	pData->insert(APICALLDATA::value_type(_ipaddress, ipaddress));
	cpu_value.Format(_T("%.1f"), getCpuCurrentValue());
	pData->insert(APICALLDATA::value_type(_cpu_value, cpu_value));
	ram_avail_value.Format(_T("%d"), availableMem);
	pData->insert(APICALLDATA::value_type(_ram_avail_value, ram_avail_value));
	ram_phys_value.Format(_T("%d"), physicalMem);
	pData->insert(APICALLDATA::value_type(_ram_phys_value, ram_phys_value));
	g_pApiAgentDlg->AddAPI(pData);
}

void CKBChatServerDlg::getRAMStatus(int& availableMem, int& physicalMem) {
	//메모리값
	MEMORYSTATUSEX MemoryStatus = { 0 };
	MemoryStatus.dwLength = sizeof(MemoryStatus);
	::GlobalMemoryStatusEx(&MemoryStatus);

	availableMem = (int)((MemoryStatus.ullTotalPhys - MemoryStatus.ullAvailPhys) / (1024 * 1024));
	physicalMem = (int)((MemoryStatus.ullTotalPhys) / (1024 * 1024));
}