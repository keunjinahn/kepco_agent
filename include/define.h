#ifndef __DEFINE_H
#define __DEFINE_H

#define SERVER_IP_ADDRESS	_T("182.162.96.205")
#define API_SERVER_IP _T("182.162.96.205")
#define FILE_SERVER_IP _T("182.162.96.205")
#define API_SERVER_PORT 8081

//#define SERVER_IP_ADDRESS	_T("14.63.213.180")
//#define API_SERVER_IP _T("14.63.213.180")
//#define FILE_SERVER_IP _T("14.63.213.180")
//#define API_SERVER_PORT 80
//
//#define SERVER_IP_ADDRESS	_T("10.7.5.62")
//#define API_SERVER_IP _T("10.7.5.62")
//#define FILE_SERVER_IP _T("10.7.5.62")
//#define API_SERVER_PORT 9090

//#define SERVER_IP_ADDRESS	_T("124.243.59.19")
//#define API_SERVER_IP _T("m.chat.lotteshopping.com")
//#define FILE_SERVER_IP _T("m.chat.lotteshopping.com")
//#define API_SERVER_PORT 80

#define KOINO_LOGIN	1

#define REQUEST_HTTPS	0
//#define LOGIN_SERVER _T("m.togetherdev.dpt.co.kr")
#define LOGIN_SERVER _T("m.together.lotteshopping.com")

//#define SERVER_IP_ADDRESS	_T("14.63.213.180")
//#define SERVER_IP_ADDRESS	_T("192.168.0.125")
//#define PORT_S	39432
#define PORT_S	8082
#define FILE_SERVER_PORT 8080

#define _TMI_IDX "TMI_IDX"
#define _TMI_ID "TMI_ID"
#define _TMI_PWD "TMI_PWD"
#define _TMI_NAME "TMI_NAME"
#define _TMI_EMAIL "TMI_EMAIL"
#define _TMI_PHON_NUM "TMI_PHON_NUM"
#define _TMI_PROF_IMAGE_PATH "TMI_PROF_IMAGE_PATH"
#define _TMI_PUSH_KEY "TMI_PUSH_KEY"
#define _TCRM_LAST_LOGIN_DATE "TCRM_LAST_LOGIN_DATE"
#define _SEARCH_STRING "SEARCH_STRING"
#define _FROM_TMI_IDX "FROM_TMI_IDX"
#define _TO_TMI_IDX "TO_TMI_IDX"
#define _TO_TMI_IDXS "TO_TMI_IDXS"
#define _TCM_CHAT_ROOM_UID "TCM_CHAT_ROOM_UID"
#define _TCM_CHAT_ROOM_TITLE "TCM_CHAT_ROOM_TITLE"
#define _TCM_TMI_PROF_IMAGE_PATH "TCM_TMI_PROF_IMAGE_PATH"
#define _TCM_CHAT_ROOM_COUNT "TCM_CHAT_ROOM_COUNT"
#define _TCM_CHAT_ROOM_BADGE "TCM_CHAT_ROOM_BADGE"
#define _TCM_CHAT_ROOM_HIDDEN "TCM_CHAT_ROOM_HIDDEN"
#define _TCM_CHAT_ROOM_ALRAM "TCM_CHAT_ROOM_ALRAM"
#define _TCM_TMI_NAME "TCM_TMI_NAME"
#define _BADGE_COUNT "BADGE_COUNT"
#define _TPC_CODE "TPC_CODE"
#define _TPC_NAME "TPC_NAME"
#define _CSTR_NM "CSTR_NM"
#define _DPT_BRAND_NM "DPT_BRAND_NM"
#define _SUPPORT_START_TIME "SUPPORT_START_TIME"
#define _SUPPORT_END_TIME "SUPPORT_END_TIME"
#define _DPT_BRAND_CI_LINK "DPT_BRAND_CI_LINK"
#define _USPR_TEL_NO "USPR_TEL_NO"
#define _TCM_MSG "TCM_MSG"
#define _TCM_MSG_TYPE "TCM_MSG_TYPE"
#define _TCM_CHAT_TIME "TCM_CHAT_TIME"
#define _TCM_MSG_UID "TCM_MSG_UID"
#define _TCM_MSG_UIDS "TCM_MSG_UIDS"
#define _TMI_DEVICE_TYPE "TMI_DEVICE_TYPE"
#define _IMAGE_DOWN_TYPE "IMAGE_DOWN_TYPE"
#define _SAVED_PATH "SAVED_PATH"
//#define _SHOP_DOMAIN _T("m.omnishop.lotteshopping.com")
#define _SHOP_DOMAIN _T("m.omnishop")
//#define _SHOP_DOMAIN _T("www.lotte.com")
#define _TCM_CHAT_ROOM_RESERVED_1 "TCM_CHAT_ROOM_RESERVED_1"
#define _TCM_CHAT_ROOM_RESERVED_2 "TCM_CHAT_ROOM_RESERVED_2"
#define _TCM_CHAT_ROOM_RESERVED_3 "TCM_CHAT_ROOM_RESERVED_3"
#define _TCM_CHAT_ROOM_RESERVED_4 "TCM_CHAT_ROOM_RESERVED_4"
#define _TCM_CHAT_ROOM_RESERVED_5 "TCM_CHAT_ROOM_RESERVED_5"
#define MAX_TEXT_WIDTH 295
#define MAX_TEXT_HEIGHT 46

class CKBChatHistItem
{
public :



	CKBChatHistItem()
	{
		
	}
	CString TMI_IDX;
	CString TMI_NAME;
	CString TMI_PROF_IMAGE_PATH;
	CString TCM_CHAT_ROOM_UID;
	CString TCM_CHAT_ROOM_TITLE;
	CString TCM_MSG_UID;
	CString TCM_MSG;
	CString TCM_MSG_TYPE;
	CString TCM_CHAT_TIME;
	CString TCM_CHAT_ROOM_COUNT;
	CString TCM_TMI_NAME;
	CString TCM_TMI_PROF_IMAGE_PATH;
	CString TCM_CHAT_ROOM_BADGE;
	CString TCM_CHAT_ROOM_HIDDEN;
	CString TCM_CHAT_ROOM_ALRAM;
	CKBChatHistItem *Clone()
	{
		CKBChatHistItem *pNew = new CKBChatHistItem();
		pNew->TMI_IDX = TMI_IDX;
		pNew->TMI_NAME = TMI_NAME;
		pNew->TMI_PROF_IMAGE_PATH = TMI_PROF_IMAGE_PATH;
		pNew->TCM_CHAT_ROOM_UID = TCM_CHAT_ROOM_UID;
		pNew->TCM_CHAT_ROOM_TITLE = TCM_CHAT_ROOM_TITLE;
		pNew->TCM_MSG_UID = TCM_MSG_UID;
		pNew->TCM_MSG = TCM_MSG;
		pNew->TCM_MSG_TYPE = TCM_MSG_TYPE;
		pNew->TCM_CHAT_TIME = TCM_CHAT_TIME;
		pNew->TCM_CHAT_ROOM_COUNT = TCM_CHAT_ROOM_COUNT;
		pNew->TCM_TMI_NAME = TCM_TMI_NAME;
		pNew->TCM_TMI_PROF_IMAGE_PATH = TCM_TMI_PROF_IMAGE_PATH;
		pNew->TCM_CHAT_ROOM_BADGE = TCM_CHAT_ROOM_BADGE;
		pNew->TCM_CHAT_ROOM_HIDDEN = TCM_CHAT_ROOM_HIDDEN;
		pNew->TCM_CHAT_ROOM_ALRAM = TCM_CHAT_ROOM_ALRAM;
		return pNew;
	}
};

class CKBChatItem
{
public :
	CKBChatItem()
	{
		m_pImage = NULL;
		bFirstMsg = FALSE;
		bSelected = FALSE;
		m_rect = CRect(0,0,0,0);
		m_ContentsRect = CRect(0,0,0,0);
		mDrawHeight = 0;
	}

    CString TMI_IDX;
    CString TMI_NAME;
    CString TMI_PROF_IMAGE_PATH;
    CString TCM_MSG;
    CString TCM_MSG_TYPE;
    CString TCM_CHAT_ROOM_UID;
    CString TCM_CHAT_TIME;
    CString TCM_MSG_UID;
    CString UNREAD_COUNT;
	CString TCM_CHAT_ROOM_RESERVED_1;
	CString TCM_CHAT_ROOM_RESERVED_2;
	CxImage mProfileImg;
	CxImage *m_pImage;
	BOOL bFirstMsg;
	BOOL bSelected;
	CRect m_rect;
	CRect m_ContentsRect;
	CStringList mUnreadList;
	CString TSP_URL;
	int mDrawHeight;
	CKBChatItem *Clone()
	{
		CKBChatItem *pNew = new CKBChatItem();
		pNew->TMI_IDX.Format(_T("%s"),TMI_IDX);
		pNew->TMI_NAME.Format(_T("%s"),TMI_NAME);
		pNew->TMI_PROF_IMAGE_PATH.Format(_T("%s"),TMI_PROF_IMAGE_PATH);
		pNew->TCM_MSG.Format(_T("%s"),TCM_MSG);
		pNew->TCM_MSG_TYPE.Format(_T("%s"),TCM_MSG_TYPE);
		pNew->TCM_CHAT_ROOM_UID.Format(_T("%s"),TCM_CHAT_ROOM_UID);
		pNew->TCM_CHAT_TIME.Format(_T("%s"),TCM_CHAT_TIME);
		pNew->TCM_MSG_UID.Format(_T("%s"),TCM_MSG_UID);
		pNew->UNREAD_COUNT.Format(_T("%s"),UNREAD_COUNT);
		pNew->bFirstMsg = bFirstMsg;
		pNew->TSP_URL.Format(_T("%s"),TSP_URL);
		pNew->TCM_CHAT_ROOM_RESERVED_1 = TCM_CHAT_ROOM_RESERVED_1;
		pNew->TCM_CHAT_ROOM_RESERVED_2 = TCM_CHAT_ROOM_RESERVED_2;
		return pNew;		
	}

	void AddMapData(APICALLDATA *pData)
	{
		pData->insert(APICALLDATA::value_type(_TMI_IDX, TMI_IDX));
		pData->insert(APICALLDATA::value_type(_TMI_NAME, AsciiToUTF8(TMI_NAME)));
		pData->insert(APICALLDATA::value_type(_TMI_PROF_IMAGE_PATH, TMI_PROF_IMAGE_PATH));
		pData->insert(APICALLDATA::value_type(_TCM_MSG, AsciiToUTF8(TCM_MSG)));
		pData->insert(APICALLDATA::value_type(_TCM_MSG_TYPE, TCM_MSG_TYPE));
		pData->insert(APICALLDATA::value_type(_TCM_CHAT_ROOM_UID, TCM_CHAT_ROOM_UID));
		pData->insert(APICALLDATA::value_type(_TCM_CHAT_TIME, TCM_CHAT_TIME));
		pData->insert(APICALLDATA::value_type(_TCM_MSG_UID, TCM_MSG_UID));
		pData->insert(APICALLDATA::value_type(_TCM_CHAT_ROOM_RESERVED_1, AsciiToUTF8(TCM_CHAT_ROOM_RESERVED_1)));
		pData->insert(APICALLDATA::value_type(_TCM_CHAT_ROOM_RESERVED_2, TCM_CHAT_ROOM_RESERVED_2));
	}
};

class CKBHistDialogItem
{
	CDialog *pDialog;
};

enum KBIZTALK_MSG
{
	WM_KBIZTALK_MSG_DCLICK = (WM_USER+2023),
	WM_KBIZTALK_MSG_API_CALL = (WM_USER+2024)
};

enum KBIZTALK_HIST_CMD {HIST_CMD_GOTOROOM=0};

enum CHAT_TYPE{CHAT_TYPE_TEXT=1,CHAT_TYPE_IMAGE=2,CHAT_TYPE_IMAGE_URL=6,CHAT_TYPE_MOVIE=3,CHAT_TYPE_MOVIE_URL=7,CHAT_TYPE_ICON=4,CHAT_TYPE_DATE=5,CHAT_TYPE_SHOP_URL=8};

enum MAIN_TAB_TYPE{MAIN_TAB_HIST=0,MAIN_TAB_HELPER,MAIN_TAB_USERS,MAIN_TAB_REQUEST,MAIN_TAB_CONFIG,MAIN_TAB_CHAT,MAIN_TAB_CHAT_INVITE,MAIN_TAB_CHAT_MEMBERS,MAIN_TAB_CHAT_SHOP};
enum IMAGE_DOWN_TYPE{IMAGE_DOWN_PROFILE=1,IMAGE_DOWN_CHAT=2,IMAGE_DOWN_MYPROFILE=3,IMAGE_DOWN_SHOP=4,IMAGE_DOWN_SHOP_LOCAL=5};

#define KEY_CMD  _T("CMD")
enum API_CMD{API_LOGIN=1,API_GETHELPERLIST=2,API_DOWNLOAD_FILE=3,API_GETRECOMMAND_MEMBER_LIST=4,API_GETFRIENDREQUEST_MEMBER_LIST=5
	,API_REQUEST_FRIEND=6,API_REQUEST_FRIEND_DELETE=7,API_REQUEST_FRIEND_ACCEPT=8,API_REQUEST_FRIEND_REJECT=9,API_ADD_CHAT_ITEM=10,API_FILE_UPLOAD=11
	,API_CHATITEM_UNREAD_LIST=12, API_UPDATE_ROOM_JOIN=13,API_GETROOM_MEMBER_LIST=14,API_UPDATE_UNREAD_CHATITEM_LIST=15,API_GETFRIENDLIST_FOR_CHAT=16
	,API_INVITE_MEMBERS=17,API_ROOM_OUT=18,API_PROFILE_FILE_UPLOAD=19,API_UPDATE_PROFILE_NAME=20,API_GET_CHAT_ROOMLISTOFME=21,API_RECOVERY_CHATITEM=22
	,API_REQUEST_SHOP_PRODUCT_LIST=23,API_UPDATE_PROFILE_PWD=24,API_GET_ALL_SHOPMASTER_LIST=25,API_SHOP_URL=26,API_DOWNLOAD_FILE_SHOP=27,API_LOGIN_LOTTE=28
	,API_GET_INVITE_LIST=29
};

#define RETURN_SUCESS _T("0")
#define RETURN_REG_NOT_ACCOUNT _T("1002")
#define RETURN_REG_INVALIDE_PASSWD _T("1004")
#define RETURN_REG_INVALIDE_DEVICE _T("1003")
#define RETURN_REG_EXIST_ACCOUNT _T("1002")
#define RETURN_ALREADY_FRIEND_REQUEST_EXIST _T("1006")
#define RETURN_ALREADY_FRIEND_EXIST _T("1007")
#define RETURN_NOT_FRIEND_RELATION _T("1008")
/*
#define API_SERVER_IP _T("14.63.213.180")
#define API_SERVER_PORT 80
#define FILE_SERVER_IP _T("14.63.213.180")
#define FILE_SERVER_PORT 8080
*/


class CKTBMember
{
public :
	CKTBMember()
	{
		pProfileImage = NULL;
		m_colorIndex = 0;
		DPT_BRAND_NM = _T("");
	}
	~CKTBMember()
	{
		if(pProfileImage)
		{
			pProfileImage->Clear();
			delete pProfileImage;
		}
	}
	int TMI_IDX;
    CString TMI_ID;
    CString TMI_PWD ;
    CString TMI_NAME;
    int TMI_COM_IDX;
    int TMI_PART_IDX;
    CString TMI_EMAIL;
    CString TMI_NAT_CODE;
    CString TMI_PHON_NUM;
    CString TMI_BIR_DAY;
    CString TMI_SEX;
    CString TMI_END_LOGIN_DATE ;
    CString TMI_PROF_IMAGE_PATH;
    CString TMI_PROF_BG_IMAGE_PATH;
    CString TMI_ONLINE_STATUS;
    CString TMI_DEVICE_TYPE ;
    CString TMI_REG_TYPE;
    CString TMI_IPADDR;
    CString TMI_PUSH_KEY;
    CString TCRM_LAST_LOGIN_DATE;
	CString TPC_CODE;
	CString TPC_NAME;
	CString DPT_BRAND_NM;
	CString SUPPORT_START_TIME;
	CString SUPPORT_END_TIME;
	CString DPT_BRAND_CI_LINK;
	CString USPR_TEL_NO;
	CString CSTR_NM;
	CxImage *pProfileImage;
	int m_colorIndex;
	CKTBMember *Clone()
	{
		CKTBMember *pNew = new CKTBMember();
		pNew->TMI_IDX = TMI_IDX;
		pNew->TMI_ID = TMI_ID;
		pNew->TMI_PWD = TMI_PWD;
		pNew->TMI_NAME = TMI_NAME;
		pNew->TMI_COM_IDX = TMI_COM_IDX;
		pNew->TMI_PART_IDX = TMI_PART_IDX;
		pNew->TMI_EMAIL = TMI_EMAIL;
		pNew->TMI_NAT_CODE = TMI_NAT_CODE;
		pNew->TMI_PHON_NUM = TMI_PHON_NUM;
		pNew->TMI_BIR_DAY = TMI_BIR_DAY;
		pNew->TMI_SEX = TMI_SEX;
		pNew->TMI_END_LOGIN_DATE = TMI_END_LOGIN_DATE;
		pNew->TMI_PROF_IMAGE_PATH = TMI_PROF_IMAGE_PATH;
		pNew->TMI_PROF_BG_IMAGE_PATH = TMI_PROF_BG_IMAGE_PATH;
		pNew->TMI_ONLINE_STATUS = TMI_ONLINE_STATUS;
		pNew->TMI_DEVICE_TYPE = TMI_DEVICE_TYPE;
		pNew->TMI_REG_TYPE = TMI_REG_TYPE;
		pNew->TMI_IPADDR = TMI_IPADDR;
		pNew->TMI_PUSH_KEY = TMI_PUSH_KEY;
		pNew->TCRM_LAST_LOGIN_DATE = TCRM_LAST_LOGIN_DATE;
		pNew->TPC_CODE=TPC_CODE;
		pNew->TPC_NAME=TPC_NAME;
		pNew->CSTR_NM=CSTR_NM;
		pNew->DPT_BRAND_NM=DPT_BRAND_NM;
		pNew->SUPPORT_START_TIME=SUPPORT_START_TIME;
		pNew->SUPPORT_END_TIME=SUPPORT_END_TIME;
		pNew->DPT_BRAND_CI_LINK=DPT_BRAND_CI_LINK;
		pNew->USPR_TEL_NO=USPR_TEL_NO;
		if(pProfileImage)
		{
			pNew->pProfileImage = new CxImage();
			pNew->pProfileImage->Copy(*pProfileImage);
		}
		return pNew;
	}
	CString GetTMI_IDX()
	{
		CString sTMI_IDX;
		sTMI_IDX.Format(_T("%d"),TMI_IDX);
		return sTMI_IDX;
	}

};

class CKTBShopProduct
{
public :
	CKTBShopProduct()
	{
		pProfileImage = NULL;
	}
	~CKTBShopProduct()
	{
		if(pProfileImage)
		{
			pProfileImage->Clear();
			delete pProfileImage;
		}
	}
	CString TSP_IDX;
    CString TSP_LINK;
	CString TSP_LINK_URL;
    CString TSP_NAME ;
    CString TSP_PRICE;
    CString TSP_DISC;
    CString TSP_URL;
	CxImage *pProfileImage;
	CKTBShopProduct *Clone()
	{
		CKTBShopProduct *pNew = new CKTBShopProduct();
		pNew->TSP_IDX = TSP_IDX;
		pNew->TSP_LINK = TSP_LINK;
		pNew->TSP_NAME = TSP_NAME;
		pNew->TSP_PRICE = TSP_PRICE;
		pNew->TSP_DISC = TSP_DISC;
		pNew->TSP_URL = TSP_URL;
		pNew->TSP_LINK_URL = TSP_LINK_URL;
		
		if(pProfileImage)
		{
			pNew->pProfileImage = new CxImage();
			pNew->pProfileImage->Copy(*pProfileImage);
		}
		return pNew;
	}
};

class CDomainParser
{
public :
	CString sDomain;
	CString sPort;
	CString sParam;
	void Parse(CString sWasUrl)
	{
		USES_CONVERSION;
		CString sURL;
		sURL.Format(_T("%s"),sWasUrl);
		sURL.MakeLower();
		BOOL bWASHTTPS = FALSE;
		int nHttpFind = sURL.Find(_T("http://"));
		int nHttpsFind = sURL.Find(_T("https://"));
		if(nHttpFind != -1)
			bWASHTTPS  = FALSE;
		if(nHttpsFind != -1)
			bWASHTTPS  = TRUE;
		int curLen = 0;
		if(nHttpFind == -1 && nHttpsFind == -1)
			curLen = 0;
		else if(nHttpFind != -1 && nHttpsFind == -1)
			curLen = 7;
		else if(nHttpFind == -1 && nHttpsFind != -1)
			curLen = 8;
		sURL = sURL.Mid(curLen,sURL.GetLength()-7);
		int nFind = sURL.Find(_T(":"));
		if(nFind != -1)
		{
			sDomain.Format(_T("%s"),sURL.Left(nFind));
		}
		else
		{
			nFind = sURL.Find(_T("/"));
			if(nFind != -1)
			{
				sDomain.Format(_T("%s"),sURL.Left(nFind));
			}
		}

		int nFind2 = sURL.Find(_T("/"),nFind);
			
		if(abs(nFind2-nFind)> 2)
		{
			sPort = sURL.Mid(nFind+1,nFind2-nFind-1);
		}
		else
		{
			if(bWASHTTPS)
				sPort = _T("463");
			else
				sPort = _T("80");
		}

		sParam = sURL.Mid(nFind2,sURL.GetLength() -1 );
	}
};

#define TAB_COUNT 4

enum SHOWMESSAGETYPE{SHOWMESSAGE_MODAL=0,SHOWMESSAGE_MODALESS};

#define SQLITE_TB_CONFIG _T("kbiztalk_config")
#define SQLITE_TB_CHAT_ROOM _T("kbtbchatroom")
#define SQLITE_TB_CHAT_ROOM_UNREAD _T("kbtbunreadchat")
#define SQLITE_TB_CHAT_ITEM _T("kbtbchatitem")
#define SQLITE_TB_CHAT_ITEM_UNREAD _T("kbtbchatitemunread")

#endif
