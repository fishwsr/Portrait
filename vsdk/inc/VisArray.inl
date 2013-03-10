// VisArray.inl
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved


#include <assert.h>


#if defined(_DEBUG) && defined(_AFX)
//#if defined(_DEBUG)
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // defined(_DEBUG)


template<class _Ty, class _A>
inline CVisAI<_Ty, _A>::CVisAI(const _A& _Al)
  : T_Base(_Al),
	m_cRef(1)
{
}

template<class _Ty, class _A>
inline CVisAI<_Ty, _A>::CVisAI(size_type _N,
		const _Ty& _V, const _A& _Al)
  : T_Base(_N, _V, _Al),
	m_cRef(1)
{
}

template<class _Ty, class _A>
inline CVisAI<_Ty, _A>::CVisAI(
		const T_Vector& _X)
  : T_Base(_X),
	m_cRef(1)
{
}

template<class _Ty, class _A>
inline CVisAI<_Ty, _A>::CVisAI(
		const T_StlVector& _X)
  : T_Base(_X),
	m_cRef(1)
{
}

template<class _Ty, class _A>
inline CVisAI<_Ty, _A>::CVisAI(const_iterator _F,
		const_iterator _L, const _A& _Al)
  : T_Base(_F, _L, _Al),
	m_cRef(1)
{
}

template<class _Ty, class _A>
inline unsigned long CVisAI<_Ty, _A>::AddRef(void)
{
	return (unsigned long) InterlockedIncrement((LONG *) &m_cRef);
}

template<class _Ty, class _A>
inline unsigned long CVisAI<_Ty, _A>::Release(void)
{
	unsigned long cRef
			= (unsigned long) InterlockedDecrement((LONG *) &m_cRef);

	if (cRef == 0)
		delete this;

	return cRef;
}

template<class _Ty, class _A>
inline void CVisAI<_Ty, _A>::Copy(const T_Base& _X)
{
	if ((&_X == 0) || (_X.empty()))
	{
		clear();
	}
	else
	{
		*this = _X;
	}
}

template<class _Ty, class _A>
inline CVisAI<_Ty, _A>::~CVisAI(void)
{
}



template<class TObj, class _A>
inline CVisArrayBase<TObj, _A>::CVisArrayBase(void)
  : m_parrayinternal(0)
{
}

template<class TObj, class _A>
inline CVisArrayBase<TObj, _A>::CVisArrayBase(
		const T_Vector& _X)
  : m_parrayinternal(new CVisAI<TObj, _A>(_X))
{
}

template<class TObj, class _A>
inline CVisArrayBase<TObj, _A>::CVisArrayBase(
		const T_StlVector& _X)
  : m_parrayinternal(new CVisAI<TObj, _A>(_X))
{
}
template<class TObj, class _A>
inline CVisArrayBase<TObj, _A>::CVisArrayBase(const _A& _Al)
  : m_parrayinternal(new CVisAI<TObj, _A>(_Al))
{
}

template<class TObj, class _A>
inline CVisArrayBase<TObj, _A>::CVisArrayBase(size_type n,
		const TObj& _V, const _A& _Al)
  : m_parrayinternal(new CVisAI<TObj, _A>(n, _V, _Al))
{
}

template<class TObj, class _A>
inline CVisArrayBase<TObj, _A>::CVisArrayBase(
		const_iterator _F, const_iterator _L, const _A& _Al)
  : m_parrayinternal(new CVisAI<TObj, _A>(_F, _L, _Al))
{
}

template<class TObj, class _A>
inline CVisArrayBase<TObj, _A>::CVisArrayBase(
		const CVisArrayBase<TObj, _A>& _X)
  : m_parrayinternal(_X.m_parrayinternal)
{
	if (m_parrayinternal != 0)
		m_parrayinternal->AddRef();
}

template<class TObj, class _A>
inline CVisArrayBase<TObj, _A>&
	CVisArrayBase<TObj, _A>::operator=(
		const CVisArrayBase<TObj, _A>& _X)
{
	if (m_parrayinternal != _X.m_parrayinternal)
	{
		if (m_parrayinternal != 0)
			m_parrayinternal->Release();

		m_parrayinternal = _X.m_parrayinternal;

		if (m_parrayinternal != 0)
			m_parrayinternal->AddRef();
	}

	return *this;
}

template<class TObj, class _A>
inline CVisArrayBase<TObj, _A>::~CVisArrayBase(void)
{
	ClearArrayInternal();
}

template<class TObj, class _A>
inline CVisArrayBase<TObj, _A>::const_iterator
	CVisArrayBase<TObj, _A>::begin(void) const
{
	if (m_parrayinternal == 0)
		return 0;
	return m_parrayinternal->begin();
}

template<class TObj, class _A>
inline CVisArrayBase<TObj, _A>::const_iterator
	CVisArrayBase<TObj, _A>::end(void) const
{
	if (m_parrayinternal == 0)
		return 0;
	return m_parrayinternal->end();
}

template<class TObj, class _A>
inline int CVisArrayBase<TObj, _A>::size(void) const
{
	if (m_parrayinternal == 0)
		return 0;
	return m_parrayinternal->size();
}

template<class TObj, class _A>
inline bool CVisArrayBase<TObj, _A>::empty(void) const
{
	if (m_parrayinternal == 0)
		return true;
	return m_parrayinternal->empty();
}

template<class TObj, class _A>
inline CVisArrayBase<TObj, _A>::const_reference
	CVisArrayBase<TObj, _A>::at(int i) const
{
	assert(m_parrayinternal != 0);
	return m_parrayinternal->at(i);
}

template<class TObj, class _A>
inline CVisArrayBase<TObj, _A>::const_reference
	CVisArrayBase<TObj, _A>::operator[](int i) const
{
	assert(m_parrayinternal != 0);
	return (*m_parrayinternal)[i];
}

template<class TObj, class _A>
inline CVisArrayBase<TObj, _A>::const_reference
	CVisArrayBase<TObj, _A>::front(void) const
{
	assert(m_parrayinternal != 0);
	return m_parrayinternal->front();
}

template<class TObj, class _A>
inline CVisArrayBase<TObj, _A>::const_reference
	CVisArrayBase<TObj, _A>::back(void) const
{
	assert(m_parrayinternal != 0);
	return m_parrayinternal->back();
}

template<class TObj, class _A>
inline void CVisArrayBase<TObj, _A>::ClearArrayInternal(void)
{
	if (m_parrayinternal != 0)
	{
		m_parrayinternal->Release();
		m_parrayinternal = 0;
	}
}

template<class TObj, class _A>
inline void CVisArrayBase<TObj, _A>::EnsureArrayInternal(void)
{
	if (m_parrayinternal == 0)
	{
		m_parrayinternal = new CVisAI<TObj, _A>;
	}
}









template<class TObj, class _A>
inline CVisArray<TObj, _A>::CVisArray(void)
  : CVisArrayBase<TObj, _A>()
{
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::CVisArray(const T_Vector& _X)
  : CVisArrayBase<TObj, _A>(_X)
{
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::CVisArray(const T_StlVector& _X)
  : CVisArrayBase<TObj, _A>(_X)
{
}
template<class TObj, class _A>
inline CVisArray<TObj, _A>::CVisArray(const _A& _Al)
  : CVisArrayBase<TObj, _A>(_Al)
{
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::CVisArray(size_type n,
		const TObj& _V, const _A& _Al)
  : CVisArrayBase<TObj, _A>(n, _V, _Al)
{
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::CVisArray(const_iterator _F,
		const_iterator _L, const _A& _Al)
  : CVisArrayBase<TObj, _A>(_F, _L, _Al)
{
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::CVisArray(const T_Array& _X)
  : CVisArrayBase<TObj, _A>(_X)
{
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>&
	CVisArray<TObj, _A>::operator=(const T_Array& _X)
{
	if (m_parrayinternal != _X.m_parrayinternal)
	{
		if (m_parrayinternal != 0)
			m_parrayinternal->Release();

		m_parrayinternal = _X.m_parrayinternal;

		if (m_parrayinternal != 0)
			m_parrayinternal->AddRef();
	}

	return *this;
}


template<class TObj, class _A>
inline bool CVisArray<TObj, _A>::operator==(const T_Vector& _X)
		const
{
	if (m_parrayinternal != 0)
		return m_parrayinternal->_Eq(_X);
	return _X.empty();
}

template<class TObj, class _A>
inline bool CVisArray<TObj, _A>::operator==(
		const T_StlVector& _X) const
{
	if (m_parrayinternal != 0)
		return m_parrayinternal->_Eq(_X);
	return _X.empty();
}

template<class TObj, class _A>
inline bool CVisArray<TObj, _A>::operator==(
		const T_ArrayBase& _X) const
{
	if (m_parrayinternal != 0)
		return m_parrayinternal->_Eq(*(_X.m_parrayinternal));
	return _X.empty();
}

template<class TObj, class _A>
inline bool CVisArray<TObj, _A>::operator<(
		const T_Vector& _X) const
{
	if (m_parrayinternal != 0)
		return m_parrayinternal->_Lt(_X);
	return !_X.empty();
}

template<class TObj, class _A>
inline bool CVisArray<TObj, _A>::operator<(
		const T_StlVector& _X) const
{
	if (m_parrayinternal != 0)
		return m_parrayinternal->_Lt(_X);
	return !_X.empty();
}
template<class TObj, class _A>
inline bool CVisArray<TObj, _A>::operator<(
		const T_ArrayBase& _X) const
{
	if (m_parrayinternal != 0)
		return m_parrayinternal->_Lt(*(_X.m_parrayinternal));
	return !_X.empty();
}


template<class TObj, class _A>
inline CVisArray<TObj, _A>::reference
	CVisArray<TObj, _A>::operator()(size_type i)
{
	EnsureArrayInternal();
	return (*m_parrayinternal)(i);
}


template<class TObj, class _A>
inline CVisArray<TObj, _A>::size_type
	CVisArray<TObj, _A>::capacity(void) const
{
	if (m_parrayinternal == 0)
		return 0;
	return m_parrayinternal->capacity();
}

template<class TObj, class _A>
inline void CVisArray<TObj, _A>::reserve(
		CVisArray<TObj, _A>::size_type n)
{
	EnsureArrayInternal();
	m_parrayinternal->reserve(n);
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::iterator
	CVisArray<TObj, _A>::begin(void)
{
	if (m_parrayinternal == 0)
		return 0;
	return m_parrayinternal->begin();
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::const_iterator
	CVisArray<TObj, _A>::begin(void) const
{
	if (m_parrayinternal == 0)
		return 0;
	return m_parrayinternal->begin();
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::iterator
	CVisArray<TObj, _A>::end(void)
{
	if (m_parrayinternal == 0)
		return 0;
	return m_parrayinternal->end();
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::const_iterator
	CVisArray<TObj, _A>::end(void) const
{
	if (m_parrayinternal == 0)
		return 0;
	return m_parrayinternal->end();
}

template<class TObj, class _A>
inline void CVisArray<TObj, _A>::resize(size_type _N,
		const TObj& _X)
{
	EnsureArrayInternal();
	m_parrayinternal->resize(_N, _X);
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::size_type
	CVisArray<TObj, _A>::size(void) const
{
	if (m_parrayinternal == 0)
		return 0;
	return m_parrayinternal->size();
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::size_type
	CVisArray<TObj, _A>::max_size(void) const
{
	if (m_parrayinternal == 0)
	{
		// From the allocator class definition:
		size_type n = ((size_type) - 1) / sizeof(TObj);
		return ((0 < n) ? n : 1);
	}
	return m_parrayinternal->max_size();
}

template<class TObj, class _A>
inline bool CVisArray<TObj, _A>::empty(void) const
{
	if (m_parrayinternal == 0)
		return true;
	return m_parrayinternal->empty();
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::const_reference
	CVisArray<TObj, _A>::at(size_type _P) const
{
	assert(m_parrayinternal != 0);
	return m_parrayinternal->at(_P);
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::reference
	CVisArray<TObj, _A>::at(size_type _P)
{
	assert(m_parrayinternal != 0);
	return m_parrayinternal->at(_P);
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::const_reference
	CVisArray<TObj, _A>::operator[](size_type _P) const
{
	assert(m_parrayinternal != 0);
	return (*m_parrayinternal)[_P];
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::reference
	CVisArray<TObj, _A>::operator[](size_type _P)
{
	assert(m_parrayinternal != 0);
	return (*m_parrayinternal)[_P];
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::reference
	CVisArray<TObj, _A>::front(void)
{
	assert(m_parrayinternal != 0);
	return m_parrayinternal->front();
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::const_reference
	CVisArray<TObj, _A>::front(void) const
{
	assert(m_parrayinternal != 0);
	return m_parrayinternal->front();
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::reference
	CVisArray<TObj, _A>::back(void)
{
	assert(m_parrayinternal != 0);
	return m_parrayinternal->back();
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::const_reference
	CVisArray<TObj, _A>::back(void) const
{
	assert(m_parrayinternal != 0);
	return m_parrayinternal->back();
}

template<class TObj, class _A>
inline void CVisArray<TObj, _A>::push_back(const TObj& _X)
{
	EnsureArrayInternal();
	m_parrayinternal->push_back(_X);
}

template<class TObj, class _A>
inline void CVisArray<TObj, _A>::pop_back(void)
{
	EnsureArrayInternal();
	m_parrayinternal->pop_back();
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::iterator
	CVisArray<TObj, _A>::insert(iterator _P, const TObj& _X)
{
	EnsureArrayInternal();
	return m_parrayinternal->insert(_P, _X);
}

template<class TObj, class _A>
inline void CVisArray<TObj, _A>::insert(iterator _P,
		size_type _M, const TObj& _X)
{
	EnsureArrayInternal();
	m_parrayinternal->insert(_P, _M, _X);
}

template<class TObj, class _A>
inline void CVisArray<TObj, _A>::insert(iterator _P,
		const_iterator _F, const_iterator _L)
{
	EnsureArrayInternal();
	m_parrayinternal->insert(_P, _F, _L);
}

template<class TObj, class _A>
inline void CVisArray<TObj, _A>::assign(const_iterator _F,
		const_iterator _L)
{
	EnsureArrayInternal();
	m_parrayinternal->assign(_F, _L);
}

template<class TObj, class _A>
inline void CVisArray<TObj, _A>::assign(size_type _N,
		const TObj& _X)
{
	EnsureArrayInternal();
	m_parrayinternal->assign(_N, _X);
}

template<class TObj, class _A>
inline void CVisArray<TObj, _A>::Copy(const T_ArrayBase& _X)
{
	EnsureArrayInternal();
	// This will COPY the elements from _X to *m_parrayinternal.
	m_parrayinternal->Copy(
		*(((const CVisArray<TObj, _A>&) _X).m_parrayinternal));
}

template<class TObj, class _A>
inline void CVisArray<TObj, _A>::clear(void)
{
	if (m_parrayinternal != 0)
		m_parrayinternal->clear();
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::iterator
	CVisArray<TObj, _A>::erase(
		CVisArray<TObj, _A>::iterator _P)
{
	assert(m_parrayinternal != 0);
	return m_parrayinternal->erase(_P);
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::iterator
	CVisArray<TObj, _A>::erase(
		CVisArray<TObj, _A>::iterator _F,
		CVisArray<TObj, _A>::iterator _L)
{
	assert(m_parrayinternal != 0);
	return m_parrayinternal->erase(_F, _L);
}

template<class TObj, class _A>
inline void CVisArray<TObj, _A>::swap(T_Vector& _X)
{
	EnsureArrayInternal();
	m_parrayinternal->swap(_X);
}

template<class TObj, class _A>
inline void CVisArray<TObj, _A>::swap(T_StlVector& _X)
{
	EnsureArrayInternal();
	m_parrayinternal->swap(_X);
}

template<class TObj, class _A>
inline void CVisArray<TObj, _A>::swap(T_Array& _X)
{
	EnsureArrayInternal();
	m_parrayinternal->swap(*(_X.m_parrayinternal));
}

template<class TObj, class _A>
inline void CVisArray<TObj, _A>::NewReference(void)
{
	ClearArrayInternal();
}





template<class TObj, class _A>
inline CVisTable<TObj, _A>::CVisTable(void)
  : CVisArray<CVisArray<TObj, _A> >()
{
}

template<class TObj, class _A>
inline CVisTable<TObj, _A>::CVisTable(const T_Vector& _X)
  : CVisArray<CVisArray<TObj, _A> >(_X)
{
}

template<class TObj, class _A>
inline CVisTable<TObj, _A>::CVisTable(const T_StlVector& _X)
  : CVisArray<CVisArray<TObj, _A> >(_X)
{
}

template<class TObj, class _A>
inline CVisTable<TObj, _A>::CVisTable(int n)
  : CVisArray<CVisArray<TObj, _A>(n)
{
}

template<class TObj, class _A>
inline CVisTable<TObj, _A>::CVisTable(
		T_ArrayBase::const_iterator _F, T_ArrayBase::const_iterator _L)
  : CVisArray<CVisArray<TObj, _A> >(_F, _L)
{
}

template<class TObj, class _A>
inline CVisTable<TObj, _A>::CVisTable(
		const CVisTable<TObj, _A>& _X)
  : CVisArray<CVisArray<TObj, _A> >(_X)
{
}

template<class TObj, class _A>
inline const TObj& CVisTable<TObj, _A>::operator()(int i, int j) const
{
	assert(i < size());
	assert(j < (*m_parrayinternal)[i].size());
	return (*m_parrayinternal)[i][j];
}

template<class TObj, class _A>
inline CVisTable<TObj, _A>::T_ArrayBase::reference
	CVisTable<TObj, _A>::operator()(int i)
{
	EnsureArrayInternal();
	return (*m_parrayinternal)(i);
}

template<class TObj, class _A>
inline TObj& CVisTable<TObj, _A>::operator()(int i, int j)
{
	EnsureArrayInternal();
	return (*m_parrayinternal)(i)(j);
}

template<class TObj, class _A>
inline CVisTable<TObj, _A>& CVisTable<TObj, _A>::operator=(
		const CVisTable<TObj, _A>& _X)
{
	return (CVisTable<TObj, _A>&)
		CVisArray<CVisArray<TObj, _A> >::operator=(_X);
}

template<class TObj, class _A>
inline void CVisTable<TObj, _A>::Copy(const T_ArrayBase& _X)
{
	if ((&_X == 0) || (_X.empty()))
	{
		clear();
	}
	else
	{
		int n = _X.size();
		resize(n);
		for (int i = 0; i < n; ++i)
			(*this)[i].Copy(_X[i]);
	}
}




template<class TObj, class _A>
inline CVisConstArray<TObj, _A>
	CVisArray<TObj, _A>::SubArray(int iFirst, int iLim) const
{
	assert(m_parrayinternal != 0);
	return CVisConstArray<TObj, _A>(*this, iFirst, iLim);
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::operator
	CVisArray<TObj, _A>::T_ConstVector&(void) const
{
// UNDONE:  Since this method is const, we can't create an array here.
//	if (m_parrayinternal == 0)
//		m_parrayinternal = new CVisAI<TObj, _A>;
	return *m_parrayinternal;
}

template<class TObj, class _A>
inline CVisArray<TObj, _A>::operator
	CVisArray<TObj, _A>::T_Vector&(void)
{
	EnsureArrayInternal();
	return *m_parrayinternal;
}



template<class TObj, class _A>
inline CVisConstArray<TObj, _A>::CVisConstArray(void)
  : m_constvector(),
	m_parrayinternal(0)
{
}

template<class TObj, class _A>
inline CVisConstArray<TObj, _A>::CVisConstArray(
		const CVisArray<TObj, _A>& _X, int iFirst, int iLim)
  : m_constvector(_X.begin() + iFirst,
			(iLim < 0) ? _X.end() : _X.begin() + iLim),
	m_parrayinternal(_X.m_parrayinternal)
{
	if (m_parrayinternal != 0)
		m_parrayinternal->AddRef();
}

template<class TObj, class _A>
inline CVisConstArray<TObj, _A>::CVisConstArray(
		const CVisConstArray<TObj, _A>& _X, int iFirst,
		int iLim)
  : m_constvector(_X.begin() + iFirst,
			(iLim < 0) ? _X.end() : _X.begin() + iLim),
	m_parrayinternal(_X.m_parrayinternal)
{
	if (m_parrayinternal != 0)
		m_parrayinternal->AddRef();
}

template<class TObj, class _A>
inline CVisConstArray<TObj, _A>&
	CVisConstArray<TObj, _A>::operator=(
		const CVisArray<TObj, _A>& _X)
{
	bool fSame = (m_parrayinternal == _X.m_parrayinternal);

	if ((!fSame) && (m_parrayinternal != 0))
		m_parrayinternal->Release();

	m_constvector = _X;
	m_parrayinternal = _X.m_parrayinternal;

	if ((!fSame) && (m_parrayinternal != 0))
		m_parrayinternal->AddRef();

	return *this;
}

template<class TObj, class _A>
inline CVisConstArray<TObj, _A>&
	CVisConstArray<TObj, _A>::operator=(
		const CVisConstArray<TObj, _A>& _X)
{
	bool fSame = (m_parrayinternal == _X.m_parrayinternal);

	if ((!fSame) && (m_parrayinternal != 0))
		m_parrayinternal->Release();

	m_constvector = _X;
	m_parrayinternal = _X.m_parrayinternal;

	if ((!fSame) && (m_parrayinternal != 0))
		m_parrayinternal->AddRef();

	return *this;
}

template<class TObj, class _A>
inline CVisConstArray<TObj, _A>::~CVisConstArray(void)
{
	if (m_parrayinternal != 0)
		m_parrayinternal->Release();
}

template<class TObj, class _A>
inline CVisConstArray<TObj, _A>::const_iterator
	CVisConstArray<TObj, _A>::begin(void) const
{
	return m_constvector.begin();
}

template<class TObj, class _A>
inline CVisConstArray<TObj, _A>::const_iterator
	CVisConstArray<TObj, _A>::end(void) const
{
	return m_constvector.end();
}

template<class TObj, class _A>
inline int CVisConstArray<TObj, _A>::size(void) const
{
	return m_constvector.size();
}

template<class TObj, class _A>
inline bool CVisConstArray<TObj, _A>::empty(void) const
{
	return m_constvector.empty();
}

template<class TObj, class _A>
inline CVisConstArray<TObj, _A>::const_reference
	CVisConstArray<TObj, _A>::at(int i) const
{
	return m_constvector.at();
}

template<class TObj, class _A>
inline CVisConstArray<TObj, _A>::const_reference
	CVisConstArray<TObj, _A>::operator[](int i) const
{
	return m_constvector[i];
}

template<class TObj, class _A>
inline CVisConstArray<TObj, _A>::const_reference
	CVisConstArray<TObj, _A>::front(void) const
{
	return m_constvector.front();
}

template<class TObj, class _A>
inline CVisConstArray<TObj, _A>::const_reference
	CVisConstArray<TObj, _A>::back(void) const
{
	return m_constvector.back();
}


#if defined(_DEBUG) && defined(_AFX)
//#if defined(_DEBUG)
#undef new
#endif // defined(_DEBUG)

