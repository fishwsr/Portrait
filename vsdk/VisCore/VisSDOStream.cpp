// VisSDOStream.cpp
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


// LATER:  We might want to add a special SDVersion tag to streams.

// UNDONE:  << and >> operators between streams.  And allow streams to
// be used in prop lists (non-printing by default).

// UNDONE:  Do we always check the printing flag in PropTypeInfo?


// This should be the first preprocessor statement in the file so that
// automatic precompiled headers work correctly.
#ifdef VIS_CORE_BUILD
	#include "VisCorePch.h"
	#include "VisCoreProj.h"
#else
	#include "VisPropListProj.h"
#endif


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG


#ifndef VIS_NO_SD_STREAM

CVisSDOStream::CVisSDOStream(void)
  : m_refostream(std::cout),
	m_iLevel(- 1),
	m_vectorestate(),
	m_mapTypedef(),
	m_setproptypeinfo(),
	m_fLotsOfNewLines(false),
	m_fStartOfLine(true),
	m_fUseNewlineNext(false),
	m_fIgnoreComments(false),
	m_fTypeBeforeDescription(true),
	m_fOpen(true)
{
	PushState(estateObjList);
}

CVisSDOStream::CVisSDOStream(std::ostream& refostream)
  : m_refostream(refostream),
	m_iLevel(- 1),
	m_vectorestate(),
	m_mapTypedef(),
	m_setproptypeinfo(),
	m_fLotsOfNewLines(false),
	m_fStartOfLine(true),
	m_fUseNewlineNext(false),
	m_fIgnoreComments(false),
	m_fTypeBeforeDescription(true),
	m_fOpen(false)
{
	PushState(estateObjList);
}


CVisSDOStream::~CVisSDOStream(void)
{
	assert(m_iLevel == 0);
	assert(StateCur() == estateObjList);
}


const char *CVisSDOStream::FileName(void) const
{
	return 0;
}

bool CVisSDOStream::UseType(CVisPropTypeInfoBase *pproptypeinfo)
{
	assert(pproptypeinfo != 0);

	bool fNewType = ((!pproptypeinfo->IsSimpleType())
			&& (m_setproptypeinfo.find(pproptypeinfo) == m_setproptypeinfo.end()));
	if (fNewType)
	{
		m_setproptypeinfo.insert(pproptypeinfo);

		if ((pproptypeinfo->AlwaysUseTypedef())
				&& (!HasTypedef(pproptypeinfo)))
		{
			CVisSDOTypedef sdotypedefT;
			UseTypedef(pproptypeinfo, sdotypedefT);
		}
	}

	return fNewType;
}

bool CVisSDOStream::UsePropListType(CVisPropTypeInfoBase *pproptypeinfo,
		const void *pvObj)
{
	bool fNewType = UseType(pproptypeinfo);

	CVisPropList *pproplist = pproptypeinfo->PPropList();
	assert(pproplist != 0);
	CVisPropList::const_iterator iteratorCur = pproplist->begin();
//	We could use iteratorMapLim instead of looping from 0 to size() below.
//	CVisPropList::const_iterator iteratorLim = pproplist->end();
//	for (; iteratorCur != iteratorLim; ++iteratorCur)
	int iLim = pproplist->CProp();
	for (int i = 0; i != iLim; ++iteratorCur, ++i)
	{
		if ((pproplist->IsPrinted(iteratorCur))
				&& ((fNewType)
					|| (pproplist->PropTypeInfo(iteratorCur).FContainsPropLists())))
		{
			(*iteratorCur).SDOFindTypes(*this, pvObj);
		}
	}

	return fNewType;
}

void CVisSDOStream::OpenGroup(bool fNewLine)
{
	assert(m_fOpen);

	ValueSep(fNewLine);

	m_refostream << ((char) echOpenGroup);

	assert((StateCur() == estateValueField)
			|| (StateCur() == estateValueList)
			|| (StateCur() == estateStartOfGroup));
	PushState(estateStartOfGroup);

	m_fStartOfLine = false;
	m_fUseNewlineNext = fNewLine;
}

void CVisSDOStream::CloseGroup(bool fNewLine)
{
	assert(m_fOpen);
	assert(m_iLevel > 0);

	assert((StateCur() == estateValueList)
			|| (StateCur() == estateStartOfGroup)
			|| (StateCur() == estateObjList));
	PopState();
	if (StateCur() == estateStartOfGroup)
		SetStateCur(estateValueList);
	else if (StateCur() == estateValueField)
		SetStateCur(estateEndOfObj);

	WhitespaceSep(fNewLine);
	m_refostream << ((char) echCloseGroup);

	m_fStartOfLine = false;
	m_fUseNewlineNext = fNewLine;
}

void CVisSDOStream::NewLine(void)
{
	OpenIfNeeded();

	m_refostream << ((char) echNewLine);
	Indent();
	m_fStartOfLine = true;
	m_fUseNewlineNext = false;
}

void CVisSDOStream::NewLine(int cNewLine)
{
	OpenIfNeeded();

	// LATER:  Should we indent each empty line?
	while (cNewLine-- > 0)
		m_refostream << ((char) echNewLine);
	Indent();
	m_fStartOfLine = true;
	m_fUseNewlineNext = false;
}

void CVisSDOStream::Indent(void)
{
	int cSpace = 2 * m_iLevel;
	while (--cSpace >= 0)
		m_refostream << ((char) echSpace);
}

void CVisSDOStream::Comment(const char *sz, bool fStartOnNewLine)
{
	// LATER:  Two small bugs exist with comments:
	// 1.  When used between values, the value separator character (',') is
	// placed at the start of the line following the comment.
	// 2.  When used at the end of a group, the end group character ('}') may
	// be indented one level too far on the line following the comment.
	if (!m_fIgnoreComments)
	{
		OpenIfNeeded();

		if (!m_fStartOfLine)
		{
			if ((fStartOnNewLine) || (m_fUseNewlineNext))
				NewLine();
			else 
				m_refostream << ((char) echSpace) << ((char) echSpace);
		}

		// LATER:  We could add code to word-break a long comment and
		// put it on multiple lines.  (Also, should we check that the
		// string doesn't contain any control characters?)
		m_refostream << "//";
		if ((sz != 0) && (*sz != 0))
			m_refostream << ((char) echSpace) << sz;
		m_fStartOfLine = false;

		m_fUseNewlineNext = false;
		NewLine();
	}
}


void CVisSDOStream::WriteSharedPrefix(void)
{
	assert(m_fOpen);

	WhitespaceSep();

	m_refostream << "shared ";

	assert((StateCur() == estateObjList)
			|| (StateCur() == estateStartOfGroup));
}

void CVisSDOStream::StartObjType(const char *szTypeIOName,
		const char *szName)
{
	assert(szTypeIOName != 0);
	assert(*szTypeIOName != 0);

	OpenIfNeeded();

	WhitespaceSep();

	m_refostream << szTypeIOName << ((char) echSpace);
	m_fStartOfLine = false;

	if ((szName == 0) || (*szName == 0))
		m_refostream << ((char) echNoName);
	else
		m_refostream << szName;

	m_refostream << ((char) echSpace) << ((char) echEquals);

	assert((StateCur() == estateObjList)
			|| (StateCur() == estateStartOfGroup));
	SetStateCur(estateValueField);
}


void CVisSDOStream::StartObjType(const char *szTypeIOName,
		const CVisDim& refdim, const char *szName)
{
	assert(szTypeIOName != 0);
	assert(*szTypeIOName != 0);

	OpenIfNeeded();

	WhitespaceSep();

	m_refostream << szTypeIOName;
	ArraySuffix(refdim);
	m_refostream << ((char) echSpace);
	m_fStartOfLine = false;

	if ((szName == 0) || (*szName == 0))
		m_refostream << ((char) echNoName);
	else
		m_refostream << szName;

	m_refostream << ((char) echSpace) << ((char) echEquals);

	assert((StateCur() == estateObjList)
			|| (StateCur() == estateStartOfGroup));
	SetStateCur(estateValueField);
}

void CVisSDOStream::ArraySuffix(const CVisDim& refdim)
{
	assert(m_fOpen);
	if (refdim.CObj() > 1)
	{
		for (int iDim = 0; iDim < refdim.CDim(); ++iDim)
			m_refostream << '[' << refdim[iDim] << ']';
	}
}

void CVisSDOStream::EndObj(bool fNewLine)
{
	assert(m_fOpen);

	// Note that this intentionally ignores the (m_fUseNewlineNext) flag.
	m_refostream << ((char) echFieldSep);

	assert(StateCur() == estateEndOfObj);
	SetStateCur(estateObjList);

	m_fStartOfLine = false;
	if (fNewLine)
		m_fUseNewlineNext = true;
}

void CVisSDOStream::EndValue(bool fNewLine)
{
	assert(m_fOpen);

	m_fStartOfLine = false;

	assert((StateCur() == estateValueField)
			|| (StateCur() == estateValueList)
			|| (StateCur() == estateStartOfGroup));
	if (StateCur() == estateValueField)
		SetStateCur(estateEndOfObj);
	else if (StateCur() == estateStartOfGroup)
		SetStateCur(estateValueList);

	if (fNewLine)
		NewLineNext();
}

void CVisSDOStream::ValueSep(bool fNewLine)
{
	assert(m_fOpen);
	if ((StateCur() != estateStartOfGroup)
			&& (StateCur() != estateValueField))
	{
		// Note that this intentionally ignores the (m_fUseNewlineNext) flag.
		m_refostream << ((char) echValueSep);
		m_fStartOfLine = false;
	}

	WhitespaceSep(fNewLine);
}

void CVisSDOStream::WhitespaceSep(bool fNewLine)
{
	assert(m_fOpen);
	if (!m_fStartOfLine)
	{
		if ((fNewLine) || (m_fUseNewlineNext))
			NewLine();
		else
			m_refostream << ((char) echSpace);

		m_fUseNewlineNext = false;
	}
}

void CVisSDOStream::WriteClassPropList(CVisPropTypeInfoBase *pproptypeinfo,
		const void *pvObjOffset)
{
	assert(pproptypeinfo != 0);
	assert(pvObjOffset != 0);

	WriteClassPropList(*(pproptypeinfo->PPropList()), pvObjOffset,
			PSDOTypedef(pproptypeinfo));
}

void CVisSDOStream::WriteClassPropList(CVisPropList& refproplist,
		const void *pvObjOffset, const CVisSDOTypedef *ptypedef)
{
	assert(m_fOpen);
	assert(&refproplist != 0);

	// Check for the special VIS_SZ_SD_USER_PROP_LIST property that is
	// sometimes used to store usre-defined properties.
	CVisPropList::const_iterator iteratorPropList = refproplist.end();
	CVisPropList *pproplistUser = 0;

	if ((ptypedef == 0) || (!ptypedef->empty()))
	{
		iteratorPropList
				= refproplist.IteratorFromPropName(VIS_SZ_SD_USER_PROP_LIST);
		if (iteratorPropList != refproplist.end())
		{
			assert(refproplist.TypeInfo(iteratorPropList)
					== typeid(CVisPropList));
			
			// The following code get a reference to the CVisPropList that
			// contains user-defined properties.
			pproplistUser = (CVisPropList *) (void *)
					(((BYTE *) (*iteratorPropList).PvObj())
							+ ((int) pvObjOffset));
		}
	}

	bool fUseGroup = (ptypedef == 0) || (ptypedef->size() != 1);
	bool fUseNewLineWithGroup = true;
	if (fUseGroup)
	{
		if (ptypedef != 0)
		{
			// LATER:  We could store the fUseNewLineWithGroup in the typedef
			// structure so that it only needs to be computed when we're told
			// about the typedef (instead of each time that the typedef is used.)
			fUseNewLineWithGroup = false;
			for (int i = 0; i < ptypedef->size(); ++i)
			{
				const char *szName = (*ptypedef)[i].c_str();
				CVisPropList::const_iterator iteratorCur
						= refproplist.IteratorFromPropName(szName);

				if (iteratorCur == refproplist.end())
				{
					assert(pproplistUser != 0);
					iteratorCur = pproplistUser->IteratorFromPropName(szName);
				}

				if (!refproplist.PropTypeInfo(iteratorCur).IsSimpleType())
				{
					fUseNewLineWithGroup = true;
					break;
				}
			}
		}

		OpenGroup(fUseNewLineWithGroup);
	}

	if (ptypedef == 0)
	{
		bool fPrintUserPropList = ((pproplistUser != 0)
				&& refproplist.IsPrinted(iteratorPropList));
		if (fPrintUserPropList)
			refproplist.SetPrinting(iteratorPropList, false);

		refproplist.WriteValue(*this, false, pvObjOffset);

		if (fPrintUserPropList)
		{
			refproplist.SetPrinting(iteratorPropList, true);

			// Later:  We might want to add a comment that the following
			// properties are user-defined.
			pproplistUser->WriteValue(*this, false, pvObjOffset);
		}
	}
	else
	{
		for (int i = 0; i < ptypedef->size(); ++i)
		{
			const char *szName = (*ptypedef)[i].c_str();
			CVisPropList::const_iterator iteratorCur
					= refproplist.IteratorFromPropName(szName);

			if (iteratorCur != refproplist.end())
			{
				// Found the value in the PropList for the class.
				(*iteratorCur).WriteValue(*this, true, pvObjOffset);
			}
			else
			{
				// We expect the value to be in the list of user-defined
				// properties.  (We assert that there is a list of user-defined
				// properties and that we find the value we need.)
				// LATER:  We currently don't allow user-define properties to be
				// used in typedefs.  If we decide that we won't add such support
				// in the future, we can remove this case.
				assert(pproplistUser != 0);
				CVisPropList::const_iterator iteratorUserProp
						= pproplistUser->IteratorFromPropName(szName);
				assert(iteratorUserProp != pproplistUser->end());
				(*iteratorCur).WriteValue(*this, true);
			}
		}
	}

	if (fUseGroup)
		CloseGroup(fUseNewLineWithGroup);
}


void CVisSDOStream::UseTypedef(CVisPropTypeInfoBase *pproptypeinfo,
		const CVisSDOTypedef& reftypedef)
{
	assert(pproptypeinfo != 0);

	OpenIfNeeded();

	// This should only be called in the root-level object list (since
	// we ask objects about the types that they use before we start printing
	// them).  If we get this assert, it probably means that the VisSDOFindTypes
	// function for the object being written (on an object that it contains)
	// did not tell the CVisSDOStream about this type (pproptypeinfo->m_strName).
	assert(m_iLevel == 0);
	assert(StateCur() == estateObjList);

	CVisSDOTypedef& reftypedefInMap = m_mapTypedef[pproptypeinfo];
	CVisPropList *pproplist = pproptypeinfo->PPropList();
	assert(pproplist != 0);

	if (reftypedef.empty())
	{
		// Special case:  Print the default fields in the typedef.
		reftypedefInMap.clear();
		if (pproplist->HasOrderedPropNameList())
		{
			int iLim = pproplist->POrderedPropNameList()->size();
			assert(iLim == pproplist->CProp());
			for (int i = 0; i != iLim; ++i)
			{
				const char *szProp
						= (*(pproplist->POrderedPropNameList()))[i].c_str();
				if (pproplist->IsPrinted(szProp))
					reftypedefInMap.push_back(szProp);
			}
		}
		else
		{
			CVisPropList::const_iterator iteratorCur = pproplist->begin();
//			We could use iteratorMapLim instead of looping from 0 to size() below.
//			CVisPropList::const_iterator iteratorLim = pproplist->end();
//			for (; iteratorCur != iteratorLim; ++iteratorCur)
			int iLim = pproplist->CProp();
			for (int i = 0; i != iLim; ++iteratorCur, ++i)
			{
				if ((*iteratorCur).IsPrinted())
					reftypedefInMap.push_back((*iteratorCur).Name());
			}
		}
	}
	else
	{
		reftypedefInMap = reftypedef;
	}

	// Add the typedef description to the stream.
	if (m_fUseNewlineNext)
		m_refostream << ((char) echNewLine);
	m_refostream << ((char) echNewLine);
	m_refostream << "typedef";
	if (m_fTypeBeforeDescription)
	{
		m_refostream << " " << pproptypeinfo->Name() << " =";
	}
	m_refostream << ((char) echNewLine) << "  ";
	m_refostream << ((char) echOpenGroup);
	m_refostream << ((char) echNewLine);

	// LATER:  Might want to add a comment that single-values won't be
	// grouped below.  (But that may require changes to the SDIStream
	// code to skip comments when reading typedefs.)

	for (int i = 0; i < reftypedefInMap.size(); ++i)
	{
		const char *szName = reftypedefInMap[i].c_str();
		CVisPropList::const_iterator iteratorCur
				= pproplist->IteratorFromPropName(szName);

		// If we get this assert, the typedef contains the name of a property
		// that is not in the property list for this class.
		// LATER:  Do we want to allow typedefs to include the names of
		// user-defined properties?  The code to check for user-defined properties
		// when using a typedef has been written.  The only difficulties is
		// getting the type name here.  (We might also want debug code to make sure
		// the the right type is used when printing the properties.)
		assert(iteratorCur != pproplist->end());
		m_refostream << "    " << pproplist->PropTypeInfo(iteratorCur).Name();
		if (pproplist->IsArray(iteratorCur))
			ArraySuffix((*iteratorCur).Dim());
		m_refostream << ' ' << szName;
		m_refostream << ((char) echFieldSep);
		m_refostream << ((char) echNewLine);
	}

	// LATER:  Not sure how to best format (indent) the typedef.
	m_refostream << "  " << ((char) echCloseGroup);
	if (!m_fTypeBeforeDescription)
	{
		m_refostream << ((char) echNewLine);
		m_refostream << "    " << pproptypeinfo->Name();
	}
	m_refostream << ((char) echFieldSep);
	m_refostream << ((char) echNewLine);
	m_refostream << ((char) echNewLine);

	m_fStartOfLine = true;
	m_fUseNewlineNext = false;
}

const CVisSDOTypedef *CVisSDOStream::PSDOTypedef(
		const CVisPropTypeInfoBase *pproptypeinfo) const
{
	CTypedefMap::const_iterator iteratorT = m_mapTypedef.find(pproptypeinfo);
	if (iteratorT == m_mapTypedef.end())
		return 0;

	return &(iteratorT->second);
}

CVisSDOTypedef *CVisSDOStream::PSDOTypedef(const CVisPropTypeInfoBase *pproptypeinfo)
{
	CTypedefMap::iterator iteratorT = m_mapTypedef.find(pproptypeinfo);
	if (iteratorT == m_mapTypedef.end())
		return 0;

	return &(iteratorT->second);
}

void CVisSDOStream::WriteCh(char ch)
{
	assert(m_fOpen);

	// Write a character surronded by single-quotes.
	// Assume that the character is a printable ASCII character.
	assert((ch >= ' ') && ((ch & 0x80) == 0));
	StartValue();
	m_refostream << ((char) echSingleQuote);
	if ((ch == (char) echSingleQuote) || (ch == (char) echEscape))
		m_refostream << ((char) echEscape);
	m_refostream << ch;
	m_refostream << ((char) echSingleQuote);
	EndValue();
}

void CVisSDOStream::WriteBool(bool f)
{
	StartValue();
	m_refostream << ((char) ((f) ? '1' : '0'));
	EndValue();
}

void CVisSDOStream::WriteSz(const char *sz)
{
	static bool fCheckedForSimpleType = false;
	static bool fSzIsSimpleType;

	if (!fCheckedForSimpleType)
		fSzIsSimpleType = VisPPropTypeInfoForClass(sz)->IsSimpleType();

	// Don't use a newline if there's only one value to print.
	StartValue((!fSzIsSimpleType) && (StateCur() != estateValueField));
	m_refostream << ((char) echDblQuote);
	if (sz != 0)
	{
		for (const char *pch = sz; *pch != 0; ++pch)
		{
			// Assume ASCII characters
			assert(((unsigned char) *pch) >= 0x20);
			assert(((*pch) & 0x80) == 0);
			if ((*pch == '\\') || (*pch == '"'))
				m_refostream << '\\';
			m_refostream << *pch;
		}
	}
	m_refostream << ((char) echDblQuote);
	EndValue(!fSzIsSimpleType);
}

void CVisSDOStream::WriteNum(const int w)
{
	StartValue();
	m_refostream << w;
	EndValue();
}

void CVisSDOStream::WriteNum(const unsigned int n)
{
	// Use hexadecimal with unsigned values
	char szBuf[64];
	sprintf(szBuf, "%#x", n);

	StartValue();
	m_refostream << szBuf;
	EndValue();
}

void CVisSDOStream::WriteNum(const long& refnum)
{
	StartValue();
	m_refostream << refnum;
	EndValue();
}

void CVisSDOStream::WriteNum(const unsigned long& refnum)
{
	// Use hexadecimal with unsigned values
	char szBuf[64];
	sprintf(szBuf, "%#x", refnum);

	StartValue();
	m_refostream << szBuf;
	EndValue();
}

void CVisSDOStream::WriteNum(const _int64& refnum)
{
	// std::stream doesn't support _int64 directly
	char szBuf[64];
	sprintf(szBuf, "%#I64d", refnum);

	StartValue();
	m_refostream << szBuf;
	EndValue();
}

void CVisSDOStream::WriteNum(const unsigned _int64& refnum)
{
	// Use hexadecimal with unsigned values
	// std::stream doesn't support _int64 directly
	char szBuf[64];
	sprintf(szBuf, "%#I64x", refnum);

	StartValue();
	m_refostream << szBuf;
	EndValue();
}

void CVisSDOStream::WriteNum(const float& refnum)
{
	StartValue();
	// LATER:  Does the locale affect the decimal point character used with
	// operator<<?  If so, we might want to use a string buf and sprintf here.
	m_refostream << refnum;
	EndValue();
}

void CVisSDOStream::WriteNum(const double& refnum)
{
	StartValue();
	// LATER:  Does the locale affect the decimal point character used with
	// operator<<?  If so, we might want to use a string buf and sprintf here.
	m_refostream << refnum;
	EndValue();
}

void CVisSDOStream::Open(void)
{
	assert(m_iLevel == 0);
	assert(StateCur() == estateObjList);
	assert(m_mapTypedef.empty());
	SetIsOpen(true);
	Comment("Microsoft Vision SDK Self-Decriping Steam Format (version 1)");
}

void CVisSDOStream::Close(void)
{
	SetIsOpen(false);
	m_mapTypedef.clear();
	m_setproptypeinfo.clear();
}


CVisSDOFStream::CVisSDOFStream(const char *szFileName)
  : CVisSDOStream(m_ofstream),
	m_ofstream(),
	m_strFileName(szFileName)
{
}

CVisSDOFStream::~CVisSDOFStream(void)
{
	if (IsOpen())
		Close();
}

const char *CVisSDOFStream::FileName(void) const
{
	return m_strFileName.c_str();
}

void CVisSDOFStream::Open(void)
{
	m_ofstream.open(FileName());

	if (!m_ofstream.is_open())
	{
		// Open failed.  Throw an exception.
		// LATER:  Do we want to add an option to not throw exceptions here?
		throw CVisFileIOError(FileName(), "Open failed", eviserrorOpen,
				"CVisSDOFStream::Open", __FILE__, __LINE__);
	}
	
	// Base class will call SetIsOpen(true);
	CVisSDOStream::Open();

#if VIS_INTERNAL_RELEASE || VIS_SDOSTREAM_ADD_FILENAME
	// Add a comment with the name of the original file.
	std::string strComment = "Original file name:  ";
	strComment += FileName();
	Comment(strComment.c_str());
#endif // VIS_INTERNAL_RELEASE || VIS_SDOSTREAM_ADD_FILENAME
}

void CVisSDOFStream::Close(void)
{
	// Base class will call SetIsOpen(false);
	CVisSDOStream::Close();
	
	m_ofstream.close();
}



VIS_DEFINE_SDO_CLASS_STD_NON_NUM(VisPropListExportDeclspec, bool)
VIS_DEFINE_SDO_CLASS_STD_NON_NUM(VisPropListExportDeclspec, char const *)
VIS_DEFINE_SDO_CLASS_STD_NON_NUM(VisPropListExportDeclspec, char *)
VIS_DEFINE_SDO_CLASS_STD_NON_NUM(VisPropListExportDeclspec, char)
VIS_DEFINE_SDO_CLASS_STD_NON_NUM(VisPropListExportDeclspec, std::string)


VIS_DEFINE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, signed char)
VIS_DEFINE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned char)
VIS_DEFINE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, short)
VIS_DEFINE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned short)
VIS_DEFINE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, int)
VIS_DEFINE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned int)
VIS_DEFINE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, long)
VIS_DEFINE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned long)
VIS_DEFINE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, _int64)
VIS_DEFINE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned _int64)
VIS_DEFINE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, float)
VIS_DEFINE_SDO_CLASS_STD_NUM(VisPropListExportDeclspec, double)


VIS_DEFINE_SDO_CLASS_NO_IO(VisPropListExportDeclspec, void const *)
VIS_DEFINE_SDO_CLASS_NO_IO(VisPropListExportDeclspec, void *)


#endif // VIS_NO_SD_STREAM
