// Sketch.cpp: implementation of the CSketch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sketch.h"
//#include "functions.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////// CPatchGeomParameter //////////////////
CPatchGeomParameter::CPatchGeomParameter(double fCx/* =0. */, double fCy/* =0. */, double fTheta/* =0. */, double fHalfSize/* =1. */, double fLength/* =1. */)
: m_fCx(fCx), m_fCy(fCy),
  m_fHalfSize(m_fHalfSize), m_fLength(fLength),
  m_fTheta(fTheta)
{
}

CPatchGeomParameter::CPatchGeomParameter(const CPatchGeomParameter& ref)
{
	m_fCx = ref.m_fCx;
	m_fCy = ref.m_fCy;
	m_fHalfSize = ref.m_fHalfSize;
	m_fLength= ref.m_fLength;
	m_fTheta = ref.m_fTheta;
}

const CPatchGeomParameter& CPatchGeomParameter::operator=(const CPatchGeomParameter& ref)
{
	if(this != &ref) 
	{
		m_fCx = ref.m_fCx;
		m_fCy = ref.m_fCy;
		m_fHalfSize = ref.m_fHalfSize;
		m_fLength= ref.m_fLength;
		m_fTheta = ref.m_fTheta;
	}
	return *this;
}

void CPatchGeomParameter::GetTransform4x4(CVisTransform4x4 &trans)
{
	trans *= GetTrans2DTMatrix(m_fCx, m_fCy);
	trans *= GetTrans2DRSMatrix(1., m_fTheta + MY_PI/2.);
//	trans *= GetTrans2DRSMatrix(1., m_fTheta);
	trans *= GetTrans2DTMatrix(-m_fHalfSize, -m_fLength/2.);
}

void CPatchGeomParameter::GetInversedTransform4x4(CVisTransform4x4 &transInv)
{
	CVisTransform4x4 trans;
	GetTransform4x4(trans);
	transInv = trans.Inverted();
}

////////////////////// CPatchTextureParameter ////////////////////
CPatchTextureParameter::CPatchTextureParameter(int nMethod/* =TEXMETHOD_ORGIGINAL */)
: m_nMethod(nMethod),
  m_fMean(0.), m_fVar(1.),
  m_nLineNum(1)
{
}

CPatchTextureParameter::CPatchTextureParameter(const CPatchTextureParameter& ref)
{
	m_nMethod = ref.m_nMethod;
	m_nLineNum = ref.m_nLineNum;
	m_fVar = ref.m_fVar;
	m_fMean = ref.m_fMean;

	if ( ref.m_data.Length()>0 )
	{
		m_data.Resize(ref.m_data.Length());
		m_data = ref.m_data;
	}
}

const CPatchTextureParameter& CPatchTextureParameter::operator=(const CPatchTextureParameter& ref)
{
	if ( this != &ref )
	{
		m_nMethod = ref.m_nMethod;
		m_nLineNum = ref.m_nLineNum;
		m_fVar = ref.m_fVar;
		m_fMean = ref.m_fMean;
		
		if ( ref.m_data.Length()>0 )
		{
			m_data.Resize(ref.m_data.Length());
			m_data = ref.m_data;
		}
	}
	return *this;
}

/////////////////// CSketchPatch ////////////////////

CSketchPatch::CSketchPatch()
{
	m_iType = 0;
	m_dWeight = 0.;
}

CSketchPatch::CSketchPatch(int iType, CPatchGeomParameter &geomParam, double weight)
{
	m_iType = iType;
	m_geomParam = geomParam;
	m_dWeight = weight;
}

CSketchPatch::~CSketchPatch()
{
}

CPoint2 CSketchPatch::GetPoint()
{
	CPoint2 p2;
	p2.X = m_geomParam.m_fCx;
	p2.Y = m_geomParam.m_fCy;

	return p2;
}

void CSketchPatch::SetPoint(CPoint2& p2)
{
	m_geomParam.m_fCx = p2.X;
	m_geomParam.m_fCy = p2.Y;
}

const CSketchPatch& CSketchPatch::operator=(const CSketchPatch& ref)
{
	if(this != &ref) {
		m_geomParam = ref.m_geomParam;
		m_dWeight = ref.m_dWeight;
		m_iType = ref.m_iType;
	}
	return *this;
}

double CSketchPatch::EndDistTo(CSketchPatch &patch)
{
	CPoint2 endP, startP;
	endP.X = m_geomParam.m_fCx + 0.5*m_geomParam.m_fLength*cos(m_geomParam.m_fTheta);
	endP.Y = m_geomParam.m_fCy + 0.5*m_geomParam.m_fLength*sin(m_geomParam.m_fTheta);

	startP.X = patch.X() - 0.5*patch.Length()*cos(patch.Theta());
	startP.Y = patch.Y() - 0.5*patch.Length()*sin(patch.Theta());

	return sqrt(SQUARE(endP.X-startP.X) + SQUARE(endP.Y-startP.Y));
}

double CSketchPatch::CenterDistTo(CSketchPatch &patch)
{
	return sqrt(SQUARE(patch.m_geomParam.m_fCx-m_geomParam.m_fCx) + 
		        SQUARE(patch.m_geomParam.m_fCy-m_geomParam.m_fCy));
}

double CSketchPatch::AngleToPatch(CSketchPatch &patch)
{
//	return Line_Angle_0_2PI(patch.Theta(), Theta());
	return 0.;
}

COutputDataFile& operator<<(COutputDataFile& fs, const CSketchPatch &sketchPatch)
{
	return Output(fs, sketchPatch);
}

CInputDataFile& operator>>(CInputDataFile& fs, CSketchPatch &sketchPatch)
{
	return Input(fs, sketchPatch);
}

COutputDataFile& Output(COutputDataFile& fs, const CSketchPatch &sketchPatch)
{
//	fs << ClassTag("[Patch]") << sep_endl;
	fs << sketchPatch.m_iType << sketchPatch.m_dWeight
	   << sketchPatch.m_geomParam.m_fCx << sketchPatch.m_geomParam.m_fCy << sketchPatch.m_geomParam.m_fTheta 
	   << sketchPatch.m_geomParam.m_fHalfSize << sketchPatch.m_geomParam.m_fLength;
	
	return fs;
}

CInputDataFile& Input(CInputDataFile& fs, CSketchPatch &sketchPatch)
{

//	fs >> ClassTag("[Patch]") >> sep_endl;
	fs >> sketchPatch.m_iType >> sketchPatch.m_dWeight
	   >> sketchPatch.m_geomParam.m_fCx >> sketchPatch.m_geomParam.m_fCy >> sketchPatch.m_geomParam.m_fTheta
	   >> sketchPatch.m_geomParam.m_fHalfSize
	   >> sketchPatch.m_geomParam.m_fLength;
	
	return fs;
}

///////////////////////////// CSketch ///////////////////////////
CSketch::CSketch()
{
	Clear();
}

CSketch::CSketch(const CSketch &sketch)
{
	Clear();
	*this = sketch;
}

CSketch::CSketch(std::vector<CSketchPatch> &vPatch, int iType/* =0 */, double fWeight/* =1. */)
{
	Clear();
}

CSketch::~CSketch()
{
	Clear();
}

void CSketch::Clear()
{
	m_dWeight = 1.;
	m_iType = 0;

	if ( size() <= 0 )
		return;

	for (int i=0; i<size(); i++)
	{
		if ( NULL != (*this)[i] )
			delete (*this)[i];
	}
	clear();
}

const CSketch& CSketch::operator=(const CSketch& s)
{
	if ( this!=&s )
	{
		Resize(s.NPatchNum());
		for (int i=0; i<s.NPatchNum(); i++)
		{
			*((*this)[i]) = *(s[i]);
		}
		
		m_dWeight = s.m_dWeight;
		m_iType = s.m_iType;
	}

	return *this;
}

void CSketch::Resize(int nPatchNum)
{
	clear();

	if ( nPatchNum<=0 ) 
		return;

	resize(nPatchNum);
	for (int i=0; i<nPatchNum; i++)
	{
		(*this)[i] = new CSketchPatch;
	}
}

bool CSketch::Insert(int iPatch, CSketchPatch &patch)
{
	if ( iPatch<0 || iPatch>NPatchNum() )
		return false;


	CSketchPatch *pNewPatch = new CSketchPatch;
	*pNewPatch = patch;
	insert(begin()+iPatch, pNewPatch);

	return true;
}

bool CSketch::Delete(int iPatch)
{
	if ( iPatch<0 || iPatch>=NPatchNum() )
		return false;

	delete (*this)[iPatch];
	erase(begin()+iPatch);

	return true;
}

CSketchPatch* CSketch::GetPatchPtr(int iPatch)
{
	if ( iPatch<0 || iPatch>=NPatchNum() )
		return NULL;

	return (*this)[iPatch];
}

CPoint2 CSketch::GetPoint(int iPatch)
{
	CPoint2 p2;
	if (iPatch<0 || iPatch>=NPatchNum()) return p2;
	return (*this)[iPatch]->GetPoint();
}

void CSketch::SetPoint(int iPatch, CPoint2& p2)
{
	if (iPatch<0 || iPatch>=NPatchNum()) return;
	(*this)[iPatch]->SetPoint(p2);
}
/*
double& CSketch::PointX(int iPatch)
{
	if (iPatch<0 || iPatch>=NPatchNum()) return 0.;
	return (*this)[iPatch]->X();
}

double& CSketch::PointY(int iPatch)
{
	if (iPatch<0 || iPatch>=NPatchNum()) return 0.;
	return (*this)[iPatch]->Y();
}

double& CSketch::Theta(int iPatch)
{
	if (iPatch<0 || iPatch>=NPatchNum()) return 0.;
	return (*this)[iPatch]->Theta();
}

double& CSketch::HalfSize(int iPatch)
{
	if (iPatch<0 || iPatch>=NPatchNum()) return 0.;
	return (*this)[iPatch]->HalfSize();
}

double& CSketch::Width(int iPatch)
{
	if (iPatch<0 || iPatch>=NPatchNum()) return 0.;
	return (*this)[iPatch]->Width();
}

double& CSketch::Length(int iPatch)
{
	if (iPatch<0 || iPatch>=NPatchNum()) return 0.;
	return (*this)[iPatch]->Length();
}
*/
COutputDataFile& operator<<(COutputDataFile& fs, const CSketch &sketch)
{
	return Output(fs, sketch);
}

CInputDataFile& operator>>(CInputDataFile& fs, CSketch &sketch)
{
	return Input(fs, sketch);
}

COutputDataFile& Output(COutputDataFile& fs, const CSketch &sketch)
{
//	fs << ClassTag("[Sketch]") << sep_endl;
	fs << sketch.m_iType << sketch.m_dWeight << sketch.NPatchNum();
	
	for (int i=0; i<sketch.NPatchNum(); i++)
	{
		fs << *(const_cast<CSketch&>(sketch).GetPatchPtr(i)); 
	}

	return fs;
}

CInputDataFile& Input(CInputDataFile& fs, CSketch &sketch)
{
	int nSize;
//	fs >> ClassTag("[Sketch]") >> sep_endl;
	fs >> sketch.m_iType >> sketch.m_dWeight >> nSize ;

	sketch.Resize(nSize);
	for (int i=0; i<nSize; i++)
	{
		fs >> *(sketch.GetPatchPtr(i)); 
	}

	return fs;
}

//////////////////////////// CSketchSet ///////////////////////////
CSketchSet::CSketchSet()
{
	m_iType = 0;
	Clear();
}

CSketchSet::~CSketchSet()
{
	Clear();
}

void CSketchSet::Clear()
{
	for (int i=0; i<size(); i++)
	{
		if ( (*this)[i] != NULL )
			delete (*this)[i];
	}
	clear();
}

bool CSketchSet::Insert(int iSketch, CSketch& sketch)
{
	if ( iSketch<0 || iSketch>NSketchNum() )
		return false;
	
	CSketch* pNewSketch = new CSketch;
	*pNewSketch = sketch;

	insert(begin()+iSketch, pNewSketch);

	return true;
}

const CSketchSet& CSketchSet::operator=(const CSketchSet& ss)
{
	if ( this!=&ss )
	{
		m_iType = ss.m_iType;
		Resize(ss.NSketchNum());
		for (int i=0; i<ss.NSketchNum(); i++)
		{
			*((*this)[i]) = *(ss[i]);
		}
	}

	return *this;
}

const int CSketchSet::NPatchNum() const
{
	int nTotalPatch = 0;

	for (int iS=0; iS<NSketchNum(); iS++)
	{
		nTotalPatch += (*this)[iS]->NPatchNum();
	}

	return nTotalPatch;
}

bool CSketchSet::Delete(int iSketch)
{
	if ( iSketch<0 || iSketch>NSketchNum() )
		return false;

	delete (*this)[iSketch];
	erase(begin()+iSketch);

	return true;
}

void CSketchSet::Resize(int nSketchNum)
{
	clear();

	if ( nSketchNum<=0 ) 
		return;

	resize(nSketchNum);
	for (int i=0; i<nSketchNum; i++)
	{
		(*this)[i] = new CSketch;
	}
}

CSketch* CSketchSet::GetSketchPtr(int iSketch)
{
	if ( iSketch<0 || iSketch>NSketchNum() )
		return NULL;

	return (*this)[iSketch];
}

COutputDataFile& operator<<(COutputDataFile& fs, const CSketchSet &ss)
{
	return Output(fs, ss);
}

CInputDataFile& operator>>(CInputDataFile& fs, CSketchSet &ss)
{
	return Input(fs, ss);
}

COutputDataFile& Output(COutputDataFile& fs, const CSketchSet &ss)
{
//	fs << ClassTag("[SketchSet]") << sep_endl;
	fs << const_cast<CSketchSet&>(ss).GetType() << ss.NSketchNum();	
	for (int i=0; i<ss.NSketchNum(); i++)
	{
		fs << *(const_cast<CSketchSet&>(ss).GetSketchPtr(i)); 
	}

	return fs;
}

CInputDataFile& Input(CInputDataFile& fs, CSketchSet &ss)
{
	int nSize;
//	fs >> ClassTag("[SketchSet]") >> sep_endl;
	fs >>  ss.GetType() >> nSize;
	ss.Resize(nSize);
	for (int i=0; i<nSize; i++)
	{
		fs >> *(ss.GetSketchPtr(i)); 
	}

	return fs;
}

///////////////////////// other functions ////////////////////////
void Sketch2Shape(CSketch &sketch, CShape& shape)
{
	if ( sketch.NPatchNum() <= 0 )
	{
		shape.Resize(0);
		return;
	}

	shape.Resize(sketch.NPatchNum());
	for (int iPoint=0; iPoint<shape.PointNum(); iPoint++)
	{
		shape.SetPoint(iPoint, sketch.GetPoint(iPoint));
	}
}

void Shape2Sketch(CShape &shape, CSketch& sketch)
{	
	sketch.Clear();

	if ( shape.PointNum() <= 0 ) return;

	sketch.Resize(shape.PointNum());
	for (int iPoint=0; iPoint<sketch.NPatchNum(); iPoint++)
	{
		sketch.SetPoint(iPoint, shape.GetPoint(iPoint));
	}
}

void SketchSet2ShapeSet(CSketchSet &sketchset, CShapeSet &shapeset)
{		
	shapeset.clear();

	if ( sketchset.NSketchNum() <= 0 ) return;

	shapeset.resize(sketchset.NSketchNum());
	for (int iSketch=0; iSketch<shapeset.size(); iSketch++)
	{
		Sketch2Shape(*(sketchset.GetSketchPtr(iSketch)), shapeset[iSketch]);
	}
}

void ShapeSet2SketchSet(CShapeSet &shapeset, CSketchSet &sketchset)
{
	if ( shapeset.size() <= 0 )
	{
		sketchset.Clear();
		return;
	}

	sketchset.Resize(shapeset.size());
	for (int iSketch=0; iSketch<sketchset.NSketchNum(); iSketch++)
	{
		Shape2Sketch(shapeset[iSketch], *(sketchset.GetSketchPtr(iSketch)));
	}
}

void Shape2SketchSet(CShape &shape, CSketchSet &ss, const CConnectedModel* pConModel/* =NULL */)
{		
	ss.Clear();

	if ( shape.PointNum() <= 0 ) return;

	if ( pConModel==NULL || pConModel->PointNum()>shape.PointNum() )
	{
		ss.Resize(1);
		Shape2Sketch(shape, *ss.GetSketchPtr(0));
	}
	else 
	{
		ss.Resize(pConModel->ContourNum());
		for ( int i=0; i<pConModel->ContourNum(); i++ )
		{
			ss.GetSketchPtr(i)->Resize(pConModel->Contour(i).PointNum());
			for ( int j=0; j<pConModel->Contour(i).PointNum(); j++ )
			{
				ss.GetSketchPtr(i)->SetPoint( j, shape.GetPoint( pConModel->Contour(i).Index(j) ) );
			}
		}
	}
}

void SketchSet2Shape(CSketchSet &ss, CShape &shape, CConnectedModel& conModel)
{
	conModel.Clear();
	shape.Clear();
	if ( ss.NSketchNum()<=0 ) return;

	shape.Resize(ss.NPatchNum());
	int index = 0;
	for ( int i=0; i<ss.NSketchNum(); i++ )
	{
		CIndexedContour c(ss.GetSketchPtr(i)->GetType(), ss.GetSketchPtr(i)->NPatchNum());
		for ( int j=0; j<ss.GetSketchPtr(i)->NPatchNum(); j++ )
		{
			shape.SetPoint(index, ss.GetSketchPtr(i)->GetPoint(j));
			c.SetIndex(j, index++);
		}
		conModel.AddNewContour(c);
	}
}