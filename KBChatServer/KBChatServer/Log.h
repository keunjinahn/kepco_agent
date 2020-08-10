// Log.h: interface for the Log class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__6C0D0CEF_4712_4D01_8A4C_762228D8CE18__INCLUDED_)
#define AFX_LOG_H__6C0D0CEF_4712_4D01_8A4C_762228D8CE18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>

class Log  
{
public:
	Log();
	virtual ~Log();
	void OpenLogFile(CString lpszFileName);
	void Write(char* message, ...);
	void Write(CString message, ...);
	void WriteString(CString strMsg);

private:
	CString m_lpszFileName;
	int filenum;
	int linenum;
	FILE* fp;
};

#endif // !defined(AFX_LOG_H__6C0D0CEF_4712_4D01_8A4C_762228D8CE18__INCLUDED_)
