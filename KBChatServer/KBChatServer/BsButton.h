#pragma once

// BsButton.h : header file
enum MYCOMMAND
{
	WM_MYCOMMAND = (WM_USER+1023)
	// �߰����� ����� �޽����� �־ ����ϼŵ� �˴ϴ�. ( ex> WM_MOVIE_UPDATE )
};

class CBsButton : public CWnd
{
public:
	CBsButton(int nType = 0);
	virtual ~CBsButton();

	// button type
	enum
	{
		PUSH_BUTTON,
		RADIO_BUTTON,
		TOGGLE_BUTTON
	};

	enum BUTTON_STATES
	{
		NORMAL,
		OVERMOUSE,
		PUSHED,		
		DISABLED
	};
protected:
	BOOL m_bRun;
	BOOL m_bHover;
	BOOL m_bDisablePaint;
	int m_nButtonType;

	CWnd* m_pParent;

	int m_w, m_h; // ��ư w, h	

	// �̹��� ��Ų. (4���� ����) 0:NORMAL, 1:PUSHED, 2:OVERMOUSE, 3:DISABLED
	CxImage m_imgSkin[4];
	int	m_nButtonCount;
	CSize m_size; // ��ư�� ����, ����
	//CRect m_rtRegion;  // ���� ��������� ��ǥ �簢��.	
	BUTTON_STATES m_state; // ���� ��ư�� ����
	BUTTON_STATES m_statePrev; // ���� ��ư ����
	BOOL m_bDisabled;
	BOOL m_bPushed; // ��ư�� ������
	BOOL m_bCheck; // üũ��ư ��Ÿ���� ��� üũ ����
	int m_nID; // ��Ʈ�� ID

	// �����Լ�
protected:
	void ReceiveMsgPushStyle(MSG* pMsg, CWnd* pParent);
	void ReceiveMsgCheckStyle(MSG* pMsg, CWnd* pParent);
	virtual void RunThisButton();
	void SetRegionFromBitmap(HBITMAP hBitmap);		

	// �����Լ�
public:
	BOOL IsPushed() { return m_bPushed; }
	void DisablePaint(BOOL b) { m_bDisablePaint = b; }
	// rect: ��ư����, pParentWnd: �θ�������, nID: ��ưID, xImage: ��Ų�̹���, nButtonCount: ��ư����
	BOOL Create(const CRect& rect, CWnd* pParentWnd, UINT nID, CxImage& xImage, int nButtonCount, 
		int nButtonType = PUSH_BUTTON);
	BOOL Create(const CPoint& pt, CWnd* pParentWnd, UINT nID, CxImage& xImage, int nButtonCount,
		int nButtonType = PUSH_BUTTON);
	void ReceiveMsg(MSG* pMsg, CWnd* pParent);
	void SetDisable(BOOL bDisable);
	//BOOL ShowWindow(int nCmdShow);

	BOOL SetWindowPos(CPoint ptNew); // ��ư�� ��ġ�� �ٲٴ� ���
	BOOL SetWindowPos(int x, int y);

	void SetCheck(BOOL bCheck);  // üũ ��Ÿ�� ��ư�� ��� ���
	BOOL IsCheck() { return m_bCheck; }

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();


public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void SetOver(BOOL bOver);
	BOOL m_bOver;
	void SetOverMsg(BOOL bOverMsg);
	BOOL m_bOverMsg;
};
