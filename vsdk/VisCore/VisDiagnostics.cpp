///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisDiagnostics.cpp -- diagnostic message handling
//
// DESCRIPTION
//  The CVisDiagnostics class is used to print diagnostic and warning
//  messages during program execution.
//
//  It is up to the application to decide how to handle these messages.
//  For example, a Windows application may wish to print the diagnostics
//  in the status bar, and warnings in a pop-up window.
//  A console (command-line) application may with to print them on stderr.
//
// PUBLIC MEMBER FUNTIONS (METHODS)
//
//  static void Setup((*fn)(const char *msg, TVisDiagnosticLevel level));
//      Set the diagnostic handling function (in application)
//
//  static void Enable(int enable);
//      Enable diagnostic reporting
//
//  static void ProgressReport(const char *msg);
//      Issue a progress report
//
//  static void Warning(const char *msg)
//      Issue a warning
//
// SEE ALSO
//  VisError.h          error handling facility (uses throw/catch)
//  VisComposite.h      example of class generating diagnostics
//
// BUGS
//   
//
// DESIGN
//  
//
// HISTORY
//  23-Sep-97  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#include "VisCorePch.h"
#include "VisCoreProj.h"

// Initialize global variables

VisCoreExport void (*CVisDiagnostics::m_fn)(const char *msg, TVisDiagnosticLevel level) = 0;

VisCoreExport int CVisDiagnostics::m_enable = 1;
