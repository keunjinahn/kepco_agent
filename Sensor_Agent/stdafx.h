
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#include <afxdisp.h>        // MFC 자동화 클래스입니다.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원


#include <afxsock.h>            // MFC 소켓 확장

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


