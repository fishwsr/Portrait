// VisThresholdFnOps.h
// Example thresholding operators.
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



// LATER:  We may want to define the fn operator methods in the class
// definition to make it cleat how the fn ops work.

template<class TPixel>
class CVisFnOpPixelLTVal
{
public:
	CVisFnOpPixelLTVal(const TPixel& refpixelVal);
	BOOL operator()(const TPixel& refpixel) const;
private:
	const TPixel m_pixelVal;
};


template<class TPixel>
class CVisFnOpPixelLEVal
{
public:
	CVisFnOpPixelLEVal(const TPixel& refpixelVal);
	BOOL operator()(const TPixel& refpixel) const;
private:
	const TPixel m_pixelVal;
};


template<class TPixel>
class CVisFnOpPixelGTVal
{
public:
	CVisFnOpPixelGTVal(const TPixel& refpixelVal);
	BOOL operator()(const TPixel& refpixel) const;
private:
	const TPixel m_pixelVal;
};


template<class TPixel>
class CVisFnOpPixelGEVal
{
public:
	CVisFnOpPixelGEVal(const TPixel& refpixelVal);
	BOOL operator()(const TPixel& refpixel) const;
private:
	const TPixel m_pixelVal;
};


template<class TPixel>
class CVisFnOpPixelNEVal
{
public:
	CVisFnOpPixelNEVal(const TPixel& refpixelVal);
	BOOL operator()(const TPixel& refpixel) const;
private:
	const TPixel m_pixelVal;
};


template<class TPixel>
class CVisFnOpPixelEQVal
{
public:
	CVisFnOpPixelEQVal(const TPixel& refpixelVal);
	BOOL operator()(const TPixel& refpixel) const;
private:
	const TPixel m_pixelVal;
};


template<class TPixel>
class CVisFnOpPixelGTValLTVal
{
public:
	CVisFnOpPixelGTValLTVal(const TPixel& refpixelGTVal,
			const TPixel& refpixelLTVal);
	BOOL operator()(const TPixel& refpixel) const;
private:
	const TPixel m_pixelGTVal;
	const TPixel m_pixelLTVal;
};


template<class TPixel>
class CVisFnOpPixelGTValLEVal
{
public:
	CVisFnOpPixelGTValLEVal(const TPixel& refpixelGTVal,
			const TPixel& refpixelLEVal);
	BOOL operator()(const TPixel& refpixel) const;
private:
	const TPixel m_pixelGTVal;
	const TPixel m_pixelLEVal;
};


template<class TPixel>
class CVisFnOpPixelGEValLTVal
{
public:
	CVisFnOpPixelGEValLTVal(const TPixel& refpixelGEVal,
			const TPixel& refpixelLTVal);
	BOOL operator()(const TPixel& refpixel) const;
private:
	const TPixel m_pixelGEVal;
	const TPixel m_pixelLTVal;
};


template<class TPixel>
class CVisFnOpPixelGEValLEVal
{
public:
	CVisFnOpPixelGEValLEVal(const TPixel& refpixelGEVal,
			const TPixel& refpixelLEVal);
	BOOL operator()(const TPixel& refpixel) const;
private:
	const TPixel m_pixelGEVal;
	const TPixel m_pixelLEVal;
};


template<class TPixel, class TFnOp>
class CVisFnOpNotPixelFnOp
{
public:
	CVisFnOpNotPixelFnOp(const TFnOp& reffnop);
	BOOL operator()(const TPixel& refpixel) const;
private:
	const TFnOp m_fnop;
};


template<class TPixel, class TFnOp1, class TFnOp2>
class CVisFnOpAndPixelFnOps
{
public:
	CVisFnOpAndPixelFnOps(const TFnOp1& reffnop1,
			const TFnOp2& reffnop2);
	BOOL operator()(const TPixel& refpixel) const;
private:
	const TFnOp1 m_fnop1;
	const TFnOp2 m_fnop2;
};


template<class TPixel, class TFnOp1, class TFnOp2>
class CVisFnOpOrPixelFnOps
{
public:
	CVisFnOpOrPixelFnOps(const TFnOp1& reffnop1,
			const TFnOp2& reffnop2);
	BOOL operator()(const TPixel& refpixel) const;
private:
	const TFnOp1 m_fnop1;
	const TFnOp2 m_fnop2;
};


typedef CVisFnOpPixelLTVal<BYTE> CVisFnOpByteLTVal;
typedef CVisFnOpPixelLEVal<BYTE> CVisFnOpByteLEVal;
typedef CVisFnOpPixelGTVal<BYTE> CVisFnOpByteGTVal;
typedef CVisFnOpPixelGEVal<BYTE> CVisFnOpByteGEVal;
typedef CVisFnOpPixelNEVal<BYTE> CVisFnOpByteNEVal;
typedef CVisFnOpPixelEQVal<BYTE> CVisFnOpByteEQVal;

typedef CVisFnOpPixelGTValLTVal<BYTE> CVisFnOpByteGTValLTVal;
typedef CVisFnOpPixelGTValLEVal<BYTE> CVisFnOpByteGTValLEVal;
typedef CVisFnOpPixelGEValLTVal<BYTE> CVisFnOpByteGEValLTVal;
typedef CVisFnOpPixelGEValLEVal<BYTE> CVisFnOpByteGEValLEVal;


#include "VisThresholdFnOps.inl"