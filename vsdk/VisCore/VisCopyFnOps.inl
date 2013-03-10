// VisCopyFnOps.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


template<class TSrc, class TDest>
inline void CVisFnOpCastSrcToDest<TSrc, TDest>::operator()(const TSrc& refsrc,
		TDest& refdest)
{
	refdest = (TDest) refsrc;
}


template<class TSrc, class TDest>
inline CVisFnOpCastSrcToDestWithMask<TSrc, TDest>::CVisFnOpCastSrcToDestWithMask(
		unsigned long ulMask)
  : m_ulMask(ulMask)
{
}


template<class TSrc, class TDest>
inline void CVisFnOpCastSrcToDestWithMask<TSrc, TDest>::operator()(
		const TSrc& refsrc, TDest& refdest)
{
	refdest = (TDest) (TSrc) (((unsigned long) refsrc) ^ m_ulMask);
}


template<class TSrc, class TDest>
inline CVisFnOpUseLUTForDest<TSrc, TDest>::CVisFnOpUseLUTForDest(
		const TDest *mpSrcToDest)
  : m_mpSrcToDest(mpSrcToDest)
{
}

template<class TSrc, class TDest>
inline void CVisFnOpUseLUTForDest<TSrc, TDest>::operator()(const TSrc& refsrc,
		TDest& refdest)
{
	refdest = m_mpSrcToDest[(BYTE) refsrc];
}


template<class TSrc, class TDest>
inline void CVisFnOpShortToLong<TSrc, TDest>::operator()(const TSrc& refsrc,
		TDest& refdest)
{
	refdest = (TDest) ((unsigned long) (refsrc | (refsrc << 16)));
}


template<class TSrc, class TDest>
inline CVisFnOpShortToLongWithMask<TSrc, TDest>::CVisFnOpShortToLongWithMask(
		unsigned long ulMask)
  : m_ulMask(ulMask)
{
}

template<class TSrc, class TDest>
inline void CVisFnOpShortToLongWithMask<TSrc, TDest>::operator()(const TSrc& refsrc,
		TDest& refdest)
{
	refdest = (TDest) (((unsigned long) (refsrc | (refsrc << 16))) ^ m_ulMask);
}



template<class TSrc, class TDest, int iShift>
inline void CVisFnOpRShiftSrcToDest<TSrc, TDest, iShift>::operator()(
		const TSrc& refsrc, TDest& refdest)
{
	refdest = (TDest) (refsrc >> iShift);
}


template<class TSrc, class TDest, int iShift>
inline CVisFnOpRShiftSrcToDestWithMask<TSrc, TDest, iShift>
	::CVisFnOpRShiftSrcToDestWithMask(
		unsigned long ulMask)
  : m_ulMask(ulMask)
{
}



template<class TSrc, class TDest, int iShift>
inline void CVisFnOpRShiftSrcToDestWithMask<TSrc, TDest, iShift>::operator()(
		const TSrc& refsrc, TDest& refdest)
{
	refdest = (TDest) (((unsigned long) (refsrc >> iShift)) ^ m_ulMask);
}



template<class TSrc, class TDest>
inline CVisFnOpScaleSrcToDest<TSrc, TDest>::CVisFnOpScaleSrcToDest(
		double dblScale)
  : m_dblScale(dblScale)
{
}

template<class TSrc, class TDest>
inline void CVisFnOpScaleSrcToDest<TSrc, TDest>::operator()(const TSrc& refsrc,
		TDest& refdest)
{
	refdest = (TDest) (refsrc * m_dblScale);
}



template<class TSrc, class TDest>
inline CVisFnOpOffsetAndScaleSrcToDest<TSrc, TDest>
	::CVisFnOpOffsetAndScaleSrcToDest(
		double dblOffset, double dblScale)
  : m_dblOffset(dblOffset),
	m_dblScale(dblScale)
{
}

template<class TSrc, class TDest>
inline void CVisFnOpOffsetAndScaleSrcToDest<TSrc, TDest>::operator()(
		const TSrc& refsrc,TDest& refdest)
{
	refdest = (TDest) ((((double) refsrc) - m_dblOffset) * m_dblScale);
}



template<class TSrc, class TDest>
inline CVisFnOpScaleSrcMagToDest<TSrc, TDest>::CVisFnOpScaleSrcMagToDest(
		double dblScale)
  : m_dblScale(dblScale)
{
}

template<class TSrc, class TDest>
inline void CVisFnOpScaleSrcMagToDest<TSrc, TDest>::operator()(const TSrc& refsrc,
		TDest& refdest)
{
	if (refsrc < 0)
		refdest = (TDest) ((- ((double) refsrc)) * m_dblScale);
	else
		refdest = (TDest) (((double) refsrc) * m_dblScale);
}



template<class TSrc, class TDest>
inline CVisFnOpOffsetAndScaleSrcMagToDest<TSrc, TDest>
	::CVisFnOpOffsetAndScaleSrcMagToDest(
		double dblOffset, double dblScale)
  : m_dblOffset(dblOffset),
	m_dblScale(dblScale)
{
}

template<class TSrc, class TDest>
inline void CVisFnOpOffsetAndScaleSrcMagToDest<TSrc, TDest>::operator()(
		const TSrc& refsrc,TDest& refdest)
{
	if (refsrc < 0)
	{
		refdest = (TDest) (((- ((double) refsrc)) - m_dblOffset) * m_dblScale);
	}
	else
	{
		refdest = (TDest) ((((double) refsrc) - m_dblOffset) * m_dblScale);
	}
}


template<class TSrc, class TDest>
inline CVisFnOpAdjustFloatToSignedInt<TSrc, TDest>::CVisFnOpAdjustFloatToSignedInt(
		TDest destMin, TDest destMax)
  : m_destMin(destMin),
	m_destMax(destMax),
	m_dblScale(- (destMin / 1.0))
{
}


template<class TSrc, class TDest>
inline void CVisFnOpAdjustFloatToSignedInt<TSrc, TDest>::operator()(
		const TSrc& refsrc, TDest& refdest)
{
	if (refsrc >= 1.0)
		refdest = m_destMax;
	else if (refsrc <= - 1.0)
		refdest = m_destMin;
	else
		refdest = (TDest) (m_dblScale * refsrc + 0.5);
}


template<class TSrc, class TDest>
inline CVisFnOpAdjustFloatToUnsignedInt<TSrc, TDest>::CVisFnOpAdjustFloatToUnsignedInt(
		TDest destMax)
  : m_destMax(destMax),
	m_dblScale(destMax / 2.0)
{
}


template<class TSrc, class TDest>
inline void CVisFnOpAdjustFloatToUnsignedInt<TSrc, TDest>::operator()(
		const TSrc& refsrc, TDest& refdest)
{
	if (refsrc >= 1.0)
		refdest = m_destMax;
	else if (refsrc <= - 1.0)
		refdest = (TDest) 0;
	else
		refdest = (TDest) (m_dblScale * (refsrc + 1.0) + 0.5);
}


template<class TSrc, class TDest>
inline void CVisFnOpAdjustFloatToFloat<TSrc, TDest>::operator()(
		const TSrc& refsrc, TDest& refdest)
{
	if (refsrc >= 1.0)
		refdest = (TDest) 1.0;
	else if (refsrc <= - 1.0)
		refdest = (TDest) - 1.0;
	else
		refdest = (TDest) refsrc;
}
