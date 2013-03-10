// VisSDOStream.h
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved



// Forward declarartions
class CVisPropTypeInfoBase;
class CVisPropList;


typedef std::vector<std::string > CVisSDOTypedef;
#pragma once

class CVisSDOStream
{
public:
	// Default constructor uses std::cout.
	VisPropListExport CVisSDOStream(void);
	VisPropListExport CVisSDOStream(std::ostream& refostream);

	virtual VisPropListExport ~CVisSDOStream(void);

	
	// Virtual function used to get a file name from derived classes that use
	// files.  The base class implementation returns 0.
	virtual VisPropListExport const char *FileName(void) const;


	// Returns true if the stream did not previously know about this type.
	VisPropListExport bool UseType(CVisPropTypeInfoBase *pproptypeinfo);

	// Returns true if the stream did not previously know about this type.
	template<class TObject>
	bool UseObjType(const TObject& refobj)
		{ return UseType(VisGetPropTypeInfo(refobj)); }


	// Returns true if the stream did not previously know about this type.
	VisPropListExport bool UsePropListType(CVisPropTypeInfoBase *pproptypeinfo,
			const void *pvObj);

	// Returns true if the stream did not previously know about this type.
	template<class TObject>
	bool UsePropListObjType(const TObject& refobj)
		{ return UsePropListType(VisGetPropTypeInfo(refobj), &refobj); }


	void StartObjType(const CVisPropTypeInfoBase *pproptypeinfo,
			const char *szName = 0);
	void StartObjType(const CVisPropTypeInfoBase *pproptypeinfo,
			const CVisDim& refdim, const char *szName = 0);

	// These two methods are used internally (but must be public for now).
	VisPropListExport void StartObjType(const char *szTypeIOName,
			const char *szName = 0);
	VisPropListExport void StartObjType(const char *szTypeIOName,
			const CVisDim& refdim, const char *szName = 0);

	template<class TObject>
	void StartObj(const TObject& refobj, const char *szName = 0)
		{ StartObjType(VisGetPropTypeInfo(refobj), szName); }

	template<class TObject>
	void StartObj(const TObject& refobj, const CVisDim& refdim,
			const char *szName = 0)
		{ StartObjType(VisGetPropTypeInfo(refobj), refdim, szName); }

	VisPropListExport void EndObj(bool fNewLine = true);


	void OpenGroup(void);
	VisPropListExport void OpenGroup(bool fNewLine);

	void CloseGroup(void);
	VisPropListExport void CloseGroup(bool fNewLine);


	VisPropListExport void NewLine(void);
	VisPropListExport void NewLine(int cNewLine);
	void NewLineNext(void);

	VisPropListExport void Comment(const char *sz, bool fStartOnNewLine = false);


#if _MSC_VER >= 6000
	template<class TObject>
	void UseObjTypedef(TObject& refobj,
			const CVisSDOTypedef& reftypedef = CVisSDOTypedef())
		{ UseTypedef(VisPPropTypeInfoForClass(refobj), reftypedef); }

	VisPropListExport void UseTypedef(CVisPropTypeInfoBase *pproptypeinfo,
			const CVisSDOTypedef& reftypedef = CVisSDOTypedef());
#else // _MSC_VER >= 6000
	template<class TObject>
	void UseObjTypedef(TObject& refobj)
		{ UseTypedef(VisPPropTypeInfoForClass(refobj), CVisSDOTypedef()); }
	template<class TObject>
	void UseObjTypedef(TObject& refobj,
			const CVisSDOTypedef& reftypedef)
		{ UseTypedef(VisPPropTypeInfoForClass(refobj), reftypedef); }

	void UseTypedef(CVisPropTypeInfoBase *pproptypeinfo)
		{ UseTypedef(pproptypeinfo, CVisSDOTypedef()); }
	VisPropListExport void UseTypedef(CVisPropTypeInfoBase *pproptypeinfo,
			const CVisSDOTypedef& reftypedef);
#endif // _MSC_VER >= 6000


	// LATER:  Add code to iterate through the list of current typedefs?
	VisPropListExport const CVisSDOTypedef *PSDOTypedef(const CVisPropTypeInfoBase *pproptypeinfo) const;
	VisPropListExport CVisSDOTypedef *PSDOTypedef(const CVisPropTypeInfoBase *pproptypeinfo);

	bool HasTypedef(const CVisPropTypeInfoBase *pproptypeinfo);


#ifdef LATER // Useful in asserts
	static bool IsValidValue(const char *sz);
	static bool IsValidSzValue(const char *sz);
	static bool IsValidNumValue(const char *sz);
	static bool IsValidTypeName(const char *sz);
	static bool IsValidVarName(const char *sz);
#endif // LATER

	// Helper methods used internally.
	VisPropListExport void WriteBool(bool f);
	VisPropListExport void WriteSz(const char *sz);
	VisPropListExport void WriteCh(char ch);
	void WriteNum(signed char w);
	void WriteNum(unsigned char n);
	void WriteNum(short w);
	void WriteNum(unsigned short n);
	VisPropListExport void WriteNum(int w);
	VisPropListExport void WriteNum(unsigned int n);
	VisPropListExport void WriteNum(const long& refnum);
	VisPropListExport void WriteNum(const unsigned long& refnum);
	VisPropListExport void WriteNum(const _int64& refnum);
	VisPropListExport void WriteNum(const unsigned _int64& refnum);
	VisPropListExport void WriteNum(const float& flt);
	VisPropListExport void WriteNum(const double& dbl);


	// These helper functions are used internally.
	template<class TObject>
	void WriteClassUsingPropList(TObject const& refobj)
		{ WriteClassPropList(VisPPropTypeInfoForClass(refobj), &refobj); }

	VisPropListExport void WriteSharedPrefix(void);

	// This flag indicates whether the type comes before or after the struct
	// description in a typedef.  (Before is more efficient when reading, but
	// after is more "C-like".)
	bool IsTypeBeforeDescription(void) const;
	void SetIsTypeBeforeDescription(bool f);

	// The first time this method is called, it will attempt to open
	// the stream.  This could throw an exception.
	void OpenIfNeeded(void);

	bool IsOpen(void) const;
	virtual VisPropListExport void Open(void);
	virtual VisPropListExport void Close(void);

	// LATER:  fVerbose?

protected:
	enum
	{
		echNewLine					= '\n',
		echSpace					= ' ',
		echOpenGroup				= '{',
		echCloseGroup				= '}',
		echValueSep					= ',',
		echFieldSep					= ';',
		echEquals					= '=',
		echEscape					= '\\',
		echSingleQuote				= '\'',
		echDblQuote					= '"',
		echNoName					= '~'  // Shouldn't be in type or var names
	};

	enum EState
	{
		estateObjList,
		estateValueField,
		estateValueList,
		estateEndOfObj,
		estateStartOfGroup,
	};

	EState StateCur(void) const;
	void SetStateCur(EState estate);
	void PushState(EState estate);
	void PopState(void);

	VisPropListExport void ArraySuffix(const CVisDim& refdim);

	void StartValue(void);
	void StartValue(bool fNewLine);

	void EndValue(void);
	VisPropListExport void EndValue(bool fNewLine);

	void ValueSep(void);
	VisPropListExport void ValueSep(bool fNewLine);

	void WhitespaceSep(void);
	VisPropListExport void WhitespaceSep(bool fNewLine);

	VisPropListExport void Indent(void);


	VisPropListExport void WriteClassPropList(CVisPropTypeInfoBase *pproptypeinfo,
			const void *pvObjOffset);
	VisPropListExport void WriteClassPropList(CVisPropList& refproplist,
			const void *pvObjOffset = 0, const CVisSDOTypedef *ptypedef = 0);

	void SetIsOpen(bool f);

private:
	typedef std::map<const CVisPropTypeInfoBase *, CVisSDOTypedef> CTypedefMap;

	std::ostream& m_refostream;
	int m_iLevel;
#if _MSC_VER >= 6000
	std::vector<EState> m_vectorestate;
#else // _MSC_VER >= 6000
	std::vector<int> m_vectorestate;
#endif // _MSC_VER >= 6000
	CTypedefMap m_mapTypedef;
	std::set<CVisPropTypeInfoBase *> m_setproptypeinfo;
	bool m_fLotsOfNewLines;
	bool m_fStartOfLine;
	bool m_fUseNewlineNext;
	bool m_fIgnoreComments;
	bool m_fTypeBeforeDescription;
	bool m_fOpen;
};


class CVisSDOFStream : public CVisSDOStream
{
public:
	// Default constructor uses std::cin.
	VisPropListExport CVisSDOFStream(const char *szFileName);

	virtual VisPropListExport ~CVisSDOFStream(void);


	virtual VisPropListExport const char *FileName(void) const;


	virtual VisPropListExport void Open(void);
	virtual VisPropListExport void Close(void);


private:
	std::ofstream m_ofstream;
	std::string m_strFileName;
};


// UNDONE:  Add a CVisSDODebugStream class that uses a CVisSDOStream to write
// into a string and then flushes the string to the debug window (in dtor or
// when a Flush method is called).


//
// Helper macros
//
// (The three macros at the bottom of this file are the most useful.)
//

#define VIS_DECLARE_SDO_FIND_TYPES_DEFAULT(Storage, TObject)	\
	Storage void VisSDOFindTypes(CVisSDOStream& refsdostream,	\
			TObject const& refobj);

#define VIS_DECLARE_SDO_FIND_TYPES_PL(Storage, TObject)			\
	Storage void VisSDOFindTypes(CVisSDOStream& refsdostream,	\
			TObject const& refobj);

#define VIS_DECLARE_SDO_FIND_TYPES_NOOP(Storage, TObject)		\
	Storage void VisSDOFindTypes(CVisSDOStream& refsdostream,	\
			TObject const& refobj);


#define VIS_DECLARE_SDO_WRITE_VALUE_USING_CLASS_METHOD(Storage, TObject)\
	Storage void VisSDWriteValue(CVisSDOStream& refsdostream,			\
			TObject const& refobj);

#define VIS_DECLARE_SDO_WRITE_VALUE_USING_CLASS_PL(Storage, TObject)\
	Storage void VisSDWriteValue(CVisSDOStream& refsdostream,		\
			TObject const& refobj);

#define VIS_DECLARE_SDO_WRITE_VALUE_NO_IO(Storage, TObject)		\
	Storage void VisSDWriteValue(CVisSDOStream& refsdostream,	\
			TObject const& refobj);


#define VIS_DECLARE_SDO_WRITE_OBJ_DEFAULT(Storage, TObject)	\
	Storage void VisSDWriteObj(CVisSDOStream& refsdostream,	\
			TObject const& refobj, const char *szName);

#define VIS_DECLARE_SDO_WRITE_OBJ_NO_IO(Storage, TObject)	\
	Storage void VisSDWriteObj(CVisSDOStream& refsdostream,	\
			TObject const& refobj, const char *szName);


#define VIS_DECLARE_SDO_WRITE_OP_DEFAULT(Storage, TObject)			\
	Storage CVisSDOStream& operator<<(CVisSDOStream& refsdostream,	\
			TObject const& refobj);


#define VIS_DECLARE_SDO_CLASS(Storage, TObject)						\
	VIS_DECLARE_SDO_FIND_TYPES_DEFAULT(Storage, TObject)			\
	VIS_DECLARE_SDO_WRITE_VALUE_USING_CLASS_METHOD(Storage, TObject)\
	VIS_DECLARE_SDO_WRITE_OBJ_DEFAULT(Storage, TObject)				\
	VIS_DECLARE_SDO_WRITE_OP_DEFAULT(Storage, TObject)

#define VIS_DECLARE_SDO_CLASS_WITH_PL(Storage, TObject)			\
	VIS_DECLARE_SDO_FIND_TYPES_PL(Storage, TObject)				\
	VIS_DECLARE_SDO_WRITE_VALUE_USING_CLASS_PL(Storage, TObject)\
	VIS_DECLARE_SDO_WRITE_OBJ_DEFAULT(Storage, TObject)			\
	VIS_DECLARE_SDO_WRITE_OP_DEFAULT(Storage, TObject)

#define VIS_DECLARE_SDO_CLASS_NO_IO(Storage, TObject)	\
	VIS_DECLARE_SDO_FIND_TYPES_NOOP(Storage, TObject)	\
	VIS_DECLARE_SDO_WRITE_VALUE_NO_IO(Storage, TObject)	\
	VIS_DECLARE_SDO_WRITE_OBJ_NO_IO(Storage, TObject)	\
	VIS_DECLARE_SDO_WRITE_OP_DEFAULT(Storage, TObject)



#define VIS_DEFINE_SDO_FIND_TYPES_DEFAULT(Storage, TObject)		\
	Storage void VisSDOFindTypes(CVisSDOStream& refsdostream,	\
			TObject const& refobj)								\
		{ refsdostream.UseObjType(refobj); }

#define VIS_DEFINE_SDO_FIND_TYPES_PL(Storage, TObject)			\
	Storage void VisSDOFindTypes(CVisSDOStream& refsdostream,	\
			TObject const& refobj)								\
		{ refsdostream.UsePropListObjType(refobj); }

#define VIS_DEFINE_SDO_FIND_TYPES_NOOP(Storage, TObject)		\
	Storage void VisSDOFindTypes(CVisSDOStream& refsdostream,	\
			TObject const& refobj)								\
		{}


#define VIS_DEFINE_SDO_WRITE_VALUE_USING_CLASS_METHOD(Storage, TObject)	\
	Storage void VisSDWriteValue(CVisSDOStream& refsdostream,			\
			TObject const& refobj)										\
		{ refobj.WriteValue(refsdostream); }

#define VIS_DEFINE_SDO_WRITE_VALUE_USING_CLASS_PL(Storage, TObject)	\
	Storage void VisSDWriteValue(CVisSDOStream& refsdostream,		\
			TObject const& refobj)									\
		{ refsdostream.WriteClassUsingPropList(						\
				(const_cast<TObject&> (refobj))); }

#define VIS_DEFINE_SDO_WRITE_VALUE_NO_IO(Storage, TObject)		\
	Storage void VisSDWriteValue(CVisSDOStream& refsdostream,	\
			TObject const& refobj)								\
		{ assert(0); }


#define VIS_DEFINE_SDO_WRITE_OBJ_DEFAULT(Storage, TObject)	\
	Storage void VisSDWriteObj(CVisSDOStream& refsdostream,	\
			TObject const& refobj, const char *szName)		\
		{													\
			VisSDOFindTypes(refsdostream, refobj);			\
															\
			refsdostream.StartObj(refobj, szName);			\
			VisSDWriteValue(refsdostream, refobj);			\
			refsdostream.EndObj();							\
		}

#define VIS_DEFINE_SDO_WRITE_OBJ_NO_IO(Storage, TObject)	\
	Storage void VisSDWriteObj(CVisSDOStream& refsdostream,	\
			TObject const& refobj, const char *szName)		\
		{ assert(0); }


#define VIS_DEFINE_SDO_WRITE_OP_DEFAULT(Storage, TObject)			\
	Storage CVisSDOStream& operator<<(CVisSDOStream& refsdostream,	\
			TObject const& refobj)									\
		{ VisSDWriteObj(refsdostream, refobj, 0);					\
				return refsdostream; }



#define VIS_DEFINE_SDO_CLASS(Storage, TObject)						\
	VIS_DEFINE_SDO_FIND_TYPES_DEFAULT(Storage, TObject)				\
	VIS_DEFINE_SDO_WRITE_VALUE_USING_CLASS_METHOD(Storage, TObject)	\
	VIS_DEFINE_SDO_WRITE_OBJ_DEFAULT(Storage, TObject)				\
	VIS_DEFINE_SDO_WRITE_OP_DEFAULT(Storage, TObject)

#define VIS_DEFINE_SDO_CLASS_WITH_PL(Storage, TObject)			\
	VIS_DEFINE_SDO_FIND_TYPES_PL(Storage, TObject)				\
	VIS_DEFINE_SDO_WRITE_VALUE_USING_CLASS_PL(Storage, TObject)	\
	VIS_DEFINE_SDO_WRITE_OBJ_DEFAULT(Storage, TObject)			\
	VIS_DEFINE_SDO_WRITE_OP_DEFAULT(Storage, TObject)

#define VIS_DEFINE_SDO_CLASS_NO_IO(Storage, TObject)	\
	VIS_DEFINE_SDO_FIND_TYPES_NOOP(Storage, TObject)	\
	VIS_DEFINE_SDO_WRITE_VALUE_NO_IO(Storage, TObject)	\
	VIS_DEFINE_SDO_WRITE_OBJ_NO_IO(Storage, TObject)	\
	VIS_DEFINE_SDO_WRITE_OP_DEFAULT(Storage, TObject)




#define VIS_SDO_FIND_TYPES_DEFAULT(TObject)	\
	VIS_DEFINE_SDO_FIND_TYPES_DEFAULT(inline, TObject)

#define VIS_SDO_FIND_TYPES_PL(TObject)	\
	VIS_DEFINE_SDO_FIND_TYPES_PL(inline, TObject)

#define VIS_SDO_FIND_TYPES_NOOP(TObject)	\
	VIS_DEFINE_SDO_FIND_TYPES_NOOP(inline, TObject)


#define VIS_SDO_WRITE_VALUE_USING_CLASS_METHOD(TObject)	\
	VIS_DEFINE_SDO_WRITE_VALUE_USING_CLASS_METHOD(inline, TObject)

#define VIS_SDO_WRITE_VALUE_USING_CLASS_PL(TObject)	\
	VIS_DEFINE_SDO_WRITE_VALUE_USING_CLASS_PL(inline, TObject)

#define VIS_SDO_WRITE_VALUE_NO_IO(TObject)	\
	VIS_DEFINE_SDO_WRITE_VALUE_NO_IO(inline, TObject)


#define VIS_SDO_WRITE_OBJ_DEFAULT(TObject)	\
	VIS_DEFINE_SDO_WRITE_OBJ_DEFAULT(inline, TObject)

#define VIS_SDO_WRITE_OBJ_NO_IO(TObject)	\
	VIS_DEFINE_SDO_WRITE_OBJ_NO_IO(inline, TObject)


#define VIS_SDO_WRITE_OP_DEFAULT(TObject)	\
	VIS_DEFINE_SDO_WRITE_OP_DEFAULT(inline, TObject)


#define VIS_TEMPLATE_SDO_CLASS(TTemplate, TObject)	\
	template<class TTemplate >						\
	VIS_SDO_FIND_TYPES_DEFAULT(TObject)				\
	template<class TTemplate >						\
	VIS_SDO_WRITE_VALUE_USING_CLASS_METHOD(TObject)	\
	template<class TTemplate >						\
	VIS_SDO_WRITE_OBJ_DEFAULT(TObject)				\
	template<class TTemplate >						\
	VIS_SDO_WRITE_OP_DEFAULT(TObject)

#define VIS_TEMPLATE_SDO_CLASS_WITH_PL(TTemplate, TObject)	\
	template<class TTemplate >								\
	VIS_SDO_FIND_TYPES_PL(TObject)							\
	template<class TTemplate >								\
	VIS_SDO_WRITE_VALUE_USING_CLASS_PL(TObject)				\
	template<class TTemplate >								\
	VIS_SDO_WRITE_OBJ_DEFAULT(TObject)						\
	template<class TTemplate >								\
	VIS_SDO_WRITE_OP_DEFAULT(TObject)

#define VIS_TEMPLATE_SDO_CLASS_NO_IO(TTemplate, TObject)\
	template<class TTemplate >							\
	VIS_SDO_FIND_TYPES_NOOP(TObject)					\
	template<class TTemplate >							\
	VIS_SDO_WRITE_VALUE_NO_IO(TObject)					\
	template<class TTemplate >							\
	VIS_SDO_WRITE_OBJ_NO_IO(TObject)					\
	template<class TTemplate >							\
	VIS_SDO_WRITE_OP_DEFAULT(TObject)



#define VIS_SDO_CLASS(TObject)						\
	VIS_SDO_FIND_TYPES_DEFAULT(TObject)				\
	VIS_SDO_WRITE_VALUE_USING_CLASS_METHOD(TObject)	\
	VIS_SDO_WRITE_OBJ_DEFAULT(TObject)				\
	VIS_SDO_WRITE_OP_DEFAULT(TObject)

#define VIS_SDO_CLASS_WITH_PL(TObject)			\
	VIS_SDO_FIND_TYPES_PL(TObject)				\
	VIS_SDO_WRITE_VALUE_USING_CLASS_PL(TObject)	\
	VIS_SDO_WRITE_OBJ_DEFAULT(TObject)			\
	VIS_SDO_WRITE_OP_DEFAULT(TObject)

#define VIS_SDO_CLASS_NO_IO(TObject)	\
	VIS_SDO_FIND_TYPES_NOOP(TObject)	\
	VIS_SDO_WRITE_VALUE_NO_IO(TObject)	\
	VIS_SDO_WRITE_OBJ_NO_IO(TObject)	\
	VIS_SDO_WRITE_OP_DEFAULT(TObject)
