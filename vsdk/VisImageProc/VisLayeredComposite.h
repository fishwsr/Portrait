///////////////////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisLayeredComposite.h -- multi-layer stereo reconstruction
//
// DESCRIPTION
//  The CVisLayeredComposite class is used to extract a multi-plane
//  stereo description from a collection of images.
//
//  The class contains the following major structures
//    CVisLayer m_input;              // input image collection w/ geometry
//    CVisLayer m_label;              // layer labels (-1 == unassigned)
//    CVisLayerArray m_layer;         // array of layer composites/mosaics + homographies
//    CVisLayer m_final_layer;        // final layer estimates w/ geometry
//    CVisLayer m_depth;              // scaled depth estimates
//    CVisLayerArray m_synthesized;   // re-synthesized image composites
//    CVisLayer m_synth_label;        // re-synthesized label composites
//    CVisIntVector m_id;             // id associate with each layer
//  These are used to hold the input image, the current labelling of each input,
//  the composites needed to compute each layer, and the composite used
//  to re-synthesize the input (for label updating).
//
//  The CVisLayeredView class is used to hold a current view (image number
//  and layer number) into a layered stereo collection.
//
// PUBLIC MEMBER FUNTIONS (METHODS)
//  CVisLayeredComposite();
//  ~CVisLayeredComposite();
//
//  ...
//
// PARAMETERS
//  k               index of image in input sequence
//  l               index of layer in layered output sequence
//  ...
//
// SEE ALSO
//  VisLayeredComposite.cpp     layered stereo implementation code
//  VisLayer.h            single layere composite class
//  VisComposite.h        composite (mosaic) class
//  VisCollection.h       image collection class
//  VisBlend.h            image blending / compositing operations
//  VisAlign.h            interactive and automatic alignment algorithms
//  VisCylindrical.h      cylindrically warped composites
//
//  Rick Szeliski's research notes, 27-Jun-97 to 24-Jul-97.
//
// BUGS
//   
//
// DESIGN
//  The design of the array (vector) of composites is tricky:
//  . We want it to be an array of pointers, because composites are
//  heavyweight objects to be creating on the fly (e.g., as default argument
//  to vector routines).
//  . We want the copy constructor and assignment to work correctly, but
//  also want to have the appropriate behavior occur during destruction
//  and vector element deletion.
//
//  Two designs come to mind:
//  1) create a ref-counted pointer encapsulation class with the right
//  copy/destruction behavior
//  2) use simple pointers, but remember to delete them before they are
//  actually removed from a list
//  The second is simpler, but more fraught with danger (don't pass around
//  these objects!).  I've decided to implement the former.
//
//  The treatment of layer labels is also problematic.
//
//	Layer id's are stored as pseudo-colored permuted integers 1..L
//	i.e., we de-interleave the bits 3-ways, and bit-reverse the 8-bit numbers
//	  0001 -> 0x800000
//	  0002 -> 0x008000
//	  0003 -> 0x808000
//	  ...
//	  0147 -> 0xa080c0
//	The octal numbers in the left column are associated with file names
//	The hex numbers in the right column define the RGB values.
//
//	The problem is when we insert a new layer in the middle, or re-order
//	layers, or merge layers together.
//
//	Do we re-number all of the layers, and hence modify the m_label entries,
//	or do we just make the layer id's non-ordered and possibly sparse?
//  For now, I will just add a new layer with a new (unique) id, and then
//  later add a "compactification" and "serialization" (renumbering) utility.
//
//  Lastly, if the number of images and layers becomes large (e.g., for a
//	large multi-image panoramic stereo dataset with hundreds of layers), we
//	don't want to assume that every image is present in every layer 
//	composition.  For this reason, we make the layer composites "sparse".
//	A layer contains only those image which were explicitly added to it
//	(we provide a utility routine to add all images to all layers, if
//	desired).
//
//	This makes navigation through a layered stereo data set tricky.
//	If we ask for the next layer in an image, or a next image in a layer,
//	this may not exist.  Instead, the semantics of next layer (segment) for
//	an image refers to the next layer which contains the image.  The next
//	image in a layer is just the next frame, and does not necessarily
//	correspond to the next input image.
//
//  (20-Jul-97)  I changed my mind on the sparsity of collections.
//  Conceptually, they are still sparse.  However, we implement a full
//  2-D image/layer table, but we only set the layer images to be non-empty
//  if there exist some labeled pixels in that layer (the segments that
//  do exist are cropped to their minimum bounding box).  This makes
//  insertion and deletion easier, as well as navigation (the user still
//  sees only the non-empty segments).  The composite class has to be
//  tweaked a little to ignore empty frames.
//
// HISTORY
//  30-Jun-97  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
//  20-Jul-97  Richard Szeliski (szeliski) at Microsoft
//  Changed implementation to full 2-D layer/image table.
//
//  24-Jul-97  Richard Szeliski (szeliski) at Microsoft
//  Broke VisLayer code off from this file.
//
// Copyright © Microsoft Corp. 1997
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _VIS_LAYERED_COMPOSITE_H_
#define _VIS_LAYERED_COMPOSITE_H_

//#include <vector>
//#include "VisLayer.h"
#include "..\VisSfM\VisSfM.h"
//#include "VisSprites.h"


///////////////////////////////////////////////////////////////////////////////////////
//																					 
//  class CVisLayerPtr  --  Smart vector of composite pointers, allocated on demand  
//																		             
///////////////////////////////////////////////////////////////////////////////////////

class CVisLayerPtr {
public:
    CVisLayerPtr() : m_ptr(0) {}
    CVisLayerPtr(const CVisLayerPtr& x) : m_ptr(0) {
        reassign(x.m_ptr);
    }
    ~CVisLayerPtr() {
        reassign(0);
    }
    CVisLayerPtr operator=(const CVisLayerPtr& x) {
        if (this != &x)
            reassign(x.m_ptr);
        return *this;
    }
    CVisLayer& operator*() {
        if (m_ptr == 0)
            reassign(new CVisLayer);
        return *m_ptr;
    }
protected:
    void reassign(CVisLayer *ptr) {
        if (m_ptr)
			m_ptr->Release();
        m_ptr = ptr;
        if (ptr)
            ptr->AddRef();
    }
    CVisLayer *m_ptr;
};


#if (_MSC_VER >= 1100)	// VC 5.0 or greater
typedef std::vector<CVisLayerPtr, std::allocator<CVisLayerPtr> > CVisLayerArray;
typedef std::vector<int, std::allocator<int> > CVisIntVector;
inline bool operator<(const CVisLayerPtr& a, const CVisLayerPtr& b)
{ return 0; }
inline bool operator==(const CVisLayerPtr& a, const CVisLayerPtr& b)
{ return 0; }
#else // (_MSC_VER >= 1100)
typedef vector<CVisLayerPtr, allocator<CVisLayerPtr> > CVisLayerArray;
typedef vector<int, allocator<int> > CVisIntVector;
#endif // (_MSC_VER >= 1100)


///////////////////////////////////////////////////////////////////////////////////////
//																					 
//          class CVisLayeredComposite  --  Multi-layer stereo reconstruction	     
//																		             
///////////////////////////////////////////////////////////////////////////////////////
//
// There are a number of different ways to predict the pixel assignment
//
enum TVisLayerPredOp {
    layer_thresholded_wta,      // winner that meets threshold
    layer_topmost               // topmost pixel (ignore difference)
};

enum TVisLayerDepthOp {
    layer_depth_pseudo_color,   // use 10 bit pseudocolors (B -> C -> G -> Y -> B)
    layer_depth_gray,           // use 8 bits gray level (R == G == B)
    layer_depth_24bit,          // use 24 bits in RGB
    layer_depth_float           // use floating point (not displayable)
};


class CVisLayeredComposite : public CVisDiagnostics {

public:
    // Constructor and destructor
    CVisLayeredComposite();
    ~CVisLayeredComposite();

    // Size functions
    int NImages()                                   { return m_input.NFrames(); }
    int NLayers()                                   { return m_final_layer.NFrames(); }
    int IsEmpty(int k, int l)                       { return (*m_layer[l]).IsEmptyFrame(k); }
    int IsEmpty(int l)                              { return (*m_layer[l]).IsEmpty(); }

	// Accessing operator member variables
	TVisLayerCompOp CompositingOperator()           { return m_comp_op; }
	TVisLayerCompOp SynthCompositingOperator()      { return m_synth_comp_op; }
	TVisLayerDiffOp DifferencingOperator()          { return m_diff_op; }
	TVisLayerDiffOp SynthDifferencingOperator()     { return m_synth_diff_op; }
	TVisLayerPredOp PredictionOperator()            { return m_pred_op; }
	TVisLayerDepthOp DepthOperator()                { return m_depth_op; }
	
	// Setting operator member variables
	void SetCompositingOperator(TVisLayerCompOp op);        
	void SetDifferencingOperator(TVisLayerDiffOp op);
	void SetSynthCompositingOperator(TVisLayerCompOp op);
	void SetSynthDifferencingOperator(TVisLayerDiffOp op);
	void SetPredictionOperator(TVisLayerPredOp op);
	void SetDepthOperator(TVisLayerDepthOp op);

	// Access to background grey level member variable
	int BackgroundGrayLevel()                       { return m_background_gray_level; }
	void SetBackgroundGrayLevel(int bgl);

    // Accessing input (and label) images
    CVisLayer          &InputCollection()           { return m_input; }
    CVisRGBAByteFrame &Input(int k)                { return m_input.Frame(k); }
    CVisRGBAByteImage &Image(int k)                { return Input(k); }
    CVisTransformChain       &InputGeometry(int k)        { return Input(k).TransformChain(); }
	CVisRGBAByteFrame &Label(int k)		        { return m_label.Frame(k); }
    CVisRGBAByteImage &InputLabel(int k)           { return m_label.Frame(k); }

    // Accessing layer and synthetic composites
    CVisLayer          &LayerComposite(int l)       { return *m_layer[l]; }
    CVisLayer          &SynthComposite(int k)       { return *m_synthesized[k]; }
	CVisRGBAByteImage &SynthLabel(int k)           { return m_synth_label.Frame(k); }

    // Accessing final layer information
    CVisLayer          &FinalCollection()           { return m_final_layer; }
    CVisRGBAByteFrame &FinalLayer(int l)           { return m_final_layer.Frame(l); }
    CVisRGBAByteImage &LayerImage(int l)           { return FinalLayer(l); }
    CVisTransformChain       &LayerGeometry(int l)        { return FinalLayer(l).TransformChain(); }

    // Accessing depth image information
    CVisLayer          &DepthCollection()           { return m_depth; }
    CVisRGBAByteFrame &DepthLayer(int l)           { return m_depth.Frame(l); }
    CVisRGBAByteImage &DepthImage(int l)           { return DepthLayer(l); }

    // File I/O
    void ReadFile(const char *filename);                 // read  ASCII layer description
    void WriteFile(const char *filename);                // write ASCII layer description
    void ReadFile(FILE *stream, const char *filename);
    void WriteFile(FILE *stream, const char *filename);

    // Inserting and deleting images (and/or camera descriptors)
    void InsertImage(CVisRGBAByteFrame& frame);
    void InsertImage(const char *filename);
    void DeleteImage(int index);
    void InsertCameras(CVisModel3D &m3d);                // insert camera matrices (+ images?)
 
	// Inserting, deleting, and moving layers
	void InsertLayer(int index = -1);
    void RaiseLayer(int index, bool to_top = true);
    void LowerLayer(int index, bool to_bottom = true);
    void DeleteLayer(int index);
    void DeleteAll();

    // Updating final layer and synthesized images	
    void UpdateFinalLayer(int l);						 // update FinalLayer(l).Image(l)
    void UpdateFinalCollection();						 // update all m_final_layer
    void UpdateDepthImage(int l);                        // update depth image for layer l
    void UpdateDepthCollection();                        // update all depth images
    void SynthUpdate(int k, int show_depth = 0);         // update k^th synthetic image
    void SynthUpdateAll(int show_depth = 0);             // update all synthetic images

	// Predicting and updating pixel assignment
	void PredictPixelAssignment();                       // and update m_synth_label
	void UpdatePixelAssignment();                        // copy m_synth_label onto m_label

	// Pixel assignment prediction operators
	void ThresholdedWTA(CVisRGBAByteImage &label,
		                CVisLayer &synth);				 // Thresholded winner takes all
    void TopmostPixel(CVisRGBAByteImage &label,
		                CVisLayer &synth);				 // Topmost pixel determines label

    // Pseudocolor computation
    int                LayerId(int l);                   // id associated with layer
    int                IdFromColor(CVisRGBABytePixel c);// permuted RGBA color	-> layer id
    CVisRGBABytePixel ColorFromLayer(int l);            // layer # -> permuted RGBA color
    int                LayerColor(int l);                // layer # -> permuted color as int

    // Masked image (segment) extraction
    void ExtractAllSegments();                           // extract all cropped segments
    void ExtractSegments(int k);                         // all segments of image k
    void ExtractSegment(int k, int l, int crop = 1);     // layer l segment of image k
    void ExtractEntireImages();                          // extract entire uncropped images
    void ExtractPartiallyCroppedImages();                // extract partially cropped images
	
    // Masked image extraction helper functions
    void ExtractSegment(int k, int l, CVisRGBAByteImage &img, int highlight = 0); 
	void ExtractSingleLayer(CVisRGBAByteImage &img, int l, CVisRGBAByteImage &img2);
	void RemoveLayer(CVisRGBAByteImage &img, int l);

	// Extract correct band for displaying diff	based on the differencing operator used
	void ExtractDiffBand(CVisRGBAByteImage &img, TVisLayerDiffOp op);		 
	
	// Bias and Gain manipulation functions
	void BiasNormalize(CVisRGBAByteImage &img, 	     // Normalize image to take into
		               double bias, double gain);        // account the bias and gain
	void EstimateBias();				                 // Estimate bias of all layers and average

	// Deghosting functions
	void DeGhostAll();	                                 // Deghost all the layers
	void DeGhost(int l);                                 // Deghost a single layer

    // Structure from motion and stereo
    void BuildModel3D(int clear_old = 1);                // build or update 3D model
    void EstimateMotion();                               // estimate camera positions
    void EstimatePlanes();                               // estimate plane equations
    void EstimateParallax();                             // estimate out-of-plane depth

    // Conversion to simpler Sprite Composite class
//    void BuildSpriteComposite(CVisSpriteComposite& sprite_composite);
    void BuildSpriteComposite();
    CVisSpriteComposite& SpriteComposite() { return m_sprites; }

protected:
    // Layer and synthetic invalidating functions
    void InvalidateAll(int mask = 0);                    // invalidate all layers
    void InvalidateAllSynth(int mask = 0);               // all synthetic image composites

    // Miscellaneous helper functions
    CVisShape ShapeOfSegment(int k, int l);
    void      InsertLabel();			               	 // create new label image
    int       UniqueId();				                 // generate an unused id

protected:
    CVisLayer       m_input;	    // input image collection w/ geometry
    CVisLayer       m_label;		// layer label pseudocolor (0 == unassigned)
    CVisLayerArray  m_layer;        // array of layer composites/mosaics + homographies
    CVisLayer       m_final_layer;  // final layer estimates w/ geometry
    CVisLayerArray  m_synthesized;  // re-synthesized image composites
    CVisLayer       m_synth_label;  // re-synthesized label composites
    CVisLayer       m_depth;        // depth map images
    CVisIntVector   m_id;           // id associate with each layer

	TVisLayerCompOp m_comp_op;		// operator used to compute the layer composites
	TVisLayerDiffOp m_diff_op;		// for difference between layer comp and warped layer
	TVisLayerCompOp m_synth_comp_op;// operator for compositeing the layers into synthetic
	TVisLayerDiffOp m_synth_diff_op;// for difference between synthetic and original image
	TVisLayerPredOp m_pred_op;      // Pixel assignment prediction operator
	TVisLayerDepthOp m_depth_op;    // depth map color display mode

	int m_background_gray_level;	// Background gray level of the window

    CVisModel3D     m_model;        // 3D model for plane structure and motion recovery
    CVisSpriteComposite m_sprites;  // list of sprites with per-layer parallax

public:
	// Pixel assignment prediction parameters
	int m_use_layer_difference;
	int m_difference_threshold;

	// Miscellaneous publically writeable options
	int m_fract_increase;			// The fractional increase of the partially cropped window
	int m_average_bias;             // Average the bias and gain estimates over the layers
    float m_f_guess;                // focal length guess (x 1/2 width)

	// Adjuster used for deghosting - public so VisLayeredView can write the options
	CVisRGBABlockAdjuster m_adjuster;

public:
	// Publicly setable flags and options
	int             m_write_labels;
	int             m_write_layers;
    static char    *filename_ext;
};


///////////////////////////////////////////////////////////////////////////////////////
//																					 
//                class CVisLayeredView  --  Layered stereo view				     
//																		             
///////////////////////////////////////////////////////////////////////////////////////
//
// 
// CVisLayeredView objects can be in one of five major modes (m_view_type):
//
enum TVisLayerView {
    VIS_LAYER_VIEW_INPUT,		      // Input images or current pixel assignment
    VIS_LAYER_VIEW_LAYER,		      // Layer composites using cropped images
    VIS_LAYER_VIEW_FULL,		      // Layer composites using uncropped images
	VIS_LAYER_VIEW_INTER,			  // Layer composites using partially cropped images
    VIS_LAYER_VIEW_DEPTH,             // Depth images, viewed in synthetic img
    VIS_LAYER_VIEW_SYNTHETIC,	      // Resynthesized input using cropped images
    VIS_LAYER_VIEW_FULL_SYNTH,	      // Resynthesized input using uncropped images
	VIS_LAYER_VIEW_INTER_SYNTH        // Resynthesized input using partially cropped images
};
//
// One of two types of data can be viewed (m_medium):
// 
enum TVisLayerMedium {
	VIS_LAYER_MEDIUM_INTENSITY,		   // Original, layers, sythesized, or difference
	VIS_LAYER_MEDIUM_PIXEL_ASSIGNMENT  // Current or predicted (pixel assignment)
};
//
// (Intensity) data can be displayed in several different modes (m_display_mode):
//
enum TVisLayerDisplayMode {
	VIS_LAYER_DM_SINGLE,		       // No compositing - just display the active image
    VIS_LAYER_DM_COMPOSITE,		       // Only display current layer composite
	VIS_LAYER_DM_OVERLAY,		       // Display active image over composite
	VIS_LAYER_DM_BLINK,			       // Blink active image over composite
	VIS_LAYER_DM_TRANSPARENT,	       // Display active image transparently over composite
	VIS_LAYER_DM_DIFFERENCE, 	       // Difference between active image and composite
	VIS_LAYER_DM_UNWARPED_DIFF 	       // Unwarped difference 
};
//
// A varying numbers of layers can be displayed (m_num_layers):
//
enum TVisLayerNumLayers {
	VIS_LAYER_NUM_SINGLE_LAYER,		  
	VIS_LAYER_NUM_ALL_LAYERS
};


class CVisLayeredView {
public:
	// Constructors and destructors
    CVisLayeredView();

    // Access to the full state of the display mode
    TVisLayerView ViewClass()		              { return m_view_type; }
    TVisLayerView SetViewClass(TVisLayerView v);
    TVisLayerMedium Medium()		              { return m_medium; }
	void SetMedium(TVisLayerMedium m)             { m_medium = m; }
    TVisLayerDisplayMode DisplayMode()	          { return m_display_mode; }
    void SetDisplayMode(TVisLayerDisplayMode dm);	
	TVisLayerNumLayers NumLayers()				  { return m_num_layers; }
	void SetNumLayers(TVisLayerNumLayers num)	  { m_num_layers = num; }

	// Access to m_composite - the currently displayed layer or synthetic image
	CVisLayer &Composite()						  { return *m_composite; }	    
	void SetComposite();							
 
	// Functions for motion between and within the layer composites
    void FirstImage()                             { SwitchImage(-2); }
    void PrevImage()                              { SwitchImage(-1); }
    void NextImage()                              { SwitchImage( 1); }
    void LastImage()                              { SwitchImage( 2); }
    void FirstLayer()                             { SwitchLayer(-2); }
    void PrevLayer()                              { SwitchLayer(-1); }
    void NextLayer()                              { SwitchLayer( 1); }
    void LastLayer()                              { SwitchLayer( 2); }

	// Major display function
    CVisRGBAByteImage& CurrentView();

	// Functions for displaying current image information in the title bar
	const char *ViewName();
    const char *TileName();

protected:
	// Motion helper functions
    void SwitchImage(int delta);		           // Update image pointer (m_image)
    void SwitchLayer(int delta);				   // Update layer pointer (m_layer)

protected:
    // Major data members
	CVisLayeredComposite *m_stereo;				   // Entire layered stereo representation
    CVisLayer            *m_composite;			   // Currently displayed composite

	// The full state of the current view mode
    TVisLayerView        m_view_type;			   // Input, layer, or synthetic
	TVisLayerMedium      m_medium;				   // Intensity or pixel assignment
	TVisLayerDisplayMode m_display_mode;		   // Single, composite, transparency, etc
	TVisLayerNumLayers   m_num_layers;			   // One (which can be changed) or all

	// Current position within the collection of layer composites
    int m_image;
    int m_layer;

private:
	// Scratch image used in CVisLayeredView::CurrentView()
    CVisRGBAByteImage m_img;	         

};

#endif // _VIS_LAYERED_COMPOSITE_H_
