////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISERROR
//
// @module VisError.h |
//
// This file defines error objects used for the exception handling 
// in MSVisSDK. The basic object, CVisError, is a base class that provides
// some information about the exception (file name, function name, etc.)
// CVisFileIOError and CVisMemoryError are derived from CVisError class.
// They contain more specific information about the nature of the exception.
//
// <nl>
//
// @devnote:(INTERNAL)
//  
// Due to a bug in VC++, the _LINE_ macro doesn't return the right 
// line number. This causes an exception object to be created with a 
// wrong line number. Once this bug is fixed, line number should be 
// reported correctly by all the error objects.
//  
// FullMessage member function returns an error message with file name,
// function name and the error message specific to a certain error object.
// Each piece of this information can be obtained separately by using appropriate
// member functions.
//
// Protected member function ComposeMessages takes a number (n) of strings 
// and concatenates them together into one message.
//
// @todo:(INTERNAL)
// We should have macros to throw the common exceptions.
//
// We might want an exception for type mismatches.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisError\.inl> <l VisError\.cpp>
//
// <nl>
// @index | VISERROR
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_ERROR_H__
#define __VIS_CORE_ERROR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <stdlib.h>
#include <string>


// Need this for the VisXImageMagick project (which doesn't include VisCoreProj.h).
#ifndef VisCoreExport
	#ifdef VIS_CORE_LIB
		#define VisCoreExport 		
	#else // VIS_CORE_LIB
		#define VisCoreExport __declspec(dllimport)
	#endif // VIS_CORE_LIB
#endif // VisCoreExport


// Need this for the VisXImageMagick project (which doesn't include VisCoreProj.h).
#ifndef VISAPI
#define VISAPI __stdcall
#endif // !VISAPI


// Helper macro
// LATER:  Add more helper macros?
#define VIS_THROW_OOM(szFunctionName) \
	throw CVisMemoryError(szFunctionName, __FILE__, __LINE__)


////////////////////////////////////////////////////////////////////////////
//
// @enum EVisError |
// Error Codes for various errors that might occur (mostly file I/O).
//
// @devnote:(INTERNAL) LATER:  May want more specific memory errors.
//
// @xref <c CVisError>
//
////////////////////////////////////////////////////////////////////////////
enum EVisError
{
	// @emem
	// Unknown error type.
	eviserrorUnknown			=  0,

	// @emem
	// A feature was not implemented when the library was built.
	eviserrorNotImplemented		=  1,

	// @emem
	// Type mismatch.
	eviserrorTypeMismatch		=  2,

	// @emem
	// Image shape mismatch.
	eviserrorImageShapeMismatch	=  3,

	// @emem
	// Out of memory error.
	eviserrorMemory				= 10,

	// @emem
	// File name expected, but no file name given.
	eviserrorNoFilename			= 20,

	// @emem
	// File open failed.
	eviserrorOpen				= 21,

	// @emem
	// File read failed.
	eviserrorRead				= 22,

	// @emem
	// File write failed.
	eviserrorWrite				= 23,

	// @emem
	// Bad file format.
	eviserrorFormat				= 24,

	// @emem
	// Bad pixel type.
	eviserrorPixFmt				= 25,

	// @emem
	// Bad image shape.
	eviserrorShape				= 26,

	// @emem
	// Corrupt file.
	eviserrorCorrupt			= 27,

	// @emem
	// An operation failed unexpectedly.  For example, a matrix operation
	// might encounter a singluar matrix or a required image operation may
	// not be supported.
	eviserrorOpFailed			= 28,

	// @emem
	// Miscellaneous error using the ImageMagisk file handler.
	eviserrorMagickMisc			= 29,

	// @emem
	// Unexpected.
	eviserrorUnexpected			= 100
};


//#define VIS_OLD_ERROR_NAMES

#ifdef VIS_OLD_ERROR_NAMES

#define VIS_ERR_C_UNKNOWN			eviserrorUnknown
#define VIS_ERR_C_MEMORY			eviserrorMemory
#define VIS_ERR_C_NO_FILENAME		eviserrorNoFilename
#define VIS_ERR_C_OPEN				eviserrorOpen
#define VIS_ERR_C_READ				eviserrorRead
#define VIS_ERR_C_WRITE				eviserrorWrite
#define VIS_ERR_C_FORMAT			eviserrorFormat
#define VIS_ERR_C_PIXELTYPE			eviserrorPixFmt
#define VIS_ERR_C_SHAPE				eviserrorShape
#define VIS_ERR_C_CORRUPT			eviserrorCorrupt
#define VIS_ERR_C_OP_FAILED			eviserrorOpFailed
#define VIS_ERR_C_MAGICK_MISC		eviserrorMagickMisc

#endif // VIS_OLD_ERROR_NAMES


// UNDONE (in Rel1):  We shouldn't use std::string in these classes, since
// std::string may need to allocate memory.  Instead, we should
// only use the static strings passed to the ctors.


////////////////////////////////////////////////////////////////////////////
//  
// @class	
//	General error object. More special error objects are derived from 
//	this class for different types of errors.
//
// LATER:  We should probably derive CVisException from the base exception
// class used by MFC.
//
// @xref <c CVisFileIOError> <c CVisMemoryError>
//
////////////////////////////////////////////////////////////////////////////
class CVisError : public exception
{
// @access Public members
public:
	// @cmember
	// Constructor.
	CVisError(const char *szMessage, long lErrorCode,   
		      const char *szFunctionName = 0, 
			  const char *szSourceFile = 0, 
			  int iLine = 0) throw();

	// @cmember
	// Get the function name.
	const char *FunctionName(void) const;

	// @cmember
	// Get the error message.
	const char *ErrorMessage(void) const;

	// @cmember
	// Same as ErrorMessage.  (exception::what() is a virtual method.)
	virtual const char *what(void) const;

	// @cmember
	// Get the source file.
	const char *SourceFile(void) const;

	// @cmember
	// Get the line number.
	int SourceLine(void) const;

	// @cmember
	// Get the error code.
	long ErrorCode(void) const;
	
	// @cmember
	// Return a complete message.
	virtual const char *FullMessage(void);


	// @cmember
	// Since we don't have run-time type information, we have a virtual
	// ThrowLast() method to make sure that type information for
	// derived classes is not lost in try-catch blocks.
	virtual void ThrowLast(void);


// @access:(IMPL) Protected members
protected:
    // @cmember:(IMPL)
	// Function name that the error occurred in.
	const char *m_szFunctionName;

	// @cmember:(IMPL)
	// Error message.
	const char *m_szMessage;

	// @cmember:(IMPL)
	// Source file that the error occurred in.
	const char *m_szSourceFile;

    // @cmember:(IMPL)
	// Full error message.
	std::string m_strFullMessage;

	// @cmember:(IMPL)
	// Line number that the error occurred on.
	int  m_line;

	// @cmember:(IMPL)
	// Error code.
	long m_code;
};

////////////////////////////////////////////////////////////////////////////
//  
// @class
//
// Error objects for file I/O error handling.
//
// @base public | CVisError
//
// @xref <c CVisError>
//
////////////////////////////////////////////////////////////////////////////
class CVisFileIOError : public CVisError 
{
// @access Public members
public:
	// @cmember
	// Constructor.
	CVisFileIOError(const char *szFilename,
		            const char *szMessage, 
				    long lErrorCode = eviserrorUnknown, 
		            const char *szFunctionName = 0, 
				    const char *szSourceFile = 0,
				    int iLine = 0) throw();

	// @cmember
	// Return file name.
	const char *FileName() const;

	// @cmember
	// Return a complete message.
	virtual const char *FullMessage();

	// @cmember
	// Since we don't have run-time type information, we have a virtual
	// ThrowLast() method to make sure that type information for
	// derived classes is not lost in try-catch blocks.
	virtual void ThrowLast(void);

// @access:(IMPL) Private members
private:
	// @cmember:(IMPL)
	// File name.
	std::string m_strFilename;
};


////////////////////////////////////////////////////////////////////////////
//
// @class
//
// Error Objects for Memory Allocation Failure.
// It is not clear if this class is needed because 
// Windows will generate it's own exception for memory
// allocation problems.
//
// @base public | CVisError
//
// @xref <c CVisError>
//
////////////////////////////////////////////////////////////////////////////
class CVisMemoryError : public CVisError 
{
// @access Public members
public:
	// @cmember
	// Constructor.
	CVisMemoryError(const char *szFunctionName = 0,
			const char *szSourceFile = 0, int iLine = 0) throw();
	
	// @cmember
	// Don't attempt to allocate memory in CVisMemoryError::FullMessage.
	virtual const char *FullMessage(void);

	// @cmember
	// Since we don't have run-time type information, we have a virtual
	// ThrowLast() method to make sure that type information for
	// derived classes is not lost in try-catch blocks.
	virtual void ThrowLast(void);
};



////////////////////////////////////////////////////////////////////////////
// UNDONE:  This class should be redone when the Stream code is moved into
// the Vision SDK.  (Move this stub to VisCoreProj.h in the test project.)
class CVisSDIError : public CVisError
{
public:
	// UNDONE:  Add code to print debug strings in VisSDK exception ctors
	CVisSDIError(const char *szMessage, const char *szWhere, int iLine = - 1)
			throw()
	  : CVisError(szMessage, eviserrorFormat, szWhere), m_iLine(iLine)
		{}
	virtual ~CVisSDIError(void) throw() {};
	virtual int ILine(void) throw() { return m_iLine; };

	// @cmember
	// Since we don't have run-time type information, we have a virtual
	// ThrowLast() method to make sure that type information for
	// derived classes is not lost in try-catch blocks.
	virtual void ThrowLast(void);
private:
	int m_iLine;
};



////////////////////////////////////////////////////////////////////////////
//
//	Translation between codes and messages
//
////////////////////////////////////////////////////////////////////////////

inline long VisErrorMessageToCode(const char *szMessage,
		const char *rgszErrorMessage[],
		long rglErrorCode[]);

VisCoreExport const char * VISAPI VisErrorCodeToMessage(long lErrorCode,
		const char *rgszErrorMessage[],
		long rglErrorCode[]);


#include "VisError.inl"


#endif // __VIS_CORE_ERROR_H__

