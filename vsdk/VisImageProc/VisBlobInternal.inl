// VisBlobInternal.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG


// UNDONE:  Now that cbAllocUnit template parameters have been removed, we can
// move some large inline function in this file to a CPP file.

template<class TFnOp>
inline void ApplyFnOpToBlobTreeParentFirst(TFnOp& fnop,
		CVisBlobTreeInternal& refpblobtree)
{
	// UNDONE:  This function has a bug in it.
	bool fSearchedChildren = false;
	
	CVisBlobInternal *pblob = refpblobtree.PBlobFirst();

	while (pblob != 0)
	{
		// Don't process the parent blob twice.
		if (!fSearchedChildren)
		{
			fnop(pblob);
		}

		CVisBlobInternal *pblobNext;

		if (fSearchedChildren)
		{
			fSearchedChildren = false;
			pblobNext = 0;
		}
		else
		{
			pblobNext = pblob->FirstChild();
		}

		if (pblobNext == 0)
		{
			// Next is not a child, so next may be a sibling or parent.
			pblobNext = pblob->NextSibling();
		}

		if (pblobNext == 0)
		{
			// Next is not a child or sibling, so it must be the parent.
			pblobNext = pblob->Parent();
			fSearchedChildren = true;
		}

		pblob = pblobNext;
	}
}


template<class TFnOp>
inline void ApplyFnOpToBlobTreeChildFirst(TFnOp& fnop,
		CVisBlobTreeInternal& refpblobtree)
{
	// UNDONE:  This function has a bug in it.
	bool fSearchedChildren = false;
	
	CVisBlobInternal *pblob = refpblobtree.PBlobFirst();

	while (pblob != 0)
	{
		CVisBlobInternal *pblobNext;

		if (fSearchedChildren)
		{
			fSearchedChildren = false;
			pblobNext = 0;
		}
		else
		{
			pblobNext = pblob->FirstChild();
		}

		if (pblobNext == 0)
		{
			// Next is not a child, so next may be a sibling or parent.
			pblobNext = pblob->NextSibling();
			if (pblobNext == 0)
			{
				pblobNext = pblob->Parent();
				fSearchedChildren = true;
			}

			// Since next is not a child of this blob, it's time to process
			// this blob.
			fnop(pblob);
		}

		pblob = pblobNext;
	}
}


inline CVisFBBRI::CVisFBBRI(
		const CVisPixRunList& refpixrunlist)
  : m_rect(refpixrunlist.ShapeImage()),
	m_pixrunenum(refpixrunlist.PixRunEnum()),
	m_rgxCur(0),
	m_rgxPrev(0),
	m_rgpblobCur(0),
	m_rgpblobPrev(0)
{
	Reset();
}

inline CVisFBBRI::~CVisFBBRI()
{
	FreeBuffers();
}

inline void CVisFBBRI::Reset(void)
{
	// Free currently allocated memory.
	FreeBuffers();

	// Reset the variables that we don't need to reallocate.
	m_pixrunenum.Reset();
	m_y = m_rect.top - 1;
	m_yy = (unsigned long) (m_y * m_y);
	m_iRCurLast = 1;

	// Allocate space for information about the current and previous row.
	const int iMax = m_rect.right - m_rect.left + 3;
	m_rgxPrev = new int[iMax];
	m_rgpblobPrev = new CVisBlobInternal *[iMax];
	m_rgxCur = new int[iMax];
	m_rgpblobCur = new CVisBlobInternal *[iMax];

	// Set up information about the background above the first row.
	m_rgxCur[0] = m_rect.left;
	m_rgxCur[1] = m_rect.right;
	m_rgpblobCur[1] = 0;

	// Get information about the first row in the image.
	NextRow();
}

inline BOOL CVisFBBRI::FMore(void) const
{
	return (m_y <= m_rect.bottom);
}

inline void CVisFBBRI::NextRow(void)
{
	assert(m_y <= m_rect.bottom);

	// Remember information about the previous row.
	int *rgxT = m_rgxPrev;
	m_rgxPrev = m_rgxCur;
	m_rgxCur = rgxT;
	CVisBlobInternal **rgpblobT = m_rgpblobPrev;
	m_rgpblobPrev = m_rgpblobCur;
	m_rgpblobCur = rgpblobT;
	m_iRPrevLast = m_iRCurLast;

	if (++m_y < m_rect.bottom)
	{
		// Find information about the next row.
		// This will loop over all pixels in the row.
		int x = m_rect.left;
		m_iRCurLast = 0;
		m_rgxCur[0] = x;
		while (x != m_rect.right)
		{
			assert(m_pixrunenum.ObjCur() >= 0);

			x += m_pixrunenum.ObjCur();

			++ m_iRCurLast;
			m_rgxCur[m_iRCurLast] = x;

			assert(x <= m_rect.right);

			// The next iteration will use the next run in the list.
			assert(m_pixrunenum.FMore());
			m_pixrunenum.Next();
		}

		// Rows must start and end with (possibly zero-length) runs of
		// background pixels, so m_iRCurLast should be odd at the end of
		// the row.
		if ((m_iRCurLast & 1) == 0)
		{
			// Add a zero-length run.
			++ m_iRCurLast;
			m_rgxCur[m_iRCurLast] = x;

			assert(m_pixrunenum.ObjCur() == 0);
			assert(m_pixrunenum.FMore());
			m_pixrunenum.Next();
		}

		// Append a large value to indicate the end of the list.
		m_rgxCur[m_iRCurLast + 1] = x + 1;
	}
	else
	{
		// We should have proccessed each run in the list.
		assert(!m_pixrunenum.FMore());

		// Set up information about the background below the last row.
		m_iRCurLast = 1;
		m_rgxCur[0] = m_rect.left;
		m_rgxCur[1] = m_rect.right;
		m_rgxCur[2] = m_rect.right + 1;
		m_rgpblobCur[1] = 0;
	}

	// Find the square of the current row coordinate.  Because we're using a
	// 32-bit member variable here, we assume that | m_y | < 2 ^ 16.
#ifndef VIS_NO_FBBRI_YY_ASSERT
	assert(((m_y & 0xffff0000) == 0) || ((m_y & 0xffff0000) == 0xffff0000));
#endif // VIS_NO_FBBRI_YY_ASSERT
	m_yy = (unsigned long) (m_y * m_y);
}


inline unsigned long CVisFBBRI::SumOfSquaresToN(
		unsigned long n)
{
	// return n * (2 * n * n + 3 * n + 1) / 6;
	return n * (n * ((n << 1) + 3) + 1) / 6;
}

inline void CVisFBBRI::FreeBuffers(void)
{
	if (m_rgxCur != 0)
	{
		delete[] m_rgxCur;
		m_rgxCur = 0;
	}
	if (m_rgxPrev != 0)
	{
		delete[] m_rgxPrev;
		m_rgxPrev = 0;
	}
	if (m_rgpblobCur != 0)
	{
		delete[] m_rgpblobCur;
		m_rgpblobCur = 0;
	}
	if (m_rgpblobPrev != 0)
	{
		delete[] m_rgpblobPrev;
		m_rgpblobPrev = 0;
	}
}



template<class TPixel>
inline void SetFgBlobsInImage(CVisBlobTreeInternal& refpblobtree,
		CVisImage<TPixel>& refimage, TPixel pixelSetValue)
{
	CVisFnOpSetFgStripsInImage<TPixel>
			fnopsetstrips(refimage, pixelSetValue);
	ApplyFnOpToBlobTreeParentFirst<CVisFnOpSetFgStripsInImage<TPixel> >(fnopsetstrips, refpblobtree);
}

template<class TPixel>
inline void XorFgBlobsInImage(CVisBlobTreeInternal& refpblobtree,
		CVisImage<TPixel>& refimage, TPixel pixelXorValue)
{
	CVisFnOpXorFgStripsInImage<TPixel>
			fnopxorstrips(refimage, pixelXorValue);
	ApplyFnOpToBlobTreeParentFirst<CVisFnOpXORFgStripsInImage<TPixel> >(fnopxorstrips, refpblobtree);
}

template<class TPixel>
inline void AddFgBlobsToImage(CVisBlobTreeInternal& refpblobtree,
		CVisImage<TPixel>& refimage, TPixel pixelAddValue)
{
	CVisFnOpAddFgStripsToImage<TPixel>
			fnopxorstrips(refimage, pixelAddValue);
	ApplyFnOpToBlobTreeParentFirst<CVisFnOpAddFgStripsToImage<TPixel> >(fnopxorstrips, refpblobtree);
}

template<class TPixel>
inline TPixel LabelBlobsInImage(CVisBlobTreeInternal& refpblobtree,
		CVisImage<TPixel>& refimage, TPixel pixelFirstLabel,
		TPixel pixelLabelStep)
{
	CVisFnOpLabelBlobs<TPixel>
			fnopLabelBlobs(refimage, pixelFirstLabel, pixelLabelStep);
	ApplyFnOpToBlobTreeParentFirst<CVisFnOpLabelBlobs<TPixel> >(fnopLabelBlobs, refpblobtree);

	return fnopLabelBlobs.NextLabel();
}


template<class TPixel>
inline TPixel LabelBlobsInImageEx(CVisBlobTreeInternal& refpblobtree,
		CVisImage<TPixel>& refimage, TPixel pixelFirstLabel,
		TPixel pixelLabelStep,
		CVisBlobInternalListEnum& refpblobenum)
{
	CVisFnOpLabelBlobsEx<TPixel>
			fnopLabelBlobsEx(refimage, pixelFirstLabel, pixelLabelStep);
	ApplyFnOpToBlobTreeParentFirst<CVisFnOpLabelBlobsEx<TPixel> >(fnopLabelBlobsEx, refpblobtree);

	refpblobenum = fnopLabelBlobsEx.PblobEnum();

	return fnopLabelBlobsEx.NextLabel();
}


inline CVisBlobInternal::CVisBlobInternal(bool fForeground,
		CVisBlobInternal *pblobParent, int xL, int xR, int y,
		const CVisFBBRI& fbbri)
  : m_fForeground(fForeground),
	m_pblobParent(pblobParent),
	m_pblobFirstChild(0),
	m_striplistbuilder(),
	m_rectBounding(xL, y, xR, y + 1),
	m_ulArea(0),
	m_llSumX(0),
	m_llSumY(0),
	m_llSumXX(0),
	m_llSumXY(0),
	m_llSumYY(0)
{
	assert(xL < xR);

	m_striplistbuilder.ObjNext().SetRow(y);
	m_striplistbuilder.ObjNext().SetLeft(xL);
	m_striplistbuilder.ObjNext().SetRight(xR);
	m_striplistbuilder.AddObj();

	if (pblobParent != 0)
	{
		// Add this blob to the parent's tree.
		m_pblobNextSibling = pblobParent->m_pblobFirstChild;
		pblobParent->m_pblobFirstChild = this;
	}
//#ifdef TAKEOUT
	else
	{
		// UNDONE:  This may not be needed.
		m_pblobNextSibling = 0;
	}
//#endif // TAKEOUT

	UpdatePixelSums(xL, xR, y, fbbri);
}

inline CVisBlobInternal::CVisBlobInternal(
		const CVisBlobInternal& refblob)
  : m_fForeground(refblob.m_fForeground),
	m_pblobParent(0),
	m_pblobNextSibling(0),
	m_pblobFirstChild(0),
	m_striplistbuilder(refblob.m_striplistbuilder),
	m_rectBounding(refblob.m_rectBounding),
	m_ulArea(refblob.m_ulArea),
	m_llSumX(refblob.m_llSumX),
	m_llSumY(refblob.m_llSumY),
	m_llSumXX(refblob.m_llSumXX),
	m_llSumXY(refblob.m_llSumXY),
	m_llSumYY(refblob.m_llSumYY)
{
	// Only the original blob is in the tree;  The copy's
	// parent, sibling, and child pointers are 0.
}

inline CVisBlobInternal& CVisBlobInternal::operator=(
		const CVisBlobInternal& refblob)
{
	m_fForeground = refblob.m_fForeground;
	m_pblobParent = 0;
	m_pblobNextSibling = 0;
	m_pblobFirstChild = 0;
	m_striplistbuilder = refblob.m_striplistbuilder;
	m_rectBounding = refblob.m_rectBounding;
	m_ulArea = refblob.m_ulArea;
	m_llSumX = refblob.m_llSumX;
	m_llSumY = refblob.m_llSumY;
	m_llSumXX = refblob.m_llSumXX;
	m_llSumXY = refblob.m_llSumXY;
	m_llSumYY = refblob.m_llSumYY;

	return *this;
}

inline void CVisBlobInternal::GetPropString(int evisblobprop,
		CVisString& refszProp, bool fAppend) const
{
	if (!fAppend)
		refszProp = "";

	// Special code to allow (this == 0).
	if (this == 0)
	{
		if ((evisblobprop & evisblobpropTitle) != 0)
			refszProp += "Background Region 0";
		else
			refszProp += "Background region connected to border of image.";

		return;
	}

	for ( ; ; )
	{
		// Remove the lowest bit from evisblobprop.
		int evisblobpropNext = (evisblobprop & (evisblobprop - 1));

		// Add a description for the current property.
		CVisString szT;
		char szBufT[100];  // Used for 64-bit integers.
		switch (evisblobprop ^ evisblobpropNext)
		{
		case evisblobpropTitle:
			szT.Format("%sground Region %d",
					(m_fForeground ? "Fore" : "Back"), m_iLabel);
			break;

		case evisblobpropParent:
			szT.Format("Parent:    %d",
					(m_pblobParent == 0) ? 0 : m_pblobParent->ILabel());
			break;

		case evisblobpropChildren:
			{
				szT = "Children:  ";
				const CVisBlobInternal *pblobChild = m_pblobFirstChild;
				if (pblobChild == 0)
				{
					szT += "None";
				}
				else
				{
					CVisString szChildren;
					szChildren.Format("%u", pblobChild->ILabel());
					szT += szChildren;

					for (pblobChild = pblobChild->m_pblobNextSibling;
							pblobChild != 0;
							pblobChild = pblobChild->m_pblobNextSibling)
					{
						szChildren.Format(", %u", pblobChild->ILabel());
						szT += szChildren;
					}
				}
			}
			break;

		case evisblobpropDepth:
			szT.Format("Depth:     %d", SafeDepthInTree());
			break;

		case evisblobpropWidth:
			szT.Format("Width:     %d", Width());
			break;

		case evisblobpropHeight:
			szT.Format("Height:    %d", Height());
			break;

		case evisblobpropArea:
			szT.Format("Area:      %d", Area());
			break;

		case evisblobpropRect:
			szT.Format("Rect:      (%d, %d, %d, %d)",
					m_rectBounding.left, m_rectBounding.top,
					m_rectBounding.right, m_rectBounding.bottom);
			break;

		case evisblobpropCentroid:
			szT.Format("Centroid:  (%4.0f, %4.2f)", DblXCentroid(), DblYCentroid());
			break;

		case evisblobpropSumX:
			szT.Format("Sum of X:  %s", _i64toa(SumX(), szBufT, 10));
			break;

		case evisblobpropSumY:
			szT.Format("Sum of Y:  %s", _i64toa(SumY(), szBufT, 10));
			break;

		case evisblobpropSumXX:
			szT.Format("Sum of XX:  %s", _i64toa(SumXX(), szBufT, 10));
			break;

		case evisblobpropSumXY:
			szT.Format("Sum of XY:  %s", _i64toa(SumXY(), szBufT, 10));
			break;

		case evisblobpropSumYY:
			szT.Format("Sum of YY:  %s", _i64toa(SumYY(), szBufT, 10));
			break;

#ifdef _DEBUG
		default:
			// Unexpected evisblobprop value.
			assert(false);
#endif // _DEBUG
		}
		refszProp += szT;

		// Are we done yet?
		if (evisblobpropNext == 0)
			break;

		evisblobprop = evisblobpropNext;

		// If there are more strings, start a new line.
		refszProp += "\n";
	}
}

inline bool CVisBlobInternal::IsForeground(void) const
{
	return m_fForeground;
}

inline CVisStripListEnum
	CVisBlobInternal::StripEnum(void) const
{
	return m_striplistbuilder;
}

inline CVisBlobInternal *CVisBlobInternal::Parent(void)
{
	return m_pblobParent;
}

inline CVisBlobInternal *CVisBlobInternal::NextSibling(void)
{
	return m_pblobNextSibling;
}

inline CVisBlobInternal *CVisBlobInternal::FirstChild(void)
{
	return m_pblobFirstChild;
}

// This is implementation dependent and may not be very efficient.
// Also, this method may assume that this blob is not a top-level blob.
inline CVisBlobInternal *CVisBlobInternal::PrevSibling(void)
{
	assert(m_pblobParent != 0);
	assert(m_pblobParent->FirstChild() != 0);
	
	CVisBlobInternal *pblob = m_pblobParent->FirstChild();
	
	if (pblob != this)
	{
		while (pblob->NextSibling() != this)
		{
			pblob = pblob->NextSibling();
			assert(pblob != 0);
		}
	}
	else
	{
		// No Prev sibling if this is the first child.
		pblob = 0;
	}

	return pblob;
}

// This is implementation dependent and may not be very efficient.
inline CVisBlobInternal *CVisBlobInternal::LastChild(void)
{
	CVisBlobInternal *pblob = m_pblobFirstChild;
	
	if (pblob != 0)
	{
		while (pblob->NextSibling() != 0)
			pblob = pblob->NextSibling();
	}

	return pblob;
}

inline unsigned int CVisBlobInternal::ILabel(void) const
{
	return m_iLabel;
}

inline void CVisBlobInternal::SetILabel(unsigned int iLabel)
{
	m_iLabel = iLabel;
}

inline unsigned int CVisBlobInternal::SafeDepthInTree(void) const
{
	unsigned int uiDepth = 0;

	// Go to the root of the tree, counting the number of blobs encountered
	// along the way.
	const CVisBlobInternal *pblob = this;
	while (pblob != 0)
	{
		pblob = pblob->m_pblobParent;
		++ uiDepth;
	}

	return uiDepth;
}

inline int CVisBlobInternal::Width(void) const
{
	return m_rectBounding.Width();
}

inline int CVisBlobInternal::Height(void) const
{
	return m_rectBounding.Height();
}

inline const CVisRect& CVisBlobInternal::RectBounding(void) const
{
	return m_rectBounding;
}

inline unsigned long CVisBlobInternal::Area(void) const
{
	return m_ulArea;
}

inline double CVisBlobInternal::DblXCentroid(void) const
{
	bool fNegative = (m_llSumX < 0);
	DWORDLONG ull = ((fNegative) ? - m_llSumX : m_llSumX);

	unsigned long ulIntegralPart = (unsigned long) (ull / m_ulArea);
	unsigned long ulFractionalPart
			= (unsigned long) (ull - (DWORDLONG) (ulIntegralPart * m_ulArea));

	double dbl = ulIntegralPart + ((double) ulFractionalPart / m_ulArea);
	if (fNegative)
		dbl = - dbl;

	return dbl;
}

inline double CVisBlobInternal::DblYCentroid(void) const
{
	bool fNegative = (m_llSumY < 0);
	DWORDLONG ull = ((fNegative) ? - m_llSumY : m_llSumY);

	unsigned long ulIntegralPart = (unsigned long) (ull / m_ulArea);
	unsigned long ulFractionalPart
			= (unsigned long) (ull - (DWORDLONG) (ulIntegralPart * m_ulArea));

	double dbl = ulIntegralPart + ((double) ulFractionalPart / m_ulArea);
	if (fNegative)
		dbl = - dbl;

	return dbl;
}

inline LONGLONG CVisBlobInternal::SumX(void) const
{
	return m_llSumX;
}

inline LONGLONG CVisBlobInternal::SumY(void) const
{
	return m_llSumY;
}

inline LONGLONG CVisBlobInternal::SumXX(void) const
{
	return m_llSumXX;
}

inline LONGLONG CVisBlobInternal::SumXY(void) const
{
	return m_llSumXY;
}

inline LONGLONG CVisBlobInternal::SumYY(void) const
{
	return m_llSumYY;
}


inline void CVisBlobInternal::AddStrip(int xL, int xR, int y,
		const CVisFBBRI& fbbri)
{
	assert(xL < xR);

	m_striplistbuilder.ObjNext().SetRow(y);
	m_striplistbuilder.ObjNext().SetLeft(xL);
	m_striplistbuilder.ObjNext().SetRight(xR);
	m_striplistbuilder.AddObj();

	// Make sure to adjust the bounding rectangle, if needed.
	if (xL < m_rectBounding.left)
		m_rectBounding.left = xL;
	else if (xR > m_rectBounding.right)
		m_rectBounding.right = xR;

	if (y < m_rectBounding.top)
		m_rectBounding.top = y;
	else if (y >= m_rectBounding.bottom)
		m_rectBounding.bottom = y + 1;

	// Add sums calculated from pixels of this strip to the blob.
	UpdatePixelSums(xL, xR, y, fbbri);
}

// This method assumes that pblob->Parent() == this->Parent().
inline void CVisBlobInternal::MergeIntoSibling(
		CVisBlobInternal *pblob)
{
	m_striplistbuilder.MergeInto(pblob->m_striplistbuilder);

	// Make sure to adjust the bounding rectangle, if needed.
	pblob->m_rectBounding |= m_rectBounding;

	if (m_pblobFirstChild != 0)
	{
		// Give the children of this blob to pblob.
		pblob->AddChildren(m_pblobFirstChild);
//#ifdef TAKEOUT
		// UNDONE:  This may not be needed.
		m_pblobFirstChild = 0;
//#endif // TAKEOUT
	}

	// Add sums calculated from pixels of this blob to pblob.
	pblob->m_ulArea += m_ulArea;
	pblob->m_llSumX += m_llSumX;
	pblob->m_llSumY += m_llSumY;
	pblob->m_llSumXX += m_llSumXX;
	pblob->m_llSumXY += m_llSumXY;
	pblob->m_llSumYY += m_llSumYY;
}

// This method assumes that this->Parent()->Parent() == pblob.
inline void CVisBlobInternal::MergeIntoGrandparent(
		CVisBlobInternal *pblob)
{
	m_striplistbuilder.MergeInto(pblob->m_striplistbuilder);

	// Make sure to adjust the bounding rectangle, if needed.
	pblob->m_rectBounding |= m_rectBounding;

	if (m_pblobFirstChild != 0)
	{
		// Give the children of this blob to pblob.
		pblob->AddChildren(m_pblobFirstChild);
//#ifdef TAKEOUT
		// UNDONE:  This may not be needed.
		m_pblobFirstChild = 0;
//#endif // TAKEOUT
	}

	// Add sums calculated from pixels of this blob to pblob.
	pblob->m_ulArea += m_ulArea;
	pblob->m_llSumX += m_llSumX;
	pblob->m_llSumY += m_llSumY;
	pblob->m_llSumXX += m_llSumXX;
	pblob->m_llSumXY += m_llSumXY;
	pblob->m_llSumYY += m_llSumYY;

//#ifdef TAKEOUT
	// Removes this blob from parent and sibling list.
	// UNDONE:  This may not be needed.
	RemoveFromParentAndSiblings();
	m_pblobParent = 0;
	m_pblobNextSibling = 0;
//#endif // TAKEOUT
}

// This method merges a blob and its children into the blob's parent.
inline void CVisBlobInternal::MergeIntoParentAndDelete(void)
{
	// Merge children into parent and delete children
	CVisBlobInternal *pblobParent = Parent();
	CVisBlobInternal *pblobChild = FirstChild();
	while (pblobChild != 0)
	{
		CVisBlobInternal *pblobNextChild
				= pblobChild->NextSibling();
		pblobChild->MergeIntoGrandparent(pblobParent);

		delete pblobChild;

		pblobChild = pblobNextChild;
	}

	// Merge this blob into parent
	m_striplistbuilder.MergeInto(pblobParent->m_striplistbuilder);

	// Make sure to adjust the bounding rectangle, if needed.
	pblobParent->m_rectBounding |= m_rectBounding;

	// Add sums calculated from pixels of this blob to pblob.
	pblobParent->m_ulArea += m_ulArea;
	pblobParent->m_llSumX += m_llSumX;
	pblobParent->m_llSumY += m_llSumY;
	pblobParent->m_llSumXX += m_llSumXX;
	pblobParent->m_llSumXY += m_llSumXY;
	pblobParent->m_llSumYY += m_llSumYY;

	// Delete this blob.
	delete this;
}


// Remove this blob from a tree of blobs.  This assumes that this blob
// is not a top-level blob.
inline void CVisBlobInternal::RemoveFromParentAndSiblings(void)
{
	assert(Parent() != 0);

	CVisBlobInternal *pblobPrevSibling = PrevSibling();
	if (pblobPrevSibling == 0)
	{
		Parent()->m_pblobFirstChild
			 = NextSibling();
	}
	else
	{
		pblobPrevSibling->m_pblobNextSibling
			 = NextSibling();
	}
}

// Add a list of blobs to this blob's list of children.  The blobs in
// list will be changed to know that this blob is their new parent.
inline void CVisBlobInternal::AddChildren(
		CVisBlobInternal *pblobFirstChild)
{
	assert(pblobFirstChild != 0);

	CVisBlobInternal *pblob = pblobFirstChild;

	// Tell each blob in the list that this blob is its new parent.
	// UNDONE:  Assert that children are within the bounding rectangle?
	pblob->m_pblobParent = this;
	while (pblob->NextSibling() != 0)
	{
		pblob = pblob->NextSibling();
		pblob->m_pblobParent = this;
	}

	// Append the current list of children to the new list of children.
	pblob->m_pblobNextSibling = FirstChild();

	// Include the new children in the list of children of this blob.
	m_pblobFirstChild = pblobFirstChild;
}


inline void CVisBlobInternal::UpdatePixelSums(int xL, int xR, int y,
		const CVisFBBRI& fbbri)
{
	// LATER:  Make these more efficient by storing partial sums in fbbri?
	unsigned long dx = (unsigned long) (xR - xL);
	m_ulArea += dx;

//#ifdef WANT_OTHER_SUMS

	m_llSumY += dx * y;

//#ifdef WANT_SECOND_ORDER_OTHER_SUMS
	m_llSumYY += dx * fbbri.YYCur();
//#endif // WANT_SECOND_ORDER_OTHER_SUMS

	long lSumX = (dx * (xL + xR - 1)) >> 1;
	m_llSumX += lSumX;

//#ifdef WANT_SECOND_ORDER_OTHER_SUMS
	m_llSumXY += lSumX * y;

	// Sums of squares of x are tricky.
	if (xL > 0)
	{
		m_llSumXX += (fbbri.SumOfSquaresToN((unsigned long) (xR - 1))
				- fbbri.SumOfSquaresToN((unsigned long) (xL - 1)));
	}
	else if (xR < 0)
	{
		m_llSumXX += (fbbri.SumOfSquaresToN((unsigned long) (- xL))
				- fbbri.SumOfSquaresToN((unsigned long) (- xR)));
	}
	else
	{
		m_llSumXX += (fbbri.SumOfSquaresToN((unsigned long) (- xL))
				+ fbbri.SumOfSquaresToN((unsigned long) (xR - 1)));
	}

//#endif // WANT_SECOND_ORDER_OTHER_SUMS

//#endif // WANT_OTHER_SUMS
}


inline CVisBlobTreeInternal::CVisBlobTreeInternal(void)
  : m_pblobFirst(0),
	m_cRef(1)
{
}


inline CVisBlobTreeInternal::~CVisBlobTreeInternal(void)
{
	DeleteTree();
}


inline unsigned long CVisBlobTreeInternal::AddRef(void)
{
	// This assert might help to catch synchronization problems.
	assert(m_cRef != 0);

#ifdef VIS_ALLOW_NULL_ADDREF

	// The check for (this != 0) allows us to safely call AddRef
	// from a NULL pointer.
	if (this != 0)
		return (unsigned long) InterlockedIncrement((LONG *) &m_cRef);

	return 0;

#else // VIS_ALLOW_NULL_ADDREF

	return (unsigned long) InterlockedIncrement((LONG *) &m_cRef);

#endif // VIS_ALLOW_NULL_ADDREF
}


inline unsigned long CVisBlobTreeInternal::Release(void)
{
	unsigned long cRef;

#ifdef VIS_ALLOW_NULL_RELEASE
	// The check for (this == 0) allows us to safely call Release
	// from a NULL pointer.
	if (this == 0)
	{
		cRef = 0;
	}
	else
#endif // VIS_ALLOW_NULL_RELEASE
	{
		// This assert might help to catch synchronization problems.
		assert(m_cRef != 0);

		cRef = (unsigned long) InterlockedDecrement((LONG *) &m_cRef);
		
		if (cRef == 0)
			delete this;
	}

	return cRef;
}

// Get a pointer to the first blob in the list of top-level blobs.
inline CVisBlobInternal *
	CVisBlobTreeInternal::PBlobFirst(void)
{
	return m_pblobFirst;
}



inline void CVisBlobTreeInternal::LabelBlobs(void)
{
	CVisFnOpSetBlobLabels fnopSetBlobLabels;
	ApplyFnOpToBlobTreeParentFirst<CVisFnOpSetBlobLabels>(fnopSetBlobLabels, *this);
}


inline void CVisBlobTreeInternal::EliminateNoiseBlobs(
		SIZE size, unsigned long ulArea)
{
	EliminateNoiseBlobs(size.cx, size.cy, ulArea);
}


inline void CVisBlobTreeInternal::EliminateNoiseBlobs(
		int dx, int dy, unsigned long ulArea)
{
	if ((dx != 0) || (dy != 0) || (ulArea != 0))
	{
		CVisFnOpElimNoiseBlobs
				fnopElimNoiseBlobs(*this, dx, dy, ulArea);
		ApplyFnOpToBlobTreeChildFirst<CVisFnOpElimNoiseBlobs>(fnopElimNoiseBlobs, *this);
	}
}


inline void CVisBlobTreeInternal::EliminateNoiseBlobs(
		unsigned long ulArea)
{
	EliminateNoiseBlobs(0, 0, ulArea);
}


inline void CVisBlobTreeInternal::RemoveNoiseBlob(
		CVisBlobInternal *pblob, bool fMergeIntoParent)
{
	assert(pblob != 0);

	CVisBlobInternal *pblobParent = pblob->Parent();
	if (pblobParent == 0)
	{
		// Remove blob from siblings.
		if (m_pblobFirst == pblob)
		{
			m_pblobFirst = pblob->NextSibling();
		}
		else
		{
			CVisBlobInternal *pblobPrev = m_pblobFirst;
			while (pblobPrev->NextSibling() != pblob)
			{
				pblobPrev = pblobPrev->NextSibling();
				assert(pblobPrev != 0);
			}
			pblobPrev->m_pblobNextSibling = pblob->NextSibling();
		}

		if (fMergeIntoParent)
		{
			// Make pblob's grandchildren top-level blobs.
			CVisBlobInternal *pblobChild = pblob->FirstChild();
			while (pblobChild != 0)
			{
				CVisBlobInternal *pblobNextChild
						= pblobChild->NextSibling();
				CVisBlobInternal *pblobGrandchild
						= pblobChild->FirstChild();
				while (pblobGrandchild != 0)
				{
					CVisBlobInternal *pblobNextGrandchild
							= pblobGrandchild->NextSibling();

					AddTopLevelBlob(pblobGrandchild);

					pblobGrandchild = pblobNextGrandchild;
				}

				// Delete each child after processing its children.
				delete pblobChild;

				pblobChild = pblobNextChild;
			}
		}

		// Delete the blob after processing its children.
		delete pblob;
	}
	else
	{
		// Remove blob from siblings and parent
		pblob->RemoveFromParentAndSiblings();

		if (fMergeIntoParent)
		{
			// Merge pblob and pblob's children into pblob's parent
			pblob->MergeIntoParentAndDelete();
		}
		else
		{
			DeleteTree(pblob);
		}
	}
}


inline void CVisBlobTreeInternal::DeleteTree(void)
{
	CVisFnOpDeleteBlob fnopdelblob;
	ApplyFnOpToBlobTreeChildFirst<CVisFnOpDeleteBlob>(fnopdelblob, *this);
}


inline void CVisBlobTreeInternal::DeleteTree(CVisBlobInternal *pblob)
{
	assert(pblob != 0);

	// First, delete the child blobs.
	// LATER:  We might want to try to remove the recursion from this method.
	CVisBlobInternal *pblobChild = pblob->FirstChild();
	while (pblobChild != 0)
	{
		CVisBlobInternal *pblobChildNext = pblobChild->NextSibling();
		DeleteTree(pblobChild);
		pblobChild = pblobChildNext;
	}

	// Then delete the current blob.
	delete pblob;
}


// Add a blob to the list of top-level blobs.
inline void CVisBlobTreeInternal::AddTopLevelBlob(
		CVisBlobInternal *pblob)
{
	assert(pblob != 0);
	assert(pblob->m_pblobNextSibling == 0);
	assert(pblob->IsForeground() != 0);

	pblob->m_pblobNextSibling = m_pblobFirst;
	pblob->m_pblobParent = 0;
	m_pblobFirst = pblob;
}



inline void CVisBlobTreeInternal::MakeTreeFromPixRunList(
		const CVisPixRunList& refpixrunlist, bool fEightConnected)
{
	// Strips are only used with single-band images.
	assert(refpixrunlist.ShapeImage().NBands() == 1);

	assert(refpixrunlist.ShapeImage().top
			< refpixrunlist.ShapeImage().bottom);

	CVisFBBRI fbbri(refpixrunlist);

	if (fEightConnected)
	{
		for (fbbri.Reset(); fbbri.FMore(); fbbri.NextRow())
		{
			// Find the blobs in the current row.
			Find8ConnectedBlobsInRow(fbbri);
		}
	}
	else
	{
		for (fbbri.Reset(); fbbri.FMore(); fbbri.NextRow())
		{
			// Find the blobs in the current row.
			Find4ConnectedBlobsInRow(fbbri);
		}
	}
}


inline CVisBlobInternal *CVisBlobTreeInternal::MergeBlobs(
		CVisBlobInternal **rgpblobPrev, int iRPrev, int iRPrevLast,
		CVisBlobInternal **rgpblobCur, int iRCur)
{
	// LATER:  Add debug counts of number of times each case entered.
	assert(rgpblobPrev != 0);
	assert(iRPrev > 0);
	assert(iRPrev <= iRPrevLast);
	assert(rgpblobCur != 0);
	assert(iRCur > 0);

	CVisBlobInternal *pblobMerged;

	CVisBlobInternal *pblobPrev = rgpblobPrev[iRPrev];
	CVisBlobInternal *pblobCur = rgpblobCur[iRCur];

	if (pblobPrev == pblobCur)
	{
		// Don't really need to merge the blobs.
		pblobMerged = pblobCur;
	}
	else
	{
		// We'll need to merge the blobs and replace one with the other.
		CVisBlobInternal *pblobReplaced;

		if (pblobPrev == 0)
		{
			// Use Prev, remove Cur.
			pblobMerged = pblobPrev;
			pblobReplaced = pblobCur;

			// Add children of pblobReplaced to top-level list
			// of blobs and remove children from pblobReplaced.
			if (pblobReplaced->FirstChild() != 0)
			{
				CVisBlobInternal *pblobLastChild
						= pblobReplaced->FirstChild();
				while (pblobLastChild->NextSibling() != 0)
				{
					pblobLastChild->m_pblobParent = 0;
					pblobLastChild = pblobLastChild->NextSibling();
				}
				pblobLastChild->m_pblobParent = 0;
				pblobLastChild->m_pblobNextSibling = m_pblobFirst;
				m_pblobFirst = pblobReplaced->FirstChild();
			}

			pblobReplaced->RemoveFromParentAndSiblings();
		}
		else if (pblobCur == 0)
		{
			// Use Cur, remove Prev.
			pblobMerged = pblobCur;
			pblobReplaced = pblobPrev;

			// Add children of pblobReplaced to top-level list
			// of blobs and remove children from pblobReplaced.
			if (pblobReplaced->FirstChild() != 0)
			{
				CVisBlobInternal *pblobLastChild
						= pblobReplaced->FirstChild();
				while (pblobLastChild->NextSibling() != 0)
				{
					pblobLastChild->m_pblobParent = 0;
					pblobLastChild = pblobLastChild->NextSibling();
				}
				pblobLastChild->m_pblobParent = 0;
				pblobLastChild->m_pblobNextSibling = m_pblobFirst;
				m_pblobFirst = pblobReplaced->FirstChild();
			}

			pblobReplaced->RemoveFromParentAndSiblings();
		}
		else 
		{
			// Need to check the parents of the blobs.  Either they'll
			// both have the same parent or one blob will be the parent
			// of the other blob's parent.
			CVisBlobInternal *pblobParentPrev = pblobPrev->Parent();
			CVisBlobInternal *pblobParentCur = pblobCur->Parent();

			if (pblobParentPrev == pblobParentCur)
			{
				// Need to determine which blob preceeds the other in
				// in the list of blobs.
				assert(pblobPrev != pblobCur);

				CVisBlobInternal *pblobPreceeding = pblobPrev;
				while ((pblobPreceeding != 0)
						&& (pblobPreceeding->NextSibling() != pblobCur))
				{
					pblobPreceeding = pblobPreceeding->NextSibling();
				}
				
				if (pblobPreceeding == 0)
				{
					// Cur preceeds Prev, so we'll want to merge Prev into Cur.
					pblobPreceeding = pblobCur;
					while (pblobPreceeding->NextSibling() != pblobPrev)
					{
						pblobPreceeding = pblobPreceeding->NextSibling();
						assert(pblobPreceeding != 0);
					}

					pblobMerged = pblobCur;
					pblobReplaced = pblobPrev;
				}
				else
				{
					// Prev preceeds Cur, so we'll want to merge Cur into Prev.
					pblobMerged = pblobPrev;
					pblobReplaced = pblobCur;
				}

				pblobReplaced->MergeIntoSibling(pblobMerged);
				pblobPreceeding->m_pblobNextSibling
						= pblobReplaced->m_pblobNextSibling;

				// This is needed.
				pblobReplaced->m_pblobNextSibling = 0;
			}
			else if ((pblobParentCur == 0)
					|| ((pblobParentPrev != 0)
						&& (pblobParentPrev->Parent() == pblobCur)))
			{
				// Use Cur, merge Prev into Cur.
				assert(pblobParentPrev->Parent() == pblobCur);
				pblobMerged = pblobCur;
				pblobReplaced = pblobPrev;
				pblobReplaced->MergeIntoGrandparent(pblobMerged);
			}
			else
			{
				assert(pblobParentCur != 0);
				assert(pblobParentCur->Parent() == pblobPrev);

				// Use Prev, merge Cur into Prev.
				pblobMerged = pblobPrev;
				pblobReplaced = pblobCur;
				pblobReplaced->MergeIntoGrandparent(pblobMerged);
			}
		}

		// Delete the blob that we're replacing.
		delete pblobReplaced;

		// Go through the lists of blobs changing the replaced blob
		// to the one that we used.
		int i;

		rgpblobPrev[iRPrev] = pblobMerged;
		for (i = iRPrev + 1; i <= iRPrevLast; ++i)
		{
			if (rgpblobPrev[i] == pblobReplaced)
				rgpblobPrev[i] = pblobMerged;
		}

		rgpblobCur[iRCur] = pblobMerged;
		for (i = 1; i < iRCur; ++i)
		{
			if (rgpblobCur[i] == pblobReplaced)
				rgpblobCur[i] = pblobMerged;
		}
	}

	return pblobMerged;
}


// Method to find components in a row of an image.
// Note:  rgpblobPrev and rgpblobCur are not const.  Entries in rgpblobPrev
// may be modified if blobs are merged.
// LATER:  Want derived blob types with different UpdatePixelSums implementations.
// Pass the type to use in as a template to this class.  (First we should try to
// speed up the UpdatePixelSums calculations, but we'll need the derived classes
// if there's still a performance cost.)
inline void CVisBlobTreeInternal::Find4ConnectedBlobsInRow(const CVisFBBRI& fbbri)
{
	// Get information from the CVisFBBRI object.  (Use local variables to ensure
	// fast access to the data.
	int y = fbbri.YCur();
	int iRPrevLast = fbbri.IRPrevLast();
	const int *rgxPrev = fbbri.RgxPrev();
	const int *rgxCur = fbbri.RgxCur();
	CVisBlobInternal **rgpblobPrev = fbbri.RgpblobPrev();
	CVisBlobInternal **rgpblobCur = fbbri.RgpblobCur();

	// LATER:  Add debug counts of number of times each case entered.
	assert(rgxPrev != 0);
	assert(rgpblobPrev != 0);
	assert(iRPrevLast > 0);
	assert(rgxCur != 0);
	assert(rgpblobCur != 0);

	// Rows must start and end with (possibly zero-length) runs of
	// background pixels, so iRPrevLast should be odd.
	assert((iRPrevLast & 1) == 1);

	// Initialize iRPrev and xRPrev so that xLPrev and xRPrev will be set
	// properly when they are advanced at the beginning of the loop.
	int xLPrev;
	int iRPrev = 0;
	int xRPrev = rgxPrev[0];

	// Initialize iRCur, xLCur, and xRCur.
	int iRCur = 1;
	int xLCur = rgxCur[0];
	int xRCur = rgxCur[1];
	bool fForegroundCur = false; // Could use fSameStripType with a small change.
	bool fFoundBlobCur = false;

	// Handle the special case of an initial 0-length border strip.
	if (xLCur == xRCur)
	{
		// Don't need to change fFoundBlobCur, but do need to set pblobCur
		// correctly.
		assert(!fFoundBlobCur);
		rgpblobCur[iRCur] = 0;

		// Advance xLCur and xRCur
		xLCur = xRCur;
		xRCur = rgxCur[++iRCur];
		fForegroundCur = !fForegroundCur;
	}

	// Note:  Need separate sections for forground and background because
	// 4-connected forground is 8-connected background.
	for ( ; ; )
	{
		// Advance xLPrev and xRPrev.
		xLPrev = xRPrev;
		xRPrev = rgxPrev[++iRPrev];

		assert(xLCur <= xLPrev);
		assert(xRCur >= xLPrev);

		// Start the loop with a background run on the row above.
		if (!fForegroundCur)
		{
			// Cur and Prev are both background strips.
			// Background is 8-connected.
			// Set blob for cur strip to blob of prev strip.
			// (May need to merge blobs here.)
			CVisBlobInternal *pblob;

			if (fFoundBlobCur)
			{
				pblob = MergeBlobs(rgpblobPrev, iRPrev, iRPrevLast,
						rgpblobCur, iRCur);
			}
			else
			{
				pblob = rgpblobPrev[iRPrev];
				rgpblobCur[iRCur] = pblob;
				fFoundBlobCur = true;
				
				if (pblob != 0)
					pblob->AddStrip(xLCur, xRCur, y, fbbri);
			}
		}
		else if ((xLCur == xLPrev) && (xRCur <= xRPrev))
		{
			// Cur is foreground, Prev is background.
			// Background is 8-connected, foreground is 4-connected.
			// New blob for the current strip.
			assert(!fFoundBlobCur);
			CVisBlobInternal *pblob = new CVisBlobInternal(
					true, rgpblobPrev[iRPrev], xLCur, xRCur, y, fbbri);
			if (rgpblobPrev[iRPrev] == 0)
				AddTopLevelBlob(pblob);
			rgpblobCur[iRCur] = pblob;
			fFoundBlobCur = true;
		}

		// Continue processing runs that lie under the background run above.
		while (xRCur <= xRPrev)
		{
			// Advance xLCur and xRCur
			assert(fFoundBlobCur);
			xLCur = xRCur;
			xRCur = rgxCur[++iRCur];
			fForegroundCur = !fForegroundCur;
			fFoundBlobCur = false;

			assert(xLCur >= xLPrev);
			assert(xLCur <= xRPrev);
			assert(xRCur >= xLCur);

			if (!fForegroundCur)
			{
				// Cur and Prev are both background strips.
				// Background is 8-connected.
				// Set blob for cur strip to blob of prev strip.
				// (May need to merge blobs here.)
				CVisBlobInternal *pblob;

				if (fFoundBlobCur)
				{
					pblob = MergeBlobs(rgpblobPrev, iRPrev, iRPrevLast,
							rgpblobCur, iRCur);
				}
				else
				{
					pblob = rgpblobPrev[iRPrev];
					rgpblobCur[iRCur] = pblob;
					fFoundBlobCur = true;
					
					if (pblob != 0)
						pblob->AddStrip(xLCur, xRCur, y, fbbri);
				}
			}
			else if (xRCur <= xRPrev)
			{
				// Cur is foreground, Prev is background.
				// Background is 8-connected, foreground is 4-connected.
				// New blob for the current strip.
				assert(!fFoundBlobCur);
				CVisBlobInternal *pblob
						= new CVisBlobInternal(true,
								rgpblobPrev[iRPrev], xLCur, xRCur, y, fbbri);
				if (rgpblobPrev[iRPrev] == 0)
					AddTopLevelBlob(pblob);
				rgpblobCur[iRCur] = pblob;
				fFoundBlobCur = true;
			}
		}

		if (iRPrev < iRPrevLast)
		{
			// Advance xLPrev and xRPrev.
			xLPrev = xRPrev;
			xRPrev = rgxPrev[++iRPrev];

			assert(xLCur <= xLPrev);
			assert(xRCur > xLPrev);

			// Continue with the a foreground run on the row above.
			if (fForegroundCur)
			{
				// Cur and Prev are both foreground strips.
				// Foreground is 4-connected.
				// Set blob for cur strip to blob of prev strip.
				// (May need to merge blobs here.)
				CVisBlobInternal *pblob;

				if (fFoundBlobCur)
				{
					pblob = MergeBlobs(rgpblobPrev, iRPrev, iRPrevLast,
							rgpblobCur, iRCur);
				}
				else
				{
					pblob = rgpblobPrev[iRPrev];
					rgpblobCur[iRCur] = pblob;
					fFoundBlobCur = true;
					
					assert(pblob != 0);  // Since Cur is foreground.
					pblob->AddStrip(xLCur, xRCur, y, fbbri);
				}
			}
			// else if ((xLCur > xLPrev) && (xRCur < xRPrev)) can't happen.

			// Continue processing runs that lie under the foreground run above.
			while (xRCur < xRPrev)
			{
				// Advance xLCur and xRCur
				assert(fFoundBlobCur);
				xLCur = xRCur;
				xRCur = rgxCur[++iRCur];
				fForegroundCur = !fForegroundCur;
				fFoundBlobCur = false;

				assert(xLCur > xLPrev);
				assert(xLCur < xRPrev);
				assert(xRCur > xLCur);

				if (fForegroundCur)
				{
					// Cur and Prev are both foreground strips.
					// Foreground is 4-connected.
					// Set blob for cur strip to blob of prev strip.
					// (May need to merge blobs here.)
					CVisBlobInternal *pblob;

					if (fFoundBlobCur)
					{
						pblob = MergeBlobs(rgpblobPrev, iRPrev, iRPrevLast,
								rgpblobCur, iRCur);
					}
					else
					{
						pblob = rgpblobPrev[iRPrev];
						rgpblobCur[iRCur] = pblob;
						fFoundBlobCur = true;
						
						assert(pblob != 0);  // Since Cur is foreground.
						pblob->AddStrip(xLCur, xRCur, y, fbbri);
					}
				}
				else if (xRCur < xRPrev)
				{
					// Cur is background, Prev is foreground.
					// Foreground is 4-connected.
					// New blob for the current strip.
					assert(!fFoundBlobCur);
					assert(rgpblobPrev[iRPrev] != 0);
					CVisBlobInternal *pblob
							= new CVisBlobInternal(false,
									rgpblobPrev[iRPrev], xLCur, xRCur, y, fbbri);
					rgpblobCur[iRCur] = pblob;
					fFoundBlobCur = true;
				}
			}
		}
		else
		{
			// We've reached the end of the line.
			break;
		}
	}
}



// Method to find components in a row of an image.
// Note:  rgpblobPrev and rgpblobCur are not const.  Entries in rgpblobPrev
// may be modified if blobs are merged.
// LATER:  This method should be reexamined.  It was converted from the
// method that looks for 4-connected foreground regions.  During the
// conversion, some special cases for image borders were added.  Perhaps
// this method can be changed to have fewer special cases.  Also, this
// method should be tested on more images to make sure that no cases were
// missed.  (It has been verified on a couple of dozen test images, so
// it does seem to work correctly.)
inline void CVisBlobTreeInternal::Find8ConnectedBlobsInRow(const CVisFBBRI& fbbri)
{
	// Get information from the CVisFBBRI object.  (Use local variables to ensure
	// fast access to the data.
	int y = fbbri.YCur();
	int iRPrevLast = fbbri.IRPrevLast();
	const int *rgxPrev = fbbri.RgxPrev();
	const int *rgxCur = fbbri.RgxCur();
	CVisBlobInternal **rgpblobPrev = fbbri.RgpblobPrev();
	CVisBlobInternal **rgpblobCur = fbbri.RgpblobCur();

	// LATER:  Add debug counts of number of times each case entered.
	assert(rgxPrev != 0);
	assert(rgpblobPrev != 0);
	assert(iRPrevLast > 0);
	assert(rgxCur != 0);
	assert(rgpblobCur != 0);

	// Rows must start and end with (possibly zero-length) runs of
	// background pixels, so iRPrevLast should be odd.
	assert((iRPrevLast & 1) == 1);

	// Initialize iRPrev and xRPrev so that xLPrev and xRPrev will be set
	// properly when they are advanced at the beginning of the loop.
	int xLPrev;
	int iRPrev = 0;
	int xRPrev = rgxPrev[0];

	// Initialize iRCur, xLCur, and xRCur.
	int iRCur = 1;
	int xLCur = rgxCur[0];
	int xRCur = rgxCur[1];
	bool fForegroundCur = false; // Could use fSameStripType with a small change.
	bool fFoundBlobCur = false;

	// Remember the end of the row, since we'll need it in a loop below.
	int xRLast = rgxPrev[iRPrevLast];

	// Handle the special case of an initial 0-length border strip.
	if (xLCur == xRCur)
	{
		// Don't need to change fFoundBlobCur, but do need to set pblobCur
		// correctly.
		assert(!fFoundBlobCur);
		rgpblobCur[iRCur] = 0;

		// Advance xLCur and xRCur
		xLCur = xRCur;
		xRCur = rgxCur[++iRCur];
		fForegroundCur = !fForegroundCur;
	}

	// Note:  Need separate sections for forground and background because
	// 4-connected forground is 8-connected background.
	for ( ; ; )
	{
		// Advance xLPrev and xRPrev.
		xLPrev = xRPrev;
		xRPrev = rgxPrev[++iRPrev];

		assert(xLCur <= xLPrev);
		assert(xRCur >= xLPrev);
		assert(xRCur <= xRLast);

		// Start the loop with a background run on the row above.
		if (!fForegroundCur)
		{
			// Cur and Prev are both background strips.
			// Background is 4-connected.
			// Set blob for cur strip to blob of prev strip.
			// (May need to merge blobs here.)
			CVisBlobInternal *pblob;

			if (fFoundBlobCur)
			{
				pblob = MergeBlobs(rgpblobPrev, iRPrev, iRPrevLast,
						rgpblobCur, iRCur);
			}
			else
			{
				pblob = rgpblobPrev[iRPrev];
				rgpblobCur[iRCur] = pblob;
				fFoundBlobCur = true;
				
				if (pblob != 0)
					pblob->AddStrip(xLCur, xRCur, y, fbbri);
			}
		}
		else if ((iRPrev == 1) && (xRCur < xRPrev))
		{
			// This is a special case where the forground strip is adjacent to
			// the border of the image.
			// Cur is foreground, Prev is background.
			// Background is 4-connected, foreground is 8-connected.
			// New blob for the current strip.
			assert(!fFoundBlobCur);
			CVisBlobInternal *pblob = new CVisBlobInternal(
					true, rgpblobPrev[iRPrev], xLCur, xRCur, y, fbbri);
			if (rgpblobPrev[iRPrev] == 0)
				AddTopLevelBlob(pblob);
			rgpblobCur[iRCur] = pblob;
			fFoundBlobCur = true;
		}

		// Continue processing runs that lie under the background run above.
		while (xRCur < xRPrev)
		{
			// Advance xLCur and xRCur
			assert(fFoundBlobCur);
			xLCur = xRCur;
			xRCur = rgxCur[++iRCur];
			fForegroundCur = !fForegroundCur;
			fFoundBlobCur = false;

			assert(xLCur >= xLPrev);
			assert(xLCur < xRPrev);
			assert(xRCur >= xLCur);
			assert(xRCur <= xRLast);

			if (!fForegroundCur)
			{
				// Cur and Prev are both background strips.
				// Background is 4-connected.
				// Set blob for cur strip to blob of prev strip.
				// (May need to merge blobs here.)
				CVisBlobInternal *pblob;

				if (fFoundBlobCur)
				{
					pblob = MergeBlobs(rgpblobPrev, iRPrev, iRPrevLast,
							rgpblobCur, iRCur);
				}
				else
				{
					pblob = rgpblobPrev[iRPrev];
					rgpblobCur[iRCur] = pblob;
					fFoundBlobCur = true;
					
					if (pblob != 0)
						pblob->AddStrip(xLCur, xRCur, y, fbbri);
				}
			}
			else if ((xLCur > xLPrev)
					&& ((xRCur < xRPrev)
						|| ((iRPrev == iRPrevLast)
							&& (xRCur == xRPrev))))
			{
				// Cur is foreground, Prev is background.
				// Background is 4-connected, foreground is 8-connected.
				// New blob for the current strip.
				assert(!fFoundBlobCur);
				CVisBlobInternal *pblob
						= new CVisBlobInternal(true,
								rgpblobPrev[iRPrev], xLCur, xRCur, y, fbbri);
				if (rgpblobPrev[iRPrev] == 0)
					AddTopLevelBlob(pblob);
				rgpblobCur[iRCur] = pblob;
				fFoundBlobCur = true;
			}
		}

		if (iRPrev < iRPrevLast)
		{
			// Advance xLPrev and xRPrev.
			xLPrev = xRPrev;
			xRPrev = rgxPrev[++iRPrev];

			// If iRPrev == 2, we could have xLCur == xLPrev.
			assert((iRPrev == 2) || (xLCur < xLPrev));
			assert(xRCur >= xLPrev);
			assert(xRCur <= xRLast);

			// Continue with the a foreground run on the row above.
			if (fForegroundCur)
			{
				// Cur and Prev are both foreground strips.
				// Foreground is 8-connected.
				// Set blob for cur strip to blob of prev strip.
				// (May need to merge blobs here.)
				CVisBlobInternal *pblob;

				if (fFoundBlobCur)
				{
					pblob = MergeBlobs(rgpblobPrev, iRPrev, iRPrevLast,
							rgpblobCur, iRCur);
				}
				else
				{
					pblob = rgpblobPrev[iRPrev];
					rgpblobCur[iRCur] = pblob;
					fFoundBlobCur = true;
					
					assert(pblob != 0);  // Since Cur is foreground.
					pblob->AddStrip(xLCur, xRCur, y, fbbri);
				}
			}
			else if ((iRPrev == 2) && (xLCur >= xLPrev) && (xRCur <= xRPrev))
			{
				// Cur is background, Prev is foreground.
				// Foreground is 8-connected.
				assert((!fFoundBlobCur) || (rgpblobCur[iRCur] == 0));
				if (!fFoundBlobCur)
				{
					// New blob for the current strip.
					assert(rgpblobPrev[iRPrev] != 0);
					CVisBlobInternal *pblob
							= new CVisBlobInternal(false,
									rgpblobPrev[iRPrev], xLCur, xRCur, y, fbbri);
					rgpblobCur[iRCur] = pblob;
					fFoundBlobCur = true;
				}
			}

			// Continue processing runs that lie under the foreground run above.
			while (xRCur <= xRPrev)
			{
				// Advance xLCur and xRCur
				assert(fFoundBlobCur);
				xLCur = xRCur;
				xRCur = rgxCur[++iRCur];
				fForegroundCur = !fForegroundCur;
				fFoundBlobCur = false;

				// Another special case:  We terminate the list of x values
				// on the current line by appending xLim.  We may be looking
				// at a stip like that here.  If so, we're done processing
				// all the strips on the current line, so it's safe to return.
				if (xRCur > xRLast)
					return;

				assert(xLCur >= xLPrev);
				assert(xLCur <= xRPrev);
				assert(xRCur >= xLCur);

				if (fForegroundCur)
				{
					// Cur and Prev are both foreground strips.
					// Foreground is 8-connected.
					// Set blob for cur strip to blob of prev strip.
					// (May need to merge blobs here.)
					CVisBlobInternal *pblob;

					if (fFoundBlobCur)
					{
						pblob = MergeBlobs(rgpblobPrev, iRPrev, iRPrevLast,
								rgpblobCur, iRCur);
					}
					else
					{
						pblob = rgpblobPrev[iRPrev];
						rgpblobCur[iRCur] = pblob;
						fFoundBlobCur = true;
						
						assert(pblob != 0);  // Since Cur is foreground.
						pblob->AddStrip(xLCur, xRCur, y, fbbri);
					}
				}
				else if (xLCur == xRCur)
				{
					// Special case:  A zero-length background strip on the
					// side of the image.
					rgpblobCur[iRCur] = 0;
					fFoundBlobCur = true;
					break;
				}
				else if (xRCur <= xRPrev)
				{
					// Cur is background, Prev is foreground.
					// Foreground is 8-connected.
					// New blob for the current strip.
					assert(!fFoundBlobCur);
					assert(rgpblobPrev[iRPrev] != 0);
					CVisBlobInternal *pblob
							= new CVisBlobInternal(false,
									rgpblobPrev[iRPrev], xLCur, xRCur, y, fbbri);
					rgpblobCur[iRCur] = pblob;
					fFoundBlobCur = true;
				}
			}
		}
		else
		{
			// Special case of a blob foreground strip that borders the right
			// edge of the image.
			assert((fFoundBlobCur)
					|| ((fForegroundCur) && (xLCur >= xLPrev) && (xRCur == xRPrev)));
			if ((!fFoundBlobCur) && (fForegroundCur))
			{
				// Cur is foreground, Prev is background.
				// Background is 4-connected, foreground is 8-connected.
				// New blob for the current strip.
				assert(!fFoundBlobCur);
				assert(xRCur <= xRLast);
				CVisBlobInternal *pblob
						= new CVisBlobInternal(true,
								rgpblobPrev[iRPrev], xLCur, xRCur, y, fbbri);
				if (rgpblobPrev[iRPrev] == 0)
					AddTopLevelBlob(pblob);
				rgpblobCur[iRCur] = pblob;
				fFoundBlobCur = true;
			}

			// Don't for get about zero-length background strips on the
			// border of the image.
			if ((fForegroundCur) && (xRCur == xRPrev))
			{
				rgpblobCur[iRCur + 1] = 0;
			}

			// We've reached the end of the line.
			break;
		}
	}
}


#ifdef _DEBUG
#undef new
#endif // _DEBUG
