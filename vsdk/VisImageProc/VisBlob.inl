// VisBlob.inl
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG





inline CVisBlob::CVisBlob(void)
  : m_pblobtreeinternal(0),
	m_pblobinternal(0)
{
}

inline CVisBlob::CVisBlob(CVisBlobTreeInternal *pblobtreeinternal,
		CVisBlobInternal *pblobinternal)
  : m_pblobtreeinternal(pblobtreeinternal),
	m_pblobinternal(pblobinternal)
{
	if (m_pblobtreeinternal != 0)
		m_pblobtreeinternal->AddRef();
}

inline CVisBlob::CVisBlob(CVisBlob& refblob)
  : m_pblobtreeinternal(refblob.m_pblobtreeinternal),
	m_pblobinternal(refblob.m_pblobinternal)
{
	if (m_pblobtreeinternal != 0)
		m_pblobtreeinternal->AddRef();

	// These asserts might help to catch synchronization problems.
	assert(m_pblobtreeinternal == refblob.m_pblobtreeinternal);
	assert(m_pblobinternal == refblob.m_pblobinternal);
}

inline CVisBlob& CVisBlob::operator=(CVisBlob& refblob)
{
	if (m_pblobtreeinternal != 0)
		m_pblobtreeinternal->Release();

	m_pblobtreeinternal = refblob.m_pblobtreeinternal;
	m_pblobinternal = refblob.m_pblobinternal;

	if (m_pblobtreeinternal != 0)
		m_pblobtreeinternal->AddRef();

	// These asserts might help to catch synchronization problems.
	assert(m_pblobtreeinternal == refblob.m_pblobtreeinternal);
	assert(m_pblobinternal == refblob.m_pblobinternal);

	return *this;
}

inline CVisBlob::~CVisBlob(void)
{
	if (m_pblobtreeinternal != 0)
		m_pblobtreeinternal->Release();

	m_pblobtreeinternal = 0;
	m_pblobinternal = 0;
}



inline bool CVisBlob::IsValid(void) const
{
	return ((m_pblobtreeinternal != 0) && (m_pblobinternal != 0));
}


inline bool CVisBlob::IsForeground(void) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return m_pblobinternal->IsForeground();
}


inline CVisBlob::iterator CVisBlob::begin(void)
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return m_pblobinternal->StripEnum().begin();
}


inline CVisBlob::iterator CVisBlob::end(void)
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return m_pblobinternal->StripEnum().end();
}


inline bool CVisBlob::HasParent(void) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return (m_pblobinternal->Parent() != 0);
}



inline bool CVisBlob::HasNextSibling(void) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return (m_pblobinternal->NextSibling() != 0);
}



inline bool CVisBlob::HasPrevSibling(void) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	bool fRet;

	if (m_pblobinternal->Parent() != 0)
	{
		fRet = (m_pblobinternal->Parent()->FirstChild() != m_pblobinternal);
	}
	else
	{
		fRet = (m_pblobtreeinternal->PBlobFirst() != m_pblobinternal);
	}

	return fRet;
}



inline bool CVisBlob::HasChildren(void) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return (m_pblobinternal->FirstChild() != 0);
}


inline CVisBlob CVisBlob::Parent(void)
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return CVisBlob(m_pblobtreeinternal, m_pblobinternal->Parent());
}

inline CVisBlob CVisBlob::NextSibling(void)
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return CVisBlob(m_pblobtreeinternal, m_pblobinternal->NextSibling());
}

inline CVisBlob CVisBlob::FirstChild(void)
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return CVisBlob(m_pblobtreeinternal, m_pblobinternal->FirstChild());
}


inline CVisBlob CVisBlob::PrevSibling(void)
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	CVisBlobInternal *pblobinternalRet;

	if (m_pblobinternal->Parent() == 0)
	{
		CVisBlobInternal *pblobinternalT = m_pblobtreeinternal->PBlobFirst();
		if (m_pblobinternal == pblobinternalT)
		{
			pblobinternalRet = 0;
		}
		else
		{
			while (pblobinternalT->NextSibling() != m_pblobinternal)
				pblobinternalT = pblobinternalT->NextSibling();
		}
	}
	else
	{
		pblobinternalRet = m_pblobinternal->PrevSibling();
	}

	return CVisBlob(m_pblobtreeinternal, pblobinternalRet);
}


inline CVisBlob CVisBlob::LastChild(void)
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return CVisBlob(m_pblobtreeinternal, m_pblobinternal->LastChild());
}


inline unsigned int CVisBlob::ILabel(void) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return m_pblobinternal->ILabel();
}


inline void CVisBlob::SetILabel(unsigned int iLabel)
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	m_pblobinternal->SetILabel(iLabel);
}


inline CVisBlobTree CVisBlob::BlobTree(void)
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return CVisBlobTree(m_pblobtreeinternal);
}


inline void CVisBlob::RemoveFromTree(bool fMergeIntoParent)
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	m_pblobtreeinternal->RemoveNoiseBlob(m_pblobinternal, fMergeIntoParent);

	m_pblobinternal = 0;
}


inline unsigned int CVisBlob::DepthInTree(void) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return m_pblobinternal->SafeDepthInTree();
}


inline int CVisBlob::Width(void) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return m_pblobinternal->Width();
}

inline int CVisBlob::Height(void) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return m_pblobinternal->Height();
}

inline const CVisRect& CVisBlob::RectBounding(void) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return m_pblobinternal->RectBounding();
}



inline unsigned long CVisBlob::Area(void) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return m_pblobinternal->Area();
}


inline double CVisBlob::DblXCentroid(void) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return m_pblobinternal->DblXCentroid();
}

inline double CVisBlob::DblYCentroid(void) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return m_pblobinternal->DblYCentroid();
}

inline LONGLONG CVisBlob::SumX(void) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return m_pblobinternal->SumX();
}

inline LONGLONG CVisBlob::SumY(void) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return m_pblobinternal->SumY();
}

inline LONGLONG CVisBlob::SumXX(void) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return m_pblobinternal->SumXX();
}

inline LONGLONG CVisBlob::SumXY(void) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return m_pblobinternal->SumXY();
}

inline LONGLONG CVisBlob::SumYY(void) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	return m_pblobinternal->SumYY();
}



inline void CVisBlob::GetPropString(int evisblobprop, CVisString& refszProp,
		bool fAppend) const
{
	assert(m_pblobtreeinternal != 0);
	assert(m_pblobinternal != 0);

	m_pblobinternal->GetPropString(evisblobprop, refszProp, fAppend);
}






inline CVisBlobTreeIterator::CVisBlobTreeIterator(
		CVisBlobTreeInternal *pblobtreeinternal,
		bool fChildrenBeforeParents, bool fAtEnd)
	: m_pblobtreeinternal(pblobtreeinternal),
	m_pblobinternal(0),
	m_fChildrenBeforeParents(fChildrenBeforeParents)
{
	if (m_pblobtreeinternal != 0)
	{
		m_pblobtreeinternal->AddRef();

		if (!fAtEnd)
			m_pblobinternal = PblobNext();  // The first blob.
	}
}


inline CVisBlobTreeIterator::CVisBlobTreeIterator(
		CVisBlobTreeIterator& refblobtreeiterator)
	: m_pblobtreeinternal(refblobtreeiterator.m_pblobtreeinternal),
	m_pblobinternal(refblobtreeiterator.m_pblobinternal),
	m_fChildrenBeforeParents(refblobtreeiterator.m_fChildrenBeforeParents)
{
	if (m_pblobtreeinternal != 0)
		m_pblobtreeinternal->AddRef();
}


inline CVisBlobTreeIterator& CVisBlobTreeIterator::operator=(
		CVisBlobTreeIterator& refblobtreeiterator)
{
	if (m_pblobtreeinternal != 0)
		m_pblobtreeinternal->Release();

	m_pblobtreeinternal = refblobtreeiterator.m_pblobtreeinternal;
	m_pblobinternal = refblobtreeiterator.m_pblobinternal;
	m_fChildrenBeforeParents = refblobtreeiterator.m_fChildrenBeforeParents;

	if (m_pblobtreeinternal != 0)
		m_pblobtreeinternal->AddRef();

	return *this;
}


inline CVisBlobTreeIterator::~CVisBlobTreeIterator(void)
{
	if (m_pblobtreeinternal != 0)
	{
		m_pblobtreeinternal->Release();
		m_pblobtreeinternal = 0;
	}
}



inline bool CVisBlobTreeIterator::IsValid(void) const
{
	return (m_pblobtreeinternal != 0);
}


inline CVisBlob CVisBlobTreeIterator::operator*(void) const
{
	assert(IsValid());

	return CVisBlob(m_pblobtreeinternal, m_pblobinternal);
}



inline CVisBlobTreeIterator& CVisBlobTreeIterator::operator++(void)
{
	assert(IsValid());

	m_pblobinternal = PblobNext();

	return *this;
}


inline CVisBlobTreeIterator& CVisBlobTreeIterator::operator--(void)
{
	assert(IsValid());

	m_pblobinternal = PblobPrev();

	return *this;
}



inline CVisBlobTreeIterator CVisBlobTreeIterator::operator++(int)
{
	assert(IsValid());

	CVisBlobTreeIterator iteratorRet = *this;

	++*this;

	return iteratorRet;
}


inline CVisBlobTreeIterator CVisBlobTreeIterator::operator--(int)
{
	assert(IsValid());

	CVisBlobTreeIterator iteratorRet = *this;

	--*this;

	return iteratorRet;
}



inline bool CVisBlobTreeIterator::operator==(
		const CVisBlobTreeIterator& refblobtreeiterator) const
{
	assert(IsValid());

	// Compare the blob pointer before comparing the blob tree pointer, since
	// the blob pointer is more likely to differ in normal use.
	return ((m_pblobinternal == refblobtreeiterator.m_pblobinternal)
			&& (m_pblobtreeinternal == refblobtreeiterator.m_pblobtreeinternal));
}


inline bool CVisBlobTreeIterator::operator!=(
		const CVisBlobTreeIterator& refblobtreeiterator) const
{
	return !(*this == refblobtreeiterator);
}



inline bool CVisBlobTreeIterator::FChildrenBeforeParents(void) const
{
	assert(IsValid());

	return m_fChildrenBeforeParents;
}


inline CVisBlobInternal *CVisBlobTreeIterator::PblobNext(void) const
{
	assert(IsValid());

	CVisBlobInternal *pblobinternalRet;
	CVisBlobInternal *pblobinternalT;

	if (m_fChildrenBeforeParents)
	{
		if (m_pblobinternal == 0)
		{
			pblobinternalRet = m_pblobtreeinternal->PBlobFirst();

			pblobinternalT = pblobinternalRet;
			while (pblobinternalT != 0)
			{
				pblobinternalRet = pblobinternalT;
				pblobinternalT = pblobinternalT->FirstChild();
			}
		}
		else
		{
			pblobinternalT = m_pblobinternal->NextSibling();
			if (pblobinternalT == 0)
			{
				pblobinternalRet = m_pblobinternal->Parent();
			}
			else
			{
				while (pblobinternalT != 0)
				{
					pblobinternalRet = pblobinternalT;
					pblobinternalT = pblobinternalT->FirstChild();
				}
			}
		}
	}
	else
	{
		if (m_pblobinternal == 0)
		{
			pblobinternalRet = m_pblobtreeinternal->PBlobFirst();
		}
		else
		{
			pblobinternalRet = m_pblobinternal->FirstChild();

			if (pblobinternalRet == 0)
			{
				pblobinternalRet = m_pblobinternal->NextSibling();

				if (pblobinternalRet == 0)
				{
					pblobinternalT = m_pblobinternal->Parent();

					while (pblobinternalT != 0)
					{
						pblobinternalRet = pblobinternalT->NextSibling();

						if (pblobinternalRet != 0)
							break;

						pblobinternalT = pblobinternalT->Parent();
					}
				}
			}
		}
	}

	return pblobinternalRet;
}


inline CVisBlobInternal *CVisBlobTreeIterator::PPrevSibling(
		CVisBlobInternal *pblobinternal,
		CVisBlobTreeInternal *pblobtreeinternal)
{
	CVisBlobInternal *pblobinternalRet;

	if (pblobinternal->Parent() == 0)
	{
		CVisBlobInternal *pblobinternalT = pblobtreeinternal->PBlobFirst();
		if (pblobinternal == pblobinternalT)
		{
			pblobinternalRet = 0;
		}
		else
		{
			while (pblobinternalT->NextSibling() != pblobinternal)
				pblobinternalT = pblobinternalT->NextSibling();
		}
	}
	else
	{
		pblobinternalRet = pblobinternal->PrevSibling();
	}

	return pblobinternalRet;
}


inline CVisBlobInternal *CVisBlobTreeIterator::PblobPrev(void) const
{
	assert(IsValid());

	CVisBlobInternal *pblobinternalRet;
	CVisBlobInternal *pblobinternalT;

	if (m_fChildrenBeforeParents)
	{
		if (m_pblobinternal == 0)
		{
			// Last top-level blob
			pblobinternalRet = m_pblobtreeinternal->PBlobFirst();

			pblobinternalT = pblobinternalRet;
			while (pblobinternalT != 0)
			{
				pblobinternalRet = pblobinternalT;
				pblobinternalT = pblobinternalT->NextSibling();
			}
		}
		else
		{
			pblobinternalRet = m_pblobinternal->LastChild();

			if (pblobinternalRet == 0)
			{
				pblobinternalRet = PPrevSibling(m_pblobinternal,
						m_pblobtreeinternal);

				pblobinternalT = m_pblobinternal;

				while (pblobinternalRet == 0)
				{
					pblobinternalT = pblobinternalT->Parent();

					if (pblobinternalT == 0)
						break;

					pblobinternalRet = PPrevSibling(pblobinternalT,
							m_pblobtreeinternal);
				}
			}
		}
	}
	else
	{
		if (m_pblobinternal == 0)
		{
			// LastChild()-> . . . LastChild()
			pblobinternalRet = m_pblobtreeinternal->PBlobFirst();

			pblobinternalT = pblobinternalRet;
			while (pblobinternalT != 0)
			{
				pblobinternalRet = pblobinternalT;
				pblobinternalT = pblobinternalT->NextSibling();
			}

			pblobinternalT = pblobinternalRet;
			while (pblobinternalT != 0)
			{
				pblobinternalRet = pblobinternalT;
				pblobinternalT = pblobinternalT->LastChild();
			}
		}
		else
		{
			if (m_pblobinternal->Parent() == 0)
			{
				pblobinternalT = m_pblobtreeinternal->PBlobFirst();
				if (m_pblobinternal == pblobinternalT)
				{
					pblobinternalRet = 0;
				}
				else
				{
					while (pblobinternalT->NextSibling() != m_pblobinternal)
					{
						pblobinternalT = pblobinternalT->NextSibling();
					}

					while (pblobinternalT != 0)
					{
						pblobinternalRet = pblobinternalT;

						pblobinternalT = pblobinternalT->LastChild();
					}
				}
			}
			else
			{
				if (m_pblobinternal == m_pblobinternal->Parent()->FirstChild())
				{
					pblobinternalRet = m_pblobinternal->Parent();
				}
				else
				{
					pblobinternalT = m_pblobinternal->PrevSibling();
					assert(pblobinternalT != 0);

					while (pblobinternalT != 0)
					{
						pblobinternalRet = pblobinternalT;

						pblobinternalT = pblobinternalT->LastChild();
					}
				}
			}
		}
	}

	return pblobinternalRet;
}





inline CVisBlobTree::CVisBlobTree(void)
  : m_pblobtreeinternal(0)
{
}

inline CVisBlobTree::CVisBlobTree(CVisBlobTreeInternal *pblobtreeinternal)
  : m_pblobtreeinternal(pblobtreeinternal)
{
	if (m_pblobtreeinternal != 0)
		m_pblobtreeinternal->AddRef();
}

inline CVisBlobTree::CVisBlobTree(CVisBlobTree& refblobtree)
  : m_pblobtreeinternal(refblobtree.m_pblobtreeinternal)
{
	if (m_pblobtreeinternal != 0)
		m_pblobtreeinternal->AddRef();

	// This assert might help to catch synchronization problems.
	assert(m_pblobtreeinternal == refblobtree.m_pblobtreeinternal);
}

inline CVisBlobTree& CVisBlobTree::operator=(CVisBlobTree& refblobtree)
{
	if (m_pblobtreeinternal != 0)
		m_pblobtreeinternal->Release();

	m_pblobtreeinternal = refblobtree.m_pblobtreeinternal;

	if (m_pblobtreeinternal != 0)
		m_pblobtreeinternal->AddRef();

	// This assert might help to catch synchronization problems.
	assert(m_pblobtreeinternal == refblobtree.m_pblobtreeinternal);

	return *this;
}

inline CVisBlobTree::~CVisBlobTree(void)
{
	if (m_pblobtreeinternal != 0)
		m_pblobtreeinternal->Release();

	m_pblobtreeinternal = 0;
}


inline bool CVisBlobTree::IsValid(void) const
{
	return (m_pblobtreeinternal != 0);
}


inline CVisBlob CVisBlobTree::BlobFirst(void)
{
	assert(m_pblobtreeinternal != 0);

	return CVisBlob(m_pblobtreeinternal, m_pblobtreeinternal->PBlobFirst());
}


inline void CVisBlobTree::EliminateNoiseBlobs(SIZE size, unsigned long ulArea)
{
	assert(m_pblobtreeinternal != 0);

	m_pblobtreeinternal->EliminateNoiseBlobs(size, ulArea);
}

inline void CVisBlobTree::EliminateNoiseBlobs(int dx, int dy, unsigned long ulArea)
{
	assert(m_pblobtreeinternal != 0);

	m_pblobtreeinternal->EliminateNoiseBlobs(dx, dy, ulArea);
}

inline void CVisBlobTree::EliminateNoiseBlobs(unsigned long ulArea)
{
	assert(m_pblobtreeinternal != 0);

	m_pblobtreeinternal->EliminateNoiseBlobs(ulArea);
}


inline void CVisBlobTree::LabelBlobs(void)
{
	assert(m_pblobtreeinternal != 0);

	m_pblobtreeinternal->LabelBlobs();
}


inline void CVisBlobTree::RemoveNoiseBlob(CVisBlob blob, bool fMergeIntoParent)
{
	assert(m_pblobtreeinternal != 0);

	m_pblobtreeinternal->RemoveNoiseBlob(blob.m_pblobinternal, fMergeIntoParent);
}



inline void CVisBlobTree::MakeTreeFromPixRunList(const CVisPixRunList& refpixrunlist,
		bool fEightConnected)
{
	if (m_pblobtreeinternal != 0)
	{
		m_pblobtreeinternal->Release();
		m_pblobtreeinternal = 0;
	}

	m_pblobtreeinternal = new CVisBlobTreeInternal();

	// UNDONE:  Throw exception if new fails.

	m_pblobtreeinternal->MakeTreeFromPixRunList(refpixrunlist, fEightConnected);
}


inline CVisBlobTree::iterator CVisBlobTree::begin(bool fChildrenBeforeParents)
{
	assert(m_pblobtreeinternal != 0);

	return CVisBlobTree::iterator(m_pblobtreeinternal, fChildrenBeforeParents,
			false);
}


inline CVisBlobTree::iterator CVisBlobTree::end(bool fChildrenBeforeParents)
{
	assert(m_pblobtreeinternal != 0);

	return CVisBlobTree::iterator(m_pblobtreeinternal, fChildrenBeforeParents,
			true);
}


inline void CVisBlobTree::clear(void)
{
	if (m_pblobtreeinternal != 0)
		m_pblobtreeinternal->Release();

	m_pblobtreeinternal = 0;
}



#ifdef _DEBUG
#undef new
#endif // _DEBUG
