
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

#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1
#include <afxsock.h>            // MFC 소켓 확장

#include "..\..\include\KBPacketh.h"
#include "ximage.h"
#include "MemDC.h"
#include "..\..\include\util.h"
#include <afxinet.h>
#include <iostream>
#include <map>
#include <string>
#include "json.h"
typedef std::map<CString, CString> APICALLDATA;

#define N_READBUF	8192

enum SENSORTYPE{SENSOR_LEAK = 0, SENSOR_TEMP, SENSOR_HUMI};

#define SHOP_INFO_COUNT 10

#define KEY_CMD  _T("CMD")
#define REQUEST_HTTPS	0
#define API_SERVER_IP _T("localhost")
#define API_SERVER_PORT 5000
enum API_CMD {
	API_SENSOR_REGISTER = 1,
	API_SENSOR_DATA = 2,
	API_CHECK_CONFIG=3
};

#define _mac "mac"
#define _location_code "location_code"
#define _device_no "device_no"
#define _sensor_state "sensor_state"

#define _leak_1_value "leak_1_value"
#define _leak_2_value "leak_2_value"

#define _temp_1_value "temp_1_value"
#define _temp_2_value "temp_2_value"
#define _temp_3_value "temp_3_value"
#define _temp_4_value "temp_4_value"

#define _humi_1_value "humi_1_value"
#define _humi_2_value "humi_2_value"
#define _humi_3_value "humi_3_value"
#define _humi_4_value "humi_4_value"

#define RETURN_SUCESS 200
#include <afxtempl.h>
	#define _ATL_APARTMENT_THREADED
#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class CKBChatServerModule : public CComModule
{
public:
	LONG Unlock();
	LONG Lock();
	LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2);
	DWORD dwThreadID;
};

extern CKBChatServerModule _Module;
#include <atlcom.h>


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


