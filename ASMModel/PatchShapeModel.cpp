#pragma warning(disable:4786)
// PatchShapeModel.cpp: implementation of the CPatchShapeModel class.
//
//////////////////////////////////////////////////////////////////////
#include "..\CSBase\csbase.h"
//#include "stdafx.h"
#include "PatchShapeModel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
CPatchShapeModel::CPatchShapeModel(const CPatchShapeModel& ref)
{
	m_ConnectedModel = ref.ConnectedModel();
	m_fRelativeShapeSize = ref.m_fRelativeShapeSize;
	m_MaskType = ref.m_MaskType;
	m_RefShape = ref.m_RefShape;

	m_vPatchHalfSize.resize(ref.m_vPatchHalfSize.size());
	for (int i = 0; i< m_vPatchHalfSize.size(); i++)
	{
		m_vPatchHalfSize[i] = ref.m_vPatchHalfSize[i];
	}
}

const CPatchShapeModel& CPatchShapeModel::operator=(const CPatchShapeModel& ref)
{
	if ( this!= &ref )
	{
		m_ConnectedModel = ref.ConnectedModel();
		m_fRelativeShapeSize = ref.m_fRelativeShapeSize;
		m_MaskType = ref.m_MaskType;
		m_RefShape = ref.m_RefShape;
		
		m_vPatchHalfSize.resize(ref.m_vPatchHalfSize.size());
		for (int i = 0; i< m_vPatchHalfSize.size(); i++)
		{
			m_vPatchHalfSize[i] = ref.m_vPatchHalfSize[i];
		}
	}

	return (*this);
}

CPatchCurveModel::CPatchCurveModel(const CPatchCurveModel& ref)
{
	m_MaskType = ref.m_MaskType;
	m_PatchHalfSize = ref.m_PatchHalfSize;
}

const CPatchCurveModel& CPatchCurveModel::operator=(const CPatchCurveModel& ref)
{
	if ( this!=&ref )
	{
		m_MaskType = ref.m_MaskType;
		m_PatchHalfSize = ref.m_PatchHalfSize;
	}
	return (*this);
}


CPatchShapeModel::CPatchShapeModel()
{

}

CPatchShapeModel::~CPatchShapeModel()
{

}

int CPatchShapeModel::NPatch() const
{
	return m_ConnectedModel.PointNum();
}

CVisGrayByteImage * CPatchShapeModel::GetMaskImg(int index, int type) const
{
	if ( 0 == type)
	{
		return NULL;
	}
	if ( 1 == type )
	{	
		int iWidth = m_vPatchHalfSize[index].cx *2 +1;
		int iHeight = m_vPatchHalfSize[index].cy *2 +1;
	
		CVisGrayByteImage * pMaskImg = new CVisGrayByteImage(iWidth, iHeight);
		pMaskImg->FillPixels(255);

		for (int ix = 0; ix < iWidth; ix++ )
		{
			double tempf = fabs(ix - m_vPatchHalfSize[index].cx);
			tempf /= m_vPatchHalfSize[index].cx;
			tempf = 1 - tempf/2;
			for (int iy = 0; iy < iHeight; iy++ )
			{
				pMaskImg->Pixel(ix,iy) *= tempf;
			}
		}
		return pMaskImg;
	}

	return NULL;
}
		
void CPatchShapeModel::GetShapeNormal(const CShape& shape, CShape& shapeNormal) const
{
	::GetShapeNormal(m_ConnectedModel, shape, shapeNormal);
}

void CPatchShapeModel::InitPatchGeomParameter(std::vector<CLocalPatchGeomParameter>& geomparam, const CShape& shape) const
{
	assert( NPatch() == shape.PointNum());

	CShape shapeNormal;
	GetShapeNormal(shape, shapeNormal);

	double Tx, Ty, s, theta;
	CShape tempShape = shape;
	
	tempShape.AlignTangent(m_RefShape, Tx, Ty, s, theta);

	geomparam.resize(NPatch());
	for (int iPatch = 0; iPatch < NPatch(); iPatch++ )
	{
		geomparam[iPatch].m_fCx = shape.PointX(iPatch);
		geomparam[iPatch].m_fCy = shape.PointY(iPatch);
		geomparam[iPatch].m_fScale = s / m_fRelativeShapeSize; 
		geomparam[iPatch].m_fTheta = atan2(shapeNormal.PointY(iPatch), shapeNormal.PointX(iPatch));
	}
}

void CPatchShapeModel::GetPatchData(const CVisGrayByteImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, std::vector<CVisDVector>& vPatchData) const
{
	vPatchData.resize(NPatch());
	
	for (int iPatch = 0; iPatch < NPatch(); iPatch++ )
	{
		GetLocalPatchData(image, geomparam[iPatch], m_vPatchHalfSize[iPatch], vPatchData[iPatch]);
	}
}

void CPatchShapeModel::GetPatchData(const CVisGrayFloatImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, std::vector<CVisDVector>& vPatchData) const
{
	vPatchData.resize(NPatch());
	
	for (int iPatch = 0; iPatch < NPatch(); iPatch++ )
	{
		GetLocalPatchData(image, geomparam[iPatch], m_vPatchHalfSize[iPatch], vPatchData[iPatch]);
	}
}

void CPatchShapeModel::PutPatchData(CVisGrayByteImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisDVector>& vPatchData) const
{
	for (int iPatch = 0; iPatch < NPatch(); iPatch++ )
	{
		PutLocalPatchData(image, geomparam[iPatch], m_vPatchHalfSize[iPatch], vPatchData[iPatch]);
	}
}

void CPatchShapeModel::PutPatchData(CVisGrayFloatImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisDVector>& vPatchData) const
{
	for (int iPatch = 0; iPatch < NPatch(); iPatch++ )
	{
		PutLocalPatchData(image, geomparam[iPatch], m_vPatchHalfSize[iPatch], vPatchData[iPatch]);
	}
}

void CPatchShapeModel::PutPatchDataWithMask(CVisGrayByteImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisDVector>& vPatchData) const
{
	CVisGrayShortImage maskImg;
	CVisGrayShortImage foreImg;

	maskImg.Allocate(image.Shape());
	maskImg.FillPixels(0);
	foreImg.Allocate(image.Shape());
	foreImg.FillPixels(0);

	for (int iPatch = 0; iPatch < NPatch(); iPatch++ )
	{
		CVisGrayByteImage * pPatchMask = GetMaskImg(iPatch, m_MaskType);
		PutLocalPatchData(foreImg, maskImg, geomparam[iPatch], m_vPatchHalfSize[iPatch], vPatchData[iPatch], pPatchMask);
	}		

	for (int ix = 0 ; ix< maskImg.Width(); ix++)
	{
		for (int iy = 0; iy < maskImg.Height(); iy++ )
		{
			int iPixelBack = image.Pixel(ix,iy);
			int iPixelMask = maskImg.Pixel(ix,iy);
			int iPixelFore = foreImg.Pixel(ix,iy);

			int iPixelDes;
			if (iPixelMask < 255)
			{
				iPixelDes = (iPixelFore * iPixelMask + iPixelBack * (255 - iPixelMask))/ 255;
			}
			else
			{
				iPixelDes = iPixelFore * 255 / iPixelMask;
			}
			

			image.Pixel(ix,iy) = iPixelDes;
		}
	}	
}

void CPatchShapeModel::PutPatchDataWithMask(CVisGrayFloatImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisDVector>& vPatchData) const
{
	CVisGrayShortImage maskImg;
	CVisGrayFloatImage foreImg;

	maskImg.Allocate(image.Shape());
	maskImg.FillPixels(0);
	foreImg.Allocate(image.Shape());
	foreImg.FillPixels(0);

	for (int iPatch = 0; iPatch < NPatch(); iPatch++ )
	{
		CVisGrayByteImage * pPatchMask = GetMaskImg(iPatch, m_MaskType);
		PutLocalPatchData(foreImg, maskImg, geomparam[iPatch], m_vPatchHalfSize[iPatch], vPatchData[iPatch], pPatchMask);
	}		

	for (int ix = 0 ; ix< maskImg.Width(); ix++)
	{
		for (int iy = 0; iy < maskImg.Height(); iy++ )
		{
			float iPixelBack = image.Pixel(ix,iy);
			int iPixelMask = maskImg.Pixel(ix,iy);
			float iPixelFore = foreImg.Pixel(ix,iy);

			float iPixelDes;
			if (iPixelMask < 255)
			{
				iPixelDes = min(255, (iPixelFore * iPixelMask + iPixelBack * (255 - iPixelMask))/ 255);
			}
			else
			{
				iPixelDes = min(255, (iPixelFore * 255 / iPixelMask));
			}

			image.Pixel(ix,iy) = iPixelDes;
		}
	}	
}

void CPatchShapeModel::Init(const CConnectedModel& ConnectedModel, const std::vector<SIZE> vPatchHalfSize, const CShape& refShape, double relSize)
{
	m_ConnectedModel = ConnectedModel;
	m_vPatchHalfSize.resize(vPatchHalfSize.size());
	for (int i = 0; i < m_vPatchHalfSize.size(); i++ )
	{
		m_vPatchHalfSize[i] = vPatchHalfSize[i];
	}
	m_RefShape = refShape;
	m_fRelativeShapeSize = relSize;
	m_MaskType = 1;	
}

COutputDataFile& operator<<(COutputDataFile& fs, const CPatchShapeModel& m)
{
	return Output(fs, m);
}

COutputDataFile& Output(COutputDataFile& fs, const CPatchShapeModel& m)
{
	fs << ClassTag("CPatchShapeModel")<< sep_endl;
	fs << m.m_ConnectedModel << sep_endl;
	for (int i = 0; i< m.NPatch(); i++ )
	{
		fs << (int)m.m_vPatchHalfSize[i].cx << (int)m.m_vPatchHalfSize[i].cy ;
	}
	fs << sep_endl;
	fs << m.m_MaskType << m.m_fRelativeShapeSize << sep_endl;
	fs << m.m_RefShape;
	return fs;
}

CInputDataFile& operator>>(CInputDataFile& fs, CPatchShapeModel& m)
{
	return Input(fs, m);
}

CInputDataFile& Input(CInputDataFile& fs, CPatchShapeModel& m)
{
	fs >> ClassTag("CPatchShapeModel") >> sep_endl;
	fs >> m.m_ConnectedModel >> sep_endl;

	m.m_vPatchHalfSize.resize(m.NPatch());
	for (int i = 0; i< m.NPatch(); i++ )
	{
		int tempx, tempy;
		fs >> tempx >> tempy;
		m.m_vPatchHalfSize[i].cx = tempx;
		m.m_vPatchHalfSize[i].cy = tempy;
	}
	fs >> m.m_MaskType >> m.m_fRelativeShapeSize >> sep_endl;
	fs >> m.m_RefShape;

	return fs;
}

CVisTransform4x4 CPatchShapeModel::GetLocalPatchTrans(int index, const std::vector<CLocalPatchGeomParameter>& geomparam) const
{
	return CPatchModel::GetLocalPatchTrans(geomparam[index] , m_vPatchHalfSize[index]);	
}

void CPatchShapeModel::GetShapeFromParameter(const std::vector<CLocalPatchGeomParameter>& geomparam, CShape& shape) const
{
	shape.Resize(NPatch());
	for (int iPatch = 0; iPatch < NPatch(); iPatch++)
	{
		shape.PointX(iPatch) = geomparam[iPatch].m_fCx;
		shape.PointY(iPatch) = geomparam[iPatch].m_fCy;
	}
}

CPatchCurveModel::CPatchCurveModel()
{
	m_MaskType = 1;
	m_PatchHalfSize.cx = 6;
	m_PatchHalfSize.cy = 4;
}

void CPatchCurveModel::Init(const SIZE PatchHalfSize, int maskType)
{
	m_PatchHalfSize = PatchHalfSize;
	m_MaskType = maskType;
}

void CPatchCurveModel::InitPatchGeomParameter(std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CShape>& vCurves) const
{
	CCubicBezierSpline spline;

	geomparam.clear();
	if (vCurves.empty())
	{
		return;
	}

	// Curve Resample
	for (int iCurve = 0; iCurve < vCurves.size(); iCurve++ )
	{
		const CShape& shape = vCurves[iCurve];

		CShape ResampledShape;
		{
			std::vector<CPoint2> vKeyPoints;
			Shape2vPoint2(shape, vKeyPoints);
			spline.Create_Catmull_Rom_Spline(vKeyPoints);
			double len = spline.Length();
			spline.Resample(vKeyPoints, len/m_PatchHalfSize.cy );
			vPoint22Shape(vKeyPoints, ResampledShape);
		}
	
		CShape shapeNormal;
		GetCurveNormal(ResampledShape, shapeNormal);

//		double Tx, Ty, s, theta;
		
		CLocalPatchGeomParameter curparam;
		for (int iPatch = 0; iPatch < ResampledShape.PointNum(); iPatch++ )
		{
			curparam.m_fCx = ResampledShape.PointX(iPatch);
			curparam.m_fCy = ResampledShape.PointY(iPatch);
			curparam.m_fScale = 1.0f; 
			curparam.m_fTheta = atan2(shapeNormal.PointY(iPatch), shapeNormal.PointX(iPatch));
			geomparam.push_back(curparam);
		}
	}
}

void CPatchCurveModel::GetPatchData(const CVisGrayByteImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, std::vector<CVisDVector>& vPatchData) const
{
	if (geomparam.empty())
	{
		vPatchData.clear();
		return;
	}

	int nPatch = geomparam.size();

	vPatchData.resize(nPatch);
	for (int iPatch = 0; iPatch < nPatch; iPatch++ )
	{
		GetLocalPatchData(image, geomparam[iPatch], PatchHalfSize(), vPatchData[iPatch]);
	}
}

void CPatchCurveModel::GetPatchImage(const CVisGrayByteImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, std::vector<CVisGrayByteImage>& vPatchData) const
{
	if (geomparam.empty())
	{
		vPatchData.clear();
		return;
	}

	int nPatch = geomparam.size();

	vPatchData.resize(nPatch);
	for (int iPatch = 0; iPatch < nPatch; iPatch++ )
	{
		GetLocalPatchImage(image, geomparam[iPatch], PatchHalfSize(), vPatchData[iPatch]);
	}
}
	
void CPatchCurveModel::PutPatchData(CVisGrayByteImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisDVector>& vPatchData) const
{
	if (geomparam.empty())
	{
		return;
	}

	int nPatch = geomparam.size();
	for (int iPatch = 0; iPatch < nPatch; iPatch++ )
	{
		PutLocalPatchData(image, geomparam[iPatch], PatchHalfSize(), vPatchData[iPatch]);
	}	
}

void CPatchCurveModel::PutPatchImage(CVisGrayByteImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisGrayByteImage>& vPatchData) const
{
	if (geomparam.empty())
	{
		return;
	}

	int nPatch = geomparam.size();
	for (int iPatch = 0; iPatch < nPatch; iPatch++ )
	{
		PutLocalPatchImage(image, geomparam[iPatch], PatchHalfSize(), vPatchData[iPatch]);
	}	
}	

void CPatchCurveModel::PutPatchDataWithMask(CVisGrayByteImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisDVector>& vPatchData) const
{
	if (geomparam.empty())
	{
		return;
	}

	CVisGrayShortImage maskImg;
	CVisGrayShortImage foreImg;
	int nPatch = geomparam.size();

	maskImg.Allocate(image.Shape());
	maskImg.FillPixels(0);
	foreImg.Allocate(image.Shape());
	foreImg.FillPixels(0);

	for (int iPatch = 0; iPatch < nPatch; iPatch++ )
	{
		CVisGrayByteImage * pPatchMask = GetMaskImg(m_MaskType);
		PutLocalPatchData(foreImg, maskImg, geomparam[iPatch], PatchHalfSize(), vPatchData[iPatch], pPatchMask);
	}		

	for (int ix = 0 ; ix< maskImg.Width(); ix++)
	{
		for (int iy = 0; iy < maskImg.Height(); iy++ )
		{
			int iPixelBack = image.Pixel(ix,iy);
			int iPixelMask = maskImg.Pixel(ix,iy);
			int iPixelFore = foreImg.Pixel(ix,iy);

			int iPixelDes;
			if (iPixelMask < 255)
			{
				iPixelDes = (iPixelFore * iPixelMask + iPixelBack * (255 - iPixelMask))/ 255;
			}
			else
			{
				iPixelDes = iPixelFore * 255 / iPixelMask;
			}
			
			image.Pixel(ix,iy) = iPixelDes;
		}
	}	
}

void CPatchCurveModel::PutPatchDataWithMask(CVisGrayByteImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisGrayByteImage>& vPatchData) const
{
	if (geomparam.empty())
	{
		return;
	}

	CVisGrayShortImage maskImg;
	CVisGrayShortImage foreImg;
	int nPatch = geomparam.size();

	maskImg.Allocate(image.Shape());
	maskImg.FillPixels(0);
	foreImg.Allocate(image.Shape());
	foreImg.FillPixels(0);

	for (int iPatch = 0; iPatch < nPatch; iPatch++ )
	{
		CVisGrayByteImage * pPatchMask = GetMaskImg(m_MaskType);
		PutLocalPatchImage(foreImg, maskImg, geomparam[iPatch], PatchHalfSize(), vPatchData[iPatch], pPatchMask);
	}		

	for (int ix = 0 ; ix< maskImg.Width(); ix++)
	{
		for (int iy = 0; iy < maskImg.Height(); iy++ )
		{
			int iPixelBack = image.Pixel(ix,iy);
			int iPixelMask = maskImg.Pixel(ix,iy);
			int iPixelFore = foreImg.Pixel(ix,iy);

			int iPixelDes;
			if (iPixelMask < 255)
			{
				iPixelDes = (iPixelFore * iPixelMask + iPixelBack * (255 - iPixelMask))/ 255;
			}
			else
			{
				iPixelDes = iPixelFore * 255 / iPixelMask;
			}
			
			image.Pixel(ix,iy) = iPixelDes;
		}
	}	
}

void CPatchCurveModel::GetPatchData(const CVisGrayFloatImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, std::vector<CVisDVector>& vPatchData) const
{
	if (geomparam.empty())
	{
		vPatchData.clear();
		return;
	}

	int nPatch = geomparam.size();

	vPatchData.resize(nPatch);
	for (int iPatch = 0; iPatch < nPatch; iPatch++ )
	{
		GetLocalPatchData(image, geomparam[iPatch], PatchHalfSize(), vPatchData[iPatch]);
	}
}

void CPatchCurveModel::PutPatchData(CVisGrayFloatImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisDVector>& vPatchData) const
{
	if (geomparam.empty())
	{
		return;
	}

	int nPatch = geomparam.size();
	for (int iPatch = 0; iPatch < nPatch; iPatch++ )
	{
		PutLocalPatchData(image, geomparam[iPatch], PatchHalfSize(), vPatchData[iPatch]);
	}	
}

void CPatchCurveModel::PutPatchDataWithMask(CVisGrayFloatImage& image, const std::vector<CLocalPatchGeomParameter>& geomparam, const std::vector<CVisDVector>& vPatchData) const
{
	if (geomparam.empty())
	{
		return;
	}

	CVisGrayShortImage maskImg;
	CVisGrayFloatImage foreImg;
	int nPatch = geomparam.size();

	maskImg.Allocate(image.Shape());
	maskImg.FillPixels(0);
	foreImg.Allocate(image.Shape());
	foreImg.FillPixels(0);


	for (int iPatch = 0; iPatch < nPatch; iPatch++ )
	{
		CVisGrayByteImage * pPatchMask = GetMaskImg(m_MaskType);
		PutLocalPatchData(foreImg, maskImg, geomparam[iPatch], PatchHalfSize(), vPatchData[iPatch], pPatchMask);
	}		

	for (int ix = 0 ; ix< maskImg.Width(); ix++)
	{
		for (int iy = 0; iy < maskImg.Height(); iy++ )
		{
			float iPixelBack = image.Pixel(ix,iy);
			int iPixelMask = maskImg.Pixel(ix,iy);
			float iPixelFore = foreImg.Pixel(ix,iy);

			float iPixelDes;
			if (iPixelMask < 255)
			{
				iPixelDes = min(255, (iPixelFore * iPixelMask + iPixelBack * (255 - iPixelMask))/ 255);
			}
			else
			{
				iPixelDes = min(255, (iPixelFore * 255 / iPixelMask));
			}

			image.Pixel(ix,iy) = iPixelDes;
		}
	}	
}

CVisGrayByteImage * CPatchCurveModel::GetMaskImg(int type) const
{
	if ( 0 == type)
	{
		return NULL;
	}
	if ( 1 == type )
	{	
		int iWidth = PatchHalfSize().cx *2 +1;
		int iHeight = PatchHalfSize().cy *2 +1;
	
		CVisGrayByteImage * pMaskImg = new CVisGrayByteImage(iWidth, iHeight);
		pMaskImg->FillPixels(255);
		//return pMaskImg;

		for (int ix = 0; ix < iWidth; ix++ )
		{
			double tempf = fabs(ix - PatchHalfSize().cx);
			tempf /= PatchHalfSize().cx;

			if (tempf > 0.5f)
			{
				tempf = 2.0f - tempf * 2.0f ;
			}
			else
			{
				tempf = 1.0f;
			}

			for (int iy = 0; iy < iHeight; iy++ )
			{
				pMaskImg->Pixel(ix,iy) *= tempf;
			}
		}
		return pMaskImg;
	}

	return NULL;
}

COutputDataFile& operator<<(COutputDataFile& fs, const CPatchCurveModel& m)
{
	return Output(fs, m);
}


CInputDataFile& operator>>(CInputDataFile& fs, CPatchCurveModel& m)
{
	return Input(fs, m);
}

COutputDataFile& Output(COutputDataFile& fs, const CPatchCurveModel& m)
{
	fs << ClassTag("PatchCurveModel") << sep_endl;
	fs << m.m_MaskType << (int)m.m_PatchHalfSize.cx << (int)m.m_PatchHalfSize.cy << sep_endl;
	return fs;
}

CInputDataFile& Input(CInputDataFile& fs, CPatchCurveModel& m)
{
	int cx, cy;
	fs >> ClassTag("PatchCurveModel") >> sep_endl;
	fs >> m.m_MaskType >> cx >> cy >> sep_endl;
	m.m_PatchHalfSize.cx = cx;
	m.m_PatchHalfSize.cy = cy;
	return fs;
}
*/
