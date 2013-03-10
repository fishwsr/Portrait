// MatrixMultiband.h: interface for the CMatrixMultiband class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIXMULTIBAND_H__DA30883D_11DF_48FD_96A9_0CA60B6FA5E5__INCLUDED_)
#define AFX_MATRIXMULTIBAND_H__DA30883D_11DF_48FD_96A9_0CA60B6FA5E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSBASE_API CMatrixMultiband  
{
public:
	CMatrixMultiband(int nBand=4);
	virtual ~CMatrixMultiband();

	void Init(int nBand);

	CMatrixMultiband& operator=(const CMatrixMultiband& ref);
	CMatrixMultiband& operator=(const CVisDMatrix& mat);
	CMatrixMultiband& operator=(double d);
	CVisDMatrix& operator[](int iBand);

	int NBand() const { return m_nBandNum; }
	void Resize(int rows, int cols);
	bool AllBandsOfSameDim() const;	
	int NRows() const;
	int NCols() const;

protected:
	int m_nBandNum;
	std::vector<CVisDMatrix> m_vMatrix;
};

CSBASE_API void Matrix3bandToImageRGBA(const CMatrixMultiband& mat, CVisRGBAByteImage& image);
CSBASE_API void ImageRGBAToMatrix3band(const CVisRGBAByteImage& image, CMatrixMultiband& mat);

#endif // !defined(AFX_MATRIXMULTIBAND_H__DA30883D_11DF_48FD_96A9_0CA60B6FA5E5__INCLUDED_)
