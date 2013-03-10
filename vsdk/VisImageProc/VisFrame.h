// VisFrame.h
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved


#pragma once


// A CVisFrame is an image with geometry information.
template<class TPixel>
class CVisFrame : public CVisImage<TPixel>
{
public:
	typedef TPixel T_Pixel;
	typedef CVisImage<TPixel> T_Image;
	typedef CVisImage<TPixel> T_BaseImage;
	typedef CVisSequence<TPixel> T_BaseSequence;


	//------------------------------------------------------------------
	// @group Constructors, assignement operators, and destructors

    CVisFrame(void);
    CVisFrame(const RECT& refrect);
    CVisFrame(const CVisShape& refshape);
    CVisFrame(const char *szFilename);

	// Construct an image from another image.
	// Warning:  Derived classes should provide their own copy constructor
	// that first constructs a default object and then uses the assignment
	// operator to synchronize  image creation, assignment, and
	// destruction.  Derived classes should not call the copy constructor
	// in the base class.
    CVisFrame(const CVisImage<TPixel>& refimage);

    CVisFrame<TPixel>& operator=(const CVisImage<TPixel>& refimage);

    virtual ~CVisFrame(void);


	//------------------------------------------------------------------
	// @group Attributes

	const CVisTransformChain& TransformChain(void) const;
	CVisTransformChain& TransformChain(void);
	void SetTransformChain(const CVisTransformChain& reftransformchain);


	const CVisRect& RectCrop(void) const;
	void SetRectCrop(const RECT& refrectCrop);

	const TPixel& PixelInvisible(void) const;
	void SetPixelInvisible(const TPixel& refpixelInvisible);

	bool HasPixelInvisible(void) const;
	void SetHasPixelInvisible(bool fHasPixelInvisible);

	// @cmember
	// Use m_rectaCrop to crop the current image, if needed.
	CVisFrame<TPixel> Cropped(void);
	CVisFrame<TPixel> Cropped(RECT& rect); // use rect to crop the current image.

    //------------------------------------------------------------------
	// @group Old Self-describing stream I/O

	// @cmember
	// Read or write properties to a self-describing stream.
	virtual void SDReadWriteProperties(CVisSDStream& s,
			bool fAddComma = false);


	//------------------------------------------------------------------
	// @group file I/O methods

	// These may be removed in a future release of the SDK.
	void Read (FILE *stream);
    void Write(FILE *stream, int indent = 0);

	// These are only temporary to get the ImageProc code converted faster.
	void ReadDescription(FILE *stream, const char *desc_type,
			 char *working_dir);
	void WriteDescription(FILE *stream, const char *desc_type,
             char *working_dir);

    //------------------------------------------------------------------
	// SDStream I / O

	// These two methods are overridden in derived classes to give type
	// information (about the derived classes) to the property list code.
	virtual CVisPropTypeInfoBase *PropTypeInfoNew(void) const;
	virtual CVisPropTypeInfoBase *PropTypeInfoLookup(void) const;

	void BuildPropList(CVisPropList& refproplist);


	//------------------------------------------------------------------
	// @group Virtual CVisImage methods

	// @cmember
	// Information about the type of this object.
	virtual const type_info& ObjectTypeInfo(int iLevel = - 1) const;

	// @cmember
	// Change this image to use the information from another image.
	virtual void Alias(const CVisImageBase& refimage);

	// @cmember
	// Change this image to use a copy of information from another image.
	virtual void Copy(const CVisImageBase& refimage);

	// @cmember
	// Make a copy of this image.  Most users of the library should not
	// use this method.
	virtual CVisImageBase *Clone(const CVisShape *pshape = 0) const;


    //------------------------------------------------------------------
	// @group Comparison operators

	// Note that these operators may need to look at all image pixels
	// to compare the images.  For that reason, we do not want to use
	// CVisImage-derived objects in STL containers.  Since we do not
	// want to use this class in STL containers, we do not define
	// operator< here.
	bool operator==(const CVisFrame<TPixel>& refframe) const;
	bool operator!=(const CVisFrame<TPixel>& refframe) const;


	// Constant used to specify the depth of this object (for use with
	// ObjectTypeInfo).
	enum { eilevelFrame = eilevelImage + 1 };
	enum { eiLevel = T_BaseImage::eiLevel + 1 };


protected:
	//------------------------------------------------------------------
	// @group Virtual CVisImage methods

	// @cmember
	// Copy an image.  This is called by the assignment operator.
	// Derived classes should implement this
	// method to make sure that images get copied correctly when a
	// pointer to the base class is used.
	virtual void Assign(const CVisImageBase& refimage);

	// @cmember
	// Read an image from a file or write the image to a file.
	virtual bool ReadWriteFile(SVisFileDescriptor& reffiledescriptor,
			bool fWrite, bool fThrowError, bool fDisplayErrorMessage);


	//------------------------------------------------------------------
	// @group Helper methods

	// @cmember
	// Copy CVisFrame-specific information from an image.  This will
	// check to see if the image is actually a CVisFrame and only
	// attempt to copy Frame information if the image is a CVisFrame.
	void CopyFrameInfo(const CVisImageBase& refimage);


private:
	CVisTransformChain m_transformchain;
	CVisRect m_rectCrop;
	TPixel m_pixelInvisible;
	bool m_fHasPixelInvisible;
};


template<class TPixel>
class CVisFrameSequence : public CVisSequence<TPixel>
{
public:
	//------------------------------------------------------------------
	// @group Type definitions

	typedef TPixel					T_Pixel;
	typedef CVisImage<TPixel>		T_Image;
	typedef CVisFrame<TPixel>		T_Frame;
	typedef CVisSequence<TPixel>	T_BaseSequence;


	//------------------------------------------------------------------
	// @group Constructors, assignement operator, and destructor

	// Note:  Use nLengthMax == - 1 to place no limits on the sequence length.
	CVisFrameSequence(int nLength = - 1,
			int evissequence = evissequenceDefault);
	CVisFrameSequence(int nLength, const T_Image& refimage,
			int evissequence = evissequenceDefault);
	CVisFrameSequence(int nLength, const CVisShape& refshape,
			int evissequence = evissequenceDefault);
	CVisFrameSequence(const CVisSequence<TPixel>& refsequence,
			int iFirst = 0, int iLim = - 1);
	CVisFrameSequence(const CVisSequence<TPixel>& refsequence,
			int iFirst, int iLim, int evissequence);
	CVisFrameSequence<TPixel>& operator=(
			const CVisSequence<TPixel>& refsequence);
	virtual ~CVisFrameSequence(void);


	//------------------------------------------------------------------
	// @group Attributes

	const TPixel& PixelInvisible(void) const;
	void SetPixelInvisible(const TPixel& pixelInvisible);

	bool HasPixelInvisible(void) const;
	void SetHasPixelInvisible(bool fHasPixelInvisible);


	//------------------------------------------------------------------
	// @group CVisSequence methods (overridden to use CVisFrame)

	const T_Frame& Front(void) const;
	T_Frame& Front(void);
	const T_Frame& Back(void) const;
	T_Frame& Back(void);
	const T_Frame& At(int i) const;
	T_Frame& At(int i);
	const T_Frame& operator[](int i) const;
	T_Frame& operator[](int i);


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

	virtual void PostReadValue(CVisSDIStream& refsdistream);
	virtual void PreWriteValue(CVisSDOStream& refsdostream) const;


    //------------------------------------------------------------------
	// @group Old self-describing stream (low-level) I/O

	// @cmember
	// Read or write properties to a self-describing stream.
	virtual void SDReadWriteProperties(CVisSDStream& s,
			bool fAddComma = false);


	// Constant used to specify the depth of this object (for use with
	// ObjectTypeInfo).
	enum { eilevelFrameSequence = eilevelSequence + 1 };
	enum { eiLevel = T_BaseSequence::eiLevel + 1 };


protected:
	virtual CVisImageBase *PImageNew(void) const;
	virtual CVisImageBase *PImageNew(const CVisShape& refshape) const;
	virtual CVisImageBase *PImageNew(const CVisImageBase& refimage) const;

	void CopyFrameSequenceInfo(const CVisSequence<TPixel>& refsequence);

private:
	TPixel m_pixelInvisible;
	bool m_fHasPixelInvisible;
};


// Utility functions for saving alpha = 0 as special colors
void VisMakePixelsInvisible(CVisRGBAByteImage &img, int v);
void VisBackInvisiblePixels(CVisRGBAByteImage &img, int v);


// Common CVisFrame class definitions


typedef CVisFrame<signed char>					CVisCharFrame;
typedef CVisFrame<short>						CVisShortFrame;
typedef CVisFrame<int>							CVisIntFrame;
typedef CVisFrame<long>							CVisLongFrame;

typedef CVisFrame<unsigned char>				CVisByteFrame;
typedef CVisFrame<unsigned char>				CVisUCharFrame;
typedef CVisFrame<unsigned short>				CVisUShortFrame;
typedef CVisFrame<unsigned int>					CVisUIntFrame;
typedef CVisFrame<unsigned long>				CVisULongFrame;

typedef CVisFrame<float>						CVisFloatFrame;
typedef CVisFrame<double>						CVisDoubleFrame;


typedef CVisFrame<CVisRGBACharPixel>			CVisRGBACharFrame;
typedef CVisFrame<CVisRGBAShortPixel>			CVisRGBAShortFrame;
typedef CVisFrame<CVisRGBAIntPixel>				CVisRGBAIntFrame;
typedef CVisFrame<CVisRGBALongPixel>			CVisRGBALongFrame;

typedef CVisFrame<CVisRGBABytePixel>			CVisRGBAByteFrame;
typedef CVisFrame<CVisRGBABytePixel>			CVisRGBAUCharFrame;
typedef CVisFrame<CVisRGBAUShortPixel>			CVisRGBAUShortFrame;
typedef CVisFrame<CVisRGBAUIntPixel>			CVisRGBAUIntFrame;
typedef CVisFrame<CVisRGBAULongPixel>			CVisRGBAULongFrame;

typedef CVisFrame<CVisRGBAFloatPixel>			CVisRGBAFloatFrame;
typedef CVisFrame<CVisRGBADoublePixel>			CVisRGBADoubleFrame;


typedef CVisFrame<CVisYUVACharPixel>			CVisYUVACharFrame;
typedef CVisFrame<CVisYUVAShortPixel>			CVisYUVAShortFrame;
typedef CVisFrame<CVisYUVAIntPixel>				CVisYUVAIntFrame;
typedef CVisFrame<CVisYUVALongPixel>			CVisYUVALongFrame;

typedef CVisFrame<CVisYUVABytePixel>			CVisYUVAByteFrame;
typedef CVisFrame<CVisYUVABytePixel>			CVisYUVAUCharFrame;
typedef CVisFrame<CVisYUVAUShortPixel>			CVisYUVAUShortFrame;
typedef CVisFrame<CVisYUVAUIntPixel>			CVisYUVAUIntFrame;
typedef CVisFrame<CVisYUVAULongPixel>			CVisYUVAULongFrame;

typedef CVisFrame<CVisYUVAFloatPixel>			CVisYUVAFloatFrame;
typedef CVisFrame<CVisYUVADoublePixel>			CVisYUVADoubleFrame;



typedef CVisFrameSequence<signed char>			CVisCharFrameSequence;
typedef CVisFrameSequence<short>				CVisShortFrameSequence;
typedef CVisFrameSequence<int>					CVisIntFrameSequence;
typedef CVisFrameSequence<long>					CVisLongFrameSequence;

typedef CVisFrameSequence<unsigned char>		CVisByteFrameSequence;
typedef CVisFrameSequence<unsigned char>		CVisUCharFrameSequence;
typedef CVisFrameSequence<unsigned short>		CVisUShortFrameSequence;
typedef CVisFrameSequence<unsigned int>			CVisUIntFrameSequence;
typedef CVisFrameSequence<unsigned long>		CVisULongFrameSequence;

typedef CVisFrameSequence<float>				CVisFloatFrameSequence;
typedef CVisFrameSequence<double>				CVisDoubleFrameSequence;


typedef CVisFrameSequence<CVisRGBACharPixel>	CVisRGBACharFrameSequence;
typedef CVisFrameSequence<CVisRGBAShortPixel>	CVisRGBAShortFrameSequence;
typedef CVisFrameSequence<CVisRGBAIntPixel>		CVisRGBAIntFrameSequence;
typedef CVisFrameSequence<CVisRGBALongPixel>	CVisRGBALongFrameSequence;

typedef CVisFrameSequence<CVisRGBABytePixel>	CVisRGBAByteFrameSequence;
typedef CVisFrameSequence<CVisRGBABytePixel>	CVisRGBAUCharFrameSequence;
typedef CVisFrameSequence<CVisRGBAUShortPixel>	CVisRGBAUShortFrameSequence;
typedef CVisFrameSequence<CVisRGBAUIntPixel>	CVisRGBAUIntFrameSequence;
typedef CVisFrameSequence<CVisRGBAULongPixel>	CVisRGBAULongFrameSequence;

typedef CVisFrameSequence<CVisRGBAFloatPixel>	CVisRGBAFloatFrameSequence;
typedef CVisFrameSequence<CVisRGBADoublePixel>	CVisRGBADoubleFrameSequence;


typedef CVisFrameSequence<CVisYUVACharPixel>	CVisYUVACharFrameSequence;
typedef CVisFrameSequence<CVisYUVAShortPixel>	CVisYUVAShortFrameSequence;
typedef CVisFrameSequence<CVisYUVAIntPixel>		CVisYUVAIntFrameSequence;
typedef CVisFrameSequence<CVisYUVALongPixel>	CVisYUVALongFrameSequence;

typedef CVisFrameSequence<CVisYUVABytePixel>	CVisYUVAByteFrameSequence;
typedef CVisFrameSequence<CVisYUVABytePixel>	CVisYUVAUCharFrameSequence;
typedef CVisFrameSequence<CVisYUVAUShortPixel>	CVisYUVAUShortFrameSequence;
typedef CVisFrameSequence<CVisYUVAUIntPixel>	CVisYUVAUIntFrameSequence;
typedef CVisFrameSequence<CVisYUVAULongPixel>	CVisYUVAULongFrameSequence;

typedef CVisFrameSequence<CVisYUVAFloatPixel>	CVisYUVAFloatFrameSequence;
typedef CVisFrameSequence<CVisYUVADoublePixel>	CVisYUVADoubleFrameSequence;




#include "VisFrame.inl"
