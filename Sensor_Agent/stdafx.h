
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.


#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����


#include <afxsock.h>            // MFC ���� Ȯ��

#include "..\include\KBPacketh.h"
#define USE_EXECPTION	1
#if USE_EXECPTION
#define	ASTRY		try
#define	ASCATCH		catch(...)
#else
#define	ASTRY		if (1)
#define	ASCATCH		else
#endif
#define ASEXCEPTION(x,a,b,c) if(USE_EXECPTION && x){ x->Exception(a,b,c); };
#define LCWM_SOCKRLT_CONN	WM_USER + 55
#define LCWM_SOCKRLT_RCV	WM_USER + 56
#define LCWM_SOCKRLT_CLOSE	WM_USER + 57
#define LCWM_DEL_CONSOCK	WM_USER + 58
#define LCWM_QUIT			WM_USER + 59
#define LCWM_SOCKRLT_RCVD	WM_USER + 60
#define	LCWM_CLOSE			WM_USER + 61
#define LCWM_LOGINRLT		WM_USER + 68
#define LCWM_SEND			WM_USER + 82

#define N_READBUF	8192
#include <afxtempl.h>
	#define _ATL_APARTMENT_THREADED
#include <atlbase.h>

#define SERVER_IP_ADDRESS	_T("localhost")
#define PORT_S 443
#define SOCKET_COUNT 40
#define	WM_ICON_NOTIFY			WM_APP+10
#define RUN_APP_NAME  _T("KCollectionServer.exe")
#define RUN_INSTALL_NAME _T("KCollectionServer_setup.exe")
#define RUN_RESOURCE_PYC_NAME  _T("event_manager.py")

#pragma warning(disable : 4996)

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


