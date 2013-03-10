// VisTransform4x4.inl
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG


// Needed for sqrt()
#include <math.h>


////////////////////////////////////////////////////////////////////////////
// CVisVector4

inline CVisVector4::CVisVector4(bool fInitialize)
{
	if (fInitialize)
	{
		m_rgdbl[0] = m_rgdbl[1] = m_rgdbl[2] = m_rgdbl[3] = 0.0;
	}
}

inline CVisVector4::CVisVector4(double x, double y, double z, double w)
{
    m_rgdbl[0] = x;
	m_rgdbl[1] = y;
	m_rgdbl[2] = z;
	m_rgdbl[3] = w;
}

inline double CVisVector4::operator[](int i) const
{
	assert((i >= 0) && (i < 4));

	return m_rgdbl[i];
}

inline double& CVisVector4::operator[](int i)
{
	assert((i >= 0) && (i < 4));

	return m_rgdbl[i];
}

inline bool CVisVector4::operator==(const CVisVector4& refvector) const
{
	return ((m_rgdbl[0] == refvector[0])
			&& (m_rgdbl[1] == refvector[1])
			&& (m_rgdbl[2] == refvector[2])
			&& (m_rgdbl[3] == refvector[3]));
}

inline bool CVisVector4::operator!=(const CVisVector4& refvector) const
{
	return !((*this) == refvector);
}

inline bool CVisVector4::operator<(const CVisVector4& refvector) const
{
	return (memcmp(this, &refvector, sizeof(CVisVector4)) < 0);
}


inline CVisVector4 CVisVector4::operator+(const CVisVector4& refvector) const
{
	return (CVisVector4(*this) += refvector);
}

inline CVisVector4 CVisVector4::operator-(const CVisVector4& refvector) const
{
	return (CVisVector4(*this) -= refvector);
}

inline double CVisVector4::operator*(const CVisVector4& refvector) const
{
	return m_rgdbl[0] * refvector[0] + m_rgdbl[1] * refvector[1]
			+ m_rgdbl[2] * refvector[2];
}

inline CVisVector4 CVisVector4::operator*(double dbl) const
{
	return (CVisVector4(*this) *= dbl);
}

inline CVisVector4 CVisVector4::operator/(double dbl) const
{
	return (CVisVector4(*this) /= dbl);
}


inline CVisVector4& CVisVector4::operator-(void)
{
	m_rgdbl[0] = - m_rgdbl[0];
	m_rgdbl[1] = - m_rgdbl[1];
	m_rgdbl[2] = - m_rgdbl[2];

	return *this;
}

inline CVisVector4& CVisVector4::operator+=(const CVisVector4& refvector)
{
	m_rgdbl[0] += refvector[0];
	m_rgdbl[1] += refvector[1];
	m_rgdbl[2] += refvector[2];

	return *this;
}

inline CVisVector4& CVisVector4::operator-=(const CVisVector4& refvector)
{
	m_rgdbl[0] -= refvector[0];
	m_rgdbl[1] -= refvector[1];
	m_rgdbl[2] -= refvector[2];

	return *this;
}

inline CVisVector4& CVisVector4::operator*=(double dbl)
{
	m_rgdbl[0] *= dbl;
	m_rgdbl[1] *= dbl;
	m_rgdbl[2] *= dbl;

	return *this;
}

inline CVisVector4& CVisVector4::operator/=(double dbl)
{
	m_rgdbl[0] /= dbl;
	m_rgdbl[1] /= dbl;
	m_rgdbl[2] /= dbl;

	return *this;
}


inline double CVisVector4::Magnitude(void) const
{
	return sqrt(MagnitudeSquared());
}


inline double CVisVector4::MagnitudeSquared(void) const
{
	return (*this) * (*this);
}

inline CVisVector4& CVisVector4::Cross(const CVisVector4& refvector)
{
	*this = Crossed(refvector);

	return *this;
}

inline CVisVector4 CVisVector4::Crossed(const CVisVector4& refvector) const
{
    CVisVector4 vector4Ret(false);
    return vector4Ret.EqCross(*this, refvector);
}



////////////////////////////////////////////////////////////////////////////
// CVisTransform4x4

inline const double *CVisTransform4x4::operator[](int i) const
{
	return m_rgrgdbl[i];
}

inline double *CVisTransform4x4::operator[](int i)
{
	return m_rgrgdbl[i];
}

inline bool CVisTransform4x4::operator!=(
		const CVisTransform4x4& reftransform4x4) const
{
	return !((*this) == reftransform4x4);
}

inline bool CVisTransform4x4::operator<(
		const CVisTransform4x4& reftransform4x4) const
{
	return (memcmp(this, &reftransform4x4, sizeof(CVisTransform4x4)) < 0);
}

inline CVisTransform4x4& CVisTransform4x4::operator*=(
		const CVisTransform4x4& reftransform4x4)
{
    *this = operator*(reftransform4x4);

    return *this;
}

inline CVisVector4 CVisTransform4x4::operator*(
		const CVisVector4& refvector4) const
{
    CVisVector4 vector4Ret(false);
    return vector4Ret.EqProd(*this, refvector4);
}

inline CVisTransform4x4 CVisTransform4x4::operator*(
		const CVisTransform4x4& reftransform4x4) const
{
    CVisTransform4x4 transform4x4Ret(false);
    return transform4x4Ret.EqProd(*this, reftransform4x4);
}

inline CVisTransform4x4 CVisTransform4x4::Translated(double tx, double ty,
		double tz) const
{
	return CVisTransform4x4(*this).Translate(tx, ty, tz);
}

inline CVisTransform4x4& CVisTransform4x4::Rotate(double angle, double ax,
		double ay, double az)
{
	*this = Rotated(angle, ax, ay, az);

	return *this;
}

inline CVisTransform4x4 CVisTransform4x4::Scaled(double sx, double sy,
		double sz) const
{
	return CVisTransform4x4(*this).Scale(sx, sy, sz);
}


inline CVisTransform4x4 CVisTransform4x4::Normalized(void) const
{
	return CVisTransform4x4(*this).Normalize();
}

inline CVisTransform4x4& CVisTransform4x4::Invert(void)
{
	*this = Inverted();

	return *this;
}

inline CVisTransform4x4 CVisTransform4x4::Transposed(void) const
{
	return CVisTransform4x4(*this).Transpose();
}

inline CVisTransform4x4 CVisTransform4x4::TransposedZW(void) const
{
	return CVisTransform4x4(*this).TransposeZW();
}

inline CVisTransform4x4 CVisTransform4x4::TransposedZWRow(void) const
{
	return CVisTransform4x4(*this).TransposeZWRow();
}



////////////////////////////////////////////////////////////////////////////
// CVisTransformChain

inline CVisTransformChain::CVisTransformChain(void)
{
	push_front(CVisTransform4x4());
}

inline CVisTransformChain::CVisTransformChain(
		const CVisTransform4x4& reftransform)
{
	assign(1, reftransform);
}

inline CVisTransformChain::CVisTransformChain(
		const CVisTransformChain& reftransformchain)
{
	if (!reftransformchain.empty())
		assign(reftransformchain.begin(), reftransformchain.end());
}

inline CVisTransformChain& CVisTransformChain::operator=(
		const CVisTransform4x4& reftransform)
{
	assign(1, reftransform);

	return *this;
}

inline CVisTransformChain& CVisTransformChain::operator=(
		const CVisTransformChain& reftransformchain)
{
	if (reftransformchain.empty())
		clear();
	else
		assign(reftransformchain.begin(), reftransformchain.end());

	return *this;
}

inline CVisTransformChain& CVisTransformChain::push_front(
		const CVisTransform4x4& reftransform)
{
	T_Deque::push_front(reftransform);

	return *this;
}

inline CVisTransformChain& CVisTransformChain::push_back(
		const CVisTransform4x4& reftransform)
{
	T_Deque::push_back(reftransform);

	return *this;
}

inline CVisTransformChain& CVisTransformChain::push_back(
		const CVisTransformChain& reftransformchain)
{
	if (!reftransformchain.empty())
		insert(end(), reftransformchain.begin(), reftransformchain.end());

	return *this;
}

inline CVisTransformChain& CVisTransformChain::Translate(double tx,
		double ty, double tz)
{
	front().Translate(tx, ty, tz);

	return *this;
}


inline CVisTransformChain& CVisTransformChain::Rotate(double angle,
		double ax, double ay, double az)
{
	front().Rotate(angle, ax, ay, az);

	return *this;
}

inline CVisTransformChain& CVisTransformChain::Scale(double sx, double sy,
		double sz)
{
	front().Scale(sx, sy, sz);

	return *this;
}

inline CVisTransformChain& CVisTransformChain::Invert(void)
{
	// LATER:  We could probably make this more efficient by doing this
	// directory.  (Maybe a VisSwapTransforms function would help.  It
	// could swap each double, using only one doubld as a temp variable.)
	*this = Inverted();

	return *this;
}

inline bool CVisTransformChain::operator!=(
		const CVisTransformChain& reftransformchain) const
{
	return !operator==(reftransformchain);
}

inline bool CVisTransformChain::operator<(
		const CVisTransformChain& reftransformchain) const
{
	// This should never be called.  (If we find that it is called, we'll need
	// to change this to a much less efficient operator that compares each transform
	// in the chains.)
	assert(0);
	return (begin() < reftransformchain.begin());
}



//
// Self-describing stream output.
//

#ifdef VIS_INCLUDE_SDSTREAM

inline CVisSDStream& operator<<(CVisSDStream& s, CVisVector4& o)
	{ return VisSDReadWriteClass(s, o); }

inline CVisSDObject<CVisVector4>& CVisSDObject<CVisVector4>::Exemplar(void)
{
	// Global variable defined in ..\VisImageProc\VisSDStreamExemplars.cpp
	extern CVisSDObject<CVisVector4> g_visvector4Exemplar;

	return g_visvector4Exemplar;
}

inline CVisSDStream& operator<<(CVisSDStream& s, CVisTransform4x4& o)
	{ return VisSDReadWriteClass(s, o); }

inline CVisSDObject<CVisTransform4x4>& CVisSDObject<CVisTransform4x4>::Exemplar(void)
{
	// Global variable defined in ..\VisImageProc\VisSDStreamExemplars.cpp
	extern CVisSDObject<CVisTransform4x4> g_vistransform4x4Exemplar;

	return g_vistransform4x4Exemplar;
}

inline CVisSDStream& operator<<(CVisSDStream& s, CVisTransformChain& o)
{ return VisSDReadWriteContainer(s, o, CVisTransform4x4()); }  // UNDONE:  We probably don't need ReadWriteFields anymore.

#ifdef TAKEOUT
inline CVisSDObject<CVisTransformChain>&
	CVisSDObject<CVisTransformChain>::Exemplar(void)
{
	// Global variable defined in ..\VisImageProc\VisSDStreamExemplars.cpp
	extern CVisSDObject<CVisTransformChain> g_vistransformchainExemplar;

	return g_vistransformchainExemplar;
}
#endif // TAKEOUT

#endif // VIS_INCLUDE_SDSTREAM

#ifdef _DEBUG
#undef new
#endif // _DEBUG

