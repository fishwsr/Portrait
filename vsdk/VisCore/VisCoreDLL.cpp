// VisCore.cpp : Defines the initialization routines for the DLL.
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved

#include "VisCorePch.h"
#include "VisCoreProj.h"

#ifdef __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)
	#include <afxdllx.h>
#endif // __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifdef __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)
	static AFX_EXTENSION_MODULE VisCoreDLL = { NULL, NULL };
#endif // __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	VisUnreferenced(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		#ifdef _DEBUG
			VisTrace(1, "VisCore.dll Initializing!\n");
		#endif // _DEBUG
		
#ifdef __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(VisCoreDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(VisCoreDLL);
#endif // __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		#ifdef _DEBUG
			VisTrace(1, "VisCore.dll Terminating!\n");
		#endif // _DEBUG
#ifdef __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)
		// Terminate the library before destructors are called
		AfxTermExtensionModule(VisCoreDLL);
#endif // __AFX_H__  // (__AFX_H__ is defined when we're using MFC.)
	}
	return 1;   // ok
}

