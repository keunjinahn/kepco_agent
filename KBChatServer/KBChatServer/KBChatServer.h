
// KBChatServer.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CKBChatServerApp:
// �� Ŭ������ ������ ���ؼ��� KBChatServer.cpp�� �����Ͻʽÿ�.
//

class CKBChatServerApp : public CWinApp
{
public:
	CKBChatServerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bATLInited;
private:
	BOOL InitATL();
};

extern CKBChatServerApp theApp;