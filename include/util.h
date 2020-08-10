#ifndef __UTIL_H
#define __UTIL_H
#include <hash_set> 
static LONGLONG KBGetCurrentTime()
{
	LONGLONG sysTimeMS = CTime::GetCurrentTime().GetTime() * 1000;
	return sysTimeMS;
}

static CString KBGetTimeStr(CString sTimeSrc)
{
	CString sTime;
	LONGLONG llTime = _wtoi64(sTimeSrc);
	time_t t = llTime/1000;
	tm *t1 = localtime (&t);
	int nTime = (t1->tm_hour < 13)? t1->tm_hour:t1->tm_hour-12;
	sTime.Format(_T("%s %d:%02d"),(t1->tm_hour < 13)? _T("오전"):_T("오후"),nTime,t1->tm_min);
	return sTime;
}

static LONGLONG KBGetZeroDayLongTime(CString sTimeSrc)
{
	CString sTime;
	LONGLONG llTime = _wtoi64(sTimeSrc);
	time_t t = llTime/1000;
	tm *t1 = localtime (&t);
	t1->tm_hour = 0;
	t1->tm_min = 0;
	t1->tm_sec = 0;
	time_t tm2_ = mktime(t1);
	return tm2_ * 1000;
}


static CString KBGetZeroDayTime()
{
	CString sTime;
	time_t tm_ = CTime::GetCurrentTime().GetTime();
	tm * t1  = localtime (&tm_);
	t1->tm_hour = 0;
	t1->tm_min = 0;
	t1->tm_sec = 0;
	time_t tm2_ = mktime(t1);
	sTime.Format(_T("%I64d"),tm2_ * 1000);
	return sTime;
}

static CString KBGetTimeDayStr(CString sTimeSrc)
{
	CString sTime;
	LONGLONG llTime = _wtoi64(sTimeSrc);
	time_t t = llTime/1000;
	tm *t1 = localtime (&t);
	//sTime.Format(_T("%d년 %d월 %d일 %02d:%02d:%02d"),1900+t1->tm_year,t1->tm_mon+1,t1->tm_mday,t1->tm_hour,t1->tm_min,t1->tm_sec);
	sTime.Format(_T("%d년 %d월 %d일"),1900+t1->tm_year,t1->tm_mon+1,t1->tm_mday);
	return sTime;

}
static int KBGetHash(CString idxs)
{
	int hash = 0, strlen = idxs.GetLength();
	char a;
	if(strlen == 0)
		return hash;
	for(int i=0;i<strlen;i++)
	{
		a = idxs.GetAt(i);
		hash = (31*hash) + a;
	}
	return hash;
}

static CString generatorRoomNo(CString idxs)
{
	CString hashCode;
	hashCode.Format(_T("%d"),KBGetHash(idxs));
	return hashCode;
}


static CString AsciiToUTF8(CString parm_ascii_string) 
{
	USES_CONVERSION;
	CString parm_utf8_string;
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
	  return parm_utf8_string;
}

static CString GetFileNameFromPath(CString strFile)
{
	USES_CONVERSION;
	CString strFileName;
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];

	TCHAR strModulePath[_MAX_PATH];
	ZeroMemory(strModulePath,_MAX_PATH);
	_wsplitpath(strFile,drive,dir,fname,ext);
	strFileName.Format(_T("%s%s"),fname,ext);

	return strFileName;
}




static BOOL IsFindFile(CString sFile)
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile(sFile);
	return bWorking;
}


static CString GetThumbNameFromImageURL(CString sImageURL)
{
	int nLen = sImageURL.ReverseFind('/');
	CString sThumbPath;
	sThumbPath.Format(_T("thumb_%s"),sImageURL.Mid(nLen+1,sImageURL.GetLength()-nLen));
	return sThumbPath;
}

static CString GetFileNameFromImageURL(CString sImageURL)
{
	int nLen = sImageURL.ReverseFind('/');
	CString sFileName;
	sFileName.Format(_T("%s"),sImageURL.Mid(nLen+1,sImageURL.GetLength()-nLen));
	return sFileName;
}

static CString GetThumbURLFromImageURL(CString sImageURL)
{
	int nLen = sImageURL.ReverseFind('/');
	CString sThumbPath;
	sThumbPath.Format(_T("/%s/thumb_%s"),sImageURL.Left(nLen),sImageURL.Mid(nLen+1,sImageURL.GetLength()-nLen));
	return sThumbPath;
}

static int GetCountChar(CString Ps_Str, TCHAR Pc_Char) 
{ 
    int Li_Count = 0; 
    for(int i = 0; i < Ps_Str.GetLength(); i++)
    {
        if(Ps_Str[i] == Pc_Char) Li_Count++; 
    }
    return Li_Count; 
} 

static CStringList * Split(CString Ps_Str, TCHAR Pc_Separator, int& Ri_Length)
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

static char *StringToUTF8(CString str)
{
	char strUtf8[256] ={0,};
	int nLen = WideCharToMultiByte(CP_UTF8, 0, str, lstrlenW(str), NULL, 0, NULL, NULL);
	WideCharToMultiByte (CP_UTF8, 0, str, lstrlenW(str), strUtf8, nLen, NULL, NULL);
	return strUtf8;
}

#endif
