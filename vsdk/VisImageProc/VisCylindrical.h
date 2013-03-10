///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisCylindrical.h -- cylindrical and spherical coordinates
//
// SPECIFICATION
//  
//
// PARAMETERS
//  
//
// DESCRIPTION
//  These routines are used for mapping to/from cylindrical or
//  spherical coordinates.  Currently, the only user is the
//  VideoMosaic application and its derivatives (TextureMapper).
//
//  These routines can also be used to perform "local warping",
//  i.e., fine local motions to remove misregistration "ghosts".
//
// SEE ALSO
//  VisImage.h        definition of image class library
//  VisWarp.h         definition of warping function
//  VisBlockAdjust.h    computation of local warp field (block correction)
//
// BUGS
//  
//
// DESIGN
//  
//
// HISTORY
//  10-Dec-96  Richard Szeliski (szeliski) at Microsoft
//  Split off from VisWarp.h
//
//  26-Mar-97  Richard Szeliski (szeliski) at Microsoft
//  Added local warping fields.
//
// Copyright © Microsoft Corp. 1996-2000
//
///////////////////////////////////////////////////////////////////////////

#ifndef _VIS_CYLINDRICAL_H_
#define _VIS_CYLINDRICAL_H_

// color adjust parameters 
typedef struct _SColorAdjustParam
{
	double     m_fAlpha;
	double     m_fBeta;
	CVisShape  m_Overlap;

    _SColorAdjustParam() 
	{ 
		m_fAlpha = m_fBeta = 0.0;
	    m_Overlap.left = m_Overlap.right = 
	    m_Overlap.top = m_Overlap.bottom = 0; 
	};
	void BuildPropList(CVisPropList& refproplist)
	{
		refproplist.AddPropReference("m_fAlpha", m_fAlpha, false);
		refproplist.AddPropReference("m_fBeta", m_fBeta, false);
		refproplist.AddPropReference("m_Overlap", m_Overlap, false);
	}
} SColorAdjustParam;

enum TVisColorDir {
    VIS_COLOR_LEFT,    // left adjust parameters
	VIS_COLOR_RIGHT,   // right adjust parameters
	VIS_COLOR_LAST
};

// A CVisWarpedFrame is an image with geometry information, a (float)
// feathered image, a (float) local warp image, and a warped image.
template<class TPixel>
class CVisWarpedFrame : public CVisCompositeFrame<TPixel>
{
public:
	typedef TPixel T_Pixel;
	typedef CVisImage<TPixel> T_Image;
	typedef CVisCompositeFrame<TPixel> T_BaseImage;
	typedef CVisCompositeFrame<TPixel> T_FrameBase;


	//------------------------------------------------------------------
	// @group Constructors, assignement operators, and destructors

    CVisWarpedFrame(void);
    CVisWarpedFrame(const RECT& refrect);
    CVisWarpedFrame(const CVisShape& refshape);
    CVisWarpedFrame(const char *szFilename);

	// Construct an image from another image.
	// Warning:  Derived classes should provide their own copy constructor
	// that first constructs a default object and then uses the assignment
	// operator to synchronize  image creation, assignment, and
	// destruction.  Derived classes should not call the copy constructor
	// in the base class.
    CVisWarpedFrame(const CVisImage<TPixel>& refimage);

    CVisWarpedFrame<TPixel>& operator=(const CVisImage<TPixel>& refimage);

    virtual ~CVisWarpedFrame(void);


	//------------------------------------------------------------------
	// @group Attributes

    // LATER:  We might want similar const methods.
    const CVisFloatImage& ImageLocalWarp(void) const;
    CVisFloatImage&     ImageLocalWarp(void);
    SColorAdjustParam&  ColorAdjust(TVisColorDir);
	void                ClearColorAdjust();
	const T_Image& ImageWarped(void) const;
	T_Image& ImageWarped(void);

	// @cmember
	// Use m_rectCrop to crop the current image, if needed.
	CVisWarpedFrame<TPixel> Cropped(void);
	CVisWarpedFrame<TPixel> Cropped(RECT& rect);


    //------------------------------------------------------------------
	// SDStream I / O

	// These two methods are overridden in derived classes to give type
	// information (about the derived classes) to the property list code.
	virtual CVisPropTypeInfoBase *PropTypeInfoNew(void) const;
	virtual CVisPropTypeInfoBase *PropTypeInfoLookup(void) const;

	void BuildPropList(CVisPropList& refproplist);

    //------------------------------------------------------------------
	// @group Old self-describing stream I/O

	// @cmember
	// Read or write properties to a self-describing stream.
	virtual void SDReadWriteProperties(CVisSDStream& s,
			bool fAddComma = false);


	//------------------------------------------------------------------
	// @group Old file I/O methods
// Use base class methods for now for compatibility with old file formats.
// (Composite and WarpedComposite code will still write additional data
// in their own sections.)

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
	enum { eilevelWarpedFrame = eilevelCompositeFrame + 1 };
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
	CVisFloatImage   m_imageLocalWarp;
	SColorAdjustParam m_ColorAdjust[2];
	T_Image m_imageWarped;
};


enum TVisWarp {
    VIS_WARP_NONE,              // no warp, flat images
    VIS_WARP_CYLINDRICAL,       // use cylindrical coordinates
    VIS_WARP_SPHERICAL,         // use spherical coordinates
    VIS_WARP_ZERO               // fill with 0z (used in LayeredComposite)
};

float FocalLengthFromHFOV(CVisImageBase& src, float horizontal_field_of_view);
float FocalLengthFromVFOV(CVisImageBase& src, float vertical_field_of_view);
float HFOVFromFocalLength(CVisImageBase& src, float focal_length);
float VFOVFromFocalLength(CVisImageBase& src, float focal_length);

CVisShape VisCylindricalShape(CVisImageBase& src, 
                                 float focal_length, float torsion, float tilt,
                                 TVisWarp warp, int inverse, int uv_spacing,
                                 float k1 = 0.0, float k2 = 0.0);

CVisFloatImage VisCylindricalUV(CVisImageBase& src, CVisImageBase &dst,
                              CVisTransform4x4& T1, CVisTransform4x4& T2,
                              TVisWarp warp, int inverse, int uv_spacing,
                              float k1 = 0.0f, float k2 = 0.0f,
                              BOOL bRadialUsesDistCoord = FALSE);

CVisFloatImage VisCylindricalUV(CVisImageBase& src, CVisImageBase &dst,
                              float focal_length, float torsion, float tilt,
                              TVisWarp warp, int inverse, int uv_spacing,
                              float k1 = 0.0, float k2 = 0.0,
                              BOOL bRadialUsesDistCoord = FALSE);


// for spherical/cylidrical map construction
CVisFloatImage VisSphericalMapUV(CVisImageBase& src, CVisImageBase &dst,
								CVisTransform4x4& TVR); 



template <class PIXELTYPE>
void VisCylindricalWarp(CVisImage<PIXELTYPE> &src, 
                          CVisImage<PIXELTYPE> &dst, 
                          TVisInterpolation interpolation,
                          float focal_length, TVisWarp warp,
                          int inverse, int uv_spacing, int zero_outside,
                          int k1 = 0.0, int k2 = 0.0);


//
//  This class derived from CVisComposite
//   performs cylindrical/spherical warping of images
//

template <class TPixel>
class CVisWarpedComposite : public CVisComposite<TPixel> {
public:
	typedef CVisWarpedFrame<T_Pixel> T_Frame;
	typedef CVisComposite<TPixel>	T_BaseSequence;
    
	// Constructor and destructor
    CVisWarpedComposite(EVisSequence evissequence = evissequenceDelayReadUntilNeeded);

    // Query/set parameters
    TVisWarp WarpMode() const         { return m_warp; }
    int LocalWarpMode() const         { return m_local_warp; }
    int WarpGridSpacing() const       { return m_warp_grid_spacing; }
    float HorizontalFOV() const       { return m_horizontal_fov; }
    float VerticalFOV() const         { return m_vertical_fov; }
    float FocalLength() const         { return m_focal_length; }
    float Torsion() const             { return m_torsion; }
    float Tilt() const                { return m_tilt; }
    float K1() const                  { return m_k1; }
    float K2() const                  { return m_k2; }
    BOOL  RadialUsesDistCoord() const { return m_bRadialUsesDistCoord; }
    float EnlargeBy() const           { return m_enlarge_by; }
    void SetWarpMode(TVisWarp w);
    void SetLocalWarpMode(int is_local);
    void SetWarpGridSpacing(int s);
    void SetHorizontalFOV(float fov);
    void SetVerticalFOV(float fov);
    void SetFocalLength(float f);
    void SetTorsion(float angle);
    void SetTilt(float angle);
    void SetK1(float k1);
    void SetK2(float k2);
    void SetRadialUsesDistCoord(BOOL val);
    void SetEnlargeBy(float enlarge_by);

    // Return warped image or warping field
    T_Image& Warped(int index);  // specialized warping code
    T_Image& Warped()     { return Warped(CurrentFrame()); }
    CVisFloatImage& GlobalWarp(int index = 0);
    CVisFloatImage& LocalWarp(int index);

    // Override collection operations (see VisCollection.h for definitions)
#ifdef TAKEOUT
    void Insert(TFrame &frame, int index = -1, int new_geometry = 0);
    void Insert(const char *filename, int index = -1);
    void Raise(int index, int to_top = 1);
    void Lower(int index, int to_bottom = 1);
    void Delete(int index);
    void DeleteAll();
#endif // TAKEOUT

    // File I/O
    virtual void ReadMetaData(FILE *stream);
    virtual void WriteMetaData(FILE *stream);
    void ReadWarpCollection(FILE *stream);
    void WriteWarpCollection(FILE *stream);
    char* WarpFilename(int index, char *prefix, char *suffix = 0,
                       char *extension = 0) const;
    void WarpToOffset(CVisRGBAByteCollection& color_coll);
    void OffsetToWarp(CVisRGBAByteCollection& color_coll);

    // Final re-sizing and cropping
    void StraightenToWidth(int base_frame, int desired_width, 
                           int crop_vertically);

    // Conversion back to projective panorama
    void CylindricalToPanoramic(int base_frame);

    // Compute/update local warp from block-based motion parameters
    void UpdateLocalWarp(int index, CVisFloatImage& patch_flow,
                         int patch_spacing, bool ignore_covariances);

    // Parameters have changed, recompute the Warped images
    void InvalidateWarp(int dont_delete_local_warps = 0);

	// clears the color adjustment parameters for each frame
	void ClearColorAdjust();

    // Conversion from warp field to colored offset field (for writing)

    // for spherical/cylidrical map construction
    T_Image& Mapped(int index); // specialized mapping code
    T_Image& Mapped()    {return Mapped(CurrentFrame()); }
    CVisFloatImage& GlobalSphericalMap(int index); 

    int SphericalMap()              {return m_sphericalMap; }
    int MapLength()                 {return m_mapLength;    }
    void SetSphericalMap(int s)     {m_sphericalMap = s;    }
    void SetMapLength(int l)        {m_mapLength = l;       }
	bool WarpingDisabled();  // Test whether Warped will actually change the image or not
					// A return value of true means Warped returns the original image

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
	enum { eilevelWarpedComposite = eilevelComposite + 1 };
	enum { eiLevel = T_BaseSequence::eiLevel + 1 };


 protected:
// UNDONE:  Make data members private?
    TVisWarp m_warp;                // warping mode
    float m_horizontal_fov;         // horizontal field of view in degrees
    float m_vertical_fov;           // vertical   field of view in degrees
    float m_focal_length;           // focal length in pixels
    float m_torsion;                // angular rotation in degrees
    float m_tilt;                   // upward tilt in degrees
    float m_k1, m_k2;               // radial distortion parameters
    float m_enlarge_by;             // amount to enlarge each frame if warped
    BOOL  m_bRadialUsesDistCoord;   // use the distorted coords 
    int m_warp_grid_spacing;        // subsampling of warp function
    CVisFloatImage m_warp_uv;       // warp coordinates (may be re-used)
    CVisShape m_warped_shape;       // shape of warped image

//    CVisCollection<T_Pixel> m_warped;  // cylindrically warped images
	T_Frame m_frameTmp;				// temporary CVisFrame object used in Cropped().
    int m_local_warp;               // use the local (per-frame) warping function
//    CVisFloatCollection m_warp_local;  // local warp coordinates
    float m_local_warp_blur;        // blurring during local warp update
    float m_warp_grid_scale;        // fixpoint scale for reading/writing

    int m_sphericalMap;             // spherical map or cylindrical map
    int m_mapLength;                // spherical map length

    friend class CVisLayeredComposite;    // needs to set m_warp_grid_scale

	virtual void PreReadValue(CVisSDIStream& refsdistream);
	virtual void PostReadValue(CVisSDIStream& refsdistream);
	virtual void PreWriteValue(CVisSDOStream& refsdostream) const;
	virtual void PostWriteValue(CVisSDOStream& refsdostream) const;

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

struct CVisLocalWarp {
    static int m_use_B_channel;     // use blue channel when storing warps
    static int m_write_as_float;    // write out the warp/flow directly
    static int m_warp_on_read;      // force re-warp of images on readin
};

//
//  Internal structure used in computing update to local warp
//

struct SVisPatchFlow {
    float C[2][2];      // confidence (Hessian)
    float u, v;         // flow estimate
    float x, y;         // center of patch
    float n;            // number of matches + 1 (for internal computation)
};

// Sample class definitions
typedef CVisWarpedComposite<CVisRGBABytePixel>  CVisColorComposite;
typedef CVisWarpedComposite<CVisRGBABytePixel>  CVisRGBAComposite;
typedef CVisWarpedComposite<BYTE>    CVisGrayComposite;


VIS_SD_CLASS_WITH_IO_PL_TYPEDEF(SColorAdjustParam)

#include "VisCylindrical.inl"


#endif // _VIS_CYLINDRICAL_H_
