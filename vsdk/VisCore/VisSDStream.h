///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisSDStream.h -- self-describing stream format for data input/output
//
// DESCRIPTION
//  The CVisSDStream class is used to read and write ASCII descriptions of
//  MSVisSDK objects.  The format for each class is printed at the top of
//  the file.  This allows data files to remain somewhat forward and
//  backward compatible, as fields are added/removed to/from classes.
//
//  The class declarations follow usual C++ syntax, with some restrictions:
//  . exactly one field per line
//  ...
//
//  Comments and whitespace can be interspersed with the declarations and data.
//  The data itself uses the curly bracket and comma separated syntax
//  of C/C++ data initialization.  Variable length lists and sets
//  are supported.
//
//  Each class which wants to be printable is responsible for providing
//  its own reading/writing routine.  Each class which wants to be described
//  must provide a printable lists of fields currently supported.  These
//  fields are accesses through the 
//
// SEE ALSO
//  VisSDStream.cpp           implementation
//
//  Rick's research notes from Sept 13, 1997.
//
// BUGS
//
//
// DESIGN
//  The current input/output is written on top of the FILE <stdio.h>
//  routines (fprintf, fscanf).  They could be re-written to operate
//  on streams, if desired (however, separate internal input and output
//  streams would be required).  This would enable output to strings,
//  for debugging purposes.
//
//  To maintain backward compatibility, we can use one of two strategies:
//  1. require newer software to recognize old field, even if these
//      are discarded during the read process
//  2. have some way to skip unrecognized fields.  This requires the
//      syntax of data to be restricted, e.g., bracketed numbers,
//      strings and characters.
//  The second approach has the advantage that code is compatible both
//  ways, i.e., old code can deal with newer data which has extra fields.
//
// Warning:  The format of these files may change in future releases.
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

#ifndef _VIS_SD_STREAM_H_
#define _VIS_SD_STREAM_H_

#ifdef VIS_SDK_NO_IMAGES

#define VISAPI __stdcall
#define VisCoreExport __declspec(dllimport)

#pragma warning(disable : 4251)  // copy constructors

// UNDONE: For now, CVisString is defined in VisDiagnosticsMsg.h
#include "VisDiagnosticsMsg.h"

#else VIS_SDK_NO_IMAGES

#include "VisCoreProj.h"

#endif // VIS_SDK_NO_IMAGES


// disable warning C4786: symbol greater than 255 character,
// okay to ignore   (found in STL Sample Programs)
#pragma warning(disable: 4786)

#include <stdio.h>
#include <assert.h>

#include <list>
#include <set>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>


class CVisSDStream; // forward declaration
class CVisSDBase;   // forward declaration
class CVisRect;   // forward declaration

// Redefined STL containers

// UNDONE:  Go through the code and remove references to the allocator in STL
// template lists when std::allocator is the default template value.
typedef std::vector<int, std::allocator<int> > CVisIntV;

// VC5.0 compiler generates warning message C4768 which I can't disable with
// #pragma warning( disable : 4768 )
// so I defined an allocator class to shorten the munged class name
struct CVisIntA : std::allocator<int> {};
struct CVisIntS : std::set<int, std::less<int>, CVisIntA> {};


// VC5.0 compiler expects these operators to be defined for all types used in vectors:
template <class T>
class CVisSortable {
public:
    inline bool operator<(const T& a) const;
    inline bool operator==(const CVisSortable<T>& a) const;
};

/*
struct CVisString : public std::string {  // re-name, for better mangling
    inline CVisString(void);
    inline CVisString(std::string s);
    inline CVisString(const char *s);
    inline operator const char*(void);
    inline char& operator[](int i);
};

*/
typedef std::deque<CVisString, std::allocator<CVisString> > CVisStringDq;
typedef std::stack<CVisString, CVisStringDq > CVisStringStk;

void erase(CVisIntV& v, int i);



#ifdef VIS_INCLUDE_SDSTREAM


// Base class for self-describing printing objects

typedef std::vector<CVisSDBase *, std::allocator<CVisSDBase *> > CVisSDBaseV;

class CVisSDBase {
public:
    VisCoreExport CVisSDBase(int n_fields);   // common initialization
	VisCoreExport virtual ~CVisSDBase(void);

    // Type-specific formatting info
    virtual const char *FieldDescriptor(int field_id) = 0;
    CVisIntV& ActiveFields(int read);
    int TypeListId(void);
    static int TypeListSize(void);

    friend class CVisSDStream;

private:
    // Reading and writing descriptions
    VisCoreExport static void ResetDescriptions(void);
    VisCoreExport static void ReadDescriptions(CVisSDStream& s);
    VisCoreExport static void WriteDescriptions(CVisSDStream& s);

    // Description read/write helper functions
    VisCoreExport int MatchField(std::string& s);
    VisCoreExport static CVisSDBase *MatchDescription(std::string& desc);
    VisCoreExport static void ReadDescription(CVisSDStream& s);
    VisCoreExport static void WriteDescription(CVisSDStream& s, CVisSDBase *obj);

    // Global list of types (descriptors)
    static CVisSDBaseV& TypeList(void);
    VisCoreExport static CVisSDBaseV *type_list;  // must be pointer, else init fails!
    int m_type_list_id;             // position in type list

protected:
    // Type-specific fields to be read/written
    int m_n_fields;                 // number of possible fields
    CVisIntV m_read_fields;         // read these fields
    CVisIntV m_write_fields;        // write these fields
};


// Templated class for per-type printing, based on fields

template <class T>
class CVisSDObject : public CVisSDBase {
public:

    // Default is for all fields to be written: override in AdjustFields();
    CVisSDObject(void);

    // Field description information
    virtual const char *FieldDescriptor(int field_id);

    // Field manipulation
    void AdjustFields(void);      // type dependant, erase selected fields
    void InsertField(int f);
    void EraseField(int f);

	// There should be a static object of this type somewhere for
	// initialization and ActFld access.  (We define an access method in
	// this class to allow different storage specifiers to be used on the
	// actual static object.)
    // LATER:  Find a way to remove this.
	static CVisSDObject& Exemplar(void);

private:
    T m_object;             // needed for ReadWriteField call
    CVisSDStream m_dummy;   // needed for ReadWriteField call
};


//  Self-describing input/output stream


class CVisSDStream {
public:
    enum status { Read, ReadDescription, Write, WriteDescription, 
                  CollectDescription, Closed };

    // Construction and destruction
    CVisSDStream(int indent = -1);     // indent < 0  =>  no indentation
    VisCoreExport ~CVisSDStream(void);

    // Opening, closing, and status
    VisCoreExport void Open(const char* filename, status s, FILE *stream = 0);
    VisCoreExport void CloseOne(void);                    // close an #include file's stream
    VisCoreExport void Close(void);                       // close all included streams
    FILE * Stream(void);
    status Status(void);

    // Reading and writing descriptions
    void CollectDescriptions(void);
    void WriteOutDescriptions(void);
    void MarkDescription(int i, int v = 1);
    int DescriptionMarked(int i);
    int DescriptionWritten(int i);

    // Reading/writing utilities (work both ways)
    VisCoreExport void SkipSpace(void);               // skip comments and whitespace
    VisCoreExport void OneChar(int c, int indent);    // read or write a single character
    VisCoreExport int  PeekChar(void);                // peek at next real character
    VisCoreExport int  SomeChar(const char *s);       // expecting one of several characters
    VisCoreExport void Comment(const char* comment);  // write a comment string
    VisCoreExport void OpenParen(void);               // open left parenthesis
    VisCoreExport void Comma(void);                   // insert a comma
    VisCoreExport void CloseParen(void);              // close right parenthesis
    VisCoreExport void NewLine(void);                 // add a newline and optionally indent
    VisCoreExport void SkipItem(void);                // skip a single item (list, number, ...)
    VisCoreExport void SkipList(void);                // skip a whole list
    VisCoreExport void QuotedString(int q, std::string& s);  // read/write a quoted string

	// Read / write standard numeric types.
	VisCoreExport CVisSDStream& Num(signed char& num, bool fUseHexForInts = false);
	VisCoreExport CVisSDStream& Num(short& num, bool fUseHexForInts = false);
	VisCoreExport CVisSDStream& Num(int& num, bool fUseHexForInts = false);
	VisCoreExport CVisSDStream& Num(long& num, bool fUseHexForInts = false);
	VisCoreExport CVisSDStream& Num(unsigned char& num, bool fUseHexForInts = false);
	VisCoreExport CVisSDStream& Num(unsigned short& num, bool fUseHexForInts = false);
	VisCoreExport CVisSDStream& Num(unsigned int& num, bool fUseHexForInts = false);
	VisCoreExport CVisSDStream& Num(unsigned long& num, bool fUseHexForInts = false);
	VisCoreExport CVisSDStream& Num(float& num, bool fUseHexForInts = false);
	VisCoreExport CVisSDStream& Num(double& num, bool fUseHexForInts = false);

private:
    status m_status;                    // stream status
    CVisString m_filename;              // current filename
    FILE * m_stream;                    // current stream
    CVisStringStk m_filename_l;         // list of open filenames
    std::stack<FILE *, std::deque<FILE *, std::allocator<FILE *> > >
        m_stream_l;                     // list of open streams
    int m_indent_level;                 // indentation level
    int m_line_number;                  // current line number (reading)

    CVisIntV m_write_description;       // list of descriptions (to be) written

    int  GetChar(void);                 // get next character (+ increment line)
    VisCoreExport std::string GetLine(void);          // get line + increment line count

    friend class CVisSDBase;
};


//  Simple data type input/output
//    these aren't templated, since we may wish to "quote" the data

// Boolean values (written as 0 or 1)
VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, bool& v);

// Numbers
VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, short& v);
VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, int& v);
VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, long& v);
VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, unsigned char& v);
VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, unsigned short& v);
VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, unsigned int& v);
VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, unsigned long& v);
VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, float& v);
VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, double& v);

// Characters and strings
VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, signed char& v);
VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, std::string& v);

//
//  Other commonly used data types;
//    TODO: these should be migrated into the class header files...
//

// UNDONE:  Should this use a RECT& as a parameter?
VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, CVisRect& r);

// Templates for classes with ReadWriteField descriptors

template <class T>
inline CVisSDStream& VisSDReadWriteClass(CVisSDStream& s, T& v);

// Templates for STL container classes

template <class T, class E>
inline CVisSDStream& VisSDReadWriteContainer(CVisSDStream& s, T& v, E def);

template <class T, class E>
inline CVisSDStream& VisSDReadWriteContainerP(CVisSDStream& s, T& v, E *ptr);

template <class T>
inline CVisSDStream& operator<<(CVisSDStream& s,
		std::vector<T, std::allocator<T> >& v);

template <class T>
inline CVisSDStream& operator<<(CVisSDStream& s,
		std::list<T, std::allocator<T> >& v);

template <class T>
inline CVisSDStream& operator<<(CVisSDStream& s,
		std::deque<T, std::allocator<T> >& v);

template <class T>
inline CVisSDStream& operator<<(CVisSDStream& s,
		std::set<T, std::less<T>, std::allocator<T> >& v);


#else // VIS_INCLUDE_SDSTREAM
//class CVisSDStream;
#endif // VIS_INCLUDE_SDSTREAM

#include "VisSDStream.inl"


#endif // _VIS_SD_STREAM_H_
