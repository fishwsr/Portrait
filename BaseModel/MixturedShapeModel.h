// MixturedShapeModel.h: interface for the CMixturedShapeModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIXTUREDSHAPEMODEL_H__83D3E408_54AD_4763_AEA8_336A3FB77AA0__INCLUDED_)
#define AFX_MIXTUREDSHAPEMODEL_H__83D3E408_54AD_4763_AEA8_336A3FB77AA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Shape.h"
#include "PPCAModel.h"
#include "MixturePPCA.h"

class CShapeModelBuilder;

class BASEMODEL_API CPPCAShapeModel
{
public:
	void SetModel(const CShape& avgShape, const CPPCAModel& model);

    void GetShapePos(double& Tx, double& Ty, double& s, double& theta, const CShape& srcShape, CShape& normaledShape) const;
	void ApplyShapePos(double& Tx, double& Ty, double& s, double& theta, CShape& desShape, const CShape& normaledShape) const;
	double MAPEstimate(const CShape& srcShape, const CVisDVector& srcVar, double& Tx, double& Ty, double& s, double& theta, CShape& refshape) const;

	friend COutputDataFile& Output(COutputDataFile& fs, const CPPCAShapeModel& model);
	friend CInputDataFile& Input(CInputDataFile& fs, CPPCAShapeModel& model);
	friend class CShapeModelBuilder;
	
protected:
	void MAPEstimatePos(const CShape& srcShape, const CVisDVector& srcVar, double& Tx, double& Ty, double& s, double& theta, CShape& refshape) const;
	double MAPEstimateRefShape(const CShape& srcShape, const CVisDVector& srcVar, double& Tx, double& Ty, double& s, double& theta, CShape& refshape) const;

    // unchanged data
    CShape m_AvgShape;
	CPPCAModel m_SModel;
};

class BASEMODEL_API CMixturedShapeModel
{
public:
    CMixturedShapeModel(void);
    virtual ~CMixturedShapeModel(void);
	friend class CShapeModelBuilder;
	friend COutputDataFile& Output(COutputDataFile& fs, const CMixturedShapeModel& model);
	friend CInputDataFile& Input(CInputDataFile& fs, CMixturedShapeModel& model);

public:
    CShape& RefShape(void) { return m_AvgShape; }
	double LogP(const CShape& shape) const;

//modified by lin begin, 2003-3
public: //for trans realistic nose to jp style
//modified by lin end, 2003-3

    void GetShapePos(double& Tx, double& Ty, double& s, double& theta, const CShape& srcShape, CShape& normaledShape) const;
	void ApplyShapePos(double& Tx, double& Ty, double& s, double& theta, CShape& desShape, const CShape& normaledShape) const;
	double MAPEstimate(const CShape& srcShape, const CVisDVector& srcVar, double& Tx, double& Ty, double& s, double& theta, CShape& shape) const;

protected:
    // unchanged data
	void MAPEstimatePos(const CShape& srcShape, const CVisDVector& srcVar, double& Tx, double& Ty, double& s, double& theta, CShape& refshape) const;
	double MAPEstimateRefShape(const CShape& srcShape, const CVisDVector& srcVar, double& Tx, double& Ty, double& s, double& theta, CShape& refshape) const;

    CShape m_AvgShape;
	CMixturePPCA m_SModel;
};

BASEMODEL_API COutputDataFile& operator<<(COutputDataFile& fs, const CPPCAShapeModel& model);
BASEMODEL_API CInputDataFile& operator>>(CInputDataFile& fs, CPPCAShapeModel& model);

BASEMODEL_API COutputDataFile& operator<<(COutputDataFile& fs, const CMixturedShapeModel& model);
BASEMODEL_API CInputDataFile& operator>>(CInputDataFile& fs, CMixturedShapeModel& model);


#endif // !defined(AFX_MIXTUREDSHAPEMODEL_H__83D3E408_54AD_4763_AEA8_336A3FB77AA0__INCLUDED_)
