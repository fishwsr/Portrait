// VisStlWrappers.h
//
// Wrapper classes for common STL classes.  For some STL types, these
// wrappers will use virtual methods to avoid problems with passing
// STL types between different program modules.
//
// In the Vision SDK, we use the following STL types:
// vector, deque, list, set, map, string, istream, ostream, ifstream, ofstream
// (We may also want to use std::stack).
//
// Copyright ?2000 Microsoft Corporation, All Rights Reserved

#pragma once


// Warning that some STL type names are too long.  (Debug info is truncated.)
#pragma warning( disable : 4786 )


#include <vector>
#include <map>

// LATER:  Add warppers for other STL classes, if needed.
//#include <deque>
//#include <set>
//#include <list>
//#include <string>


#define VisStlMapVirtual virtual


// Forward declarations
template<class TObj, class _A = std::allocator<TObj> >
class CVisConstVector;

template<class TObj, class _A = std::allocator<TObj> >
class CVisArrayInternal;

template<class TObj, class _A = std::allocator<TObj> >
class CVisArrayBase;

template<class TObj, class _A = std::allocator<TObj> >
class CVisArray;

template<class TObj, class _A = std::allocator<TObj> >
class CVisConstArray;


template<class TObj, class _A = std::allocator<TObj> >
class CVisStlVector : public std::vector<TObj, _A>
{
public:
	//------------------------------------------------------------------
	// Constructors, assignment operator, and destructor

	// Constructors that take references to objects to be copied.
	// (based on the std::vector constructors)
	CVisStlVector(const _A& _Al = _A());
	CVisStlVector(size_type n, const TObj& _V = TObj(), const _A& _Al = _A());
	CVisStlVector(const_iterator _F, const_iterator _L, const _A& _Al = _A());
	CVisStlVector(const std::vector<TObj, _A>& _X);
	CVisStlVector(const CVisConstVector<TObj, _A>& _X);
	CVisStlVector(const CVisArray<TObj, _A>& _X);
	CVisStlVector(const CVisConstArray<TObj, _A>& _X);
	
	// Overloaded assignment operators
	CVisStlVector<TObj, _A>& operator=(
			const std::vector<TObj, _A>& _X);
	CVisStlVector<TObj, _A>& operator=(
			const CVisConstVector<TObj, _A>& _X);
	CVisStlVector<TObj, _A>& operator=(
			const CVisArray<TObj, _A>& _X);
	CVisStlVector<TObj, _A>& operator=(
			const CVisConstArray<TObj, _A>& _X);

	
	//------------------------------------------------------------------
	// Comparison methods (that call the std::vector methods)

	bool operator==(const std::vector<TObj, _A>& _X) const;
	bool operator<(const std::vector<TObj, _A>& _X) const;

	bool _Eq(const std::vector<TObj, _A>& _X) const;
	bool _Lt(const std::vector<TObj, _A>& _X) const;

	
	//------------------------------------------------------------------
	// Overloaded function call operator

	// This method will resize the vector if needed to make sure that
	// the reference specified is valid.
	reference operator()(size_type _P);
};


// This class contains const_iterators that can be used to examine,
// but not modify, the elements in a vector or an array.  The elements
// are not copied, only pointers to the elements are copied.  The
// references used by this class could be invalidated if elements are
// added to or removed from the vector or array.
template<class TObj, class _A = std::allocator<TObj> >
class CVisConstVector
{
public:
	typedef const TObj * const_iterator;
	typedef const TObj & const_reference;


	//------------------------------------------------------------------
	// Constructors, assignment operator, and destructor

	// Constructors that take references to objects to be copied.
	// (based on the std::vector constructors)
	CVisConstVector(void);
	CVisConstVector(const_iterator _F, const_iterator _L);
	CVisConstVector(const std::vector<TObj, _A>& _X);
	CVisConstVector(const CVisArray<TObj, _A>& _X);
	CVisConstVector(const CVisConstArray<TObj, _A>& _X);
	
	// Overloaded assignment operators
	CVisConstVector<TObj, _A>& operator=(
			const std::vector<TObj, _A>& _X);
	CVisConstVector<TObj, _A>& operator=(
			const CVisArray<TObj, _A>& _X);
	CVisConstVector<TObj, _A>& operator=(
			const CVisConstArray<TObj, _A>& _X);


	//------------------------------------------------------------------
	// std::vector methods

	const_iterator begin(void) const;
	const_iterator end(void) const;

	int size(void) const;
	bool empty(void) const;

	const_reference at(int i) const;
	const_reference operator[](int i) const;

	const_reference front(void) const;
	const_reference back(void) const;


protected:
	const_iterator m_pFirst;
	const_iterator m_pLim;
};


// With the map class, we wrap std::map and use virtual methods
// to keep all calls to std::map in the same program module.
template<class _K, class _Ty, class _Pr = std::less<_K>,
		class _A = std::allocator<_Ty> >
class CVisStlMap {
public:
	typedef CVisStlMap<_K, _Ty, _Pr, _A> T_This;
	typedef std::map<_K, _Ty, _Pr, _A> T_Map;
	typedef std::pair<const _K, _Ty> value_type;

	typedef T_Map::_Kfn _Kfn;
	typedef T_Map::value_compare value_compare;

	typedef _K key_type;
	typedef _Ty referent_type;
	typedef _Pr key_compare;
	typedef _A allocator_type;
	typedef _A::reference _Tref;
	typedef T_Map::_Imp _Imp;
	typedef _Imp::size_type size_type;
	typedef _Imp::difference_type difference_type;
	typedef _Imp::reference reference;
	typedef _Imp::const_reference const_reference;
	typedef _Imp::iterator iterator;
	typedef _Imp::const_iterator const_iterator;
	typedef _Imp::reverse_iterator reverse_iterator;
	typedef _Imp::const_reverse_iterator const_reverse_iterator;
	typedef std::pair<iterator, bool> _Pairib;
	typedef std::pair<iterator, iterator> _Pairii;
	typedef std::pair<const_iterator, const_iterator> _Paircc;
	typedef const value_type *_It;

	CVisStlMap(const T_Map& _X)
	  : m_map(_X) {}
	CVisStlMap(const _Pr& _Pred = _Pr(), const _A& _Al = _A())
	  : m_map(_Pred, _Al) {}
	CVisStlMap(_It _F, _It _L, const _Pr& _Pred = _Pr(),
			const _A& _Al = _A())
			: m_map(_It, _L, _Pred, _Al) {}

	VisStlMapVirtual T_This& operator=(const T_Map& _X)
		{m_map = _X; return (*this); }

	VisStlMapVirtual void swap(T_Map& _X)
		{m_map.swap(_X); }
	VisStlMapVirtual void swap(T_This& _X)
		{m_map.swap(_X.m_map); }
	friend void swap(T_Map& _X, T_This& _Y)
		{_Y.swap(_X); }
	friend void swap(T_This& _X, T_Map& _Y)
		{_X.swap(_Y); }
	friend void swap(T_This& _X, T_This& _Y)
		{_X.swap(_Y); }

	VisStlMapVirtual iterator begin()
		{return (m_map.begin()); }
	VisStlMapVirtual const_iterator begin() const
		{return (m_map.begin()); }
	VisStlMapVirtual iterator end()
		{return (m_map.end()); }
	VisStlMapVirtual const_iterator end() const
		{return (m_map.end()); }
	VisStlMapVirtual reverse_iterator rbegin()
		{return (m_map.rbegin()); }
	VisStlMapVirtual const_reverse_iterator rbegin() const
		{return (m_map.rbegin()); }
	VisStlMapVirtual reverse_iterator rend()
		{return (m_map.rend()); }
	VisStlMapVirtual const_reverse_iterator rend() const
		{return (m_map.rend()); }
	VisStlMapVirtual size_type size() const
		{return (m_map.size()); }
	VisStlMapVirtual size_type max_size() const
		{return (m_map.max_size()); }
	VisStlMapVirtual bool empty() const
		{return (m_map.empty()); }
	VisStlMapVirtual _A get_allocator() const
		{return (m_map.get_allocator()); }
	VisStlMapVirtual _Tref operator[](const key_type& _Kv)
		{iterator _P = insert(value_type(_Kv, _Ty())).first;
		return ((*_P).second); }
	VisStlMapVirtual _Pairib insert(const value_type& _X)
		{_Imp::_Pairib _Ans = m_map.insert(_X);
		return (_Pairib(_Ans.first, _Ans.second)); }
	VisStlMapVirtual iterator insert(iterator _P, const value_type& _X)
		{return (m_map.insert((_Imp::iterator&)_P, _X)); }
	VisStlMapVirtual void insert(_It _F, _It _L)
		{for (; _F != _L; ++_F)
			m_map.insert(*_F); }
	VisStlMapVirtual iterator erase(iterator _P)
		{return (m_map.erase((_Imp::iterator&)_P)); }
	VisStlMapVirtual iterator erase(iterator _F, iterator _L)
		{return (m_map.erase((_Imp::iterator&)_F,
			(_Imp::iterator&)_L)); }
	VisStlMapVirtual size_type erase(const _K& _Kv)
		{return (m_map.erase(_Kv)); }
	VisStlMapVirtual void clear()
		{m_map.clear(); }
	VisStlMapVirtual key_compare key_comp() const
		{return (m_map.key_comp()); }
	VisStlMapVirtual value_compare value_comp() const
		{return (value_compare(m_map.key_comp())); }
	VisStlMapVirtual iterator find(const _K& _Kv)
		{return (m_map.find(_Kv)); }
	VisStlMapVirtual const_iterator find(const _K& _Kv) const
		{return (m_map.find(_Kv)); }
	VisStlMapVirtual size_type count(const _K& _Kv) const
		{return (m_map.count(_Kv)); }
	VisStlMapVirtual iterator lower_bound(const _K& _Kv)
		{return (m_map.lower_bound(_Kv)); }
	VisStlMapVirtual const_iterator lower_bound(const _K& _Kv) const
		{return (m_map.lower_bound(_Kv)); }
	VisStlMapVirtual iterator upper_bound(const _K& _Kv)
		{return (m_map.upper_bound(_Kv)); }
	VisStlMapVirtual const_iterator upper_bound(const _K& _Kv) const
		{return (m_map.upper_bound(_Kv)); }
	VisStlMapVirtual _Pairii equal_range(const _K& _Kv)
		{return (m_map.equal_range(_Kv)); }
	VisStlMapVirtual _Paircc equal_range(const _K& _Kv) const
		{return (m_map.equal_range(_Kv)); }

protected:
	T_Map m_map;
};



#include "VisArray.h"  // Which will include "VisStlWrappers.inl"
