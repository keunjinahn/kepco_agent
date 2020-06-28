// MsgrSvr.h: interface for the CMsgrSvr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGRSVR_H__9F8E9D2B_7E1D_432F_B8D2_57C58794C8A1__INCLUDED_)
#define AFX_MSGRSVR_H__9F8E9D2B_7E1D_432F_B8D2_57C58794C8A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "ConSock.h"
typedef std::map<SOCKET, CConSock *> SOCKMAP;
typedef std::list<CConSock *> SOCKLIST;
typedef struct head{
	int 	msgtype;
	int	msglen;
}HEAD;

typedef struct body{
	char 	msgstr[1024];
}BODY;

//#define PORT_S	39432
#define PORT_S	8080

#define RESOURCE_RGS 1
#define REG_GUID _T("{F5C7F99A-64BF-47f0-A0C6-22D8C4A98BF5}")



class CMsgrSvr
{
public:
	int HndlPacket(CConSock *pSock);
	void PostCloseMsg(SOCKET soc);
	void CleanUp();
	BOOL Init();
	void Run();
	void Stop();
	BOOL Start();
	CMsgrSvr();
	virtual ~CMsgrSvr();
	void CheckUser();

	LRESULT _WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SameIdClose(CConSock *pSock);
	void CreateLogFile();
	void WriteLog(char *pFunc, int iNum, int iErrNum, char *pId = NULL);
	void WriteLog(int iNum, char *pId);	
private:

	void CloseSocket(SOCKET soc, BOOL bToss, BOOL bDown = FALSE);
	LRESULT HndlClose(WPARAM wParam, LPARAM lParam);
	void HndlRead(SOCKET soc);
	LRESULT HndlSockCli(WPARAM wParam, LPARAM lParam);
	void HndlAccept();
	BOOL InitSock();
	LRESULT HndlConSock(WPARAM wParam, LPARAM lParam);
	void HndlConnect(SOCKET soc, BOOL bError = FALSE);
	LRESULT HndlConSockCli(WPARAM wParam, LPARAM lParam);
	unsigned m_idThr;
	unsigned long m_hThr;
	static unsigned __stdcall ThrProc( void *pParam );

	HWND m_hWnd;
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HndlSockLis(WPARAM wParam, LPARAM lParam);
	SOCKET m_socLis;
	char *m_bufRead;

	SOCKMAP m_mapSock;
	SOCKLIST m_listSocks;
	HANDLE m_hLogFile;
	CStringList * Split(CString Ps_Str, TCHAR Pc_Separator, int& Ri_Length);
	void RelayMessageToIdxs(KBPKT_HDR *pHdr,CConSock *pSock,CStringList *pList);
	void RelayMessageToIdx(KBPKT_HDR *pHdr,CConSock *pSock,char *toIdx);
};

extern CMsgrSvr g_MsgrSvr;

#endif // !defined(AFX_MSGRSVR_H__9F8E9D2B_7E1D_432F_B8D2_57C58794C8A1__INCLUDED_)
