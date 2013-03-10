// VisObjList.h
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



// Constant
// Note:  If CVisList had copy ctor and assignment op, we can't be sure
// that m_pnodeFirstNotFull will always be valid.  For this reason, we don't
// allow a copy ctor or assignment operator.  A CVisList object is only
// used when building lists.  When an object stores information from a
// list, it should store a CVisListEnum object, not a CVisList object.
// UNDONE:  Need to find the performance effects of changes to this value.
enum { kcObjAllocUnitDefault = 20 };


// Forward declarations
template<class TObj, int kcObjAllocUnit> class CVisObjListNode;
template<class TObj, int kcObjAllocUnit> class CVisObjListEnum;
template<class TObj, int kcObjAllocUnit> class CVisObjListBuilder;


template<class TObj, int kcObjAllocUnit>
class CVisObjListNode
{
public:
	CVisObjListNode();

	unsigned long AddRef(void);
	unsigned long Release(void);

	int CObj(void) const;
	BOOL FIsFull(void) const;

	TObj& ObjNext(void);

	TObj *RgObj(void);

	// This returns a pointer to a node that has space for additional
	// objects.
	CVisObjListNode<TObj, kcObjAllocUnit> *AddObj(void);
	CVisObjListNode<TObj, kcObjAllocUnit> *AddObj(const TObj& obj);

	CVisObjListNode<TObj, kcObjAllocUnit> *PNodePrev(void);
	CVisObjListNode<TObj, kcObjAllocUnit> *PNodeNext(void);

	void SetPNodePrev(CVisObjListNode<TObj, kcObjAllocUnit> *pnode);
	void SetPNodeNext(CVisObjListNode<TObj, kcObjAllocUnit> *pnode);

private:
	int m_cRef;
	int m_cObjCur;
	CVisObjListNode<TObj, kcObjAllocUnit> *m_pnodePrev;
	CVisObjListNode<TObj, kcObjAllocUnit> *m_pnodeNext;

	// Note:  The four data members above should ensure that this is
	// aligned on an 8-byte boundry.  Be very careful adding members
	// to this class.
	TObj m_rgObj[kcObjAllocUnit];

	~CVisObjListNode(void);
};



template<class TObj, int kcObjAllocUnit = kcObjAllocUnitDefault>
class CVisObjListEnum
{
public:
	CVisObjListEnum(void);
	CVisObjListEnum(
		const CVisObjListEnum<TObj, kcObjAllocUnit>& reflistobjenum);
	CVisObjListEnum& operator=(
		const CVisObjListEnum<TObj, kcObjAllocUnit>& reflistobjenum);
	~CVisObjListEnum(void);

	BOOL FMore(void) const;
	TObj& ObjCur(void);
	void Next(void);
	void Reset(void);

	// UNDONE:  Replace the above methods with these iterator methods.
	bool operator==(
			const CVisObjListEnum<TObj, kcObjAllocUnit>& reflistobjenum) const;
	bool operator!=(
			const CVisObjListEnum<TObj, kcObjAllocUnit>& reflistobjenum) const;
	TObj& operator*(void) const;
	CVisObjListEnum<TObj, kcObjAllocUnit>& operator++(void);
	CVisObjListEnum<TObj, kcObjAllocUnit> operator++(int);

	// These are helper methods that are not in all iterator classes.
	CVisObjListEnum<TObj, kcObjAllocUnit> begin(void);
	CVisObjListEnum<TObj, kcObjAllocUnit> end(void);

protected:
	CVisObjListEnum(CVisObjListNode<TObj, kcObjAllocUnit> *pnodeFirst,
			CVisObjListNode<TObj, kcObjAllocUnit> *pnodeCur,
			int iObjCur);

	CVisObjListNode<TObj, kcObjAllocUnit> *PNodeFirst(void) const;

	// This will set the pointer to the first node in the list.  It will
	// not adjust the pointers in any nodes in the list.  Callers should
	// be careful to keep track of list nodes to avoid memory leaks when
	// calling this method.
	void SetPNodeFirst(CVisObjListNode<TObj, kcObjAllocUnit> *pnode);

	// This is used when merging a list into another list.  The data from
	// this list is given to the other list and this function is called
	// by a derived class to clear the data in this object.  After this
	// method is called, this object should be deleted.
	void ClearData(void);

private:
	CVisObjListNode<TObj, kcObjAllocUnit> *m_pnodeFirst;
	CVisObjListNode<TObj, kcObjAllocUnit> *m_pnodeCur;
	int m_iObjCur;
};


template<class TObj, int kcObjAllocUnit = kcObjAllocUnitDefault>
class CVisObjListBuilder : public CVisObjListEnum<TObj, kcObjAllocUnit>
{
public:
	CVisObjListBuilder(void);
	CVisObjListBuilder(
		const CVisObjListEnum<TObj, kcObjAllocUnit>& reflistobjenum);
	CVisObjListBuilder& operator=(
		const CVisObjListEnum<TObj, kcObjAllocUnit>& reflistobjenum);

	// Only the original CVisObjListBuilder object can be used to add
	// objects to the list.
	BOOL FCanAdd(void) const;

	TObj& ObjNext(void);

	void AddObj(void);
	void AddObj(const TObj& obj);

	// Merge this list into another list.
	void MergeInto(CVisObjListBuilder<TObj, kcObjAllocUnit>& reflistbuilder);

protected:
	CVisObjListNode<TObj, kcObjAllocUnit> *PNodeFirstNotFull(void) const;

	// Add the nodes from another list to this list.
	void AddNodes(const CVisObjListBuilder<TObj, kcObjAllocUnit>&
			reflistbuilder);

private:
	// Note:  We could avoid the use of this member by using
	// m_pnodeCur in its place when constructing the list.  But
	// we can protect against undesired list modifications by
	// adding the m_pnodeFirstNotFull data member.
	CVisObjListNode<TObj, kcObjAllocUnit> *m_pnodeFirstNotFull;
};


#include "VisObjList.inl"
