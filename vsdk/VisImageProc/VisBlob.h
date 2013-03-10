// VisBlob.h
//
// Copyright © 1999 - 2000 Microsoft Corporation, All Rights Reserved

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



// Values used to specify strings describing properties of this blob.
// UNDONE:  The CVisBlobInternal class should not have its own enum like this.
enum EVisBlobProp
{
	evisblobpropTitle				= 0x0001,
	evisblobpropParent				= 0x0002,
	evisblobpropChildren			= 0x0004,
	evisblobpropDepth				= 0x0008,
	evisblobpropWidth				= 0x0010,
	evisblobpropHeight				= 0x0020,
	evisblobpropArea				= 0x0040,
	evisblobpropRect				= 0x0080,
	evisblobpropCentroid			= 0x0100,

	// LATER:  Probably want to use moment strings instead of these.
	evisblobpropSumX				= 0x0200,
	evisblobpropSumY				= 0x0400,
	evisblobpropSumXX				= 0x0800,
	evisblobpropSumXY				= 0x1000,
	evisblobpropSumYY				= 0x2000,

	evisblobpropLast				= 0x2000,
	evisblobpropAll					= 0x3fff
};


#include "VisBlobInternal.h"


// Forward declaration
class CVisBlobTree;


class CVisBlob
{
public:
	typedef CVisStripListEnum iterator;


	CVisBlob(void);
	CVisBlob(CVisBlobTreeInternal *pblobtreeinternal,
			CVisBlobInternal *pblobinternal);
	CVisBlob(CVisBlob& refblob);
	CVisBlob& operator=(CVisBlob& refblob);
	~CVisBlob(void);


	bool IsValid(void) const;

	bool IsForeground(void) const;


	// LATER:  Add const methods that return const_iterators?
	iterator begin(void);
	iterator end(void);


	bool HasParent(void) const;
	bool HasNextSibling(void) const;
	bool HasPrevSibling(void) const;
	bool HasChildren(void) const;

	CVisBlob Parent(void);
	CVisBlob NextSibling(void);
	CVisBlob FirstChild(void);

	// This is implementation dependent and may not be very efficient.
	CVisBlob PrevSibling(void);
	// This is implementation dependent and may not be very efficient.
	CVisBlob LastChild(void);


	// Blob tree methods.
	CVisBlobTree BlobTree(void);

	// This method will remove a blob from the tree.  It will invalidate
	// copies of this blob and iterators pointing to this blob.  If the
	// blob has children, they will become children of the merged blob or
	// be deleted if the blob is deleted.
	void RemoveFromTree(bool fMergeIntoParent = true);

	// Find the depth of the blob in the tree.  Depth information is not
	// stored in the blob.  Instead it is computed by counting the number
	// of blobs between this blob and the root of the tree.
	unsigned int DepthInTree(void) const;


	// Get the label for this blob.  This is set in the standard
	// blob-labeling functions.  It may not be initialized if the
	// blobs have not been labeled using one of the standard
	// blob-labeling functions.
	unsigned int ILabel(void) const;

	void SetILabel(unsigned int iLabel);


	int Width(void) const;
	int Height(void) const;
	const CVisRect& RectBounding(void) const;

	// LATER:  Add a compile-time option to use 64-bit uint for area?
	unsigned long Area(void) const;

	double DblXCentroid(void) const;
	double DblYCentroid(void) const;
	LONGLONG SumX(void) const;
	LONGLONG SumY(void) const;
	LONGLONG SumXX(void) const;
	LONGLONG SumXY(void) const;
	LONGLONG SumYY(void) const;


	// Get a string describing one or more properties of this blob.
	// UNDONE:  Should this be a helper function instead of a method?
	// UNDONE:  Use std::string instead of CString.
	void GetPropString(int evisblobprop, CVisString& refszProp,
			bool fAppend = false) const;


	// LATER:  It could be nice to have a method to check whether a
	// pixel is contained in this blob (or its children).

	// UNDONE:  We probably should also provide methods to modify the strips
	// in a blob, to merge a blob into its parents, and to merge the
	// children of a blob into the blob.

	// UNDONE:  Add a method to get the CVisBlobTree containing this Blob.

private:
	friend class CVisBlobTree;

	// Pointer to the internal blob class.
	CVisBlobInternal *m_pblobinternal;

	// Blob tree containing this blob.
	// UNDONE:  Should we use a CVisBlobTree?
	CVisBlobTreeInternal *m_pblobtreeinternal;
};


class CVisBlobTreeIterator
{
public:
	CVisBlobTreeIterator(CVisBlobTreeInternal *pblobtreeinternal = 0,
			bool fChildrenBeforeParents = false, bool fAtEnd = false);
	CVisBlobTreeIterator(CVisBlobTreeIterator& refblobtreeiterator);
	CVisBlobTreeIterator& operator=(CVisBlobTreeIterator& refblobtreeiterator);
	~CVisBlobTreeIterator(void);

	bool IsValid(void) const;

	CVisBlob operator*(void) const;

	// Prefix increment and decrement.
	CVisBlobTreeIterator& operator++(void);
	CVisBlobTreeIterator& operator--(void);

	// Postfix increment and decrement.
	CVisBlobTreeIterator operator++(int);
	CVisBlobTreeIterator operator--(int);

	bool operator==(const CVisBlobTreeIterator& refblobtreeiterator) const;
	bool operator!=(const CVisBlobTreeIterator& refblobtreeiterator) const;

protected:
	bool FChildrenBeforeParents(void) const;

	CVisBlobInternal *PblobNext(void) const;
	CVisBlobInternal *PblobPrev(void) const;

	static CVisBlobInternal *PPrevSibling(CVisBlobInternal *pblobinternal,
			CVisBlobTreeInternal *pblobtreeinternal);


private:
	CVisBlobInternal *m_pblobinternal;
	CVisBlobTreeInternal *m_pblobtreeinternal;
	bool m_fChildrenBeforeParents;
};


class CVisBlobTree
{
public:
	typedef CVisBlobTreeIterator iterator;


	CVisBlobTree(void);
	CVisBlobTree(CVisBlobTreeInternal *pblobtreeinternal);
	CVisBlobTree(CVisBlobTree& refblobtree);
	CVisBlobTree& operator=(CVisBlobTree& refblobtree);
	~CVisBlobTree(void);

	bool IsValid(void) const;

	// Get a copy of the first blob in the list of top-level blobs.
	CVisBlob BlobFirst(void);

	// Remove noise blobs whose size is less than the size specified
	// or whose area is less than the area specified.
	void EliminateNoiseBlobs(SIZE size, unsigned long ulArea = 0);
	void EliminateNoiseBlobs(int dx, int dy, unsigned long ulArea = 0);
	void EliminateNoiseBlobs(unsigned long ulArea);

	void LabelBlobs(void);

	// Merge blob into parent and remove blob from tree.
	// Helper method used when eliminating noise blobs.
	// UNDONE:  Should this be named erase?
	void RemoveNoiseBlob(CVisBlob blob, bool fMergeIntoParent = true);


	// STL-like methods used to go through the blobs in the tree.
	iterator begin(bool fChildrenBeforeParents = false);
	iterator end(bool fChildrenBeforeParents = false);

	void clear(void);

	// UNDONE:  Make sure this function is correct.  Make sure that it
	// asserts or deletes any existing tree in case it's called a second time.
	void MakeTreeFromPixRunList(const CVisPixRunList& refpixrunlist,
			bool fEightConnected = false);

private:
	CVisBlobTreeInternal *m_pblobtreeinternal;
};


// UNDONE:  External blob functions


#include "VisBlob.inl"
