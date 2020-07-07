#if !defined(AFX_HOSTWND_H__F84F0E15_1D9F_44B8_A4B0_7FBACCE3EEFA__INCLUDED_)
#define AFX_HOSTWND_H__F84F0E15_1D9F_44B8_A4B0_7FBACCE3EEFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HostWnd.h : header file
//

#include "ConSock.h"
class CCmdPacket;
/////////////////////////////////////////////////////////////////////////////
// CHostWnd window

class CHostWnd : public CWnd
{
// Construction
public:
	CHostWnd();

// Attributes
public:

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHostWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL LoginToSvr(BOOL bForceEnter=FALSE);
	void SendTcpData(CCmdPacket *pCmdPacket,int senderID,int cmd,const char * strCmd);
	void DeleteSvrSock();
	CConSock *SendToSvr(char *pbuf, int cb, int threshold = -1);
	void Ping();
	virtual ~CHostWnd();
	char *m_pPayload;
protected:
	//{{AFX_MSG(CHostWnd)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	

	afx_msg LRESULT OnSockrltConn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSockrltRcv(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSockrltClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDelConsock(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLcwmQuit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLcwmSend(WPARAM wParam, LPARAM lParam);
public :
	CConSock *m_pSvrSock;
	BOOL m_bOnline;
	CString	m_sOpenedAddr;
	UINT	m_uOpenedPort;
	BOOL m_bAckCheck;
	DWORD g_tcpLoginCehckCnt;

	DWORD m_dwFailServerConnectCount;
	void SendShopData(CShopInfo* pShopInfo, CString serverIp, int port);
	float GetSendValue(float fValue);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOSTWND_H__F84F0E15_1D9F_44B8_A4B0_7FBACCE3EEFA__INCLUDED_)
