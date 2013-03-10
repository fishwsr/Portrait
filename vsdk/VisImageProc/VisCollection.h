///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisCollection.h -- image collection
//
// DESCRIPTION
//  CVisCollection is used to store and create collections of images,
//  e.g., multi-layer image compositions or image sequences.
//
//  The collections are stored as an array of frames, each of which
//  contains an image pointer, potentially a location/geometry,
//  and other ancilliary information (e.g., sequence id, timestamp, ...).
//
//  A collection can also have a rendered view, which is a composition
//  of the underlyting frames.  For a plain image sequence, the view
//  is simply a pointer to the currently "topmost" (selected) frame image.
//
// PUBLIC MEMBER FUNTIONS (METHODS)
//  CVisCollection();
//  ~CVisCollection();
//
//  int NFrames();
//  int CurrentCVisFrame();
//  void SetCurrentCVisFrame(int index);
//  void SetFirst();
//  void SetLast();
//  void SetNext();
//  void SetPrev();
//
//  void Insert(CVisFrameType &frame, int index = -1);
//  void Insert(T &image, int index = -1);
//  void Raise(int index, int to_top = 1);
//  void Lower(int index, int to_bottom = 1);
//  void Delete(int index);
//
//  CVisFrameType& operator[](int index);
//  CVisFrameType& operator()();
//
//  int ReadCollection(const char *filename, int read_files = 0);
//  int WriteCollection(const char *filename, int write_files = 0);
//  int ReadVideo(const char *filename);
//  int WriteVideo(const char *filename);
//  int ReadFile(int index);
//  int WriteFile(int index);
//  int ReadFiles();
//  int WriteFiles();
//  int AddFile(const char *filename, int read_file = 0, int de_interlace = 0);
//  int AddFiles(const char *filenames, int read_files = 0, int de_interlace = 0);
//
// PARAMETERS
//  index           index of current frame
//  collection_type type of collection (sequence, composition, ...)
//  frame           a single frame (image) and ancilliary information
//  filename        collection or image filename
//  read_files      read files right now (or defer till accessed with [])
//  write_files     write files (not just the collection description)
//  de_interlace    convert input image(s) into two frames: even and odd
//
// SEE ALSO
//  VisCollection.cpp     collection implementation code
//  VisComposite.h        compositions / views of image collections
//
// BUGS
//   
//
// DESIGN
//  We may want to split this file into VisFrame and VisCollection.  Also,
//  a suggestion was made that the base VisFrame class not contain any
//  geometry, and that the geometry be put into a derived class.
//  Other types of collections might include images from a single AVI file
//  tagged with time stamps, files from a directory, or files retrieved
//  by an image database search engine.
//  Note that the only geometry-dependent elements of class CVisFrame
//  are m_geometry, which stores the meta-information; Geometry(), which
//  can set or retrieve it; and ReadDescription and WriteDescription,
//  which perform i/o of the meta-information.  Issues:  How best to
//  represent more general meta-information; whether to associate meta-
//  information with the Collection, with each image via a Frame, or
//  even within the Image object itself.
//  Aside from the meta-information, the Frame provides a file association
//  for an image.  This might be generalized e.g. in the case of an image
//  which is a frame from an AVI file.
//
// HISTORY
//  05-Jun-96  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//  
//  CLASS:
//      CVisFrame
//  
//  DECLARATION:
//      template <class TypedImage> class CVisFrame : public TypedImage::
//  
//  MEMBER FUNCTIONS:
//      public:
//          CVisFrame();
//          CVisFrame(const CVisFrame &f);
//          CVisFrame(const char *filename);
//          CVisFrame(CVisShape shape);
//          CVisFrame(const TypedImage &image);
//          ~CVisFrame();
//          TypedImage& Image();
//          char* Filename();
//          void SetImage(const TypedImage& image);
//          void SetFilename(const char *filename);
//          CVisGeometry& Geometry();
//          virtual void ReadImage();
//          virtual void WriteImage();
//          virtual void ReadDescription (FILE *stream, const char *desc_type);
//          virtual void WriteDescription(FILE *stream, const char *desc_type);
//      private:
//  
////////////////////////////////////////////////////////////////////////////
//  
//  CLASS:
//      CVisCollection
//  
//  DECLARATION:
//      template <class CVisFrameType> class CVisCollection::
//  
//  MEMBER FUNCTIONS:
//      public:
//          CVisCollection();
//          ~CVisCollection();
//          CVisCollection(const CVisCollection<CVisFrameType>& c);
//          CVisCollection<CVisFrameType>& operator=(const CVisCollection<CVisFrameType>& c);
//          void CreateCollection(int n_frames, int n_rows, int n_cols);
//          int NFrames();
//          int IsEmpty();
//          int CurrentCVisFrame();
//          void SetCurrentCVisFrame(int index);
//          void SetFirst();
//          void SetLast();
//          void SetNext();
//          void SetPrev();
//          void Insert(CVisFrameType &frame, int index = -1);
//          void Insert(const char *filename, int index = -1);
//          void Raise(int index, int to_top = 1);
//          void Lower(int index, int to_bottom = 1);
//          void Delete(int index);
//          void DeleteAll();
//          CVisFrameType& operator [] (int index);
//          CVisFrameType& operator [] (int index);
//          CVisFrameType& CVisFrame(int index);
//          CVisFrameType& CVisFrame(int index);
//          CVisFrameType& CVisFrame();
//          CVisFrameType& CVisFrame();
//          void ReadCollection(const char *filename,
//                      int read_files = 0);
//          void WriteCollection(const char *filename, const char *desc_type = 0,
//                      int write_files = 0);
//          void ReadCollection (FILE *stream);
//          void WriteCollection(FILE *stream);
//          virtual void ReadMetaData(FILE *stream);
//          virtual void WriteMetaData(FILE *stream);
//          void ReadFiles();
//          void WriteFiles();
//          void AddFile(const char *filename, int read_file = 0);
//      protected:
//  
////////////////////////////////////////////////////////////////////////////

#ifndef __VIS_COLLECTION_H__
#define __VIS_COLLECTION_H__

#include "VisMatrix.h"

//
//  CVisFrame class:  hold one image in a sequence or collection
//

//
//  Collection class for holding multiple images
//


////////////////////////////////////////////////////////////////////////////
//  
//  CLASS:
//      CVisCollection
//  
////////////////////////////////////////////////////////////////////////////
template <class TPixel>
class CVisCollection : public CVisFrameSequence<TPixel>
{
public:
	typedef TPixel T_Pixel;
	typedef CVisImage<TPixel> T_Image;
	typedef CVisFrame<TPixel> T_Frame;
	typedef CVisFrameSequence<TPixel>	T_BaseSequence;


    // Constructor and destructor
    CVisCollection(EVisSequence evissequence = evissequenceDelayReadUntilNeeded);
    ~CVisCollection() {};

    // Copy constructor and assignment
    CVisCollection(const CVisCollection<T_Pixel>& c)
		: CVisFrameSequence<T_Pixel>(c) {}
    CVisCollection<TPixel>& operator=(const CVisCollection<T_Pixel>& c)
	{ ((CVisFrameSequence<T_Pixel>&) *this) = c; return *this; }

    // Builds a collection of n_frames images of the same size,
	// not a constructor so can be called on elements of 
	// a collection array.
//	void CreateCollection(int n_frames, int n_rows, int n_cols)
//	{ Assign(n_frames, CVisShape(0, 0, n_cols, n_rows, 0)); }

    // Query and setting functions
    int NFrames() const            { return Length(); }
//    int IsEmpty() const            { return m_n_frames == 0; }
    int CurrentFrame() const       { return ICur(); }
    void SetCurrentFrame(int index) { SetICur(index); }
//    void SetFirst()                 { m_current_frame = 0; }
//    void SetLast()                  { m_current_frame = m_n_frames-1; }
//    void SetNext()                  {
//        if (m_current_frame < m_n_frames-1) m_current_frame++; }
//    void SetPrev()                  {
//        if (m_current_frame > 0) m_current_frame--; }

	// UNDONE:  Need to change code to use pixel values instead of integer
	// values for invisible pixels, since pixel values are stored in the
	// CVisFrame and CVisFrameSequence classes.
    int InvisiblePixel() const;
    void SetInvisiblePixel(int v);
//    int ReadIfEmpty() const         { return m_read_if_empty; }
//    void SetReadIfEmpty(int b)      { m_read_if_empty = b; }

    // Collection operations (see also File I/O below)
// UNDONE:  How often is new_geometry used?
//    virtual void Insert(T_Frame &frame, int index = -1, int new_geometry = 0);
    virtual void Insert(T_Frame &frame, int index = -1)
	{
		if (index < 0)
			PushBack(frame);
		else
			CVisFrameSequence< T_Pixel >::Insert(index, frame);
	}
    virtual void Insert(const char *filename, int index = -1)
	{
		if (index < 0)
			PushBack(filename);
		else
			CVisFrameSequence< T_Pixel >::Insert(index, filename);
	}
    virtual void Raise(int i, bool fToFront = true)
		{ MoveForward(i, fToFront); }
    virtual void Lower(int i, bool fToBack = true)
		{ MoveBackward(i, fToBack); }
    virtual void Delete(int index) { Erase(index); }
    virtual void DeleteAll() { Clear(); }

//	void FrameTransformChainDelete(int frame, int p){ Frame(frame).TransformChainDelete(p);}


	//------------------------------------------------------------------
	// @group CVisSequence methods (overridden to use T_Frame)

	const T_Frame& Front(void) const
		{ return (const T_Frame&) CVisFrameSequence<T_Pixel>::Front(); }
	T_Frame& Front(void)
		{ return (T_Frame&) CVisFrameSequence<T_Pixel>::Front(); }
	const T_Frame& Back(void) const
		{ return (const T_Frame&) CVisFrameSequence<T_Pixel>::Back(); }
	T_Frame& Back(void)
		{ return (T_Frame&) CVisFrameSequence<T_Pixel>::Back(); }
	const T_Frame& At(int i) const
		{ return (const T_Frame&) CVisFrameSequence<T_Pixel>::At(i); }
	T_Frame& At(int i)
		{ return (T_Frame&) CVisFrameSequence<T_Pixel>::At(i); }
	const T_Frame& operator[](int i) const
		{ return (const T_Frame&) CVisFrameSequence<T_Pixel>::At(i); }
	T_Frame& operator[](int i)
		{ return (T_Frame&) CVisFrameSequence<T_Pixel>::At(i); }


	//------------------------------------------------------------------
	// @group C++ type information

	virtual const type_info& ImageTypeInfo(void) const;
	virtual const type_info& ObjectTypeInfo(int iLevel = - 1) const;

	virtual CVisSequenceBase *Clone(bool fCopyThis = true) const;


    //------------------------------------------------------------------
	// SDStream I / O

	// These two methods are overridden in derived classes to give type
	// information (about the derived classes) to the property list code.
	virtual CVisPropTypeInfoBase *PropTypeInfoNew(void) const;
	virtual CVisPropTypeInfoBase *PropTypeInfoLookup(void) const;

	void BuildPropList(CVisPropList& refproplist);


    //------------------------------------------------------------------
	// @group Old self-describing stream (low-level) I/O

	// @cmember
	// Read or write properties to a self-describing stream.
	virtual void SDReadWriteProperties(CVisSDStream& s,
			bool fAddComma = false);


    // Accessing individual frames
	T_Frame& Frame(int index)         { return At(index); }
	const T_Frame& Frame(int index) const   { return At(index); }
    T_Frame& Frame()                  { return At(ICur()); }
    const T_Frame& Frame() const      { return At(ICur()); }

    // File I/O
// UNDONE:  Review these.  See if we can change the Collection implementations
	// to use the data in the Sequence class.
    void ReadCollection(const char *filename,
         int read_files = 0,
         FILE *istream = 0);      // read description and (optionally) files
    void WriteCollection(const char *filename, const char *desc_type = 0,
         int write_files = 0,
         FILE *ostream = 0);      // write description and (optionally) files
    void SetWorkingDir(const char *dirname)
        { strcpy(m_working_dir, dirname); }

// UNDONE:  Review these.  See if we can change the Collection implementations
	// to use the data in the Sequence class.
    void ReadCollection (FILE *stream);         // read  frame descriptions
    void WriteCollection(FILE *stream);         // write frame descriptions
    virtual void ReadMetaData(FILE *stream);    // read ancilliary information
    virtual void WriteMetaData(FILE *stream);   // write ancilliary information
    // void ReadVideo(const char *filename);    // read video from a file
    // void WriteVideo(const char *filename);   // write video to a file
//    void ReadFiles();                           // read in constituent frames
//    void WriteFiles();                          // write out constituent frames
//    void AddFile(const char *filename, int read_file = 0);
    void ReNameSequence(const char *szBase,
			const char *szExtension, int cchDigits = 4)
			{ RenameFiles(szBase, szExtension, cchDigits); }



	// Constant used to specify the depth of this object (for use with
	// ObjectTypeInfo).
	enum { eilevelCollection = eilevelFrameSequence + 1 };
	enum { eiLevel = T_BaseSequence::eiLevel + 1 };

protected:
// UNDONE:  Make data members private?
//    int m_n_frames;                 // number of frames
//    int m_current_frame;            // current frame
//    T_Frame **m_frame;            // array of frame pointers
    char m_descriptor[1024];        // file format descriptor
    char m_working_dir[1024];       // directory containing collection files
//    int m_invisible_pixel;          // value of invisible pixel (-1: none)
//    int m_read_if_empty;            // force read from file if empty

	virtual CVisImageBase *PImageNew(void) const
		{
			T_Frame *pframe = new T_Frame();
			if (HasPixelInvisible())
				pframe->SetPixelInvisible(PixelInvisible());
			return pframe;
		}
	virtual CVisImageBase *PImageNew(const CVisShape& refshape) const
		{
			T_Frame *pframe = new T_Frame(refshape);
			if (HasPixelInvisible())
				pframe->SetPixelInvisible(PixelInvisible());
			return pframe;
		}
	virtual CVisImageBase *PImageNew(const CVisImageBase& refimage) const
		{
			assert(refimage.PixelTypeInfo() == PixelTypeInfo());
			T_Frame *pframe = new T_Frame();
			if (HasPixelInvisible())
				pframe->SetPixelInvisible(PixelInvisible());
			*pframe = (const T_Image&) refimage;
			return pframe;
		}
};



// UNDONE:  These are only temporary, until invisible pixel code is changed.
inline int CVisCollection<BYTE>::InvisiblePixel(void) const
{
	// Default implementation for BYTE or float (or other standard types).
	if (HasPixelInvisible())
		return (int) PixelInvisible();
	return -1;
}

inline int CVisCollection<int>::InvisiblePixel(void) const
{
	// Default implementation for BYTE or float (or other standard types).
	if (HasPixelInvisible())
		return (int) PixelInvisible();
	return -1;
}

inline int CVisCollection<float>::InvisiblePixel(void) const
{
	// Default implementation for BYTE or float (or other standard types).
	if (HasPixelInvisible())
		return (int) PixelInvisible();
	return -1;
}

inline int CVisCollection<double>::InvisiblePixel(void) const
{
	// Default implementation for BYTE or float (or other standard types).
	if (HasPixelInvisible())
		return (int) PixelInvisible();
	return -1;
}

inline int CVisCollection<CVisRGBABytePixel>::InvisiblePixel(void) const
{
	// Specialized RGBA implemenation.
	if (HasPixelInvisible())
	{
		assert(PixelInvisible().R() == PixelInvisible().G());
		assert(PixelInvisible().R() == PixelInvisible().B());

		return (int) PixelInvisible().R();
	}

	return -1;
}

inline int CVisCollection<CVisRGBAIntPixel>::InvisiblePixel(void) const
{
	// Specialized RGBA implemenation.
	if (HasPixelInvisible())
	{
		assert(PixelInvisible().R() == PixelInvisible().G());
		assert(PixelInvisible().R() == PixelInvisible().B());

		return (int) PixelInvisible().R();
	}

	return -1;
}

inline int CVisCollection<CVisRGBAFloatPixel>::InvisiblePixel(void) const
{
	// Specialized RGBA implemenation.
	if (HasPixelInvisible())
	{
		assert(PixelInvisible().R() == PixelInvisible().G());
		assert(PixelInvisible().R() == PixelInvisible().B());

		return (int) PixelInvisible().R();
	}

	return -1;
}

inline int CVisCollection<CVisRGBADoublePixel>::InvisiblePixel(void) const
{
	// Specialized RGBA implemenation.
	if (HasPixelInvisible())
	{
		assert(PixelInvisible().R() == PixelInvisible().G());
		assert(PixelInvisible().R() == PixelInvisible().B());

		return (int) PixelInvisible().R();
	}

	return -1;
}


template<class TPixel>
void CVisCollection<TPixel>::SetInvisiblePixel(int v)
{
	if (v < 0)
	{
		SetHasPixelInvisible(false);
	}
	else
	{
		SetHasPixelInvisible(true);
		SetPixelInvisible(v);
	}
}





#ifndef VIS_ONLY_DECLARE_PROPLIST
// Specialized PropList helper functions for common image types
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCollection<BYTE> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCollection<int> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCollection<float> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCollection<double> const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCollection<CVisRGBA<BYTE> > const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCollection<CVisRGBA<int> > const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCollection<CVisRGBA<float> > const &refobj, bool fAlwaysTypedef);
CVisPropTypeInfoBase *VisPPropTypeInfoNewWithIOPropList(
		CVisCollection<CVisRGBA<double> > const &refobj, bool fAlwaysTypedef);
#endif // !VIS_ONLY_DECLARE_PROPLIST

template <class TPixel>
inline CVisPropTypeInfoBase *CVisCollection<TPixel>::PropTypeInfoNew(void) const
{
#ifdef VIS_ONLY_DECLARE_PROPLIST
	assert(0);
	return 0;
#else // VIS_ONLY_DECLARE_PROPLIST
	return VisPPropTypeInfoNewWithIOPropList(*this, false);
#endif // !VIS_ONLY_DECLARE_PROPLIST
}

template <class TPixel>
inline CVisPropTypeInfoBase *CVisCollection<TPixel>::PropTypeInfoLookup(void) const
{
#ifdef VIS_ONLY_DECLARE_PROPLIST
	assert(0);
	return 0;
#else // VIS_ONLY_DECLARE_PROPLIST
	return VisLookupPropTypeInfo(typeid(CVisCollection<TPixel>));
#endif // !VIS_ONLY_DECLARE_PROPLIST
}


template <class TPixel>
inline void CVisCollection<TPixel>::BuildPropList(CVisPropList& refproplist)
{
	T_BaseSequence::BuildPropList(refproplist);
	// UNDONE:  Do we need to write m_descriptor?
}



#ifdef TAKEOUT // In the VisMatrix project.
// Utility functions for saving alpha = 0 as special colors
void VisMakePixelsInvisible(CVisRGBAByteImage &img, int v);
void VisBackInvisiblePixels(CVisRGBAByteImage &img, int v);

// Sample class definitions

typedef CVisFrame<CVisByteImage>                CVisByteFrame;
typedef CVisFrame<CVisIntImage>                 CVisIntFrame;
typedef CVisFrame<CVisFloatImage>               CVisFloatFrame;
typedef CVisFrame<CVisDoubleImage>              CVisDoubleFrame;
typedef CVisFrame<CVisRGBAByteImage>           CVisRGBAByteFrame;
typedef CVisFrame<CVisRGBAIntImage>            CVisRGBAIntFrame;
typedef CVisFrame<CVisRGBAFloatImage>          CVisRGBAFloatFrame;
typedef CVisFrame<CVisRGBADoubleImage>         CVisRGBADoubleFrame;
#endif // TAKEOUT

typedef CVisCollection<CVisBytePixel>         CVisByteCollection;
typedef CVisCollection<CVisIntPixel>          CVisIntCollection;
typedef CVisCollection<CVisFloatPixel>        CVisFloatCollection;
typedef CVisCollection<CVisDoublePixel>       CVisDoubleCollection;

typedef CVisCollection<CVisRGBABytePixel>    CVisRGBAByteCollection;
typedef CVisCollection<CVisRGBAIntPixel>     CVisRGBAIntCollection;
typedef CVisCollection<CVisRGBAFloatPixel>   CVisRGBAFloatCollection;
typedef CVisCollection<CVisRGBADoublePixel>  CVisRGBADoubleCollection;

#endif // __VIS_COLLECTION_H__