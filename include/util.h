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
	sTime.Format(_T("%s %d:%02d"),(t1->tm_hour < 13)? _T("����"):_T("����"),nTime,t1->tm_min);
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
	//sTime.Format(_T("%d�� %d�� %d�� %02d:%02d:%02d"),1900+t1->tm_year,t1->tm_mon+1,t1->tm_mday,t1->tm_hour,t1->tm_min,t1->tm_sec);
	sTime.Format(_T("%d�� %d�� %d��"),1900+t1->tm_year,t1->tm_mon+1,t1->tm_mday);
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
 
     // �ƽ�Ű �ڵ带 UTF8������ �ڵ�� ��ȯ�ؾ� �Ѵ�. �ƽ�Ű �ڵ带 UTF8 �ڵ�� ��ȯ�Ҷ��� 
     // �ƽ�Ű �ڵ带 �����ڵ�� ���� ��ȯ�ϰ� ��ȯ�� �����ڵ带 UTF8 �ڵ�� ��ȯ�ؾ� �Ѵ�.
 
     // �ƽ�Ű �ڵ�ε� ���ڿ��� �����ڵ�ȭ ������ ���� ���̸� ���Ѵ�.
      int temp_length = MultiByteToWideChar(CP_ACP, 0, W2A(parm_ascii_string), -1, NULL, 0);


     // ��ȯ�� �����ڵ带 ������ ������ �Ҵ��Ѵ�.
     BSTR unicode_str = SysAllocStringLen(NULL, temp_length + 1);
 
     // �ƽ�Ű �ڵ�ε� ���ڿ��� ���� �ڵ� ������ ���ڿ��� �����Ѵ�.
      MultiByteToWideChar(CP_ACP, 0, W2A(parm_ascii_string), -1, unicode_str, temp_length);
 
     // �����ڵ� ������ ���ڿ��� UTF8 �������� ���������� �ʿ��� �޸� ������ ũ�⸦ ��´�.
     temp_length = WideCharToMultiByte( CP_UTF8, 0, unicode_str, -1, NULL, 0, NULL, NULL );
 
     if(temp_length > 0){
        CString str;


        // UTF8 �ڵ带 ������ �޸� ������ �Ҵ��Ѵ�.
        char *p_utf8_string = new char[temp_length];
        memset(p_utf8_string, 0, temp_length);
        // �����ڵ带 UTF8�ڵ�� ��ȯ�Ѵ�.
        WideCharToMultiByte(CP_UTF8, 0, unicode_str, -1, p_utf8_string, temp_length, NULL, NULL);
  
        // UTF8 �������� ����� ���ڿ��� �� ������ �ڵ尪���� �� URL�� ���Ǵ� �������� ��ȯ�Ѵ�.
        for(int i = 0; i < temp_length - 1; i++){
            if(p_utf8_string[i] & 0x80){
                // ���� �ڵ尡 �ѱ��� ���..
                // UTF8 �ڵ�� ǥ���� �ѱ��� 3����Ʈ�� ǥ�õȴ�. "�ѱ�"  ->  %ED%95%9C%EA%B8%80
                for(int sub_i = 0; sub_i < 3; sub_i++){
                    str.Format(_T("%%%X"), p_utf8_string[i] & 0x00FF);
                    parm_utf8_string += str;
                    i++;
                } 
    
                i--;
            } else {
                // ���� �ڵ尡 ������ ���, ������� �״�� ����Ѵ�.
               parm_utf8_string += p_utf8_string[i];
            }
        }                                                              
  
        delete[] p_utf8_string;
     }
 
      // �����ڵ� ������ ���ڿ��� �����ϱ� ���� �����ߴ� �޸𸮸� �����Ѵ�.
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
