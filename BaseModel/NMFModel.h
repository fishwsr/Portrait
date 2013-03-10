// NMFModel.h: interface for the CNMFModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NMFMODEL_H__EDC84E1B_712F_4927_841E_EAA5EE9DD932__INCLUDED_)
#define AFX_NMFMODEL_H__EDC84E1B_712F_4927_841E_EAA5EE9DD932__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNMFModelBuilder;

// Non-negative Matrix Factorization model,
// which is different from the PCA or VQ in the sense that
// all baisis are pursuited under non-negative constraints.
// It leads to extraction of part-based and additive basis.
class BASEMODEL_API CNMFModel  
{
public:
	friend class CNMFModelBuilder;
	friend COutputDataFile& Output(COutputDataFile& ofs, const CNMFModel& model);
	friend CInputDataFile& Input(CInputDataFile& ifs, CNMFModel& model);
	
	CNMFModel();
	virtual ~CNMFModel();

	const CVisDVector& AvgData() const { return m_AvgData; } 
	const int DataDimension() const { return m_iDataDimension; } // data dimension
	const int NbBasis() const { return m_iNbBasis; } // number of basis
	const CVisDMatrix& W() const { return m_W; } // basis matrix
	const CVisDMatrix& H() const { return m_H; } // encodings matrix
	void Reconstruct(const CVisDVector& encoding, CVisDVector& data) const; // reconstruct from the encoding
	void Encode(const CVisDVector& data, CVisDVector& encoding) const; // project to W and compute the encoding
	void Regularize(const CVisDVector& rawdata, CVisDVector& regdata) const; // regularize raw-data to reg-data by the model
	
	bool IsReady() const { return m_fbReady; }

	double ErrS() const { return m_ErrS; }
	double InvErrS() const { return m_InvErrS; }

protected:
	void CalcEncodingMatrix();
	void CalcReconstructMatrix();
	
	void NormalizeW();
	void NormalizeH();

	CVisDVector m_AvgData;

	int m_iDataDimension; // n: dimension of the data vector
	int m_iNbBasis;    // r: denotes the rank of basis matrix W, and depends on number of basis desired

	CVisDMatrix m_W;  // W (n x r): basis matrix, with each column is a base vector
	CVisDMatrix m_H;  // H (r x m): encodings matrix, with each row as one encoding of a data vector

	// initialize only after built or loaded
	// no need to save to the model
	CVisDMatrix m_EncodeMat;		 // encoding matrix: (W_t * W)'*W_t       
	CVisDMatrix m_ReconstructMat;	 // reconstruct matrix: W * ((W_t * W)'*W_t)

	double m_dOffset;
	double m_dScale;

	// variance of reconstruction error
	// don't changed them once model is trained
	double m_ErrS, m_InvErrS; 

	bool m_fbReady;
};

BASEMODEL_API COutputDataFile& operator<<(COutputDataFile& ofs, const CNMFModel& model);
BASEMODEL_API CInputDataFile& operator>>(CInputDataFile& ifs, CNMFModel& model);

#endif // !defined(AFX_NMFMODEL_H__EDC84E1B_712F_4927_841E_EAA5EE9DD932__INCLUDED_)
