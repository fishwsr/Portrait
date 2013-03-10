///////////////////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisLayeredComposite.cpp -- multi-layer stereo reconstruction
//
// DESCRIPTION
//  The CVisLayeredComposite class is used to extract a multi-plane
//  stereo description from a collection of images.
//
// SEE ALSO
//  VisLayeredComposite.h   longer description of this class
//
// BUGS
//  See VisLayeredComposite.h
//
//  The whole internal representation right now is a mess!
//  We have lots of image collections, whereas we should really only have
//  one richer frame type, e.g., a sprite with depth.
//
// DESIGN
//  See VisLayeredComposite.h
//
// HISTORY
//  30-Jun-97  Richard Szeliski (szeliski) at Microsoft
//  Created.
//
// Copyright © Microsoft Corp. 1997
//
///////////////////////////////////////////////////////////////////////////////////////


#include "VisImageProc.h"

#include "VisLayeredComposite.h"  // redundant, in case it moves from ImageProc.h
//#include <algorithm>              // \msdev\include\algorithm:  find() generic function


///////////////////////////////////////////////////////////////////////////////////////
//
//  Static data member initializtion
//
///////////////////////////////////////////////////////////////////////////////////////

char *CVisLayeredComposite::filename_ext                 = "tga"; 


///////////////////////////////////////////////////////////////////////////////////////
//
//  Constructor and destructor
//
///////////////////////////////////////////////////////////////////////////////////////

CVisLayeredComposite::CVisLayeredComposite()
	// CVisLayer objects
  : m_input(evissequenceDelayReadUntilNeeded),
	m_label(evissequenceDelayReadUntilNeeded),
	m_final_layer(evissequenceDontDelayRead),
	m_synth_label(evissequenceDontDelayRead),
	m_depth(evissequenceDontDelayRead),
	// CVisLayerArray objects
	m_layer(),
	m_synthesized()
{
	// Default compositing and differencing operators
	m_comp_op       = layer_comp_feather;
	m_diff_op       = layer_diff_intensity;
	m_synth_comp_op = layer_comp_over;
	m_synth_diff_op = layer_diff_intensity;
    m_depth_op      = layer_depth_pseudo_color;

	// Pixel assignment prediction parameters
	m_pred_op              = layer_thresholded_wta;
 	m_use_layer_difference = 1;
	m_difference_threshold = 5;
	
	// Miscellaneous publically writeable options
	m_fract_increase = 25;
	m_average_bias   =  1;
    m_f_guess        = 0.625;       // about right for Dayton data set?

	// Publicly setable flags and options
	m_write_labels = 0;
	m_write_layers = 0;
	m_background_gray_level  = 0;

	// Set the default adjuster options
	m_adjuster.subsample    = 16;
	m_adjuster.search_area2 =  4;
	m_adjuster.m_matchpairs   =  0;
}

CVisLayeredComposite::~CVisLayeredComposite()
{
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  Setting operator member variables - changing an operator may invalidate a lot
//
///////////////////////////////////////////////////////////////////////////////////////

void CVisLayeredComposite::SetCompositingOperator(TVisLayerCompOp op)        
{ 
	if (m_comp_op != op) {
		m_comp_op = op; 
		m_write_layers = 1;
		InvalidateAll(); 
	}
}

void CVisLayeredComposite::SetDifferencingOperator(TVisLayerDiffOp op)  
{ 
	if (m_diff_op != op) {
		m_diff_op = op;
		InvalidateAll(VIS_VALID_TOP | VIS_VALID_BOTTOM |
					  VIS_VALID_TOP_BKGD | VIS_VALID_COMPOSITE);
	}
}

void CVisLayeredComposite::SetSynthCompositingOperator(TVisLayerCompOp op) 
{ 
	if (m_synth_comp_op != op) {
		m_synth_comp_op = op; 
		InvalidateAllSynth(); 
	}
}

void CVisLayeredComposite::SetSynthDifferencingOperator(TVisLayerDiffOp op)
{ 
	if (m_synth_diff_op != op) {
		m_synth_diff_op = op;
		InvalidateAllSynth(VIS_VALID_TOP | VIS_VALID_BOTTOM |
					       VIS_VALID_TOP_BKGD);
	}
}

void CVisLayeredComposite::SetPredictionOperator(TVisLayerPredOp op)
{
	if (m_pred_op != op) {
		m_pred_op = op;
		PredictPixelAssignment();
	}
}

void CVisLayeredComposite::SetDepthOperator(TVisLayerDepthOp op)
{
	if (m_depth_op != op) {
		m_depth_op = op;
        UpdateDepthCollection();
	}
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  Access to background grey level member variable
//
///////////////////////////////////////////////////////////////////////////////////////

void CVisLayeredComposite::SetBackgroundGrayLevel(int bgl)
{
	m_background_gray_level = bgl;
	m_input.SetBackgroundGrayLevel(bgl);
	m_final_layer.SetBackgroundGrayLevel(bgl);
	for(int l = 0; l < NLayers(); l++)
		LayerComposite(l).SetBackgroundGrayLevel(bgl);
	for(int k = 0; k < NImages(); k++)
		SynthComposite(k).SetBackgroundGrayLevel(bgl);
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  File I/O - This is not robust to incorrectly formated input
//
///////////////////////////////////////////////////////////////////////////////////////

// Should be the same as in VisCollection.h:
static char *collection_header = "# ImageCollection, frame = %s";

void CVisLayeredComposite::ReadFile(const char *filename)
{
    // Open the file
    FILE *stream;
    if (filename == 0 || filename[0] == 0 || strcmp(filename, "-") == 0)
        stream = stdin;
    else
        stream = fopen(filename, "r");
    if (stream == 0)
        throw CVisFileIOError(filename, "Could not open the file for reading",
            eviserrorOpen, "CVisLayeredComposite::ReadFile", __FILE__);

    // Read the constituent structures
    ReadFile(stream, filename);

    // Read the 3D model
    CVisSDStream sdstream;
    sdstream.Open(0, CVisSDStream::Read, stream);
    try {
        sdstream << m_model;
    } catch (CVisFileIOError &err) {
        // No 3D model was appended to file
        CVisDiagnostics::ProgressReport("No 3D model was found in %s",
            filename);
        int ignore = int(&err);
    }

    // Close the file
    if (stream && stream != stdout)
        fclose(stream);
}

void CVisLayeredComposite::WriteFile(const char *filename)
{
    // Open the file
    FILE *stream;
    if (filename == 0 || filename[0] == 0 || strcmp(filename, "-") == 0)
        stream = stdout;
    else
        stream = fopen(filename, "w");
    if (stream == 0)
        throw CVisFileIOError(filename, "Could not open the file for writing",
            eviserrorOpen, "CVisLayeredComposite::WriteFile", __FILE__);

    // Write the constituent structures
    WriteFile(stream, filename);

    // Write the 3D model
    CVisSDStream sdstream;
    sdstream.Open(0, CVisSDStream::Write, stream);
    sdstream.Comment("\n3D model: ");
    sdstream << m_model;

    // Close the file
    if (stream && stream != stdout)
        fclose(stream);
}

void CVisLayeredComposite::ReadFile(FILE *stream, const char *filename)
{
	// Check the header (after that, assume it succeeds)
	char line[1024];
	fgets(line, 1024, stream);
	if (strcmp(line, "# Layered Stereo File\n"))
		throw CVisFileIOError(filename, "Not a legal .MLS (Layered Stereo) file",
							  eviserrorRead, "CVisLayeredComposite::ReadFile",
							  __FILE__);

    // Read the input image collection
    ProgressReport("Reading input images");
    fscanf(stream, "\n# Input Image Collection:\n");
    m_input.ReadCollection(filename, 1, stream);
 
    // Read the input image labels
    ProgressReport("Reading label images");
    fscanf(stream, "\n# Input Labels:\n");
    m_label.ReadCollection(filename, 1, stream);
	m_write_labels = 0;

    // Read the output layers collection
    ProgressReport("Reading layer images");
    fscanf(stream, "\n# Final Layers:\n");
    try {
        m_final_layer.ReadCollection(filename, 1, stream);
    	m_write_layers = 0;
    } 
    catch (CVisFileIOError &err) {
        if (err.ErrorCode() == eviserrorShape) { // Layer files inconsistent
            m_write_layers = 1;     // have to write new ones
            m_final_layer.Invalidate();
        } else
            err.ThrowLast();      // some other exception
    }

    // Re-create the id list
    for (int l = 0; l < NLayers(); l++) {
        const char *filename = m_final_layer[l].Name();
        char *slash = strrchr(filename, '\\');
        if (slash)
            filename = slash+1;
        else if ((slash = strrchr(filename, '/')) != (char *) 0)
            filename = slash+1;
        int id;
        sscanf(filename, "L%o", &id);
        m_id.push_back(id);

        // Create the depth images
        char depth_name[1024];
        sprintf(depth_name, "D%03o.%s", id, filename_ext);
        CVisRGBAByteFrame dframe(LayerImage(l).Shape());
        dframe.SetName(depth_name);
        m_depth.PushBack(dframe);
    }

    // Read the individual layer composites
    fscanf(stream, "\n# Layer Composites:\n");
    m_layer.resize(m_final_layer.NFrames());	// allocate composites
    for (l = 0; l < NLayers(); l++) {
		ProgressReport("Reading layer %d", l);

        CVisLayer &comp = LayerComposite(l);
        comp.ReadCollection(filename, 0, stream, VIS_LAYER_MEMBERSHIP_IN);
		comp.SetCurrentFrame(0);
		comp.InvalidateWarp(1);
	}

    // Update the synthetic composites and label composites
	for(int k = 0; k < NImages(); k++) {
		ProgressReport("Inserting synthetic image %d", k);

		m_synthesized.push_back(CVisLayerPtr());
		CVisLayer &synth = SynthComposite(k);
		while (synth.NFrames() < NLayers()) {
			const char *filename = FinalLayer(synth.NFrames()).Name();
//			synth.Insert(filename, -1, VIS_LAYER_MEMBERSHIP_IN);
			synth.PushBack(filename);
			synth.Back().SetEVisLayerMembership(VIS_LAYER_MEMBERSHIP_IN);
		}

		CVisRGBAByteFrame &frame = m_input.Frame(k);
//		CVisRGBAByteImage synth_label(frame.Shape());
//		synth_label.ClearPixels();
//		CVisRGBAByteFrame synth_label_frame(synth_label);
//		m_synth_label.Insert(synth_label_frame);
		m_synth_label.PushBack(frame.Shape());
		m_synth_label.Back().ClearPixels();
	}
}

void CVisLayeredComposite::WriteFile(FILE *stream, const char *filename)
{
    fprintf(stream, "# Layered Stereo File\n");

    // Write the input image collection
    fprintf(stream, "\n# Input Image Collection:\n");
    m_input.WriteCollection(filename, "filename_geometry", 0, stream, 1);
    fprintf(stream, "# end of Image Collection\n");

    // Write the input image labels
    fprintf(stream, "\n# Input Labels:\n");
    m_label.WriteCollection(filename, "filename_only", m_write_labels, stream);
    fprintf(stream, "# end of Image Collection\n");

    // Write the output layers collection
    if (m_write_layers)
        UpdateFinalCollection();
    m_final_layer.SetInvisiblePixel(-2);
    fprintf(stream, "\n# Final Layers:\n");
    m_final_layer.WriteCollection(filename, "filename_layered_stereo_shape", m_write_layers, stream);
    fprintf(stream, "# end of Image Collection\n");

    // Write the individual layer composites
    fprintf(stream, "\n# Layer Composites:\n");
    assert(m_final_layer.NFrames() == m_layer.size());
    for (int l = 0; l < NLayers(); l++) {
        CVisLayer &comp = LayerComposite(l);
        comp.WriteCollection(filename, "filename_layered_stereo_shape", 0, stream, 1);
	    fprintf(stream, "# end of Image Collection\n");
    }
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  Inserting and deleting images
//
///////////////////////////////////////////////////////////////////////////////////////

void CVisLayeredComposite::InsertImage(CVisRGBAByteFrame & frame)
{
	// UNDONE:  Is this method needed?
    m_input.Insert(frame);
	InsertLabel();
}

void CVisLayeredComposite::InsertImage(const char *filename)
{
	// UNDONE:  Is this method needed?
    m_input.Insert(filename);
	InsertLabel();
}

void CVisLayeredComposite::DeleteImage(int index)
{
	// UNDONE:  Is this method needed?
	assert(index >= 0 && index <= NImages());

	// Delete the image from the input, label, and synthetic label composites
	m_input.Delete(index);
	m_label.Delete(index);
	m_synth_label.Delete(index);

	// For each layer, delete the image
	for(int l = 0; l < NLayers(); l++)
		LayerComposite(l).Delete(index);

	// Remove the appropriate composite from the synthetic layer array
	m_synthesized.erase(&m_synthesized[index]);

	// Final layer will have changed
	m_write_layers = 1;
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  Inserting, deleting, and moving layers
//
///////////////////////////////////////////////////////////////////////////////////////

void CVisLayeredComposite::InsertLayer(int index)
{
	// Get first available id
    int id = UniqueId();

	// Compute filename corresponding to ID
    char layer_name[1023], depth_name[1023];
    sprintf(layer_name, "L%03o.%s", id, filename_ext);
    sprintf(depth_name, "D%03o.%s", id, filename_ext);

	// Insert an empty image into m_final_layer for this layer
    CVisRGBAByteFrame frame(CVisShape(0, 0, 1, 1));
    frame.SetName(layer_name);
    m_final_layer.Insert(frame, index);

	// Insert an empty image into m_depth for this layer
    CVisRGBAByteFrame dframe(CVisShape(0, 0, 1, 1));
    dframe.SetName(depth_name);
    m_depth.Insert(dframe, index);

	// Create a layer and add it to the array of layers at the right location  
    if (index < 0)
        index = m_layer.size();
    m_layer.insert(&m_layer[index]);

	// Update the id array
    m_id.insert(&m_id[index], id);

    // Insert NImages() empty frames into the new layer (*m_layer[index]) 
    CVisLayer &layer = LayerComposite(index);
    for (int k = 0; k < NImages(); k++) {
		// Create the filename from the input image name and the layer name
		char *filename = new char[strlen(Input(k).Name()) +
								  strlen(FinalLayer(index).Name()) + 2];
		strcpy(filename, Input(k).Name());
		filename[strlen(Input(k).Name())] = '_';
		strcpy(filename + (strlen(Input(k).Name()) + 1), 
			   FinalLayer(index).Name());

		// Insert the framw into the new layer
//        CVisRGBAByteFrame f(filename);
//        layer.Insert(f,-1,0,VIS_LAYER_MEMBERSHIP_OUT);
        layer.PushBack(filename);
		layer.Back().SetEVisLayerMembership(VIS_LAYER_MEMBERSHIP_OUT);

		delete [] filename;
    }
	layer.SetBackgroundGrayLevel(m_background_gray_level);

    // Add an empty image to each of the NImage() synthetic images in the correct place
    for (k = 0; k < NImages(); k++) {
        CVisLayer &synth = SynthComposite(k);
//        synth.Insert(layer_name, index, VIS_LAYER_MEMBERSHIP_IN);
        synth.Insert(layer_name, index);
		synth.At(index).SetEVisLayerMembership(VIS_LAYER_MEMBERSHIP_OUT);
    }

	// Ensure this layer is written out
    m_write_layers = 1;
}

void CVisLayeredComposite::RaiseLayer(int index, bool to_top)
{
	assert(index >= 1 && index < NLayers());

	// Raise the appropriate image in the synthetic images and in the final layer
	for(int k = 0; k < NImages(); k++) {
		SynthComposite(k).Raise(index, to_top);
	}
	m_final_layer.Raise(index, to_top);
	m_depth.Raise(index, to_top);

	// Get copies of the layer and its id
	CVisLayerPtr current_layer = m_layer[index];
	int current_id = m_id[index];

	// Delete the entries in the layer and id arrays
	m_layer.erase(&m_layer[index]);
	m_id.erase(&m_id[index]);

	// Reinsert the layer and its id at the correct point
	if (to_top)	{
		m_layer.insert(&m_layer[0], current_layer);
		m_id.insert(&m_id[0], current_id);
	}
	else {
		m_layer.insert(&m_layer[index - 1], current_layer);
		m_id.insert(&m_id[index - 1], current_id);
	}
}

void CVisLayeredComposite::LowerLayer(int index, bool to_bottom)
{
	assert(index >= 0 && index < NLayers() - 1);

	// Lower the appropriate image in the synthetic images and in the final layer
	m_final_layer.Lower(index, to_bottom);
	m_depth.Lower(index, to_bottom);
	for(int k = 0; k < NImages(); k++) {
		SynthComposite(k).Lower(index, to_bottom);
	}

	// Get copies of the layer and its id
	CVisLayerPtr current_layer = m_layer[index];
	int current_id = m_id[index];

	// Delete the entries in the layer and id arrays
	m_layer.erase(&m_layer[index]);
	m_id.erase(&m_id[index]);

	// Reinsert the layer and its id at the correct point
	if (to_bottom || index == NLayers() - 2)	{
		m_layer.push_back(current_layer);
		m_id.push_back(current_id);
	}
	else {
		m_layer.insert(&m_layer[index + 1], current_layer);
		m_id.insert(&m_id[index + 1], current_id);
	}
}

void CVisLayeredComposite::DeleteLayer(int index)
{
	// Before deleting id, must update the label information
	for(int k = 0; k < NImages(); k++) {
		RemoveLayer(InputLabel(k), index);
	}
	m_write_labels = 1;

    // Remove the layer, its id, and the appropriate final layer and synthetic images
	m_final_layer.Delete(index);
	m_depth.Delete(index);
	m_id.erase(&m_id[index]);
	m_layer.erase(&m_layer[index]);
	for(k = 0; k < NImages(); k++) {
		SynthComposite(k).Delete(index);
	}
}

void CVisLayeredComposite::DeleteAll()
{
    m_input.DeleteAll();
    m_label.DeleteAll();
    m_final_layer.DeleteAll();
    m_depth.DeleteAll();
    m_layer.clear();
    m_synthesized.clear();
    m_synth_label.DeleteAll();
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  Updating final layer and synthesized images
//
///////////////////////////////////////////////////////////////////////////////////////

void CVisLayeredComposite::UpdateFinalLayer(int l)
{
    ProgressReport("Updating final layer %d", l);

    // Copy the bottom layer of layer l onto the l^th image of m_final_layer
    CVisLayer &comp = LayerComposite(l);
    if (comp.NFrames() == 0)
        LayerImage(l) = CVisRGBAByteImage();
    else {
        LayerImage(l) = comp.BottomAlpha(m_comp_op);
    }
}

void CVisLayeredComposite::UpdateFinalCollection()
{
    for (int l = 0; l < NLayers(); l++)
        UpdateFinalLayer(l);
}

//
//  Depth image extraction
//

static int VisDHat(int v)
{
    return (v < 0) ? 0 : (v < 256) ? v :
        (v <  769) ? 255 : (v < 1024) ? 1024 - v : 0;
}

static CVisRGBABytePixel VisPseudoColor1024(int v)
{
    return CVisRGBABytePixel(VisDHat(v - 512), VisDHat(v),
                              VisDHat(v + 512), 255);
}

#ifdef _DEBUG
static void VisDHatPrint(char *filename, int step = 1)
{
    static int nrows = 16;
    CVisRGBAByteImage bar(0, 0, nrows, 1024/step);
    for (int y = 0; y < nrows; y++)
        for (int x = 0; x < 1024/step; x++)
            bar.Pixel(x, y) = VisPseudoColor1024(x * step);
    bar.WriteFile(filename);
}
#endif

static void VisLayerComputeZ(CVisRGBAByteImage& img, CVisTransform4x4 M,
                             TVisLayerDepthOp op, CVisRGBAByteImage& mask,
                             CVisFloatImage& parallax, float parallax_scale)
{
    static float max_range = 1.0;   // assumes re-scaled (X,Y,Z) values
    static float offset = 0.5;
    float scale = 0.5 / max_range;
    int iscale = (op == layer_depth_pseudo_color) ? 1024 :
        (op == layer_depth_gray) ? 256 : (1 << 24);

    CVisVector4 p(0, 0, 0, 1), q;
    for (int y = img.Top(); y < img.Bottom(); y++) {
        p[1] = y;
        for (int x = img.Left(); x < img.Right(); x++) {

            // Check if mask is 0 (invisible)
            CVisRGBABytePixel& cp = img.Pixel(x, y);
            float& fp = *(float *) &cp;
            int& ip = *(int *) &cp;
            if (0 == *(int *) &mask.Pixel(x, y)) {
                ip = 0;
                continue;
            }

            // Add parallax value to w, if it exists
            if (parallax_scale != 0.0)
                p[2] = parallax.Pixel(x, y, 0) * parallax_scale;    // use horizontal flow

            // Transform the point, scale and clip it
            p[0] = x;
            q = M.ProjectVector(p);
            float sz = scale * q[2] + offset;
            sz = (sz < 0.0) ? 0.0 : (sz > 1.0) ? 1.0 : sz;
            int v = int(sz * (iscale-2)) + 1;   // don't use 0 (for invisible)

            // Set the appropriate pixel value
            switch (op) {
            case layer_depth_pseudo_color:
                cp = VisPseudoColor1024(v);
                break;
            case layer_depth_gray:
                cp = CVisRGBABytePixel(v, v, v, 255);
                break;
            case layer_depth_24bit:
                ip = v;
                break;
            case layer_depth_float:
                fp = q[2];     // don't bother normalizing
                break;
            }

        }
    }
}

void CVisLayeredComposite::UpdateDepthImage(int l)
{
    ProgressReport("Computing depth image %d", l);

    // Compute the inverse projection (screen -> world)
    CVisModel2D& m2d = *m_model.m2d[0];
    CVisTransform4x4 M = m2d.geom.CompleteTransform();
    CVisVector4& n = m_model.pl[l].n;
    for (int j = 0; j < 4; j++)
        M[2][j] = n[j];
    CVisTransform4x4 P = M.Inverted();

    // Compute the de-ghosting based parallax, if it exists
    CVisFloatImage parallax;
    float parallax_scale = 0.0;
    CVisLayer& comp = LayerComposite(l);
    static int compute_parallax = 1;
    if (comp.LocalWarpMode() && compute_parallax) {
        
        // Convert the warp fields into fixpoint RGB images
        CVisRGBAComposite flow_comp;
        CVisLocalWarp::m_use_B_channel = 0;     // don't use B channel
        comp.WarpToOffset(flow_comp);

        // Copy the geometry
        for (int k = 0; k < comp.NFrames(); k++)
            flow_comp[k].TransformChain() = comp[k].TransformChain();

        // The correct thing to do is to perform a WEIGHTED blend
        //  (with the accumulator being proportional to k) -- see 18-Nov-97 notes
        // Instead, for now, we just do an over (last frame over others).
        flow_comp.SetCompositingOperator(composite_over);
        flow_comp.SetBoundingBoxSize(0.0);
        CVisRGBAByteImage cflow = flow_comp.BottomAlpha();
#ifdef _DEBUG
        char filename[1024];
        sprintf(filename, "L%03o_flow.bmp", LayerId(l));
        cflow.WriteFile(filename);
#endif
        // Trim by layer shape (why aren't the shapes the same?)
        CVisShape l_shape = LayerImage(l).Shape();
        cflow = cflow.SubImage(l_shape);

        // Covert back into a floating float field
        CVisRGBAComposite flow_result, color_result;
        flow_result.PushBack(FinalLayer(l));
        flow_result.SetWarpMode(VIS_WARP_ZERO);
        flow_result.SetLocalWarpMode(1);
        flow_result.m_warp_grid_scale = comp.m_warp_grid_scale;
        CVisRGBAByteFrame crf(cflow);
        color_result.Insert(crf);
        flow_result.OffsetToWarp(color_result);
        parallax = flow_result.LocalWarp(0);

#if 0   // no longer used (see Rick's notes, Nov 20, 1997)
        // Set the scale according to the t vector
        //  (see Rick's notes, Nov 18, 1997)
        CVisModel2D& m2d0 = *m_model.m2d[0];
        CVisModel2D& m2d1 = *m_model.m2d[NImages()-1];
        CVisTransform4x4 P0 = m2d0.geom.CompleteTransform();
        CVisTransform4x4 T1 = m2d1.geom[1];
        CVisTransform4x4 T1i = T1.Inverted();
        CVisVector4 ql(T1i[0][3], T1i[1][3], T1i[2][3], T1i[3][3]);
        CVisVector4 epipole = P0 * ql;
        parallax_scale = 1.0 / epipole[3];     // z component of translation
#endif

        // Perturb the projection with a small z change, 
        //  compute expected parallax (see Rick's notes, Nov 20, 1997)
        static float zd = 1.0 / 1024;
        M[3][3] += zd;
        CVisTransform4x4 P2 = M.Inverted();
        CVisModel2D& m2d1 = *m_model.m2d[NImages()-1];
        CVisTransform4x4 P3 = m2d1.geom.CompleteTransform();
        CRect bbox = m2d.bbox;
        CVisVector4 c(bbox.left+0.5*bbox.Width(), bbox.top+0.5*bbox.Height(), 1, 0);
        CVisVector4 c0 = (P3 * P).ProjectVector(c);
        CVisVector4 c1 = (P3 * P2).ProjectVector(c);
        float sample_parallax = c1[0] - c0[0];
        parallax_scale = zd / sample_parallax;
    }

    // Transform all points in layer, and keep the z value
    CVisRGBAByteImage& layer = LayerImage(l);
    CVisRGBAByteImage& depth = DepthImage(l);
    depth.Allocate(layer.Shape());
    VisLayerComputeZ(depth, P, DepthOperator(), layer,
                     parallax, parallax_scale);
}

void CVisLayeredComposite::UpdateDepthCollection()
{
    // Check if 3D model has been computed...
    if (m_model.m2d.size() < NImages() || 
        m_model.pl.size()  < NLayers()) {
        Warning("UpdateDepthCollection: 3D model has not been initialized;\
 try calling Estimate Motion and Estimate Planes.");
        return;
    }

    UpdateFinalCollection();    // to get appropriate sizes & masks
    for (int l = 0; l < NLayers(); l++)
        UpdateDepthImage(l);
#ifdef _DEBUG
    VisDHatPrint("LS_pseudo_color.bmp", 4);
#endif
}

void CVisLayeredComposite::SynthUpdate(int k, int show_depth)
{
    ProgressReport("Updating synthetic image %d", k);

	// Get the k^th synthetic image
    CVisLayer &synth = SynthComposite(k);
    assert(synth.NFrames() == NLayers());

    // Update the images to point to final layers
    for (int l = 0; l < NLayers(); l++) {
		// Point the l^th image of synth(k) at l^th image of m_final_layer
        if (show_depth)
            synth[l] = DepthImage(l);
        else
            synth[l] = LayerImage(l);

        // Set the geometry of the l^th image of the k^th synthetic image to be the 
		// inverse of the geometry of the k^th image of the l^th layer
        CVisLayer &layer = LayerComposite(l);
        CVisTransform4x4 t = layer.LayerTransform(k);
        CVisTransform4x4 ti = t.Inverted();
        synth.SetLayerTransform(l, ti);
        if (show_depth)
            continue;
			
		// Also	copy the unwarped layer difference into the synthetic
		CVisLayer &comp = LayerComposite(l);
		int layer_old_current_frame = comp.CurrentFrame();
		comp.SetCurrentFrame(k);
		comp.UnWarpedDiff(m_comp_op, m_diff_op);
		synth.UnwarpedDiffImage(l) = comp.UnwarpedDiffImage(k);
		comp.SetCurrentFrame(layer_old_current_frame);
    }

	// Everything in the k^th synthetic image has probably changed
	synth.Invalidate();
}

void CVisLayeredComposite::SynthUpdateAll(int show_depth)
{
	assert(m_synthesized.size() == NImages());
    
    // Update the composites.
    for (int k = 0; k < NImages(); k++)
        SynthUpdate(k, show_depth);
}


///////////////////////////////////////////////////////////////////////////////////////
//
//	Predict the pixel assignment from synthetic images and update m_synth_label
//
///////////////////////////////////////////////////////////////////////////////////////

void CVisLayeredComposite::PredictPixelAssignment()
{
	// Predict pixel assignment for each input image
	for(int k = 0; k < NImages(); k++) {
        ProgressReport("Predicting pixel assignment for image %d", k);

		// Get the k^th synthetic image composite and k^th synthetic label image
		CVisLayer &synth = SynthComposite(k);
		CVisRGBAByteImage &label_image = SynthLabel(k);

		// Remember the current frame of the k^th synthetic image composite
		int old_current_frame = synth.CurrentFrame();

		// For each of the l layer, make sure difference has been computed
        //  (unless just selecting topmost pixel)
		for(int l = 0; l < NLayers() && m_pred_op != layer_topmost; l++) {
			synth.SetCurrentFrame(l);
			synth.SynthDifference(m_synth_comp_op, m_synth_diff_op, Image(k));
		}

		// Reinstate the old current frame
		synth.SetCurrentFrame(old_current_frame);

		// Actually, do the prediction
		switch (m_pred_op) {
		case layer_thresholded_wta:
			ThresholdedWTA(label_image, synth);
			break;
		case layer_topmost:
			TopmostPixel(label_image, synth);
			break;
		default:
			assert(0);
			break;
		}
	}
}

void CVisLayeredComposite::UpdatePixelAssignment()
{
	// Copy everything from m_synth_label to m_label
	for(int k = 0; k < NImages(); k++) {
		assert(SynthLabel(k).Shape() == InputLabel(k).Shape());
		SynthLabel(k).CopyPixelsTo(InputLabel(k));
	}

	// Ensure new labels written out
	m_write_labels = 1;
	m_write_layers = 1;
}


///////////////////////////////////////////////////////////////////////////////////////
//  
//  Pixel assignment prediction operators
//
///////////////////////////////////////////////////////////////////////////////////////

void CVisLayeredComposite::ThresholdedWTA(CVisRGBAByteImage &label, CVisLayer &synth)
{
	// For each pixel in the label image
	for(int i = label.Top(); i < label.Bottom(); i++ ) {
		for(int j = label.Left(); j < label.Right(); j++ ) {
			// Initialize the current best to invalid values
			int best_diff  = 256;
			int best_layer = -1;

			for(int l = 0; l < NLayers(); l++) {
				// Get the l^th difference image
				CVisRGBAByteImage &diff_image = m_use_layer_difference ? 
					                             synth.UnwarpedDiffImage(l) :
				                                 synth.DifferenceImage(l);
						  
				if (diff_image.ContainsPoint(j, i)) {
					// Get the (i,j)^th pixel value, if there is one
					CVisRGBABytePixel& diff = diff_image.Pixel(j, i);

					// Check to see whether it has a lower difference than all previous
					// layers (and is below threshold.) If so, update current best.
					if (diff.A() && diff.R() < best_diff && 
						diff.R() < m_difference_threshold) {
						best_diff = diff.R();
						best_layer = l;
					}
				}
			}

			// Update the (i,j)^th pixel of the synthetic label image			
			*((CVisRGBABytePixel *) label.PixelAddress(i,j)) = ColorFromLayer(best_layer);
		}
	}
}

struct CVisOverlayColor {
    CVisOverlayColor(CVisRGBABytePixel ic) : c(ic) {};
    void operator()(CVisRGBABytePixel& comp, CVisRGBABytePixel& layr) {
        if (layr.A()) comp = c;
    }
    CVisRGBABytePixel c;   // layer id color
};

void CVisLayeredComposite::TopmostPixel(CVisRGBAByteImage &label, 
                                        CVisLayer &synth)
{
    // Clear to unassigned
    label.FillPixels(ColorFromLayer(-1));

    // Process from back to front
    for (int l = 0; l < NLayers(); l++) {

        // Get the current "sprite" (layer), and trim label region
        synth.SetCurrentFrame(l);
        CVisRGBAByteImage &img = synth.TopAlpha();
        CVisShape s = (label.Shape() & img.Shape());
        if (s.Height() <= 0 || s.Width() <= 0)
            continue;
        CVisRGBAByteImage ltrim = label.SubImage(s);
        CVisRGBAByteImage strim = img.SubImage(s);

        // Overlay with layer color where non-empty
        CVisOverlayColor ovrly(ColorFromLayer(l));
        VisMap2(ovrly, ltrim, strim);
    }
}

///////////////////////////////////////////////////////////////////////////////////////
//  
//  Pseudocolor computation
//    Encode each layer id into an RGB color - really just for visual debugging
//    The encoding un-interleaves the bits into R,G,B, and then bit-reverses them
//
///////////////////////////////////////////////////////////////////////////////////////

int CVisLayeredComposite::LayerId(int l) 
{ 
	assert(l >= -1 && l < NLayers());

	if (l < 0 || l >= NLayers())
		return 0;
	else
		return m_id[l]; 
}

int CVisLayeredComposite::IdFromColor(CVisRGBABytePixel rgba)
{
    int id = 0;
    for (int j = 0; j < 8; j++)
        id = (id << 3) | 
            (((rgba.R() >> j) & 1) << 0) |
            (((rgba.G() >> j) & 1) << 1) |
            (((rgba.B() >> j) & 1) << 2);
    return id;
}

CVisRGBABytePixel CVisLayeredComposite::ColorFromLayer(int l)
{
	int id = LayerId(l);
    assert(0 <= id && id < (1 << 24));

    CVisRGBABytePixel rgba(0, 0, 0, 255);
    for (int j = 0; id; j++, id >>= 3) {
        rgba.SetR(rgba.R() | (((id >> 0) & 1) << (7 - j)));
        rgba.SetG(rgba.G() | (((id >> 1) & 1) << (7 - j)));
        rgba.SetB(rgba.B() | (((id >> 2) & 1) << (7 - j)));
    }

    return rgba;
}

int CVisLayeredComposite::LayerColor(int l)
{
    CVisRGBABytePixel c = ColorFromLayer(l); 
	return *(int *) &c;
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  Masked image (segment) extraction
//
///////////////////////////////////////////////////////////////////////////////////////

void CVisLayeredComposite::ExtractAllSegments()
{
	// For each input image
    for (int k = 0; k < NImages(); k++)
        ExtractSegments(k);
}

void CVisLayeredComposite::ExtractSegments(int k)
{
	// For each layer in the image k
    for (int l = 0; l < NLayers(); l++)
        ExtractSegment(k, l, 1);
}

void CVisLayeredComposite::ExtractSegment(int k, int l, int crop)
{
    CVisShape input_shape;
    if (crop) {
		// Find shape of crop region
        input_shape = ShapeOfSegment(k, l);
	}
	else {
		// Use the entire image
		input_shape = Image(k).Shape();
	}
    
	// Get a reference to the destination of the extraction
	CVisRGBAByteImage &img = LayerComposite(l)[k];

	if (input_shape.Height() > 0 && input_shape.Width() > 0)	{
		// Update the shape of the destination, if necessary
		if (img.Shape() != input_shape)	{
			img.Allocate(input_shape); 
		}

		// Extract masked image	into destination image
	    ExtractSegment(k, l, img, 0);

		if (m_input.UsingBias()) {
			BiasNormalize(img, LayerComposite(l).Bias(k), LayerComposite(l).Gain(k));
		}
		ProgressReport("Extracting layer %d from image %d", l, k);
	}
	else {
		// Empty crop region, so empty the destination
		img.Allocate(1,1);
		ProgressReport("Layer %d in image %d is empty", l, k);
	}

	// Top, bottom, composite, difference, of the l^th layer are all now invalid
	// since we changed image k of it
	LayerComposite(l).Invalidate();
	LayerComposite(l).InvalidateWarp(1);
}

void CVisLayeredComposite::ExtractEntireImages()
{
	ProgressReport("Extracting full images from input");

    for (int k = 0; k < NImages(); k++)	{
		for (int l = 0; l < NLayers(); l++) {
			// Get shape of input image
			CVisShape input_shape = Image(k).Shape();

			// Get destination for extraction
			CVisRGBAByteImage &img = LayerComposite(l)[k];

			// Update the shape of the destination, if necessary
			if (img.Shape() != input_shape)	{
				img.Allocate(input_shape);
			}

			// Copy pixels into destination
			if (input_shape.Height() > 0 && input_shape.Width() > 0) {
				Image(k).CopyPixelsTo(img);
			}

			if (m_input.UsingBias()) {
				BiasNormalize(img, LayerComposite(l).Bias(k), LayerComposite(l).Gain(k));
			}

 			// Top, bottom, composite, difference, of the l^th layer are all 
			// now invalid since we changed image k of it
			if (k == 0) { 
				// Only call once for each layer
				LayerComposite(l).InvalidateWarp(1);
				LayerComposite(l).Invalidate();
			}
		}
	}
}

void CVisLayeredComposite::ExtractPartiallyCroppedImages()
{
	ProgressReport("Extracting partially cropped images from input");

    for (int k = 0; k < NImages(); k++)	{
	    for (int l = 0; l < NLayers(); l++)	{
			// Get shape of the crop region
			CVisShape input_shape = ShapeOfSegment(k, l);

			// Enlarge crop region by a constant fraction
			CVisShape crop_shape;
			crop_shape.left = input_shape.Left() - (input_shape.Width()*m_fract_increase)/200;
			crop_shape.top = input_shape.Top() - (input_shape.Height()*m_fract_increase)/200;
			crop_shape.right = crop_shape.left + input_shape.Width() + (input_shape.Width()*m_fract_increase)/100;
			crop_shape.bottom = crop_shape.top + input_shape.Height() + (input_shape.Height()*m_fract_increase)/100;

			// Intersect the crop region with the input image shape
			CVisRGBAByteImage &dest_img  = LayerComposite(l)[k];
			CVisRGBAByteImage &input_img = InputCollection()[k];
			crop_shape &= input_img.Shape();
 			
			if (crop_shape.Height() > 0 && crop_shape.Width() > 0)	{
				// Update the shape of the destination, if necessary
				if (dest_img.Shape() != crop_shape)	{
					dest_img.Allocate(crop_shape); 
				}

				// Crop the input and copy the pixels into the layer
				CVisRGBAByteImage cropped_input = input_img.SubImage(crop_shape);
				cropped_input.CopyPixelsTo(dest_img);

				if (m_input.UsingBias()) {
					BiasNormalize(dest_img, LayerComposite(l).Bias(k), LayerComposite(l).Gain(k));
				}
				ProgressReport("Extracting layer %d from image %d", l, k);
			}
			else {
				// Empty crop region, so empty the destination
				dest_img.Allocate(1,1);
				ProgressReport("Layer %d in image %d is empty", l, k);
			}

			// Top, bottom, composite, difference, of the l^th layer are all now invalid
			// since we changed image k of it
			if (k == 0) { 
				LayerComposite(l).Invalidate();
				LayerComposite(l).InvalidateWarp(1);
			}
	   	}
	}
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  Masked image extraction helper functions
//
///////////////////////////////////////////////////////////////////////////////////////

// Function object for zeroing out non-labeled pixels
struct ExtractMaskFn {      
    int id;
    ExtractMaskFn(int i) : id(i) {}
    void operator()(int label, int orig, int& masked)
    {
        masked = (label == id) ? orig : 0;
    }
};

// Function object for brightening labeled pixels and darkening others (3:1)
struct HighlightFn {        
    int id;
    HighlightFn(int i) : id(i) {}
    void operator()(int label, int orig, int& masked)
    {
        // Note: only works on little-endian !!!
#if 0   // inactive -> 1/4 brightness seems too dark
        int quarter = (orig & 0xfcfcfc) >> 2;
        masked = (label == id) ? orig - quarter + 0x3f3f3f : quarter;
#else
        int half = (orig & 0xfefefe) >> 1;
        masked = half + ((label == id) ? 0x808080 : 0);
#endif
    }
};

struct RemoveLayerFn {
	int id;
	RemoveLayerFn(int i) : id(i) {}
	void operator()(int& label)
	{ 
		if (label == id) 
			label = 0;
	}
};

void CVisLayeredComposite::ExtractSegment(int k, int l, CVisRGBAByteImage &img,
                                          int highlight)
{
    CVisIntImage ilabel;
	ilabel.Alias(InputLabel(k));
    CVisIntImage iorig;
	iorig.Alias(Image(k));
    CVisIntImage imaskd;
	imaskd.Alias(img);

    // Trim to output (img) shape
    if (iorig.Shape() != imaskd.Shape()) {
        ilabel = ilabel.SubImage(imaskd.Shape());
        iorig  =  iorig.SubImage(imaskd.Shape());
    }

    // Perform the masking
    if (highlight) {
        HighlightFn fn(LayerColor(l));
        VisMap3(fn, ilabel, iorig, imaskd);
    } else {
        ExtractMaskFn fn(LayerColor(l));
        VisMap3(fn, ilabel, iorig, imaskd);
    }
}

void CVisLayeredComposite::ExtractSingleLayer(CVisRGBAByteImage &img, int l,
											  CVisRGBAByteImage &img2)
{
    CVisIntImage iorig;
	iorig.Alias(img);
    CVisIntImage imaskd ;
	imaskd.Alias(img2);

    // Trim to output (img2) shape
    if (iorig.Shape() != imaskd.Shape()) {
        iorig  =  iorig.SubImage(imaskd.Shape());
    }

    // Perform the masking
    ExtractMaskFn fn(LayerColor(l));
    VisMap3(fn, iorig, iorig, imaskd);
}

void CVisLayeredComposite::RemoveLayer(CVisRGBAByteImage &img, int l)
{
	CVisIntImage iorig ;
	iorig.Alias(img);

	RemoveLayerFn fn(LayerColor(l));
	VisMap1(fn, iorig);
}

///////////////////////////////////////////////////////////////////////////////////////
//
// Extract correct band for displaying diff	based on the differencing operator used
//
///////////////////////////////////////////////////////////////////////////////////////

struct ExtractBandFn {
	int band;
	ExtractBandFn(int bnd = 0) : band(bnd) {}
	void operator()(CVisRGBABytePixel &pixel)
	{
		switch (band) {
		case 1:
			pixel.SetG(pixel.R());
			pixel.SetB(pixel.R());
			break;
		case 2:
			pixel.SetR(pixel.G());
			pixel.SetB(pixel.G());
			break;
		case 3:
			pixel.SetG(pixel.B());
			pixel.SetR(pixel.B());
			break;
		case 4:
			pixel.SetR((pixel.G() + pixel.B()) / 2);
			pixel.SetG(pixel.R());
			pixel.SetB(pixel.R());
		case 0:
		default:
			break;
		}
	}
};

void CVisLayeredComposite::ExtractDiffBand(CVisRGBAByteImage &img, TVisLayerDiffOp op)
{
	switch (op) {
	case layer_diff_intensity:
		{
			ExtractBandFn fn(1);
			VisMap1(fn, img);
		}
		break;
	case layer_diff_normal_flow:
		assert(0);
		break;
	case layer_diff_residual_flow:
		{
			ExtractBandFn fn(4);
			VisMap1(fn, img);
		}
		break;
	case layer_diff_int_res_flow:
		{
			ExtractBandFn fn(0);
			VisMap1(fn, img);
		}
		break;
	default:
		assert(0);
		break;
	}
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  Bias and Gain manipulation functions
//
///////////////////////////////////////////////////////////////////////////////////////

void CVisLayeredComposite::BiasNormalize(CVisRGBAByteImage &img, double bias, 
										 double gain)
{
	assert(gain > 0);
	double gain_rcp = 1 / gain;
	for(int i = 0; i < img.Height(); i++) {
		CVisRGBABytePixel *pixel = img.PtrToFirstPixelInRow(i+img.Top());
		for(int j = 0; j < img.Width(); j++) {
			if (pixel->A()) {
				double temp = (pixel->R() - bias) * gain_rcp;
				pixel->SetR((temp > 255.0) ? 255 : (temp < 1.0) ? 0 : int(temp));
				temp = (pixel->G() - bias) * gain_rcp;
				pixel->SetG((temp > 255.0) ? 255 : (temp < 1.0) ? 0 : int(temp));
				temp = (pixel->B() - bias) * gain_rcp;
				pixel->SetB((temp > 255.0) ? 255 : (temp < 1.0) ? 0 : int(temp));
			}
			pixel++;
		}
	}
}

void CVisLayeredComposite::EstimateBias()
{
	// Estimate the bias in each layer
	for(int l = 0; l < NLayers(); l++)
		LayerComposite(l).EstimateBias(m_comp_op);

	// Totals for normalizing accross the images - see later
	double sum_gains = 0.0, sum_biases = 0.0, prod_gains = 1.0;

	for(int k = 0; k < NImages(); k++) {

		// Average the estimates, weighting according to the number of pixels used to 
		// estimate the bias for that layer
		double total_bias = 0.0, total_gain = 0.0, average_bias, average_gain;
		int total_num_pixels = 0;
		for(l = 0; l < NLayers(); l++) {
			total_bias += LayerComposite(l).NumberOfPixels(k) * LayerComposite(l).Bias(k);
			total_gain += LayerComposite(l).NumberOfPixels(k) * log(LayerComposite(l).Gain(k));
			total_num_pixels += LayerComposite(l).NumberOfPixels(k);
		}
		if (total_num_pixels) {
			average_bias = total_bias / double(total_num_pixels);
			average_gain = exp(total_gain / double(total_num_pixels));
		}
		else {
			average_bias = m_input.Bias(k);
			average_gain = m_input.Gain(k);
		}
	
		// Copy the estimates into the input composite
		m_input.SetBias(k, average_bias);
		m_input.SetGain(k, average_gain);

		sum_gains  += average_gain;
		prod_gains *= average_gain;
		sum_biases += average_bias;
	}

	// Normalize so that sum of biases = 0.0, and product of gains = 1.0
	for(k = 0; k < NImages(); k++) {
		double normalizing_bias = - (sum_biases / sum_gains);
		double normalizing_gain = pow(prod_gains, - 1.0 / double(NImages()));

		m_input.SetBias(k, m_input.Bias(k) + normalizing_bias * m_input.Gain(k));
		m_input.SetGain(k, m_input.Gain(k) * normalizing_gain);
							 
		if (m_average_bias) {
			// Copy the estimates into all the layer composites
			for(l = 0; l < NLayers(); l++) {
				LayerComposite(l).SetBias(k, m_input.Bias(k));
				LayerComposite(l).SetGain(k, m_input.Gain(k));
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  Deghosting functions
//
///////////////////////////////////////////////////////////////////////////////////////

void CVisLayeredComposite::DeGhostAll()
{
	for(int l = 0; l < NLayers(); l++) {
		DeGhost(l);
	}
}

void CVisLayeredComposite::DeGhost(int l)
{
	assert(l >= 0 && l < NLayers());

	// Set up the adjuster and set the local warp mode
	CVisLayer &comp = LayerComposite(l);
	m_adjuster.Initialize(&comp);
	comp.SetLocalWarpMode(1);

	// Perform the deghosting
	if (m_adjuster.m_matchpairs) {
		m_adjuster.ComputeMatchAll();
	}							   
	else {
		m_adjuster.m_matchpairs = NImages(); 
		m_adjuster.ComputeMatchAll();
	}
	m_adjuster.DeGhostAll();

	// The top, bottom, composite and differences will now be out of date
	m_write_layers = 1;
	comp.Invalidate();
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  Structure from Motion functions
//
///////////////////////////////////////////////////////////////////////////////////////

void CVisLayeredComposite::BuildModel3D(int clear_old)
{
    // Clear out old information
    if (clear_old) {
        // Destroy the 2D models (since they are pointers)
        for (int f = 0; f < m_model.m2d.size(); f++)
            delete m_model.m2d[f];
        m_model.m2d.clear();

        // Destroy the point and plane lists
        m_model.vl.clear();         // vector of vertices in this model
        m_model.pl.clear();         // vector of planes   in this model
        m_model.sv.clear();         // set of selected vertices
    }

    // Create new 2D models (views)
    for (int k = m_model.m2d.size(); k < NImages(); k++) {

        // Create 2D model and link
        m_model.m2d.push_back(new CVisModel2D);
        CVisModel2D& m2d = *m_model.m2d[k];
        m2d.Link3D(&m_model);

        // Fill in the image and shape info
        m2d.m_img = Image(k);
        m2d.m_composite_filename = std::string(Input(k).Name());
        m2d.bbox = m2d.m_img.Shape();

        // Fill in the geometry (adapted from VisGenerateTestViews)
        int width = m2d.bbox.Width(), height = m2d.bbox.Height();
        float distance = m_f_guess;
        float s = 0.5 * width * distance;   // unit [-1,+1] box fills width
        CVisTransform4x4 V(s, s, 0, 0);
        V[2][3] = V[3][2] = 1.0;
        V.Translate(0.5*width, 0.5*height);
        CVisTransform4x4 M;
        m2d.geom = CVisTransformChain(V);
        m2d.geom.push_back(CVisTransformChain(M));
        m2d.M = m2d.geom.CompleteTransform();
    }

    // Create new planes (layers)
    while (m_model.pl.size() < NLayers())
        m_model.AddPlane();

    // Copy the homographies into the 2D planes
	for (int l = 0; l < NLayers(); l++) {
    	CVisLayer& layer = LayerComposite(l);

        for (int k = 0; k < NImages(); k++) {
            CVisModel2D& m2d = *m_model.m2d[k];
            while (m2d.pl.size() <= l)
                m2d.pl.push_back(CVisPlane2D());
            CVisPlane2D& pl = m2d.pl[l];
            CVisRGBAByteFrame& element = layer.Frame(k);
            CVisTransformChain& geom = element.TransformChain();
			CVisShape sh = element.Shape();

            // Check for valid homography
		    if (layer.CompositeMembership(k) != VIS_LAYER_MEMBERSHIP_IN ||
                sh.Height() <= 1 || sh.Width() <= 1) {
                pl.id = -1;     // inactive
                pl.H = CVisTransform4x4(0, 0, 0, 0);
                pl.c = 0;
                pl.bbox = CRect(0, 0, 0, 0);
            } else {
                pl.id = l;
                pl.H = geom.CompleteTransform().Inverted().TransposedZW();
                pl.c = 1;
                pl.bbox = sh;
            }
        }
	}
}

void CVisLayeredComposite::EstimateMotion()
{
    static int n_extra      = 7;    // 7 points per homography
    static int calibrated   = 1;    // calibrated reconstruction
    static int rescale_data = 1;    // rescale to unit size
    static int adjust_f     = 0;    // don't adjust focal length
    static int no_rotation  = 0;    // only for Flower Garden sequence
    
    BuildModel3D(1);
    if (n_extra > 0)    // for debugging only
    VisHallucinateCorrespondences(m_model, n_extra, 1);
    if (n_extra > 0)    // for debugging only
    VisReconstructModel(m_model, CSfM_bundle, calibrated, 
                        rescale_data, adjust_f, no_rotation);
}

void CVisLayeredComposite::EstimatePlanes()
{
    static int n_extra  = 7;    // 8 points per homography
    static int n_iter   = 4;    // 4 iterations of re-weighted LS

    // Build the 3D model if necessary, or just update homographies
    if (m_model.m2d.size() != NImages()) {
        CVisDiagnostics::Warning("\
3D model and Layered Stereo descriptor have different number of images.\
  Recomputing the 3D model (including camera poses)");
        EstimateMotion();
    } else {
        BuildModel3D(0);
        VisHallucinateCorrespondences(m_model, n_extra, 1);
    }

    // Reconstruct the 3D points and plane equations
    for (int i = 0; i < n_iter; i++)
        VisAdjustPoints(m_model, i > 0, 0, 1);
    VisFitPlanes(m_model, 0);
    for (i = 0; i < n_iter; i++)
        VisAdjustPoints(m_model, 1, 1, 1);

    // Reproject points into 2D and recompute homographies
    VisProjectViews(m_model, 0.0, 0);
    //  VisComputeHomographies(m_model); // called in VisProjectViews()

    // Alternative homography recomputation
    if (n_extra == 7) {
        CVisModel2D& m2d0 = *m_model.m2d[0];
        CVisTransform4x4 M0 = m2d0.geom.CompleteTransform();
        int n_planes = m_model.pl.size();
        for (int p = 0; p < n_planes; p++) {
            CVisVector4& n = m_model.pl[p].n;
            for (int j = 0; j < 4; j++)
                M0[2][j] = n[j];
            CVisTransform4x4 P0 = M0.Inverted();
            for (int f = 0; f < NImages(); f++) {
                CVisModel2D& m2d = *m_model.m2d[f];
                CVisTransform4x4 M = m2d.geom.CompleteTransform();
                CVisTransform4x4 H1 = M * P0;
                CVisTransform4x4 H2 = H1.TransposedZW();
                for (int j = 0; j < 4; j++)
                    H2[j][3] = H2[3][j] = (j == 3);
                CVisPlane2D& pl = m2d.pl[p];  // valid 2D plane
                CVisTransform4x4& H = pl.H;   // Homography to be updated
                H = H2;
                H1[0][0];     // this statement for debugging only
            }
        }
    }

    // Replace the current layer transforms with 2D plane homographies
	for (int l = 0; l < NLayers(); l++) {
    	CVisLayer& layer = LayerComposite(l);

        for (int k = 0; k < NImages(); k++) {
            CVisPlane2D& pl = m_model.m2d[k]->pl[l];
            CVisRGBAByteFrame& element = layer.Frame(k);
            CVisTransformChain& geom = element.TransformChain();
			CVisShape sh = element.Shape();
            if (pl.id != l)
                continue;   // not valid
            if (pl.H[3][3] == 0.0)
                continue;   // why is this homography not valid???
            geom[0] = pl.H.TransposedZW().Inverted();
        }
        layer.Invalidate();
	}
	m_write_layers = 1;
}

void CVisLayeredComposite::EstimateParallax()
{
    // Build the SpriteComposite to hold the results
    BuildSpriteComposite();

    // Estimate out-of-plane depth for each sprite/layer
	for (int l = 0; l < NLayers(); l++) {
    	CVisLayer& layer = LayerComposite(l);
        CVisSprite& sprite = SpriteComposite().Sprite(l);
        CVisStereo stereo;      // stereo matcher for this level

        // Copy the images into the stereo data set
        for (int k = 0; k < NImages(); k++)
            stereo.Frame(k) = layer.Frame(k);
        (*(CVisSprite *) &stereo) = sprite; // copy output image & camera

        // Perform the plane sweep algorithm
        stereo.PlaneSweep();

        // Copy the depth map back to the sprite, along with the improved composite
        sprite.ZImage() = stereo.ZImage();
            // where is the depth map name allocated? (scan for D%0)
        // m_depth.Frame(l) = stereo.ZImage();
    }
}

void CVisLayeredComposite::InsertCameras(CVisModel3D& m3d)
{
    // Build the 3D model if necessary, or just update homographies
    if (NImages() > 0 && m3d.m2d.size() != NImages())
        CVisDiagnostics::Warning("\
Inserted 3D model and Layered Stereo descriptor have different number of images.\
  Will add new images to end.");

    // Check that first image name is consistent
    if (m_model.m2d.size() > 0) {
        CVisString m3d_name = m3d.m2d[0]->m_composite_filename;
        CVisString mmm_name = m_model.m2d[0]->m_composite_filename;
        if (NImages() > 0 && m3d_name != mmm_name) {
            CVisString warn = m3d_name + " does not match " + mmm_name;
            CVisDiagnostics::Warning(warn);
        }
    }

    // Add new images to the end
    while (NImages() < m3d.m2d.size()) {
        CVisString filename = m3d.m2d[NImages()]->m_composite_filename;
        InsertImage(filename);
    }

    // Pad up the internal 3D model
    BuildModel3D(0);

    // Copy the camera information
    for (int k = 0; k < NImages(); k++) {
        CVisModel2D &src = *m3d.m2d[k];
        CVisModel2D &dst = *m_model.m2d[k];

        dst.M = src.M;
        dst.geom = src.geom;
        dst.F = src.F;
        dst.e = src.e;
    }
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  Conversion to simpler Sprite Composite class
//
///////////////////////////////////////////////////////////////////////////////////////

void CVisLayeredComposite::BuildSpriteComposite()
{
    // Create the list of sprites (layers)
    CVisModel2D& m2d0 = *m_model.m2d[0];
    m_sprites.DeleteAll();
	for(int l = 0; l < NLayers(); l++) {
        CVisSprite s;
        s.Image()       = LayerImage(l);
        s.Filename()    = FinalLayer(l).Name();
        s.Camera()      = m2d0.geom;
        s.PlaneEqn()    = m_model.PlaneEqn(l);
        m_sprites.Insert(s);
    }

    // Create the list of views (input images)
    m_sprites.ViewList().DeleteAll();
	for(int k = 0; k < NImages(); k++) {
        CVisModel2D& m2d = *m_model.m2d[k];
        CVisSprite s;
        s.Image()       = Image(k);
        s.Filename()    = Input(k).Name();
        s.Camera()      = m2d.geom;
        m_sprites.ViewList().Insert(s);
    }
}

///////////////////////////////////////////////////////////////////////////////////////
//
//  Layer and synthetic invalidating functions
//
///////////////////////////////////////////////////////////////////////////////////////

void CVisLayeredComposite::InvalidateAll(int mask)
{
    // Invalidate all layer composites
    for (int l = 0; l < NLayers(); l++)
        LayerComposite(l).InvalidateAll(mask);
}

void CVisLayeredComposite::InvalidateAllSynth(int mask)
{
	// Invalidate all the synthetic image composites
    for (int k = 0; k < m_synthesized.size(); k++)
        SynthComposite(k).InvalidateAll(mask);
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  Miscellaneous helper functions
//
///////////////////////////////////////////////////////////////////////////////////////

CVisShape CVisLayeredComposite::ShapeOfSegment(int k, int l)
{
    CVisIntImage ilabel;
	ilabel.Alias(InputLabel(k));

	// Get color corresponding to layer l
    int ic = LayerColor(l);

	// This is an invalid rectangle - right boundary is at far left of label image,
	// top boundary is at the bottom of the label image, etc...
    int srow = ilabel.Bottom() - 1, erow = ilabel.Top();
    int scol = ilabel.Right() - 1, ecol = ilabel.Left();

	// For each row in label image
    for (int r = ilabel.Top(); r < ilabel.Bottom();  r++) {

		// Get pointer to start of current row
        int *ip = ilabel.PtrToFirstPixelInRow(r);

		// For each column in label image
        int sc = ilabel.Left(), ec = ilabel.Right() - 1;
        for (int c = sc; c <= ec; c++) {

			// If there is a correctly labeled pixel here, can increase the
			// size of the rectangle - move right boundary further right, etc
            if (ip[c] == ic) {
                if (c < scol)
                    scol = c;
                if (c > ecol)
                    ecol = c;
                if (r < srow)
                    srow = r;
                if (r > erow)
                    erow = r;
            }
        }
    }

    if (srow <= erow && scol <= ecol) {
		// If found some pixels of correct color - rectangle is valid
        return CVisShape(scol, srow, scol+ecol+1, srow+erow+1);
	}
    else {
        return CVisShape(0, 0, 0, 0);
	}
}

void CVisLayeredComposite::InsertLabel()
{
	// Get frame of last inserted input
    CVisRGBAByteFrame &frame = m_input.Frame(m_input.NFrames()-1);

	// Create a label image of same shape, clear its pixels, and wrap it in a frame
    CVisRGBAByteImage label(frame.Shape());
    label.ClearPixels();
    CVisRGBAByteFrame label_frame(label);

	// Create filename by prepending "B" to input image filename
    char *filename = m_input.WarpFilename(m_input.NFrames()-1, "B");

	// Set filename of label frame and insert into the label layer
	label_frame.SetName(filename);
    m_label.Insert(label_frame);

	// Force write of new label image and clear up filename
	m_write_labels = 1;
	delete filename;

	// Create another empty image of the same shape and insert it into m_synth_label
//	CVisRGBAByteImage synth_label(frame.Shape());
//	synth_label.ClearPixels();
//	CVisRGBAByteFrame synth_label_frame(synth_label);
//	m_synth_label.Insert(synth_label_frame);
	m_synth_label.PushBack(frame.Shape());
	m_synth_label.Back().ClearPixels();

    // Insert a blank image into all layers
    for (int l = 0; l < NLayers(); l++) {
		// Create the filename from the input image name and the layer name
		char *filename = new char[strlen(Input(NImages()-1).Name()) +
								  strlen(FinalLayer(l).Name()) + 2];
		strcpy(filename, Input(NImages()-1).Name());
		filename[strlen(Input(NImages()-1).Name())] = '_';
		strcpy(filename + (strlen(Input(NImages()-1).Name()) + 1), 
			   FinalLayer(l).Name());

		// Insert the blank image
		CVisLayer &layer = LayerComposite(l);
//        CVisRGBAByteFrame f(filename);
//        layer.Insert(f,-1,0,VIS_LAYER_MEMBERSHIP_OUT);
        layer.PushBack(filename);
		layer.Back().SetEVisLayerMembership(VIS_LAYER_MEMBERSHIP_OUT);

		delete [] filename;
    }

	// Add a synthetic image composite and add a frame to it for each layer
    m_synthesized.push_back(CVisLayerPtr());
    CVisLayer &synth = SynthComposite(NImages()-1);
    while (synth.NFrames() < NLayers()) {
        const char *filename = FinalLayer(synth.NFrames()).Name();
//        synth.Insert(filename, -1, VIS_LAYER_MEMBERSHIP_IN);
        synth.PushBack(filename);
		synth.Back().SetEVisLayerMembership(VIS_LAYER_MEMBERSHIP_IN);
    }
	synth.SetBackgroundGrayLevel(m_background_gray_level);
}

int CVisLayeredComposite::UniqueId()
{
    // Find the first id not in the set of used ids
    for (int id = 1; id > 0; id++) {
#if (_MSC_VER >= 1100)	// VC 5.0 or greater
        CVisIntVector::iterator where = std::find(m_id.begin(), m_id.end(), id);
#else
		CVisIntVector::iterator where = find(m_id.begin(), m_id.end(), id);
#endif
        if (where == m_id.end())
            return id;
    }

	assert(0);
    return 0;    
}


///////////////////////////////////////////////////////////////////////////////////////
//																					 //
//                class CVisLayeredView  --  Layered stereo view				     //
//																		             //
///////////////////////////////////////////////////////////////////////////////////////

CVisLayeredView::CVisLayeredView()
{
    m_stereo                = 0;
    m_composite             = 0;
    m_view_type             = VIS_LAYER_VIEW_INPUT;
	m_medium                = VIS_LAYER_MEDIUM_INTENSITY;
	m_display_mode          = VIS_LAYER_DM_SINGLE;
	m_num_layers            = VIS_LAYER_NUM_ALL_LAYERS;
    m_image                 = 0;
    m_layer                 = 0;
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  Access to the full state of the display mode 
//
///////////////////////////////////////////////////////////////////////////////////////

TVisLayerView CVisLayeredView::SetViewClass(TVisLayerView v)
{
	TVisLayerView old_view_type = m_view_type;

	if (m_view_type != v) {
	    // Update m_view_type, m_composite, m_layer and m_image
		m_view_type = v;
		SetComposite();						 

		// Certain combinations of m_view_type, m_display_mode, m_medium, and 
		// m_num_layers are incompatible, so reset the final 3 appropriately
		switch (v) {
		case VIS_LAYER_VIEW_INPUT:
			SetDisplayMode(VIS_LAYER_DM_SINGLE);
			if (old_view_type == VIS_LAYER_VIEW_LAYER ||
				old_view_type == VIS_LAYER_VIEW_FULL ||
				old_view_type == VIS_LAYER_VIEW_INTER) {
				SetNumLayers(VIS_LAYER_NUM_ALL_LAYERS);
			}
			break;
		case VIS_LAYER_VIEW_LAYER:
			SetMedium(VIS_LAYER_MEDIUM_INTENSITY);
			SetNumLayers(VIS_LAYER_NUM_SINGLE_LAYER);
			if (old_view_type != VIS_LAYER_VIEW_FULL &&
				old_view_type != VIS_LAYER_VIEW_INTER) {
				SetDisplayMode(VIS_LAYER_DM_SINGLE);
			}
			break;
		case VIS_LAYER_VIEW_FULL:
			SetMedium(VIS_LAYER_MEDIUM_INTENSITY);
			SetNumLayers(VIS_LAYER_NUM_SINGLE_LAYER);
			if (old_view_type != VIS_LAYER_VIEW_LAYER &&
				old_view_type != VIS_LAYER_VIEW_INTER) {
				SetDisplayMode(VIS_LAYER_DM_SINGLE);
			}
			break;
		case VIS_LAYER_VIEW_INTER:
			SetMedium(VIS_LAYER_MEDIUM_INTENSITY);
			SetNumLayers(VIS_LAYER_NUM_SINGLE_LAYER);
			if (old_view_type != VIS_LAYER_VIEW_FULL &&
				old_view_type != VIS_LAYER_VIEW_LAYER) {
				SetDisplayMode(VIS_LAYER_DM_SINGLE);
			}
			break;
		case VIS_LAYER_VIEW_SYNTHETIC:
		case VIS_LAYER_VIEW_FULL_SYNTH:
		case VIS_LAYER_VIEW_INTER_SYNTH:
        case VIS_LAYER_VIEW_DEPTH:
			SetDisplayMode(VIS_LAYER_DM_SINGLE);
			SetMedium(VIS_LAYER_MEDIUM_INTENSITY);
			if (old_view_type == VIS_LAYER_VIEW_LAYER ||
				old_view_type == VIS_LAYER_VIEW_FULL ||
				old_view_type == VIS_LAYER_VIEW_INTER) {
				SetNumLayers(VIS_LAYER_NUM_ALL_LAYERS);
			}
			break;
		default:
			assert(0);
		}
	}

	return old_view_type;
}

void CVisLayeredView::SetDisplayMode(TVisLayerDisplayMode dm)
{	
	m_display_mode = dm;
	if (m_composite) {
		// m_composite->m_composite may have become invalid
		m_composite->Invalidate(VIS_VALID_TOP | VIS_VALID_TOP_BKGD |
		                        VIS_VALID_BOTTOM | VIS_VALID_DIFFERENCE);
	}
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  Access to m_composite - the currently displayed layer or synthetic image 
//
///////////////////////////////////////////////////////////////////////////////////////

// Updates the value of m_composite. If input m_composite=0. If looking at layer, 
// m_composite points to the m_layer^th layer. If looking at a synthetic, points
// to m_image^th image. The rest is error checking.
void CVisLayeredView::SetComposite()
{
	switch (m_view_type) {
	case VIS_LAYER_VIEW_INPUT:
		m_composite = 0;
		break;
	case VIS_LAYER_VIEW_LAYER:
	case VIS_LAYER_VIEW_FULL:
	case VIS_LAYER_VIEW_INTER:
		if (m_stereo->NLayers() == 0) {
			assert(m_layer == 0);
			m_composite = 0;
		}
		else {
			assert(m_layer >= 0 && m_layer < m_stereo->NLayers());
			m_composite = &m_stereo->LayerComposite(m_layer);
			if (m_stereo->NImages() > 0) {
				m_composite->SetCurrentFrame(m_image);
			}
		}
		break;
	case VIS_LAYER_VIEW_SYNTHETIC:
	case VIS_LAYER_VIEW_FULL_SYNTH:
	case VIS_LAYER_VIEW_INTER_SYNTH:
	case VIS_LAYER_VIEW_DEPTH:
		if (m_stereo->NImages() == 0) {
			assert(m_image == 0);
			m_composite = 0;
		}
		else {
			assert(m_image >= 0 && m_image < m_stereo->NImages());
			m_composite = &m_stereo->SynthComposite(m_image);
			if (m_stereo->NLayers() > 0) {
				m_composite->SetCurrentFrame(m_layer);
			}
		}
		break;
	default:
		assert(0);
	}
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  Major display function - one big nested switch statement
//
///////////////////////////////////////////////////////////////////////////////////////

CVisRGBAByteImage& CVisLayeredView::CurrentView()
{
	if (m_stereo->NImages() == 0 || m_stereo->NLayers() == 0) {
		m_img = CVisRGBAByteImage(0, 0, 1, 1);
		return m_img;
	}
	CVisRGBAByteImage &img  = m_stereo->Image(m_image);
	CVisRGBAByteImage &img2 = m_stereo->InputLabel(m_image);
	CVisRGBAByteImage &img3 = m_stereo->SynthLabel(m_image);
	int bgl = m_stereo->BackgroundGrayLevel();
	switch (m_view_type) {
    case VIS_LAYER_VIEW_INPUT:
		assert(m_image >= 0 && m_image < m_stereo->NImages());
		switch (m_medium) {
		case VIS_LAYER_MEDIUM_INTENSITY:
			switch (m_num_layers) {
			case VIS_LAYER_NUM_SINGLE_LAYER:
				if (m_img.Shape() != img.Shape()) {
					m_img.Allocate(img.Shape());
				}
				m_stereo->ExtractSegment(m_image, m_layer, m_img, 1);
				return m_img;
			case VIS_LAYER_NUM_ALL_LAYERS:
				return img;
			default:
				assert(0);
				m_img = CVisRGBAByteImage(0, 0, 1, 1);
				return m_img;
			}
		case VIS_LAYER_MEDIUM_PIXEL_ASSIGNMENT:
			switch (m_num_layers) {
			case VIS_LAYER_NUM_SINGLE_LAYER:
				if (m_img.Shape() != img2.Shape()) {
					m_img.Allocate(img2.Shape());
				}
				m_stereo->ExtractSingleLayer(img2, m_layer, m_img);
				return m_img;
			case VIS_LAYER_NUM_ALL_LAYERS:
					return m_stereo->InputLabel(m_image);
			default:
				assert(0);
				m_img = CVisRGBAByteImage(0, 0, 1, 1);
				return m_img;
			}	
		default:
			assert(0);
			m_img = CVisRGBAByteImage(0, 0, 1, 1);
			return m_img;
		}
	case VIS_LAYER_VIEW_FULL_SYNTH:
	case VIS_LAYER_VIEW_INTER_SYNTH:
    case VIS_LAYER_VIEW_SYNTHETIC:
    case VIS_LAYER_VIEW_DEPTH:
		assert(m_image >= 0 && m_image < m_stereo->NImages());
		if (m_composite->NFrames() == 0) {
			m_img = CVisRGBAByteImage(0, 0, 1, 1);
			return m_img;
		}
		else {
			switch (m_medium) {
			case VIS_LAYER_MEDIUM_INTENSITY:
				switch (m_display_mode) {
				case VIS_LAYER_DM_SINGLE:
					switch (m_num_layers) {
					case VIS_LAYER_NUM_SINGLE_LAYER:
						return m_composite->TopLayer();
					case VIS_LAYER_NUM_ALL_LAYERS:
						return m_composite->BottomLayer(m_stereo->SynthCompositingOperator());
					default:
						assert(0);
  						m_img = CVisRGBAByteImage(0, 0, 1, 1);
						return m_img;
					}
				case VIS_LAYER_DM_BLINK:
					if (m_composite->Blink()) {
						switch (m_num_layers) {
						case VIS_LAYER_NUM_SINGLE_LAYER:
							return m_composite->TopLayer();
						case VIS_LAYER_NUM_ALL_LAYERS:
							return m_composite->BottomLayer(m_stereo->SynthCompositingOperator());
						default:
							assert(0);
  							m_img = CVisRGBAByteImage(0, 0, 1, 1);
							return m_img;
						}
					}
					else { 
						return m_stereo->Image(m_image);
					}
					break;
				case VIS_LAYER_DM_DIFFERENCE:
					switch (m_num_layers) {
					case VIS_LAYER_NUM_SINGLE_LAYER:
						{
							CVisRGBAByteImage &img = m_composite->SynthDifference(
															m_stereo->CompositingOperator(),
															m_stereo->DifferencingOperator(),
															m_stereo->Image(m_image));
							m_img.Allocate(img.Shape());
							m_img.FillPixels(CVisRGBABytePixel(bgl, bgl, bgl, 255));
							VisComposite(img, m_img, m_img, composite_over);
							m_stereo->ExtractDiffBand(m_img, m_stereo->SynthDifferencingOperator()); 
							return m_img;
						}
					case VIS_LAYER_NUM_ALL_LAYERS:
						{
							CVisRGBAByteImage &img = m_composite->CompSynthDiff(
															m_stereo->CompositingOperator(),
															m_stereo->DifferencingOperator(),
															m_stereo->Image(m_image));
							m_img.Allocate(img.Shape());
							m_img.FillPixels(CVisRGBABytePixel(bgl, bgl, bgl, 255));
							VisComposite(img, m_img, m_img, composite_over);
							m_stereo->ExtractDiffBand(m_img, m_stereo->SynthDifferencingOperator()); 
							return m_img;
						}
					default:
						assert(0);
  						m_img = CVisRGBAByteImage(0, 0, 1, 1);
						return m_img;
					}
				case VIS_LAYER_DM_UNWARPED_DIFF:
					switch (m_num_layers) {
					case VIS_LAYER_NUM_SINGLE_LAYER:
						m_img.Allocate(m_composite->UnwarpedDiffImage(m_layer).Shape());
						m_img.FillPixels(CVisRGBABytePixel(bgl, bgl, bgl, 255));
						VisComposite(m_composite->UnwarpedDiffImage(m_layer), m_img, m_img, 
							                                        composite_over);
						m_stereo->ExtractDiffBand(m_img, m_stereo->SynthDifferencingOperator()); 
						return m_img;
					default:
						assert(0);
  						m_img = CVisRGBAByteImage(0, 0, 1, 1);
						return m_img;
					}
				default:
					assert(0);
  					m_img = CVisRGBAByteImage(0, 0, 1, 1);
					return m_img;
				}
			case VIS_LAYER_MEDIUM_PIXEL_ASSIGNMENT:
				switch (m_num_layers) {
				case VIS_LAYER_NUM_SINGLE_LAYER:
					if (m_img.Shape() != img3.Shape()) {
						m_img.Allocate(img3.Shape());
					}
					m_stereo->ExtractSingleLayer(img3, m_layer, m_img);
					return m_img;
				case VIS_LAYER_NUM_ALL_LAYERS:
						return m_stereo->SynthLabel(m_image);
				default:
					assert(0);
					m_img = CVisRGBAByteImage(0, 0, 1, 1);
					return m_img;
				}	
			default:
				assert(0);
				m_img = CVisRGBAByteImage(0, 0, 1, 1);
				return m_img;
			}
		}
    case VIS_LAYER_VIEW_LAYER:
    case VIS_LAYER_VIEW_FULL:
    case VIS_LAYER_VIEW_INTER:
    	assert(m_layer >= 0 && m_layer < m_stereo->NLayers());
        if (m_composite->NFrames() == 0) {
            m_img = CVisRGBAByteImage(0, 0, 1, 1);
            return m_img;
        }
		else {
			switch (m_display_mode) {
			case VIS_LAYER_DM_SINGLE:
				return m_composite->TopLayer();
			case VIS_LAYER_DM_COMPOSITE:
				return m_composite->BottomLayer(m_stereo->CompositingOperator());
			case VIS_LAYER_DM_BLINK:
				return m_composite->BlinkLayer(m_stereo->CompositingOperator());
			case VIS_LAYER_DM_TRANSPARENT:
				return m_composite->Composite(composite_average, 
											  m_stereo->CompositingOperator());
			case VIS_LAYER_DM_OVERLAY:
				return m_composite->Composite(composite_over, 
											  m_stereo->CompositingOperator());
			case VIS_LAYER_DM_DIFFERENCE:
				{
					CVisRGBAByteImage &img = m_composite->Difference(
						                       m_stereo->CompositingOperator(),
											   m_stereo->DifferencingOperator());
					m_img.Allocate(img.Shape());
					m_img.FillPixels(CVisRGBABytePixel(bgl, bgl, bgl, 255));
					VisComposite(img, m_img, m_img, composite_over);
					m_stereo->ExtractDiffBand(m_img, m_stereo->DifferencingOperator()); 
					return m_img;
				}
			case VIS_LAYER_DM_UNWARPED_DIFF:
				{
					CVisRGBAByteImage &img = m_composite->UnWarpedDiff(
						                         m_stereo->CompositingOperator(),
												 m_stereo->DifferencingOperator());
					m_img.Allocate(img.Shape());
					m_img.FillPixels(CVisRGBABytePixel(bgl, bgl, bgl, 255));
					VisComposite(img, m_img, m_img, composite_over);
					m_stereo->ExtractDiffBand(m_img, m_stereo->DifferencingOperator()); 
					return m_img;
				}
			default:
				assert(0);
			}
		}
	default:
		assert(0);
		m_img = CVisRGBAByteImage(0, 0, 1, 1);
    }
    return m_img;
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  Functions for displaying current image information in the title bar 
//
///////////////////////////////////////////////////////////////////////////////////////

const char* CVisLayeredView::ViewName()
{
    switch (m_view_type) {
    case VIS_LAYER_VIEW_INPUT:
	case VIS_LAYER_VIEW_FULL_SYNTH:
	case VIS_LAYER_VIEW_INTER_SYNTH:
    case VIS_LAYER_VIEW_SYNTHETIC:
    case VIS_LAYER_VIEW_DEPTH:
        if (m_stereo->NImages() == 0)
            return "No images";
        else  {
			assert(m_image >= 0 && m_image < m_stereo->NImages());
			const char *filename,*slash;
			switch(m_medium) {
			case VIS_LAYER_MEDIUM_INTENSITY:
				filename = m_stereo->Input(m_image).Name();
				break;
            case VIS_LAYER_MEDIUM_PIXEL_ASSIGNMENT:
				filename = m_stereo->Label(m_image).Name();
				break;
			default:
				assert(0);
				return "";
				break;
			}
            slash = strrchr(filename, '\\');
            if (slash)
                filename = slash+1;
            return filename;
        } 
		break;
    case VIS_LAYER_VIEW_LAYER:
    case VIS_LAYER_VIEW_FULL:
    case VIS_LAYER_VIEW_INTER:
        if (m_stereo->NLayers() == 0)
            return "No layers";
        else {
			assert(m_layer >=0 && m_layer < m_stereo->NLayers());
            const char *filename = m_stereo->FinalLayer(m_layer).Name();
            const char *slash = strrchr(filename, '\\');
            if (slash)
                filename = slash+1;
            return filename;
        }
		break;
    }
	assert(0);
    return "";
}

const char* CVisLayeredView::TileName()
{
    switch (m_view_type) {
    case VIS_LAYER_VIEW_INPUT:
	case VIS_LAYER_VIEW_FULL_SYNTH:
	case VIS_LAYER_VIEW_INTER_SYNTH:
    case VIS_LAYER_VIEW_SYNTHETIC:
    case VIS_LAYER_VIEW_DEPTH:
		switch (m_num_layers) {
		case VIS_LAYER_NUM_SINGLE_LAYER:
			if (m_stereo->NLayers() == 0)
				return "No layers";
			else {
				assert(m_layer >= 0 && m_layer < m_stereo->NLayers());
				const char *filename = m_stereo->FinalLayer(m_layer).Name();
				const char *slash = strrchr(filename, '\\');
				if (slash)
					filename = slash+1;
				return filename;
			}
		case VIS_LAYER_NUM_ALL_LAYERS:
			if (m_stereo->NLayers() == 0)
				return "No layers";
			else 
			    return "All layers";
		default:
			assert(0);
			return "";
		}
    case VIS_LAYER_VIEW_LAYER:
    case VIS_LAYER_VIEW_FULL:
    case VIS_LAYER_VIEW_INTER:
		if (m_stereo->NImages() == 0)
			return "No images";
        else if (m_stereo->NLayers() == 0)
			return "";
		else if (m_stereo->LayerComposite(m_layer).IsEmpty())
			return "All cropped regions empty";
		else if (m_display_mode == VIS_LAYER_DM_COMPOSITE)
            return "Composite";
        else {
    		assert(m_layer >= 0 && m_layer < m_stereo->NLayers());
            const char *filename = m_stereo->Input(m_image).Name();
            const char *slash = strrchr(filename, '\\');
            if (slash)
                filename = slash+1;
            return filename;
        }
    default:
		assert(0);
		return "";
	}
}


///////////////////////////////////////////////////////////////////////////////////////
//
//  Motion helper functions 
//
///////////////////////////////////////////////////////////////////////////////////////

void CVisLayeredView::SwitchImage(int delta)
{
	switch (m_view_type) {
	case VIS_LAYER_VIEW_LAYER:
	case VIS_LAYER_VIEW_FULL:
	case VIS_LAYER_VIEW_INTER:
		SetComposite();
        if (m_composite) {
			switch (delta) {
			case -2: m_image = 0; break;
			case -1: m_image = __max(0, m_image-1); break;
			case  1: m_image = __min(m_stereo->NImages()-1, m_image+1); break;
			case  2: m_image = m_stereo->NImages()-1; break;
			default: assert(0);
			}
			m_composite->SetCurrentFrame(m_image);
		}
		break;
	case VIS_LAYER_VIEW_FULL_SYNTH:
	case VIS_LAYER_VIEW_INTER_SYNTH:
	case VIS_LAYER_VIEW_SYNTHETIC:
    case VIS_LAYER_VIEW_DEPTH:
	case VIS_LAYER_VIEW_INPUT:
		switch (delta) {
		case -2: m_image = 0; break;
		case -1: m_image = __max(0, m_image-1); break;
		case  1: m_image = __min(m_stereo->NImages()-1, m_image+1); break;
		case  2: m_image = m_stereo->NImages()-1; break;
		default: assert(0);
		}
		SetComposite();
		if (m_composite) {
			m_composite->SetCurrentFrame(m_layer);
		}
		break;
	default:
		assert(0);
		break;
	}
}

void CVisLayeredView::SwitchLayer(int delta)
{
	switch (m_view_type) {
	case VIS_LAYER_VIEW_FULL_SYNTH:
	case VIS_LAYER_VIEW_INTER_SYNTH:
	case VIS_LAYER_VIEW_SYNTHETIC:
    case VIS_LAYER_VIEW_DEPTH:
		SetComposite();
        if (m_composite) {
			int old_layer = m_layer;
			switch (delta) {
			case -2: m_layer = 0; break;
			case -1: m_layer = __max(0, m_layer-1); break;
			case  1: m_layer = __min(m_stereo->NLayers()-1, m_layer+1); break;
			case  2: m_layer = m_stereo->NLayers()-1; break;
			default: assert(0);
			}
			m_composite->SetCurrentFrame(m_layer);
		}
		break;
	case VIS_LAYER_VIEW_LAYER:
	case VIS_LAYER_VIEW_FULL:
	case VIS_LAYER_VIEW_INTER:
	case VIS_LAYER_VIEW_INPUT:
		switch (delta) {
		case -2: m_layer = 0; break;
		case -1: m_layer = __max(0, m_layer-1); break;
		case  1: m_layer = __min(m_stereo->NLayers()-1, m_layer+1); break;
		case  2: m_layer = m_stereo->NLayers()-1; break;
		default: assert(0);
		}
		SetComposite();
		if (m_composite)
			m_composite->SetCurrentFrame(m_image);
		break;
	default:
		assert(0);
		break;
	}
}
