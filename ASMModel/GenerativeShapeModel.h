// GenerativeShapeModel.h: interface for the CGenerativeShapeModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENERATIVESHAPEMODEL_H__A4892F91_023D_4FF6_B258_BC7A6E04CC44__INCLUDED_)
#define AFX_GENERATIVESHAPEMODEL_H__A4892F91_023D_4FF6_B258_BC7A6E04CC44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PatchShapeModel.h"
#include "LocalPatchModel.h"
/*
class CGenerativeShapeModelBuilder;
class CGenerativeShapeModel;

class ASMMODEL_API CGenerativeShapeModelParam
{
public:
	std::vector<CLocalPatchGeomParameter> m_vGeomParameter;
	std::vector<CLocalPatchTextureParameter> m_vTextureParameter;

};

class ASMMODEL_API CGenerativeShapeSketchModel
{
protected:
	std::vector<CLocalPatchTextureSketchModel> m_vLocalSketchModel;
public:
	void Reconstrcut(const CGenerativeShapeModel& model, CVisGrayByteImage& image, const CGenerativeShapeModelParam& param) const; 
	void ReconstrcutWithMask(const CGenerativeShapeModel& model, CVisGrayByteImage& image, const CGenerativeShapeModelParam& param) const; 

	friend class CGenerativeShapeModelBuilder;
	friend COutputDataFile& Output(COutputDataFile& fs, const CGenerativeShapeSketchModel& m);
	friend CInputDataFile& Input(CInputDataFile& fs, CGenerativeShapeSketchModel& m);
};

class ASMMODEL_API CGenerativeShapeModel  
{
public:
	friend class CGenerativeShapeModelBuilder;
	friend class CGenerativeShapeSketchModel;

	CGenerativeShapeModel();
	virtual ~CGenerativeShapeModel();

	double MAPInference(const CVisGrayByteImage& image, CGenerativeShapeModelParam& param) const;
	void Reconstrcut(CVisGrayByteImage& image, const CGenerativeShapeModelParam& param) const; 
	void ReconstrcutWithMask(CVisGrayByteImage& image, const CGenerativeShapeModelParam& param) const; 

	friend COutputDataFile& Output(COutputDataFile& fs, const CGenerativeShapeModel& m);
	friend CInputDataFile& Input(CInputDataFile& fs, CGenerativeShapeModel& m);

	// Debug Function
	void TestGetPutPatch(const CVisGrayByteImage& srcImage, CVisGrayByteImage& desImage, const CGenerativeShapeModelParam& param);
	void GetOneSampleWithParam(CVisGrayByteImage& image, CGenerativeShapeModelParam& param) const;

	CPatchShapeModel& PatchShapeModel() {return m_PatchImageModel; };

protected:
	CShapeModel m_ShapeModel;
	CPatchShapeModel m_PatchImageModel;
	std::vector<CLocalPatchTextureModel> m_vLocalModel;
	
	double MAPLocalInference(const CVisGrayByteImage& image, CGenerativeShapeModelParam& param) const;
	void LocalSample(CVisGrayByteImage& image, CGenerativeShapeModelParam& param) const;

	int NPatch() const;
	void GetPatchCenter(const CGenerativeShapeModelParam& param, CShape& shape) const;
};


ASMMODEL_API COutputDataFile& operator<<(COutputDataFile& fs, const CGenerativeShapeModelParam& p);
ASMMODEL_API CInputDataFile& operator>>(CInputDataFile& fs, CGenerativeShapeModelParam& p);

ASMMODEL_API COutputDataFile& operator<<(COutputDataFile& fs, const CGenerativeShapeModel& m);
ASMMODEL_API CInputDataFile& operator>>(CInputDataFile& fs, CGenerativeShapeModel& m);

ASMMODEL_API COutputDataFile& operator<<(COutputDataFile& fs, const CGenerativeShapeSketchModel& m);
ASMMODEL_API CInputDataFile& operator>>(CInputDataFile& fs, CGenerativeShapeSketchModel& m);

*/
#endif // !defined(AFX_GENERATIVESHAPEMODEL_H__A4892F91_023D_4FF6_B258_BC7A6E04CC44__INCLUDED_)
