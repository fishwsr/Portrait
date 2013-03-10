
// Shape.h: interface for the CShape class.
//
///////////////////////////////////////////////////////////////////////////

#ifndef __CSBASE_SHAPE_H__
#define __CSBASE_SHAPE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CPoint2;
class CSBASE_API CShape
{
public:
    CShape(void);
	virtual ~CShape(void);

public:
	friend CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const CShape& shape);
	friend CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, const CShape& shape);

    void Clear(void);

	CShape& operator/=(double factor);
	CShape& operator+=(const CShape &shape);
	bool operator==(const CShape& shape) const;

    void Scale(double s);
    void Scale(double sX, double sY);
    void ScaleAroundCenter(double s);
	void ScaleAroundCenter(double cx, double cy, double s);

    void Translate(double dx, double dy);
	void Rotation(double theta, double cx=0., double cy=0.);
	void RotateAroundCenter(double theta);

    void CenterPoint(double& x, double& y) const;
    void CenterPoint(double& x, double& y, int start, int count) const;

    void AroundRect(double& left, double& top, double& right, double& bottom) const;

	double Length(double interval=0.2) const;
    double Distance(const CShape& refShape) const;
    double PointToPointAvgDistance(const CShape& refShape) const;

    int NearestPoint(double x, double y) const;

    void Resize(int size) { m_PointNum = size; m_Points.Resize(size * 2); }
    short PointNum(void) const { return m_PointNum; }
    const CVisDVector& Points(void) const { return m_Points; }
    CVisDVector& Points(void) { return m_Points; }
    const double& PointX(int index) const { return m_Points[index * 2]; }
    double& PointX(int index) { return m_Points[index * 2]; }
    const double& PointY(int index) const { return m_Points[index * 2 + 1]; }
    double& PointY(int index) { return m_Points[index * 2 + 1]; }

	CPoint2 GetPoint(int index) const;
	void SetPoint(int index, const CPoint2& p2);

	// edited by zijian 2005-08-08, begin
	void AlignTangent(const CShape& refshape);
	void AlignTangent(const CShape &refshape,double& Tx,double& Ty,double& s,double& theta);

	void AlignTangent(const CShape& refshape, std::vector<bool>& vbConstrained, double& Tx, double& Ty, double& s, double& theta);
	void AlignTangent(const CShape& refshape, std::vector<bool>& vbConstrained, double& cx, double& cy, double& Tx, double& Ty, double& s, double& theta);
	void AlignTangent(const CShape& refshape, std::vector<bool>& vbConstrained);

	void AlignTP(const CShape& refshape, std::vector<bool>& vbConstrained);
	void AlignTP(const CShape& refshape, std::vector<bool>& vbConstrained, double& cx, double& cy, double& Tx, double& Ty, double& s, double& theta);

	void FlipHorizontal(double cx);
	void FlipVertical(double cy);
	// edited by zijian 2005-08-08, end

	double Norm2();
	void Normalize();

	const CShape& operator=(const CShape& ref);

protected:
    short m_PointNum;
    CVisDVector m_Points;
};

class CSBASE_API CShapeSet : public std::vector<CShape>
{
public:
    CShapeSet(void);
    virtual ~CShapeSet(void);
	void Add(const CShapeSet& ss);	
	void AroundRect(double &left, double& top, double& right, double& bottom);

public:
    CShapeSet& operator/=(double factor);

    void Scale(double s);
    void Scale(double sX, double sY);
    void ScaleAroundCenter(double s);
	
	void ScaleAroundCenter(double cx, double cy, double s);

    void Translate(double dx, double dy);
	void Rotation(double theta);
	void RotateAroundCenter(double theta);

    void CenterPoint(double& x, double& y);

	void Rank(bool descent=true);

	int PointNum() const;

	bool MeanShape(CShape& shape) const;
};

class CConnectedModel;

CSBASE_API void SmoothShape(CShape& shape, CConnectedModel& conModel, double m_dSmooth_Param1 = 0.5, double m_dSmooth_Param2 = 1.0);
CSBASE_API void shape2shapeset(const CShape& shape, CShapeSet& ss, const CConnectedModel& conModel);
CSBASE_API void shapeset2shape(const CShapeSet& ss, CShape& shape, CConnectedModel& conModel);

CSBASE_API void ResampleCurve(const CShape& srcCurve, CShape& dstCurve, int nStepByPixels);
CSBASE_API void ResampleShape(const CShape& orgshape, CShape& dstshape, int nPointNum);
CSBASE_API void ResampleShape(const CShape& srcShape, const CConnectedModel& srcConModel, CShape& dstShape, CConnectedModel& dstConModel, double fSampleStep);

CSBASE_API void GetSubShape(const CShape& orgshape, int orgindex, CShape& subshape, int subindex, int number);
CSBASE_API void GetSubShape(const CShape& orgshape, CShape& subshape, int number, const int * index);
CSBASE_API void PutSubShape(CShape& desshape, const CShape& subshape, int number, int startindex);
CSBASE_API void Combine2Shapes(const CShape& srcShape1, const CShape& srcShape2, CShape &desShape);

CSBASE_API void POINTS2Shape(PPOINT lpPoints, int count, CShape& shape);

CSBASE_API void GetResample2DRTSMatrix(CVisTransform4x4 &destM, CShape &srcShape, CShape &destShape);
CSBASE_API void GetTrans2DRTSMatrix(CVisTransform4x4 &destM, CShape &srcShape, CShape &destShape);
CSBASE_API void Apply2DRTSTransform(CVisTransform4x4 &transM, const CShape &srcShape, CShape &destShape);
CSBASE_API void Apply2DRTSTransform(CVisTransform4x4 &transM, const CShapeSet &srcShapeSet, CShapeSet &destShape);
CSBASE_API void Apply2DRTSTransform(CVisTransform4x4 &transM, CShape &shape);

CSBASE_API CVisTransform4x4 GetTrans2DTMatrix(double transx, double transy);
CSBASE_API CVisTransform4x4 GetTrans2DRSMatrix(double scale, double theta);
CSBASE_API CVisTransform4x4 GetTrans2DRSMatrix2(double sx, double sy, double theta);

//CSBASE_API void GetTrans2DRTSMatrix(CVisTransform4x4 &destM, double cxorg, double cyorg, double theta, double scale, double cxdes, double cydes);

//modified by lin begin, 2003-3
CSBASE_API void Apply2DRTSTransform(CVisTransform4x4& transM,double inx,double iny,double& outx,double& outy);
//modified by lin end

CSBASE_API void DrawShape(HDC hdc, CShape& path, bool fClosed = false);

//modified by lin begin, 2003-4-20
CSBASE_API void Shape2POINTS(CShape& shape, PPOINT lpPoints, bool fClosed = false);
CSBASE_API void DrawMaskReg(CShape& contour, CVisGrayByteImage& MaskImg);
//modified by lin end, 2003-4-20
CSBASE_API void GetConvexMaskImage(CShape& srcShape, int width, int height, CVisGrayByteImage& maskImg, int extraWidth = 0);


CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const CShape& shape);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, CShape& shape);

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const std::vector<CShape>& shape);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, std::vector<CShape>& shape);


#endif // __CSBASE_SHAPE_H__
