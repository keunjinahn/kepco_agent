// ConSock.h: interface for the CConSock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONSOCK_H__E3FDE7CB_8DE8_4C4C_93EB_750F29D5F8FC__INCLUDED_)
#define AFX_CONSOCK_H__E3FDE7CB_8DE8_4C4C_93EB_750F29D5F8FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
class SENDBUF
{
public:
	char *buf;
	int len;
	int sent;
	SENDBUF()
	{
		buf = NULL;
		sent = 0;
	}
	~SENDBUF()
	{
		if (buf)
			delete [] buf;
	}
};

typedef std::list<SENDBUF *> BUFLIST;
class CConSock;
typedef std::list<CConSock *> DRIVERLIST;
class CConSock  
{
public:
	int GetPendingCnt();
	char TMI_IDX[SZ_ID];
	char TMI_NAME[SZ_NAME];
	KBPKT_HDR *m_pHdrRs;
	BOOL IsEmpty();
	BOOL Recv();
	SOCKET m_soc;
	int Send();
	int Send(const char *buf, int len);
	CConSock();
	virtual ~CConSock();
	BOOL m_bLive;
	BOOL m_bShutDown;
	char m_szIpAddress[16];
	DWORD m_dwPingTic;
private:
// for sending
	BUFLIST m_listSnd;

// for receiving
	char *m_bufRs;
	int m_lenRs;
	int m_cbBufRs;
};

inline int CConSock::GetPendingCnt()
{
	return m_listSnd.size();
}

#endif // !defined(AFX_CONSOCK_H__E3FDE7CB_8DE8_4C4C_93EB_750F29D5F8FC__INCLUDED_)
