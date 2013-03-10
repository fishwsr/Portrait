
#pragma once

#include "asmml2dsearchmodel.h"

class ASMMODEL_API CASMMLSearchModelBayesian :
	public CASMML2DSearchModel
{
public:
    CASMMLSearchModelBayesian(void);
    virtual ~CASMMLSearchModelBayesian(void);

	// Interface with FaceAlignmentObj
	void ResetShape() { SetAvgShape(); };
	void GetCurShape(CShape& shape){shape = SearchResult();};
	bool SearchIterative(int MaxSteps);
	bool IfConverge() { return true;};
	void SetConstrain(const double Var, std::vector<bool>& vbConstrained, const CShape& constrainShape) 
	{CASMMLSearchModel::SetConstrain(vbConstrained,constrainShape);m_ConstrainVar = Var;};
	
	void GetAvgShape(CShape& AvgShape);
    virtual void SetAvgShape(void);
	virtual void SetShape(const CShape& initshape);

	double ScaleX() {return m_fScaleX;};
	double ScaleY() {return m_fScaleY;};

public:
    virtual void UpdateShapeWithModel(void);
    virtual bool UpdateShapeWithImage(void);
	
protected:
	double m_fScaleX, m_fScaleY;
	int m_iSize;
	int SetEigenNumber();

	// EM UPDATION FOR MODEL
	bool UpdateShapeImage();

	void UpdateModelBayesian();
	void UpdateModelICM();
	void UpdateModelNMF();
	
	bool SearchConvergeBayesian(int MaxSteps,double thresh,int MinLevel);
	bool SearchBayesian(int MinLevel,int MaxLevel=LONG_MAX);
	bool SearchOneStepBayesian();
	bool SearchOneLevelBayesian();
	
	void InitShapeParam();
	void InitPosParam();
	
	// BASIC STEPS IN ECM UPDATES
	bool GetPosParamWithBayesian(int iNbEigen);
	bool GetShapeParamWithBayesian(int iNbEigen);

	// BASIC STEPS IN BAYESIAN UPDATES
	bool GetShapeAndPosBayesian(int iNbEigen,CShape& refshape,double norm);
	void GetRefShapeBayesian(CShape& refshape,double& norm,int iNbEigen);
	
	// BASIC STEPS IN NMF BAYESIAN UPDATES
	bool GetShapeAndPosNMF(CShape& refshape, double norm);
	void GetRefShapeNMF(CShape& refshape, double& norm);

	// linear constraints
	void SetTangentConstraint();

protected:
	CVisDMatrix m_TangentConstraint;
	bool m_fInhomogenousLikelihood;	
	CShape m_RefShape;
	
protected:
	CVisDMatrix m_B;
	CVisDVector m_b;
	CVisDMatrix m_ScalorMat;

	CVisDVector m_vDev;
	CShape m_ConstrainPoints;	

// For Bayesian Testing
	bool m_fTangentConstraint; // Test the effect of Tangent Constraint on/off
	bool m_fUseEM; // Test the effect of Tangent Constraint on/off	

	double m_ConstrainVar;
};

