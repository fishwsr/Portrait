///////////////////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisLayer.h -- a single layer in a multi-layer stereo reconstruction
//
// DESCRIPTION
//  The CVisLayer class is used to represent a single layer within
//  a multi-plane stereo scene recovered from a collection of images.
//
//  This class is derived from CVisRGBAComposite.  It contains a reference
//  count, for creating a smart pointer, and auxilliary fields such
//  as membership, bias and gain, and residual motion estimates.
//
// PUBLIC MEMBER FUNTIONS (METHODS)
//  CVisLayer();
//  ~CVisLayer();
//
//  ...
//
// PARAMETERS
//  ...
//
// SEE ALSO
//  VisLayer.cpp          layer composite implementation code
//  VisLayeredComposite.h layered stereo class
//  VisComposite.h        composite (mosaic) class
//  VisCollection.h       image collection class
//  VisBlend.h            image blending / compositing operations
//  VisAlign.h            interactive and automatic alignment algorithms
//  VisCylindrical.h      cylindrically warped composites
//
//  Rick Szeliski's research notes, 27-Jun-97 to 24-Jul-97.
//
// BUGS
//
//
// DESIGN
//
//
// NOTES
//  - Various array data members (such as m_membership) are allocated to length 1 
//    arrays in the constructor and their first element initialized to a sensible 
//    value. This should be changed so the pointers are null until space is actually
//    required since it may be hiding bugs. Similarly, empty images are currently 
//    all allocated to 1x1 images (e.g. using ReAllocate(1,1)). This should be 
//    changed at some point again because it may also be covering up some serious 
//    bugs. (For example, I had problems with VisIntersection of shapes, etc.)
//  - There is a lot of duplicate code in ReadCollection(), Insert(frame), 
//    Insert(filename), Raise(), and Lower(). Probably should try to merge the 
//    required functionality into one function to avoid errors being introduced 
//    should new data members be added to CVisLayer.
//   
// HISTORY
//  24-Jul-97  Richard Szeliski (szeliski) at Microsoft
//  Broke off from VisLayeredComposite class.
//
// Copyright © Microsoft Corp. 1997
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _VIS_LAYERED_H_
#define _VIS_LAYERED_H_

#include <vector>


///////////////////////////////////////////////////////////////////////////////////////
//																					 
//         class CVisLayer  --  A single layer of a multi-layer reconstruction	     
//																		             
///////////////////////////////////////////////////////////////////////////////////////
//
// 
// Each image in a CVisLayer object can either be part of computing the composite
// or not (information stored in m_membership):
//
enum TVisLayerMembership {
	VIS_LAYER_MEMBERSHIP_OUT,
	VIS_LAYER_MEMBERSHIP_IN
};
//
// There are two different types of difference, warped and unwarped. Either neither,
// both or just the warped diff can be valid (stored in m_valid_diff)
//
enum TVisDifferenceValid {
    VIS_INVALID_DIFF = 0,
    VIS_VALID_WARPED_DIFF = 1,
	VIS_VALID_UNWARPED_DIFF = 2
};
//
// There are a number of ways to form the composite image (ie. background layer):
//
enum TVisLayerCompOp {
	layer_comp_feather,
	layer_comp_over,
	layer_comp_median
};
//
// There are a number of ways the difference images can be computed:
//
enum TVisLayerDiffOp {
	layer_diff_intensity,
	layer_diff_normal_flow,
	layer_diff_residual_flow,
	layer_diff_int_res_flow
};



// A CVisWarpedFrame is an image with geometry information, a (float)
// feathered image, a (float) local warp image, and a warped image.
class CVisLayerFrame : public CVisWarpedFrame<CVisRGBABytePixel>
{
public:
	typedef CVisRGBABytePixel T_Pixel;
	typedef CVisImage<CVisRGBABytePixel> T_Image;
	typedef CVisWarpedFrame<CVisRGBABytePixel> T_FrameBase;


	//------------------------------------------------------------------
	// @group Constructors, assignement operators, and destructors

    CVisLayerFrame(void);
    CVisLayerFrame(const RECT& refrect);
    CVisLayerFrame(const CVisShape& refshape);
    CVisLayerFrame(const char *szFilename);

	// Construct an image from another image.
	// Warning:  Derived classes should provide their own copy constructor
	// that first constructs a default object and then uses the assignment
	// operator to synchronize  image creation, assignment, and
	// destruction.  Derived classes should not call the copy constructor
	// in the base class.
    CVisLayerFrame(const T_Image& refimage);

    CVisLayerFrame& operator=(const T_Image& refimage);

    virtual ~CVisLayerFrame(void);


	//------------------------------------------------------------------
	// @group Attributes

	TVisLayerMembership EVisLayerMembership(void) const
		{ return m_evislayermembership; }
	int GrfValidDifference(void) const
		{ return m_grfValidDifference; }
	double DblBias(void) const
		{ return m_dblBias; }
	double DblGain(void) const
		{ return m_dblGain; }
	int CPixel(void) const
		{ return m_cpixel; }
	const T_Image& ImageDifference(void) const
		{ return m_imageDifference; }
	const T_Image& ImageUnwarpedDifference(void) const
		{ return m_imageUnwarpedDifference; }

	void SetEVisLayerMembership(TVisLayerMembership evislayermembership)
		{ m_evislayermembership = evislayermembership; }
	void SetGrfValidDifference(int grfValidDifference)
		{ m_grfValidDifference = grfValidDifference; }
	void SetDblBias(double dblBias)
		{ m_dblBias = dblBias; }
	void SetDblGain(double dblGain)
		{ m_dblGain = dblGain; }
	void SetCPixel(int cpixel)
		{ m_cpixel = cpixel; }

	T_Image& ImageDifference(void)
		{ return m_imageDifference; }
	T_Image& ImageUnwarpedDifference(void)
		{ return m_imageUnwarpedDifference; }

	// @cmember
	// Use m_rectCrop to crop the current image, if needed.
	CVisLayerFrame Cropped(void);
	CVisLayerFrame Cropped(CRect& rect);


    //------------------------------------------------------------------
	// @group Self-describing stream I/O

	// @cmember
	// Read or write properties to a self-describing stream.
	virtual void SDReadWriteProperties(CVisSDStream& s,
			bool fAddComma = false);


	//------------------------------------------------------------------
	// @group Old file I/O methods
// Use base class methods for now for compatibility with old file formats.
// (Composite, WarpedComposite, and Layer code will still write additional
// data in their own sections.)

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
	enum { eilevelLayerFrame = eilevelWarpedFrame + 1 };


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
	T_Image m_imageDifference;
	T_Image m_imageUnwarpedDifference;
	double m_dblBias;
	double m_dblGain;
	TVisLayerMembership m_evislayermembership;
	int m_cpixel;
	int m_grfValidDifference;
};


class CVisLayer : public CVisRGBAComposite {
// class CVisLayer : public CVisStereo {
public:
	typedef CVisRGBABytePixel T_Pixel;
	typedef CVisRGBAByteImage T_Image;
	typedef CVisLayerFrame T_Frame;

	
	// Constructor and destructor
	// UNDONE:  The default options are different for layers than
	// for other composites.
    CVisLayer(EVisSequence evissequence = evissequenceDontDelayRead);
	~CVisLayer(); 

	// Reference counting
	unsigned long AddRef(void);
	unsigned long Release(void);


	// Functions for accessing composite membership information
	TVisLayerMembership CompositeMembership(int index); 
	void SetCompositeMembership(int index, TVisLayerMembership membership);
	
	// Functions for accessing difference images
	CVisRGBAByteImage& DifferenceImage(int index)   { return At(index).ImageDifference(); }
	CVisRGBAByteImage& UnwarpedDiffImage(int index) { return At(index).ImageUnwarpedDifference(); }

	// Functions for accessing the bias and gain information
	void   DoUseBias()                  { m_fUseBias = true; }
	void   DontUseBias()                { m_fUseBias = false; }
	bool   UsingBias()                  { return m_fUseBias; }
	double Bias(int i)                  { return At(i).DblBias(); }
	double Gain(int i)                  { return At(i).DblGain(); }
	int    NumberOfPixels(int i)		{ return At(i).CPixel(); }
	void   SetBias(int i, double bias)  { At(i).SetDblBias(bias); }
	void   SetGain(int i, double gain)  { At(i).SetDblGain(gain); }

	// Override VisCollection operations so we can keep membership information
	// up to date (see VisCollection.h for details of overidden functions)
	void ReadCollection(const char *filename, int read_files = 0, FILE *istream = 0,
						TVisLayerMembership member = VIS_LAYER_MEMBERSHIP_OUT);
	void WriteCollection(const char *filename, const char *desc_type = 0,
                         int write_files = 0, FILE *ostream = 0, int write_bias = 0);

#ifdef TAKEOUT
    void Insert(CVisRGBAByteFrame &frame, int index = -1, int new_geometry = 0, 
				TVisLayerMembership member = VIS_LAYER_MEMBERSHIP_OUT);
    void Insert(const char *filename, int index = -1,
				TVisLayerMembership member = VIS_LAYER_MEMBERSHIP_OUT);
    void Raise(int index, bool to_top = true);
    void Lower(int index, bool to_bottom = true);
    void Delete(int index);
    void DeleteAll();
#endif // TAKEOUT

	// Detecting empty frames
    int  IsEmpty();
    int  IsEmptyFrame(int index);

    // Override frame positioning 
 	void SetCurrentFrame(int index);  // UNDONE:  Replace with base class method.
#ifdef TAKEOUT
    void SetFirst();
    void SetLast();
    void SetNext();
    void SetPrev();
#endif // TAKEOUT

	// Override shape computation functions so that we can incorporate composite 
	// membership information (not all of these need to be overridden)
    CVisShape TopLayerShape()          { return LayerShape(CurrentFrame()); }
	CVisShape BottomLayerShape(int omit_this_image = -1);
    CVisShape CompositeShape()         { return (TopLayerShape() | BottomLayerShape()); }
    CVisShape DifferenceShape(int i)   { return DifferenceImage(i).Shape(); }
    CVisShape UnwarpedDiffShape(int i) { return UnwarpedDiffImage(i).Shape(); }

	// Override view computation functions so that we can incorporate composite 
	// membership information (and later change the way the composite is computed)
    CVisRGBAByteImage& TopLayer();
    CVisRGBAByteImage& TopAlpha();
    CVisRGBAByteImage& BottomLayer(TVisLayerCompOp op);
    CVisRGBAByteImage& BottomAlpha(TVisLayerCompOp op);
    CVisRGBAByteImage& Composite(EVisComposite comp_op, TVisLayerCompOp bgl_op); 
    CVisRGBAByteImage& BlinkLayer(TVisLayerCompOp op);

	// Difference computation functions
    CVisRGBAByteImage& Difference(TVisLayerCompOp comp_op, TVisLayerDiffOp diff_op);
    CVisRGBAByteImage& UnWarpedDiff(TVisLayerCompOp comp_op, TVisLayerDiffOp diff_op);
    CVisRGBAByteImage& SynthDifference(TVisLayerCompOp comp_op, TVisLayerDiffOp diff_op, 
										CVisRGBAByteImage &im);
    CVisRGBAByteImage& CompSynthDiff(TVisLayerCompOp comp_op, TVisLayerDiffOp diff_op, 
									  CVisRGBAByteImage &im);

	// Layer invalidation: Need to wrap CVisRGBAComposite::Invalidate() to maintain
	// validity information for the differences
	void Invalidate(int mask = 0) { Invalidate(mask, CurrentFrame()); }
	void Invalidate(int mask, int image);
	void InvalidateAll(int mask = 0);
	
	// Miscellaneous functions
	void EstimateBias(TVisLayerCompOp op);
	void SetViewGeometry(const CVisTransformChain& geometry)
		{ CVisRGBAComposite::SetViewGeometry(geometry); Invalidate(); }

	// Flow scale
	double DblFlowScale(void) const
		{ return m_dblFlowScale; }
	void SetDblFlowScale(double dblFlowScale)
		{ m_dblFlowScale = dblFlowScale; }


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
	// @group Self-describing stream (low-level) I/O

	// @cmember
	// Read or write properties to a self-describing stream.
	virtual void SDReadWriteProperties(CVisSDStream& s,
			bool fAddComma = false);

	// Constant used to specify the depth of this object (for use with
	// ObjectTypeInfo).
	enum { eilevelLayer = eilevelWarpedComposite + 1 };


protected:
	// Override view computation functions so that we can incorporate composite 
	// membership information (and later change the way the composite is computed)
    void FormTopLayer(CVisShape shape);
    void FormTopWBkgd(CVisShape shape);
    void FormBottomLayer(CVisShape shape, TVisLayerCompOp op);
    void FormComposite(CVisShape shape, EVisComposite comp_op); 
	void FormDifference(CVisRGBAByteImage &im1, CVisRGBAByteImage &im2,  
		                CVisRGBAByteImage &im3, CVisShape shape, TVisLayerDiffOp op);

	// Compositing functions
	void LayerCompFeather();
	void LayerCompOver();

	// Differencing functions
	void CopyFlowIntoDifference(int index, CVisRGBAByteImage &img);
	void LayerDiffIntensity(CVisRGBAByteImage &im1, CVisRGBAByteImage &im2, 
							CVisRGBAByteImage &im3);


	// The following CVisSequenceBase methods are used to notify the CVisLayer
	// when the index of the current image or the order of images in the
	// sequence is changed.  When this happens, the composite needs to be
	// invalidated.
	virtual void OnICurChanged(void);
	virtual void OnOrderChanged(void);
	virtual void OnSequenceChanged(void);

protected:

#ifdef TAKEOUT
	int                  m_nframes;			// Updated so it always equals NFrames()
	TVisLayerMembership *m_membership;		// Composite membership information

    CVisRGBAByteImage  *m_difference;		// Difference of each image from composite 
    CVisRGBAByteImage  *m_unwarped_diff;	// Difference warped back onto original image
	int                 *m_valid_diff;		// Validity of difference images
	double              *m_bias;            // Estimate of the camera bias
	double              *m_gain;            // Estimate of the camera gain
	int                 *m_num_pixels;      // Number of pixels used to estimate bias/gain
#endif // TAKEOUT

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


private:
	// Major data members
	unsigned long		 m_cRef;			// Reference count
	bool                 m_fUseBias;        // Whether to use bias and gain estimates
	double				 m_dblFlowScale;	// No. of grey levels per unit flow magnitude
};

#endif // _VIS_LAYER_H
