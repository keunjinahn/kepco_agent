// ConSock.cpp : implementation file
//

#include "stdafx.h"
#include "Sensor_Agent.h"
#include "ConSock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "HostWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CConSock

CConSock::CConSock()
{
	m_pRefData = NULL;
	nErrcntSend = nErrcntReceive = 0;
	m_pbufRcv = NULL;
	m_iRcvd = 0;

	m_fConnected = FALSE;

	m_cbBufRcv = N_BUFRCV;
	m_pbufRcv = new char[m_cbBufRcv];
	m_pRsHdr = (KBPKT_HDR *) m_pbufRcv;
	m_pRsData = m_pbufRcv + sizeof(KBPKT_HDR);
}

CConSock::~CConSock()
{
	if (m_pRefData)
		delete [] m_pRefData;
	if (m_pbufRcv)
		delete [] m_pbufRcv;
	POSITION pos = m_listSendBuf.GetHeadPosition();
	while (pos)
		delete m_listSendBuf.GetNext(pos);
}


// CConSock member functions

void CConSock::OnConnect( int nErrorCode )
{

	if (nErrorCode == 0)
	{
		m_fConnected = TRUE;

		int opt = 0; 
		int rn = 0; 
		SendBuf();
	}
	else {
		m_pWnd->PostMessage(LCWM_SOCKRLT_CONN, nErrorCode, (LPARAM) this);
	}

//	CAsyncSocket::OnConnect(nErrorCode);
}

void CConSock::OnReceive( int nErrorCode )
{
	if (nErrorCode == 0)
	{
		int iRead = Receive(m_pbufRcv + m_iRcvd, m_cbBufRcv - m_iRcvd);
		m_iRcvd += iRead;
		if (iRead == SOCKET_ERROR) {
			if (GetLastError() != WSAEWOULDBLOCK) {
				nErrcntReceive++;
				if (nErrcntReceive > 10)
					m_pWnd->PostMessage(LCWM_SOCKRLT_CLOSE, nErrorCode, (LPARAM) this);
			}

			return;
		}

		
		while (m_iRcvd > 0 && m_iRcvd >= sizeof(KBPKT_HDR)) {
			if (m_pRsHdr->size >= 0) {
				char sSize[3];
				strncpy_s(sSize, m_pRsHdr->size, 2);
				int dwSize = atoi(sSize);
				LONG iExpectedDataSize = dwSize + sizeof(KBPKT_HDR);

				if (iExpectedDataSize > m_iRcvd) {
					if (m_iRcvd == m_cbBufRcv) {
						m_cbBufRcv = iExpectedDataSize;
						char *pbuf = new char[m_cbBufRcv];
						CopyMemory(pbuf, m_pbufRcv, m_iRcvd);
						delete [] m_pbufRcv;
						m_pbufRcv = pbuf;
						m_pRsHdr = (KBPKT_HDR *) m_pbufRcv;
						m_pRsData = m_pbufRcv + sizeof(KBPKT_HDR);
					}

					break;
				}

				m_iRsData = m_iRcvd - sizeof(KBPKT_HDR);
				nErrcntReceive = 0;
				m_pWnd->SendMessage(LCWM_SOCKRLT_RCV, 0, (LPARAM) this);

				m_iRcvd -= iExpectedDataSize;
				if (m_iRcvd > 0)
					CopyMemory(m_pbufRcv, m_pbufRcv + iExpectedDataSize, m_iRcvd);
			}
			else {
				if (m_iRcvd == sizeof(KBPKT_HDR))
					break;

				m_iRsData = m_iRcvd - sizeof(KBPKT_HDR);
				if (m_pWnd->SendMessage(LCWM_SOCKRLT_RCV, 0, (LPARAM) this) > 0) {
					// need to receive more data
					CopyMemory(m_pRsData, m_pRsData + m_iRcvd - sizeof(KBPKT_HDR) - m_iRsData, m_iRsData);
					m_iRcvd = sizeof(KBPKT_HDR) + m_iRsData;
				}
				else {
					LONG iExpectedDataSize = m_iRcvd - m_iRsData;


					m_iRcvd = m_iRsData;
					if (m_iRcvd > 0)
						CopyMemory(m_pbufRcv, m_pbufRcv + iExpectedDataSize, m_iRcvd);
				}
			}
		}
	}
	else {
		nErrcntReceive++;
		if (nErrcntReceive > 10)
			m_pWnd->PostMessage(LCWM_SOCKRLT_CLOSE, nErrorCode, (LPARAM) this);
	}

//	CAsyncSocket::OnReceive(nErrorCode);
}

void CConSock::OnClose( int nErrorCode )
{
	m_pWnd->PostMessage(LCWM_SOCKRLT_CLOSE, nErrorCode, (LPARAM) this);
	CAsyncSocket::OnClose(nErrorCode);
}

BOOL CConSock::Create(char *pbuf, int cb, CWnd *pWnd, BOOL bAspCall /* = FALSE */)
{
	if (!CAsyncSocket::Create())
		return FALSE;
	m_pWnd = pWnd;
	if (pbuf) {
		SENDBUF *pBuf = new SENDBUF;
		pBuf->buf = pbuf;
		pBuf->len = cb;
		m_listSendBuf.AddTail(pBuf);
//		delete pBuf;
	}



	AsyncSelect(FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);




//	delete pbuf;
	return TRUE;
}

void CConSock::Accept(CWnd *pWnd)
{
	m_pWnd = pWnd;

	AsyncSelect(FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
}

void CConSock::OnSend(int nErrorCode) 
{
	SendBuf();
//	CAsyncSocket::OnSend(nErrorCode);
}

BOOL CConSock::SendBuf()
{
	while (!m_listSendBuf.IsEmpty()) {
		SENDBUF *pBuf = m_listSendBuf.GetHead();

		int iSent = Send(pBuf->buf + pBuf->sent, pBuf->len - pBuf->sent);
		if (iSent == SOCKET_ERROR) {
			if (GetLastError() != WSAEWOULDBLOCK) {
				nErrcntSend++;				//ReLogin
				if (nErrcntSend > 10)		//ReLogin
					m_pWnd->PostMessage(LCWM_SOCKRLT_CLOSE, GetLastError(), (LPARAM) this);
				return FALSE;
			}
			else {
				return TRUE;
			}
		}

		nErrcntSend = 0;					//ReLogin

		pBuf->sent += iSent;
		if (pBuf->sent >= pBuf->len) {
			m_listSendBuf.RemoveHead();
			delete pBuf;
		}
	}

	return TRUE;
}

BOOL CConSock::AdvSend(void *pbuf, int len)
{
	try
	{
		SENDBUF *pBuf = new SENDBUF;
		pBuf->buf = (char *) pbuf;
		pBuf->len = len;
		m_listSendBuf.AddTail(pBuf);

		BOOL temp;

		if (m_fConnected && m_listSendBuf.GetCount()) {
			temp = SendBuf();
	//		delete pBuf;
			return temp;
		}

		else {
			return TRUE;
		}
	}
	catch(...)
	{
		return FALSE;
	}
}