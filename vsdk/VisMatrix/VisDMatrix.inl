// VisDMatrix.inl
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG



inline CVisDVector::CVisDVector(void)
  : m_length(0), m_data(0), m_fExternalStorage(false)
{
}

inline CVisDVector::CVisDVector(int length, double *storage)
{
    SetSize(length, storage);
}

inline CVisDVector::CVisDVector(const CVisDVector &refvector)
{
    SetSize(refvector.Length());
	*this = refvector;
}

inline CVisDVector::~CVisDVector(void)
{
	FreeBuffer();
}

inline bool CVisDVector::operator!=(const CVisDVector& refvector) const
{
    return !(*this == refvector);
}

inline CVisDVector& CVisDVector::operator+=(const CVisDVector& refvector)
{
    return EqSum(*this, refvector); 
}

inline CVisDVector& CVisDVector::operator-=(const CVisDVector& refvector)
{
    return EqDiff(*this, refvector); 
}

inline CVisDVector CVisDVector::operator+(const CVisDVector& refvector) const
{
    assert(Length() == refvector.Length()); 
    CVisDVector vectorRet(*this);

    return (vectorRet += refvector); 
}

inline CVisDVector CVisDVector::operator-(const CVisDVector& refvector) const
{
    assert(Length() == refvector.Length()); 
    CVisDVector vectorRet(*this);

    return (vectorRet -= refvector); 
}

inline CVisDVector CVisDVector::operator*(double dbl) const
{
    CVisDVector vectorRet(*this);

    return (vectorRet *= dbl); 
}

inline CVisDVector CVisDVector::operator/(double dbl) const
{
	assert(dbl != 0);

    CVisDVector vectorRet(*this);

    return (vectorRet /= dbl); 
}




inline CVisDMatrix::CVisDMatrix(void)
  : m_nRows(0), m_nCols(0), m_data(0), m_fExternalStorage(false)
{
}
inline CVisDMatrix::CVisDMatrix(int rows, int cols, double *storage)
{
    SetSize(rows, cols, storage);
}

inline CVisDMatrix::CVisDMatrix(const CVisDMatrix &refmatrix)
{
    SetSize(refmatrix.NRows(), refmatrix.NCols());
	*this = refmatrix;
}

inline CVisDMatrix::~CVisDMatrix(void)
{
	FreeBuffer();
}

inline bool CVisDMatrix::operator!=(const CVisDMatrix& refmatrix) const
{
    return !(*this == refmatrix);
}

inline CVisDMatrix CVisDMatrix::operator+(const CVisDMatrix& refmatrix) const
{
    CVisDMatrix matrixRet(*this);

    return matrixRet.EqSum(*this, refmatrix);
}

inline CVisDMatrix CVisDMatrix::operator-(const CVisDMatrix& refmatrix) const
{
    CVisDMatrix matrixRet(*this);

    return matrixRet.EqDiff(*this, refmatrix);
}

inline CVisDVector CVisDMatrix::operator*(const CVisDVector& refvector) const
{
    CVisDVector vectorRet(NRows());

	return vectorRet.EqProd(*this, refvector);
}

inline CVisDMatrix CVisDMatrix::operator*(const CVisDMatrix& refmatrix) const
{
    CVisDMatrix matrixRet(NRows(), refmatrix.NCols());

    return matrixRet.EqProd(*this, refmatrix);
}

inline CVisDMatrix CVisDMatrix::operator*(double dbl) const
{
    CVisDMatrix matrixRet(*this);

    return (matrixRet *= dbl);
}

inline CVisDMatrix CVisDMatrix::operator/(double dbl) const
{
    CVisDMatrix matrixRet(*this);

    return (matrixRet /= dbl);
}


inline CVisDMatrix& CVisDMatrix::Transpose(void)
{
	*this = Transposed();

	return *this;
}

inline CVisDMatrix CVisDMatrix::Submatrix(const int newRows, const int newCols,
		const int i0, const int j0) const
{
	CVisDMatrix submat(newRows, newCols);
	Submatrix(newRows, newCols, submat, i0, j0);
	return submat;
}

inline CVisDVector VisCrossProduct(const CVisDVector &v1, const CVisDVector &v2)
{
	CVisDVector vOut;
	VisCrossProduct(v1, v2, vOut);
	return vOut;
}

//
// Self-describing stream output.
//
// LATER:  We should consider having both op<< and op>> with these streams.
//
#ifdef VIS_INCLUDE_SDSTREAM
inline CVisSDStream& operator<<(CVisSDStream& s, CVisDVector& o)
	{ return VisSDReadWriteClass(s, o); }

inline CVisSDObject<CVisDVector>& CVisSDObject<CVisDVector>::Exemplar(void)
{
	// Global variable defined in ..\VisImageProc\VisSDStreamExemplars.cpp
	extern CVisSDObject<CVisDVector> g_visdvectorExemplar;

	return g_visdvectorExemplar;
}

inline CVisSDStream& operator<<(CVisSDStream& s, CVisDMatrix& o)
	{ return VisSDReadWriteClass(s, o); }
inline CVisSDObject<CVisDMatrix>& CVisSDObject<CVisDMatrix>::Exemplar(void)
{
	// Global variable defined in ..\VisImageProc\VisSDStreamExemplars.cpp
	extern CVisSDObject<CVisDMatrix> g_visdmatrixExemplar;

	return g_visdmatrixExemplar;
}
#endif

#ifdef _DEBUG
#undef new
#endif // _DEBUG

