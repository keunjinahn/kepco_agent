
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

#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1
#include <afxsock.h>            // MFC ���� Ȯ��

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

enum SENSORTYPE{SENSOR_LEAK = 0, SENSOR_TEMP=1, SENSOR_HUMI=2};
enum CHECKRESULT{CHECK_NORMAL = 0, CHECK_WARRING=1, CHECK_ERROR=2};
enum INCIDENTTYPE{INCIDENT_THRESHOLD = 1, INCIDENT_ANNOMALY=2};

#define SHOP_INFO_COUNT 10

#define KEY_CMD  _T("CMD")
#define REQUEST_HTTPS	0

#define	WM_ICON_NOTIFY			WM_APP+10

enum API_CMD {
	API_SENSOR_REGISTER = 1,
	API_SENSOR_DATA = 2,
	API_CHECK_CONFIG = 3,
	API_INCIDENT_DATA = 4,
	API_DOWNLOAD_FILE = 5,
	API_RESOURCE=6
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
#define _sensor_msg	  "sensor_msg"

#define _sensor_value "sensor_value"
#define _error_code "error_code"
#define _incident_type "incident_type"
#define _level "level"
#define _ipaddress "ipaddress"
#define _cpu_value "cpu_value"
#define _ram_avail_value "ram_avail_value"
#define _ram_phys_value "ram_phys_value"

#define RETURN_SUCESS 200
#define RETURN_NOT_AREA_CODE 409

#define RUN_APP_NAME  _T("KCollectionServer_setup.exe")
#define RUN_APP_NAME_RESOURCE  _T("KCollectionResource_setup.exe")
#define AGENT_VERSION _T("1.0.2")
#define AGENT_VERSION_RESOURCE _T("1.0.2")

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


