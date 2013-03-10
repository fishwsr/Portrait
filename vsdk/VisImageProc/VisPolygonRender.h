///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisPolygonRender.h -- triangle/quad texture-mapping code
//
// DESCRIPTION
//  The CVisPolygonRender class implements polygon (triangle/quad)
//  texture-mapped (or solid color) rendering.
//
//  It is designed to handle a wide range of rendering situations,
//  including perspective/affine (u,v) coordinates, optional parallax,
//  and z-buffered rendering.  It is NOT designed to be a full
//  replacement for an OpenGL or D3D renderer.  For example, it
//  does not do any shading, or alpha interpolation for multipass rendering.
//
//  The current implementation uses a quad representation internally
//  to represent "slabs" of the clipped polygon being rendered,
//  i.e., quadtrilaterals where the top and bottom sides are x-axis aligned.
//  (An alternative internal representation we may wish to explore
//  is triangle-based, i.e., subdivide until affine triangles are obtained.)
//
//  When the texture coordinates are affine, then each quad is rendered
//  directly using incremental integer arithmetic for generating (u,v)
//  coordinates.
//
//  When texture coordinates are perspective, each quad is optionally
//  recursively subdivided both horizontally and vertically until the
//  (u,v) errors at the midpoint are below a threshold, or the quad
//  becomes too small (user-settable threshold).
//
//  In either case, the rendering area is clipped to stay within the
//  texture map (minus some borders, depending on the interpolation mode).
//  Texture maps are NOT wrapped or clamped.
//
//  This code is meant to replace the code in VisResample.cpp, and should
//  be more efficient in most cases (we hope!)
//
//
//  DESCRIPTION OF MEMBER FUNCTIONS AND PARAMETERS
//
//  void SetInputCamera(CVisTransform4x4 &xfrm);
//  CVisTransform4x4& InputCamera();
//  void SetOutputCamera(CVisTransform4x4 &xfrm);
//  CVisTransform4x4& OutputCamera();
//
//      These routines set the 4x4 projection matrices relating
//      world to output (view screen) and world to input (texture map)
//      coordinates.  If simple 2-D texturing is desired, these
//      routines should be set to the identity (default value).
//      The FOURTH row provides the "W" values used for perspective
//      division.  The third row is ignored, and is replace by a
//      plane equation if using RenderInput().
//
//  void SetTextureMode(bool b);
//  bool TextureMode();
//
//      Turn texture mapping on/off (if off, use FillColor)
//
//  void SetInterpolation(EVisInterpolation v);
//  EVisInterpolation SetInterpolation();
//
//      Set the pixel resampling mode to one of:
//          Nearest,        // nearest neighbor interpolation
//          Bilinear,       // bilinear interpolation
//          Bicubic         // bicubic interpolation
//      Mip-mapping, elliptical filtering, etc. not currently supported
//
//  void SetParallax(EVisParallax p);
//  EVisParallax SetParallax();
//
//      Set the per-pixel parallax mode:
//          None = 0,       // no per-pixel parallax
//          Fake = 1,       // apply un-warped depth correction
//          True = 2,       // pre-warp depth map for true correction
//          Flow = 3,       // pre-compute correction, not depth
//          Pass2= 4        // forward warp colors w/ parallax, then use homography
//
//  void SetCompositing(EVisComposite comp);
//  EVisComposite Compositing();
//
//      Set the compositing mode (for multi-layer effects)
//          Copy,           // copy pixels
//          Over,           // write pixels over dst
//          Under           // write pixels under dst
//
//  void SetMinSpan(int s);
//  int MinSpan();
//
//      Set minimum length of subdivided span in perspective subdivision.
//      Runs of length MinSpan or shorter are always drawn as one line.
//
//  void SetUVAccuracy(float a);
//  float UVAccuracy();
//
//      Threshold (in pixels) for determining if a span should be 
//      subdivided.  If max perspective divide error is greater than
//      threshold, then the quad is subdivided.
//
//  void RenderTriangle(CVisVector4 p[3]);
//  void RenderTriangle(CVisVector4 p[3], CVisVector4 u[3]);
//  void RenderQuad(CVisVector4 p[4]);
//  void RenderQuad(CVisVector4 p[4], CVisVector4 u[4]);
//  void RenderPolygon(int n, CVisVector4 p[]);
//  void RenderPolygon(int n, CVisVector4 p[], CVisVector4 u[]);
//
//      Render the planar convex polygon.  The p[] are 3D coordinates
//      which will be projected into the output, and optionally the
//      the input, if using texture mapping and no explicit u[] is given.
//      The p[] coordinates should be normalized (last element is 1).
//      The u[] coordinatse, if given, provide the texture-map coordinates.
//      These coordinates should have their third element set to 1 if
//      using 2-D coordinates.  The last element should always be 1.
//      If no u[] coordinates are given, the InputCamera's transform is used.
//
//  void RenderInput(CVisVector4 n);
//  void RenderInput()
//      Draw all of input onto a plane and then output.
//      If the plane isn't specified, the z = 1 plane is used.
//
//
//  These routines are specific to the pixel type:
//
//  void SetInput(ImageType &input_image);
//  ImageType& Input();
//  void SetOutput(ImageType &output_image);
//  ImageType& Output();
//
//      Set the input (texture) and output (new view) images.
//      If a soft roll-off is desired for the texture image,
//      it should be padded with 0 alpha pixels around the edge
//      (the size of the pad depends on the order of interpolant).
//
//  void SetZImage(CVisByteImage &z_image);
//  CVisByteImage& ZImage();
//
//      Set the Out-of-plane parallax (displacement) image
//
//  void SetZBuffer(CVisFloatImage &image);
//  CVisFloatImage& ZBuffer();
//  void ClearZBuffer();
//
//      Set and/or clear the Z-buffer image.  This image contains
//      the W values relative to the output image.  These are the
//      values returned by the multiplying with the FOURTH row
//      of the OutputCamera matrix, not the third row.
//
//  void SetFillColor(PixelType &val);
//  PixelType FillColor();
//
//      Set the fill "color" for solid fill
//
//  virtual void NextPseudoColor();
//  static PixelType PseudoColor(int i);
//
//      Increment the pseudo-color used for item buffer painting.
//      Call this before RenderTriangle().  If you want to know
//      what the actual color value is, use FillColor().  If you
//      want to convert the triangle id, call PseudoColor().
//
//  void Initialize(bool clear_image);
//
//      This routine MUST be called after changing parameters and before
//      calling any RenderXXX routine.  clear_image also clears the
//      Z-buffer, if applicable.
//
// SEE ALSO
//  VisPolygonRender.cpp    implementation code
//  VisImage.h              definition of image class library
//  VisGeometry.h           definition of geometric transformations
//  VisResample.h           resampling algorithm implementation
//
//  Rick's notes, Apr-May 1998.
//  Digital Image Warping, George Wolberg, IEEE Computer Society Press, 1990
//
// BUGS
//  The code will not work correctly if the polygons are not convex
//  or planar (both in X and U).
//
//  Multiband images are not currently supported.
//
// DESIGN
//  
//
// HISTORY
//  29-Apr-98  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#ifndef _VIS_POLYGON_RENDER_H_
#define _VIS_POLYGON_RENDER_H_

//
//  The CVisPolygonVertex class keeps information about polygon vertices
//  This class is used internally by CVisPolygonRender.
//

struct CVisPolygonVertex : CVisSortable<CVisPolygonVertex>
{
    double X, Y, Z; // un-normalized (homogeneous) dst location
    double U, V, W; // un-normalized (homogeneous) src location
    double d;       // distance to plane (used in clipping)
    double pad;     // pad out to 8 doubles (?)

    void SetAverage(CVisPolygonVertex& v0, CVisPolygonVertex& v1);
    void NormalizeXY();
    void NormalizeUV();
    void SplitEdge(double y, CVisPolygonVertex& prev);

    CVisPolygonVertex& operator+=(CVisPolygonVertex& v);
    CVisPolygonVertex& operator-=(CVisPolygonVertex& v);
    CVisPolygonVertex& operator*=(double s);
};

//
//  The CVisVertexChain is the clipped polygon descriptor
//  This class is used internally by CVisPolygonRender.
//

class CVisVertexChain : public std::vector<CVisPolygonVertex>
{
public:
    bool ClipToShape(bool UVW, RECT& shape);
    void NormalizeXY();
    void NormalizeUV();
    void FinalClip(RECT& XY_shape, RECT& UV_shape,
                   int normalized_UV);
    void FormSides(CVisVertexChain& left, CVisVertexChain& right);
private:
    bool ClipToPlane(bool UVW, CVisVector4& plane);
    void ClipPt(CVisPolygonVertex& v0, CVisPolygonVertex& v1);
    CVisPolygonVertex& elt(int i)   { return (*this)[i]; }
};

//
//  The base CVisPolygonRender class, independent of pixel type
//

class CVisPolygonRender
{
public:
    // Constructor and destructor
    CVisPolygonRender();
    ~CVisPolygonRender();

    // Enumerated type definitions
    enum EVisTextureMode {      // DON'T REORDER!
        SolidFill,      // fill with solid color
        SolidFillZ,     // fill with solid color with z-buffering
        Affine,         // use affine texture coordinate, no foreshortening
        ShowSubdivision,// fill each subdivided segment with different color (for debugging, only)
        Perspective,    // use approximate perspective texture mapping
        PerPixel,       // use exact perspective (per-pixel divide: slow)
        PerPixelZ       // use exact perspective with z-buffering
    };
    enum EVisInterpolation {
        Nearest,        // nearest neighbor interpolation
        Bilinear,       // bilinear interpolation
        Bicubic         // bicubic interpolation
    };
    enum EVisParallax {
        None = 0,       // no per-pixel parallax
        Fake = 1,       // apply un-warped depth correction
        True = 2,       // pre-warp depth map for true correction
        Flow = 3,       // pre-compute correction, not depth
        Pass2= 4        // forward warp colors w/ parallax, then use homography
    };
    enum EVisComposite {
        Copy,           // copy pixels
        Over,           // write pixels over dst
        Under           // write pixels under dst
    };

    // Input and output view transforms
    void SetInputCamera(CVisTransform4x4 &xfrm) { m_input_xfrm = xfrm; }
    CVisTransform4x4& InputCamera()             { return m_input_xfrm; }
    void SetOutputCamera(CVisTransform4x4 &xfrm){ m_output_xfrm = xfrm; }
    CVisTransform4x4& OutputCamera()            { return m_output_xfrm; }

    // Texture-mapping mode (on/off)
    void SetTextureMode(EVisTextureMode m)      { m_texture_mode = m; }
    EVisTextureMode TextureMode()               { return m_texture_mode; }
    
    // Pixel interpolation mode
    void SetInterpolation(EVisInterpolation v)  { m_interpolation = v; }
    EVisInterpolation SetInterpolation()        { return m_interpolation; }

    // Parallax correction (out-of-plane displacements)
    void SetParallax(EVisParallax p)            { m_parallax = p; }
    EVisParallax SetParallax()                  { return m_parallax; }

    // Compositing operation
    void SetCompositing(EVisComposite comp)     { m_comp = comp; }
    EVisComposite Compositing()                 { return m_comp; }

    // Minimum length of subdivided span and (u,v) accuracy
    void SetMinSpan(int s)                      { m_min_span = s; }
    int MinSpan()                               { return m_min_span; }
    void SetUVAccuracy(float a)                 { m_uv_accuracy = a; }
    float UVAccuracy()                          { return m_uv_accuracy; }

    // Rendering primitives for convex polygons
    void RenderTriangle(CVisVector4 p[3])   { RenderPolygon(3, p); }
    void RenderTriangle(CVisVector4 p[3], CVisVector4 u[3])
                                            { RenderPolygon(4, p, u); }
    void RenderQuad(CVisVector4 p[4])       { RenderPolygon(4, p); }
    void RenderQuad(CVisVector4 p[4], CVisVector4 u[4])
                                            { RenderPolygon(4, p, u); }
    void RenderPolygon(int n, CVisVector4 p[]);
    void RenderPolygon(int n, CVisVector4 p[], CVisVector4 u[]);

    // Rendering primitive to draw all of input onto a plane
    void RenderInput(CVisVector4 n);
    void RenderInput() { RenderInput(CVisVector4(0, 0, 1, 1)); } // z=1 plane

protected:
    // Implementation routines
    void ProjectVertex(CVisPolygonVertex &v, CVisVector4& p, CVisVector4& u);
    void SubdivideQuad(CVisPolygonVertex &v00, CVisPolygonVertex &v01,
                       CVisPolygonVertex &v10, CVisPolygonVertex &v11);

    void RenderQuad(CVisPolygonVertex &v00, CVisPolygonVertex &v01,
                    CVisPolygonVertex &v10, CVisPolygonVertex &v11);
    void RenderQuadFloat(CVisPolygonVertex &v00, CVisPolygonVertex &v01,
                         CVisPolygonVertex &v10, CVisPolygonVertex &v11);
    virtual void NextPseudoColor() = 0;

    // Virtual LineResample routines
    virtual void LineResampleFill(int x, int y, unsigned int n) = 0;
    virtual void LineResample(int x, int y, unsigned int n,
                      int u, int v, int du, int dv) = 0;
    virtual void LineResampleFloat(int x, int y, unsigned int n,
                      CVisPolygonVertex& v, CVisPolygonVertex& d) = 0;

protected:
    RECT m_output_shape;       // output (rendered) image shape
    RECT m_input_shape;        // input (texture) image shape
    CVisRect m_input_trimmed;      // trimmed input shape (adjust by interp.)

    CVisTransform4x4 m_output_xfrm; // world -> (x,y,z) mapping
    CVisTransform4x4 m_input_xfrm;  // world -> (u,v,w) mapping

    EVisTextureMode m_texture_mode;     // texture/rendering mode
    EVisInterpolation m_interpolation;  // pixel interpolation mode
    EVisParallax m_parallax;    // parallax / offset rendering mode
    EVisComposite m_comp;       // compositing operation
    int m_min_span;             // minimum length in subdividing perspective quad
    float m_uv_accuracy;        // desired (u,v) accuracy in perspective subdivide
    float m_uv_slope;           // maximum slope for two-pass re-sampler

    CVisVertexChain m_polygon;  // list of (clipped) polygon vertices
    CVisVertexChain m_left;     // left side of polygon
    CVisVertexChain m_right;    // right side of polygon
};

//
// The pixel-type specific polygon renderer
//

template <class PixelType>
class CVisPolygonRenderOf :
    public CVisPolygonRender
{
public:
#if 0   // not needed?
    // Constructor and destructor
    CVisPolygonRenderOf();
    ~CVisPolygonRenderOf();
#endif

    // Image type definition
	typedef CVisImage<PixelType> ImageType;
    typedef CVisPolygonRenderOf<PixelType> MyType;

    // Input and output images
    void SetInput(ImageType &input_image)   { m_input = input_image; }
    ImageType& Input()                      { return m_input; }
    void SetOutput(ImageType &output_image) { m_output = output_image; }
    ImageType& Output()                     { return m_output; }

    // Out-of-plane parallax (displacement) image
    void SetZImage(CVisByteImage &z_image)  { m_z_image = z_image; }
    CVisByteImage& ZImage()                 { return m_z_image; }

    // Z-buffer image (should this be float or int ???)
    void SetZBuffer(CVisFloatImage &image)  { m_z_buffer = image; }
    CVisFloatImage& ZBuffer()               { return m_z_buffer; }
    void ClearZBuffer()                     { m_z_buffer.ClearPixels(); }

    // Fill "color" for solid fill (texture-map mode == off)
    void SetFillColor(PixelType &val)       { m_fill_color = val; }
    PixelType FillColor()                   { return m_fill_color; }

    // Pseudo-color can be used for painting an item buffer
    virtual void NextPseudoColor();         // set fill color to next pseudo-color
    static PixelType PseudoColor(int i);    // return associated pseudo-color

    // Initialize internal state for rendering
    //  (call after setting or changing parameters)
    void Initialize(bool clear_image = true);

private:
    // LineResample routines
    virtual void LineResampleFill(int x, int y, unsigned int n);
    virtual void LineResample(int x, int y, unsigned int n,
                              int u, int v, int du, int dv);
    virtual void LineResampleFloat(int x, int y, unsigned int n,
                              CVisPolygonVertex& v, CVisPolygonVertex& d);
private:
    ImageType m_input;          // input (texture) image
    ImageType m_output;         // output (rendered) image
    CVisByteImage m_z_image;    // parallax image (input)
    CVisFloatImage m_z_buffer;  // z-buffer image (optional)

    PixelType m_fill_color;     // color for solid (filled) rendering
    int m_pseudo_color_id;      // next pseudocolor to be generated
    ImageType m_shifted;        // input array shifted so origin is (0,0)
#ifdef USE_ILIFFE
    PixelType **m_input_array;  // 2-D accessor array
#endif
};

#endif _VIS_POLYGON_RENDER_H_