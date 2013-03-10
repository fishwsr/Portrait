// VisImSrcDll.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#include "VisImSrcPch.h"
#include "VisImSrcProj.h"

#ifdef __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)
	#include <afxdllx.h>
#endif // __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



extern HINSTANCE imsrcCurrentInstanceHandle;


#ifdef VIS_IMSRC_DLL
#ifdef __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)
	static AFX_EXTENSION_MODULE VisImSrcDLL = { FALSE, NULL, NULL, NULL,NULL };
#endif // __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)

int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	VisUnreferenced(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{

		#ifdef _DEBUG
			VisTrace(1, "ImageDisplay.dll Initializing!\n");
		#endif // _DEBUG
		imsrcCurrentInstanceHandle = hInstance;

#ifdef __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)
		afxCurrentInstanceHandle = hInstance;
		afxCurrentResourceHandle = hInstance;

		::SetErrorMode(SEM_FAILCRITICALERRORS);

		// Extension DLL one-time initialization
		AfxInitExtensionModule(VisImSrcDLL, hInstance);

		// Insert this DLL into the resource chain
		new CDynLinkLibrary(VisImSrcDLL);
#endif // __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)
		

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		#ifdef _DEBUG
			VisTrace(1, "ImageDisplay.dll Terminating!\n");
		#endif // _DEBUG
	}
	
	return 1;   // ok
}

#endif // VIS_IMSRC_DLL

