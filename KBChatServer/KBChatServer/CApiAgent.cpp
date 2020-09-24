// CApiAgent.cpp: 구현 파일
//

#include "stdafx.h"
#include "KBChatServer.h"
#include "CApiAgent.h"
#include "afxdialogex.h"
#include "KBChatServerDlg.h"

// CApiAgent 대화 상자
#define TIMER_WEB_API_SEND	1
IMPLEMENT_DYNAMIC(CApiAgent, CDialogEx)
CApiAgent* g_pApiAgentDlg = NULL;
CRITICAL_SECTION m_csEventWebApi;


CString UTF8ToString(char* utf8)
{
	CString strReturn = _T("");
	wchar_t strUnicode[256] = { 0, };
	char	strUTF8[256] = { 0, };
	strcpy_s(strUTF8, 256, utf8);
	int nLen = MultiByteToWideChar(CP_UTF8, 0, strUTF8, strlen(strUTF8), NULL, NULL);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8, strlen(strUTF8), strUnicode, nLen);
	strReturn.Format(_T("%s"), strUnicode);
	return strReturn;
}


CApiAgent::CApiAgent(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_APIAGENT_DLG, pParent)
{
	m_hThWebApiEvent = NULL;
	m_idThrWebApiEvent = 0;
}

CApiAgent::~CApiAgent()
{
}

void CApiAgent::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CApiAgent, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CApiAgent 메시지 처리기


BOOL CApiAgent::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	g_pApiAgentDlg = this;
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetTimer(TIMER_WEB_API_SEND, 100, NULL);
	InitializeCriticalSection(&m_csEventWebApi);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CApiAgent::OnDestroy()
{
	CDialogEx::OnDestroy();
	while (!m_listWebApiCall.IsEmpty())
		delete m_listWebApiCall.RemoveTail();
	DeleteCriticalSection(&m_csEventWebApi);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CApiAgent::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case TIMER_WEB_API_SEND:
	{
		USES_CONVERSION;
		if (m_listWebApiCall.IsEmpty())
			return;

		m_hThWebApiEvent = (HANDLE)_beginthreadex(0, 0, ThreadWebApiCall, this, 0, &m_idThrWebApiEvent);
		if (!m_hThWebApiEvent) {
			return;
		}

	}
	break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CApiAgent::AddAPI(APICALLDATA* pParamMap)
{
	if (m_listWebApiCall.GetCount() > 50)
	{
		while(!m_listWebApiCall.IsEmpty())
		{
			delete (APICALLDATA*)m_listWebApiCall.RemoveHead();
		}
	}
	m_listWebApiCall.AddTail(pParamMap);
}


unsigned CApiAgent::ThreadWebApiCall(void* pParam)
{
	EnterCriticalSection(&m_csEventWebApi);
	CApiAgent* pThis = (CApiAgent*)pParam;
	if (!pThis->m_listWebApiCall.IsEmpty())
	{
		APICALLDATA* pItem = (APICALLDATA*)pThis->m_listWebApiCall.RemoveHead();
		pThis->makeJsonData(pItem);
		delete pItem;
	}
	LeaveCriticalSection(&m_csEventWebApi);
	return 0;
}

CString CApiAgent::makeGetParam(APICALLDATA* pParamMap)
{
	CString sData = _T("");
	CString strParam;

	APICALLDATA::iterator it = pParamMap->begin();
	while (it != pParamMap->end())
	{
		CString sKey = (*it).first;
		if (sKey == KEY_CMD)
		{
			it++;
			continue;
		}
		CString sValue = (*it).second;
		sData += sKey + _T("=") + sValue;
		it++;
		if (it != pParamMap->end())
			sData += _T("&");
	}
	return sData;
}

CString CApiAgent::makePostParam(APICALLDATA* pParamMap)
{
	CString sData = _T("{");
	CString strParam;
	APICALLDATA::iterator it = pParamMap->begin();
	while (it != pParamMap->end())
	{
		CString sKey = (*it).first;
		if (sKey == KEY_CMD)
		{
			it++;
			continue;
		}
		CString sValue = (*it).second;
		sData += _T("\"") + sKey + _T("\":\"") + sValue + _T("\"");
		it++;
		if (it != pParamMap->end())
			sData += _T(",");

	}
	sData += _T("}");
	return sData;
}

BOOL CApiAgent::makeJsonData(APICALLDATA* pParamMap)
{
	USES_CONVERSION;
	APICALLDATA::iterator it;
	it = pParamMap->find(KEY_CMD);
	if (it != pParamMap->end())
	{
		CString value = (*it).second;
		CString sData = _T("");
		switch (_wtoi(value))
		{
			case API_SENSOR_REGISTER:
			{
				sData = makePostParam(pParamMap);
				call_API(_wtoi(value), _T("/api/v1/agent/device_register?"), sData, _T("POST"));
			}
			break;
			case API_SENSOR_DATA:
			{
				sData = makePostParam(pParamMap);
				call_API(_wtoi(value), _T("/api/v1/agent/sensor_data?"), sData, _T("POST"));
			}
			break;
			case API_CHECK_CONFIG:
			{
				sData = makeGetParam(pParamMap);
				call_API(_wtoi(value), _T("/api/v1/agent/check_config?"), sData, _T("GET"));
			}
			break;
			case API_INCIDENT_DATA:
			{
				sData = makePostParam(pParamMap);
				call_API(_wtoi(value), _T("/api/v1/agent/incident_data?"), sData, _T("POST"));
			}
			break;
			case API_DOWNLOAD_FILE:
			{
				DownloadFileFromURL(pParamMap, g_pServerDlg->m_ConfigInfo.server_ip, g_pServerDlg->m_ConfigInfo.web_port);
			}
			break;
			case API_RESOURCE:
			{
				sData = makePostParam(pParamMap);
				call_API(_wtoi(value), _T("/api/v1/agent/kepco_resource_send?"), sData, _T("POST"));
			}
			break;
		}
	}
	else
		return FALSE;
	return TRUE;
}

void CApiAgent::call_API(int cmd, CString sApiPath, CString sFormData, CString sMethod)
{
	USES_CONVERSION;

	CString strParam;
	strParam.Format(_T("%s"), sApiPath, sFormData);
	//theApp.AppLog.Write(_T("[CAPIAgentDlg - call_API] - cmd %d, param : %s"),cmd,strParam);
	CString strResult;
//	if (REQUEST_HTTPS)
//		strResult = GetHttpSQuery(API_SERVER_IP, API_SERVER_PORT, sApiPath, sFormData);
//	else
//		strResult = GetHttpQuery(API_SERVER_IP, API_SERVER_PORT, strParam);
	strResult = GetHttpQuery(g_pServerDlg->m_ConfigInfo.server_ip, g_pServerDlg->m_ConfigInfo.port, sApiPath, sFormData,sMethod);
	if (strResult.GetLength() < 10)
		return;
	Json::Reader reader;
	Json::Value root;
	std::string config_doc = W2A(strResult);
	bool parsingSuccessful = reader.parse(config_doc, root);
	if (!parsingSuccessful)
	{
		CString strMsg;
		strMsg.Format(_T("%s"), reader.getFormatedErrorMessages());
		return;
	}

	parse_ReturnData(cmd, root);

}


CString CApiAgent::GetHttpQuery(CString strIP, int nPort, CString strUrl,CString strParams,CString method)
{
	USES_CONVERSION;
	//OutputDebugString(strUrl);
	HINTERNET hOpen = NULL, hConnect = NULL, hRequest = NULL;
	DWORD dwSize = 0, dwFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_NO_COOKIES;
	hOpen = InternetOpen(_T("Microsoft Internet Explorer"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	hConnect = InternetConnect(hOpen, strIP, nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	CString strResult = _T("");
	if (hConnect == NULL)
	{
		InternetCloseHandle(hOpen);
		return _T("");
	}

	BOOL bResult = FALSE;

	if (method == _T("POST") && g_pServerDlg->m_bWasServerConnected == TRUE)
	{
		LPCTSTR rgpszAcceptTypes[2] = { _T("application/json"), NULL };
		hRequest = HttpOpenRequest(hConnect, method, strUrl, NULL, NULL, rgpszAcceptTypes, 0, 0);
		if (hRequest != NULL)
		{
			HttpAddRequestHeaders(hRequest, _T("Content-Type: application/json\r\n"), -1, HTTP_ADDREQ_FLAG_ADD);
			//theApp.AppLog.WriteString(strParams);
			bResult = HttpSendRequest(hRequest, NULL, 0, W2A(strParams), strlen(W2A(strParams)));
		}
	}
	else if (method == _T("GET"))
	{
		LPCTSTR accept[2] = { _T("*/*"), NULL };
		CString strFullUrl;
		strFullUrl.Format(_T("%s%s"), strUrl, strParams);
		hRequest = HttpOpenRequest(hConnect, method, strFullUrl, NULL, NULL, accept, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);
		if (hRequest != NULL)
		{
			bResult = HttpSendRequest(hRequest, NULL, NULL, NULL, 0);
		}
	}

	LPTSTR m_html = _T("");
	BOOL bQuery = InternetQueryDataAvailable(hRequest, &dwSize, 0, 0);
	if (method == _T("GET") && dwSize == 0)
		g_pServerDlg->m_bWasServerConnected = FALSE;
	if (bQuery && dwSize > 0)
	{

		DWORD dwTotal = 0; \
			char* pData = (char*)malloc(dwTotal + dwSize + 1);
		while (pData && dwSize)
		{
			DWORD dwRead = 0;
			InternetReadFile(hRequest, pData + dwTotal, dwSize, &dwRead);
			dwTotal += dwRead;
			pData[dwTotal] = NULL;
			InternetQueryDataAvailable(hRequest, &dwSize, 0, 0);
			if (dwSize > 0)
			{
				pData = (char*)realloc(pData, dwTotal + dwSize + 1);
			}
		}
		if (pData)
		{
			// 받는 데이타 UTF8에서 UNICODE 로 변경 
			int len = MultiByteToWideChar(CP_UTF8, 0, pData, dwTotal, NULL, NULL);
			m_html = (LPWSTR)calloc(len + 1, sizeof(WCHAR));
			if (m_html)
			{
				MultiByteToWideChar(CP_UTF8, 0, pData, dwTotal, m_html, len + 1);
			}
			free(pData);
		}
		strResult.Format(_T("%s"), m_html);
		/*
		CString strLog;
		strLog.Format(_T("webpage call result : %d , return : %s"), bResult, strResult);
		BSTR bstrMsg = strLog.AllocSysString();
		//AddLog(bstrMsg);
		SysFreeString(bstrMsg);
		*/


		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		
	}
	InternetCloseHandle(hOpen);
	return strResult;
}


void CApiAgent::parse_ReturnData(int cmd, Json::Value root)
{
	USES_CONVERSION;
	switch (cmd)
	{
	case API_SENSOR_REGISTER:
		//parse_Login(root);
		break;
	case API_CHECK_CONFIG:
		parse_Check_Config(root);
		break;
	}
}


void CApiAgent::parse_Check_Config(Json::Value root)
{
	USES_CONVERSION;
	int retCode = root.get("status", "").asInt();
	if (retCode == RETURN_SUCESS)
	{
		g_pServerDlg->m_bWasServerConnected = TRUE;
		Json::Value itemJson = root.get("objects", "");
		CString agent_ver;
		agent_ver.Format(_T("%s"), A2W(itemJson.get("agent_ver", "").asString().c_str()));

		g_pServerDlg->m_ConfigInfo.data_upload_second = itemJson.get("data_upload_second", "").asInt();
		g_pServerDlg->m_ConfigInfo.enable_upload = itemJson.get("enable_upload", "").asInt();
		g_pServerDlg->m_ConfigInfo.config_check_second = itemJson.get("config_check_second", "").asInt();
		g_pServerDlg->m_ConfigInfo.humi_R01_enable = itemJson.get("humi_R01_enable", "").asBool();
		g_pServerDlg->m_ConfigInfo.leak_R01_enable = itemJson.get("leak_R01_enable", "").asBool();
		g_pServerDlg->m_ConfigInfo.temp_R01_enable = itemJson.get("temp_R01_enable", "").asBool();

		g_pServerDlg->m_ConfigInfo.humi_R01_error = itemJson.get("humi_R01_error", "").asInt();
		g_pServerDlg->m_ConfigInfo.humi_R01_warning = itemJson.get("humi_R01_warning", "").asInt();
		g_pServerDlg->m_ConfigInfo.leak_R01_error = itemJson.get("leak_R01_error", "").asInt();
		g_pServerDlg->m_ConfigInfo.leak_R01_warning = itemJson.get("leak_R01_warning", "").asInt();
		g_pServerDlg->m_ConfigInfo.temp_R01_error = itemJson.get("temp_R01_error", "").asInt();
		g_pServerDlg->m_ConfigInfo.temp_R01_warning = itemJson.get("temp_R01_warning", "").asInt();
		int agent_restart = itemJson.get("agent_restart", "").asInt();
		if (agent_restart == 1)
			g_pServerDlg->EndDialog(IDOK);

		Json::Value arrayJson = itemJson.get("devices_info", "");

		while (!g_pServerDlg->m_listArea.IsEmpty())
			delete g_pServerDlg->m_listArea.RemoveTail();

		for (int i = 0; i < arrayJson.size(); i++)
		{
			Json::Value itemJson;
			itemJson = arrayJson.get(i, "");
			CString shop_name;
			shop_name.Format(_T("%s"), UTF8ToString((char *)itemJson.get("shop_name", "").asString().c_str()));
			CString mac;
			mac.Format(_T("%s"), A2W(itemJson.get("mac", "").asString().c_str()));
			CString last_updated_date;
			last_updated_date.Format(_T("%s"), A2W(itemJson.get("last_updated_date", "").asString().c_str()));
			CAreaInfo* pAreaInfo = new CAreaInfo();
			pAreaInfo->strShopName = shop_name;
			pAreaInfo->mac = mac.Trim();
			CShopInfo* pShopInfo = g_pServerDlg->GetShopInfo(mac);
			if (pShopInfo == NULL)
			{
				pShopInfo = new CShopInfo();
				pShopInfo->strShopName = shop_name;
				pShopInfo->m_objData.mac = mac;
				g_pServerDlg->m_ConfigInfo.m_pShowList->AddTail(pShopInfo);
			}
			pShopInfo->m_bIncident = itemJson.get("status", "").asInt();
			pShopInfo->m_last_updated_date = _wtol(last_updated_date);
			g_pServerDlg->m_listArea.AddTail(pAreaInfo);
			if (i == 0 && g_pServerDlg->m_pCurShopInfo == NULL)
				g_pServerDlg->m_pCurShopInfo = pShopInfo;
		}
		

		if (agent_ver.GetLength() > 1 
			&& AGENT_VERSION != agent_ver)
		{
			CString sCmd;
			sCmd.Format(_T("%d"), API_DOWNLOAD_FILE);
			APICALLDATA* pData = new APICALLDATA;
			pData->insert(APICALLDATA::value_type(KEY_CMD, sCmd));
			CString strUrl = _T("");
			//strUrl.Format(_T("http://%s:%d\\download\\%s"), g_pServerDlg->m_ConfigInfo.server_ip, g_pServerDlg->m_ConfigInfo.web_port, RUN_APP_NAME);
			//strUrl.Format(_T("http://%s:%d\\download\\%s"),_T("203.241.124.20"),443, RUN_APP_NAME);
			strUrl.Format(_T("download\\%s"),RUN_APP_NAME);
			pData->insert(APICALLDATA::value_type(_T("DOWNLOAD_URL"),strUrl));
			pData->insert(APICALLDATA::value_type(_T("FILE_NAME"), RUN_APP_NAME));
			g_pApiAgentDlg->AddAPI(pData);
		}
	}
	/*
	else if (retCode == RETURN_NOT_AREA_CODE)
	{
		if (g_pServerDlg->m_ConfigInfo.agent_type == AGENT_RESOURCE)
		{
			Json::Value itemJson = root.get("objects", "");
			CString agent_ver;
			agent_ver.Format(_T("%s"), A2W(itemJson.get("agent_ver", "").asString().c_str()));
			g_pServerDlg->m_ConfigInfo.data_upload_second = itemJson.get("data_upload_second", "").asInt();

			if (agent_ver.GetLength() > 1
				&& AGENT_VERSION_RESOURCE != agent_ver)
			{
				CString sCmd;
				sCmd.Format(_T("%d"), API_DOWNLOAD_FILE);
				APICALLDATA* pData = new APICALLDATA;
				pData->insert(APICALLDATA::value_type(KEY_CMD, sCmd));
				CString strUrl = _T("");
				//strUrl.Format(_T("http://%s:%d\\download\\%s"), g_pServerDlg->m_ConfigInfo.server_ip, g_pServerDlg->m_ConfigInfo.web_port, RUN_APP_NAME);
				//strUrl.Format(_T("http://%s:%d\\download\\%s"),_T("203.241.124.20"),443, RUN_APP_NAME);
				strUrl.Format(_T("download\\%s"), RUN_APP_NAME_RESOURCE);
				pData->insert(APICALLDATA::value_type(_T("DOWNLOAD_URL"), strUrl));
				pData->insert(APICALLDATA::value_type(_T("FILE_NAME"), RUN_APP_NAME));
				g_pApiAgentDlg->AddAPI(pData);
			}

		}
		g_pServerDlg->m_bWasServerConnected = TRUE;
	}
	*/
}


void CApiAgent::DownloadFileFromURL(APICALLDATA* pData, CString sServer, int nPort)
{
	USES_CONVERSION;
	APICALLDATA::iterator it;
	it = pData->find(_T("DOWNLOAD_URL"));
	if (it != pData->end())
	{
		CString sLocalPath;
		CString sURL = (*it).second;
		HANDLE h_wait_event = CreateEvent(NULL, TRUE, FALSE, _T("GIFW_01"));
		HINTERNET h_session = ::InternetOpen(_T("Tipssoft Example"), PRE_CONFIG_INTERNET_ACCESS, NULL, INTERNET_INVALID_PORT_NUMBER, 0);
		HINTERNET h_connect = ::InternetConnect(h_session, sServer, nPort, _T(""), _T(""), INTERNET_SERVICE_HTTP, 0, 0);
		//HINTERNET h_connect = ::InternetConnect(h_session, _T("203.241.124.20"), 443, _T(""), _T(""), INTERNET_SERVICE_HTTP, 0, 0);
		// 접속된 사이트에서 특정 경로에 있는 파일을 읽을수 있도록 요청한다. 그림의 경로를 명시할때는 사이트 주소를 제외한 나머지 경로만
		// 명시하면 됩니다.
		HINTERNET h_http_file = ::HttpOpenRequest(h_connect, _T("GET"), sURL,
			HTTP_VERSION, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
		// 지정한 파일의 내용을 전송할것을 요청한다.
		BOOL request_flag = ::HttpSendRequest(h_http_file, NULL, 0, 0, 0);
		if (request_flag == TRUE) {

			// 전송받은 그림을 저장할 파일을 생성합니다.
			it = pData->find(_T("FILE_NAME"));
			if (it != pData->end())
			{
				CString sFileName = (*it).second;

				sLocalPath.Format(_T("%s\\%s"), g_pServerDlg->GetProgramPathW(), sFileName);
				FILE* p_file = fopen(W2A(sLocalPath), "wb");

				char buffer[1025], error_count = 0;
				DWORD read_byte = 0, buffer_size = 1024;

				// 한번에 최대 1024 크기만큼 네트워크를 통해서 그림정보를 수신합니다.
				while (::InternetReadFile(h_http_file, buffer, buffer_size, &read_byte)) {
					// 1바이트라도 수신했다면 읽은 정보를 파일에 저장한다.
					if (read_byte > 0 && p_file != NULL) fwrite(buffer, 1, read_byte, p_file);

					if (read_byte < 1024) {
						// 1024 보다 적은 경우를 읽은 경우, 렉에 의해서 못읽는 경우도 있기 때문에
						// 최대 5번까지 재시도한다. 재시도를 하는 경우 50ms의 지연시간을 부여하여
						// 전송을 기다리도록 한다.
						error_count++;
						if (error_count > 5) break;
						else ::WaitForSingleObject(h_wait_event, 50);
					}
					else error_count = 0;
				}


				// 그림을 수신하기 위해 생성한 정보를 닫는다.
				::InternetCloseHandle(h_http_file);

				// 그림을 저장한 파일을 닫는다.
				if (p_file != NULL) fclose(p_file);
			}

		}

		// 인터넷을 사용하기 위해서 연결한 핸들과 세션 핸들을 닫는다.
		::InternetCloseHandle(h_connect);
		::InternetCloseHandle(h_session);

		// 이벤트 객체를 제거한다.
		CloseHandle(h_wait_event);

		///////////////다운로드 파일 실행///////////
		g_pServerDlg->UpdateDownloadCompleteToIni(1);
	}
}
