#ifndef __FACEDEFINE_H__
#define __FACEDEFINE_H__		1

#define MES_FILE_HEADER			"Human Face Mesh"//msh文件头
const int TEXTURE_WIDTH = 512;
const int TEXTURE_HEIGHT = 512;
const float LIGHT_RADIUS = 5000.0f;

enum SHOWMODELTYPE//show model type
{
	SFM_NONE,		//show none
	SFM_VERTICES,	//show vertices
	SFM_MESH,		//show mesh
	SFM_RENDER,		//show render
	SFM_TEXTURE,	//show texture
};

typedef struct POINT3D
{
	float x;//x坐标
	float y;//y坐标
	float z;//z坐标
}POINT3D;

typedef struct TRIANGLE
{
	int first;//第一个顶点序号
	int second;//第二个顶点序号
	int third;//第三个顶点序号
}TRIANGLE;

typedef struct FACESTRUCTURE
{
	int	totalVertices;//顶点总数
	int totalFaces;//面总数
	int totalFeatures;//特征点数
	POINT3D *coordinate;//顶点坐标
	TRIANGLE *faces;//面的顶点
	int *features;//特征点
}FACESTRUCTURE;

#endif