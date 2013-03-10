// VisCompareFnOps.h


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



template<class TNum>
class CVisFnOpCompareEQ
{
public:
	CVisFnOpCompareEQ(const TNum& refnum = TNum())
		: m_num(refnum) {}

	bool FCompare(const TNum& refnum) const
		{ return (refnum == m_num); }

	bool FCompare(const TNum& refnum1, const TNum& refnum2) const
		{ return (refnum1 == refnum2); }

protected:
	const TNum m_num;
};


template<class TNum>
class CVisFnOpCompareNE
{
public:
	CVisFnOpCompareNE(const TNum& refnum = TNum())
		: m_num(refnum) {}

	bool FCompare(const TNum& refnum) const
		{ return (refnum != m_num); }

	bool FCompare(const TNum& refnum1, const TNum& refnum2) const
		{ return (refnum1 != refnum2); }

protected:
	const TNum m_num;
};


template<class TNum>
class CVisFnOpCompareLT
{
public:
	CVisFnOpCompareLT(const TNum& refnum = TNum())
		: m_num(refnum) {}

	bool FCompare(const TNum& refnum) const
		{ return (refnum < m_num); }

	bool FCompare(const TNum& refnum1, const TNum& refnum2) const
		{ return (refnum1 < refnum2); }

protected:
	const TNum m_num;
};


template<class TNum>
class CVisFnOpCompareLE
{
public:
	CVisFnOpCompareLE(const TNum& refnum = TNum())
		: m_num(refnum) {}

	bool FCompare(const TNum& refnum) const
		{ return (refnum <= m_num); }

	bool FCompare(const TNum& refnum1, const TNum& refnum2) const
		{ return (refnum1 <= refnum2); }

protected:
	const TNum m_num;
};


template<class TNum>
class CVisFnOpCompareGT
{
public:
	CVisFnOpCompareGT(const TNum& refnum = TNum())
		: m_num(refnum) {}

	bool FCompare(const TNum& refnum) const
		{ return (refnum > m_num); }

	bool FCompare(const TNum& refnum1, const TNum& refnum2) const
		{ return (refnum1 > refnum2); }

protected:
	const TNum m_num;
};


template<class TNum>
class CVisFnOpCompareGE
{
public:
	CVisFnOpCompareGE(const TNum& refnum = TNum())
		: m_num(refnum) {}

	bool FCompare(const TNum& refnum) const
		{ return (refnum >= m_num); }

	bool FCompare(const TNum& refnum1, const TNum& refnum2) const
		{ return (refnum1 >= refnum2); }

protected:
	const TNum m_num;
};


template<class TNum>
class CVisFnOpCompareBetween
{
public:
	CVisFnOpCompareBetween(const TNum& refnumLow = TNum(),
			const TNum& refnumHigh = TNum())
		: m_numLow(refnumLow), m_numHigh(refnumHigh) {}

	bool FCompare(const TNum& refnum) const
		{ return ((refnum >= m_numLow) && (refnum <= m_numHigh)); }

	bool FCompare(const TNum& refnum1, const TNum& refnum2,
			const TNum& refnum3) const
		{ return ((refnum1 >= refnum2) && (refnum1 <= refnum3)); }

protected:
	const TNum m_numLow;
	const TNum m_numHigh;
};


template<class TNum>
class CVisFnOpCompareOutside
{
public:
	CVisFnOpCompareOutside(const TNum& refnumLow = TNum(),
			const TNum& refnumHigh = TNum())
		: m_numLow(refnumLow), m_numHigh(refnumHigh) {}

	bool FCompare(const TNum& refnum) const
		{ return ((refnum < m_numLow) || (refnum > m_numHigh)); }

	bool FCompare(const TNum& refnum1, const TNum& refnum2,
			const TNum& refnum3) const
		{ return ((refnum1 < refnum2) || (refnum1 > refnum3)); }

protected:
	const TNum m_numLow;
	const TNum m_numHigh;
};

