// VisDiagnosticsManager.cpp
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
#include "VisCorePch.h"
#include "VisCoreProj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_FILE_LINE_LENGTH		 300
#define MAX_TYPE_LEVEL_LENGTH		 100
#define MAX_PROGRESS_LENGTH			  20
#define MAX_DESCRIPTION_LENGTH		3000

#define LOG_FILENAME_LENGTH			  20

//const char* gvis_szDiagnosticsKey	=	VIS_SZ_REG_ROOT "\\Diagnostics";
const char* gvis_szDiagnosticsKey			=	"SOFTWARE\\Microsoft\\VisSDK\\Diagnostics";
const char* gvis_szDiagnosticsDefaultKey	=	"\\Default";
const char* gvis_szDiagnosticsAppsKey		=	"\\Apps";

const char* gvis_szWarning			=	"Warning";
const char* gvis_szError			=	"Error";
const char* gvis_szTrace			=	"Trace";
const char* gvis_szProgressReport	=	"ProgressReport";

const char* gvis_szDiagnosticsEnabled	= "Enabled";
const char* gvis_szShowConsoleWnd		= "ShowConsoleWnd";
const char* gvis_szLogFileName			= "LogFile";

const char* gvis_szDebugStringLevel =	"DebugStringLevel";
const char* gvis_szConsoleLevel		=	"ConsoleLevel";
const char* gvis_szLogLevel			=	"LogFileLevel";

const char* gvis_szVisWarning			=	"VisWarning";
const char* gvis_szVisError				=	"VisError";
const char* gvis_szVisTrace				=	"VisTrace";
const char* gvis_szVisProgressReport	=	"VisProgressReport";

const char* gvis_szVisDefaultLogFileName =	"VisSDKLog.txt";

const DWORD gvis_dwDefaultLevel			=	5;

const char* gvis_szLevelFormat		  =	"[%s %d] ";
const char* gvis_szFileFormat		  =	"%s(%d): ";

const char* gvis_szProgressFormat	  = " (Progress:%d/%d)";

typedef std::vector<SVisDiagnosticsCallbackInfo> CVisDiagnosticsCallbackVector;


// UNDONE: Initializing levels from the registry could be delayed
// if required, use a flag in the constructor. Can be initialized
// when the first function is accessed.
CVisDiagnosticsManager::CVisDiagnosticsManager(void) 
{
	if(!InitializeLevelsFromRegistry())
	{
		SetLevelsModified(true);
	}
}

CVisDiagnosticsManager::~CVisDiagnosticsManager(void)
{
	if(LevelsModified())
		WriteToRegistry();
}

void CVisDiagnosticsManager::SetLogFileName(const char* szLogFileName)
{
	PGlobals()->SetLogFileName(szLogFileName);
}

char* CVisDiagnosticsManager::LogFileName(void)
{
	if(PGlobals()->LogFileName() == NULL)
	{
		CVisString strKey(gvis_szDiagnosticsKey);
		strKey += gvis_szDiagnosticsDefaultKey;
		CVisString strAppKey(gvis_szDiagnosticsKey);
		strAppKey += gvis_szDiagnosticsAppsKey;
		strAppKey += "\\" ;
		strAppKey += PGlobals()->AppName();

		FILE* pLogFile = NULL;
		char* szFileName = new char[LOG_FILENAME_LENGTH];

		HKEY hDiagnostics;
		if((VisFOpenRegKey(&hDiagnostics, strAppKey) && 
			VisFReadSzFromReg(szFileName, LOG_FILENAME_LENGTH, hDiagnostics, gvis_szLogFileName)) ||
			(VisFOpenRegKey(&hDiagnostics, strKey) && 
			VisFReadSzFromReg(szFileName, LOG_FILENAME_LENGTH, hDiagnostics, gvis_szLogFileName)))
		{
			SetLogFileName(szFileName);
		}
		else
		{
			SetLogFileName(gvis_szVisDefaultLogFileName);
		}
	}
	return PGlobals()->LogFileName();
}


void CVisDiagnosticsManager::SetLogFile(FILE* pLogFile)
{
	PGlobals()->SetLogFile(pLogFile);
}

FILE* CVisDiagnosticsManager::LogFile(void)
{
	if((PGlobals()->LogFile() == NULL) && (LogFileName() != NULL))
	{
		FILE* pLogFile = fopen(LogFileName(), "a+");
		if(pLogFile)
			SetLogFile(pLogFile);
	}
	return PGlobals()->LogFile();
}

void  CVisDiagnosticsManager::SetConsoleWindow(CVisConsoleWnd* pConsoleWnd)
{
	PGlobals()->SetConsoleWindow(pConsoleWnd);
}

CVisConsoleWnd* CVisDiagnosticsManager::ConsoleWindow(void)
{
	if( PGlobals()->ConsoleWindow() == NULL ) 
	{
		CVisConsoleWnd* pConsoleWnd = new CVisConsoleWnd;
		SetConsoleWindow(pConsoleWnd);
		pConsoleWnd->InitInstance(::GetModuleHandle(0), SW_SHOW);
	}
	return PGlobals()->ConsoleWindow();
}

void CVisDiagnosticsManager::AddStringToConsole(const char* szDescription)
{
	ConsoleWindow()->AddMessage(szDescription);
}

void CVisDiagnosticsManager::DisplayConsoleWindow(void)
{
	ConsoleWindow()->ShowWindow(SW_SHOW);
}

void CVisDiagnosticsManager::HideConsoleWindow(void)
{
	if(PGlobals()->ConsoleWindow())
		ConsoleWindow()->ShowWindow(SW_HIDE);
}


bool CVisDiagnosticsManager::InitializeLevelsFromRegistry(void)
{
	bool fInitialize = false;
	HKEY hDiagnostics;
	CVisString strKey(gvis_szDiagnosticsKey);
	strKey += gvis_szDiagnosticsDefaultKey;
	CVisString strAppKey(gvis_szDiagnosticsKey);
	strAppKey += gvis_szDiagnosticsAppsKey;
	strAppKey += "\\" ;
	strAppKey += PGlobals()->AppName();
	if((fInitialize = VisFOpenRegKey(&hDiagnostics, strAppKey)) ||
		VisFOpenRegKey(&hDiagnostics, strKey))
	{
		HKEY hTypes;
		DWORD dwLevel;
		
		fInitialize = VisFReadDwordFromReg(&dwLevel, hDiagnostics, gvis_szShowConsoleWnd) && fInitialize;
		dwLevel ? EnableConsoleWnd() : DisableConsoleWnd();
		if(VisFOpenRegKey(&hTypes, gvis_szWarning, hDiagnostics))
		{
			fInitialize = VisFReadDwordFromReg(&dwLevel, hTypes, gvis_szDebugStringLevel) && fInitialize;
			SetWarningDebugStringLevel(dwLevel);
			fInitialize = VisFReadDwordFromReg(&dwLevel, hTypes, gvis_szConsoleLevel) && fInitialize;
			SetWarningConsoleLevel(dwLevel);
			fInitialize = VisFReadDwordFromReg(&dwLevel, hTypes, gvis_szLogLevel) && fInitialize;
			SetWarningLogLevel(dwLevel);
		}
		if(VisFOpenRegKey(&hTypes, gvis_szError, hDiagnostics))
		{
			fInitialize = VisFReadDwordFromReg(&dwLevel, hTypes, gvis_szDebugStringLevel) && fInitialize;
			SetErrorDebugStringLevel(dwLevel);
			fInitialize = VisFReadDwordFromReg(&dwLevel, hTypes, gvis_szConsoleLevel) && fInitialize;
			SetErrorConsoleLevel(dwLevel);
			fInitialize = VisFReadDwordFromReg(&dwLevel, hTypes, gvis_szLogLevel) && fInitialize;
			SetErrorLogLevel(dwLevel);
		}
		if(VisFOpenRegKey(&hTypes, gvis_szTrace, hDiagnostics))
		{
			fInitialize = VisFReadDwordFromReg(&dwLevel, hTypes, gvis_szDebugStringLevel) && fInitialize;
			SetTraceDebugStringLevel(dwLevel);
			fInitialize = VisFReadDwordFromReg(&dwLevel, hTypes, gvis_szConsoleLevel) && fInitialize;
			SetTraceConsoleLevel(dwLevel);
			fInitialize = VisFReadDwordFromReg(&dwLevel, hTypes, gvis_szLogLevel) && fInitialize;
			SetTraceLogLevel(dwLevel);
		}
		if(VisFOpenRegKey(&hTypes, gvis_szProgressReport, hDiagnostics))
		{
			fInitialize = VisFReadDwordFromReg(&dwLevel, hTypes, gvis_szDebugStringLevel) && fInitialize;
			SetProgressDebugStringLevel(dwLevel);
			fInitialize = VisFReadDwordFromReg(&dwLevel, hTypes, gvis_szConsoleLevel) && fInitialize;
			SetProgressConsoleLevel(dwLevel);
			fInitialize = VisFReadDwordFromReg(&dwLevel, hTypes, gvis_szLogLevel) && fInitialize;
			SetProgressLogLevel(dwLevel);
		}
	}
	return fInitialize;
}

void CVisDiagnosticsManager::WriteToRegistry(void)
{
	HKEY hDiagnostics;
	CVisString strAppKey(gvis_szDiagnosticsKey);
	strAppKey += gvis_szDiagnosticsAppsKey;
	strAppKey += "\\";
	strAppKey += PGlobals()->AppName();
	if(VisFOpenRegKey(&hDiagnostics, strAppKey.c_str()) ||
		VisFCreateRegKey(&hDiagnostics, strAppKey.c_str()))
	{
		HKEY hTypes;
		VisFWriteDwordToReg(ShowConsoleWnd(), hDiagnostics, gvis_szShowConsoleWnd);
		VisFWriteSzToReg(LogFileName(), hDiagnostics, gvis_szLogFileName);
		if(VisFOpenRegKey(&hTypes, gvis_szWarning, hDiagnostics) ||
			VisFCreateRegKey(&hTypes, gvis_szWarning, hDiagnostics))
		{
			VisFWriteDwordToReg(WarningDebugStringLevel(), hTypes, gvis_szDebugStringLevel);
			VisFWriteDwordToReg(WarningConsoleLevel(), hTypes, gvis_szConsoleLevel);
			VisFWriteDwordToReg(WarningLogLevel(), hTypes, gvis_szLogLevel);
		}
		if(VisFOpenRegKey(&hTypes, gvis_szError, hDiagnostics) ||
			VisFCreateRegKey(&hTypes, gvis_szError, hDiagnostics))
		{
			VisFWriteDwordToReg(ErrorDebugStringLevel(), hTypes, gvis_szDebugStringLevel);
			VisFWriteDwordToReg(ErrorConsoleLevel(), hTypes, gvis_szConsoleLevel);
			VisFWriteDwordToReg(ErrorLogLevel(), hTypes, gvis_szLogLevel);
		}
		if(VisFOpenRegKey(&hTypes, gvis_szTrace, hDiagnostics) ||
			VisFCreateRegKey(&hTypes, gvis_szTrace, hDiagnostics))
		{
			VisFWriteDwordToReg(TraceDebugStringLevel(), hTypes, gvis_szDebugStringLevel);
			VisFWriteDwordToReg(TraceConsoleLevel(), hTypes, gvis_szConsoleLevel);
			VisFWriteDwordToReg(TraceLogLevel(), hTypes, gvis_szLogLevel);
		}
		if(VisFOpenRegKey(&hTypes, gvis_szProgressReport, hDiagnostics) ||
			VisFCreateRegKey(&hTypes, gvis_szProgressReport, hDiagnostics))
		{
			VisFWriteDwordToReg(ProgressDebugStringLevel(), hTypes, gvis_szDebugStringLevel);
			VisFWriteDwordToReg(ProgressConsoleLevel(), hTypes, gvis_szConsoleLevel);
			VisFWriteDwordToReg(ProgressLogLevel(), hTypes, gvis_szLogLevel);
		}
		SetLevelsModified(false);
	}
}

void CVisDiagnosticsManager::AddCallbackInfo(int nDiagnosticsType, DWORD dwLevel,
												EVisDiagDescription evisdiagdescriptionFlags,
												VisPfnDiagnosticsCallback pfnCallback, 
												void* pvUser)
{
	InsertCallback(nDiagnosticsType, dwLevel, evisdiagdescriptionFlags, pfnCallback,
					evisdiagcallbackDefault, pvUser);
}

void CVisDiagnosticsManager::AddCallbackInfo(int nDiagnosticsType, DWORD dwLevel,
												EVisDiagDescription evisdiagdescriptionFlags,
												VisPfnDiagnosticsProgressCallback pfnCallback, 
												void* pvUser)
{
	InsertCallback(nDiagnosticsType, dwLevel, evisdiagdescriptionFlags, pfnCallback,
					evisdiagcallbackProgress, pvUser);

}

void CVisDiagnosticsManager::InsertCallback(int nDiagnosticsType, DWORD dwLevel,
											EVisDiagDescription evisdiagdescriptionFlags,
											void* pfnCallback, EVisDiagCallback eviscallbackType,
											void* pvUser)
{
	SVisDiagnosticsCallbackInfo visdiagnosticsInfo = { nDiagnosticsType,
														dwLevel,
														evisdiagdescriptionFlags,
														pfnCallback,
														pvUser,
														eviscallbackType };

	RemoveCallback(nDiagnosticsType, pfnCallback, pvUser);
	CVisDiagnosticsCallbackVector* vectorCallback = PGlobals()->DiagCallbackList();
	
	if(!MergeCallbackInfo(nDiagnosticsType, dwLevel, evisdiagdescriptionFlags,
						  pfnCallback, pvUser))
	{
		vectorCallback->push_back(visdiagnosticsInfo);
	}

}

bool CVisDiagnosticsManager::MergeCallbackInfo(int nDiagnosticsType, DWORD dwLevel,
												EVisDiagDescription evisdiagdescriptionFlags,
												void* pfnCallback, void* pvUser)
{
	CVisDiagnosticsCallbackVector* vectorCallback = PGlobals()->DiagCallbackList();
	
	CVisDiagnosticsCallbackVector::iterator callbackIterator = vectorCallback->begin();	
	bool fInsert = false;
	for( ; callbackIterator != vectorCallback->end(); callbackIterator++)
	{
		SVisDiagnosticsCallbackInfo& refcallbackCur = *callbackIterator;
		if((refcallbackCur.m_pfnCallback == pfnCallback) &&
			(refcallbackCur.m_pvUser == pvUser) &&
			(refcallbackCur.m_enumdescriptionFlags == evisdiagdescriptionFlags) &&
			(refcallbackCur.m_dwLevel == dwLevel))
		{
			refcallbackCur.m_nDiagnosticsType |= nDiagnosticsType;
			fInsert = true;
			break;
		}
	}
	return fInsert;
}

void CVisDiagnosticsManager::SetCallbackFunction(int nDiagnosticsType, DWORD dwLevel,
												VisPfnDiagnosticsCallback pfnCallback, void* pvUser,
												 EVisDiagDescription evisdiagdescriptionFlags)
{
	AddCallbackInfo(nDiagnosticsType, dwLevel, evisdiagdescriptionFlags, pfnCallback, pvUser);
}

void CVisDiagnosticsManager::SetCallbackFunction(int nDiagnosticsType, VisPfnDiagnosticsCallback pfnCallback, 
												 void* pvUser, EVisDiagDescription evisdiagdescriptionFlags)
{
	AddCallbackInfo(nDiagnosticsType, gvis_dwDefaultLevel, evisdiagdescriptionFlags, pfnCallback, pvUser);
}

void CVisDiagnosticsManager::SetCallbackFunction(int nDiagnosticsType, DWORD dwLevel,
												VisPfnDiagnosticsProgressCallback pfnCallback, void* pvUser,
												 EVisDiagDescription evisdiagdescriptionFlags)
{
	AddCallbackInfo(nDiagnosticsType, dwLevel, evisdiagdescriptionFlags, pfnCallback, pvUser);
}

void CVisDiagnosticsManager::SetCallbackFunction(int nDiagnosticsType, VisPfnDiagnosticsProgressCallback pfnCallback, 
													void* pvUser, EVisDiagDescription evisdiagdescriptionFlags)
{
	AddCallbackInfo(nDiagnosticsType, gvis_dwDefaultLevel, evisdiagdescriptionFlags, pfnCallback, pvUser);
}


void CVisDiagnosticsManager::SetWarningFunction(DWORD dwLevel, VisPfnDiagnosticsCallback pfnWarning, 
						void* pvUser,
						EVisDiagDescription evisdiagdescriptionFlags)
{
	AddCallbackInfo(evisdiagnosticsWarning, dwLevel, evisdiagdescriptionFlags, pfnWarning, pvUser);
}

void CVisDiagnosticsManager::SetWarningFunction(VisPfnDiagnosticsCallback pfnWarning, void* pvUser,
						EVisDiagDescription evisdiagdescriptionFlags)
{
	AddCallbackInfo(evisdiagnosticsWarning, gvis_dwDefaultLevel, evisdiagdescriptionFlags, pfnWarning, pvUser);}


void CVisDiagnosticsManager::SetErrorFunction(DWORD dwLevel, VisPfnDiagnosticsCallback pfnError, 
						void* pvUser, EVisDiagDescription evisdiagdescriptionFlags)
{
	AddCallbackInfo(evisdiagnosticsError, dwLevel, evisdiagdescriptionFlags, pfnError, pvUser);
}


void CVisDiagnosticsManager::SetErrorFunction(VisPfnDiagnosticsCallback pfnError, void* pvUser,
						EVisDiagDescription evisdiagdescriptionFlags)
{
	AddCallbackInfo(evisdiagnosticsError, gvis_dwDefaultLevel, evisdiagdescriptionFlags, pfnError, pvUser);
}


void CVisDiagnosticsManager::SetTraceFunction(DWORD dwLevel, VisPfnDiagnosticsCallback pfnTrace, 
						void* pvUser, EVisDiagDescription evisdiagdescriptionFlags)
{
	AddCallbackInfo(evisdiagnosticsTrace, dwLevel, evisdiagdescriptionFlags, pfnTrace, pvUser);
}

void CVisDiagnosticsManager::SetTraceFunction(VisPfnDiagnosticsCallback pfnTrace, void* pvUser,
						EVisDiagDescription evisdiagdescriptionFlags)
{
	AddCallbackInfo(evisdiagnosticsTrace, gvis_dwDefaultLevel, evisdiagdescriptionFlags, pfnTrace, pvUser);
}


void CVisDiagnosticsManager::SetProgressReportFunction(DWORD dwLevel, 
						VisPfnDiagnosticsCallback pfnProgressReport, 
						void* pvUser, EVisDiagDescription evisdiagdescriptionFlags)
{
	AddCallbackInfo(evisdiagnosticsProgress, dwLevel, evisdiagdescriptionFlags, pfnProgressReport, pvUser);
}

void CVisDiagnosticsManager::SetProgressReportFunction(VisPfnDiagnosticsCallback pfnProgressReport, 
								void* pvUser, EVisDiagDescription evisdiagdescriptionFlags)
{
	AddCallbackInfo(evisdiagnosticsProgress, gvis_dwDefaultLevel, evisdiagdescriptionFlags, pfnProgressReport, pvUser);
}

void CVisDiagnosticsManager::SetProgressReportFunction(DWORD dwLevel, 
											VisPfnDiagnosticsProgressCallback pfnProgressReport, 
											void* pvUser ,EVisDiagDescription evisdiagdescriptionFlags)
{
	AddCallbackInfo(evisdiagnosticsProgress, dwLevel, evisdiagdescriptionFlags, pfnProgressReport, pvUser);
}

void CVisDiagnosticsManager::SetProgressReportFunction(VisPfnDiagnosticsProgressCallback pfnProgressReport, 
											void* pvUser, EVisDiagDescription evisdiagdescriptionFlags)
{
	AddCallbackInfo(evisdiagnosticsProgress, gvis_dwDefaultLevel, evisdiagdescriptionFlags, pfnProgressReport, pvUser);
}

void CVisDiagnosticsManager::RemoveCallback(const void* pfnCallback, const void* pvUser)
{
	if(PGlobals()->DiagnosticsCallbackAvailable())
	{
		CVisDiagnosticsCallbackVector* vectorCallback = PGlobals()->DiagCallbackList();
		
		CVisDiagnosticsCallbackVector::iterator callbackIterator = vectorCallback->begin();	
		while(callbackIterator != vectorCallback->end())
		{
			SVisDiagnosticsCallbackInfo& refcallbackCur = *callbackIterator;
			if((refcallbackCur.m_pfnCallback == pfnCallback) &&
				(refcallbackCur.m_pvUser == pvUser))
			{
				callbackIterator = vectorCallback->erase(callbackIterator);
			}
			else
			{
				callbackIterator++;
			}
		}
	}
}

void CVisDiagnosticsManager::RemoveCallback(int nDiagnosticsType, const void* pfnCallback, 
											const void* pvUser)
{
	if(PGlobals()->DiagnosticsCallbackAvailable())
	{
		CVisDiagnosticsCallbackVector* vectorCallback = PGlobals()->DiagCallbackList();
	
		CVisDiagnosticsCallbackVector::iterator callbackIterator = vectorCallback->begin();	
		while(callbackIterator != vectorCallback->end())
		{
			SVisDiagnosticsCallbackInfo& refcallbackCur = *callbackIterator;
			bool fMoveIterator = true;
			if((refcallbackCur.m_pfnCallback == pfnCallback) &&
				(refcallbackCur.m_pvUser == pvUser))
			{
				// Save current flags
				int nDiagType = refcallbackCur.m_nDiagnosticsType;
				
				// Clear the flags we're changing.  If all flags are changed, erase this element.
				if(!(refcallbackCur.m_nDiagnosticsType &= (~nDiagnosticsType)))
				{
					callbackIterator = vectorCallback->erase(callbackIterator);
					fMoveIterator = false;
				}
				
				// Remember the flags that we cleared above.  If there are no more
				// flags to check, we can stop early.
				if(!(nDiagnosticsType &= (~nDiagType)))
				{
					break;
				}
			}
			if(fMoveIterator)
			{
				callbackIterator++;
			}
		}
	}
}

void CVisDiagnosticsManager::RemoveWarningCallback(const void* pfnCallback, const void* pvUser)
{
	RemoveCallback(evisdiagnosticsWarning, pfnCallback, pvUser);
}

void CVisDiagnosticsManager::RemoveErrorCallback(const void* pfnCallback, const void* pvUser)
{
	RemoveCallback(evisdiagnosticsError, pfnCallback, pvUser);
}

void CVisDiagnosticsManager::RemoveTraceCallback(const void* pfnCallback, const void* pvUser)
{
	RemoveCallback(evisdiagnosticsTrace, pfnCallback, pvUser);
}

void CVisDiagnosticsManager::RemoveProgressCallback(const void* pfnCallback, const void* pvUser)
{
	RemoveCallback(evisdiagnosticsProgress, pfnCallback, pvUser);
}

const char* CVisDiagnosticsManager::DescriptionText(EVisDiagnostics evisdiagnosticsType)
{
	char* szDiagnosticsType = NULL;
	switch(evisdiagnosticsType)
	{
		case evisdiagnosticsWarning:
		{
			szDiagnosticsType = (char *)gvis_szVisWarning;
			break;
		}
		case evisdiagnosticsError:
		{
			szDiagnosticsType = (char *)gvis_szVisError;
			break;
		}
		case evisdiagnosticsTrace:
		{
			szDiagnosticsType = (char *)gvis_szVisTrace;
			break;
		}
		case evisdiagnosticsProgress:
		{
			szDiagnosticsType = (char *)gvis_szVisProgressReport;
			break;
		}

	}// ENDOF switch(evisdiagnosticsType)

	return szDiagnosticsType;
}


void CVisDiagnosticsManager::FormatDiagnosticMessage(char* szDescription, const char* szFileLine, 
													 const char* szTypeLevel, const char* szMessage, 
													 EVisDiagDescription evisDescriptionFlag)
{
	switch(evisDescriptionFlag)
	{
		case evisdiagdescriptionFileAndLine:
		{
			strcpy(szDescription, szFileLine);
			strcat(szDescription, szMessage);
			break;
		}
		case evisdiagdescriptionTypeAndLevel:
		{
			strcpy(szDescription, szTypeLevel);
			strcat(szDescription, szMessage);
			break;
		}
		case evisdiagdescriptionAll:
		{
			strcpy(szDescription, szFileLine);
			strcat(strcat(szDescription, szTypeLevel), szMessage);
			break;
		}

	} // ENDOF switch(evisDescriptionFlag)
}

void CVisDiagnosticsManager::DoMessage(EVisDiagnostics evisdiagnosticsType, DWORD dwLevel, 
										const char* szFile, int iLine,
										const char* szMessage, int nCur, int nLim)
{
	char szFileLine[MAX_FILE_LINE_LENGTH];
	char szTypeLevel[MAX_TYPE_LEVEL_LENGTH];
	
	char szProgress[MAX_PROGRESS_LENGTH];

	char szMsgBuffer[MAX_DESCRIPTION_LENGTH];

	int cchTotal = sprintf(szFileLine, gvis_szFileFormat, szFile, iLine);
	cchTotal += sprintf(szTypeLevel, gvis_szLevelFormat, 
								DescriptionText(evisdiagnosticsType), dwLevel);

	cchTotal += strlen(szMessage);

	if(nLim)
		cchTotal += sprintf(szProgress, gvis_szProgressFormat, nCur, nLim);

	char* szDescription = NULL;
	if(cchTotal < sizeof(szMsgBuffer))
	{
		szDescription = szMsgBuffer;
	}
	else
	{
		szDescription = new char[cchTotal + 1];
	}

	EVisDiagDescription descriptionFlags = evisdiagdescriptionNil;

	// Flags are passed to this function as reference, so that the format description
	// can be used for displaying to the std o/p
	TriggerUserFunction(szDescription, szFileLine, szTypeLevel, szMessage, 
						dwLevel, evisdiagnosticsType, descriptionFlags, nCur, nLim);


	DisplayToStandardOutput(szDescription, 
							//true if szDescription can be used
							(descriptionFlags == evisdiagdescriptionAll),
							dwLevel, evisdiagnosticsType, szFileLine, 
							szTypeLevel, szMessage, 
							nLim ? szProgress : NULL);

	if(cchTotal >= sizeof(szMsgBuffer))
		delete szDescription;
}

void CVisDiagnosticsManager::TriggerUserFunction(char* szDescription, const char* szFileLine, 
												 const char* szTypeLevel, const char* szMessage, 
												 DWORD dwLevel, EVisDiagnostics evisdiagnosticsType,
												 EVisDiagDescription& descriptionFlags,
												 int nCur, int nLim)
{
	CVisGlobals* pvisGlobals = PGlobals();

	if(pvisGlobals->DiagnosticsCallbackAvailable() == false)
		return;

	CVisDiagnosticsCallbackVector* vectorCallback = pvisGlobals->DiagCallbackList();
	
	CVisDiagnosticsCallbackVector::iterator callbackIterator = vectorCallback->begin();	
	for( ; callbackIterator != vectorCallback->end(); callbackIterator++)
	{
		SVisDiagnosticsCallbackInfo& refcallbackCur = *callbackIterator;
		if((dwLevel <= refcallbackCur.m_dwLevel) &&
			(refcallbackCur.m_nDiagnosticsType & evisdiagnosticsType))
		{
			if(refcallbackCur.m_enumdescriptionFlags != descriptionFlags)
			{
				FormatDiagnosticMessage(szDescription, szFileLine, szTypeLevel, 
										szMessage, refcallbackCur.m_enumdescriptionFlags);
				descriptionFlags = refcallbackCur.m_enumdescriptionFlags;
			}
			switch(refcallbackCur.m_enumcallbackType)
			{
				case evisdiagcallbackDefault:
				{
					VisPfnDiagnosticsCallback diagnosticsDefault =
									(VisPfnDiagnosticsCallback)(refcallbackCur.m_pfnCallback);
					diagnosticsDefault(refcallbackCur.m_pvUser, evisdiagnosticsType, 
												dwLevel, szDescription);
					break;
				}
				case evisdiagcallbackProgress:
				{
					VisPfnDiagnosticsProgressCallback diagnosticsProgress =
									(VisPfnDiagnosticsProgressCallback)(refcallbackCur.m_pfnCallback);
					diagnosticsProgress(refcallbackCur.m_pvUser, evisdiagnosticsType, 
												dwLevel, nCur, nLim, szDescription);
					break;
				}

			} // ENDOF switch(callbackSelect.m_enumcallbackType)
		}
	
	} // ENDOF for(callbackIterator != vectorCallback->end())
}

void CVisDiagnosticsManager::DisplayToStandardOutput(char* szDescription, bool fFormatReady,
													 DWORD dwLevel, EVisDiagnostics evisdiagnosticsType,
													 const char* szFileLine, const char* szTypeLevel, 
													 const char* szMessage, const char* szProgress)
{
	bool fDebugWindow = false;
	bool fConsole = false;
	bool fLog = false;

	CVisGlobals* pvisGlobals = PGlobals();

	switch(evisdiagnosticsType)
	{
		case evisdiagnosticsWarning:
		{
			if(dwLevel <= WarningDebugStringLevel())
			{
				fDebugWindow = true;
			}
			if(dwLevel <= WarningConsoleLevel())
			{
				fConsole = true;
			}
			if((dwLevel <= WarningLogLevel()) &&
				(LogFile() != NULL))
			{
				fLog = true;
			}
			break;
		}
		case evisdiagnosticsError:
		{
			if(dwLevel <= ErrorDebugStringLevel())
			{
				fDebugWindow = true;
			}
			if(dwLevel <= ErrorConsoleLevel())
			{
				fConsole = true;
			}
			if((dwLevel <= ErrorLogLevel()) &&
				(LogFile() != NULL))
			{
				fLog = true;
			}
			break;
		}
		case evisdiagnosticsTrace:
		{
			if(dwLevel <= TraceDebugStringLevel())
			{
				fDebugWindow = true;
			}
			if(dwLevel <= TraceConsoleLevel())
			{
				fConsole = true;
			}
			if((dwLevel <= TraceLogLevel()) &&
				(LogFile() != NULL))
			{
				fLog = true;
			}
			break;
		}
		case evisdiagnosticsProgress:
		{
			if(dwLevel <= ProgressDebugStringLevel())
			{
				fDebugWindow = true;
			}
			if(dwLevel <= ProgressConsoleLevel())
			{
				fConsole = true;
			}
			if((dwLevel <= ProgressLogLevel()) &&
				(LogFile() != NULL))
			{
				fLog = true;
			}
			break;
		}
	}

	if(fDebugWindow || fConsole || fLog)
	{
		if(!fFormatReady)
		{
			sprintf(szDescription, "%s%s%s", szFileLine, szTypeLevel, szMessage);
		}
		if((evisdiagnosticsType == evisdiagnosticsProgress) && szProgress)
		{
			strcat(szDescription, szProgress);
		}
	}
	
	if(fDebugWindow)
	{
#ifdef VIS_USER_BUILD_erikhan // UNDONE:  Use a better macro name here.
		OutputToDebugWindow(szFileLine);
		OutputToDebugWindow(szTypeLevel);
		OutputToDebugWindow("\n\t");
		OutputToDebugWindow(szMessage);
#else // VIS_USER_BUILD_erikhan
		OutputToDebugWindow(szDescription);
#endif // VIS_USER_BUILD_erikhan
	}
	if(fConsole)
	{
		OutputToConsole(szDescription);
	}
	if(fLog)
	{
		WriteToLogFile(szDescription);
	}
}

void CVisDiagnosticsManager::OutputToDebugWindow(const char* szDescription)
{
	OutputDebugString(szDescription);
}

void CVisDiagnosticsManager::OutputToConsole(const char* szDescription)
{
	if(ShowConsoleWnd())
	{
		AddStringToConsole(szDescription);
	}
}

void CVisDiagnosticsManager::WriteToLogFile(const char* szDescription)
{
	FILE* pLogFile = LogFile();
	CVisString visDescription(szDescription);
	visDescription += "\n";
	fwrite(visDescription, visDescription.size(), 1, pLogFile);
	fflush(pLogFile);
}