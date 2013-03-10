
// ShapeModel.h: interface for the CShapeModel class.
//
///////////////////////////////////////////////////////////////////////////


#ifndef __BASEMODEL_SHAPEMODEL_H__
#define __BASEMODEL_SHAPEMODEL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NMFModel.h"
#include "PCA2Model.h"

enum EDataAnalysisMethod{ePCA=0, eNMF=1, ePPCA=2};

class CShapeModelBuilder;

class BASEMODEL_API CShapeModel
{
public:
    CShapeModel(void);
    virtual ~CShapeModel(void);
	friend class CShapeModelBuilder;
	friend COutputDataFile& Output(COutputDataFile& fs, const CShapeModel& model);
	friend CInputDataFile& Input(CInputDataFile& fs, CShapeModel& model);

public:
	CShape& RefShape(void) { return m_AvgShape; }
	CPCA2Model& SModel(){ return m_PCA2Model; }
	CNMFModel& NMFModel() { return m_NMFModel; }
	
	double LogP(const CShape& shape) const;

//modified by lin begin, 2003-3
public: //for trans realistic nose to jp style
//modified by lin end, 2003-3

// modified by zijian begin 2006-6
    void GenerateShapeWithPos(double Tx, double Ty, double s, double theta, CVisDVector& param, CShape& desshape, EDataAnalysisMethod eMethod=ePCA);
    void GenerateShape(CVisDVector& param, CShape& desshape, EDataAnalysisMethod eMethod=ePCA);

    void GetShapeParamAndPos(double& Tx, double& Ty, double& s, double& theta, CVisDVector& param, const CShape& srcshape, EDataAnalysisMethod eMethod=ePCA);
    void GetShapeParam(CVisDVector& param, const CShape& srcshape, EDataAnalysisMethod eMethod=ePCA);

	void GetShapePos(double& Tx, double& Ty, double& s, double& theta, const CShape& srcShape, CShape& normaledShape) const;
	static void ApplyShapePos(double& Tx, double& Ty, double& s, double& theta, CShape& desShape, const CShape& normaledShape);
// modified by zijian end, 2006-6
	
	void GetShapeBase(int iEigen, int nVarChange, CShape &shape); // only for PCA model
	
protected:
    // unchanged data
    short m_ModelWithoutAlign;
    CShape m_AvgShape;

    CPCA2Model m_PCA2Model;
	CNMFModel m_NMFModel;
	CPPCAModel m_PPCAModel;
};

BASEMODEL_API COutputDataFile& operator<<(COutputDataFile& fs, const CShapeModel& model);
BASEMODEL_API CInputDataFile& operator>>(CInputDataFile& fs, CShapeModel& model);


#endif // __BASEMODEL_SHAPEMODEL_H__
