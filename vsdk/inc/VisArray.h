// VisArray.h
//
// Reference counted array class based on std::vector.
//
// UNDONE:  Add SDStream I/O support to this class.
//
// Copyright ?2000 Microsoft Corporation, All Rights Reserved

#pragma once


#include "VisStlWrappers.h"


template<class _Ty, class _A = std::allocator<_Ty> >
//class CVisAI : public std::vector<_Ty, _A>
class CVisAI : public CVisStlVector<_Ty, _A>
{
public:
	//------------------------------------------------------------------
	// Base class type definitions

	typedef std::vector<_Ty, _A> T_Vector;
	typedef CVisStlVector<_Ty, _A> T_StlVector;
	typedef T_StlVector T_Base;


	//------------------------------------------------------------------
	// Constructors (which call the std::vector constructors)

	CVisAI(const _A& _Al = _A());
	CVisAI(size_type _N, const _Ty& _V = _Ty(),
			const _A& _Al = _A());
	CVisAI(const T_Vector& _X);
	CVisAI(const T_StlVector& _X);
	CVisAI(const_iterator _F, const_iterator _L,
			const _A& _Al = _A());


	//------------------------------------------------------------------
	// Reference counting

	unsigned long AddRef(void);
	unsigned long Release(void);


	//------------------------------------------------------------------
	// Copy method

	void Copy(const T_Base& _X);


protected:
	//------------------------------------------------------------------
	// Protected destructor ensures that this class is not created on
	// the stack.
	~CVisAI(void);

private:
	unsigned long m_cRef;
};


// This class contains a reference to internal array memory and
// const methods that call the array methods.
template<class TObj, class _A = std::allocator<TObj> >
class CVisArrayBase
{
public:
	//------------------------------------------------------------------
	// Typedefs

	typedef CVisArrayBase<TObj, _A> T_ArrayBase;
	typedef std::vector<TObj, _A> T_Vector;
	typedef const std::vector<TObj, _A> T_ConstVector;
	typedef CVisStlVector<TObj, _A> T_StlVector;

	typedef _A allocator_type;
	typedef _A::size_type size_type;
	typedef _A::difference_type difference_type;
	typedef _A::pointer _Tptr;
	typedef _A::const_pointer _Ctptr;
	typedef _A::reference reference;
	typedef _A::const_reference const_reference;
	typedef _A::value_type value_type;
	typedef _Tptr iterator;
	typedef _Ctptr const_iterator;
	// We don't include typedefs for reverse iterators


	//------------------------------------------------------------------
	// Constructors, assignment operator, and destructor

	// Constructors that take references to objects to be copied.
	// (based on the std::vector constructors)
	CVisArrayBase(void);
	CVisArrayBase(const CVisArrayBase<TObj, _A>& _X);
	
	// Assignment operator
	CVisArrayBase<TObj, _A>& operator=(
			const CVisArrayBase<TObj, _A>& _X);

	// Destructor
	~CVisArrayBase(void);


	//------------------------------------------------------------------
	// const std::vector methods

	const_iterator begin(void) const;
	const_iterator end(void) const;

	int size(void) const;
	bool empty(void) const;

	const_reference at(int i) const;
	const_reference operator[](int i) const;

	const_reference front(void) const;
	const_reference back(void) const;


protected:
	CVisAI<TObj, _A> *m_parrayinternal;

	// Protected constructors (called by derived classes)
	CVisArrayBase(const T_Vector& _X);
	CVisArrayBase(const T_StlVector& _X);
	CVisArrayBase(const _A& _Al);
	CVisArrayBase(size_type n, const TObj& _V = TObj(), const _A& _Al = _A());
	CVisArrayBase(const_iterator _F, const_iterator _L, const _A& _Al = _A());

	void ClearArrayInternal(void);

	void EnsureArrayInternal(void);
};


// UNDONE:  Remove the ConstArray class or make it a base class of Array.
template<class TObj, class _A = std::allocator<TObj> >
class CVisArray : public CVisArrayBase<TObj, _A>
{
public:
	//------------------------------------------------------------------
	// Typedefs

	typedef CVisArray<TObj, _A> T_Array;

	//------------------------------------------------------------------
	// Constructors, assignment operator, and destructor

	// Constructors that take references to objects to be copied.
	// (based on the std::vector constructors)
	CVisArray(void);
	CVisArray(const T_Vector& _X);
	CVisArray(const T_StlVector& _X);
	CVisArray(const _A& _Al);
	CVisArray(size_type n, const TObj& _V = TObj(), const _A& _Al = _A());
	CVisArray(const_iterator _F, const_iterator _L, const _A& _Al = _A());

	// Copy constructor and assignment operator
	// (These will copy a reference to the memory containing the vector
	// of objects.  To copy the objects, use the Assign method.)
	CVisArray(const T_Array& _X);
	T_Array& operator=(const T_Array& _X);

	
	//------------------------------------------------------------------
	// Comparison methods (that call the std::vector methods)

	bool operator==(const T_Vector& _X) const;
	bool operator==(const T_StlVector& _X) const;
	bool operator==(const T_ArrayBase& _X) const;

	bool operator<(const T_Vector& _X) const;
	bool operator<(const T_StlVector& _X) const;
	bool operator<(const T_ArrayBase& _X) const;

	
	//------------------------------------------------------------------
	// Overloaded function call operator

	// This method will resize the array if needed to make sure that
	// the reference specified is valid.
	reference operator()(size_type i);


	//------------------------------------------------------------------
	// Copy method

	// Use this method (instead of the copy constructor or the assign
	// methods) to copy object values from one array to another.
	void Copy(const T_ArrayBase& _X);


	//------------------------------------------------------------------
	// std::vector methods

	size_type capacity(void) const;
	void reserve(size_type n);

	iterator begin(void);
	const_iterator begin(void) const;
	iterator end(void);
	const_iterator end(void) const;

	void resize(size_type _N, const TObj& _X = TObj());
	size_type size(void) const;
	size_type max_size(void) const;
	bool empty(void) const;

	const_reference at(size_type _P) const;
	reference at(size_type _P);
	const_reference operator[](size_type _P) const;
	reference operator[](size_type _P);

	reference front(void);
	const_reference front(void) const;
	reference back(void);
	const_reference back(void) const;

	void push_back(const TObj& _X = TObj());
	void pop_back(void);

	iterator insert(iterator _P, const TObj& _X = TObj());
	void insert(iterator _P, size_type _M, const TObj& _X);
	void insert(iterator _P, const_iterator _F, const_iterator _L);

	void assign(const_iterator _F, const_iterator _L);
	void assign(size_type _N, const TObj& _X = TObj());

	// Use this method (instead of the copy constructor) to copy object
	// values from one array to another.
	void Assign(const T_Array& _X);

	void clear(void);
	iterator erase(iterator _P);
	iterator erase(iterator _F, iterator _L);

	void swap(T_Vector& _X);
	void swap(T_StlVector& _X);
	void swap(T_Array& _X);


	//------------------------------------------------------------------
	// CVisArray methods

	// Release the reference to the current data and get a reference to
	// a new (empty) vector.
	void NewReference(void);

	// Return a CVisConstArray which references a subset of this array.
	CVisConstArray<TObj, _A> SubArray(int iFirst = 0,
			int iLim = - 1) const;


	//------------------------------------------------------------------
	// Cast operators

	operator T_ConstVector&(void) const;
	operator T_Vector&(void);

protected:
	friend class CVisConstArray<TObj, _A>;
};


template<class TObj, class _A = std::allocator<TObj> >
class CVisTable : public CVisArray<CVisArray<TObj, _A> >
{
public:
	//------------------------------------------------------------------
	// Typedefs

	// (The compiler doesn't seem to get these from CVisArrayBase.)

	typedef CVisArray<TObj, _A> T_ArrayObj;
	typedef CVisArrayBase<T_ArrayObj> T_ArrayBase;
	typedef std::vector<T_ArrayObj> T_Vector;
	typedef const std::vector<T_ArrayObj> T_ConstVector;
	typedef CVisStlVector<T_ArrayObj> T_StlVector;

	
	//------------------------------------------------------------------
	// Constructors, assignment operator, and destructor

	// Constructors that take references to objects to be copied.
	// (based on the std::vector constructors)
	CVisTable(void);
	CVisTable(const T_Vector& _X);
	CVisTable(const T_StlVector& _X);
	CVisTable(int n);
	CVisTable(T_ArrayBase::const_iterator _F, T_ArrayBase::const_iterator _L);

	// Copy constructor and assignment operator
	// (These will copy a reference to the memory containing the vector
	// of objects.  To copy the objects, use the Assign method.)
	CVisTable(const CVisTable<TObj, _A>& _X);
	CVisTable<TObj, _A>& operator=(const CVisTable<TObj, _A>& _X);

	
	//------------------------------------------------------------------
	// Overloaded function call operator

	// This method is const, so it will not resize the array.
	const TObj& operator()(int i, int j) const;

	// These methods will resize the array if needed to make sure that
	// the reference specified is valid.
	T_ArrayBase::reference operator()(int i);
	TObj& operator()(int i, int j);


	//------------------------------------------------------------------
	// Copy method

	// Use this method (instead of the copy constructor or the assign
	// methods) to copy object values from one array to another.
	void Copy(const T_ArrayBase& _X);
};


// This class contains const_iterators that can be used to examine,
// but not modify, the elements in an array.  The elements are not
// copied, only pointers to the elements are copied.  This keeps a
// reference to the internal array memory, but it could be invalidated
// if elements are added to or removed from the array.
template<class TObj, class _A = std::allocator<TObj> >
class CVisConstArray
{
public:
	typedef const TObj * const_iterator;
	typedef const TObj & const_reference;


	//------------------------------------------------------------------
	// Constructors, assignment operator, and destructor

	// Constructors that take references to objects to be copied.
	// (based on the std::vector constructors)
	CVisConstArray(void);
	CVisConstArray(const CVisArray<TObj, _A>& _X,
			int iFirst = 0, int iLim = - 1);
	CVisConstArray(const CVisConstArray<TObj, _A>& _X,
			int iFirst = 0, int iLim = - 1);
	
	// Overloaded assignment operator
	CVisConstArray<TObj, _A>& operator=(
			const CVisArray<TObj, _A>& _X);
	CVisConstArray<TObj, _A>& operator=(
			const CVisConstArray<TObj, _A>& _X);

	// Destructor
	~CVisConstArray(void);


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
	CVisConstVector<TObj, _A> m_constvector;
	CVisAI<TObj, _A> *m_parrayinternal;
};


#include "VisStlWrappers.inl"
#include "VisArray.inl"
