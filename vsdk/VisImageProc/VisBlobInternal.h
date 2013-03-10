// VisBlobInternal.h
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


// Note:  We originally designed these classes with a single kcObjAllocUnit
// template parameter.  It might be better to have separate kcPixRunAlloc
// and kcBlobAlloc paramters for the CVisBlobTreeInternal-derived classes.  We
// might want to change this sometime later.

// Note:  With a little more work we could add perimeter information
// while finding blobs.


// LATER:  Add typedefs that do not require template parameters for
// standard blob and blob tree types.

// LATER:  Later we may want to pass the AddStrip function (or a
// derived CVisBlobInternal class) as a template argument to the classes that
// find connected regions.  One AddStrip function could just add the
// strip, another could add the strip and find first-order moments,
// and a third could add the strip and find second-order moments.


// LATER:  Add debug asserts that cobjAllocUnit template params are as expected
// for blobs, strips, and pixruns.

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



// Forward declaration.
class CVisBlobInternal;
class CVisBlobTreeInternal;

enum { evisallocunitBlob = kcObjAllocUnitDefault };

typedef CVisObjListEnum<CVisBlobInternal *, evisallocunitBlob> CVisBlobInternalListEnum;
typedef CVisObjListBuilder<CVisBlobInternal *, evisallocunitBlob> CVisBlobInternalListBuilder;


#ifdef UNDONE
class CVisBlobTree
{
public:
	CVisBlobTree(void);
	CVisBlobTree(CVisBlobTree& refblobtree);
	CVisBlobTree& operator=(CVisBlobTree& refblobtree);
	~CVisBlobTree(void);

	// Get a pointer to the first blob in the list of top-level blobs.
	CVisBlobInternal *PBlobFirst(void);

	// Remove noise blobs whose size is less than the size specified
	// or whose area is less than the area specified.
	void EliminateNoiseBlobs(SIZE size, unsigned long ulArea = 0);
	void EliminateNoiseBlobs(int dx, int dy, unsigned long ulArea = 0);
	void EliminateNoiseBlobs(unsigned long ulArea);

	void LabelBlobs(void);

	// Helper method used when eliminating noise blobs.
	void RemoveNoiseBlob(CVisBlobInternal *pblob);


	// UNDONE:  Make sure this function is correct.  Make sure that it
	// asserts or deletes any existing tree in case it's called a second time.
	void MakeTreeFromPixRunList(const CVisPixRunList& refpixrunlist,
			bool fEightConnected = false);

private:
	CVisBlobTreeInternal *m_pblobtreeinternal;
};
#endif // UNDONE


// Helper class used to pass information about the current row
// being processed and the row preceeding it in the image.
class CVisFBBRI
{
public:
	CVisFBBRI(const CVisPixRunList& refpixrunlist);
	~CVisFBBRI();

	int YCur(void) const
		{ return m_y; }
	int YYCur(void) const
		{ return m_yy; }
	int IRPrevLast(void) const
		{ return m_iRPrevLast; }
	int IRCurLast(void) const
		{ return m_iRCurLast; }
	const int *RgxCur(void) const
		{ return m_rgxCur; }
	const int *RgxPrev(void) const
		{ return m_rgxPrev; }
	CVisBlobInternal **RgpblobCur(void) const
		{ return m_rgpblobCur; }
	CVisBlobInternal **RgpblobPrev(void) const
		{ return m_rgpblobPrev; }


	void Reset(void);
	BOOL FMore(void) const;
	void NextRow(void);

	static unsigned long SumOfSquaresToN(unsigned long n);

private:
	const RECT m_rect;
	CVisPixRunListEnum m_pixrunenum;
	int m_y;
	int m_iRPrevLast;
	int m_iRCurLast;
	int *m_rgxCur;
	int *m_rgxPrev;
	CVisBlobInternal **m_rgpblobCur;
	CVisBlobInternal **m_rgpblobPrev;

	// Variables to aid in calcualation involving sums of pixel values.
	// LATER:  Add arrays of partial sums to help with moments.
	unsigned long m_yy;

	void FreeBuffers(void);
};


class CVisBlobTreeInternal
{
public:
	CVisBlobTreeInternal(void);  // UNDONE:  Should this be protected?
	~CVisBlobTreeInternal(void);  // UNDONE:  Should this be protected?

	unsigned long AddRef(void);
	unsigned long Release(void);

	// Get a pointer to the first blob in the list of top-level blobs.
	CVisBlobInternal *PBlobFirst(void);

	// Remove noise blobs whose size is less than the size specified
	// or whose area is less than the area specified.
	void EliminateNoiseBlobs(SIZE size, unsigned long ulArea = 0);
	void EliminateNoiseBlobs(int dx, int dy, unsigned long ulArea = 0);
	void EliminateNoiseBlobs(unsigned long ulArea);

	void LabelBlobs(void);

	// Helper method used when eliminating noise blobs.
	void RemoveNoiseBlob(CVisBlobInternal *pblob, bool fMergeIntoParent = true);


	// UNDONE:  Make sure this function is correct.  Make sure that it
	// asserts or deletes any existing tree in case it's called a second time.
	void MakeTreeFromPixRunList(const CVisPixRunList& refpixrunlist,
			bool fEightConnected = false);

protected:
	// Add a blob to the list of top-level blobs.
	void AddTopLevelBlob(CVisBlobInternal *pblob);

	CVisBlobInternal *MergeBlobs(
			CVisBlobInternal **rgpblobPrev, int iRPrev,
			int iRPrevLast, CVisBlobInternal **rgpblobCur,
			int iRCur);

	void Find4ConnectedBlobsInRow(const CVisFBBRI& fbbri);
	void Find8ConnectedBlobsInRow(const CVisFBBRI& fbbri);

	void DeleteTree(void);
	void DeleteTree(CVisBlobInternal *pblob);

private:
	// The pointer to the first blob in the list of top-level blobs.
	CVisBlobInternal *m_pblobFirst;

	// Reference count.
	int m_cRef;
};


// Apply a function operator to each node in the tree of blobs.  The
// function operator will be applied to a blob and its children
// before the blob's next sibling is processed.  (So, the function
// that processes children before processing parents can be used to
// delete all blobs in the tree or to merge noise blobs into their
// parents.)  These functions are efficient (and not recursive).
template<class TFnOp>
void ApplyFnOpToBlobTreeParentFirst(TFnOp& fnop,
		CVisBlobTreeInternal& refpblobtree);

template<class TFnOp>
void ApplyFnOpToBlobTreeChildFirst(TFnOp& fnop,
		CVisBlobTreeInternal& refpblobtree);


template<class TPixel>
void SetFgBlobsInImage(CVisBlobTreeInternal& refpblobtree,
		CVisImage<TPixel>& refimage, TPixel pixelSetValue);

template<class TPixel>
void XorFgBlobsInImage(CVisBlobTreeInternal& refpblobtree,
		CVisImage<TPixel>& refimage, TPixel pixelXorValue);

template<class TPixel>
void AddFgBlobsToImage(CVisBlobTreeInternal& refpblobtree,
		CVisImage<TPixel>& refimage, TPixel pixelAddValue);


// Set image pixels corresponding to blobs.
template<class TPixel>
TPixel LabelBlobsInImage(CVisBlobTreeInternal& refpblobtree,
		CVisImage<TPixel>& refimage, TPixel pixelFirstLabel,
		TPixel pixelLabelStep);

// Set blob labels, image pixels corresponding to blobs, and build
// a list of all blobs in the tree.
template<class TPixel>
TPixel LabelBlobsInImageEx(CVisBlobTreeInternal& refpblobtree,
		CVisImage<TPixel>& refimage, TPixel pixelFirstLabel,
		TPixel pixelLabelStep,
		CVisBlobInternalListEnum&
		refpblobenum);



// LATER:  We might want to make a general tree-node base class containing
// just the parent, next sibling, and first child pointers.
// LATER:  We might want to create a CBlobBase class that doesn't have
// any template arguments.
class CVisBlobInternal
{
	friend void CVisBlobTreeInternal::AddTopLevelBlob(
			CVisBlobInternal *pblob);
	friend CVisBlobInternal *
		CVisBlobTreeInternal::MergeBlobs(
			CVisBlobInternal **rgpblobPrev, int iRPrev,
			int iRPrevLast, CVisBlobInternal **rgpblobCur,
			int iRCur);
	friend void CVisBlobTreeInternal::RemoveNoiseBlob(
			CVisBlobInternal *pblob, bool fMergeIntoParent);
	friend void CVisBlobTreeInternal::DeleteTree(
			CVisBlobInternal *pblob);

public:
	CVisBlobInternal(bool fForeground, CVisBlobInternal *pblobParent,
			int xL, int xR, int y,
			const CVisFBBRI& fbbri);
	CVisBlobInternal(const CVisBlobInternal& refblob);
	CVisBlobInternal& operator=(
			const CVisBlobInternal& refblob);

	// Get a string describing one or more properties of this blob.
	void GetPropString(int evisblobprop, CVisString& refszProp,
			bool fAppend = false) const;

	bool IsForeground(void) const;

	CVisStripListEnum StripEnum(void) const;

	CVisBlobInternal *Parent(void);
	CVisBlobInternal *NextSibling(void);
	CVisBlobInternal *FirstChild(void);

	// This is implementation dependent and may not be very efficient.
	// Also, this method may assume that this blob is not a top-level blob.
	CVisBlobInternal *PrevSibling(void);
	// This is implementation dependent and may not be very efficient.
	CVisBlobInternal *LastChild(void);

	void AddStrip(int xL, int xR, int y,
			const CVisFBBRI& fbbri);
	void SetParent(CVisBlobInternal *pblob);

	// Get the label for this blob.  This is set in the standard
	// blob-labeling functions.  It may not be initialized if the
	// blobs have not been labeled using one of the standard
	// blob-labeling functions.
	unsigned int ILabel(void) const;

	void SetILabel(unsigned int iLabel);

	// Find the depth of the blob in the tree.  Depth information is not
	// stored in the blob.  Instead it is computed by counting the number
	// of blobs between this blob and the root of the tree.  It is safe
	// to call this method using a NULL pointer to a blob.
	unsigned int SafeDepthInTree(void) const;

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

	// LATER:  It could be nice to have a method to check whether a
	// pixel is contained in this blob (or its children).

protected:
	// Add information from a strip to the sums of pixel values.
	void UpdatePixelSums(int xL, int xR, int y,
			const CVisFBBRI& fbbri);
private:
	// Does this blob represent forground pixels or background pixels?
	bool m_fForeground;

	// Pointer to parent.
	CVisBlobInternal *m_pblobParent;

	// Pointer to sibling.
	CVisBlobInternal *m_pblobNextSibling;

	// Pointer to first child.
	CVisBlobInternal *m_pblobFirstChild;

	// List of strips.
	CVisStripListBuilder m_striplistbuilder;

	// Area of blob.
	unsigned long m_ulArea;

	// Bounding rectangle.
	CVisRect m_rectBounding;

	// The label for this blob.  This is set in the standard
	// blob-labeling functions.  It may not be initialized if the
	// blobs have not been labeled using one of the standard
	// blob-labeling functions.
	unsigned int m_iLabel;

	// Sum of x values of all pixels in the blob.
	LONGLONG m_llSumX;

	// Sum of y values of all pixels in the blob.
	LONGLONG m_llSumY;

	// Sum of squares of x values of all pixels in the blob.
	LONGLONG m_llSumXX;

	// Sum of products of x and y values of all pixels in the blob.
	// (For each pixel in the blob, we find x * y and add it to the
	// sum.)
	LONGLONG m_llSumXY;

	// Sum of squares of y values of all pixels in the blob.
	LONGLONG m_llSumYY;

	// LATER:  Flags (is this noise, was this background), region number,
	// perimeter, moments, area, depth?,...

	// This method assumes that pblob->Parent() == this->Parent().
	void MergeIntoSibling(CVisBlobInternal *pblob);

	// This method assumes that this->Parent()->Parent() == pblob.
	void MergeIntoGrandparent(CVisBlobInternal *pblob);

	// This method merges a blob and its children into the blob's parent.
	void MergeIntoParentAndDelete(void);

	// Remove this blob from a tree of blobs.  This assumes that this blob
	// is not a top-level blob.
	void RemoveFromParentAndSiblings(void);

	// Add a list of blobs to this blob's list of children.  The blobs in
	// list will be changed to know that this blob is their new parent.
	void AddChildren(CVisBlobInternal *pblobFirstChild);
};


#include "VisBlobFnOp.h"
#include "VisBlobInternal.inl"
