// LocalPatchModel.cpp: implementation of the CLocalPatchModel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LocalPatchModel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
CLocalPatchTextureModel::CLocalPatchTextureModel()
{
}

CLocalPatchTextureModel::~CLocalPatchTextureModel()
{
}

double CLocalPatchTextureModel::LogP(const CVisDVector& data) const
{
	return m_MPPCAModel.LogP(data);
}

double CLocalPatchTextureModel::JointLogP(const CVisDVector& data, const CLocalPatchTextureParameter& param) const	
{
	return m_MPPCAModel.JointLogP(data, param.m_iCluster, param.m_PCAParameter);
}

double CLocalPatchTextureModel::MAPInference(const CVisDVector& data, CLocalPatchTextureParameter& param) const
{
	return m_MPPCAModel.MAPInference(data, param.m_iCluster, param.m_PCAParameter);
}

void CLocalPatchTextureModel::Reconstruct(const CLocalPatchTextureParameter& param, CVisDVector& data) const
{
	m_MPPCAModel.Reconstruct(param.m_PCAParameter, param.m_iCluster, data);
}


COutputDataFile& Output(COutputDataFile& fs, const CLocalPatchTextureModel& m)
{
	fs << ClassTag("CLocalPatchTextureModel") << sep_endl;
	fs << m.m_MPPCAModel;
	return fs;
}

COutputDataFile& operator<<(COutputDataFile& fs, const CLocalPatchTextureModel& m)
{
	return Output(fs, m);
}

CInputDataFile& Input(CInputDataFile& fs, CLocalPatchTextureModel& m)
{
	fs >> ClassTag("CLocalPatchTextureModel") >> sep_endl;
	fs >> m.m_MPPCAModel;
	return fs;
}

CInputDataFile& operator>>(CInputDataFile& fs, CLocalPatchTextureModel& m)
{
	return Input(fs, m);
}

COutputDataFile& Output(COutputDataFile& fs, const CLocalPatchTextureSketchModel& m)
{
	fs << ClassTag("CLocalPatchTextureSketchModel") << sep_endl;
	fs << m.m_iIndexMode << m.m_fVectorModel << sep_endl;  
	fs << m.m_vSketch <<  sep_endl;
	
	if ( 1 == m.m_iIndexMode )
	{
		fs << (int)(m.m_vClusterIndex.size());
		for (int i = 0; i< m.m_vClusterIndex.size(); i++)
		{
			fs << m.m_vClusterIndex[i] ;
		}
		fs << sep_endl;
	}	

	if ( true == m.m_fVectorModel )
	{
		fs << (int)(m.m_vVectorSketch.size()) << sep_endl;
		for (int i =0 ; i < m.m_vVectorSketch.size(); i++)
		{
			fs << m.m_vVectorSketch[i];
			if ( i < m.m_vVectorSketch.size() - 1)
			{
				fs << sep_endl;
			}
		}
	}
	return fs;
}

CInputDataFile& Input(CInputDataFile& fs, CLocalPatchTextureSketchModel& m)
{
	fs >> ClassTag("CLocalPatchTextureSketchModel") >> sep_endl;
	fs >> m.m_iIndexMode >> m.m_fVectorModel >> sep_endl;  
	fs >> m.m_vSketch;

	if ( 1 == m.m_iIndexMode )
	{
		int l;
		fs >> l;
		m.m_vClusterIndex.resize(l);
		for (int i = 0; i< m.m_vClusterIndex.size(); i++)
		{
			fs >> m.m_vClusterIndex[i] ;
		}
		fs >> sep_endl;
	}	

	if ( true == m.m_fVectorModel )
	{
		int l;
		fs >> l >> sep_endl;
		m.m_vVectorSketch.resize(l);
		for (int i =0 ; i < m.m_vVectorSketch.size(); i++)
		{
			fs >> m.m_vVectorSketch[i];
			if ( i < m.m_vVectorSketch.size() - 1)
			{
				fs >> sep_endl;
			}
		}
	}

	return fs;
}

COutputDataFile& operator<<(COutputDataFile& fs, const CLocalPatchTextureSketchModel& m)
{
	return Output(fs, m);
}

CInputDataFile& operator>>(CInputDataFile& fs, CLocalPatchTextureSketchModel& m)
{
	return Input(fs, m);
}

CLocalPatchTextureSketchModel::CLocalPatchTextureSketchModel()
: m_iIndexMode(0), m_fVectorModel(false)
{
}

void CLocalPatchTextureSketchModel::Reconstruct(const CLocalPatchTextureParameter& param, CVisDVector& data) const
{
	int index = param.m_iCluster;

	if ( 1 == m_iIndexMode )
	{
		assert(index < m_vClusterIndex.size());
		index  = m_vClusterIndex[index];
	}

	assert( index < m_vSketch.NRows() );	

	data.Resize(m_vSketch.NCols());
	data = m_vSketch.Row(index);
}

void CLocalPatchTextureSketchModel::Reconstruct(int index, CVisDVector& data) const
{
	if ( 1 == m_iIndexMode )
	{
		assert(index < m_vClusterIndex.size());
		index  = m_vClusterIndex[index];
	}
	
	assert( index < m_vSketch.NRows() );

	data.Resize(m_vSketch.NCols());
	data = m_vSketch.Row(index);	
}

void CLocalPatchTextureSketchModel::MeanImage(int index, CVisDVector& data) const
{
	if ( 1 == m_iIndexMode )
	{
		assert(index < m_vClusterIndex.size());
		index  = m_vClusterIndex[index];
	}
	
	assert( index < m_vImage.NRows() );

	data.Resize(m_vImage.NCols());
	data = m_vImage.Row(index);	
}

void CLocalPatchTextureModel::GetOneSampleAndParam(CVisDVector& data, CLocalPatchTextureParameter& param) const
{
	CMixturePPCASampler sampler(m_MPPCAModel);
	sampler.GetOneSampleAndParam(data, param.m_iCluster, param.m_PCAParameter);
}

void CLocalPatchTextureSketchModel::VectorSketch(int index, CShape& vs) const
{
	assert(m_fVectorModel);

	if ( 1 == m_iIndexMode)
	{
		assert(index < m_vClusterIndex.size());
		index  = m_vClusterIndex[index];
	}

	{
		assert( index < m_vVectorSketch.size());
		vs = m_vVectorSketch[index];
	}
}
*/