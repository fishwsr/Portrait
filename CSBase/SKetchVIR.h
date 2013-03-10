// SketchVIR.h: interface for the CSketchVIR class.
//
///////////////////////////////////////////////////////////////////////////

#ifndef __CSBASE_SKETCHVIR_H__
#define __CSBASE_SKETCHVIR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SketchDefs.h"

class CSBASE_API CSketchVIR
{
public:
    //static bool Render(HDC hdc, const RECT * prcBound, const HANDLE hSkt);
    static bool Render(Gdiplus::Graphics &gr, const CSketchResult * lpResult, const RECT * prcBound = NULL);
    static bool Render(HDC hdc, const CSketchResult * lpResultt, const RECT * prcBound = NULL);
    static bool Render(CVisGrayByteImage * lpImage, const CSketchResult * lpResultt, const RECT * prcBound = NULL);

    static bool RenderContour(Gdiplus::Graphics &gr, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderHair(Gdiplus::Graphics &gr, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderEar(Gdiplus::Graphics &gr, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderBrow(Gdiplus::Graphics &gr, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderEye(Gdiplus::Graphics &gr, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderNose(Gdiplus::Graphics &gr, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderMouth(Gdiplus::Graphics &gr, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderTemplate(Gdiplus::Graphics &gr, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderBGStrokes(Gdiplus::Graphics &gr, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);

	//static bool RenderJpHair(Gdiplus::Graphics &gr, const CPartResult * lpPartRet);
	static bool RenderJpHair(Gdiplus::Graphics &gr, const CPartResult * lpPartRet,const CPartResultCtrl* pDrawCtrl);

    static bool RenderContour(HDC hdc, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderHair(HDC hdc, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderEar(HDC hdc, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderBrow(HDC hdc, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderEye(HDC hdc, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderNose(HDC hdc, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderMouth(HDC hdc, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderTemplate(HDC hdc, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderBGStrokes(HDC hdc, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);

    static bool RenderContour(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderHair(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderEar(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderBrow(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderEye(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderNose(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderMouth(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderTemplate(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);
    static bool RenderBGStrokes(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);

	static void DrawLineWithSktroke(Gdiplus::Graphics &gr, long color, const CStroke &stroke, const CStrokeCtrl &control, double width);
	static void DrawControlPoints(Gdiplus::Graphics &gr, long color, const CStroke &stroke, const CStrokeCtrl &control, double width);
	static void DrawEndPoints(Gdiplus::Graphics &gr, long color, const CStroke &stroke, const CStrokeCtrl &control, double width);

	static void DrawLineWithSktroke(Gdiplus::Graphics &gr, long color, const CShape &stroke, const CStrokeCtrl &control, double width);
	static void DrawStrokeSetWithSktroke(Gdiplus::Graphics &gr, long color, const std::vector<CShape>& strokeSet, const CStrokeCtrl &control, double width);
	
	static long GetRealColorValue(unsigned char chValue);
//private:
    static void DrawChgWidStroke(Gdiplus::Graphics &gr, const CStroke& stroke, const CStrokeCtrl& control, double dScale, long color);
    static void DrawRegion(Gdiplus::Graphics &gr, long color, const CShape &shape);
	static void DrawCircle(Gdiplus::Graphics &gr, long color, double cx, double cy, double r);
    static void DrawClip(Gdiplus::Graphics &gr, long color, const Gdiplus::PointF *pCurve, int count);
	static void AddCurve(Gdiplus::Graphics &gr, long color, const CPoint2 *pDrawPoints, int count, const CPoint2 *pDrawPoints2, int count2);
	static void AddCurve(Gdiplus::Graphics &gr, long color, const CShape &shape, const CShape &shape2);
	static void AddCurve(Gdiplus::Graphics &gr, long color, const CShape &shape);
// added by sunlei 2002.9.14 begin
    static bool DrawFilledRegion(Gdiplus::Graphics &gr, long color, const CStrokeSet &strokeSet);
	static bool DrawCurve(Gdiplus::Graphics &gr, long color, const CShape& shape,double width);
	static bool RenderJpComplexRegion(Gdiplus::Graphics &gr, const CPartResult * lpPartRet);
// added by sunlei 2002.9.14 end
	static void ScaleSketch(CSketchResult & sketch, const RECT * prcBound);
};

typedef bool(*RenderPartFunc)(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl);

#endif // __CSBASE_SKETCHVIR_H__
