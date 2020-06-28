#pragma once

// BsButton.h : header file
enum MYCOMMAND
{
	WM_MYCOMMAND = (WM_USER+1023)
	// 추가적인 사용자 메시지를 넣어서 사용하셔도 됩니다. ( ex> WM_MOVIE_UPDATE )
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

	int m_w, m_h; // 버튼 w, h	

	// 이미지 스킨. (4가지 상태) 0:NORMAL, 1:PUSHED, 2:OVERMOUSE, 3:DISABLED
	CxImage m_imgSkin[4];
	int	m_nButtonCount;
	CSize m_size; // 버튼의 가로, 세로
	//CRect m_rtRegion;  // 메인 윈도우상의 좌표 사각형.	
	BUTTON_STATES m_state; // 현재 버튼의 상태
	BUTTON_STATES m_statePrev; // 이전 버튼 상태
	BOOL m_bDisabled;
	BOOL m_bPushed; // 버튼을 눌렀다
	BOOL m_bCheck; // 체크버튼 스타일의 경우 체크 여부
	int m_nID; // 컨트롤 ID

	// 내부함수
protected:
	void ReceiveMsgPushStyle(MSG* pMsg, CWnd* pParent);
	void ReceiveMsgCheckStyle(MSG* pMsg, CWnd* pParent);
	virtual void RunThisButton();
	void SetRegionFromBitmap(HBITMAP hBitmap);		

	// 공개함수
public:
	BOOL IsPushed() { return m_bPushed; }
	void DisablePaint(BOOL b) { m_bDisablePaint = b; }
	// rect: 버튼영역, pParentWnd: 부모윈도우, nID: 버튼ID, xImage: 스킨이미지, nButtonCount: 버튼갯수
	BOOL Create(const CRect& rect, CWnd* pParentWnd, UINT nID, CxImage& xImage, int nButtonCount, 
		int nButtonType = PUSH_BUTTON);
	BOOL Create(const CPoint& pt, CWnd* pParentWnd, UINT nID, CxImage& xImage, int nButtonCount,
		int nButtonType = PUSH_BUTTON);
	void ReceiveMsg(MSG* pMsg, CWnd* pParent);
	void SetDisable(BOOL bDisable);
	//BOOL ShowWindow(int nCmdShow);

	BOOL SetWindowPos(CPoint ptNew); // 버튼의 위치를 바꾸는 경우
	BOOL SetWindowPos(int x, int y);

	void SetCheck(BOOL bCheck);  // 체크 스타일 버튼인 경우 사용
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
