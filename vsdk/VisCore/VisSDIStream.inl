// VisSDIStream.inl
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG



inline bool CVisSDIStream::UseStrictTypeMatching(void) const
{
	return m_fUseStrictTypeMatching;
}

inline void CVisSDIStream::SetUseStrictTypeMatching(bool f)
{
	m_fUseStrictTypeMatching = f;
}

inline int CVisSDIStream::ILine(void) const
{
	return m_iLine;
}


inline void CVisSDIStream::StartObjType(
		const CVisPropTypeInfoBase *pproptypeinfo)
{
	assert(pproptypeinfo != 0);
	StartObjType(pproptypeinfo->Name());
}


inline bool CVisSDIStream::IsWhitespaceCh(int wCh)
{
	// Assume that the only characters <= ' ' are whitespace characters.
	assert((wCh >= ' ') || (wCh == '\n') || (wCh == '\t'));
	return (wCh <= ' ');
}

inline void CVisSDIStream::SkipThroughEndOfField(void)
{
	SkipThroughEndCh(echFieldSep);
}

inline void CVisSDIStream::SkipThroughEndOfValue(void)
{
	SkipThroughEndCh(echValueSep);
}

inline void CVisSDIStream::StartValue(void)
{
	ValueSep();
}

inline CVisSDIStream::EState CVisSDIStream::StateCur(void) const
{
	return (CVisSDIStream::EState) m_vectorestate[m_iLevel];
}

inline void CVisSDIStream::SetStateCur(CVisSDIStream::EState estate)
{
	m_vectorestate[m_iLevel] = estate;
}

inline void CVisSDIStream::PushState(CVisSDIStream::EState estate)
{
	++m_iLevel;
	m_vectorestate.push_back(estate);
}

inline void CVisSDIStream::PopState(void)
{
	assert(m_iLevel > 0);
	-- m_iLevel;
	m_vectorestate.pop_back();
}


inline void CVisSDIStream::UseType(CVisPropTypeInfoBase *pproptypeinfo)
{
	// With input streams, we only need to make sure that we have type
	// information for top-level objects when they are read.
	// LATER:  Some of the SDIFindTypes calls are not needed and could
	// be removed to improve performance when reading from streams.
	assert(pproptypeinfo != 0);
}

inline void CVisSDIStream::UsePropListType(CVisPropTypeInfoBase *pproptypeinfo)
{
	// With input streams, we only need to make sure that we have type
	// information for top-level objects when they are read.
	// LATER:  Some of the SDIFindTypes calls are not needed and could
	// be removed to improve performance when reading from streams.
	assert(pproptypeinfo != 0);
}

inline void CVisSDIStream::OpenIfNeeded(void)
{
	if (!m_fOpen)
		Open();
}

inline bool CVisSDIStream::IsOpen(void) const
{
	return m_fOpen;
}

inline void CVisSDIStream::SetIsOpen(bool f)
{
	m_fOpen = f;
}



inline void VisSDReadValue(CVisSDIStream& refsdistream,
		bool& reff)
	{ refsdistream.ReadBool(reff); }

inline void VisSDReadValue(CVisSDIStream& refsdistream,
		char& refch)
	{ refsdistream.ReadCh(refch); }

inline void VisSDReadValue(CVisSDIStream& refsdistream,
		std::string& refstr)
	{ refsdistream.ReadStr(refstr); }


// Shouldn't need to call VisSDIFindTypes for standard types.
#define VIS_DECLARE_SDI_READ_OBJ_STDOBJ(Storage, TObject)	\
	Storage void VisSDReadObj(CVisSDIStream& refsdistream,	\
			TObject& refobj);

#define VIS_DECLARE_SDI_READ_VALUE_STD_NUM(Storage, TObject)\
	Storage void VisSDReadValue(CVisSDIStream& refsdistream,\
			TObject& refnum);


#define VIS_DEFINE_SDI_READ_OBJ_STDOBJ(Storage, TObject)	\
	Storage void VisSDReadObj(CVisSDIStream& refsdistream,	\
			TObject& refobj)								\
		{													\
			refsdistream.StartObj(refobj);					\
			VisSDReadValue(refsdistream, refobj);			\
			refsdistream.EndObj();							\
		}

#define VIS_DEFINE_SDI_READ_VALUE_STD_NUM(Storage, TObject)	\
	Storage void VisSDReadValue(CVisSDIStream& refsdistream,\
			TObject& refnum)								\
		{ refsdistream.ReadNum(refnum); }


#define VIS_SDI_READ_OBJ_STDOBJ(TObject)	\
	VIS_DEFINE_SDI_READ_OBJ_STDOBJ(inline, TObject)

#define VIS_SDI_READ_VALUE_STD_NUM(TObject)	\
	VIS_DEFINE_SDI_READ_VALUE_STD_NUM(inline, TObject)



#define VIS_DECLARE_SDI_CLASS_STD_NUM(Storage, TObject)		\
	VIS_DECLARE_SDI_FIND_TYPES_NOOP(Storage, TObject)		\
	VIS_DECLARE_SDI_READ_VALUE_STD_NUM(Storage, TObject)		\
	VIS_DECLARE_SDI_READ_OBJ_STDOBJ(Storage, TObject)		\
	VIS_DECLARE_SDI_READ_OP_DEFAULT(Storage, TObject)

#define VIS_DECLARE_SDI_CLASS_STD_NON_NUM(Storage, TObject)	\
	VIS_DECLARE_SDI_FIND_TYPES_NOOP(Storage, TObject)		\
	VIS_DECLARE_SDI_READ_OBJ_STDOBJ(Storage, TObject)		\
	VIS_DECLARE_SDI_READ_OP_DEFAULT(Storage, TObject)


#define VIS_DEFINE_SDI_CLASS_STD_NUM(Storage, TObject)		\
	VIS_DEFINE_SDI_FIND_TYPES_NOOP(Storage, TObject)		\
	VIS_DEFINE_SDI_READ_VALUE_STD_NUM(Storage, TObject)		\
	VIS_DEFINE_SDI_READ_OBJ_STDOBJ(Storage, TObject)		\
	VIS_DEFINE_SDI_READ_OP_DEFAULT(Storage, TObject)

#define VIS_DEFINE_SDI_CLASS_STD_NON_NUM(Storage, TObject)	\
	VIS_DEFINE_SDI_FIND_TYPES_NOOP(Storage, TObject)		\
	VIS_DEFINE_SDI_READ_OBJ_STDOBJ(Storage, TObject)		\
	VIS_DEFINE_SDI_READ_OP_DEFAULT(Storage, TObject)


#define VIS_SDI_CLASS_STD_NUM(TObject)		\
	VIS_SDI_FIND_TYPES_NOOP(TObject)		\
	VIS_SDI_READ_VALUE_STD_NUM(TObject)		\
	VIS_SDI_READ_OBJ_STDOBJ(TObject)		\
	VIS_SDI_READ_OP_DEFAULT(TObject)

#define VIS_SDI_CLASS_STD_NON_NUM(TObject)	\
	VIS_SDI_FIND_TYPES_NOOP(TObject)		\
	VIS_SDI_READ_OBJ_STDOBJ(TObject)		\
	VIS_SDI_READ_OP_DEFAULT(TObject)



VIS_DECLARE_SDI_CLASS_STD_NON_NUM(VisPropListExportDeclspec, bool)
VIS_DECLARE_SDI_CLASS_STD_NON_NUM(VisPropListExportDeclspec, char)
VIS_DECLARE_SDI_CLASS_STD_NON_NUM(VisPropListExportDeclspec, std::string)


VIS_DECLARE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, signed char)
VIS_DECLARE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned char)
VIS_DECLARE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, short)
VIS_DECLARE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned short)
VIS_DECLARE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, int)
VIS_DECLARE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned int)
VIS_DECLARE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, long)
VIS_DECLARE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned long)
VIS_DECLARE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, _int64)
VIS_DECLARE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned _int64)
VIS_DECLARE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, float)
VIS_DECLARE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, double)


VIS_DECLARE_SDI_CLASS_NO_IO(VisPropListExportDeclspec, void const *)
VIS_DECLARE_SDI_CLASS_NO_IO(VisPropListExportDeclspec, void *)



// Common STL containers

template<class TObject>
inline void VisSDIFindTypes(CVisSDIStream& refsdistream,
		std::deque<TObject> const& refdeque)
{
	if (refdeque.empty())
		VisSDIFindTypes(refsdistream, TObject());
	else
		VisSDIFindTypes(refsdistream, refdeque.front());

	CVisPropTypeInfoBase *pproptypeinfo = VisGetPropTypeInfo(refdeque);

	refsdistream.UseType(pproptypeinfo);
}

template<class TObject>
inline void VisSDReadValue(CVisSDIStream& refsdistream,
		std::deque<TObject>& refdeque)
{
	assert(VisPPropTypeInfoForClass(TObject()) != 0);
	refsdistream.OpenGroup();

	refdeque.clear();
	while (!refsdistream.AtEndOfGroup())
	{
		refdeque.push_back(TObject());
		VisSDReadValue(refsdistream, refdeque.back());
	}

	refsdistream.CloseGroup();
}

template<class TObject>
inline void VisSDReadObj(CVisSDIStream& refsdistream,
		std::deque<TObject>& refdeque)
{
	VisSDIFindTypes(refsdistream, refdeque);

	refsdistream.StartObj(refdeque);
	VisSDReadValue(refsdistream, refdeque);
	refsdistream.EndObj();
}

template<class TObject>
inline CVisSDIStream& operator>>(CVisSDIStream& refsdistream,
		std::deque<TObject>& refdeque)
{
	VisSDReadObj(refsdistream, refdeque);
	return refsdistream;
}



template<class TObject>
inline void VisSDIFindTypes(CVisSDIStream& refsdistream,
		std::vector<TObject> const& refvector)
{
	if (refvector.empty())
		VisSDIFindTypes(refsdistream, TObject());
	else
		VisSDIFindTypes(refsdistream, refvector.front());

	CVisPropTypeInfoBase *pproptypeinfo = VisGetPropTypeInfo(refvector);

	refsdistream.UseType(pproptypeinfo);
}

template<class TObject>
inline void VisSDReadValue(CVisSDIStream& refsdistream,
		std::vector<TObject>& refvector)
{
	assert(VisPPropTypeInfoForClass(TObject()) != 0);
	refsdistream.OpenGroup();

	refvector.clear();
	while (!refsdistream.AtEndOfGroup())
	{
		refvector.push_back(TObject());
		VisSDReadValue(refsdistream, refvector.back());
	}

	refsdistream.CloseGroup();
}

template<class TObject>
inline void VisSDReadObj(CVisSDIStream& refsdistream,
		std::vector<TObject>& refvector)
{
	VisSDIFindTypes(refsdistream, refvector);

	refsdistream.StartObj(refvector);
	VisSDReadValue(refsdistream, refvector);
	refsdistream.EndObj();
}

template<class TObject>
inline CVisSDIStream& operator>>(CVisSDIStream& refsdistream,
		std::vector<TObject>& refvector)
{
	VisSDReadObj(refsdistream, refvector, 0);
	return refsdistream;
}


#ifdef _DEBUG
#undef new
#endif // _DEBUG
