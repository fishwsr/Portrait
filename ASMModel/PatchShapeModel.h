// PatchShapeModel.h: interface for the CPatchShapeModel class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning(disable:4786)

#if !defined(AFX_PATCHSHAPEMODEL_H__8CF98C05_F02C_413A_9F0A_A5F6C8694B30__INCLUDED_)
#define AFX_PATCHSHAPEMODEL_H__8CF98C05_F02C_413A_9F0A_A5F6C8694B30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
class ASMMODEL_API CPatchCurveModel : public CPatchModel
{
public:
	CPatchCurveModel();

	CPatchCurveModel(const CPatchCurveModel& ref);
	const CPatchCurveModel& operator=(const CPatchCurveModel& ref);

	void Init(const SIZE PatchHalfSize, int maskType = 1);
	void InitPatchGeomParameter(std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CShape>& vCurves) const;

	void GetPatchImage(const CVisGrayByteImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, std::vector<CVisGrayByteImage>& vPatchData) const; 
	void PutPatchImage(CVisGrayByteImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisGrayByteImage>& vPatchData) const; 
	void PutPatchDataWithMask(CVisGrayByteImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisGrayByteImage>& vPatchData) const; 

	void GetPatchData(const CVisGrayByteImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, std::vector<CVisDVector>& vPatchData) const; 
	void PutPatchData(CVisGrayByteImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisDVector>& vPatchData) const; 
	void PutPatchDataWithMask(CVisGrayByteImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisDVector>& vPatchData) const; 

	void GetPatchData(const CVisGrayFloatImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, std::vector<CVisDVector>& vPatchData) const; 
	void PutPatchData(CVisGrayFloatImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisDVector>& vPatchData) const; 
	void PutPatchDataWithMask(CVisGrayFloatImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisDVector>& vPatchData) const; 

	SIZE PatchHalfSize() const {return m_PatchHalfSize;};

	friend COutputDataFile& Output(COutputDataFile& fs, const CPatchCurveModel& m);
	friend CInputDataFile& Input(CInputDataFile& fs, CPatchCurveModel& m);

protected:
	virtual CVisGrayByteImage * GetMaskImg(int type = 0) const; 

	SIZE m_PatchHalfSize;
	// MaskType  = 0 : no patch mask ; MaskType = 1 : linear reduced mask  
	int m_MaskType;
};

// Get/Put patches from/to image
class ASMMODEL_API CPatchShapeModel : public CPatchModel 
{
public:
	CPatchShapeModel();
	virtual ~CPatchShapeModel();
	
	CPatchShapeModel(const CPatchShapeModel& ref);
	const CPatchShapeModel& operator=(const CPatchShapeModel& ref);

	void Init(const CConnectedModel& ConnectedModel, const std::vector<SIZE> vPatchHalfSize, const CShape& refShape, double relSize);
	
	void InitPatchGeomParameter(std::vector<CLocalPatchGeomParameter>& geomparam, const CShape& shape) const;
	void GetShapeFromParameter(const std::vector<CLocalPatchGeomParameter>& geomparam, CShape& shape) const;

	void GetPatchData(const CVisGrayByteImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, std::vector<CVisDVector>& vPatchData) const; 
	void PutPatchData(CVisGrayByteImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisDVector>& vPatchData) const; 
	void PutPatchDataWithMask(CVisGrayByteImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisDVector>& vPatchData) const; 
	void GetPatchData(const CVisGrayFloatImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, std::vector<CVisDVector>& vPatchData) const; 
	void PutPatchData(CVisGrayFloatImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisDVector>& vPatchData) const; 
	void PutPatchDataWithMask(CVisGrayFloatImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisDVector>& vPatchData) const; 

	int NPatch() const;

	friend COutputDataFile& Output(COutputDataFile& fs, const CPatchShapeModel& m);
	friend CInputDataFile& Input(CInputDataFile& fs, CPatchShapeModel& m);

	SIZE PatchHalfSize(int index) const {return m_vPatchHalfSize[index];};
	
	CVisTransform4x4 GetLocalPatchTrans(int index, const std::vector<CLocalPatchGeomParameter>& geomparam) const;
	const CConnectedModel& ConnectedModel() const {return m_ConnectedModel; }; 

protected:
	virtual CVisGrayByteImage * GetMaskImg(int index, int type = 0) const; 
	void CPatchShapeModel::GetShapeNormal(const CShape& shape, CShape& shapeNormal) const;

	CConnectedModel m_ConnectedModel;
	std::vector<SIZE> m_vPatchHalfSize;
	
	// To determine the shape size
	double m_fRelativeShapeSize;
	CShape m_RefShape;

	// MaskType  = 0 : no patch mask ; MaskType = 1 : linear reduced mask  
	int m_MaskType;
};

ASMMODEL_API COutputDataFile& operator<<(COutputDataFile& fs, const CPatchShapeModel& m);
ASMMODEL_API CInputDataFile& operator>>(CInputDataFile& fs, CPatchShapeModel& m);

ASMMODEL_API COutputDataFile& operator<<(COutputDataFile& fs, const CPatchCurveModel& m);
ASMMODEL_API CInputDataFile& operator>>(CInputDataFile& fs, CPatchCurveModel& m);
*/
#endif // !defined(AFX_PATCHSHAPEMODEL_H__8CF98C05_F02C_413A_9F0A_A5F6C8694B30__INCLUDED_)
