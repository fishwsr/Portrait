///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisPyramid.h -- image pyramid
//
// DESCRIPTION
//  CVisPyramid is used to store and create low-pass and band-pass
//  image pyramids.  By default, the pyramids have a 2:1 inter-level
//  size ratio.  Also by default, a 5-tap 1,4,6,4,1 binomial kernel
//  is used for the low-pass filtering.
//
// PUBLIC MEMBER FUNTIONS (METHODS)
//  CVisPyramid(int n_levels,
//               EVisPyramid pyramid_type = pyramid_low_pass,
//               CVisFloatImage &kernel = VisConvolutionKernel_14641,
//               TVisImagePadding replication = VIS_PADDING_REPLICATE.
//               int band_pass_bias = 0);
//  ~CVisPyramid();
//  void SetBase(TImage &img);
//
//  Image& operator[](int level);
//  Image& LowPass(int level);
//  Image& BandPass(int level);
//
//  void Construct();
//  void Construct(int from_level, int to_level);
//  void Reconstruct();
//  void Reconstruct(int from_level, int to_level);
//  void Reduce(TImage &src, TImage &dst);
//  void Expand(TImage &src, TImage &dst);
//
//  int ReadFile(const char *filename);
//  int WriteFile(const char *filename);
//
// PARAMETERS
//  n_levels            number of pyramid levels
//  pyramid_type        low-pass only, or band-pass also (two variants)
//    = pyramid_band_pass:      subtract expanded lower resolution image
//    = pyramid_band_pass_alt:  subtract low-pass filtered self
//  kernel              optional floating point filtering kernel
//  replication         choose replication used after each decimation
//  band_pass_bias      added to band-pass images (128 for gray/color?)
//  img                 source image to construct the pyramid
//  from_level          starting level for Construct or Reconstruct
//  to_level            ending level for Construct or Reconstruct
//  src, dst            source and destination for Reduce and Expand
//  filename            foo.img gets written as foo.{bp,lp}<n>.img
//
//  The dereferencing operator[] is equivalent to LowPass if
//  pyramid_low_pass is being used, or to BandPass otherwise.
//
//  SetBase replaces the base image with a new one.
//
//  Construct is the normal way to build a fully populated pyramid.
//  It can be called many times for a given pyramid, using an updated
//  version of img or even a different sized image.
//
//  Reconstruct sums up the band-pass representations, placing the
//  intermediate results in low-pass (overwrites the original low-pass pyramid,
//  including the original input image!)
//
//  Reduce and Expand are called recursively by Construct.  They are
//  normally not called externally, but can be called to interpolate
//  between levels, say in a coarse-to-fine algorithm.
//
// SEE ALSO
//  CVisPyramid.cpp    pyramid construction code
//
// BUGS
//   
//
// DESIGN
//  
//
// HISTORY
//  20-May-96  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#ifndef _VIS_PYRAMID_H_
#define _VIS_PYRAMID_H_


// UNDONE:  Change this to use standard enum names.
enum EVisPyramid {
    pyramid_low_pass,           // build only the low-pass pyramid
    pyramid_band_pass,          // + traditional band-pass (reconstructable)
    pyramid_band_pass_alt       // + alternative band-pass (non-aliased)
};

template <class TImage, class TImageFloat = CVisRGBAFloatImage>
class CVisPyramid {
public:
    // Constructor and destructor
    CVisPyramid(int n_levels,
                 EVisPyramid pyramid_type = pyramid_low_pass,
                 CVisFloatImage &kernel = VisConvolutionKernel_14641,
                 EVisPad evispad = evispadReplicate,
                 int band_pass_bias = 0);
    ~CVisPyramid();

    // Query functions
    int NLevels()                   { return m_n_levels; }
    EVisPyramid PyramidType()  { return m_pyramid_type; }
    EVisPad Padding()               { return m_evispad; }
    int BandPassBias()              { return m_band_pass_bias; }

    // Return an element of the pyramid
    TImage& operator[](int level);   // returns low-pass or band-pass
    TImage& LowPass(int level);      // returns low-pass image
    TImage& BandPass(int level);     // returns band-pass image
    TImageFloat& LowPassFloat(int level); 
    TImageFloat& BandPassFloat(int level); 

    // Perform construction step(s)
    void SetBase(TImage &img);
    void Construct();                               // build whole pyramid
    void Construct(int from_level, int to_level);   // build partial pyramid
    void ReConstruct();                             // sum up band-pass
    void ReConstruct(int from_level, int to_level); // sum up band-pass
    void Reduce(TImage &src, TImage &dst);  // reduce image size
    void Expand(TImage &src, TImage &dst);  // expand image size

    // File I/O:  placeholder -- this may change in the future
    void ReadFile(const char *filename);
    void WriteFile(const char *filename);

private:
    EVisPyramid m_pyramid_type;
    int m_n_levels;                 // number of levels
    EVisPad m_evispad;          // padding after each Reduce operation
    int m_band_pass_bias;           // amount to add to band-pass image

    TImage *m_base;             // original input image
    TImage *m_low_pass;         // low-pass pyramid
    TImage *m_band_pass;        // band-pass pyramid
    TImageFloat *m_low_pass_Float; 
    TImageFloat *m_band_pass_Float; 
    CVisFloatImage *m_kernel;           // filter kernel

    void ReadWriteFile(const char *filename, int write);
};

typedef CVisPyramid<CVisRGBAByteImage, CVisRGBAFloatImage>  CVisRGBAPyramid;
typedef CVisPyramid<CVisByteImage, CVisFloatImage>    CVisGrayPyramid;
typedef CVisPyramid<CVisFloatImage, CVisFloatImage>  CVisFloatPyramid;

#endif // _VIS_PYRAMID_H_
