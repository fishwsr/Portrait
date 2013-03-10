///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisStereo.cpp -- compute depth map from collection of images
//
// DESCRIPTION
//  CVisStereo class is used to store a collection of images with associated
//  camera matrices (like CVisComposite), and to compute a depth map and
//  consensus color map from this collection.
//
// SEE ALSO
//  VisStereo.h         more detailed description of class and parameters
//  VisComposite.h      compositions / views of image collections
//  VisSprite.h         images and collections of images with depth
//
// BUGS
//   
//
// DESIGN
//  
//
// HISTORY
//  07-Mar-98  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////


#include "VisImageProcPch.h"
#include "VisImageProc.h"


//
// Constructors and destructor
//

CVisStereo::CVisStereo()
{
    m_error_type = VIS_STEREO_SSD;  // per-pixel error metric
    m_spatial_blur_steps = 0;       // # of 14641 convolutions (for now)
    m_error_threshold = -1.0;        // maximum error allowed for valid match
	m_regularization_steps = 0; 	// number of steps to regularize depth estimate

    m_d_min   = -64;                // minimum depth examined
    m_d_step  =   8;                // step in depths
    m_d_steps =  16;                // number of depths examined

    m_current_depth = 0.0;   // current depth plane being evaluated

	m_minNumVotes = 2;					// minimum number of contributing images to each voxel
	m_boxFilterSize = 0;		// size of box filter, disabled if zero
	m_spatialMinFilterSize = 0;     // size of spatial min filter, disabled if zero
	m_thresholdFilterSize = 2;		// size of threshold edge fattening filter, disabled if zero
	m_fillInInvalidErrors = true;		// fill in invalid errors with neighboring errors?
	// there's a bug in subpixel
	m_useSubpixel = false;				// compute subpixel depths?
	m_writeMeanImages = true;			// write mean images to files?
	m_writeErrorImages = false;		// write error images to files?
	m_writeConfidenceImage = true;		// write confidence image?
}

char *CVisStereo::MeanFilename(int d)
{
	static char name[200];
	sprintf(name, "bottom%02d.bmp", d);
	return name;
}

char *CVisStereo::ErrorFilename(int d)
{
	static char name[200];
	sprintf(name, "error%02d.bmp", d); 
	return name;
}

char *CVisStereo::ConfidenceFilename()
{
	static char name[200];
	sprintf(name, "conf.bmp"); 
	return name;
}

//  returns homography from depth plane at current_depth to image plane of image i
CVisTransform4x4 CVisStereo::DepthPlaneHomography(int i, double current_depth)
{
    // Compute homography from camera geometries and plane equation
    CVisTransform4x4 M0 = m_camera.CompleteTransform();

//	M0.Write(stdout);

    // Scaled normal equation goes in third row
    CVisVector4& n = PlaneEqn();
    for (int j = 0; j < 4; j++)
        M0[2][j] = n[j];

    // Inverse transformation from image to world
    CVisTransform4x4 P0 = M0.Inverted();

    // Add depth-based offset
    if (current_depth != 0.0) 
	{
        double d = current_depth * ZScale();
        for (int row = 0; row < 4; row++)
            P0[row][3] += d * P0[row][2];
    }

    // Camera matrix for "sprite" or "composite"
    CVisTransform4x4 M  = Frame(i).TransformChain().CompleteTransform(); 
        // we could also use m_view_geometry...
    // Final image to composite/sprite transformation
    CVisTransform4x4 H1 = M * P0;
#ifdef NOT_NEEDED   // I hope
    H1.Normalize();
#endif

    // Reset the third row and column to identity
    for (j = 0; j < 4; j++)
        H1[j][2] = H1[2][j] = (j == 2);

    return H1.Inverted();
}

double CVisStereo::DepthOfPlane(int d)
{
	return double(m_d_min + m_d_step * d);
}

CVisTransform4x4 CVisStereo::DepthPlaneHomography(int i, int d)
{
	double depth = DepthOfPlane(d);
	return DepthPlaneHomography(i, depth);
}


CVisTransform4x4 CVisStereo::LayerTransform(int i)
{
	return DepthPlaneHomography(i, m_current_depth);
}

//
//  Fill in the gaps in a cost image (due to out-of-bounds or invisible pixels)
//  See Rick's research notes, 16-Nov-98.
//
//  PullPush code taken from VisMotionEstimator for use in VisStereo
//  it probably should be placed somewhere on its own anyway..

void VisPullPush( CVisFloatImage& img0 )
{
    static int min_size = 16;
    if (img0.Width() < min_size || img0.Height() < min_size)
        return;

    //  Create a smaller image
    CVisFloatImage& kernel = VisConvolutionKernel_14641;
    CVisShape s2 = img0.Shape().Decimated(2);
    CVisFloatImage img2(s2);
    VisConvolveSeparable(img0, img2, kernel, kernel, 2);

    // Process that image
    VisPullPush(img2);

    // Expand back to original size
    CVisShape s1 = img0.Shape();
    CVisFloatImage img1(s1);
    VisConvolveSeparable(img2, img1, kernel, kernel, -2);

    // Take a per-pixel max, depending on weight
    int r, c, nrows = img0.Height(), ncols = img0.Width();
    for (r = 0; r < nrows; r++) {
        float *p0 = img0.PtrToFirstPixelInRow(r + img0.Top());
        float *p1 = img1.PtrToFirstPixelInRow(r + img1.Top());
        for (c = 0; c < ncols; c++, p0 += 2, p1 += 2) {
            if (p1[1] > p0[1]) {    // weight from coarse level is higher
                p0[0] = p1[0];
                p0[1] = p1[1];
            }
        }
    }
}

void VisPyramidalFillGaps( CVisFloatImage& cost )
{
    // Create a 2-banded image for costs and weights
    CVisShape s2 = cost.Shape();
    s2.SetNBands(2);
    CVisFloatImage wt_cost(s2);

    // Copy in the cost values
    int r, c, nrows = cost.Height(), ncols = cost.Width();
    for (r = 0; r < nrows; r++) {
        float *p0 = cost.PtrToFirstPixelInRow(r + cost.Top());
        float *p1 = wt_cost.PtrToFirstPixelInRow(r + wt_cost.Top());
        for (c = 0; c < ncols; c++, p0++, p1 += 2) {
            p1[1] = (p0[0] >= 0.0);     // negative value is invalid pixel
            p1[0] = p0[0] * p1[1];
        }
    }

    // Perform a Pull/Push operation, similar to the one in Lumigraph paper
    VisPullPush(wt_cost);

    // Normalize the results
    for (r = 0; r < nrows; r++) {
        float *p0 = cost.PtrToFirstPixelInRow(r + cost.Top());
        float *p1 = wt_cost.PtrToFirstPixelInRow(r + wt_cost.Top());
        for (c = 0; c < ncols; c++, p0++, p1 += 2) {
            p0[0] = p1[0];
            if (p1[1] != 1.0 && p1[0] > 0.0)
                p0[0] /= p1[1];
        }
    }
}

static inline float MinValInArray(float *arr, int start, int end, int stride = 1)
{
	float min = FLT_MAX;
	for(int i = start; i <= end; i++)
	{
		if(arr[i] < min)
		{
			min = arr[i*stride];
		}	
	}
	return min;
}

// ignore negative values? no
static void VisSpatialMinFilter( CVisFloatImage &im, int windowSize)
{
    CVisShape s2 = im.Shape();
    CVisFloatImage filtered(s2);

	// use two pass approach

	// compute mins over rows
    // Copy in the cost values
    int r, c, nrows = im.Height(), ncols = im.Width();
	int halfSize = windowSize/2;

    for (r = 0; r < nrows; r++) 
	{
        float *p0 = im.PtrToFirstPixelInRow(r);
        float *p1 = filtered.PtrToFirstPixelInRow(r);
        for (c = 0; c < ncols; c++) 
		{
			int startCol = ((c - halfSize) >= 0) ? (c - halfSize) : 0;
			int endCol = ((c + halfSize) < ncols) ? (c + halfSize) : ncols-1;
			p1[c] = MinValInArray(p0, startCol, endCol);
        }
    }
/*
    for (r = 0; r < nrows; r++) 
	{
        float *p0 = im.PtrToFirstPixelInRow(r);
        float *p1 = filtered.PtrToFirstPixelInRow(r);
        for (c = 0; c < ncols; c++) 
		{
			p0[c] = p1[c];
        }
    }
*/
	// compute mins over cols
	// this must be pretty inefficient
	for(c = 0; c < ncols; c++)
	{
		float *filteredCol = &filtered.PtrToFirstPixelInRow(0)[c];
		for(r = 0; r < nrows; r++)
		{
			int startRow = ((r - halfSize) >= 0) ? (r - halfSize) : 0;
			int endRow = ((r + halfSize) < nrows) ? (r + halfSize) : nrows-1;

			im.PtrToFirstPixelInRow(r)[c] = MinValInArray(filteredCol, startRow, endRow, ncols);
		}
	}
/*
    for (r = 0; r < nrows; r++) 
	{
        float *p0 = im.PtrToFirstPixelInRow(r);
        float *p1 = filtered.PtrToFirstPixelInRow(r);
        for (c = 0; c < ncols; c++) 
		{
			p0[c] = p1[c];
        }
    }
*/
}

//
// Compute error between layers and composite
//  The "mean" colors are in m_bottom_layer.
//  The warped input images are bands in m_layers.
//  We wish to compute the difference between each band in m_layers
//  and m_bottom_alpha, and then aggregate differences across all
//  bands.
//

struct VisIntCompare {  // compare two integers
    int operator()(int i1, int i2) {
        return i1 <= i2;
    }
};

void CVisStereo::ComputePerPixelError(int d)
{
    // Compute error from mean image (m_bottom_layer) and aggregate
    int ncols = m_layers.Width(), nbands = m_layers.NBands();
    int *buf = new int[nbands];     // for MAD computation

    // An optional bias-gain subtraction stage could go here...

    // Compute differences from mean and aggregate across layers
    for (int r = m_layers.Top(); r < m_layers.Bottom(); r++) 
	{
        CVisRGBABytePixel *layr = m_layers.PtrToFirstPixelInRow(r);
        CVisRGBABytePixel *mean = m_bottom_alpha.PtrToFirstPixelInRow(r);
        float             *errr = m_error.PtrToFirstPixelInRow(r);
        for (int c = 0; c < ncols; c++, layr += nbands) 
		{
			// shouldn't need following lines, counting votes should handle it
			// but leave in for performance
            errr[c] = -1.0f;
            if ((mean[c].A() != 255) && (m_minNumVotes > 0))  // composite is non-opaque
                continue;

            double sum = 0.0;
            int rm = mean[c].R(), gm = mean[c].G(), bm = mean[c].B();

            // Gather up all the opaque pixels
			for (int b = 0, n = 0; b < nbands; b++) 
			{
//                if (layr[b].A() == 255) 
				// if it's not completely transparent..
				if(layr[b].A() != 0)
				{
                    int rd = abs(int(layr[b].R() - rm));
                    int gd = abs(int(layr[b].G() - gm));
                    int bd = abs(int(layr[b].B() - bm));

                    // Optional difference limiting could go here...

                    // Aggregate across layers
                    switch (m_error_type) 
					{
                    case VIS_STEREO_SSD:
                        sum += rd*rd + gd*gd + bd*bd;
                        break;
                    case VIS_STEREO_SAD:
                        sum += rd + gd + bd;
                        break;
                    case VIS_STEREO_MAD:
                        buf[n] = (rd + gd + bd);
                        break;
                    }
                    n++;
                }
            }

			// record number of votes in alpha channel
			// 255 denotes all voted
			// 255 - x denotes that x images did not vote
			mean[c].SetA(255 - nbands + n);

			// if not enough images vote, set invalid error
			// set color for now to visualize
			if(n < m_minNumVotes)
			{
				errr[c] = -1.0f;
//				mean[c].SetR(255);
//				mean[c].SetG(0);
//				mean[c].SetB(0);
			}
			else   // we have enough votes
			{
				if (m_error_type == VIS_STEREO_MAD) 
				{
					if (n > 2)
						sum = VisMedian(VisIntCompare(), buf, n, n/2) / 3.0;
					else if (n > 0)
						sum = buf[0] / 3.0;
				} 
				else
				{
					// Normalize by n ???
					if (n > 1)
						sum /= 3.0 * n;
				}

				// if m_minNumVotes == 0, then sum should be 0
	            errr[c] = sum;
			}
//            if (n == 0)
//              mean[c].SetA(254);    // mark as failed match
        }
    }

	delete []buf;

	if(m_writeErrorImages)
	{
		CVisRGBAByteImage errorOne(m_error.Shape());
		for (int r = m_error.Top(); r < m_error.Bottom(); r++) 
		{
			float *errs  = m_error.PtrToFirstPixelInRow(r);
			CVisRGBABytePixel *e1 = errorOne.PtrToFirstPixelInRow(r); 
			for (int c = 0; c < ncols; c++) 
			{
				if(errs[c] < 0.0f)
				{
					e1[c].SetR(255);
					e1[c].SetG(0);
					e1[c].SetB(0);
				}
				else
				{
					int e2 = errs[c]/2;				// why divide by 2? because 2 images?
					e2 = (e2 > 255? 255 : e2); 
					e1[c].SetR(e2);
					e1[c].SetG(e2);
					e1[c].SetB(e2); 
				}
			}
//			char filename[50]; 
//			sprintf(filename, "error%02d_%02d.bmp", p, d); 
			errorOne.WriteFile(ErrorFilename(d)); 
		}
	}
}


//
// Compute error at a given disparity plane
//

#if 1
template <class TPixel>
void VisCopyBand(CVisImage<TPixel> img0, int b0,
                 CVisImage<TPixel> img1, int b1)
{
    int r, c, nrows = img0.Height(), ncols = img0.Width();
    int nbands0 = img0.NBands(), nbands1 = img1.NBands();
    // VisCheckConformal(&img0, &img1);
    for (r = 0; r < nrows; r++) {
        TPixel *p0 = img0.PtrToFirstPixelInRow(r + img0.Top()) + b0;
        TPixel *p1 = img1.PtrToFirstPixelInRow(r + img1.Top()) + b1;
        for (c = 0; c < ncols; c++, p0 += nbands0, p1 += nbands1) {
            *p0 = *p1;
        }
    }
}
#endif

void CVisStereo::ProcessPlane(int d)
{
    // Compute the composite/mean image (and warped layers)
//    m_current_depth = double(m_d_min + m_d_step * d);
    m_current_depth = DepthOfPlane(d);

#ifdef DEBUGGING
	printf("current depth is %lf\n", m_current_depth);
	// print out homographies
	for(int i = 0; i < NFrames(); i++)
	{
		CVisTransform4x4 h = LayerTransform(i);
		h.Write(stdout);
		printf("\n");
	}
#endif

    SetDisplayMode(VIS_COMPOSITE_VIEW_COMPLETE);
    SetInterpolation(VIS_INTERP_LINEAR);
    SetNeighborFrames(0);
	m_save_layers = 1; 
    CVisShape sh = Image().Shape();
//	SetCompositingOperator(composite_accumulate); 
//	SetCompositingOperator(composite_median); 
	Invalidate(); 
	printf("\tbottom layer..");
    FormBottomLayer(sh, 0);
	printf(" done.\n");
	m_n_accumulated = 0; // needed if composite_feather is used: modify VisComposite? 

    // Compute the per-pixel error
    ComputePerPixelError(d);

	// possibly fill in invalid error values with neighboring errors
	// using the PullPush algorithm
	if(m_fillInInvalidErrors)
	{
		VisPyramidalFillGaps(m_error);
	}

    // Convert into confidence and aggregate spatially...
    //  TODO: replace this with more general code later
	if(m_boxFilterSize > 0)
	{
		// build box filter kernel
		CVisFloatImage kernelImage(CVisRect(-m_boxFilterSize/2, 0, m_boxFilterSize/2 + 1, 1), 1, evisimoptDefault);
		float *k = kernelImage.PtrToFirstPixelInRow(0);
		for(int i = 0; i < m_boxFilterSize; i++)
			k[i] = 1.0f/m_boxFilterSize;

		VisConvolveSeparable(m_error, m_error, kernelImage, kernelImage, 1);
	}
	else
	{
		for (int b = 0; b < m_spatial_blur_steps; b++)
		{
			VisConvolveSeparable(m_error, m_error,
				VisConvolutionKernel_14641,
				VisConvolutionKernel_14641, 1);
		}
	}

	if(m_spatialMinFilterSize != 0)
	{
		VisSpatialMinFilter(m_error, m_spatialMinFilterSize);
	}

    // Store in multiband image of colors and confidences
//    VisCopyBand(m_errors, d, m_error, 0);
	m_error.CopyPixelsTo(0, d, m_errors);
//    VisCopyBand(m_colors, d, m_bottom_alpha, 0);
	m_bottom_alpha.CopyPixelsTo(0, d, m_colors);

//	char filename[50]; 
//	sprintf(filename, "bottom%02d_%02d.bmp", p, d);

    if (m_writeMeanImages)
	    m_bottom_alpha.WriteFile(MeanFilename(d));
}

void CVisStereo::DepthRegularization(CVisByteImage &copyDepth)
{
	int hSize = 2;		// regularization window size is 2*hSize + 1
	CVisByteImage &zim = copyDepth; 
	CVisByteImage &origzim = ZImage(); 
	int ncols = zim.Width(); 

	for (int r = zim.Top() + hSize; r < zim.Bottom() - hSize; r++)
	{
		unsigned char *depf1 = zim.PtrToFirstPixelInRow(r-1); 
		unsigned char *depth = zim.PtrToFirstPixelInRow(r); 
		unsigned char *depz1 = zim.PtrToFirstPixelInRow(r+1); 
		unsigned char *conf  = m_confidence.PtrToFirstPixelInRow(r); 
        CVisRGBABytePixel *color = Image().PtrToFirstPixelInRow(r);
		unsigned char *origdepth = origzim.PtrToFirstPixelInRow(r); 

		for (int c = hSize; c < ncols - hSize; c++)
		{
			if (color[c].A() <= 250) // != 255) //
			{
				continue;
			}

			if (conf[c] == 255)
			{
				continue; 
			}
			// can be done in integer arithmetic 
			// different summation? order dependent?
			float neighbors = 0.0; 
			float sumweight = 0.0; 
			int cDepth = c + zim.Left(); 
			int cConf  = c + m_confidence.Left(); 
			int rConf  = r - zim.Top() + m_confidence.Top(); 

			for (int rr = -hSize; rr <= hSize; rr++)
			{
				for (int cc = -hSize; cc <= hSize; cc++)
				{
					if (rr == 0 && cc == 0)
						continue; 
					neighbors += zim.Pixel(cDepth+cc, r+rr) * m_confidence.Pixel(cConf+cc, rConf+rr); 
					sumweight += m_confidence.Pixel(cConf+cc, rConf+rr); 
				}
			}

			if (sumweight != 0) 
				neighbors /= sumweight; 
			else
				neighbors = ( depf1[c-1] + depf1[c] + depf1[c+1]
							+ depth[c-1] 			+ depth[c+1]
							+ depz1[c-1] + depz1[c] + depz1[c+1])/8.0; 

			float weight = conf[c]/255.0; 
			depth[c] = (int) (weight * origdepth[c] + (1.0 - weight) * neighbors); 
		}
	}
}

void CVisStereo::WinnerTakeAll()
{
    // Find best (winning) disparity
    int ncols  = m_errors.Width();
    int nbands = m_errors.NBands();     // should == m_d_steps;

	CVisShape newShape = m_errors.Shape(); 
	newShape.SetNBands(1); 
	CVisRGBAByteImage errorOut(newShape); 
	CVisRGBAByteImage errorOne(newShape);

    // Sanity check for ZImage same shape as Image()
    CVisRGBAByteImage& img = Image();
    CVisByteImage& zimg = ZImage();
    if (img.Shape() != zimg.Shape())
        zimg.Allocate(img.Shape());

	// write floating point, multiband error image
	// this would be more useful as a sequence, let's convert it
#ifdef WRITE_FP_ERRORS	
    if (m_writeErrorImages)
	{
//	    m_errors.WriteFile("tmp_errors.msv");
		// create sequence
		CVisFloatSequence errorSeq;
		CVisShape errShape = m_errors.Shape();
		errShape.SetNBands(2);
		char errName[100];
		for(int b = 0; b < m_errors.NBands(); b++)
		{
			errorSeq.Push(errShape);
			sprintf(errName, "temp_error%2d.msv", b);
			errorSeq[b].SetName(errName);
			m_errors.CopyPixelsTo(b, 0, errorSeq[b]);
		}
		errorSeq.WriteSequenceInfo("tmp_errors.seq", true);
	}
#endif // WRITE_FP_ERRORS

    // Iterate over the pixels
    for (int r = m_errors.Top(); r < m_errors.Bottom(); r++) 
	{
        float              *errs  = m_errors.PtrToFirstPixelInRow(r);
        float              *errr  = m_error.PtrToFirstPixelInRow(r);
        CVisRGBABytePixel *colrs = m_colors.PtrToFirstPixelInRow(r);
        CVisRGBABytePixel *color = Image().PtrToFirstPixelInRow(r);
        unsigned char      *depth = ZImage().PtrToFirstPixelInRow(r);

		CVisRGBABytePixel *newerror = errorOut.PtrToFirstPixelInRow(r); 
		unsigned char	   *conf  = m_confidence.PtrToFirstPixelInRow(r); 

        for (int c = 0; c < ncols; c++, newerror++, errs += nbands, colrs += nbands) 
		{
			conf[c]  = 0; 

#ifdef IGNOREOUTSIDE
			if (color[c].A() <= 250) // != 255) //
			{
				depth[c] = 128;
				color[c] = CVisRGBABytePixel(0, 0, 0, 0);
				continue;
			}
#endif //IGNOREOUTSIDE

            depth[c] = 128;
            //color[c] = CVisRGBABytePixel(0, 0, 0, 0);
            int d_best = -1;
			
            // Iterate over the bands (depths) to find best match
			// only consider valid errors, i.e., those with enough votes
			int maxNumVoters = m_layers.NBands();
			for (int d = 0; d < nbands; d++) 
			{
//                if (colrs[d].A() == 255 && (d_best < 0 || errs[d] < errs[d_best]))
				// using error as a flag here is not a good idea
				// probably need the validity map, stored in m_colors alpha channel
                if ( (maxNumVoters - (255 - colrs[d].A()) >= m_minNumVotes) &&
					 (d_best < 0 || errs[d] < errs[d_best]) )
                   d_best = d;
            }

			// what to do if no d_best?
			// handled later
			if(d_best == -1)
			{

			}

			float minError = 0.0f; 
			double df = 0.0; 

			if (d_best >= 0)
			{
				minError = errs[d_best]; 
//				ASSERT( minError >= 0.0f );
			}

#if 1
            // "Sub-pixel" depth refinement
            if (m_error_type == VIS_STEREO_SSD &&
                (m_d_step != 1) && (m_d_step != -1) &&
//                d_best != 0 && d_best != m_d_steps-1) 
                (d_best > 0) && (d_best != m_d_steps-1))  // avoid d_best = -1
			{
                float e0 = errs[d_best-1];
                float e1 = errs[d_best  ];
                float e2 = errs[d_best+1];
                double a = 0.5 * (e0 + e2 - 2.0 * e1);

				e1 *= 0.125; // hack, how to properly scale the conf? 
				
				// 'a' should always be positive
				if (a > e1)
				{
					conf[c] = 255; 
				}
				else if (a > 0)
				{
					conf[c] = (int) (255 * a/e1); 
				}

				if(m_useSubpixel)
				{
					// using subpixel depth estimate to reduce number of bits
					double b = e2 - e1 - (2.0 * d_best + 1.0) * a;
					//if (a > 0.0 && fabs(b) <= a && b != 0.0) {
					if (a > 0.0) 
					{
						double bestDepth = 0.5 * b / a; 
						df = bestDepth - d_best; // should be within (-1.0, 1.0)
						minError  -= 0.25 * b * b / a;
					}
				}
           }
#endif // 1

            // Store the winning depth, error, and color
            if ( (d_best >= 0) && ( (m_error_threshold <= 0.0) || (minError <= m_error_threshold))) 
			{
            //if (minError <= m_error_threshold) {
                errr[c]  = minError;
                depth[c] = m_d_min + d_best * m_d_step + 128; // 8-bit range [-128, 127]
                color[c] = colrs[d_best];

				if (m_useSubpixel) // sub-pixel refinement
				{
					depth[c] += (int) (df * m_d_step + 0.5); 
					// depth[c] = m_d_min + (int) (bestDepth * m_d_step + 0.5) + 128; 
                    int s = (df > 0.0) ? 1 : -1;
					color[c] += fabs(df) * (colrs[d_best+s] - colrs[d_best]); 
				}

				// clamp to 255
				int minHalfError = (minError/2 > 255) ? 255 : (int) (minError/2);
				newerror->SetRGB(minHalfError, minHalfError, minHalfError);
            } 
			else   // this is what happens if d_best == -1
			{
                errr[c] = -1.0;
                
				// keep the original image
				//color[c] = CVisRGBABytePixel(0, 0, 0, 0);
				depth[c] = 0; 
                //color[c] = colrs[m_d_step/2]; // d=0, where the plane is
                //depth[c] = 128; // assume it is on the plane
				newerror->SetRGB(0, 0, 255);
            }
        }
    }

	if(m_writeConfidenceImage)
		m_confidence.WriteFile(ConfidenceFilename()); 

	// a hole-filling procedure to cleanup? 
#if 0
    for (int b = 0; b < m_spatial_blur_steps; b++)
        VisConvolveSeparable(ZImage(), ZImage(),
                             VisConvolutionKernel_14641,
                             VisConvolutionKernel_14641, 1);
#else

	CVisByteImage copyDepth(ZImage().Shape()); 
	ZImage().CopyPixelsTo(copyDepth); 

	int smooth = m_regularization_steps; 
	while (smooth-- > 0)
		DepthRegularization(copyDepth); 

	copyDepth.CopyPixelsTo(ZImage()); 

#endif

	// convert float error images to RGBA and write out
	if(m_writeErrorImages)
	{
		errorOut.WriteFile("errormap.bmp");
	}
}		


void CVisStereo::PlaneSweep()
{
    // Process all planes, then find winners

    // Allocate the intermediate images
    if (m_error.Shape() != Image().Shape() || m_errors.NBands() != m_d_steps) 
	{
        CVisShape sh = Image().Shape();
        m_bottom_alpha.Allocate(sh);
        m_error.Allocate(sh);
		m_confidence.Allocate(sh); 

        sh.SetNBands(m_d_steps);
        m_errors.Allocate(sh);
        m_colors.Allocate(sh);
    }

	// initialize things (don't need to?)
	ZImage().FillPixels(CVisBytePixel(0));

    // Step through the depth planes
    for (int d = 0; d < m_d_steps; d++)
	{
		printf("processing plane %d\n", d);
		ProcessPlane(d);
	}

    // Optionally do more iterative aggregation
    //  See [Scharstein & Szeliski, CVPR'96]

    // Find the winner
	WinnerTakeAll();
}

// New SD Stream I/O functions

void CVisStereo::BuildPropList(CVisPropList& refproplist)
{
    refproplist.AddPropReference("m_sprite", ((CVisSprite&) *this), false);   
    refproplist.AddPropReference("m_rgbacomposite", ((CVisRGBAComposite&) *this), false);   

    refproplist.AddPropReference("m_error_type", m_error_type, false);   
    refproplist.AddPropReference("m_spatial_blur_steps", m_spatial_blur_steps, false);
	refproplist.AddPropReference("m_error_threshold", m_error_threshold, false);
	refproplist.AddPropReference("m_regularization_steps", m_regularization_steps, false);

    refproplist.AddPropReference("m_d_min", m_d_min, false);
    refproplist.AddPropReference("m_d_step", m_d_step, false);
    refproplist.AddPropReference("m_d_steps", m_d_steps, false);

	// new members
	refproplist.AddPropReference("m_minNumVotes", m_minNumVotes, false);
	refproplist.AddPropReference("m_boxFilterSize", m_boxFilterSize, false);
	refproplist.AddPropReference("m_spatialMinFilterSize", m_spatialMinFilterSize, false);
	refproplist.AddPropReference("m_thresholdFilterSize", m_thresholdFilterSize, false);
	refproplist.AddPropReference("m_fillInInvalidErrors", m_fillInInvalidErrors, false);
	refproplist.AddPropReference("m_useSubpixel", m_useSubpixel, false);
	refproplist.AddPropReference("m_writeMeanImages", m_writeMeanImages, false);
	refproplist.AddPropReference("m_writeErrorImages", m_writeErrorImages, false);
	refproplist.AddPropReference("m_writeConfidenceImage", m_writeConfidenceImage, false);
}


void CVisStereo::ReadStereoValue(CVisSDIStream& refsdistream)
{
	refsdistream.ReadClassUsingPropList(*this);

	// UNDONE:  Fix up other members here

}
