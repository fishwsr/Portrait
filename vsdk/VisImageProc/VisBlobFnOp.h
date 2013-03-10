// VisBlobFnOp.h
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000




// Function operator to delete blobs
class CVisFnOpDeleteBlob
{
public:
	void operator()(CVisBlobInternal *pblob)
	{
		delete pblob;
	}
};


// Function operator to add strips from foreground blobs to an image.
class CVisFnOpFindFgArea
{
public:
	CVisFnOpFindFgArea(void)
	  : m_ulArea(0)
	{
	}

	void operator()(CVisBlobInternal *pblob)
	{
		assert(pblob != 0);
		if (pblob->IsForeground())
			{
				m_ulArea += pblob->Area();
			}
	}

	unsigned long Area(void) const
		{ return m_ulArea; }

private:
	unsigned long m_ulArea;
};


// Function operator to set pixels in image.
template<class TPixel>
class CVisFnOpSetFgStripsInImage
{
public:
	CVisFnOpSetFgStripsInImage(CVisImage<TPixel>& refimage,
			const TPixel& pixelSetValue)
	  : m_refimage(refimage),
		m_pixelSetValue(pixelSetValue)
	{
	}

	void operator()(CVisBlobInternal *pblob)
	{
		assert(pblob != 0);
		if (pblob->IsForeground())
			{
				CVisStripList striplist(m_refimage.Shape(),
						pblob->StripEnum());
				SetImagePixelsFromStripList<TPixel>(
						m_refimage, m_pixelSetValue, striplist);
			}
	}

private:
	CVisImage<TPixel>& m_refimage;
	TPixel m_pixelSetValue;
};


// Function operator to add strips from foreground blobs to an image.
// Function operator to label blobs.
class CVisFnOpSetBlobLabels
{
public:
	CVisFnOpSetBlobLabels(void)
	  : m_cBlob(0)
	{
	}

	void operator()(CVisBlobInternal *pblob)
	{
		pblob->SetILabel(++m_cBlob);
	}

	unsigned int CBlob(void) const
		{ return m_cBlob; }

private:
	unsigned int m_cBlob;
};


template<class TPixel>
class CVisFnOpXorFgStripsInImage
{
public:
	CVisFnOpXorFgStripsInImage(CVisImage<TPixel>& refimage,
			const TPixel& pixelXorValue)
	  : m_refimage(refimage),
		m_pixelXorValue(pixelXorValue)
	{
	}

	void operator()(CVisBlobInternal *pblob)
	{
		assert(pblob != 0);
		if (pblob->IsForeground())
			{
				CVisStripList striplist(m_refimage.Shape(),
						pblob->StripEnum());
				XorImagePixelsInStripList<TPixel>(
						m_refimage, m_pixelXorValue, striplist);
			}
	}

private:
	CVisImage<TPixel>& m_refimage;
	TPixel m_pixelXorValue;
};


template<class TPixel>
class CVisFnOpAddFgStripsToImage
{
public:
	CVisFnOpAddFgStripsToImage(CVisImage<TPixel>& refimage,
			const TPixel& pixelAddValue)
	  : m_refimage(refimage),
		m_pixelAddValue(pixelAddValue)
	{
	}

	void operator()(CVisBlobInternal *pblob)
	{
		assert(pblob != 0);
		if (pblob->IsForeground())
			{
				CVisStripList striplist(m_refimage.Shape(),
						pblob->StripEnum());
				AddImagePixelsToStripList<TPixel>(
						m_refimage, m_pixelAddValue, striplist);
			}
	}

private:
	CVisImage<TPixel>& m_refimage;
	TPixel m_pixelAddValue;
};


// Function operator to label blobs in an image.  Note that this
// function operator does not set label information in the blobs.
template<class TPixel>
class CVisFnOpLabelBlobs
{
public:
	CVisFnOpLabelBlobs(CVisImage<TPixel>& refimage,
			TPixel pixelFirstLabel, TPixel pixelLabelStep)
	  : m_refimage(refimage),
		m_pixelLabelCur(pixelFirstLabel),
		m_pixelLabelStep(pixelLabelStep)
	{
	}

	void operator()(CVisBlobInternal *pblob)
	{
		assert(pblob != 0);
		CVisStripList striplist(m_refimage.Shape(),
				pblob->StripEnum());
		SetImagePixelsFromStripList<TPixel>(
				m_refimage, m_pixelLabelCur, striplist);
		m_pixelLabelCur += m_pixelLabelStep;
	}

	TPixel NextLabel(void) const
	{
		return m_pixelLabelCur;
	}

private:
	CVisImage<TPixel>& m_refimage;
	TPixel m_pixelLabelCur;
	TPixel m_pixelLabelStep;
};


// Function operator to label blobs in an image.  Note that this
// function operator does set label information in the blobs.
template<class TPixel>
class CVisFnOpLabelBlobsEx : public CVisFnOpLabelBlobs<TPixel>
{
public:
	CVisFnOpLabelBlobsEx(CVisImage<TPixel>& refimage,
			TPixel pixelFirstLabel, TPixel pixelLabelStep)
	  : CVisFnOpLabelBlobs<TPixel>(refimage,
			pixelFirstLabel, pixelLabelStep),
		m_pixelLabelFirst(pixelFirstLabel),
		m_cBlob(0),
		m_pbloblistbuilder()
	{
	}

	void operator()(CVisBlobInternal *pblob)
	{
		m_pbloblistbuilder.AddObj(pblob);
		pblob->SetILabel(++m_cBlob);
		CVisFnOpLabelBlobs<TPixel>::operator()(pblob);
	}

	TPixel FirstLabel(void) const
	{
		return m_pixelLabelFirst;
	}

	CVisBlobInternalListEnum PblobEnum(void) const
	{
		return m_pbloblistbuilder;
	}

	unsigned int CBlob(void) const
		{ return m_cBlob; }

private:
	TPixel m_pixelLabelFirst;

	unsigned int m_cBlob;

	// List of labeled blobs.
	CVisBlobInternalListBuilder m_pbloblistbuilder;
};


// Function operator to eliminate noise blobs
class CVisFnOpElimNoiseBlobs
{
public:
	// Remove noise blobs whose size is less than the size specified
	// or whose area is less than the area specified.
	CVisFnOpElimNoiseBlobs(CVisBlobTreeInternal& refblobtree,
			SIZE size, unsigned long ulArea = 0)
	  : m_refblobtree(refblobtree),
		m_size(size),
		m_ulArea(ulArea)
	{
	}

	CVisFnOpElimNoiseBlobs(CVisBlobTreeInternal& refblobtree,
			int dx, int dy, unsigned long ulArea = 0)
	  : m_refblobtree(refblobtree),
		m_size(dx, dy),
		m_ulArea(ulArea)
	{
	}

	CVisFnOpElimNoiseBlobs(CVisBlobTreeInternal& refblobtree,
			unsigned long ulArea)
	  : m_refblobtree(refblobtree),
		m_size(0, 0),
		m_ulArea(ulArea)
	{
	}

	void operator()(CVisBlobInternal *pblob)
	{
		if ((pblob->Area() < m_ulArea)
				|| (pblob->RectBounding().Width() < m_size.cx)
				|| (pblob->RectBounding().Height() < m_size.cy))
		{
			// Merge blob into parent.
			m_refblobtree.RemoveNoiseBlob(pblob);
		}
	}

private:
	CVisBlobTreeInternal& m_refblobtree;
	CVisSize m_size;
	unsigned long m_ulArea;
};

