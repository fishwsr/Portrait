// Sketch.h: interface for the CSketch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKETCH_H__A569F982_7BCA_4ACC_A739_DA389976EB00__INCLUDED_)
#define AFX_SKETCH_H__A569F982_7BCA_4ACC_A739_DA389976EB00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DRAWINGTYPE_SPLINE 0
#define DRAWINGTYPE_SEGMENT 1

#include "ConnectedModel.h"

class CSBASE_API CPatchGeomParameter
{
public:
	CPatchGeomParameter(double fCx=0., double fCy=0., double fTheta=0., double fHalfSize=1., double fLength=1.);
	CPatchGeomParameter(const CPatchGeomParameter& ref);
	const CPatchGeomParameter& operator=(const CPatchGeomParameter& ref);

	void GetTransform4x4(CVisTransform4x4 &trans);
	void GetInversedTransform4x4(CVisTransform4x4 &transInv);

	double m_fCx;
	double m_fCy;
	double m_fHalfSize, m_fLength;
	double m_fTheta;
};

CSBASE_API COutputDataFile& operator<<(COutputDataFile& fs, const CPatchGeomParameter &param);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& fs, CPatchGeomParameter &param);

enum ETextureParamType
{
	eTexOriginal=0, // Store all pixels
	eTexProfile=1,  // Store only profile along normal direction
	eTexCluster=2,  // Store Cluster index
	eTexMixppca=3   // Store Cluster index and Encodings
};

class CSBASE_API CPatchTextureParameter
{
public:	
	CPatchTextureParameter(int nMethod=eTexOriginal);
	CPatchTextureParameter(const CPatchTextureParameter& ref);
	const CPatchTextureParameter& operator=(const CPatchTextureParameter& ref);

	int	m_nMethod;

	// PixelValue = (m_data + m_fMean) * m_fVar;
	float m_fMean;
	float m_fVar;

	// eTexOriginal: length of the patch
	// eTexProfile: 1
	// eTexCluster: 1
	// eTexMixppca: number of encodings
	int	m_nLineNum;		

	CVisDVector m_data;
};

CSBASE_API COutputDataFile& operator<<(COutputDataFile& fs, const CPatchTextureParameter &param);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& fs, CPatchTextureParameter &param);

class CSBASE_API CSketchPatch
{
public:

	CSketchPatch(void);
	CSketchPatch(int iType, CPatchGeomParameter &geomParam, double weight);
	virtual ~CSketchPatch(void);
	
	const CSketchPatch& operator=(const CSketchPatch &ref);

	int&					GetType() {return m_iType;}
	double&					GetWeight() { return m_dWeight; }
	CPatchGeomParameter&	GetGeomParam() { return m_geomParam; }
	CPatchTextureParameter& GetTextureParam() { return m_textParam; }

	CPoint2		GetPoint();
	void		SetPoint(CPoint2& p2);

	double&		X(){ return m_geomParam.m_fCx; }
	double&		Y(){ return m_geomParam.m_fCy; }
	double&		Theta() { return m_geomParam.m_fTheta; }
	double&		Length() { return m_geomParam.m_fLength; }
	double&		HalfSize() { return m_geomParam.m_fHalfSize; }
	double		Width() { return m_geomParam.m_fHalfSize*2.+1.; }

	double AngleToPatch(CSketchPatch &patch);
	double CenterDistTo(CSketchPatch &patch);
	double EndDistTo(CSketchPatch &patch);

	friend class CSketch;
	friend class CSketchSet;
	friend COutputDataFile& Output(COutputDataFile& fs, const CSketchPatch &sketchPatch);
	friend CInputDataFile& Input(CInputDataFile& fs, CSketchPatch &sketchPatch);

protected:
	
	CPatchGeomParameter		m_geomParam;
	CPatchTextureParameter	m_textParam;
	double m_dWeight;	
	int m_iType;			
};

CSBASE_API COutputDataFile& operator<<(COutputDataFile& fs, const CSketchPatch &sketchPatch);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& fs, CSketchPatch &sketchPatch);

class CSBASE_API CSketch : public std::vector<CSketchPatch*>
{
public:

	CSketch(void);
	virtual ~CSketch(void);
	CSketch(const CSketch &sketch);
	CSketch(std::vector<CSketchPatch> &vPatch, int iType=0, double fWeight=1.);

	const CSketch& operator=(const CSketch& s);

	void Clear();
	void Resize(int nPatchNum);
	bool Delete(int iPatch);
	bool Insert(int iPatch, CSketchPatch& patch);
    const int NPatchNum() const { return size(); }

	int&    GetType() { return m_iType; }	
	double&	GetWeight() { return m_dWeight; }
	
	CSketchPatch*	GetPatchPtr(int iPatch);
	CPoint2			GetPoint(int iPatch);
	void			SetPoint(int iPatch, CPoint2& p2);
/*
	double&			PointX(int iPatch);
	double&			PointY(int iPatch);
	double&			Theta(int iPatch);
	double&			HalfSize(int iPatch);
	double&			Length(int iPatch);
	double			Width(int iPatch);
*/	
	void Translate(double dx, double dy);
	void Scale(double s);
	void Rotate(double theta);	

	friend COutputDataFile& Output(COutputDataFile& fs, const CSketch &sketch);
	friend CInputDataFile& Input(CInputDataFile& fs, CSketch &sketch);

protected:
	int m_iType;
	double m_dWeight;
};

CSBASE_API COutputDataFile& operator<<(COutputDataFile& fs, const CSketch &sketch);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& fs, CSketch &sketch);

class CSBASE_API CSketchSet : public std::vector<CSketch*>
{
public:
	CSketchSet(void);
	virtual ~CSketchSet(void);
	void Clear();

	const CSketchSet& operator=(const CSketchSet &ss);

	void		Resize(int nSketchNum);
	bool		Insert(int iSketch, CSketch& sketch);
	bool		Delete(int iSketch);
	const int   NSketchNum() const { return size(); }
	const int	NPatchNum() const ;
	CSketch*	GetSketchPtr(int iSketch);
	int&		GetType() { return m_iType;}	// type of the sketchset, serve as mode of SS

	void Translate(double dx, double dy);
	void Scale(double s);
	void Rotate(double theta);	

	friend COutputDataFile& Output(COutputDataFile& fs, const CSketchSet &ss);
	friend CInputDataFile& Input(CInputDataFile& fs, CSketchSet &ss);

protected:
	int	m_iType;
};

CSBASE_API COutputDataFile& operator<<(COutputDataFile& fs, const CSketchSet &ss);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& fs, CSketchSet &ss);

double CSBASE_API DistSketchToPatch(CSketch &sketch, CSketchPatch &patch, CPoint2 &nearestPoint2, int &iPatch, float &theta);

void CSBASE_API DrawSketchCurve(CVisRGBAByteImage &image, COLORREF color=RGB(0,0,0), float width=1., int iDrawType=DRAWINGTYPE_SPLINE);
void CSBASE_API DrawSketchCurve(HDC hdc, COLORREF color=RGB(0,0,0), float width=1., int iDrawType=DRAWINGTYPE_SPLINE);
void CSBASE_API DrawSketchWithStroke(CVisRGBAByteImage &image, COLORREF color=RGB(0,0,0), float width=1.);
void CSBASE_API DrawSketchWithStroke(HDC hdc, COLORREF color=RGB(0,0,0), float width=1.);

void CSBASE_API Shape2Sketch(CShape &shape, CSketch &sketch);
void CSBASE_API Sketch2Shape(CSketch &sketch, CShape &shape);

void CSBASE_API ShapeSet2SketchSet(CShapeSet &shapeset, CSketchSet &sketchset);
void CSBASE_API SketchSet2ShapeSet(CSketchSet &sketchset, CShapeSet &shapeset);

void CSBASE_API Shape2SketchSet(CShape &shape, CSketchSet &sketchset, const CConnectedModel* pConModel=NULL);
void CSBASE_API SketchSet2Shape(CSketchSet &sketchset, CShape &shape, CConnectedModel& conModel);

void CSBASE_API SketchToBezierSpline(CSketch &sketch, CCubicBezierSpline &spline);
void CSBASE_API BezierSplineToSketch(CCubicBezierSpline& spline, CSketch &sketch);

#endif // !defined(AFX_SKETCH_H__A569F982_7BCA_4ACC_A739_DA389976EB00__INCLUDED_)
