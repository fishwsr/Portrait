// MixturedShapeModel.cpp: implementation of the CMixturedShapeModel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MixturedShapeModel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define VERBOSE 5

void CPPCAShapeModel::SetModel(const CShape& avgShape, const CPPCAModel& model)
{
	m_AvgShape = avgShape;
	m_SModel = model;
}

void CPPCAShapeModel::GetShapePos(double& Tx, double& Ty, double& s, double& theta, const CShape& srcShape, CShape& normaledShape) const
{
	normaledShape = srcShape;
	normaledShape.AlignTangent(m_AvgShape, Tx, Ty, s, theta);
}

void CPPCAShapeModel::ApplyShapePos(double& Tx, double& Ty, double& s, double& theta, CShape& desShape, const CShape& normaledShape) const
{
	double a = s * cos(theta);
    double b = s * sin(theta);

    double x, y;
    int m_iNbPoint = normaledShape.PointNum();
	desShape.Resize(m_iNbPoint);
    
	//rotate and scale
    for (int i = 0; i < m_iNbPoint; i++)
    {
        x = normaledShape.PointX(i);
        y = normaledShape.PointY(i);
        desShape.PointX(i) = a * x - b * y;
        desShape.PointY(i) = b * x + a * y;
    }
    //translate
    desShape.Translate(Tx, Ty);		
}

double CPPCAShapeModel::MAPEstimate(const CShape& srcShape, const CVisDVector& srcVar, double& Tx, double& Ty, double& s, double& theta, CShape& refshape) const
{
	int nStep = 20;
	bool bConverge = false;

	double OldTx, OldTy, OldS, OldTheta;
	CShape OldRefShape;
	double LogPost;

	refshape = m_AvgShape;

	if ( VERBOSE == 5 )
	{
//		std::cout << std::endl;
	}
	for (int i = 0; (!bConverge) && (i <nStep); i++ )
	{
		OldTx = Tx;
		OldTy = Ty;
		OldS = s;
		OldTheta = theta;
		OldRefShape = refshape;
		
		MAPEstimatePos(srcShape, srcVar, Tx, Ty, s, theta, refshape);
		LogPost = MAPEstimateRefShape(srcShape, srcVar, Tx, Ty, s, theta, refshape);
		if ( VERBOSE == 5 )
		{
//			std::cout << " " << LogPost;
		}

		if (i!=0) // Converge Check
		{
			double temp;
			temp = fabs(Tx-OldTx);
			temp += fabs(Ty-OldTy);
			temp += fabs(OldS/s - 1);
			temp += fabs(theta-OldTheta);
			temp += OldRefShape.Distance(refshape);
			if (temp<0.0001)
			{
				bConverge = true;
			}
		}	
	}
	if ( VERBOSE == 5 )
	{
//		std::cout << std::endl;
	}
	
	return LogPost;
}

void CPPCAShapeModel::MAPEstimatePos(const CShape& srcShape, const CVisDVector& srcVar, double& Tx, double& Ty, double& s, double& theta, CShape& refshape) const
{
	int iNbPoint=srcShape.PointNum();
	
	// get pose parameter
	double a,b, c,d;
	{
		CVisDMatrix AMat(refshape.PointNum()*2,4);
		CVisDVector B(refshape.PointNum()*2);
		int i;

		B = srcShape.Points();
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
			double InvDev = (1/srcVar[i]);
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
	Tx = c; Ty = d;
	s = sqrt(a*a+b*b);	// scale
	theta = - atan2(b,a);	// rotation
}

double CPPCAShapeModel::MAPEstimateRefShape(const CShape& srcShape, const CVisDVector& srcVar, double& Tx, double& Ty, double& s, double& theta, CShape& refshape) const
{
	// inverse of srcShape
	CShape invshape;
	invshape = srcShape;
	invshape.Translate(-Tx,-Ty);
	invshape.Scale(1/s);
	invshape.Rotation(-theta);
	
	bool bConverge = false;
	//refshape = invshape;	
	CVisDVector param; //= m_EigenParam;
	CShape OldShape;
	
	//for (int iStep = 0; (!bConverge) && (iStep < 20); iStep++)
	{
		OldShape = refshape;

		// Get Param*
		{
			m_SModel.GetParam(refshape.Points(), param);
		}

		// Get refshape*
		CShape reconstrcutShape;
		{
			m_SModel.Reconstruct(param, reconstrcutShape.Points());	
	
			for (int i=0; i<refshape.PointNum(); i++)
			{
				double delta = srcVar[i]/(s*s);
				double p= m_SModel.Sigma2() / ( m_SModel.Sigma2() + delta );
				if (p > 0.4)
				{
					p = 0.4;
				}
				refshape.PointX(i) = reconstrcutShape.PointX(i) * (1-p) + invshape.PointX(i) * p;
				refshape.PointY(i) = reconstrcutShape.PointY(i) * (1-p) + invshape.PointY(i) * p;
			}
		}

		double tempf = OldShape.Distance(refshape);
		if (tempf < 0.00001)
		{
			bConverge = true;
		}
	}
	
	double LogP = 0;
	LogP = m_SModel.LogP(refshape.Points());
	{
		for (int i = 0; i< refshape.PointNum(); i++)
		{
			double delta = srcVar[i]/(s*s);
			double detx = invshape.PointX(i) - refshape.PointX(i);
			double dety = invshape.PointX(i) - refshape.PointX(i);
			LogP += CSGaussian::LogGaussian(0, delta, detx);
			LogP += CSGaussian::LogGaussian(0, delta, dety);
		}
	}
	
	refshape.AlignTangent(m_AvgShape);
	return LogP;
}

CMixturedShapeModel::CMixturedShapeModel()
{

}

CMixturedShapeModel::~CMixturedShapeModel()
{

}

void CMixturedShapeModel::GetShapePos(double& Tx, double& Ty, double& s, double& theta, const CShape& srcShape, CShape& normaledShape) const
{
	normaledShape = srcShape;
	normaledShape.AlignTangent(m_AvgShape, Tx, Ty, s, theta);
}

void CMixturedShapeModel::ApplyShapePos(double& Tx, double& Ty, double& s, double& theta, CShape& desShape, const CShape& normaledShape) const
{
	double a = s * cos(theta);
    double b = s * sin(theta);

    double x, y;
    int m_iNbPoint = normaledShape.PointNum();
	desShape.Resize(m_iNbPoint);
    
	//rotate and scale
    for (int i = 0; i < m_iNbPoint; i++)
    {
        x = normaledShape.PointX(i);
        y = normaledShape.PointY(i);
        desShape.PointX(i) = a * x - b * y;
        desShape.PointY(i) = b * x + a * y;
    }
    //translate
    desShape.Translate(Tx, Ty);		
}

double CMixturedShapeModel::LogP(const CShape& shape) const
{
	double tx, ty, theta, s;
	CShape normaledShape;
	GetShapePos(tx,ty, s, theta, shape, normaledShape);
	return m_SModel.LogP(normaledShape.Points());
}

double CMixturedShapeModel::MAPEstimate(const CShape& srcShape, const CVisDVector& srcVar, double& Tx, double& Ty, double& s, double& theta, CShape& refshape) const
{
	int nStep = 20;
	bool bConverge = false;

	double OldTx, OldTy, OldS, OldTheta, LogPost1, LogPost2;
	CShape OldRefShape;

	if ( VERBOSE == 5 )
	{
//		std::cout << std::endl << "Method1" ;
	}
	for (int i = 0; (!bConverge) && (i <nStep); i++ )
	{
		OldTx = Tx;
		OldTy = Ty;
		OldS = s;
		OldTheta = theta;
		OldRefShape = refshape;
		
		MAPEstimatePos(srcShape, srcVar, Tx, Ty, s, theta, refshape);
		LogPost1 = MAPEstimateRefShape(srcShape, srcVar, Tx, Ty, s, theta, refshape);
		if ( VERBOSE == 5 )
		{
//			std::cout << " " << LogPost1;
		}

		if (i!=0) // Converge Check
		{
			double temp;
			temp = fabs(Tx-OldTx);
			temp += fabs(Ty-OldTy);
			temp += fabs(OldS/s - 1);
			temp += fabs(theta-OldTheta);
			temp += OldRefShape.Distance(refshape);
			if (temp<0.0001)
			{
				bConverge = true;
			}
		}	
	}
	
	if ( VERBOSE == 5 )
	{
//		std::cout << std::endl << "Method2" ;
	}
	
	{
		CPPCAShapeModel tempModel;
		double tempLogP, tempTx, tempTy, tempS, tempTheta;
		CShape tempRefShape;
		
		for (int i = 0; i< m_SModel.NCluster(); i++)
		{
			tempModel.SetModel(m_AvgShape, m_SModel.ClusterModel(i));
			tempLogP = tempModel.MAPEstimate(srcShape, srcVar, tempTx, tempTy, tempS, tempTheta, tempRefShape);
			tempLogP += m_SModel.MixingWeight()[i];

			if ( (i == 0) || ( (i != 0)&& (tempLogP > LogPost2)))
			{
				Tx = tempTx;
				Ty = tempTy;
				s = tempS;
				theta = tempTheta;
				refshape = tempRefShape;
				LogPost2 = tempLogP;
			}
		}
	}

	if ( VERBOSE == 5 )
	{
//		std::cout << std::endl << LogPost1 << " " << LogPost2 << std::endl;
	}
	
	return LogPost1;
}

void CMixturedShapeModel::MAPEstimatePos(const CShape& srcShape, const CVisDVector& srcVar, double& Tx, double& Ty, double& s, double& theta, CShape& refshape) const
{
	int iNbPoint=srcShape.PointNum();
	
	// get pose parameter
	double a,b, c,d;
	{
		CVisDMatrix AMat(refshape.PointNum()*2,4);
		CVisDVector B(refshape.PointNum()*2);
		int i;

		B = srcShape.Points();
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
			double InvDev = (1/srcVar[i]);
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
	Tx = c; Ty = d;
	s = sqrt(a*a+b*b);	// scale
	theta = - atan2(b,a);	// rotation	
}

double CMixturedShapeModel::MAPEstimateRefShape(const CShape& srcShape, const CVisDVector& srcVar, double& Tx, double& Ty, double& s, double& theta, CShape& refshape) const
{
	// inverse of srcShape
	CShape invshape;
	invshape = srcShape;
	invshape.Translate(-Tx,-Ty);
	invshape.Scale(1/s);
	invshape.Rotation(-theta);
	
	bool bConverge = false;
	//refshape = invshape;	
	int iCluster;
	CVisDVector param; //= m_EigenParam;
	CShape OldShape;
	double LogP = 0;
	
	//for (int iStep = 0; (!bConverge) && (iStep < 20); iStep++)
	{
		OldShape = refshape;

		// Get Param*
		{
			m_SModel.MAPInference(refshape.Points(), iCluster, param);
		}

		// Get refshape*
		CShape reconstrcutShape;
		{
			m_SModel.Reconstruct(param, iCluster, reconstrcutShape.Points());	
	
			for (int i=0; i<refshape.PointNum(); i++)
			{
				double sigma = m_SModel.ClusterModel(iCluster).Sigma2();
				double delta = srcVar[i]/(s*s);
				double p= sigma / ( sigma + delta );
				if (p > 0.4)
				{
					p = 0.4;
				}
				refshape.PointX(i) = reconstrcutShape.PointX(i) * (1-p) + invshape.PointX(i) * p;
				refshape.PointY(i) = reconstrcutShape.PointY(i) * (1-p) + invshape.PointY(i) * p;
			}
		}

		double tempf = OldShape.Distance(refshape);
		if (tempf < 0.00001)
		{
			bConverge = true;
		}

		LogP = m_SModel.LogP(refshape.Points());
		{
			for (int i = 0; i< refshape.PointNum(); i++)
			{
				double delta = srcVar[i]/(s*s);
				double detx = invshape.PointX(i) - refshape.PointX(i);
				double dety = invshape.PointX(i) - refshape.PointX(i);
				LogP += CSGaussian::LogGaussian(0, delta, detx);
				LogP += CSGaussian::LogGaussian(0, delta, dety);
			}
		}
	}

	refshape.AlignTangent(m_AvgShape);
	return LogP;
}

COutputDataFile& Output(COutputDataFile& fs, const CPPCAShapeModel& model)
{
	fs << ClassTag("CPPCAShapeModel") << sep_endl;
	fs << model.m_AvgShape << sep_endl;
	fs << model.m_SModel;
	return fs;
}

CInputDataFile& Input(CInputDataFile& fs, CPPCAShapeModel& model)
{
	fs >> ClassTag("CPPCAShapeModel") >> sep_endl;
	fs >> model.m_AvgShape >> sep_endl;
	fs >> model.m_SModel;
	return fs;
}

COutputDataFile& operator<<(COutputDataFile& fs, const CPPCAShapeModel& model)
{
	return Output(fs,model);
}

CInputDataFile& operator>>(CInputDataFile& fs, CPPCAShapeModel& model)
{
	return Input(fs,model);
}

COutputDataFile& Output(COutputDataFile& fs, const CMixturedShapeModel& model)
{
	fs << ClassTag("CPPCAShapeModel") << sep_endl;
	fs << model.m_AvgShape << sep_endl;
	fs << model.m_SModel;
	return fs;
}

CInputDataFile& Input(CInputDataFile& fs, CMixturedShapeModel& model)
{
	fs >> ClassTag("CPPCAShapeModel") >> sep_endl;
	fs >> model.m_AvgShape >> sep_endl;
	fs >> model.m_SModel;
	return fs;
}

COutputDataFile& operator<<(COutputDataFile& fs, const CMixturedShapeModel& model)
{
	return Output(fs,model);
}

CInputDataFile& operator>>(CInputDataFile& fs, CMixturedShapeModel& model)
{
	return Input(fs,model);
}