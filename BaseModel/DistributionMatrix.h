// DistributionMatrix.h: interface for the CDistributionMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISTRIBUTIONMATRIX_H__466FC46D_0A17_4CE9_A601_53121AA42E0C__INCLUDED_)
#define AFX_DISTRIBUTIONMATRIX_H__466FC46D_0A17_4CE9_A601_53121AA42E0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template<class _distributionType>
class BASEMODEL_API CDistributionMatrix  
{
public:
	CDistributionMatrix(int nRows=1, int nCols=1);
	virtual ~CDistributionMatrix();

	int                 NRows() { return m_nRows; }
	int                 NCols() { return m_nCols; }
	_distributionType*	GetOne(int r, int c);
	void				Resize(int nRows, int nCols);
	void				Clear();
	
protected:
	int			m_nRows;
	int			m_nCols;
	std::vector<_distributionType>	m_vDistributionArray;
};

template<class _distributionType>
CDistributionMatrix<_distributionType>::CDistributionMatrix(int nRows/* =1 */, int nCols/* =1 */)
{
	Resize(nRows, nCols);
}

template<class _distributionType>
CDistributionMatrix<_distributionType>::~CDistributionMatrix()
{
	Clear();
}

template<class _distributionType>
void CDistributionMatrix<_distributionType>::Resize(int nRows, int nCols)
{
	nRows = nRows<1 ? 1 : nRows;
	nCols = nCols<1 ? 1 : nCols;

	m_nRows = nRows;
	m_nCols = nCols;
	m_vDistributionArray.resize(m_nRows * m_nCols);
}

template<class _distributionType>
void CDistributionMatrix<_distributionType>::Clear()
{
	m_vDistributionArray.clear();
}

template<class _distributionType>
_distributionType* CDistributionMatrix<_distributionType>::GetOne(int r, int c)
{
	if (r<0 || r>=m_nRows || c<0 || c>=m_nCols) return NULL;
	return &m_vDistributionArray[r*m_nCols+c];
}

template<class _distributionType>
BASEMODEL_API COutputDataFile& operator<<(COutputDataFile& ofs, const CDistributionMatrix<_distributionType>& distMatrix)
{
	return Output(ofs, distMatrix);
}


template<class _distributionType>
BASEMODEL_API CInputDataFile& operator>>(CInputDataFile& ifs, CDistributionMatrix<_distributionType>& distMatrix)
{
	return Input(ifs, distMatrix);
}

template<class _distributionType>
BASEMODEL_API COutputDataFile& Output(COutputDataFile& ofs, const CDistributionMatrix<_distributionType>& distMatrix)
{
	ofs << ClassTag("DistMatrix") << sep_endl;
	ofs << distMatrix.m_nRows << distMatrix.m_nCols << sep_endl;
	for (int r=0; r<distMatrix.m_nRows; r++)
	{
		for (int c=0; c<distMatrix.m_nCols; c++)
		{
			ofs << distMatrix.m_vDistributionArray[r*distMatrix.m_nCols+c];
		}
	}
	return ofs;
}

template<class _distributionType>
BASEMODEL_API CInputDataFile& Input(CInputDataFile& ifs, CDistributionMatrix<_distributionType>& distMatrix)
{
	distMatrix.Clear();
	int nRows, nCols;

	ifs >> ClassTag("DistMatrix") >> sep_endl;
	ifs >> nRows >> nCols >> sep_endl;
	distMatrix.Resize(nRows, nCols);
	for (int r=0; r<distMatrix.m_nRows; r++)
	{
		for (int c=0; c<distMatrix.m_nCols; c++)
		{
			ifs >> distMatrix.m_vDistributionArray[r*distMatrix.m_nCols+c];
		}
	}
	return ifs;
}

#define CGaussianKernelMatrix			CDistributionMatrix<CGaussianKernel>
#define CSGaussianMatrix				CDistributionMatrix<CSGaussian>
#define CGaussianModelMatrix			CDistributionMatrix<CGaussianModel>
#define CMixtureGaussianKernelMatrix	CDistributionMatrix<CMixtureGaussianKernel>
#define CMixtureGaussianMatrix			CDistributionMatrix<CMixtureGaussian>

#endif // !defined(AFX_DISTRIBUTIONMATRIX_H__466FC46D_0A17_4CE9_A601_53121AA42E0C__INCLUDED_)
