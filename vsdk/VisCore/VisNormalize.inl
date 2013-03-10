// VisNormalize.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#include "float.h"


template<class TPixel>
inline bool VisIsSigned(const TPixel& refpixel)
{
	// Defalut is signed.
	return true;
}

inline bool VisIsSigned(const unsigned char& refpixel)
{
	return false;
}

inline bool VisIsSigned(const unsigned short& refpixel)
{
	return false;
}

inline bool VisIsSigned(const unsigned int& refpixel)
{
	return false;
}

inline bool VisIsSigned(const unsigned long& refpixel)
{
	return false;
}


template<class TPixel>
inline TPixel VisRangeMin(const TPixel& refpixel, bool fBoundFp)
{
	assert(0);

	return 0;
}

inline signed char VisRangeMin(const signed char& refpixel, bool fBoundFp)
{
	return (char) (1 << 7);
}

inline short VisRangeMin(const short& refpixel, bool fBoundFp)
{
	return (short) (1 << 15);
}

inline int VisRangeMin(const int& refpixel, bool fBoundFp)
{
	return (int) (1 << 31);
}

inline long VisRangeMin(const long& refpixel, bool fBoundFp)
{
	return (long) (1 << 31);
}

inline unsigned char VisRangeMin(const unsigned char& refpixel, bool fBoundFp)
{
	return (char) 0;
}

inline unsigned short VisRangeMin(const unsigned short& refpixel, bool fBoundFp)
{
	return (short) 0;
}

inline unsigned int VisRangeMin(const unsigned int& refpixel, bool fBoundFp)
{
	return (int) 0;
}

inline unsigned long VisRangeMin(const unsigned long& refpixel, bool fBoundFp)
{
	return (long) 0;
}

inline float VisRangeMin(const float& refpixel, bool fBoundFp)
{
	if (fBoundFp)
		return (float) - 1.0;

	return (float) - (FLT_MAX);
}

inline double VisRangeMin(const double& refpixel, bool fBoundFp)
{
	if (fBoundFp)
		return (double) - 1.0;

	return (double) - (DBL_MAX);
}



template<class TPixel>
inline TPixel VisRangeMax(const TPixel& refpixel, bool fBoundFp)
{
	assert(0);

	return 0;
}

inline signed char VisRangeMax(const signed char& refpixel, bool fBoundFp)
{
	return (signed char) 0x7f;
}

inline short VisRangeMax(const short& refpixel, bool fBoundFp)
{
	return (short) 0x7fff;
}

inline int VisRangeMax(const int& refpixel, bool fBoundFp)
{
	return (int) 0x7fffffff;
}

inline long VisRangeMax(const long& refpixel, bool fBoundFp)
{
	return (long) 0x7fffffff;
}

inline unsigned char VisRangeMax(const unsigned char& refpixel, bool fBoundFp)
{
	return (unsigned char) 0xff;
}

inline unsigned short VisRangeMax(const unsigned short& refpixel, bool fBoundFp)
{
	return (unsigned short) 0xffff;
}

inline unsigned int VisRangeMax(const unsigned int& refpixel, bool fBoundFp)
{
	return (unsigned int) 0xffffffff;
}

inline unsigned long VisRangeMax(const unsigned long& refpixel, bool fBoundFp)
{
	return (unsigned long) 0xffffffff;
}

inline float VisRangeMax(const float& refpixel, bool fBoundFp)
{
	if (fBoundFp)
		return (float) 1.0;

	return (float) (FLT_MAX);
}

inline double VisRangeMax(const double& refpixel, bool fBoundFp)
{
	if (fBoundFp)
		return (double) 1.0;

	return (double) (DBL_MAX);
}


template<class TPixel>
inline CVisFnOpFindMinMax<TPixel>::CVisFnOpFindMinMax(void)
  : m_fIsSigned(VisIsSigned(TPixel()))
{
	m_pixelMin = VisRangeMax(TPixel(), false);
	m_pixelMax = VisRangeMin(TPixel(), false);
	m_pixelMinAbs = VisRangeMax(TPixel(), false);

	assert(m_pixelMin > m_pixelMax);
}

template<class TPixel>
inline void CVisFnOpFindMinMax<TPixel>::operator()(const TPixel& refpixel)
{
	if (refpixel < m_pixelMin)
		m_pixelMin = refpixel;
	else if (refpixel > m_pixelMax)
		m_pixelMax = refpixel;

	if (m_fIsSigned)
	{
		if (refpixel >= 0)
		{
			if (refpixel < m_pixelMinAbs)
				m_pixelMinAbs = refpixel;
		}
		else
		{
			if (- refpixel < m_pixelMinAbs)
				m_pixelMinAbs = - refpixel;
		}
	}
}

template<class TPixel>
inline TPixel CVisFnOpFindMinMax<TPixel>::Min(void) const
{
	return m_pixelMin;
}

template<class TPixel>
inline TPixel CVisFnOpFindMinMax<TPixel>::Max(void) const
{
	return m_pixelMax;
}

template<class TPixel>
inline TPixel CVisFnOpFindMinMax<TPixel>::MinAbs(void) const
{
	if (VisIsSigned(TPixel()))
		return m_pixelMinAbs;

	return m_pixelMin;
}

template<class TPixel>
inline TPixel CVisFnOpFindMinMax<TPixel>::MaxAbs(void) const
{
	// Warning: The most negative number (eg 1 << 31)?
	// It may have a larger magnitude than MaxAbs.
	if ((m_pixelMin < 0) && (- m_pixelMin > m_pixelMax))
		return - m_pixelMin;

	return m_pixelMax;
}

template<class TPixel>
inline double CVisFnOpFindMinMax<TPixel>::DblMaxAbs(void) const
{
	// This should work correctly with the most negative ints (eg 1 << 31).
	if ((m_pixelMin < 0)
			&& (- ((double) m_pixelMin) > ((double) m_pixelMax)))
	{
		return - ((double) m_pixelMin);
	}

	return (double) m_pixelMax;
}

template<class TPixel>
inline double CVisFnOpFindMinMax<TPixel>::DblMin(EVisNormalize evisnormalize)
		const
{
	double dblMin;

	switch(evisnormalize)
	{
	case evisnormalizeRelative:
		dblMin = (double) Min();
		break;

	case evisnormalizeAbsolute:
		{
			if (VisIsSigned(TPixel()))
				dblMin = - DblMaxAbs();
			else
				dblMin = 0.0;
		}
		break;

	case evisnormalizeRelMagnitude:
		dblMin = (double) MinAbs();
		break;

	case evisnormalizeAbsMagnitude:
		dblMin = 0.0;
		break;

#ifdef _DEBUG
default:
	assert(0);
	break;
#endif // _DEBUG
	}

	return dblMin;
}

template<class TPixel>
inline double CVisFnOpFindMinMax<TPixel>::DblMax(EVisNormalize evisnormalize)
		const
{
	double dblMax;

	if (evisnormalize == evisnormalizeRelative)
		dblMax = (double) Max();
	else
		dblMax = DblMaxAbs();

	return dblMax;
}
