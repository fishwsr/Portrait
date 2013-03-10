// SketchDefs.h: interface for the SketchDefs.
//
///////////////////////////////////////////////////////////////////////////

#ifndef __CSBASE_SKETCHDEFS_H__
#define __CSBASE_SKETCHDEFS_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//#import <msxml3.dll>

#include "CubicBezierSpline.h"

const int PART_FIRST    = 0;
const int PART_CONTOUR  = 0;
const int PART_HAIR     = 1;
const int PART_EAR      = 2;
const int PART_BROW     = 3;
const int PART_EYE      = 4;
const int PART_NOSE     = 5;
const int PART_MOUTH    = 6;
const int PART_TEMPLATE = 7;
const int PART_BGSTROKES = 8;
// added by sunlei 2002.9.25 begin
const int PART_JP1HAIR_B = 9;
const int PART_JP1HAIR_F = 10;
const int PART_JP1GLASS = 11;
const int PART_JP1BEARD = 12;
const int PART_LAST     = 12;
const int PART_COUNT    = 13;
// added by sunlei 2002.9.25 end

const int GEN_FIRST    = 0;
const int GEN_LAST    = 6;
const int GEN_COUNT    = 7;

const int TEMPLATE_FIRST    = 0;
const int TEMPLATE_HAIR     = 0;
const int TEMPLATE_GLASSES  = 1;
const int TEMPLATE_BEARD    = 2;
// added by sunlei 2002.9.27 begin
/*
// added by sunlei 2002.9.27 end
const int TEMPLATE_LAST     = 2;
const int TEMPLATE_COUNT    = 3;
// added by sunlei 2002.9.27 begin
*/
const int TEMPLATE_EYELASH  = 3;
const int TEMPLATE_LAST     = 3;
const int TEMPLATE_COUNT    = 4;
// added by sunlei 2002.9.27 end

const int EYE_FIRST         = 0;
const int EYE_BALL          = 0;
const int EYE_BRIGHT_POINT  = 1;
const int EYE_UP_LINE       = 2;
const int EYE_BELOW_LINE    = 3;
const int EYE_DOUBLE_LINE   = 4;
const int EYE_TEST_LINE     = 5;
const int EYE_TEST_LINE1    = 6;
const int EYE_EX_CLIP1      = 7;
const int EYE_EX_CLIP2      = 8;
const int EYE_DRAW_CTRL     = 9;
const int EYE_LAST          = 9;
const int EYE_COUNT         = 10;

const int EYE_DRAW_DOUBLE_LINE  = 0;
const int EYE_DRAW_BALL         = 1;
const int EYE_DRAW_BRIGHT_POINT = 2;
const int EYE_DRAW_EYE_LINE     = 3;
const int EYE_DRAW_TEST_LINE    = 4;
const int EYE_DRAW_TEST_LINE1   = 5;
const int EYE_DRAW_CTRL_COUNT   = 6;

const int DT_EYE_POS = 0;
const int DT_NOSE_POS = 1;
const int DT_MOUTH_POS = 2;
const int DT_BROW_POS = 3;
const int DT_CONTOUR_POS = 4;
const int DT_EAR_POS = 5;
const int DT_HAIR_POS = 6;
const int DT_CTRL_COUNT = 7;

enum FACEPART
{
    FP_LeftEye = 0,
    FP_RightEye = 1,
// added by sunlei 2002.9.27 begin
/*
// added by sunlei 2002.9.27 end
    FP_Mouth = 2
// added by sunlei 2002.9.27 begin
*/
    FP_Mouth = 2,
	FP_Eye = 3,
    FP_Nose = 4,
	FP_LeftBrow = 5,
	FP_RightBrow = 6,
	FP_Contour = 7
// added by sunlei 2002.9.27 end
};

enum DRAWTYPE
{
    DT_NOCHANGE = 0,
    DT_NODRAW = 1,
    DT_REPLACE = 2,
// added by sunlei 2002.9.27 begin
/*
// added by sunlei 2002.9.27 end
    DT_MODIFY = 3
// added by sunlei 2002.9.27 begin
*/
    DT_MODIFY = 3,
    DT_NODRAW_LEFT = 4,
    DT_NODRAW_RIGHT = 5,
    DT_REPLACE_LEFT = 6,
    DT_REPLACE_RIGHT = 7
// added by sunlei 2002.9.27 end
};

// added by sunlei 2002.9.27 begin
/*
// added by sunlei 2002.9.27 end
const int FACEPARTS_NUM = 3;
// added by sunlei 2002.9.27 begin
*/
const int FACEPARTS_NUM = 8;
// added by sunlei 2002.9.27 end

class CSBASE_API CStrokeCtrl
{
public:
    CStrokeCtrl(int PredefinedType = 1); 
    virtual ~CStrokeCtrl(void);

public:
    const CStrokeCtrl& operator=(const CStrokeCtrl& src);
    CStrokeCtrl& operator/=(double factor);

public:
    void Clear(void);

    double width1(double u) const;
    double width2(double u) const;

public:
    CCubicBezierSpline m_Splines[2];
};

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const CStrokeCtrl& sc);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, CStrokeCtrl& sc);


class CSBASE_API CStrokeCtrlSet : public std::vector<CStrokeCtrl>
{
public:
    CStrokeCtrlSet& operator/=(double factor);
public:
    void Clear(void);
};

class CSBASE_API CPartResultCtrl
{
public:
    CPartResultCtrl(void);
    virtual ~CPartResultCtrl(void);

public:
    CPartResultCtrl& operator/=(double factor);

public:
    void Clear(void);

public:
    std::vector<CStrokeCtrlSet> m_vStrokeCtrlSet;
};


typedef enum
{
	STROKE_REGION_CATMULLROM =0,
	STROKE_STROKE = 1,
	STROKE_SIMPLELINE = 100,
	STROKE_REGION_CUBICBEZIERS = 255
} STROKETYPE;

//
// stroketype	== STROKE_REGION_CATMULLROM	: Fill region, the points in stroke.m_path is the key points 
//							  of Catmull-Rom spline
//				== STROKE_REGION_CUBICBEZIERS : Fill region, the points in stroke.m_path is the controll 
//						  points of Cubic Bezier splines
//				== 1 ~ 99 : Draw stroke in predefined style and the width equals stroke.m_ucWidth
//              == 100 ~ 199 : Draw stroke in simpleline style and the width equals stroke.m_ucWidth
//


class CSBASE_API CStroke
{
public:
    CStroke(void);
    virtual ~CStroke(void);

public:
    const CStroke& operator=(const CStroke& src);
    CStroke& operator/=(double factor);
    void Translate(double x, double y);
	void Scale(double sX, double sY);
    void TranslateBall(double x, double y);

public:

public:
    void Clear(void);

public:
	STROKETYPE m_Type;
    unsigned char m_ucLineColor;
    unsigned char m_ucLineWidth;
    CShape m_Path;
};

CSBASE_API void DrawControlledWidthLine(const CCubicBezierSpline& spline, int num, CPoint2 *points, const CStrokeCtrl &control, double widthfactor);

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const CStroke& s);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, CStroke& s);

class CSBASE_API CStrokeSet : public std::vector<CStroke>
{
public:
    CStrokeSet& operator/=(double factor);
    void Translate(double x, double y);
	void Scale(double sX, double sY);
    void TranslateEye(double x, double y);

public:
    void AroundRect(double & left, double & top, double & right, double & bottom) const;

public:
    void Clear(void);
};

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const CStrokeSet& ss);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, CStrokeSet& ss);

class CSBASE_API CPartResult
{
public:
    CPartResult(void);
    virtual ~CPartResult(void);

public:
    void Clear(void);
    CPartResult& operator /=(double factor);
    void Translate(double x, double y);
    void TranslateEye(double x, double y);

public:
    std::vector<CStrokeSet> m_vStrokeSet;
};

class CSBASE_API CSketchResult
{
public:
    CSketchResult(void);
    virtual ~CSketchResult(void);

public:
    void AroundRect(double & left, double & top, double & right, double & bottom) const;

	CSketchResult& operator/=(double factor);
    void Translate(double x, double y);
    void Translate(const RECT & rect);

	// XML support
	/*
	CSBASE_API bool XMLLoad(const WCHAR *fname);
	CSBASE_API bool XMLSave(const WCHAR *fname);

	bool XMLLoadStrokeCtrl(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peRenderctrl, const char *role, CStrokeCtrl & sc);
	bool XMLLoadShape(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peShape, const char *role, CShape & shape);
	bool XMLLoadEye(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peFacepart, const char *role, CStrokeSet & ss);
	bool XMLLoadNose(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peFacepart, const char *role, CPartResult & pr);
	bool XMLLoadOther(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peFacepart, const char *role, CStrokeSet & ss);

	bool XMLLoadSpline(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peSpline, const char *role, CCubicBezierSpline & spline);
	bool XMLLoadLine(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peLine, const char *role, CStroke & stroke);
	bool XMLLoadOval(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peOval, const char *role, CStroke & stroke);

	bool XMLSaveStrokeCtrl(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peBaseface, const char *role, const CStrokeCtrl & sc);
	bool XMLSaveShape(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peBaseface, const char *role, const CShape & shape);
	bool XMLSaveEye(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peBaseface, const char *role, const CStrokeSet & ss);
	bool XMLSaveNose(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peBaseface, const char *role, const CPartResult & pr);
	bool XMLSaveOther(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peBaseface, const char *role, const CStrokeSet & ss);

	bool XMLSaveSpline(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peRenderctrl, const char *role, const CCubicBezierSpline & spline);
	bool XMLSaveLine(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peFacepart, const char *role, const CStroke & stroke);
	bool XMLSaveOval(MSXML2::IXMLDOMDocument2 * pXMLDoc, MSXML2::IXMLDOMElement * peFacepart, const char *role, const CStroke & stroke);
	*/
public:
    void Clear(void);

public:
	WCHAR			m_name[MAX_PATH];
	WCHAR			m_id[MAX_PATH];
	//char			m_renderstyle[256];
	WCHAR			m_linestyle[MAX_PATH];

    int m_nConfig;
    int m_nRenderStyle;
    int m_nHairCatID, m_nHairTempID, m_nGlassesCatID, m_nGlassesID, m_nBeardCatID, m_nBeardID;

    CPartResultCtrl m_defPartRetCtrl;
    std::vector<short>   m_DrawControl;
    CShape          m_shape;
    CPartResult     m_agPartRet[PART_COUNT];
    CPartResultCtrl m_agPartRetCtrl[PART_COUNT];
};

CSBASE_API void ApplyTransform(const CStroke& orgStroke, CStroke& desStroke, const CGeomTransform& geomTrans, const bool fNotInv = true, const bool fResample = false);
CSBASE_API void ApplyTransform(const CStrokeSet& orgStrokeSet, CStrokeSet& desStrokeSet, const CGeomTransform& geomTrans, const bool fNotInv = true, const bool fResample = false);

//modified by lin begin, 2003-5
CSBASE_API void Apply2DRTSTransform(CVisTransform4x4& transM, CStroke& DesStroke);
CSBASE_API void Apply2DRTSTransform(CVisTransform4x4& transM, CStrokeSet& DesStrokeSet);
CSBASE_API void Apply2DRTSTransform(CVisTransform4x4& transM, std::vector<CStrokeSet>& vecDesStrokeSet);
CSBASE_API void Apply2DRTSTransform(CVisTransform4x4& transM, CPartResult& PartResult);
//modified by lin end, 2003-5

CSBASE_API void SmoothCurve(CShape& shape, double m_dSmooth_Param1 = 0.5, double m_dSmooth_Param2 = 1.0);
CSBASE_API void SmoothCurveSet(std::vector<CShape>& vShape, double m_dSmooth_Param1 = 0.5, double m_dSmooth_Param2 = 1.0);

#endif // __CSBASE_SKETCHDEFS_H__
