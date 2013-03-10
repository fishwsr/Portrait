///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisPolygonRender.h -- triangle/quad texture-mapping code
//
// DESCRIPTION
//  The CVisPolygonRender class implements polygon (triangle/quad)
//  texture-mapped (or solid color) rendering.
//
// SEE ALSO
//  VisPolygonRender.h      longer description
//
// BUGS
//  
//
// DESIGN
//  We could use and Iliffe vector for slightly faster pixel accesses
//  inside the LineResample routines.  However, since the fastest,
//  nearest neighbor version still takes about 9ops/pixel, saving
//  an extra memory reference shouldn't be a big deal.  (Won't the
//  Assume no Aliasing (/Oa) switch remove this extra read anyway?)
//
// HISTORY
//  29-Apr-98  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////


#include "VisImageProcPch.h"
#include "VisImageProc.h"


#ifndef SCALE_BITS
#define SCALE_BITS 16       // (u,v) fixed point scaling
#endif
#define SCALE (1 << SCALE_BITS)
#define ISCALE (1.0 / SCALE)

static inline int iceil(int v)
{
    return ((v + (SCALE-1)) & ~(SCALE-1)) / SCALE;
}

static inline int iscale(double v)
{
    return int(SCALE * v);  // don't bother rounding
}

//
//  The CVisPolygonVertex class is used internally to subdivide
//  polygons.
//

static inline int TestMidpoint(CVisPolygonVertex& v0,
                               CVisPolygonVertex& v1,
                               float max_error)
{
    // Compute the approximation error
    double Ud = fabs(v0.U - v1.U);
    double Vd = fabs(v0.V - v1.V);
    double Wd = fabs(v0.W - v1.W);
    double Ws = v0.W + v1.W;
    double UVm = __max(Ud, Vd);
    double UVe = UVm * Wd;
    return UVe > max_error * Ws;
}

inline void CVisPolygonVertex::SetAverage(CVisPolygonVertex& v0,
                                          CVisPolygonVertex& v1)
{
    // Compute average 3D vertex (U,V,W)
    double Ws = v0.W + v1.W;
    W = 0.5 * Ws;
    double Wi = 1.0 / Ws;
    U = (v0.U * v0.W + v1.U * v1.W) * Wi;
    V = (v0.V * v0.W + v1.V * v1.W) * Wi;

    // Compute average (x,y,z)
    X = 0.5*(v0.X + v1.X);
    Y = 0.5*(v0.Y + v1.Y);
    Z = 0.5*(v0.Z + v1.Z);
}

inline void CVisPolygonVertex::NormalizeXY()
{
    // Divide through by Z
    if (Z != 1.0) {
        double Z_inv = 1.0 / Z;
        X *= Z_inv;
        Y *= Z_inv;
        U *= Z_inv;
        V *= Z_inv;
        W *= Z_inv;
    }
}

inline void CVisPolygonVertex::NormalizeUV()
{
    // Keep coords in (U,V) noramlized + W form (Rick's notes, May 23, 1998)
    if (W != 1.0) {
        double W_inv = 1.0 / W;
        U *= W_inv;
        V *= W_inv;
    }
}

inline void CVisPolygonVertex::SplitEdge(double y, CVisPolygonVertex& prev)
{
    // Adjust vertex by splitting edge to previous vertex
    double a = (y - Y) / (prev.Y - Y);
    X += a * (prev.X - X);
    Y += a * (prev.Y - Y);
    Z += a * (prev.Z - Z);
    U += a * (prev.U - U);
    V += a * (prev.V - V);
    W += a * (prev.W - W);
}

//  These operations are used by RenderQuadFloat:

inline CVisPolygonVertex& CVisPolygonVertex::operator+=(CVisPolygonVertex& v)
{
    X += v.X; Y += v.Y; Z += v.Z;
    U += v.U; V += v.V; W += v.W;
    return *this;
}

inline CVisPolygonVertex& CVisPolygonVertex::operator-=(CVisPolygonVertex& v)
{
    X -= v.X; Y -= v.Y; Z -= v.Z;
    U -= v.U; V -= v.V; W -= v.W;
    return *this;
}

inline CVisPolygonVertex& CVisPolygonVertex::operator*=(double s)
{
    X *= s; Y *= s; Z *= s;
    U *= s; V *= s; W *= s;
    return *this;
}

static inline CVisPolygonVertex operator+(CVisPolygonVertex  v0,
                                          CVisPolygonVertex& v1)
{
    return v0 += v1;
}

static inline CVisPolygonVertex operator-(CVisPolygonVertex  v0,
                                          CVisPolygonVertex& v1)
{
    return v0 -= v1;
}

static inline CVisPolygonVertex operator*(double s, CVisPolygonVertex v0)
{
    return v0 *= s;
}


//
//  The CVisVertexChain is the clipped polygon descriptor
//  We use a std::vector, which isn't very efficient at mid-point insertion.
//  However, clipping rarely occurs, so we'll take this hit, and
//  not pay a price for unclipped polygons.
//
//  Note that at this stage, (U,V,W) contain the 3D (X,Y,Z) coordinates
//

bool CVisVertexChain::ClipToShape(bool UVW, RECT& s)
{
    // Clip to x/z, y/z or z planes (see Rick's notes, May 13, 1998)
    static double epsilon = 1.0 / (1 << 20);

    // Return as soon as one all outside test fails
    return
        ClipToPlane(UVW, CVisVector4(-1, 0,  s.left, 0)) ||
        ClipToPlane(UVW, CVisVector4( 1, 0, -(s.right), 0)) ||
        ClipToPlane(UVW, CVisVector4(0, -1,  s.top, 0)) ||
        ClipToPlane(UVW, CVisVector4(0,  1, -(s.bottom), 0)) ||
        ClipToPlane(UVW, CVisVector4( 0, 0, -1, epsilon));
}

bool CVisVertexChain::ClipToPlane(bool UVW, CVisVector4& p)
{
    // Test all vertices against the plane equation
    int i, n = size(), outside = 0;
    CVisPolygonVertex *v = begin();
    for (i = 0; i < n; i++) {
        double *q = (UVW) ? &v[i].U : &v[i].X;
        double dist = q[0] * p[0] + q[1] * p[1] + q[2] * p[2] + p[3];
        v[i].d = dist;
        outside  += (dist > 0.0);
    }
    if (outside == 0)
        return false;       // all vertices inside
    else if (outside == n) {
        clear();            // clear out vertex list
        return true;        // all vertices outside
    }

    // Create the new list.  Since the original polygon is
    // supposed to be convex, at most one new vertex can be created.

    // Find first outside vertex
    if (v[n-1].d > 0)
        for (i = n-1; v[i-1].d > 0; i--);
    else
        for (i = 0; v[i].d <= 0.0; i++);

    // Insert or delete vertex, as needed
    if (outside == 1)
        insert(&elt(i), elt(i));
    else if (outside > 2) {
        int m1 = __min(outside-2, n-(i+1));
        if (m1 > 0)
            erase(&elt(i+1), &elt(i+1+m1));
        int m0 = outside-2 - m1;
        if (m0 > 0) {
            erase(&elt(0), &elt(m0));
            i -= m0;
        }
    }
    n = size();

    // Compute the clipped points
    int i0 = (i+n-1)%n, i1 = i, i2 = (i+1)%n, i3 = (i+2)%n;
    ClipPt(elt(i1), elt(i0));
    ClipPt(elt(i2), elt(i3));

    return false;
}

void CVisVertexChain::ClipPt(CVisPolygonVertex& v0,
                             CVisPolygonVertex& v1)
{
    // Modify v0 based on intersection of line v0-v1 with plane (d == 0)
    double a = v0.d / (v0.d - v1.d);
    double *p0 = &v0.X, *p1 = &v1.X;
    assert(&v0.U == (p0 + 3));      // sanity check, layout is o.k.
    for (int i = 0; i < 6; i++)
        p0[i] += a * (p1[i] - p0[i]);
}

void CVisVertexChain::NormalizeXY()
{
    // Normalize all the vertices
    for (int i = 0; i < size(); i++) {
        elt(i).NormalizeXY();

        // To be on the safe side, remove any duplicates
        if (i > 0 && elt(i).X == elt(i-1).X && elt(i).Y == elt(i-1).Y) {
            erase(&elt(i));
            i -= 1;
        }
    }
    if (size() > 0 && front().X == back().X && front().Y == back().Y)
        erase(&back());
}

void CVisVertexChain::NormalizeUV()
{
    // Normalize all the vertices
    for (int i = 0; i < size(); i++)
        elt(i).NormalizeUV();
}

void CVisVertexChain::FinalClip(RECT& XY_shape,
                                RECT& UV_shape, int normalized_UV)
{
    // Clip all normalized vertices
    CVisPolygonVertex *v = begin();
    static double e = 1.0 / (1 << 20);      // fudge factor
    for (int i = 0, n = size(); i < n; i++) {
        v[i].X = __max(XY_shape.left,
                 __min(XY_shape.right, v[i].X));
        v[i].Y = __max(XY_shape.top,
                 __min(XY_shape.bottom, v[i].Y));
        if (normalized_UV == 1) {
            v[i].U = __max(UV_shape.left+e,
                     __min(UV_shape.right-e, v[i].U));
            v[i].V = __max(UV_shape.top+e,
                     __min(UV_shape.bottom-e, v[i].V));
        } else if (normalized_UV == 2) {
            double W = v[i].W;
            v[i].U = __max(W*(UV_shape.left+e),
                     __min(W*(UV_shape.right-e), v[i].U));
            v[i].V = __max(W*(UV_shape.top+e),
                     __min(W*(UV_shape.bottom-e), v[i].V));
        }
    }
}

static inline int nmod(int i, int n)
{
    return (i < 0) ? i + n : (i >= n) ? i - n : i;
}

void CVisVertexChain::FormSides(CVisVertexChain& left,
                                CVisVertexChain& right)
{
    // Find endpoints of both chains
    CVisPolygonVertex *v = begin();
    int i, n = size(), lmin = 0, lmax = 0, rmin = 0, rmax = 0;
    for (i = 1; i < n; i++) {
        if (v[i].Y  < v[lmin].Y || v[i].Y == v[lmin].Y && v[i].X < v[lmin].X)
            lmin = i;
        if (v[i].Y  < v[rmin].Y || v[i].Y == v[rmin].Y && v[i].X > v[rmin].X)
            rmin = i;
        if (v[i].Y  > v[lmax].Y || v[i].Y == v[lmax].Y && v[i].X < v[lmax].X)
            lmax = i;
        if (v[i].Y  > v[rmax].Y || v[i].Y == v[rmax].Y && v[i].X > v[rmax].X)
            rmax = i;
    }

    // Find the direction of traversal
    CVisPolygonVertex& vl1 = v[nmod(lmin-1, n)];
    CVisPolygonVertex& vl0 = v[     lmin      ];
    CVisPolygonVertex& vl2 = v[nmod(lmin+1, n)];
    int ldir = (vl1.X-vl0.X)*(vl2.Y-vl0.Y) <
               (vl2.X-vl0.X)*(vl1.Y-vl0.Y) ? -1 : 1;
    CVisPolygonVertex& vr1 = v[nmod(rmin-1, n)];
    CVisPolygonVertex& vr0 = v[     rmin      ];
    CVisPolygonVertex& vr2 = v[nmod(rmin+1, n)];
    int rdir = (vr1.X-vr0.X)*(vr2.Y-vr0.Y) >
               (vr2.X-vr0.X)*(vr1.Y-vr0.Y) ? -1 : 1;
    assert(ldir == - rdir);     // sanity check

    // Create both chains, interpolating vertices if necessary
    left.clear();
    right.clear();
    for (int l = lmin, r = rmin; l != lmax || r != rmax;) {

        // Copy the two vertices
        left.push_back(v[l]);
        right.push_back(v[r]);
        assert(left.size() < size() && right.size() < size());

        if (fabs(v[l].Y - v[r].Y) < 1.0e-4) {   // fudge factor
            // Same Y value, just update both pointers
            l = nmod(l+ldir, n);
            r = nmod(r+rdir, n);
        } else if (v[l].Y < v[r].Y) {
            // Subdivide right edge
            right.back().SplitEdge(v[l].Y, right.end()[-2]);
            l = nmod(l+ldir, n);
        } else {
            // Subdivide left edge
            left.back().SplitEdge(v[r].Y, left.end()[-2]);
            r = nmod(r+rdir, n);
        }
    }

    // Copy the last two vertices
    left.push_back(v[lmax]);
    right.push_back(v[rmax]);
}




//
//  The base CVisPolygonRender class
//

CVisPolygonRender::CVisPolygonRender() :
    m_texture_mode(Perspective),// enable texture mapping
    m_interpolation(Bilinear),  // pixel interpolation mode
    m_parallax(None),           // parallax / offset rendering mode
    m_comp(Copy),               // compositing operation
    m_min_span(8),              // minimum length in subdividing perspective quad
    m_uv_accuracy(1.0 / 64),    // desired (u,v) accuracy in perspective subdivide
    m_uv_slope(1.0 / 4.0)       // maximum slope for two-pass re-sampler
{
    // Nothing special here
    //  do we need to initialize the m_input_xfrm and m_output_xfrm?
    m_output_xfrm = CVisTransform4x4();
    m_input_xfrm  = CVisTransform4x4();
}

CVisPolygonRender::~CVisPolygonRender()
{
    // Nothing special here
}


//
// Rendering primitives for convex polygons
//

void CVisPolygonRender::RenderInput(CVisVector4 n)
{
    CVisVector4 p[4];

    // Form the output image -> world transform matrix
    memcpy(m_output_xfrm[3], &n[0], sizeof(CVisVector4));
    CVisTransform4x4 Mi = (m_output_xfrm).Inverted();

    // Compute (X,Y,Z) values corresponding to point on plane
    //  code adapted from VisResampleCorners()
    CVisRect sh = m_output_shape;
    double xc[2] = {sh.left, sh.right};
    double yc[2] = {sh.top, sh.bottom};
    for (int i = 0; i < 4; i++) {
        CVisVector4 v(xc[(i == 1) | (i == 2)], yc[(i >> 1)]);
        p[i] = (Mi).ProjectVector( v);
    }

    // Render it
    RenderQuad(p);
}

void CVisPolygonRender::RenderPolygon(int n, CVisVector4 p[])
{
    CVisVector4 *u = new CVisVector4[n];
    
    // Project the (X,Y,Z) coordinates into (U,V,W)
    for (int i = 0; i < n; i++) {
        u[i] = m_input_xfrm * p[i];
        u[i][2] = u[i][3];          // copy into 3rd element
        u[i][3] = 1.0;              // not needed?
    }

    // Render the polygon
    RenderPolygon(n, p, u);
}

#if 0   // not needed, after all?
static CVisVector4 PlaneFit(CVisVector4 p[3])
{
    if (p[0][2] == p[1][2] && p[1][2] == p[2][2])   // same z value
        return CVisVector4(0, 0, 1, -p[0][2]);
    else {
        CVisVector4 d1(p[1][0]-p[0][0], p[1][1]-p[0][1], p[1][2]-p[0][2], 1);
        CVisVector4 d2(p[2][0]-p[0][0], p[2][1]-p[0][1], p[2][2]-p[0][2], 1);
        CVisVector4 n;
        VisCross3D(n, d1, d2);
        n[3] = -((n) * ( p[0]));
        return n;
    }
}
#endif

void CVisPolygonRender::RenderPolygon(int n, CVisVector4 p[], CVisVector4 u[])
{
    if (n < 3)
        return;

    // Form the m_polygon and clip it
    m_polygon.resize(n);
    for (int i = 0; i < n; i++)
        ProjectVertex(m_polygon[i], p[i], u[i]);
    if (m_polygon.ClipToShape(false, m_output_shape))
        return;     // polygon clipped away
    if (int(m_texture_mode) > int(SolidFillZ) &&
        m_polygon.ClipToShape(true, m_input_trimmed))
        return;

    // Normalize the coordinates, and form the two side chains
    //  see Rick's notes, May 24, 1998
    m_polygon.NormalizeXY();
    if (m_polygon.size() < 3)
        return;
    m_polygon.FormSides(m_left, m_right);
    int UV_normalize =
        (int(m_texture_mode) > int(SolidFillZ)) +
        (int(m_texture_mode) > int(Perspective));
    if (UV_normalize == 1) {
        m_left. NormalizeUV();
        m_right.NormalizeUV();
    }
    m_left .FinalClip(m_output_shape, m_input_trimmed, UV_normalize);
    m_right.FinalClip(m_output_shape, m_input_trimmed, UV_normalize);

    // Render the quads (slabs)
    n = m_left.size();
    assert(n == m_right.size());
    for (i = 0; i < n-1; i++) {
        switch (m_texture_mode) {
         case SolidFill:      // fill with solid color
         case Affine:         // use affine texture coordinate, no foreshortening
            RenderQuad(m_left[i  ], m_right[i  ],
                       m_left[i+1], m_right[i+1]);
            break;
         case ShowSubdivision:// fill each subdivided segment with different color (for debugging, only)
         case Perspective:    // use approximate perspective texture mapping
            SubdivideQuad(m_left[i  ], m_right[i  ],
                          m_left[i+1], m_right[i+1]);
            break;
         case SolidFillZ:     // fill with solid color with z-buffering
         case PerPixel:       // use exact perspective (per-pixel divide: slow)
         case PerPixelZ:      // use exact perspective with z-buffering
            RenderQuadFloat(m_left[i  ], m_right[i  ],
                            m_left[i+1], m_right[i+1]);
            break;
         default:
            assert(0);      // shouldn't get here
        }
    }
}

void CVisPolygonRender::ProjectVertex(CVisPolygonVertex &v,
                                      CVisVector4& p, CVisVector4& u)
{
    CVisVector4 x = m_output_xfrm * p;
    v.X = x[0], v.Y = x[1], v.Z = x[3];     // use FOURTH component!
    v.U = u[0], v.V = u[1], v.W = u[2];
}


//
//  Quad (slab) subdivision in perspective mode
//

void CVisPolygonRender::SubdivideQuad(CVisPolygonVertex &v00,
                                      CVisPolygonVertex &v01,
                                      CVisPolygonVertex &v10,
                                      CVisPolygonVertex &v11)
{
    // Sanity check for empty quad
    if (ceil(v01.X) - ceil(v00.X) < 1 &&
        ceil(v11.X) - ceil(v10.X) < 1 ||
        ceil(v10.Y) - ceil(v00.Y) < 1)
        return;

    CVisPolygonVertex v0m, v1m, vm0, vm1, vmm;

    // Determine which of the four sides need splitting
    bool split_X = 
        v01.X - v00.X > m_min_span &&
        TestMidpoint(v00, v01, m_uv_accuracy) ||
        v11.X - v10.X > m_min_span &&
        TestMidpoint(v10, v11, m_uv_accuracy);
    if (split_X) {
        v0m.SetAverage(v00, v01);
        v1m.SetAverage(v10, v11);
    }
    bool split_Y = 
        v10.Y - v00.Y > m_min_span &&
        TestMidpoint(v00, v10, m_uv_accuracy) ||
        v11.Y - v01.Y > m_min_span &&
        TestMidpoint(v01, v11, m_uv_accuracy);
    if (split_Y) {
        vm0.SetAverage(v00, v10);
        vm1.SetAverage(v01, v11);
    }

    // No splits required (most common, i.e., bottom case)
    if (! (split_X || split_Y)) {
        RenderQuad(v00, v01, v10, v11);
        return;
    }
    
    // Split horizontally (one piece on top of the other)
    if (split_Y && ! split_X) {
        SubdivideQuad(v00, v01, vm0, vm1);
        SubdivideQuad(vm0, vm1, v10, v11);
        return;
    }

    // Split vertically (one piece beside the other)
    if (split_X && ! split_Y) {
        SubdivideQuad(v00, v0m, v10, v1m);
        SubdivideQuad(v0m, v01, v1m, v11);
        return;
    }

    // Need to compute center point
    vmm.SetAverage(vm0, vm1);

    // Four-way split (we could do some 3-ways, but not much savings...)
    SubdivideQuad(v00, v0m, vm0, vmm);
    SubdivideQuad(v0m, v01, vmm, vm1);
    SubdivideQuad(vm0, vmm, v10, v1m);
    SubdivideQuad(vmm, vm1, v1m, v11);
}

void CVisPolygonRender::RenderQuad(CVisPolygonVertex &v00,
                                   CVisPolygonVertex &v01,
                                   CVisPolygonVertex &v10,
                                   CVisPolygonVertex &v11)
{
    // Compute starting, end, and delta (x,y) values
    int iy0 = int(ceil(v00.Y));         // first line to render
    int iy1 = int(ceil(v10.Y));         // first line to not render
    double fy0 = iy0 - v00.Y;
    double dy  = v10.Y - v00.Y;
    double dyi = 1.0 / dy;
    double dxl = v10.X - v00.X;
    double dx0_dyf = dxl * dyi;
    double dxr = v11.X - v01.X;
    double dx1_dyf = dxr * dyi;
    int dx0_dy = iscale(dx0_dyf);       // slope of left  edge
    int dx1_dy = iscale(dx1_dyf);       // slope of right edge
    int x0 = iscale(v00.X + dx0_dyf * fy0);  // start column
    int x1 = iscale(v01.X + dx1_dyf * fy0);  // end column

    // If not using texture coordinates, just fill in color
    if (m_texture_mode == ShowSubdivision)
        NextPseudoColor();
    if (m_texture_mode == SolidFill ||
        m_texture_mode == ShowSubdivision) {
        for (int iy = iy0; iy < iy1; iy++) {

            // Compute left and right edges, trim to integer start/end values
            int ix0 = iceil(x0);
            int ix1 = iceil(x1);
            int n = ix1 - ix0;     // don't render last pixel, if integral

            // Call the line resampling code
            LineResampleFill(ix0, iy, n);
            x0 += dx0_dy;
            x1 += dx1_dy;
        }
        return;
    }

    // Compute starting, end, and delta (u,v) values
    double dx0 = v01.X - v00.X;
    double dx1 = v11.X - v10.X;
    double du_dx0, dv_dx0, du_dx1, dv_dx1;
    if (dx0 > 0) {
        double dx0i = 1.0 / dx0;
        du_dx0 = (v01.U - v00.U) * dx0i;
        dv_dx0 = (v01.V - v00.V) * dx0i;
    }
    if (dx1 > 0) {
        double dx1i = 1.0 / dx1;
        du_dx1 = (v11.U - v10.U) * dx1i;
        dv_dx1 = (v11.V - v10.V) * dx1i;
    }
    int du_dx = iscale((dx0 > 0) ? du_dx0 : du_dx1);
    int dv_dx = iscale((dx0 > 0) ? dv_dx0 : dv_dx1);
    int du_dx_dy = 0, dv_dx_dy = 0;
    if (dx0 > 0 && dx1 > 0) {
        du_dx_dy = iscale((du_dx1 - du_dx0) * dyi);
        dv_dx_dy = iscale((dv_dx1 - dv_dx0) * dyi);
    }
    // see Rick's notes, 23-May-98
    int du_dy = iscale((v10.U - v00.U - 
                    ISCALE * (du_dx + du_dx_dy * dy) * dxl) * dyi);
    int dv_dy = iscale((v10.V - v00.V - 
                    ISCALE * (dv_dx + dv_dx_dy * dy) * dxl) * dyi);
    du_dx += int(du_dx_dy * fy0);    // update for first line
    dv_dx += int(dv_dx_dy * fy0);
    double fx0 = dx0_dyf * fy0;
    int u0 = iscale(v00.U) + int(du_dy * fy0 + du_dx * fx0);
    int v0 = iscale(v00.V) + int(dv_dy * fy0 + dv_dx * fx0);
    
    // Shift by input origin, so we are always positive
    u0 -= iscale(m_input_shape.left);
    v0 -= iscale(m_input_shape.top);

    // Compute first column rendered, and correct u and v to it
    int ix0_old = iceil(x0);    // so that dix0 = 0 on first iteration...
    double fx0c = ix0_old - ISCALE * x0;
    u0 += int(fx0c * du_dx);
    v0 += int(fx0c * dv_dx);

    // Render all scale lines except for last one (if integral)
    for (int iy = iy0; iy < iy1; iy++, x0 += dx0_dy, x1 += dx1_dy,
                                       u0 += du_dy,  v0 += dv_dy) {

        // Compute left and right edges, trim to integer start/end values
        int ix0 = iceil(x0);
        int ix1 = iceil(x1);
        int n = ix1 - ix0;     // don't render last pixel, if integral

        // Compute change in x from previous line, and adjust (u0, v0)
        int dix0 = ix0 - ix0_old;
        ix0_old = ix0;
        u0 += dix0 * du_dx;
        v0 += dix0 * dv_dx;

        // Call the line resampling code
        LineResample(ix0, iy, n, u0, v0, du_dx, dv_dx);

        // Adjust du_dx and dv_dx for bilinear resampling
        du_dx += du_dx_dy;
        dv_dx += dv_dx_dy;
    }
}

//
//  Floating point renderer with per-pixel divide: slow by accurate
//

void CVisPolygonRender::RenderQuadFloat(CVisPolygonVertex &v00,
                                        CVisPolygonVertex &v01,
                                        CVisPolygonVertex &v10,
                                        CVisPolygonVertex &v11)
{
    // Compute starting, end, and delta (x,y) and (U,V,W) values
    int iy0 = int(ceil(v00.Y));         // first line to render
    int iy1 = int(ceil(v10.Y));         // first line to not render
    double dy  = v10.Y - v00.Y;
    double dyi = 1.0 / dy;
    double fy0 = iy0 - v00.Y;
    CVisPolygonVertex d0 = dyi*(v10 - v00);
    CVisPolygonVertex d1 = dyi*(v11 - v01);
    CVisPolygonVertex v0 = v00 + fy0*d0;
    CVisPolygonVertex v1 = v01 + fy0*d1;
    
    // Shift by input origin, so we are always positive
    v0.U -= m_input_shape.left * v0.W;
    v0.V -= m_input_shape.top * v0.W;

    // Render all scale lines except for last one (if integral)
    for (int iy = iy0; iy < iy1; iy++, v0 += d0, v1 += d1) {

        // Compute left and right edges, trim to integer start/end values
        int ix0 = int(ceil(v0.X));
        int ix1 = int(ceil(v1.X));
        int n = ix1 - ix0;     // don't render last pixel, if integral
        double dx  = v1.X - v0.X;
        double dxi = 1.0 / dx;
        double fx0 = ix0 - v0.X;
        CVisPolygonVertex d2 = dxi*(v1 - v0);
        CVisPolygonVertex v2 = v0 + fx0*d2;

        // Call the line resampling code
        LineResampleFloat(ix0, iy, n, v2, d2);
    }
}

//
// The pixel-type specific polygon renderer
//

template <class PixelType>
void CVisPolygonRenderOf<PixelType>::Initialize(bool clear_image)
{
    // Save the (trimmed) input/output image shapes
    m_output_shape = m_output.Shape();
    if (int(m_texture_mode) > int(SolidFillZ)) {
        int b0 = (m_interpolation == Bicubic) ? 1 : 0;
        int b1 = (m_interpolation == Bicubic) ? 2 :
                 (m_interpolation == Bilinear) ? 1 : 0;
        m_input_shape = m_input.Shape();
        m_input_trimmed = m_input_shape;
	    m_input_trimmed.InflateRect(- b0, - b0, - b1, - b1);

        // Shift origin of input so it's always (0,0)
        //  remember to subtract m_input origin from (u,v) in LineResample
        m_shifted = m_input;
        m_shifted.ResetOrigin(0, 0);
#ifdef USE_ILIFFE
        // This has to be replaced with a new and a call
        // to .PpvFillIliffeVector()
        m_input_array = m_shifted.PixelArray();
#endif
    }

    // Check that z-buffer is the right size
    if (m_texture_mode == SolidFillZ || m_texture_mode == PerPixelZ) {
        if (m_output.Shape() != m_z_buffer.Shape())
            m_z_buffer.Allocate(m_output.Shape());
        if (clear_image)
            m_z_buffer.ClearPixels();
    }

    // Optionally clear the image
    if (clear_image)
        m_output.ClearPixels();

    m_pseudo_color_id = 0;
}

//
//  Scanline re-sampling (rendering)
//

template <class PixelType>
void CVisPolygonRenderOf<PixelType>::LineResampleFill
    (int x, int y, unsigned int n)
{
    PixelType *dst = &m_output.Pixel( x, y);
    for (int i = 0; i < n; i++)
        dst[i] = m_fill_color;
}

template <class PixelType>
void CVisPolygonRenderOf<PixelType>::LineResample
    (int x, int y, unsigned int n, int u, int v, int du, int dv)
{
    PixelType *dst = &m_output.Pixel( x, y);
#ifdef USE_ILIFFE
    PixelType **src = m_input_array;
#endif
    if (m_interpolation == Nearest) {
        for (int i = 0; i < n; i++, u += du, v += dv) {
            int iu = u / SCALE, iv = v / SCALE;
#ifdef USE_ILIFFE
            dst[i] = src[iv][iu];
#else
            dst[i] = m_shifted.Pixel(iu, iv);
#endif
        }
    } else if (m_interpolation == Bilinear) {
        for (int i = 0; i < n; i++, u += du, v += dv) {
            int iu = u / SCALE, iv = v / SCALE;
#ifdef USE_ILIFFE
            PixelType *src0 = &src[iv][iu];
#else
            PixelType *src0 = &m_shifted.Pixel(iu, iv);
#endif
            int fu = u & (SCALE-1), fv = v & (SCALE - 1);
            if (fv == 0)
                dst[i] = PixelSampleLinear(src0[0], src0[1], fu);
            else {
#ifdef USE_ILIFFE
                PixelType *src1 = &src[iv+1][iu];
#else
                PixelType *src1 = &m_shifted.Pixel(iu, iv+1);
#endif
                PixelType p0 = PixelSampleLinear(src0[0], src0[1], fu);
                PixelType p1 = PixelSampleLinear(src1[0], src1[1], fu);
                dst[i]       = PixelSampleLinear(p0,      p1,      fv);
            }
        }
    } else {
        // Bicubic not implemented yet...
    }
}

template <class PixelType>
void CVisPolygonRenderOf<PixelType>::LineResampleFloat
    (int x, int y, unsigned int n,
     CVisPolygonVertex& uv, CVisPolygonVertex& d)
{
    bool z_buffered = m_texture_mode == SolidFillZ ||
                      m_texture_mode == PerPixelZ;
    PixelType *dst = &m_output.Pixel(x, y);
    float *zbuf = (z_buffered) ? &m_z_buffer.Pixel(x,y) : 0;
    double u = uv.U, v = uv.V, w = uv.W, z = uv.Z;
    double du = d.U, dv = d.V, dw = d.W, dz = d.Z;
    for (int i = 0; i < n; i++, u += du, v += dv, w += dw, z += dz) {
        double uw = u / w, vw = v / w;
        int iu = int(uw), iv = int(vw);

        // Optionally test the z-buffer
        if (z_buffered) {
            if (zbuf[i] == 0 || z < zbuf[i])
                zbuf[i] = z;
            else
                continue;   // occluded pixel, skip
        }

        // Solid fill mode
        if (m_texture_mode == SolidFillZ) {
            dst[i] = m_fill_color;
        }

        // Nearest neighbor mode
        else if (m_interpolation == Nearest) {
            dst[i] = m_shifted.Pixel(iu, iv);
        }

        // Bilinear interpolation
        else if (m_interpolation == Bilinear) {
            PixelType *src0 = &m_shifted.Pixel(iu, iv);
            int fu = int(SCALE *(uw - iu));
            int fv = int(SCALE *(vw - iv));
            if (fv == 0)
                dst[i] = PixelSampleLinear(src0[0], src0[1], fu);
            else {
                PixelType *src1 = &m_shifted.Pixel(iu, iv+1);
                PixelType p0 = PixelSampleLinear(src0[0], src0[1], fu);
                PixelType p1 = PixelSampleLinear(src1[0], src1[1], fu);
                dst[i]       = PixelSampleLinear(p0,      p1,      fv);
            }
        } else {
            // Bicubic not implemented yet...
        }
    }
}

//
//  These routines are used to pseudo-color each polygon
//

template <class PixelType>
void CVisPolygonRenderOf<PixelType>::NextPseudoColor()
{
    // Set fill color to next pseudo-color
    m_pseudo_color_id += 1;
    SetFillColor(PseudoColor(m_pseudo_color_id));
}

template <class PixelType>
PixelType CVisPolygonRenderOf<PixelType>::PseudoColor(int i)
{
    // If this doesn't compile, you have to provide your own version...
    return PixelType(i);
}

CVisRGBABytePixel CVisPolygonRenderOf<CVisRGBABytePixel>::PseudoColor(int i)
{
    int r = 0, g = 0, b = 0;
    assert(i > 0 && i < (1 << 24));
    for (int j = 0; i; j++, i >>= 3) {
        r |= ((i >> 0) & 1) << (7 - j);
        g |= ((i >> 1) & 1) << (7 - j);
        b |= ((i >> 2) & 1) << (7 - j);
    }
    return CVisRGBABytePixel(r, g, b, 255);
}

//
// Explicit template instantiation
//

#pragma warning(disable : 4660)  // template class already instantiated

template CVisPolygonRenderOf<CVisRGBABytePixel>;

#pragma warning(default : 4660)
