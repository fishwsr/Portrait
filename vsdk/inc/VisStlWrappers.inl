// VisStlWrappers.inl
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved


#include <assert.h>


#if defined(_DEBUG) && defined(_AFX)
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // defined(_DEBUG) && defined(_AFX)




template<class TObj, class _A>
inline CVisStlVector<TObj, _A>::CVisStlVector(const _A& _Al)
  : std::vector<TObj, _A>(_Al)
{
}

template<class TObj, class _A>
inline CVisStlVector<TObj, _A>::CVisStlVector(size_type n, const TObj& _V,
		const _A& _Al)
  : std::vector<TObj, _A>(n, _V, _Al)
{
}

template<class TObj, class _A>
inline CVisStlVector<TObj, _A>::CVisStlVector(const_iterator _F, const_iterator _L,
		const _A& _Al)
  : std::vector<TObj, _A>(_F, _L, _Al)
{
}

template<class TObj, class _A>
inline CVisStlVector<TObj, _A>::CVisStlVector(const std::vector<TObj, _A>& _X)
  : std::vector<TObj, _A>(_X)
{
}

template<class TObj, class _A>
inline CVisStlVector<TObj, _A>::CVisStlVector(const CVisConstVector<TObj, _A>& _X)
  : std::vector<TObj, _A>()
{
	if (!_X.empty())
		assign(_X.begin(), _X.end());
}

template<class TObj, class _A>
inline CVisStlVector<TObj, _A>::CVisStlVector(const CVisArray<TObj, _A>& _X)
  : std::vector<TObj, _A>()
{
	if (!_X.empty())
		assign(_X.begin(), _X.end());
}

template<class TObj, class _A>
inline CVisStlVector<TObj, _A>::CVisStlVector(const CVisConstArray<TObj, _A>& _X)
  : std::vector<TObj, _A>()
{
	if (!_X.empty())
		assign(_X.begin(), _X.end());
}

template<class TObj, class _A>
inline CVisStlVector<TObj, _A>& CVisStlVector<TObj, _A>::operator=(
		const std::vector<TObj, _A>& _X)
{
	std::vector<TObj, _A>::operator=(_X);

	return *this;
}

template<class TObj, class _A>
inline CVisStlVector<TObj, _A>& CVisStlVector<TObj, _A>::operator=(
		const CVisConstVector<TObj, _A>& _X)
{
	if (_X.empty())
		clear();
	else
		assign(_X.begin(), _X.end());

	return *this;
}

template<class TObj, class _A>
inline CVisStlVector<TObj, _A>& CVisStlVector<TObj, _A>::operator=(
		const CVisArray<TObj, _A>& _X)
{
	if (_X.empty())
		clear();
	else
		assign(_X.begin(), _X.end());

	return *this;
}

template<class TObj, class _A>
inline CVisStlVector<TObj, _A>& CVisStlVector<TObj, _A>::operator=(
		const CVisConstArray<TObj, _A>& _X)
{
	if (_X.empty())
		clear();
	else
		assign(_X.begin(), _X.end());

	return *this;
}


template<class TObj, class _A>
inline bool CVisStlVector<TObj, _A>::operator==(const std::vector<TObj, _A>& _X) const
{
	return _Eq(_X);
}

template<class TObj, class _A>
inline bool CVisStlVector<TObj, _A>::operator<(const std::vector<TObj, _A>& _X) const
{
	return _Lt(_X);
}

template<class TObj, class _A>
inline bool CVisStlVector<TObj, _A>::_Eq(const std::vector<TObj, _A>& _X) const
{
	return std::vector<TObj, _A>::_Eq(_X);
}

template<class TObj, class _A>
inline bool CVisStlVector<TObj, _A>::_Lt(const std::vector<TObj, _A>& _X) const
{
	return std::vector<TObj, _A>::_Lt(_X);
}


template<class TObj, class _A>
inline CVisStlVector<TObj, _A>::reference CVisStlVector<TObj, _A>::operator()(size_type _P)
{
	if (size() <= _P)
		resize(_P + 1);
	return (*this)[_P];
}



template<class TObj, class _A>
inline CVisConstVector<TObj, _A>::CVisConstVector(void)
  : m_pFirst(0),
	m_pLim(0)
{
}

template<class TObj, class _A>
inline CVisConstVector<TObj, _A>::CVisConstVector(
		const_iterator _F, const_iterator _L)
  : m_pFirst(_F),
	m_pLim(_L)
{
}

template<class TObj, class _A>
inline CVisConstVector<TObj, _A>::CVisConstVector(
		const std::vector<TObj, _A>& _X)
  : m_pFirst(_X.begin()),
	m_pLim(_X.end())
{
}

template<class TObj, class _A>
inline CVisConstVector<TObj, _A>::CVisConstVector(
		const CVisArray<TObj, _A>& _X)
  : m_pFirst(_X.begin()),
	m_pLim(_X.end())
{
}

template<class TObj, class _A>
inline CVisConstVector<TObj, _A>::CVisConstVector(
		const CVisConstArray<TObj, _A>& _X)
  : m_pFirst(_X.begin()),
	m_pLim(_X.end())
{
}

template<class TObj, class _A>
inline CVisConstVector<TObj, _A>& CVisConstVector<TObj, _A>::operator=(
		const std::vector<TObj, _A>& _X)
{
	m_pFirst = _X.begin();
	m_pLim = _X.end();

	return *this;
}

template<class TObj, class _A>
inline CVisConstVector<TObj, _A>& CVisConstVector<TObj, _A>::operator=(
		const CVisArray<TObj, _A>& _X)
{
	m_pFirst = _X.begin();
	m_pLim = _X.end();

	return *this;
}

template<class TObj, class _A>
inline CVisConstVector<TObj, _A>& CVisConstVector<TObj, _A>::operator=(
		const CVisConstArray<TObj, _A>& _X)
{
	m_pFirst = _X.begin();
	m_pLim = _X.end();

	return *this;
}

template<class TObj, class _A>
inline CVisConstVector<TObj, _A>::const_iterator
	CVisConstVector<TObj, _A>::begin(void) const
{
	return m_pFirst;
}

template<class TObj, class _A>
inline CVisConstVector<TObj, _A>::const_iterator
	CVisConstVector<TObj, _A>::end(void) const
{
	return m_pLim;
}

template<class TObj, class _A>
inline int CVisConstVector<TObj, _A>::size(void) const
{
	return m_pLim - m_pFirst;
}

template<class TObj, class _A>
inline bool CVisConstVector<TObj, _A>::empty(void) const
{
	return (m_pLim == m_pFirst);
}

template<class TObj, class _A>
inline CVisConstVector<TObj, _A>::const_reference
	CVisConstVector<TObj, _A>::at(int i) const
{
	return m_pFirst[i];
}

template<class TObj, class _A>
inline CVisConstVector<TObj, _A>::const_reference
	CVisConstVector<TObj, _A>::operator[](int i) const
{
	return m_pFirst[i];
}

template<class TObj, class _A>
inline CVisConstVector<TObj, _A>::const_reference
	CVisConstVector<TObj, _A>::front(void) const
{
	return *m_pFirst;
}

template<class TObj, class _A>
inline CVisConstVector<TObj, _A>::const_reference
	CVisConstVector<TObj, _A>::back(void) const
{
	return *(m_pLim - 1);
}



#if defined(_DEBUG) && defined(_AFX)
#undef new
#endif // defined(_DEBUG) && defined(_AFX)

