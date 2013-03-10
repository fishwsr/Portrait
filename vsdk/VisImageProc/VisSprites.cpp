///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisSprites.cpp -- collection of sprites (images on planes)
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
// SEE ALSO
//  VisSprites.h        more detailed description
//
// BUGS
//   
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

#include "VisImageProcPch.h"
#include "VisImageProc.h"

#include <direct.h>

//
//  CVisSprite class
//

CVisSprite::CVisSprite() : 
    m_z_scale(0.0), m_plane_eqn(0, 0, 1, 1), m_pyramid(0)
{
}

CVisSprite::~CVisSprite()
{
    delete m_pyramid;
}

// Mask used for faster warp and composite

void CVisSprite::RecomputeMask(int totally_opaque, int neighbors)
{
    // Create an image 1/8 as wide (using bit vector)
    m_image_shape = m_image.Shape();
    CVisShape sh = m_image_shape;
    CVisShape sh8;
	sh8.left = (sh.left-(sh.left & 7))/8;
	sh8.top = sh.top;
	sh8.right = sh8.left+sh.Width()/8+3;
	sh8.bottom = sh.bottom;

    if (m_opaque.Shape() != sh8)
        m_opaque.Allocate(sh8);
    m_opaque.ClearPixels();
    if (totally_opaque < 0)
        return;     // no pre-computation

    // Compute the opaque pixels
    int threshold = (totally_opaque) ? 255 : 1;
    for (int r = m_image.Top(); r <= m_image.Bottom() - 1; r++) {
        int scol = m_image.Left(), ecol = m_image.Right() - 1;
        int ecol8 = (m_opaque.Right() )*8;
        int i, j = (scol - (scol & 7))/8, k = scol - j*8, mw = 0;
        CVisRGBABytePixel *c = m_image.PtrToFirstPixelInRow(r);
        unsigned char *m = m_opaque.PtrToFirstPixelInRow(r);
        for (i = scol; i < ecol8; i++, k++, c++) {
            int b = (i <= ecol && c->A() >= threshold);
            mw = (mw >> 1) | (b << 7);
            if (k == 7) {
                *m++ = mw;
                k = -1;
            }
        }
    }

    // Or with neighboring values, in case interpolation is being used
    for (int n = 0; n < neighbors; n++) {
        for (int r = m_image.Top(); r < m_image.Bottom() - 1; r++) {
            unsigned char *m = m_opaque.PtrToFirstPixelInRow(r);
            int i, ncols = m_opaque.Width()-1;
            for (i = 0; i < ncols; i++)
                m[i] |= ((int(m[i]) >> 1) | (int(m[i+1]) << 7));
        }
    }
}

void CVisSprite::ComputeZMinMax()
{
    int zmin = 255, zmax = 0;
    for (int r = m_z_image.Top(); r <= m_z_image.Bottom() - 1; r++) {
        CVisRGBABytePixel *sp =   m_image.PtrToFirstPixelInRow(r);
        BYTE               *dp = m_z_image.PtrToFirstPixelInRow(r);
        int ncols = m_z_image.Width();
        for (int c = 0; c < ncols; c++) {
            if (sp[c].A() > 0) {
                if (zmin > dp[c])
                    zmin = dp[c];
                if (zmax < dp[c])
                    zmax = dp[c];
                if (dp[c] == 0)
                    dp[c] = 1;  // use 0 to indicate invisible
            } else
                dp[c] = 0;      // use 0 to indicate invisible
        }
    }
    m_zmin = zmin, m_zmax = zmax;
}

// Reading and writing files

void CVisSprite::ReadImage(int invisible_pixel)
{
    if (m_filename.size() > 0) {
        CVisDiagnostics::ProgressReport("Reading %s", m_filename);
        if (m_image.Shape() != CVisShape(m_image_shape))
            m_image.Allocate(CVisShape(m_image_shape));
        m_image.ReadFile(m_filename.c_str());
        if (invisible_pixel >= 0)
            VisMakePixelsInvisible(m_image, invisible_pixel);
    }
#ifdef THROW_ERROR_ON_NO_FILENAME
    else
        throw CVisFileIOError("", "no filename specified",
            VIS_ERR_C_NO_FILENAME, "CVisSprite::ReadImage()", __FILE__, __LINE__);
#endif
    if (m_z_filename.size() > 0) {
        CVisDiagnostics::ProgressReport("Reading %s", m_z_filename);
        if (m_z_image.Shape() != CVisShape(m_image_shape))
            m_z_image.Allocate(CVisShape(m_image_shape));
        m_z_image.ReadFile(m_z_filename.c_str());
        ComputeZMinMax();
    }
}

void CVisSprite::WriteImage(int invisible_pixel)
{
    if (m_filename.size() > 0) {
        CVisDiagnostics::ProgressReport("Writing %s", m_filename);
        CVisRGBAByteImage img = m_image;
        if (invisible_pixel >= 0) {
            // Back the image with a non-zero color (e.g., white)
            img.Allocate(img.Shape());
            m_image.CopyPixelsTo(img);
            VisBackInvisiblePixels(img, invisible_pixel);
        }

        // Write with a file descriptor, so that we can set has_alpha_channel
        static SVisFileDescriptor blank;
        SVisFileDescriptor fd = blank;
        fd.filename = m_filename.c_str();
        fd.has_alpha_channel =
            (invisible_pixel == -2);  // special code, means "write alpha"
        img.WriteFile(fd);
    }
#ifdef THROW_ERROR_ON_NO_FILENAME
    else
        throw CVisFileIOError("", "no filename specified",
            VIS_ERR_C_NO_FILENAME, "CVisSprite::WriteImage()", __FILE__, __LINE__);
#endif
    if (m_z_filename.size() > 0) {
        CVisDiagnostics::ProgressReport("Writing %s", m_z_filename);
        m_z_image.WriteFile(m_z_filename.c_str());
    }
}


//
//  CVisSpriteCollection class
//

CVisSpriteCollection ::CVisSpriteCollection () : 
    m_invisible_pixel(-2)
{
    m_invisible_pixel = -2;     // value of invisible pixel (-1: none, -2: write alpha)
    m_current = 0;              // index of current sprite (for display)
}

CVisSpriteCollection ::~CVisSpriteCollection ()
{
}

void CVisSpriteCollection::ReadImages()
{
    for (int i = 0; i < NFrames(); i++)
        Sprite(i).ReadImage(m_invisible_pixel);
}

void CVisSpriteCollection::WriteImages()
{
    for (int i = 0; i < NFrames(); i++)
        Sprite(i).WriteImage(m_invisible_pixel);
}

void CVisSpriteCollection::Insert(const char *filename)
{
    CVisSprite s;
    s.Filename() = filename;
    s.ReadImage();
    push_back(s);
}

void CVisSpriteCollection::Insert(const char *stem, 
                                  const char *extension,
                                  int sequence_digits)
{
    // Insert copy of last frame, but with new filename
    CVisSprite s;
    char format[128], filename[1024];
    sprintf(format, "%%s%%0%dd.%%s", sequence_digits);
    sprintf(filename, format, stem, NFrames(), extension);
    s.Filename() = filename;
    CVisShape shape = (NFrames() > 0) ? Sprite(NFrames()-1).Image().Shape() : 
        CVisShape(0, 0, 300, 300);
    s.Image()    = CVisRGBAByteImage(shape);
    push_back(s);
}

// This code taken from VisCollection.cpp:
void CVisSpriteCollection::SetWorkingDir(const char *filename)
{
    m_working_dir.erase();
    char *e1 = strrchr(filename, '/'), *e2 = strrchr(filename, '\\');
    int i = (e2) ? e2 - filename : (e1) ? e1 - filename : -1;
    m_working_dir = std::string(filename).substr(0, i);
    _chdir(m_working_dir.c_str());
}

void CVisSpriteCollection::ReNameSequence(const char *stem,
                                           const char *extension,
                                           int sequence_digits)
{
    char format[128], filename[1024];
    sprintf(format, "%%s%%0%dd.%%s", sequence_digits);
    for (int i = 0; i < NFrames(); i++) {
        sprintf(filename, format, stem, i, extension);
        Sprite(i).Filename() = filename;
    }
}

void CVisSpriteCollection::IncrementCurrent(int delta)
{
	switch (delta) {
	case -2: m_current = 0; break;
	case -1: m_current = __max(0, m_current-1); break;
	case  1: m_current = __min(NFrames()-1, m_current+1); break;
	case  2: m_current = NFrames()-1; break;
	default: m_current = m_current;   // do nothing
	}
}

//
// CVisSpriteComposite class
//

CVisSpriteComposite ::CVisSpriteComposite ()
{
    m_fast_render = 1;              // warp and composite simultaneously
    m_parallax = Vis_Parallax_None; // out-of-plane parallax rendering technique
    m_render_depth = 0;             // render depth offset instead of color image
    m_gap_size = 1;                 // max gap size to fill in fwd warp
    m_splat_size = 1;               // size of splat used in forward mapping
    m_affine = 0;                   // use affine warp (approximation)
    m_resample_error = 0.125;       // max projective resampling error
    m_warp_interpolation = VIS_INTERP_LINEAR; // interpolation on resampling
    m_zoom_interpolation = VIS_INTERP_LINEAR; // interpolation on zooming
    m_background_gray_level = 255;  // gray level for background
    m_zoom = 1.0;                   // zoom factor
    m_single_sprite = -1;           // only render this sprite
    m_use_polygon_renderer = 0;     // use the new scanline converter
}

CVisSpriteComposite ::~CVisSpriteComposite ()
{
}

// Reading and writing files (override VisSpriteCollection)

void CVisSpriteComposite::ReadImages(int read_sprites, int read_views)
{
    if (read_sprites)
        CVisSpriteCollection::ReadImages();
    if (read_views)
        ViewList().ReadImages();
    RecomputeMasks();
}

void CVisSpriteComposite::WriteImages(int write_sprites, int write_views)
{
    if (write_sprites)
        CVisSpriteCollection::WriteImages();
    if (write_views)
        ViewList().WriteImages();
}

void CVisSpriteComposite::SetWorkingDir(const char *filename)
{
    CVisSpriteCollection::SetWorkingDir(filename);
    ViewList().SetWorkingDir(filename);
}

void CVisSpriteComposite::ReNameViewSequence(const char *stem,
                                             const char *extension,
                                             int sequence_digits)
{
    // Only rename the view sequence
    ViewList().ReNameSequence(stem, extension, sequence_digits);
}

// Set up geometry and transformations

void CVisSpriteComposite::SetCamera(const CVisTransformChain& geometry)
{
    m_camera = geometry;
    Invalidate();
}

void CVisSpriteComposite::SetShape(CVisShape sh)
{
    m_image.Allocate(CVisShape(sh));
    Invalidate();
}

void CVisSpriteComposite::SetViewFrame(int v)
{
    ViewList().SetCurrentFrame(v);
    SetCamera(ViewList().Sprite().Camera());
    CVisShape curr_shape = ViewList().Sprite().Image().Shape();
    if (m_image.Shape() != curr_shape)
        m_image.Allocate(curr_shape);
    Invalidate();
}

// Layer geometry and warped shape

CVisTransform4x4 CVisSpriteComposite::SpriteTransform(int i, CVisVector4& parallax,
                                                      CVisVector4& epipole)
{
    // Compute homography from camera geometries and plane equation
    CVisSprite& s = Sprite(i);
    CVisTransform4x4 M0 = s.Camera().CompleteTransform();
    CVisVector4& n = s.PlaneEqn();
    double z_scale_inv = (s.ZScale() != 0.0) ? 1.0 / s.ZScale() : 1.0;
    for (int j = 0; j < 4; j++)
        M0[2][j] = z_scale_inv * n[j];
    CVisTransform4x4 P0 = M0.Inverted();
    CVisTransform4x4 M = m_camera.CompleteTransform();
    CVisTransform4x4 H1 = M * P0;
#ifdef NOT_NEEDED   // I hope
    H1 = VisMatrixNormalize(H1);
#endif

    // Affine approximation:  recenter around center, modify bottom row
    if (m_affine > 1) {
        CVisShape b = s.Image().Shape();
        double cx = 0.5*(b.Left() + b.Right());
        double cy = 0.5*(b.Top() + b.Bottom());
        CVisTransform4x4 A1 = H1.Translated(-cx, -cy);
        for (j = 0; j < 4; j++)
            A1[3][j] = (j == 3);
        H1 = A1.Translated(cx, cy);
    }

    // Extract the epipole from H1 matrix
    CVisVector4 e1(H1[0][2], H1[1][2], 0.0, H1[3][2]);
    
    // Project center point and displaced center point to the new view
    CVisRect r = s.Image().Shape();
    CVisVector4 c0((r.left+r.right )/2, (r.top+r.bottom )/2, 1);
    CVisVector4 c1 = H1.ProjectVector(c0);  // where in screen
    c1[2] = 0;      // put it on the plane
    
    // Reset the third row and column to identity
    for (j = 0; j < 4; j++)
        H1[j][2] = H1[2][j] = (j == 2);

    // Compute the depth-based parallax vector
	CVisTransform4x4 H1i = H1.Inverted();
    CVisVector4 c2 = H1i.ProjectVector(c1);    // in sprite
    parallax = CVisVector4(c2[0]-c0[0], c2[1]-c0[1], 0, 1);

    // Compute the epipole corrected by Homography (pre-shift parallax)
    CVisVector4 e2 = H1i * e1;    // true epipole
    epipole = CVisVector4(e2[0], e2[1], e2[3], 1.0);

    return H1;
}

CVisShape CVisSpriteComposite::SpriteShape(int i)
{
    CVisSprite& s = Sprite(i);
    CVisVector4 p, e;
    CVisTransform4x4 t = SpriteTransform(i, p, e);
    CVisShape shape = VisResampleShape(s.Image().Shape(), t);
    return shape;
}

void CVisSpriteComposite::SpriteCenter(int i, int center[2])
{
}

// Compute and return the composition

void CVisSpriteComposite::RecomputeMasks()
{
    for (int l = 0; l < NFrames(); l++)
        Sprite(l).RecomputeMask(0, 1);  // for possible bilinear interpolation
}

CVisRGBAByteImage& CVisSpriteComposite::Composite()
{
    // Composition of sprites (new view)
    if (! m_valid)
        FormComposite();
    m_valid = 1;
    return m_image;
}

struct VisGrayToColor {
    void operator()(BYTE& in, CVisRGBABytePixel& out)
    { out = CVisRGBABytePixel(in, in, in); }
};

void CVisSpriteComposite::FormComposite()
{
    if (m_image.Width() == 0 || m_image.Height() == 0)
        return;

    // Clear the mask for possible fast rendering
    if (m_fast_render) {
        RecomputeMask(-1, 0);
        m_opaque.ClearPixels();
    }

    // Reset the resampling error
    if (m_affine > 0)
        VisResampleMaxUVError = 10.0;
    else
        VisResampleMaxUVError = m_resample_error;
    int true_perspective = (m_affine < 0);
    int parallax_flow = (m_parallax == Vis_Parallax_Flow);

    // Setup the new renderer
    m_renderer.SetInterpolation(
        (m_warp_interpolation == VIS_INTERP_LINEAR) ?
        CVisPolygonRender::Bilinear : CVisPolygonRender::Nearest);
    m_renderer.SetTextureMode(m_affine ?
        CVisPolygonRender::Affine : CVisPolygonRender::Perspective);
    m_renderer.SetUVAccuracy(m_resample_error);

    // Recompute composition, front to back
    m_image.ClearPixels();
    CVisShape comp_shape = m_image.Shape();
    for (int l = NFrames()-1; l >= 0; l--) {
        CVisSprite& s = Sprite(l);
        CVisRGBAByteImage &simg = s.Image();

        // Optionally render only single sprite
        if (m_single_sprite >= 0 && l != m_single_sprite)
            continue;

        // Compute the layer transformation and shape
        CVisVector4 parallax, epipole;
        CVisTransform4x4 t = SpriteTransform(l, parallax, epipole);
        CVisShape layr_shape = VisResampleShape(simg.Shape(), t);
        CVisShape both_shape = (comp_shape & layr_shape);
        if (both_shape.Width() <= 0 || both_shape.Height() <= 0)
            continue;
#if 0
        // Don't bother displaying, since clashes with frame rate...
        CVisDiagnostics::ProgressReport(" layer %d has size %d x %d",
            l, both_shape.Width(), both_shape.Height());
#endif
        CVisRGBAByteImage comp_trim = m_image.SubImage(both_shape);
        CVisTransform4x4 ti = t.Inverted();

        // Out of plane parallax
        CVisByteImage *depth = (m_parallax != Vis_Parallax_None && s.ZImage().Height() > 0) ?
            &s.ZImage() : 0;
        double u_p = (true_perspective) ? -epipole[0] : -parallax[0];
        double v_p = (true_perspective) ? -epipole[1] : -parallax[1];
        double w_p = (true_perspective) ? -epipole[2] : 0.0;

        // Forward map the depth map
        CVisByteImage warped_depth;
        if (m_parallax > Vis_Parallax_Fake && depth) {
#ifdef TAKEOUT
    SVisFileDescriptor filedescriptorT0;
	ZeroMemory(&filedescriptorT0, sizeof(SVisFileDescriptor));

	char szFilenameT0[] = "c:\\Projects\\TmpA.bmp";
	szFilenameT0[15] += l;
	filedescriptorT0.filename = szFilenameT0;

//	filedescriptorT0.has_alpha_channel = 1;

    CVisByteImage(*depth).WriteFile(filedescriptorT0);
	szFilenameT0[15] -= l;
#endif // TAKEOUT
            warped_depth = VisForwardWarpDepthMap(*depth, m_z_warped,
                -u_p, -v_p, -w_p, s.ZMin(), s.ZMax(), 
                true_perspective, parallax_flow, m_gap_size, m_splat_size);
            depth = &warped_depth;
#ifdef TAKEOUT
    SVisFileDescriptor filedescriptorT1;
	ZeroMemory(&filedescriptorT1, sizeof(SVisFileDescriptor));

	char szFilenameT1[] = "c:\\Projects\\Tmp0.bmp";
	szFilenameT1[15] += l;
	filedescriptorT1.filename = szFilenameT1;

//	filedescriptorT1.has_alpha_channel = 1;

    CVisByteImage(*depth).WriteFile(filedescriptorT1);
	szFilenameT1[15] -= l;
#endif // TAKEOUT
        }

        // Render from depth map instead of color image
        //  this is for debugging only...
        CVisRGBAByteImage gimg;
        if (m_render_depth && depth) {
            // Convert from gray-level to color
            gimg.Allocate(depth->Shape());
            VisMap2(VisGrayToColor(), *depth, gimg);
            depth = 0;      // disable final parallax-based warp
            ti[1][0] += 1.0e-20;    // tweak so it isn't a zoom!
#ifdef DEBUG
            gimg.WriteFile("tmpZ.tga");
#endif
        }
        CVisRGBAByteImage& simg2 = (m_render_depth && gimg.Height() > 0) ?
                gimg : simg;

        // Do a joint resampling / compositing
        if (m_fast_render) {
            VisResample(simg2, comp_trim, ti, m_warp_interpolation,
                        0, (l < NFrames()-1), true_perspective, depth, 
                        parallax_flow, u_p, v_p, w_p);
        }

        // Do a separate resampling and over computation
        else {
            CVisRGBAByteImage layer(both_shape);
            if (m_use_polygon_renderer) {
                m_renderer.SetInput(simg2);
                m_renderer.SetOutput(layer);
                // m_renderer.SetZImage(depth);
#ifdef TRIANGLE_TEST
                m_renderer.SetOutputCamera(ti.Inverted());   // or is it t?
                m_renderer.SetTextureMode(CVisPolygonRender::SolidFillZ);
                CVisVector4 p0(105.2, 105.3, 1), p1(170.2, 105.4, 1),
                            p2(105.3, 180.3, 1), p3(171.7, 192.9, 1);
                CVisVector4 p[4] = {p0, p1, p2, p3};
                m_renderer.Initialize(true);
                m_renderer.NextPseudoColor();
                m_renderer.RenderTriangle(&p[0]);
                m_renderer.NextPseudoColor();
                m_renderer.RenderTriangle(&p[1]);
#else
                m_renderer.SetInputCamera(ti);   // or is it t?
                m_renderer.Initialize(true);
                m_renderer.RenderInput();
#endif
            } else
                
            VisResample(simg2, layer, ti, m_warp_interpolation,
                1, 0, true_perspective, depth, 
                parallax_flow, u_p, v_p, w_p);
            VisComposite(comp_trim, layer, comp_trim);
        }
    }

    // Fill in the background
    VisBackInvisiblePixels(m_image, m_background_gray_level);

    m_valid = 1;
}

// Self-describing stream functions

const char *CVisSprite::ReadWriteField(CVisSDStream& s, int field_id)
{
    switch (FieldType(field_id)) {
    case Name:
        s.Comment("CVisSprite");
        return "class CVisSprite {";
    case FilenamE:
        s.Comment("\nsprite filename");
        s << m_filename;
        return "CVisString m_filename;      // sprite image filename";
    case ImageShape:
        s.Comment("\nsprite shape");
        m_image_shape = m_image.Shape();    // just to be on the safe side...
        s << m_image_shape;
        return "CVisRegion m_image_shape;   // shape of sprite";
    case ZFilenamE:
        s.Comment("\noptional z offset file and scale");
        s << m_z_filename;
        return "CVisString m_z_filename;    // offset image filename";
    case ZScalE:
        s << m_z_scale;
        return "float m_z_scale;            // offset scale (to world coordinates)";
    case CamerA:
        s.Comment("\nsprite camera");
        s << m_camera;
        return "CVisGeometry m_camera;      // camera transform chain (world->screen)";
    case Plane:
        s.Comment("\nsprite plane equation");
        s << m_plane_eqn;
        return "CVisVector4 m_plane_eqn;    // 3D plane equation";
    }
    return 0;
}
/*
CVisSDObject<CVisSprite> g_visspriteExemplar;
*/
CVisSDObject<CVisSprite>& CVisSDObject<CVisSprite>::Exemplar(void)
{
	// Global variable defined in ..\VisImageProc\VisSDStreamExemplars.cpp
	extern CVisSDObject< CVisSprite >			g_visspriteExemplar;

	return g_visspriteExemplar; 
}
CVisSDStream& operator<<(CVisSDStream& s, CVisSprite& o) {
    return VisSDReadWriteClass(s, o); }

CVisSDStream& operator<<(CVisSDStream& s, CVisSpriteL& o) {
    return VisSDReadWriteContainer(s, o, CVisSprite());
}

const char *CVisSpriteCollection::ReadWriteField(CVisSDStream& s, int field_id)
{
    switch (FieldType(field_id)) {
    case Name:
        s.Comment("CVisSpriteCollection");
        return "class CVisSpriteCollection :";
    case BaseCVisSpriteL:
        s << (*(CVisSpriteL *) this);
        return "public CVisSpriteL {        // base class";
    case InvisiblePixeL:
        s.Comment("\n invisible pixel value");
        s << m_invisible_pixel;
        return "int m_invisible_pixel;      // value of invisible pixel (-1: none, -2: write alpha)";
    }
    return 0;
}
/*
CVisSDObject<CVisSpriteCollection> g_visspritecollectionExemplar;
*/
CVisSDObject<CVisSpriteCollection>& CVisSDObject<CVisSpriteCollection>::Exemplar(void)
{
	// Global variable defined in ..\VisImageProc\VisSDStreamExemplars.cpp
	extern CVisSDObject< CVisSpriteCollection > g_visspritecollectionExemplar;
	return g_visspritecollectionExemplar; 
}
CVisSDStream& operator<<(CVisSDStream& s, CVisSpriteCollection& o) {
    return VisSDReadWriteClass(s, o); }

const char *CVisSpriteComposite::ReadWriteField(CVisSDStream& s, int field_id)
{
    switch (FieldType(field_id)) {
    case Name:
        s.Comment("CVisSpriteComposite");
        return "class CVisSpriteComposite :";
    case BaseSpriteCollection:
        s.Comment("\nsprite collection");
        s << (*(CVisSpriteCollection *) this);
        return "public CVisSpriteCollection  {  // base class";
    case ViewLisT:
        s.Comment("\nview collection");
        s << m_view_list;
        return "CVisSpriteCollection m_view;    // list of default viewpoints";
    case AffinE:
        s.Comment("\nrendering parameters (see class description above)");
        s << m_affine;
        return "int m_affine;                   // use affine warp (1: per-scanline, 2: global)";
    case ResErr:
        s << m_resample_error;
        return "double m_resample_error;        // max projective resampling error";
    case WarpInt:
        { int i = int(m_warp_interpolation);
          s << i;
          m_warp_interpolation = TVisInterpolation(i);
        }
        return "TVisInterpolation m_warp_interpolation; // interpolation on resampling";
    case ZoomInt:
        { int i = int(m_zoom_interpolation);
          s << i;
          m_zoom_interpolation = TVisInterpolation(i);
        }
        return "TVisInterpolation m_zoom_interpolation; // interpolation on zooming";
    case Bkgd:
        s << m_background_gray_level;
        return "int m_background_gray_level;    // gray level for background";
    case ZooM:
        s << m_zoom;
        return "double m_zoom;                  // zoom factor";
    }
    return 0;
}

/*
CVisSDObject<CVisSpriteComposite> g_visspritecompositeExemplar;
*/
CVisSDObject<CVisSpriteComposite>& CVisSDObject<CVisSpriteComposite>::Exemplar(void)
{
	// Global variable defined in ..\VisImageProc\VisSDStreamExemplars.cpp
	extern CVisSDObject< CVisSpriteComposite > g_visspritecompositeExemplar;
	return g_visspritecompositeExemplar; 
}
CVisSDStream& operator<<(CVisSDStream& s, CVisSpriteComposite& o) {
    return VisSDReadWriteClass(s, o); }



// New SD Stream I/O functions

void CVisSprite::BuildPropList(CVisPropList& refproplist)
{
	// try to write out only the minimal amount of information required
	// to construct the sprite
//	refproplist.AddPropReference("m_image", m_image, false);
	refproplist.AddPropReference("m_image_shape", m_image_shape, false);
    refproplist.AddPropReference("m_filename", m_filename, false);
//	refproplist.AddPropReference("m_z_image", m_z_image, false);
    refproplist.AddPropReference("m_z_filename", m_z_filename, false);
    refproplist.AddPropReference("m_z_scale", m_z_scale, false);
//	refproplist.AddPropReference("m_zmin", m_zmin, false); 
//	refproplist.AddPropReference("m_zmax", m_zmax, false);
//	refproplist.AddPropReference("m_z_warped", m_z_warped, false);
    refproplist.AddPropReference("m_camera", m_camera, false);
    refproplist.AddPropReference("m_plane_eqn", m_plane_eqn, false);
//    refproplist.AddPropReference("m_opaque", m_opaque, false);
//    CVisRGBAPyramid *m_pyramid; // MIP map (optional)
}

void CVisSprite::ReadSpriteValue(CVisSDIStream& refsdistream)
{
	// UNDONE:  Change working directory if needed

	refsdistream.ReadClassUsingPropList(*this);

	ReadImage(-1);
	ComputeZMinMax();
	// not sure what this mask is for
	RecomputeMask(-1, 0);

	// UNDONE:  Change working directory back if needed
}

void CVisSprite::WriteSpriteValue(CVisSDOStream& refsdostream)
{
	// UNDONE:  Change working directory if needed

	refsdostream.WriteClassUsingPropList(*this);

	WriteImage(-1);

	// UNDONE:  Change working directory back if needed
}
