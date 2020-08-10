// ConSock.cpp: implementation of the CConSock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KBChatServer.h"
#include "ConSock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

#include "MsgrSvr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConSock::CConSock()
{
	m_soc = INVALID_SOCKET;

	m_cbBufRs = N_READBUF;
	m_bufRs = new char[m_cbBufRs];
	m_lenRs = 0;
	m_pHdrRs = (KBPKT_HDR *) m_bufRs;

	TMI_IDX[0] = '\0';
	m_bLive = TRUE;
	m_bShutDown = FALSE;
	m_szIpAddress[0] = '\0';
	TMI_NAME[0] = '\0';
	m_dwPingTic = ::GetTickCount();
}

CConSock::~CConSock()
{
	if (m_soc != INVALID_SOCKET)
		closesocket(m_soc);
	m_soc = INVALID_SOCKET;
	if (m_bufRs)
		delete [] m_bufRs;
	BUFLIST::iterator it = m_listSnd.begin();
	while (it != m_listSnd.end())
	{
		delete *it;
		it++;
	}
	m_listSnd.clear();
}

int CConSock::Send(const char *buf, int len)
{
	SENDBUF *pbuf = new SENDBUF;
	pbuf->buf = (char *) buf;
	pbuf->len = len;
	if(m_soc == INVALID_SOCKET)
		return 0;
	m_listSnd.push_back(pbuf);
	if (m_listSnd.size() >= 1)
	{
		return Send();
	}
	else
	{
		return 0;
	}
}

int CConSock::Send()
{
	BUFLIST::iterator it = m_listSnd.begin();
	while (it != m_listSnd.end())
	{
		
		SENDBUF *pbuf = *it;
		int iSent = send(m_soc, pbuf->buf + pbuf->sent, pbuf->len - pbuf->sent, 0);
		if (iSent == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				return 0;
			}
			else
			{
				m_listSnd.pop_front();
				delete pbuf;
				return -1;
			}
		}
		else
		{
			if(iSent < 0)
			{
				m_listSnd.pop_front();
				delete pbuf;
				return -1;
			}
			pbuf->sent += iSent;
			if (pbuf->len > pbuf->sent)
			{
				return 0;
			}

		}

		m_listSnd.pop_front();
		delete pbuf;

		it = m_listSnd.begin();
	}

	return 1;
}

BOOL CConSock::Recv()
{
	USES_CONVERSION;
	int iRead = recv(m_soc, m_bufRs + m_lenRs, m_cbBufRs - m_lenRs, 0);

//	CString strMsg;
	//strMsg.Format(_T("rev size : %d, msg : %s",), iRead, A2W(m_bufRs));
	//theApp.AppLog.Write(_T("%s"), strMsg);

	if (iRead == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			g_MsgrSvr.PostCloseMsg(m_soc);
			return FALSE;
		}
		else
			return TRUE;
	}
	m_lenRs += iRead; 
	while (m_lenRs >= sizeof(KBPKT_HDR))
	{
		if (m_lenRs < 0)
		{
			m_lenRs = 0;
			break;
		}
		char sSize[3];
		strncpy(sSize, m_pHdrRs->size, 2);
		int dwSize = atoi(sSize);
		dwSize = 47;
		DWORD dwTotalSize = dwSize + sizeof(KBPKT_HDR);
		if (m_lenRs < dwTotalSize)
		{
			if (m_lenRs == m_cbBufRs)
			{
				m_cbBufRs = dwTotalSize;
				char *pbuf = new char[m_cbBufRs];
				CopyMemory(pbuf, m_bufRs, m_lenRs);
				delete [] m_bufRs;
				m_bufRs = pbuf;
				m_pHdrRs = (KBPKT_HDR *) m_bufRs;
			}

			break;
		}

		int iRlt = g_MsgrSvr.HndlPacket(this);
		m_lenRs -= dwTotalSize;
		if (m_lenRs > 0)
			CopyMemory(m_bufRs, m_bufRs + dwTotalSize, m_lenRs);
	}

	return TRUE;
}

BOOL CConSock::IsEmpty()
{
	return m_listSnd.empty();
}
