///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisColorAdjust.cpp -- panoramic image color adjustment
//
// DESCRIPTION
//  The routines in this file perform color adjustment between 
//  overlapping images in a panorama.
//
//
// SEE ALSO
//  VisColorAdjust.h    description
//
// DESIGN
//  
//
// HISTORY
//  08-Aug-99  Matt Uyttendaele (mattu) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#include "VisImageProcPch.h"
#include "VisImageProc.h"

template <class F, class I>
CVisColorAdjuster<F,I>::CVisColorAdjuster()
{    
    // Clear out pointers
    m_composite     = 0;
}

template <class F, class I>
CVisColorAdjuster<F,I>::~CVisColorAdjuster()
{
}

// Initialize coloradjuster from a composite
template <class F, class I>
void CVisColorAdjuster<F,I>::Initialize(CVisWarpedComposite<TFrame::T_Pixel> *composite)
{
	m_composite = composite;
}

template <class I>
static Histogram( CVisRGBAByteImage& histo, I& src, I& matte )
{
	assert( matte.Shape() == src.Shape() );
	
	// count occurences of each value
	int redcount[256];
	int bluecount[256];
	int grncount[256];

	memset( redcount, 0, sizeof(redcount) );
	memset( grncount, 0, sizeof(grncount) );
	memset( bluecount, 0, sizeof(bluecount) );
	
	for( int y = matte.Shape().Top(); y < matte.Shape().Bottom(); y++ )
	{
	    for( int x = matte.Shape().Left(); x < matte.Shape().Right(); x++ )
		{
			if( matte.Pixel(x,y).A() != 0 )
			{
				redcount[src.Pixel(x,y).R()]++;
				grncount[src.Pixel(x,y).G()]++;
				bluecount[src.Pixel(x,y).B()]++;			
			}
		}
	}

	// scale to the max occurence
	int maxcnt = 0;
	for( int i=0; i<256; i++ )
	{
		if( redcount[i] > maxcnt )
			maxcnt = redcount[i];
		if( grncount[i] > maxcnt )
			maxcnt = grncount[i];
		if( bluecount[i] > maxcnt )
			maxcnt = bluecount[i];
	}

	histo.Allocate( 256, 256 );
	histo.FillPixels( CVisRGBABytePixel(0xFF, 0xFF, 0xFF, 0xFF) );

	for( i=0; i<256; i++ )
	{
		double scale = 255. / log((double)maxcnt);
		int ry = 255 - (int)(log((double)redcount[i])*scale);
		int gy = 255 - (int)(log((double)grncount[i])*scale);
		int by = 255 - (int)(log((double)bluecount[i])*scale);

#define clip0_255(a) ((a>255)? 255: (a<0)? 0: a)

		histo.Pixel(i,clip0_255(ry)) = CVisRGBABytePixel(0xFF, 0x00, 0x00, 0xFF);
		histo.Pixel(i,clip0_255(gy)) = CVisRGBABytePixel(0x00, 0xFF, 0x00, 0xFF);
		histo.Pixel(i,clip0_255(by)) = CVisRGBABytePixel(0x00, 0x00, 0xFF, 0xFF);
	}
}

template <class I>
static HistogramScatter( CVisRGBAByteImage& histo, I& src1, I& src2, I& matte, 
						 double alpha, double beta )
{
	assert( matte.Shape() == src1.Shape() );
	assert( matte.Shape() == src2.Shape() );
	
	// plot corresponding values
	histo.Allocate( 256, 256 );
	histo.FillPixels( CVisRGBABytePixel(0xFF, 0xFF, 0xFF, 0xFF) );
	
	for( int y = matte.Shape().Top(); y < matte.Shape().Bottom(); y++ )
	{
	    for( int x = matte.Shape().Left(); x < matte.Shape().Right(); x++ )
		{
			if( matte.Pixel(x,y).A() != 0 )
			{
#define YIQconv( r, g, b) (.3*(float)(r) + .59*(float)(g) + .11*(float)(b))
				int y1 = (int)YIQconv( src1.Pixel(x,y).R(), src1.Pixel(x,y).G(), src1.Pixel(x,y).B() ); 
				int y2 = (int)YIQconv( src2.Pixel(x,y).R(), src2.Pixel(x,y).G(), src2.Pixel(x,y).B() );

				y2 = (y1 + y2) / 2;

				histo.Pixel(clip0_255(y1),255-clip0_255(y2)) = CVisRGBABytePixel(0x00, 0x00, 0x00, 0xFF);
			}
		}
	}

	// plot the fit
	for( int x = 0; x < 255; x++ )
	{
		histo.Pixel(x,255-clip0_255((int)(alpha*(double)x+beta))) = 
			CVisRGBABytePixel(0xFF, 0x00, 0x00, 0xFF);
	}
}

template <class I>
static void FitColors(I& adj, I& target, I& matte, double& fAlpha, double& fBeta)
{
	double sum_adj2   = 0.;
	double sum_adj    = 0.;
	double sum_adjavg = 0.;
	double sum_avg    = 0.;
	double sum_n      = 0.;

    for( int y = matte.Shape().Top(); y < matte.Shape().Bottom(); y++ )
	{
	    for( int x = matte.Shape().Left(); x < matte.Shape().Right(); x++ )
		{
			if( matte.Pixel(x,y).A() != 0 )
			{
				double ya = YIQconv( adj.Pixel(x,y).R(), adj.Pixel(x,y).G(), adj.Pixel(x,y).B() );

				if( ya > 16. && ya < 240. )
				{
				    double yt = YIQconv( target.Pixel(x,y).R(), target.Pixel(x,y).G(), target.Pixel(x,y).B() );
    
				    double avg = (ya + yt) / 2.;
				
				    sum_adj2   += ya * ya;
				    sum_adj    += ya;
				    sum_adjavg += ya*avg; 
				    sum_avg    += avg;
					sum_n++;
				}
			}
		}
	}


	CVisDMatrix AtA = CVisDMatrix(2, 2); 
	AtA[0][0] = sum_adj2;
	AtA[0][1] = sum_adj;
	AtA[1][0] = sum_adj;
	AtA[1][1] = sum_n;

    CVisDMatrix Atb = CVisDMatrix(2, 1);
	Atb[0][0] = sum_adjavg;
	Atb[1][0] = sum_avg;

	CVisDMatrix res = AtA.Inverted() * Atb;
	fAlpha = res[0][0];
	fBeta  = res[1][0];
}

template <class F, class I>
void CVisColorAdjuster<F,I>::ComputeMatch(int j, int k, TVisColorDir dir )
{
    // Compute the relative transform
    CVisTransform4x4 t_j = 
		m_composite->Frame(j).TransformChain().CompleteTransform();
    CVisTransform4x4 t_k = 
		m_composite->Frame(k).TransformChain().CompleteTransform().Inverted();
    CVisTransform4x4 motion = t_k * t_j;

    // Trim the images against each other:
    //     This code simplified from CVisAligner::RecomputeBoundingBoxes()

    I undist_j = m_composite->Warped(j);
    I undist_k = m_composite->Warped(k);
 
    // get bbox on j
    CVisShape trimmed_shape_undist_j = undist_j.Shape();
    CVisShape resamp_shape_k = VisResampleShape(undist_k.Shape(), motion.Inverted());
    trimmed_shape_undist_j   = (trimmed_shape_undist_j & resamp_shape_k);
   
	// for adjacent frames any overlap is sufficient, otherwise overlap must
	// be at least 1/8 the size of the input image
	if ( !( (abs(j-k)==1 && 
		     trimmed_shape_undist_j.Width()  > 0 && 
			 trimmed_shape_undist_j.Height() > 0 ) ) )
	{

		int minw = undist_j.Shape().Width() >> 3;
		int minh = undist_j.Shape().Height() >> 3;

		if ( trimmed_shape_undist_j.Width()  < minw && 
			 trimmed_shape_undist_j.Height() < minh )
		{
		    m_composite->ProgressReport("Frame %d and frame %d do NOT overlap", j, k); 
            return; 
		}
	}

	// trim jth image to overlap region
	I trimmed_undist_j = 
        undist_j.SubImage(trimmed_shape_undist_j);

	I warped_undist_k(trimmed_shape_undist_j);
    VisResample(undist_k, warped_undist_k, 
		        motion, VIS_INTERP_LINEAR, 1);

	// store the color adjust results
	SColorAdjustParam& p = m_composite->At(j).ColorAdjust(dir);
	p.m_Overlap = trimmed_shape_undist_j;
	FitColors(trimmed_undist_j, warped_undist_k, warped_undist_k, 
		      p.m_fAlpha, p.m_fBeta );

#ifdef DEBUG_ALIGN
	char filename[1024];
    sprintf(filename, "C:\\temp\\clrj%d_%d.tga", j, k);
    trimmed_undist_j.WriteFile(filename);  
    sprintf(filename, "C:\\temp\\clrwrp%d_%d.tga", j, k);
    warped_undist_k.WriteFile(filename);
	
	CVisRGBAByteImage histo;

	//sprintf(filename, "C:\\temp\\histoj%d_%d.tga", j, k);
	//Histogram(histo, trimmed_undist_j, warped_undist_k);
	//histo.WriteFile(filename);
	//sprintf(filename, "C:\\temp\\histok%d_%d.tga", j, k);
	//Histogram(histo, warped_undist_k, warped_undist_k);
	//histo.WriteFile(filename);
	sprintf(filename, "C:\\temp\\histoscatter%d_%d.tga", j, k);
    HistogramScatter(histo, trimmed_undist_j, warped_undist_k, warped_undist_k, p.m_fAlpha, p.m_fBeta);
	histo.WriteFile(filename);
#endif
}

template <class F, class I>
void CVisColorAdjuster<F,I>::ComputeMatchAll()
{
    // Compute all of the pairwise matches
    for (int j = 0; j < NFrames(); j++)
	{
		// right neighbor
		if( !(NFrames() == 2 && j == 1) )
			ComputeMatch( j, (j==NFrames()-1)? 0: j+1, VIS_COLOR_RIGHT );

		// left neighbor
		if( !(NFrames() == 2 && j == 0) )
	        ComputeMatch( j, (j==0)? NFrames()-1: j-1, VIS_COLOR_LEFT );
    }

}

inline void PixelColorAdjust(CVisRGBABytePixel* pv, double alpha, double beta )
{
	int* pi = (int*)pv;

#define ColorAdjust( v ) ((double)(v)*alpha+beta)
	int r = ColorAdjust( ((*pi) & 0xFF0000) >> 16 );
	int g = ColorAdjust( ((*pi) & 0x00FF00) >> 8 );
	int b = ColorAdjust( ((*pi) & 0x0000FF) >> 0  );
#undef ColorAdjust

#define ClipColor( v )   ((int)(((v)>255.0)? 255.0: ((v)<0.0)? 0.0: (v)))
    *pi = ((*pi) & 0xFF000000) |
		  (ClipColor(r) << 16) | (ClipColor(g) << 8) | (ClipColor(b) << 0 );
#undef ClipColor

}

template <class TPixel>
static void LineColorAdjustGrad( TPixel *dst, int x_min, int x_max,
							     double alpha, double beta )
{
	for( int x = x_min; x < x_max; x++ )
	{
		PixelColorAdjust( dst, alpha, beta );
		dst++;
	}
}

template <class TPixel>
static void LineColorAdjustGrad( TPixel *dst, int x_min, int x_max,
								double alpha_left, double beta_left, 
								double alpha_right, double beta_right )
{			
	double alpha = alpha_left;
	double beta  = beta_left;
	double alpha_inc = (alpha_right-alpha_left)/(x_max-x_min);
	double beta_inc =  (beta_right-beta_left)/(x_max-x_min);
	
	for( int x = x_min; x < x_max; x++ )
	{
		PixelColorAdjust( dst, alpha, beta );
		alpha += alpha_inc;
		beta  += beta_inc;
		dst++;
	}
}


template <class TImage>
void VisAdjustColor( TImage &dst,
					 SColorAdjustParam& adjacent0,
					 SColorAdjustParam& adjacent1,
					 double fall_off)
{
	int x_min = 0, x_max = 0;
   
	double alpha_left  = 1.0, beta_left  = 0.0;
	double alpha_right = 1.0, beta_right = 0.0;

	if( (adjacent0.m_Overlap.Width() !=0 && adjacent0.m_Overlap.Height() != 0) )
	{
		x_min = dst.Left();
		x_max = dst.Right();
        alpha_left = adjacent0.m_fAlpha;
		beta_left  = adjacent0.m_fBeta;
	}
	
	if( (adjacent1.m_Overlap.Width() !=0 && adjacent1.m_Overlap.Height() != 0) )
	{
		x_min = dst.Left();
		x_max = dst.Right();
        alpha_right = adjacent1.m_fAlpha;
		beta_right  = adjacent1.m_fBeta;
	}

	if( x_min != x_max )
	{
 		for (int y = dst.Top(); y < dst.Bottom(); y++)
		{	
			LineColorAdjustGrad( &dst.Pixel(x_min, y, 0), x_min, x_max,
                                 alpha_left, beta_left, alpha_right, beta_right );
        }		
	}
}

#pragma warning(disable : 4660)  // template class already instantiated

template CVisRGBAColorAdjuster;
// template CVisGrayColorAdjuster; 

template void VisAdjustColor( CVisWarpedFrame<CVisRGBABytePixel> &dst,
					SColorAdjustParam& adjacent0,
					SColorAdjustParam& adjacent1,
					double fall_off);

#pragma warning(default : 4660)
