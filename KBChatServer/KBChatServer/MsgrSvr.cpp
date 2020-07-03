// MsgrSvr.cpp: implementation of the CMsgrSvr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsgrSvr.h"
#include "KBChatServer.h"
#include <process.h>
#include <time.h>
#include <Shlwapi.h>
#include "KBChatServerDlg.h"
#include <sys/timeb.h>
#include <math.h>
#define MSGRSVRWNDCLS _T("KBMSGSVRCLS")
#define LCWM_SOCKLIS	WM_USER + 55
#define LCWM_SOCKCLI	WM_USER + 56
#define LCWM_CREATE		WM_USER + 57
#define LCWM_CLOSE		WM_USER + 58

#define LCWM_CONSOCK	WM_USER + 59
#define LCWM_CONSOCKCLI	WM_USER + 60
#include "UTF8Conv.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgrSvr::CMsgrSvr()
{
	m_hThr = NULL;
	m_socLis = INVALID_SOCKET;
	m_bufRead = NULL;
}

CMsgrSvr::~CMsgrSvr()
{

}

CMsgrSvr g_MsgrSvr;

BOOL CMsgrSvr::Start()
{
	m_hThr = _beginthreadex(NULL, 0, ThrProc, this, 0, &m_idThr);
	if (m_hThr)
		return TRUE;
	else
		return FALSE;
}

void CMsgrSvr::Stop()
{
	PostThreadMessage(m_idThr, WM_QUIT, 0, 0);
	while (WaitForSingleObject((HANDLE) m_hThr, 1000) == WAIT_TIMEOUT)
		PostThreadMessage(m_idThr, WM_QUIT, 0, 0);
	CloseHandle((HANDLE) m_hThr);
}

void CMsgrSvr::Run()
{
	if(!Init())
	{
		CleanUp();
		return;
	}
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
        DispatchMessage(&msg);
	}

	CleanUp();
}

unsigned CMsgrSvr::ThrProc( void *pParam )
{
	CMsgrSvr *pThis = (CMsgrSvr *) pParam;

	pThis->Run();

	return 0;
}

BOOL CMsgrSvr::Init()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())


	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _Module.GetModuleInstance();
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = MSGRSVRWNDCLS;
	wc.hIconSm = NULL;
	RegisterClassEx(&wc);

	m_hWnd = CreateWindow(MSGRSVRWNDCLS, _T("KBMSGSVR"), 0, -100, -100, 10, 10, NULL, NULL, wc.hInstance, NULL);
	if (!m_hWnd)
		return FALSE;
	PostMessage(m_hWnd, LCWM_CREATE, 0, 0);

	if (InitSock())
	{
		WSAAsyncSelect(m_socLis, m_hWnd, LCWM_SOCKLIS, FD_ACCEPT);
	}
	else
	{
		return FALSE;
	}
	CoInitialize(NULL);	
	return TRUE;
}

void CMsgrSvr::CleanUp()
{
	closesocket(m_socLis);
	m_socLis = INVALID_SOCKET;

	SOCKMAP::iterator it = m_mapSock.begin();
	while (it != m_mapSock.end())
	{
		delete (*it).second;
		it++;
	}
	m_mapSock.clear();

	DestroyWindow(m_hWnd);

	delete [] m_bufRead;

	CoUninitialize();
}

LRESULT CMsgrSvr::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return g_MsgrSvr._WindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CMsgrSvr::_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == LCWM_SOCKLIS)
		return HndlSockLis(wParam, lParam);
	else if (uMsg == LCWM_SOCKCLI)
		return HndlSockCli(wParam, lParam);
	else if (uMsg == LCWM_CLOSE)
		return HndlClose(wParam, lParam);
	else if (uMsg == LCWM_CONSOCK)
		return HndlConSock(wParam, lParam);
	else if (uMsg == LCWM_CONSOCKCLI)
		return HndlConSockCli(wParam, lParam);
	else if (uMsg == LCWM_CREATE)
	{
		SetTimer(m_hWnd, 0, 15000, NULL);
		return 0;
	}
	else if (uMsg == WM_TIMER && wParam == 0)
	{
		CheckUser();
		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		KillTimer(m_hWnd, 0);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL CMsgrSvr::InitSock()
{
	m_socLis = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_socLis == INVALID_SOCKET)
		return FALSE;

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons (PORT_S);
	sin.sin_addr.s_addr = INADDR_ANY;
	if (bind(m_socLis, (sockaddr *) &sin, sizeof(sin)) == SOCKET_ERROR
		|| listen(m_socLis, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(m_socLis);
		m_socLis = INVALID_SOCKET;
		return FALSE;
	}

    int opt;; 
    int rn; 

	rn = sizeof(int); 
	// 현재 RCVBUF 값을 얻어온다. 
	if (::getsockopt(m_socLis, SOL_SOCKET, SO_RCVBUF, (char *)&opt, (int *)&rn) < 0) 
	{ 
		perror("Set Error"); 
		return 1; 
	} 

	opt = N_READBUF; 
	if (setsockopt(m_socLis, SOL_SOCKET, SO_RCVBUF, (char *)&opt, (int)rn) < 0) 
	{ 
		perror("Set Error"); 
		return 1; 
	}

	opt = N_READBUF; 
	if (setsockopt(m_socLis, SOL_SOCKET, SO_SNDBUF, (char *)&opt, (int)rn) < 0) 
	{ 
		perror("Set Error"); 
		return 1; 
	} 

	opt = 1; 
	if (setsockopt(m_socLis, SOL_SOCKET, TCP_NODELAY, (char *)&opt, (int)rn) < 0) 
	{ 
		perror("Set Error"); 
		return 1; 
	} 

	return TRUE;
}

LRESULT CMsgrSvr::HndlConSock(WPARAM wParam, LPARAM lParam)
{
	SOCKET soc = (SOCKET) wParam;
	if (WSAGETSELECTERROR(lParam) != 0)
	{
		HndlConnect(soc, TRUE);
		return 0;
	}

	WORD wEvt = WSAGETSELECTEVENT(lParam);
	if (wEvt == FD_CONNECT)
		HndlConnect(soc);

	return 0;
}

void CMsgrSvr::HndlConnect(SOCKET soc, BOOL bError /* = FALSE */)
{
	if (bError || WSAAsyncSelect(soc, m_hWnd, LCWM_CONSOCKCLI, FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
	{
		return;
	}
}

LRESULT CMsgrSvr::HndlSockLis(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam) != 0)
		return 0;

	WORD wEvt = WSAGETSELECTEVENT(lParam);
	if (wEvt == FD_ACCEPT)
		HndlAccept();

	return 0;
}


void CMsgrSvr::HndlAccept()
{
	USES_CONVERSION;
	SOCKADDR_IN m_clientAddr;		//0921
	int length;						//0921
	length = sizeof(m_clientAddr);	//0921

	SOCKET soc = accept(m_socLis, (SOCKADDR*)&m_clientAddr,&length);	//0921
	if (soc == INVALID_SOCKET)
		return;

	if (WSAAsyncSelect(soc, m_hWnd, LCWM_SOCKCLI, FD_WRITE | FD_READ | FD_CLOSE) == SOCKET_ERROR)
		closesocket(soc);
	else
	{
		CConSock *pSock = NULL;
		try
		{
			pSock = new CConSock;
			pSock->m_soc = soc;
		}
		catch (...)
		{

		}
		strcpy(pSock->m_szIpAddress, inet_ntoa(m_clientAddr.sin_addr));
		m_mapSock.insert(SOCKMAP::value_type(soc, pSock));

		//g_pServerDlg->LogMessage(LOG_CONNECT,A2W(pSock->m_szIpAddress));
		WriteLog("HndlAccept - ", 0, 0,pSock->m_szIpAddress);

	}
}


LRESULT CMsgrSvr::HndlSockCli(WPARAM wParam, LPARAM lParam)
{
	SOCKET soc = (SOCKET) wParam;
	if (WSAGETSELECTERROR(lParam) != 0)
	{
		CConSock *pSock = NULL;
		SOCKMAP::iterator it;
    	it = m_mapSock.find(soc);
		if (it != m_mapSock.end())
			pSock = (*it).second;
		if(pSock)
		{
			pSock->m_bShutDown = TRUE;
			CloseSocket(soc, TRUE, TRUE);
		}
		else
			CloseSocket(soc, TRUE);
		return 0;
	}

	WORD wEvt = WSAGETSELECTEVENT(lParam);
	if (wEvt == FD_READ)
		HndlRead(soc);
	else
	if (wEvt == FD_WRITE)
	{
		SOCKMAP::iterator it;
    	it = m_mapSock.find(soc);
		if (it != m_mapSock.end())
		{
			CConSock *pSock = (*it).second;
			pSock->Send();
		}
	}
	else
		CloseSocket(soc, TRUE);

	return 0;
}

LRESULT CMsgrSvr::HndlConSockCli(WPARAM wParam, LPARAM lParam)
{
	SOCKET soc = (SOCKET) wParam;
	if (WSAGETSELECTERROR(lParam) != 0)
	{
		return 0;
	}

	WORD wEvt = WSAGETSELECTEVENT(lParam);
	return 0;
}

void CMsgrSvr::HndlRead(SOCKET soc)
{
	SOCKMAP::iterator it;
    it = m_mapSock.find(soc);
	if (it != m_mapSock.end())
	{
		CConSock *pSock = (*it).second;
		pSock->Recv();
	}
}

void CMsgrSvr::CreateLogFile()
{
	USES_CONVERSION;
	char buf[1024];
	char LogFileName[1024];
	time_t tt = time(NULL);
	struct tm *ptm = localtime(&tt);
	char dirBuf[MAX_PATH];
	BOOL bRet;

	strcpy(dirBuf, "c:\\KBChatServer\\log");
	bRet = ::CreateDirectory(A2W(dirBuf), NULL);
	strcat(dirBuf, "\\svrLog");
	bRet = ::CreateDirectory(A2W(dirBuf), NULL);
	sprintf(dirBuf, "%s\\%d", dirBuf, PORT_S);
	bRet = ::CreateDirectory(A2W(dirBuf), NULL);

	sprintf(LogFileName, "%s\\log_%04i%02i%02i%02i%02i.txt", dirBuf, ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min);
	m_hLogFile = CreateFile(A2W(LogFileName), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	if (m_hLogFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwWritten;
		sprintf(buf, "[날짜 : 시간] / 패킷(1 = 정상, 2 = 비정상) / 아이디 / 방장 여부 / 에러(-1 = 소켓 생성 에러, 0 = 성공, 1 = SELECT 에러, 2 = Send 에러, 3 = SELECT2 에러,4 = 서버측 Close 에러)\r\n\r\n\r\n");
		WriteFile(m_hLogFile, buf, strlen(buf), &dwWritten, NULL);
	}

}

static void AsciiToUTF8(CString parm_ascii_string, CString &parm_utf8_string) 
{
	USES_CONVERSION;
     parm_utf8_string.Empty(); 
 
     // 아스키 코드를 UTF8형식의 코드로 변환해야 한다. 아스키 코드를 UTF8 코드로 변환할때는 
     // 아스키 코드를 유니코드로 먼저 변환하고 변환된 유니코드를 UTF8 코드로 변환해야 한다.
 
     // 아스키 코드로된 문자열을 유니코드화 시켰을 때의 길이를 구한다.
      int temp_length = MultiByteToWideChar(CP_ACP, 0, W2A(parm_ascii_string), -1, NULL, 0);


     // 변환된 유니코드를 저장할 공간을 할당한다.
     BSTR unicode_str = SysAllocStringLen(NULL, temp_length + 1);
 
     // 아스키 코드로된 문자열을 유니 코드 형식의 문자열로 변경한다.
      MultiByteToWideChar(CP_ACP, 0, W2A(parm_ascii_string), -1, unicode_str, temp_length);
 
     // 유니코드 형식의 문자열을 UTF8 형식으로 변경했을때 필요한 메모리 공간의 크기를 얻는다.
     temp_length = WideCharToMultiByte( CP_UTF8, 0, unicode_str, -1, NULL, 0, NULL, NULL );
 
     if(temp_length > 0){
        CString str;


        // UTF8 코드를 저장할 메모리 공간을 할당한다.
        char *p_utf8_string = new char[temp_length];
        memset(p_utf8_string, 0, temp_length);
        // 유니코드를 UTF8코드로 변환한다.
        WideCharToMultiByte(CP_UTF8, 0, unicode_str, -1, p_utf8_string, temp_length, NULL, NULL);
  
        // UTF8 형식으로 변경된 문자열을 각 문자의 코드값별로 웹 URL에 사용되는 형식으로 변환한다.
        for(int i = 0; i < temp_length - 1; i++){
            if(p_utf8_string[i] & 0x80){
                // 현재 코드가 한글인 경우..
                // UTF8 코드로 표현된 한글은 3바이트로 표시된다. "한글"  ->  %ED%95%9C%EA%B8%80
                for(int sub_i = 0; sub_i < 3; sub_i++){
                    str.Format(_T("%%%X"), p_utf8_string[i] & 0x00FF);
                    parm_utf8_string += str;
                    i++;
                } 
    
                i--;
            } else {
                // 현재 코드가 영문인 경우, 변경없이 그대로 사용한다.
               parm_utf8_string += p_utf8_string[i];
            }
        }                                                              
  
        delete[] p_utf8_string;
     }
 
      // 유니코드 형식의 문자열을 저장하기 위해 생성했던 메모리를 삭제한다.
      SysFreeString(unicode_str);
}
int CMsgrSvr::HndlPacket(CConSock *pSock)
{
	KBPKT_HDR *pHdr = pSock->m_pHdrRs;
	char *padv = (char *) (pHdr + 1);
	USES_CONVERSION;

	switch (pHdr->cmd)
	{
	case '5':
		{
			WriteLog("HndlPacket", SOCK_CMD_DATA, 0);
			KBPKT_DATA *pPkt = (KBPKT_DATA*) padv;
			/*
			CString strMsg;
			char strMac[13];
			//strncpy(strMac, pHdr->mac, 12);
			//strMac[12] = '\0';
			memcpy(strMac, pHdr->mac, 12);
			strMac[12] = '\0';
			strMsg.Format(_T("mac : %s , device no : %s , sensor state : %s, 누수1 : %c, 누수2: %c"), A2W(strMac),pPkt->device_no,pPkt->sensor_state,pPkt->leak_1_value, pPkt->leak_2_value);
			*/
			KBPKT_DATA_OBJ dataObj;
			dataObj.setPacket(pHdr, pPkt);
			g_pServerDlg->SensorDataInsert(&dataObj);
			g_pServerDlg->LogMessage(LOG_LOGIN, dataObj.getPacketLog());
		}
		break;
	default:
		return -1;
	}

	return 1;	// end with sending
}

void CMsgrSvr::RelayMessageToIdxs(KBPKT_HDR *pHdr,CConSock *pSock,CStringList *pList)
{
	USES_CONVERSION;

}

void CMsgrSvr::RelayMessageToIdx(KBPKT_HDR *pHdr,CConSock *pSock,char *toIdx)
{
	USES_CONVERSION;
}



void CMsgrSvr::CheckUser()
{
	USES_CONVERSION;
}


void CMsgrSvr::PostCloseMsg(SOCKET soc)
{
//	WriteLog(10, 0);
	PostMessage(m_hWnd, LCWM_CLOSE, 0, (LPARAM) soc);
//	WriteLog(10, 1);
}

LRESULT CMsgrSvr::HndlClose(WPARAM wParam, LPARAM lParam)
{
	SOCKET soc = (SOCKET) lParam;

		CConSock *pSock = NULL;
		SOCKMAP::iterator it;
    	it = m_mapSock.find(soc);
		if (it != m_mapSock.end())
			pSock = (*it).second;
		if(pSock)
		{
			pSock->m_bShutDown = TRUE;
			CloseSocket(soc, TRUE, TRUE);
		}
		else
			CloseSocket(soc, TRUE);
	return 0;
}
 
void CMsgrSvr::CloseSocket(SOCKET soc, BOOL bToss, BOOL bDown/* = FALSE*/)
{
	WriteLog("CloseSocket", 12, 0);
	USES_CONVERSION;
	int errorLine = 0;
	SOCKMAP::iterator it;
	try
	{
		it = m_mapSock.find(soc);
		if (it != m_mapSock.end())
		{
			CConSock *pSock = (*it).second;
			m_mapSock.erase(it);
			m_listSocks.remove(pSock);

			delete pSock;
			pSock = NULL;


			WriteLog("CloseSocket", 12, 13);
		}
		else
			closesocket(soc);

	}
	catch(...)
	{
		WriteLog("exception CloseSocket", errorLine , 5);
	}
	WriteLog("CloseSocket", 12, 5);
}

void CMsgrSvr::WriteLog(char *pFunc, int iNum, int iErrNum, char *pId)
{
	if (m_hLogFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize;
		dwFileSize = GetFileSize(m_hLogFile, NULL);
		if(dwFileSize == INVALID_FILE_SIZE)
		{
			CloseHandle(m_hLogFile);
			m_hLogFile = INVALID_HANDLE_VALUE;
			CreateLogFile();
		}
		else if(dwFileSize > 5240000 * 2)
		{
			CloseHandle(m_hLogFile);
			m_hLogFile = INVALID_HANDLE_VALUE;
			CreateLogFile();
		}
		if(m_hLogFile == INVALID_HANDLE_VALUE)
			return;

		SYSTEMTIME stTime;			
		GetLocalTime(&stTime);
		char buftime[1024];
		buftime[0] ='\0';
		sprintf(buftime, "%02d_%02d:%02d:%02d", stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);

		char buf[1024];
		DWORD dwWritten;
		sprintf(buf, "{ %s } === [%s] :: (%d), (%d) , (%s) ===\r\n", pFunc, buftime, iNum, iErrNum, pId ? pId : "NULL");
		WriteFile(m_hLogFile, buf, strlen(buf), &dwWritten, NULL);
	}
}

void CMsgrSvr::WriteLog(int iNum, char *pId)
{
	if (m_hLogFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize;
		dwFileSize = GetFileSize(m_hLogFile, NULL);
		if(dwFileSize == INVALID_FILE_SIZE)
		{
			CloseHandle(m_hLogFile);
			m_hLogFile = INVALID_HANDLE_VALUE;
			CreateLogFile();
		}
		else if(dwFileSize > 10240000 * 4)
		{
			CloseHandle(m_hLogFile);
			m_hLogFile = INVALID_HANDLE_VALUE;
			CreateLogFile();
		}
		if(m_hLogFile == INVALID_HANDLE_VALUE)
			return;

		SYSTEMTIME stTime;			
		GetLocalTime(&stTime);
		char buftime[1024];
		buftime[0] ='\0';
		sprintf(buftime, "%02d_%02d:%02d:%02d", stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);

		char buf[1024];
		DWORD dwWritten;
		sprintf(buf, "WriteLog === [%s] :: (%d), (%s) ===\r\n", buftime, iNum, pId);
		WriteFile(m_hLogFile, buf, strlen(buf), &dwWritten, NULL);
	}
}

int CMsgrSvr::GetCountChar(CString Ps_Str, TCHAR Pc_Char)
{ 
    int Li_Count = 0; 
    for(int i = 0; i < Ps_Str.GetLength(); i++)
    {
        if(Ps_Str[i] == Pc_Char) Li_Count++; 
    }
    return Li_Count; 
} 

CStringList * CMsgrSvr::Split(CString Ps_Str, TCHAR Pc_Separator, int& Ri_Length)
{
    Ri_Length = GetCountChar(Ps_Str, Pc_Separator) + 1;
    if (Ps_Str[0]                      == Pc_Separator) Ri_Length--;
    if (Ps_Str[Ps_Str.GetLength() - 1] == Pc_Separator) Ri_Length--;
    CString* Rsa_Str = new CString[Ri_Length];
	CStringList *pList = new CStringList();
    int Li_Count = 0;
    CString Ls_Token;
    for (int i = 0; i < Ri_Length; i++)
    {
        AfxExtractSubString(Ls_Token, Ps_Str, i, Pc_Separator);
		pList->AddTail(Ls_Token);
    }

    return pList;
}
