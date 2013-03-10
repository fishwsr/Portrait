// GaussianModel.cpp: implementation of the CGaussianModel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GaussianModel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGaussianModel::CGaussianModel()
{
	m_nDataDimesion = 1;
	m_AvgData.Resize(1);
	m_RotateMatrix.Resize(1,1);
}

CGaussianModel::~CGaussianModel()
{

}

double CGaussianModel::LogP(const CVisDVector& data) const
{
	double LogP = 0;
	CVisDVector temp;
	temp = data - m_AvgData;
	temp = m_RotateMatrix * temp;

	for (int i = 0; i < m_nDataDimesion; i++ )
	{
		CSGaussian SGaussian(0, m_RotateVar2[i]);
		LogP += SGaussian.LogP(temp[i]);
	}

	return LogP;

/*	
	CVisDVector temp2 = m_InvCovMat * temp;
	double term1 = - (temp * temp2) / 2;
	double term2 = - log( 2 * MY_PI ) * m_nDataDimesion / 2;
	double term3 = - log(m_CovMatDeterminant )/2;
*/
//	return term1 + term2 + term3 ;
}

double CGaussianModel::LogPWithBias(const CVisDVector& data) const
{
	return LogP(data);
	//CVisDVector temp;
	//temp = data - m_AvgData;
	//return - (temp * (m_InvCovMat * temp)) / 2;
}

void CGaussianModel::InitParametersWithCovMat(const CVisDMatrix& CovMat)
{
	//m_CovMatDeterminant = m_CovMat.Determinant();
	//m_InvCovMat = m_CovMat.Inverted();
	m_RotateVar2.Resize(m_nDataDimesion);
	m_RotateMatrix.Resize(m_nDataDimesion, m_nDataDimesion);

	//VisDMatrixSVD()
	if (m_nDataDimesion > 1)
	{
		VisDMatrixSymmetricEigen(CovMat, m_RotateVar2, m_RotateMatrix);
		m_RotateMatrix.Transpose();
	}
	else
	{
		m_RotateMatrix[0][0] = 1.0f;
		m_RotateVar2[0] = CovMat[0][0];
	}
}

const CVisDVector& CGaussianModel::AvgData() const
{
	return m_AvgData; 
}

void CGaussianModel::GetCovMatrix(CVisDMatrix& CovMat)
{
	CovMat.Resize(m_nDataDimesion, m_nDataDimesion);
	
	for (int i = 0; i< m_nDataDimesion; i++)
	{
		CovMat.SetRow( i,  m_RotateMatrix.Row(i) * m_RotateVar2[i]);
	}
	CovMat = m_RotateMatrix * CovMat;
}

CSGaussian::CSGaussian(double mean, double sigma2)
: m_mean(mean), m_sigma2(sigma2) 
{
}

void CSGaussian::SetModel(double mean/* =0. */, double sigma2/* =1. */)
{
	m_mean = mean;
	m_sigma2 = sigma2;
}

void CSGaussian::SetModel(std::vector<double>& vdata)
{
	if ( vdata.size()<=0 ) return;
	if ( vdata.size()==1 )
	{
		m_mean = vdata[0];
		m_sigma2 = 0.;
		return;
	}

	m_mean = 0.;
	for ( int i=0; i<vdata.size(); i++ )
	{
		m_mean += vdata[i];		
	}
	m_mean /= double(vdata.size());

	m_sigma2 = 0.;
	for ( i=0; i<vdata.size(); i++ )
	{
		double tempf = vdata[i]-m_mean;
		m_sigma2 += tempf*tempf;
	}

	m_sigma2 /= double(vdata.size()-1);
}

double CSGaussian::LogGaussian(double mean, double sigma2, double data)
{
	double tempf1 = - log(2* MY_PI * sigma2)/2;
	double tempf2 = -(data-mean)*(data-mean)/(2*sigma2);
	return  tempf1 + tempf2;
}

double CSGaussian::LogP(double data) const
{
	double tempf1 = - log(2* MY_PI * m_sigma2)/2;
	double tempf2 = -(data-m_mean)*(data-m_mean)/(2*m_sigma2);
	return  tempf1 + tempf2;
}

double CSGaussian::LogPWithBias(double data) const
{
	return -(data-m_mean)*(data-m_mean)/(2*m_sigma2);
}

COutputDataFile& Output(COutputDataFile& fs, const CSGaussian& model)
{
	fs << ClassTag("CSGaussian") << model.m_mean << model.m_sigma2 << sep_endl;
	return fs;
}

COutputDataFile& operator<<(COutputDataFile& fs, const CSGaussian& model)
{
	return Output(fs, model);
}

CInputDataFile& Input(CInputDataFile& fs, CSGaussian& model)
{
	fs >> ClassTag("CSGaussian") >> model.m_mean >> model.m_sigma2 >> sep_endl;
	return fs;
}

CInputDataFile& operator>>(CInputDataFile& fs, CSGaussian& model)
{
	return Input(fs, model);
}

COutputDataFile& Output(COutputDataFile& fs, const CGaussianModel& model)
{
	fs << ClassTag("CGaussianModel") << sep_endl;
	fs << ClassTag("DataDimension") << model.m_nDataDimesion << sep_endl;
	fs << ClassTag("AvgData") << sep_endl;
	fs << model.m_AvgData << sep_endl;
	//fs << ClassTag("CovMatrix:") << sep_endl;
	//fs << model.m_CovMat;
	fs << ClassTag("RotateMatrix:") << sep_endl;
	fs << model.m_RotateMatrix << sep_endl;
	fs << ClassTag("RotateVar2:") << sep_endl;
	fs << model.m_RotateVar2 ; 
	return fs;
}

COutputDataFile& operator<<(COutputDataFile& fs, const CGaussianModel& model)
{
	return Output(fs, model);
}

CInputDataFile& Input(CInputDataFile& fs, CGaussianModel& model)
{
	fs >> ClassTag("CGaussianModel") >> sep_endl;
	fs >> ClassTag("DataDimension") >> model.m_nDataDimesion >> sep_endl;
	fs >> ClassTag("AvgData") >> sep_endl;
	fs >> model.m_AvgData >> sep_endl;
	//fs >> ClassTag("CovMatrix:") >> sep_endl;
	//fs >> model.m_CovMat;
	fs >> ClassTag("RotateMatrix:") >> sep_endl;
	fs >> model.m_RotateMatrix >> sep_endl;
	fs >> ClassTag("RotateVar2:") >> sep_endl;
	fs >> model.m_RotateVar2 ; 
	//model.InitParametersWithCovMat();
	return fs;
}

CInputDataFile& operator>>(CInputDataFile& fs, CGaussianModel& model)
{
	return Input(fs, model);
}

double CSGaussianSampler::GetOneSample(int method)
{
	const CSGaussian& model = (const CSGaussian&)(m_ModelRef);
	return rand_gaussian() * m_sigma + model.m_mean;
}

CGaussianSampler::CGaussianSampler(const CGaussianModel& model)
	: CSampler(model)
{
	m_InvRotateMatrix = model.m_RotateMatrix.Transposed();
	m_RotateVar.Resize(model.NDataDimension());

	for (int iData = 0; iData < model.NDataDimension(); iData++ )
	{
		m_RotateVar[iData] = sqrt(model.m_RotateVar2[iData]);
	}
}

void CGaussianSampler::GetOneSample(CVisDVector& data, int method)
{
	const CGaussianModel& model =(const CGaussianModel&)(m_ModelRef);
	CVisDVector tempData(m_ModelRef.NDataDimension());

	for (int iData = 0; iData < m_ModelRef.NDataDimension(); iData++ )
	{
		tempData[iData] = rand_gaussian() * m_RotateVar[iData];
	}

	// Rotate 
	data.Resize(m_ModelRef.NDataDimension());
	data = m_InvRotateMatrix * tempData + model.m_AvgData;
}

void CGaussianModel::SetModel(int nDimension, const CVisDVector& AvgData, const CVisDMatrix& CovMat)
{
	m_nDataDimesion = nDimension;
	m_AvgData.Resize(nDimension);
	m_AvgData = AvgData;
	InitParametersWithCovMat(CovMat);
}

	
double CGaussianKernel::LogP(const CVisDVector& data) const
{
	double logp = 0;
	CSGaussian sg(0, m_sigma * m_sigma);
	for (int i = 0; i < NDataDimension(); i++ )
	{
		logp += sg.LogP(data[i] - m_AvgData[i]);
	}
	return logp;
}

double CGaussianKernel::LogPWithBias(const CVisDVector& data) const
{
	return LogP(data);
}

void CGaussianKernel::SetModel(int nDimension, const CVisDVector& AvgData, double sigma)
{
	m_sigma = sigma;
	m_nDataDimesion = nDimension;
	m_AvgData.Resize(m_nDataDimesion);
	m_AvgData = AvgData;
}

CGuassianKernelSampler::CGuassianKernelSampler(const CGaussianKernel& model)
: CSampler(model)
{

}

void CGuassianKernelSampler::GetOneSample(CVisDVector& data, int method)
{
	const CGaussianKernel& model =(const CGaussianKernel&)(m_ModelRef);
	data.Resize(model.NDataDimension());

	data = model.m_AvgData;
	for (int i = 0; i < model.NDataDimension(); i++ )
	{
		data[i] += rand_gaussian() * model.m_sigma;
	}
}

COutputDataFile& Output(COutputDataFile& ofs, const CGaussianKernel& model)
{
	ofs << ClassTag("GaussianKernel") << sep_endl;
	ofs << model.m_nDataDimesion << model.m_sigma << sep_endl;
	ofs << model.m_AvgData;
	return ofs;
}

COutputDataFile& operator<<(COutputDataFile& ofs, const CGaussianKernel& model)
{
	return Output(ofs, model);
}

CInputDataFile& Input(CInputDataFile& ifs, CGaussianKernel& model)
{
	ifs >> ClassTag("GaussianKernel") >> sep_endl;
	ifs >> model.m_nDataDimesion >> model.m_sigma >> sep_endl;
	ifs >> model.m_AvgData;
	return ifs;
}

CInputDataFile& operator>>(CInputDataFile& ifs, CGaussianKernel& model)
{
	return Input(ifs, model);
}

//                       
//                       |  CovXX   CovXY |
//           JointCov =  |                |
//						 |	CovYX   CovYY |				
//

CGaussianModel& CJointGaussianModel::GetMarginalX()
{
	return m_MarginalX;
}

CGaussianModel& CJointGaussianModel::GetMarginalY()
{
	return m_MarginalY;
}

void CJointGaussianModel::GetConditionalX(CGaussianModel& xconditionalmodel, const CVisDVector& y)
{
	CVisDVector NewMean;
	NewMean.Resize(m_nDataDimensionX);

	NewMean = m_MeanX + m_CovXY * m_CovYY.Inverted() * y;

	CVisDMatrix NewCov;
	NewCov.Resize(m_nDataDimensionX, m_nDataDimensionX);
	NewCov = m_CovXX - m_CovXY * m_CovYY.Inverted() * m_CovYX;

	xconditionalmodel.SetModel(m_nDataDimensionX, NewMean, NewCov);
}

void CJointGaussianModel::GetConditionalY(CGaussianModel& yconditionalmodel, const CVisDVector& x)
{
	CVisDVector NewMean;
	NewMean.Resize(m_nDataDimensionY);

	NewMean = m_MeanY + m_CovYX * m_CovXX.Inverted() * x;

	CVisDMatrix NewCov;
	NewCov.Resize(m_nDataDimensionY, m_nDataDimensionY);
	NewCov = m_CovYY - m_CovYX * m_CovXX.Inverted() * m_CovXY;

	yconditionalmodel.SetModel(m_nDataDimensionY, NewMean, NewCov);
}

double CJointGaussianModel::LogP(const CVisDVector& data) const
{
	return m_JointModel.LogP(data);
}

void CJointGaussianModel::InitByJointModel()
{
	assert(m_nDataDimensionX+m_nDataDimensionY == m_JointModel.NDataDimension());

	int r, c;
	CVisDMatrix CovMat;
	m_JointModel.GetCovMatrix(CovMat);
	
	m_MeanX.Resize(m_nDataDimensionX);
	for ( r=0; r<m_nDataDimensionX; r++ )
	{
		m_MeanX[r] = m_JointModel.AvgData()[r];
	}
	
	m_MeanY.Resize(m_nDataDimensionY);
	for ( r=0; r<m_nDataDimensionY; r++ )
	{
		m_MeanY[r] = m_JointModel.AvgData()[r+m_nDataDimensionX];
	}
	
	m_CovXX.Resize(m_nDataDimensionX, m_nDataDimensionX);
	for ( r=0; r<m_CovXX.NRows(); r++ )
	{
		for ( int c=0; c<m_CovXX.NCols(); c++ )
		{
			m_CovXX[r][c] = CovMat[r][c];
		}
	}
	
	m_CovXY.Resize(m_nDataDimensionX, m_nDataDimensionY);
	for ( r=0; r<m_CovXY.NRows(); r++ )
	{
		for ( c=0; c<m_CovXY.NCols(); c++ )
		{
			m_CovXY[r][c] = CovMat[r][c+m_nDataDimensionX];
		}
	}
	
	m_CovYX.Resize(m_nDataDimensionY, m_nDataDimensionX);
	for ( r=0; r<m_CovYX.NRows(); r++ )
	{
		for ( c=0; c<m_CovYX.NCols(); c++ )
		{
			m_CovYX[r][c] = CovMat[r+m_nDataDimensionX][c];
		}
	}
	
	m_CovYY.Resize(m_nDataDimensionY, m_nDataDimensionY);
	for ( r=0; r<m_CovYY.NRows(); r++ )
	{
		for ( int c=0; c<m_CovYY.NCols(); c++ )
		{
			m_CovYY[r][c] = CovMat[r+m_nDataDimensionX][c+m_nDataDimensionX];
		}
	}
	
	m_MarginalX.SetModel(m_nDataDimensionX, m_MeanX, m_CovXX);
	
	m_MarginalY.SetModel(m_nDataDimensionY, m_MeanY, m_CovYY);
}

// NRows : sample number;
// NCols : data dimension
void CJointGaussianModel::SetModel(const CVisDMatrix& dataMatX, const CVisDMatrix& dataMatY)
{
	assert(dataMatX.NRows() == dataMatY.NRows());

	int nSample = dataMatX.NRows();
	int nDimensionX = dataMatX.NCols();
	int nDimensionY = dataMatY.NCols();
	int nDimension = nDimensionX + nDimensionY;

	CVisDVector mean(nDimension);
	CVisDMatrix dataMat(nSample, nDimension);

	CVisDVector meanX(nDimensionX), meanY(nDimensionY);
	meanX = VisDMatrixSumRow(dataMatX);
	meanY = VisDMatrixSumRow(dataMatY);

	// calculate the Mean
	{
		for ( int i=0; i<nSample; i++ )
		{
			int j;
			for ( j=0; j<nDimensionX; j++ )
			{
				mean[j] += dataMatX[i][j];
			}
			for ( j=0; j<nDimensionY; j++ )
			{
				mean[nDimensionX+j] += dataMatY[i][j];
			}
		}
		mean /= nSample;
	}

	// calculate the CovMat
	{
		for ( int i=0; i<nSample; i++ )
		{
			int j;
			for ( j=0; j<nDimensionX; j++ )
			{
				dataMat[i][j] = dataMatX[i][j] - mean[j];
			}
			for ( j=0; j<nDimensionY; j++ )
			{
				dataMat[i][nDimensionX+j] = dataMatY[i][j] - mean[nDimensionX+j];
			}
		}
	}

	SetModel(nDimensionX, nDimensionY, mean, dataMat.Transposed()*dataMat);
}

void CJointGaussianModel::SetModel(int nDimensionX, int nDimensionY, const CVisDVector& mean, const CVisDMatrix& CovMat)
{
	assert(nDimensionX+nDimensionY == mean.Length());

	m_nDataDimensionX = nDimensionX;
	m_nDataDimensionY = nDimensionY;

	m_JointModel.SetModel(nDimensionX + nDimensionY, mean, CovMat);
	InitByJointModel();
}

COutputDataFile& Output(COutputDataFile& fs, const CJointGaussianModel& model)
{	
	fs << ClassTag("CJointGaussianModel") << sep_endl;
	fs << ClassTag("XDimension") << model.m_nDataDimensionX << ClassTag("YDimension") << model.m_nDataDimensionY << sep_endl;
//	fs << ClassTag("XMean") << model.m_MeanX 
//	   << ClassTag("YMean") << model.m_MeanY
//	   << ClassTag("CovXX") << model.m_CovXX 
//	   << ClassTag("CovXY") << model.m_CovXY
//	   << ClassTag("CovYY") << model.m_CovYY
//	   << ClassTag("CovYX") << model.m_CovYX << sep_endl;
	fs << ClassTag("JointModel") <<  model.m_JointModel;

	return fs;
}

COutputDataFile& operator<<(COutputDataFile& fs, const CJointGaussianModel& model)
{
	return Output(fs, model);
}

CInputDataFile& Input(CInputDataFile& fs, CJointGaussianModel& model)
{
	fs >> ClassTag("CJointGaussianModel") >> sep_endl;
	fs >> ClassTag("XDimension") >> model.m_nDataDimensionX >> ClassTag("YDimension") >> model.m_nDataDimensionY >> sep_endl;
//	fs >> ClassTag("XMean") >> model.m_MeanX 
//	   >> ClassTag("YMean") >> model.m_MeanY
//	   >> ClassTag("CovXX") >> model.m_CovXX 
//	   >> ClassTag("CovXY") >> model.m_CovXY
//	   >> ClassTag("CovYY") >> model.m_CovYY
//	   >> ClassTag("CovYX") >> model.m_CovYX >> sep_endl;
	fs >> ClassTag("JointModel") >>  model.m_JointModel;

	model.InitByJointModel();

	return fs;
}

CInputDataFile& operator>>(CInputDataFile& fs, CJointGaussianModel& model)
{
	return Input(fs, model);
}
