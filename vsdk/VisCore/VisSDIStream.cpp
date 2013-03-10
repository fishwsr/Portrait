// VisSDIStream.cpp
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


// This should be the first preprocessor statement in the file so that
// automatic precompiled headers work correctly.
#ifdef VIS_CORE_BUILD
	#include "VisCorePch.h"
	#include "VisCoreProj.h"
#else
	#include "VisPropListProj.h"
#endif

// LATER:  Should we allow a numeric type (like int) to read
// a different numeric type (like long) from a file?  If so,
// what rules should we use (especially with the __int64 types)?
// What about overflow?


// UNDONE:  Make sure that we have PropTypeInfo for the object being read
// before we attempt to get pproptypeinfo from the type name that we read.
// (There may be more than one place that we need to check.)

// UNDONE:  Make sure that we skip unrecognized fields (without throwing
// exceptions) when reading classes as proerty lists.

// UNDONE:  Mark polymorphic types (in PropTypeInfo) to avoid strict
// type matching.

// UNDONE:  We need code to handle and unrecognized vector or deque type.
// Ideally, we should read the deque or vector as a deque or vector of
// CVisProp objects.  Even if we don't do that, we probably need code to
// skip reading a value as a prop list when the value is a deque or vector
// (since the group will contain a value list, not an object list).


// LATER:  If we read an unknown type, we might want to remember the order
// in which properties were read so that we can write them back in the
// same order.  (It shouldn't cause an error if we write them in a different
// order, but the file might be more human-readable if we preserve the order.)


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG


#ifndef VIS_NO_SD_STREAM

CVisSDIStream::CVisSDIStream(bool fUseStrictTypeMatching)
  : m_refistream(std::cin),
	m_iLevel(- 1),
	m_vectorestate(),
	m_mapTypedef(),
	m_iLine(1),
	m_fUseStrictTypeMatching(fUseStrictTypeMatching),
	m_fOpen(true)
{
	PushState(estateObjList);
}

CVisSDIStream::CVisSDIStream(std::istream& refistream,
		bool fUseStrictTypeMatching)
  : m_refistream(refistream),
	m_iLevel(- 1),
	m_vectorestate(),
	m_mapTypedef(),
	m_iLine(1),
	m_fUseStrictTypeMatching(fUseStrictTypeMatching),
	m_fOpen(false)
{
	PushState(estateObjList);
}


CVisSDIStream::~CVisSDIStream(void)
{
	assert(m_iLevel == 0);
	assert(StateCur() == estateObjList);
}

const char *CVisSDIStream::FileName(void) const
{
	return 0;
}

bool CVisSDIStream::SkipWhitespaceNoThrow(void)
{
	assert(m_fOpen);

	int wChNext;
	int wChLim = std::char_traits<char>::eof();
	while ((wChNext = m_refistream.peek()) != wChLim)
	{
		if (!IsWhitespaceCh(wChNext))
		{
			if (wChNext == ((char) echStartComment))
			{
				SkipThroughEndOfComment();
				continue;
			}
			else
				break;
		}

		if (wChNext == echNewLine)
			++ m_iLine;

		m_refistream.get();
	}

	// Returns true if not at EOF.
	return (wChNext != wChLim);
}

void CVisSDIStream::SkipWhitespace(void)
{
	if (!SkipWhitespaceNoThrow())
	{
		throw CVisSDIError("Unexpected EOF", "CVisSDIStream::SkipWhitespace");
	}
}

void CVisSDIStream::SkipAssignment(void)
{
	SkipWhitespace();
	if (m_refistream.peek() != ((char) echEquals))
	{
		throw CVisSDIError("Assignment character not found",
				"CVisSDIStream::SkipAssignment", ILine());
	}
	m_refistream.get();
}

void CVisSDIStream::SkipThroughEndOfString(void)
{
	int wChCur;
	int wChLim = std::char_traits<char>::eof();
	while ((wChCur = m_refistream.get()) != wChLim)
	{
		// We don't expect newline characters in strings.
		if (wChCur == (char) echNewLine)
		{
			throw CVisSDIError("Corrupt string found",
					"CVisSDIStream::SkipThroughEndOfString", ILine());
		}
		if (wChCur == (echDblQuote))
			break;
		if (wChCur == (echEscape))
		{
			wChCur = m_refistream.get();
			if (wChCur == wChLim)
				break;
		}
	}

	if (wChCur == wChLim)
	{
		throw CVisSDIError("Unexpected EOF",
				"CVisSDIStream::SkipThroughEndOfString");
	}
}

void CVisSDIStream::SkipThroughEndOfGroup(void)
{
	int iLevel = 1;
	int wChCur;
	int wChLim = std::char_traits<char>::eof();
	while ((wChCur = m_refistream.get()) != wChLim)
	{
		if (wChCur == (echDblQuote))
		{
			SkipThroughEndOfString();
		}
		else if (wChCur == (echStartComment))
		{
			SkipThroughEndOfComment();
		}
		else if (wChCur == (echOpenGroup))
		{
			++ iLevel;
		}
		else if (wChCur == (echCloseGroup))
		{
			if (--iLevel == 0)
				break;
		}
	}

	if (wChCur == wChLim)
	{
		throw CVisSDIError("Unexpected EOF",
				"CVisSDIStream::SkipThroughEndOfGroup");
	}
}

void CVisSDIStream::SkipThroughEndCh(int chEnd)
{
	assert((chEnd == echFieldSep) || (chEnd == echValueSep));
	int wChCur;
	int wChLim = std::char_traits<char>::eof();
	while ((wChCur = m_refistream.get()) != wChLim)
	{
		if (wChCur == echDblQuote)
			SkipThroughEndOfString();
		else if (wChCur == echStartComment)
			SkipThroughEndOfComment();
		else if (wChCur == echOpenGroup)
			SkipThroughEndOfGroup();
		else if (wChCur == chEnd)
			break;
	}

	if (wChCur == wChLim)
	{
		throw CVisSDIError("Unexpected EOF",
				"CVisSDIStream::SkipThroughEndCh");
	}
}

void CVisSDIStream::SkipValue(void)
{
	assert((StateCur() == estateValueField)
			|| (StateCur() == estateValueList)
			|| (StateCur() == estateStartOfGroup));

	if (StateCur() == estateValueField)
		SkipThroughEndOfField();
	else
		SkipThroughEndOfValue();
}

void CVisSDIStream::SkipThroughEndOfComment(void)
{
	int wChCur;
	int wChLim = std::char_traits<char>::eof();
	while ((wChCur = m_refistream.get()) != wChLim)
	{
		if (wChCur == (echNewLine))
			break;
	}

	if (wChCur == wChLim)
	{
		throw CVisSDIError("Unexpected EOF",
				"CVisSDIStream::SkipThroughEndOfComment");
	}

	++ m_iLine;
}

void CVisSDIStream::ReadToken(std::string& refstr, bool fAllowValueSep)
{
	assert(refstr.empty());

	SkipWhitespace();

	int wChNext;
	int wChLim = std::char_traits<char>::eof();
	while ((wChNext = m_refistream.peek()) != wChLim)
	{
		if ((IsWhitespaceCh(wChNext)) || (wChNext == ';')|| (wChNext == '}')
				|| (wChNext == '{') || (!fAllowValueSep && (wChNext == ','))
				|| (wChNext == '='))
			break;

		if (wChNext == ((char) echStartComment))
		{
			SkipThroughEndOfComment();
			break;
		}

		// LATER:  This could be inefficient if we have to check every
		// character in the string to find where it ends.
		refstr += ((char) wChNext);
		m_refistream.get();
	}

	if (wChNext == wChLim)
	{
		throw CVisSDIError("Unexpected EOF",
				"CVisSDIStream::ReadToken");
	}
}

void CVisSDIStream::ReadTypedef(void)
{
	assert(m_iLevel == 0);
	assert(StateCur() == estateObjList);

	std::string strTypedefName;
	CVisSDITypedef sditypedefT;
	CVisSDITypedef *psditypedef = &sditypedefT;

	SkipWhitespace();
	bool fTypeBeforeDescription = !AtStartOfGroup();

	if (fTypeBeforeDescription)
	{
		ReadType(strTypedefName);

		// LATER:  Asserts that it's a valid type name.
		assert(!strTypedefName.empty());

		psditypedef = &(m_mapTypedef[strTypedefName]);

		// Skip an (optional) assignment operator after the type name.
		SkipWhitespace();
		if (m_refistream.peek() == ((char) echEquals))
			m_refistream.get();
	}

	SetStateCur(estateValueField);
	OpenGroup();

	while (!AtEndOfGroup())
	{
		psditypedef->push_back(SVisSDITypedefInfo());
		ReadTypeNameAndDim(psditypedef->back().strType, psditypedef->back().strName,
				psditypedef->back().dim, &(psditypedef->back().fShared), true);

		if ((psditypedef->back().strType.empty())
				|| (psditypedef->back().strName.empty()))
		{
			throw CVisSDIError("Invalid typedef found",
					"CVisSDIStream::ReadTypedef", ILine());
		}

		EndObj();
	}

	CloseGroup();

	if (!fTypeBeforeDescription)
	{
		SkipWhitespace();

		// ReadType will read the type name and call EndObj.
		ReadType(strTypedefName);

		// LATER:  Asserts that it's a valid type name.
		assert(!strTypedefName.empty());

		// Add the typedef to the map.
		m_mapTypedef[strTypedefName] = *psditypedef;
	}

	EndObj();
}

void CVisSDIStream::ReadTypeNameAndDim(std::string& refstrType,
		std::string& refstrName, CVisDim& refdim, bool *pfShared, bool fForTypedef)
{
	OpenIfNeeded();

	// We assume that the strings passed in are empty.
	assert(refstrType.empty());
	assert(refstrName.empty());

	if (pfShared != 0)
		*pfShared = false;

	int wChNext;
	int wChLim = std::char_traits<char>::eof();
	while ((wChNext = m_refistream.peek()) != wChLim)
	{
		if (IsWhitespaceCh(wChNext))
		{
			SkipWhitespace();
		}
		else if ((refstrType.empty()) && (refstrName == "typedef"))
		{
			ReadTypedef();
			refstrName = "";
			continue;
		}
		else if ((wChNext == '=') || (wChNext == ';') || (wChNext == '{')
				|| (wChNext == '}'))
		{
			break;
		}
		else
		{
			// Append the last token to the type string.
			if (!refstrName.empty())
			{
				if (refstrType.empty())
				{
					if (refstrName == "shared")
					{
						if (pfShared != 0)
							*pfShared = true;
						refstrName = "";
						continue;
					}
				}
				else
				{
					refstrType += ((char) echSpace);
				}
				refstrType += refstrName;
				refstrName = "";
			}

			ReadToken(refstrName, true);
		}
	}

	if (wChNext == wChLim)
	{
		throw CVisSDIError("Unexpected EOF",
				"CVisSDIStream::ReadTypeNameAndDim");
	}

	if ((refstrType.empty()) && (!fForTypedef))
	{
		throw CVisSDIError("Empty type name",
				"CVisSDIStream::ReadTypeNameAndDim", ILine());
	}
	if (refstrName.empty())
	{
		throw CVisSDIError("Empty value name",
				"CVisSDIStream::ReadTypeNameAndDim", ILine());
	}

	if (fForTypedef)
	{
		assert((StateCur() == estateObjList)
				|| (StateCur() == estateStartOfGroup)
				|| (StateCur() == estateEndOfObj));
		SetStateCur(estateEndOfObj);
	}
	else
	{
		SkipAssignment();

		assert((StateCur() == estateObjList)
				|| (StateCur() == estateStartOfGroup));
		SetStateCur(estateValueField);
	}

	if ((refstrName.length() == 1) && (refstrName[0] == ((char) echNoName)))
		refstrName = "";

	refdim = ParseArraySuffix(refstrType);
}


void CVisSDIStream::ReadType(std::string& refstrType)
{
	std::string strT;
	CVisDim dimT;
	bool boolT = false;
	ReadTypeNameAndDim(refstrType, strT, dimT, &boolT, true);

	if (dimT.CDim() != 0)
	{
		throw CVisSDIError("Invalid array typedef found",
				"CVisSDIStream::ReadType", ILine());
	}

	if (!strT.empty())
	{
		if (!refstrType.empty())
			refstrType += ((char) echSpace);
		refstrType += strT;
	}
}

void CVisSDIStream::StartObjType(const char *szType)
{
	std::string strType;
	std::string strName;
	bool fShared;
	CVisDim dim;
	ReadTypeNameAndDim(strType, strName, dim, &fShared);

	// We don't expect the value to be marked as shared.
	// (But if we get this assert, we should review the code, because it
	// might indicate a case that we overlooked when writing this method.)
	assert(!fShared);

	// Check that the type is as expected.
	assert(szType != 0);
	if (strType != szType)
	{
		// LATER:  Should we add code to allow similar standard numeric types
		// (say int and long) to be mismatched even if UseStrictTypeMatching()
		// is true?

#ifdef _DEBUG
		OutputDebugString("VisSDK Warning:  Property '");
		OutputDebugString(strName.c_str());
		OutputDebugString("' type '");
		OutputDebugString(strType.c_str());
		OutputDebugString("' doesn't match type '");
		OutputDebugString(szType);
		OutputDebugString("' in CVisSDIStream::StartObjType.\n");
#endif // _DEBUG

		if (UseStrictTypeMatching())
		{
			throw CVisSDIError("Type mismatch",
					"CVisSDIStream::StartObjType", ILine());
		}
	}

	// Check that this is not an array.  (StartObjType should not be
	// called when reading arrays.)
	if (dim.CDim() != 0)
	{
		throw CVisSDIError("Array type unexpected",
				"CVisSDIStream::StartObjType", ILine());
	}
}

void CVisSDIStream::EndObj(void)
{
	SkipWhitespace();
	if (m_refistream.peek() != ((char) echFieldSep))
	{
		throw CVisSDIError("Field separator (';') not found",
				"CVisSDIStream::EndObj", ILine());
	}
	m_refistream.get();

	assert(StateCur() == estateEndOfObj);
	SetStateCur(estateObjList);
}


bool CVisSDIStream::AtEndOfFile(void)
{
	// Should only be called in the top-level object list.
	assert(m_iLevel == 0);
	assert(StateCur() == estateObjList);

	OpenIfNeeded();

	return !SkipWhitespaceNoThrow();
}

bool CVisSDIStream::AtStartOfGroup(void)
{
	// LATER:  asserts?

	OpenIfNeeded();

	SkipWhitespace();
	return (m_refistream.peek() == ((char) echOpenGroup));
}

bool CVisSDIStream::AtEndOfGroup(void)
{
	assert(m_iLevel > 0);
	assert((StateCur() == estateValueList)
			|| (StateCur() == estateStartOfGroup)
			|| (StateCur() == estateObjList));

	OpenIfNeeded();

	SkipWhitespace();
	return (m_refistream.peek() == ((char) echCloseGroup));
}

void CVisSDIStream::OpenGroup(void)
{
	ValueSep();

	SkipWhitespace();
	if (m_refistream.peek() != ((char) echOpenGroup))
	{
		throw CVisSDIError("Open group ('{') not found",
				"CVisSDIStream::OpenGroup", ILine());
	}
	m_refistream.get();

	assert((StateCur() == estateValueField)
			|| (StateCur() == estateValueList)
			|| (StateCur() == estateStartOfGroup));
	PushState(estateStartOfGroup);
}

void CVisSDIStream::CloseGroup(void)
{
	assert(m_iLevel > 0);

	SkipWhitespace();
	if (m_refistream.peek() != ((char) echCloseGroup))
	{
		throw CVisSDIError("Open group ('}') not found",
				"CVisSDIStream::CloseGroup", ILine());
	}
	m_refistream.get();

	assert((StateCur() == estateValueList)
			|| (StateCur() == estateStartOfGroup)
			|| (StateCur() == estateObjList));
	PopState();
	if (StateCur() == estateStartOfGroup)
		SetStateCur(estateValueList);
	else if (StateCur() == estateValueField)
		SetStateCur(estateEndOfObj);
}


void CVisSDIStream::EndValue(void)
{
	assert((StateCur() == estateValueField)
			|| (StateCur() == estateValueList)
			|| (StateCur() == estateStartOfGroup));
	if (StateCur() == estateValueField)
		SetStateCur(estateEndOfObj);
	else if (StateCur() == estateStartOfGroup)
		SetStateCur(estateValueList);
}

void CVisSDIStream::ValueSep(void)
{
	if ((StateCur() != estateStartOfGroup)
			&& (StateCur() != estateValueField))
	{
		SkipWhitespace();
		if (m_refistream.peek() != ((char) echValueSep))
		{
			throw CVisSDIError("Value separator (',') not found",
					"CVisSDIStream::ValueSep", ILine());
		}
		m_refistream.get();
	}

	SkipWhitespace();
}


bool CVisSDIStream::IsValidArraySuffix(const char *sz)
{
	enum Expect
	{
		expectOpen,
		expectNumStart,
		expectNumEnd,
		expectClose
	};

	assert(sz != 0);

	Expect expect = expectOpen;
	int cDim = 0;
	for (const char *pch = sz; *pch != 0; ++pch)
	{
		switch (expect)
		{
		case expectOpen:
			if (*pch == ((char) echStartArrayDim))
			{
				++ cDim;
				expect = expectNumStart;
			}
			else if (*pch != ((char) echSpace))
			{
				return false;
			}
			break;

		case expectNumStart:
			if ((*pch >= '0') && (*pch <= '9'))
				expect = expectNumEnd;
			else if (*pch != ((char) echSpace))
				return false;
			break;

		case expectNumEnd:
			if ((*pch < '0') || (*pch > '9'))
			{
				if (*pch == ((char) echEndArrayDim))
					expect = expectOpen;
				else if (*pch == ((char) echSpace))
					expect = expectClose;
				else
					return false;
			}
			break;

		case expectClose:
			if (*pch == ((char) echEndArrayDim))
				expect = expectOpen;
			else if (*pch != ((char) echSpace))
				return false;
			break;
		}
	}
	return ((cDim <= 4) && (expect == expectOpen));
}


CVisDim CVisSDIStream::ParseArraySuffix(std::string& refstr)
{
	enum Expect
	{
		expectOpen,
		expectNumStart,
		expectNumEnd,
		expectClose
	};

	Expect expect = expectOpen;
	int iDim = - 1;
	int rgwDim[4] = {0, 0, 0, 0};

	int ichSuffix = refstr.find('[');
	if (ichSuffix != std::basic_string<char>::npos)
	{
		for (const char *pch = refstr.c_str() + ichSuffix; *pch != 0; ++pch)
		{
			switch (expect)
			{
			case expectOpen:
				if (*pch == ((char) echStartArrayDim))
				{
					if (++iDim >= 4)
					{
						throw CVisSDIError("Too many array dimensions", "CVisSDIStream::ParseArraySuffix", ILine());
					}
					expect = expectNumStart;
				}
				else if (*pch != ((char) echSpace))
				{
					throw CVisSDIError("Unexpected character",
							"CVisSDIStream::ParseArraySuffix", ILine());
				}
				break;

			case expectNumStart:
				if ((*pch >= '0') && (*pch <= '9'))
				{
					rgwDim[iDim] = (*pch - '0');
					expect = expectNumEnd;
				}
				else if (*pch != ((char) echSpace))
				{
					throw CVisSDIError("Unexpected character",
							"CVisSDIStream::ParseArraySuffix", ILine());
				}
				break;

			case expectNumEnd:
				if ((*pch >= '0') && (*pch <= '9'))
					rgwDim[iDim] = rgwDim[iDim] * 10 + (*pch - '0');
				else if (*pch == ((char) echEndArrayDim))
					expect = expectOpen;
				else if (*pch == ((char) echSpace))
					expect = expectClose;
				else
				{
					throw CVisSDIError("Unexpected character",
							"CVisSDIStream::ParseArraySuffix", ILine());
				}
				break;

			case expectClose:
				if (*pch == ((char) echEndArrayDim))
					expect = expectOpen;
				else if (*pch != ((char) echSpace))
				{
					throw CVisSDIError("Unexpected character",
							"CVisSDIStream::ParseArraySuffix", ILine());
				}
				break;
			}
		}

		// earse the array suffix
		refstr.erase(ichSuffix);

		// Assume that the type name is not empty and that there are no spaces
		// between the type name and the array suffix.
		if ((ichSuffix == 0) ||
				(refstr[ichSuffix - 1] == ((char) echSpace)))
		{
			throw CVisSDIError("Invalid type name",
					"CVisSDIStream::ParseArraySuffix", ILine());
		}
	}

	return CVisDim(rgwDim[0], rgwDim[1], rgwDim[2], rgwDim[3]);
}


void CVisSDIStream::ReadCh(char& refch)
{
	StartValue();

	// Assume a character surronded by single-quotes.
	int wChCur;
	int wChLim = std::char_traits<char>::eof();
	wChCur = m_refistream.get();
	assert(wChCur != wChLim);
	assert(wChCur == (char) echSingleQuote);

	wChCur = m_refistream.get();
	assert(wChCur != wChLim);
	assert((wChCur >= ' ') && ((wChCur & 0x80) == 0));
	if (wChCur == (char) echEscape)
	{
		// Special case:  We need to escape the single-quote and escape characters.
		wChCur = m_refistream.get();
		assert(wChCur != wChLim);
		assert((wChCur == (char) echSingleQuote) || (wChCur == (char) echEscape));
	}
	refch = (char) wChCur;

	wChCur = m_refistream.get();
	assert(wChCur != wChLim);
	assert(wChCur == (char) echSingleQuote);
	EndValue();

	if (wChCur == wChLim)
	{
		throw CVisSDIError("Unexpected EOF", "CVisSDIStream::ReadCh");
	}
}

void CVisSDIStream::ReadBool(bool& reff)
{
	StartValue();

	int wChCur;
	int wChLim = std::char_traits<char>::eof();
	wChCur = m_refistream.get();
	assert(wChCur != wChLim);
	assert((wChCur == '0') || (wChCur == '1'));
	reff = (wChCur != '0');

	EndValue();

	if (wChCur == wChLim)
	{
		throw CVisSDIError("Unexpected EOF", "CVisSDIStream::ReadBool");
	}
}

void CVisSDIStream::ReadNum(signed char& refnum)
{
	int wT;
	ReadNum(wT);
	refnum = (char) wT;
}

void CVisSDIStream::ReadNum(unsigned char& refnum)
{
	unsigned int nT;
	ReadNum(nT);
	refnum = (unsigned char) nT;
}

void CVisSDIStream::ReadNum(short& refnum)
{
	int wT;
	ReadNum(wT);
	refnum = (short) wT;
}

void CVisSDIStream::ReadNum(unsigned short& refnum)
{
	unsigned int nT;
	ReadNum(nT);
	refnum = (unsigned short) nT;
}

void CVisSDIStream::ReadNum(int& refnum)
{
	StartValue();
	// LATER:  We might want to use a string buf and sscanf instead of
	// operator>> to read hex values.  (But we expect that int values
	// are written using operator<<, so sscanf shouldn't be needed.)
	m_refistream >> refnum;
	EndValue();
}

void CVisSDIStream::ReadNum(unsigned int& refnum)
{
	StartValue();

	std::string strBuf;
	ReadToken(strBuf);
	sscanf(strBuf.c_str(), "%x", &refnum);

	EndValue();
}

void CVisSDIStream::ReadNum(long& refnum)
{
	StartValue();
	m_refistream >> refnum;
	EndValue();
}

void CVisSDIStream::ReadNum(unsigned long& refnum)
{
	StartValue();

	std::string strBuf;
	ReadToken(strBuf);
	sscanf(strBuf.c_str(), "%x", &refnum);

	EndValue();
}

void CVisSDIStream::ReadNum(_int64& refnum)
{
	StartValue();

	std::string strBuf;
	ReadToken(strBuf);
	sscanf(strBuf.c_str(), "%I64i", &refnum);

	EndValue();
}

void CVisSDIStream::ReadNum(unsigned _int64& refnum)
{
	StartValue();

	std::string strBuf;
	ReadToken(strBuf);
	sscanf(strBuf.c_str(), "%I64i", &refnum);

	EndValue();
}

void CVisSDIStream::ReadNum(float& refnum)
{
	StartValue();
	// LATER:  Does the locale affect the decimal point character used with
	// operator>>?  If so, we might want to use a string buf and sscanf here.
	m_refistream >> refnum;
	EndValue();
}

void CVisSDIStream::ReadNum(double& refnum)
{
	StartValue();
	// LATER:  Does the locale affect the decimal point character used with
	// operator>>?  If so, we might want to use a string buf and sscanf here.
	m_refistream >> refnum;
	EndValue();
}

void CVisSDIStream::ReadStr(std::string& refstr, bool fClearString)
{
	if (fClearString)
		refstr = "";

	StartValue();

	int wChCur;
	int wChLim = std::char_traits<char>::eof();

	// Strings begin and end with double-quote characters.
	wChCur = m_refistream.get();
	assert(wChCur == (char) echDblQuote);

	if ((wChCur != (char) echDblQuote) && (wChCur != wChLim))
	{
		throw CVisSDIError("Beginning of string not found",
				"CVisSDIStream::ReadStr", ILine());
	}

	while ((wChCur = m_refistream.get()) != wChLim)
	{
		// We don't expect newline characters in strings.
		assert(wChCur != (char) echNewLine);
		if (wChCur == (echDblQuote))
			break;
		if (wChCur == (echEscape))
		{
			wChCur = m_refistream.get();
			if (wChCur == wChLim)
				break;
		}

		refstr += (char) wChCur;
	}

	if (wChCur == wChLim)
	{
		throw CVisSDIError("Unexpected EOF", "CVisSDIStream::ReadStr");
	}

	EndValue();
}


const CVisSDITypedef *CVisSDIStream::PTypedef(const char *szType) const
{
	CTypedefMap::const_iterator iteratorTypedef = m_mapTypedef.find(szType);
	if (iteratorTypedef != m_mapTypedef.end())
		return &(iteratorTypedef->second);
	return 0;
}

void CVisSDIStream::ReadClassPropList(CVisPropTypeInfoBase *pproptypeinfo,
		void *pvObjOffset)
{
	assert(m_fOpen);
	assert(pproptypeinfo != 0);
	assert(pvObjOffset != 0);

	CVisPropList& refproplist = *(pproptypeinfo->PPropList());
	assert(&refproplist != 0);

	refproplist.ReadValue(*this, pvObjOffset);
}

void CVisSDIStream::Open(void)
{
	assert(m_iLevel == 0);
	assert(StateCur() == estateObjList);
	assert(m_mapTypedef.empty());
	SetIsOpen(true);
}

void CVisSDIStream::Close(void)
{
	SetIsOpen(false);
	m_mapTypedef.clear();
}


CVisSDIFStream::CVisSDIFStream(const char *szFileName,
		bool fUseStrictTypeMatching)
  : CVisSDIStream(m_ifstream, fUseStrictTypeMatching),
	m_ifstream(),
	m_strFileName(szFileName)
{
	assert((szFileName != 0) && (*szFileName != 0));
}

CVisSDIFStream::~CVisSDIFStream(void)
{
	if (IsOpen())
		Close();
}

const char *CVisSDIFStream::FileName(void) const
{
	return m_strFileName.c_str();
}

void CVisSDIFStream::Open(void)
{
	m_ifstream.open(FileName());

	if (!m_ifstream.is_open())
	{
		// Open failed.  Throw an exception.
		// LATER:  Do we want to add an option to not throw exceptions here?
		throw CVisFileIOError(FileName(), "Open failed", eviserrorOpen,
				"CVisSDIFStream::Open", __FILE__, __LINE__);
	}

	// Base class will call SetIsOpen(true);
	CVisSDIStream::Open();
}

void CVisSDIFStream::Close(void)
{
	// Base class will call SetIsOpen(false);
	CVisSDIStream::Close();
	
	m_ifstream.close();
}



VIS_DEFINE_SDI_CLASS_STD_NON_NUM(VisPropListExportDeclspec, bool)
VIS_DEFINE_SDI_CLASS_STD_NON_NUM(VisPropListExportDeclspec, char)
VIS_DEFINE_SDI_CLASS_STD_NON_NUM(VisPropListExportDeclspec, std::string)


VIS_DEFINE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, signed char)
VIS_DEFINE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned char)
VIS_DEFINE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, short)
VIS_DEFINE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned short)
VIS_DEFINE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, int)
VIS_DEFINE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned int)
VIS_DEFINE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, long)
VIS_DEFINE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned long)
VIS_DEFINE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, _int64)
VIS_DEFINE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, unsigned _int64)
VIS_DEFINE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, float)
VIS_DEFINE_SDI_CLASS_STD_NUM(VisPropListExportDeclspec, double)


VIS_DEFINE_SDI_CLASS_NO_IO(VisPropListExportDeclspec, void const *)
VIS_DEFINE_SDI_CLASS_NO_IO(VisPropListExportDeclspec, void *)



#endif // VIS_NO_SD_STREAM
