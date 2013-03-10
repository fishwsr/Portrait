///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisSprites.h -- collection of sprites (images on planes)
//
// DESCRIPTION
//  CVisSpriteCollection is used to store and create collections of sprites,
//  e.g., multi-layer sprite compositions which can then be interactively
//  rendered.
//
//  The collections are stored as an array of sprites, each of which
//  contains an image, a geometry describing the camera location from
//  which the sprite was taken or composited, a 3D plane equation,
//  and optionally an offset image.
//
//  A SpriteComposite also has a rendered view, which is a composition
//  of the underlyting sprites, and a collection of "reference views"
//  typically corresponding to input images, or a precomputed animation.
//
// PUBLIC MEMBER FUNTIONS (METHODS)
//  CVisSpriteCollection();
//  ~CVisSpriteCollection();
//
//  int NFrames();
//
//  CVisSprite& operator[](int index);
//  CVisSprite& operator()();
//
//  int ReadCollection(const char *filename, int read_files = 0);
//  int WriteCollection(const char *filename, int write_files = 0);
//  int ReadFile(int index);
//  int WriteFile(int index);
//  int ReadFiles();
//  int WriteFiles();
//
// PARAMETERS
//  index           index of current frame
//  collection_type type of collection (sequence, composition, ...)
//  frame           a single frame (image) and ancilliary information
//  filename        collection or image filename
//  read_files      read files right now (or defer till accessed with [])
//  write_files     write files (not just the collection description)
//
// SEE ALSO
//  VisSprites.cpp      implementation code
//  VisCollection.h     simpler image collection
//  VisComposite.h      compositions / views of image collections
//
// BUGS
//   VisSpriteCollection is very similar to VisCollection<T>.
//   These two classes should be merged into a single one.
//
// DESIGN
//  
//
// HISTORY
//  11-Dec-97  Richard Szeliski (szeliski) at Microsoft
//  Created from code in VisCollection.h and VisComposite.h.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#ifndef __VIS_SPRITE_COLLECTION_H__
#define __VIS_SPRITE_COLLECTION_H__



//
//  CVisSprite class:  hold one image and its relationship to a 3D surface
//

class CVisSprite
{
public:
    // Constructors and destructor
    CVisSprite();
    ~CVisSprite();

    // Changing and querying the filename, image, and geometry
    CVisRGBAByteImage& Image()     { return m_image; }
    CVisString& Filename()          { return m_filename; }
    CVisByteImage& ZImage()         { return m_z_image; }
    CVisString& ZFilename()         { return m_z_filename; }
    CVisTransformChain& Camera()          { return m_camera; }
    CVisVector4& PlaneEqn()         { return m_plane_eqn; }
    float& ZScale()                 { return m_z_scale; }
    int ZMin()                      { return m_zmin; }
    int ZMax()                      { return m_zmax; }
	CVisRect &Shape()					{ return m_image_shape; }

    // Mask used for faster warp and composite (not currently used)
    CVisByteImage& Mask()           { return m_opaque; }
    void RecomputeMask(int totally_opaque, int neighbors);

    // Z map range computation
    void ComputeZMinMax();

    // Reading and writing files
    void ReadImage (int invisible_pixel = -1);
    void WriteImage(int invisible_pixel = -1);

    // Self-describing input/output format
	// UNDONE:  Change enum names to use the eft prefix.
    enum FieldType { Name, FilenamE, ImageShape, ZFilenamE, ZScalE,
                        CamerA, Plane, eftEnd };
    const char *ReadWriteField(CVisSDStream& s, int field_id);

	void BuildPropList(CVisPropList& refproplist);
	void ReadSpriteValue(CVisSDIStream& refsdistream);
	void WriteSpriteValue(CVisSDOStream& refsdostream);

protected:
    CVisRGBAByteImage m_image; // sprite image (w/ alpha channel)
    CVisRect m_image_shape;   // shape of sprite
    CVisString m_filename;      // sprite image filename
    CVisByteImage m_z_image;    // offset (displacement) image (scaled)
    CVisString m_z_filename;    // offset image filename
    float m_z_scale;            // offset scale (to world coordinates)
    int m_zmin, m_zmax;         // min/max z values
    CVisByteImage m_z_warped;   // forward warped z image
    CVisTransformChain m_camera;      // camera transform chain (world->screen)
    CVisVector4 m_plane_eqn;    // 3D plane equation

    CVisByteImage m_opaque;     // bit image for opacity-based acceleration
    CVisRGBAPyramid *m_pyramid; // MIP map (optional)
};


//
//  SpriteCollection class for holding multiple sprites
//

typedef std::vector< CVisSprite, std::allocator< CVisSprite > > CVisSpriteL;
#if (_MSC_VER >= 1100)	// VC 5.0 or greater
inline bool operator< (const CVisSprite& a, const CVisSprite& b) { return 0; }
inline bool operator==(const CVisSprite& a, const CVisSprite& b) { return 0; }
#endif // (_MSC_VER >= 1100)

class CVisSpriteCollection : public CVisSpriteL
{
public:
    // Constructor and destructor
    CVisSpriteCollection();
    ~CVisSpriteCollection();

    // Query and setting functions
    int NFrames() const             { return size(); }
    int IsEmpty() const             { return empty(); }
    int InvisiblePixel() const      { return m_invisible_pixel; }
    void SetInvisiblePixel(int v)   { m_invisible_pixel = v; }

    // Collection operations (see also File I/O below)
    void Insert(CVisSprite &sprite, int index = -1) {
        if (index < 0)
            push_back(sprite);
        else
            insert(&(*this)[index], sprite);
    }
    void Insert(const char *filename);
    void Insert(const char *stem, const char *extension,
                int sequence_digits = 3);
    void Delete(int index)  { erase(&(*this)[index]); }
    void DeleteAll()        { clear(); }

    // Accessing individual frames
    int CurrentFrame()             { return m_current; }
    void SetCurrentFrame(int i)    { m_current = i; }
    void IncrementCurrent(int delta);  // -2: home, +2: end
	CVisSprite& Sprite(int index)   { return (*this)[index]; }
	CVisSprite& Sprite()            { return Sprite(CurrentFrame()); }

    // Reading and writing files
    void ReadImages();
    void WriteImages();
    void SetWorkingDir(const char *filename);
    void ReNameSequence(const char *stem, const char *extension,
                        int sequence_digits = 3);

    // Self-describing input/output format
	// UNDONE:  Change enum names to use the eft prefix.
    enum FieldType { Name, BaseCVisSpriteL, InvisiblePixeL, eftEnd };
    const char *ReadWriteField(CVisSDStream& s, int field_id);

protected:
    CVisString m_working_dir;   // directory containing collection files
    int m_invisible_pixel;      // value of invisible pixel (-1: none, -2: write alpha)
    int m_current;              // index of current sprite (for display)
};


//
//  SpriteComposite class for rendering multiple sprites
//

typedef std::vector<CVisTransformChain, std::allocator<CVisTransformChain> > CVisGeometryL;

// Parallax (depth correction) options:
enum TVisParallax {
    Vis_Parallax_None = 0,  // no per-pixel parallax
    Vis_Parallax_Fake = 1,  // apply un-warped depth correction
    Vis_Parallax_True = 2,  // pre-warp depth map for true correction
    Vis_Parallax_Flow = 3,  // pre-compute correction, not depth
    Vis_Parallax_2Pass= 4   // forward warp colors w/ parallax, then use homography
};

class CVisSpriteComposite : public CVisSpriteCollection,
    public CVisSprite
{
public:
    // Constructor and destructor
    CVisSpriteComposite();
    ~CVisSpriteComposite();

    // Rendering options (affect warp / resample)
    TVisInterpolation Resampling()  { return m_warp_interpolation; }
    void SetResampling(TVisInterpolation i)  { m_warp_interpolation = i; }
    TVisInterpolation ZoomInterpolation()   { return m_zoom_interpolation; }
    void SetZoomInterpolation(TVisInterpolation i)  { m_zoom_interpolation = i; }
    int AffineSprite()              { return m_affine; }
    void SetAffineSprite(int b)     { m_affine = b; Invalidate(); }
    float ResampleError()           { return VisResampleMaxUVError; }
    void SetResampleError(float e)  { VisResampleMaxUVError = e; Invalidate(); }
    int BackgroundGrayLevel()           { return m_background_gray_level; }
    void SetBackgroundGrayLevel(int l)  { m_background_gray_level = l; }
    int FastRender()                { return m_fast_render; }
    void SetFastRender(int f)       { m_fast_render = f; Invalidate(); }
    TVisParallax Parallax()         { return m_parallax; }
    void SetParallax(TVisParallax p) { m_parallax = p; }
    int ZGapSize()                  { return m_gap_size; }
    void SetZGapSize(int s)         { m_gap_size = s; }
    int SplatSize()                 { return m_splat_size; }
    void SetSplatSize(int s)        { m_splat_size = s; }
    bool RenderDepth()              { return m_render_depth; }
    void SetRenderDepth(bool b)     { m_render_depth = b; }

    // Current view manipulation
    double Zoom()                   { return m_zoom; }
    void SetZoom(double zoom)       { m_zoom = zoom; Invalidate(); }
    CVisTransformChain& Camera()          { return m_camera; }
    void SetCamera(const CVisTransformChain& geometry);
    CVisShape Shape()               { return m_image.Shape(); }
    void SetShape(CVisShape sh);    // re-allocates m_image
    int SingleSprite()              { return m_single_sprite; }
    void SetSingleSprite(int l)
        { if (m_single_sprite != l) Invalidate(); m_single_sprite = l; }

    // View list (pose and precomputed image) manipulation
    CVisSpriteCollection& ViewList()   { return m_view_list; }
    int NViews()                    { return ViewList().size(); }
    int ViewFrame()                 { return ViewList().CurrentFrame(); }
    void SetViewFrame(int v);      // reset ViewGeometry

    // Layer invalidation (when geometry has changed ...)
    void Invalidate(int mask = 0)   { m_valid = 0; }


    // Layer geometry and warped shape
    CVisTransform4x4 SpriteTransform(int i, CVisVector4& parallax,
                                     CVisVector4& epipole);
    CVisShape SpriteShape(int i);   // shape of sprite in composition
    void SpriteCenter(int i, int center[2]);

    // Compute and return the composition
    CVisRGBAByteImage& Composite();// composition of sprites (new view)
    CVisShape CompositeShape()      { return Composite().Shape(); }
    void FormComposite();           // recompute composition
    void RecomputeMasks();

    // Reading and writing files (override VisSpriteCollection)
    void ReadImages (int read_sprites  = 1, int read_views  = 1);
    void WriteImages(int write_sprites = 0, int write_views = 1);
    void SetWorkingDir(const char *filename);
    void ReNameViewSequence(const char *stem, const char *extension,
                           int sequence_digits = 3);

    // Self-describing input/output format
	// UNDONE:  Change enum names to use the eft prefix.
    enum FieldType { Name, BaseSpriteCollection, ViewLisT,
                        AffinE, ResErr, WarpInt, ZoomInt, Bkgd, ZooM, eftEnd };
    const char *ReadWriteField(CVisSDStream& s, int field_id);

protected:
    CVisSpriteCollection m_view_list; // list of default viewpoints
    int m_valid;                    // composite is valid

    int m_fast_render;              // warp and composite simultaneously
    TVisParallax m_parallax;        // out-of-plane parallax rendering technique
    bool m_render_depth;            // render depth offset instead of color image
    int m_gap_size;                 // max gap size to fill in fwd warp
    int m_splat_size;               // size of splat used in forward mapping
    int m_affine;                   // use affine warp (1: per-scanline, 2: global)
    double m_resample_error;        // max projective resampling error
    TVisInterpolation m_warp_interpolation; // interpolation on resampling
    TVisInterpolation m_zoom_interpolation; // interpolation on zooming
    int m_background_gray_level;    // gray level for background
    double m_zoom;                  // zoom factor
    int m_single_sprite;            // only render this sprite

    int m_use_polygon_renderer;     // use the new scanline converter
    CVisPolygonRenderOf<CVisRGBABytePixel>
        m_renderer;                 // new polygon renderer
};

// Old self-describing stream functions

CVisSDStream& operator<<(CVisSDStream& s, CVisSprite& o);
CVisSDStream& operator<<(CVisSDStream& s, CVisSpriteCollection& o);
CVisSDStream& operator<<(CVisSDStream& s, CVisSpriteComposite& o);

// New SD Stream stuff

//VIS_SD_CLASS_WITH_IO_PL(CVisSprite)
VIS_PROP_LIST_CLASS_WITH_IO_PL(CVisSprite)
//VIS_SDO_CLASS_WITH_PL(CVisSprite)
//VIS_SDI_CLASS_WITH_PL(CVisSprite)

VIS_SDO_FIND_TYPES_PL(CVisSprite)
inline void VisSDWriteValue(CVisSDOStream& refsdostream,
							CVisSprite const& refobj)
		{ const_cast<CVisSprite&>(refobj).WriteSpriteValue(refsdostream); }
VIS_SDO_WRITE_OBJ_DEFAULT(CVisSprite)
VIS_SDO_WRITE_OP_DEFAULT(CVisSprite)

VIS_SDI_FIND_TYPES_PL(CVisSprite)
inline void VisSDReadValue(CVisSDIStream& refsdistream,	
		CVisSprite& refobj)
	{ refobj.ReadSpriteValue(refsdistream); }
VIS_SDI_READ_OBJ_DEFAULT(CVisSprite)
VIS_SDI_READ_OP_DEFAULT(CVisSprite)

#endif // __VIS_SPRITE_COLLECTION_H__
