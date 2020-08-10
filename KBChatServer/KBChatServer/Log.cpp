// Log.cpp: implementation of the Log class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Log.h"
#include "direct.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRITICAL_SECTION cs;
Log::Log()
{
	InitializeCriticalSection(&cs);
	fp = NULL;
}

Log::~Log()
{
	if(fp != NULL) fclose(fp);
	DeleteCriticalSection(&cs);
}

void Log::Write(char *message, ... )
{
	USES_CONVERSION;
	char msgbuf[4096];
	va_list val;

	EnterCriticalSection(&cs);
	SYSTEMTIME current_time;
	GetLocalTime(&current_time);
	
	va_start(val, message);
	_vsnprintf(msgbuf, 4096,message, val);
	va_end(val);

	fprintf(fp, "[%.4ld/%.2ld/%.2ld %.2ld:%.2ld:%.2ld] %s\n",current_time.wYear, current_time.wMonth, current_time.wDay,
										current_time.wHour, current_time.wMinute, current_time.wSecond,
										msgbuf);
	fflush(fp);
	linenum++;
	if(linenum > 10000) OpenLogFile(m_lpszFileName);
	LeaveCriticalSection(&cs);
}

void Log::Write(CString message, ... )
{
	USES_CONVERSION;
	EnterCriticalSection(&cs);
	SYSTEMTIME current_time;
	GetLocalTime(&current_time);
	va_list val;
	va_list args;
    va_start(args, message);
	CString str;
	str.Format(_T("[%.4ld/%.2ld/%.2ld %.2ld:%.2ld:%.2ld]"),current_time.wYear, current_time.wMonth, current_time.wDay,
										current_time.wHour, current_time.wMinute, current_time.wSecond);
	CString strFormat;
    strFormat.FormatV(message, args);
	_fwprintf_p(fp, _T("%s-%s\n"),str,strFormat);
	fflush(fp);
	linenum++;
	if(linenum > 10000) OpenLogFile(m_lpszFileName);
	LeaveCriticalSection(&cs);
}

void Log::WriteString(CString strMsg)
{
	USES_CONVERSION;
	EnterCriticalSection(&cs);
	
	_fwprintf_p(fp, L"[%s]\n",strMsg);
	fflush(fp);
	linenum++;
	if(linenum > 10000) OpenLogFile(m_lpszFileName);
	LeaveCriticalSection(&cs);

}

void Log::OpenLogFile(CString lpszFileName)
{
	USES_CONVERSION;
	if(fp != NULL) fclose(fp);
	m_lpszFileName = lpszFileName;
	SYSTEMTIME current_time;
	GetLocalTime(&current_time);
	fp = _wfopen(m_lpszFileName,_T("a"));
	linenum = 0;
}


