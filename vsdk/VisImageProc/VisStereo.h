///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisStereo.h -- compute depth map from collection of images
//
// DESCRIPTION
//  CVisStereo class is used to store a collection of images with associated
//  camera matrices (like CVisComposite), and to compute a depth map and
//  consensus color map from this collection.
//
//  A variety of algorithms can be used to achieve this (each implementation
//  can be its own derived class).  We envisage at least two being implemented
//  intitially:
//
//  1) A "plane sweep" algorithm [Collins, CVPR'96][Seitz & Dyer, CVPR'97]
//      [Szeliski & Golland, ICCV'98].  
//   a) For every disparity hypothesis, a homography is computed between 
//      each input image and the stereo "reference" image.
//   b) Each input image is then warped, a consensus (mean) color image 
//      is estimated, and a per-pixel variance away from this mean is estimated.
//   c) The variances are converted into confidences, which are then aggregated
//      spatially.
//   d) A winning disparity is selected at each pixel (or no disparity, if the
//      confidence is too low.
//   e) Optionally, a "fractional" disparity is estimated based on the local
//      confidence vs. disparity curve
//
//  2) A coarse-to-fine "optic flow"-like algorithm [Quam, IUW'84]
//      [Bergen et al, ECCV'92] [Szeliski & Coughlan, CVPR'94]
//   a) An initial disparity estimate (say 0) is selected at the coarsest
//      pyramid level
//   b) Each input image is warped according to the current per-pixel disparity
//      estimate, and a consensus (mean) color image is estimated,
//   c) Per-pixel errors, intensity gradients, and partials of (x,y) positions
//      with respect to incremental disparity changes are computed.
//   d) These are used to compute the first and second derivatives of the local
//      error with respect to disparity changes (Levengberg-Marquardt)
//   e) These quantities are aggregated spatially (corresponds to a local
//      patch-based SSD measure), and an incremental disparity step is taken
//      (checking for a decrease in local squared error?)
//   f) After iterating for a while at a given level, the disparity estimates
//      are projected to the next finer level.
//
//  These two approaches can be mixed, i.e., a plane-sweep algorithm could be
//  used to initialize a fine-level gradient-based correction, or it could be
//  used to initialize the coarse level of a hierarchical warp stereo algorithm.
//
//  To deal with self-occlusion in the disparity surface, it is possible to
//  determine the "visibility" of each input image pixel with respect to the 
//  reference camera, and to "erase" invisible pixels before re-computing the
//  stereo match [Szeliski & Golland, ICCV'98].
//
// INPUT
//  The input to the algorithm consists of:
//  . a collection of alpha-matted images, each of which has a camera matrix
//  . the size (actually, CVisShape) of the reference image (and hence depth map)
//  . a camera matrix for the "reference image"
//  . a plane equation for the "0" disparity plane
//  . minimum and maximum disparities
//  . a scale factor for converting disparities into plane equation offsets
//    (see the Tech Note on "Sprites with Depth" for the exact definitions).
//  There are helper functions for determining some of these quantities, e.g.,
//  determining the output image size from the set of homographies at 0 depth
//  (plus some padding), or for setting the disparity range and scale factor
//  based on maximum expected pixel motion
//
// OUTPUT
//  The output consists of:
//  . the disparity map, with a special value indicating "no depth estimate".
//    Currently, the disparity map is a CVisByteImage,
//    allowing for a (integer) disparities in the range [-127,+127].
//  . the concensus alpha-matted "sprite" image (color distribution)
//  . a per-pixel "confidence".  This could take several forms.  A plausible
//    candidate is inverse variance, which goes to 0 at "bad" pixels.
//    Unfortunately, this may fail to distinguish between "all good" matches
//    in uniform areas, and "no good" matches...
//
// INTERNAL REPRESENTATIONS
//  The plane sweep algorithm need to at least hold a color (mean) and
//  variance/confidence image for each candidate disparity.  It also needs
//  a temporary CVisRGBAComposite to compute the current mean and variance.
//
//  The coarse-to-fine algorithm requires a pyramids of images (although it could
//  also build a pyramid of CVisStereo's).  It needs a concensus image,
//  from which the errors and image gradients can be computed.  It also needs
//  per-pixel estimates of error, gradient-weighted error, and second derivatives
//  (the 1-D analogues to the structures in SVisGradientBlock).
//  Another question:  should this class be build on top of CVisAligner???
//
// PUBLIC MEMBER FUNTIONS (METHODS)
//
// PARAMETERS
//  name            description...
//
// SEE ALSO
//  VisStereo.cpp       implementation code
//  VisComposite.h      compositions / views of image collections
//  VisSprite.h         images and collections of images with depth
//
// BUGS
//   
//
// DESIGN
//  In aggregating the error in a plane sweep algorithm, e.g., SSSD,
//  do we aggregate across input images first, then optionally
//  convert into confidences, and then aggregate spatially (e.g.,
//  using non-linear iteration), or do we aggregate within each
//  input layer separately (e.g., to allow for bias/gain computation)?
//
//  It seems like the latter is an optional step that can be
//  inserted into ComputeError at the beginning of the aggregation,
//  if desired, so we will adopt the former scheme.
//
// HISTORY
//  05-Mar-98  Richard Szeliski (szeliski) at Microsoft
//  40 years old today, and still hacking code...
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#ifndef __VIS_STEREO_H__
#define __VIS_STEREO_H__

//
//  CVisStereo class:  hold one image and its relationship to a 3D surface
//

enum TVisStereoError {  // error computation type
    VIS_STEREO_SSD = 1, // Sum of Squared Differences (quadratic error)
    VIS_STEREO_SAD,     // Sum of Absolute Differences
    VIS_STEREO_MAD,     // Median of Absolute Differences
};

class CVisStereo : 
    public CVisSprite,
    public CVisRGBAComposite
{
public:
    // Constructors and destructor
    CVisStereo();

#if 0   // fill in all of these later...
    // Query and setting functions
    TVisStereoError ErrorType()  { return m_error_type; }
    void SetErrorType(TVisStereoError t) { m_error_type = t; }
#endif

    // Layer geometry and warped shape
	double DepthOfPlane(int d);					// gives actual depth value corresponding to plane d
	CVisTransform4x4 DepthPlaneHomography(int i, double current_depth);
	CVisTransform4x4 DepthPlaneHomography(int i, int d);
    virtual CVisTransform4x4 LayerTransform(int i);        // transform including base

    // Compute error between layers and composite (in CVisComposite)
    void ComputePerPixelError(int d=0);

    // Plane sweep algorithm
    void ProcessPlane(int d);		      // process the d'th depth plane
    virtual void PlaneSweep();            // process all planes, then find winners
    void WinnerTakeAll();				  // find best (winning) disparity

	// Compute per-pixel confidence 
	void ComputeConfidence(); 

	// Smooth depth map using regularization
	void DepthRegularization(CVisByteImage &copyDepth); 

    // Gradient descent algorithm...

    // Self-describing input/output format
    enum FieldType { Name, BaseSprite, BaseColorComposite, 
                        eftEnd };
    const char *ReadWriteField(CVisSDStream& s, int field_id);

	// some debug functions
	virtual char *MeanFilename(int d);
	virtual char *ErrorFilename(int d);
	virtual char *ConfidenceFilename();

    //------------------------------------------------------------------
	// SDStream I / O

	void BuildPropList(CVisPropList& refproplist);
	void ReadStereoValue(CVisSDIStream& refsdistream);

// protected:
    TVisStereoError m_error_type;   // per-pixel error metric
    int m_spatial_blur_steps;       // # of 14641 convolutions (for now)
    double m_error_threshold;       // maximum error allowed for valid match
	double m_sensorStdDev;			// per pixel (assumed constant) std. deviation of image sensor
	int m_regularization_steps;		// number of steps to regularize depth estimate

    int m_d_min;                    // minimum depth examined
    int m_d_step;                   // step in depths
    int m_d_steps;                  // number of depths examined

	int m_minNumVotes;				// minimum number of contributing images to each voxel
	int m_boxFilterSize;			// size of box filter, disabled if zero
	int m_spatialMinFilterSize;     // size of spatial min filter, disabled if zero
	int m_thresholdFilterSize;		// size of threshold edge fattening filter, disabled if zero
	bool m_fillInInvalidErrors;		// fill in invalid errors with neighboring errors?
	bool m_useSubpixel;				// compute subpixel depths?
	bool m_writeMeanImages;			// write mean images to files?
	bool m_writeErrorImages;		// write error images to files?
	bool m_writeConfidenceImage;	// write confidence image?

protected:
    double m_current_depth;         // current depth plane being evaluated

    CVisFloatImage m_error;         // per-pixel compositing error
    CVisFloatImage m_errors;        // errors at all disparities
    CVisRGBAByteImage m_colors;    // colors at all disparities

	CVisByteImage m_confidence;		// per-pixel confidence of depth estimate
};

// Old Self-describing stream functions

CVisSDStream& operator<<(CVisSDStream& s, CVisStereo& o);

// New SD Stream stuff


//VIS_SD_CLASS_WITH_IO_PL(CVisStereo)
VIS_PROP_LIST_CLASS_WITH_IO_PL(CVisStereo)
VIS_SDO_CLASS_WITH_PL(CVisStereo)
//VIS_SDI_CLASS_WITH_PL(CVisStereo)

VIS_SDI_FIND_TYPES_PL(CVisStereo)
inline void VisSDReadValue(CVisSDIStream& refsdistream,	
		CVisStereo& refobj)
	{ refobj.ReadStereoValue(refsdistream); }
VIS_SDI_READ_OBJ_DEFAULT(CVisStereo)
VIS_SDI_READ_OP_DEFAULT(CVisStereo)

// Push-pull code for filling in uncertain (< 0) values:
void VisPullPush( CVisFloatImage& img0 );   // helper function
void VisPyramidalFillGaps( CVisFloatImage& cost );  // call this one

#endif // __VIS_Stereo_COLLECTION_H__
