// VisPixelFnOps.h
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



template<class TPixel>
class CVisFnOpNegatePixels
{
public:
	void operator()(TPixel& refpixel) const
		{ refpixel = (TPixel) (- refpixel); }

	void operator()(TPixel& refpixel1, const TPixel& refpixel2) const
		{ refpixel1 = - refpixel2; }
};


template<class TPixel>
class CVisFnOpAddPixels
{
public:
	CVisFnOpAddPixels(const TPixel& refpixel = TPixel())
		: m_pixel(refpixel) {}

	void operator()(TPixel& refpixel) const
		{ refpixel += m_pixel; }

	void operator()(TPixel& refpixel1, const TPixel& refpixel2) const
		{ refpixel1 += refpixel2; }

	void operator()(TPixel& refpixel1, const TPixel& refpixel2,
			const TPixel& refpixel3) const
		{ refpixel1 = (TPixel) (refpixel2 + refpixel3); }

protected:
	const TPixel m_pixel;
};


template<class TPixel>
class CVisFnOpSubPixels
{
public:
	CVisFnOpSubPixels(const TPixel& refpixel = TPixel())
		: m_pixel(refpixel) {}

	void operator()(TPixel& refpixel) const
		{ refpixel -= m_pixel; }

	void operator()(TPixel& refpixel1, const TPixel& refpixel2) const
		{ refpixel1 -= refpixel2; }

	void operator()(TPixel& refpixel1, const TPixel& refpixel2,
			const TPixel& refpixel3) const
		{ refpixel1 = (TPixel) (refpixel2 - refpixel3); }

protected:
	const TPixel m_pixel;
};


template<class TPixel>
class CVisFnOpMultPixels
{
public:
	CVisFnOpMultPixels(const TPixel& refpixel = TPixel())
		: m_pixel(refpixel) {}

	void operator()(TPixel& refpixel) const
		{ refpixel *= m_pixel; }

	void operator()(TPixel& refpixel1, const TPixel& refpixel2) const
		{ refpixel1 *= refpixel2; }

	void operator()(TPixel& refpixel1, const TPixel& refpixel2,
			const TPixel& refpixel3) const
		{ refpixel1 = (TPixel) (refpixel2 * refpixel3); }

protected:
	const TPixel m_pixel;
};


template<class TPixel>
class CVisFnOpDivPixels
{
public:
	CVisFnOpDivPixels(const TPixel& refpixel = TPixel())
		: m_pixel(refpixel) {}

	void operator()(TPixel& refpixel) const
		{ refpixel /= m_pixel; }

	void operator()(TPixel& refpixel1, const TPixel& refpixel2) const
		{ refpixel1 /= refpixel2; }

	void operator()(TPixel& refpixel1, const TPixel& refpixel2,
			const TPixel& refpixel3) const
		{ refpixel1 = (TPixel) (refpixel2 / refpixel3); }

protected:
	const TPixel m_pixel;
};



template<class TPixel>
class CVisFnOpNotPixels
{
public:
	void operator()(TPixel& refpixel) const
		{ refpixel = (TPixel) (~ refpixel); }

	void operator()(TPixel& refpixel1, const TPixel& refpixel2) const
		{ refpixel1 = - refpixel2; }
};


template<class TPixel>
class CVisFnOpAndPixels
{
public:
	CVisFnOpAndPixels(const TPixel& refpixel = TPixel())
		: m_pixel(refpixel) {}

	void operator()(TPixel& refpixel) const
		{ refpixel &= m_pixel; }

	void operator()(TPixel& refpixel1, const TPixel& refpixel2) const
		{ refpixel1 &= refpixel2; }

	void operator()(TPixel& refpixel1, const TPixel& refpixel2,
			const TPixel& refpixel3) const
		{ refpixel1 = (TPixel) (refpixel2 & refpixel3); }

protected:
	const TPixel m_pixel;
};


template<class TPixel>
class CVisFnOpOrPixels
{
public:
	CVisFnOpOrPixels(const TPixel& refpixel = TPixel())
		: m_pixel(refpixel) {}

	void operator()(TPixel& refpixel) const
		{ refpixel |= m_pixel; }

	void operator()(TPixel& refpixel1, const TPixel& refpixel2) const
		{ refpixel1 |= refpixel2; }

	void operator()(TPixel& refpixel1, const TPixel& refpixel2,
			const TPixel& refpixel3) const
		{ refpixel1 = (TPixel) (refpixel2 | refpixel3); }

protected:
	const TPixel m_pixel;
};


template<class TPixel>
class CVisFnOpLShiftPixels
{
public:
	CVisFnOpLShiftPixels(const TPixel& refpixel = TPixel())
		: m_pixel(refpixel) {}

	void operator()(TPixel& refpixel) const
		{ refpixel <<= m_pixel; }

	void operator()(TPixel& refpixel1, const TPixel& refpixel2) const
		{ refpixel1 <<= refpixel2; }

	void operator()(TPixel& refpixel1, const TPixel& refpixel2,
			const TPixel& refpixel3) const
		{ refpixel1 = (TPixel) (refpixel2 << refpixel3); }

protected:
	const TPixel m_pixel;
};


template<class TPixel>
class CVisFnOpRShiftPixels
{
public:
	CVisFnOpRShiftPixels(const TPixel& refpixel = TPixel())
		: m_pixel(refpixel) {}

	void operator()(TPixel& refpixel) const
		{ refpixel >>= m_pixel; }

	void operator()(TPixel& refpixel1, const TPixel& refpixel2) const
		{ refpixel1 >>= refpixel2; }

	void operator()(TPixel& refpixel1, const TPixel& refpixel2,
			const TPixel& refpixel3) const
		{ refpixel1 = (TPixel) (refpixel2 >> refpixel3); }

protected:
	const TPixel m_pixel;
};
