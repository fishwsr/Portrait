///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisBlockAdjust.h -- panoramic image alignment adjustment
//
// DESCRIPTION
//  The routines in this file can be used to perform a "block adjustment"
//  to the initial alignment produced by VisAlign.cpp.
//
//  The algorithm works like this:
//  
//  Step A: preprocessing to obtain a collection of {u_ij}
// 
//  1.  for each frame j
//  2.      for each frame k that overlaps frame j
//  3.          warp frame k into j's coordinate system
//  4.              for each i (8x8 grid) inside the intersection frame j and frame k
//  5.                  do a patch translation alignment to find u_ik^{~}
//  6.                  convert u_ik^{~} back to u_ik (in frame k coordinate)
//  7.                  that corresponds u_ij (in frame j coordinate)
//
//  Step B: minimization using the collection of {u_ij}
//
//  1.  min || u_ij - R_j^{k} u_ik||    -> relative orientation
//  2.  min || u_ij - R_j x_i|| C_ij    -> unit normal least-squares (C_ij is confidence)
//  3.  min || u_ij - V R_j V x_i|| C_ij    -> scale minimization
//      step 3 is optional and only necessary when f is unknown. 
//
// SEE ALSO
//  VisBlockAdjust.cpp  implementation
//  VisAlign.h          description of aligner class
//  VisGeometry.h       geometric transformations
//  VisResample.h       global geometric warping
//  VisTrack.h          a tracker implementation precursor to block adjust
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

#ifndef _VIS_BLOCK_ADJUST_H_
#define _VIS_BLOCK_ADJUST_H_
                         

#define     MAXFRAME4       200         // max number of frames is 50 

struct SVisBlockMatchPair 
{
    int br, bc;                         // block indices
    float   uij[2];                     // grid point i in frame j
    float   uuik[2];                    // matching point of frame k in frame j
    float   uik[2];                     // matching point of frame k
    float   C[2][2];                    // confidence measure (Hessian from aligner)
    int     valid; 
    CVisTransform4x4    *transform;      // u_ik = tranform * u_ij
};

struct SVisBlockMatchList 
{
    int     j;                          // initial frame
    int     k;                          // matched frame
    int     n;                         // total matched points for j and k
    int     overlap;                    // whether two images overlap
    SVisBlockMatchPair      *nbp;       // list of SVisBlockPair
}; 

typedef CVisImage<SVisBlockMatchList> CVisBlockMatchImage;

template <class TFrame, class TImage>
class CVisBlockAdjuster : public CVisAligner<TImage>
{
public:
    // Constructor and destructor
    CVisBlockAdjuster();
    ~CVisBlockAdjuster();

    // Initialize blockadjuster from a composite
    void Initialize(CVisWarpedComposite<TFrame::T_Pixel> *composite);

    // Frame info from sequence
    int NFrames()
        { return (m_composite) ? m_composite->NFrames() : 0; }
    int CurrentFrame()
        { return (m_composite) ? m_composite->CurrentFrame() : 0; }

    // Block adjustment routines
    void ConstructMatchList(SVisBlockMatchList &l, int level); 
    void ComputeMatch(int j, int k, SVisBlockMatchList &l);
    void ComputeMatchAll();
    void DeleteMatchAll();
    void BlockSolver(); 
    void RestoreGeometry();
    void AccumulateFrames(); 

    // De-ghosting routines
    void AccumulatePatchMotion(SVisBlockMatchList &l,
                               const CVisFloatImage &patch_flow);
    CVisFloatImage ComputePatchMotion(int j);
    void DeGhostAll(bool ignore_covariances = true);

    // number of matching pairs: +-2, or 0 (all pairs)
    int m_matchpairs;         // number of neighbors to be matched
    int m_match_only_0;     // match to frame 0 only (for stereo)

	// ietration steps 
	int m_iteration_steps; 

private:
    // Internal functions
    CVisBlockMatchImage m_match_list;
    CVisWarpedComposite<TFrame::T_Pixel> *m_composite;
    int m_br_offset, m_bc_offset;           // block offsets to trimmed fixed image
    CVisDMatrix m_H;                        // H x = y 
    CVisDVector m_x; 
    CVisDVector m_y; 
    int m_nAdjustParameters;                  // 3F or 3F+1 or 4F
};

typedef CVisBlockAdjuster<CVisRGBAByteCompositeFrame,CVisRGBAByteImage> CVisRGBABlockAdjuster;
typedef CVisBlockAdjuster<CVisByteCompositeFrame,CVisByteImage>           CVisGrayBlockAdjuster;


#endif // _VIS_BLOCK_ADJUST_H_
