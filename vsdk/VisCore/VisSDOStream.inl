// VisSDOStream.inl
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG



// LATER:  We might consider a m_cNewLineNext variable that would be
// incremented in calls to NewLine.  It could help to keep separators
// a the ends of values.

inline void CVisSDOStream::OpenGroup(void)
{
	OpenGroup(m_fLotsOfNewLines);
}

inline void CVisSDOStream::CloseGroup(void)
{
	CloseGroup(m_fLotsOfNewLines);
}

inline void CVisSDOStream::NewLineNext(void)
{
	if (!m_fStartOfLine)
		m_fUseNewlineNext = true;
}



inline void CVisSDOStream::StartObjType(
		const CVisPropTypeInfoBase *pproptypeinfo, const char *szName)
{
	assert(pproptypeinfo != 0);
	StartObjType(pproptypeinfo->Name(), szName);
}


inline void CVisSDOStream::StartObjType(
		const CVisPropTypeInfoBase *pproptypeinfo, const CVisDim& refdim,
		const char *szName)
{
	assert(pproptypeinfo != 0);
	StartObjType(pproptypeinfo->Name(), refdim, szName);
}


inline bool CVisSDOStream::HasTypedef(const CVisPropTypeInfoBase *pproptypeinfo)
{
	return (PSDOTypedef(pproptypeinfo) != 0);
}

inline void CVisSDOStream::WriteNum(signed char w)
{
	WriteNum((int) w);
}

inline void CVisSDOStream::WriteNum(unsigned char n)
{
	WriteNum((unsigned int) n);
}

inline void CVisSDOStream::WriteNum(short w)
{
	WriteNum((int) w);
}

inline void CVisSDOStream::WriteNum(unsigned short n)
{
	WriteNum((unsigned int) n);
}


inline CVisSDOStream::EState CVisSDOStream::StateCur(void) const
{
	return (CVisSDOStream::EState) m_vectorestate[m_iLevel];
}

inline void CVisSDOStream::SetStateCur(CVisSDOStream::EState estate)
{
	m_vectorestate[m_iLevel] = estate;
}

inline void CVisSDOStream::PushState(CVisSDOStream::EState estate)
{
	++m_iLevel;
	m_vectorestate.push_back(estate);
}

inline void CVisSDOStream::PopState(void)
{
	assert(m_iLevel > 0);
	-- m_iLevel;
	m_vectorestate.pop_back();
}



inline void CVisSDOStream::StartValue(void)
{ StartValue(m_fLotsOfNewLines); }

inline void CVisSDOStream::StartValue(bool fNewLine)
{
	ValueSep(fNewLine);
}

inline void CVisSDOStream::EndValue(void)
{
	EndValue(m_fLotsOfNewLines);
}


inline void CVisSDOStream::ValueSep(void)
{
	ValueSep(m_fLotsOfNewLines);
}

inline void CVisSDOStream::WhitespaceSep(void)
{
	WhitespaceSep(m_fLotsOfNewLines);
}

inline bool CVisSDOStream::IsTypeBeforeDescription(void) const
{
	return m_fTypeBeforeDescription;
}

inline void CVisSDOStream::SetIsTypeBeforeDescription(bool f)
{
	m_fTypeBeforeDescription = f;
}

inline void CVisSDOStream::OpenIfNeeded(void)
{
	if (!m_fOpen)
		Open();
}

inline bool CVisSDOStream::IsOpen(void) const
{
	return m_fOpen;
}

inline void CVisSDOStream::SetIsOpen(bool f)
{
	m_fOpen = f;
}



inline void VisSDWriteValue(CVisSDOStream& refsdostream,
		bool const& reff)
	{ refsdostream.WriteBool(reff); }

inline void VisSDWriteValue(CVisSDOStream& refsdostream,
		char * const& refsz)
	{ refsdostream.WriteSz(refsz); }

inline void VisSDWriteValue(CVisSDOStream& refsdostream,
		const char * const& refsz)
	{ refsdostream.WriteSz(refsz); }

inline void VisSDWriteValue(CVisSDOStream& refsdostream,
		const char& refch)
	{ refsdostream.WriteCh(refch); }

inline void VisSDWriteValue(CVisSDOStream& refsdostream,
		std::string const& refstr)
	{ refsdostream.WriteSz(refstr.c_str()); }



#define VIS_DECLARE_SDO_WRITE_OBJ_STDOBJ(Storage, TObject)	\
	Storage void VisSDWriteObj(CVisSDOStream& refsdostream,	\
			TObject const& refobj, const char *szName);

#define VIS_DECLARE_SDO_WRITE_VALUE_STD_NUM(Storage, TObject)\
	Storage void VisSDWriteValue(CVisSDOStream& refsdostream,\
			TObject const& refnum);


#define VIS_DEFINE_SDO_WRITE_OBJ_STDOBJ(Storage, TObject)	\
	Storage void VisSDWriteObj(CVisSDOStream& refsdostream,	\
			TObject const& refobj, const char *szName)		\
		{													\
			refsdostream.StartObj(refobj, szName);			\
			VisSDWriteValue(refsdostream, refobj);			\
			refsdostream.EndObj();							\
		}

#define VIS_DEFINE_SDO_WRITE_VALUE_STD_NUM(Storage, TObject)	\
	Storage void VisSDWriteValue(CVisSDOStream& refsdostream,	\
			TObject const& refnum)								\
		{ refsdostream.WriteNum(refnum); }


#define VIS_SDO_WRITE_OBJ_STDOBJ(TObject)	\
	VIS_DEFINE_SDO_WRITE_OBJ_STDOBJ(inline, TObject)

#define VIS_SDO_WRITE_VALUE_STD_NUM(TObject)	\
	VIS_DEFINE_SDO_WRITE_VALUE_STD_NUM(inline, TObject)



#define VIS_DECLARE_SDO_CLASS_STD_NUM(Storage, TObject)		\
	VIS_DECLARE_SDO_FIND_TYPES_NOOP(Storage, TObject)		\
	VIS_DECLARE_SDO_WRITE_VALUE_STD_NUM(Storage, TObject)	\
	VIS_DECLARE_SDO_WRITE_OBJ_STDOBJ(Storage, TObject)		\
	VIS_DECLARE_SDO_WRITE_OP_DEFAULT(Storage, TObject)

#define VIS_DECLARE_SDO_CLASS_STD_NON_NUM(Storage, TObject)	\
	VIS_DECLARE_SDO_FIND_TYPES_NOOP(Storage, TObject)		\
	VIS_DECLARE_SDO_WRITE_OBJ_STDOBJ(Storage, TObject)		\
	VIS_DECLARE_SDO_WRITE_OP_DEFAULT(Storage, TObject)


#define VIS_DEFINE_SDO_CLASS_STD_NUM(Storage, TObject)		\
	VIS_DEFINE_SDO_FIND_TYPES_NOOP(Storage, TObject)		\
	VIS_DEFINE_SDO_WRITE_VALUE_STD_NUM(Storage, TObject)	\
	VIS_DEFINE_SDO_WRITE_OBJ_STDOBJ(Storage, TObject)		\
	VIS_DEFINE_SDO_WRITE_OP_DEFAULT(Storage, TObject)

#define VIS_DEFINE_SDO_CLASS_STD_NON_NUM(Storage, TObject)	\
	VIS_DEFINE_SDO_FIND_TYPES_NOOP(Storage, TObject)		\
	VIS_DEFINE_SDO_WRITE_OBJ_STDOBJ(Storage, TObject)		\
	VIS_DEFINE_SDO_WRITE_OP_DEFAULT(Storage, TObject)


#define VIS_SDO_CLASS_STD_NUM(TObject)		\
	VIS_SDO_FIND_TYPES_NOOP(TObject)		\
	VIS_SDO_WRITE_VALUE_STD_NUM(TObject)	\
	VIS_SDO_WRITE_OBJ_STDOBJ(TObject)		\
	VIS_SDO_WRITE_OP_DEFAULT(TObject)

#define VIS_SDO_CLASS_STD_NON_NUM(TObject)	\
	VIS_SDO_FIND_TYPES_NOOP(TObject)		\
	VIS_SDO_WRITE_OBJ_STDOBJ(TObject)		\
	VIS_SDO_WRITE_OP_DEFAULT(TObject)



VIS_DECLARE_SDO_CLASS_STD_NON_NUM(VisPropListExportDeclspec, bool)
VIS_DECLARE_SDO_CLASS_STD_NON_NUM(VisPropListExportDeclspec, char const *)
VIS_DECLARE_SDO_CLASS_STD_NON_NUM(VisPropListExportDeclspec, char *)
VIS_DECLARE_SDO_CLASS_STD_NON_NUM(VisPropListExportDeclspec, char)
VIS_DECLARE_SDO_CLASS_STD_NON_NUM(VisPropListExportDeclspec, std::string)


VIS_DECLARE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, signed char)
VIS_DECLARE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned char)
VIS_DECLARE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, short)
VIS_DECLARE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned short)
VIS_DECLARE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, int)
VIS_DECLARE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned int)
VIS_DECLARE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, long)
VIS_DECLARE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned long)
VIS_DECLARE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, _int64)
VIS_DECLARE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned _int64)
VIS_DECLARE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, float)
VIS_DECLARE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, double)


VIS_DECLARE_SDO_CLASS_NO_IO(VisPropListExportDeclspec, void const *)
VIS_DECLARE_SDO_CLASS_NO_IO(VisPropListExportDeclspec, void *)



// Common STL containers

template<class TObject>
inline void VisSDOFindTypes(CVisSDOStream& refsdostream,
		std::deque<TObject> const& refdeque)
{
	if (refdeque.empty())
		VisSDOFindTypes(refsdostream, TObject());
	else
		VisSDOFindTypes(refsdostream, refdeque.front());

	CVisPropTypeInfoBase *pproptypeinfo = VisGetPropTypeInfo(refdeque);

	refsdostream.UseType(pproptypeinfo);
}

template<class TObject>
inline void VisSDWriteValue(CVisSDOStream& refsdostream,
		std::deque<TObject> const& refdeque)
{
	assert(VisPPropTypeInfoForClass(TObject()) != 0);
	bool fUseNewLines = ((!refdeque.empty())
			&& (!VisPPropTypeInfoForClass(refdeque.front())->IsSimpleType()));
	refsdostream.OpenGroup(fUseNewLines);

	const std::deque<TObject>::const_iterator iteratorLim = refdeque.end();
	std::deque<TObject>::const_iterator iteratorCur = refdeque.begin();
	for (; iteratorCur != iteratorLim; ++iteratorCur)
	{
		VisSDWriteValue(refsdostream, *iteratorCur);
		if (fUseNewLines)
			refsdostream.NewLineNext();
	}

	refsdostream.CloseGroup(fUseNewLines);
}

template<class TObject>
inline void VisSDWriteObj(CVisSDOStream& refsdostream,
		std::deque<TObject> const& refdeque, const char *szName)
{
	VisSDOFindTypes(refsdostream, refdeque);

	refsdostream.StartObj(refdeque, szName);
	VisSDWriteValue(refsdostream, refdeque);
	refsdostream.EndObj();
}

template<class TObject>
inline CVisSDOStream& operator<<(CVisSDOStream& refsdostream,
		std::deque<TObject> const& refdeque)
{
	VisSDWriteObj(refsdostream, refdeque, 0);
	return refsdostream;
}


template<class TObject>
inline void VisSDOFindTypes(CVisSDOStream& refsdostream,
		std::deque<TObject>& refdeque)
{
	VisSDOFindTypes(refsdostream, (std::deque<TObject> const &) refdeque);
}

template<class TObject>
inline void VisSDWriteValue(CVisSDOStream& refsdostream,
		std::deque<TObject>& refdeque)
{
	VisSDWriteValue(refsdostream, (std::deque<TObject> const &) refdeque);
}

template<class TObject>
inline void VisSDWriteObj(CVisSDOStream& refsdostream,
		std::deque<TObject>& refdeque, const char *szName)
{
	VisSDWriteObj(refsdostream, (std::deque<TObject> const &) refdeque);
}

template<class TObject>
inline CVisSDOStream& operator<<(CVisSDOStream& refsdostream,
		std::deque<TObject>& refdeque)
{
	return operator<<(refsdostream, (std::deque<TObject> const &) refdeque);
}



template<class TObject>
inline void VisSDOFindTypes(CVisSDOStream& refsdostream,
		std::vector<TObject> const& refvector)
{
	if (refvector.empty())
		VisSDOFindTypes(refsdostream, TObject());
	else
		VisSDOFindTypes(refsdostream, refvector.front());

	CVisPropTypeInfoBase *pproptypeinfo = VisGetPropTypeInfo(refvector);

	refsdostream.UseType(pproptypeinfo);
}

template<class TObject>
inline void VisSDWriteValue(CVisSDOStream& refsdostream,
		std::vector<TObject> const& refvector)
{
	assert(VisPPropTypeInfoForClass(TObject()) != 0);
	bool fUseNewLines = ((!refvector.empty())
			&& (!VisPPropTypeInfoForClass(refvector.front())->IsSimpleType()));
	refsdostream.OpenGroup(fUseNewLines);

	const std::vector<TObject>::const_iterator iteratorLim = refvector.end();
	std::vector<TObject>::const_iterator iteratorCur = refvector.begin();
	for (; iteratorCur != iteratorLim; ++iteratorCur)
	{
		VisSDWriteValue(refsdostream, *iteratorCur);
		if (fUseNewLines)
			refsdostream.NewLineNext();
	}

	refsdostream.CloseGroup(fUseNewLines);
}

template<class TObject>
inline void VisSDWriteObj(CVisSDOStream& refsdostream,
		std::vector<TObject> const& refvector, const char *szName)
{
	VisSDOFindTypes(refsdostream, refvector);

	refsdostream.StartObj(refvector, szName);
	VisSDWriteValue(refsdostream, refvector);
	refsdostream.EndObj();
}

template<class TObject>
inline CVisSDOStream& operator<<(CVisSDOStream& refsdostream,
		std::vector<TObject> const& refvector)
{
	VisSDWriteObj(refsdostream, refvector, 0);
	return refsdostream;
}


template<class TObject>
inline void VisSDOFindTypes(CVisSDOStream& refsdostream,
		std::vector<TObject>& refvector)
{
	VisSDOFindTypes(refsdostream, (std::vector<TObject> const &) refvector);
}

template<class TObject>
inline void VisSDWriteValue(CVisSDOStream& refsdostream,
		std::vector<TObject>& refvector)
{
	VisSDWriteValue(refsdostream, (std::vector<TObject> const &) refvector);
}

template<class TObject>
inline void VisSDWriteObj(CVisSDOStream& refsdostream,
		std::vector<TObject>& refvector, const char *szName)
{
	VisSDWriteObj(refsdostream, (std::vector<TObject> const &) refvector);
}

template<class TObject>
inline CVisSDOStream& operator<<(CVisSDOStream& refsdostream,
		std::vector<TObject>& refvector)
{
	return operator<<(refsdostream, (std::vector<TObject> const &) refvector);
}


#ifdef _DEBUG
#undef new
#endif // _DEBUG
