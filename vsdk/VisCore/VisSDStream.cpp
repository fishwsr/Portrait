///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisSDStream.cpp -- self-describing stream format for data input/output
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
//  VisSDStream.h           longer description
//
//  Rick's research notes from Sept 13, 1997.
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////

#include "VisCorePch.h"
#include "VisCoreProj.h"

#ifdef VIS_INCLUDE_SDSTREAM

#include <direct.h>
// #include <VisCollection\VisCollection.h>
#include "VisSDStream.h"
#include <algorithm>


#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
#define THIS_FILE __FILE__
#endif // _DEBUG


// Global class to clean

//  Global static variable initialization

VisCoreExport CVisSDBaseV *CVisSDBase::type_list = 0;

static char *begin_description = "#pragma begin description";
static char   *end_description = "#pragma end description";

//
// Reading and writing descriptions
//

VisCoreExport CVisSDBase::CVisSDBase(int n_fields)
{
    if (type_list == 0)
        type_list = new CVisSDBaseV;
    m_type_list_id = TypeList().size();
    TypeList().push_back(this);     // push this object onto type list
    m_n_fields = n_fields;
}

VisCoreExport CVisSDBase::~CVisSDBase(void)
{
	// Note:  This assumes that CVisSDBase objects are all static, so that
	// their destructors are called after all file I/O has been completed.
	// LATER:  We should assert that no file I/O is attempted after the
	// first CVisSDBase destructor is called.
	if (type_list != 0)
	{
		delete type_list;
		type_list = 0;
	}
}

static std::string TrimmedString(std::string& s)
{
    int start = s.find_first_not_of(" \t");
    int comment = s.find("//");
    int last = s.find_last_not_of(" \t", comment);
    return (start >= 0) ? s.substr(start, last-start+1) : s;
}

VisCoreExport int CVisSDBase::MatchField(std::string& s)
{
    // Match the space-trimmed string against one of the fields
    for (int i = 0; i < m_n_fields; i++) {
        std::string desc = FieldDescriptor(i);
        std::string  s1 = TrimmedString(desc);
        std::string  s2 = TrimmedString(s);
        if (s1 == s2)
            return i;
    }
    return -1;
}

VisCoreExport CVisSDBase *CVisSDBase::MatchDescription(std::string& desc)
{
    // Match against one of the registered descriptors
	// UNDONE (in Rel1):  This should be made more efficient!
    for (CVisSDBaseV::iterator it  = TypeList().begin(); it != TypeList().end(); it++) {
        if ((*it)->MatchField(desc) == 0)   // matching eftName enum
            return (*it);
    }
    return 0;
}

VisCoreExport void CVisSDBase::ReadDescription(CVisSDStream& s)
{
    // Find the specific object corresponding to class name
    std::string  name = s.GetLine();
    CVisSDBase *obj = MatchDescription(name);
    CVisIntV& af = *(obj ? &obj->ActiveFields(1) : 0);
    if (obj)
        af.clear();
#ifdef _DEBUG
    else
		OutputDebugString("CVisSDBase::ReadDescription:  did not find class!");

	// LATER:  Improve this part
//        CVisDiagnostics::Warning("CVisSDBase::ReadDescription:  did not find class %s",
//            &name[0]);
#endif // _DEBUG

    // Fill in the read fields
    for (std::string  field = s.GetLine(); field[0] != '}'; field = s.GetLine()) {
        if (obj) {
            int i = obj->MatchField(field);
            af.push_back(i);    // push back -1's as well (unrecognized field)
        }
    }
    s.SkipSpace();  // skip trailing blanks, and look for #pragma
}

VisCoreExport void CVisSDBase::ResetDescriptions()
{
    // Default the active read fields to write fields
    for (CVisSDBaseV::iterator it  = TypeList().begin();
         it != TypeList().end(); it++) {
        CVisIntV& rf = (*it)->ActiveFields(1);
        CVisIntV& wf = (*it)->ActiveFields(0);
        rf.resize(wf.size());
        std::copy(wf.begin(), wf.end(), rf.begin());
    }
}

VisCoreExport void CVisSDBase::ReadDescriptions(CVisSDStream& s)
{
    s.SkipSpace();      // skip initial comments
    while (s.Status() == CVisSDStream::ReadDescription) { // find begin_description
        ReadDescription(s);
    }
}

VisCoreExport void CVisSDBase::WriteDescription(CVisSDStream& s, CVisSDBase *obj)
{
    FILE *stream = s.Stream();
    CVisIntV& af = obj->ActiveFields(0);
    const char *name = obj->FieldDescriptor(0);
    fputs("\n", stream);
    fputs(name, stream);
    for (CVisIntV::iterator fi = af.begin(); fi != af.end(); fi++) {
        const char *desc = obj->FieldDescriptor(*fi);
        fputs("\n    ", stream); // indent
        fputs(desc, stream);
    }
    fputs("\n};\n", stream);
}

VisCoreExport void CVisSDBase::WriteDescriptions(CVisSDStream& s)
{
    fprintf(s.Stream(), "\n%s\n", begin_description);
    s.m_status = CVisSDStream::WriteDescription;

    for (int i = 0; i < TypeList().size(); i++) {
        if (s.DescriptionMarked(i) && ! s.DescriptionWritten(i)) {
            WriteDescription(s, TypeList()[i]);
            s.MarkDescription(i, 2);    // mark as written
        }
    }
    s.m_status = CVisSDStream::Write;
    fprintf(s.Stream(), "\n%s\n\n", end_description);
}

//
//  Stream class implementation
//

VisCoreExport CVisSDStream::~CVisSDStream()
{
    Close();    // can add more code or stop debugger here
}

VisCoreExport void CVisSDStream::Open(const char* filename, status status, FILE *istream)
{
    // Clear out the description flags
    int n_fields = CVisSDBase::TypeListSize();
    m_write_description.resize(n_fields, 0);

    // Open the appropriate file (code taken from VisCollection.cpp)
    m_stream = istream;
    if (m_stream == 0) {
    if (filename == 0 || filename[0] == 0 || strcmp(filename, "-") == 0)
        m_stream = (status == Read) ? stdin : stdout;
    else
        m_stream = fopen(filename, (status == Read) ? "r" : "w");
    if (m_stream == 0)
        throw CVisFileIOError(filename, "could not open file for reading",
                eviserrorOpen, "CVisSDStream::Open", __FILE__, __LINE__);
    } else
        filename = "";  // indicates no file
    m_filename = filename ? filename : "";
    m_status = status;

    // Push onto the stack
    m_filename_l.push(m_filename);
    m_stream_l.push(m_stream);

    // Set the working directory
    std::string  working_dir = filename;
    if (m_stream != stdin) {
        char *e1 = strrchr(filename, '/'), *e2 = strrchr(filename, '\\');
        int i = (e2) ? e2 - filename : (e1) ? e1 - filename : -1;
        if (i >= 0) {
            working_dir[i] = 0;
            _chdir(&working_dir[0]);
        }
    }

    // Reset the descriptions
    if (status == Read)
        CVisSDBase::ResetDescriptions();
}

VisCoreExport void CVisSDStream::CloseOne()
{
    // close an #include file's stream
    if (m_filename != "" && m_filename != "-") {
        int code = fclose(m_stream);
        if (code != 0)
            throw CVisFileIOError(m_filename, "error closing file",
                eviserrorOpen, "CVisSDStream::CloseOne", __FILE__, __LINE__);
    }
    m_filename_l.pop();
    m_stream_l.pop();
    if (!m_stream_l.empty()) {
        m_stream = m_stream_l.top();
        m_filename = m_filename_l.top();
    } else {
        m_status = Closed;
        m_stream = 0;   // redundant
    }
}

VisCoreExport void CVisSDStream::Close()
{
    // close all included streams
    while (! m_stream_l.empty())
        CloseOne();
}

VisCoreExport std::string  CVisSDStream::GetLine()
{
    char command[1024];
    if (fgets(command, 1024, m_stream) == NULL) {
        throw CVisFileIOError(m_filename, "end of file", eviserrorRead);
    }
    if (command[strlen(command)-1] == '\n') {
        m_line_number += 1;
        command[strlen(command)-1] = 0;     // strip newline
    }
    return std::string (command);
}

// Reading/writing utilities (work both ways)

VisCoreExport void CVisSDStream::SkipSpace()
{
    // Skip comments and whitespace, and process includes
    if (m_status != Read && m_status != ReadDescription)
        return;

    while (! feof(m_stream)) {
        
        // Skip whitespace
        int c = GetChar();
        while (isspace(c) && !feof(m_stream))
            c = GetChar();

        // Dispatch based on character
        switch (c) {

        case '/':
            if (GetChar() == '/')                      // skip rest of line
                while(GetChar() != '\n' && !feof(m_stream));
            else {
                char msg[100];
                sprintf(msg, "unexpected '/' at line %d", m_line_number);
                throw CVisFileIOError(m_filename, msg, eviserrorRead);
            }
            break;

        case '#':
            {
                ungetc(c, m_stream);
                std::string  line = GetLine();
                char *command = &line[0];
                if (strcmp(command, "#include") == 0) {
                    // Open a new file for reading
                    CVisString filename;
                    *this << std::string (filename);
                    Open(filename, Read);
                } else if (strcmp(command, begin_description) == 0) {
                    // Start reading description
                    m_status = ReadDescription;
                    CVisSDBase::ReadDescriptions(*this);
                } else if (strcmp(command, end_description) == 0) {
                    // Stop reading description
                    m_status = Read;
                } else {
                    char msg[100];
                    sprintf(msg, "unexpected '#' at line %d", m_line_number);
                    throw CVisFileIOError(m_filename, msg, eviserrorRead);
                }
            }
            break;

        default:    // finished skipping
            ungetc(c, m_stream);
            return;
        }

        // Check if current input has been exhausted
        if (feof(m_stream)) {
            if (m_stream_l.size() > 1)
                CloseOne();
        }
    }

    // Should we throw an exception is we still have an feof here?
    if (feof(m_stream)) {
        throw CVisFileIOError(m_filename, "Unexpected end of file", eviserrorRead);
    }
}

VisCoreExport void CVisSDStream::OneChar(int c, int indent)
{
    // read or write a single character
    if (m_status == Write) {
        fputc(c, m_stream);
        if (m_indent_level >= 0)
            m_indent_level += indent;
    } else if (m_status == Read || m_status == ReadDescription) {
        SkipSpace();
        int cin = GetChar();
        if (cin != c) {
            char msg[100];
            sprintf(msg, "expecting a '%c' at line %d", c, m_line_number);
            throw CVisFileIOError(m_filename, msg, eviserrorRead);
        }
    }
}

VisCoreExport int CVisSDStream::PeekChar()
{
    // peek at next real character
    if (m_status == Read || m_status == ReadDescription) {
        SkipSpace();
        int c = GetChar();
        ungetc(c, m_stream);    // can't be a newline, so m_line_number o.k.
        return c;
    }
    return 0;
}

VisCoreExport int CVisSDStream::SomeChar(const char *s)
{
    // expecting one of several characters
    if (m_status == Read || m_status == ReadDescription) {
        int c = PeekChar();
        if (strchr(s, c) == 0) {
            char msg[100];
            sprintf(msg, "expecting a \"%s\" at line %d", s, m_line_number);
            throw CVisFileIOError(m_filename, msg, eviserrorRead);
        }
        return c;
    }
    return 0;
}

VisCoreExport void CVisSDStream::Comment(const char* comment)
{
    // write a comment string
    if (m_status == Write) {
        const char *p = comment;
        while (*p == '\n')          // add newlines before comment
            NewLine(), p++;
        if (p == comment)
            fputs("  ", m_stream);    // put two spaces before comment
        fputs("// ", m_stream);       // put another space after //
        fputs(p, m_stream);
        NewLine();        // start a new line
    }
}

// UNDONE (in Rel1):  Add a static flag to specify whether newlines should
// be written with each OpenParen and CloseParen call.  (With that option,
// we could get the indenting right.)
VisCoreExport void CVisSDStream::OpenParen()
{
    // open left parenthesis
    OneChar('{', 1);
}

VisCoreExport void CVisSDStream::Comma()
{
    // insert a comma
    OneChar(',', 0);
}

VisCoreExport void CVisSDStream::CloseParen()
{
    // close right parenthesis
    OneChar('}', -1);
}

VisCoreExport void CVisSDStream::NewLine()
{
    // add a newline and optionally indent
    if (m_status == Write) {
        fputc('\n', m_stream);
        for (int i = 0; i < m_indent_level; i++)
            fputc(' ', m_stream);
    }
}

VisCoreExport void CVisSDStream::SkipItem()
{
    // Skip a single item, or list of items/lists
    if (Status() != Read)
        return;
    int c = PeekChar();
    if (c == '{') {     // list
        SkipList();
    } else if (c == '"' || c == '\'') {     // string or char
        std::string  v;
        QuotedString(c, v);
    } else {            // must be a number
        double v;
        *this << v;
    }
}

VisCoreExport void CVisSDStream::SkipList()
{
    // Skip a balanced list of items
    OpenParen();
    while (PeekChar() != '}') {
        SkipItem();
        if (SomeChar(",}") == ',')
            Comma();
    }
    CloseParen();
}

//  More specific data formatting

VisCoreExport void CVisSDStream::QuotedString(int q, std::string & s)
{
    // Read/write a quoted string
    if (m_status == Write) {
        fputc(q, m_stream);
        for (const char *p = &s[0]; p[0]; p++) {
            if (*p == '"' || *p == '\\')
                fputc('\\', m_stream);
            fputc(*p, m_stream);
        }
        fputc(q, m_stream);
    } else if (m_status == Read || m_status == ReadDescription) {
        OneChar(q, 0);
        s.erase();
        for (int c = GetChar(); c != q && ! feof(m_stream);
                 c = GetChar()) {
            if (c == '\\')
                s += char(GetChar());
            else
                s += char(c);
        }
    }
}


template <class T>
inline CVisSDStream& NumericWrite(CVisSDStream& s, const char *fmt, T& v,
		bool fAlwaysUseFmt = false)
{
    // Read/write a numeric quatity (shorten to int, if possible)
    if (s.Status() == CVisSDStream::Write) {
        int iv = int(v);
        if ((!fAlwaysUseFmt) && (fabs(double(iv - v)) < 1.0e-12)
				&& (-9999 <= iv) && (iv <= 9999)) {
            fprintf(s.Stream(), "%d", iv);
		} else {
            fprintf(s.Stream(), fmt, v);
		}
    } else if (s.Status() == CVisSDStream::Read) {
        s.SomeChar("0123456789-.");
        fscanf(s.Stream(), fmt, &v);
    }
    return s;
}


VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, bool& v)
{
	int w = ((v) ? 1 : 0);

    if (s.Status() == CVisSDStream::Read)
	{
		CVisSDStream& sRet = NumericWrite(s, "%d", w);
		v = ((w == 0) ? false : true);
		return sRet;
	}


	return NumericWrite(s, "%d", w);
}


VisCoreExport CVisSDStream& CVisSDStream::Num(signed char& num, bool fUseHexForInts)
{
	int w = (int) num;
	const char *szFmt = (fUseHexForInts ? "0x%x" : "%d");
    if (Status() == CVisSDStream::Write) {
		NumericWrite(*this, szFmt, w, true);
    } else if (Status() == CVisSDStream::Read) {
		NumericWrite(*this, szFmt, w, true);
		num = (signed char) w;
    }

	return *this;
}

VisCoreExport CVisSDStream& CVisSDStream::Num(unsigned char& num, bool fUseHexForInts)
{
	unsigned int n = (unsigned int) num;
	const char *szFmt = (fUseHexForInts ? "0x%x" : "%u");
    if (Status() == CVisSDStream::Write) {
		NumericWrite(*this, szFmt, n, true);
    } else if (Status() == CVisSDStream::Read) {
		NumericWrite(*this, szFmt, n, true);
		num = (unsigned char) n;
    }

	return *this;
}

VisCoreExport CVisSDStream& CVisSDStream::Num(short& num, bool fUseHexForInts)
{
 
	return NumericWrite(*this, (fUseHexForInts ? "0x%hx" : "%hd"), num, true);
}

VisCoreExport CVisSDStream& CVisSDStream::Num(int& num, bool fUseHexForInts)
{
    return NumericWrite(*this, (fUseHexForInts ? "0x%x" : "%d"), num, true);
}

VisCoreExport CVisSDStream& CVisSDStream::Num(long& num, bool fUseHexForInts)
{
    return NumericWrite(*this, (fUseHexForInts ? "0x%lx" : "%ld"), num, true);
}

VisCoreExport CVisSDStream& CVisSDStream::Num(unsigned short& num, bool fUseHexForInts)
{
    return NumericWrite(*this, (fUseHexForInts ? "0x%hx" : "%hu"), num, true);
}

VisCoreExport CVisSDStream& CVisSDStream::Num(unsigned int& num, bool fUseHexForInts)
{
    return NumericWrite(*this, (fUseHexForInts ? "0x%x" : "%u"), num, true);
}

VisCoreExport CVisSDStream& CVisSDStream::Num(unsigned long& num, bool fUseHexForInts)
{
    return NumericWrite(*this, (fUseHexForInts ? "0x%lx" : "%lu"), num, true);
}

VisCoreExport CVisSDStream& CVisSDStream::Num(float& num, bool fUseHexForInts)
{
    return NumericWrite(*this, "%g", num);
}

VisCoreExport CVisSDStream& CVisSDStream::Num(double& num, bool fUseHexForInts)
{
    return NumericWrite(*this, "%lg", num);
}


// LATER:  << for unsigned char, __int64, unsigned __int64.
VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, short& v)
{
    return s.Num(v);
}

VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, int& v)
{
    return s.Num(v);
}

VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, long& v)
{
    return s.Num(v);
}

VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, unsigned char& v)
{
    return s.Num(v);
}

VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, unsigned short& v)
{
    return s.Num(v);
}

VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, unsigned int& v)
{
    return s.Num(v);
}

VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, unsigned long& v)
{
    return s.Num(v);
}

VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, float& v)
{
    return s.Num(v);
}

VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, double& v)
{
    return s.Num(v);
}


// Signed char is a character, not a number.
VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, signed char& v)
{
    std::string  str; str += v;
    s.QuotedString('\'', str);
    v = str[0]; // read back
    return s;
}

VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, std::string & v)
{
    s.QuotedString('"', v);
    return s;
}

//
//  Other commonly used data types;
//    TODO: these should be migrated into the class header files...
//


VisCoreExport CVisSDStream& VISAPI operator<<(CVisSDStream& s, CVisRect& r)
{
	s.OpenParen();

#ifdef LATER // UNDONE:  We'll need to change existing files to use the new order.

	s << r.left;
	s.Comma();
	s << r.top;
	s.Comma();
	s << r.right;
	s.Comma();
	s << r.bottom;

#else // LATER

	s << r.left;
	s.Comma();
	s << r.top;
	s.Comma();

    if (s.Status() == CVisSDStream::Write)
	{
        int dy = r.Height();
        int dx = r.Width();

		s << dx;
		s.Comma();
		s << dy;
    }
	else if (s.Status() == CVisSDStream::Read)
	{
        int dy = 0;
        int dx = 0;

		s << dx;
		s.Comma();
		s << dy;

		r.right = r.left + dx;
		r.bottom = r.top + dy;
    }

#endif // LATER

	s.CloseParen();

    return s;
}


// For now, we need static exemplars for common pixel and image types.
// LATER:  What file do these belong in?  Can we avoid the need for them?

//VisCoreExport CVisSDObject< CVisImageBase > g_visimagebaseExemplar;

//VisCoreExport CVisSDObject< CVisSequenceBase > g_vissequencebaseExemplar;
#endif