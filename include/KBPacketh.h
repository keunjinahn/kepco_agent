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
class KBPKT_HDR
{
public:
	BYTE cmd;
	char mac[12];
	char size[2];
};

class KBPKT_DATA
{
public:
	BYTE ack;
	char device_no[2];
	char sensor_state[2];
	char leak_1_value;
	char leak_2_value;
	char temp_1_value[4];
	char humi_1_value[4];
	char temp_2_value[4];
	char humi_2_value[4];
	char temp_3_value[4];
	char humi_3_value[4];
	char temp_4_value[4];
	char humi_4_value[4];
	BYTE etx;
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
		memcpy(strValue2, W2A(srcData.Mid(nOffset, 5)), 5);
		strValue2[5] = '\0';
		temp_1_value = atof(strValue2);
		nOffset += 5;

		memcpy(strValue2, W2A(srcData.Mid(nOffset, 5)), 5);
		strValue2[5] = '\0';
		temp_2_value = atof(strValue2);
		nOffset += 5;

		memcpy(strValue2, W2A(srcData.Mid(nOffset, 5)), 5);
		strValue2[5] = '\0';
		temp_3_value = atof(strValue2);
		nOffset += 5;

		memcpy(strValue2, W2A(srcData.Mid(nOffset, 5)), 5);
		strValue2[5] = '\0';
		temp_4_value = atof(strValue2);
		nOffset += 5;

		memcpy(strValue2, W2A(srcData.Mid(nOffset, 5)), 5);
		strValue2[5] = '\0';
		humi_1_value = atof(strValue2);
		nOffset += 5;

		memcpy(strValue2, W2A(srcData.Mid(nOffset, 5)), 5);
		strValue2[5] = '\0';
		humi_2_value = atof(strValue2);
		nOffset += 5;

		memcpy(strValue2, W2A(srcData.Mid(nOffset, 5)), 5);
		strValue2[5] = '\0';
		humi_3_value = atof(strValue2);
		nOffset += 5;

		memcpy(strValue2, W2A(srcData.Mid(nOffset, 5)), 5);
		strValue2[5] = '\0';
		humi_4_value = atof(strValue2);
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

		char strValue[3];
		memcpy(strValue, pPkt->sensor_state, 2);
		strValue[2] = '\0';
		sensor_state = atoi(strValue);
		memset(strValue, 3, '\0');

		CString sValue;
		sValue.Format(_T("%c"), pPkt->leak_1_value);
		leak_1_value = _wtoi(sValue);
		sValue.Format(_T("%c"), pPkt->leak_2_value);
		leak_2_value = _wtoi(sValue);


		char strValue2[5];
		memcpy(strValue2, pPkt->humi_1_value,4);
		strValue2[4] = '\0';
		sValue.Format(_T("%c%c%c.%c"), strValue2[0], strValue2[1], strValue2[2], strValue2[3]);
		humi_1_value = _wtof(sValue);
		memset(strValue2, 5, '\0');

		memcpy(strValue2, pPkt->humi_2_value, 4);
		strValue2[4] = '\0';
		sValue.Format(_T("%c%c%c.%c"), strValue2[0], strValue2[1], strValue2[2], strValue2[3]);
		humi_2_value = _wtof(sValue);
		memset(strValue2, 5, '\0');

		memcpy(strValue2, pPkt->humi_3_value, 4);
		strValue2[4] = '\0';
		sValue.Format(_T("%c%c%c.%c"), strValue2[0], strValue2[1], strValue2[2], strValue2[3]);
		humi_3_value = _wtof(sValue);
		memset(strValue2, 5, '\0');

		memcpy(strValue2, pPkt->humi_4_value, 4);
		strValue2[4] = '\0';
		sValue.Format(_T("%c%c%c.%c"), strValue2[0], strValue2[1], strValue2[2], strValue2[3]);
		humi_4_value = _wtof(sValue);
		memset(strValue2, 5, '\0');


		memcpy(strValue2, pPkt->temp_1_value, 4);
		strValue2[4] = '\0';
		sValue.Format(_T("%c%c%c.%c"), strValue2[0], strValue2[1], strValue2[2], strValue2[3]);
		temp_1_value = _wtof(sValue);
		memset(strValue2, 5, '\0');

		memcpy(strValue2, pPkt->temp_2_value, 4);
		strValue2[4] = '\0';
		sValue.Format(_T("%c%c%c.%c"), strValue2[0], strValue2[1], strValue2[2], strValue2[3]);
		temp_2_value = _wtof(sValue);
		memset(strValue2, 5, '\0');

		memcpy(strValue2, pPkt->temp_3_value, 4);
		strValue2[4] = '\0';
		sValue.Format(_T("%c%c%c.%c"), strValue2[0], strValue2[1], strValue2[2], strValue2[3]);
		temp_3_value = _wtof(sValue);
		memset(strValue2, 5, '\0');

		memcpy(strValue2, pPkt->temp_4_value, 4);
		strValue2[4] = '\0';
		sValue.Format(_T("%c%c%c.%c"), strValue2[0], strValue2[1], strValue2[2], strValue2[3]);
		temp_4_value = _wtof(sValue);
		memset(strValue2, 5, '\0');


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
	DWORD dwCount;
	CString mac;
	BYTE ack='1';
	BYTE etx='1';
};

class CShopInfo
{
public :
	CShopInfo()
	{
		m_bActive = FALSE;
		m_bIncident = FALSE;
		m_currentTic = ::GetTickCount();
	};
	KBPKT_DATA_OBJ m_objData;
	CString strShopName;
	BOOL m_bActive;
	BOOL m_bIncident;
	DWORD m_currentTic;
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
		data_upload_second = 1000;
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
	};
	CString locationname;
	CString locationcode;
	CStringList listShop;
	CStringList listShopMac;
	CPtrList* m_pShowList;
	int data_upload_second;
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
};

#pragma pack()

#endif