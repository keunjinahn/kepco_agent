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
		strPacket.Format(_T("mac : %s , device no : %d , sensor state : %d, ����1 : %d,����2 : %d,����1 : %.1f,����2 : %.1f,����3 : %.1f,����4 : %.1f,�µ�1 : %.1f,�µ�2 : %.1f,�µ�3 : %.1f,�µ�4 : %.1f") \
		,mac, device_no, sensor_state, leak_1_value, leak_2_value, humi_1_value, humi_2_value, humi_3_value, humi_4_value, temp_1_value, temp_2_value, temp_3_value, temp_4_value);
		return strPacket;
	}
	int device_no;
	int sensor_state;
	int leak_1_value;
	int leak_2_value;
	float temp_1_value;
	float temp_2_value;
	float temp_3_value;
	float temp_4_value;
	float humi_1_value;
	float humi_2_value;
	float humi_3_value;
	float humi_4_value;
	CString mac;
};

class CShopInfo
{
public :
	KBPKT_DATA_OBJ m_objData;
	CString strShopName;
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
	};
	CString locationname;
	CString locationcode;
	CStringList listShop;
	CStringList listShopMac;
	CPtrList* m_pShowList;
};

#pragma pack()

#endif