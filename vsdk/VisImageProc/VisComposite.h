///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisComposite.h -- image composites (multi-layer displays)
//
// DESCRIPTION
//  The CVisComposite class is used to assemble the collection
//  of images in a CVisCollection into a single image for display.
//
// *** OLD *** 
//
//  There are several modes for display VisComposites:
//  1. movie sequence: just display a single frame
//  2. film strip: display frames in a horizontal format
//  3. table: use a two-dimensional, fixed width table
//  4. true layered composite:
//  ?. offset layers: display frames stacked and skewed - NOT YET IMPLEMENTED
//
//  There is an interesting interaction between the current frame idea
//  in the CVisCollection, and the current frame in CVisComposite.
//  For a composite, the selected frame in a collection must be common
//  to all views (composites).  For a movie, we want each composite
//  (view) to have an independent selection.  Each class thus
//  has its own concept of a current frame (see notes of June 6, 1996).
//
// ** END OLD ***
//
//  Note:  the above statements regarding display are no longer true...
//  Matthew will incorporate the required functionality into the CPane
//  window class.
//
//  Instead, the CVisComposite class simply adds interactive
//  and/or automatic image alignment functionality to CVisCollection.
//
// PUBLIC MEMBER FUNTIONS (METHODS)
//  CVisComposite(TVisCompositeType composite_type = VIS_COMPOSITE_MOVIE);
//  ~CVisComposite();
//
//  ...
//
// PARAMETERS
//  composite_type  type of composite (movie, strip, layered, ...)
//  index           index of current frame
//  frame           a single frame (image) and ancilliary information
//  filename        composite or image filename
//  read_files      read files right now (or defer till accessed with [])
//  write_files     write files (not just the composite description)
//
// SEE ALSO
//  VisComposite.cpp      composite construction code
//  VisCollection.h       image collection class
//  VisBlend.h            image blending / compositing operations
//  VisAlign.h            interactive and automatic alignment algorithms
//  VisCylindrical.h      cylindrically warped composites
//
//  Rick Szeliski's research notes, 06-Jun-96.
//
// BUGS
//  The current version uses the frame's geometry to store the mapping
//  from the frame's image to "world" (or base/view) coordinates.
//  This is the inverse of the usual vision/graphics usage of a camera
//  matrix mapping world to screen coordinates (see also VisSfm.lib).
//  The next version should reverse this...
//
// DESIGN
//  The intent in creating separate classes for CVisCollection and
//  CVisComposite is to support a multi-view document-view user
//  interface.  A CVisCollection can be used to build a document class,
//  while a separate CVisComposite can be built into each view.
//  Thus, multiple views (different zooms / directions, different current
//  frames) can be active on the same documet.
//
// HISTORY
//  06-Jun-96  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
//  14-Jun-96  Richard Szeliski (szeliski) at Microsoft
//  Redesigned functionality.
//
//  10-Dec-96  Richard Szeliski (szeliski) at Microsoft
//  Moved CVisWarpedComposite to VisCylindrical.h.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#ifndef _VIS_COMPOSITE_H_
#define _VIS_COMPOSITE_H_


// A CVisCompositeFrame is an image with geometry information and a
// (float) feathered image.
template<class TPixel>
class CVisCompositeFrame : public CVisFrame<TPixel>
{
public:
	typedef TPixel T_Pixel;
	typedef CVisImage<TPixel> T_Image;
	typedef CVisFrame<TPixel> T_BaseImage;
	typedef CVisFrame<TPixel> T_FrameBase;
	typedef CVisCollection<TPixel>	T_BaseSequence;


	//------------------------------------------------------------------
	// @group Constructors, assignement operators, and destructors

    CVisCompositeFrame(void);
    CVisCompositeFrame(const RECT& refrect);
    CVisCompositeFrame(const CVisShape& refshape);
    CVisCompositeFrame(const char *szFilename);

	// Construct an image from another image.
	// Warning:  Derived classes should provide their own copy constructor
	// that first constructs a default object and then uses the assignment
	// operator to synchronize  image creation, assignment, and
	// destruction.  Derived classes should not call the copy constructor
	// in the base class.
    CVisCompositeFrame(const CVisImage<TPixel>& refimage);

    CVisCompositeFrame<TPixel>& operator=(const CVisImage<TPixel>& refimage);

    virtual ~CVisCompositeFrame(void);


	//------------------------------------------------------------------
	// @group Attributes

	// Feathering weight image
    // LATER:  We might want a similar const method.
    CVisFloatImage& ImageFeather(void);

	// @cmember
	// Use m_rectCrop to crop the current image, if needed.
	CVisCompositeFrame<TPixel> Cropped(void);
	CVisCompositeFrame<TPixel> Cropped(RECT& rect);


    //------------------------------------------------------------------
	// SDStream I / O

	// These two methods are overridden in derived classes to give type
	// information (about the derived classes) to the property list code.
	virtual CVisPropTypeInfoBase *PropTypeInfoNew(void) const;
	virtual CVisPropTypeInfoBase *PropTypeInfoLookup(void) const;

	void BuildPropList(CVisPropList& refproplist)
		{ T_BaseImage::BuildPropList(refproplist); }  // UNDONE


    //------------------------------------------------------------------
	// @group Old self-describing stream I/O

	// @cmember
	// Read or write properties to a self-describing stream.
	virtual void SDReadWriteProperties(CVisSDStream& s,
			bool fAddComma = false);


	//------------------------------------------------------------------
	// @group Old file I/O methods
// Use base class methods for now for compatibility with old file formats.
// (Composite code will still write feathered images in their own data block.)

	// These may be removed in a future release of the SDK.
//	void Read (FILE *stream);
//  void Write(FILE *stream, int indent = 0);

	// These are only temporary to get the ImageProc code converted faster.
//	void ReadDescription(FILE *stream, const char *desc_type,
//			 char *working_dir);
//	void WriteDescription(FILE *stream, const char *desc_type,
//             char *working_dir);


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



	// Constant used to specify the depth of this object (for use with
	// ObjectTypeInfo).
	enum { eilevelCompositeFrame = eilevelFrame + 1 };
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


private:
    CVisFloatImage m_imageFeather;   // feathering weight image
};


enum TVisCompositeType { // antiquated: this functionality should be in CPane
    VIS_COMPOSITE_MOVIE,      // just display current frame
    VIS_COMPOSITE_STRIP,      // horizontal strip
    VIS_COMPOSITE_TABLE,      // two-dimensional table
    VIS_COMPOSITE_LAYERED,    // true layered composition
    VIS_COMPOSITE_INVALID     // dummy end of enumeration list
};

enum TVisCompositeView {   // composition view type
    VIS_COMPOSITE_VIEW_SEQUENCE,     // just show selected image, no geometric deformation
    VIS_COMPOSITE_VIEW_OVERLAY,      // show top layer over others composited
    VIS_COMPOSITE_VIEW_BLINK,        // blink top layer over others composited
    VIS_COMPOSITE_VIEW_TRANSPARENT,  // show top layer transparently
    VIS_COMPOSITE_VIEW_INVISIBLE,    // don't show top layer (only others)
    VIS_COMPOSITE_VIEW_COMPLETE,     // show complete composition
    VIS_COMPOSITE_VIEW_SEGMENT,      // show masked image (VisLayeredComposite)
    VIS_COMPOSITE_VIEW_LABEL        // show label  image (VisLayeredComposite)
};

enum TVisCompositeValid {
    VIS_VALID_TOP = 1,          // top layer is valid
    VIS_VALID_BOTTOM = 2,       // bottom layer is valid
    VIS_VALID_COMPOSITE = 4,    // composite layer is valid
    VIS_VALID_ACCUMULATOR = 8,  // accumulator is (partially) valid
    VIS_VALID_TOP_BKGD = 16,    // top layer with backing color is valid
    VIS_VALID_DIFFERENCE = 32   // difference between top layer and composite is valid
};

template<class TPixel>
class CVisComposite : public CVisCollection<TPixel>, 
                      public CVisDiagnostics {
public:
//	typedef TFrame::T_Image			T_Image;
	typedef CVisCompositeFrame<TPixel> T_Frame;
	typedef CVisCollection<TPixel>	T_BaseSequence;

    // Constructor and destructor
    CVisComposite(EVisSequence evissequence = evissequenceDelayReadUntilNeeded);
    ~CVisComposite();

    // Query and setting functions
    EVisComposite CompositingOperator()  { return m_operator; }
    void SetCompositingOperator(EVisComposite o) { m_operator = o; }
    int FeatherDistance()               { return m_feather_distance; }
    void SetFeatherDistance(int d)      { m_feather_distance = d; }
    TVisCompositeView DisplayMode()     { return m_overlay; }
    void SetDisplayMode(TVisCompositeView v);
    float BoundingBoxSize()             { return m_bounding_box; }
    void SetBoundingBoxSize(float b)    { m_bounding_box = b; }
    CVisShape CroppedSize()             { return m_crop_region; }
    void SetCroppedSize(CVisShape sh)   { m_crop_region = sh; }
    TVisInterpolation Interpolation()   { return m_interpolation; }
    void SetInterpolation(TVisInterpolation i)  { m_interpolation = i; }
    float MaxPixelStretch()             { return m_max_pixel_stretch; }
    void SetMaxPixelStretch(float v)    { m_max_pixel_stretch = v; }
    int Blink()                         { return m_blink; }
    void SetBlink(int blink)            { m_blink = blink; }
    int NeighborFrames()                { return m_neighbor_frames; }
    void SetNeighborFrames(int n)       { m_neighbor_frames = n; }
    int SkipFrames()                    { return m_skip_frames; }
    void SetSkipFrames(int n)           { m_skip_frames = n; }
    int BackgroundGrayLevel()           { return m_background_gray_level; }
    void SetBackgroundGrayLevel(int l)  { m_background_gray_level = l; }

    // Reset current frame geometry to base geometry
    void ResetCurrentFrameGeometry();
    void PredictLastFrameGeometry(int n_last);

    // Current view manipulation
    void SetZoom(double zoom)       { m_zoom = zoom; Invalidate(); }
    double Zoom()                   { return m_zoom; }
    void SetViewGeometry(const CVisTransformChain& geometry); 
    CVisTransformChain& ViewGeometry()  { return m_view_geometry; }

    // Layer invalidation (when geometry has changed ...)
    void Invalidate(int mask = 0)   { m_valid_layers &= (mask | m_always_valid); m_generation++; }
    int& AlwaysValid()              { return m_always_valid; }
    int Generation()                { return m_generation; }

    // Convert view_geometry and all frame geometry to 6-transform chain
    void MakeViewGeometryChain(double f);
    // and back to 1-transform 
    void CollapseGeometryChain();

    // Layer geometry and warped shape
    virtual CVisTransform4x4 LayerTransform(int i);        // transform including base
    virtual void SetLayerTransform(int i, CVisTransform4x4 t);
    CVisTransformChain LayerGeometry(int i, double f);             // transform chain including base
    void SetLayerGeometry(int i, CVisTransformChain& g, double f);
    CVisShape LayerShape(int i, int ignore_strecth = 0);    // shape (size) of layer
    void LayerCenter(int i, int center[2],
                     double zoom = 1.0); // center of warped layer
    void LayerOutline(int i, int line[4][2][2],
                      double zoom = 1.0); // clipped corners of warped layer

    // Control of which frames contribute to composite (bottom layer)
    int IsActive(int index);

    // Return a view on the composition
    T_Image& TopLayer();         // top layer only
    CVisShape TopLayerShape();     // shape (size) of whole top layer
    T_Image& TopLayerAlpha();    // top layer with no background (alpha matted)
    void FormTopLayer(CVisShape shape);  // recompute top layer
    void FormTopWBkgd(CVisShape shape);  // recompute top layer with background
    T_Image& BottomAlpha();      // bottom layers with alpha channel
    T_Image& BottomLayer();      // bottom layers only
    CVisShape BottomLayerShape();  // shape (size) of composed bottom layers
    void FormBottomLayer(CVisShape shape,// assemble bottom layers
                         int omit_current = 1);
    T_Image& Composite();        // complete composition
    CVisShape CompositeShape();    // shape (size) of complete composition
    void FormComposite(CVisShape shape); // recompute composition
    T_Image& CroppedBottomLayer(); // cropped final composition
    T_Image& CurrentView();      // view set in DisplayMode()

    // Closing the loop on panoramas:
    //  find the next frame with the same filename and the relative transform
    int FindNextSame(int index, CVisTransform4x4& translation);
    int FindPreviousSame(int index); 

#ifdef TAKEOUT
    // Override collection operations (see VisCollection.h for definitions)
    virtual void Insert(TFrame &frame, int index = -1, int new_geometry = 0);
    virtual void Insert(const char *filename, int index = -1);
    virtual void Raise(int index, int to_top = 1);
    virtual void Lower(int index, int to_bottom = 1);
    virtual void Delete(int index);
    virtual void DeleteAll();
#endif // TAKEOUT

    // File I/O
    virtual void ReadMetaData(FILE *stream);
    virtual void WriteMetaData(FILE *stream);

    // Hook for additional image warping
    virtual T_Image& Warped(int index);  // potentially warped image
    virtual T_Image& Warped()     { return Warped(CurrentFrame()); }


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


	// Constant used to specify the depth of this object (for use with
	// ObjectTypeInfo).
	enum { eilevelComposite = eilevelCollection + 1 };
	enum { eiLevel = T_BaseSequence::eiLevel + 1 };

public:
    // changed to public for spherical map warping and blending 
    // harry shum 7-25-97
//    CVisFloatCollection m_feather;   // feathering weight images

protected:
// UNDONE:  Make data members private?
    EVisComposite m_operator;    // see VisBlend.h
    int m_feather_distance;         // maximum distance on feathering
    TVisCompositeView m_overlay;    // current display mode
    double m_zoom;                  // zoom factor
    CVisTransformChain m_view_geometry;   // viewing parameters
    float m_bounding_box;           // bottom is this times as large as top (0 = nop)
    TVisInterpolation m_interpolation; // interpolation on zooming
    float m_max_pixel_stretch;      // maximum distortion in composite

    T_Image m_top_layer;         // top layer (above active sprite)
    T_Image m_top_w_bkgd;        // top layer with backing color
    T_Image m_bottom_alpha;      // bottom layer, with alpha channel
    T_Image m_bottom_layer;      // bottom layer (below active sprite)
    T_Image m_composite;         // complete composition
    T_Image m_bottom_cropped;    // cropped bottom layer
    CVisShape m_crop_region;        // cropping region for final output
    CVisRGBAFloatImage m_accumulator;   // accumulator for weighted blends (should really be templated somehow...)
    CVisRGBAByteImage m_layers;    // array of layer images (for median comp.)
    // CVisByteCollection m_feather;   // feathering weight images
    int m_n_accumulated;            // number of frames currently accumulated
    int m_i_not_accumulated;        // id of frame not accumulated
    int m_valid_layers;             // layers are valid
    int m_always_valid;             // make these layers always valid
    int m_blink;                    // blinks after each CurrentView() call
    int m_background_gray_level;    // gray level for background
    int m_neighbor_frames;          // only use nearby frames in composite
    int m_generation;               // generation count
    int m_skip_frames;              // skip every nth frame
    int m_save_layers;              // save warped layers in m_layers

    // for pyramid blending
    int m_blend_level; 
    CVisPyramid<CVisRGBAByteImage>* m_pyramid_accumulator; // corresponding to m_accumulator
    CVisPyramid<CVisRGBAByteImage>* m_pyramid_layer; // to ReConstruct m_bottom_alpha

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

// De-interlacing routine
template <class T>
void DeInterlace(T &image, int odd_field);



typedef CVisCompositeFrame<signed char>					CVisCharCompositeFrame;
typedef CVisCompositeFrame<short>						CVisShortCompositeFrame;
typedef CVisCompositeFrame<int>							CVisIntCompositeFrame;
typedef CVisCompositeFrame<long>						CVisLongCompositeFrame;

typedef CVisCompositeFrame<unsigned char>				CVisByteCompositeFrame;
typedef CVisCompositeFrame<unsigned char>				CVisUCharCompositeFrame;
typedef CVisCompositeFrame<unsigned short>				CVisUShortCompositeFrame;
typedef CVisCompositeFrame<unsigned int>				CVisUIntCompositeFrame;
typedef CVisCompositeFrame<unsigned long>				CVisULongCompositeFrame;

typedef CVisCompositeFrame<float>						CVisFloatCompositeFrame;
typedef CVisCompositeFrame<double>						CVisDoubleCompositeFrame;


typedef CVisCompositeFrame<CVisRGBACharPixel>			CVisRGBACharCompositeFrame;
typedef CVisCompositeFrame<CVisRGBAShortPixel>			CVisRGBAShortCompositeFrame;
typedef CVisCompositeFrame<CVisRGBAIntPixel>			CVisRGBAIntCompositeFrame;
typedef CVisCompositeFrame<CVisRGBALongPixel>			CVisRGBALongCompositeFrame;

typedef CVisCompositeFrame<CVisRGBABytePixel>			CVisRGBAByteCompositeFrame;
typedef CVisCompositeFrame<CVisRGBABytePixel>			CVisRGBAUCharCompositeFrame;
typedef CVisCompositeFrame<CVisRGBAUShortPixel>			CVisRGBAUShortCompositeFrame;
typedef CVisCompositeFrame<CVisRGBAUIntPixel>			CVisRGBAUIntCompositeFrame;
typedef CVisCompositeFrame<CVisRGBAULongPixel>			CVisRGBAULongCompositeFrame;

typedef CVisCompositeFrame<CVisRGBAFloatPixel>			CVisRGBAFloatCompositeFrame;
typedef CVisCompositeFrame<CVisRGBADoublePixel>			CVisRGBADoubleCompositeFrame;



#include "VisComposite.inl"


#endif // _VIS_COMPOSITE_H_
