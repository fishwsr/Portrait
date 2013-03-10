///////////////////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisLayer.cpp -- single layer of a multi-layer stereo reconstruction
//
// DESCRIPTION
//  The CVisLayer class is used to represent a single layer within
//  a multi-plane stereo scene recovered from a collection of images.
//
// SEE ALSO
//  VisLayer.h   longer description of this class
//
// BUGS
//  See VisLayer.h 
//
// DESIGN
//  See VisLayer.h
//
// HISTORY
//  24-Jul-97  Richard Szeliski (szeliski) at Microsoft
//  Broke off from VisLayeredComposite class.
//
// Copyright © Microsoft Corp. 1997
//
///////////////////////////////////////////////////////////////////////////////////////


#include "VisImageProc.h"


//#include <assert.h>
//#include <math.h>
#include <algorithm>    // \msdev\include\algorithm:  find() generic function


//
// Layer Frame
//
CVisLayerFrame::CVisLayerFrame(void)
  : T_FrameBase(),
	m_imageDifference(),
	m_imageUnwarpedDifference(),
	m_dblBias(0.0),
	m_dblGain(1.0),
	m_evislayermembership(VIS_LAYER_MEMBERSHIP_OUT),
	m_cpixel(0),
	m_grfValidDifference(0)
{
}

CVisLayerFrame::CVisLayerFrame(const RECT& refrect)
  : T_FrameBase(refrect),
	m_imageDifference(),
	m_imageUnwarpedDifference(),
	m_dblBias(0.0),
	m_dblGain(1.0),
	m_evislayermembership(VIS_LAYER_MEMBERSHIP_OUT),
	m_cpixel(0),
	m_grfValidDifference(0)
{
	// UNDONE:  Should local warp and warped images be allocated
	// and initialized here?
}

CVisLayerFrame::CVisLayerFrame(const CVisShape& refshape)
  : T_FrameBase(refshape),
	m_imageDifference(),
	m_imageUnwarpedDifference(),
	m_dblBias(0.0),
	m_dblGain(1.0),
	m_evislayermembership(VIS_LAYER_MEMBERSHIP_OUT),
	m_cpixel(0),
	m_grfValidDifference(0)
{
	// UNDONE:  Should local warp and warped images be allocated
	// and initialized here?
}

CVisLayerFrame::CVisLayerFrame(const char *szFilename)
  : T_FrameBase(),
	m_imageDifference(),
	m_imageUnwarpedDifference(),
	m_dblBias(0.0),
	m_dblGain(1.0),
	m_evislayermembership(VIS_LAYER_MEMBERSHIP_OUT),
	m_cpixel(0),
	m_grfValidDifference(0)
{
	FReadFile(szFilename);
}

CVisLayerFrame::CVisLayerFrame(
		const T_Image& refimage)
  : T_FrameBase(),
	m_imageDifference(),
	m_imageUnwarpedDifference(),
	m_dblBias(0.0),
	m_dblGain(1.0),
	m_evislayermembership(VIS_LAYER_MEMBERSHIP_OUT),
	m_cpixel(0),
	m_grfValidDifference(0)
{
	(*this) = refimage;
}

CVisLayerFrame& CVisLayerFrame::operator=(
		const T_Image& refimage)
{
	// The CVisImage operator= method with synchronize assignment
	// and call the virtual Assign method to do the assignment.
	T_Image::operator=(refimage);

	return *this;
}

CVisLayerFrame::~CVisLayerFrame(void)
{
}


CVisLayerFrame CVisLayerFrame::Cropped(void)
{
	CVisLayerFrame compframeRet = T_FrameBase::Cropped();

	if (m_grfValidDifference)
	{
		if (compframeRet.m_imageDifference.IsValid())
			compframeRet.m_imageDifference.SetRect(RectCrop());

		if (compframeRet.m_imageUnwarpedDifference.IsValid())
			compframeRet.m_imageUnwarpedDifference.SetRect(RectCrop());

		// UNDONE:  Does compframeRet.m_cpixel need to be updated?
	}

	return compframeRet;
}

CVisLayerFrame CVisLayerFrame::Cropped(CRect& rect)
{
	CVisLayerFrame compframeRet = T_FrameBase::Cropped(rect);

	if (m_grfValidDifference)
	{
		if (compframeRet.m_imageDifference.IsValid())
			compframeRet.m_imageDifference.SetRect(rect);

		if (compframeRet.m_imageUnwarpedDifference.IsValid())
			compframeRet.m_imageUnwarpedDifference.SetRect(rect);

		// UNDONE:  Does compframeRet.m_cpixel need to be updated?
	}

	return compframeRet;
}


void CVisLayerFrame::SDReadWriteProperties(CVisSDStream& s,
		bool fAddComma)
{
	// Just call the base class for now.
	// UNDONE:  Do we need to write difference images or other data?
	T_FrameBase::SDReadWriteProperties(s, fAddComma);
}

const type_info& CVisLayerFrame::ObjectTypeInfo(int iLevel) const
{
	if ((iLevel < 0) || (iLevel >= eilevelWarpedFrame))
		return typeid(CVisLayerFrame);

	return T_FrameBase::ObjectTypeInfo(iLevel);
}

void CVisLayerFrame::Alias(const CVisImageBase& refimage)
{
	T_FrameBase::Alias(refimage);
	
	// Also copy layer data.
	if ((ObjectTypeInfo(eilevelLayerFrame)
					== refimage.ObjectTypeInfo(eilevelLayerFrame))
			&& (NBands() == refimage.NBands()))
	{
		if (m_grfValidDifference & VIS_VALID_WARPED_DIFF)
		{
			if (((const CVisLayerFrame&) refimage).m_imageDifference.IsValid())
				m_imageDifference.Alias(
						((const CVisLayerFrame&) refimage).m_imageDifference);
			else
				m_imageDifference.Deallocate();
		}
		else
		{
			m_imageDifference.Deallocate();
		}

		if (m_grfValidDifference & VIS_VALID_UNWARPED_DIFF)
		{
			if (((const CVisLayerFrame&) refimage).m_imageUnwarpedDifference.IsValid())
				m_imageUnwarpedDifference.Alias(
						((const CVisLayerFrame&) refimage).m_imageUnwarpedDifference);
			else
				m_imageUnwarpedDifference.Deallocate();
		}
		else
		{
			m_imageUnwarpedDifference.Deallocate();
		}

		// UNDONE:  Are the following assignments always right?  (They certainly are
		// right when copying the frames stored in CVisSequenceBase.)
		m_dblBias = ((const CVisLayerFrame&) refimage).m_dblBias;
		m_dblGain = ((const CVisLayerFrame&) refimage).m_dblGain;
		m_evislayermembership = ((const CVisLayerFrame&) refimage).m_evislayermembership;
		m_cpixel = ((const CVisLayerFrame&) refimage).m_cpixel;
		m_grfValidDifference = ((const CVisLayerFrame&) refimage).m_grfValidDifference;
	}
	else
	{
		m_imageDifference.Deallocate();
		m_imageDifference.Deallocate();
		m_grfValidDifference = false;

		// UNDONE:  Should additional data members be changed in this case?
		assert(0);
	}
}

void CVisLayerFrame::Copy(const CVisImageBase& refimage)
{
	T_FrameBase::Copy(refimage);
	
	// Also copy layer data.
	if ((ObjectTypeInfo(eilevelLayerFrame)
					== refimage.ObjectTypeInfo(eilevelLayerFrame))
			&& (NBands() == refimage.NBands()))
	{
		if (m_grfValidDifference & VIS_VALID_WARPED_DIFF)
		{
			if (((const CVisLayerFrame&) refimage).m_imageDifference.IsValid())
				m_imageDifference.Copy(
						((const CVisLayerFrame&) refimage).m_imageDifference);
			else
				m_imageDifference.Deallocate();
		}
		else
		{
			m_imageDifference.Deallocate();
		}

		if (m_grfValidDifference & VIS_VALID_UNWARPED_DIFF)
		{

			if (((const CVisLayerFrame&) refimage).m_imageUnwarpedDifference.IsValid())
				m_imageUnwarpedDifference.Copy(
						((const CVisLayerFrame&) refimage).m_imageUnwarpedDifference);
			else
				m_imageUnwarpedDifference.Deallocate();
		}
		else
		{
			m_imageUnwarpedDifference.Deallocate();
		}

		// UNDONE:  Are the following assignments always right?  (They certainly are
		// right when copying the frames stored in CVisSequenceBase.)
		m_dblBias = ((const CVisLayerFrame&) refimage).m_dblBias;
		m_dblGain = ((const CVisLayerFrame&) refimage).m_dblGain;
		m_evislayermembership = ((const CVisLayerFrame&) refimage).m_evislayermembership;
		m_cpixel = ((const CVisLayerFrame&) refimage).m_cpixel;
		m_grfValidDifference = ((const CVisLayerFrame&) refimage).m_grfValidDifference;
	}
	else
	{
		m_imageDifference.Deallocate();
		m_imageDifference.Deallocate();
		m_grfValidDifference = false;

		// UNDONE:  Should additional data members be changed in this case?
		assert(0);
	}
}

CVisImageBase *CVisLayerFrame::Clone(const CVisShape *pshape) const
{
	if (pshape == 0)
		return new CVisLayerFrame(*this);
	
	return new CVisLayerFrame(*pshape);
}


void CVisLayerFrame::Assign(const CVisImageBase& refimage)
{
	T_FrameBase::Assign(refimage);
	
	// Also copy layer data.
	if ((ObjectTypeInfo(eilevelLayerFrame)
					== refimage.ObjectTypeInfo(eilevelLayerFrame))
			&& (NBands() == refimage.NBands()))
	{
		if (m_grfValidDifference & VIS_VALID_WARPED_DIFF)
		{
			if (((const CVisLayerFrame&) refimage).m_imageDifference.IsValid())
				m_imageDifference
						= ((const CVisLayerFrame&) refimage).m_imageDifference;
			else
				m_imageDifference.Deallocate();
		}
		else
		{
			m_imageDifference.Deallocate();
		}

		if (m_grfValidDifference & VIS_VALID_UNWARPED_DIFF)
		{

			if (((const CVisLayerFrame&) refimage).m_imageUnwarpedDifference.IsValid())
				m_imageUnwarpedDifference
						= ((const CVisLayerFrame&) refimage).m_imageUnwarpedDifference;
			else
				m_imageUnwarpedDifference.Deallocate();
		}
		else
		{
			m_imageUnwarpedDifference.Deallocate();
		}

		// UNDONE:  Are the following assignments always right?  (They certainly are
		// right when copying the frames stored in CVisSequenceBase.)
		m_dblBias = ((const CVisLayerFrame&) refimage).m_dblBias;
		m_dblGain = ((const CVisLayerFrame&) refimage).m_dblGain;
		m_evislayermembership = ((const CVisLayerFrame&) refimage).m_evislayermembership;
		m_cpixel = ((const CVisLayerFrame&) refimage).m_cpixel;
		m_grfValidDifference = ((const CVisLayerFrame&) refimage).m_grfValidDifference;
	}
	else
	{
		m_imageDifference.Deallocate();
		m_imageDifference.Deallocate();
		m_grfValidDifference = false;

		// UNDONE:  Should additional data members be changed in this case?
		assert(0);
	}
}

bool CVisLayerFrame::ReadWriteFile(
		SVisFileDescriptor& reffiledescriptor,
		bool fWrite, bool fThrowError, bool fDisplayErrorMessage)
{
	bool fRet =  CVisImageBase::ReadWriteFile(reffiledescriptor, fWrite,
			fThrowError, fDisplayErrorMessage);

	// UNDONE:  Should the local warp image be changed?

	return fRet;
}


///////////////////////////////////////////////////////////////////////////////////////
//
// Constructors and destructor
//
///////////////////////////////////////////////////////////////////////////////////////

CVisLayer::CVisLayer(EVisSequence evissequence)
  : CVisRGBAComposite(evissequence)
{
	// Reference count for pointer class construction (see VisLayeredComposite.h)
	m_cRef			= 0;
	
	// Miscellaneous publically writeable options
	m_dblFlowScale = 100.0; // 1 pixel of flow displacement = 100 grey levels

	m_fUseBias      = true;

#ifdef TAKEOUT
	// Major data members
	m_nframes       = 0;
	m_membership    = new TVisLayerMembership[1];
	m_membership[0] = VIS_LAYER_MEMBERSHIP_OUT;
	m_difference    = new CVisRGBAByteImage[1];
	m_difference[0].Allocate(1,1);
	m_unwarped_diff = new CVisRGBAByteImage[1];
	m_unwarped_diff[0].Allocate(1,1);
	m_valid_diff    = new int[1];
	m_valid_diff[0] = 0;
	m_bias          = new double[1];
	m_bias[0]       = 0.0;
	m_gain          = new double[1];
	m_gain[0]       = 1.0;
	m_num_pixels    = new int[1];
	m_num_pixels[0] = 0;
#endif // TAKEOUT

	SetBoundingBoxSize(-1.0);						              // Ignore bounding box
}

CVisLayer::~CVisLayer()
{
#ifdef TAKEOUT
	delete [] m_num_pixels;
	delete [] m_gain;
	delete [] m_bias;
	delete [] m_valid_diff;
	delete [] m_difference;
	delete [] m_unwarped_diff;
	delete [] m_membership;
#endif // TAKEOUT
}


// Reference counting
unsigned long CVisLayer::AddRef(void)
{
	return ++m_cRef;
}

unsigned long CVisLayer::Release(void)
{
	assert(m_cRef > 0);

	unsigned long cRef = --m_cRef;
	if (cRef == 0)
		delete this;

	return cRef;
}

///////////////////////////////////////////////////////////////////////////////////////
//
// Functions for accessing member variables
//
///////////////////////////////////////////////////////////////////////////////////////

TVisLayerMembership CVisLayer::CompositeMembership(int index) 
{ 
	if (Length() > 0) {
		assert(index >= 0 && index < Length());
		return At(index).EVisLayerMembership();
	}
	else {
		// Act sensibly if called when there are no images in the layer
		assert(Length() == 0);
		return VIS_LAYER_MEMBERSHIP_OUT;
	}
}

void CVisLayer::SetCompositeMembership(int index, TVisLayerMembership membership)
{
	if (Length() > 0) {
		assert(index >= 0 && index < Length());
		if (At(index).EVisLayerMembership() != membership) {
			Invalidate(VIS_VALID_TOP | VIS_VALID_TOP_BKGD);
			At(index).SetEVisLayerMembership(membership);
		}
	}
	else {
		// Act sensibly if called when there are no images in the layer
		assert(Length() == 0);
	}
}


///////////////////////////////////////////////////////////////////////////////////////
//
// Overridden VisCollection operations 
//
///////////////////////////////////////////////////////////////////////////////////////

void CVisLayer::ReadCollection(const char *filename, int read_files, FILE *istream,
							   TVisLayerMembership member)
{
	int iLimBeforeRead = Length();

	CVisRGBAComposite::ReadCollection(filename, read_files, istream);

	int iLimAfterRead = Length();

	// Update membership and difference information
	if (iLimAfterRead > iLimBeforeRead)
	{
#ifdef TAKEOUT
	int n_frames = NFrames();
	if (n_frames > m_nframes) {						              // If some images read
		assert(n_frames > 0);

		// Allocate space for information about new images
		TVisLayerMembership *membership    = new TVisLayerMembership[n_frames];
		CVisRGBAByteImage  *difference    = new CVisRGBAByteImage[n_frames];
		CVisRGBAByteImage  *unwarped_diff = new CVisRGBAByteImage[n_frames];
		int                 *valid_diff    = new int[n_frames];
		double              *bias          = new double[n_frames];
		double              *gain          = new double[n_frames];
		int                 *num_pixels    = new int[n_frames];

		// Copy old data into new space
		for(int i = 0; i < m_nframes; i++) {
			membership[i] = m_membership[i];
			difference[i].Allocate(1,1);				 // Don't copy difference
			unwarped_diff[i].Allocate(1,1);			 // Don't copy difference
			valid_diff[i] = 0;							 // Invalidate old differences
			bias[i]       = m_bias[i];
			gain[i]       = m_gain[i];
			num_pixels[i] = m_num_pixels[i];
		}

		// Initialize newly read members
		for( ; i < n_frames	; i++) {
			membership[i] = member;                      // Default composite membership
			difference[i].Allocate(1,1);
			unwarped_diff[i].Allocate(1,1);
			valid_diff[i] = 0;							 // Invalidate new differences
			bias[i]       = 0.0;
			gain[i]       = 1.0;
			num_pixels[i] = 0;
		}

		// Delete old space
		delete [] m_num_pixels;
		delete [] m_gain;
		delete [] m_bias;
		delete [] m_valid_diff;
		delete [] m_difference;
		delete [] m_unwarped_diff;
		delete [] m_membership;

		// Update member variables
		m_membership    = membership;
		m_difference    = difference;
		m_unwarped_diff = unwarped_diff;
		m_valid_diff    = valid_diff;
		m_bias          = bias;
		m_gain          = gain;
		m_num_pixels    = num_pixels;

		m_nframes = n_frames;
#endif // TAKEOUT

		Invalidate();

		// Read in membership information - if it is there
		int input_membership, k = 0;

		// A hack - required because the reading of local warp seems to just eat up a #
		// rather than an entire line of code
		if (m_local_warp) {
			// Eat up the next line of text
			char c;
			do {
				c = fgetc(istream);
			} 
			while (!(c == '\n' || c == EOF));
		}

		// Check to see whether membership information is there
		if (fscanf(istream, (k == Length() - 1) ? "%d\n" : "%d ", &input_membership) == 1) {
			// If the first integer is read in OK, assume the rest goes OK
			At(k).SetEVisLayerMembership((input_membership) ? VIS_LAYER_MEMBERSHIP_IN:
				                                   VIS_LAYER_MEMBERSHIP_OUT);
			for(k = 1; k < Length(); k++) {
				fscanf(istream, (k == Length() - 1) ? "%d\n" : "%d ", &input_membership);
				At(k).SetEVisLayerMembership((input_membership) ? VIS_LAYER_MEMBERSHIP_IN:
				                                       VIS_LAYER_MEMBERSHIP_OUT);
			}

			// Eat up the next line of text
			char c;
			do {
				c = fgetc(istream);
			} 
			while (!(c == '\n' || c == EOF));
		}
		
		// Read in the bias and gain information - if it is there
		float input_bias, input_gain;
		k = 0;
		// Check to see whether bias and gain information is there
		if (fscanf(istream, (k == Length() - 1) ? "%f\n" : "%f ", &input_bias) == 1) {
			// If the first float is read in OK, assume the rest goes OK
			At(k).SetDblBias(input_bias);
			for(k = 1; k < Length(); k++) {
				fscanf(istream, (k == Length() - 1) ? "%f\n" : "%f ", &input_bias);
				At(k).SetDblBias(input_bias);
			}
			for(k = 0; k < Length(); k++) {
				fscanf(istream, (k == Length() - 1) ? "%f\n" : "%f ", &input_gain);
				At(k).SetDblGain(input_gain);
			}

			// Eat up the next line of text
			char c;
			do {
				c = fgetc(istream);
			} 
			while (!(c == '\n' || c == EOF));
		}
	}
}

void CVisLayer::WriteCollection(const char *filename, const char *desc_type,
								int write_files, FILE *ostream, int write_bias)
{
	CVisRGBAComposite::WriteCollection(filename, desc_type, write_files, ostream);

	if (NFrames() > 0) {
		fprintf(ostream, "# Layer Membership Information\n");
		for(int k = 0; k < NFrames(); k++) {
			fprintf(ostream, (At(k).EVisLayerMembership() == VIS_LAYER_MEMBERSHIP_OUT) ? "0" : "1");
			fprintf(ostream, (k == NFrames() - 1) ? "\n" : " ");
		}
		
		if (write_bias) {
			fprintf(ostream, "# Bias and Gain Information\n");
			for(k = 0; k < NFrames(); k++) {
				fprintf(ostream, (k == NFrames() - 1) ? "%f\n" : "%f ", At(k).DblBias());
			}
			for(k = 0; k < NFrames(); k++) {
				fprintf(ostream, (k == NFrames() - 1) ? "%f\n" : "%f ", At(k).DblGain());
			}
		}
	}
}

#ifdef TAKEOUT
void CVisLayer::Insert(CVisRGBAByteFrame &frame, int index, 
					   int new_geometry, TVisLayerMembership member)
{
	assert(index <= m_nframes);

	// UNDONE:  new_geometry flag in CVisCollection::Insert.
	assert(new_geometry == 0);
    CVisRGBAComposite::Insert(frame, index);

	assert(m_nframes == NFrames() - 1);
    m_nframes = NFrames();
	
	// Allocate space for information about new image
	TVisLayerMembership *membership    = new TVisLayerMembership[m_nframes];
	CVisRGBAByteImage  *difference    = new CVisRGBAByteImage[m_nframes];
	CVisRGBAByteImage  *unwarped_diff = new CVisRGBAByteImage[m_nframes];
	int                 *valid_diff    = new int[m_nframes];
	double              *bias          = new double[m_nframes];
	double              *gain          = new double[m_nframes];
	int                 *num_pixels    = new int[m_nframes];

	// Copy old data into new space	inserting information for new image
	if (index<0) {
 		index = m_nframes-1;
	}
    for (int i = 0; i < index; i++)	{
        membership[i] = m_membership[i];
		difference[i].Allocate(1,1);					// Don't copy difference
		unwarped_diff[i].Allocate(1,1);				// Don't copy difference
		valid_diff[i] = 0;							    // Invalidate old differences
	    bias[i]       = m_bias[i];
		gain[i]       = m_gain[i];
		num_pixels[i] = m_num_pixels[i];
	}
    membership[index] = member;							// Default composite membership
	difference[index].Allocate(1,1);
	unwarped_diff[index].Allocate(1,1);
	valid_diff[index] = 0;								// Invalidate new diffference
	bias[index]       = 0.0;
	gain[index]       = 1.0;
	num_pixels[index] = 0;
    for (i = index+1; i < m_nframes; i++) {
        membership[i] = m_membership[i-1];
		difference[i].Allocate(1,1);					// Don't copy difference
		unwarped_diff[i].Allocate(1,1);				// Don't copy difference
		valid_diff[i] = 0;								// Invalidate old differences
	    bias[i]       = m_bias[i-1];
		gain[i]       = m_gain[i-1];
		num_pixels[i] = m_num_pixels[i-1];
	}

	// Delete old space
	delete [] m_num_pixels;
	delete [] m_gain;
	delete [] m_bias;
	delete [] m_valid_diff;
	delete [] m_difference;
	delete [] m_unwarped_diff;
	delete [] m_membership;

	// Update member variables
	m_membership    = membership;
	m_difference    = difference;
	m_unwarped_diff = unwarped_diff;
	m_valid_diff    = valid_diff;
	m_bias          = bias;
	m_gain          = gain;
	m_num_pixels    = num_pixels;
}

void CVisLayer::Insert(const char *filename, int index, TVisLayerMembership member)
{
	assert(index <= m_nframes);
	CVisRGBAComposite::Insert(filename, index);

	assert(m_nframes == NFrames() - 1);
    m_nframes = NFrames();
	
	// Allocate space for information about new image
	TVisLayerMembership *membership    = new TVisLayerMembership[m_nframes];
	CVisRGBAByteImage  *difference    = new CVisRGBAByteImage[m_nframes];
	CVisRGBAByteImage  *unwarped_diff = new CVisRGBAByteImage[m_nframes];
	int                 *valid_diff    = new int[m_nframes];
	double              *bias          = new double[m_nframes];
	double              *gain          = new double[m_nframes];
	int                 *num_pixels    = new int[m_nframes];

	// Copy old data into new space	inserting information for new image
	if (index<0) {
		index = m_nframes-1;
	}
    for (int i = 0; i < index; i++)	{
        membership[i] = m_membership[i];
		difference[i].Allocate(1,1);					// Don't copy difference
		unwarped_diff[i].Allocate(1,1);				// Don't copy difference
		valid_diff[i] = 0;							    // Invalidate old differences
	    bias[i]       = m_bias[i];
		gain[i]       = m_gain[i];
		num_pixels[i] = m_num_pixels[i];
	}
    membership[index] = member;							// Default composite membership
	difference[index].Allocate(1,1);
	unwarped_diff[index].Allocate(1,1);
	valid_diff[index] = 0;								// Invalidate new diffference
	bias[index]       = 0.0;
	gain[index]       = 1.0;
	num_pixels[index] = 0;
    for (i = index+1; i < m_nframes; i++) {
        membership[i] = m_membership[i-1];
		difference[i].Allocate(1,1);					// Don't copy difference
		unwarped_diff[i].Allocate(1,1);				// Don't copy difference
		valid_diff[i] = 0;								// Invalidate old differences
	    bias[i]       = m_bias[i-1];
		gain[i]       = m_gain[i-1];
		num_pixels[i] = m_num_pixels[i-1];
	}

	// Delete old space
	delete [] m_num_pixels;
	delete [] m_gain;
	delete [] m_bias;
	delete [] m_valid_diff;
	delete [] m_difference;
	delete [] m_unwarped_diff;
	delete [] m_membership;

	// Update member variables
	m_membership    = membership;
	m_difference    = difference;
	m_unwarped_diff = unwarped_diff;
	m_valid_diff    = valid_diff;
	m_bias          = bias;
	m_gain          = gain;
	m_num_pixels    = num_pixels;
}

void CVisLayer::Raise(int index, bool to_top)
{
	CVisRGBAComposite::Raise(index, to_top);
	assert(m_nframes == NFrames());

	// Reorder membership information
	// Ignore difference information since we will invalidate it
    if (index > 0) {
        int dst_index = (to_top) ? 0 : index - 1;
		
		// Save rising image data
        TVisLayerMembership save_member     = m_membership[index];
		double              save_bias       = m_bias[index];
		double              save_gain       = m_gain[index];
		int                 save_num_pixels	= m_num_pixels[index];

		// Shift everying between rising image and its destination down 
        for (int i = index; i > dst_index; i--)	{
            m_membership[i] = m_membership[i-1];
			m_bias[i]       = m_bias[i-1];
			m_gain[i]       = m_gain[i-1];
			m_num_pixels[i] = m_num_pixels[i-1];
		}

		// Insert saved image into its destination
        m_membership[dst_index] = save_member; 
		m_bias[dst_index]       = save_bias;
		m_gain[dst_index]       = save_gain;
		m_num_pixels[dst_index] = save_num_pixels;
    }

	// Since compositing may depend upon the order of the images, invalidate everything
	Invalidate();
}

void CVisLayer::Lower(int index, bool to_bottom)
{
	CVisRGBAComposite::Lower(index, to_bottom);
	assert(m_nframes == NFrames());

	// Reorder membership information
	// Ignore difference information since we will invalidate it
    if (index < m_nframes-1) {
        int dst_index = (to_bottom) ? m_nframes - 1 : index + 1;
		
		// Save lowered image data
        TVisLayerMembership save_member     = m_membership[index];
		double              save_bias       = m_bias[index];
		double              save_gain       = m_gain[index];
		int                 save_num_pixels	= m_num_pixels[index];

		// Shift everying between lowered image and its destination up 
        for (int i = index; i < dst_index; i++)	{
            m_membership[i] = m_membership[i+1];
			m_bias[i]       = m_bias[i+1];
			m_gain[i]       = m_gain[i+1];
			m_num_pixels[i] = m_num_pixels[i+1];
		}

		// Insert saved image into its destination
        m_membership[dst_index] = save_member;
		m_bias[dst_index]       = save_bias;
		m_gain[dst_index]       = save_gain;
		m_num_pixels[dst_index] = save_num_pixels;
    }

	// Since compositing may depend upon the order of the images, invalidate everything
	Invalidate();
}

void CVisLayer::Delete(int index)
{
    assert(0 <= index && index < m_nframes);
	CVisRGBAComposite::Delete(index);
	assert(m_nframes == NFrames() + 1);

    // Shift everything below deleted image up one spot
	// Ignore difference information since we will invalidate it
    for (int i = index; i < m_nframes - 1; i++) {
        m_membership[i] = m_membership[i+1];
		m_bias[i]       = m_bias[i+1];
		m_gain[i]       = m_gain[i+1];
		m_num_pixels[i] = m_num_pixels[i+1];
	}

	m_nframes = NFrames();
	Invalidate();
}

void CVisLayer::DeleteAll()
{
	CVisRGBAComposite::DeleteAll();
	assert(NFrames() == 0);

	m_nframes = NFrames();
	Invalidate();
}
#endif // TAKEOUT


void CVisLayer::OnICurChanged(void)
{
	// If position changed, top layer and composite are invalid
	Invalidate(VIS_VALID_BOTTOM | VIS_VALID_DIFFERENCE);
}


void CVisLayer::OnOrderChanged(void)
{
	Invalidate();
}


void CVisLayer::OnSequenceChanged(void)
{
	Invalidate();
}


///////////////////////////////////////////////////////////////////////////////////////
//
// Overridden frame positioning, skipping empty frames
//
///////////////////////////////////////////////////////////////////////////////////////

int CVisLayer::IsEmpty()
{
	for(int i=0; i < NFrames(); i++) {
		if (!IsEmptyFrame(i)) {
			return 0;
		}
	}
	return 1;
}

int CVisLayer::IsEmptyFrame(int index)
{
	assert(0 <= index && index < NFrames());
    CVisRGBAByteImage &img = (*this)[index];
    return (img.Height() <= 0 || img.Width() <= 0);
}


#ifdef TAKEOUT
void CVisLayer::SetFirst()
{
	// Check for empty layer 
    if (Length() == 0) {
		CVisRGBAComposite::SetFirst();
        return;
    }

	// Remember old position
	int old_pos = ICur();

	// Update checking for empty frames
    CVisRGBAByteCollection::SetFirst();
    while (ICur() < Length()-1 && IsEmptyFrame(ICur()))
        CVisRGBAByteCollection::SetNext();
    if (IsEmptyFrame(ICur()))
        CVisRGBAByteCollection::SetFirst();

	// If position changed, top layer and composite are invalid
	if (old_pos != ICur()) {
		Invalidate(VIS_VALID_BOTTOM | VIS_VALID_DIFFERENCE);
	}
}

void CVisLayer::SetLast()
{
	// Check for empty layer 
    if (Length() == 0) {
		CVisRGBAComposite::SetLast();
        return;
    }

	// Remember old position
	int old_pos = ICur();

	// Update checking for empty frames
    CVisRGBAByteCollection::SetLast();
    while (ICur() > 0 && IsEmptyFrame(ICur()))
        CVisRGBAByteCollection::SetPrev();

	// If position changed, top layer and composite are invalid
	if (old_pos != ICur()) {
		Invalidate(VIS_VALID_BOTTOM | VIS_VALID_DIFFERENCE);
	}
}

void CVisLayer::SetNext()
{
	// Check for empty layer 
    if (Length() == 0) {
		CVisRGBAComposite::SetNext();
        return;
    }

	// Remember old position
    int current = ICur();

	// Update checking for empty frames
    CVisRGBAByteCollection::SetNext();
    while (ICur() < Length()-1 && IsEmptyFrame(ICur()))
        CVisRGBAByteCollection::SetNext();
    if (IsEmptyFrame(ICur()))
        SetICur(current);

	// If position changed, top layer and composite are invalid
	if (current != ICur()) {
		Invalidate(VIS_VALID_BOTTOM | VIS_VALID_DIFFERENCE);
	}
}

void CVisLayer::SetPrev()
{
	// Check for empty layer 
    if (Length() == 0) {
		CVisRGBAComposite::SetPrev();
        return;
    }

	// Remember old position
    int current = ICur();

	// Update checking for empty frames
    CVisRGBAByteCollection::SetPrev();
    while (ICur() > 0 && IsEmptyFrame(ICur()))
        CVisRGBAByteCollection::SetPrev();
    if (IsEmptyFrame(ICur()))
        SetICur(current);

	// If position changed, top layer and composite are invalid
	if (current != ICur()) {
		Invalidate(VIS_VALID_BOTTOM | VIS_VALID_DIFFERENCE);
	}
}
#endif // TAKEOUT


///////////////////////////////////////////////////////////////////////////////////////
//
// Overridden shape computation functions
//
///////////////////////////////////////////////////////////////////////////////////////

 
CVisShape CVisLayer::BottomLayerShape(int omit_this_image)
{
    CVisShape shape(0, 0, 0, 0, 1);

	// Take union of all shapes which are used to compute the composite (bottom layer)
    for (int i = 0; i < NFrames(); i++) {
        if (CompositeMembership(i) == VIS_LAYER_MEMBERSHIP_IN &&
			i != omit_this_image) {
			CVisShape sh = LayerShape(i, 1);
			if (sh.Height() > 0 && sh.Width() > 0) {
				if (shape.Height()) {
					shape |= sh;
				}
				else {
					shape = sh;
				}
				shape.SetNBands(Warped(i).NBands());
			}
		}
    }

    // Trim shape to m_bounding_box times the size of top layer
    if (m_bounding_box > 0.0f) {
        CVisShape top_shape = TopLayerShape();
        int dwidth  = int(top_shape.Width() * (m_bounding_box - 1) / 2);
        int dheight = int(top_shape.Height() * (m_bounding_box - 1) / 2);
        CVisShape trim_shape = top_shape;
		trim_shape.InflateRect(dwidth, dheight);
        shape &= trim_shape;
    }

    return shape;
}


///////////////////////////////////////////////////////////////////////////////////////
//
// Overridden view computation functions
// 
///////////////////////////////////////////////////////////////////////////////////////

// This could be left in CVisRGBAComposite, but I have copied it here for consistency
CVisRGBAByteImage& CVisLayer::TopLayer()
{
	if (! (m_valid_layers & VIS_VALID_TOP))
        FormTopLayer(TopLayerShape());
    if (! (m_valid_layers & VIS_VALID_TOP_BKGD))
        FormTopWBkgd(m_top_layer.Shape());
    return m_top_w_bkgd;
}

// Add this to basic VisComposite class...
CVisRGBAByteImage& CVisLayer::TopAlpha()
{
	if (! (m_valid_layers & VIS_VALID_TOP))
        FormTopLayer(TopLayerShape());
    return m_top_layer;
}

// This could be left in CVisRGBAComposite, but I have copied it here for consistency
CVisRGBAByteImage& CVisLayer::BottomLayer(TVisLayerCompOp op)
{
    if (! (m_valid_layers & VIS_VALID_BOTTOM))
        FormBottomLayer(BottomLayerShape(), op);
    return m_bottom_layer;
}

// Needs to be here to call CVisLayer::BottomLayerShape()
CVisRGBAByteImage& CVisLayer::BottomAlpha(TVisLayerCompOp op)
{
    if (! (m_valid_layers & VIS_VALID_BOTTOM))
        FormBottomLayer(BottomLayerShape(), op);
	return m_bottom_alpha;
}

// Needs to be here to call CVisLayer::CompositeShape()
CVisRGBAByteImage& CVisLayer::Composite(EVisComposite comp_op,
										 TVisLayerCompOp bgl_op)
{
    if (! (m_valid_layers & VIS_VALID_TOP))
        FormTopLayer(TopLayerShape());
    if (! (m_valid_layers & VIS_VALID_BOTTOM))
        FormBottomLayer(BottomLayerShape(), bgl_op);
    if (! (m_valid_layers & VIS_VALID_COMPOSITE))
        FormComposite(CompositeShape(), comp_op);
    return m_composite;
}


// Needs to be here to call CVisLayer::BottomLayer()
CVisRGBAByteImage& CVisLayer::BlinkLayer(TVisLayerCompOp op)
{
	if (m_blink)
		return BottomLayer(op);
	else
		return Composite(composite_over, op);
}


// The difference function for regular layers. It computes the difference between 
// the top layer and the bottom layer and places the result in m_difference[] 
CVisRGBAByteImage& CVisLayer::Difference(TVisLayerCompOp comp_op,
										  TVisLayerDiffOp diff_op)
{
	int index = CurrentFrame();

    if (! (At(index).GrfValidDifference() & VIS_VALID_WARPED_DIFF)) {
		if (! (m_valid_layers & VIS_VALID_TOP))
			FormTopLayer(TopLayerShape());
		if (! (m_valid_layers & VIS_VALID_BOTTOM))
			FormBottomLayer(BottomLayerShape(), comp_op);

	    FormDifference(m_top_layer, m_bottom_alpha, At(index).ImageDifference(), 
			           (BottomLayerShape(index) & TopLayerShape()), diff_op);

		if (m_local_warp) {
			CopyFlowIntoDifference(index, At(index).ImageDifference());
		}

		// Update Validity information
		At(index).SetGrfValidDifference(VIS_VALID_WARPED_DIFF);
	}

	return At(index).ImageDifference();
}

// First computes the difference usinge CVisLayerDifference and then warps
// it back into the original image coordinate frame using the layer transform
CVisRGBAByteImage& CVisLayer::UnWarpedDiff(TVisLayerCompOp comp_op,
										    TVisLayerDiffOp diff_op)
{
	int	index = CurrentFrame();

	if (! (At(index).GrfValidDifference() & VIS_VALID_UNWARPED_DIFF)) {
	
		// Get the warped difference image and its shape
		Difference(comp_op, diff_op);
		CVisRGBAByteImage &difference = At(index).ImageDifference();
		CVisShape warped_shape = difference.Shape();

		if (warped_shape.Height() > 0 && warped_shape.Width() > 0) {
			// Get the layer transform
			CVisTransform4x4 t  = LayerTransform(index);
			CVisTransform4x4 ti	= t.Inverted();

			// Compute the unwarped shape
			CVisShape unwarped_shape = VisResampleShape(warped_shape, ti);

			// Warp the difference back
			if (unwarped_shape.Height() > 0 && unwarped_shape.Width() > 0) {
				if (unwarped_shape != At(index).ImageUnwarpedDifference().Shape()) {
					At(index).ImageUnwarpedDifference().Allocate(unwarped_shape);
				}
				VisResample(difference, At(index).ImageUnwarpedDifference(), t);
			}
			else {
				At(index).ImageUnwarpedDifference().Allocate(1,1);
			}
		}
		else {
			At(index).ImageUnwarpedDifference().Allocate(1,1);
		}

		At(index).SetGrfValidDifference(
				At(index).GrfValidDifference() | VIS_VALID_UNWARPED_DIFF);
	}

	return At(index).ImageUnwarpedDifference();
}


// The first difference function for synthetic images. It computes the difference 
// between the input image corresponding to this synthetic image and one of the layers 
// which form this synthetic image and places result in m_difference[] 
CVisRGBAByteImage& CVisLayer::SynthDifference(TVisLayerCompOp comp_op,
									      TVisLayerDiffOp diff_op, CVisRGBAByteImage &im)
{
	int index = CurrentFrame();

    if (! (At(index).GrfValidDifference() & VIS_VALID_WARPED_DIFF)) {
		if (! (m_valid_layers & VIS_VALID_TOP))
			FormTopLayer(TopLayerShape());

	    FormDifference(m_top_layer, im, At(index).ImageDifference(),
			           (TopLayerShape() & im.Shape()), diff_op);

		// Update Validity information
		At(index).SetGrfValidDifference(VIS_VALID_WARPED_DIFF);
	}

	return At(index).ImageDifference();
}

// The second difference function for synthetic images. It computes the difference 
// between the input image corresponding to this synthetic image and the composite of 
// the layers which form this synthetic image and places result in m_composite 
CVisRGBAByteImage& CVisLayer::CompSynthDiff(TVisLayerCompOp comp_op,
									    TVisLayerDiffOp diff_op, CVisRGBAByteImage &im)
{
	if (! (m_valid_layers & VIS_VALID_BOTTOM))
		FormBottomLayer(BottomLayerShape(), comp_op);
	if (! (m_valid_layers & VIS_VALID_COMPOSITE))
		FormDifference(m_bottom_alpha, im, m_composite, 
					   (BottomLayerShape() & im.Shape()), diff_op);

	 // Update Validity information
     m_valid_layers |= VIS_VALID_COMPOSITE;

	return m_composite;
}


///////////////////////////////////////////////////////////////////////////////////////
//
// Layer invalidation: Need to wrap CVisRGBAComposite::Invalidate() to maintain
//                     validity information for the differences
//
///////////////////////////////////////////////////////////////////////////////////////

void CVisLayer::SetCurrentFrame(int index) 
{
	assert(index >= 0 && index <= Length());

	if (index != ICur()) { 
		SetICur(index);
		Invalidate(VIS_VALID_BOTTOM | VIS_VALID_DIFFERENCE);
	}

}

void CVisLayer::InvalidateAll(int mask)
{
	Invalidate(mask);
	
	if (!(mask & VIS_VALID_DIFFERENCE)) {
		for(int i = 0; i < NFrames(); i++) {
			At(i).SetGrfValidDifference(0);
		}
	}
}

void CVisLayer::Invalidate(int mask, int image)
{
	if (!(mask & VIS_VALID_BOTTOM)) {
		// If the bottom layer changes, all differences are invalid
		// (This is not true for synthetic images, but this just results 
		// in slightly more computation. I can't see an easy fix to this
		// except for trying not to call Invalidate on synthetic images.)
		for(int i = 0; i < NFrames(); i++) {
			At(i).SetGrfValidDifference(0);
		}
	}
	else if (!(mask & VIS_VALID_DIFFERENCE)) {
		At(image).SetGrfValidDifference(0);
	}

	CVisRGBAComposite::Invalidate(mask);
}


///////////////////////////////////////////////////////////////////////////////////////
//
// Miscellaneous functions
//
///////////////////////////////////////////////////////////////////////////////////////

void CVisLayer::EstimateBias(TVisLayerCompOp op)
{
	// Remember old current frame
	int old_current_frame = CurrentFrame();

	// Force computation of the bottom layer
	FormBottomLayer(BottomLayerShape(), op);

	for(int k = 0; k < NFrames(); k++) {
		// Force computation of the k^th layer
		SetCurrentFrame(k);
		FormTopLayer(TopLayerShape());

		// Compute intersection of top and bottom layers, and take subimages 
 		CVisShape shape = (m_bottom_alpha.Shape() & m_top_layer.Shape());
		CVisRGBAByteImage top_sub = m_top_layer.SubImage(shape);
		CVisRGBAByteImage bot_sub = m_bottom_alpha.SubImage(shape);

		// Compute statistics
		double sum_top = 0.0, sum_bot = 0.0, sum_bot_squared = 0.0, sum_bot_top = 0.0;
		int num_pixels = 0;
		for(int i = 0; i < shape.Height(); i++) {
			CVisRGBABytePixel *top_pixel = top_sub.PtrToFirstPixelInRow(i+top_sub.Top());
			CVisRGBABytePixel *bot_pixel = bot_sub.PtrToFirstPixelInRow(i+bot_sub.Top());

			for(int j = 0; j < shape.Width(); j++) {
				if (top_pixel->A() == 255 && bot_pixel->A() == 255) {
					num_pixels++;
					double top_intensity = sqrt(double(top_pixel->R() * top_pixel->R() +
													   top_pixel->G() * top_pixel->G() +
													   top_pixel->B() * top_pixel->B()) / 3.0);
					double bot_intensity = sqrt(double(bot_pixel->R() * bot_pixel->R() +
				 									   bot_pixel->G() * bot_pixel->G() +
													   bot_pixel->B() * bot_pixel->B()) / 3.0);
					sum_top         += top_intensity;
					sum_bot         += bot_intensity;
					sum_bot_squared += bot_intensity * bot_intensity;
					sum_bot_top     += top_intensity * bot_intensity;
				}
				top_pixel++;
				bot_pixel++;
			}
		}
			
		// Update the gain and bias	- least squares estimate
		if (num_pixels * sum_bot_squared - sum_bot * sum_bot != 0.0) {
			double new_gain = (num_pixels * sum_bot_top - sum_bot * sum_top) /
								(num_pixels * sum_bot_squared - sum_bot * sum_bot);
			double new_bias = (sum_top * sum_bot_squared - sum_bot * sum_bot_top) /
								(num_pixels * sum_bot_squared - sum_bot * sum_bot);
			// Take into account the fact that that current frame already has a
			// gain and bias
			SetBias(k, Bias(k) + new_bias * Gain(k));
			SetGain(k, new_gain * Gain(k));
			At(k).SetCPixel(num_pixels);
		}
	}
		
	// Restore old value of current frame
	SetCurrentFrame(old_current_frame);
}


///////////////////////////////////////////////////////////////////////////////////////
//
// Overridden view computation functions
//
///////////////////////////////////////////////////////////////////////////////////////

// This could be left in CVisRGBAComposite, but I have copied it here for consistency
void CVisLayer::FormTopLayer(CVisShape shape)
{
	if (shape.Height() > 0 && shape.Width() > 0 ) {
		ProgressReport("Forming top layer");

		// Planar transformation
		CVisTransform4x4 t = LayerTransform(CurrentFrame()).Inverted();

		// Create the resampled image
		if (m_top_layer.Shape() != shape)
			m_top_layer.Allocate(shape);

		// Resample it
		if (shape.Height() > 0 && shape.Width() > 0)
			VisResample(Warped(), m_top_layer, t);
	}
	else {
		ProgressReport("Top layer with background is empty");

		m_top_layer.Allocate(1,1);
	}

    // Update Validity information
	m_valid_layers |= VIS_VALID_TOP;
}

// This could be left in CVisRGBAComposite, but I have copied it here for consistency
void CVisLayer::FormTopWBkgd(CVisShape shape)
{
	if (shape.Height() > 0 && shape.Width() > 0 ) {
		ProgressReport("Forming top layer with background");

		// Create the top layer image with background
		if (m_top_w_bkgd.Shape() != shape)
			m_top_w_bkgd.Allocate(shape);

		// Form the visible top layer (with desired opaque background value)
		int g = m_background_gray_level;
		m_top_w_bkgd.FillPixels(CVisRGBABytePixel(g, g, g, 255));
		VisComposite(m_top_layer, m_top_w_bkgd, m_top_w_bkgd, composite_over);
	}
	else {
		ProgressReport("Top layer with background is empty");

		m_top_w_bkgd.Allocate(1,1);
	}

    // Update Validity information
    m_valid_layers |= VIS_VALID_TOP_BKGD;
}


void CVisLayer::FormBottomLayer(CVisShape shape, TVisLayerCompOp op)
{
	if (shape.Height() > 0 && shape.Width() > 0 ) {
		ProgressReport("Forming bottom layer");

		// Allocate the bottom layer
		if (m_bottom_layer.Shape() != shape) {
			m_bottom_layer.Allocate(shape);
			m_bottom_alpha.Allocate(shape);
		}

		// Leave the real work to new compositing function which can be plugged in here
		switch(op) {

		case layer_comp_feather: 
			LayerCompFeather();
			break;

		case layer_comp_over:
			LayerCompOver();
			break;

 		case layer_comp_median:
			assert(0);
			break;

		default:
			assert(0);
			break;
		}

		ProgressReport("Finishing up bottom layer");

		// Form the visible bottom layer (with desired opaque background value)
		int g = m_background_gray_level;
		m_bottom_layer.FillPixels(CVisRGBABytePixel(g, g, g, 255));
		VisComposite(m_bottom_alpha, m_bottom_layer, m_bottom_layer, composite_over);
	}
	else {
		ProgressReport("Bottom layer is empty");

		m_bottom_layer.Allocate(1,1);
		m_bottom_alpha.Allocate(1,1);
	}

    // Update Validity information
    m_valid_layers |= VIS_VALID_BOTTOM;
}

// This is largely copied from CVisRGBAComposite::FormComposite()
// I have just rewritten it so that the compositing operator is passed as
// an argument rather than being calculated using CVisRGBAComposite::DisplayMode().
// I chose to do this so we can forget about updating the display mode of the 
// CVisRGBAComposite object and just keep it in the CVisLayeredComposite object
void CVisLayer::FormComposite(CVisShape shape, EVisComposite comp_op)
{
	if (shape.Height() > 0 && shape.Width() > 0 ) {
		ProgressReport("Forming composite");

		// Allocate the composite layer
		if (m_composite.Shape() != shape)
			m_composite.Allocate(shape);
    
		// The correct code should first trim the composite, bottom and top
		// layers by the supplied shape, and then overlay them ...
		if ((m_composite.Shape() == m_top_layer.Shape()) &&
			(m_composite.Shape() == m_bottom_layer.Shape()))
			VisComposite(m_top_layer, m_bottom_layer, m_composite, comp_op);
		else {
			if (m_composite.Shape() == m_bottom_layer.Shape())
				m_bottom_layer.CopyPixelsTo(m_composite);
			else {
				int g = m_background_gray_level;
				m_composite.FillPixels(CVisRGBABytePixel(g, g, g, 255));
				CRect sh = (m_composite.Shape() & m_bottom_layer.Shape());
				if (sh.Height() > 0 && sh.Width() > 0) { // check non-empty
					CVisRGBAByteImage bott_sub = m_bottom_layer.SubImage(sh);
					CVisRGBAByteImage comp_sub = m_composite.SubImage(sh);
					bott_sub.CopyPixelsTo(comp_sub);
				}
			}
			CVisShape top_shape = m_top_layer.Shape();
			top_shape &= m_composite.Shape();
			CVisRGBAByteImage top_sub = m_top_layer.SubImage(top_shape);
			CVisRGBAByteImage comp_sub = m_composite.SubImage(top_shape);
			VisComposite(top_sub, comp_sub, comp_sub, comp_op);
		}
	}
	else {
		ProgressReport("Composite is empty");

		m_composite.Allocate(1,1);
	}

    // Update Validity information
    m_valid_layers |= VIS_VALID_COMPOSITE;
}

// Like FormBottomLayer, this is just a skeleton. The real work is done by
// the new differencing operators which we will define.
void CVisLayer::FormDifference(CVisRGBAByteImage &im1, CVisRGBAByteImage &im2, 
							   CVisRGBAByteImage &im3, CVisShape shape, TVisLayerDiffOp op)
{
    if (shape.Height() > 0 && shape.Width() > 0 ) {
		ProgressReport("Forming difference");

		// Allocate the composite layer
		if (im3.Shape() != shape)
			im3.Allocate(shape);
		
		// Zero out the pixels 
		im3.FillPixels(CVisRGBABytePixel(0, 0, 0, 0));

		switch (op) {

		case layer_diff_intensity:
			LayerDiffIntensity(im1, im2, im3);
			break;

		case layer_diff_normal_flow:
			assert(0);
			break;

		case layer_diff_residual_flow:
			LayerDiffIntensity(im1, im2, im3);			 // Called to set the alpha value correctly
			CopyFlowIntoDifference(CurrentFrame(), im3); 
			break;

		case layer_diff_int_res_flow:
			LayerDiffIntensity(im1, im2, im3);
			CopyFlowIntoDifference(CurrentFrame(), im3);
			break;

		default:
			assert(0);
			break;
		}
	}
	else {
		ProgressReport("Difference is empty");

		im3.Allocate(1,1);
	}
}


///////////////////////////////////////////////////////////////////////////////////////
//
// New compositing functions should be added here
//
///////////////////////////////////////////////////////////////////////////////////////

// This first function is largely copied from CVisRGBAComposite::FormBottomLayer()
void CVisLayer::LayerCompFeather()
{
	CVisShape shape = m_bottom_layer.Shape();

	// Initialize accumulator
	if (m_accumulator.Height() == 0)
		//m_accumulator = CVisRGBAIntImage(shape);
		m_accumulator = CVisRGBAFloatImage(shape);
	if (m_accumulator.Shape() != shape)
		m_accumulator.Allocate(shape);
	m_accumulator.ClearPixels();

	// Sequence through the images 
	for (int i = 0; i < NFrames(); i++) {
		if (CompositeMembership(i) == VIS_LAYER_MEMBERSHIP_IN) {
			ProgressReport("Compositing frame %d", i);
			
			// Get the transform and the compute the shape to be warped onto
			CVisTransform4x4 t = LayerTransform(i).Inverted();
			CVisShape sh = LayerShape(i, 1); 
			sh &= shape;

			if (sh.Height() > 0 && sh.Width() > 0) {
				// Actually resample image i
				CVisRGBAByteImage layer(sh);
				VisResample(Warped(i), layer, t);

				// Compute weighting function from warped layer
                //CVisByteImage& w0 = m_feather[i];
                CVisFloatImage& w0 = (*this)[i].ImageFeather();
                if (Warped(i).Shape() != w0.Shape())
                    w0 = VisFloatCityBlockDistance(Warped(i), m_feather_distance); 
				//CVisByteImage w(sh);
				CVisFloatImage w(sh);
                VisResample(w0, w, t);

				// Accumulate image i
				VisAccumulate(layer, m_accumulator, w, 0);
			}
		}
	}

	// Final normalization and readout
    VisAccumulateNormalize(m_accumulator, m_bottom_alpha, 0);
}

// Also largely copied from CVisRGBAComposite::FormBottomLayer()
void CVisLayer::LayerCompOver()
{
	CVisShape shape = m_bottom_layer.Shape();
    m_bottom_alpha.ClearPixels();

	// Sequence through the images 
#ifdef LAST_TO_FIRST
	for (int i = NFrames() - 1; i >= 0; i--) 
#else
	for (int i = 0; i < NFrames(); i++) 
#endif
    {
		if (CompositeMembership(i) == VIS_LAYER_MEMBERSHIP_IN) {
			// Get the transform and the compute the shape to be warped onto
			CVisTransform4x4 t = LayerTransform(i).Inverted();
			CVisShape sh = LayerShape(i, 1); 
			sh &= shape;

			if (sh.Height() > 0 && sh.Width() > 0) {
				// Actually resample image i
				CVisRGBAByteImage layer(sh);
				VisResample(Warped(i), layer, t);
				CVisRGBAByteImage bott_sub = m_bottom_alpha.SubImage(sh);

				ProgressReport("Compositing frame %d", i);
				
				// Composite over the previously resampled images
				VisComposite(layer, bott_sub, bott_sub, composite_over);
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////
//
// New differencing functions should be added here
//
///////////////////////////////////////////////////////////////////////////////////////

void CVisLayer::CopyFlowIntoDifference(int index, CVisRGBAByteImage &img)
{
	if (m_local_warp) {
		// Get the Flow Image and the layer transformation
		CVisFloatImage &flow = At(index).ImageLocalWarp(); 
		CVisTransform4x4  t = LayerTransform(index);
		CVisTransform4x4 ti	= t.Inverted();

		// Force the recomputation of m_warp_uv
		LocalWarp(index);

		// Warp forward the local flow image and m_warp_uv - first compute shape
		CVisShape warped_flow_shape	= VisResampleShape(flow.Shape(), t);
		warped_flow_shape.SetNBands(2);
		CVisShape warped_uv_shape   = VisResampleShape(m_warp_uv.Shape(), t);
		warped_uv_shape.SetNBands(2);

		// Warp forward the flow image and m_warp_uv - allocate space 
		CVisFloatImage warped_flow(warped_flow_shape);
		CVisFloatImage warped_uv(warped_uv_shape);

 		// Warp forward the flow image and m_warp_uv - finally perform warping
		VisResample(flow, warped_flow, ti);
		VisResample(m_warp_uv, warped_uv, ti);

		// Compute the intersection of the three shapes
   		CRect shape = (CRect(warped_flow_shape) & CRect(warped_uv_shape));
		shape  &= CRect(img.Shape());

		if (shape.Height() > 0 && shape.Width() > 0) {
			// Take the subimages
			CVisFloatImage      uv = warped_flow.SubImage(shape);
			CVisFloatImage     uv2 = warped_uv.SubImage(shape);
			CVisRGBAByteImage cuv = img.SubImage(shape);	  		

			// Copy the flow values into the difference image img
			for (int r = 0; r < uv.Height(); r++) {
				float *pf = uv.PtrToFirstPixelInRow(r+uv.Top());
				float *bf = uv2.PtrToFirstPixelInRow(r+uv2.Top());
				CVisRGBABytePixel *pc = cuv.PtrToFirstPixelInRow(r+cuv.Top());
				for (int c = 0; c < uv.Width(); c++, pf += 2, bf += 2, pc++) {
					int iu = int(fabs((pf[0] - bf[0]) * m_dblFlowScale));
					int iv = int(fabs((pf[1] - bf[1]) * m_dblFlowScale));
					pc->SetG((iu > 255) ? 255 : iu);
					pc->SetB((iv > 255) ? 255 : iv);
				}
			}
		}
	}
}
	
void CVisLayer::LayerDiffIntensity(CVisRGBAByteImage &im1, CVisRGBAByteImage &im2,
								   CVisRGBAByteImage &im3)
{
	// Crop the two images to be differenced
	CVisRGBAByteImage top_sub = im1.SubImage(im3.Shape());
	CVisRGBAByteImage bot_sub = im2.SubImage(im3.Shape());

	// Loop over all rows
	for(int i = 0; i < im3.Height(); i++) {
		// 
		CVisRGBABytePixel *diff_pixel = im3.PtrToFirstPixelInRow(i+im3.Top());
		CVisRGBABytePixel *top_pixel = top_sub.PtrToFirstPixelInRow(i+top_sub.Top());
		CVisRGBABytePixel *bot_pixel = bot_sub.PtrToFirstPixelInRow(i+bot_sub.Top());

		// Loop over all columns
		for(int j = 0; j < im3.Width(); j++, diff_pixel++, top_pixel++, bot_pixel++) {
			if (top_pixel->A() == 255 && bot_pixel->A() == 255) {
				// Assume most pixels are completely opaque
				int temp = (top_pixel->R() - bot_pixel->R()) * (top_pixel->R() - bot_pixel->R())
						 + (top_pixel->G() - bot_pixel->G()) * (top_pixel->G() - bot_pixel->G())
					     + (top_pixel->B() - bot_pixel->B()) * (top_pixel->B() - bot_pixel->B());
				temp = double(temp);
				diff_pixel->SetR((temp > 255) ? 255 : temp);
				diff_pixel->SetA(255);
			}
			else if (top_pixel->A() && bot_pixel->A()) {
				// Correct dealing of alpha values for premultiplied alphas
				int temp = ((bot_pixel->A() * top_pixel->R() - top_pixel->A() * bot_pixel->R()) * 
							(bot_pixel->A() * top_pixel->R() - top_pixel->A() * bot_pixel->R())) +
						   ((bot_pixel->A() * top_pixel->G() - top_pixel->A() * bot_pixel->G()) * 
							(bot_pixel->A() * top_pixel->G() - top_pixel->A() * bot_pixel->G())) +
					       ((bot_pixel->A() * top_pixel->B() - top_pixel->A() * bot_pixel->B()) * 
							(bot_pixel->A() * top_pixel->B() - top_pixel->A() * bot_pixel->B()));
				temp /= 255 * 255;
				temp = double(temp);
				diff_pixel->SetR((temp > 255) ? 255 : temp);
				diff_pixel->SetA((top_pixel->A() + bot_pixel->A()) / 2);
			}
			else {
				// At least one of the two pixels is not there, so can't compute a difference
				diff_pixel->SetR(0);
				diff_pixel->SetA(0);
			}
		}
	}
}


//
// Virtual CVisSequence methods
// 

const type_info& CVisLayer::ImageTypeInfo(void) const
{
	return typeid(T_Frame);
}

const type_info& CVisLayer::ObjectTypeInfo(int iLevel) const
{
	if ((iLevel < 0) || (iLevel >= eilevelLayer))
		return typeid(CVisLayer);

	return CVisRGBAComposite::ObjectTypeInfo(iLevel);
}


CVisSequenceBase *CVisLayer::Clone(bool fCopyThis) const
{
	if (fCopyThis)
		return new CVisLayer(*this);

	return new CVisLayer;
}


void CVisLayer::SDReadWriteProperties(CVisSDStream& s,
		bool fAddComma)
{
	// UNDONE
	CVisRGBAComposite::SDReadWriteProperties(s, fAddComma);
}
