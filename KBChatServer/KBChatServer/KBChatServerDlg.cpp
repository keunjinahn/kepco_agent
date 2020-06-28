
// KBChatServerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "KBChatServer.h"
#include "KBChatServerDlg.h"
#include "afxdialogex.h"
#include "MsgrSvr.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CKBChatServerDlg ��ȭ ����



CKBChatServerDlg *g_pServerDlg = NULL;
CKBChatServerDlg::CKBChatServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKBChatServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME2);
	m_fontLocation.CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, _T("����ü"));
	m_fontShop.CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, _T("����ü"));
	m_fontSensorValue.CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, _T("����ü"));
	m_pLogWindow = NULL;
	m_pCurShopInfo = NULL;


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
END_MESSAGE_MAP()


// CKBChatServerDlg �޽��� ó����

BOOL CKBChatServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	g_pServerDlg = this;
	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	WSADATA wsaData;
    if (WSAStartup (MAKEWORD(2, 0), &wsaData) != SOCKET_ERROR)
	{
		g_MsgrSvr.Start();
	}
	LoadConfig();
	InitControl();
	MoveWindow(CRect(0,0,1283, 869));
	CenterWindow();
	
	SetWindowText(_T("KConnectionServer"));

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CKBChatServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
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
		strLocation.Format(_T("%s ��������"), m_ConfigInfo.locationname);
		dc.DrawText(strLocation, CRect(194,17,1200,57), DT_CENTER | DT_TOP | DT_SINGLELINE);
		pOldFont = dc.SelectObject(&m_fontShop);
		CString strShop;
		strShop.Format(_T("%s"), m_ConfigInfo.listShop.GetHead());
		dc.DrawText(strShop, CRect(839,104, 1107, 131), DT_CENTER | DT_TOP | DT_SINGLELINE);
		CString strTitle;
		//strTitle.Format(_T("%s"), _T("D-TRS ��������"));
		strTitle.Format(_T("%s"), _T("ȣ���� MOT"));
		dc.DrawText(strTitle, CRect(17, 17, 175, 44), DT_CENTER | DT_TOP | DT_SINGLELINE);

		pOldFont = DrawSensorInfo(&dc);

		dc.SelectObject(pOldFont);

		

		ReleaseDC(dc);
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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

void CKBChatServerDlg::SensorDataInsert(KBPKT_DATA_OBJ *pObj)
{
	USES_CONVERSION;
	char* host = "127.0.0.1";
	char* userId = "root";
	char* passwd = "1234";
	char* dbName = "kcollectiondb";
	int port = 3306;
	mysql_init(&m_Mysql);
	if (mysql_real_connect(&m_Mysql, host, userId, passwd, dbName, port, NULL, 0) == NULL)
	{
		return;
	}
	else {
		mysql_query(&m_Mysql, "set names euckr"); // �ѱ� ���� ����
	}

	CString strQuery = _T("");
	strQuery.Format(_T("insert into tb_collection_hist(device_no, sensor_state,leak_1_value,leak_2_value,temp_1_value,temp_2_value,temp_3_value,temp_4_value \
		,humi_1_value, humi_2_value, humi_3_value, humi_4_value) values('%d', '%d', '%d', '%d', '%.1f', '%.1f', '%.1f', '%.1f', '%.1f', '%.1f', '%.1f', '%.1f')")
		, pObj->device_no, pObj->sensor_state, pObj->leak_1_value, pObj->leak_2_value, pObj->temp_1_value, pObj->temp_2_value, pObj->temp_3_value, pObj->temp_4_value
		, pObj->humi_1_value, pObj->humi_2_value, pObj->humi_3_value, pObj->humi_4_value);

	if (mysql_query(&m_Mysql, W2A(strQuery))) {
		return;
	}

	int check = _CrtDumpMemoryLeaks();

	mysql_close(&m_Mysql);

	CShopInfo* pShopInfo = GetShopInfo(pObj->mac);
	if (pShopInfo != NULL)
	{
		pShopInfo->m_objData.device_no = pObj->device_no;
		pShopInfo->m_objData.sensor_state = pObj->sensor_state;
		pShopInfo->m_objData.leak_1_value = pObj->leak_1_value;
		pShopInfo->m_objData.leak_2_value = pObj->leak_2_value;
		pShopInfo->m_objData.temp_1_value = pObj->temp_1_value;
		pShopInfo->m_objData.temp_2_value = pObj->temp_2_value;
		pShopInfo->m_objData.temp_3_value = pObj->temp_3_value;
		pShopInfo->m_objData.temp_4_value = pObj->temp_4_value;
		pShopInfo->m_objData.humi_1_value = pObj->humi_1_value;
		pShopInfo->m_objData.humi_2_value = pObj->humi_2_value;
		pShopInfo->m_objData.humi_3_value = pObj->humi_3_value;
		pShopInfo->m_objData.humi_4_value = pObj->humi_4_value;
	}
	Invalidate(false);
}

CShopInfo* CKBChatServerDlg::GetShopInfo(CString srcMac)
{
	POSITION pos = m_ConfigInfo.m_pShowList->GetHeadPosition();
	while (pos)
	{
		CShopInfo* pShopInfo = (CShopInfo*)m_ConfigInfo.m_pShowList->GetNext(pos);
		if (pShopInfo->m_objData.mac == srcMac)
			return pShopInfo;
	}
	return NULL;
}

void CKBChatServerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	
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
	// TODO: Add your message handler code here
}


void CKBChatServerDlg::OnBnClickedBtnExit()
{
	if(AfxMessageBox(L"���α׷��� �����Ͻðڽ��ϱ�?", MB_YESNO) == IDYES)
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
	

	m_pLogWindow = new LogWindow();
	m_pLogWindow->Create(IDD_LOGWINDOW, this);
	m_pLogWindow->ShowWindow(SW_HIDE);
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (point.y < 91)
	{
		POINT pt;
		GetCursorPos(&pt);
		SendMessage(WM_LBUTTONUP, 0, MAKELPARAM(point.x, point.y));
		SendMessage(WM_SYSCOMMAND, 0xf012, MAKELPARAM(pt.x, pt.y));
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
	::GetPrivateProfileString(strKey, _T("locationname"), _T("111"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
	m_ConfigInfo.locationname.Format(_T("%s"),strValue);
	::GetPrivateProfileString(strKey, _T("locationcode"), _T("1"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
	m_ConfigInfo.locationcode.Format(_T("%s"), strValue);

	::GetPrivateProfileString(strKey, _T("shopcount"), _T("1"), strValue.GetBuffer(256), 256, strConfig.GetBuffer(256));
	int nShopCount = _wtoi(strValue);
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
}


void CKBChatServerDlg::ShowLoginDlg()
{
	if (m_pLogWindow)
	{
		m_pLogWindow->ShowWindow(SW_SHOW);
	}
}



void CKBChatServerDlg::LogMessage(LOGTYPE typeLog, CString strMsg)
{
	if (m_pLogWindow)
	{
		m_pLogWindow->LogMessage(typeLog,strMsg);
	}
}

void CKBChatServerDlg::CheckValueDC(int nValue, SENSORTYPE type, CMemDC_* pDC,CRect *pRect)
{
	pDC->SetTextColor(RGB(200, 200, 200));
	BOOL bWarring = FALSE;
	switch (type)
	{
	case SENSOR_LEAK: if (nValue > 0) bWarring = TRUE; break;
	case SENSOR_TEMP: if (nValue > 28) bWarring = TRUE; break;
	case SENSOR_HUMI: if (nValue > 80) bWarring = TRUE; break;
	default:
		bWarring = FALSE;
	}

	if (bWarring == TRUE)
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

CFont* CKBChatServerDlg::DrawSensorInfo(CMemDC_ *pDC)
{
	CFont* pOldFont = pDC->SelectObject(&m_fontSensorValue);
	if (m_pCurShopInfo != NULL)
	{
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
		temp_1_value.Format(_T("%.1f"), m_pCurShopInfo->m_objData.temp_1_value);
		CheckValueDC(m_pCurShopInfo->m_objData.temp_1_value, SENSOR_TEMP, pDC, &m_rectSensor[0]);
		pDC->DrawText(temp_1_value, m_rectSensor[0], DT_CENTER | DT_TOP | DT_SINGLELINE);
		CString temp_2_value;
		temp_2_value.Format(_T("%.1f"), m_pCurShopInfo->m_objData.temp_2_value);
		CheckValueDC(m_pCurShopInfo->m_objData.temp_2_value, SENSOR_TEMP, pDC, &m_rectSensor[1]);
		pDC->DrawText(temp_2_value, m_rectSensor[1], DT_CENTER | DT_TOP | DT_SINGLELINE);
		CString temp_3_value;
		temp_3_value.Format(_T("%.1f"), m_pCurShopInfo->m_objData.temp_3_value);
		CheckValueDC(m_pCurShopInfo->m_objData.temp_3_value, SENSOR_TEMP, pDC, &m_rectSensor[2]);
		pDC->DrawText(temp_3_value, m_rectSensor[2], DT_CENTER | DT_TOP | DT_SINGLELINE);
		CString temp_4_value;
		temp_4_value.Format(_T("%.1f"), m_pCurShopInfo->m_objData.temp_4_value);
		CheckValueDC(m_pCurShopInfo->m_objData.temp_4_value, SENSOR_TEMP, pDC, &m_rectSensor[3]);
		pDC->DrawText(temp_4_value, m_rectSensor[3], DT_CENTER | DT_TOP | DT_SINGLELINE);


		CString humi_1_value;
		humi_1_value.Format(_T("%.1f"), m_pCurShopInfo->m_objData.humi_1_value);
		CheckValueDC(m_pCurShopInfo->m_objData.humi_1_value, SENSOR_HUMI, pDC, &m_rectSensor[5]);
		pDC->DrawText(humi_1_value, m_rectSensor[5], DT_CENTER | DT_TOP | DT_SINGLELINE);
		CString humi_2_value;
		humi_2_value.Format(_T("%.1f"), m_pCurShopInfo->m_objData.humi_2_value);
		CheckValueDC(m_pCurShopInfo->m_objData.humi_2_value, SENSOR_HUMI, pDC, &m_rectSensor[6]);
		pDC->DrawText(humi_2_value, m_rectSensor[6], DT_CENTER | DT_TOP | DT_SINGLELINE);
		CString humi_3_value;
		humi_3_value.Format(_T("%.1f"), m_pCurShopInfo->m_objData.humi_3_value);
		CheckValueDC(m_pCurShopInfo->m_objData.humi_3_value, SENSOR_HUMI, pDC, &m_rectSensor[7]);
		pDC->DrawText(humi_3_value, m_rectSensor[7], DT_CENTER | DT_TOP | DT_SINGLELINE);
		CString humi_4_value;
		humi_4_value.Format(_T("%.1f"), m_pCurShopInfo->m_objData.humi_4_value);
		CheckValueDC(m_pCurShopInfo->m_objData.humi_4_value, SENSOR_HUMI, pDC, &m_rectSensor[8]);
		pDC->DrawText(humi_4_value, m_rectSensor[8], DT_CENTER | DT_TOP | DT_SINGLELINE);


	}
	return pOldFont;
}