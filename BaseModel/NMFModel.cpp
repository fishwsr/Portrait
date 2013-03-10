// NMFModel.cpp: implementation of the CNMFModel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NMFModel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNMFModel::CNMFModel()
: m_iNbBasis(0),
m_iDataDimension(0),
m_dOffset(0.),
m_dScale(1.),
m_fbReady(false),
m_ErrS(0.), m_InvErrS(0.)
{
}

CNMFModel::~CNMFModel()
{
}

void CNMFModel::Regularize(const CVisDVector& rawdata, CVisDVector& regdata) const
{
	assert(m_fbReady);
	assert(rawdata.Length() == m_iDataDimension);
	
	regdata.Resize(rawdata.Length());
	regdata = rawdata - m_AvgData;
/*
	double* dataptr;
	
	dataptr = regdata.data();
	for ( int i=0; i<m_iDataDimension; i++ )
	{
		*dataptr = (*dataptr - m_dOffset) * m_dScale;
		dataptr++;
	}
*/
	regdata = m_ReconstructMat * regdata;
/*	
	dataptr = regdata.data();
	for ( i=0; i<m_iDataDimension; i++ )
	{
		*dataptr = (*dataptr / m_dScale) + m_dOffset;
	}
*/
	regdata += m_AvgData;
}

void CNMFModel::Reconstruct(const CVisDVector& encoding, CVisDVector& data) const
{
	assert(m_fbReady);
	
	data = m_W * encoding;
/*	
	double* dataptr = data.data();
	for ( int i=0; i<m_iDataDimension; i++ )
	{
		*dataptr = *dataptr / m_dScale + m_dOffset;
		dataptr++;
	}
*/
	data += m_AvgData;
}

void CNMFModel::Encode(const CVisDVector& data, CVisDVector& encoding) const
{
	assert(m_fbReady);
	assert(data.Length() == m_iDataDimension);

	CVisDVector orgdata = data - m_AvgData;
/*	
	double* dataptr = data.data();
	double* avgdataptr = m_AvgData.data();
	for ( int i=0; i<m_iDataDimension; i++ )
	{
	*dataptr = (*dataptr - *avgdataptr++ - m_dOffset) * m_dScale;
	dataptr++;
	}
*/
	encoding = m_EncodeMat * orgdata;
}

// m_iNbBasis x m_iDataDimension
void CNMFModel::CalcEncodingMatrix()
{
	assert(m_iNbBasis==m_W.NCols());
	assert(m_iDataDimension==m_W.NRows());

	CVisDMatrix Wt = m_W.Transposed();
	CVisDMatrix WtxW = Wt * m_W;
	// make sure WtxW can be inversed
	for ( int i=0; i<WtxW.NRows(); i++ )
	{
		for ( int j=0; j<WtxW.NCols(); j++ )
		{
			WtxW[i][j] += 1e-9;
		}
	}
	m_EncodeMat.Resize(m_iNbBasis, m_iDataDimension);
	m_EncodeMat = WtxW.Inverted() * Wt;	
}

void CNMFModel::CalcReconstructMatrix()
{
	assert(m_iNbBasis==m_EncodeMat.NRows());
	assert(m_iDataDimension==m_EncodeMat.NCols());
	
	m_ReconstructMat.Resize(m_iDataDimension, m_iDataDimension);
	m_ReconstructMat = m_W * m_EncodeMat;
}

void CNMFModel::NormalizeH()
{
	assert(m_iNbBasis==m_H.NRows());
	assert(m_iDataDimension==m_W.NRows());
	
	CVisDMatrix Lambda(m_iNbBasis, m_iNbBasis), InvLambda(m_iNbBasis, m_iNbBasis);
	Lambda = 0.0;
	InvLambda = 0.0;
	for ( int i=0; i<m_iNbBasis; i++ )
	{
		double tempf = VisDVectorSumL2(m_H.Row(i)) + 1e-9;
		Lambda[i][i] = tempf;
		InvLambda[i][i] = 1./tempf;
	}

	m_W = m_W * Lambda;
	m_H = InvLambda * m_H;
}

void CNMFModel::NormalizeW()
{
	assert(m_iNbBasis==m_H.NRows());
	assert(m_iDataDimension==m_W.NRows());

	CVisDMatrix Lambda(m_iNbBasis, m_iNbBasis), InvLambda(m_iNbBasis, m_iNbBasis);
	Lambda = 0.0;
	InvLambda = 0.0;
	for ( int i=0; i<m_iNbBasis; i++ )
	{
		double tempf = VisDVectorSumL2(m_W.Column(i)) + 1e-9;
		Lambda[i][i] = tempf;
		InvLambda[i][i] = 1./tempf;
	}

	m_W = m_W * InvLambda;
	m_H = Lambda * m_H;
}

COutputDataFile& Output(COutputDataFile& fs, const CNMFModel& model)
{
	fs << ClassTag("CNMFModel") << model.m_fbReady << sep_endl;
	
	if ( model.m_fbReady )
	{
		fs << model.m_dOffset << model.m_dScale << model.m_AvgData << sep_endl;
		fs << model.m_W << model.m_H << sep_endl;
		fs << model.m_ErrS << model.m_InvErrS << sep_endl;
	}
	
	return fs;
}

CInputDataFile& Input(CInputDataFile& fs, CNMFModel& model)
{
	fs >> ClassTag("CNMFModel") >> model.m_fbReady >> sep_endl;
	
	if ( model.m_fbReady )
	{
		fs >> model.m_dOffset >> model.m_dScale >> model.m_AvgData >> sep_endl;
		fs >> model.m_W >> model.m_H >> sep_endl;
		fs >> model.m_ErrS >> model.m_InvErrS >> sep_endl;

		assert(model.m_W.NCols() == model.m_H.NRows());
		model.m_iDataDimension = model.m_W.NRows();
		model.m_iNbBasis = model.m_W.NCols();

		model.CalcEncodingMatrix();
		model.CalcReconstructMatrix();
	}
	
	return fs;
	
}

COutputDataFile& operator<<(COutputDataFile& fs, const CNMFModel& model)
{
	return Output(fs, model);
}

CInputDataFile& operator>>(CInputDataFile& fs, CNMFModel& model)
{
	return Input(fs, model);
}
