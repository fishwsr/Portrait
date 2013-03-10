// SketchVIR.cpp: implementation of the CSketchVIR class.
//
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SketchVIR.h"

//#include <crtdbg.h>
//#include <tchar.h>

double g_flScaleFactor = 1.0f;
double g_flNoseFactor = 0.1f;
double g_flMouthFactor = 0.1f;

// init the color table
//modified by lin begin, 2003-4
static	long    g_lColorTable[16] = 
{   0xff000000, 0xffff0000, 0xff00ff00, 0xff0000ff,
	//brown(159,35,0),  yellow(255,232,88), red_brown(202,98,88), green(0,187,50)
	0xff9f2300,         0xffffe858,         0xffca6258,           0xff00bb32,
	//pink(255,136,157), brown1(159,96,0),  blue(93,179,212),     red1(255,0,45)
    0xffff889d,          0xff9f6000,        0xff5db3d4,           0xffff002d,
    //pink1(255,210,237), pink2(255,63,153)
	0xffffd2ed,           0xffff3f99, 0xffffffff, 0xffffffff
};
//modified by lin end, 2003-4

long CSketchVIR::GetRealColorValue(unsigned char chValue)
{
    long color;
    if (chValue < 16)
    {
        color = g_lColorTable[chValue];
    }
    else
    {
        color = chValue * (256 * 256 + 256 + 1);
    }
    return color;
}

void CSketchVIR::ScaleSketch(CSketchResult & result, const RECT * prcBound)
{
	double left, top, right, bottom;
	
	result.AroundRect(left, top, right, bottom);
	double dW, dH, dRatio;

	double faceleft, facetop, faceright, facebottom;	
	result.m_shape.AroundRect(faceleft, facetop, faceright, facebottom);
	dW = (faceright - faceleft)*1.92f;
	dW /= (double)((prcBound->right - prcBound->left) );
	dH = (facebottom - facetop)*2.1f;
	dH /= (double)((prcBound->bottom - prcBound->top) );
	
	
	/*
    dW = max(right - left, 256.0);
	if (dW > 256.0)
	{
		dW = max(dW, 512.0);
	}
	dH = max(bottom - top, 256.0);
	if (dH > 256.0)
	{
		dH = max(dH, 512.0);
	}
    dW = right - left;
    dH = bottom - top;
    dW /= (double)((prcBound->right - prcBound->left) * 0.85f);
    dH /= (double)((prcBound->bottom - prcBound->top) * 0.85f);
	*/

    dRatio = max(dW, dH);
    result /= dRatio;

	double x1, x2, y1;//, y2;
	result.AroundRect(left, top, right, bottom);
	result.m_shape.AroundRect(faceleft, facetop, faceright, facebottom);
 
	//x1 = (left + right) / 2.0f;
	x1 = (faceleft + faceright) / 2.0f;
	//y1 = (top + bottom) / 2.0f;
	y1 = (facebottom - facetop) * 0.15f + facebottom;
	if (y1 < bottom)
	{
		y1 = bottom;
	}

	x2 = (double)(prcBound->right + prcBound->left) / 2.0f;
	//y2 = (double)(prcBound->bottom + prcBound->top) / 2.0f;


	//result.Translate(x2 - x1, (y2 - y1) + ((prcBound->bottom - prcBound->top) - (bottom - top)) / 2.0f);
	result.Translate( x2 - x1, prcBound->bottom - y1);

	g_flScaleFactor = 1.0f / dRatio;
}

/*
bool CSketchVIR::Render(HDC hdc, const RECT * prcBound, const HANDLE hSkt)
{
    if (!hdc)
    {
        return false;
    }

    CSketchResult result;
    if (!result.Decode(hSkt))
    {
        return false;
    }

	ScaleSketch(result, prcBound);

	if (!Render(hdc, &result))
    {
        return false;
    }

    return true;
}
*/

bool CSketchVIR::Render(Gdiplus::Graphics &gr, const CSketchResult * lpResultIn, const RECT * prcBound)
{
    if (!lpResultIn)
    {
        return false;
    }
	
	CSketchResult sketch = *lpResultIn;
	CSketchResult * lpResult = &sketch;
	if (NULL != prcBound)
	{
		ScaleSketch(sketch, prcBound);
	}

// added by sunlei 2002.9.14 begin
    if (lpResult->m_agPartRet[PART_JP1HAIR_B].m_vStrokeSet.size() > 0)  //need to render those templates
		//modified by lin begin, 2003-5
		RenderJpHair(gr, &lpResult->m_agPartRet[PART_JP1HAIR_B], &lpResult->m_defPartRetCtrl);
		//RenderJpHair(gr, &lpResult->m_agPartRet[PART_JP1HAIR_B],NULL);
	//modified by lin end, 2003-5
// added by sunlei 2002.9.14 end
	DRAWTYPE DrawType = (DRAWTYPE)lpResult->m_DrawControl[DT_EAR_POS];
    if (DT_NODRAW != DrawType)
	{
		if (lpResult->m_agPartRetCtrl[PART_EAR].m_vStrokeCtrlSet.size() < 1)
			RenderEar(gr, &lpResult->m_agPartRet[PART_EAR], &lpResult->m_defPartRetCtrl);
		else
			RenderEar(gr, &lpResult->m_agPartRet[PART_EAR], &lpResult->m_agPartRetCtrl[PART_EAR]);
	}

	DrawType = (DRAWTYPE)lpResult->m_DrawControl[DT_CONTOUR_POS];
    if (DT_NODRAW != DrawType)
	{
		if (lpResult->m_agPartRetCtrl[PART_CONTOUR].m_vStrokeCtrlSet.size() < 1)
			RenderContour(gr, &lpResult->m_agPartRet[PART_CONTOUR], &lpResult->m_defPartRetCtrl);
		else
			RenderContour(gr, &lpResult->m_agPartRet[PART_CONTOUR], &lpResult->m_agPartRetCtrl[PART_CONTOUR]);
	}

	DrawType = (DRAWTYPE)lpResult->m_DrawControl[DT_HAIR_POS];
    if (DT_NODRAW != DrawType)
	{
		if (lpResult->m_agPartRetCtrl[PART_HAIR].m_vStrokeCtrlSet.size() < 1)
			RenderHair(gr, &lpResult->m_agPartRet[PART_HAIR], &lpResult->m_defPartRetCtrl);
		else
			RenderHair(gr, &lpResult->m_agPartRet[PART_HAIR], &lpResult->m_agPartRetCtrl[PART_HAIR]);
	}

	DrawType = (DRAWTYPE)lpResult->m_DrawControl[DT_BROW_POS];
    if (DT_NODRAW != DrawType)
	{
		if (lpResult->m_agPartRetCtrl[PART_BROW].m_vStrokeCtrlSet.size() < 1)
			RenderBrow(gr, &lpResult->m_agPartRet[PART_BROW], &lpResult->m_defPartRetCtrl);
		else
			RenderBrow(gr, &lpResult->m_agPartRet[PART_BROW], &lpResult->m_agPartRetCtrl[PART_BROW]);
	}

	DrawType = (DRAWTYPE)lpResult->m_DrawControl[DT_EYE_POS];
    if (DT_NODRAW != DrawType)
	{
		if (lpResult->m_agPartRetCtrl[PART_EYE].m_vStrokeCtrlSet.size() < 1)
			RenderEye(gr, &lpResult->m_agPartRet[PART_EYE], &lpResult->m_defPartRetCtrl);
		else
			RenderEye(gr, &lpResult->m_agPartRet[PART_EYE], &lpResult->m_agPartRetCtrl[PART_EYE]);
	}

	DrawType = (DRAWTYPE)lpResult->m_DrawControl[DT_NOSE_POS];
    if (DT_NODRAW != DrawType)
	{
		if (lpResult->m_agPartRetCtrl[PART_NOSE].m_vStrokeCtrlSet.size() < 1)
			RenderNose(gr, &lpResult->m_agPartRet[PART_NOSE], &lpResult->m_defPartRetCtrl);
		else
			RenderNose(gr, &lpResult->m_agPartRet[PART_NOSE], &lpResult->m_agPartRetCtrl[PART_NOSE]);
	}

	DrawType = (DRAWTYPE)lpResult->m_DrawControl[DT_MOUTH_POS];
    if (DT_NODRAW != DrawType)
	{
		if (lpResult->m_agPartRetCtrl[PART_MOUTH].m_vStrokeCtrlSet.size() < 1)
			RenderMouth(gr, &lpResult->m_agPartRet[PART_MOUTH], &lpResult->m_defPartRetCtrl);
		else
			RenderMouth(gr, &lpResult->m_agPartRet[PART_MOUTH], &lpResult->m_agPartRetCtrl[PART_MOUTH]);
	}


// added by sunlei 2002.9.14 begin
    if (lpResult->m_agPartRet[PART_JP1HAIR_F].m_vStrokeSet.size() > 0)  //need to render those templates
		RenderJpHair(gr, &lpResult->m_agPartRet[PART_JP1HAIR_F], &lpResult->m_defPartRetCtrl);
// added by sunlei 2002.9.14 end
    if (lpResult->m_agPartRet[PART_TEMPLATE].m_vStrokeSet.size() > 0)  //need to render those templates
    {
        if (lpResult->m_agPartRetCtrl[PART_TEMPLATE].m_vStrokeCtrlSet.size() < 1)
        {
	        RenderTemplate(gr, &lpResult->m_agPartRet[PART_TEMPLATE], &lpResult->m_defPartRetCtrl);
        }
        else
        {
	        RenderTemplate(gr, &lpResult->m_agPartRet[PART_TEMPLATE], &lpResult->m_agPartRetCtrl[PART_TEMPLATE]);
        }
    }
// added by sunlei 2002.9.25 begin
    if (lpResult->m_agPartRet[PART_JP1BEARD].m_vStrokeSet.size() > 0)  //need to render those templates
		RenderJpComplexRegion(gr, &lpResult->m_agPartRet[PART_JP1BEARD]);
    if (lpResult->m_agPartRet[PART_JP1GLASS].m_vStrokeSet.size() > 0)  //need to render those templates
	{
		if (lpResult->m_agPartRet[PART_JP1GLASS].m_vStrokeSet.size() == 1)
		{	// new style glasses, render as bgstroke
	        RenderBGStrokes(gr, &lpResult->m_agPartRet[PART_JP1GLASS], &lpResult->m_defPartRetCtrl);
		}
		else
		{	// old style glasses
			RenderJpComplexRegion(gr, &lpResult->m_agPartRet[PART_JP1GLASS]);
		}
	}
// added by sunlei 2002.9.25 end

    if (lpResult->m_agPartRet[PART_BGSTROKES].m_vStrokeSet.size() > 0)
        if (lpResult->m_agPartRetCtrl[PART_BGSTROKES].m_vStrokeCtrlSet.size() < 1)
	        RenderBGStrokes(gr, &lpResult->m_agPartRet[PART_BGSTROKES], &lpResult->m_defPartRetCtrl);
        else
	        RenderBGStrokes(gr, &lpResult->m_agPartRet[PART_BGSTROKES], &lpResult->m_agPartRetCtrl[PART_BGSTROKES]);
    /*
    double left, top, right, bottom;
    lpResult->AroundRect(left, top, right, bottom);
    Gdiplus::Pen pen(Color::Blue, 2.0f);
    gr.DrawRectangle(&pen, (REAL)left, (REAL)top, right - left, bottom - top);
    WCHAR szMsg[255];
    swprintf(szMsg, L"%f, %f, \n%f, %f", left, top, right, bottom);
    Gdiplus::Font myFont(L"Arial", 9);
    Gdiplus::PointF origin(0.0f, 0.0f);
    Gdiplus::SolidBrush blackBrush(Color(255, 0, 0, 0));
    gr.DrawString(szMsg, -1, &myFont, origin, &blackBrush);
    */
    return true;
}

bool CSketchVIR::RenderContour(Gdiplus::Graphics &gr, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    if (!lpPartRetCtrl)
    {
        return false;
    }

    if (1 != lpPartRet->m_vStrokeSet.size())
    {
        return false;
    }

	gr.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

    ///modified by qfyin to fix render control bug, 2002-08-06
    CStroke stroke;
	CStroke stroke1, stroke2, stroke3;

    stroke = lpPartRet->m_vStrokeSet[0][0];
    CStrokeCtrl control = lpPartRetCtrl->m_vStrokeCtrlSet[0][0];
    ///to here

    if (stroke.m_Path.PointNum() != 19)
    {
        return false;
    }

    stroke1.m_Path.Resize(7);
    stroke2.m_Path.Resize(5);
    stroke3.m_Path.Resize(7);

	for (int i = 0; i < 7; i++)
	{
		stroke1.m_Path.PointX(i) = stroke.m_Path.PointX(i);
		stroke1.m_Path.PointY(i) = stroke.m_Path.PointY(i);
		stroke3.m_Path.PointX(i) = stroke.m_Path.PointX(18 - i);
		stroke3.m_Path.PointY(i) = stroke.m_Path.PointY(18 - i);
	}

    for (i = 0; i < 5; i++)
	{
		stroke2.m_Path.PointX(i) = stroke.m_Path.PointX(7 + i);
		stroke2.m_Path.PointY(i) = stroke.m_Path.PointY(7 + i);
	}

// added by sunlei 2002.9.14 begin
	int iPointNum = stroke.m_Path.PointNum();
	double	dbFaceWidth;
	Gdiplus::PointF * pCurve = new Gdiplus::PointF[iPointNum + 2];
	dbFaceWidth = fabs(stroke.m_Path.PointX(0) - stroke.m_Path.PointX(iPointNum-1));
	pCurve[0].X = (float) stroke.m_Path.PointX(0)+(float)dbFaceWidth / 4.0f;
	pCurve[0].Y = (float) stroke.m_Path.PointY(0)-(float)dbFaceWidth * 0.55f;
	for (i = 0; i < iPointNum; i++)
	{
		pCurve[i+1].X = (float) stroke.m_Path.PointX(i);
		pCurve[i+1].Y = (float) stroke.m_Path.PointY(i);
	}
	pCurve[iPointNum+1].X = (float) stroke.m_Path.PointX(iPointNum-1) - (float)dbFaceWidth / 4.0f;
	pCurve[iPointNum+1].Y = (float) stroke.m_Path.PointY(iPointNum-1) - (float)dbFaceWidth * 0.55f;
	DrawClip(gr, 0xffffff, pCurve, iPointNum + 2);
	delete []pCurve;
    DrawLineWithSktroke(gr, stroke.m_ucLineColor, stroke, control, stroke.m_ucLineWidth);
// added by sunlei 2002.9.14 end

    return true;
}

bool CSketchVIR::RenderHair(Gdiplus::Graphics &gr, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    if (!lpPartRetCtrl)
    {
        return false;
    }

    if (1 != lpPartRet->m_vStrokeSet.size() || 2 != lpPartRet->m_vStrokeSet[0].size() )
    {
        return false;
    }

	gr.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

    CShape FrontHairShape, BackHairShape;
    CStroke hairStroke;
    CStrokeCtrl control = lpPartRetCtrl->m_vStrokeCtrlSet[0][0];

    FrontHairShape = lpPartRet->m_vStrokeSet[0][0].m_Path;
    BackHairShape = lpPartRet->m_vStrokeSet[0][1].m_Path;

	hairStroke.m_Path.Resize(FrontHairShape.PointNum() + BackHairShape.PointNum());
    ::PutSubShape(hairStroke.m_Path, FrontHairShape, FrontHairShape.PointNum(), 0);
	::PutSubShape(hairStroke.m_Path, BackHairShape, BackHairShape.PointNum(), FrontHairShape.PointNum());

	DrawRegion(gr, 0x000000, hairStroke.m_Path);
	DrawLineWithSktroke(gr, 0x000000, lpPartRet->m_vStrokeSet[0][1], control, lpPartRet->m_vStrokeSet[0][1].m_ucLineWidth * 2);

	int iTotalPoint = FrontHairShape.PointNum();
	bool *bBreak = new bool[iTotalPoint];
	for (int i = 0; i < iTotalPoint; i++)
	{
		bBreak[i] = true;
	}

	for (i = 1; i < iTotalPoint - 1; i++)
	{
		double dx1, dy1, dx2, dy2;
		dx1 = FrontHairShape.PointX(i) - FrontHairShape.PointX(i - 1);
		dy1 = FrontHairShape.PointY(i) - FrontHairShape.PointY(i - 1);
		dx2 = FrontHairShape.PointX(i + 1) - FrontHairShape.PointX(i);
		dy2 = FrontHairShape.PointY(i + 1) - FrontHairShape.PointY(i);

		if ((dx1 * dx2 + dy1 * dy2) < 0)
			bBreak[i] = true;
		else
			bBreak[i] = false;
	}

	i = 0;
	int j = 0;
	int line_no = 2;
	while(i < iTotalPoint - 1)
	{
		j = i + 1;
		while (!bBreak[j]) j++;
		int curlen = j - i;
		double dx1, dy1, det;
		dx1 = FrontHairShape.PointX(j) - FrontHairShape.PointX(i);
		dy1 = FrontHairShape.PointY(j) - FrontHairShape.PointY(i);
		det = dx1 * dx1 + dy1 * dy1;
		det = sqrt(det);
		if (det > 10)
		{
			CShape tempShape;
			tempShape.Resize(j - i + 1);
			for (int k = i; k <= j; k++)
			{
				tempShape.PointX(k - i) = FrontHairShape.PointX(k);
				tempShape.PointY(k - i) = FrontHairShape.PointY(k);	
			}
            CStroke tempStroke;
            tempStroke.m_Path = tempShape;
            tempStroke.m_ucLineColor = lpPartRet->m_vStrokeSet[0][0].m_ucLineColor;
            tempStroke.m_ucLineWidth = lpPartRet->m_vStrokeSet[0][0].m_ucLineWidth;
        	DrawLineWithSktroke(gr, 0x000000, tempStroke, control, tempStroke.m_ucLineWidth * 2);
		}	
		i = j;			
	}

	delete []bBreak;

    return true;
}

bool CSketchVIR::RenderEar(Gdiplus::Graphics &gr, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    if (!lpPartRetCtrl)
    {
        return false;
    }

    if (2 != lpPartRet->m_vStrokeSet.size())
    {
        return false;
    }

	gr.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
    ///modified by qfyin to fix render control bug, 2002-08-06
    CStroke LEarStroke, REarStroke;
    LEarStroke = lpPartRet->m_vStrokeSet[0][0];
    REarStroke = lpPartRet->m_vStrokeSet[1][0];
    CStrokeCtrl control = lpPartRetCtrl->m_vStrokeCtrlSet[0][0];
    ///to here
	if (LEarStroke.m_Path.PointNum() > 0)
	{
		Gdiplus::PointF * pCurve = new Gdiplus::PointF[LEarStroke.m_Path.PointNum()];
		for (int i = 0; i < LEarStroke.m_Path.PointNum(); i++)
		{
			pCurve[i].X = (float) LEarStroke.m_Path.PointX(i);
			pCurve[i].Y = (float) LEarStroke.m_Path.PointY(i);
		}
		DrawClip(gr, 0xffffff, pCurve, LEarStroke.m_Path.PointNum());
		delete []pCurve;
		DrawLineWithSktroke(gr, 0x000000, LEarStroke, control, LEarStroke.m_ucLineWidth * 2);
	}

	if (REarStroke.m_Path.PointNum() > 0)
	{
		Gdiplus::PointF * pCurve = new Gdiplus::PointF[REarStroke.m_Path.PointNum()];
		for (int i = 0; i < REarStroke.m_Path.PointNum(); i++)
		{
			pCurve[i].X = (float) REarStroke.m_Path.PointX(i);
			pCurve[i].Y = (float) REarStroke.m_Path.PointY(i);
		}
		DrawClip(gr, 0xffffff, pCurve, REarStroke.m_Path.PointNum());
		delete []pCurve;
		DrawLineWithSktroke(gr, 0x000000, REarStroke, control, REarStroke.m_ucLineWidth * 2);
	}

    return true;
}

bool CSketchVIR::RenderBrow(Gdiplus::Graphics &gr, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    if (!lpPartRetCtrl)
    {
        return false;
    }

    if (2 != lpPartRet->m_vStrokeSet.size())
    {
        return false;
    }

	gr.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

	if ((lpPartRet->m_vStrokeSet[0].size()==1)&&(lpPartRet->m_vStrokeSet[0].size()==1))
	{
		CShape LBrowShape;
		CShape RBrowShape;

		LBrowShape = lpPartRet->m_vStrokeSet[0][0].m_Path;
		RBrowShape = lpPartRet->m_vStrokeSet[1][0].m_Path;

		CShape LUBrowShape, RUBrowShape, LBBrowShape, RBBrowShape;

		LUBrowShape.Resize(7);
		GetSubShape(LBrowShape, 0, LUBrowShape, 0, 7);

		RUBrowShape.Resize(7);
		GetSubShape(RBrowShape, 0, RUBrowShape, 0, 7);

		LBBrowShape.Resize(6);
		GetSubShape(LBrowShape, 6, LBBrowShape, 0, 5);
		LBBrowShape.PointX(5) = LBrowShape.PointX(0);
		LBBrowShape.PointY(5) = LBrowShape.PointY(0);

		RBBrowShape.Resize(6);
		GetSubShape(RBrowShape, 6, RBBrowShape, 0, 5);
		RBBrowShape.PointX(5) = RBrowShape.PointX(0);
		RBBrowShape.PointY(5) = RBrowShape.PointY(0);

		AddCurve(gr, 0x000000, LUBrowShape, LBBrowShape);

		AddCurve(gr, 0x000000, RUBrowShape, RBBrowShape);
	}
	else	//draw template strokeset
	{
		//delete the final contour stroke
		CPartResult TempPartRet = *lpPartRet;
		std::vector<CStrokeSet>& vecStrokeSet = TempPartRet.m_vStrokeSet;
		for(int k=0; k<2; k++)
		{
			CStrokeSet& StrokeSet = vecStrokeSet[k];
			StrokeSet.pop_back();
		}
		RenderBGStrokes(gr, &TempPartRet, lpPartRetCtrl);
	}
	return true;
}

bool CSketchVIR::RenderEye(Gdiplus::Graphics &gr, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    if (!lpPartRetCtrl)
    {
        return false;
    }

    if (2 != lpPartRet->m_vStrokeSet.size())
    {
        return false;
    }

	gr.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

// added by sunlei 2002.9.14 begin
	int iCtrlsetNum = 0;
	int	iCtrlNum = 0;
	iCtrlsetNum = lpPartRetCtrl->m_vStrokeCtrlSet.size();
	if (0 < iCtrlsetNum)
		iCtrlNum = lpPartRetCtrl->m_vStrokeCtrlSet[0].size();
// added by sunlei 2002.9.14 end
    CStrokeCtrl control = lpPartRetCtrl->m_vStrokeCtrlSet[0][0];

    //render the left eye
    CStrokeSet LEyeStrokeSet = lpPartRet->m_vStrokeSet[0];

	Gdiplus::PointF *pCurve = NULL;
	int iULPtNum = LEyeStrokeSet[EYE_UP_LINE].m_Path.PointNum();
	int iBLPtNum = LEyeStrokeSet[EYE_BELOW_LINE].m_Path.PointNum();
	
    Gdiplus::GraphicsPath gp;
	assert(iULPtNum + iBLPtNum > 0);
	if (iBLPtNum > 0)
	{	// use two line to clip
		pCurve = new Gdiplus::PointF[iULPtNum + iBLPtNum];
		for (int i = 0; i < iULPtNum; i++)
		{
			pCurve[i].X = (float) LEyeStrokeSet[EYE_UP_LINE].m_Path.PointX(i);
			pCurve[i].Y = (float) LEyeStrokeSet[EYE_UP_LINE].m_Path.PointY(i);
		}
		for (i = 0; i < iBLPtNum; i++)
		{
			pCurve[i + iULPtNum].X = (float) LEyeStrokeSet[EYE_BELOW_LINE].m_Path.PointX(iBLPtNum - 1 - i);
			pCurve[i + iULPtNum].Y = (float) LEyeStrokeSet[EYE_BELOW_LINE].m_Path.PointY(iBLPtNum - 1 - i);
		}
	}
	else
	{	// use one line to clip
		iBLPtNum = iULPtNum;
		pCurve = new Gdiplus::PointF[iULPtNum + iBLPtNum];
		for (int i = 0; i < iULPtNum; i++)
		{
			pCurve[i].X = (float) LEyeStrokeSet[EYE_UP_LINE].m_Path.PointX(i);
			pCurve[i].Y = (float) LEyeStrokeSet[EYE_UP_LINE].m_Path.PointY(i);
			pCurve[i + iULPtNum].X = (float) LEyeStrokeSet[EYE_UP_LINE].m_Path.PointX(iBLPtNum - 1 - i);
			pCurve[i + iULPtNum].Y = (float) LEyeStrokeSet[EYE_UP_LINE].m_Path.PointY(iBLPtNum - 1 - i) + (float)fabs(LEyeStrokeSet[EYE_BALL].m_Path.PointX(1)) * 3.0f;
		}
	}
	
// added by sunlei 2002.9.14 begin
	if (iBLPtNum > 0)
// added by sunlei 2002.9.14 end
	gp.AddClosedCurve(pCurve, iULPtNum + iBLPtNum);
	Gdiplus::SolidBrush brush(Gdiplus::Color::White);
// added by sunlei 2002.9.14 begin
	if (iBLPtNum > 0)
	{
// added by sunlei 2002.9.14 end
	gr.FillPath(&brush, &gp);
	gr.SetClip(&gp);
// added by sunlei 2002.9.14 begin
	}
// added by sunlei 2002.9.14 end

    if (LEyeStrokeSet[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_BALL))
    {
	    DrawCircle(gr,
            0x000000,
            LEyeStrokeSet[EYE_BALL].m_Path.PointX(0),
            LEyeStrokeSet[EYE_BALL].m_Path.PointY(0),
            LEyeStrokeSet[EYE_BALL].m_Path.PointX(1) );
    }
    if (LEyeStrokeSet[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_BRIGHT_POINT))
    {
	    DrawCircle(gr,
            0xffffff,
            LEyeStrokeSet[EYE_BRIGHT_POINT].m_Path.PointX(0),
            LEyeStrokeSet[EYE_BRIGHT_POINT].m_Path.PointY(0),
            LEyeStrokeSet[EYE_BRIGHT_POINT].m_Path.PointX(1) );
    }
// added by sunlei 2002.9.14 begin
	if (iBLPtNum > 0)
// added by sunlei 2002.9.14 end
	gr.ResetClip();
	
    delete[] pCurve;

// added by sunlei 2002.9.14 begin
	if (EYE_UP_LINE < iCtrlNum)
		control = lpPartRetCtrl->m_vStrokeCtrlSet[0][EYE_UP_LINE];
// added by sunlei 2002.9.14 end
    if (LEyeStrokeSet[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_EYE_LINE))
    {
	    DrawLineWithSktroke(gr,
            0x000000,
            LEyeStrokeSet[EYE_UP_LINE],
            control,
            LEyeStrokeSet[EYE_UP_LINE].m_ucLineWidth);
	    DrawLineWithSktroke(gr,
            0x000000,
            LEyeStrokeSet[EYE_BELOW_LINE],
            control,
            LEyeStrokeSet[EYE_BELOW_LINE].m_ucLineWidth);
    }
    if (LEyeStrokeSet[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_DOUBLE_LINE))
    {
	    DrawLineWithSktroke(gr,
            0x000000,
            LEyeStrokeSet[EYE_DOUBLE_LINE],
            control,
            LEyeStrokeSet[EYE_DOUBLE_LINE].m_ucLineWidth);
    }
// added by sunlei 2002.9.14 begin
	if (EYE_TEST_LINE < iCtrlNum)
		control = lpPartRetCtrl->m_vStrokeCtrlSet[0][EYE_TEST_LINE];
// added by sunlei 2002.9.14 end
	///modified by qfyin to change the testline/testline1 color, 2002-08-06
    if (LEyeStrokeSet[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_TEST_LINE))
    {
	    DrawLineWithSktroke(gr,
// added by sunlei 2002.9.14 begin
/*
            0xC0C0C0,
*/
			LEyeStrokeSet[EYE_TEST_LINE].m_ucLineColor * (256 * 256 + 256 + 1),
// added by sunlei 2002.9.14 end
            LEyeStrokeSet[EYE_TEST_LINE],
            control,
            LEyeStrokeSet[EYE_TEST_LINE].m_ucLineWidth);
    }    
    if (LEyeStrokeSet[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_TEST_LINE1))
    {
	    DrawLineWithSktroke(gr,
// added by sunlei 2002.9.14 begin
/*
            0xC0C0C0,
*/
			LEyeStrokeSet[EYE_TEST_LINE1].m_ucLineColor * (256 * 256 + 256 + 1),
// added by sunlei 2002.9.14 end
            LEyeStrokeSet[EYE_TEST_LINE1],
            control,
            LEyeStrokeSet[EYE_TEST_LINE1].m_ucLineWidth);
    }

    //render the right eye
    CStrokeSet REyeStrokeSet = lpPartRet->m_vStrokeSet[1];

// added by sunlei 2002.9.14 begin
	if (1 < iCtrlsetNum)
		iCtrlNum = lpPartRetCtrl->m_vStrokeCtrlSet[1].size();
// added by sunlei 2002.9.14 end

	iULPtNum = REyeStrokeSet[EYE_UP_LINE].m_Path.PointNum();
	iBLPtNum = REyeStrokeSet[EYE_BELOW_LINE].m_Path.PointNum();
	
	if (iBLPtNum > 0)
	{	// use two line to clip
		pCurve = new Gdiplus::PointF[iULPtNum + iBLPtNum];
		for (int i = 0; i < iULPtNum; i++)
		{
			pCurve[i].X = (float) REyeStrokeSet[EYE_UP_LINE].m_Path.PointX(i);
			pCurve[i].Y = (float) REyeStrokeSet[EYE_UP_LINE].m_Path.PointY(i);
		}
		for (i = 0; i < iBLPtNum; i++)
		{
			pCurve[i + iULPtNum].X = (float) REyeStrokeSet[EYE_BELOW_LINE].m_Path.PointX(iBLPtNum - 1 - i);
			pCurve[i + iULPtNum].Y = (float) REyeStrokeSet[EYE_BELOW_LINE].m_Path.PointY(iBLPtNum - 1 - i);
		}
	}
	else
	{	// use one line to clip
		iBLPtNum = iULPtNum;
		pCurve = new Gdiplus::PointF[iULPtNum + iBLPtNum];
		for (int i = 0; i < iULPtNum; i++)
		{
			pCurve[i].X = (float) REyeStrokeSet[EYE_UP_LINE].m_Path.PointX(i);
			pCurve[i].Y = (float) REyeStrokeSet[EYE_UP_LINE].m_Path.PointY(i);
			pCurve[i + iULPtNum].X = (float) REyeStrokeSet[EYE_UP_LINE].m_Path.PointX(iBLPtNum - 1 - i);
			pCurve[i + iULPtNum].Y = (float) REyeStrokeSet[EYE_UP_LINE].m_Path.PointY(iBLPtNum - 1 - i) + (Gdiplus::REAL)fabs(REyeStrokeSet[EYE_BALL].m_Path.PointX(1)) * 3.0f;
		}
	}
	
    gp.Reset();
	
// added by sunlei 2002.9.14 begin
	if (iBLPtNum > 0)
	{
// added by sunlei 2002.9.14 end
    gp.AddClosedCurve(pCurve, iULPtNum + iBLPtNum);
	gr.FillPath(&brush, &gp);
	gr.SetClip(&gp);
// added by sunlei 2002.9.14 begin
	}
// added by sunlei 2002.9.14 end

    if (REyeStrokeSet[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_BALL))
    {
	    DrawCircle(gr,
            0x000000,
            REyeStrokeSet[EYE_BALL].m_Path.PointX(0),
            REyeStrokeSet[EYE_BALL].m_Path.PointY(0),
            REyeStrokeSet[EYE_BALL].m_Path.PointX(1) );
    }
    if (REyeStrokeSet[EYE_DRAW_CTRL].m_Path.PointX((EYE_DRAW_BRIGHT_POINT)))
    {
	    DrawCircle(gr,
            0xffffff,
            REyeStrokeSet[EYE_BRIGHT_POINT].m_Path.PointX(0),
            REyeStrokeSet[EYE_BRIGHT_POINT].m_Path.PointY(0),
            REyeStrokeSet[EYE_BRIGHT_POINT].m_Path.PointX(1) );
    }
	gr.ResetClip();
	
    delete[] pCurve;

// added by sunlei 2002.9.14 begin
	if (EYE_UP_LINE < iCtrlNum)
		control = lpPartRetCtrl->m_vStrokeCtrlSet[1][EYE_UP_LINE];
// added by sunlei 2002.9.14 end
    if (REyeStrokeSet[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_EYE_LINE))
    {
	    DrawLineWithSktroke(gr,
            0x000000,
            REyeStrokeSet[EYE_UP_LINE],
            control,
            REyeStrokeSet[EYE_UP_LINE].m_ucLineWidth);
	    DrawLineWithSktroke(gr,
            0x000000,
            REyeStrokeSet[EYE_BELOW_LINE],
            control,
            REyeStrokeSet[EYE_BELOW_LINE].m_ucLineWidth);
    }

    if (REyeStrokeSet[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_DOUBLE_LINE))
    {
	    DrawLineWithSktroke(gr,
            0x000000,
            REyeStrokeSet[EYE_DOUBLE_LINE],
            control,
            REyeStrokeSet[EYE_DOUBLE_LINE].m_ucLineWidth);
    }
// added by sunlei 2002.9.14 begin
	if (EYE_TEST_LINE < iCtrlNum)
		control = lpPartRetCtrl->m_vStrokeCtrlSet[1][EYE_TEST_LINE];
// added by sunlei 2002.9.14 end
	///modified by qfyin to change the testline/testline1 color, 2002-08-06
    if (REyeStrokeSet[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_TEST_LINE))
    {
	    DrawLineWithSktroke(gr,
// added by sunlei 2002.9.14 begin
/*
            0xC0C0C0,
*/
			REyeStrokeSet[EYE_TEST_LINE].m_ucLineColor * (256 * 256 + 256 + 1),
// added by sunlei 2002.9.14 end
            REyeStrokeSet[EYE_TEST_LINE],
            control,
            REyeStrokeSet[EYE_TEST_LINE].m_ucLineWidth);
    }
    
    if (REyeStrokeSet[EYE_DRAW_CTRL].m_Path.PointX(EYE_DRAW_TEST_LINE1))
    {
	    DrawLineWithSktroke(gr,
// added by sunlei 2002.9.14 begin
/*
            0xC0C0C0,
*/
			REyeStrokeSet[EYE_TEST_LINE1].m_ucLineColor * (256 * 256 + 256 + 1),
// added by sunlei 2002.9.14 end
            REyeStrokeSet[EYE_TEST_LINE1],
            control,
            REyeStrokeSet[EYE_TEST_LINE1].m_ucLineWidth);
    }
    
    return true;
}

bool CSketchVIR::RenderNose(Gdiplus::Graphics &gr, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    if (!lpPartRetCtrl)
    {
        return false;
    }

    if (1 != lpPartRet->m_vStrokeSet.size() && 2 != lpPartRet->m_vStrokeSet.size())
    {
        return false;
    }

	gr.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	
    CStrokeCtrl control = lpPartRetCtrl->m_vStrokeCtrlSet[0][0];

	for (int i = 0; i < lpPartRet->m_vStrokeSet[0].size(); i++)
	{
        CStroke stroke = lpPartRet->m_vStrokeSet[0][i];
		DrawLineWithSktroke(gr, stroke.m_ucLineColor * (256 * 256 + 256 + 1), stroke, control, (double)stroke.m_ucLineWidth * g_flNoseFactor);
	}

	if (lpPartRet->m_vStrokeSet.size() > 1)
	for (i = 0; i < lpPartRet->m_vStrokeSet[1].size(); i++)
	{
        CStroke stroke = lpPartRet->m_vStrokeSet[1][i];
		DrawLineWithSktroke(gr, stroke.m_ucLineColor * (256 * 256 + 256 + 1), stroke, control, (double)stroke.m_ucLineWidth * g_flNoseFactor);
	}

    return true;
}

bool CSketchVIR::RenderMouth(Gdiplus::Graphics &gr, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    if (!lpPartRetCtrl)
    {
        return false;
    }

    if (1 != lpPartRet->m_vStrokeSet.size())
    {
        return false;
    }

	gr.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

// added by sunlei 2002.9.14 begin
	int iCtrlsetNum = 0;
	int	iCtrlNum = 0;
	iCtrlsetNum = lpPartRetCtrl->m_vStrokeCtrlSet.size();
	if (0 < iCtrlsetNum)
		iCtrlNum = lpPartRetCtrl->m_vStrokeCtrlSet[0].size();
// added by sunlei 2002.9.14 end
    CStrokeCtrl control = lpPartRetCtrl->m_vStrokeCtrlSet[0][0];

	for (int i = 0; i < lpPartRet->m_vStrokeSet[0].size(); i++)
	{
// added by sunlei 2002.9.14 begin
		if (i < iCtrlNum)
			control = lpPartRetCtrl->m_vStrokeCtrlSet[0][i];
// added by sunlei 2002.9.14 end
        CStroke stroke = lpPartRet->m_vStrokeSet[0][i];
		DrawLineWithSktroke(gr, GetRealColorValue(stroke.m_ucLineColor), stroke, control, (double)stroke.m_ucLineWidth * g_flMouthFactor);
	}

    return true;
}

bool CSketchVIR::RenderTemplate(Gdiplus::Graphics &gr, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    _ASSERTE(NULL != lpPartRet && NULL != lpPartRetCtrl);
    if (NULL == lpPartRetCtrl || NULL == lpPartRet)
    {
        return false;
    }

    for(int iTemplate = TEMPLATE_FIRST; iTemplate < TEMPLATE_COUNT; iTemplate ++)
    {
        //calculate the clip region and fill it.
        const CStrokeCtrlSet & StrokeCtrlSet = lpPartRetCtrl->m_vStrokeCtrlSet[iTemplate];
        const CStrokeSet & StrokeSet = lpPartRet->m_vStrokeSet[iTemplate];
        if (StrokeSet.size() == 0)
        {
            continue;
        }

        const CShape &region = StrokeSet[0].m_Path;
        gr.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

        int iRegionPointNum = region.PointNum();
        if (iRegionPointNum > 0)
        {
			std::vector<CPoint2> points;
            CCubicBezierSpline spline;

            points.resize(iRegionPointNum);
            for (int i = 0; i < iRegionPointNum; i++)
            {
                points[i].X = (float) region.PointX(i);
                points[i].Y = (float) region.PointY(i);
            }
            spline.Create_Catmull_Rom_Spline(points);

            Gdiplus::PointF *pts = new Gdiplus::PointF[spline.NbSegment() * 3 + 1];
            pts[0].X = spline.m_Segments[0].m_ControlPoints[0].X;
            pts[0].Y = spline.m_Segments[0].m_ControlPoints[0].Y;
            for (i = 0; i < spline.NbSegment(); i++)
            {
                pts[i * 3 + 1].X = spline.m_Segments[i].m_ControlPoints[1].X;
                pts[i * 3 + 1].Y = spline.m_Segments[i].m_ControlPoints[1].Y;
                pts[i * 3 + 2].X = spline.m_Segments[i].m_ControlPoints[2].X;
                pts[i * 3 + 2].Y = spline.m_Segments[i].m_ControlPoints[2].Y;
                pts[i * 3 + 3].X = spline.m_Segments[i].m_ControlPoints[3].X;
                pts[i * 3 + 3].Y = spline.m_Segments[i].m_ControlPoints[3].Y;
            }

            Gdiplus::GraphicsPath gp;
            gp.AddBeziers(pts, spline.NbSegment() * 3 + 1);
            Gdiplus::SolidBrush brush(Gdiplus::Color::White);
            gr.FillPath(&brush, &gp);
            delete[] pts;
        }

        //Draw the template strokes
        //the first stroke's shape means Clip Region, so we skip it
        for(int i = 1; i < StrokeSet.size(); i ++)
        {
            if (StrokeCtrlSet.size() > 1)
            {
                DrawChgWidStroke(gr,
                    StrokeSet[i],
                    StrokeCtrlSet[i - 1],
                    1.0,
                    GetRealColorValue(StrokeSet[i].m_ucLineColor));
            }
            else
            {
                DrawLineWithSktroke(gr,
                GetRealColorValue(StrokeSet[i].m_ucLineColor),
                StrokeSet[i],
                StrokeCtrlSet[0],
                StrokeSet[i].m_ucLineWidth);
            }
        }
    }

    return true;
}

bool CSketchVIR::RenderBGStrokes(Gdiplus::Graphics &gr, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    if (!lpPartRetCtrl)
    {
        return false;
    }

    
	gr.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

    CStrokeCtrl control = lpPartRetCtrl->m_vStrokeCtrlSet[0][0];

	for(int k=0; k<lpPartRet->m_vStrokeSet.size(); k++)
	{
		for (int i = 0; i < lpPartRet->m_vStrokeSet[k].size(); i++)
		{
			CStroke stroke = lpPartRet->m_vStrokeSet[k][i];
			long color = GetRealColorValue(stroke.m_ucLineColor);
			DrawLineWithSktroke(gr, color, stroke, control, stroke.m_ucLineWidth);
		}
	}
	

    return true;
}

bool CSketchVIR::Render(HDC hdc, const CSketchResult * lpResultIn, const RECT * prcBound)
{
/*
#ifdef _DEBUG
    ///add by qfyin for debug
    CSketchResult *p = (CSketchResult *)lpResultIn;

    TCHAR szTempPath[MAX_PATH];
    if (0 != GetTempPath(MAX_PATH, szTempPath))
    {
        _tcscat(szTempPath, _T("\\result.skt"));
        p->WriteFile(szTempPath, _O_TEXT);
    }
    ///to here
#endif
*/
	CSketchResult sketch = *lpResultIn;
	CSketchResult * lpResult = &sketch;
	if (NULL != prcBound)
	{
		ScaleSketch(sketch, prcBound);
	}
    Gdiplus::Graphics gr(hdc);
    return Render(gr, lpResult);
}

bool CSketchVIR::RenderContour(HDC hdc, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    Gdiplus::Graphics gr(hdc);
    return RenderContour(gr, lpPartRet, lpPartRetCtrl);
}

bool CSketchVIR::RenderHair(HDC hdc, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    Gdiplus::Graphics gr(hdc);
    return RenderHair(gr, lpPartRet, lpPartRetCtrl);
}

bool CSketchVIR::RenderEar(HDC hdc, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    Gdiplus::Graphics gr(hdc);
    return RenderEar(gr, lpPartRet, lpPartRetCtrl);
}

bool CSketchVIR::RenderBrow(HDC hdc, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    Gdiplus::Graphics gr(hdc);
    return RenderBrow(gr, lpPartRet, lpPartRetCtrl);
}

bool CSketchVIR::RenderEye(HDC hdc, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    Gdiplus::Graphics gr(hdc);
    return RenderEye(gr, lpPartRet, lpPartRetCtrl);
}

bool CSketchVIR::RenderNose(HDC hdc, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    Gdiplus::Graphics gr(hdc);
    return RenderNose(gr, lpPartRet, lpPartRetCtrl);
}

bool CSketchVIR::RenderMouth(HDC hdc, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    Gdiplus::Graphics gr(hdc);
    return RenderMouth(gr, lpPartRet, lpPartRetCtrl);
}

bool CSketchVIR::RenderTemplate(HDC hdc, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    Gdiplus::Graphics gr(hdc);
    return RenderTemplate(gr, lpPartRet, lpPartRetCtrl);
}

bool CSketchVIR::RenderBGStrokes(HDC hdc, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    Gdiplus::Graphics gr(hdc);
    return RenderBGStrokes(gr, lpPartRet, lpPartRetCtrl);
}

bool CSketchVIR::Render(CVisGrayByteImage * lpImage, const CSketchResult * lpResultIn, const RECT * prcBound)
{
    HDC hdc = lpImage->Hdc();
	
	CSketchResult sketch = *lpResultIn;
	CSketchResult * lpResult = &sketch;
	if (NULL != prcBound)
	{
		ScaleSketch(sketch, prcBound);
	}
    
	bool ret = Render(hdc, lpResult);
    lpImage->DestroyHdc();
    return ret;
}

bool CSketchVIR::RenderContour(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    HDC hdc = lpImage->Hdc();
    bool ret = RenderContour(hdc, lpPartRet, lpPartRetCtrl);
    lpImage->DestroyHdc();
    return ret;
}

bool CSketchVIR::RenderHair(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    HDC hdc = lpImage->Hdc();
    bool ret = RenderHair(hdc, lpPartRet, lpPartRetCtrl);
    lpImage->DestroyHdc();
    return ret;
}

bool CSketchVIR::RenderEar(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    HDC hdc = lpImage->Hdc();
    bool ret = RenderEar(hdc, lpPartRet, lpPartRetCtrl);
    lpImage->DestroyHdc();
    return ret;
}

bool CSketchVIR::RenderBrow(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    HDC hdc = lpImage->Hdc();
    bool ret = RenderBrow(hdc, lpPartRet, lpPartRetCtrl);
    lpImage->DestroyHdc();
    return ret;
}

bool CSketchVIR::RenderEye(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    HDC hdc = lpImage->Hdc();
    bool ret = RenderEye(hdc, lpPartRet, lpPartRetCtrl);
    lpImage->DestroyHdc();
    return ret;
}

bool CSketchVIR::RenderNose(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    HDC hdc = lpImage->Hdc();
    bool ret = RenderNose(hdc, lpPartRet, lpPartRetCtrl);
    lpImage->DestroyHdc();
    return ret;
}

bool CSketchVIR::RenderMouth(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    HDC hdc = lpImage->Hdc();
    bool ret = RenderMouth(hdc, lpPartRet, lpPartRetCtrl);
    lpImage->DestroyHdc();
    return ret;
}

bool CSketchVIR::RenderTemplate(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    HDC hdc = lpImage->Hdc();
    bool ret = RenderTemplate(hdc, lpPartRet, lpPartRetCtrl);
    lpImage->DestroyHdc();
    return ret;
}

bool CSketchVIR::RenderBGStrokes(CVisGrayByteImage * lpImage, const CPartResult * lpPartRet, const CPartResultCtrl * lpPartRetCtrl)
{
    HDC hdc = lpImage->Hdc();
    bool ret = RenderBGStrokes(hdc, lpPartRet, lpPartRetCtrl);
    lpImage->DestroyHdc();
    return ret;
}

void CSketchVIR::DrawChgWidStroke(Gdiplus::Graphics &gr, const CStroke& stroke, const CStrokeCtrl& control, double dScale, long color)
{
    if (control.m_Splines[0].NbSegment() < 1 || control.m_Splines[1].NbSegment() < 1 )
        return;

    //adjust the line width
	std::vector<CPoint2> pEdgeCtrPts[2];		
    for(int k = 0; k < 2; k++)
    {
        CCubicBezierSpline Spline = control.m_Splines[k];
        Spline.Resample(Spline.NbSegment() * 20);
        Spline.GetKeyPoints(pEdgeCtrPts[k]);
    }

    BYTE cr = (BYTE) (color >> 16) & 255;
    BYTE cg = (BYTE) (color >> 8) & 255;
    BYTE cb = (BYTE) color & 255;
    Gdiplus::SolidBrush solidBrush(Gdiplus::Color(cr, cg, cb));

    Gdiplus::GraphicsPath ph;
    ph.StartFigure();

    int ipoint;
    double dX1, dY1, dX2, dY2;
    for (ipoint = 0; ipoint < pEdgeCtrPts[0].size() - 1; ipoint++)
    {
        dX1 = pEdgeCtrPts[0][ipoint].X;
        dY1 = pEdgeCtrPts[0][ipoint].Y;

        dX2 = pEdgeCtrPts[0][ipoint + 1].X;
        dY2 = pEdgeCtrPts[0][ipoint + 1].Y;

        ph.AddLine(pEdgeCtrPts[0][ipoint], pEdgeCtrPts[0][ipoint + 1]);
    }

    for (ipoint = pEdgeCtrPts[1].size() - 1; ipoint > 0; ipoint--)
        ph.AddLine(pEdgeCtrPts[1][ipoint], pEdgeCtrPts[1][ipoint - 1]);

    ph.CloseFigure();

    gr.FillPath(&solidBrush,&ph);		
}

// stroke.m_ucLineWidth	== STROKE_REGION_CATMULLROM	: Fill region, the points in stroke.m_path 
//												  is the key points of Catmull-Rom spline
//					== STROKE_REGION_CUBICBEZIERS : Fill region, the points in stroke.m_path 
//													is the controll points of Cubic Bezier splines
//					== else : Draw stroke in predefined style and the width equals stroke.m_ucLineWidth
//
void CSketchVIR::DrawLineWithSktroke(Gdiplus::Graphics &gr,
                                     long color,
                                     const CStroke& stroke,
                                     const CStrokeCtrl& control,
                                     double width)
{
	if (stroke.m_Type == STROKE_REGION_CATMULLROM)
	{	
		// region of Catmull-Rom spline
		Gdiplus::PointF *pCurve = NULL;
		int iPtNum = 0;
		Gdiplus::GraphicsPath gp(Gdiplus::FillModeWinding);
		iPtNum = stroke.m_Path.PointNum();
		pCurve = new Gdiplus::PointF[iPtNum];
		for (int k = 0; k < iPtNum; k++)
		{
			pCurve[k].X = (float) stroke.m_Path.PointX(k);
			pCurve[k].Y = (float) stroke.m_Path.PointY(k);
		}
		gp.AddClosedCurve(pCurve, iPtNum);
		delete []pCurve;
		BYTE* pbColor = NULL;
		pbColor = (BYTE*)(&color);
		Gdiplus::SolidBrush brush(Gdiplus::Color(*(pbColor+2),*(pbColor+1),*(pbColor)));
		gr.FillPath(&brush, &gp);
	}
	else if (stroke.m_Type == STROKE_REGION_CUBICBEZIERS)
	{
		// region of Cubic Bezier splines
		Gdiplus::PointF *pCurve = NULL;
		Gdiplus::GraphicsPath gp(Gdiplus::FillModeWinding);
		int iPtNum = stroke.m_Path.PointNum();
		pCurve = new Gdiplus::PointF[iPtNum];
		for (int k = 0; k < iPtNum; k++)
		{
			pCurve[k].X = (float) stroke.m_Path.PointX(k);
			pCurve[k].Y = (float) stroke.m_Path.PointY(k);
		}
		
		gp.AddBeziers(pCurve, iPtNum);
		delete []pCurve;
		BYTE* pbColor = NULL;
		pbColor = (BYTE*)(&color);
		Gdiplus::SolidBrush brush(Gdiplus::Color(*(pbColor+2),*(pbColor+1),*(pbColor)));
		gr.FillPath(&brush, &gp);
	}
	else if (stroke.m_Type == STROKE_STROKE)
	{
		// Stroke
		std::vector<CPoint2> points;
		CPoint2 *pDrawPoints, *pDrawPoints2;

		CShape shape = stroke.m_Path;
		if (shape.PointNum() <= 0)
		{
			return;
		}
		points.resize(shape.PointNum());
		for (int i = 0; i < shape.PointNum(); i++)
		{
			points[i].X = (float) shape.PointX(i);
			points[i].Y = (float) shape.PointY(i);
		}

		CCubicBezierSpline spline; // = const_cast<CCubicBezierSpline &>(stroke.m_Spline);

		spline.Create_Catmull_Rom_Spline(points);

		int iSample = 1;

		pDrawPoints = new CPoint2[spline.NbSegment() * iSample + 1];
		pDrawPoints2 = new CPoint2[spline.NbSegment() * iSample + 1];	

		DrawControlledWidthLine(spline, iSample, pDrawPoints, control, width * g_flScaleFactor);
		DrawControlledWidthLine(spline, iSample, pDrawPoints2, control, -width * g_flScaleFactor);
		AddCurve(gr, color, pDrawPoints, spline.NbSegment() * iSample + 1, pDrawPoints2, spline.NbSegment() * iSample + 1);

		delete []pDrawPoints;
		delete []pDrawPoints2;
	}
	else if (stroke.m_Type == STROKE_SIMPLELINE)
	{
		BYTE* pbColor = NULL;
		pbColor = (BYTE*)(&color);
		Gdiplus::Pen pen(Gdiplus::Color(*(pbColor+2),*(pbColor+1),*(pbColor)), (Gdiplus::REAL)width);
		Gdiplus::PointF *pCurve = NULL;
		Gdiplus::GraphicsPath gp(Gdiplus::FillModeWinding);
		int iPtNum = stroke.m_Path.PointNum();
		pCurve = new Gdiplus::PointF[iPtNum];
		for (int k = 0; k < iPtNum; k++)
		{
			pCurve[k].X = (float) stroke.m_Path.PointX(k);
			pCurve[k].Y = (float) stroke.m_Path.PointY(k);
		}
		gp.AddBeziers(pCurve, iPtNum);
		delete []pCurve;
		gr.DrawPath(&pen, &gp);

	}
}

void CSketchVIR::DrawControlPoints(Gdiplus::Graphics &gr, long color, const CStroke &stroke, const CStrokeCtrl &control, double width)
{
	Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0), (Gdiplus::REAL)2.f);
	//gr.DrawLine(&pen, 250, 0, 0, 250);
	for (int iP = 0; iP < stroke.m_Path.PointNum(); iP++)
	{
		float x, y;
		x = stroke.m_Path.PointX(iP);
		y = stroke.m_Path.PointY(iP);
		gr.DrawLine(&pen, x , y , x +1, y +1);
	}
}

void CSketchVIR::DrawEndPoints(Gdiplus::Graphics &gr, long color, const CStroke &stroke, const CStrokeCtrl &control, double width)
{
	Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0), (Gdiplus::REAL)2.f);
	
	Gdiplus::REAL x, y;
	Gdiplus::REAL diameter = width;
	
	// start point
	x = stroke.m_Path.PointX(0) - 0.5*diameter;
	y = stroke.m_Path.PointY(0) - 0.5*diameter;
	gr.DrawEllipse(&pen, x, y, diameter, diameter);

	// end point
	x = stroke.m_Path.PointX(stroke.m_Path.PointNum()-1) - 0.5*diameter;
	y = stroke.m_Path.PointY(stroke.m_Path.PointNum()-1) - 0.5*diameter;
	gr.DrawEllipse(&pen, x, y, diameter, diameter);

}


void CSketchVIR::DrawRegion(Gdiplus::Graphics &gr, long color, const CShape &shape)
{
	if (shape.PointNum() < 2) return;
	AddCurve(gr, color, shape);
}

void CSketchVIR::DrawCircle(Gdiplus::Graphics &gr, long color, double cx, double cy, double r)
{
	BYTE cr = (BYTE) (color >> 16) & 255;
	BYTE cg = (BYTE) (color >> 8) & 255;
	BYTE cb = (BYTE) color & 255;
	Gdiplus::SolidBrush brush(Gdiplus::Color(cr, cg, cb));
    Gdiplus::RectF rect((float) (cx-r), (float) (cy-r), (float)(2.0f * r), (float) (2.0f * r) );
	gr.FillEllipse(&brush, rect);
}

void CSketchVIR::DrawClip(Gdiplus::Graphics &gr, long color, const Gdiplus::PointF *pCurve, int count)
{
	BYTE cr = (BYTE) (color >> 16) & 255;
	BYTE cg = (BYTE) (color >> 8) & 255;
	BYTE cb = (BYTE) color & 255;
	Gdiplus::SolidBrush brush(Gdiplus::Color(cr, cg, cb));
	gr.FillClosedCurve(&brush, pCurve, count);
}

//画出两条尾、尾相连的曲线
void CSketchVIR::AddCurve(Gdiplus::Graphics &gr, long color, const CPoint2 *pDrawPoints, int count, const CPoint2 *pDrawPoints2, int count2)
{
	if (count < 2)
	{
		return;
	}

	CCubicBezierSpline spline;
	std::vector<CPoint2> points;
	points.resize(count);
	for (int i = 0; i < count; i++)
	{
		points[i].X = pDrawPoints[i].X;
		points[i].Y = pDrawPoints[i].Y;
	}
	spline.Create_Catmull_Rom_Spline(points);

	CCubicBezierSpline spline2;
	std::vector<CPoint2> points2;
	points2.resize(count2);
	for (i = 0; i < count2; i++)
	{
		points2[i].X = pDrawPoints2[i].X;
		points2[i].Y = pDrawPoints2[i].Y;
	}
	spline2.Create_Catmull_Rom_Spline(points2);

	int cPtNum1, cPtNum2, cPtNum;
	Gdiplus::GraphicsPath gp;
	cPtNum1 = spline.NbSegment();
	cPtNum2 = spline2.NbSegment();
	cPtNum = (cPtNum1 > cPtNum2)?cPtNum1:cPtNum2;

	Gdiplus::PointF *pts = new Gdiplus::PointF[cPtNum * 3 + 1];
	pts[0].X = spline.m_Segments[0].m_ControlPoints[0].X;
	pts[0].Y = spline.m_Segments[0].m_ControlPoints[0].Y;
	for(i = 0;	i < cPtNum1; i ++)
	{
		pts[i * 3 + 1].X = spline.m_Segments[i].m_ControlPoints[1].X;
		pts[i * 3 + 1].Y = spline.m_Segments[i].m_ControlPoints[1].Y;
		pts[i * 3 + 2].X = spline.m_Segments[i].m_ControlPoints[2].X;
		pts[i * 3 + 2].Y = spline.m_Segments[i].m_ControlPoints[2].Y;
		pts[i * 3 + 3].X = spline.m_Segments[i].m_ControlPoints[3].X;
		pts[i * 3 + 3].Y = spline.m_Segments[i].m_ControlPoints[3].Y;
	}
	gp.AddBeziers(pts, cPtNum1 * 3 + 1);
	
	Gdiplus::REAL x1 = pts[cPtNum1 * 3].X;
	Gdiplus::REAL y1 = pts[cPtNum1 * 3].Y;
	Gdiplus::REAL x2 = spline2.m_Segments[cPtNum2 - 1].m_ControlPoints[3].X;
	Gdiplus::REAL y2 = spline2.m_Segments[cPtNum2 - 1].m_ControlPoints[3].Y;
	gp.AddLine(x1, y1, x2, y2);

	pts[0].X = x2;
	pts[0].Y = y2;
	for(i = 0; i < cPtNum2; i ++)
	{
		pts[i * 3 + 1].X = spline2.m_Segments[cPtNum2 - 1 - i].m_ControlPoints[2].X;
		pts[i * 3 + 1].Y = spline2.m_Segments[cPtNum2 - 1 - i].m_ControlPoints[2].Y;
		pts[i * 3 + 2].X = spline2.m_Segments[cPtNum2 - 1 - i].m_ControlPoints[1].X;
		pts[i * 3 + 2].Y = spline2.m_Segments[cPtNum2 - 1 - i].m_ControlPoints[1].Y;
		pts[i * 3 + 3].X = spline2.m_Segments[cPtNum2 - 1 - i].m_ControlPoints[0].X;
		pts[i * 3 + 3].Y = spline2.m_Segments[cPtNum2 - 1 - i].m_ControlPoints[0].Y;
	}
	gp.AddBeziers(pts, cPtNum2 * 3 + 1);

	BYTE cr = (BYTE) (color >> 16) & 255;
	BYTE cg = (BYTE) (color >> 8) & 255;
	BYTE cb = (BYTE) color & 255;
	Gdiplus::SolidBrush brush(Gdiplus::Color(cr, cg, cb));
	Gdiplus::Pen pen(Gdiplus::Color(cr, cg, cb), 1.0f);
	gr.FillPath(&brush, &gp);
	gr.DrawPath(&pen, &gp);

	delete[] pts;
}

//画出两条尾、首相连的曲线
void CSketchVIR::AddCurve(Gdiplus::Graphics &gr, long color, const CShape &shape, const CShape &shape2)
{
	CCubicBezierSpline spline;
	std::vector<CPoint2> points;
	points.resize(shape.PointNum());
	for (int i = 0; i < shape.PointNum(); i++)
	{
		points[i].X = (float) shape.PointX(i);
		points[i].Y = (float) shape.PointY(i);
	}
	spline.Create_Catmull_Rom_Spline(points);

	CCubicBezierSpline spline2;
	std::vector<CPoint2> points2;
	points2.resize(shape2.PointNum());
	for (i = 0; i < shape2.PointNum(); i++)
	{
		points2[i].X = (float) shape2.PointX(i);
		points2[i].Y = (float) shape2.PointY(i);
	}
	spline2.Create_Catmull_Rom_Spline(points2);

	int cPtNum1, cPtNum2, cPtNum;
	Gdiplus::GraphicsPath gp;
	cPtNum1 = spline.NbSegment();
	cPtNum2 = spline2.NbSegment();
	cPtNum = (cPtNum1 > cPtNum2)?cPtNum1:cPtNum2;

	Gdiplus::PointF *pts = new Gdiplus::PointF[cPtNum * 3 + 1];
	pts[0].X = spline.m_Segments[0].m_ControlPoints[0].X;
	pts[0].Y = spline.m_Segments[0].m_ControlPoints[0].Y;
	for(i = 0;	i < cPtNum1; i ++)
	{
		pts[i * 3 + 1].X = spline.m_Segments[i].m_ControlPoints[1].X;
		pts[i * 3 + 1].Y = spline.m_Segments[i].m_ControlPoints[1].Y;
		pts[i * 3 + 2].X = spline.m_Segments[i].m_ControlPoints[2].X;
		pts[i * 3 + 2].Y = spline.m_Segments[i].m_ControlPoints[2].Y;
		pts[i * 3 + 3].X = spline.m_Segments[i].m_ControlPoints[3].X;
		pts[i * 3 + 3].Y = spline.m_Segments[i].m_ControlPoints[3].Y;
	}
	gp.AddBeziers(pts, cPtNum1 * 3 + 1);
	
	Gdiplus::REAL x1 = pts[cPtNum1 * 3].X;
	Gdiplus::REAL y1 = pts[cPtNum1 * 3].Y;
	Gdiplus::REAL x2 = spline2.m_Segments[0].m_ControlPoints[0].X;
	Gdiplus::REAL y2 = spline2.m_Segments[0].m_ControlPoints[0].Y;
	gp.AddLine(x1, y1, x2, y2);

	pts[0].X = x2;
	pts[0].Y = y2;
	for(i = 0; i < cPtNum2; i ++)
	{
		pts[i * 3 + 1].X = spline2.m_Segments[ i].m_ControlPoints[1].X;
		pts[i * 3 + 1].Y = spline2.m_Segments[ i].m_ControlPoints[1].Y;
		pts[i * 3 + 2].X = spline2.m_Segments[ i].m_ControlPoints[2].X;
		pts[i * 3 + 2].Y = spline2.m_Segments[ i].m_ControlPoints[2].Y;
		pts[i * 3 + 3].X = spline2.m_Segments[ i].m_ControlPoints[3].X;
		pts[i * 3 + 3].Y = spline2.m_Segments[ i].m_ControlPoints[3].Y;
	}
	gp.AddBeziers(pts, cPtNum2 * 3 + 1);

	BYTE cr = (BYTE) (color >> 16) & 255;
	BYTE cg = (BYTE) (color >> 8) & 255;
	BYTE cb = (BYTE) color & 255;
	Gdiplus::SolidBrush brush(Gdiplus::Color(cr, cg, cb));
	Gdiplus::Pen pen(Gdiplus::Color(cr, cg, cb), 1.0f);
	gr.FillPath(&brush, &gp);
	gr.DrawPath(&pen, &gp);

	delete[] pts;
}

//画出一条曲线
void CSketchVIR::AddCurve(Gdiplus::Graphics &gr, long color, const CShape &shape)
{
	CCubicBezierSpline spline;
	std::vector<CPoint2> points;
	
	points.resize(shape.PointNum());
	for (int i = 0; i < shape.PointNum(); i++)
	{
		points[i].X = (float) shape.PointX(i);
		points[i].Y = (float) shape.PointY(i);
	}
	spline.Create_Catmull_Rom_Spline(points);
	
	Gdiplus::PointF *pts = new Gdiplus::PointF[spline.NbSegment() * 3 + 1];
	pts[0].X = spline.m_Segments[0].m_ControlPoints[0].X;
	pts[0].Y = spline.m_Segments[0].m_ControlPoints[0].Y;
	for (i = 0; i < spline.NbSegment(); i++)
	{
		pts[i * 3 + 1].X = spline.m_Segments[i].m_ControlPoints[1].X;
		pts[i * 3 + 1].Y = spline.m_Segments[i].m_ControlPoints[1].Y;
        pts[i * 3 + 2].X = spline.m_Segments[i].m_ControlPoints[2].X;
		pts[i * 3 + 2].Y = spline.m_Segments[i].m_ControlPoints[2].Y;
		pts[i * 3 + 3].X = spline.m_Segments[i].m_ControlPoints[3].X;
		pts[i * 3 + 3].Y = spline.m_Segments[i].m_ControlPoints[3].Y;
	}

	Gdiplus::GraphicsPath gp;
	gp.AddBeziers(pts, spline.NbSegment() * 3 + 1);

    BYTE cr = (BYTE) (color >> 16) & 255;
	BYTE cg = (BYTE) (color >> 8) & 255;
	BYTE cb = (BYTE) color & 255;
	Gdiplus::SolidBrush brush(Gdiplus::Color(cr, cg, cb));
	gr.FillPath(&brush, &gp);

	delete[] pts;
}

bool CSketchVIR::RenderJpHair(Gdiplus::Graphics &gr, const CPartResult * lpPartRet,const CPartResultCtrl* pDrawCtrl)
{
	int     iStrokeSetNum;
	int     iStrokeNum;
	CShape    shapeNew;
	int     i,j;     // loopvar
	long fillColor;
	gr.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	iStrokeSetNum = lpPartRet->m_vStrokeSet.size();
	if (0 >= iStrokeSetNum)	return true;	

	//modified by lin begin, 2003-5     	
	//to render illustrator drawn templates 
	STROKETYPE type = lpPartRet->m_vStrokeSet[0][0].m_Type;
	if((type==STROKE_SIMPLELINE)||(type==STROKE_REGION_CUBICBEZIERS))
	{
		RenderBGStrokes(gr, lpPartRet, pDrawCtrl);
		return true;
	}

	
	for (i = 0; i < iStrokeSetNum; i++)
	{
		iStrokeNum = lpPartRet->m_vStrokeSet[i].size();
		if (0 > iStrokeNum)	continue;
        
		
		
		if (0 >= lpPartRet->m_vStrokeSet[i][0].m_ucLineWidth)
		{        // fill region
    		fillColor = GetRealColorValue(lpPartRet->m_vStrokeSet[i][0].m_ucLineColor);
			DrawFilledRegion(gr, fillColor, lpPartRet->m_vStrokeSet[i]);
		}
		else
		{        // draw stroke
			for (j = 0; j < lpPartRet->m_vStrokeSet[i].size(); j++)
			{
        		fillColor = GetRealColorValue(lpPartRet->m_vStrokeSet[i][j].m_ucLineColor);
				if (pDrawCtrl)
					DrawLineWithSktroke(gr, fillColor, lpPartRet->m_vStrokeSet[i][j],(pDrawCtrl->m_vStrokeCtrlSet[0][0]),lpPartRet->m_vStrokeSet[i][j].m_ucLineWidth);
				else
					DrawCurve(gr, fillColor, lpPartRet->m_vStrokeSet[i][j].m_Path,lpPartRet->m_vStrokeSet[i][j].m_ucLineWidth);
			}
		}
		
	}

	//modified by lin end, 2003-5
	return true;
}

bool CSketchVIR::DrawFilledRegion(Gdiplus::Graphics &gr, long color, const CStrokeSet &strokeSet)
{
	int	nStrokeNum;
	CCubicBezierSpline spline;
	std::vector<CPoint2> points;
	int cPtNum,i,k;
	CCubicBezierSpline spline2;
	std::vector<CPoint2> points2;
	Gdiplus::GraphicsPath gp;
	Gdiplus::PointF *pts = NULL;
	Gdiplus::REAL x0,y0,x1,y1,x2,y2;

	nStrokeNum = strokeSet.size();
	if (0 > nStrokeNum)
		return true;
	points.resize(strokeSet[0].m_Path.PointNum());
	for (i = 0; i < strokeSet[0].m_Path.PointNum(); i++)
	{
		points[i].X = (float) strokeSet[0].m_Path.PointX(i);
		points[i].Y = (float) strokeSet[0].m_Path.PointY(i);
	}
	spline.Create_Catmull_Rom_Spline(points);

	cPtNum = spline.NbSegment();

	pts = new Gdiplus::PointF[cPtNum * 3 + 1];
	x0 = pts[0].X = spline.m_Segments[0].m_ControlPoints[0].X;
	y0 = pts[0].Y = spline.m_Segments[0].m_ControlPoints[0].Y;
	x2 = spline.m_Segments[0].m_ControlPoints[0].X;
	y2 = spline.m_Segments[0].m_ControlPoints[0].Y;
	for(i = 0;	i < cPtNum; i ++)
	{
		pts[i * 3 + 1].X = spline.m_Segments[i].m_ControlPoints[1].X;
		pts[i * 3 + 1].Y = spline.m_Segments[i].m_ControlPoints[1].Y;
		pts[i * 3 + 2].X = spline.m_Segments[i].m_ControlPoints[2].X;
		pts[i * 3 + 2].Y = spline.m_Segments[i].m_ControlPoints[2].Y;
		pts[i * 3 + 3].X = spline.m_Segments[i].m_ControlPoints[3].X;
		pts[i * 3 + 3].Y = spline.m_Segments[i].m_ControlPoints[3].Y;
	}
	gp.AddBeziers(pts, cPtNum * 3 + 1);

	for (k = 1; k < nStrokeNum; k++)
	{
		x1 = pts[cPtNum * 3].X;
		y1 = pts[cPtNum * 3].Y;
		delete[] pts;

		points2.resize(strokeSet[k].m_Path.PointNum());
		for (i = 0; i < strokeSet[k].m_Path.PointNum(); i++)
		{
			points2[i].X = (float) strokeSet[k].m_Path.PointX(i);
			points2[i].Y = (float) strokeSet[k].m_Path.PointY(i);
		}
		spline2.Create_Catmull_Rom_Spline(points2);

		cPtNum = spline2.NbSegment();
		pts = new Gdiplus::PointF[cPtNum * 3 + 1];
		
		x2 = spline2.m_Segments[0].m_ControlPoints[0].X;
		y2 = spline2.m_Segments[0].m_ControlPoints[0].Y;
		gp.AddLine(x1, y1, x2, y2);

		pts[0].X = x2;
		pts[0].Y = y2;
		for(i = 0; i < cPtNum; i ++)
		{
			pts[i * 3 + 1].X = spline2.m_Segments[ i].m_ControlPoints[1].X;
			pts[i * 3 + 1].Y = spline2.m_Segments[ i].m_ControlPoints[1].Y;
			pts[i * 3 + 2].X = spline2.m_Segments[ i].m_ControlPoints[2].X;
			pts[i * 3 + 2].Y = spline2.m_Segments[ i].m_ControlPoints[2].Y;
			pts[i * 3 + 3].X = spline2.m_Segments[ i].m_ControlPoints[3].X;
			pts[i * 3 + 3].Y = spline2.m_Segments[ i].m_ControlPoints[3].Y;
		}
		gp.AddBeziers(pts, cPtNum * 3 + 1);
	}
//	gp.AddLine(x2, y2, x0, y0);

	delete[] pts;

	BYTE cr = (BYTE) (color >> 16) & 255;
	BYTE cg = (BYTE) (color >> 8) & 255;
	BYTE cb = (BYTE) color & 255;
	Gdiplus::SolidBrush brush(Gdiplus::Color(cr, cg, cb));
	Gdiplus::Pen pen(Gdiplus::Color(cr, cg, cb), 1.0f);
	gr.FillPath(&brush, &gp);
//	gr.DrawPath(&pen, &gp);
    return true;
}

bool CSketchVIR::DrawCurve(Gdiplus::Graphics &gr, long color, const CShape& shape,double width)
{
	if (0 > shape.PointNum())
		return false;

	CCubicBezierSpline spline;
	gr.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	Gdiplus::PointF * points;
	points = new Gdiplus::PointF[shape.PointNum()];

	for (int ipoint=0;ipoint<shape.PointNum();ipoint++)
	{
		points[ipoint].X = (Gdiplus::REAL)shape.PointX(ipoint);
		points[ipoint].Y = (Gdiplus::REAL)shape.PointY(ipoint);
	}

	BYTE cr = (BYTE) (color >> 16) & 255;
	BYTE cg = (BYTE) (color >> 8) & 255;
	BYTE cb = (BYTE) color & 255;
	Gdiplus::Pen pen(Gdiplus::Color(cr, cg, cb), (Gdiplus::REAL)width);
	gr.DrawCurve(&pen,points,shape.PointNum());
	delete []points;
	return true;
}

/*bool CSketchVIR::DrawCurve(Gdiplus::Graphics &gr, long color, const CShape& shape,double width)
{
	if (0 > shape.PointNum())
		return false;
	CCubicBezierSpline spline;
	gr.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	vector<CPoint2> points;

	points.resize(shape.PointNum());
	for (int ipoint=0;ipoint<shape.PointNum();ipoint++)
	{
		points[ipoint].X = shape.PointX(ipoint);
		points[ipoint].Y = shape.PointY(ipoint);
	}
	spline.Create_Catmull_Rom_Spline(points);

	BYTE cr = (BYTE) (color >> 16) & 255;
	BYTE cg = (BYTE) (color >> 8) & 255;
	BYTE cb = (BYTE) color & 255;
	Gdiplus::Pen pen(Gdiplus::Color(cr, cg, cb), width);
	for (int i=0;i<spline.NbSegment();i++)
	{
		CCubicBezier& seg = spline.m_Segments[i]; 
		gr.DrawBezier(&pen,seg.ControlPoint(0).X,seg.ControlPoint(0).Y,seg.ControlPoint(1).X,seg.ControlPoint(1).Y,seg.ControlPoint(2).X,seg.ControlPoint(2).Y,seg.ControlPoint(3).X,seg.ControlPoint(3).Y);
	}
	return true;
}
*/

bool CSketchVIR::RenderJpComplexRegion(Gdiplus::Graphics &gr, const CPartResult * lpPartRet)
{
	int	iStrokeSetNum;
	int	iStrokeNum;
	CShape	shapeNew;
	int	i,j,k;	// loopvar
	unsigned char	ucColor,ucFillColor;
	unsigned long	ulLineColor;
	Gdiplus::PointF *pCurve = NULL;
	int iPtNum = 0;
	int	iTotalPtNum = 0,iTotalPtIndex = 0;
    Gdiplus::GraphicsPath gp;
	bool	fFillRegion;

	fFillRegion = false;

	gr.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

	iStrokeSetNum = lpPartRet->m_vStrokeSet.size();
	if (0 > iStrokeSetNum)
		return true;
	for (i = 0; i < iStrokeSetNum; i++)
	{
		iStrokeNum = lpPartRet->m_vStrokeSet[i].size();
		if (0 > iStrokeNum)
			continue;
		ucColor = lpPartRet->m_vStrokeSet[i][0].m_ucLineColor;
		if (0 >= lpPartRet->m_vStrokeSet[i][0].m_ucLineWidth)
		{	// fill region
			iTotalPtNum = 0;
			for (j = 0; j < iStrokeNum; j++)
			{
				iPtNum = lpPartRet->m_vStrokeSet[i][j].m_Path.PointNum();
				iTotalPtNum += iPtNum;
			}
			pCurve = new Gdiplus::PointF[iTotalPtNum];
			iTotalPtIndex = 0;
			for (j = 0; j < iStrokeNum; j++)
			{
				iPtNum = lpPartRet->m_vStrokeSet[i][j].m_Path.PointNum();
				for (k = 0; k < iPtNum; k++)
				{
					pCurve[iTotalPtIndex].X = (float) lpPartRet->m_vStrokeSet[i][j].m_Path.PointX(k);
					pCurve[iTotalPtIndex].Y = (float) lpPartRet->m_vStrokeSet[i][j].m_Path.PointY(k);
					iTotalPtIndex ++;
				}
			}

			if (0 < iTotalPtNum)
			{
				if (!fFillRegion)
				{
					fFillRegion = true;
					ucFillColor = lpPartRet->m_vStrokeSet[i][0].m_ucLineColor;
				}
				gp.AddClosedCurve(pCurve, iTotalPtNum);
				delete []pCurve;
				pCurve = NULL;
			}
		}
		else
		{	// draw stroke
			for (j = 0; j < iStrokeNum; j++)
			{
				ulLineColor = GetRealColorValue(ucColor);
				DrawCurve(gr, ulLineColor, lpPartRet->m_vStrokeSet[i][j].m_Path,lpPartRet->m_vStrokeSet[i][j].m_ucLineWidth);
			}
		}
	}
	if (fFillRegion)
	{
        ulLineColor = GetRealColorValue(ucFillColor);
		//Gdiplus::SolidBrush brush(Gdiplus::Color(ucFillColor, ucFillColor, ucFillColor));
		Gdiplus::SolidBrush brush(Gdiplus::Color((BYTE)((ulLineColor >> 16) & 255), (BYTE)((ulLineColor >> 8) & 255), (BYTE)(ulLineColor & 255)));
		gr.FillPath(&brush, &gp);
	}
	return true;
}
// added by sunlei 2002.9.14 end

void CSketchVIR::DrawLineWithSktroke(Gdiplus::Graphics &gr, long color, const CShape &stroke, const CStrokeCtrl &control, double width)
{
	CStroke s;
	s.m_Type = STROKE_STROKE;
	s.m_ucLineWidth = 2;
	s.m_ucLineColor = 0;
	s.m_Path = stroke;

	DrawLineWithSktroke(gr, color, s, control, width);
}

void CSketchVIR::DrawStrokeSetWithSktroke(Gdiplus::Graphics &gr, long color, const std::vector<CShape>& strokeSet, const CStrokeCtrl &control, double width)
{
	for (int iStroke = 0; iStroke < strokeSet.size(); iStroke ++ )
	{
		DrawLineWithSktroke(gr, color, strokeSet[iStroke] , control, width);
	}
}
	
