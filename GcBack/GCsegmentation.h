#ifndef _GCSEGMENTATION_H_
#define _GCSEGMENTATION_H_
#pragma once


// PROJECT INCLUDES
#include <stdio.h>
#include <vector>
#include "../CImg.h"
#include "graph.h"
#include "skClustering.h"
 
// MEANSHIFT PROJECT INCLUDES
#include "../meanshift/msImageProcessor.h"
#include "../meanshift/ms.h"
#include "../meanshift/msSys.h"
#include "../meanshift/tdef.h"
#include "../meanshift/rlist.h"

// NAME SPACE USING 
using namespace cimg_library;
using namespace std;
using namespace meanshift;

// some constants used in the project
#define HARD_OBJ_LABEL 0
#define HARD_BCK_LABEL 1
#define UNKNOWN_LABEL  2
#define LAMBDA 200
//#define LAMBDA 200

typedef struct 
{
	float w_s_link;
	float w_t_link;
}typeTweight;

typedef struct 
{
	int x;
	int y;
}Ptposition;


class GCsegmentation
{
public:
	// constructor 
	GCsegmentation();
    //destructor
	~GCsegmentation(void);

	//*xxxxxxxxxxxxxxxxxxxxxxxxxxxx
	//    Data
	//*xxxxxxxxxxxxxxxxxxxxxxxxxxxx 
    CImg<unsigned char> m_pOrginalImg;     // Original Image
    CImg<unsigned char> m_pResultImg;      // Result Image
	int				m_Width;	                            						   
	int				m_Height;
	int				m_GmmNum;                          // cluster modes
	int				m_nDim;                            // data vector dimension
	msImageProcessor *m_pMsIP;	           // Mean-shift segmentation results
	int				*m_pMsLabels;			   // meanshift region label
	float			*m_pMsModes;             // ms_modes
	int				*m_pMsModePointCounts;   // ms_modePointCounts
	int				m_MsRegionCount;		   // ms_regionCount
	float			m_MsSpatial;             // Parameters of mean-shift
	float			m_MsColor;
	float			m_MsMinRgns;

	vector<float>	m_vForeMean;                // foreground clustering center
	vector<float>	m_vBackMean;                // background clustering center
	Graph			*m_pG;				   // graph
	Graph::node_id	*m_pNodes;			   // node


	//*xxxxxxxxxxxxxxxxxxxxxxxxxxxx
	//    Operation
	//*xxxxxxxxxxxxxxxxxxxxxxxxxxxx
	/** implement functions as follows
	*  (1) convert an RGB image to LUV
	*  (2) do meanshift on the original image
	*  (3) initialize the n-link
	*/
	void        Initialize(CImg<unsigned char> pOrginalImg);
    void        InitiLikelihood(vector<Ptposition>FrontStroke, vector<Ptposition>BackStroke);
	void        SetLikelihood(Graph* g, vector<Ptposition>FrontStroke, vector<Ptposition>BackStroke );
	typeTweight Get_Likelihood(int x);
	int         Get_node_type(int pix, vector<Ptposition>FrontStroke, vector<Ptposition>BackStroke);
	float       Get_neigbour_weight(int num_p, int num_q);    
    bool        valid_neighbour(int wid, int heig);
	CImg<unsigned char> segmentation( vector<Ptposition>FrontStroke, vector<Ptposition>BackStroke );
	void rgb2luv(CImg<unsigned char> &src, CImg< float> &dst);

	//*xxxxxxxxxxxxxxxxxxxxxxxxxxxx
	//    Usage
	//*xxxxxxxxxxxxxxxxxxxxxxxxxxxx
	//  GCsegmentation Gcs=new GCsegmentation(OrignalImage);
	//  Lambda=0.9
	//  CImg ResultImg=Gcs.segmentation(OrignalImage, FrontStroke, BackStroke, Lambda);
};
#endif
