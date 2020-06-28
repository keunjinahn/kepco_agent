// BsButton.cpp : implementation file
//

#include "stdafx.h"
#include "KBChatServer.h"
#include "BsButton.h"
#include "XGDIHelper.h"
#include "MemDC.h"
#include "ximage.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define TRANSPARENT_COLOR (RGB(255,0,255)) // 투명값 설정
#define SAFE_DELETE(p) { if(p) delete(p); (p)=NULL; } // delete 매크로
#define SAFE_DELETE_ARRAY(p) { if(p) delete []p ; (p)=NULL; } // 배열 delete 매크로
#define min2max(a, x, b) max(a, min(b, x))      // a : 최소값, b:최대값, x : 원하는값.
/////////////////////////////////////////////////////////////////////////////
// CBsButton

CBsButton::CBsButton(int nType)
{
	m_bDisablePaint = FALSE;
	m_w = m_h = 0;	
	m_nButtonType = nType;
	m_bCheck = FALSE;
	m_pParent = NULL;
	m_state = m_statePrev = NORMAL;
	m_bDisabled = FALSE;	
	m_bPushed = FALSE;
	m_nButtonCount = 0;
	m_bHover = FALSE;
	m_bOver = FALSE;
	m_bOverMsg = FALSE;
}

CBsButton::~CBsButton()
{
}


BEGIN_MESSAGE_MAP(CBsButton, CWnd)	
	ON_WM_PAINT()	
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBsButton message handlers

BOOL CBsButton::Create(const CPoint& pt, CWnd* pParentWnd, UINT nID, CxImage& xImage, int nButtonCount, 
					   int nButtonType)
{
	m_nButtonCount = nButtonCount;
	CRect rt;
	m_w = xImage.GetWidth();
	m_h = xImage.GetHeight();
	rt.SetRect(pt.x, pt.y, pt.x + (m_w/nButtonCount), pt.y+m_h);
	return Create(rt, pParentWnd, nID, xImage, nButtonCount, nButtonType);
}

BOOL CBsButton::SetWindowPos(int x, int y)
{
	CPoint pt(x,y);
	return SetWindowPos(pt);
}

BOOL CBsButton::SetWindowPos(CPoint ptNew)
{
	// 	CRect rtNew;	
	// 	rtNew.SetRect(ptNew.x, ptNew.y, ptNew.x + (m_w/m_nButtonCount), ptNew.y+m_h);
	// 	m_rtRegion = rtNew;
	return CWnd::SetWindowPos(NULL, ptNew.x, ptNew.y, 0, 0, SWP_NOSIZE);
}

// BOOL CBsButton::ShowWindow(int nCmdShow)
// {
// 	if(nCmdShow == SW_SHOW)
// 		SetDisable(FALSE);
// 	else if(nCmdShow == SW_HIDE)
// 		SetDisable(TRUE);
// 
// 	return CWnd::ShowWindow(nCmdShow);
// }

BOOL CBsButton::Create(const CRect& rect, CWnd* pParentWnd, UINT nID, CxImage& xImage, int nButtonCount,
					   int nButtonType)
{
	m_nButtonCount = nButtonCount;
	m_nButtonType = nButtonType;
	m_nID = nID;
	m_pParent = pParentWnd;
	//m_rtRegion = rect;

	m_w = xImage.GetWidth();
	m_h = xImage.GetHeight();

	m_size.cx = m_w / nButtonCount;
	m_size.cy = m_h;
	ASSERT(m_size.cx > 0 && m_size.cy > 0);

	for(int i=0; i<nButtonCount; ++i)
	{		
		CRect rtCrop(0, 0, m_size.cx, m_size.cy);
		rtCrop.left  = m_size.cx*i;
		rtCrop.right = m_size.cx*(i+1);
		xImage.Crop(rtCrop, &m_imgSkin[i]);
	}

	return CWnd::Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rect, pParentWnd, nID);
}

void CBsButton::SetRegionFromBitmap(HBITMAP hBitmap)
{
	// set window region from bitmap
	HANDLE hInst = ::AfxGetInstanceHandle();	

	HRGN hrgnWindow = CXGDIHelper::CreateRgnFromBitmap(hBitmap, TRANSPARENT_COLOR);
	::DeleteObject(hBitmap);

	HWND hwnd = this->GetSafeHwnd();
	// Set region
	if( hrgnWindow )
	{
		if( ::SetWindowRgn(hwnd, hrgnWindow, TRUE) == 0 )
			TRACE(_T("SetWindowRgn failed\n"));
	}	
	::DeleteObject(hrgnWindow);	
}

void CBsButton::OnPaint() 
{
	CPaintDC paintDC(this); // device context for painting

	if(m_bDisablePaint)
		return;

	// TODO: Add your message handler code here
	CMemDC_ memdc(&paintDC);
	HDC hdc = memdc.GetSafeHdc();
	HBITMAP hBitmap = m_imgSkin[NORMAL].MakeBitmap(hdc);

	if(m_bOver)
	{
		if(m_nButtonCount >= 3)
		{
			//HBITMAP hBitmap = m_imgSkin[OVERMOUSE].MakeBitmap(hdc);
			SetRegionFromBitmap(hBitmap);
			m_imgSkin[OVERMOUSE].Draw2(hdc, CRect(0,0,m_size.cx,m_size.cy));
		}
		else
		{
			SetRegionFromBitmap(hBitmap);
			m_imgSkin[NORMAL].Draw2(hdc, CRect(0,0,m_size.cx,m_size.cy));
		}
	}
	else
	{
		switch(m_state)
		{
		case NORMAL:
			if(m_nButtonCount >= 1)
			{			
				SetRegionFromBitmap(hBitmap);
				m_imgSkin[NORMAL].Draw2(hdc, CRect(0,0,m_size.cx,m_size.cy));
			}
			break;

		case OVERMOUSE:
			if(m_nButtonCount >= 3)
			{
				//HBITMAP hBitmap = m_imgSkin[OVERMOUSE].MakeBitmap(hdc);
				SetRegionFromBitmap(hBitmap);
				m_imgSkin[OVERMOUSE].Draw2(hdc, CRect(0,0,m_size.cx,m_size.cy));
			}
			else
			{
				SetRegionFromBitmap(hBitmap);
				m_imgSkin[NORMAL].Draw2(hdc, CRect(0,0,m_size.cx,m_size.cy));
			}
			break;

		case PUSHED:
			if(m_nButtonCount >= 3)
			{
				//HBITMAP hBitmap = m_imgSkin[PUSHED].MakeBitmap(hdc);
				SetRegionFromBitmap(hBitmap);
				m_imgSkin[PUSHED].Draw2(hdc, CRect(0,0,m_size.cx,m_size.cy));
			}
			else if(m_nButtonCount >= 2)
			{
				SetRegionFromBitmap(hBitmap);
				m_imgSkin[OVERMOUSE].Draw2(hdc, CRect(0,0,m_size.cx,m_size.cy));
			}
			else
			{
				SetRegionFromBitmap(hBitmap);
				m_imgSkin[NORMAL].Draw2(hdc, CRect(0,0,m_size.cx,m_size.cy));
			}
			break;	

		case DISABLED:		
			if(m_nButtonCount >= 4)
			{
				//HBITMAP hBitmap = m_imgSkin[DISABLED].MakeBitmap(hdc);
				SetRegionFromBitmap(hBitmap);
				m_imgSkin[DISABLED].Draw2(hdc, CRect(0,0,m_size.cx,m_size.cy));
			}
			break;
		}

		DeleteObject(hBitmap);
	}

	// Do not call CWnd::OnPaint() for painting messages
}

void CBsButton::RunThisButton()
{	
	ASSERT(m_pParent);
	if(m_pParent)
	{		
		m_pParent->SendMessage(WM_MYCOMMAND, m_nID, (LPARAM)0);
	}
}

void CBsButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bDisabled)
	{
		return;
	}

	// _TrackMouseEvent 중복 생성 피하기 위해
	if(!m_bHover)
	{
		BOOL bRet = FALSE;
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);		
		tme.dwFlags = TME_LEAVE;		
		tme.hwndTrack = GetSafeHwnd();
		tme.dwHoverTime = 100;// HOVER_DEFAULT;

		m_bHover = ::_TrackMouseEvent (&tme);

		if(m_bOverMsg)
		{
			if(m_pParent)
			{		
				m_pParent->PostMessage(WM_MYCOMMAND, m_nID, (LPARAM)1);
			}
		}

		if((nFlags & MK_LBUTTON ) == MK_LBUTTON && m_bPushed)
		{
			m_state = PUSHED;

		}
		else
		{
			if(m_nButtonType == PUSH_BUTTON)
			{
				m_state = OVERMOUSE;
			}
			else if(m_nButtonType == RADIO_BUTTON || m_nButtonType == TOGGLE_BUTTON)
			{
				if(m_bCheck)
				{
					m_state = PUSHED;

				}
				else
				{
					m_state = OVERMOUSE;
				}
			}
		}
		Invalidate();
	}

	//SetCursor(theApp.LoadCursor(IDC_ARROW));

	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CBsButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if(m_bDisabled)
	{
		return 0L;
	}

	if( m_nButtonType == PUSH_BUTTON )
	{
		m_state = NORMAL;
	}
	else if( m_nButtonType == RADIO_BUTTON )		
	{
		CWnd* pCap = GetCapture();
		//if((pMsg->wParam & MK_LBUTTON ) == MK_LBUTTON)
		if(pCap)
		{
			m_state = NORMAL;
		}
		else
		{
			if(m_bCheck)
				m_state = PUSHED;
			else
				m_state = NORMAL;
		}
	}
	else if( m_nButtonType == TOGGLE_BUTTON )
	{
		if(m_bCheck)
			m_state = PUSHED;
		else
			m_state = NORMAL;
	}

	m_bHover = FALSE;
	Invalidate();

	return 0L;
}

void CBsButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_bDisabled)
	{
		return;
	}

	m_bPushed = TRUE;
	SetCapture();

	if( m_nButtonType == PUSH_BUTTON )
	{
		m_state = PUSHED;
	}
	else if( m_nButtonType == RADIO_BUTTON )
	{
		if(m_bCheck)
			m_statePrev = PUSHED;
		else 
			m_statePrev = NORMAL;
		m_bCheck = TRUE;
		m_state = PUSHED;
	}
	else if( m_nButtonType == TOGGLE_BUTTON )
	{
		if(m_bCheck)
			m_statePrev = PUSHED;
		else 
			m_statePrev = NORMAL;

		m_bCheck = !m_bCheck;
		if(m_bCheck)
			m_state = PUSHED;
		else
			m_state = OVERMOUSE;
	}

	Invalidate();

	CWnd::OnLButtonDown(nFlags, point);
}

void CBsButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	::ReleaseCapture();

	if(m_bDisabled)
	{
		return;
	}

	CRect rtClient;
	GetClientRect(rtClient);
	if(!rtClient.PtInRect(point))
	{
		if( m_nButtonType == PUSH_BUTTON )
		{
			Invalidate();
			return;
		}
		else if( m_nButtonType == TOGGLE_BUTTON )
		{
			if(m_statePrev == PUSHED)
			{
				m_bCheck = TRUE;
				Invalidate();
			}
			else
			{
				m_bCheck = FALSE;
				Invalidate();
			}
			return;
		}
		else if( m_nButtonType == RADIO_BUTTON )
		{
			if(m_statePrev == PUSHED)
			{
				m_bCheck = TRUE;
				Invalidate();
			}
			else
			{
				m_bCheck = FALSE;
				Invalidate();
			}
			return;
		}
	}

	if(!m_bPushed)
	{
		return;
	}

	if( m_nButtonType == PUSH_BUTTON )
	{
		m_state = OVERMOUSE;
	}
	else if( m_nButtonType == RADIO_BUTTON )
	{
		m_bCheck = TRUE;
		m_state = PUSHED;
		m_statePrev = PUSHED;
	}	

	Invalidate();

	m_bPushed = FALSE;
	RunThisButton();

	CWnd::OnLButtonUp(nFlags, point);
}

void CBsButton::SetCheck(BOOL bCheck) 
{ 
	m_bCheck = bCheck;

	if(bCheck)
		m_state = PUSHED;
	else 
		m_state = NORMAL;
	Invalidate();
}

void CBsButton::SetDisable(BOOL bDisable) 
{ 
	if(bDisable)
	{
		m_bDisabled = TRUE; 
		m_state = DISABLED; 
	}
	else
	{
		m_bDisabled = FALSE;
		m_state = NORMAL;
	}

	Invalidate(); 
}
BOOL CBsButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CWnd::OnEraseBkgnd(pDC);
}


void CBsButton::SetOver(BOOL bOver)
{
	m_bOver = bOver;

}

void CBsButton::SetOverMsg(BOOL bOverMsg)
{
	m_bOverMsg = bOverMsg;
}