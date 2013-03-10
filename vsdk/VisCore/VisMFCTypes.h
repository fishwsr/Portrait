// VisMFCTypes.h
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved


#pragma once


class CVisRect : 
#ifdef VIS_IS_USING_MFC
	public CRect
#else // VIS_IS_USING_MFC
	public RECT
#endif // VIS_IS_USING_MFC
{
public:
	CVisRect() {}
	CVisRect(int l, int t, int r, int b) { left = l; top = t; right = r; bottom = b; }
	CVisRect(POINT pt1, POINT pt2) { left = pt1.x; top = pt1.y; right = pt2.x; bottom = pt2.y; }
	CVisRect(POINT pt, SIZE size) { left = pt.x; top = pt.y; right = pt.x + size.cx; bottom = pt.y + size.cy; }
	CVisRect(const RECT& refrect) { left = refrect.left; top = refrect.top; right = refrect.right; bottom = refrect.bottom; }
	CVisRect(LPCRECT lpSrcRect) { left = lpSrcRect->left; top = lpSrcRect->top; right = lpSrcRect->right; bottom = lpSrcRect->bottom; }
	CVisRect& operator=(const RECT& refrect) { left = refrect.left; top = refrect.top; right = refrect.right; bottom = refrect.bottom; return *this; }

#ifndef  VIS_IS_USING_MFC
	int Width(void) const { return right - left; }
	int Height(void) const { return bottom - top; }
	SIZE Size(void) const { SIZE size; size.cx = Width(); size.cy = Height(); return size; }
	POINT TopLeft(void) const { return *((POINT *) this); }
	POINT BottomRight(void) const { return *((POINT *) &right); }
	POINT CenterPoint(void) const { POINT point; point.x = (left + right) / 2; point.y = (top + bottom) / 2; return point; }

	BOOL OffsetRect(int dx, int dy) { return ::OffsetRect(this, dx, dy); }
	BOOL InflateRect(int dx, int dy) { return ::InflateRect(this, dx, dy); }
	void InflateRect(int dx1, int dy1, int dx2, int dy2) { left -= dx1; top -= dy1; right += dx2; bottom += dy2; }
	bool operator==(const RECT& refrect) const { return ((left == refrect.left) && (top == refrect.top) && (right == refrect.right) && (bottom == refrect.bottom)); }
	bool operator!=(const RECT& refrect) const { return !(*this == refrect); }
	CVisRect& operator&=(const RECT& refrect) { ::IntersectRect(this, this, &refrect); return *this; }  // UNDONE:  Can this be both src and dest here?
	CVisRect& operator|=(const RECT& refrect) { ::UnionRect(this, this, &refrect); return *this; }  // UNDONE:  Can this be both src and dest here?
	CVisRect operator&(const RECT& refrect) const { CVisRect rectRet(*this); rectRet &= refrect; return rectRet; }
	CVisRect operator|(const RECT& refrect) const { CVisRect rectRet(*this); rectRet |= refrect; return rectRet; }
	void NormalizeRect()
	{
		int w;
		if (left > right)
		{
			w = left;
			left = right;
			right = w;
		}
		if (top > bottom)
		{
			w = top;
			top = bottom;
			bottom = w;
		}
	}
#endif // VIS_IS_USING_MFC
};

class CVisPoint : 
#ifdef VIS_IS_USING_MFC
	public CPoint
#else // VIS_IS_USING_MFC
	public POINT
#endif // VIS_IS_USING_MFC
{
public:
	CVisPoint(){ }
	CVisPoint(int initX, int initY){ x = initX; y = initY; }
	CVisPoint(POINT initPt){ x = initPt.x; y = initPt.y; }
	CVisPoint(SIZE initSize){ x = initSize.cx; y = initSize.cy; }
	CVisPoint(DWORD dwPoint){ x = (short)LOWORD(dwPoint); y = (short)HIWORD(dwPoint); }
	CVisPoint& operator=(const POINT& refPoint){ x = refPoint.x; y = refPoint.y; return *this;}
};


class CVisSize : 
#ifdef VIS_IS_USING_MFC
	public CSize
#else // VIS_IS_USING_MFC
	public SIZE
#endif // VIS_IS_USING_MFC
{
public:
	CVisSize(){ }
	CVisSize(int initCX, int initCY){ cx = initCX; cy = initCY; }
	CVisSize(SIZE initSize){ cx = initSize.cx; cy = initSize.cy; }
	CVisSize(POINT initPt){ cx = initPt.x; cy = initPt.y; }
	CVisSize(DWORD dwSize){ cx = (short)LOWORD(dwSize); cy = (short)HIWORD(dwSize); }

	CVisSize& operator=(const SIZE& refSize){ cx = refSize.cx; cy = refSize.cy; return *this;}
};
