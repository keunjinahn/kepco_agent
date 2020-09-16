#ifndef __KBPACKETH_H
#define __KBPACKETH_H

#pragma pack(1)
#define SZ_ID			21
#define SZ_NAME			25
#define SZ_PROFILE		50
#define SZ_ITEMUID		50
#define SZ_ROOMUID		20
#define SZ_TIME			20
#define SZ_ROOMTITLE	50
#define SZ_MSG			1024
#define SZ_IDXS			1024
#define SZ_RESERVED		100

#define PKT_TYPE_RELAY	3
#define PKT_TYPE_REQ	2

#define SOCK_CMD_DATA	5
#define SOCK_CMD_LOGIN_RQ	30001
#define SOCK_CMD_LOGIN_RS	30002
#define SOCK_CMD_CHAT_MSG	30003
#define SOCK_CMD_PING		30004
#define SOCK_CMD_ROOM_OUT	30005
#define SOCK_CMD_ROOM_JOIN	30006
#define SOCK_CMD_CHAT_MSG_ACK	30007
#define SOCK_CMD_CHAT_MSG_RECORVERY	30008
enum LOGTYPE { LOG_CONNECT = 0, LOG_LOGIN, LOG_LEAVE, LOG_SOCKCLOSE, LOG_CHANNELMAKE, LOG_JOIN, LOG_CHECK, LOG_PING , LOG_SEND
};
enum AGENT_TYPE{AGENT_SENSOR=1,AGENT_RESOURCE=2};

float Hex2float(char* hexadecimal);
int char2Int(char c);
float GetGenValue(float time, float amplitude, int sensor_type);
float GetGenValue2(float time, float amplitude);

class KBPKT_HDR
{
public:
	BYTE cmd;
	char mac[12];
	char size[2];
};

#define SENSOR_VALUE_LEN 5
class KBPKT_DATA
{
public:
	BYTE ack;
	char device_no[2];
	char leak_1_value;
	char leak_2_value;
	char temp_1_value[SENSOR_VALUE_LEN];
	char humi_1_value[SENSOR_VALUE_LEN];
	char temp_2_value[SENSOR_VALUE_LEN];
	char humi_2_value[SENSOR_VALUE_LEN];
	char temp_3_value[SENSOR_VALUE_LEN];
	char humi_3_value[SENSOR_VALUE_LEN];
	char temp_4_value[SENSOR_VALUE_LEN];
	char humi_4_value[SENSOR_VALUE_LEN];
	BYTE etx;
	BYTE end;
};

class KBPKT_DATA_OBJ
{
public:
	KBPKT_DATA_OBJ()
	{
		mac = _T("");
		device_no = 0;
		sensor_state = 0;
		leak_1_value = 0;
		leak_2_value = 0;
		temp_1_value = -999;
		temp_2_value = -999;
		temp_3_value = -999;
		temp_4_value = -999;
		humi_1_value = -999;
		humi_2_value = -999;
		humi_3_value = -999;
		humi_4_value = -999;
		leak_1_value_total = 0;
		leak_1_value_min = 0;
		leak_1_value_max = 0;
		leak_2_value_total = 0;
		leak_2_value_min = 0;
		leak_2_value_max = 0;
		temp_1_value_total = 0;
		temp_1_value_min = 0;
		temp_1_value_max = 0;
		temp_2_value_total = 0;
		temp_2_value_min = 0;
		temp_2_value_max = 0;
		temp_3_value_total = 0;
		temp_3_value_min = 0;
		temp_3_value_max = 0;
		temp_4_value_total = 0;
		temp_4_value_min = 0;
		temp_4_value_max = 0;
		humi_1_value_total = 0;
		humi_1_value_min = 0;
		humi_1_value_max = 0;
		humi_2_value_total = 0;
		humi_2_value_min = 0;
		humi_2_value_max = 0;
		humi_3_value_total = 0;
		humi_3_value_min = 0;
		humi_3_value_max = 0;
		humi_4_value_total = 0;
		humi_4_value_min = 0;
		humi_4_value_max = 0;

		temp_1_state = 0;
		temp_2_state = 0;
		temp_3_state = 0;
		temp_4_state = 0;
		humi_1_state = 0;
		humi_2_state = 0;
		humi_3_state = 0;
		humi_4_state = 0;

		dwCount = 0;
	};
	void setData(CString _mac, char* _device_no, char* _sensor_state, char _leak_1_value, char _leak_2_value,
		char* _temp_1_value, char* _temp_2_value, char* _temp_3_value, char* _temp_4_value,
		char* _humi_1_value, char* _humi_2_value, char* _humi_3_value, char* _humi_4_value
	)
	{
		mac = _mac;
		device_no = atoi(_device_no);
		sensor_state = atoi(_sensor_state);

		CString sValue;
		sValue.Format(_T("%c"), _leak_1_value);
		leak_1_value = _wtoi(sValue);
		sValue.Format(_T("%c"), _leak_2_value);
		leak_2_value = _wtoi(sValue);
		temp_1_value = atof(_temp_1_value);
		temp_2_value = atof(_temp_2_value);
		temp_3_value = atof(_temp_3_value);
		temp_4_value = atof(_temp_4_value);
		humi_1_value = atof(_humi_1_value);
		humi_2_value = atof(_humi_2_value);
		humi_3_value = atof(_humi_3_value);
		humi_4_value = atof(_humi_4_value);
	}
	
	float convertValue(char* sValue)
	{
		if (atof(sValue) > 99.9)
			return 99.9;
		else if (atof(sValue) < -99.9)
			return -99.9;
		return atof(sValue);
	}
	void setDataParser(CString srcData)
	{
		USES_CONVERSION;
		mac.Format(_T("%s"), srcData.Left(12));

		char strDeviceNo[3];
		memcpy(strDeviceNo, W2A(srcData.Mid(12,1)),1);
		strDeviceNo[2] = '\0';
		device_no = atoi(strDeviceNo);

		char strValue[3];
		memcpy(strValue, W2A(srcData.Mid(13, 1)),1);
		strValue[2] = '\0';
		sensor_state = atoi(strValue);
		memset(strValue, 3, '\0');

		CString sValue;
		sValue.Format(_T("%c"), W2A(srcData.Mid(14, 1)), 1);
		leak_1_value = _wtoi(sValue);
		sValue.Format(_T("%c"), W2A(srcData.Mid(15, 1)), 1);
		leak_2_value = _wtoi(sValue);

		int nOffset = 16;
		char strValue2[6];
		memcpy(strValue2, W2A(srcData.Mid(nOffset, SENSOR_VALUE_LEN)), SENSOR_VALUE_LEN);
		strValue2[SENSOR_VALUE_LEN] = '\0';
		temp_1_value = convertValue(strValue2);
		nOffset += SENSOR_VALUE_LEN;

		memcpy(strValue2, W2A(srcData.Mid(nOffset, SENSOR_VALUE_LEN)), SENSOR_VALUE_LEN);
		strValue2[SENSOR_VALUE_LEN] = '\0';
		temp_2_value = convertValue(strValue2);
		nOffset += SENSOR_VALUE_LEN;

		memcpy(strValue2, W2A(srcData.Mid(nOffset, SENSOR_VALUE_LEN)), SENSOR_VALUE_LEN);
		strValue2[SENSOR_VALUE_LEN] = '\0';
		temp_3_value = convertValue(strValue2);
		nOffset += SENSOR_VALUE_LEN;

		memcpy(strValue2, W2A(srcData.Mid(nOffset, SENSOR_VALUE_LEN)), SENSOR_VALUE_LEN);
		strValue2[SENSOR_VALUE_LEN] = '\0';
		temp_4_value = convertValue(strValue2);
		nOffset += SENSOR_VALUE_LEN;

		memcpy(strValue2, W2A(srcData.Mid(nOffset, SENSOR_VALUE_LEN)), SENSOR_VALUE_LEN);
		strValue2[SENSOR_VALUE_LEN] = '\0';
		humi_1_value = atof(strValue2);
		nOffset += SENSOR_VALUE_LEN;

		memcpy(strValue2, W2A(srcData.Mid(nOffset, SENSOR_VALUE_LEN)), SENSOR_VALUE_LEN);
		strValue2[SENSOR_VALUE_LEN] = '\0';
		humi_2_value = convertValue(strValue2);
		nOffset += SENSOR_VALUE_LEN;

		memcpy(strValue2, W2A(srcData.Mid(nOffset, SENSOR_VALUE_LEN)), SENSOR_VALUE_LEN);
		strValue2[SENSOR_VALUE_LEN] = '\0';
		humi_3_value = convertValue(strValue2);
		nOffset += SENSOR_VALUE_LEN;

		memcpy(strValue2, W2A(srcData.Mid(nOffset, SENSOR_VALUE_LEN)), SENSOR_VALUE_LEN);
		strValue2[SENSOR_VALUE_LEN] = '\0';
		humi_4_value = convertValue(strValue2);
	}

	void setPacket(KBPKT_HDR* pHdr,KBPKT_DATA* pPkt)
	{
		USES_CONVERSION;
		char strMac[13];
		memcpy(strMac, pHdr->mac, 12);
		strMac[12] = '\0';
		mac.Format(_T("%s"), A2W(strMac));
		char strDeviceNo[3];
		memcpy(strDeviceNo, pPkt->device_no, 2);
		strDeviceNo[2] = '\0';
		device_no = atoi(strDeviceNo);

		CString sValue;
		sValue.Format(_T("%c"), pPkt->leak_1_value);
		leak_1_value = _wtoi(sValue);
		sValue.Format(_T("%c"), pPkt->leak_2_value);
		leak_2_value = _wtoi(sValue);


		char strValue2[SENSOR_VALUE_LEN+1];

		memcpy(strValue2, pPkt->temp_1_value, SENSOR_VALUE_LEN);
		strValue2[SENSOR_VALUE_LEN] = '\0';
		temp_1_state = char2Int(strValue2[0]);
		temp_1_value = Hex2float(&strValue2[1]);

		memcpy(strValue2, pPkt->temp_2_value, SENSOR_VALUE_LEN);
		strValue2[SENSOR_VALUE_LEN] = '\0';
		temp_2_state = char2Int(strValue2[0]);
		temp_2_value = Hex2float(&strValue2[1]);

		memcpy(strValue2, pPkt->temp_3_value, SENSOR_VALUE_LEN);
		strValue2[SENSOR_VALUE_LEN] = '\0';
		temp_3_state = char2Int(strValue2[0]);
		temp_3_value = Hex2float(&strValue2[1]);

		memcpy(strValue2, pPkt->temp_4_value, SENSOR_VALUE_LEN);
		strValue2[SENSOR_VALUE_LEN] = '\0';
		temp_4_state = char2Int(strValue2[0]);
		temp_4_value = Hex2float(&strValue2[1]);

		memcpy(strValue2, pPkt->humi_1_value, SENSOR_VALUE_LEN);
		strValue2[SENSOR_VALUE_LEN] = '\0';
		humi_1_state = char2Int(strValue2[0]);
		humi_1_value = Hex2float(&strValue2[1]);


		memcpy(strValue2, pPkt->humi_2_value, SENSOR_VALUE_LEN);
		strValue2[SENSOR_VALUE_LEN] = '\0';
		humi_2_state = char2Int(strValue2[0]);
		humi_2_value = Hex2float(&strValue2[1]);


		memcpy(strValue2, pPkt->humi_3_value, SENSOR_VALUE_LEN);
		strValue2[SENSOR_VALUE_LEN] = '\0';
		humi_3_state = char2Int(strValue2[0]);
		humi_3_value = Hex2float(&strValue2[1]);


		memcpy(strValue2, pPkt->humi_4_value, SENSOR_VALUE_LEN);
		strValue2[SENSOR_VALUE_LEN] = '\0';
		humi_4_state = char2Int(strValue2[0]);
		humi_4_value = Hex2float(&strValue2[1]);

	}

	CString getPacketLog() 
	{
		CString strPacket = _T("");
		strPacket.Format(_T("mac : %s , device no : %d , sensor state : %d, 누수1 : %d,누수2 : %d,습도1 : %.1f,습도2 : %.1f,습도3 : %.1f,습도4 : %.1f,온도1 : %.1f,온도2 : %.1f,온도3 : %.1f,온도4 : %.1f") \
		,mac, device_no, sensor_state, leak_1_value, leak_2_value, humi_1_value, humi_2_value, humi_3_value, humi_4_value, temp_1_value, temp_2_value, temp_3_value, temp_4_value);
		return strPacket;
	}
	CString cValue(float fValue)
	{
		CString sValue;
		if (fValue >= 0)
			sValue.Format(_T("+%.1f"),fValue);
		else
			sValue.Format(_T("%.1f"), fValue);
		return sValue;
	}

	CString getPacket()
	{
		CString strPacket = _T("");
		strPacket.Format(_T("%c%s%d%d%d%d%s%s%s%s%s%s%s%s%c") \
			,ack, mac, device_no, sensor_state, leak_1_value, leak_2_value, cValue(temp_1_value), cValue(temp_2_value), cValue(temp_3_value), cValue(temp_4_value), cValue(humi_1_value), cValue(humi_2_value), cValue(humi_3_value), cValue(humi_4_value),etx);
		return strPacket;
	}

	CString getData()
	{
		CString strPacket = _T("");
		strPacket.Format(_T("%s%d%d%d%d%s%s%s%s%s%s%s%s") \
			, mac, device_no, sensor_state, leak_1_value, leak_2_value, cValue(temp_1_value), cValue(temp_2_value), cValue(temp_3_value), cValue(temp_4_value), cValue(humi_1_value), cValue(humi_2_value), cValue(humi_3_value), cValue(humi_4_value));
		return strPacket;
	}

	int device_no;
	int sensor_state;
	int leak_1_value;
	int leak_1_value_total;
	int leak_1_value_min;
	int leak_1_value_max;
	int leak_2_value;
	int leak_2_value_total;
	int leak_2_value_min;
	int leak_2_value_max;
	float temp_1_value;
	float temp_1_value_total;
	float temp_1_value_min;
	float temp_1_value_max;
	float temp_2_value;
	float temp_2_value_total;
	float temp_2_value_min;
	float temp_2_value_max;
	float temp_3_value;
	float temp_3_value_total;
	float temp_3_value_min;
	float temp_3_value_max;
	float temp_4_value;
	float temp_4_value_total;
	float temp_4_value_min;
	float temp_4_value_max;

	float humi_1_value;
	float humi_1_value_total;
	float humi_1_value_min;
	float humi_1_value_max;

	float humi_2_value;
	float humi_2_value_total;
	float humi_2_value_min;
	float humi_2_value_max;
	float humi_3_value;
	float humi_3_value_total;
	float humi_3_value_min;
	float humi_3_value_max;

	float humi_4_value;
	float humi_4_value_total;
	float humi_4_value_min;
	float humi_4_value_max;

	int temp_1_state;
	int temp_2_state;
	int temp_3_state;
	int temp_4_state;
	int humi_1_state;
	int humi_2_state;
	int humi_3_state;
	int humi_4_state;


	DWORD dwCount;
	CString mac;
	BYTE ack='-';
	BYTE etx='9';
};

class CShopInfo
{
public :
	CShopInfo()
	{

		m_bIncident = FALSE;
		CTime t = CTime::GetCurrentTime();
		CString sTime = _T("");
		sTime.Format(_T("%02d%02d%02d%02d%02d"), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
		m_currentTic = ::GetTickCount();
		m_current_date = _wtol(sTime);
		m_last_updated_date = _wtol(sTime);
	};
	KBPKT_DATA_OBJ m_objData;
	CString strShopName;

	BOOL m_bIncident;
	DWORD m_currentTic;
	DWORD m_current_date;
	DWORD m_last_updated_date;
};

class CAreaInfo
{
public:
	CAreaInfo()
	{
		m_bActive = FALSE;
		mac = _T("");
		strShopName = _T("");
	};
	CString mac;
	CString strShopName;
	BOOL m_bActive;
};

class KBPKT_LOGIN_RS
{
public:
	DWORD flags;
	BOOL bSameID;
};

class KBPKT_PING
{
public:
	DWORD type;
};


class KBPKT_CHAT_MSG
{
public:
	char TMI_PROF_IMAGE_PATH[SZ_PROFILE];
	char TCM_CHAT_ROOM_TITLE[SZ_ROOMTITLE];
	char TCM_CHAT_TIME[SZ_TIME];
	char TO_TMI_IDXS[SZ_IDXS];
	char TCM_MSG_UID[SZ_ITEMUID];
	char TCM_CHAT_ROOM_UID[SZ_ROOMUID];
	BYTE TCM_MSG_TYPE;
	char TCM_MSG[SZ_MSG];
	char TCM_CHAT_ROOM_RESERVED_1[SZ_RESERVED];
	char TCM_CHAT_ROOM_RESERVED_2[SZ_RESERVED];
};

class KBPKT_CHAT_MSG_ACK
{
public:
	char TO_TMI_IDX[SZ_ID];
	char TCM_MSG_UID[SZ_ITEMUID];
	char TCM_CHAT_ROOM_UID[SZ_ROOMUID];
};

class KBPKT_CHAT_MSG_RECORVERY
{
public:
	char TCM_MSG_UID[SZ_ITEMUID];
	char TCM_CHAT_ROOM_UID[SZ_ROOMUID];
	char TO_TMI_IDXS[SZ_IDXS];
	
};

class KBPKT_ROOM_OUT
{
public:
	char TCM_CHAT_ROOM_UID[SZ_ROOMUID];
	char TO_TMI_IDXS[SZ_IDXS];
};

class KBPKT_ROOM_JOIN
{
public:
	char TMI_PROF_IMAGE_PATH[SZ_PROFILE];
	char TCM_CHAT_ROOM_UID[SZ_ROOMUID];
	char TO_TMI_IDXS[SZ_IDXS];
};

class CConfigInfo
{
public:
	CConfigInfo()
	{
		m_pShowList = new CPtrList();
		
		humi_R01_error = 100;
		humi_R01_warning = 90;
		leak_R01_error = 1;
		leak_R01_warning = 1;
		temp_R01_error = 40;
		temp_R01_warning = 40;
		humi_R01_enable = FALSE;
		leak_R01_enable = FALSE;
		temp_R01_enable = FALSE;
		agent_ver = _T("1.0.0");
		update_completed = 0;
		server_ip = _T("localhost");
		port = 5000;
		web_port = 8080;
		agent_type = AGENT_SENSOR;
		config_check_second = 30000;
		enable_upload = 1;
		data_upload_second = 1000;
	};
	CString locationname;
	CString locationcode;
	CStringList listShop;
	CStringList listShopMac;
	CPtrList* m_pShowList;
	BOOL leak_R01_enable;
	BOOL temp_R01_enable;
	BOOL humi_R01_enable;
	int humi_R01_error;
	int humi_R01_warning;
	int leak_R01_error;
	int leak_R01_warning;
	int temp_R01_error;
	int temp_R01_warning;
	CString agent_ver;
	int update_completed;
	CString server_ip;
	int port;
	int web_port;
	AGENT_TYPE agent_type;
	int config_check_second;
	int enable_upload;
	int data_upload_second;
};


static char* dec2Hex(int decimal)
{
	char hexadecimal[20] = { 0, };    // 16진수로 된 문자열을 저장할 배열
	char hexadecimal2[20] = { 0, };
	int position = 0;
	while (1)
	{
		int mod = decimal % 16;    // 16으로 나누었을 때 나머지를 구함
		if (mod < 10) // 나머지가 10보다 작으면
		{
			// 숫자 0의 ASCII 코드 값 48 + 나머지
			hexadecimal[position] = 48 + mod;
		}
		else    // 나머지가 10보다 크거나 같으면
		{
			// 나머지에서 10을 뺀 값과 영문 대문자 A의 ASCII 코드 값 65를 더함
			hexadecimal[position] = 65 + (mod - 10);
		}

		decimal = decimal / 16;    // 16으로 나눈 몫을 저장

		position++;    // 자릿수 변경

		if (decimal == 0)    // 몫이 0이되면 반복을 끝냄
			break;
	}

	int k = 0;
	// 배열의 요소를 역순으로 출력
	for (int i = position - 1; i >= 0; i--)
	{
		hexadecimal2[k++] = hexadecimal[i];
	}
	return  &hexadecimal2[0];
}

static int Hex2Com(char* hexadecimal)
{
	int decimal = 0;                  // 10진수를 저장할 변수
	int position = 0;
	for (int i = strlen(hexadecimal) - 1; i >= 0; i--)    // 문자열을 역순으로 반복
	{
		char ch = hexadecimal[i];         // 각 자릿수에 해당하는 문자를 얻음

		if (ch >= 48 && ch <= 57)         // 문자가 0~9이면(ASCII 코드 48~57)
		{
			// 문자에서 0에 해당하는 ASCII 코드 값을 빼고
			// 16에 자릿수를 거듭제곱한 값을 곱함
			decimal += (ch - 48) * pow(16, position);
		}
		else if (ch >= 65 && ch <= 70)    // 문자가 A~F이면(ASCII 코드 65~70)
		{                                 // 대문자로 된 16진수의 처리
			// 문자에서 (A에 해당하는 ASCII 코드 값 - 10)을 빼고
			// 16에 자릿수를 거듭제곱한 값을 곱함
			decimal += (ch - (65 - 10)) * pow(16, position);
		}
		else if (ch >= 97 && ch <= 102)   // 문자가 a~f이면(ASCII 코드 97~102)
		{                                 // 소문자로 된 16진수의 처리
			// 문자에서 (a에 해당하는 ASCII 코드 값 - 10)을 빼고
			// 16에 자릿수를 거듭제곱한 값을 곱함
			decimal += (ch - (97 - 10)) * pow(16, position);
		}

		position++;
	}

	return -(65536 - decimal);
}

static int Hex2dec(char* hexadecimal)
{
	int decimal = 0;                  // 10진수를 저장할 변수
	int position = 0;
	for (int i = strlen(hexadecimal) - 1; i >= 0; i--)    // 문자열을 역순으로 반복
	{
		char ch = hexadecimal[i];         // 각 자릿수에 해당하는 문자를 얻음

		if (ch >= 48 && ch <= 57)         // 문자가 0~9이면(ASCII 코드 48~57)
		{
			// 문자에서 0에 해당하는 ASCII 코드 값을 빼고
			// 16에 자릿수를 거듭제곱한 값을 곱함
			decimal += (ch - 48) * pow(16, position);
		}
		else if (ch >= 65 && ch <= 70)    // 문자가 A~F이면(ASCII 코드 65~70)
		{                                 // 대문자로 된 16진수의 처리
			// 문자에서 (A에 해당하는 ASCII 코드 값 - 10)을 빼고
			// 16에 자릿수를 거듭제곱한 값을 곱함
			decimal += (ch - (65 - 10)) * pow(16, position);
		}
		else if (ch >= 97 && ch <= 102)   // 문자가 a~f이면(ASCII 코드 97~102)
		{                                 // 소문자로 된 16진수의 처리
			// 문자에서 (a에 해당하는 ASCII 코드 값 - 10)을 빼고
			// 16에 자릿수를 거듭제곱한 값을 곱함
			decimal += (ch - (97 - 10)) * pow(16, position);
		}

		position++;
	}

	return decimal;
}

static float Hex2float(char* hexadecimal)
{
	int decimal = 0;                  // 10진수를 저장할 변수
	int position = 0;
	for (int i = strlen(hexadecimal) - 1; i >= 0; i--)    // 문자열을 역순으로 반복
	{
		char ch = hexadecimal[i];         // 각 자릿수에 해당하는 문자를 얻음

		if (ch >= 48 && ch <= 57)         // 문자가 0~9이면(ASCII 코드 48~57)
		{
			// 문자에서 0에 해당하는 ASCII 코드 값을 빼고
			// 16에 자릿수를 거듭제곱한 값을 곱함
			decimal += (ch - 48) * pow(16, position);
		}
		else if (ch >= 65 && ch <= 70)    // 문자가 A~F이면(ASCII 코드 65~70)
		{                                 // 대문자로 된 16진수의 처리
			// 문자에서 (A에 해당하는 ASCII 코드 값 - 10)을 빼고
			// 16에 자릿수를 거듭제곱한 값을 곱함
			decimal += (ch - (65 - 10)) * pow(16, position);
		}
		else if (ch >= 97 && ch <= 102)   // 문자가 a~f이면(ASCII 코드 97~102)
		{                                 // 소문자로 된 16진수의 처리
			// 문자에서 (a에 해당하는 ASCII 코드 값 - 10)을 빼고
			// 16에 자릿수를 거듭제곱한 값을 곱함
			decimal += (ch - (97 - 10)) * pow(16, position);
		}

		position++;
	}

	char strValue[4];
	memset(strValue, '\0', 4);
	CString strFloat;
	if (decimal < 1000)
	{
		if (decimal > 999)
			decimal = 999;
		sprintf_s(strValue, "%03d", decimal);
		strFloat.Format(_T("%c%c.%c"), strValue[0], strValue[1], strValue[2]);
	}
	else
	{
		int _decimal = (65536 - decimal);
		if (_decimal > 999)
			_decimal = 999;
		sprintf_s(strValue, "%03d", _decimal);
		strFloat.Format(_T("-%c%c.%c"), strValue[0], strValue[1], strValue[2]);
	}
	return _wtof(strFloat);
}
static int char2Int(char c)
{
	return (int)c - 48;
}

/*
static float GetGenValue(float time, float amplitude)
{
	float retData = float(200.0 * sin(time) * 0.1 + 30.0 + amplitude);
	return retData;

}
*/
static float GetGenValue(float time, float amplitude,int sensor_type)
{
	float s_value = 22.1;
	float o_value = 0;
	float offset = 0.1;
	if ((int)time % 2 == 0)
		offset = -0.1;
	else
		offset = 0.1;
	if((int)time % 192 == 0)
		offset = 2.0;
	else if ((int)time % 193 == 0)
		offset = 3.0;
	else if ((int)time % 194 == 0)
		offset = 4.0;
	else if ((int)time % 195 == 0)
		offset = 5.0;
	else if ((int)time % 196 == 0)
		offset = 6.0;
	else if ((int)time % 197 == 0)
		offset = 7.0;
	else if ((int)time % 198 == 0)
		offset = 8.0;
	else if ((int)time % 199 == 0)
		offset = 9.0;
	else if ((int)time % 200 == 0)
	{
		offset = 20.0;
		s_value = 20.1;
	}

	float retData = s_value + offset;
	return retData;

}

static float GetGenValue2(float time,float amplitude, int sensor_type)
{


	float value = 0.0;
	float frequency = 1.0;
	float phase = 2.0;
	float invert = 1.0;
	float offset = 2.0;

	switch (sensor_type)
	{
	case 1:  offset = offset + (float)10.0; break;
	case 2:  offset = offset - (float)10.0; break;
	case 3:  offset = offset + (float)20.0; break;
	case 4:  offset = offset - (float)20.0; break;
	case 5:  offset = offset + (float)30.0; break;
	case 6:  offset = offset - (float)30.0; break;
	case 7:  offset = offset + (float)40.0; break;
	case 8:  offset = offset - (float)40.0; break;
	}

	float t = frequency * time + phase;
	value = (float)sin(20.0 * 3.14195 * t);
	float retData = (invert * amplitude * value + offset);
	return retData;

}

#pragma pack()

#endif