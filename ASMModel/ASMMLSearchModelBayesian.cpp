
#include "stdafx.h"
#include "ASMModel.h"
#include "asmmlsearchmodelbayesian.h"


#ifdef HCHEN_DEBUG
int g_iCount = 0; // The index of the shapes of each steps.
void WriteShape(CShape& shape)
{
	char fname[1024];
	sprintf(fname,"c:\\temp\\Shape_%04d.pts",g_iCount);
	g_iCount++;
	shape.WriteFile(fname,_O_TEXT);
}

#endif

CASMMLSearchModelBayesian::CASMMLSearchModelBayesian() : CASMML2DSearchModel()
{
	// from base class
    m_pImagePyramid = NULL;
    m_pSearchImg = NULL;
/*  SetModelParam();
    SetSearchParam();
    m_LoacalStructType = 0;
    m_iCurLevel = 0;
    m_iCurLevelScale = 1;
    m_bNeedConstrain = false;
*/
	// current class
	m_fTangentConstraint = true;
	m_fUseEM = true;
	m_fInhomogenousLikelihood = true;
	m_ConstrainVar = 4; 
	m_fScaleX = 1.0;
	m_fScaleY = 1.0;
	m_iSize = 512;
}

void CASMMLSearchModelBayesian::GetAvgShape(CShape& AvgShape)
{	
    int iNbEigen = m_PCA2Model.NbEigen();
	
	CVisDVector eigenparam(iNbEigen);  
	eigenparam = 0.;
	AvgShape = m_AvgShape;

    if (!m_ModelWithoutAlign)
    {
		GenerateShapeWithPos(m_TxInTraining, m_TyInTraining, m_SInTraining, m_ThetaInTraining, eigenparam, AvgShape);
    }
    else
    {
		GenerateShape(eigenparam, AvgShape);
    }
}

void CASMMLSearchModelBayesian::SetAvgShape(void)
{
	CASMMLSearchModel::SetAvgShape();
	m_RefShape = m_AvgShape;
}

void CASMMLSearchModelBayesian::SetShape(const CShape& initshape)
{
	CASMMLSearchModel::SetShape(initshape);
	m_RefShape = m_AvgShape;
}


CASMMLSearchModelBayesian::~CASMMLSearchModelBayesian()
{

}

void CASMMLSearchModelBayesian::UpdateShapeWithModel()
{
	if ( 0 == m_SearchMode )
	{
		UpdateModelICM();
	}
	else if ( 1 == m_SearchMode )
	{
		UpdateModelBayesian();
	}
	else if ( 2 == m_SearchMode )
	{
		UpdateModelNMF();
	}
}

bool CASMMLSearchModelBayesian::UpdateShapeWithImage()
{
	return UpdateShapeImage();
}

bool CASMMLSearchModelBayesian::UpdateShapeImage()
{
	int iNbPoints;
	iNbPoints = m_AvgShape.PointNum();
	GetShapeNormal();
	int iConvergePoints=0;

	double m_UpdateDev=0;
	m_vDev.Resize(iNbPoints);
	double temp=(m_iCurLevelScale/2.0)*(m_iCurLevelScale/2.0);

	for (int iPoint=0;iPoint<iNbPoints;iPoint++)
	{
		double desx,desy;
		bool bConverge;
    	//bConverge = SearchOnePointBayesian(iPoint ,desx,desy);
		bConverge = SearchOnePoint(iPoint ,desx,desy);
		if (bConverge)
			iConvergePoints++;

		m_UpdateDev += (m_SearchShape.PointX(iPoint)-desx)*(m_SearchShape.PointX(iPoint)-desx);
		m_UpdateDev += (m_SearchShape.PointY(iPoint)-desy)*(m_SearchShape.PointY(iPoint)-desy);
		m_vDev[iPoint]=(m_SearchShape.PointX(iPoint)-desx)*(m_SearchShape.PointX(iPoint)-desx)/2
			+(m_SearchShape.PointY(iPoint)-desy)*(m_SearchShape.PointY(iPoint)-desy)/2;

		if (m_vDev[iPoint]<temp)
			m_vDev[iPoint]=temp;

		m_SearchShape.PointX(iPoint)=desx;
		m_SearchShape.PointY(iPoint)=desy;
	}

	m_UpdateDev /= (2*iNbPoints);

	if (m_bNeedConstrain)
	{
		m_fInhomogenousLikelihood = true;
	}

	if (m_fInhomogenousLikelihood)
	{
		m_fTangentConstraint = false;
		m_fUseEM = false;
		m_vDev = m_UpdateDev;
	}
	else
	{
		m_vDev = m_UpdateDev;
	}
	
	// Set Constraint
	if (m_bNeedConstrain)
	{
		// Restore constraint points
		for (int iPoint=0; iPoint<m_SearchShape.PointNum(); iPoint++)
		{
			if (m_bConstrained[iPoint])
			{
				m_SearchShape.PointX(iPoint) = m_ConstrainShape.PointX(iPoint);
				m_SearchShape.PointY(iPoint) = m_ConstrainShape.PointY(iPoint);
				m_vDev[iPoint] = temp;
			}
		}
	}

	if (m_UpdateDev<0.0001)
		return true;
	else
		return false;
}

void CASMMLSearchModelBayesian::UpdateModelICM()
{
	CShape& refShape = m_RefShape;
	refShape = m_AvgShape;	// initial refernce shape
	//GenerateShape(m_EigenParam,refShape);
	
	double dLogP = m_ShapeModel.MAPEstimate(m_SearchShape, m_vDev , m_Tx, m_Ty, m_S, m_Theta, refShape);
	GetShapeParam(m_EigenParam, refShape);
	m_ShapeModel.ApplyShapePos(m_Tx, m_Ty, m_S, m_Theta, m_SearchShape, refShape);
}

void CASMMLSearchModelBayesian::UpdateModelNMF()
{	
	// EM algorithm to solve the parameters
	bool Converge = false;
	int MaxStep = 20;
	
	CShape& refshape = m_RefShape;
	refshape = m_AvgShape;	// initial refernce shape
	GenerateShape(m_NMFEncoding, refshape, eNMF);
	double norm=0;
	
	int i;
	if (m_fInhomogenousLikelihood)
	{
		for (i=0;i<refshape.PointNum();i++)
			norm += (refshape.PointX(i)*refshape.PointX(i)+refshape.PointY(i)*refshape.PointY(i))/m_vDev[i];
	}
	else
	{
		for (i=0;i<refshape.PointNum();i++)
			norm += (refshape.PointX(i)*refshape.PointX(i)+refshape.PointY(i)*refshape.PointY(i));
	}
	
	//norm=refshape.Norm();
	for (i=0; i<MaxStep; i++)
	{
		Converge = GetShapeAndPosNMF(refshape, norm);
		GetRefShapeNMF(refshape, norm);
		if (Converge) break;
	}
	GetShapeAndPosNMF(refshape, norm);
	GenerateShapeWithPos(m_Tx, m_Ty, m_S, m_Theta, m_NMFEncoding, m_SearchShape, eNMF);
	//GenerateShapeFromRef(m_Tx,m_Ty,m_S,m_Theta,m_SearchShape, refshape);
}

void CASMMLSearchModelBayesian::UpdateModelBayesian()
{
	int iNbEigen = SetEigenNumber();

	// EM algorithm to solve the parameters
	bool Converge=false;
	int MaxStep = 20;

	CShape& refshape = m_RefShape;
	refshape = m_AvgShape;	// initial refernce shape
	GenerateShape(m_EigenParam, refshape);
	double norm=0;
	
	int i;
	if (m_fInhomogenousLikelihood)
	{
		for (i=0;i<refshape.PointNum();i++)
			norm += (refshape.PointX(i)*refshape.PointX(i)+refshape.PointY(i)*refshape.PointY(i))/m_vDev[i];
	}
	else
	{
		for (i=0;i<refshape.PointNum();i++)
			norm += (refshape.PointX(i)*refshape.PointX(i)+refshape.PointY(i)*refshape.PointY(i));
	}

	//norm=refshape.Norm();
 	for (i=0;i<MaxStep;i++)
	{
		Converge = GetShapeAndPosBayesian(iNbEigen,refshape,norm);
		GetRefShapeBayesian(refshape,norm,iNbEigen);
		if (Converge) break;
	}
	GetShapeAndPosBayesian(iNbEigen,refshape,norm);
	GenerateShapeWithPos(m_Tx,m_Ty,m_S,m_Theta,m_EigenParam,m_SearchShape);
	//GenerateShapeFromRef(m_Tx,m_Ty,m_S,m_Theta,m_SearchShape, refshape);
}

bool CASMMLSearchModelBayesian::SearchOneStepBayesian()
{	
	bool bConverge;
	bConverge = UpdateShapeImage();
	CShape OldShape;
	OldShape = m_SearchShape;

	UpdateShapeWithModel();

	return bConverge;
}

bool CASMMLSearchModelBayesian::SearchOneLevelBayesian()
{
	bool bConverge;
	int iSteps, TotSteps;

	bConverge = false;
	iSteps=0;
	TotSteps = m_iMaxStepPerLevel;
	if (m_iCurLevel == m_iTotLevel-1)
		TotSteps *= 2;
	
	while ((!bConverge)&&(iSteps<TotSteps))
	{
		bConverge = SearchOneStepBayesian();
		iSteps++;
	}

	return bConverge;
}


void CASMMLSearchModelBayesian::InitPosParam()
{
	m_Tx = m_TxInTraining;
	m_Ty = m_TyInTraining;
	m_S = m_SInTraining;
	m_Theta = m_ThetaInTraining;
}
		
                                   
void CASMMLSearchModelBayesian::InitShapeParam()
{
	m_EigenParam.Resize(SModel().NbEigen());
	for (int i=0;i<SModel().NbEigen();i++)
		m_EigenParam[i] = 0;

	m_NMFEncoding.Resize(m_NMFModel.NbBasis());
	m_NMFEncoding = 0.;

	m_RefShape = m_AvgShape;	
}

// SET NUMBER OF EIGENS USED IN MODEL 
int CASMMLSearchModelBayesian::SetEigenNumber()
{
	return SModel().NbEigen();

	int iNbEigen;
	{
		switch (m_iCurLevel)
		{
		case 3:
			iNbEigen = 5; break;
		case 2:
			iNbEigen = 40; break;
		case 1:
			iNbEigen = 90; break;
		case 0:
		default:
			iNbEigen = 162; break;
		}
	}
	iNbEigen = min(iNbEigen,SModel().NbEigen());
	return iNbEigen;
}

bool CASMMLSearchModelBayesian::GetShapeAndPosNMF(CShape& refshape, double norm)
{
	int i, iNbPoint = m_SearchShape.PointNum();
	CVisDVector encoding0 = m_NMFEncoding;
	
	double Tx0,Ty0,s0,theta0;
	Tx0=m_Tx; Ty0=m_Ty; s0=m_S; theta0=m_Theta;

	// errors in tangent shape model
	double errs = m_NMFModel.ErrS();
	m_NMFModel.Encode(refshape.Points(), m_NMFEncoding);

	// get pose parameter
	double a,b, c,d;
	if (!m_fInhomogenousLikelihood)
	{
		double S11,S12,S21,S22;
		S11=S12=S21=S22=m_Tx=m_Ty=0;
		for (i=0;i<iNbPoint;i++)
		{
			double x1,x2,y1,y2;
		
			x1 = refshape.PointX(i); x2 = refshape.PointY(i);
			y1 = m_SearchShape.PointX(i); y2 = m_SearchShape.PointY(i);
		
			m_Tx += y1 ; m_Ty += y2 ;
			
			S11 += x1*y1; S22 += x2*y2;
			S12 += x1*y2; S21 += x2*y1;
		} 
		m_Tx /= iNbPoint; m_Ty /= iNbPoint;	// translation
		if (norm==0) return false;
		a = (S11+S22)/norm; b =(S21-S12)/norm;
		c = m_Tx; d = m_Ty;
	}
	else
	{
		CVisDMatrix AMat(refshape.PointNum()*2,4);
		CVisDVector B(refshape.PointNum()*2);
		B = m_SearchShape.Points();
		for (i=0; i<refshape.PointNum(); i++)
		{
			AMat[i*2][0] = refshape.PointX(i);
			AMat[i*2+1][0] = refshape.PointY(i);
			AMat[i*2][1] = -refshape.PointY(i);
			AMat[i*2+1][1] = refshape.PointX(i);
			AMat[i*2][2] = 1;
			AMat[i*2+1][2] = 0;
			AMat[i*2][3] = 0;
			AMat[i*2+1][3] = 1;
		}
		
		for (i=0; i<refshape.PointNum(); i++)
		{
			double InvDev = (1/m_vDev[i]);
			AMat[i*2][0] *= InvDev;
			AMat[i*2+1][0] *= InvDev;
			AMat[i*2][1] *= InvDev;
			AMat[i*2+1][1] *= InvDev;
			AMat[i*2][2] *= InvDev;
			AMat[i*2+1][2] *= InvDev;
			AMat[i*2][3] *= InvDev;
			AMat[i*2+1][3] *= InvDev;
			B[i*2] *= InvDev;
			B[i*2+1] *= InvDev;
		}
		
		CVisDVector X;
		X.Resize(4);
		VisDMatrixLeastSquares(AMat,B,X);
		a = X[0]; b= - X[1]; c=X[2]; d=X[3];
	}
	
	m_Tx = c; m_Ty = d;
	m_S = sqrt(a*a+b*b);	// scale
	m_Theta = - atan2(b,a);	// rotation
	
	//  Converge Check ( update deviation is less than a theshold )
	double temp;
	temp = fabs(m_Tx-Tx0)/Tx0 + fabs(m_Ty-Ty0)/Ty0 + fabs(m_S-s0)/s0 + fabs(m_Theta-theta0)/theta0;
	for (i=0; i<m_NMFEncoding.Length(); i++)
    {
		temp += fabs(m_NMFEncoding[i]-encoding0[i]);
    }
	if (temp<0.0001)
		return true;
	else 
		return false;	
}

// BASIC FUNCTION IN EM PARAMETER UPDATION
bool CASMMLSearchModelBayesian::GetShapeAndPosBayesian(int iNbEigen,CShape& refshape,double norm)
{
	int iNbPoint=m_SearchShape.PointNum();
	CVisDVector eigenparam0=m_EigenParam;
	double Tx0,Ty0,s0,theta0;
	Tx0=m_Tx; Ty0=m_Ty; s0=m_S; theta0=m_Theta;

	// errors in tangent shape model
	double errs=0;
	for (int i=iNbEigen;i<SModel().NbEigen();i++)
		errs += SModel().EigenVal()[i];
	errs /= (2*iNbPoint-4);
	errs += SModel().ErrS();

	// get eigen parameter
	for (i=0;i<iNbEigen;i++)
	{	
		m_EigenParam[i] = SModel().EigenVec().Row(i) * refshape.Points();
		m_EigenParam[i] *= SModel().EigenVal()[i]/(SModel().EigenVal()[i]+errs);
	}
	for (i=iNbEigen; i<m_EigenParam.Length(); i++)
		m_EigenParam[i] = 0;

	// get pose parameter
	double a,b, c,d;
	if (!m_fInhomogenousLikelihood)
	{
		double S11,S12,S21,S22;
		S11=S12=S21=S22=m_Tx=m_Ty=0;
		for (i=0;i<iNbPoint;i++)
		{
			double x1,x2,y1,y2;
		
			x1 = refshape.PointX(i); x2 = refshape.PointY(i);
			y1 = m_SearchShape.PointX(i); y2 = m_SearchShape.PointY(i);
		
			m_Tx += y1 ; m_Ty += y2 ;
			
			S11 += x1*y1; S22 += x2*y2;
			S12 += x1*y2; S21 += x2*y1;
		} 
		m_Tx /= iNbPoint; m_Ty /= iNbPoint;	// translation
		if (norm==0) return false;
		a = (S11+S22)/norm; b =(S21-S12)/norm;
		c = m_Tx; d = m_Ty;
	}
	else
	{
		CVisDMatrix AMat(refshape.PointNum()*2,4);
		CVisDVector B(refshape.PointNum()*2);
		B = m_SearchShape.Points();
		for (i=0; i<refshape.PointNum(); i++)
		{
			AMat[i*2][0] = refshape.PointX(i);
			AMat[i*2+1][0] = refshape.PointY(i);
			AMat[i*2][1] = -refshape.PointY(i);
			AMat[i*2+1][1] = refshape.PointX(i);
			AMat[i*2][2] = 1;
			AMat[i*2+1][2] = 0;
			AMat[i*2][3] = 0;
			AMat[i*2+1][3] = 1;
		}

		for (i=0; i<refshape.PointNum(); i++)
		{
			double InvDev = (1/m_vDev[i]);
			AMat[i*2][0] *= InvDev;
			AMat[i*2+1][0] *= InvDev;
			AMat[i*2][1] *= InvDev;
			AMat[i*2+1][1] *= InvDev;
			AMat[i*2][2] *= InvDev;
			AMat[i*2+1][2] *= InvDev;
			AMat[i*2][3] *= InvDev;
			AMat[i*2+1][3] *= InvDev;
			B[i*2] *= InvDev;
			B[i*2+1] *= InvDev;
		}

		CVisDVector X;
		X.Resize(4);
		VisDMatrixLeastSquares(AMat,B,X);
		a = X[0]; b= - X[1]; c=X[2]; d=X[3];
	}

	m_Tx = c; m_Ty = d;
	m_S = sqrt(a*a+b*b);	// scale
	m_Theta = - atan2(b,a);	// rotation

	//  Converge Check ( update deviation is less than a theshold )
	double temp;
	temp = fabs(m_Tx-Tx0)/Tx0 + fabs(m_Ty-Ty0)/Ty0 + fabs(m_S-s0)/s0 + fabs(m_Theta-theta0)/theta0;
	for (i=0;i<iNbEigen;i++)
    {
		temp += fabs(m_EigenParam[i]-eigenparam0[i])/eigenparam0[i];
    }
	if (temp<0.0001)
		return true;
	else 
		return false;
}

void CASMMLSearchModelBayesian::GetRefShapeNMF(CShape& refshape, double& norm)
{
	int i, iNbPoint = m_SearchShape.PointNum();
	// inverse of m_SearchShape
	CShape invshape=m_SearchShape;
	invshape.Translate(-m_Tx,-m_Ty);
	invshape.Scale(1/m_S);
	invshape.Rotation(-m_Theta);
	
	// errors in tangent shape model

	double errs = m_NMFModel.ErrS();

	// Inhomogenous noEM noTangent
	if (m_fInhomogenousLikelihood)
	{
		CShape reconstructShape;
		m_NMFModel.Reconstruct(m_NMFEncoding, reconstructShape.Points());
		reconstructShape.Points() += m_AvgShape.Points();
		for (i=0; i<refshape.PointNum(); i++)
		{
			double delta = m_vDev[i]/(m_S*m_S);
			double p=errs/(errs+delta);
			refshape.PointX(i) = reconstructShape.PointX(i) * (1-p) + invshape.PointX(i) * p;
			refshape.PointY(i) = reconstructShape.PointY(i) * (1-p) + invshape.PointY(i) * p;
		}	
		refshape.AlignTangent(m_AvgShape);
		norm =0;
		for (i=0;i<refshape.PointNum();i++)
			norm += (refshape.PointX(i)*refshape.PointX(i)+refshape.PointY(i)*refshape.PointY(i))/m_vDev[i];
	}
	else
	{
		// linear constrain on tangent space
		CVisDMatrix A;
		A.Resize(4,2*iNbPoint);
		CVisDMatrix B;
		B.Resize(2*iNbPoint,4);
		
		A=m_TangentConstraint;
		B=A.Transposed();
		
		// the reference shape
		double delta = m_vDev[0]/(m_S*m_S);
		double p=errs/(errs+delta);
		
		m_NMFModel.Reconstruct(m_NMFEncoding, refshape.Points());
		refshape.Points() += m_AvgShape.Points();
		refshape.Points() *= (1-p);
		refshape.Points() += invshape.Points() * p;
		
		if (m_fTangentConstraint)
		{
			refshape.Points() = refshape.Points() - B * (A * refshape.Points()) + m_AvgShape.Points();
			if (m_fUseEM)
				norm = refshape.Norm2()+(2.0*iNbPoint-4)/(1/errs+1/delta);
			else
				norm = refshape.Norm2();
		}
		else
		{
			if (m_fUseEM)
				norm = refshape.Norm2() + (2.0*iNbPoint)/(1/errs+1/delta);
			else
				norm = refshape.Norm2();
		}
	}
}

void CASMMLSearchModelBayesian::GetRefShapeBayesian(CShape& refshape,double& norm,int iNbEigen)
{
	int iNbPoint=m_SearchShape.PointNum();
	// inverse of m_SearchShape
	CShape invshape=m_SearchShape;
	invshape.Translate(-m_Tx,-m_Ty);
	invshape.Scale(1/m_S);
	invshape.Rotation(-m_Theta);
	
	// errors in tangent shape model
	double errs=0;
	for(int i=iNbEigen;i<SModel().NbEigen();i++)
		errs += SModel().EigenVal()[i];
	errs /= (2*iNbPoint-4);
	errs += SModel().ErrS();

	// Inhomogenous noEM noTangent
	if (m_fInhomogenousLikelihood)
	{
		CShape reconstructShape = m_AvgShape;
		for (i=0;i<iNbEigen;i++)
		{
			reconstructShape.Points() += SModel().EigenVec().Row(i) * m_EigenParam[i];
		}
		for (i=0; i<refshape.PointNum(); i++)
		{
			double delta = m_vDev[i]/(m_S*m_S);
			double p=errs/(errs+delta);
			refshape.PointX(i) = reconstructShape.PointX(i) * (1-p) + invshape.PointX(i) * p;
			refshape.PointY(i) = reconstructShape.PointY(i) * (1-p) + invshape.PointY(i) * p;
		}	
		refshape.AlignTangent(m_AvgShape);
		norm =0;
		for (i=0;i<refshape.PointNum();i++)
			norm += (refshape.PointX(i)*refshape.PointX(i)+refshape.PointY(i)*refshape.PointY(i))/m_vDev[i];
	}
	else
	{
		// linear constrain on tangent space
		CVisDMatrix A;
		A.Resize(4,2*iNbPoint);
		CVisDMatrix B;
		B.Resize(2*iNbPoint,4);
		
		A=m_TangentConstraint;
		B=A.Transposed();
		
		// the reference shape
		double delta = m_vDev[0]/(m_S*m_S);
		double p=errs/(errs+delta);
		
		refshape = m_AvgShape;
		for (i=0;i<iNbEigen;i++)
		{
			refshape.Points() += SModel().EigenVec().Row(i) * m_EigenParam[i];
		}
		refshape.Points() *= (1-p);
		refshape.Points() += invshape.Points() * p;
		
		if (m_fTangentConstraint)
		{
			refshape.Points() = refshape.Points() - B * (A * refshape.Points()) + m_AvgShape.Points();
			if (m_fUseEM)
				norm = refshape.Norm2()+(2.0*iNbPoint-4)/(1/errs+1/delta);
			else
				norm = refshape.Norm2();
		}
		else
		{
			if (m_fUseEM)
				norm = refshape.Norm2() + (2.0*iNbPoint)/(1/errs+1/delta);
			else
				norm = refshape.Norm2();
		}
	}
}

// SET THE CONSTRAINTS INTANGENT SPACE (SPANDED BY ROW VECTORS) 
void CASMMLSearchModelBayesian::SetTangentConstraint()
{
	// vectors orthogonal to tangent space
	int iNbPoint=m_AvgShape.PointNum();
	m_TangentConstraint.Resize(4,2*iNbPoint);
	//m_TangentConstraint[0]=m_AvgShape.Points();
	m_TangentConstraint.SetRow(0, m_AvgShape.Points());
	for (int i=0;i<iNbPoint;i++)
	{		
		m_TangentConstraint[1][2*i] = - m_AvgShape.PointY(i);
		m_TangentConstraint[1][2*i+1] = m_AvgShape.PointX(i);
		m_TangentConstraint[3][2*i] = m_TangentConstraint[2][2*i+1] = 1/sqrt(iNbPoint);
		m_TangentConstraint[3][2*i+1] = m_TangentConstraint[2][2*i] = 0;
	}
}

bool CASMMLSearchModelBayesian::SearchConvergeBayesian(int MaxSteps,double thresh,int MinLevel)
{
	SetTangentConstraint();

	CShape lastShape;
	lastShape = m_SearchShape;
	int iStep;	
	for (iStep=0;iStep<MaxSteps;iStep++)           
	{
	//	if (SearchBayesian(MinLevel)==false)
		SearchBayesian(MinLevel);
		double dist=lastShape.PointToPointAvgDistance(m_SearchShape);
		//TRACE1("\n dist: %f",dist);
		lastShape = m_SearchShape;

		if (dist<thresh)
		{
			return true;
		}
	}
	return false;
}

bool CASMMLSearchModelBayesian::SearchBayesian(int MinLevel,int MaxLevel)
{
	int iLevel;
	// input parameters check
	if (MaxLevel>m_iTotLevel-1) MaxLevel = m_iTotLevel-1;
	if (MaxLevel<0) MaxLevel=0;
	if (MinLevel<0) MinLevel=0;
	if (MinLevel>MaxLevel) MinLevel = MaxLevel;

	for (iLevel = MaxLevel ; iLevel>=MinLevel;iLevel--)
	{
		bool ret;
		SetCurrLevel(iLevel);
		ret = SearchOneLevelBayesian();
	}

	return true;
}

bool CASMMLSearchModelBayesian::SearchIterative(int MaxSteps) 
{
	return SearchConvergeBayesian(MaxSteps, 2.0, 0);
}