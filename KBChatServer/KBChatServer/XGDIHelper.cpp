// XGDIHelper.cpp: implementation of the CXGDIHelper class.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include "XGDIHelper.h"

//-------------------------------------------------------------------------------------------------
//
// CXGDIHelper impl.
//
//-------------------------------------------------------------------------------------------------

/**************************************************************************************************
/ CXGDIHelper::CXGDIHelper()
/ 
**************************************************************************************************/
CXGDIHelper::CXGDIHelper()
{
	
}

/**************************************************************************************************
/ CXGDIHelper::~CXGDIHelper()
/ 
**************************************************************************************************/
CXGDIHelper::~CXGDIHelper()
{
	
}

/**************************************************************************************************
/ void CXGDIHelper::DrawBar(HDC hDC, RECT *lpRect, COLORREF clrBgColor, COLORREF clrBorderColor)
/ 
**************************************************************************************************/
void CXGDIHelper::DrawBar(HDC hDC, RECT *lpRect, COLORREF clrColor, COLORREF clrBorderColor)
{
	HBRUSH hbrBar = ::CreateSolidBrush(clrColor);
	::FillRect(hDC, lpRect, hbrBar);
	::DeleteObject(hbrBar);
	
	HBRUSH hbrBorder = ::CreateSolidBrush(clrBorderColor);
	::FrameRect(hDC, lpRect, hbrBorder);
	::DeleteObject(hbrBorder);
	
	//::InflateRect(lpRect, -1, -1);
	
}

/**************************************************************************************************
/ void CXGDIHelper::FillRect(HDC hDC, RECT *lpRect, COLORREF clrColor)
/ 
**************************************************************************************************/
void CXGDIHelper::FillRect(HDC hDC, RECT *lpRect, COLORREF clrColor)
{
	HBRUSH hbr = ::CreateSolidBrush(clrColor);
	::FillRect(hDC, lpRect, hbr);
	::DeleteObject(hbr);
	
	//::InflateRect(lpRect, -1, -1);
	
}

/**************************************************************************************************
/ void CXGDIHelper::FrameRect(HDC hDC, RECT *lpRect, COLORREF clrColor)
/ 
**************************************************************************************************/
void CXGDIHelper::FrameRect(HDC hDC, RECT *lpRect, COLORREF clrColor)
{
	HBRUSH hbr = ::CreateSolidBrush(clrColor);
	::FrameRect(hDC, lpRect, hbr);
	::DeleteObject(hbr);
	
	//::InflateRect(lpRect, -1, -1);
	
}

/**************************************************************************************************
/ void CXGDIHelper::FillGradationRect(HDC hDC, const RECT* lpRect, COLORREF clrBgnRGB, COLORREF clrEndRGB, INT nStep)
/ 
**************************************************************************************************/
void CXGDIHelper::FillGradationRect(HDC hDC, const RECT* lpRect, COLORREF clrBgnRGB, COLORREF clrEndRGB, INT nStep)
{
	//nStep
	//start rgb
	//end rgb
	
	//get begin r, g, b value
	int bgn_r = GetRValue(clrBgnRGB);
	int bgn_g = GetGValue(clrBgnRGB);
	int bgn_b = GetBValue(clrBgnRGB);
	
	//get end r, g, b value
	int end_r = GetRValue(clrEndRGB);
	int end_g = GetGValue(clrEndRGB);
	int end_b = GetBValue(clrEndRGB);
	
	//get nStep r, g, b value
	int nStep_r = (end_r - bgn_r) / nStep;
	int nStep_g = (end_g - bgn_g) / nStep;
	int nStep_b = (end_b - bgn_b) / nStep;
	
	int start = lpRect->top;
	int gab = (lpRect->bottom - lpRect->top) / nStep;
	int right = lpRect->left + (lpRect->right - lpRect->left);//for right of lpRect
	int remain = (lpRect->bottom - lpRect->top) - nStep * gab;
	
	
	for( int i=0; i<nStep-1; i++ ) 
	{
		CBrush br;
		br.CreateSolidBrush(
			RGB(
			bgn_r += nStep_r,
			bgn_g += nStep_g, 
			bgn_b += nStep_b
			)//end of RGB
			);
		
		::FillRect(
			hDC,
			(LPCRECT)CRect(lpRect->left, start, right, start + gab),   
			(HBRUSH)br
			);//end of FillRect
		start += gab;
	}//end of for
	
	CBrush br;
	br.CreateSolidBrush(
		RGB(
		bgn_r += nStep_r,
		bgn_g += nStep_g, 
		bgn_b += nStep_b
		)//end of RGB
		);
	
	::FillRect(
		hDC,
		(LPCRECT)CRect(lpRect->left, start, right, start + gab + remain),   
		(HBRUSH)br
		);//end of FillRect
}

/**************************************************************************************************
/ void CXGDIHelper::DrawBitmap(HDC hDC, RECT *lpRect, WORD wBitmapID)
/ 
**************************************************************************************************/
void CXGDIHelper::DrawBitmap(HDC hDC, INT nX, INT nY, HBITMAP hBitmap)
{
	HDC hMemDC = CreateCompatibleDC(hDC);											// create memory dc
	if( !hMemDC ) 
	{
		TRACE(_T("fail to CreateCompatibleDC in CXGDIHelper::DrawBitmap()\r\n"));
		return;
	}
	
	// draw bitmap
	HBITMAP hNewBitmap = hBitmap;
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hNewBitmap);
	BITMAP bmp;
	::GetObject(hNewBitmap, sizeof(BITMAP), &bmp);
	::BitBlt(hDC, nX, nY, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCCOPY);
	::DeleteObject(hNewBitmap);
	// restore it
	::SelectObject(hMemDC, hOldBitmap);	
	::DeleteDC(hMemDC);
}

// Created by Bosun Son 2006.10.26 , referred to Jeff Prosise.
void CXGDIHelper::DrawTransparent(CDC *pDC, int x, int y,COLORREF clrTransparency, HBITMAP hBitmap)
{
	CBitmap _THIS;
	_THIS.Attach(hBitmap);

    BITMAP bm;
    _THIS.GetBitmap (&bm);
    CPoint size (bm.bmWidth, bm.bmHeight);
    pDC->DPtoLP (&size);

    CPoint org (0, 0);
    pDC->DPtoLP (&org);

	//
    // Create a memory DC (dcImage) and select the bitmap into it.
	//
    CDC dcImage;
    dcImage.CreateCompatibleDC (pDC);
    CBitmap* pOldBitmapImage = dcImage.SelectObject(&_THIS);
    dcImage.SetMapMode (pDC->GetMapMode ());

	//
    // Create a second memory DC (dcAnd) and in it create an AND mask.
	//
    CDC dcAnd;
    dcAnd.CreateCompatibleDC (pDC);
    dcAnd.SetMapMode (pDC->GetMapMode ());

    CBitmap bitmapAnd;
    bitmapAnd.CreateBitmap (bm.bmWidth, bm.bmHeight, 1, 1, NULL);
    CBitmap* pOldBitmapAnd = dcAnd.SelectObject (&bitmapAnd);

    dcImage.SetBkColor (clrTransparency);
    dcAnd.BitBlt (org.x, org.y, size.x, size.y, &dcImage, org.x, org.y,
        SRCCOPY);

	//
    // Create a third memory DC (dcXor) and in it create an XOR mask.
	//
    CDC dcXor;
    dcXor.CreateCompatibleDC (pDC);
    dcXor.SetMapMode (pDC->GetMapMode ());

    CBitmap bitmapXor;
    bitmapXor.CreateCompatibleBitmap (&dcImage, bm.bmWidth, bm.bmHeight);
    CBitmap* pOldBitmapXor = dcXor.SelectObject (&bitmapXor);

    dcXor.BitBlt (org.x, org.y, size.x, size.y, &dcImage, org.x, org.y,
        SRCCOPY);

    dcXor.BitBlt (org.x, org.y, size.x, size.y, &dcAnd, org.x, org.y,
        0x220326);

	//
    // Copy the pixels in the destination rectangle to a temporary
    // memory DC (dcTemp).
	//
    CDC dcTemp;
    dcTemp.CreateCompatibleDC (pDC);
    dcTemp.SetMapMode (pDC->GetMapMode ());

    CBitmap bitmapTemp;
    bitmapTemp.CreateCompatibleBitmap (&dcImage, bm.bmWidth, bm.bmHeight);
    CBitmap* pOldBitmapTemp = dcTemp.SelectObject (&bitmapTemp);

    dcTemp.BitBlt (org.x, org.y, size.x, size.y, pDC, x, y, SRCCOPY);

	//
    // Generate the final image by applying the AND and XOR masks to
    // the image in the temporary memory DC.
	//
    dcTemp.BitBlt (org.x, org.y, size.x, size.y, &dcAnd, org.x, org.y,
        SRCAND);

    dcTemp.BitBlt (org.x, org.y, size.x, size.y, &dcXor, org.x, org.y,
        SRCINVERT);

	//
    // Blit the resulting image to the screen.
	//
    pDC->BitBlt (x, y, size.x, size.y, &dcTemp, org.x, org.y, SRCCOPY);

	//
    // Restore the default bitmaps.
	//
    dcTemp.SelectObject (pOldBitmapTemp);
    dcXor.SelectObject (pOldBitmapXor);
    dcAnd.SelectObject (pOldBitmapAnd);
    dcImage.SelectObject (pOldBitmapImage);
}

/**************************************************************************************************
/ HRGN CXGDIHelper::CreateRgnFromBitmap(HBITMAP hBitmap, DWORD dwTransColor)
/ 
**************************************************************************************************/
HRGN CXGDIHelper::CreateRgnFromBitmap(HBITMAP hBitmap, DWORD dwTransColor)
{  
	// region handle
	HRGN hRgn = NULL;
	
	// rect region count
	int nCntRect = 0;
	
	// dc create
	HDC hDC = ::CreateCompatibleDC(NULL);
	if (!hDC) return NULL;
	
	// bitmap create
	BITMAP bm;
	GetObject(hBitmap, sizeof(BITMAP), &bm);
	
	LPRGNDATA  pRgnData  = (LPRGNDATA) new BYTE[sizeof(RGNDATAHEADER) + sizeof(RECT) * bm.bmWidth * bm.bmHeight];
	LPCOLORREF pScanData = new COLORREF[bm.bmWidth];
	if (pRgnData && pScanData){
		LPRECT pRect = (LPRECT)pRgnData->Buffer;
		BITMAPINFOHEADER bi;                  
		ZeroMemory(&bi, sizeof(BITMAPINFOHEADER));
		bi.biSize        = sizeof(BITMAPINFOHEADER);
		bi.biWidth       = bm.bmWidth;
		bi.biHeight      = bm.bmHeight;
		bi.biPlanes      = 1;
		bi.biBitCount    = 32;          
		bi.biCompression = BI_RGB;
		for (int y = 1; y <= bm.bmHeight; y++){
			
			
			GetDIBits(hDC, hBitmap, bm.bmHeight - y, 1, pScanData, (LPBITMAPINFO)&bi, DIB_RGB_COLORS);
			
			for (int x = 0; x < bm.bmWidth; x++)
			{
				if (pScanData[x] != dwTransColor)
				{               
					pRect->left = x;                               
					for (; x < bm.bmWidth; x++)                    
						if (pScanData[x] == dwTransColor) break;   
						pRect->right  = x;                             
						pRect->top    = y - 1;
						pRect->bottom = y;
						pRect++;
						nCntRect++;
				}
			}
		}
		
		pRgnData->rdh.dwSize         = sizeof(RGNDATAHEADER);
		pRgnData->rdh.iType          = RDH_RECTANGLES;
		pRgnData->rdh.nRgnSize       = sizeof(RGNDATAHEADER) + sizeof(RECT) * nCntRect;
		pRgnData->rdh.nCount         = nCntRect;
		pRgnData->rdh.rcBound.top    = 0;
		pRgnData->rdh.rcBound.left   = 0;
		pRgnData->rdh.rcBound.bottom = bm.bmHeight;
		pRgnData->rdh.rcBound.right  = bm.bmWidth;
		hRgn = ExtCreateRegion(NULL, pRgnData->rdh.nRgnSize, pRgnData);
	}
	delete[] pScanData; 
	delete[] pRgnData;  
	DeleteDC(hDC);
	
	return hRgn;
}

void CXGDIHelper::DrawLine(HDC hDC, INT nStartX, INT nStartY, INT nEndX, INT nEndY, COLORREF clrColor)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 1, clrColor);
	HGDIOBJ hOldPen = ::SelectObject(hDC, hPen);
	
	::MoveToEx(hDC, nStartX, nStartY, NULL);			// 
	::LineTo(hDC, nEndX, nEndY);
	
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);
}