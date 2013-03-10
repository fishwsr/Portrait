/**
 ****************************************************************************
 * <P> XML.c - implementation file for basic XML parser written in ANSI C++
 * for portability. It works by using recursion and a node tree for breaking
 * down the elements of an XML document.  </P>
 *
 * @version     V1.19
 * @author      Frank Vanden Berghen
 *
 * BSD license:
 * Copyright (c) 2002, Frank Vanden Berghen
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Frank Vanden Berghen nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************
 */
#ifndef __INCLUDE_XML_NODE__
#define __INCLUDE_XML_NODE__

#include <stdlib.h>

#ifndef WIN32
#include <wchar.h> // to have 'wcsrtombs' for ANSI version
                   // to have 'mbsrtowcs' for UNICODE version
#else
#include <tchar.h>
#endif

// Some common types for char set portable code
#ifdef _UNICODE
    #ifndef WIN32
        #define _T(c) L ## c
    #endif
    #ifndef LPCTSTR
        #define LPCTSTR const wchar_t *
    #endif /* LPCTSTR */
    #ifndef LPTSTR
        #define LPTSTR wchar_t *
    #endif /* LPTSTR */
    #ifndef TCHAR
        #define TCHAR wchar_t
    #endif /* TCHAR */
#else
    #ifndef WIN32
        #define _T(c) c
    #endif
    #ifndef LPCTSTR
        #define LPCTSTR const char *
    #endif /* LPCTSTR */
    #ifndef LPTSTR
        #define LPTSTR char *
    #endif /* LPTSTR */
    #ifndef TCHAR
        #define TCHAR char
    #endif /* TCHAR */
#endif
#ifndef FALSE
    #define FALSE 0
#endif /* FALSE */
#ifndef TRUE
    #define TRUE 1
#endif /* TRUE */


// Enumeration for XML parse errors.
typedef enum XMLError
{
    eXMLErrorNone = 0,
    eXMLErrorMissingEndTag,
    eXMLErrorEmpty,
    eXMLErrorFirstNotStartTag,
    eXMLErrorMissingTagName,
    eXMLErrorMissingEndTagName,
    eXMLErrorNoMatchingQuote,
    eXMLErrorUnmatchedEndTag,
    eXMLErrorUnexpectedToken,
    eXMLErrorInvalidTag,
    eXMLErrorNoElements,
    eXMLErrorFileNotFound,
    eXMLErrorFirstTagNotFound,
    eXMLErrorUnknownEscapeSequence
} XMLError;

// Enumeration used to manage type of data. Use in conjonction with structure XMLNodeContents
typedef enum XMLElementType
{
    eNodeChild=0,
    eNodeAttribute=1,
    eNodeText=2,
    eNodeClear=3,
    eNodeNULL=4
} XMLElementType;

// Structure used to obtain error details if the parse fails.
typedef struct XMLResults
{
    enum XMLError error;
    int  nLine,nColumn;
} XMLResults;

// Structure for XML clear (unformatted) node (usually comments)
typedef struct {
    LPCTSTR lpszOpenTag; LPCTSTR lpszValue; LPCTSTR lpszCloseTag;
} XMLClear;

// Structure for XML attribute.
typedef struct {
    LPCTSTR lpszName; LPCTSTR lpszValue;
} XMLAttribute;

struct XMLNodeContents;

typedef struct XMLNode
{
//  friend class XMLNode;
  protected:
    typedef struct XMLNodeDataTag // to allow shallow copy and "intelligent/smart" pointers (automatic delete):
    {
        LPCTSTR                lpszName;        // Element name (=NULL if root)
        int                    nChild,          // Num of child nodes
                               nText,           // Num of text fields
                               nClear,          // Num of Clear fields (comments)
                               nAttribute,      // Num of attributes
                               isDeclaration;   // Whether node is an XML declaration - '<?xml ?>'
        struct XMLNodeDataTag  *pParent;        // Pointer to parent element (=NULL if root)
        XMLNode                *pChild;         // Array of child nodes
        LPCTSTR                *pText;          // Array of text fields
        XMLClear               *pClear;         // Array of clear fields
        XMLAttribute           *pAttribute;     // Array of attributes
        int                    *pOrder;         // order in which the child_nodes,text_fields,clear_fields and
        int                    ref_count;       // for garbage collection (smart pointers)
    } XMLNodeData;
    XMLNodeData *d;

    // protected constructor: use one of these four methods to get your first instance of XMLNode:
    //  - parseString
    //  - parseFile
    //  - openFileHelper
    //  - createXMLTopNode
    XMLNode(XMLNodeData *pParent, LPCTSTR lpszName, int isDeclaration);

  public:

    // You can create your first instance of XMLNode with these 3 parse functions:
    // (see complete explanation of parameters below)

    static XMLNode parseString   (LPCTSTR      lpszXML, LPCTSTR tag=NULL, XMLResults *pResults=NULL);
    static XMLNode parseFile     (const char *filename, LPCTSTR tag=NULL, XMLResults *pResults=NULL, char automaticCharConversion=1);
    static XMLNode openFileHelper(const char *filename, LPCTSTR tag,                                 char automaticCharConversion=1);

    // The tag parameter should be the name of the first tag inside the XML file.
    // If the tag parameter is omitted, the 3 functions return a node that represents
    // the head of the xml document including the declaration term (<? ... ?>).

    // A note about the "automaticCharConversion" parameter:
    //     If "automaticCharConversion=1" and if this library is compiled in UNICODE mode, then
    //     the "parseFile" and "openFileHelper" functions will test if the file contains ASCII
    //     characters. If this is the case, then the file will be loaded and converted in memory to
    //     UNICODE before being parsed. If "automaticCharConversion=0", no conversion will be performed.
    //
    //     If "automaticCharConversion=1" and if this library is compiled in ASCII mode, then
    //     the "parseFile" and "openFileHelper" functions will test if the file contains UNICODE
    //     characters. If this is the case, then the file will be loaded and converted in memory to
    //     ASCII before being parsed. If "automaticCharConversion=0", no conversion will be performed.
    //
    //     It's useful to set sometime "automaticCharConversion=0" to disable any conversion because
    //     the test to detect the file-type (ASCII or UNICODE) may fail (rarely).

    // If the XML document is corrupted:
    //   - The "openFileHelper" method will stop execution and display an error message.
    //   - The 2 other methods will initialize the "pResults" variable with some information that
    //     can be used to trace the error.
    // you can have a detailed explanation of the parsing error with this function:
    static LPCTSTR getError(XMLError error);

    LPCTSTR getName();                                               // name of the node
    LPCTSTR getText(int i=0);                                        // return ith text field
    int nText();                                                     // nbr of text field
    XMLNode getChildNode(int i=0);                                   // return ith child node
    XMLNode getChildNode(LPCTSTR name, int i);                       // return ith child node with specific name
                                                                     //     (return an empty node if failing)
    XMLNode getChildNode(LPCTSTR name, int *i=NULL);                 // return next child node with specific name
                                                                     //     (return an empty node if failing)
    XMLNode getChildNodeWithAttribute(LPCTSTR tagName,               // return child node with specific name/attribute
                                      LPCTSTR attributeName,         //     (return an empty node if failing)
                                      LPCTSTR attributeValue=NULL,   //
                                      int *i=NULL);                  // 
    int nChildNode(LPCTSTR name);                                    // return the number of child node with specific name
    int nChildNode();                                                // nbr of child node
    XMLAttribute getAttribute(int i=0);                              // return ith attribute
    char isAttributeSet(LPCTSTR name);                               // test if an attribute with a specific name is given
    LPCTSTR getAttribute(LPCTSTR name, int i);                       // return ith attribute content with specific name
                                                                     //     (return a NULL if failing)
    LPCTSTR getAttribute(LPCTSTR name, int *i=NULL);                 // return next attribute content with specific name
                                                                     //     (return a NULL if failing)
    int nAttribute();                                                // nbr of attribute
    XMLClear getClear(int i=0);                                      // return ith clear field (comment)
    int nClear();                                                    // nbr of clear field
    LPTSTR createXMLString(int nFormat, int *pnSize=NULL);           // create XML string starting from current XMLNode
    XMLNodeContents enumContents(int i);                             // enumerate all the different contents (child,text,
                                                                     //     clear,attribute) of the current XMLNode. The order
                                                                     //     is reflecting the order of the original file/string
    int nElement();                                                  // nbr of different contents for current node
    char isEmpty();                                                  // is this node Empty?
    char isDeclaration();                                            // is this node a declaration <? .... ?>

// to allow shallow copy:
    ~XMLNode();
    XMLNode(const XMLNode &A);
    XMLNode& operator=( const XMLNode& A );

    XMLNode(): d(NULL){};
    static XMLNode emptyXMLNode;
    static XMLClear emptyXMLClear;
    static XMLAttribute emptyXMLAttribute;

    // The following functions allows you to create from scratch a XMLNode structure
    // The strings given as parameters for these methods will be free'd by the XMLNode class:
    static XMLNode createXMLTopNode();
    XMLNode addChild(LPCTSTR lpszName, int isDeclaration=FALSE);
	void setName(LPCTSTR lpszName);
    XMLAttribute *addAttribute(LPCTSTR lpszName, LPCTSTR lpszValuev);
    LPCTSTR addText(LPCTSTR lpszValue);
    XMLClear *addClear(LPCTSTR lpszValue, LPCTSTR lpszOpen, LPCTSTR lpszClose);
    XMLNode addChild(XMLNode nodeToAdd);                // If the "nodeToAdd" has some parents, it will be detached
                                                        // from it's parents before being attached to the current XMLNode

    // some deletion functions:
    void deleteNodeContent();                           // delete the content of this XMLNode and the subtree
    void deleteAttribute(int i=0);
    void deleteAttribute(XMLAttribute *anAttribute);
    void deleteAttribute(LPCTSTR lpszName);             // string parameter must "point" exactly to the memory
                                                        // location of the attribute's name to remove.
    void deleteAttributeRelaxed(LPCTSTR lpszName);      // string parameter can point to anywhere.
                                                        // the first attribute's name to match is deleted.
    void deleteText(int i=0);
    void deleteText(LPCTSTR lpszValue);
    void deleteClear(int i=0);
    void deleteClear(XMLClear *p);
    void deleteClear(LPCTSTR lpszValue);

private:

    // these are functions used internally (don't bother about them):
    static void destroyCurrentBuffer(XMLNodeData *d);
    int ParseClearTag(void *pXML, void *pClear);
    int ParseXMLElement(void *pXML);
    void addToOrder(int index, int type);
    static int CreateXMLStringR(XMLNodeData *pEntry, LPTSTR lpszMarker, int nFormat);
    static void *enumContent(XMLNodeData *pEntry,int i, XMLElementType *nodeType);
    static int nElement(XMLNodeData *pEntry);
    static void removeOrderElement(XMLNodeData *d, XMLElementType t, int index);
    static void exactMemory(XMLNodeData *d);
    static void detachFromParent(XMLNodeData *d);
} XMLNode;

// This structure is given by the function "enumContents".
typedef struct XMLNodeContents
{
    // This dictates what's the content of the XMLNodeContent
    enum XMLElementType type;
    // should be an union to access the appropriate data.
    // compiler does not allow union of object with constructor... too bad.
    XMLNode child;
    XMLAttribute attrib;
    LPCTSTR text;
    XMLClear clear;

} XMLNodeContents;

// The 2 following functions are processing strings so that all the characters
// &,",',<,> are replaced by their XML equivalent: &amp;, &quot;, &apos;, &lt;, &gt;.
// The second function ("toXMLStringFast") allows you to re-use the same output
// buffer for all the conversions so that only a few memory allocations are performed.
// If the output buffer is too small to contain the resulting string, it will
// be enlarged. These 2 functions are useful when creating from scratch an
// XML file using printf (or cout).
LPTSTR toXMLString(LPCTSTR source);
LPTSTR toXMLStringFast(LPTSTR *destBuffer,int *destSz, LPCTSTR source);
// you should not use this one (there is a possibility of "destination-buffer-overflow"):
LPTSTR toXMLString(LPTSTR dest,LPCTSTR source);

// duplicate (copy in a new allocated buffer) the source string
LPTSTR stringDup(LPCTSTR source, int cbData=0);

#endif
