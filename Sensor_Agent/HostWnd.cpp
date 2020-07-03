// HostWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Sensor_Agent.h"
#include "HostWnd.h"
#include "Sensor_AgentDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CHostWnd
CHostWnd::CHostWnd()
{
	m_bOnline = FALSE;
	m_bAckCheck = TRUE;
	g_tcpLoginCehckCnt = 0;
	m_dwFailServerConnectCount = 0;
}



CHostWnd::~CHostWnd()
{
	DeleteSvrSock();
}

CHostWnd g_HostWnd;

BEGIN_MESSAGE_MAP(CHostWnd, CWnd)
	//{{AFX_MSG_MAP(CHostWnd)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE( LCWM_SOCKRLT_CONN, OnSockrltConn )
	ON_MESSAGE( LCWM_SOCKRLT_RCV, OnSockrltRcv )

	ON_MESSAGE( LCWM_SOCKRLT_CLOSE, OnSockrltClose )
	ON_MESSAGE( LCWM_DEL_CONSOCK, OnDelConsock )
	ON_MESSAGE( LCWM_QUIT, OnLcwmQuit )
	ON_MESSAGE( LCWM_SEND, OnLcwmSend )
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHostWnd message handlers

LRESULT CHostWnd::OnSockrltConn(WPARAM wParam, LPARAM lParam)
{
	CConSock *pSoc = (CConSock *) lParam;
	PostThreadMessage(GetCurrentThreadId(), LCWM_LOGINRLT, 999, 0);
	return 0;
}

LRESULT CHostWnd::OnSockrltRcv(WPARAM wParam, LPARAM lParam)
{
	USES_CONVERSION;
	ASTRY
	{
		if (wParam == 0)
		{
			CConSock *pSoc = (CConSock *) lParam;
			switch (pSoc->m_pRsHdr->cmd)
			{
			case SOCK_CMD_LOGIN_RS :
				{
					/*
					KBPKT_LOGIN_RS *pPkt = (KBPKT_LOGIN_RS *) pSoc->m_pRsData;
					if(pPkt->flags == 1)
					{
						m_bOnline = TRUE;
						SetTimer(0,5000,NULL);
					}
					else if(pPkt->flags == 9)
					{
						m_bOnline = FALSE;
						DeleteSvrSock();
						g_pMainDlg->KillConnectTimer();
						if(g_pMainDlg->ShowMessageBox(9,_T("KBizTalk"),_T("중복 로그인으로 프로그램을 종료합니다.")))
						{
							g_pMainDlg->PostMessageW(WM_CLOSE);
						}
					}
					else
					{
						m_bOnline = FALSE;
						DeleteSvrSock();
						g_pMainDlg->ShowMessageBox(0,_T("KBizTalk"),_T("이미 서버에 접속중입니다."),SHOWMESSAGE_MODALESS,1000);
					}
					*/
				}
				break;
			}
		}
	}
	ASCATCH
	{

	}
	return 0;
}

LRESULT CHostWnd::OnSockrltClose(WPARAM wParam, LPARAM lParam)
{
	DeleteSvrSock();
	m_bOnline = FALSE;
	m_dwFailServerConnectCount++;
	return 0;
}

LRESULT CHostWnd::OnDelConsock(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CHostWnd::OnLcwmQuit(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CHostWnd::Ping()
{
	USES_CONVERSION;
	/*
	int iSize = sizeof(KBPKT_HDR) + sizeof(KBPKT_PING);
	char *pbuf = new char[iSize];
	ZeroMemory(pbuf, iSize);
	KBPKT_HDR *pHdr = (KBPKT_HDR *) pbuf;
	pHdr->cmd = 5;
	strcpy(pHdr->TMI_IDX,W2A(theApp.m_MyMember.GetTMI_IDX()));
	pHdr->size = sizeof(KBPKT_PING);
	KBPKT_PING *pPkt = (KBPKT_PING *) (pbuf + sizeof(KBPKT_HDR));
	pPkt->type = 0;
	SendToSvr(pbuf, iSize);
	*/
}

CConSock *CHostWnd::SendToSvr(char *pbuf, int cb, int threshold)
{

	ASTRY
	{
		if (!m_pSvrSock)
		{
			delete [] pbuf;
			if(m_bOnline)
			{
				//LoginToSvr();
				m_bOnline = FALSE;
				SetTimer(0,3000,NULL);
			}
			return NULL;
		}

		KBPKT_HDR *pHdr = (KBPKT_HDR *) pbuf;
		if (threshold < 0)
			m_pSvrSock->AdvSend(pbuf, cb);
		else
		if (threshold > m_pSvrSock->GetPendingCnt())
			m_pSvrSock->AdvSend(pbuf, cb);
		else
		{
			delete [] pbuf;
		}
	}
	ASCATCH
	{
	}
	return m_pSvrSock;
}

void CHostWnd::DeleteSvrSock()
{
	if(m_pSvrSock)
	{
		delete m_pSvrSock;
		m_pSvrSock = NULL;
	}

}


char * convertValue(float fValue)
{
	USES_CONVERSION;
	CString sValue;
	float fTemp = int(fValue);
	if(fValue >=0)
		sValue.Format(_T("+%02d%d"), (int)fValue, (int)((fValue - fTemp) * 10));
	else
		sValue.Format(_T("%02d%d"), (int)fValue, (int)((fValue - fTemp) * 10));
	return W2A(sValue);

}
void CHostWnd::SendShopData(CShopInfo* pShopInfo,CString serverIp,int port)
{
	USES_CONVERSION;
	try
	{
		int iSize = sizeof(KBPKT_HDR) + sizeof(KBPKT_DATA);
		char* pbuf = new char[iSize];

		ZeroMemory(pbuf, iSize);
		KBPKT_HDR* pHdr = (KBPKT_HDR*)pbuf;
		pHdr->cmd = '5';
		memcpy(pHdr->mac, W2A(pShopInfo->m_objData.mac), 12);
		memcpy(pHdr->size, "40", 2);
		KBPKT_DATA* pdata = (KBPKT_DATA*)(pbuf + sizeof(KBPKT_HDR));
		pdata->ack = pShopInfo->m_objData.ack;
		CString strDeviceNo;
		strDeviceNo.Format(_T("%02d"), pShopInfo->m_objData.device_no);
		memcpy(pdata->device_no, W2A(strDeviceNo), 2);
		pdata->leak_1_value = char(pShopInfo->m_objData.leak_1_value);
		pdata->leak_2_value = char(pShopInfo->m_objData.leak_2_value);

		memcpy(pdata->temp_1_value, convertValue(pShopInfo->m_objData.temp_1_value), 4);
		memcpy(pdata->temp_2_value, convertValue(pShopInfo->m_objData.temp_2_value), 4);
		memcpy(pdata->temp_3_value, convertValue(pShopInfo->m_objData.temp_3_value), 4);
		memcpy(pdata->temp_4_value, convertValue(pShopInfo->m_objData.temp_4_value), 4);

		memcpy(pdata->humi_1_value, convertValue(pShopInfo->m_objData.humi_1_value), 4);
		memcpy(pdata->humi_2_value, convertValue(pShopInfo->m_objData.humi_2_value), 4);
		memcpy(pdata->humi_3_value, convertValue(pShopInfo->m_objData.humi_3_value), 4);
		memcpy(pdata->humi_4_value, convertValue(pShopInfo->m_objData.humi_4_value), 4);
		pdata->etx = pShopInfo->m_objData.etx;
		if (m_pSvrSock)
		{
			DeleteSvrSock();
		}
		if (!m_pSvrSock)
		{
			m_pSvrSock = new CConSock;
			BOOL bCheck = m_pSvrSock->Create(pbuf, iSize, this);
		}
		if (m_pSvrSock)
		{
			BOOL bConnect = m_pSvrSock->Connect(serverIp, port);
		}
	}
	catch (...)
	{

	}
}
BOOL CHostWnd::LoginToSvr(BOOL bForceEnter)
{
	USES_CONVERSION;
	try
	{
		int iSize = sizeof(KBPKT_HDR) + sizeof(KBPKT_DATA);
		char *pbuf = new char[iSize];

		ZeroMemory(pbuf, iSize);
		KBPKT_HDR *pHdr = (KBPKT_HDR *) pbuf; 
		pHdr->cmd = '5';
		memcpy(pHdr->mac,"9ca525a33f4c",12);
		memcpy(pHdr->size, "40",2);
		KBPKT_DATA *pdata = (KBPKT_DATA*) (pbuf + sizeof(KBPKT_HDR));
		pdata->ack = '0';
		memcpy(pdata->device_no, "01", 2);
		pdata->leak_1_value = '0';
		pdata->leak_2_value = '0';
		memcpy(pdata->temp_1_value, "+201",4);
		memcpy(pdata->temp_2_value, "+201", 4);
		memcpy(pdata->temp_3_value, "+201", 4);
		memcpy(pdata->temp_4_value, "+201", 4);
		memcpy(pdata->humi_1_value, "+301", 4);
		memcpy(pdata->humi_2_value, "+301", 4);
		memcpy(pdata->humi_3_value, "+301", 4);
		memcpy(pdata->humi_4_value, "+301", 4);
		pdata->etx = '0';
		if(m_pSvrSock)
		{
			DeleteSvrSock();
		}
		if (!m_pSvrSock)
		{
			m_pSvrSock = new CConSock;
			BOOL bCheck = m_pSvrSock->Create(pbuf, iSize, this);
		}
		if(m_pSvrSock)
		{
			BOOL bConnect = m_pSvrSock->Connect(_T("localhost"),8080);
		}
	}
	catch (...) 
	{

	}
	return TRUE;
}

void CHostWnd::OnDestroy() 
{
	DeleteSvrSock();
	CWnd::OnDestroy();
}

LRESULT CHostWnd::OnLcwmSend(WPARAM wParam, LPARAM lParam)
{
	SendToSvr((char *) lParam, wParam);
	return 0;
}

void CHostWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 0)
	{
		if (m_bOnline)
		{
			Ping();
		}
	}
}
