// VisCopyFnOps.h
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


template<class TSrc, class TDest>
class CVisFnOpCastSrcToDest
{
public:
	inline void operator()(const TSrc& refsrc, TDest& refdest);
};


template<class TSrc, class TDest>
class CVisFnOpCastSrcToDestWithMask
{
public:
	inline CVisFnOpCastSrcToDestWithMask(unsigned long ulMask);
	inline void operator()(const TSrc& refsrc, TDest& refdest);
protected:
	const unsigned long m_ulMask;
};


template<class TSrc, class TDest>
class CVisFnOpUseLUTForDest
{
public:
	inline CVisFnOpUseLUTForDest(const TDest *mpSrcToDest);
	inline void operator()(const TSrc& refsrc, TDest& refdest);
protected:
	const TDest *m_mpSrcToDest;
};


template<class TSrc, class TDest>
class CVisFnOpShortToLong
{
public:
	inline void operator()(const TSrc& refsrc, TDest& refdest);
};


template<class TSrc, class TDest>
class CVisFnOpShortToLongWithMask
{
public:
	inline CVisFnOpShortToLongWithMask(unsigned long ulMask);
	inline void operator()(const TSrc& refsrc, TDest& refdest);
protected:
	const unsigned long m_ulMask;
};


template<class TSrc, class TDest, int iShift>
class CVisFnOpRShiftSrcToDest
{
public:
	inline void operator()(const TSrc& refsrc, TDest& refdest);
};


template<class TSrc, class TDest, int iShift>
class CVisFnOpRShiftSrcToDestWithMask
{
public:
	inline CVisFnOpRShiftSrcToDestWithMask(unsigned long ulMask);
	inline void operator()(const TSrc& refsrc, TDest& refdest);
protected:
	const unsigned long m_ulMask;
};


template<class TSrc, class TDest>
class CVisFnOpScaleSrcToDest
{
public:
	CVisFnOpScaleSrcToDest(double dblScale);
	inline void operator()(const TSrc& refsrc, TDest& refdest);
protected:
	const double m_dblScale;
};


template<class TSrc, class TDest>
class CVisFnOpOffsetAndScaleSrcToDest
{
public:
	CVisFnOpOffsetAndScaleSrcToDest(double dblOffset, double dblScale);
	inline void operator()(const TSrc& refsrc, TDest& refdest);
protected:
	const double m_dblOffset;
	const double m_dblScale;
};


template<class TSrc, class TDest>
class CVisFnOpScaleSrcMagToDest
{
public:
	CVisFnOpScaleSrcMagToDest(double dblScale);
	inline void operator()(const TSrc& refsrc, TDest& refdest);
protected:
	const double m_dblScale;
};


template<class TSrc, class TDest>
class CVisFnOpOffsetAndScaleSrcMagToDest
{
public:
	CVisFnOpOffsetAndScaleSrcMagToDest(double dblOffset, double dblScale);
	inline void operator()(const TSrc& refsrc, TDest& refdest);
protected:
	const double m_dblOffset;
	const double m_dblScale;
};


template<class TSrc, class TDest>
class CVisFnOpAdjustFloatToSignedInt
{
public:
	CVisFnOpAdjustFloatToSignedInt(TDest destMin, TDest destMax);
	inline void operator()(const TSrc& refsrc, TDest& refdest);
protected:
	const TDest m_destMin;
	const TDest m_destMax;
	const double m_dblScale;
};


template<class TSrc, class TDest>
class CVisFnOpAdjustFloatToUnsignedInt
{
public:
	CVisFnOpAdjustFloatToUnsignedInt(TDest destMax);
	inline void operator()(const TSrc& refsrc, TDest& refdest);
protected:
	const TDest m_destMax;
	const double m_dblScale;
};


template<class TSrc, class TDest>
class CVisFnOpAdjustFloatToFloat
{
public:
	inline void operator()(const TSrc& refsrc, TDest& refdest);
};


#include "VisCopyFnOps.inl"

