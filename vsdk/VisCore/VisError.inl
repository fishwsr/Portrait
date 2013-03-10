////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISERROR
//
// @module VisError.inl |
//
// This file defines inline functions for the error objects used for the
// exception handling  in MSVisSDK.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisError\.h> <l VisError\.cpp>
//
// <nl>
// @index | VISERROR
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_ERROR_INL__
#define __VIS_CORE_ERROR_INL__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// @mfunc Creates an error object with given data.
inline CVisError::CVisError(
     const char *szMessage,				// @parm Error message.
	 long lErrorCode,					// @parm Error code.
	 const char *szFunctionName,		// @parm Name of function that caused an exception.
	 const char *szSourceFile,			// @parm Source file.
	 int iLine)							// @parm Source line number.
 :  exception(szMessage),
	m_szMessage(szMessage),
	m_szFunctionName(szFunctionName),
	m_szSourceFile(szSourceFile),
	m_code(lErrorCode),
	m_line(iLine)
{
}

// @func Look through the message array and return a code from code array
//        with the same index. If the message is not in the message array,
//        the code for UNKNOWN error is returned.
// @rdesc An error code (defined in VisError.h) that corresponds to this particular error.
inline long VisErrorMessageToCode(
       const char *szMessage,			// @parm Error message that corresponds to this particular error.
	   const char *rgszErrorMessage[],	// @parm An array of error messages
	   long rglErrorCode[])				// @parm A dictionary of error codes
{
	for ( int i = 0; rgszErrorMessage[i]; i++ )
	{
		if ( strstr(szMessage, rgszErrorMessage[i]) != NULL )
			return rglErrorCode[i];
	}

	return eviserrorUnknown;
}



////////////////////////////////////////////////////////////////////////////
//
// @mfunc const char * | CVisError | FunctionName |
//
// Get the function name.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline const char *CVisError::FunctionName(void) const
{
	return m_szFunctionName;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const char * | CVisError | ErrorMessage |
//
// Get the error message.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline const char *CVisError::ErrorMessage(void) const
{
	return m_szMessage;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const char * | CVisError | what |
//
// Same as ErrorMessage.  (exception::what() is a virtual method.)
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline const char *CVisError::what(void) const
{
	return ErrorMessage();
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const char * | CVisError | SourceFile |
//
// Get the source file.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline const char *CVisError::SourceFile(void) const
{
	return m_szSourceFile;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc int | CVisError | SourceLine |
//
// Get the line number.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline int CVisError::SourceLine(void) const
{
	return m_line;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc long | CVisError | ErrorCode |
//
// Get the error code.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline long CVisError::ErrorCode(void) const
{
	return m_code;
} 


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const char * | CVisError | FullMessage |
//
// Return a complete message.
//
////////////////////////////////////////////////////////////////////////////
inline const char *CVisError::FullMessage(void)
{ 
    m_strFullMessage = "Exception: \nsource file: ";
	m_strFullMessage += m_szSourceFile;
	m_strFullMessage += "\nfunction name: ";
	m_strFullMessage += m_szFunctionName;
	m_strFullMessage += "\nmessage: ";
	m_strFullMessage += m_szMessage;

    return m_strFullMessage.c_str();
}



////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisFileIOError | CVisFileIOError |
//
// Constructor.
//
// @parm const char * | szFilename |
// Name of file that had the I / O error.
//
// @parm const char * | szMessage |
// Detailed error message.
//
// @parmopt long | lErrorCode | <e EVisError::eviserrorUnknown> |
// Error code.
//
// @parmopt const char * | szFunctionName | 0 |
// Name of the function where the error occurred.
//
// @parmopt const char * | szSourceFile | 0 |
// Name of the file containing the function where the error occurred.
//
// @parmopt int | iLine | 0 |
// Line in the file containing the function where the error occurred.
//
////////////////////////////////////////////////////////////////////////////
inline CVisFileIOError::CVisFileIOError(const char *szFilename,
		        const char *szMessage, 
				long lErrorCode, 
		        const char *szFunctionName, 
				const char *szSourceFile,
				int iLine)
   : CVisError(szMessage, lErrorCode, szFunctionName,
		szSourceFile, iLine)
{
    m_strFilename = szFilename; 
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const char * | CVisFileIOError | FileName |
//
// Return file name.
//
// @this const
//
////////////////////////////////////////////////////////////////////////////
inline const char *CVisFileIOError::FileName(void) const
{
	return m_strFilename.c_str();
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const char * | CVisFileIOError | FullMessage |
//
// Return a complete message.
//
////////////////////////////////////////////////////////////////////////////
inline const char *CVisFileIOError::FullMessage(void)
{
    m_strFullMessage = "Exception: \nsource file: ";
	m_strFullMessage += m_szSourceFile;
	m_strFullMessage += "\nfunction name: ";
	m_strFullMessage += m_szFunctionName;
	m_strFullMessage += "\nfile name: ";
	m_strFullMessage += m_strFilename;
	m_strFullMessage += "\nmessage: ";
	m_strFullMessage += m_szMessage;

    return m_strFullMessage.c_str();
}



////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisMemoryError | CVisMemoryError |
//
// Constructor.
//
// @parmopt const char * | szFunctionName | 0 |
// Name of the function where the error occurred.
//
// @parmopt const char * | szSourceFile | 0 |
// Name of the file containing the function where the error occurred.
//
// @parmopt int | iLine | 0 |
// Line in the file containing the function where the error occurred.
//
////////////////////////////////////////////////////////////////////////////
inline CVisMemoryError::CVisMemoryError(const char *szFunctionName,
		const char *szSourceFile, int iLine)
  : CVisError("Memory allocation failed", eviserrorMemory, szFunctionName,
		szSourceFile, iLine)
{
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc const char * | CVisFileIOError | FullMessage |
//
// Don't attempt to allocate memory in CVisMemoryError::FullMessage.
//
////////////////////////////////////////////////////////////////////////////
inline const char *CVisMemoryError::FullMessage(void)
{
    return m_szMessage;
}


// Since we don't have run-time type information, we have virtual
// ThrowLast() methods to make sure that type information for
// derived classes is not lost in try-catch blocks.
inline void CVisError::ThrowLast(void)
{
	throw *this;
}

inline void CVisFileIOError::ThrowLast(void)
{
	throw *this;
}

inline void CVisMemoryError::ThrowLast(void)
{
	throw *this;
}

inline void CVisSDIError::ThrowLast(void)
{
	throw *this;
}



#endif // __VIS_CORE_ERROR_INL__

