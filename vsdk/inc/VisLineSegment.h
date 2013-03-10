// VisLineSegment.h
//
// Class used to represent line segments.
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved

#pragma once


template<class TPoint>
class CVisLineSegment : public std::pair<TPoint, TPoint>
{
public:
	//---------------------------------------------------------------------
	// Type definition

	typedef TPoint T_Point;


	//---------------------------------------------------------------------
	// Constructors

	CVisLineSegment(void);
	CVisLineSegment(const TPoint& refpoint0, const TPoint& refpoint1);


	//---------------------------------------------------------------------
	// Point access

	const TPoint& operator[](int i) const;
	TPoint& operator[](int i);

	const TPoint& StartPoint(void) const;
	TPoint& StartPoint(int i);

	const TPoint& EndPoint(void) const;
	TPoint& EndPoint(int i);
};




// Inline functions
// LATER:  We may want to move these to an INL file.


template<class TPoint>
inline CVisLineSegment<TPoint>::CVisLineSegment(void)
  : std::pair<TPoint, TPoint>()
{
}

template<class TPoint>
inline CVisLineSegment<TPoint>::CVisLineSegment(const TPoint& refpoint0,
		const TPoint& refpoint1)
  : std::pair<TPoint, TPoint>(refpoint0, refpoint1)
{
}

template<class TPoint>
inline const TPoint& CVisLineSegment<TPoint>::operator[](int i) const
{
	assert((i == 0) || (i == 1));
	return ((i == 0) ? first : second);
}

template<class TPoint>
inline TPoint& CVisLineSegment<TPoint>::operator[](int i)
{
	assert((i == 0) || (i == 1));
	return ((i == 0) ? first : second);
}

template<class TPoint>
inline const TPoint& CVisLineSegment<TPoint>::StartPoint(void) const
{
	return first;
}

template<class TPoint>
inline TPoint& CVisLineSegment<TPoint>::StartPoint(int i)

{
	return first;
}

template<class TPoint>
inline const TPoint& CVisLineSegment<TPoint>::EndPoint(void) const
{
	return second;
}

template<class TPoint>
inline TPoint& CVisLineSegment<TPoint>::EndPoint(int i)
{
	return second;
}
