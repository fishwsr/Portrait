///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisAlign.h -- global image alignment (registration) routine
//
// DESCRIPTION
//  The CVisAligner class aligns images using a global group
//  transformation such as a translation, rotation, affine or planar
//  projective (perspective) transformation.  The transformation is
//  described using a 4x4 homogeneous coordinate transform matrix
//  (the third row and column are ignored -- see VisGeometry.h 
//  for a more detailed description)
//
//  ImageAlignStep proceeds in the following manner:
//      1. re-sample deformable into warped by current transformation
//      2. compute the image gradients (several options exist)
//      3. accumulate the outer product gradients and
//         gradient/error products, possibly on a coarser grid
//      4. accumulate the Hessian and gradient vectors for the parameters
//      5. solve for the change in transformation and update
//      6. compute the change in pixel positions
//
// PUBLIC MEMBER FUNTIONS (METHODS)
//  CVisAligner();
//  ~CVisAligner();
//
//  void SetFixed(Image &img);
//  void SetDeformable(Image &img);
//
//  void SetAlignment(CVisTransform4x4 &transform);
//  void GetAlignment(CVisTransform4x4 &transform);
//  void SetTranslation(double uv[2]);
//  void GetTranslation(double uv[2]);
//
//  void RecomputeBoundingBoxes(int slop);
//  void SingleStep(int level);
//  void RunMultilevel(int n_levels, int max_iterations,
//                     double min_uv_change);
//
// PARAMETERS
//  img                 source images being aligned, alpha-premultiplied
//  transform           4x4 transformation matrix
//  uv                  translational part of transformation
//
//  PUBLIC MEMBER VARIABLES (OPTIONS)
//
//  motion_model        current motion model
//  band_pass           band-pass images (set before SetFixed/Deformable)
//  initial_blur        number of initial 121 blur steps
//  adjust_bias         recompute bias after each iteration
//  robustify           use robust outlier removal
//  use_mask            use masked images or alpha channel
//  bound_box_extra     extra pixels around bbox edge
//  subsample           speedup Hessian and gradient computation
//  fixed_Hessian       don't recompute Hessian
//  store_error         store error image (for debugging)
//
//  bias                current bias (DC offset)
//  RMS_error           root mean squared intensity difference
//  uv_correction       root mean squared pixel motion due to correction
//
// SEE ALSO
//  VisAlign.cpp      alignment algorithm implementation
//  VisGeometry.h     geometric transformations
//  VisResample.h     global geometric warping
//
// BUGS
//   
//
// DESIGN
//  
//
// HISTORY
//  16-May-96  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#ifndef _VIS_ALIGN_H_
#define _VIS_ALIGN_H_

#ifndef ALIGN_MAX_LEVELS
#define ALIGN_MAX_LEVELS 8      // maximum height of image pyramids
#endif

enum TVisAlignmentModel {                              // # of parameters
    VIS_ALIGNMENT_TRANSLATION = 1,  // translation pure                 2
    VIS_ALIGNMENT_ROTATION    = 2,  //  " + rotation                    3
    VIS_ALIGNMENT_SIMILARITY  = 3,  //  " + rotation and scale          4
    VIS_ALIGNMENT_ASPECT      = 4,  //  " + aspect ratio and scale      4
    VIS_ALIGNMENT_AFFINE      = 5,  // affine                           6
    VIS_ALIGNMENT_PROJECTIVE  = 6,  // planar projective (perspective)  8
    VIS_ALIGNMENT_FREEROTATION =7,  // free rotation                    3
                                    // if unknown focal length          4
    VIS_ALIGNMENT_PROJ_ROTNORM =8,  // projective but normalized using
                                    // estimated f0, f1, and rotation   8
    VIS_ALIGNMENT_FOCALLENGTH  =9,  // unknown f1, but known f0         1
    VIS_ALIGNMENT_INVALID           // should not be used
};

//
//  Internal structure used for accumulating gradients and Hessians
//

struct SVisGradientBlock {
    float n;            // number of pixels (or alpha)
    float e;            // error
    float e2;           // error^2

    float e_gx;         // error * horizontal gradient
    float e_gy;         // error * vertical   gradient

    float gx2;          // horizontal gradient ^ 2
    float gx_gy;        // cross product of gradients
    float gy2;          // vertical   gradient ^ 2

    float x;            // x coordinate of block center
    float y;            // y coordinate of block center
    float w;            // weighting (for robust version)
    float u;            // integral displacement estimate
    float v;            // integral displacement estimate
};

template <class TImage>
class CVisAligner {
public:
    // Constructor and destructor
    CVisAligner();
    ~CVisAligner();

    // Set or update (reset) the two images being aligned
    void SetFixed(TImage &img);
    void SetDeformable(TImage &img);

    // Initialize and read back motion parameters (two forms)
    void SetAlignment(CVisTransform4x4  transform);
    void GetAlignment(CVisTransform4x4 &transform);
    void SetAlignment(CVisTransformChain  geometry);
    void GetAlignment(CVisTransformChain &geometry);
    void SetTranslation(double uv[2]);
    void GetTranslation(double uv[2]);

    // Set and read focal length estimates
    void SetF0F1(double f0, double f1)  { m_f0 = f0, m_f1 = f1; }
    double GetF0()          { return m_f0; }
    double GetF1()          { return m_f1; }

    // calculate error given a transform
    double CalcError();

    // Control the iteration
    void SingleStep(int level);
    int  SingleStep();  // step at current_level, optionally decrease level
    void RunMultilevel(int max_iterations);

    // Alignment algorithm options
    TVisAlignmentModel motion_model;   // current motion model
    int n_levels;               // number of levels in coarse-to-fine
    int band_pass;              // use band-pass filtered images
    int initial_blur;           // number of initial 121 blur steps
    int adjust_bias;            // recompute bias after each iteration
    int robustify;              // use robust outlier removal
    float robust_cutoff;        // multiple of sigma to use as cutoff
    int decrease_E;             // no change if E has not decreased
    int use_mask;               // use masked images or alpha channel
    int bound_box_extra;        // extra pixels around bbox edge
    int subsample;              // speedup Hessian and gradient computation
    int fixed_Hessian;          // don't recompute Hessian
    int store_error;            // store error image (for debugging)
    int border_width;           // ignore pixels around border
    float max_uv_change;        // maximum allowed motion per step
    float min_uv_change;        // motion threshold for decreasing level
    int max_inner;              // maximum inner iterations per step
    int max_steps_level;        // maximum steps per level
    int search_area2;           // half-size of correlation search area
    int search_subdivide;       // subdivide into n^2 problems
    float search_margin;        // winner must beat (0,0) cost by this much
    int search_coarsest;        // only perform search at coarsest level
    int horizontal_flow;        // constrain flow to be horizontal
    int vertical_flow;          // constrain flow to be vertical
    int gridded_xy;             // snap (x,y) coordinates inside block to grid

    // Alignment algorithm state (current level)
    int current_level;          // current level for step
    int stopat_level;           // level to stop (usually 0)

    // Algorithm statistics (user setable at startup)
    float bias;                 // current bias (DC offset)
    float old_error;            // previous RMS error (for diagnostics)
    float RMS_error;            // current RMS error (used by robustify)
    float uv_correction;        // amount of incremental flow at corners
    float percent_outliers;     // number of blocks rejected as outliers

//private:
protected:
    // Internal functions
    void InitLevel(int level);  // precompute filtered images
    void RecomputeBoundingBoxes();  // shrink images to smaller size
    void AccumulatePixelGradients(int level, int recompute_Hessian,
                                  int search, int compute_uv = 0, int use_old_uv = 0);
    void MakeWeightsRobust(int level, int search);
    void AccumulateParameterGradients(int level, int recompute_Hessian,
                                      int search);
    void UpdateParameters(int level, float step_size);

    CVisTransform4x4 m_motion;     // current motion estimate

    // Free-rotation specific parameters
    CVisTransformChain m_geometry;    // 6-element rotation chain
    float m_f0, m_f1;           // current focal length estimates

    int m_valid_fixed_levels;   // valid levels in fixed pyramid
    int m_valid_float_levels;   // valid levels in deformable pyramid
    int m_valid_Hessian;        // valid Hessian at this level (bit vector)
    int m_searched_level;       // already searched this level (bit vector)
    int m_search_block;         // size of search block

    TImage m_fixed_original;        // original fixed image
    TImage m_float_original;        // original deformable image
    CVisPyramid<TImage> *m_fixed;   // fixed image
    CVisPyramid<TImage> *m_float;   // image being adjusted
    TImage *m_warped;               // float after warping
    CVisFloatImage *m_gradient;         // gradient of warped
    CVisFloatImage *m_search_uv;        // 2-D search results

    CVisShape trimmed_fixed_shape;   // shape of fixed cropped by transformed float
    CVisShape trimmed_float_shape;   // shape of float cropped by transformed fixed

    // Q: should these be made float to avoid misalignment?
    //  they may be, unless the CVisAligner is created by new!
    double A[8][8];         // Hessian matrix
    double b[8];            // weighted error . gradient
    double c;               // sum squared error

	//the following members are used for tracking purpose.
private: //KE, to record the search result and used in tracking refine.
	float u0;
	float v0;
public:
	void GetHessian(double hessian[8][8]);
	void GetB(double error_b[8]);
	float GetU0(void){ return u0;}
	float GetV0(void){ return v0;}
	void SetU0(float u){u0 = u;}
	void SetV0(float v){v0 = v;}
};

typedef CVisAligner<CVisRGBAByteImage> CVisRGBAAligner;
typedef CVisAligner<CVisByteImage>   CVisGrayAligner;

// add zoom for floating focal length
//CVisTransform4x4 ComputeInteractiveDeformation(CVisTransform4x4 motion,
CVisTransformChain ComputeInteractiveDeformation(CVisTransform4x4 motion,
    CVisShape sh, TVisAlignmentModel motion_model,
    CVisVector4 p0, CVisVector4 p1, double f, double zoom, 
    CVisTransformChain view);

// rotation matrix from Rodrigues formula, also needed in adjuster class
CVisTransform4x4 RodriguesFormula(double rx, double ry, double rz);

//  Note:  we could define some derived classes which have specialized
//      parameters, e.g., more robust, faster, fixed pattern, etc...

#endif // _VIS_ALIGN_H_
