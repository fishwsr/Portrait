
// ShapeModel.cpp: implementation of the CShapeModel class.
//
///////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "BaseModel.h"
#include "ShapeModel.h"


CShapeModel::CShapeModel(void)
{
    m_ModelWithoutAlign = 0;
}

CShapeModel::~CShapeModel(void)
{
	
}

void CShapeModel::GenerateShape(CVisDVector& param, CShape& desshape, EDataAnalysisMethod eMethod /*=ePCA*/)
{
    desshape.Resize(m_AvgShape.PointNum());
	
	if ( ePCA == eMethod )
	{
		assert(m_PCA2Model.IsReady());
		//m_Model.Reconstruct(eigenparam, desshape.Points());
		m_PCA2Model.Generate(desshape.Points(), param);
	}
	else if ( eNMF == eMethod )
	{
		assert(m_NMFModel.IsReady());
		m_NMFModel.Reconstruct(param, desshape.Points());
	}
}

void CShapeModel::GenerateShapeWithPos(double Tx, double Ty, double s, double theta, CVisDVector& param, CShape& desshape,EDataAnalysisMethod eMethod /*=ePCA*/)
{
	GenerateShape(param, desshape, eMethod);
	
    double a = s * cos(theta);
    double b = s * sin(theta);
    double x, y;
    int m_iNbPoint = desshape.PointNum();
    //rotate and scale
    for (int i = 0; i < m_iNbPoint; i++)
    {
        x = desshape.PointX(i);
        y = desshape.PointY(i);
        desshape.PointX(i) = a * x - b * y;
        desshape.PointY(i) = b * x + a * y;
    }
    //translate
    desshape.Translate(Tx, Ty);
}

void CShapeModel::GetShapeBase(int iEigen, int nVarChange, CShape &shape)
{
	CVisDVector eigenparam;
	eigenparam.Resize(m_PCA2Model.NbEigen());
	eigenparam = 0.;

	iEigen = min(eigenparam.Length()-1, max(0, iEigen));
	nVarChange = min(5, max(0, nVarChange));

	eigenparam[iEigen] = m_PCA2Model.EigenVal(iEigen)*nVarChange;

	GenerateShape(eigenparam, shape);
}

void CShapeModel::GetShapeParam(CVisDVector& param, const CShape& srcshape, EDataAnalysisMethod eMethod /*=ePCA*/)
{
	if ( ePCA == eMethod )
	{
		assert(m_PCA2Model.IsReady());
		m_PCA2Model.GetEigenParam(srcshape.Points(), param);
		//m_Model.GetParam(srcshape.Points(), eigenparam);
	}
	else if ( eNMF == eMethod )
	{
		assert(m_NMFModel.IsReady());
		m_NMFModel.Encode(srcshape.Points(), param);
/*
		// debug, begin
		SaveToFile("raw_shape.pts", srcshape);
		CShape newshape = srcshape;
		m_NMFModel.Regularize(srcshape.Points(), newshape.Points());
		SaveToFile("reg_shape.pts", newshape);
		printf("point_dist: %lf", srcshape.Distance(newshape));
		// debug, end
*/
	}
}

void CShapeModel::GetShapePos(double& Tx, double& Ty, double& s, double& theta, const CShape& srcShape, CShape& normaledShape) const
{
	normaledShape = srcShape;
	normaledShape.AlignTangent(m_AvgShape, Tx, Ty, s, theta);
}

void CShapeModel::ApplyShapePos(double& Tx, double& Ty, double& s, double& theta, CShape& desShape, const CShape& normaledShape)
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

void CShapeModel::GetShapeParamAndPos(double& Tx, double& Ty, double& s, double& theta, CVisDVector& param, const CShape& srcshape, EDataAnalysisMethod eMethod /*=ePCA*/)
{
    CShape& refShape = m_AvgShape;
    CShape workShape = srcshape;
    int m_iNbPoint = srcshape.PointNum();
    
    // 1. Get the tranlate parameter and set the center of srcshape to (0,0)
    workShape.CenterPoint(Tx, Ty);
    workShape.Translate(-Tx, -Ty);

    double x, y, x1, y1;
    // 2. get the align parameter to avgshape (x',y')
    // SXX1 = E(x*x') SYY1 = E(y*y') SXY1 = E(x*y') SYX1 = E(y*x')
    // SXX = E(x*x) SYY = E(y*y)
    // x' = ax -by
    // y' = bx +ay
    // a = (SXX1+SYY1)/(SXX+SYY)
    // b = (SXY1-SYX1)/(SXX+SYY)
    double SXX, SYY, SXX1, SYY1, SXY1, SYX1;
    SXX = SYY = SXX1 = SYY1 = SXY1 = SYX1 = 0.0;
    for (int i = 0; i < m_iNbPoint; i++)
    {
        x = workShape.PointX(i);
        y = workShape.PointY(i);
        x1 = refShape.PointX(i);
        y1 = refShape.PointY(i);
        SXX += x * x;
        SYY += y * y;
        SXX1 += x * x1;
        SYY1 += y * y1;
        SXY1 += x * y1;
        SYX1 += y * x1;
    }
    if (SXX + SYY == 0) return;
    double a = (SXX1 + SYY1) / (SXX + SYY);
    double b = (SXY1 - SYX1) / (SXX + SYY);

    // 3. align the workshape
    // x' = ax -by
    // y' = bx +ay
    for (i = 0; i < m_iNbPoint; i++)
    {
        x = workShape.PointX(i);
        y = workShape.PointY(i);
        workShape.PointX(i) = a * x - b * y;
        workShape.PointY(i) = b * x + a * y;
    }
    
    // 4. project into tangent space
    double SX1X1;
    SX1X1 = SXX1 = 0;   
    for (i = 0; i < m_iNbPoint; i++)
    {
        x = workShape.PointX(i);
        y = workShape.PointY(i);
        x1 = refShape.PointX(i);
        y1 = refShape.PointY(i);
        SX1X1 += x1 * x1;
        SXX1 += x * x1;
    }
    double stangent = SX1X1 / SXX1;
    workShape.Scale(stangent);

    // 5. get the pos and shape parameter
    s = sqrt(a * a + b * b) * stangent; s = 1.0 / s;
    theta = - atan2(b, a);
    GetShapeParam(param, workShape, eMethod);
}

/*
bool CShapeModel::ReadFile_HChen(FILE *fp)
{
	bool ret = false;
	char tempstr[1024];

    // Init CShape
    if (fscanf(fp, "%s", tempstr) == EOF) goto exit;
    if (!m_AvgShape.TxtRead(fp)) goto exit;

    // Init CPCA2Model
    if (!m_PCA2Model.ReadFile_HChen(fp)) goto exit;

    ret = true;

exit:
    return ret;
}

#ifdef DATA_TXT2BIN
bool CShapeModel::TxtRead(FILE *fp)
{
    bool ret = false;

	char g_tmpbuf[1024];
    // Init CShape
    if (fscanf(fp, "%s", g_tmpbuf) == EOF) goto exit;
    if (!m_AvgShape.TxtRead(fp)) goto exit;

    // Init CPCA2Model
    if (!m_PCA2Model.TxtRead(fp)) goto exit;

    ret = true;

exit:
    return ret;
}

bool CShapeModel::BinWrite(FILE *fp) const
{
    bool ret = false;
    
    bool fOldFlag = SetOctFlag(false);

    if (!m_AvgShape.BinWrite(fp)) goto exit;

    SetOctFlag(fOldFlag);

	if (!m_PCA2Model.BinWrite(fp)) goto exit;

    ret = true;

exit:
    return ret;
}
#endif // DATA_TXT2BIN


BYTE* CShapeModel::Decode(BYTE* buffer)
{
    bool fOldFlag = SetOctFlag(false);
    buffer = m_AvgShape.Decode(buffer);
    SetOctFlag(fOldFlag);

    buffer = m_PCA2Model.Decode(buffer);

    return buffer;
}
*/

COutputDataFile& Output(COutputDataFile& fs, const CShapeModel& model)
{
	fs << ClassTag("CShapeModel") << sep_endl;
	fs << model.m_AvgShape << sep_endl;
	fs << model.m_PCA2Model << sep_endl;
	fs << model.m_PPCAModel << sep_endl;
	fs << model.m_NMFModel << sep_endl;
	return fs;
}

CInputDataFile& Input(CInputDataFile& fs, CShapeModel& model)
{
	fs >> ClassTag("CShapeModel") >> sep_endl;
	fs >> model.m_AvgShape >> sep_endl;
	fs >> model.m_PCA2Model >> sep_endl;
	fs >> model.m_PPCAModel >> sep_endl;
	fs >> model.m_NMFModel >> sep_endl;
	return fs;
}

COutputDataFile& operator<<(COutputDataFile& fs, const CShapeModel& model)
{
	return Output(fs,model);
}

CInputDataFile& operator>>(CInputDataFile& fs, CShapeModel& model)
{
	return Input(fs,model);
}

double CShapeModel::LogP(const CShape& shape) const
{
	double Tx, Ty, s, theta;
	CShape normaledShape = shape;
	normaledShape.AlignTangent(m_AvgShape, Tx, Ty, s, theta);
	
	return m_PPCAModel.LogP(normaledShape.Points());
}

