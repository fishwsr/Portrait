// VisLuciformWrappers.inl
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved


// CVisVector2TC

template<class TCoord>
inline CVisVector2TC<TCoord>::CVisVector2TC( void )
  : GXVector2TC<TCoord>()
{
}

template<class TCoord>
inline CVisVector2TC<TCoord>::CVisVector2TC( const TCoord& d )
  : GXVector2TC<TCoord>(d)
{
}

template<class TCoord>
inline CVisVector2TC<TCoord>::CVisVector2TC( const TCoord& du,
		const TCoord& dv )
  : GXVector2TC<TCoord>(du, dv)
{
}

template<class TCoord>
inline CVisVector2TC<TCoord>::CVisVector2TC(
		const GXVector2TC<TCoord>& refobj )
  : GXVector2TC<TCoord>(refobj)
{
}

template<class TCoord>
inline CVisVector2TC<TCoord>& CVisVector2TC<TCoord>::operator=(
		const GXVector2TC<TCoord>& refobj )
{
	GXVector2TC<TCoord>::operator=(refobj);
	return *this;
}

template<class TCoord>
inline CVisVector2TC<TCoord>::CVisVector2TC( const CVisDVector& refvector )
  : GXVector2TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1])
{
	assert((refvector.Length() > 1) && (refvector.Length() <= 4));
}

template<class TCoord>
inline CVisVector2TC<TCoord>& CVisVector2TC<TCoord>::operator=(
		const CVisDVector& refvector )
{
	assert((refvector.Length() > 1) && (refvector.Length() <= 4));
	du = (TCoord) refvector[0];
	dv = (TCoord) refvector[1];
	return *this;
}

template<class TCoord>
inline CVisVector2TC<TCoord>::CVisVector2TC( const CVisVector4& refvector )
  : GXVector2TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1])
{
}

template<class TCoord>
inline CVisVector2TC<TCoord>& CVisVector2TC<TCoord>::operator=(
		const CVisVector4& refvector )
{
	du = (TCoord) refvector[0];
	dv = (TCoord) refvector[1];
	return *this;
}


// CVisCoVector2TC

template<class TCoord>
inline CVisCoVector2TC<TCoord>::CVisCoVector2TC( void )
  : GXCoVector2TC<TCoord>()
{
}

template<class TCoord>
inline CVisCoVector2TC<TCoord>::CVisCoVector2TC( const TCoord& n )
  : GXCoVector2TC<TCoord>(n)
{
}

template<class TCoord>
inline CVisCoVector2TC<TCoord>::CVisCoVector2TC( const TCoord& nu,
		const TCoord& nv )
  : GXCoVector2TC<TCoord>(nu, nv)
{
}

template<class TCoord>
inline CVisCoVector2TC<TCoord>::CVisCoVector2TC(
		const GXCoVector2TC<TCoord>& refobj )
  : GXCoVector2TC<TCoord>(refobj)
{
}

template<class TCoord>
inline CVisCoVector2TC<TCoord>& CVisCoVector2TC<TCoord>::operator=(
		const GXCoVector2TC<TCoord>& refobj )
{
	GXCoVector2TC<TCoord>::operator=(refobj);
	return *this;
}

template<class TCoord>
inline CVisCoVector2TC<TCoord>::CVisCoVector2TC( const CVisDVector& refvector )
  : GXCoVector2TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1])
{
	assert((refvector.Length() > 1) && (refvector.Length() <= 4));
}

template<class TCoord>
inline CVisCoVector2TC<TCoord>& CVisCoVector2TC<TCoord>::operator=(
		const CVisDVector& refvector )
{
	assert((refvector.Length() > 1) && (refvector.Length() <= 4));
	nu = (TCoord) refvector[0];
	nv = (TCoord) refvector[1];
	return *this;
}

template<class TCoord>
inline CVisCoVector2TC<TCoord>::CVisCoVector2TC( const CVisVector4& refvector )
  : GXCoVector2TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1])
{
}

template<class TCoord>
inline CVisCoVector2TC<TCoord>& CVisCoVector2TC<TCoord>::operator=(
		const CVisVector4& refvector )
{
	nu = (TCoord) refvector[0];
	nv = (TCoord) refvector[1];
	return *this;
}


template<class TCoord>
inline TCoord CVisCoVector2TC<TCoord>::operator*(
		const GXVector2TC<TCoord>& refvector ) const
{
	return (*this)[0] * refvector[0] + (*this)[1] * refvector[1];
}

template<class TCoord>
inline CVisCoVector2TC<TCoord> CVisCoVector2TC<TCoord>::operator*(
		const GXMatrix2TC<TCoord>& refmatrix ) const
{
	CVisCoVector2TC<TCoord> covectorResult;

	covectorResult[0] = (*this)[0] * refmatrix[0][0] + (*this)[1] * refmatrix[1][0];
	covectorResult[1] = (*this)[0] * refmatrix[0][1] + (*this)[1] * refmatrix[1][1];

	return covectorResult;
}

template<class TCoord>
inline CVisCoVector2TC<TCoord>& CVisCoVector2TC<TCoord>::operator*=(
		const GXMatrix2TC<TCoord>& refmatrix )
{
	TCoord coordT = (*this)[0] * refmatrix[0][0] + (*this)[1] * refmatrix[1][0];
	(*this)[1] = (*this)[0] * refmatrix[0][1] + (*this)[1] * refmatrix[1][1];
	(*this)[0] = coordT;

	return *this;
}


// CVisMatrix2TC

template<class TCoord>
inline CVisMatrix2TC<TCoord>::CVisMatrix2TC( void )
  : GXMatrix2TC<TCoord>()
{
}

template<class TCoord>
inline CVisMatrix2TC<TCoord>::CVisMatrix2TC( const TCoord& s )
  : GXMatrix2TC<TCoord>(s)
{
}

template<class TCoord>
inline CVisMatrix2TC<TCoord>::CVisMatrix2TC( const GXVector2TC<TCoord>& vCol1,
		const GXVector2TC<TCoord>& vCol2 )
  : GXMatrix2TC<TCoord>(vCol1, vCol2)
{
}

template<class TCoord>
inline CVisMatrix2TC<TCoord>::CVisMatrix2TC(
		const GXCoVector2TC<TCoord>& covRow1,
		const GXCoVector2TC<TCoord>& covRow2 )
  : GXMatrix2TC<TCoord>(covRow1, covRow2)
{
}

template<class TCoord>
inline CVisMatrix2TC<TCoord>::CVisMatrix2TC(
		const GXMatrix2TC<TCoord>& refobj )
  : GXMatrix2TC<TCoord>(refobj)
{
}

template<class TCoord>
inline CVisMatrix2TC<TCoord>& CVisMatrix2TC<TCoord>::operator=(
		const GXMatrix2TC<TCoord>& refobj )
{
	GXMatrix2TC<TCoord>::operator=(refobj);
	return *this;
}

template<class TCoord>
inline CVisMatrix2TC<TCoord> CVisMatrix2TC<TCoord>::Transposed(void) const
{
	CVisDMatrix dmatrix(*this);
	return CVisMatrix2TC<TCoord>(dmatrix.Transpose());
}

template<class TCoord>
inline CVisMatrix2TC<TCoord>& CVisMatrix2TC<TCoord>::Transpose(void)
{
	CVisDMatrix dmatrix(*this);
	*this = dmatrix.Transpose();
	return *this;
}

template<class TCoord>
inline CVisMatrix2TC<TCoord>& CVisMatrix2TC<TCoord>::Transpose(
		const CVisMatrix2TC<TCoord>& refmatrix)
{
	CVisDMatrix dmatrix(refmatrix);
	*this = dmatrix.Transpose();
	return *this;
}

template<class TCoord>
inline CVisMatrix2TC<TCoord> CVisMatrix2TC<TCoord>::Inverted(void) const
{
	CVisDMatrix dmatrix(*this);
	return CVisMatrix2TC<TCoord>(dmatrix.Invert());
}

template<class TCoord>
inline CVisMatrix2TC<TCoord>& CVisMatrix2TC<TCoord>::Inverse(void)
{
	CVisDMatrix dmatrix(*this);
	*this = dmatrix.Invert();
	return *this;
}

template<class TCoord>
inline CVisMatrix2TC<TCoord>& CVisMatrix2TC<TCoord>::Inverse(
		const CVisMatrix2TC<TCoord>& refmatrix)
{
	CVisDMatrix dmatrix(refmatrix);
	*this = dmatrix.Invert();
	return *this;
}

template<class TCoord>
inline CVisMatrix2TC<TCoord>::CVisMatrix2TC( const CVisDMatrix& refmatrix )
{
	CopyMatrix(refmatrix);
}

template<class TCoord>
inline CVisMatrix2TC<TCoord>& CVisMatrix2TC<TCoord>::operator=(
		const CVisDMatrix& refmatrix )
{
	CopyMatrix(refmatrix);
	return *this;
}

template<class TCoord>
inline CVisMatrix2TC<TCoord>::CVisMatrix2TC( const CVisTransform4x4& refmatrix )
{
	CopyMatrix(refmatrix);
}

template<class TCoord>
inline CVisMatrix2TC<TCoord>& CVisMatrix2TC<TCoord>::operator=(
		const CVisTransform4x4& refmatrix )
{
	CopyMatrix(refmatrix);
	return *this;
}

template<class TCoord>
inline void CVisMatrix2TC<TCoord>::CopyMatrix( const CVisDMatrix& refmatrix )
{
	assert((refmatrix.NRows() >= 2) && (refmatrix.NRows() <= 4));
	assert((refmatrix.NCols() >= 2) && (refmatrix.NCols() <= 4));
	for (int iRow = 0; iRow < 2; ++iRow)
	{
		const double *pdblRowSrc = refmatrix[iRow];
		TCoord *ptcRowDest = mat[iRow];
		for (int iCol = 0; iCol < 2; ++iCol)
			ptcRowDest[iCol] = (TCoord) pdblRowSrc[iCol];
	}
}

template<class TCoord>
inline void CVisMatrix2TC<TCoord>::CopyMatrix( const CVisTransform4x4& refmatrix )
{
	for (int iRow = 0; iRow < 2; ++iRow)
	{
		const double *pdblRowSrc = refmatrix[iRow];
		TCoord *ptcRowDest = mat[iRow];
		for (int iCol = 0; iCol < 2; ++iCol)
			ptcRowDest[iCol] = (TCoord) pdblRowSrc[iCol];
	}
}


template<class TCoord>
inline CVisVector2TC<TCoord> CVisMatrix2TC<TCoord>::operator*(
		const GXVector2TC<TCoord>& refvector ) const
{
	CVisVector2TC<TCoord> vectorResult;

	vectorResult[0] = (*this)[0][0] * refvector[0] + (*this)[0][1] * refvector[1];
	vectorResult[1] = (*this)[1][0] * refvector[0] + (*this)[1][1] * refvector[1];

	return vectorResult;
}


// CVisPoint2TC

template<class TCoord>
inline CVisPoint2TC<TCoord>::CVisPoint2TC( void )
  : GXPoint2TC<TCoord>()
{
}

#ifdef LATER // This ctor seems to be missing from the Luciform header files
template<class TCoord>
inline CVisPoint2TC<TCoord>::CVisPoint2TC( const TCoord& x )
  : GXPoint2TC<TCoord>(x)
{
}
#endif // LATER

template<class TCoord>
inline CVisPoint2TC<TCoord>::CVisPoint2TC( const TCoord& u, const TCoord& v )
  : GXPoint2TC<TCoord>(u, v)
{
}

template<class TCoord>
inline CVisPoint2TC<TCoord>::CVisPoint2TC(
		const GXPoint2TC<TCoord>& refobj )
  : GXPoint2TC<TCoord>(refobj)
{
}

template<class TCoord>
inline CVisPoint2TC<TCoord>& CVisPoint2TC<TCoord>::operator=(
		const GXPoint2TC<TCoord>& refobj )
{
	GXPoint2TC<TCoord>::operator=(refobj);
	return *this;
}

template<class TCoord>
inline CVisPoint2TC<TCoord>::CVisPoint2TC( const CVisDVector& refvector )
  : GXPoint2TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1])
{
	assert((refvector.Length() > 1) && (refvector.Length() <= 4));
}

template<class TCoord>
inline CVisPoint2TC<TCoord>& CVisPoint2TC<TCoord>::operator=(
		const CVisDVector& refvector )
{
	assert((refvector.Length() > 1) && (refvector.Length() <= 4));
	u = (TCoord) refvector[0];
	v = (TCoord) refvector[1];
	return *this;
}

template<class TCoord>
inline CVisPoint2TC<TCoord>::CVisPoint2TC( const CVisVector4& refvector )
  : GXPoint2TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1])
{
}

template<class TCoord>
inline CVisPoint2TC<TCoord>& CVisPoint2TC<TCoord>::operator=(
		const CVisVector4& refvector )
{
	u = (TCoord) refvector[0];
	v = (TCoord) refvector[1];
	return *this;
}


// CVisVector3TC

template<class TCoord>
inline CVisVector3TC<TCoord>::CVisVector3TC( void )
  : GXVector3TC<TCoord>()
{
}

template<class TCoord>
inline CVisVector3TC<TCoord>::CVisVector3TC( const TCoord& d )
  : GXVector3TC<TCoord>(d)
{
}

template<class TCoord>
inline CVisVector3TC<TCoord>::CVisVector3TC( const TCoord& dx, const TCoord& dy,
		const TCoord& dz )
  : GXVector3TC<TCoord>(dx, dy, dz)
{
}

template<class TCoord>
inline CVisVector3TC<TCoord>::CVisVector3TC(
		const GXVector3TC<TCoord>& refobj )
  : GXVector3TC<TCoord>(refobj)
{
}

template<class TCoord>
inline CVisVector3TC<TCoord>& CVisVector3TC<TCoord>::operator=(
		const GXVector3TC<TCoord>& refobj )
{
	GXVector3TC<TCoord>::operator=(refobj);
	return *this;
}

template<class TCoord>
inline CVisVector3TC<TCoord>::CVisVector3TC( const CVisDVector& refvector )
  : GXVector3TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1],
		(TCoord) refvector[2])
{
	assert((refvector.Length() > 2) && (refvector.Length() <= 4));
}

template<class TCoord>
inline CVisVector3TC<TCoord>& CVisVector3TC<TCoord>::operator=(
		const CVisDVector& refvector )
{
	assert((refvector.Length() > 2) && (refvector.Length() <= 4));
	dx = (TCoord) refvector[0];
	dy = (TCoord) refvector[1];
	dz = (TCoord) refvector[2];
	return *this;
}

template<class TCoord>
inline CVisVector3TC<TCoord>::CVisVector3TC( const CVisVector4& refvector )
  : GXVector3TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1],
		(TCoord) refvector[2])
{
}

template<class TCoord>
inline CVisVector3TC<TCoord>& CVisVector3TC<TCoord>::operator=(
		const CVisVector4& refvector )
{
	dx = (TCoord) refvector[0];
	dy = (TCoord) refvector[1];
	dz = (TCoord) refvector[2];
	return *this;
}


// CVisCoVector3TC

template<class TCoord>
inline CVisCoVector3TC<TCoord>::CVisCoVector3TC( void )
  : GXCoVector3TC<TCoord>()
{
}

template<class TCoord>
inline CVisCoVector3TC<TCoord>::CVisCoVector3TC( const TCoord& n )
  : GXCoVector3TC<TCoord>(n)
{
}

template<class TCoord>
inline CVisCoVector3TC<TCoord>::CVisCoVector3TC( const TCoord& nx,
		const TCoord& ny, const TCoord& nz )
  : GXCoVector3TC<TCoord>(nx, ny, nz)
{
}

template<class TCoord>
inline CVisCoVector3TC<TCoord>::CVisCoVector3TC(
		const GXCoVector3TC<TCoord>& refobj )
  : GXCoVector3TC<TCoord>(refobj)
{
}

template<class TCoord>
inline CVisCoVector3TC<TCoord>& CVisCoVector3TC<TCoord>::operator=(
		const GXCoVector3TC<TCoord>& refobj )
{
	GXCoVector3TC<TCoord>::operator=(refobj);
	return *this;
}

template<class TCoord>
inline CVisCoVector3TC<TCoord>::CVisCoVector3TC( const CVisDVector& refvector )
  : GXCoVector3TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1],
		(TCoord) refvector[2])
{
	assert((refvector.Length() > 2) && (refvector.Length() <= 4));
}

template<class TCoord>
inline CVisCoVector3TC<TCoord>& CVisCoVector3TC<TCoord>::operator=(
		const CVisDVector& refvector )
{
	assert((refvector.Length() > 2) && (refvector.Length() <= 4));
	nx = (TCoord) refvector[0];
	ny = (TCoord) refvector[1];
	nz = (TCoord) refvector[2];
	return *this;
}

template<class TCoord>
inline CVisCoVector3TC<TCoord>::CVisCoVector3TC( const CVisVector4& refvector )
  : GXCoVector3TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1],
		(TCoord) refvector[2])
{
}

template<class TCoord>
inline CVisCoVector3TC<TCoord>& CVisCoVector3TC<TCoord>::operator=(
		const CVisVector4& refvector )
{
	nx = (TCoord) refvector[0];
	ny = (TCoord) refvector[1];
	nz = (TCoord) refvector[2];
	return *this;
}


template<class TCoord>
inline TCoord CVisCoVector3TC<TCoord>::operator*(
		const GXVector3TC<TCoord>& refvector ) const
{
	return (*this)[0] * refvector[0] + (*this)[1] * refvector[1]
			+ (*this)[2] * refvector[2];
}

template<class TCoord>
inline CVisCoVector3TC<TCoord> CVisCoVector3TC<TCoord>::operator*(
		const GXMatrix3TC<TCoord>& refmatrix ) const
{
	CVisCoVector3TC<TCoord> covectorResult;

	covectorResult[0] = (*this)[0] * refmatrix[0][0] + (*this)[1] * refmatrix[1][0]
			+ (*this)[2] * refmatrix[2][0];
	covectorResult[1] = (*this)[0] * refmatrix[0][1] + (*this)[1] * refmatrix[1][1]
			+ (*this)[2] * refmatrix[2][1];
	covectorResult[2] = (*this)[0] * refmatrix[0][2] + (*this)[1] * refmatrix[1][2]
			+ (*this)[2] * refmatrix[2][2];

	return covectorResult;
}

template<class TCoord>
inline CVisCoVector3TC<TCoord>& CVisCoVector3TC<TCoord>::operator*=(
		const GXMatrix3TC<TCoord>& refmatrix )
{
	TCoord coordT0 = (*this)[0] * refmatrix[0][0] + (*this)[1] * refmatrix[1][0]
			+ (*this)[2] * refmatrix[2][0];
	TCoord coordT1 = (*this)[0] * refmatrix[0][1] + (*this)[1] * refmatrix[1][1]
			+ (*this)[2] * refmatrix[2][1];
	(*this)[2] = (*this)[0] * refmatrix[0][2] + (*this)[1] * refmatrix[1][2]
			+ (*this)[2] * refmatrix[2][2];
	(*this)[0] = coordT0;
	(*this)[1] = coordT1;

	return *this;
}


// CVisMatrix3TC

template<class TCoord>
inline CVisMatrix3TC<TCoord>::CVisMatrix3TC( void )
  : GXMatrix3TC<TCoord>()
{
}

template<class TCoord>
inline CVisMatrix3TC<TCoord>::CVisMatrix3TC( const TCoord& s )
  : GXMatrix3TC<TCoord>(s)
{
}

template<class TCoord>
inline CVisMatrix3TC<TCoord>::CVisMatrix3TC( const GXVector3TC<TCoord>& vCol1,
		const GXVector3TC<TCoord>& vCol2,
		const GXVector3TC<TCoord>& vCol3 )
  : GXMatrix3TC<TCoord>(vCol1, vCol2, vCol3)
{
}

template<class TCoord>
inline CVisMatrix3TC<TCoord>::CVisMatrix3TC(
		const GXCoVector3TC<TCoord>& covRow1,
		const GXCoVector3TC<TCoord>& covRow2,
		const GXCoVector3TC<TCoord>& covRow3 )
  : GXMatrix3TC<TCoord>(covRow1, covRow2, covRow3)
{
}

template<class TCoord>
inline CVisMatrix3TC<TCoord>::CVisMatrix3TC(
		const GXMatrix3TC<TCoord>& refobj )
  : GXMatrix3TC<TCoord>(refobj)
{
}

template<class TCoord>
inline CVisMatrix3TC<TCoord>& CVisMatrix3TC<TCoord>::operator=(
		const GXMatrix3TC<TCoord>& refobj )
{
	GXMatrix3TC<TCoord>::operator=(refobj);
	return *this;
}

template<class TCoord>
inline CVisMatrix3TC<TCoord>::CVisMatrix3TC( const CVisDMatrix& refmatrix )
{
	CopyMatrix(refmatrix);
}

template<class TCoord>
inline CVisMatrix3TC<TCoord>& CVisMatrix3TC<TCoord>::operator=(
		const CVisDMatrix& refmatrix )
{
	CopyMatrix(refmatrix);
	return *this;
}

template<class TCoord>
inline CVisMatrix3TC<TCoord>::CVisMatrix3TC( const CVisTransform4x4& refmatrix )
{
	CopyMatrix(refmatrix);
}

template<class TCoord>
inline CVisMatrix3TC<TCoord>& CVisMatrix3TC<TCoord>::operator=(
		const CVisTransform4x4& refmatrix )
{
	CopyMatrix(refmatrix);
	return *this;
}

template<class TCoord>
inline CVisVector3TC<TCoord> CVisMatrix3TC<TCoord>::operator*(
		const GXVector3TC<TCoord>& refvector ) const
{
	CVisVector3TC<TCoord> vectorResult;

	vectorResult[0] = (*this)[0][0] * refvector[0] + (*this)[0][1] * refvector[1]
			+ (*this)[0][2] * refvector[2];
	vectorResult[1] = (*this)[1][0] * refvector[0] + (*this)[1][1] * refvector[1]
			+ (*this)[1][2] * refvector[2];
	vectorResult[2] = (*this)[2][0] * refvector[0] + (*this)[2][1] * refvector[1]
			+ (*this)[2][2] * refvector[2];

	return vectorResult;
}

template<class TCoord>
inline CVisMatrix3TC<TCoord> CVisMatrix3TC<TCoord>::Transposed(void) const
{
	CVisDMatrix dmatrix(*this);
	return CVisMatrix3TC<TCoord>(dmatrix.Transpose());
}

template<class TCoord>
inline CVisMatrix3TC<TCoord>& CVisMatrix3TC<TCoord>::Transpose(void)
{
	CVisDMatrix dmatrix(*this);
	*this = dmatrix.Transpose();
	return *this;
}

template<class TCoord>
inline CVisMatrix3TC<TCoord>& CVisMatrix3TC<TCoord>::Transpose(
		const CVisMatrix3TC<TCoord>& refmatrix)
{
	CVisDMatrix dmatrix(refmatrix);
	*this = dmatrix.Transpose();
	return *this;
}

template<class TCoord>
inline CVisMatrix3TC<TCoord> CVisMatrix3TC<TCoord>::Inverted(void) const
{
	CVisDMatrix dmatrix(*this);
	return CVisMatrix3TC<TCoord>(dmatrix.Invert());
}

template<class TCoord>
inline CVisMatrix3TC<TCoord>& CVisMatrix3TC<TCoord>::Inverse(void)
{
	CVisDMatrix dmatrix(*this);
	*this = dmatrix.Invert();
	return *this;
}

template<class TCoord>
inline CVisMatrix3TC<TCoord>& CVisMatrix3TC<TCoord>::Inverse(
		const CVisMatrix3TC<TCoord>& refmatrix)
{
	CVisDMatrix dmatrix(refmatrix);
	*this = dmatrix.Invert();
	return *this;
}

template<class TCoord>
inline void CVisMatrix3TC<TCoord>::CopyMatrix( const CVisDMatrix& refmatrix )
{
	assert((refmatrix.NRows() >= 3) && (refmatrix.NRows() <= 4));
	assert((refmatrix.NCols() >= 3) && (refmatrix.NCols() <= 4));
	for (int iRow = 0; iRow < 3; ++iRow)
	{
		const double *pdblRowSrc = refmatrix[iRow];
		TCoord *ptcRowDest = mat[iRow];
		for (int iCol = 0; iCol < 3; ++iCol)
			ptcRowDest[iCol] = (TCoord) pdblRowSrc[iCol];
	}
}

template<class TCoord>
inline void CVisMatrix3TC<TCoord>::CopyMatrix( const CVisTransform4x4& refmatrix )
{
	for (int iRow = 0; iRow < 3; ++iRow)
	{
		const double *pdblRowSrc = refmatrix[iRow];
		TCoord *ptcRowDest = mat[iRow];
		for (int iCol = 0; iCol < 3; ++iCol)
			ptcRowDest[iCol] = (TCoord) pdblRowSrc[iCol];
	}
}


// CVisPoint3TC

template<class TCoord>
inline CVisPoint3TC<TCoord>::CVisPoint3TC( void )
  : GXPoint3TC<TCoord>()
{
}

template<class TCoord>
inline CVisPoint3TC<TCoord>::CVisPoint3TC( const TCoord& x )
  : GXPoint3TC<TCoord>(x)
{
}

template<class TCoord>
inline CVisPoint3TC<TCoord>::CVisPoint3TC( const TCoord& x,
		const TCoord& y, const TCoord& z )
  : GXPoint3TC<TCoord>(x, y, z)
{
}

template<class TCoord>
inline CVisPoint3TC<TCoord>::CVisPoint3TC(
		const GXPoint3TC<TCoord>& refobj )
  : GXPoint3TC<TCoord>(refobj)
{
}

template<class TCoord>
inline CVisPoint3TC<TCoord>& CVisPoint3TC<TCoord>::operator=(
		const GXPoint3TC<TCoord>& refobj )
{
	GXPoint3TC<TCoord>::operator=(refobj);
	return *this;
}

template<class TCoord>
inline CVisPoint3TC<TCoord>::CVisPoint3TC( const CVisDVector& refvector )
  : GXPoint3TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1],
		(TCoord) refvector[2])
{
	assert((refvector.Length() > 2) && (refvector.Length() <= 4));
}

template<class TCoord>
inline CVisPoint3TC<TCoord>& CVisPoint3TC<TCoord>::operator=(
		const CVisDVector& refvector )
{
	assert((refvector.Length() > 2) && (refvector.Length() <= 4));
	x = (TCoord) refvector[0];
	y = (TCoord) refvector[1];
	z = (TCoord) refvector[2];
	return *this;
}

template<class TCoord>
inline CVisPoint3TC<TCoord>::CVisPoint3TC( const CVisVector4& refvector )
  : GXPoint3TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1],
		(TCoord) refvector[2])
{
}

template<class TCoord>
inline CVisPoint3TC<TCoord>& CVisPoint3TC<TCoord>::operator=(
		const CVisVector4& refvector )
{
	x = (TCoord) refvector[0];
	y = (TCoord) refvector[1];
	z = (TCoord) refvector[2];
	return *this;
}



// CVisVector4TC

template<class TCoord>
inline CVisVector4TC<TCoord>::CVisVector4TC( void )
  : GXVector4TC<TCoord>()
{
}

template<class TCoord>
inline CVisVector4TC<TCoord>::CVisVector4TC( const TCoord& d )
  : GXVector4TC<TCoord>(d)
{
}

template<class TCoord>
inline CVisVector4TC<TCoord>::CVisVector4TC( const TCoord& dx, const TCoord& dy,
		const TCoord& dz,
	const TCoord& dw )
  : GXVector4TC<TCoord>(dx, dy, dz, dw)
{
}

template<class TCoord>
inline CVisVector4TC<TCoord>::CVisVector4TC(
		const GXVector4TC<TCoord>& refobj )
  : GXVector4TC<TCoord>(refobj)
{
}

template<class TCoord>
inline CVisVector4TC<TCoord>& CVisVector4TC<TCoord>::operator=(
		const GXVector4TC<TCoord>& refobj )
{
	GXVector4TC<TCoord>::operator=(refobj);
	return *this;
}

template<class TCoord>
inline CVisVector4TC<TCoord>::CVisVector4TC( const CVisDVector& refvector )
  : GXVector4TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1],
		(TCoord) refvector[2], (TCoord) refvector[3])
{
	assert(refvector.Length() == 4);
}

template<class TCoord>
inline CVisVector4TC<TCoord>& CVisVector4TC<TCoord>::operator=(
		const CVisDVector& refvector )
{
	assert(refvector.Length() == 4);
	dx = (TCoord) refvector[0];
	dy = (TCoord) refvector[1];
	dz = (TCoord) refvector[2];
	dw = (TCoord) refvector[3];
	return *this;
}

template<class TCoord>
inline CVisVector4TC<TCoord>::CVisVector4TC( const CVisVector4& refvector )
  : GXVector4TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1],
		(TCoord) refvector[2], (TCoord) refvector[3])
{
}

template<class TCoord>
inline CVisVector4TC<TCoord>& CVisVector4TC<TCoord>::operator=(
		const CVisVector4& refvector )
{
	dx = (TCoord) refvector[0];
	dy = (TCoord) refvector[1];
	dz = (TCoord) refvector[2];
	dw = (TCoord) refvector[3];
	return *this;
}


// CVisCoVector4TC

template<class TCoord>
inline CVisCoVector4TC<TCoord>::CVisCoVector4TC( void )
  : GXCoVector4TC<TCoord>()
{
}

template<class TCoord>
inline CVisCoVector4TC<TCoord>::CVisCoVector4TC( const TCoord& n )
  : GXCoVector4TC<TCoord>(n)
{
}

template<class TCoord>
inline CVisCoVector4TC<TCoord>::CVisCoVector4TC( const TCoord& nx,
		const TCoord& ny, const TCoord& nz,
	const TCoord& nw )
  : GXCoVector4TC<TCoord>(nx, ny, nz, nw)
{
}

template<class TCoord>
inline CVisCoVector4TC<TCoord>::CVisCoVector4TC(
		const GXCoVector4TC<TCoord>& refobj )
  : GXCoVector4TC<TCoord>(refobj)
{
}

template<class TCoord>
inline CVisCoVector4TC<TCoord>& CVisCoVector4TC<TCoord>::operator=(
		const GXCoVector4TC<TCoord>& refobj )
{
	GXCoVector4TC<TCoord>::operator=(refobj);
	return *this;
}

template<class TCoord>
inline CVisCoVector4TC<TCoord>::CVisCoVector4TC( const CVisDVector& refvector )
  : GXCoVector4TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1],
		(TCoord) refvector[2], (TCoord) refvector[3])
{
	assert(refvector.Length() == 4);
}

template<class TCoord>
inline CVisCoVector4TC<TCoord>& CVisCoVector4TC<TCoord>::operator=(
		const CVisDVector& refvector )
{
	assert(refvector.Length() == 4);
	nx = (TCoord) refvector[0];
	ny = (TCoord) refvector[1];
	nz = (TCoord) refvector[2];
	nw = (TCoord) refvector[3];
	return *this;
}

template<class TCoord>
inline CVisCoVector4TC<TCoord>::CVisCoVector4TC( const CVisVector4& refvector )
  : GXCoVector4TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1],
		(TCoord) refvector[2], (TCoord) refvector[3])
{
}

template<class TCoord>
inline CVisCoVector4TC<TCoord>& CVisCoVector4TC<TCoord>::operator=(
		const CVisVector4& refvector )
{
	nx = (TCoord) refvector[0];
	ny = (TCoord) refvector[1];
	nz = (TCoord) refvector[2];
	nw = (TCoord) refvector[3];
	return *this;
}


template<class TCoord>
inline TCoord CVisCoVector4TC<TCoord>::operator*(
		const GXVector4TC<TCoord>& refvector ) const
{
	return (*this)[0] * refvector[0] + (*this)[1] * refvector[1]
			+ (*this)[2] * refvector[2] + (*this)[3] * refvector[3];
}

template<class TCoord>
inline CVisCoVector4TC<TCoord> CVisCoVector4TC<TCoord>::operator*(
		const GXMatrix4TC<TCoord>& refmatrix ) const
{
	CVisCoVector4TC<TCoord> covectorResult;

	covectorResult[0] = (*this)[0] * refmatrix[0][0] + (*this)[1] * refmatrix[1][0]
			+ (*this)[2] * refmatrix[2][0] + (*this)[3] * refmatrix[3][0];
	covectorResult[1] = (*this)[0] * refmatrix[0][1] + (*this)[1] * refmatrix[1][1]
			+ (*this)[2] * refmatrix[2][1] + (*this)[3] * refmatrix[3][1];
	covectorResult[2] = (*this)[0] * refmatrix[0][2] + (*this)[1] * refmatrix[1][2]
			+ (*this)[2] * refmatrix[2][2] + (*this)[3] * refmatrix[3][2];
	covectorResult[3] = (*this)[0] * refmatrix[0][3] + (*this)[1] * refmatrix[1][3]
			+ (*this)[2] * refmatrix[2][3] + (*this)[3] * refmatrix[3][3];

	return covectorResult;
}

template<class TCoord>
inline CVisCoVector4TC<TCoord>& CVisCoVector4TC<TCoord>::operator*=(
		const GXMatrix4TC<TCoord>& refmatrix )
{
	*this = ((*this) * refmatrix);
	return *this;
}


// CVisMatrix4TC

template<class TCoord>
inline CVisMatrix4TC<TCoord>::CVisMatrix4TC( void )
  : GXMatrix4TC<TCoord>()
{
}

template<class TCoord>
inline CVisMatrix4TC<TCoord>::CVisMatrix4TC( const TCoord& s )
  : GXMatrix4TC<TCoord>(s)
{
}

template<class TCoord>
inline CVisMatrix4TC<TCoord>::CVisMatrix4TC( const GXVector4TC<TCoord>& vCol1,
		const GXVector4TC<TCoord>& vCol2,
		const GXVector4TC<TCoord>& vCol3,
		const GXVector4TC<TCoord>& vCol4 )
  : GXMatrix4TC<TCoord>(vCol1, vCol2, vCol3, vCol4)
{
}

template<class TCoord>
inline CVisMatrix4TC<TCoord>::CVisMatrix4TC(
		const GXCoVector4TC<TCoord>& covRow1,
		const GXCoVector4TC<TCoord>& covRow2,
		const GXCoVector4TC<TCoord>& covRow3,
		const GXCoVector4TC<TCoord>& covRow4 )
  : GXMatrix4TC<TCoord>(covRow1, covRow2, covRow3, covRow4)
{
}

template<class TCoord>
inline CVisMatrix4TC<TCoord>::CVisMatrix4TC(
		const GXMatrix4TC<TCoord>& refobj )
  : GXMatrix4TC<TCoord>(refobj)
{
}

template<class TCoord>
inline CVisMatrix4TC<TCoord>& CVisMatrix4TC<TCoord>::operator=(
		const GXMatrix4TC<TCoord>& refobj )
{
	GXMatrix4TC<TCoord>::operator=(refobj);
	return *this;
}

template<class TCoord>
inline CVisMatrix4TC<TCoord>::CVisMatrix4TC( const CVisDMatrix& refmatrix )
{
	CopyMatrix(refmatrix);
}

template<class TCoord>
inline CVisMatrix4TC<TCoord>& CVisMatrix4TC<TCoord>::operator=(
		const CVisDMatrix& refmatrix )
{
	CopyMatrix(refmatrix);
	return *this;
}

template<class TCoord>
inline CVisMatrix4TC<TCoord>::CVisMatrix4TC( const CVisTransform4x4& refmatrix )
{
	CopyMatrix(refmatrix);
}

template<class TCoord>
inline CVisMatrix4TC<TCoord>& CVisMatrix4TC<TCoord>::operator=(
		const CVisTransform4x4& refmatrix )
{
	CopyMatrix(refmatrix);
	return *this;
}

template<class TCoord>
inline CVisVector4TC<TCoord> CVisMatrix4TC<TCoord>::operator*(
		const GXVector4TC<TCoord>& refvector ) const
{
	CVisVector4TC<TCoord> vectorResult;

	vectorResult[0] = (*this)[0][0] * refvector[0] + (*this)[0][1] * refvector[1]
			+ (*this)[0][2] * refvector[2] + (*this)[0][3] * refvector[3];
	vectorResult[1] = (*this)[1][0] * refvector[0] + (*this)[1][1] * refvector[1]
			+ (*this)[1][2] * refvector[2] + (*this)[1][3] * refvector[3];
	vectorResult[2] = (*this)[2][0] * refvector[0] + (*this)[2][1] * refvector[1]
			+ (*this)[2][2] * refvector[2] + (*this)[2][3] * refvector[3];
	vectorResult[3] = (*this)[3][0] * refvector[0] + (*this)[3][1] * refvector[1]
			+ (*this)[3][2] * refvector[2] + (*this)[3][3] * refvector[3];

	return vectorResult;
}

template<class TCoord>
inline CVisMatrix4TC<TCoord> CVisMatrix4TC<TCoord>::Transposed(void) const
{
	CVisDMatrix dmatrix(*this);
	return CVisMatrix4TC<TCoord>(dmatrix.Transpose());
}

template<class TCoord>
inline CVisMatrix4TC<TCoord>& CVisMatrix4TC<TCoord>::Transpose(void)
{
	CVisDMatrix dmatrix(*this);
	*this = dmatrix.Transpose();
	return *this;
}

template<class TCoord>
inline CVisMatrix4TC<TCoord>& CVisMatrix4TC<TCoord>::Transpose(
		const CVisMatrix4TC<TCoord>& refmatrix)
{
	CVisDMatrix dmatrix(refmatrix);
	*this = dmatrix.Transpose();
	return *this;
}

template<class TCoord>
inline CVisMatrix4TC<TCoord> CVisMatrix4TC<TCoord>::Inverted(void) const
{
	CVisDMatrix dmatrix(*this);
	return CVisMatrix4TC<TCoord>(dmatrix.Invert());
}

template<class TCoord>
inline CVisMatrix4TC<TCoord>& CVisMatrix4TC<TCoord>::Inverse(void)
{
	CVisDMatrix dmatrix(*this);
	*this = dmatrix.Invert();
	return *this;
}

template<class TCoord>
inline CVisMatrix4TC<TCoord>& CVisMatrix4TC<TCoord>::Inverse(
		const CVisMatrix4TC<TCoord>& refmatrix)
{
	CVisDMatrix dmatrix(refmatrix);
	*this = dmatrix.Invert();
	return *this;
}

template<class TCoord>
inline void CVisMatrix4TC<TCoord>::CopyMatrix( const CVisDMatrix& refmatrix )
{
	assert(refmatrix.NRows() == 4);
	assert(refmatrix.NCols() == 4);
	for (int iRow = 0; iRow < 4; ++iRow)
	{
		const double *pdblRowSrc = refmatrix[iRow];
		TCoord *ptcRowDest = mat[iRow];
		for (int iCol = 0; iCol < 4; ++iCol)
			ptcRowDest[iCol] = (TCoord) pdblRowSrc[iCol];
	}
}

template<class TCoord>
inline void CVisMatrix4TC<TCoord>::CopyMatrix( const CVisTransform4x4& refmatrix )
{
	for (int iRow = 0; iRow < 4; ++iRow)
	{
		const double *pdblRowSrc = refmatrix[iRow];
		TCoord *ptcRowDest = mat[iRow];
		for (int iCol = 0; iCol < 4; ++iCol)
			ptcRowDest[iCol] = (TCoord) pdblRowSrc[iCol];
	}
}


// CVisPoint4TC

template<class TCoord>
inline CVisPoint4TC<TCoord>::CVisPoint4TC( void )
  : GXPoint4TC<TCoord>()
{
}

template<class TCoord>
inline CVisPoint4TC<TCoord>::CVisPoint4TC( const TCoord& x )
  : GXPoint4TC<TCoord>(x)
{
}

template<class TCoord>
inline CVisPoint4TC<TCoord>::CVisPoint4TC( const TCoord& x, const TCoord& y,
		const TCoord& z, const TCoord& w )
  : GXPoint4TC<TCoord>(x, y, z, w)
{
}

template<class TCoord>
inline CVisPoint4TC<TCoord>::CVisPoint4TC(
		const GXPoint4TC<TCoord>& refobj )
  : GXPoint4TC<TCoord>(refobj)
{
}

template<class TCoord>
inline CVisPoint4TC<TCoord>& CVisPoint4TC<TCoord>::operator=(
		const GXPoint4TC<TCoord>& refobj )
{
	GXPoint4TC<TCoord>::operator=(refobj);
	return *this;
}

template<class TCoord>
inline CVisPoint4TC<TCoord>::CVisPoint4TC( const CVisDVector& refvector )
  : GXPoint4TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1],
		(TCoord) refvector[2], (TCoord) refvector[3])
{
	assert(refvector.Length() == 4);
}

template<class TCoord>
inline CVisPoint4TC<TCoord>& CVisPoint4TC<TCoord>::operator=(
		const CVisDVector& refvector )
{
	assert(refvector.Length() == 4);
	x = (TCoord) refvector[0];
	y = (TCoord) refvector[1];
	z = (TCoord) refvector[2];
	w = (TCoord) refvector[3];
	return *this;
}

template<class TCoord>
inline CVisPoint4TC<TCoord>::CVisPoint4TC( const CVisVector4& refvector )
  : GXPoint4TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1],
		(TCoord) refvector[2], (TCoord) refvector[3])
{
}

template<class TCoord>
inline CVisPoint4TC<TCoord>& CVisPoint4TC<TCoord>::operator=(
		const CVisVector4& refvector )
{
	x = (TCoord) refvector[0];
	y = (TCoord) refvector[1];
	z = (TCoord) refvector[2];
	w = (TCoord) refvector[3];
	return *this;
}



// CVisHPoint2TC

template<class TCoord>
inline CVisHPoint2TC<TCoord>::CVisHPoint2TC( void )
  : GXHPoint2TC<TCoord>()
{
}

template<class TCoord>
inline CVisHPoint2TC<TCoord>::CVisHPoint2TC( const GXPoint2TC<TCoord>& p )
  : GXHPoint2TC<TCoord>(p)
{
}

template<class TCoord>
inline CVisHPoint2TC<TCoord>::CVisHPoint2TC( const TCoord& x, const TCoord& y,
		const TCoord& w )
  : GXHPoint2TC<TCoord>(x, y, w)
{
}

template<class TCoord>
inline CVisHPoint2TC<TCoord>::CVisHPoint2TC(
		const GXHPoint2TC<TCoord>& refobj )
  : GXHPoint2TC<TCoord>(refobj)
{
}

template<class TCoord>
inline CVisHPoint2TC<TCoord>& CVisHPoint2TC<TCoord>::operator=(
		const GXHPoint2TC<TCoord>& refobj )
{
	GXHPoint2TC<TCoord>::operator=(refobj);
	return *this;
}

template<class TCoord>
inline CVisHPoint2TC<TCoord>::CVisHPoint2TC( const CVisDVector& refvector )
  : GXHPoint2TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1],
		(TCoord) ((refvector.Length() > 2)
				? refvector[refvector.Length() - 1] : 1.0))
{
	assert((refvector.Length() > 1) && (refvector.Length() <= 4));
}

template<class TCoord>
inline CVisHPoint2TC<TCoord>& CVisHPoint2TC<TCoord>::operator=(
		const CVisDVector& refvector )
{
	assert((refvector.Length() > 1) && (refvector.Length() <= 4));
	u = (TCoord) refvector[0];
	v = (TCoord) refvector[1];
	w = (TCoord) ((refvector.Length() > 2)
			? refvector[refvector.Length() - 1] : 1.0));
	return *this;
}

template<class TCoord>
inline CVisHPoint2TC<TCoord>::CVisHPoint2TC( const CVisVector4& refvector )
  : GXHPoint2TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1],
		(TCoord) refvector[3])
{
}

template<class TCoord>
inline CVisHPoint2TC<TCoord>& CVisHPoint2TC<TCoord>::operator=(
		const CVisVector4& refvector )
{
	u = (TCoord) refvector[0];
	v = (TCoord) refvector[1];
	w = (TCoord) refvector[3];
	return *this;
}


// CVisHPoint3TC

template<class TCoord>
inline CVisHPoint3TC<TCoord>::CVisHPoint3TC( void )
  : GXHPoint3TC<TCoord>()
{
}

template<class TCoord>
inline CVisHPoint3TC<TCoord>::CVisHPoint3TC( const GXPoint3TC<TCoord>& p )
  : GXHPoint3TC<TCoord>(p)
{
}

template<class TCoord>
inline CVisHPoint3TC<TCoord>::CVisHPoint3TC( const TCoord& x, const TCoord& y,
		const TCoord& z, const TCoord& w )
  : GXHPoint3TC<TCoord>(x, y, z, w)
{
}

template<class TCoord>
inline CVisHPoint3TC<TCoord>::CVisHPoint3TC(
		const GXHPoint3TC<TCoord>& refobj )
  : GXHPoint3TC<TCoord>(refobj)
{
}

template<class TCoord>
inline CVisHPoint3TC<TCoord>& CVisHPoint3TC<TCoord>::operator=(
		const GXHPoint3TC<TCoord>& refobj )
{
	GXHPoint3TC<TCoord>::operator=(refobj);
	return *this;
}

template<class TCoord>
inline CVisHPoint3TC<TCoord>::CVisHPoint3TC( const CVisDVector& refvector )
  : GXHPoint3TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1],
		(TCoord) refvector[2],
		(TCoord) ((refvector.Length() == 4) ? refvector[3] : 1.0))
{
	assert((refvector.Length() == 3) || (refvector.Length() == 4));
}

template<class TCoord>
inline CVisHPoint3TC<TCoord>& CVisHPoint3TC<TCoord>::operator=(
		const CVisDVector& refvector )
{
	assert((refvector.Length() == 3) || (refvector.Length() == 4));
	x = (TCoord) refvector[0];
	y = (TCoord) refvector[1];
	z = (TCoord) refvector[2];
	w = (TCoord) ((refvector.Length() == 4) ? refvector[3] : 1.0);
	return *this;
}

template<class TCoord>
inline CVisHPoint3TC<TCoord>::CVisHPoint3TC( const CVisVector4& refvector )
  : GXHPoint3TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1],
		(TCoord) refvector[2], (TCoord) refvector[3])
{
}

template<class TCoord>
inline CVisHPoint3TC<TCoord>& CVisHPoint3TC<TCoord>::operator=(
		const CVisVector4& refvector )
{
	x = (TCoord) refvector[0];
	y = (TCoord) refvector[1];
	z = (TCoord) refvector[2];
	w = (TCoord) refvector[3];
	return *this;
}


// CVisHPlane3TC

template<class TCoord>
inline CVisHPlane3TC<TCoord>::CVisHPlane3TC( void )
  : GXHPlane3TC<TCoord>()
{
}

template<class TCoord>
inline CVisHPlane3TC<TCoord>::CVisHPlane3TC( const GXPoint3TC<TCoord>& p1,
		const GXPoint3TC<TCoord>& p2, const GXPoint3TC<TCoord>& p3 )
  : GXHPlane3TC<TCoord>(p1, p2, p3)
{
}

template<class TCoord>
inline CVisHPlane3TC<TCoord>::CVisHPlane3TC( const TCoord& x, const TCoord& y,
		const TCoord& z, const TCoord& w )
  : GXHPlane3TC<TCoord>(x, y, z, w)
{
}

template<class TCoord>
inline CVisHPlane3TC<TCoord>::CVisHPlane3TC(
		const GXHPlane3TC<TCoord>& refobj )
  : GXHPlane3TC<TCoord>(refobj)
{
}

template<class TCoord>
inline CVisHPlane3TC<TCoord>& CVisHPlane3TC<TCoord>::operator=(
		const GXHPlane3TC<TCoord>& refobj )
{
	GXHPlane3TC<TCoord>::operator=(refobj);
	return *this;
}

template<class TCoord>
inline CVisHPlane3TC<TCoord>::CVisHPlane3TC( const CVisDVector& refvector )
  : GXHPlane3TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1],
		(TCoord) refvector[2],
		(TCoord) ((refvector.Length() == 4) ? refvector[3] : 1.0))
{
	assert((refvector.Length() == 3) || (refvector.Length() == 4));
}

template<class TCoord>
inline CVisHPlane3TC<TCoord>& CVisHPlane3TC<TCoord>::operator=(
		const CVisDVector& refvector )
{
	assert((refvector.Length() == 3) || (refvector.Length() == 4));
	x = (TCoord) refvector[0];
	y = (TCoord) refvector[1];
	z = (TCoord) refvector[2];
	w = (TCoord) ((refvector.Length() == 4) ? refvector[3] : 1.0);
	return *this;
}

template<class TCoord>
inline CVisHPlane3TC<TCoord>::CVisHPlane3TC( const CVisVector4& refvector )
  : GXHPlane3TC<TCoord>((TCoord) refvector[0], (TCoord) refvector[1],
		(TCoord) refvector[2], (TCoord) refvector[3])
{
}

template<class TCoord>
inline CVisHPlane3TC<TCoord>& CVisHPlane3TC<TCoord>::operator=(
		const CVisVector4& refvector )
{
	x = (TCoord) refvector[0];
	y = (TCoord) refvector[1];
	z = (TCoord) refvector[2];
	w = (TCoord) refvector[3];
	return *this;
}


// CVisQuaternion

inline CVisQuaternion::CVisQuaternion( void )
  : GXQuaternion()
{
}

inline CVisQuaternion::CVisQuaternion( const GXVector3& v, float f )
  : GXQuaternion(v, f)
{
}

inline CVisQuaternion::CVisQuaternion( const GXQuaternion& refobj )
  : GXQuaternion(refobj)
{
}

template<class TCoord>
inline CVisQuaternion& CVisQuaternion::operator=( const GXQuaternion& refobj )
{
	GXQuaternion::operator=(refobj);
	return *this;
}



template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>::CVisMatrixChain(void)
  : m_deque()
	m_fValidProduct(false)
{
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>::CVisMatrixChain(const TMatrix& refmatrix)
  : m_deque()
	m_fValidProduct(false)
{
	Assign(refmatrix);
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::operator=(
		const TMatrix& refmatrix)
{
	Assign(refmatrix);
	return *this;
}

template<class TMatrix, class _A>
inline int CVisMatrixChain<TMatrix, _A>::Capacity(void) const
{
	return (int) m_deque.capacity();
}

template<class TMatrix, class _A>
inline void CVisMatrixChain<TMatrix, _A>::Reserve(int n)
{
	return m_deque.reserve((T_Deque::size_type) n);
}

template<class TMatrix, class _A>
inline void CVisMatrixChain<TMatrix, _A>::Resize(int n)
{
	return m_deque.resize((T_Deque::size_type) n);
}

template<class TMatrix, class _A>
inline int CVisMatrixChain<TMatrix, _A>::Size(void) const
{
	return (int) m_deque.size();
}

template<class TMatrix, class _A>
inline int CVisMatrixChain<TMatrix, _A>::MaxSize(void) const
{
	return (int) m_deque.max_size();
}

template<class TMatrix, class _A>
inline bool CVisMatrixChain<TMatrix, _A>::IsEmpty(void) const
{
	return m_deque.empty();
}

template<class TMatrix, class _A>
inline const TMatrix& CVisMatrixChain<TMatrix, _A>::At(int i) const
{
	assert((i >= 0) && (i < m_deque.size()));
	return m_deque.at(i);
}

template<class TMatrix, class _A>
inline const TMatrix& CVisMatrixChain<TMatrix, _A>::operator[](int i) const

{
	assert((i >= 0) && (i < m_deque.size()));
	return m_deque[i];
}

template<class TMatrix, class _A>
inline const TMatrix& CVisMatrixChain<TMatrix, _A>::Front(void) const
{
	return m_deque.front();
}

template<class TMatrix, class _A>
inline const TMatrix& CVisMatrixChain<TMatrix, _A>::Back(void) const
{
	return m_deque.back();
}

template<class TMatrix, class _A>
inline const TMatrix *CVisMatrixChain<TMatrix, _A>::Begin(void) const
{
	return m_deque.begin();
}

template<class TMatrix, class _A>
inline const TMatrix *CVisMatrixChain<TMatrix, _A>::End(void) const
{
	return m_deque.end();
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::SetFront(void)
{
	assert(!m_deque.empty());
	SetToIdentity(m_deque.front());
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::SetFront(
		const TMatrix& refmatrix)
{
	assert(!m_deque.empty());
	m_deque.front() = refmatrix;
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::SetI(int i)
{
	assert((i >= 0) && (i < m_deque.size()));
	SetToIdentity(m_deque.at(i));
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::SetI(int i,
		const TMatrix& refmatrix)
{
	assert((i >= 0) && (i < m_deque.size()));
	m_deque.at(i) = refmatrix;
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::SetBack(void)
{
	assert(!m_deque.empty());
	SetToIdentity(m_deque.back());
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::SetBack(
		const TMatrix& refmatrix)

{
	assert(!m_deque.empty());
	m_deque.back() = refmatrix;
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::PushFront(void)
{
	T_Matrix matrixT;
	SetToIdentity(matrixT);
	m_deque.push_front(matrixT);
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::PushFront(
		const TMatrix& refmatrix)
{
	m_deque.push_front(refmatrix);
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::PushBack(void)
{
	T_Matrix matrixT;
	SetToIdentity(matrixT);
	m_deque.push_back(matrixT);
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::PushBack(
		const TMatrix& refmatrix)
{
	m_deque.push_back(refmatrix);
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::PushBack(
		const CVisMatrixChain<TMatrix, _A>& refmatrixchain)
{
	for (int i = 0; i < refmatrixchain.Size(); ++i)
		m_deque.push_back(refmatrixchain[i]);
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline void CVisMatrixChain<TMatrix, _A>::PopFront(void)
{
	assert(!m_deque.empty());
	m_deque.pop_front();
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline void CVisMatrixChain<TMatrix, _A>::PopBack(void)
{
	assert(!m_deque.empty());
	m_deque.pop_back();
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::Insert(int i)
{
	assert((i >= 0) && (i <= m_deque.size()));
	T_Matrix matrixT;
	SetToIdentity(matrixT);
	m_deque.insert(m_deque.begin() + i, matrixT);
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::Insert(int i,
		int n)
{
	assert((i >= 0) && (i <= m_deque.size()));
	T_Matrix matrixT;
	SetToIdentity(matrixT);
	m_deque.insert(m_deque.begin() + i, (T_Deque::size_type) n, matrixT);
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::Insert(int i,
		const TMatrix& refmatrix)
{
	assert((i >= 0) && (i <= m_deque.size()));
	m_deque.insert(m_deque.begin() + i, refmatrix);
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::Insert(int i,
		int n, const TMatrix& refmatrix)
{
	assert((i >= 0) && (i <= m_deque.size()));
	m_deque.insert(m_deque.begin() + i, (T_Deque::size_type) n, refmatrix);
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::Assign(void)
{
	T_Matrix matrixT;
	SetToIdentity(matrixT);
	m_deque.assign((T_Deque::size_type) 1, matrixT);
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::Assign(int n)
{
	T_Matrix matrixT;
	SetToIdentity(matrixT);
	m_deque.assign((T_Deque::size_type) n, matrixT);
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::Assign(
		const TMatrix& refmatrix)
{
	m_deque.assign((T_Deque::size_type) 1, refmatrix);
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::Assign(int n,
		const TMatrix& refmatrix)
{
	m_deque.assign((T_Deque::size_type) n, refmatrix);
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::Assign(
		const CVisMatrixChain<TMatrix, _A>& refmatrixchain)
{
	m_deque.clear();
	for (int i = 0; i < refmatrixchain.Size(); ++i)
		m_deque.push_back(refmatrixchain[i]);
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::Assign(
		const CVisMatrixChain<TMatrix, _A>& refmatrixchain, int iFirst, int iLim)
{
	assert((iFirst >= 0) && (iFirst < refmatrixchain.m_deque.size()));
	assert((iLim >= 0) && (iLim <= refmatrixchain.m_deque.size()));
	m_deque.clear();
	for (int i = iFirst; i < iLim; ++i)
		m_deque.push_back(refmatrixchain[i]);
	m_fValidProduct = false;
	return *this;
}

template<class TMatrix, class _A>
inline void CVisMatrixChain<TMatrix, _A>::Clear(void)
{
	m_deque.clear();
	m_fValidProduct = false;
}

template<class TMatrix, class _A>
inline void CVisMatrixChain<TMatrix, _A>::Erase(int i)
{
	assert((i >= 0) && (i < m_deque.size()));
	m_deque.erase(m_deque.begin() + i);
	m_fValidProduct = false;
}

template<class TMatrix, class _A>
inline void CVisMatrixChain<TMatrix, _A>::Erase(int iFirst, int iLim)
{
	assert((iFirst >= 0) && (iFirst < refmatrixchain.m_deque.size()));
	assert((iLim >= 0) && (iLim <= refmatrixchain.m_deque.size()));
	m_deque.erase(m_deque.begin() + iFirst, m_deque.begin() + iLim);
	m_fValidProduct = false;
}

template<class TMatrix, class _A>
inline void CVisMatrixChain<TMatrix, _A>::Swap(
		CVisMatrixChain<TMatrix, _A>& refmatrixchain)
{
	m_deque.swap(refmatrixchain.m_deque);

	if (m_fValidProduct && refmatrixchain.m_fValidProduct)
	{
		T_Matrix matrixT = m_matrixProduct;
		m_matrixProduct = refmatrixchain.m_matrixProduct;
		refmatrixchain.m_matrixProduct = matrixT;
	}
	else if (m_fValidProduct)
	{
		refmatrixchain.m_matrixProduct = m_matrixProduct;
	}
	else if (refmatrixchain.m_fValidProduct)
	{
		m_matrixProduct = refmatrixchain.m_matrixProduct;
	}

	bool f = m_fValidProduct;
	m_fValidProduct = refmatrixchain.m_fValidProduct;
	refmatrixchain.m_fValidProduct = f;
}

template<class TMatrix, class _A>
inline TMatrix CVisMatrixChain<TMatrix, _A>::ProductMatrix(
		void) const
{
	// UNDONE:  Should we have a non-const ProductMatrix method that modifies
	// m_matrixProduct and a const method that never modifies m_matrixProduct,
	// or is it okay to cheat and modify m_matrixProduct in the const method
	// if needed?

	if (!m_fValidProduct)
	{
		T_This pThis = const_cast<CVisMatrixChain<TMatrix, _A> * >(this);
		if (m_deque.empty())
		{
			SetToIdentity(pThis->m_matrixProduct);
		}
		else
		{
			pThis->m_matrixProduct = m_deque.front();
			for (int i = 1; i < m_deque.size(); ++i)
				pThis->m_matrixProduct *= m_deque[i];
		}

		pThis->m_fValidProduct = true;
	}

	return m_matrixProduct;
}

template<class TMatrix, class _A>
inline CVisMatrixChain<TMatrix, _A>& CVisMatrixChain<TMatrix, _A>::Invert(void)
{
	int iLast =  m_deque.size() - 1;
	int iMid = ((int) m_deque.size()) / 2;
	for (int i = 0; i < iMid; ++i)
	{
		T_Matrix matrixT = m_deque[i].Inverted();
		m_deque[i] = m_deque[iLast - i].Inverted();
		m_deque[iLast - i] = matrixT;
	}
	if ((iMid & 1) != 0)
	{
		m_deque[iMid] = m_deque[iMid].Inverted();
	}
	m_fValidProduct = false;

	return *this;
}

template<class TMatrix, class _A>
inline void CVisMatrixChain<TMatrix, _A>::SetToIdentity(void)
{
	Assign();
	return *this;
}

#ifdef LATER
template<class TMatrix, class _A>
inline bool CVisMatrixChain<TMatrix, _A>::operator==(
		const CVisMatrixChain<TMatrix, _A>& reftransformchain) const
{
	// UNDONE
	assert(0):
	return false;
}

template<class TMatrix, class _A>
inline bool CVisMatrixChain<TMatrix, _A>::operator!=(
		const CVisMatrixChain<TMatrix, _A>& reftransformchain) const
{
	// UNDONE
	assert(0):
	return false;
}

template<class TMatrix, class _A>
inline bool CVisMatrixChain<TMatrix, _A>::operator<(
		const CVisMatrixChain<TMatrix, _A>& reftransformchain) const
{
	// UNDONE
	assert(0):
	return false;
}
#endif // LATER



template<class TCoord>
void VisCopyMatrixToMatrix( const GXMatrix2TC<TCoord>& refmatrixSrc,
		CVisDMatrix& refmatrixDest )
{
	refmatrixDest.Resize(2, 2);
	for (int iRow = 0; iRow < 2; ++iRow)
	{
		const TCoord *ptcRowSrc = refmatrixSrc[iRow];
		double *pdblRowDest = refmatrixDest[iRow];
		for (int iCol = 0; iCol < 2; ++iCol)
			pdblRowDest[iCol] = (double) ptcRowSrc[iCol];
	}
}

template<class TCoord>
void VisCopyMatrixToMatrix( const GXMatrix3TC<TCoord>& refmatrixSrc,
		CVisDMatrix& refmatrixDest )
{
	refmatrixDest.Resize(3, 3);
	for (int iRow = 0; iRow < 3; ++iRow)
	{
		const TCoord *ptcRowSrc = refmatrixSrc[iRow];
		double *pdblRowDest = refmatrixDest[iRow];
		for (int iCol = 0; iCol < 3; ++iCol)
			pdblRowDest[iCol] = (double) ptcRowSrc[iCol];
	}
}

template<class TCoord>
void VisCopyMatrixToMatrix( const GXMatrix4TC<TCoord>& refmatrixSrc,
		CVisDMatrix& refmatrixDest )
{
	refmatrixDest.Resize(4, 4);
	for (int iRow = 0; iRow < 4; ++iRow)
	{
		const TCoord *ptcRowSrc = refmatrixSrc[iRow];
		double *pdblRowDest = refmatrixDest[iRow];
		for (int iCol = 0; iCol < 4; ++iCol)
			pdblRowDest[iCol] = (double) ptcRowSrc[iCol];
	}
}


template<class TCoord>
void VisSetToZero(GXPoint2TC<TCoord>& refpoint)
{
	refpoint[0] = refpoint[1] = (TCoord) 0;
}

template<class TCoord>
void VisSetToZero(GXVector2TC<TCoord>& refvector)
{
	refvector[0] = refvector[1] = (TCoord) 0;
}

template<class TCoord>
void VisSetToZero(GXCoVector2TC<TCoord>& refcovector)
{
	refcovector[0] = refcovector[1] = (TCoord) 0;
}

template<class TCoord>
void VisSetToZero(GXMatrix2TC<TCoord>& refmatrix)
{
	ZeroMemory(refmatrix[0], 4 * sizeof(TCoord));
}

template<class TCoord>
void VisSetToIdentity(GXMatrix2TC<TCoord>& refmatrix)
{
	refmatrix.SetToIdentity();
}


template<class TCoord>
void VisSetToZero(GXPoint3TC<TCoord>& refpoint)
{
	refpoint[0] = refpoint[1] = refpoint[2] = (TCoord) 0;
}

template<class TCoord>
void VisSetToZero(GXVector3TC<TCoord>& refvector)
{
	refvector[0] = refvector[1] = refvector[2] = (TCoord) 0;
}

template<class TCoord>
void VisSetToZero(GXCoVector3TC<TCoord>& refcovector)
{
	refcovector[0] = refcovector[1] = refcovector[2] = (TCoord) 0;
}

template<class TCoord>
void VisSetToZero(GXMatrix3TC<TCoord>& refmatrix)
{
	ZeroMemory(refmatrix[0], 9 * sizeof(TCoord));
}

template<class TCoord>
void VisSetToIdentity(GXMatrix3TC<TCoord>& refmatrix)
{
	refmatrix.SetToIdentity();
}


template<class TCoord>
void VisSetToZero(GXPoint4TC<TCoord>& refpoint)
{
	refpoint[0] = refpoint[1] = refpoint[2] = refpoint[3] = (TCoord) 0;
}

template<class TCoord>
void VisSetToZero(GXVector4TC<TCoord>& refvector)
{
	refvector[0] = refvector[1] = refvector[2] = refvector[3] = (TCoord) 0;
}

template<class TCoord>
void VisSetToZero(GXCoVector4TC<TCoord>& refcovector)
{
	refcovector[0] = refcovector[1] = refcovector[2] = refcovector[3]
			= (TCoord) 0;
}

template<class TCoord>
void VisSetToZero(GXMatrix4TC<TCoord>& refmatrix)
{
	ZeroMemory(refmatrix[0], 16 * sizeof(TCoord));
}

template<class TCoord>
void VisSetToIdentity(GXMatrix4TC<TCoord>& refmatrix)
{
	refmatrix.SetToIdentity();
}


template<class TCoord>
void VisSetToZero(GXHPoint2TC<TCoord>& refhpoint)
{
	refhpoint[0] = refhpoint[1] = (TCoord) 0;
	refhpoint[2] = (TCoord) 1;
}

template<class TCoord>
void VisSetToZero(GXHPoint3TC<TCoord>& refhpoint)
{
	refhpoint[0] = refhpoint[1] = refhpoint[2] = (TCoord) 0;
	refhpoint[3] = (TCoord) 1;
}

template<class TCoord>
void VisSetToZero(GXHPlane3TC<TCoord>& refhplane)
{
	refhplane[0] = refhplane[1] = refhplane[2] = (TCoord) 0;
	refhplane[3] = (TCoord) 1;
}


////////////////////////////////////////////////////////////////////////////
// The following code is from the CVisTransform4x4 class in the Vision SDK.

template<class TCoord>
inline CVisMatrix4TC<TCoord>& CVisMatrix4TC<TCoord>::TransposeZW(void)
{
	// Idea is that Transposed[i][j] = Original[i ^ (i > 1)][j ^ (j > 1)].
	TCoord coordTemp;
    for (int i = 0; i < 4; i++)
	{
        coordTemp = (*this)[2][i];
        (*this)[2][i] = (*this)[3][i];
        (*this)[3][i] = coordTemp;
	}
    for (i = 0; i < 4; i++)
	{
        coordTemp = (*this)[i][2];
        (*this)[i][2] = (*this)[i][3];
        (*this)[i][3] = coordTemp;
	}

    return *this;
}

template<class TCoord>
inline CVisMatrix4TC<TCoord> CVisMatrix4TC<TCoord>::TransposedZW(void) const
{
	return CVisMatrix4TC<TCoord>(*this).TransposeZW();
}

template<class TCoord>
inline CVisMatrix4TC<TCoord>& CVisMatrix4TC<TCoord>::TransposeZWRow(void)
{
	// Idea is that Transposed[j][i] = Original[i ^ (i > 1)][j].
	TCoord coordTemp;
    for (int i = 0; i < 4; i++)
	{
        coordTemp = (*this)[2][i];
        (*this)[2][i] = (*this)[3][i];
        (*this)[3][i] = coordTemp;
	}

    return *this;
}

template<class TCoord>
inline CVisMatrix4TC<TCoord> CVisMatrix4TC<TCoord>::TransposedZWRow(void) const
{
	return CVisMatrix4TC<TCoord>(*this).TransposeZWRow();
}

// Compute a projection of a vector defined by a this transform.
template<class TCoord>
inline CVisHPoint3TC<TCoord> CVisMatrix4TC<TCoord>::Project(
		const GXHPoint3TC<TCoord>& v) const
{
    CVisHPoint3TC<TCoord> result;

    for (int i = 0; i < 3; i++)
	{
        result[i] = (*this)[i][0] * v[0] + (*this)[i][1] * v[1]
				+ (*this)[i][2] * v[2] + (*this)[i][3] * v[3];
	}

    TCoord s = (*this)[3][0] * v[0] + (*this)[3][1] * v[1]
			+ (*this)[3][2] * v[2] + (*this)[3][3] * v[3];
    TCoord d = (s > 0.0) ? 1.0 / s : (s < 0.0) ? -1.0/ s : 1.0;
    for (i = 0; i < 3; i++)
        result[i] *= d;
    result[3] = (s > 0.0) ? 1.0 : (s < 0.0) ? -1.0 : 0.0;

    return result;
}

// Helper function for VisProjectLine
template<class TCoord>
inline void VisClipEnd(GXHPoint3TC<TCoord>& e0, GXHPoint3TC<TCoord>& e1,
		int i, double lambda, TCoord s)
{
    for (int j = 0; j < 4; j++)
        e0[j] += lambda * (e1[j] - e0[j]);
    e0[i] = s;
    if (e0[3] != 1.0) {
        TCoord d = (TCoord) (1.0 / s);
        for (j = 0; j < 3; j++)
            e0[j] *= d;
        e0[3] = 1.0;
    }
}

// Helper function for VisProjectLine
template<class TCoord>
inline int VisClipPlane(GXHPoint3TC<TCoord>& e0, GXHPoint3TC<TCoord>& e1,
		int i, bool gt, TCoord s)
{
    TCoord x0 = e0[i], x1= e1[i];
    bool ok0 = (gt) ? x0 > s : x0 < s;
    bool ok1 = (gt) ? x1 > s : x1 < s;
    if (ok0 == ok1)
        return ! ok0;       // both endpoints lie outside or inside
    else if (ok1) {
        TCoord lambda = (s - x0) / (x1 - x0);
        VisClipEnd(e0, e1, i, lambda, s);
    } else if (ok0) {
        TCoord lambda = (s - x1) / (x0 - x1);
        VisClipEnd(e1, e0, i, lambda, s);
    }
    return 0;               // success after clipping
}

// Helper function for VisProjectLine
template<class TCoord>
inline void VisClipLine(GXHPoint3TC<TCoord>& e0, GXHPoint3TC<TCoord>& e1,
		TCoord max_size)
{
    if (VisClipPlane(e0, e1, 3, true, (TCoord) (1.0 / max_size)) ||
        VisClipPlane(e0, e1, 0, false,  max_size) ||
        VisClipPlane(e0, e1, 0, true, -max_size) ||
        VisClipPlane(e0, e1, 1, false,  max_size) ||
        VisClipPlane(e0, e1, 1, true, -max_size))
        e0[0] = e0[1] = e1[0] = e1[1] = max_size;   // clipped out
}

template<class TCoord>
inline void CVisMatrix4TC<TCoord>::ProjectLine(const GXHPoint3TC<TCoord>& v0,
		const GXHPoint3TC<TCoord>& v1, GXHPoint3TC<TCoord>& e0,
		GXHPoint3TC<TCoord>& e1, TCoord max_size) const
{
	// Project both endpoints of a line, and then clip so that
	//  w > 0.0 and |x| and |y| <= max_size.
	// If both endpoints are invisible, e0 = e1 = (max_size, max_size);
	// See Rick's notes from Dec. 12, 1996 and Aug 25, 1997.
	//
	// Note that after clipping, only the (x,y,z) components are valid.

    e0 = ProjectVector(v0);
    e1 = ProjectVector(v1);
#if 1   // new code, Aug 25, 1997
    VisClipLine(e0, e1, max_size);
#else   // old code, Dec 12, 1996
    TCoord x0 = e0[0], x1 = e1[0];
    TCoord y0 = e0[1], y1 = e1[1];
    TCoord z0 = e0[3], z1 = e1[3];
    TCoord s0 = (TCoord) __max(fabs(x0), fabs(y0));
    TCoord s1 = (TCoord) __max(fabs(x1), fabs(y1));
    TCoord tiny = 1.0 / max_size;
    if (z0 <= s0 * tiny && z1 <= s1 * tiny) {
        // This test may clip out some lines that pass through visible volume
        e0[0] = e1[0] = e1[0] = e1[1] = max_size;
    } else if (z0 <= s0 * tiny || z1 <= s1 * tiny) {
		// if we are here, z0 = z1 = 1; 
		// then we use the mid point for clip

		if (z0 == z1) z1 = -z1;
        TCoord clipx = (z0 * x1 - z1 * x0) / (z0 - z1);
        TCoord clipy = (z0 * y1 - z1 * y0) / (z0 - z1);

        TCoord s = (TCoord) __max(__max(fabs(clipx), fabs(clipy)), tiny);
        if (z0 <= s0 * tiny)
            e0[0] = clipx * max_size/s, e0[1] = clipy * max_size/s;
        else
            e1[0] = clipx * max_size/s, e1[1] = clipy * max_size/s;
    }
#endif
}

template<class TCoord>
inline CVisMatrix4TC<TCoord> CVisMatrix4TC<TCoord>::AppRot(void) const
{
    CVisDMatrix dmatrixThis(4, 4);
	dmatrixThis = *this;

    CVisMatrix4TC<TCoord> matrixAppRot;
	matrixAppRot = dmatrixThis * VisDMatrixSqrtInverse(dmatrixThis);

    return matrixAppRot;
}


