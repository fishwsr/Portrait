// LocalPatchModel.h: interface for the CLocalPatchModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOCALPATCHMODEL_H__BC09C335_6E16_4C95_8EE1_7CC07CC21884__INCLUDED_)
#define AFX_LOCALPATCHMODEL_H__BC09C335_6E16_4C95_8EE1_7CC07CC21884__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*
class CLocalPatchModelBuilder;  

class ASMMODEL_API CLocalPatchTextureSketchModel
{
protected:
	bool m_fVectorModel; // F: no vector model; T: have vector model
	int m_iIndexMode; // 0 : non-index model ; 1 : index mode;

	CVisDMatrix m_vImage;
	CVisDMatrix m_vSketch;

	std::vector<int> m_vClusterIndex;
	std::vector<CShape> m_vVectorSketch;

public:

	CLocalPatchTextureSketchModel();

	const CVisDMatrix& vSketch() const { return m_vSketch;};
	const CVisDMatrix& vImage() const {return m_vImage;};

	void VectorSketch(int index, CShape& vs) const;
	void MeanImage(int index, CVisDVector& data) const;
	
	void Reconstruct(const CLocalPatchTextureParameter& param, CVisDVector& data) const; 
	void Reconstruct(int index, CVisDVector& data) const;
	
	friend class CLocalPatchModelBuilder;  
	friend COutputDataFile& Output(COutputDataFile& ifs, const CLocalPatchTextureSketchModel& m);
	friend CInputDataFile& Input(CInputDataFile& ifs, CLocalPatchTextureSketchModel& m);
};

class ASMMODEL_API CLocalPatchTextureModel : CDistribution
{
public:
	friend class CLocalPatchModelBuilder;  

	CLocalPatchTextureModel();
	virtual ~CLocalPatchTextureModel();
	
	virtual double LogP(const CVisDVector& data) const;

	double JointLogP(const CVisDVector& data, const CLocalPatchTextureParameter& param) const;	
	double MAPInference(const CVisDVector& data, CLocalPatchTextureParameter& param) const;
	void Reconstruct(const CLocalPatchTextureParameter& param, CVisDVector& data) const;

	void GetOneSampleAndParam(CVisDVector& data, CLocalPatchTextureParameter& param) const;

	friend COutputDataFile& Output(COutputDataFile& ifs, const CLocalPatchTextureModel& m);
	friend CInputDataFile& Input(CInputDataFile& ifs, CLocalPatchTextureModel& m);

	const CMixturePPCA& Model() const { return m_MPPCAModel; } ;
protected:
	 
	SIZE m_PatchHalfSize;
	CMixturePPCA m_MPPCAModel;
};

ASMMODEL_API COutputDataFile& operator<<(COutputDataFile& fs, const CLocalPatchTextureSketchModel& m);
ASMMODEL_API CInputDataFile& operator>>(CInputDataFile& fs, CLocalPatchTextureSketchModel& m);

ASMMODEL_API COutputDataFile& operator<<(COutputDataFile& fs, const CLocalPatchTextureModel& m);
ASMMODEL_API CInputDataFile& operator>>(CInputDataFile& fs, CLocalPatchTextureModel& m);
*/
#endif // !defined(AFX_LOCALPATCHMODEL_H__BC09C335_6E16_4C95_8EE1_7CC07CC21884__INCLUDED_)
