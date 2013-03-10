// VisDiagnosticsMsg.h
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved


#pragma once


#include <string>
class CVisString : public std::string
{
	public:

		CVisString(void);
		CVisString(const CVisString& refstringSrc);
		CVisString(const char *sz);
//		CVisString(const unsigned char* psz);
		CVisString(const std::string& refstr);

	    operator const char*(void) const;
		char& operator[](int nIndex) const;

		VisCoreExport void Format(const char *szFormat, ...);

		VisCoreExport void Format(const char *szFormat, va_list argList);

	protected:
		

		static int FindLength(const char *szFormat, ...);
		static int FindLength(const char *szFormat, va_list argList);

}; // class CVisString


class VisCoreExport CVisDiagnosticMsg
{
	public:
		
		CVisDiagnosticMsg(const char* szFile, int iLine);

		void Warning(int iLevel, const char *szFormat, ...);

		void Error(int iLevel, const char *szFormat, ...);

		void Trace(int iLevel, const char *szFormat, ...);

		void ProgressReport(int iLevel, int nCur, int nLim, const char *szFormat, ...);

	private:

		const char* m_szFile;
		int			m_iLine;

}; // class CVisDiagnosticMsg

#include "VisDiagnosticsMsg.inl"
