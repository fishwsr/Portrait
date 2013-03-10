///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisDiagnostics.h -- diagnostic message handling
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
//  static void ProgressReport(const char *format,
//          int a0, int a1 = 0, int a2 = 0, int a3 = 0);
//      Format message with int arguments
//
//  static void Warning(const char *format,
//          int a0, int a1 = 0, int a2 = 0, int a3 = 0);
//      Format message with int arguments
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

#ifndef _VIS_DIAGNOSTICS_H_
#define _VIS_DIAGNOSTICS_H_

#include <stdio.h>

// UNDONE:  Use standard names.
enum TVisDiagnosticLevel {
    VIS_DIAGNOSTIC_PROGRESS,    // progress report
    VIS_DIAGNOSTIC_WARNING      // warning (pop-up box, etc...)
};


class CVisDiagnostics {
public:

    // Diagnostics handler setup and enabling
    static void Setup(void (*fn)(const char *msg, TVisDiagnosticLevel level))
        { m_fn = fn; }
    static void Enable(int enable)
        { m_enable = enable; }

    // Diagnostic functions
    static void ProgressReport(const char *msg)
        { if (m_enable && m_fn) m_fn(msg, VIS_DIAGNOSTIC_PROGRESS); }
    static void Warning(const char *msg)
        { if (m_enable && m_fn) m_fn(msg, VIS_DIAGNOSTIC_WARNING); }

    // Helper functions for formatting messages with int or string arguments
    static void ProgressReport(const char *format,
        int a0, int a1 = 0, int a2 = 0, int a3 = 0)
        { char msg[4096]; sprintf(msg, format, a0, a1, a2, a3); ProgressReport(msg); }
    static void ProgressReport(const char *format,
        const char *a0, const char *a1 = 0, const char *a2 = 0, const char *a3 = 0)
        { char msg[4096]; sprintf(msg, format, a0, a1, a2, a3); ProgressReport(msg); }
    static void Warning(const char *format,
        int a0, int a1 = 0, int a2 = 0, int a3 = 0)
        { char msg[4096]; sprintf(msg, format, a0, a1, a2, a3); Warning(msg); }
    static void Warning(const char *format,
        const char *a0, const char *a1 = 0, const char *a2 = 0, const char *a3 = 0)
        { char msg[4096]; sprintf(msg, format, a0, a1, a2, a3); Warning(msg); }

private:
    VisCoreExport static void (*m_fn)(const char *msg, TVisDiagnosticLevel level);
    VisCoreExport static int m_enable;   // enable progress reporting
};

#endif // _VIS_DIAGNOSTICS_H_
