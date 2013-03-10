// VisDiagnosticsMsg.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
#include "VisCorePch.h"
#include "VisCoreProj.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FORCE_INT64     0x40000
#define FORCE_ANSI      0x10000
#define FORCE_UNICODE   0x20000

#define TCHAR_ARG   TCHAR
#define WCHAR_ARG   WCHAR
#define CHAR_ARG    char

void CVisString::Format(const char *szFormat, ...)
{
	va_list argList;
	va_start(argList, szFormat);
	Format(szFormat, argList);
}

void CVisString::Format(const char *szFormat, va_list argList)
{
	int nLength = FindLength(szFormat, argList);
	resize(nLength);
	resize(vsprintf(begin(), szFormat, argList));
}

void CVisDiagnosticMsg::Warning(int iLevel, const char *szFormat, ...)
{
	if(PGlobals()->DiagnosticsEnabled())
	{
		CVisString strWarning;
		va_list argList;
		va_start(argList, szFormat);
		strWarning.Format(szFormat, argList);

		CVisDiagnosticsManager::DoMessage(evisdiagnosticsWarning, iLevel, m_szFile, m_iLine,
										  strWarning.c_str());
	}
}

void CVisDiagnosticMsg::Error(int iLevel, const char *szFormat, ...)
{
	if(PGlobals()->DiagnosticsEnabled())
	{
		CVisString strError;
		va_list argList;
		va_start(argList, szFormat);
		strError.Format(szFormat, argList);

		CVisDiagnosticsManager::DoMessage(evisdiagnosticsError, iLevel, m_szFile, m_iLine,
										  strError.c_str());
	}
}

void CVisDiagnosticMsg::Trace(int iLevel, const char *szFormat, ...)
{
	if(PGlobals()->DiagnosticsEnabled())
	{
		CVisString strTrace;
		va_list argList;
		va_start(argList, szFormat);
		strTrace.Format(szFormat, argList);

		CVisDiagnosticsManager::DoMessage(evisdiagnosticsTrace, iLevel, m_szFile, m_iLine,
										  strTrace.c_str());
	}	
}

void CVisDiagnosticMsg::ProgressReport(int iLevel, int nCur, int nLim, const char *szFormat, ...)
{
	if(PGlobals()->DiagnosticsEnabled())
	{
		CVisString strProgress;
		va_list argList;
		va_start(argList, szFormat);
		strProgress.Format(szFormat, argList);

		CVisDiagnosticsManager::DoMessage(evisdiagnosticsProgress, iLevel, m_szFile, m_iLine,
										  strProgress.c_str(), nCur, nLim);
	}

}

int CVisString::FindLength(const char *szFormat, ...)
{
	va_list argList;
	va_start(argList, szFormat);
	return FindLength(szFormat, argList);
}

int CVisString::FindLength(const char *szFormat, va_list argList)
{
	va_list argListSave = argList;

	int nMaxLen = 0;
	for (const char *sz = szFormat; *sz != '\0'; sz = _tcsinc(sz))
	{
		// handle '%' character, but watch out for '%%'
		if (*sz != '%' || *(sz = _tcsinc(sz)) == '%')
		{
			nMaxLen += _tclen(sz);
			continue;
		}

		int nItemLen = 0;

		// handle '%' character with format
		int nWidth = 0;
		for (; *sz != '\0'; sz = _tcsinc(sz))
		{
			// check for valid flags
			if (*sz == '#')
				nMaxLen += 2;   // for '0x'
			else if (*sz == '*')
				nWidth = va_arg(argList, int);
			else if (*sz == '-' || *sz == '+' || *sz == '0' ||
				*sz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = _ttoi(sz);
			for (; *sz != '\0' && _istdigit(*sz); sz = _tcsinc(sz))
				;
		}
		assert(nWidth >= 0);

		int nPrecision = 0;
		if (*sz == '.')
		{
			// skip past '.' separator (width.precision)
			sz = _tcsinc(sz);

			// get precision and skip it
			if (*sz == '*')
			{
				nPrecision = va_arg(argList, int);
				sz = _tcsinc(sz);
			}
			else
			{
				nPrecision = _ttoi(sz);
				for (; *sz != '\0' && _istdigit(*sz); sz = _tcsinc(sz))
					;
			}
			assert(nPrecision >= 0);
		}

		// should be on type modifier or specifier
		int nModifier = 0;
		if (_tcsncmp(sz, _T("I64"), 3) == 0)
		{
			sz += 3;
			nModifier = FORCE_INT64;
		}
		else
		{
			switch (*sz)
			{
			// modifiers that affect size
			case 'h':
				nModifier = FORCE_ANSI;
				sz = _tcsinc(sz);
				break;
			case 'l':
				nModifier = FORCE_UNICODE;
				sz = _tcsinc(sz);
				break;

			// modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				sz = _tcsinc(sz);
				break;
			}
		}

		// now should be on specifier
		switch (*sz | nModifier)
		{
		// single characters
		case 'c':
		case 'C':
			nItemLen = 2;
			va_arg(argList, TCHAR_ARG);
			break;
		case 'c'|FORCE_ANSI:
		case 'C'|FORCE_ANSI:
			nItemLen = 2;
			va_arg(argList, CHAR_ARG);
			break;
		case 'c'|FORCE_UNICODE:
		case 'C'|FORCE_UNICODE:
			nItemLen = 2;
			va_arg(argList, WCHAR_ARG);
			break;

		// strings
		case 's':
			{
				LPCTSTR pstrNextArg = va_arg(argList, LPCTSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6;  // "(null)"
				else
				{
				   nItemLen = lstrlen(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
			}
			break;

		case 'S':
			{
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = lstrlenA(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
			}
			break;

		case 's'|FORCE_ANSI:
		case 'S'|FORCE_ANSI:
			{
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = lstrlenA(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
			}
			break;

		case 's'|FORCE_UNICODE:
		case 'S'|FORCE_UNICODE:
			{
				LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = wcslen(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
			}
			break;
		}

		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
			if (nPrecision != 0)
				nItemLen = min(nItemLen, nPrecision);
			nItemLen = max(nItemLen, nWidth);
		}
		else
		{
			switch (*sz)
			{
			// integers
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				if (nModifier & FORCE_INT64)
					va_arg(argList, __int64);
				else
					va_arg(argList, int);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			case 'e':
			case 'g':
			case 'G':
				va_arg(argList, double);
				nItemLen = 128;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			case 'f':
				{
					double f;
					LPTSTR pszTemp;

					// 312 == strlen("-1+(309 zeroes).")
					// 309 zeroes == max precision of a double
					// 6 == adjustment in case precision is not specified,
					//   which means that the precision defaults to 6
					pszTemp = (LPTSTR)_alloca(max(nWidth, 312+nPrecision+6));

					f = va_arg(argList, double);
					_stprintf( pszTemp, _T( "%*.*f" ), nWidth, nPrecision+6, f );
					nItemLen = _tcslen(pszTemp);
				}
				break;

			case 'p':
				va_arg(argList, void*);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			// no output
			case 'n':
				va_arg(argList, int*);
				break;

			default:
				assert(FALSE);  // unknown formatting option
			}
		}

		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}
	va_end(argListSave);
	
	return nMaxLen;
}
