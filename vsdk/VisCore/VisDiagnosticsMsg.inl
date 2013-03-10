// VisDiagnosticsMsg.h
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
inline CVisDiagnosticMsg::CVisDiagnosticMsg(const char* szFile, int iLine)
		: m_szFile(szFile), m_iLine(iLine)
{
}

inline CVisString::CVisString(void)
{
}

inline CVisString::CVisString(const CVisString& refstringSrc)
{
	*this = refstringSrc;
}

inline CVisString::CVisString(const char *sz)
{
	*((std::string *) this) = sz;
}

inline CVisString::CVisString(const std::string& refstr)
{
	*this = refstr.c_str();
}

inline CVisString::operator const char*(void) const
{
	return c_str(); 
}

inline char& CVisString::operator[](int i) const
{
	return (*(std::string *) this)[i];
}



#if defined(VIS_DIAGNOSTICS_ENABLED) || defined(_DEBUG)

	#define VisWarning			CVisDiagnosticMsg( __FILE__, __LINE__).Warning
	#define VisError			CVisDiagnosticMsg( __FILE__, __LINE__).Error
	#define VisTrace			CVisDiagnosticMsg( __FILE__, __LINE__).Trace
	#define VisProgressReport	CVisDiagnosticMsg( __FILE__, __LINE__).ProgressReport

#else // defined(VIS_DIAGNOSTICS_ENABLED) || defined(_DEBUG)

inline __cdecl VisNoOperation(...) {}

	#define VisWarning			VisNoOperation
	#define VisError			VisNoOperation
	#define VisTrace			VisNoOperation
	#define VisProgressReport	VisNoOperation

#endif // defined(VIS_DIAGNOSTICS_ENABLED) || defined(_DEBUG)
