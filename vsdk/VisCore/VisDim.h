// VisDim.h
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


// Forward declaration
class CVisDim;
#pragma once

class VisPropListExport CVisDimIndex
{
public:
	CVisDimIndex(int i0 = 0, int i1 = 0, int i2 = 0, int i3 = 0);
	CVisDimIndex(const CVisDimIndex& refdimindex);
	CVisDimIndex& operator=(const CVisDimIndex& refdimindex);

	int operator[](int i) const;

	bool operator==(const CVisDimIndex& refdimindex) const;
	bool operator!=(const CVisDimIndex& refdimindex) const;

#ifdef _DEBUG
	bool IsInRange(const CVisDim& refdim) const;
#endif _DEBUG

protected:
	int m_rgi[4];
};


class VisPropListExport CVisDimIterator : public CVisDimIndex
{
public:
	CVisDimIterator(const CVisDim& refdim);
	CVisDimIterator(const CVisDim& refdim, int i0, int i1 = 0, int i2 = 0,
			int i3 = 0);
	CVisDimIterator(const CVisDim& refdim, const CVisDimIndex& refdimindex);
	CVisDimIterator(const CVisDimIterator& refdimiterator);
	CVisDimIterator& operator=(const CVisDimIterator& refdimiterator);
	CVisDimIterator& operator=(const CVisDimIndex& refdimindex);

	bool operator==(const CVisDimIndex& refdimindex) const;
	bool operator==(const CVisDimIterator& refdimiterator) const;
	bool operator!=(const CVisDimIndex& refdimindex) const;
	bool operator!=(const CVisDimIterator& refdimiterator) const;

	CVisDimIterator& operator++(void);
	CVisDimIterator operator++(int);

#ifdef LATER
	CVisDimIterator& operator--(void);
	CVisDimIterator operator--(int);
#endif // LATER
	
	const CVisDim& Dim(void) const;

protected:
	friend class CVisDim;

	void SetToEnd(void);

private:
	const CVisDim *m_pdim;
	bool m_fAtEnd;
};


class VisPropListExport CVisDim
{
public:
	CVisDim(int i0 = 0, int i1 = 0, int i2 = 0, int i3 = 0);
	CVisDim(const CVisDim& refdim);
	CVisDim& operator=(const CVisDim& refdim);


	bool operator==(const CVisDim& refdim) const;
	bool operator!=(const CVisDim& refdim) const;

	int operator[](int i) const;

	int CDim(void) const;
	int CObj(void) const;

	CVisDimIterator begin(void) const;
	CVisDimIterator end(void) const;


private:
	int m_cDim;
	int m_cObj;
	int m_rgwDim[4];
};


#include "VisDim.inl"
