
#pragma once

#include "Point2.h"

class CSBASE_API CGeomTransform
{
protected:
	bool m_fTransReady;
	bool m_fInvTransReady;
	CGeomTransform(bool fTransReady = false, bool fInvTransReady = false);
public:
	virtual ~CGeomTransform(void);

	bool TransReady() const;
	bool InvTransReady() const;

	virtual bool Transform(const CPoint2 inPt,  CPoint2& outPt) const = 0;
	virtual bool InvTransform(const CPoint2 inPt,  CPoint2& outPt) const = 0;
	
	virtual CGeomTransform * Clone() const =0;
	
	void Testing() const;
};

class CGeomTransSequence : public CGeomTransform
{
protected:
	CGeomTransform * m_pTrans1;
	CGeomTransform * m_pTrans2;
public:
	CGeomTransSequence(CGeomTransform * tran1, CGeomTransform * tran2);
	virtual ~CGeomTransSequence();
	void Clear();

	virtual bool Transform(const CPoint2 inPt,  CPoint2& outPt) const;
	virtual bool InvTransform(const CPoint2 inPt,  CPoint2& outPt) const;

	virtual CGeomTransform * Clone() const;
};

//************************************************************************
//*																		 *
//*                           inline functions							 *
//*																		 *
//************************************************************************
inline CGeomTransform::CGeomTransform(bool fTransReady, bool fInvTransReady)
	: m_fTransReady(fTransReady), m_fInvTransReady(fInvTransReady) 
{
};

inline CGeomTransform::~CGeomTransform(void)
{
};

inline bool CGeomTransform::TransReady() const
{
	return m_fTransReady;
};

inline bool CGeomTransform::InvTransReady() const
{
	return m_fInvTransReady;
};

inline CGeomTransSequence::~CGeomTransSequence()
{
	Clear();
};

inline void CGeomTransSequence::Clear() 
{
	delete m_pTrans1; 
	delete m_pTrans2;
	m_fTransReady = false;
	m_fInvTransReady = false;
};

inline bool CGeomTransSequence::Transform(const CPoint2 inPt,  CPoint2& outPt) const
{
	if (!m_fTransReady)
		return false;
	m_pTrans1->Transform( inPt, outPt );
	m_pTrans2->Transform( outPt, outPt );
	return true;
};

inline bool CGeomTransSequence::InvTransform(const CPoint2 inPt,  CPoint2& outPt) const
{
	if (!m_fInvTransReady)
		return false;
	m_pTrans2->InvTransform( inPt, outPt );
	m_pTrans1->InvTransform( outPt, outPt );
	return true;
};

inline CGeomTransform * CGeomTransSequence::Clone() const
{
	CGeomTransSequence * newS = new CGeomTransSequence(m_pTrans1->Clone(),m_pTrans2->Clone());
	return newS;
};

CSBASE_API void ApplyTransform(const CShape& orgShape, CShape& desShape, const CGeomTransform& geomTrans, const bool fNotInv = true);
