/**************************************************************************************************
/ XGDIHelper.h: interface for the CXGDIHelper class.
**************************************************************************************************/
//-------------------------------------------------------------------------------------------------

#if !defined(AFX_XGDIHELPER_H__8DE7E420_D4C3_4125_8B86_04F4CE3A3296__INCLUDED_)
#define AFX_XGDIHELPER_H__8DE7E420_D4C3_4125_8B86_04F4CE3A3296__INCLUDED_

//-------------------------------------------------------------------------------------------------
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-------------------------------------------------------------------------------------------------
//
// CXGDIHelper decl.
//
//-------------------------------------------------------------------------------------------------
class CXGDIHelper  
{
public:
	CXGDIHelper();
	virtual ~CXGDIHelper();
	
	static void FillRect(HDC hDC, RECT *lpRect, COLORREF clrColor);
	static void FrameRect(HDC hDC, RECT *lpRect, COLORREF clrColor);
	static void DrawBar(HDC hDC, RECT *lpRect, COLORREF clrColor, COLORREF clrBorderColor);
	static void FillGradationRect(HDC hDC, const RECT* lpRect, COLORREF clrBgnRGB, COLORREF clrEndRGB, INT nStep);
	static void DrawBitmap(HDC hDC, INT nX, INT nY, HBITMAP hBitmap);
	static HRGN CreateRgnFromBitmap(HBITMAP hBitmap, DWORD dwTransColor);
	static void DrawLine(HDC hDC, INT nStartX, INT nStartY, INT nEndX, INT nEndY, COLORREF clrColor);
	static void DrawTransparent(CDC *pDC, int x, int y,COLORREF clrTransparency, HBITMAP hBitmap);
};

//-------------------------------------------------------------------------------------------------
#endif // !defined(AFX_XGDIHELPER_H__8DE7E420_D4C3_4125_8B86_04F4CE3A3296__INCLUDED_)
//-------------------------------------------------------------------------------------------------
