// VisSDIStream.h
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#include <iostream>

#include <vector>
#include <deque>
#include <map>


// Forward declarartion
class CVisPropTypeInfoBase;
#pragma once

typedef struct
{
	std::string strType;
	std::string strName;
	CVisDim dim;
	bool fShared;
	bool rgfUnused[3];
}
					SVisSDITypedefInfo;

#if _MSC_VER < 6000
// == and < are required to use STL functions in VC++ 5.0.
inline bool operator==(const SVisSDITypedefInfo& refsditypedefinfo1,
		const SVisSDITypedefInfo& refsditypedefinfo2)
{
	return ((refsditypedefinfo1.strType == refsditypedefinfo2.strType)
			&& (refsditypedefinfo1.strName == refsditypedefinfo2.strName));
}

inline bool operator<(const SVisSDITypedefInfo& refsditypedefinfo1,
		const SVisSDITypedefInfo& refsditypedefinfo2)
{
	return ((refsditypedefinfo1.strType < refsditypedefinfo2.strType)
		|| ((refsditypedefinfo1.strType == refsditypedefinfo2.strType)
			&& (refsditypedefinfo1.strName < refsditypedefinfo2.strName)));
}
#endif // _MSC_VER < 6000

typedef std::vector<SVisSDITypedefInfo> CVisSDITypedef;


class CVisSDIStream
{
public:
	// Default constructor uses std::cin.
	VisPropListExport CVisSDIStream(bool fUseStrictTypeMatching = false);
	VisPropListExport CVisSDIStream(std::istream& refistream,
			bool fUseStrictTypeMatching = false);

	virtual VisPropListExport ~CVisSDIStream(void);

	
	// Virtual function used to get a file name from derived classes that use
	// files.  The base class implementation returns 0.
	virtual VisPropListExport const char *FileName(void) const;


	bool UseStrictTypeMatching(void) const;
	void SetUseStrictTypeMatching(bool f);


	VisPropListExport bool AtEndOfFile(void);
	VisPropListExport bool AtStartOfGroup(void);
	VisPropListExport bool AtEndOfGroup(void);

	VisPropListExport void OpenGroup(void);
	VisPropListExport void CloseGroup(void);


	VisPropListExport void ReadTypeNameAndDim(std::string& refstrType, std::string& refstrName,
			CVisDim& refdim, bool *pfShared = 0, bool fForTypedef = false);

	VisPropListExport void StartObjType(const char *szType);
	void StartObjType(const CVisPropTypeInfoBase *pproptypeinfo);

	template<class TObject>
	void StartObj(TObject& refobj)
		{ StartObjType(VisGetPropTypeInfo(refobj)); }

	VisPropListExport void EndObj(void);


	void UseType(CVisPropTypeInfoBase *pproptypeinfo);

	template<class TObject>
	void UseObjType(const TObject& refobj)
		{ UseType(VisGetPropTypeInfo(refobj)); }


	void UsePropListType(CVisPropTypeInfoBase *pproptypeinfo);

	template<class TObject>
	void UsePropListObjType(const TObject& refobj)
		{ UsePropListType(VisGetPropTypeInfo(refobj)); }


	// These helper functions are used internally.
	template<class TObject>
	void ReadClassUsingPropList(TObject& refobj)
		{ ReadClassPropList(VisGetPropTypeInfo(refobj), &refobj); }

	VisPropListExport const CVisSDITypedef *PTypedef(const char *szType) const;



	// Helper methods used internally.
	VisPropListExport void ReadCh(char& refch);
	VisPropListExport void ReadBool(bool& reff);
	VisPropListExport void ReadNum(signed char& refnum);
	VisPropListExport void ReadNum(unsigned char& refnum);
	VisPropListExport void ReadNum(short& refnum);
	VisPropListExport void ReadNum(unsigned short& refnum);
	VisPropListExport void ReadNum(int& refnum);
	VisPropListExport void ReadNum(unsigned int& refnum);
	VisPropListExport void ReadNum(long& refnum);
	VisPropListExport void ReadNum(unsigned long& refnum);
	VisPropListExport void ReadNum(_int64& refnum);
	VisPropListExport void ReadNum(unsigned _int64& refnum);
	VisPropListExport void ReadNum(float& flt);
	VisPropListExport void ReadNum(double& dbl);
	VisPropListExport void ReadStr(std::string& refstr, bool fClearString = true);

	VisPropListExport void SkipValue(void);

	int ILine(void) const;

	// The first time this method is called, it will attempt to open
	// the stream.  This could throw an exception.
	void OpenIfNeeded(void);

	bool IsOpen(void) const;
	virtual VisPropListExport void Open(void);
	virtual VisPropListExport void Close(void);


protected:
	enum
	{
		echNewLine					= '\n',
		echSpace					= ' ',
		echOpenGroup				= '{',
		echCloseGroup				= '}',
		echStartArrayDim			= '[',
		echEndArrayDim				= ']',
		echValueSep					= ',',
		echFieldSep					= ';',
		echEquals					= '=',
		echStartComment				= '/',
		echEscape					= '\\',
		echSingleQuote				= '\'',
		echDblQuote					= '"',

		// '~' shouldn't be in type or var names
		echNoName					= '~' 
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


	void StartValue(void);
	VisPropListExport void EndValue(void);

	VisPropListExport void ValueSep(void);

	VisPropListExport void SkipWhitespace(void);
	VisPropListExport bool SkipWhitespaceNoThrow(void);
	VisPropListExport void SkipAssignment(void);
	VisPropListExport void SkipThroughEndOfString(void);
	VisPropListExport void SkipThroughEndOfGroup(void);
	VisPropListExport void SkipThroughEndOfComment(void);
	VisPropListExport void SkipThroughEndCh(int chEnd);
	void SkipThroughEndOfField(void);
	void SkipThroughEndOfValue(void);

	bool IsWhitespaceCh(int wCh);

	VisPropListExport void ReadToken(std::string& refstr, bool fAllowValueSep = false);
	VisPropListExport void ReadType(std::string& refstrType);
	VisPropListExport void ReadTypedef(void);

	VisPropListExport void ReadClassPropList(CVisPropTypeInfoBase *pproptypeinfo,
			void *pvObjOffset);

#ifdef LATER // Useful in asserts
	static bool IsValidValue(const char *sz);
	static bool IsValidSzValue(const char *sz);
	static bool IsValidNumValue(const char *sz);
	static bool IsValidTypeName(const char *sz);
	static bool IsValidVarName(const char *sz);
	static bool IsStdIntegralType(const char *sz);
	static bool IsStdFloatType(const char *sz);
#endif // LATER
	static VisPropListExport bool IsValidArraySuffix(const char *sz);

	// This will remove the array suffix and return the array dimensions.
	VisPropListExport CVisDim ParseArraySuffix(std::string& refstr);

	void SetIsOpen(bool f);


private:
	typedef std::map<std::string, CVisSDITypedef> CTypedefMap;

	std::istream& m_refistream;
	int m_iLevel;
#if _MSC_VER >= 6000
	std::vector<EState> m_vectorestate;
#else // _MSC_VER >= 6000
	std::vector<int> m_vectorestate;
#endif // _MSC_VER >= 6000
	CTypedefMap m_mapTypedef;
	int m_iLine;
	bool m_fUseStrictTypeMatching;
	bool m_fOpen;
};


class CVisSDIFStream : public CVisSDIStream
{
public:
	// Default constructor uses std::cin.
	VisPropListExport CVisSDIFStream(const char *szFileName,
			bool fUseStrictTypeMatching = false);

	virtual VisPropListExport ~CVisSDIFStream(void);


	virtual VisPropListExport const char *FileName(void) const;


	virtual VisPropListExport void Open(void);
	virtual VisPropListExport void Close(void);


private:
	std::ifstream m_ifstream;
	std::string m_strFileName;
};



//
// Helper macros
//
// (The three macros at the bottom of this file are the most useful.)
//

#define VIS_DECLARE_SDI_FIND_TYPES_DEFAULT(Storage, TObject)	\
	Storage void VisSDIFindTypes(CVisSDIStream& refsdistream,	\
			TObject const& refobj);

#define VIS_DECLARE_SDI_FIND_TYPES_PL(Storage, TObject)			\
	Storage void VisSDIFindTypes(CVisSDIStream& refsdistream,	\
			TObject const& refobj);

#define VIS_DECLARE_SDI_FIND_TYPES_NOOP(Storage, TObject)	\
	Storage void VisSDIFindTypes(CVisSDIStream& refsdistream,\
			TObject const& refobj);


#define VIS_DECLARE_SDI_READ_VALUE_USING_CLASS_METHOD(Storage, TObject)	\
	Storage void VisSDReadValue(CVisSDIStream& refsdistream,				\
			TObject& refobj);

#define VIS_DECLARE_SDI_READ_VALUE_USING_CLASS_PL(Storage, TObject)	\
	Storage void VisSDReadValue(CVisSDIStream& refsdistream,			\
			TObject& refobj);

#define VIS_DECLARE_SDI_READ_VALUE_NO_IO(Storage, TObject)	\
	Storage void VisSDReadValue(CVisSDIStream& refsdistream,	\
			TObject& refobj);


#define VIS_DECLARE_SDI_READ_OBJ_DEFAULT(Storage, TObject)	\
	Storage void VisSDReadObj(CVisSDIStream& refsdistream,	\
			TObject& refobj);

#define VIS_DECLARE_SDI_READ_OBJ_NO_IO(Storage, TObject)	\
	Storage void VisSDReadObj(CVisSDIStream& refsdistream,	\
			TObject& refobj);


#define VIS_DECLARE_SDI_READ_OP_DEFAULT(Storage, TObject)			\
	Storage CVisSDIStream& operator>>(CVisSDIStream& refsdistream,	\
			TObject& refobj);



#define VIS_DECLARE_SDI_CLASS(Storage, TObject)						\
	VIS_DECLARE_SDI_FIND_TYPES_DEFAULT(Storage, TObject)			\
	VIS_DECLARE_SDI_READ_VALUE_USING_CLASS_METHOD(Storage, TObject)	\
	VIS_DECLARE_SDI_READ_OBJ_DEFAULT(Storage, TObject)				\
	VIS_DECLARE_SDI_READ_OP_DEFAULT(Storage, TObject)

#define VIS_DECLARE_SDI_CLASS_WITH_PL(Storage, TObject)			\
	VIS_DECLARE_SDI_FIND_TYPES_PL(Storage, TObject)				\
	VIS_DECLARE_SDI_READ_VALUE_USING_CLASS_PL(Storage, TObject)	\
	VIS_DECLARE_SDI_READ_OBJ_DEFAULT(Storage, TObject)			\
	VIS_DECLARE_SDI_READ_OP_DEFAULT(Storage, TObject)

#define VIS_DECLARE_SDI_CLASS_NO_IO(Storage, TObject)	\
	VIS_DECLARE_SDI_FIND_TYPES_NOOP(Storage, TObject)	\
	VIS_DECLARE_SDI_READ_VALUE_NO_IO(Storage, TObject)	\
	VIS_DECLARE_SDI_READ_OBJ_NO_IO(Storage, TObject)	\
	VIS_DECLARE_SDI_READ_OP_DEFAULT(Storage, TObject)


#define VIS_DEFINE_SDI_FIND_TYPES_DEFAULT(Storage, TObject)		\
	Storage void VisSDIFindTypes(CVisSDIStream& refsdistream,	\
			TObject const& refobj)								\
		{ refsdistream.UseObjType(refobj); }

#define VIS_DEFINE_SDI_FIND_TYPES_PL(Storage, TObject)			\
	Storage void VisSDIFindTypes(CVisSDIStream& refsdistream,	\
			TObject const& refobj)								\
		{ refsdistream.UsePropListObjType(refobj); }

#define VIS_DEFINE_SDI_FIND_TYPES_NOOP(Storage, TObject)		\
	Storage void VisSDIFindTypes(CVisSDIStream& refsdistream,	\
			TObject const& refobj)								\
		{}


#define VIS_DEFINE_SDI_READ_VALUE_USING_CLASS_METHOD(Storage, TObject)	\
	Storage void VisSDReadValue(CVisSDIStream& refsdistream,			\
			TObject& refobj)											\
		{ refobj.ReadValue(refsdistream); }

#define VIS_DEFINE_SDI_READ_VALUE_USING_CLASS_PL(Storage, TObject)	\
	Storage void VisSDReadValue(CVisSDIStream& refsdistream,		\
			TObject& refobj)										\
		{ refsdistream.ReadClassUsingPropList(refobj); }

#define VIS_DEFINE_SDI_READ_VALUE_NO_IO(Storage, TObject)	\
	Storage void VisSDReadValue(CVisSDIStream& refsdistream,\
			TObject& refobj)								\
		{ assert(0); }


#define VIS_DEFINE_SDI_READ_OBJ_DEFAULT(Storage, TObject)	\
	Storage void VisSDReadObj(CVisSDIStream& refsdistream,	\
			TObject& refobj)								\
		{													\
			VisSDIFindTypes(refsdistream, refobj);			\
															\
			refsdistream.StartObj(refobj);					\
			VisSDReadValue(refsdistream, refobj);			\
			refsdistream.EndObj();							\
		}

#define VIS_DEFINE_SDI_READ_OBJ_NO_IO(Storage, TObject)		\
	Storage void VisSDReadObj(CVisSDIStream& refsdistream,	\
			TObject& refobj)								\
		{ assert(0); }


#define VIS_DEFINE_SDI_READ_OP_DEFAULT(Storage, TObject)			\
	Storage CVisSDIStream& operator>>(CVisSDIStream& refsdistream,	\
			TObject& refobj)										\
		{ VisSDReadObj(refsdistream, refobj); return refsdistream; }



#define VIS_DEFINE_SDI_CLASS(Storage, TObject)						\
	VIS_DEFINE_SDI_FIND_TYPES_DEFAULT(Storage, TObject)				\
	VIS_DEFINE_SDI_READ_VALUE_USING_CLASS_METHOD(Storage, TObject)	\
	VIS_DEFINE_SDI_READ_OBJ_DEFAULT(Storage, TObject)				\
	VIS_DEFINE_SDI_READ_OP_DEFAULT(Storage, TObject)

#define VIS_DEFINE_SDI_CLASS_WITH_PL(Storage, TObject)			\
	VIS_DEFINE_SDI_FIND_TYPES_PL(Storage, TObject)				\
	VIS_DEFINE_SDI_READ_VALUE_USING_CLASS_PL(Storage, TObject)	\
	VIS_DEFINE_SDI_READ_OBJ_DEFAULT(Storage, TObject)			\
	VIS_DEFINE_SDI_READ_OP_DEFAULT(Storage, TObject)

#define VIS_DEFINE_SDI_CLASS_NO_IO(Storage, TObject)	\
	VIS_DEFINE_SDI_FIND_TYPES_NOOP(Storage, TObject)	\
	VIS_DEFINE_SDI_READ_VALUE_NO_IO(Storage, TObject)	\
	VIS_DEFINE_SDI_READ_OBJ_NO_IO(Storage, TObject)		\
	VIS_DEFINE_SDI_READ_OP_DEFAULT(Storage, TObject)




#define VIS_SDI_FIND_TYPES_DEFAULT(TObject)	\
	VIS_DEFINE_SDI_FIND_TYPES_DEFAULT(inline, TObject)

#define VIS_SDI_FIND_TYPES_PL(TObject)	\
	VIS_DEFINE_SDI_FIND_TYPES_PL(inline, TObject)

#define VIS_SDI_FIND_TYPES_NOOP(TObject)	\
	VIS_DEFINE_SDI_FIND_TYPES_NOOP(inline, TObject)


#define VIS_SDI_READ_VALUE_USING_CLASS_METHOD(TObject)	\
	VIS_DEFINE_SDI_READ_VALUE_USING_CLASS_METHOD(inline, TObject)

#define VIS_SDI_READ_VALUE_USING_CLASS_PL(TObject)	\
	VIS_DEFINE_SDI_READ_VALUE_USING_CLASS_PL(inline, TObject)

#define VIS_SDI_READ_VALUE_NO_IO(TObject)	\
	VIS_DEFINE_SDI_READ_VALUE_NO_IO(inline, TObject)


#define VIS_SDI_READ_OBJ_DEFAULT(TObject)	\
	VIS_DEFINE_SDI_READ_OBJ_DEFAULT(inline, TObject)

#define VIS_SDI_READ_OBJ_NO_IO(TObject)		\
	VIS_DEFINE_SDI_READ_OBJ_NO_IO(inline, TObject)


#define VIS_SDI_READ_OP_DEFAULT(TObject)	\
	VIS_DEFINE_SDI_READ_OP_DEFAULT(inline, TObject)


#define VIS_TEMPLATE_SDI_CLASS(TTemplate, TObject)	\
	template<class TTemplate >						\
	VIS_SDI_FIND_TYPES_DEFAULT(TObject)				\
	template<class TTemplate >						\
	VIS_SDI_READ_VALUE_USING_CLASS_METHOD(TObject)	\
	template<class TTemplate >						\
	VIS_SDI_READ_OBJ_DEFAULT(TObject)				\
	template<class TTemplate >						\
	VIS_SDI_READ_OP_DEFAULT(TObject)

#define VIS_TEMPLATE_SDI_CLASS_WITH_PL(TTemplate, TObject)	\
	template<class TTemplate >								\
	VIS_SDI_FIND_TYPES_PL(TObject)							\
	template<class TTemplate >								\
	VIS_SDI_READ_VALUE_USING_CLASS_PL(TObject)				\
	template<class TTemplate >								\
	VIS_SDI_READ_OBJ_DEFAULT(TObject)						\
	template<class TTemplate >								\
	VIS_SDI_READ_OP_DEFAULT(TObject)

#define VIS_TEMPLATE_SDI_CLASS_NO_IO(TTemplate, TObject)\
	template<class TTemplate >							\
	VIS_SDI_FIND_TYPES_NOOP(TObject)					\
	template<class TTemplate >							\
	VIS_SDI_READ_VALUE_NO_IO(TObject)					\
	template<class TTemplate >							\
	VIS_SDI_READ_OBJ_NO_IO(TObject)						\
	template<class TTemplate >							\
	VIS_SDI_READ_OP_DEFAULT(TObject)



#define VIS_SDI_CLASS(TObject)						\
	VIS_SDI_FIND_TYPES_DEFAULT(TObject)				\
	VIS_SDI_READ_VALUE_USING_CLASS_METHOD(TObject)	\
	VIS_SDI_READ_OBJ_DEFAULT(TObject)				\
	VIS_SDI_READ_OP_DEFAULT(TObject)

#define VIS_SDI_CLASS_WITH_PL(TObject)			\
	VIS_SDI_FIND_TYPES_PL(TObject)				\
	VIS_SDI_READ_VALUE_USING_CLASS_PL(TObject)	\
	VIS_SDI_READ_OBJ_DEFAULT(TObject)			\
	VIS_SDI_READ_OP_DEFAULT(TObject)

#define VIS_SDI_CLASS_NO_IO(TObject)	\
	VIS_SDI_FIND_TYPES_NOOP(TObject)	\
	VIS_SDI_READ_VALUE_NO_IO(TObject)	\
	VIS_SDI_READ_OBJ_NO_IO(TObject)		\
	VIS_SDI_READ_OP_DEFAULT(TObject)
