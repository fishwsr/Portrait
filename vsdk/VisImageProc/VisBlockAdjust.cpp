///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisBlockAdjust.cpp -- panoramic image alignment adjustment
//
// DESCRIPTION
//  The routines in this file can be used to perform a "block adjustment"
//  to the initial alignment produced by VisAlign.cpp.
//
//
// SEE ALSO
//  VisBlockAdjust.h    description
//  VisAlign.h          description of aligner class
//  VisGeometry.h       geometric transformations
//  VisResample.h       global geometric warping
//  VisTrack.h          a tracker description: precursor to block adjust
//
//  Richard Szeliski's research notes, July 17, July 26, Aug 13, Sept 3.
//  Rick's research notes Mar. 4, 1997. 
//
// BUGS
//   
//
// DESIGN
//  
//
// HISTORY
//  03-Sep-96  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////


#include "VisImageProcPch.h"
#include "VisImageProc.h"


#ifndef M_PI 
#define M_PI    3.1415926536
#endif // M_PI
#define RadianToDegree  180.0/M_PI


#if defined(_DEBUG) && defined(_AFXDLL)
#include <afxwin.h>         // MFC core and standard components
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

#define FFF 280

template <class F, class I>
CVisBlockAdjuster<F,I>::CVisBlockAdjuster()
{
    // Reset default parameters (see VisAlign.cpp for defaults)
    n_levels        = 1;    // number of levels in coarse-to-fine
    band_pass       = 0;    // use band-pass filtered images
    initial_blur    = 0;    // number of initial 121 blur steps
    adjust_bias     = 1;    // recompute bias after each iteration
    robustify       = 0;    // use robust outlier removal
    decrease_E      = 0;    // no change if E has not decreased
    use_mask        = 1;    // use masked images or alpha channel
    //bound_box_extra = 8;  // extra pixels around bbox edge
    bound_box_extra = -1;   // no trimming at all
    subsample       = 32;    // speedup Hessian and gradient computation
    //subsample       = 8;    // speedup Hessian and gradient computation
    fixed_Hessian   = 1;    // don't recompute Hessian
    store_error     = 0;    // store error image (for debugging)
    border_width    = 0;    // ignore pixels around border
    max_uv_change   = 2.0f; // maximum allowed motion per step
    min_uv_change   = 0.1f; // motion threshold for decreasing level
    max_inner       = 4;   // maximum inner iterations per step
    max_steps_level = 5;   // maximum steps per level

    search_area2    = 4;    // half-size of correlation search area
    search_subdivide= 0;    // subdivide into n^2 problems
    search_coarsest = 1;    // only perform search at coarsest level
    
    m_match_only_0  = 0;

    // Clear out pointers
    m_composite     = 0;

	// iteration steps 
	m_iteration_steps = 5; // changed from 30 -> 5, Rick, 20-Oct-99
}

template <class F, class I>
CVisBlockAdjuster<F,I>::~CVisBlockAdjuster()
{
}

template <class F, class I>
//void CVisBlockAdjuster<F,I>::Initialize(CVisComposite<F, I> *composite)
//void CVisBlockAdjuster<F,I>::Initialize(CVisRGBAComposite *composite)
void CVisBlockAdjuster<F,I>::Initialize(CVisWarpedComposite<F::T_Pixel> *composite)
{
    m_composite = composite;
    // SetupAdjacencyGraph(m_sequence, m_transform); 
}


template <class F, class I>
void CVisBlockAdjuster<F,I>::ConstructMatchList(SVisBlockMatchList &l, int level)
{
    I               &fixed = (*m_fixed)[level];
    CVisFloatImage  &grad  = m_gradient[level];
    int s  = subsample;

    // Compute how many valid blocks there are
    int width = fixed.Width(), height = fixed.Height();
    int count = 0; 
    int br, bc; 
    // int bw = 2 was used in block adjust 
    int bw = 0; 
    for (br = bw; br < grad.Height()-bw; br++) {

        for (bc = bw; bc < grad.Width()-bw; bc++) {

            SVisGradientBlock& gb = *(SVisGradientBlock *)
                &grad.Pixel(bc + grad.Left(), br + grad.Top(), 0);

            if (gb.n > 0.5 * s * s)
                count ++; 
            else 
                gb.n = 0; 
        }
    }


    // Fill in the block match pairs
    l.n = count; 
    l.nbp = new SVisBlockMatchPair[count]; 

    count = 0; 
    float total_uv = 0.0; 
    for (br = bw; br < grad.Height()-bw; br++) {
        for (bc = bw; bc < grad.Width()-bw; bc++) {

            SVisGradientBlock& gb = *(SVisGradientBlock *)
                &grad.Pixel(bc + grad.Left(), br + grad.Top(), 0);

            if (gb.n > 0)
            {
                SVisBlockMatchPair& pBp = l.nbp[count++]; 
                pBp.br = br + grad.Top() + m_br_offset;
                pBp.bc = bc + grad.Left() + m_bc_offset;
                pBp.uij[0] = gb.x +  0.5f * width + fixed.Left(); 
                pBp.uij[1] = gb.y +  0.5f * height + fixed.Top(); 

                pBp.uuik[0] = pBp.uij[0] + gb.u;
                pBp.uuik[1] = pBp.uij[1] + gb.v; 

                CVisVector4 v1 = CVisVector4(pBp.uuik[0], pBp.uuik[1]); 
                CVisTransform4x4 i_motion = m_motion.Inverted(); 
                CVisVector4 vx = i_motion.ProjectVector(v1);    // 5-Oct-99: wrong one?
                CVisVector4 v2 = m_motion.ProjectVector(v1);    // 5-Oct-99: szeliski -- changed to this one (?)
                pBp.uik[0] = v2[0]; 
                pBp.uik[1] = v2[1]; 

                // should we compute 2 eigenvalues instead? 
                // lambda = 0.5 * (a+b +/- sqrt((a-b)^2 + 4*c^2))
#ifdef NEED_EIGEN
                double aplusb = gb.gx2 + gb.gy2;
                double aminusb = gb.gx2 - gb.gy2;
                double sqrterm = sqrt(aminusb * aminusb + 4*gb.gx_gy*gb.gx_gy); 

                pBp.C[0][0] = 0.5 * (aplusb + sqrterm); 
                pBp.C[1][1] = 0.5 * (aplusb - sqrterm); 
                pBp.C[0][1] = pBp.C[1][1] / (pBp.C[0][0] + 0.0001); 
#else
                pBp.C[0][0] = gb.gx2; 
                pBp.C[0][1] = gb.gx_gy; 
                pBp.C[1][0] = gb.gx_gy; 
                pBp.C[1][1] = gb.gy2; 
#endif
                pBp.valid = 1;      // not really needed/used?

                float movement = gb.u * gb.u + gb.v * gb.v; 
                total_uv += movement; 
#ifdef DEBUG
                CVisVector4 v3 = CVisVector4(pBp.uij[0], pBp.uij[1]);
                CVisVector4 v4 = m_motion.ProjectVector(v3); 

                static bool debug_corresp = false;
                if (debug_corresp) {
                    char dbg_string[1024];
                    sprintf(dbg_string, "matching points: (%f %f) (%f %f) (%f %f)\n", 
                        pBp.uij[0], pBp.uij[1], pBp.uuik[0], pBp.uuik[1], 
                        pBp.uik[0], pBp.uik[1]); 
                    m_composite->ProgressReport(dbg_string);
                }
#endif //DEBUG
            }

        }
    }

    // how much movement after block adjust? should be very small
    if (count > 0) {
        total_uv /= (float) count; 
        total_uv = sqrt(total_uv); 
    }

}

template <class F, class I>
void CVisBlockAdjuster<F,I>::ComputeMatch(int j, int k, SVisBlockMatchList &l)
{
	int n_matches = m_matchpairs; 
    // Initialize the BlockMatchList
    l.j = j;
    l.k = k;
    l.n = 0;
    l.nbp = 0;

    // need to write a function which computes whether two frames overlap 
    // given their transformations, see harry's notes on 3-28-97
    // but for now ...
#if 0
    if (k == j-1 || k == j+1 || k == j-2 || k == j+2)
        l.overlap = 1; 
    else 
    {
        l.overlap = 0; 
        return;
    }
#endif

    if (n_matches > 0)
    {
        if (k > j + n_matches || k < j - n_matches)
        {
            l.overlap = 0; 
            return; 
        }
    }

#if 1
    I& img_j = m_composite->Frame(j);
    I& img_k = m_composite->Frame(k);
#endif
    
    // Compute the relative transform.  This trnasform takes a ray in j projects it out to
	// world coords then projectss back into k.
    CVisTransform4x4 t_j = m_composite->Frame(j).TransformChain().CompleteTransform();
    CVisTransform4x4 t_k = m_composite->Frame(k).TransformChain().CompleteTransform().Inverted();
    CVisTransform4x4 t = t_k * t_j;

    SetAlignment(t);    // is this needed?  m_motion used below...

    // Trim the images against each other:
    
    // Some of this code simplified from CVisAligner::AlignStep():

    CVisShape shape_j = img_j.Shape(); 
    CVisShape shape_k = img_k.Shape(); 
    CVisShape warped_k = VisResampleShape(shape_k, t);
    CVisShape inter_shape = (shape_j & warped_k); 
    int quadR = shape_j.Height() >> 3; 
    int quadC = shape_j.Width() >> 3; 
    if ( (abs(j-k)==1 && inter_shape.Width() > 0 && inter_shape.Height() > 0 ) ||
		 (inter_shape.Width() > quadC || inter_shape.Height() > quadR) )
    {
        l.overlap = 1; 
    }
    else 
	{
        l.overlap = 0;
		m_composite->ProgressReport("Frame %d and frame %d do NOT overlap", j, k); 
        return; 
	}
    // Pull out the j'th and k'th images
    // Warped() is not 8-parameter warp; it is warped with radial distortion etc. 
    SetFixed(m_composite->Warped(j));
    SetDeformable(m_composite->Warped(k));
    
    int level = 0;  // for now, just a single level
    // bound_box_extra = search_area2 + 2;     // conservative enough?
    search_subdivide = 10000000;    // forces m_search_uv and m_gradient same size

    try 
    {
        InitLevel(level);
    } 
    catch(CVisError& ) 
    {   // no intersection between fixed and float
        return;     // all done
    }

    // Progress report
    m_composite->ProgressReport("Matching frames %d and %d", j, k);

    // Compute displacement between trimmed and original
    CVisShape original_shape = m_fixed_original.Shape();
    CVisShape trimmed_shape  = (*m_fixed)[0].Shape();
    m_br_offset = (trimmed_shape.Top() - original_shape.Top()) / subsample;
    m_bc_offset = (trimmed_shape.Left() - original_shape.Left()) / subsample;

    // Resample (trimmed) image k into j'th coordinate system
    VisResample((*m_float)[level], m_warped[level],
                  m_motion, VIS_INTERP_LINEAR, 1);

    static bool debug_overlap = false;
    if (debug_overlap) {
    char filename[1024];
    sprintf(filename, "C:\\temp\\blkj%d_%d.tga", j, k);
    (*m_fixed)[level].WriteFile(filename);
    sprintf(filename, "C:\\temp\\blkk%d_%d.tga", j, k);
    (*m_float)[level].WriteFile(filename);
    sprintf(filename, "C:\\temp\\blkwrp%d_%d.tga", j, k);
    m_warped[level].WriteFile(filename);
    }

    // Compute the block-based motion field
    AccumulatePixelGradients(level, 1, 1, 1, 0);   // explicit search & gradient descent (Rick, 14-Oct-99)
    // AccumulatePixelGradients(level, 0, 1, 0, 0);   // explicit search
    // AccumulatePixelGradients(level, 1, 0, 1, 1);   // gradient descent (from prev. pt)
    // need to write a function which computes translational motion for each patch
    m_search_uv[level].CopyPixelsTo(m_gradient[level]); 

#if 0
    if (robustify)  // add later?
        MakeWeightsRobust(level);
#endif

    ConstructMatchList(l, level); 
}

template <class F, class I>
void CVisBlockAdjuster<F,I>::ComputeMatchAll()
{
    // (Re-)allocate the match list
    m_match_list = CVisBlockMatchImage(NFrames(), NFrames());
    m_match_list.ClearPixels();

    // Compute all of the pairwise matches
    for (int j = 0; j < NFrames(); j++) {
		BOOL bOverlap = 0;
        for (int k = 0; k < (m_match_only_0 ? 1 : NFrames()); k++) {
        // for (int k = j+1; k < NFrames(); k++) {
            if (j == k)
                continue;
            ComputeMatch(j, k, m_match_list.Pixel(k, j));
            bOverlap |= m_match_list.Pixel(k, j).overlap;
        }
		// if frames don't overlap at all AccumulateFrames()
		// will fail
		assert(bOverlap);
    }

}

template <class F, class I>
void CVisBlockAdjuster<F,I>::DeleteMatchAll()
{
    // Delete all of the pairwise matches (heap-allocated lists)
    for (int j = 0; j < NFrames(); j++) {
        for (int k = 0; k < (m_match_only_0 ? 1 : NFrames()); k++) {
            if (j == k)
                continue;
            SVisBlockMatchList &l = m_match_list.Pixel(k, j);
            delete [] l.nbp;
        }
    }

}

//
// modified from ConstrainSystem() in Align.cpp
// should modify it s.t. it takes **L 
//  L has n+1 items and L[n] = d
// 
//  Modify Ax=b by a linear constraint L where L[k] = 1
//      (see Rick's research notes, June 13, 1996)
//

static void ConstrainedSystem(CVisDMatrix &A, CVisDVector &b, const CVisDVector &L,
                            int k, int n)
{
    // Set A += (Akk L - Ak)L^T + L (Akk L - Ak)^T
    double Akk = A[k][k];
    CVisDVector Ak(n);  // make a copy, so it isn't clobbered during update
    memcpy(&Ak[0], &A[k][0], n*sizeof(double));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i][j] += (Akk * L[i] - Ak[i])*L[j] +
                       (Akk * L[j] - Ak[j])*L[i];

    // Set b += (Akk L - Ak)d   + L(Akk d - bk)
    double bk = b[k], d = L[n];
    for (i = 0; i < n; i++)
        b[i] += (Akk * L[i] - Ak[i])*d + L[i]*(Akk*d - bk);
}


static void NegativeSkewedMatrix(CVisDMatrix& M, double x, double y, double z)
{
    // should assert Height Left >= 3 
    M[0][0] = M[1][1] = M[2][2] = 0; 
    M[0][1] = z;
    M[0][2] = -y; 
    M[1][2] = x; 
    M[1][0] = -M[0][1]; 
    M[2][0] = -M[0][2]; 
    M[2][1] = -M[1][2]; 
}

static double eXtotal, eXcount;

// need to input Rj and Rk to compute gradient wrt focal length
static void AccumulatePairGradientHessian(int j, int k, SVisBlockMatchList &l, 
                                          CVisDMatrix& H, CVisDVector& y, 
                                          float fj, float fk, 
                                          float cx, float cy, 
                                          const CVisTransform4x4 &Rj, const CVisTransform4x4 &Rk)
{
    assert (fj > 0 && fk > 0); 

    CVisDVector nXij = CVisDVector(3); 
    CVisDVector nXik = CVisDVector(3); 
    CVisDVector  Xij = CVisDVector(3); 
    CVisDVector  Xik = CVisDVector(3); 
    CVisDVector eXi  = CVisDVector(3); 

    CVisDMatrix Mij = CVisDMatrix(3, 4); 
    CVisDMatrix Mik = CVisDMatrix(3, 4); 
    int jj, kk; 
    double normij;
    double normik; 
    double lij2; 
    double lij3; 
    double vfij[3]; 
    double lik2; 
    double lik3; 
    double vfik[3]; 
    double eXt = 0.0, eXc = 0.0;

    // loop through all grids whenever there is a valid one
    // add its effect to HMatrix and yVector
    // to solve the linear eq   H x = y
    // see notes on 3-22 and 3-23-97
    for (int c = 0; c < l.n; c++) 
    {

        SVisBlockMatchPair& pBp = l.nbp[c]; 
       // get the corresponding ij and ij points, add f to become a 3 vector
        nXij[0] = pBp.uij[0] - cx; 
        nXij[1] = pBp.uij[1] - cy; 
        nXij[2] = fj; 
        normij = sqrt(nXij[0]*nXij[0] + nXij[1]*nXij[1] + nXij[2]*nXij[2]); 

        nXik[0] = pBp.uik[0] - cx; 
        nXik[1] = pBp.uik[1] - cy; 
        nXik[2] = fk; 
        normik = sqrt(nXik[0]*nXik[0] + nXik[1]*nXik[1] + nXik[2]*nXik[2]); 

        // normalize nXij, nXik
        for (jj = 0; jj <3; jj++)
        {
            nXij[jj] /= normij; 
            nXik[jj] /= normik; 
        }

        // get Xij, Xik
        for (jj=0; jj<3; jj++)
        {
            Xij[jj] = Rj[jj][0]*nXij[0] + Rj[jj][1]*nXij[1] + Rj[jj][2]*nXij[2]; 
            Xik[jj] = Rk[jj][0]*nXik[0] + Rk[jj][1]*nXik[1] + Rk[jj][2]*nXik[2]; 
            eXi[jj] = -(Xij[jj] - Xik[jj]); //note negative e is used
            eXt    += eXi[jj] * eXi[jj];    // sum-squared error
        }
        eXc += 1.0;

        // assign Mij[3][3] and Mik as negative skewed matrix
        NegativeSkewedMatrix(Mij, Xij[0], Xij[1], Xij[2]); 
        NegativeSkewedMatrix(Mik, Xik[0], Xik[1], Xik[2]); 

        lij2 = normij * normij; 
        lij3 = normij * lij2; 

        vfij[0] = -pBp.uij[0] * fj / lij3; 
        vfij[1] = -pBp.uij[1] * fj / lij3; 
        vfij[2] = (lij2 - fj*fj)   / lij3; 

        for (jj = 0; jj < 3; jj++)
            Mij[jj][3] = Rj[jj][0]*vfij[0] + Rj[jj][1]*vfij[1] + Rj[jj][2]*vfij[2]; 

        lik2 = normik * normik; 
        lik3 = normik * lik2; 
        vfik[0] = -pBp.uik[0] * fk / lik3; 
        vfik[1] = -pBp.uik[1] * fk / lik3; 
        vfik[2] = (lik2 - fk*fk)   / lik3; 

        for (jj = 0; jj < 3; jj++)
            Mik[jj][3] = Rk[jj][0]*vfik[0] + Rk[jj][1]*vfik[1] + Rk[jj][2]*vfik[2]; 

        // use -Mik because xij - xik is to be minimized
        for (jj = 0; jj < 3; jj++)
            for (kk = 0; kk < 4; kk++)
            Mik[jj][kk] = - Mik[jj][kk]; 
        
        
        CVisDVector egj = CVisDVector(4); 
        CVisDVector egk = CVisDVector(4); 
        CVisDMatrix Aggtjj = CVisDMatrix(4, 4); 
        CVisDMatrix Aggtkk = CVisDMatrix(4, 4);
        CVisDMatrix Aggtjk = CVisDMatrix(4, 4);

        // diagonal terms are zero; transpose of Mij is used. 
        CVisDMatrix transposej = Mij.Transposed();
		Aggtjj = transposej * Mij;

        CVisDMatrix transposek = Mik.Transposed(); 
		Aggtkk = transposek * Mik;
		Aggtjk = transposej * Mik;

		egj = transposej * eXi;
		egk = transposek * eXi;

        // four 4x4 blocks needs to be updated
        // add Aggtjj at j-j block
        // add Aggtjk at j-k and k-j blocks 
        // add Aggtkk at k-k block of HMatrix
        // add egj and egk at j and k block of yVector
        for (jj = 0; jj < 4; jj++)
        {
            y[4*j+jj] += egj[jj]; 
            y[4*k+jj] += egk[jj]; 
            for (kk = 0; kk < 4; kk++)
            {
                H[4*j+jj][4*j+kk] += Aggtjj[jj][kk]; // j-j-th 4x4 block
                H[4*k+jj][4*k+kk] += Aggtkk[jj][kk]; // j-k-th 4x4 block

                H[4*j+jj][4*k+kk] += Aggtjk[jj][kk]; // j-k-th 4x4 block
                H[4*k+jj][4*j+kk] += Aggtjk[kk][jj]; // k-j-th 4x4 block
            }
        } // for jj

        assert(H.IsSymmetric()); 
    } // for c

    // Report on errors
#ifdef _DEBUG
    double eXRMS = sqrt(eXt / (eXc + (eXc == 0.0)));
    char msg[1024];
    sprintf(msg, "RMS error (%d,%d) = %.4f", j, k, eXRMS);
    CVisDiagnostics::ProgressReport(msg);
#endif
    eXtotal += eXt, eXcount += eXc;
}

// note that M[2][j] = 0 and M[i][2] = 0 but M[2][2] = 1
// third row and third column
static void HomographyNormalized(CVisTransform4x4 &M)
{
	if (M[3][3] != 0)
	{
		double k = 1.0 / M[3][3]; 
		for (int i = 0; i < 4; i++)
		{
			if (i == 2) 
				continue; 
			for (int j = 0; j < 4; j++)
			{
				if (j == 2) 
					continue; 
				M[i][j] *= k; 
			}
		}
		M[3][3] = 1.0; // to be safe
	}
}

static void HomographyProjectPoint(const CVisTransform4x4 &M, 
								   const CVisDVector &input, 
								   CVisDVector &output)
{
	double x = input[0], y = input[1]; 
	double denom = M[3][0] * x + M[3][1] * y + M[3][3]; 
	
	if (denom != 0)
	{
		denom = 1.0 / denom; 
	}
	
	output[0] = (M[0][0] * x + M[0][1] * y + M[0][3]) * denom; 
	output[1] = (M[1][0] * x + M[1][1] * y + M[1][3]) * denom; 
}

// need to input Mj and Mk to compute gradient wrt 8-parameters
static void AccumulatePairGradientHessianPerspective(
													 int j, int k, SVisBlockMatchList &l, 
													 CVisDMatrix& H, CVisDVector& vy, 
													 float cx, float cy, 
													 const CVisTransform4x4 &Mj, 
													 const CVisTransform4x4 &Mk)
{
	int nParams = 8; 
#if 0
	CVisTransform4x4 Mjk = Mj * Mk.Inverted(); 
	CVisTransform4x4 MjkInv = Mjk.Inverted(); 
#else
	CVisTransform4x4 MjkInv = Mj * Mk.Inverted(); 
	CVisTransform4x4 Mjk = MjkInv.Inverted(); 
#endif
//	HomographyNormalized(Mjk); 
//	HomographyNormalized(MjkInv); 

    CVisDVector nXij = CVisDVector(2); 
    CVisDVector nXik = CVisDVector(2); 
    CVisDVector  Xij = CVisDVector(2); 
    CVisDVector  Xik = CVisDVector(2); 
    CVisDVector eXi  = CVisDVector(2); 

    CVisDMatrix Hij = CVisDMatrix(2, nParams); 
    CVisDMatrix Hik = CVisDMatrix(2, nParams); 
    CVisDMatrix Hik2 = CVisDMatrix(2, nParams); 
	CVisDMatrix J  = CVisDMatrix(nParams, nParams); 

    int jj, kk; 

    // loop through all grids whenever there is a valid one
    // add its effect to HMatrix and yVector
    // to solve the linear eq   H x = y
    // see notes on 3-22 and 3-23-97
    for (int c = 0; c < l.n; c++) 
    {

        SVisBlockMatchPair& pBp = l.nbp[c]; 
		// get the corresponding ij and ik points
        nXij[0] = pBp.uij[0]; // - cx; 
        nXij[1] = pBp.uij[1]; // - cy; 

        nXik[0] = pBp.uik[0]; // - cx; 
        nXik[1] = pBp.uik[1]; // - cy; 
	
        // get Xij, Xik
		// Xij is the same as nXij; Xik is nXik projected by Mjk
		HomographyProjectPoint(Mjk, nXik, Xik); 
		//HomographyProjectPoint(MjkInv, nXik, Xik); 
        for (jj=0; jj<2; jj++)
        {
            eXi[jj]   = nXij[jj] - Xik[jj]; 
        }

		// get Hij from Xik
		Hij = 0.0; // set all elements to zeros. 
		//double x = nXij[0], y = nXij[1]; 
		double x = Xik[0], y = Xik[1]; 
		Hij[0][0] = x; Hij[0][1] = y; Hij[0][2] = 1.0; Hij[0][6] = -x*x; Hij[0][7] = -x*y; //Hij[0][8] = -x; 
		Hij[1][3] = x; Hij[1][4] = y; Hij[1][5] = 1.0; Hij[1][6] = -x*y; Hij[1][7] = -y*y; //Hij[1][8] = -y; 
/*
		double x2 = Xik[0], y2 = Xik[1]; 
		Hik2[0][0] = x2; Hik2[0][1] = y2; Hik2[0][2] = 1.0; Hik2[0][6] = -x2*x2; Hik2[0][7] = -x2*y2; //Hij[0][8] = -x; 
		Hik2[1][3] = x2; Hik2[1][4] = y2; Hik2[1][5] = 1.0; Hik2[1][6] = -x2*y2; Hik2[1][7] = -y2*y2; //Hij[1][8] = -y; 
*/
		// get J = d djk / d dk (Eq. 62 of tech report)
		// see harry's note 8/31/98

		// get Hik from Hij and J
		J = 0.0; 
		double a11 = Mjk[0][0], a12 = Mjk[0][1], a13 = Mjk[0][3]; 
		double a21 = Mjk[1][0], a22 = Mjk[1][1], a23 = Mjk[1][3]; 
		double a31 = Mjk[3][0], a32 = Mjk[3][1], a33 = Mjk[3][3]; 

		double b11 = MjkInv[0][0], b12 = MjkInv[0][1], b13 = MjkInv[0][3]; 
		double b21 = MjkInv[1][0], b22 = MjkInv[1][1], b23 = MjkInv[1][3]; 
		double b31 = MjkInv[3][0], b32 = MjkInv[3][1], b33 = MjkInv[3][3]; 

		J[0][0] = a11*b11; J[0][1] = a11*b21; J[0][2] = a11*b31;
		J[0][3] = a12*b11; J[0][4] = a12*b21; J[0][5] = a12*b31;
		J[0][6] = a13*b11; J[0][7] = a13*b21; //J[0][8] = a13*b31; 

		J[1][0] = a11*b12; J[1][1] = a11*b22; J[1][2] = a11*b32;
		J[1][3] = a12*b12; J[1][4] = a12*b22; J[1][5] = a12*b32;
		J[1][6] = a13*b12; J[1][7] = a13*b22; //J[1][8] = a13*b32; 

		J[2][0] = a11*b13; J[2][1] = a11*b23; J[2][2] = a11*b33;
		J[2][3] = a12*b13; J[2][4] = a12*b23; J[2][5] = a12*b33;
		J[2][6] = a13*b13; J[2][7] = a13*b23; //J[2][8] = a13*b33; 


		J[3][0] = a21*b11; J[3][1] = a21*b21; J[3][2] = a21*b31;
		J[3][3] = a22*b11; J[3][4] = a22*b21; J[3][5] = a22*b31;
		J[3][6] = a23*b11; J[3][7] = a23*b21; //J[3][8] = a13*b31; 

		J[4][0] = a21*b12; J[4][1] = a21*b22; J[4][2] = a21*b32;
		J[4][3] = a22*b12; J[4][4] = a22*b22; J[4][5] = a22*b32;
		J[4][6] = a23*b12; J[4][7] = a23*b22; //J[4][8] = a23*b32; 

		J[5][0] = a21*b13; J[5][1] = a21*b23; J[5][2] = a21*b33;
		J[5][3] = a22*b13; J[5][4] = a22*b23; J[5][5] = a22*b33;
		J[5][6] = a23*b13; J[5][7] = a23*b23; //J[2][8] = a23*b33; 


		J[6][0] = a31*b11; J[6][1] = a31*b21; J[6][2] = a31*b31;
		J[6][3] = a32*b11; J[6][4] = a32*b21; J[6][5] = a32*b31;
		J[6][6] = a33*b11; J[6][7] = a33*b21; //J[6][8] = a33*b31; 

		J[7][0] = a31*b12; J[7][1] = a31*b22; J[7][2] = a31*b32;
		J[7][3] = a32*b12; J[7][4] = a32*b22; J[7][5] = a32*b32;
		J[7][6] = a33*b12; J[7][7] = a33*b22; //J[7][8] = a33*b32; 
/*
		J[8][0] = a31*b13; J[8][1] = a31*b23; J[8][2] = a31*b33;
		J[8][3] = a32*b13; J[8][4] = a32*b23; J[8][5] = a32*b33;
		J[8][6] = a33*b13; J[8][7] = a33*b23; //J[8][8] = a33*b33; 
*/
		Hik = Hij * J;         
        
        CVisDVector egj = CVisDVector(nParams); 
        CVisDVector egk = CVisDVector(nParams); 
        CVisDMatrix Aggtjj = CVisDMatrix(nParams, nParams); 
        CVisDMatrix Aggtkk = CVisDMatrix(nParams, nParams);
        CVisDMatrix Aggtjk = CVisDMatrix(nParams, nParams);

        // diagonal terms are zero; transpose of Mij is used. 
        CVisDMatrix transposej = Hij.Transposed();			// 8 x 2
		Aggtjj = transposej * Hij;							// 8 x 8

        CVisDMatrix transposek = Hik.Transposed();			// 8 x 2
		Aggtkk = transposek * Hik;							// 8 x 8
		Aggtjk = transposej * Hik;							// 8 x 8

		egj = transposej * eXi;								// 8 x 1
		egk = transposek * eXi;								// 8 x 1

        // four 4x4 blocks needs to be updated
        // add Aggtjj at j-j block
        // add Aggtjk at j-k and k-j blocks 
        // add Aggtkk at k-k block of HMatrix
        // add egj and egk at j and k block of yVector

        for (jj = 0; jj < nParams; jj++)
        {
            for (kk = 0; kk < nParams; kk++)
            {
                H[nParams*j+jj][nParams*j+kk] += Aggtjj[jj][kk]; // j-j-th 4x4 block
                H[nParams*k+jj][nParams*k+kk] += Aggtkk[jj][kk]; // j-k-th 4x4 block

                H[nParams*j+jj][nParams*k+kk] += Aggtjk[jj][kk]; // j-k-th 4x4 block
                H[nParams*k+jj][nParams*j+kk] += Aggtjk[kk][jj]; // k-j-th 4x4 block
            }
			egj[jj] = 0; 
            vy[nParams*j+jj] += egj[jj]; 
            vy[nParams*k+jj] += egk[jj]; 
        } // for jj

        assert(H.IsSymmetric()); 
    } // for c
}
													 

template <class F, class I>
void CVisBlockAdjuster<F,I>::AccumulateFrames()
{
    int n = NFrames(); 
	int m; // unknown parameters

	switch( motion_model )
	{
	case VIS_ALIGNMENT_PROJECTIVE:
		m = 8; 
		break; 

	case VIS_ALIGNMENT_FREEROTATION:
		m = 4; 
		// focal length has to be given before running m_adjuster.AccumulateFrames(). 
	    assert(m_f0 > 0 && m_f1 > 0); 
		break; 

	default:
		m = 0; 
		break; 
	}

	int mn = m*n; 

    // Initialize the gradient and Hessian accumulators
    m_x.Resize(mn); 
    m_y.Resize(mn); 
    m_H.Resize(mn, mn); 

    // initialize each element to zero
    m_y = 0.0; 
    m_H = 0.0; 

    //for (int i = 0; i < n*4; i++)
    //    (*m_H)[i][i] = 1.0;  // just to be on the safe side...

    //m_composite->FocalLength() = FFF; 
    // should not need this if we bring up properties dialog before blockadjust
    //VisExtractTVRVT(*m_composite); 
    CVisShape s = (*m_composite)[0].Shape(); // if all images have same size
    double cx = 0.5*(s.left + s.right - 1);
    double cy = 0.5*(s.top + s.bottom - 1); 

	// revisit this later. harry 9/1/98
    for (int i=0; i<n; i++)
    {
        CVisTransformChain &g = (*m_composite)[i].TransformChain(); 
        assert(g.size() == 3 || g.size() == 1); // RVT format only or perspective mode

        int k = m_composite->FindPreviousSame(i); 
        // force the i-th frame to use the same geometry as k-th
        if (k >= 0)
			// g = CVisTransformChain((*m_composite)[k].TransformChain()); 
            // a new copy is used 
            g = (*m_composite)[k].TransformChain(); 

    }

    eXtotal = eXcount = 0.0;
	// block adjuster can be called a few times to refine the result. 
	int j; 
	switch( motion_model )
	{
	case VIS_ALIGNMENT_PROJECTIVE:
		// normalize homography
		for (j = 0; j < n; j++) 
		{
			CVisTransform4x4 &M = (*m_composite)[j].TransformChain()[0]; 
			HomographyNormalized(M); 
		}

		// current Mj and Mk are used for accumulation
		for (j = 0; j < n; j++) 
		{
			CVisTransform4x4 Mj = (*m_composite)[j].TransformChain()[0]; 
			//CVisTransform4x4 MjInv = Mj.Inverted(); 

			for (int k = 0; k < n; k++) 
			{
				if (j == k || !m_match_list.Pixel(k, j).overlap)
					continue;

				CVisTransform4x4 Mk = (*m_composite)[k].TransformChain()[0]; 
				//CVisTransform4x4 MkInv = Mk.Inverted(); 

				AccumulatePairGradientHessianPerspective(j, k, m_match_list.Pixel(k, j), 
					m_H, m_y, cx, cy, Mj, Mk);
			}
		}
		break; 
	case VIS_ALIGNMENT_FREEROTATION:
		// current Rj and Rk are used for Accumulate... 
		for (j = 0; j < n; j++) 
		{
			// CVisTransform4x4 Rj = (*m_composite)[j].TransformChain().Next().Next()[0]; 
			CVisTransform4x4 Rj = (*m_composite)[j].TransformChain()[0]; 
			CVisTransform4x4 RjInv = Rj.Inverted(); 

			for (int k = 0; k < n; k++) 
			//for (int k = j+1; k < n; k++) 
			{
				if (j == k || !m_match_list.Pixel(k, j).overlap)
					continue;

				CVisTransform4x4 Rk = (*m_composite)[k].TransformChain()[0]; 
				CVisTransform4x4 RkInv = Rk.Inverted(); 
				CVisTransform4x4 Identity; 

				AccumulatePairGradientHessian(j, k, m_match_list.Pixel(k, j), 
					m_H, m_y, m_f0, m_f1, cx, cy, Rj, Rk);
			}
		}
		break; 
	default:
		break; 
	}

    // Report on errors
    double eXRMS = sqrt(eXtotal / (eXcount + (eXcount == 0.0)));
    char msg[1024];
    sprintf(msg, "RMS error (all frames) = %.4f", eXRMS);
    CVisDiagnostics::ProgressReport(msg);
}

static void ConstrainedLastIsFirst(CVisDMatrix &A, CVisDVector &b, int nFrames)
{
    int n = 4*nFrames; 
    CVisDVector L(n+1); 

    L = 0.0; 

    int k = 4*(nFrames - 1); 
    L[0] = -1.0, L[k] = 1.0; 
    ConstrainedSystem(A, b, L, k, n); 
    L[0] = 0.0, L[k] = 0.0;

    k++; 
    L[1] = -1.0, L[k] = 1.0; 
    ConstrainedSystem(A, b, L, k, n); 
    L[1] = 0.0, L[k] = 0.0;

    k++; 
    L[2] = -1.0, L[k] = 1.0; 
    ConstrainedSystem(A, b, L, k, n); 
    L[2] = 0.0, L[k] = 0.0;

    k++; 
    L[3] = -1.0, L[k] = 1.0; 
    ConstrainedSystem(A, b, L, k, n); 
    L[3] = 0.0, L[k] = 0.0;

}

static void ConstrainedJEqualsK(CVisDMatrix &A, CVisDVector &b, int nFrames, int nParams, int j, int k)
{
    int size = nParams*nFrames; 
    CVisDVector L(size+1); 

    L = 0.0; 

    for (int jj = nParams*j, kk = nParams*k; jj < nParams*(j+1); jj++, kk++)
    {
        L[jj] = 1.0, L[kk] = -1.0; 
        ConstrainedSystem(A, b, L, jj, size); 
        L[jj] = 0.0, L[kk] = 0.0;
    }
}

static void ConstrainedFirstIdentity(CVisDMatrix &A, CVisDVector &b, int nFrames, int nParams)
{
    int size = nParams*nFrames; 
    CVisDVector L(size + 1); 

    L = 0.0; 

#if 0
    // omega_00 = 0
    L[0] = 1.0; 
    ConstrainedSystem(A, b, L, 0, size); 
    // omega_01 = 0
    L[0] = 0.0, L[1] = 1.0; 
    ConstrainedSystem(A, b, L, 1, size); 
    // omega_02 = 0
    L[1] = 0.0; L[2] = 1.0; 
    ConstrainedSystem(A, b, L, 2, size); 
#else

	for (int i = 0; i < nParams; i++)
	{
		L[i] = 1.0; 
		ConstrainedSystem(A, b, L, i, size); 
		L[i] = 0.0; 
	}
#endif

}

static void ConstrainedKnownFocalLength(CVisDMatrix &A, CVisDVector &b, int nFrames, double f)
{
    int n = 4*nFrames; 
    int d = n + 1; 
    CVisDVector L(d); 
    int i; 

    L = 0.0; 
    // the constraint should be df = 0, not f = m_f0
    //L[n] = f; 
    L[n] = 0.0; // just for reminder

    for (i=0; i<nFrames; i++)
    {
        // f_i = f
        int k = i*4+3; 
        L[k] = 1.0; 
        ConstrainedSystem(A, b, L, k, n);
        L[k] = 0; 
    }
}

static void ConstrainedSameFocalLength(CVisDMatrix &A, CVisDVector &b, int nFrames)
{
    int n = 4*nFrames; 
    int d = n + 1; 
    CVisDVector L(d); 
    int i; 

    L = 0.0; 
    L[3] = -1.0; 

    for (i=1; i<nFrames; i++)
    {
        // f_0 = f_i, i = 1, ..., n-1
        int k = i*4 + 3; 
        L[k] = 1.0; 
        ConstrainedSystem(A, b, L, k, n);
        L[k] = 0; 
    }
}


template <class F, class I>
void CVisBlockAdjuster<F,I>::BlockSolver()
{
    int nFrames = NFrames(); 
	int nParams; // unknown parameters

	switch( motion_model )
	{
	case VIS_ALIGNMENT_PROJECTIVE:
		nParams = 8; 
		break; 

	case VIS_ALIGNMENT_FREEROTATION:
		nParams = 4; 
		// focal length has to be given before running m_adjuster.AccumulateFrames(). 
	    assert(m_f0 > 0 && m_f1 > 0); 
		break; 

	default:
		nParams = 0; 
		break; 
	}

	int size = nParams*nFrames; 

    // Form the CVisDMatrix and CVisDVector, and call the linear solver
    CVisDMatrix& Am = m_H;
    CVisDVector& bv = m_y; 
    CVisDVector mv(size);
    mv = 0.0;

    assert(Am.IsSymmetric());

    m_nAdjustParameters = 1; 

	if (motion_model == VIS_ALIGNMENT_FREEROTATION)
	{
		if (m_nAdjustParameters == 0)
		{
			// all focal length are the same and known as m_f0
			ConstrainedKnownFocalLength(Am, bv, nFrames, m_f0);
			// do we want to swap rows s.t. rotation matrices are on top
			// because there are essentially 3n unknowns
			//size = 3*n; 
		}
		else if (m_nAdjustParameters == 1)
		{
			// all focal length are same but unknown
			ConstrainedSameFocalLength(Am, bv, nFrames); 
			//size = 3*n + 1; 
		}

		assert(Am.IsSymmetric());
	}

    // enforce the first transform is identity
    ConstrainedFirstIdentity(Am, bv, nFrames, nParams); 

    assert(Am.IsSymmetric());

#if 1
    int j, k; 
    // FindNextSame() only finds one file (k) with same name (j)
    for (k=1; k<nFrames; k++)    {
        j = m_composite->FindPreviousSame(k); 
        if (j >= 0)
            ConstrainedJEqualsK(Am, bv, nFrames, nParams, j, k);
    }
    assert(Am.IsSymmetric()); 
#else
    
    ConstrainedLastIsFirst(Am, bv, nFrames); 
    assert(Am.IsSymmetric());
#endif

    VisDMatrixSolveSPD(Am, bv, mv, size);

//    float step_size = 1.0; 
	double step_size = 0.5; 
	if ( motion_model == VIS_ALIGNMENT_PROJECTIVE)
		step_size = 0.001; 

    for (int i = 0; i < size; i++)
        m_x[i] = step_size * mv[i]; 
}    

template <class F, class I>
void CVisBlockAdjuster<F,I>::RestoreGeometry()
{
    // set the new geometry/transform back for each frame
    int n = NFrames(); 

	if (motion_model == VIS_ALIGNMENT_FREEROTATION)
	{
		// the first rotation remains identity
		for (int i=0; i<n; i++)
		{
			CVisTransformChain &g = (*m_composite)[i].TransformChain(); 

			CVisTransform4x4 &V = g[1]; 
			//CVisTransform4x4 &V2 =g.Next().Next().Next()[0]; 

			//if (i > 0)
			if (i >= 0)
			{
				// CVisTransform4x4 &R = g.Next().Next()[0]; 
				CVisTransform4x4 &R = g[0]; 
				CVisTransform4x4 dR = RodriguesFormula(m_x[i*4], m_x[i*4+1], m_x[i*4+2]); 
				R = dR * R; 
			}

			// need to rewrite this if different f's
			// a very subtle bug here for same frames j and k
			// because same geometry addresses are used!
			// if (m_nAdjustParameters != 0 && (m_composite->FindPreviousSame(i) == -1))
			// not longer needed to check same j,k because fresh copy is used
			if (m_nAdjustParameters != 0)
			{
				/*
				V[0][0] += m_x[i*4+3]; 
				V[1][1] += m_x[i*4+3]; 
            
				if (V[0][0] > 0.0f && V[1][1] > 0.0f)
				{
					V2[0][0] = 1.0f / V[0][0]; 
					V2[1][1] = 1.0f / V[1][1]; 
				}*/
				double f = 1.0/V[0][0]; 
				f += m_x[i*4+3]; 
				assert(f > 0); 
				V[0][0] = 1.0/f; 
				V[1][1] = V[0][0];
			}

			if (i == 1)
				//m_f0 = m_f1 = V[0][0]; 
				m_f0 = m_f1 = 1.0/V[0][0]; 
		}
    }
	else if (motion_model == VIS_ALIGNMENT_PROJECTIVE)
	{
		int index = 0; 
		for (int i=0; i<n; i++)
		{
			CVisTransformChain &g = (*m_composite)[i].TransformChain(); 
			CVisTransform4x4 &t = g[0]; 
			t[0][0] += m_x[index++]; 
			t[0][1] += m_x[index++]; 
			t[0][3] += m_x[index++];
			t[1][0] += m_x[index++];
			t[1][1] += m_x[index++];
			t[1][3] += m_x[index++];
			t[3][0] += m_x[index++];
			t[3][1] += m_x[index++];
		}
	}

    // stripe out focal length and rotation matrix from xVector
    // assign them properly into geometry chain TVRVT at each frame
}

//
// De-ghosting (local warp adjustment) routines
//

template <class F, class I>
void CVisBlockAdjuster<F,I>::AccumulatePatchMotion(SVisBlockMatchList &l,
                                                   const CVisFloatImage &patch_flow)
{
    for (int i = 0; i < l.n; i++)
    {
        SVisBlockMatchPair& pBp = l.nbp[i];
        if (! pBp.valid )
            continue;
        SVisPatchFlow& pf = *(SVisPatchFlow *) &patch_flow.Pixel(pBp.bc, pBp.br, 0);
        memcpy(&pf.C[0][0], &pBp.C[0][0], 4*sizeof(float));
        pf.u += pBp.uuik[0] - pBp.uij[0];
        pf.v += pBp.uuik[1] - pBp.uij[1];
        pf.x += pBp.uij[0];
        pf.y += pBp.uij[1];
        pf.n++;
    }
}

template <class F, class I>
CVisFloatImage CVisBlockAdjuster<F,I>::ComputePatchMotion(int j)
{
    // Allocate a per-patch motion field (see VisCylindrical.h)
    I& img_j = m_composite->Warped(j);
    CVisShape patch_shape = img_j.Shape().Decimated(subsample, true);
    patch_shape.SetNBands(sizeof(SVisPatchFlow) / sizeof(float));
    CVisFloatImage patch_flow(patch_shape);
    patch_flow.ClearPixels();

    // Accumulate candidate positions from each overlapping frame
    for (int k = 0; k < NFrames(); k++) {
        SVisBlockMatchList &l = m_match_list.Pixel(k, j);
        if (j != k && l.overlap && l.n > 0)
            AccumulatePatchMotion(l, patch_flow);
    }

    // Compute the average new location
    for (int r = patch_flow.Top(); r < patch_flow.Bottom(); r++) {
        SVisPatchFlow *pf = (SVisPatchFlow *) patch_flow.PtrToFirstPixelInRow(r);
        for (int c = patch_flow.Left(); c < patch_flow.Right(); c++, pf++) {
            if (pf->n > 0.0f) {
                pf->u /= (pf->n + 1);
                pf->v /= (pf->n + 1);
                pf->x /= pf->n;
                pf->y /= pf->n;
            }
        }
    }

    return patch_flow;
}

template <class F, class I>
void CVisBlockAdjuster<F,I>::DeGhostAll(bool ignore_covariances)
{
    // If in global warp mode, need to allocate an array of patch_flow's
    CVisFloatImage *patch_flow_array =
        new CVisFloatImage[m_composite->NFrames()];

    // Adjust each frame in turn
    for (int j = 0; j < NFrames(); j++) {

        // Progress report
        m_composite->ProgressReport("DeGhosting frame %d", j);

        // Compute the per-patch motion field
        CVisFloatImage patch_flow = ComputePatchMotion(j);
        if (m_composite->LocalWarpMode() == 2)
            patch_flow_array[j] = patch_flow;

        // Adjust the local warp field
        if (m_composite->LocalWarpMode() == 1)
            m_composite->UpdateLocalWarp(j, patch_flow, subsample, ignore_covariances);
    }

    // In global warp mode, now do the global update
    if (m_composite->LocalWarpMode() == 2)
        m_composite->UpdateLocalWarp(0, patch_flow_array[0], subsample, ignore_covariances);

    // Invalidate the warps
    m_composite->InvalidateWarp(2); // actually, just invalidate the images
    delete [m_composite->NFrames()] patch_flow_array;
}

//
// Explicit template instantiation
//

// what is this pixel type? 
//TVisPixel CVisBlockMatchImage::class_pixel_type = VIS_PIXEL_FLOAT;

#pragma warning(disable : 4660)  // template class already instantiated

template CVisRGBABlockAdjuster;
// template CVisGrayBlockAdjuster; 

#pragma warning(default : 4660)
