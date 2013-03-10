// VisObjList.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG


template<class TObj, int kcObjAllocUnit>
inline CVisObjListNode<TObj, kcObjAllocUnit>::CVisObjListNode()
  : m_cRef(1),
	m_cObjCur(0),
	m_pnodePrev(0),
	m_pnodeNext(0)
{
	assert(kcObjAllocUnit > 0);
}


template<class TObj, int kcObjAllocUnit>
inline unsigned long CVisObjListNode<TObj, kcObjAllocUnit>::AddRef(void)
{
	// AddRef and release should only be called on the first node in
	// a list.
	assert(m_pnodePrev == 0);

	return ++m_cRef;
}


template<class TObj, int kcObjAllocUnit>
inline unsigned long CVisObjListNode<TObj, kcObjAllocUnit>::Release(void)
{
	// AddRef and release should only be called on the first node in
	// a list.
	assert(m_pnodePrev == 0);

	unsigned long cRef = --m_cRef;

	if (cRef == 0)
		delete this;

	return cRef;
}


template<class TObj, int kcObjAllocUnit>
inline BOOL CVisObjListNode<TObj, kcObjAllocUnit>::FIsFull(void) const
{
	return (m_cObjCur >= kcObjAllocUnit);
}


template<class TObj, int kcObjAllocUnit>
inline int CVisObjListNode<TObj, kcObjAllocUnit>::CObj(void) const
{
	return m_cObjCur;
}


template<class TObj, int kcObjAllocUnit>
inline TObj *CVisObjListNode<TObj, kcObjAllocUnit>::RgObj(void)
{
	return m_rgObj;
}


template<class TObj, int kcObjAllocUnit>
inline TObj& CVisObjListNode<TObj, kcObjAllocUnit>::ObjNext(void)
{
	assert(!FIsFull());
	return RgObj()[m_cObjCur];
}


// This returns a pointer to a node that has space for additional objects.
template<class TObj, int kcObjAllocUnit>
inline CVisObjListNode<TObj, kcObjAllocUnit> *
		CVisObjListNode<TObj, kcObjAllocUnit>::AddObj(void)
{
	++ m_cObjCur;

	if (!FIsFull())
		return this;

	// Need to find the next node with room for additional objects.
	CVisObjListNode<TObj, kcObjAllocUnit> *pnodePrev = this;
	CVisObjListNode<TObj, kcObjAllocUnit> *pnode = m_pnodeNext;
	while ((pnode != 0) && (pnode->FIsFull()))
	{
		pnodePrev = pnode;
		pnode = pnode->m_pnodeNext;
	}

	if (pnode == 0)
	{
		pnode = new CVisObjListNode<TObj, kcObjAllocUnit>;
		assert(pnode != 0);  // operator new should throw.
		pnodePrev->m_pnodeNext = pnode;
		pnode->m_pnodePrev = pnodePrev;
	}

	return pnode;
}


// This returns a pointer to a node that has space for additional objects.
template<class TObj, int kcObjAllocUnit>
inline CVisObjListNode<TObj, kcObjAllocUnit> *
		CVisObjListNode<TObj, kcObjAllocUnit>::AddObj(const TObj& obj)
{
	ObjNext() = obj;
	return AddObj();
}


template<class TObj, int kcObjAllocUnit>
inline CVisObjListNode<TObj, kcObjAllocUnit> *
		CVisObjListNode<TObj, kcObjAllocUnit>::PNodePrev(void)
{
	return m_pnodePrev;
}


template<class TObj, int kcObjAllocUnit>
inline CVisObjListNode<TObj, kcObjAllocUnit> *
		CVisObjListNode<TObj, kcObjAllocUnit>::PNodeNext(void)
{
	return m_pnodeNext;
}

template<class TObj, int kcObjAllocUnit>
inline void CVisObjListNode<TObj, kcObjAllocUnit>::SetPNodePrev(
		CVisObjListNode<TObj, kcObjAllocUnit> *pnode)
{
	m_pnodePrev = pnode;
}

template<class TObj, int kcObjAllocUnit>
inline void CVisObjListNode<TObj, kcObjAllocUnit>::SetPNodeNext(
		CVisObjListNode<TObj, kcObjAllocUnit> *pnode)
{
	m_pnodeNext = pnode;
}


template<class TObj, int kcObjAllocUnit>
inline CVisObjListNode<TObj, kcObjAllocUnit>::~CVisObjListNode(void)
{
	if (m_pnodeNext != 0)
	{
		m_pnodeNext->m_pnodePrev = 0;
		m_pnodeNext->Release();
	}
}



template<class TObj, int kcObjAllocUnit>
inline CVisObjListEnum<TObj, kcObjAllocUnit>::CVisObjListEnum(
		CVisObjListNode<TObj, kcObjAllocUnit> *pnodeFirst,
		CVisObjListNode<TObj, kcObjAllocUnit> *pnodeCur,
		int iObjCur)
  : m_pnodeFirst(pnodeFirst),
	m_pnodeCur(pnodeCur),
	m_iObjCur(iObjCur)
{
	assert(m_pnodeFirst != 0);
	assert(m_pnodeCur != 0);

	m_pnodeFirst->AddRef();
}


template<class TObj, int kcObjAllocUnit>
inline CVisObjListEnum<TObj, kcObjAllocUnit>::CVisObjListEnum(
		const CVisObjListEnum<TObj, kcObjAllocUnit>& reflistobjenum)
  : m_pnodeFirst(reflistobjenum.m_pnodeFirst),
	m_pnodeCur(reflistobjenum.m_pnodeCur),
	m_iObjCur(reflistobjenum.m_iObjCur)
{
	assert(m_pnodeFirst != 0);
	assert(m_pnodeCur != 0);

	m_pnodeFirst->AddRef();
}


template<class TObj, int kcObjAllocUnit>
inline CVisObjListEnum<TObj, kcObjAllocUnit>&
	CVisObjListEnum<TObj, kcObjAllocUnit>::operator=(
		const CVisObjListEnum<TObj, kcObjAllocUnit>& reflistobjenum)
{
	if (m_pnodeFirst != 0)
		m_pnodeFirst->Release();

	m_pnodeFirst = reflistobjenum.m_pnodeFirst;
	m_pnodeCur = reflistobjenum.m_pnodeCur;
	m_iObjCur = reflistobjenum.m_iObjCur;

	assert(m_pnodeFirst != 0);
	assert(m_pnodeCur != 0);

	m_pnodeFirst->AddRef();

	return *this;
}


template<class TObj, int kcObjAllocUnit>
inline CVisObjListEnum<TObj, kcObjAllocUnit>::~CVisObjListEnum(void)
{
	if (m_pnodeFirst != 0)
		m_pnodeFirst->Release();
}


template<class TObj, int kcObjAllocUnit>
inline TObj& CVisObjListEnum<TObj, kcObjAllocUnit>::ObjCur(void)
{
	assert(FMore());
	return m_pnodeCur->RgObj()[m_iObjCur];
}


template<class TObj, int kcObjAllocUnit>
inline BOOL CVisObjListEnum<TObj, kcObjAllocUnit>::FMore(void) const
{
	return (m_iObjCur < m_pnodeCur->CObj());
}


template<class TObj, int kcObjAllocUnit>
inline void CVisObjListEnum<TObj, kcObjAllocUnit>::Next(void)
{
	// Use while instead of if to skip empty nodes.
	while ((++m_iObjCur >= m_pnodeCur->CObj())
			&& (m_pnodeCur->PNodeNext() != 0))
	{
		m_pnodeCur = m_pnodeCur->PNodeNext();
		m_iObjCur = - 1;
	}
}


template<class TObj, int kcObjAllocUnit>
inline void CVisObjListEnum<TObj, kcObjAllocUnit>::Reset(void)
{
	m_pnodeCur = m_pnodeFirst;
	m_iObjCur = 0;
}





template<class TObj, int kcObjAllocUnit>
inline bool CVisObjListEnum<TObj, kcObjAllocUnit>::operator==(
		const CVisObjListEnum& reflistobjenum) const
{
	assert((reflistobjenum.m_pnodeCur != m_pnodeCur)
			|| (reflistobjenum.m_iObjCur != m_iObjCur)
			|| (reflistobjenum.m_pnodeFirst == m_pnodeFirst));

	return ((reflistobjenum.m_pnodeCur == m_pnodeCur)
			&& (reflistobjenum.m_iObjCur == m_iObjCur));
}


template<class TObj, int kcObjAllocUnit>
inline bool CVisObjListEnum<TObj, kcObjAllocUnit>::operator!=(
		const CVisObjListEnum& reflistobjenum) const
{
	return !operator==(reflistobjenum);
}


template<class TObj, int kcObjAllocUnit>
inline TObj& CVisObjListEnum<TObj, kcObjAllocUnit>::operator*(void) const
{
	return ObjCur();
}


template<class TObj, int kcObjAllocUnit>
inline CVisObjListEnum<TObj, kcObjAllocUnit>&
	CVisObjListEnum<TObj, kcObjAllocUnit>::operator++(void)
{
	if (FMore())
		Next();

	return *this;
}


template<class TObj, int kcObjAllocUnit>
inline CVisObjListEnum<TObj, kcObjAllocUnit>
	CVisObjListEnum<TObj, kcObjAllocUnit>::operator++(int)
{
	CVisObjListEnum<TObj, kcObjAllocUnit> iteratorRet = *this;

	++*this;

	return iteratorRet;
}


template<class TObj, int kcObjAllocUnit>
inline CVisObjListEnum<TObj, kcObjAllocUnit>
	CVisObjListEnum<TObj, kcObjAllocUnit>::begin(void)
{
	CVisObjListEnum<TObj, kcObjAllocUnit> iteratorRet = *this;

	iteratorRet.Reset();

	return iteratorRet;
}


template<class TObj, int kcObjAllocUnit>
inline CVisObjListEnum<TObj, kcObjAllocUnit>
	CVisObjListEnum<TObj, kcObjAllocUnit>::end(void)
{
	// Find the last node.
	CVisObjListNode<TObj, kcObjAllocUnit> *pnodeCur = m_pnodeFirst;
	while (pnodeCur->PNodeNext() != 0)
		pnodeCur = m_pnodeCur->PNodeNext();

	return CVisObjListEnum<TObj, kcObjAllocUnit>(m_pnodeFirst, pnodeCur,
			pnodeCur->CObj());
}




template<class TObj, int kcObjAllocUnit>
inline CVisObjListEnum<TObj, kcObjAllocUnit>::CVisObjListEnum(void)
  : m_pnodeFirst(new CVisObjListNode<TObj, kcObjAllocUnit>),
	m_pnodeCur(m_pnodeFirst),
	m_iObjCur(0)
{
	assert(m_pnodeFirst != 0);
}

template<class TObj, int kcObjAllocUnit>
inline CVisObjListNode<TObj, kcObjAllocUnit> *
	CVisObjListEnum<TObj, kcObjAllocUnit>::PNodeFirst(void) const
{
	return m_pnodeFirst;
}

template<class TObj, int kcObjAllocUnit>
inline void
	CVisObjListEnum<TObj, kcObjAllocUnit>::SetPNodeFirst(
		CVisObjListNode<TObj, kcObjAllocUnit> *pnode)
{
	m_pnodeFirst = pnode;
	m_pnodeCur = pnode;
	m_iObjCur = 0;
}

template<class TObj, int kcObjAllocUnit>
inline void
	CVisObjListEnum<TObj, kcObjAllocUnit>::ClearData(void)
{
	m_pnodeFirst = 0;
	m_pnodeCur = 0;
	m_iObjCur = 0;
}


template<class TObj, int kcObjAllocUnit>
inline CVisObjListBuilder<TObj, kcObjAllocUnit>::CVisObjListBuilder(void)
  : CVisObjListEnum<TObj, kcObjAllocUnit>(),
	m_pnodeFirstNotFull(PNodeFirst())
{
}


template<class TObj, int kcObjAllocUnit>
inline CVisObjListBuilder<TObj, kcObjAllocUnit>::CVisObjListBuilder(
		const CVisObjListEnum<TObj, kcObjAllocUnit>& reflistobjenum)
  : CVisObjListEnum<TObj, kcObjAllocUnit>(reflistobjenum),
	m_pnodeFirstNotFull(0)
{
	// Only the original CVisObjListBuilder object can be used to add
	// objects to the list.
}


template<class TObj, int kcObjAllocUnit>
inline CVisObjListBuilder<TObj, kcObjAllocUnit>&
	CVisObjListBuilder<TObj, kcObjAllocUnit>::operator=(
		const CVisObjListEnum<TObj, kcObjAllocUnit>& reflistobjenum)
{
	*((CVisObjListEnum<TObj, kcObjAllocUnit> *) this) = reflistobjenum;

	// Only the original CVisObjListBuilder object can be used to add
	// objects to the list.
	m_pnodeFirstNotFull = 0;

	return *this;
}


template<class TObj, int kcObjAllocUnit>
inline TObj& CVisObjListBuilder<TObj, kcObjAllocUnit>::ObjNext(void)
{
	assert(FCanAdd());
	return m_pnodeFirstNotFull->ObjNext();
}


template<class TObj, int kcObjAllocUnit>
inline BOOL CVisObjListBuilder<TObj, kcObjAllocUnit>::FCanAdd(void) const
{
	assert((m_pnodeFirstNotFull == 0)
			|| (!(m_pnodeFirstNotFull->FIsFull())));

	return (m_pnodeFirstNotFull != 0);
}


template<class TObj, int kcObjAllocUnit>
inline void CVisObjListBuilder<TObj, kcObjAllocUnit>::AddObj(void)
{
	assert(FCanAdd());
	m_pnodeFirstNotFull = m_pnodeFirstNotFull->AddObj();
}


template<class TObj, int kcObjAllocUnit>
inline void CVisObjListBuilder<TObj, kcObjAllocUnit>::AddObj(const TObj& obj)
{
	assert(FCanAdd());
	m_pnodeFirstNotFull = m_pnodeFirstNotFull->AddObj(obj);
}

template<class TObj, int kcObjAllocUnit>
inline void
	CVisObjListBuilder<TObj, kcObjAllocUnit>::MergeInto(
		CVisObjListBuilder<TObj, kcObjAllocUnit>& reflistbuilder)
{
	reflistbuilder.AddNodes(*this);
	ClearData();
}


template<class TObj, int kcObjAllocUnit>
inline CVisObjListNode<TObj, kcObjAllocUnit> *
	CVisObjListBuilder<TObj, kcObjAllocUnit>::PNodeFirstNotFull(void) const
{
	return m_pnodeFirstNotFull;
}

template<class TObj, int kcObjAllocUnit>
inline void
	CVisObjListBuilder<TObj, kcObjAllocUnit>::AddNodes(
		const CVisObjListBuilder<TObj, kcObjAllocUnit>& reflistbuilder)
{
	CVisObjListNode<TObj, kcObjAllocUnit> *pnodeFirstOther
			= reflistbuilder.PNodeFirst();
	CVisObjListNode<TObj, kcObjAllocUnit> *pnodeFirstNotFullOther
			= reflistbuilder.PNodeFirstNotFull();
	assert(pnodeFirstOther != 0);
	assert(pnodeFirstNotFullOther != 0);

	CVisObjListNode<TObj, kcObjAllocUnit> *pnodeLastOther
			= pnodeFirstNotFullOther;
	while (pnodeLastOther->PNodeNext() != 0)
		pnodeLastOther = pnodeLastOther->PNodeNext();
	assert(pnodeLastOther != 0);

	if (PNodeFirst() == PNodeFirstNotFull())
	{
		SetPNodeFirst(pnodeFirstOther);

		pnodeLastOther->SetPNodeNext(PNodeFirstNotFull());
		PNodeFirstNotFull()->SetPNodePrev(pnodeLastOther);

		m_pnodeFirstNotFull = pnodeFirstNotFullOther;
	}
	else
	{
		CVisObjListNode<TObj, kcObjAllocUnit> *pnodeLastFull
				= PNodeFirstNotFull()->PNodePrev();
		assert(pnodeLastFull != 0);

		pnodeLastFull->SetPNodeNext(pnodeFirstOther);
		pnodeFirstOther->SetPNodePrev(pnodeLastFull);

		pnodeLastOther->SetPNodeNext(PNodeFirstNotFull());
		PNodeFirstNotFull()->SetPNodePrev(pnodeLastOther);

		m_pnodeFirstNotFull = pnodeFirstNotFullOther;
	}
}


#ifdef _DEBUG
#undef new
#endif // _DEBUG
