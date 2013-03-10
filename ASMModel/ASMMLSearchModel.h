
// ASMMLSearchModel.h: interface for the CASMMLSearchModel class.
//
//////////////////////////////////////////////////////////////////////


#ifndef __ASMMODEL_MLSEARCHMODEL_H__
#define __ASMMODEL_MLSEARCHMODEL_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "ASMPCALocalStruct.h"
//#include "ConnectedModel.h"

class CASMModelBuilder;

class ASMMODEL_API CASMMLSearchModel : public CShapeModel
{
public:
    CASMMLSearchModel(void);
    virtual ~CASMMLSearchModel(void);
	
	friend class CASMModelBuilder;
	friend COutputDataFile& Output(COutputDataFile& ifs, const CASMMLSearchModel& model);
	friend CInputDataFile& Input(CInputDataFile& ifs, CASMMLSearchModel& model);

public:
    void SetModelParam(int totlevel = 4, int localstructlength = 3, int withoutalign = 0);
    void SetSearchParam(int searchlength = 2, int stepsperlevel = 5, double convergethreshold = 0.9);

    virtual void SetAvgShape(void);
    virtual void SetShape(const CShape& initshape);
    void SetSearchImage(CVisGrayByteImage* pSrcImg);
	CVisGrayByteImage*	SearchImage() { return m_pSearchImg;}

    bool SearchConverge(int MaxSteps, double thresh, int MinLevel);
    bool Search(int MinLevel = 0, int MaxLevel = LONG_MAX);

    virtual void UpdateShapeWithModel(void);
    virtual bool UpdateShapeWithImage(void);

    CShape& SearchResult(void){ return m_SearchShape; }
    CASMPCALocalStruct& LocalStruct(int iPoint, int iLevel) { return m_vLocalStruct[iPoint * m_iTotLevel + iLevel]; }
    CConnectedModel& ConnectModel() { return m_ConnectModel; }
	CMixturedShapeModel& ShapeModel() { return m_ShapeModel; }
   
protected:
	
	void Clear(void);
	//CShape& SearchResult(void) { return m_SearchShape; }

    bool SearchOneLevel(void);
    bool SearchOneStep(void);
    bool SearchTopLevelOneStep(void);

public: //modified by lin, 2003-3
    void ConstrainModel(CVisDVector& eigenparam, double factor = 3.0, int starteigen = 0);
    void SetCurrLevel(int iCurLevel);

protected:
    void GetShapeNormal(void);
    virtual bool SearchOnePoint(int iPoint, double& desx, double& desy) = 0;

protected:
    // SHAPE DATA
    CConnectedModel m_ConnectModel;
    double m_SInTraining, m_TxInTraining, m_TyInTraining, m_ThetaInTraining; 

	CMixturedShapeModel m_ShapeModel;
	//CPPCAShapeModel m_ShapeModel;

    int m_LoacalStructType; // 0 - grey    1 - gray level

    // LOCAL STRUCT
    int m_iLocalStructLength;
    std::vector<CASMPCALocalStruct> m_vLocalStruct; // no change

    // MULTI LEVEL CONTROL
    int m_iCurLevel, m_iTotLevel, m_iCurLevelScale;
    CVisGrayPyramid *m_pImagePyramid;
    
    // SEARCH CONTROL
    int m_iSearchLength, m_iMaxStepPerLevel; 
    double m_fConvergeThreshold;

    // SEARCH DATA
    CVisGrayByteImage *m_pSearchImg;
    CShape m_SearchShape; // current search result
    CShape m_ShapeNormal; // the normal vec of current shape
    double m_Tx, m_Ty, m_S, m_Theta;
    CVisDVector m_EigenParam;	// for ppca and pca models
	CVisDVector m_NMFEncoding;  // for nmf model

public:
	void NeedConstraint(bool fb=true) { m_bNeedConstrain = fb; } 
    void SetConstrain(const std::vector<bool>& bConstrained, const CShape& constrainShape);
	void UpdateConstrainShape(const CShape& constrainShape);
    void ClearConstrain(void);
	void EnableConstrain(bool b=true);

	int m_SearchMode;
	
protected:
	bool m_bNeedConstrain;
	std::vector<bool> m_bConstrained;
	CShape m_ConstrainShape;
};

ASMMODEL_API COutputDataFile& operator<<(COutputDataFile& fs, const CASMMLSearchModel& model);
ASMMODEL_API CInputDataFile& operator>>(CInputDataFile& fs, CASMMLSearchModel& model);

#endif // __ASMMODEL_MLSEARCHMODEL_H__
