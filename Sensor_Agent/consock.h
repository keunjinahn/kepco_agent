#if !defined(AFX_CONSOCK_H__2F7B4330_E199_482F_8C86_54B65006D337__INCLUDED_)
#define AFX_CONSOCK_H__2F7B4330_E199_482F_8C86_54B65006D337__INCLUDED_

#pragma once
// ConSock.h : header file
//

#define N_BUFRCV	2048


class CRcvedData
{
public:
	KBPKT_HDR m_hdrRs;
	char *m_pRsData;

	CRcvedData()
	{
		m_pRsData = NULL;
	}

	~CRcvedData()
	{
		if (m_pRsData)
			delete [] m_pRsData;
	}
};

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

/////////////////////////////////////////////////////////////////////////////
// CConSock command target

class CConSock : public CAsyncSocket
{
// Attributes
public:
	KBPKT_HDR *m_pRsHdr;
	char *m_pRsData;
	int m_iRsData;

	char *m_pRefData;

	int		nErrcntSend, nErrcntReceive;

	BOOL m_fConnected;

// Operations
public:
	CConSock();
	virtual ~CConSock();
// Overrides
public:
	int GetPendingCnt();
	BOOL AdvSend(void *pbuf, int len);
	void Accept(CWnd *pWnd);
	BOOL Create(char *pbuf, int cb, CWnd *pWnd,BOOL bAspCall = FALSE);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConSock)
	public:
	virtual void OnSend(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CConSock)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	virtual void OnConnect( int nErrorCode );
	virtual void OnReceive( int nErrorCode );
	virtual void OnClose( int nErrorCode );

private:
	BOOL SendBuf();
	CList<SENDBUF *, SENDBUF *&> m_listSendBuf;
	CWnd *m_pWnd;

	char *m_pbufRcv;
	int m_iRcvd;
	int m_cbBufRcv;
};

inline int CConSock::GetPendingCnt()
{
	return m_listSendBuf.GetCount();
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONSOCK_H__2F7B4330_E199_482F_8C86_54B65006D337__INCLUDED_)
