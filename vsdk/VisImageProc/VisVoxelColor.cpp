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
#include "VisVoxelColor.h"
// if enabled, include ldi headers
#ifdef VC_LDI

#endif

//
// Constructors and destructor
//
CVisVoxelColor::CVisVoxelColor()
: CVisStereo()
{
	m_writeThresholdImages = true;		// write thresholded images?
	m_numPasses = 1;			// how many times to iterate plane sweep step
	m_errorThresholdIncrement = 100.0; // how much to increase the error threshold each step
	m_numImagesToDecide = 1;
	m_writeSliceImages = false;
}

char *CVisVoxelColor::MeanFilename(int d)
{
	static char name[200];
	sprintf(name, "bottom%02d_%02d.bmp", d, m_currentPass);
	return name;
}

char *CVisVoxelColor::ErrorFilename(int d)
{
	static char name[200];
	sprintf(name, "error%02d_%02d.bmp", d, m_currentPass); 
	return name;
}

char *CVisVoxelColor::ThresholdFilename(int d)
{
	static char name[200];
	sprintf(name, "mask%02d_%02d.bmp", d, m_currentPass); 
	return name;
}

char *CVisVoxelColor::SliceFilename(int d)
{
	static char name[200];
	sprintf(name, "slice%02d.bmp", d); 
	return name;
}

char *CVisVoxelColor::ConfidenceFilename()
{
	static char name[200];
	sprintf(name, "conf.bmp"); 
	return name;
}

const unsigned char undefinedVoxel = 0;
const unsigned char emptyVoxel = 1;
const unsigned char occupiedVoxel = 2;

// don't know what's there
inline bool VoxelIsUndefined(unsigned char v)
{
	return (v == undefinedVoxel);
}

// know nothing is there
inline bool VoxelIsEmpty(unsigned char v)
{
	return (v == emptyVoxel);
}

// know something is there
inline bool VoxelIsOccupied(unsigned char v)
{
	return (v >= occupiedVoxel);
}

inline unsigned char OccupiedVoxel(unsigned char v = 0)
{
	return (occupiedVoxel + v);
}

// adds a one pixel border around opaque pixels
// this will accentuate speckle noise unfortunately
void CVisVoxelColor::FattenUsedPixelMaskEdges()
{
	CVisRGBAByteImage tempMask(m_usedPixelMask.Shape());

	m_usedPixelMask.CopyPixelsTo(tempMask);

	int r,c;

	for(r = 0; r < m_usedPixelMask.Height()-1; r++)
	{
		CVisRGBABytePixel *maskRow1 = m_usedPixelMask.PtrToFirstPixelInRow(r);
		CVisRGBABytePixel *maskRow2 = m_usedPixelMask.PtrToFirstPixelInRow(r+1);
        CVisRGBABytePixel *tempRow1 = tempMask.PtrToFirstPixelInRow(r);
		for(c = 0; c < m_usedPixelMask.Width()-1; c++)
		{
			if(tempRow1[c].A() == 255)
			{
				maskRow1[c+1] = CVisRGBABytePixel(255,255,255,255);
				maskRow2[c]   = CVisRGBABytePixel(255,255,255,255);
			}
		}
	}
	for(r = m_usedPixelMask.Height()-1; r > 0; r--)
	{
		CVisRGBABytePixel *maskRow1 = m_usedPixelMask.PtrToFirstPixelInRow(r);
		CVisRGBABytePixel *maskRow2 = m_usedPixelMask.PtrToFirstPixelInRow(r-1);
        CVisRGBABytePixel *tempRow1 = tempMask.PtrToFirstPixelInRow(r);
		for(c = m_usedPixelMask.Width()-1; c > 0; c--)
		{
			if(tempRow1[c].A() == 255)
			{
				maskRow1[c-1] = CVisRGBABytePixel(255,255,255,255);
				maskRow2[c]   = CVisRGBABytePixel(255,255,255,255);
			}
		}
	}
}

// compute which voxels are candidates for coloring at this depth
// determines which voxels are empty, or undecided
// values in m_undefinedVoxelMap are 0 if voxel undefined
void CVisVoxelColor::ComputeUndefinedVoxelMap(int d, int p)
{
	int f,r,c;

	// let's try byte images - can we resample them?
	CVisByteImage subMask(m_usedPixelMask.Shape());
	CVisByteImage resampledSubMask(m_usedPixelMask.Shape());

	m_undefinedVoxelMap.FillPixels(0);

	for(f = 0; f < NFrames(); f++)
	{
		// threshold the depth map for this image at depth d
		CVisByteImage &depthMap = m_depthMaps[f];
		subMask.FillPixels(CVisBytePixel(0));
		for(r = 0; r < depthMap.Height(); r++)
		{
			CVisBytePixel *submaskRow = subMask.PtrToFirstPixelInRow(r);
			CVisBytePixel *depthRow = depthMap.PtrToFirstPixelInRow(r);
			for(c = 0; c < depthMap.Width(); c++)
			{
				// if the depth map has an entry and that entry is less than d
				if(!VoxelIsUndefined(depthRow[c]) && ((depthRow[c]-occupiedVoxel) < d))
				{
					submaskRow[c] = 255;
				}
			}
		}

		// resample the submask to the proper depth plane
		CVisTransform4x4 Hfd = DepthPlaneHomography(f, d).Inverted();
		VisResample(subMask, resampledSubMask, Hfd, VIS_INTERP_LINEAR);

		// composite into final mask
		// count how many images contribute to the mask at each pixel
		for(r = 0; r < m_undefinedVoxelMap.Height(); r++)
		{
			CVisBytePixel *submaskRow = resampledSubMask.PtrToFirstPixelInRow(r);
			CVisBytePixel *finalmaskRow = m_undefinedVoxelMap.PtrToFirstPixelInRow(r);
			for(c = 0; c < m_undefinedVoxelMap.Width(); c++)
			{
				if(submaskRow[c] != 0)
					finalmaskRow[c]++;
//					finalmaskRow[c] = 255;
			}
		}
	}

	// for debugging
//	char filename[200];
//	sprintf(filename, "voxmap\\vm_%02d_%02d.pgm", p, d);
//	m_undefinedVoxelMap.WriteFile(filename);
}

// uses m_error_threshold which is also used for a different purpose in WinnerTakeAll
// however, WinnerTakeAll will not be called if ThresholdPlane is
// ThresholdPlane builds a mask of used pixels at depth d
// for now assumes that m_error and m_bottom_alpha have been computed for plane d
void CVisVoxelColor::ThresholdPlane(int d, int p)
{
	CVisFloatImage &errImage = m_error;
//	CVisByteImage &voxels = m_voxels[d];

	// initialize to all transparent pixels
	m_usedPixelMask.FillPixels(CVisRGBABytePixel(0,0,0,0));

	// if error below threshold, set to opaque white
	int r,c;
	int maxNumVoters = m_layers.NBands();
	for(r = 0; r < errImage.Height(); r++)
	{
		float *err = errImage.PtrToFirstPixelInRow(r);
		CVisRGBABytePixel *mask = m_usedPixelMask.PtrToFirstPixelInRow(r);
        CVisRGBABytePixel *mean = m_bottom_alpha.PtrToFirstPixelInRow(r);
		CVisBytePixel *vox = m_undefinedVoxelMap.PtrToFirstPixelInRow(r);
		for(c = 0; c < errImage.Width(); c++)
		{
			// if enough images contribute, the error is low enough, and the voxel
			// is still undecided
			if( (maxNumVoters - (255 - mean[c].A()) >= m_minNumVotes) && 
				(err[c] < m_error_threshold) && (vox[c] < m_numImagesToDecide))
			{
				mask[c] = CVisRGBABytePixel(255,255,255,255);
			}
		}
	}

	// filter to fatten up threshold edges
	for(int i = 0; i < m_thresholdFilterSize; i++)
	{
		FattenUsedPixelMaskEdges();
	}

	if(m_writeThresholdImages)
	{
//		char filename[100];
//		sprintf(filename, "mask%02d_%02d.bmp", p, d);
		m_usedPixelMask.WriteFile(ThresholdFilename(d));
	}
}

// copies data into the voxel data structure
// assumes m_usedPixelMask has been set
// this is very slow
void CVisVoxelColor::FillInMaskedVoxels(int d, int p)
{
	CVisFloatImage &errImage = m_error;
	CVisByteImage &voxels = m_voxels[d];
	CVisRGBAByteImage &voxelColors = m_voxelColors[d];

	// copy occupied voxels into voxel map
	// don't obscure stuff that's already there (might because of fattening)
	int r,c;
	for(r = 0; r < errImage.Height(); r++)
	{
		CVisRGBABytePixel *mask = m_usedPixelMask.PtrToFirstPixelInRow(r);
        CVisRGBABytePixel *mean = m_bottom_alpha.PtrToFirstPixelInRow(r);
		CVisRGBABytePixel *color = voxelColors.PtrToFirstPixelInRow(r);
		CVisBytePixel *vox = voxels.PtrToFirstPixelInRow(r);
		for(c = 0; c < errImage.Width(); c++)
		{
			// if masked, possibly copy into voxel map
			if(mask[c].A() == 255)
			{
				if(VoxelIsUndefined(vox[c]))
				{
					vox[c] = OccupiedVoxel(p);			// encode the pass number for debugging purposes
					color[c] = mean[c];
				}
			}
		}
	}

/*
	// propagate empty voxels
	// for each image:
	//    determine sub mask corresponding to its contribution to the whole mask
	//    warp submask to all depth layers > d, setting undefined voxels to empty
	// can VisResample use images that aren't RGBA pixels?
	CVisRGBAByteImage subMask(m_usedPixelMask.Shape());
	CVisRGBAByteImage resampledSubMask(m_usedPixelMask.Shape());

	for(int f = 0; f < NFrames(); f++)
	{
		// build submask
		subMask.FillPixels(CVisRGBABytePixel(0,0,0,0));
		int nbands = m_layers.NBands();
		for(r = 0; r < subMask.Height(); r++)
		{
			CVisRGBABytePixel *layerRow = m_layers.PtrToFirstPixelInRow(r);
			CVisRGBABytePixel *submaskRow = subMask.PtrToFirstPixelInRow(r);
			CVisRGBABytePixel *maskRow = m_usedPixelMask.PtrToFirstPixelInRow(r);
			for(c = 0; c < subMask.Width(); c++)
			{
				// if this image contributed to this pixel, and the mask is opaque
				if( ((layerRow + nbands*c)[f].A() != 0) && (maskRow[c].A() == 255))
				{
					submaskRow[c] = CVisRGBABytePixel(255,255,255,255);
				}
			}
		}

		CVisTransform4x4 Hdf = DepthPlaneHomography(f, d);

		// for each depth > d
		for(int depth = d + 1; depth < m_d_steps; depth++)
		{
			// resample mask
			CVisTransform4x4 Hfdepth = DepthPlaneHomography(f, depth).Inverted();
			CVisTransform4x4 H = Hfdepth*Hdf;
			VisResample(subMask, resampledSubMask, H, VIS_INTERP_LINEAR);

			// copy into voxel map
			bool wroteVoxel = false;	// possible early out
			for(r = 0; r < resampledSubMask.Height(); r++)
			{
				CVisBytePixel *voxRow = m_voxels[depth].PtrToFirstPixelInRow(r);
				CVisRGBABytePixel *maskRow = resampledSubMask.PtrToFirstPixelInRow(r);
				for(c = 0; c < resampledSubMask.Width(); c++)
				{
					if(VoxelIsUndefined(voxRow[c]) && (maskRow[c].A() != 0))
					{
						voxRow[c] = emptyVoxel;
						wroteVoxel = true;
					}
				}
			}
			if(!wroteVoxel)
				break;
		}
	}
*/
}

// assumes m_current_depth is set correctly
// this function clears out pixels in the reference images that have already been accounted for
void CVisVoxelColor::MaskUsedPixels(int d, int p)
{
	CVisTransform4x4 H;

    CVisShape sh = Image().Shape();
	CVisRGBAByteImage resampledMask(sh);

	// for each reference image: warp mask to it's image plane, apply to alpha channel
	for(int f = 0; f < NFrames(); f++)
	{
		H = LayerTransform(f);
		// use nearest meighbor for now
		VisResample(m_usedPixelMask, resampledMask, H, VIS_INTERP_LINEAR); 

		// walk mask and clear out the alpha channel of the layer
		// also build depthmap here
		// there is probably a faster way to do this
		CVisRGBAByteImage &refImage = Frame(f);
		for(int r = 0; r < refImage.Height(); r++)
		{
			CVisRGBABytePixel *mask = resampledMask.PtrToFirstPixelInRow(r);
			CVisRGBABytePixel *ref = refImage.PtrToFirstPixelInRow(r);
			CVisBytePixel *dmap = m_depthMaps[f].PtrToFirstPixelInRow(r);
			for(int c = 0; c < refImage.Width(); c++)
			{
				if(mask[c].A() != 0)
				{
					ref[c] = CVisRGBABytePixel(0,0,0,0);
					if(VoxelIsUndefined(dmap[c]))   // abuse of voxel functions here
					{
						dmap[c] = OccupiedVoxel(d);
					}
				}
			}
		}

		// 25*32 == alot of images, so only write for last image for now
//		if(d == 22)
//		{
//			char filename[200];
//			sprintf(filename, "masks\\m_p%02d_d%02d_i%02d.bmp", p, d, f);
//			refImage.WriteFile(filename);
//		}

		// don't want to overwrite my images on disk?
		refImage.SetIsDirty(false);
	}
}

void CVisVoxelColor::CompositeDepthLayers()
{
	// for each voxel map:
	// go back to front, use painter's algorithm to composite
	// then fill in gaps possibly

	int d,r,c;

	CVisRGBAByteImage coloredDepth(ZImage().Shape());	// color coded depth image for debugging

	// composite with painter's algorithm
	for(d = 0; d < m_d_steps; d++)
	{
		CVisByteImage &vox = m_voxels[d];

		// copy to ZImage()
		for(r = 0; r < vox.Height(); r++)
		{
			unsigned char *voxrow = vox.PtrToFirstPixelInRow(r);
			unsigned char *zrow = ZImage().PtrToFirstPixelInRow(r);
			CVisRGBABytePixel *colorrow = coloredDepth.PtrToFirstPixelInRow(r);
			for(c = 0; c < vox.Width(); c++)
			{
				// zero is empty, nonzero is occupied
				if(VoxelIsOccupied(voxrow[c]))
				{
					zrow[c] = d;
					// which pass?
					// scale assuming d = 64
					CVisRGBABytePixel col;
					switch(voxrow[c]-occupiedVoxel)
					{
					case 0:
						col = CVisRGBABytePixel(d<<2, 0, 0);
						break;
					case 1:
						col = CVisRGBABytePixel(0, d<<2, 0);
						break;
					case 2:
						col = CVisRGBABytePixel(0, 0, d<<2);
						break;
					case 3:
						col = CVisRGBABytePixel(d<<2, d<<2, 0);
						break;
					case 4:
						col = CVisRGBABytePixel(d<<2, 0, d<<2);
						break;
					case 5:
						col = CVisRGBABytePixel(0, d<<2, d<<2);
						break;
					default:
						col = CVisRGBABytePixel(d<<2, d<<2, d<<2);
						break;
					}
					colorrow[c] = col;
				}
			}
		}
	}

	coloredDepth.WriteFile("colored_depth.bmp");

	// write out individual depth maps
//	int f;
//	char filename[200];
//	for(f = 0; f < NFrames(); f++)
//	{
//		sprintf(filename, "dmaps\\d%02d.pgm", f);
//		m_depthMaps[f].WriteFile(filename);
//	}

	// write out color voxel slices and file describing them

	if(m_writeSliceImages)
	{
		FILE *sliceFile;
		if((sliceFile = fopen("slices\\slices.txt", "w")) == NULL)
		{
			printf("cannot open slice description file\n");
		}
		else
		{
			// write reference camera information to slice file
			// camera 0 is instrinsics
			m_camera[0].Write(sliceFile);
			fprintf(sliceFile, "\n");
			fprintf(sliceFile, "%d\n", m_d_steps);
			for(d = 0; d < m_d_steps; d++)
			{
				m_voxelColors[d].WriteFile(SliceFilename(d));
				fprintf(sliceFile, "%lf slice%02d.bmp\n", DepthOfPlane(d), d);			
			}
			fclose(sliceFile);
		}
	}

/*
	// convert result to floating point to use in pull/push algorithm
	CVisFloatImage zfloat(ZImage().Shape());
	
	for(r = 0; r < zfloat.Height(); r++)
	{
		float *floatrow = zfloat.PtrToFirstPixelInRow(r);
		unsigned char *zrow = ZImage().PtrToFirstPixelInRow(r);
		for(c = 0; c < zfloat.Width(); c++)
		{
			if(zrow[c] == 0)  // if unassigned, set to negative
			{
				floatrow[c] = -1;
			}
			else
			{
				floatrow[c] = static_cast<float>(zrow[c]);
			}
		}
	}

	// fill in gaps and copy back to ZImage()
	VisPyramidalFillGaps(zfloat);
	for(r = 0; r < zfloat.Height(); r++)
	{
		float *floatrow = zfloat.PtrToFirstPixelInRow(r);
		unsigned char *zrow = ZImage().PtrToFirstPixelInRow(r);
		for(c = 0; c < zfloat.Width(); c++)
		{
			zrow[c] = static_cast<unsigned char>(floatrow[c]);
		}
	}
*/
}

void CVisVoxelColor::PlaneSweep()
{
    // Process all planes, then find winners

    // Allocate the intermediate images
    if (m_error.Shape() != Image().Shape() || m_errors.NBands() != m_d_steps) 
	{
        CVisShape sh = Image().Shape();
        m_bottom_alpha.Allocate(sh);
        m_error.Allocate(sh);
		m_confidence.Allocate(sh); 

		m_usedPixelMask.Allocate(sh);
		m_undefinedVoxelMap.Allocate(sh);
		m_voxels.Resize(m_d_steps, sh);
		m_voxelColors.Resize(m_d_steps, sh);
		m_depthMaps.Resize(NFrames(), sh);

        sh.SetNBands(m_d_steps);
        m_errors.Allocate(sh);
        m_colors.Allocate(sh);
    }

    // Step through the depth planes
	// let's go front to back now

	// initialize things
	printf("initializing..\n");
	ZImage().FillPixels(CVisBytePixel(0));

	int d,f;
	for(d = 0; d < m_d_steps; d++)
	{
		m_voxels[d].FillPixels(undefinedVoxel);

		// initialize to transparent voxels
		m_voxelColors[d].FillPixels(CVisRGBABytePixel(0,0,0,0));
	}
	for(f = 0; f < NFrames(); f++)
	{
		m_depthMaps[f].FillPixels(undefinedVoxel);
	}

	for(int p = 0; p < m_numPasses; p++)
	{
		m_currentPass = p;
		for (d = m_d_steps-1; d >= 0; d--)
		{
			printf("pass %d: processing plane %d\n", p, d);
			ProcessPlane(d);

			printf("\tmarking undefined..\n");
			ComputeUndefinedVoxelMap(d, p);

			printf("\tthresholding..\n");
			ThresholdPlane(d, p);

			printf("\tmasking..\n");
			MaskUsedPixels(d, p);

			printf("\tpopulating voxels..\n");
			FillInMaskedVoxels(d, p);
		}

		m_error_threshold += m_errorThresholdIncrement; 
	}

	// patch up m_error_threshold
	// so that doesn't change when write the class out
	m_error_threshold -= m_numPasses*m_errorThresholdIncrement;

	CompositeDepthLayers();
}

#ifdef VC_LDI
void CVisVoxelColor::WriteLDI(char *filename)
{

}
#endif

void CVisVoxelColor::BuildPropList(CVisPropList& refproplist)
{
    refproplist.AddPropReference("m_stereo", ((CVisStereo&) *this), false);   

	refproplist.AddPropReference("m_writeThresholdImages", m_writeThresholdImages, false);
	refproplist.AddPropReference("m_writeSliceImages", m_writeSliceImages, false);
	refproplist.AddPropReference("m_numPasses", m_numPasses, false);
	refproplist.AddPropReference("m_errorThresholdIncrement", m_errorThresholdIncrement, false);
	refproplist.AddPropReference("m_numImagesToDecide", m_numImagesToDecide, false);
}
