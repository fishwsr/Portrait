///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisVoxelColor.h -- compute voxel map from collection of images
//
// DESCRIPTION
//
// INPUT
//
// OUTPUT
//
// INTERNAL REPRESENTATIONS
//
// PUBLIC MEMBER FUNTIONS (METHODS)
//
// PARAMETERS
//  name            description...
//
// SEE ALSO
//  VisStereo.cpp       implementation code
//  VisComposite.h      compositions / views of image collections
//  VisSprite.h         images and collections of images with depth
//
// BUGS
//   
//
// DESIGN
//
// HISTORY
//  16-Aug-99  Chris Buehler (t-chrbue) at Microsoft
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#ifndef __VIS_VOXELCOLOR_H__
#define __VIS_VOXELCOLOR_H__

// define VC_LDI if you're linking with libLDI and want to write LDIs
#define VC_LDI

//
//  CVisVoxelColor class:  implements voxel coloring variants on top of CVisStereo
//

class CVisVoxelColor : public CVisStereo
{
public:
    // Constructors and destructor
    CVisVoxelColor();

    void PlaneSweep();              // process all planes, then find winners

	// Voxel coloring algorithm
	void FattenUsedPixelMaskEdges();			// mask 4-neighbors of masked pixels
	void ComputeUndefinedVoxelMap(int d, int p=0);	// determines which voxels are defined or not
	void ThresholdPlane(int d, int p=0);		// apply threshold operator to error image of plane d
	void FillInMaskedVoxels(int d, int p=0);	// copy thresholded image into voxel map
	void MaskUsedPixels(int d, int p=0);		// alpha mask out pixels in reference images
	void CompositeDepthLayers();				// visualize depth volume

	// debug filenames
	virtual char *MeanFilename(int d);
	virtual char *ErrorFilename(int d);
	virtual char *ConfidenceFilename();
	virtual char *ThresholdFilename(int d);
	virtual char *SliceFilename(int d);

    //------------------------------------------------------------------
	// SDStream I / O

	void BuildPropList(CVisPropList& refproplist);

#ifdef VC_LDI
	void WriteLDI(char *filename);
#endif

protected:
	bool m_writeThresholdImages;		// write thresholded images?
	bool m_writeSliceImages;			// write voxel slice images?
	int  m_numPasses;					// how many times to iterate voxel coloring step
	double m_errorThresholdIncrement;	// how much to increase the error threshold each step
	int m_currentPass;
	int m_numImagesToDecide;			// the number of images that need to say a voxel is empty before it's empty

    CVisRGBAByteImage m_usedPixelMask;    // temporary storage for used pixels
	CVisByteSequence m_voxels;				// voxel occupancy map
	CVisRGBAByteSequence m_voxelColors;		// voxel colors

	CVisByteSequence m_depthMaps;			// a depth map for each input image
	CVisByteImage m_undefinedVoxelMap;		// a map of undefined voxels
};

VIS_SD_CLASS_WITH_IO_PL(CVisVoxelColor)

#endif
