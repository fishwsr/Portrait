// GenerativeShapeModel.cpp: implementation of the CGenerativeShapeModel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GenerativeShapeModel.h"
#include "VisImageProc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
CGenerativeShapeModel::CGenerativeShapeModel()
{

}

CGenerativeShapeModel::~CGenerativeShapeModel()
{

}

double CGenerativeShapeModel::MAPInference(const CVisGrayByteImage& image, CGenerativeShapeModelParam& param) const
{
	double LogP = 0;
	double LogPShape = 0;
	double LogPTexture = 0;
	// Shape initialized
	
	// Init param by shape
	//InitPatchGeomParameter()

	param.m_vTextureParameter.resize(NPatch());
	LogPTexture = MAPLocalInference(image, param);

	LogP = LogPTexture;
	return LogP;
}

double CGenerativeShapeModel::MAPLocalInference(const CVisGrayByteImage& image, CGenerativeShapeModelParam& param) const
{
	double LogP = 0;
	
	std::vector<CVisDVector> vPatchData;	
	m_PatchImageModel.GetPatchData(image, param.m_vGeomParameter, vPatchData);


	for (int iPatch = 0; iPatch < NPatch(); iPatch++ )
	{
		LogP += m_vLocalModel[iPatch].MAPInference(vPatchData[iPatch], param.m_vTextureParameter[iPatch]);
	}
	return LogP;
}

void CGenerativeShapeModel::Reconstrcut(CVisGrayByteImage& image, const CGenerativeShapeModelParam& param) const 
{
	std::vector<CVisDVector> vPatchData;	
	vPatchData.resize(NPatch());

	for (int iPatch = 0; iPatch < NPatch(); iPatch++ )
	{
		CVisDVector PatchData;
		m_vLocalModel[iPatch].Reconstruct(param.m_vTextureParameter[iPatch], vPatchData[iPatch]);
	}
	m_PatchImageModel.PutPatchData(image, param.m_vGeomParameter, vPatchData);
}

void CGenerativeShapeModel::ReconstrcutWithMask(CVisGrayByteImage& image, const CGenerativeShapeModelParam& param) const
{
	std::vector<CVisDVector> vPatchData;	
	vPatchData.resize(NPatch());

	for (int iPatch = 0; iPatch < NPatch(); iPatch++ )
	{
		CVisDVector PatchData;
		m_vLocalModel[iPatch].Reconstruct(param.m_vTextureParameter[iPatch], vPatchData[iPatch]);
	}
	m_PatchImageModel.PutPatchDataWithMask(image, param.m_vGeomParameter, vPatchData);
}

int CGenerativeShapeModel::NPatch() const
{
	return m_PatchImageModel.NPatch();
}

void CGenerativeShapeModel::GetPatchCenter(const CGenerativeShapeModelParam& param, CShape& shape) const
{
	shape.Resize(NPatch());

	for (int iPatch = 0; iPatch < NPatch(); iPatch++ )
	{
		shape.PointX(iPatch) = param.m_vGeomParameter[iPatch].m_fCx;
		shape.PointY(iPatch) = param.m_vGeomParameter[iPatch].m_fCy;
	}
}

void CGenerativeShapeModel::TestGetPutPatch(const CVisGrayByteImage& srcImage, CVisGrayByteImage& desImage, const CGenerativeShapeModelParam& param)
{
	std::vector<CVisDVector> vPatchData;	
	vPatchData.resize(NPatch());

	m_PatchImageModel.GetPatchData(srcImage, param.m_vGeomParameter, vPatchData);
	m_PatchImageModel.PutPatchDataWithMask(desImage, param.m_vGeomParameter, vPatchData);
}


COutputDataFile& operator<<(COutputDataFile& fs, const CGenerativeShapeModelParam& p)
{
	fs << ClassTag("CGenerativeShapeModelParam") << sep_endl;

	assert( p.m_vGeomParameter.size() == p.m_vTextureParameter.size() );
	fs << (int)p.m_vGeomParameter.size() << sep_endl;
	
	for (int i=0; i< p.m_vGeomParameter.size(); i++ )
	{
		fs << p.m_vGeomParameter[i];

		if (i != p.m_vGeomParameter.size() - 1 )
		{
			fs << sep_endl;
		}
	}

	for (i=0; i< p.m_vTextureParameter.size(); i++ )
	{
		fs << p.m_vTextureParameter[i];

		if (i != p.m_vTextureParameter.size() - 1 )
		{
			fs << sep_endl;
		}
	}

	return fs;
}

CInputDataFile& operator>>(CInputDataFile& fs, CGenerativeShapeModelParam& p)
{
	fs >> ClassTag("CGenerativeShapeModelParam") >> sep_endl;

	int nPatch;
	fs >> nPatch >> sep_endl;

	p.m_vGeomParameter.resize(nPatch);
	p.m_vTextureParameter.resize(nPatch);
	
	for (int i=0; i< p.m_vGeomParameter.size(); i++ )
	{
		fs >> p.m_vGeomParameter[i];

		if (i != p.m_vGeomParameter.size() - 1 )
		{
			fs >> sep_endl;
		}
	}

	for (i=0; i< p.m_vTextureParameter.size(); i++ )
	{
		fs >> p.m_vTextureParameter[i];

		if (i != p.m_vTextureParameter.size() - 1 )
		{
			fs >> sep_endl;
		}
	}

	return fs;
}

COutputDataFile& Output(COutputDataFile& fs, const CGenerativeShapeModel& m)
{
	fs << ClassTag("CGenerativeShapeModel") << sep_endl;

	fs << m.m_ShapeModel << sep_endl;
	fs << m.m_PatchImageModel << sep_endl;
	
	int nPatch = m.NPatch();
	assert( nPatch == m.m_vLocalModel.size() );

	for (int iPatch = 0; iPatch < nPatch; iPatch++ )
	{
		fs << m.m_vLocalModel[iPatch];

		if (iPatch != nPatch - 1 )
		{
			fs << sep_endl;
		}
	}
	return fs;
}

CInputDataFile& Input(CInputDataFile& fs, CGenerativeShapeModel& m)
{
	fs >> ClassTag("CGenerativeShapeModel") >> sep_endl;

	fs >> m.m_ShapeModel >> sep_endl;
	fs >> m.m_PatchImageModel >> sep_endl;

	int nPatch = m.NPatch();
	m.m_vLocalModel.resize(nPatch);

	for (int iPatch = 0; iPatch < nPatch; iPatch++ )
	{
		fs >> m.m_vLocalModel[iPatch];

		if (iPatch != nPatch - 1 )
		{
			fs >> sep_endl;
		}
	}
	return fs;
}

COutputDataFile& operator<<(COutputDataFile& fs, const CGenerativeShapeModel& m)
{
	return Output(fs, m);
}

CInputDataFile& operator>>(CInputDataFile& fs, CGenerativeShapeModel& m)
{
	return Input(fs, m);
}

void CGenerativeShapeSketchModel::Reconstrcut(const CGenerativeShapeModel& model, CVisGrayByteImage& image, const CGenerativeShapeModelParam& param) const
{
	std::vector<CVisDVector> vPatchData;
	vPatchData.resize(model.NPatch());

	for (int iPatch = 0; iPatch < model.NPatch() ; iPatch++ )
	{
		m_vLocalSketchModel[iPatch].Reconstruct(param.m_vTextureParameter[iPatch], vPatchData[iPatch]);
	}
	model.m_PatchImageModel.PutPatchData(image, param.m_vGeomParameter, vPatchData);
}

void CGenerativeShapeSketchModel::ReconstrcutWithMask(const CGenerativeShapeModel& model, CVisGrayByteImage& image, const CGenerativeShapeModelParam& param) const
{
	std::vector<CVisDVector> vPatchData;
	vPatchData.resize(model.NPatch());

	for (int iPatch = 0; iPatch < model.NPatch() ; iPatch++ )
	{
		m_vLocalSketchModel[iPatch].Reconstruct(param.m_vTextureParameter[iPatch], vPatchData[iPatch]);
	}
	model.m_PatchImageModel.PutPatchData(image, param.m_vGeomParameter, vPatchData);
}

COutputDataFile& Output(COutputDataFile& fs, const CGenerativeShapeSketchModel& m)
{
	fs << ClassTag("CGenerativeShapeSketchModel") << sep_endl;
	fs << (int)m.m_vLocalSketchModel.size() << sep_endl;
	for (int i = 0; i< m.m_vLocalSketchModel.size(); i++ )
	{
		fs << m.m_vLocalSketchModel[i];
		if (i < m.m_vLocalSketchModel.size() -1 )
		{
			fs << sep_endl;
		}
	}
	return fs;
}

CInputDataFile& Input(CInputDataFile& fs, CGenerativeShapeSketchModel& m)
{
	fs >> ClassTag("CGenerativeShapeSketchModel") >> sep_endl;
	int nPatch;
	fs >> nPatch >> sep_endl;
	m.m_vLocalSketchModel.resize(nPatch);
	for (int i = 0; i< m.m_vLocalSketchModel.size(); i++ )
	{
		fs >> m.m_vLocalSketchModel[i];
		if (i < m.m_vLocalSketchModel.size() -1 )
		{
			fs >> sep_endl;
		}
	}
	return fs;	
}

void CGenerativeShapeModel::GetOneSampleWithParam(CVisGrayByteImage& image, CGenerativeShapeModelParam& param) const
{
	LocalSample(image, param);
}

void CGenerativeShapeModel::LocalSample(CVisGrayByteImage& image, CGenerativeShapeModelParam& param) const
{
	std::vector<CVisDVector> vPatchData;
	vPatchData.resize(NPatch());

	for (int iPatch = 0; iPatch < NPatch(); iPatch++)
	{
		// Sample 
		m_vLocalModel[iPatch].GetOneSampleAndParam(vPatchData[iPatch], param.m_vTextureParameter[iPatch]);
	}
	m_PatchImageModel.PutPatchDataWithMask(image, param.m_vGeomParameter, vPatchData);
}
*/