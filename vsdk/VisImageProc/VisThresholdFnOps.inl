// VisThresholdFnOps.inl
// Example thresholding operators.
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


template<class TPixel>
inline CVisFnOpPixelLTVal<TPixel>::CVisFnOpPixelLTVal(
		const TPixel& refpixelVal)
  : m_pixelVal(refpixelVal)
{
}

template<class TPixel>
inline BOOL CVisFnOpPixelLTVal<TPixel>::operator()(
		const TPixel& refpixel) const
{
	return (refpixel < m_pixelVal);
}


template<class TPixel>
inline CVisFnOpPixelLEVal<TPixel>::CVisFnOpPixelLEVal(
		const TPixel& refpixelVal)
  : m_pixelVal(refpixelVal)
{
}

template<class TPixel>
inline BOOL CVisFnOpPixelLEVal<TPixel>::operator()(
		const TPixel& refpixel) const
{
	return (refpixel <= m_pixelVal);
}


template<class TPixel>
inline CVisFnOpPixelGTVal<TPixel>::CVisFnOpPixelGTVal(
		const TPixel& refpixelVal)
  : m_pixelVal(refpixelVal)
{
}

template<class TPixel>
inline BOOL CVisFnOpPixelGTVal<TPixel>::operator()(
		const TPixel& refpixel) const
{
	return (refpixel > m_pixelVal);
}


template<class TPixel>
inline CVisFnOpPixelGEVal<TPixel>::CVisFnOpPixelGEVal(
		const TPixel& refpixelVal)
  : m_pixelVal(refpixelVal)
{
}

template<class TPixel>
inline BOOL CVisFnOpPixelGEVal<TPixel>::operator()(
		const TPixel& refpixel) const
{
	return (refpixel >= m_pixelVal);
}


template<class TPixel>
inline CVisFnOpPixelNEVal<TPixel>::CVisFnOpPixelNEVal(
		const TPixel& refpixelVal)
  : m_pixelVal(refpixelVal)
{
}

template<class TPixel>
inline BOOL CVisFnOpPixelNEVal<TPixel>::operator()(
		const TPixel& refpixel) const
{
	return (refpixel != m_pixelVal);
}


template<class TPixel>
inline CVisFnOpPixelEQVal<TPixel>::CVisFnOpPixelEQVal(
		const TPixel& refpixelVal)
  : m_pixelVal(refpixelVal)
{
}

template<class TPixel>
inline BOOL CVisFnOpPixelEQVal<TPixel>::operator()(
		const TPixel& refpixel) const
{
	return (refpixel == m_pixelVal);
}


template<class TPixel>
inline CVisFnOpPixelGTValLTVal<TPixel>::CVisFnOpPixelGTValLTVal(
		const TPixel& refpixelGTVal, const TPixel& refpixelLTVal)
  : m_pixelGTVal(refpixelGTVal),
	m_pixelLTVal(refpixelLTVal)
{
}

template<class TPixel>
inline BOOL CVisFnOpPixelGTValLTVal<TPixel>::operator()(
		const TPixel& refpixel) const
{
	return ((refpixel > m_pixelGTVal) && (refpixel < m_pixelLTVal));
}


template<class TPixel>
inline CVisFnOpPixelGTValLEVal<TPixel>::CVisFnOpPixelGTValLEVal(
		const TPixel& refpixelGTVal, const TPixel& refpixelLEVal)
  : m_pixelGTVal(refpixelGTVal),
	m_pixelLEVal(refpixelLEVal)
{
}

template<class TPixel>
inline BOOL CVisFnOpPixelGTValLEVal<TPixel>::operator()(
		const TPixel& refpixel) const
{
	return ((refpixel > m_pixelGTVal) && (refpixel <= m_pixelLEVal));
}


template<class TPixel>
inline CVisFnOpPixelGEValLTVal<TPixel>::CVisFnOpPixelGEValLTVal(
		const TPixel& refpixelGEVal, const TPixel& refpixelLTVal)
  : m_pixelGEVal(refpixelGEVal),
	m_pixelLTVal(refpixelLTVal)
{
}

template<class TPixel>
inline BOOL CVisFnOpPixelGEValLTVal<TPixel>::operator()(
		const TPixel& refpixel) const
{
	return ((refpixel >= m_pixelGEVal) && (refpixel < m_pixelLTVal));
}


template<class TPixel>
inline CVisFnOpPixelGEValLEVal<TPixel>::CVisFnOpPixelGEValLEVal(
		const TPixel& refpixelGEVal, const TPixel& refpixelLEVal)
  : m_pixelGEVal(refpixelGEVal),
	m_pixelLEVal(refpixelLEVal)
{
}

template<class TPixel>
inline BOOL CVisFnOpPixelGEValLEVal<TPixel>::operator()(
		const TPixel& refpixel) const
{
	return ((refpixel >= m_pixelGEVal) && (refpixel <= m_pixelLEVal));
}


template<class TPixel, class TFnOp>
inline CVisFnOpNotPixelFnOp<TPixel, TFnOp>::CVisFnOpNotPixelFnOp(
		const TFnOp& reffnop)
  : m_fnop(reffnop)
{
}

template<class TPixel, class TFnOp>
inline BOOL CVisFnOpNotPixelFnOp<TPixel, TFnOp>::operator()(
		const TPixel& refpixel) const
{
	return !m_fnop(refpixel);
}


template<class TPixel, class TFnOp1, class TFnOp2>
inline CVisFnOpAndPixelFnOps<TPixel, TFnOp1, TFnOp2>::CVisFnOpAndPixelFnOps(
		const TFnOp1& reffnop1,
		const TFnOp2& reffnop2)
  : m_fnop1(reffnop1),
  m_fnop2(reffnop2)
{
}

template<class TPixel, class TFnOp1, class TFnOp2>
inline BOOL CVisFnOpAndPixelFnOps<TPixel, TFnOp1, TFnOp2>::operator()(
		const TPixel& refpixel) const
{
	return (m_fnop1(refpixel) && m_fnop2(refpixel));
}


template<class TPixel, class TFnOp1, class TFnOp2>
inline CVisFnOpOrPixelFnOps<TPixel, TFnOp1, TFnOp2>::CVisFnOpOrPixelFnOps(
		const TFnOp1& reffnop1,
		const TFnOp2& reffnop2)
  : m_fnop1(reffnop1),
  m_fnop2(reffnop2)
{
}

template<class TPixel, class TFnOp1, class TFnOp2>
inline BOOL CVisFnOpOrPixelFnOps<TPixel, TFnOp1, TFnOp2>::operator()(
		const TPixel& refpixel) const
{
	return (m_fnop1(refpixel) || m_fnop2(refpixel));
}
