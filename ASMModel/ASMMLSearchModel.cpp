
// ASMMLSearchModel.cpp: implementation of the CASMMLSearchModel class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ASMModel.h"
#include "ASMMLSearchModel.h"


static char s_tmpbuf[256];


CASMMLSearchModel::CASMMLSearchModel(void)
{
    m_pImagePyramid = NULL;
    SetModelParam();
    SetSearchParam();
    m_LoacalStructType = 0;
    m_iCurLevel = 0;
    m_iCurLevelScale = 1;
    m_pSearchImg = NULL;

    m_bNeedConstrain = false;

	Clear();
	
	m_SInTraining = 0.;
	m_ThetaInTraining = 0.;
	m_TxInTraining = 0.;
	m_TyInTraining = 0.;

	m_S = 0.;
	m_Theta = 0.;
	m_Tx = 0.;
	m_Ty = 0.;

	m_bConstrained.clear();
	m_ConnectModel.Clear();
	m_ConstrainShape.Clear();
	m_SearchShape.Clear();
	m_ShapeNormal.Clear();
	m_vLocalStruct.clear();

	m_SearchMode = 0;	
}

CASMMLSearchModel::~CASMMLSearchModel(void)
{
    Clear();
}

/*
bool CASMMLSearchModel::ReadFile_HChen(FILE *fp)
{
    bool ret = false;

    int iLevel, iPoint;

    // Load CASMMLSearchModel
    if (fscanf(fp, "%s", s_tmpbuf) == EOF) goto exit;
    if (strcmp(s_tmpbuf, "CASMMLSearchModel2") == 0)
    {
        fscanf(fp, "%s %d", s_tmpbuf, &m_LoacalStructType);
    }

    if (fscanf(fp, "%s", s_tmpbuf) == EOF) goto exit;

    if (fscanf(fp, "%s %d", s_tmpbuf, &m_iSearchLength) == EOF) goto exit;

    if (fscanf(fp, "%s %le", s_tmpbuf, &m_fConvergeThreshold) == EOF) goto exit;

    if (fscanf(fp, "%s %le", s_tmpbuf, &m_SInTraining) == EOF) goto exit;

    if (fscanf(fp, "%s %le", s_tmpbuf, &m_TxInTraining) == EOF) goto exit;

    if (fscanf(fp, "%s %le", s_tmpbuf, &m_TyInTraining) == EOF) goto exit;

    if (fscanf(fp, "%s %le", s_tmpbuf, &m_ThetaInTraining) == EOF) goto exit;

    if (fscanf(fp, "%s %d", s_tmpbuf, &m_iLocalStructLength) == EOF) goto exit;

    if (fscanf(fp, "%s %d", s_tmpbuf, &m_iTotLevel) == EOF) goto exit;

    if (fscanf(fp, "%s %d", s_tmpbuf, &m_iMaxStepPerLevel) == EOF) goto exit;

    if (fscanf(fp, "%s %d", s_tmpbuf, &m_ModelWithoutAlign) == EOF) goto exit;

    // Init CConnectModel
    fscanf(fp, "%s", s_tmpbuf);
    if (!m_ConnectModel.TxtRead(fp)) goto exit;

    // Init CShapeModel
    if (!CShapeModel::ReadFile_HChen(fp)) goto exit;

    // Init CASMPCALocalStructs
    if (fscanf(fp, "%s", s_tmpbuf) == EOF) goto exit;
    m_vLocalStruct.resize(m_ConnectModel.PointNum() * m_iTotLevel);
    for (iLevel = 0; iLevel < m_iTotLevel; iLevel++)
    {
        for (iPoint = 0; iPoint < m_ConnectModel.PointNum(); iPoint++)
        {
            if (!m_vLocalStruct[iPoint * m_iTotLevel + iLevel].ReadFile_HChen(fp)) goto exit;
        }
    }

    SetAvgShape();

    ret = true;

exit:
    return ret;
}

#ifdef DATA_TXT2BIN
bool CASMMLSearchModel::TxtRead(FILE *fp)
{
    bool ret = false;

    int iLevel, iPoint;

    // Load CASMMLSearchModel
    if (fscanf(fp, "%s", s_tmpbuf) == EOF) goto exit;
    if (strcmp(s_tmpbuf, "CASMMLSearchModel2") == 0)
    {
        fscanf(fp, "%s %d", s_tmpbuf, &m_LoacalStructType);
    }

    if (fscanf(fp, "%s", s_tmpbuf) == EOF) goto exit;

    if (fscanf(fp, "%s %d", s_tmpbuf, &m_iSearchLength) == EOF) goto exit;

    if (fscanf(fp, "%s %le", s_tmpbuf, &m_fConvergeThreshold) == EOF) goto exit;

    if (fscanf(fp, "%s %le", s_tmpbuf, &m_SInTraining) == EOF) goto exit;

    if (fscanf(fp, "%s %le", s_tmpbuf, &m_TxInTraining) == EOF) goto exit;

    if (fscanf(fp, "%s %le", s_tmpbuf, &m_TyInTraining) == EOF) goto exit;

    if (fscanf(fp, "%s %le", s_tmpbuf, &m_ThetaInTraining) == EOF) goto exit;

    if (fscanf(fp, "%s %d", s_tmpbuf, &m_iLocalStructLength) == EOF) goto exit;

    if (fscanf(fp, "%s %d", s_tmpbuf, &m_iTotLevel) == EOF) goto exit;

    if (fscanf(fp, "%s %d", s_tmpbuf, &m_iMaxStepPerLevel) == EOF) goto exit;

    if (fscanf(fp, "%s %d", s_tmpbuf, &m_ModelWithoutAlign) == EOF) goto exit;

    // Init CConnectModel
    fscanf(fp, "%s", s_tmpbuf);
    if (!m_ConnectModel.TxtRead(fp)) goto exit;

    // Init CShapeModel
    if (!CShapeModel::TxtRead(fp)) goto exit;

    // Init CASMPCALocalStructs
    if (fscanf(fp, "%s", s_tmpbuf) == EOF) goto exit;
    m_vLocalStruct.resize(m_ConnectModel.PointNum() * m_iTotLevel);
    for (iLevel = 0; iLevel < m_iTotLevel; iLevel++)
    {
        for (iPoint = 0; iPoint < m_ConnectModel.PointNum(); iPoint++)
        {
            if (!m_vLocalStruct[iPoint * m_iTotLevel + iLevel].TxtRead(fp)) goto exit;
        }
    }

    SetAvgShape();

    ret = true;

exit:
    return ret;
}

bool CASMMLSearchModel::BinWrite(FILE *fp)
{
    bool ret = false;

    int iLevel, iPoint;
    short tmp;

    tmp = m_LoacalStructType;
    if (fwrite(&tmp, sizeof(short), 1, fp) != 1) goto exit;

    tmp = m_iSearchLength;
    if (fwrite(&tmp, sizeof(short), 1, fp) != 1) goto exit;

    if (fwrite(&m_fConvergeThreshold, sizeof(double), 1, fp) != 1) goto exit;

    if (fwrite(&m_SInTraining, sizeof(double), 1, fp) != 1) goto exit;

    if (fwrite(&m_TxInTraining, sizeof(double), 1, fp) != 1) goto exit;

    if (fwrite(&m_TyInTraining, sizeof(double), 1, fp) != 1) goto exit;

    if (fwrite(&m_ThetaInTraining, sizeof(double), 1, fp) != 1) goto exit;

    tmp = m_iLocalStructLength;
    if (fwrite(&tmp, sizeof(short), 1, fp) != 1) goto exit;

    tmp = m_iTotLevel;
    if (fwrite(&tmp, sizeof(short), 1, fp) != 1) goto exit;

    tmp = m_iMaxStepPerLevel;
    if (fwrite(&tmp, sizeof(short), 1, fp) != 1) goto exit;

    tmp = m_ModelWithoutAlign;
    if (fwrite(&tmp, sizeof(short), 1, fp) != 1) goto exit;

    if (!m_ConnectModel.BinWrite(fp)) goto exit;

    if (!CShapeModel::BinWrite(fp)) goto exit;

    for (iLevel = 0; iLevel < m_iTotLevel; iLevel++)
    {
        for (iPoint = 0; iPoint < m_ConnectModel.PointNum(); iPoint++)
        {
            if (!m_vLocalStruct[iPoint * m_iTotLevel + iLevel].BinWrite(fp)) goto exit;
        }
    }

    ret = true;

exit:
    return ret;
}
#endif // DATA_TXT2BIN


BYTE* CASMMLSearchModel::Decode(BYTE* buffer)
{
    m_LoacalStructType = *((short *)buffer);
    buffer += sizeof(short);
    
    m_iSearchLength = *((short *)buffer);
    buffer += sizeof(short);
    
    m_fConvergeThreshold = *((double *)buffer);
    buffer += sizeof(double);
    
    m_SInTraining = *((double *)buffer);
    buffer += sizeof(double);
    
    m_TxInTraining = *((double *)buffer);
    buffer += sizeof(double);
    
    m_TyInTraining = *((double *)buffer);
    buffer += sizeof(double);
    
    m_ThetaInTraining = *((double *)buffer);
    buffer += sizeof(double);

    m_iLocalStructLength = *((short *)buffer);
    buffer += sizeof(short);
    
    m_iTotLevel = *((short *)buffer);
    buffer += sizeof(short);
    
    m_iMaxStepPerLevel = *((short *)buffer);
    buffer += sizeof(short);
    
    m_ModelWithoutAlign = *((short *)buffer);
    buffer += sizeof(short);
    
    buffer = m_ConnectModel.Decode(buffer);
    
    buffer = CShapeModel::Decode(buffer);
    
    m_vLocalStruct.resize(m_ConnectModel.PointNum() * m_iTotLevel);
    for (int iLevel = 0; iLevel < m_iTotLevel; iLevel++)
    {
        for (int iPoint = 0; iPoint < m_ConnectModel.PointNum(); iPoint++)
        {
            buffer = m_vLocalStruct[iPoint * m_iTotLevel + iLevel].Decode(buffer);
        }
    }

    SetAvgShape();

    return buffer;
}
*/

void CASMMLSearchModel::Clear(void)
{
    if (m_pImagePyramid)
    {
        delete m_pImagePyramid;
        m_pImagePyramid = NULL;
    }
}


void CASMMLSearchModel::SetModelParam(int totlevel, int localstructlength, int withoutalign)
{
    Clear();
    m_iTotLevel = totlevel;
    m_pImagePyramid = new CVisGrayPyramid(totlevel);

    m_iLocalStructLength = localstructlength;
    m_ModelWithoutAlign = withoutalign; 
}

void CASMMLSearchModel::SetSearchParam(int searchlength, int stepsperlevel, double convergethreshold)
{
    m_iSearchLength = searchlength;
    m_iMaxStepPerLevel = stepsperlevel;
    m_fConvergeThreshold = convergethreshold;
}

void CASMMLSearchModel::SetAvgShape(void)
{
    m_Tx = m_TxInTraining;
    m_Ty = m_TyInTraining;
    m_S = m_SInTraining;
    m_Theta = m_ThetaInTraining;

    int iNbEigen = m_PCA2Model.NbEigen();
   
	m_EigenParam.Resize(iNbEigen);
    m_EigenParam = 0.0;

	m_NMFEncoding.Resize(m_NMFModel.NbBasis());
	m_NMFEncoding = 0.0;
    
	m_SearchShape = m_AvgShape;
    if (!m_ModelWithoutAlign)
    {
		GenerateShapeWithPos(m_Tx, m_Ty, m_S, m_Theta, m_EigenParam, m_SearchShape);
    }
    else
    {
		GenerateShape(m_EigenParam, m_SearchShape);
    }
}

void CASMMLSearchModel::SetShape(const CShape& initshape)
{ 
	m_SearchShape = initshape; 

	double Tx, Ty, S, Theta;

	if ( m_PCA2Model.IsReady() )
	{
		GetShapeParamAndPos(Tx, Ty, S, Theta, m_EigenParam, m_SearchShape, ePCA);
		if ( m_SearchMode==0 || m_SearchMode==1 )
		{
			m_Tx = Tx;
			m_Ty =Ty;
			m_S = S;
			m_Theta = Theta;
		}
	}

	if ( m_NMFModel.IsReady() )
	{
		GetShapeParamAndPos(Tx, Ty, S, Theta, m_NMFEncoding, m_SearchShape, eNMF);
		if ( m_SearchMode==2 )
		{
			m_Tx = Tx;
			m_Ty =Ty;
			m_S = S;
			m_Theta = Theta;
		}
	}
}

void CASMMLSearchModel::SetSearchImage(CVisGrayByteImage* pSrcImg)
{
	if(m_pImagePyramid==NULL)
	    m_pImagePyramid = new CVisGrayPyramid(m_iTotLevel);
    m_pImagePyramid->SetBase(*pSrcImg);
    m_pImagePyramid->Construct();
}


//
// Search until converge. Here we use the distance between search result as the converge rule. 
// We can use thresh to set the converge threshold , and at most we iterates MaxSteps .
// 
bool CASMMLSearchModel::SearchConverge(int MaxSteps, double thresh, int MinLevel)
{
    long dist;
    CShape lastShape = m_SearchShape;
    for (int iStep = 0; iStep < MaxSteps; iStep++)
    {
        if (!Search(MinLevel)) return false;
        dist = (long) lastShape.Distance(m_SearchShape);
        lastShape = m_SearchShape;
        if (dist < thresh) return true;
    }
    return false;
}

//
// Multi-level search.(from top level to buttom level) 
// This is the basic multilvel active shape model search 
//
bool CASMMLSearchModel::Search(int MinLevel, int MaxLevel)
{
    // input parameters check
    if (MaxLevel > m_iTotLevel - 1) MaxLevel = m_iTotLevel - 1;
    if (MaxLevel < 0) MaxLevel = 0;
    if (MinLevel < 0) MinLevel = 0;
    if (MinLevel > MaxLevel) MinLevel = MaxLevel;

    for (int iLevel = MaxLevel ; iLevel >= MinLevel; iLevel--)
    {
        SetCurrLevel(iLevel);
        if (!SearchOneLevel() && iLevel == MaxLevel) return false;
    }
    return true;
}

//
// For one level , we iterates at most m_iMaxStepPerLevel steps
//
bool CASMMLSearchModel::SearchOneLevel(void)
{
    bool bConverge = false;
    int iSteps = 0;
    if (m_iCurLevel == m_iTotLevel - 1) // the top level
    {
        while ((!bConverge) && (iSteps < m_iMaxStepPerLevel * 2))
        {
            bConverge = SearchTopLevelOneStep();
            iSteps++;
        }
    }
    else 
    {
        while ((!bConverge) && (iSteps < m_iMaxStepPerLevel))
        {
            bConverge = SearchOneStep();
            iSteps++;
        }
    }
    return bConverge;
}

//
//  The basic search step in ASM 
//
bool CASMMLSearchModel::SearchOneStep(void)
{
    bool bConverge = UpdateShapeWithImage();
    UpdateShapeWithModel();
    return bConverge;
}

//
// It can be the same as the SearchOneStep
// But I modify something to make it more efficient for the toppest level search
//
bool CASMMLSearchModel::SearchTopLevelOneStep(void)
{
    double oldtheta = m_Theta;
    CShape oldShape = m_SearchShape;

    bool bConverge = UpdateShapeWithImage();
    CShape imageUpdateShape = m_SearchShape;
    UpdateShapeWithModel();
    m_Theta += (m_Theta - oldtheta);

    if (bConverge)
    {
        oldtheta = m_Theta;
        oldShape = m_SearchShape;
        bConverge = SearchOneStep();
        m_Theta += (m_Theta-oldtheta);

        if (bConverge)
        {
            double avgdist = oldShape.PointToPointAvgDistance(m_SearchShape);
            if (avgdist < m_iCurLevelScale)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        m_Theta += (m_Theta - oldtheta);
        return false;
    }
}


// Fit the current shape into model space
// Input: m_SearchShape = the current shape
// Output: m_a,m_b,m_EigenParam = fitted shape parameter
//         m_SearchShape = fitted shape  
void CASMMLSearchModel::UpdateShapeWithModel(void)
{
    if (!m_bNeedConstrain)
    {
        if (!m_ModelWithoutAlign)
        {
			GetShapeParamAndPos(m_Tx,m_Ty,m_S,m_Theta,m_EigenParam,m_SearchShape);
        }
        else
        {
			GetShapeParam(m_EigenParam, m_SearchShape);
        }

        ConstrainModel(m_EigenParam);

        if (!m_ModelWithoutAlign)
        {
            GenerateShapeWithPos(m_Tx,m_Ty,m_S,m_Theta,m_EigenParam,m_SearchShape);
        }
        else
        {
            GenerateShape(m_EigenParam,m_SearchShape);
        }
    }
    else
    {
        for (int iStep = 0; iStep < 4; iStep++ )
        {
            // 1. restore constrained shape
            int pointNum = m_SearchShape.PointNum();
            for (int i = 0; i < pointNum; i++)
            {
				if (m_bConstrained[i] == true)
                {
                    m_SearchShape.PointX(i) = m_ConstrainShape.PointX(i);
                    m_SearchShape.PointY(i) = m_ConstrainShape.PointY(i);
                }
            }	
            // 2. 
            if (!m_ModelWithoutAlign)
            {
                GetShapeParamAndPos(m_Tx,m_Ty,m_S,m_Theta,m_EigenParam,m_SearchShape);
            }
            else
            {
                GetShapeParam(m_EigenParam,m_SearchShape);
            }
            ConstrainModel(m_EigenParam);
            // 3. regenerate the shape
            if (!m_ModelWithoutAlign)
            {
                GenerateShapeWithPos(m_Tx,m_Ty,m_S,m_Theta,m_EigenParam,m_SearchShape);
            }
            else
            {
                GenerateShape(m_EigenParam,m_SearchShape);
            }
		}
	}
}

//
// It is one phase of the search step . For each point, search the best position only by the image
//
bool CASMMLSearchModel::UpdateShapeWithImage(void)
{
    int iNbPoints = m_AvgShape.PointNum();

    GetShapeNormal();

    double desx, desy;
    int iConvergePoints = 0;
    for (int iPoint = 0; iPoint < iNbPoints; iPoint++)
    {
        if (SearchOnePoint(iPoint, desx, desy))
        {
            iConvergePoints++;
        }
        m_SearchShape.PointX(iPoint) = desx;
        m_SearchShape.PointY(iPoint) = desy;
    }
    
    if (iConvergePoints > m_fConvergeThreshold * iNbPoints )
    {
        return true; // Converge
    }

    return false; // Not Converge 
}



//
// We modify it from CASMModel::ConstrainModel to make it more hard constrain to the top level 
//
void CASMMLSearchModel::ConstrainModel(CVisDVector& eigenparam, double factor, int starteigen)
{
    double thresh;

    int iNbEigen = m_PCA2Model.NbEigen();
	for (int iEigen = starteigen; iEigen < iNbEigen; iEigen++)
	{
		if (m_iCurLevel == m_iTotLevel-1)
		{
			// the top level
			if (iEigen < 5)
            {
				thresh = factor * 2 * sqrt(m_PCA2Model.EigenVal(iEigen)) / 3;	
            }
            else
            {
                thresh = 0.0;
            }
		}
		else
        {
			thresh = factor * sqrt(m_PCA2Model.EigenVal(iEigen));
        }
		if (fabs(eigenparam[iEigen]) > thresh)
		{
			if (eigenparam[iEigen] < 0) eigenparam[iEigen] = -thresh;
			else eigenparam[iEigen] = thresh;
		}
	}
}


//
// Calc the Normal( m_ShapeNormal) of each point of Current Search Shape (m_SearchShape)
//
void CASMMLSearchModel::GetShapeNormal(void)
{
	::GetShapeNormal(m_ConnectModel, m_SearchShape, m_ShapeNormal);
/*
    int iNbContour = m_ConnectModel.ContourNum();
    assert(iNbContour >= 1);

    m_ShapeNormal = m_AvgShape;

    // for each contour
    for (int iContour = 0; iContour < iNbContour; iContour++)
    {
        CIndexedContour& curContour = m_ConnectModel.Contour(iContour);
        int iNbPointOfContour = curContour.PointNum();
        int iCurPoint, iPrevPoint, iNextPoint;
        // for each indexed point of a contour
        for (int iPointOfContour = 0; iPointOfContour < iNbPointOfContour; iPointOfContour++)
        {
            // first get the index of prev/current/next points 
            iCurPoint = curContour.Index(iPointOfContour); //Get Current point index
            //If current point is the the first point of a contour
            if (iPointOfContour == 0) 
            {
                if (curContour.Type() == CONTOUR_OPEN) 
                {
                    iPrevPoint = iNextPoint = curContour.Index(iPointOfContour + 1);
                }
                else // CONTOUR_CLOSED
                {
                    iNextPoint = curContour.Index(iPointOfContour + 1);
                    iPrevPoint = curContour.Index(iNbPointOfContour - 1);
                }
            }
            else if (iPointOfContour == iNbPointOfContour - 1)
            {
                if (curContour.Type() == CONTOUR_OPEN) 
                {
                    iPrevPoint = iNextPoint = curContour.Index(iPointOfContour - 1);
                }
                else // CONTOUR_CLOSED
                {
                    iPrevPoint = curContour.Index(iPointOfContour - 1);
                    iNextPoint = curContour.Index(0);
                }
            }
            else
            {
                iPrevPoint = curContour.Index(iPointOfContour - 1);
                iNextPoint = curContour.Index(iPointOfContour + 1); 
            }
            // calc the normal of current point 
            double EPS = 0.00001;
            double dx1, dy1, dx2, dy2, tempf;
            // get the vector (dx1,dy1) = currentpoint -> prev point
            //                (dx2,dy2) = next point   -> currentpoint  
            dx1 = m_SearchShape.PointX(iCurPoint) - m_SearchShape.PointX(iPrevPoint);
            dy1 = m_SearchShape.PointY(iCurPoint) - m_SearchShape.PointY(iPrevPoint);
            dx2 = m_SearchShape.PointX(iNextPoint) - m_SearchShape.PointX(iCurPoint);
            dy2 = m_SearchShape.PointY(iNextPoint) - m_SearchShape.PointY(iCurPoint);
            // Normalize the (dx1,dy1)
            tempf = sqrt(dx1 * dx1 + dy1 * dy1);
            if (tempf < EPS)
            {
                dx1 = dy1 = 0;
            }
            else
            {
                dx1 /= tempf;
                dy1 /= tempf;
            }
            tempf = sqrt(dx2 * dx2 + dy2 * dy2);
            // Normalize (dx2,dy2)
            if (tempf < EPS)
            {
                dx2 = dy2 = 0;
            }
            else
            {
                dx2 /= tempf;
                dy2 /= tempf;
            }
            // rotate the (dx1,dy1) and (dx2,dy2) by x'=-y y'=x
            tempf = dx1; dx1 = -dy1; dy1 = tempf;
            tempf = dx2; dx2 = -dy2; dy2 = tempf;
            // avg vector (dx1,dy1) and (dx2,dy2)
            double dx, dy;
            dx = (dx1 + dx2); dy = (dy1 + dy2);
            tempf = sqrt(dx * dx + dy * dy);
            // if current point is the end point of an open contour tempf will be zero 
            // So we consider it seperately.
            if ((curContour.Type() == CONTOUR_OPEN) && ((iPointOfContour == 0) || (iPointOfContour == iNbPointOfContour - 1)))
            {
                dx = dx1;
                dy = dy1;
            }
            else if (tempf<EPS) 
            {
                dx = dx1;
                dy = dy1;
            }
            else
            {
                dx /= tempf;
                dy /= tempf;
            }
            // save normal to shapenormal
            m_ShapeNormal.PointX(iCurPoint) = dx;
            m_ShapeNormal.PointY(iCurPoint) = dy;
        }
    }
*/
}


void CASMMLSearchModel::SetCurrLevel(int iCurLevel)
{
    if (iCurLevel < 0)
    {
        iCurLevel = 0;
    }
    if (iCurLevel >= m_iTotLevel)
    {
        iCurLevel = m_iTotLevel - 1;
    }

    m_iCurLevel = iCurLevel;
    m_iCurLevelScale = 1;
    while (iCurLevel > 0)
    {
        m_iCurLevelScale *= 2;
        iCurLevel--;
    }
}


void CASMMLSearchModel::UpdateConstrainShape(const CShape& constrainShape)
{
	if (m_bConstrained.size() != constrainShape.PointNum())
	{
		assert(0);
		return;
	}

	m_ConstrainShape = constrainShape;
}

void CASMMLSearchModel::SetConstrain(const std::vector<bool>& bConstrained, const CShape& constrainShape)
{
    if (bConstrained.size() != m_AvgShape.PointNum())
    {
        assert(0);
        return;
    }

    m_bConstrained.resize(m_AvgShape.PointNum());
    m_ConstrainShape.Resize(m_AvgShape.PointNum());
    m_bConstrained = bConstrained;
    m_ConstrainShape = constrainShape;
    
/*
	m_bNeedConstrain = false;
	for (int i = 0; i < m_bConstrained.size() ; i++)
    {
        if (m_bConstrained[i]==true)
        {
            m_bNeedConstrain = true;
            break;
        }
    }
*/
}


void CASMMLSearchModel::ClearConstrain(void)
{
    m_bNeedConstrain = false;
}

void CASMMLSearchModel::EnableConstrain(bool b/*=true*/)
{
	m_bNeedConstrain = b;
}

COutputDataFile& Output(COutputDataFile& fs, const CASMMLSearchModel& model)
{
    int iLevel, iPoint;
    
    fs << ClassTag("CASMMLSearchModel") << sep_endl;
	fs << ClassTag("LocalStructureType") << sep_semicolon << model.m_LoacalStructType << sep_endl;
	fs << ClassTag("SearchLength") << sep_semicolon << model.m_iSearchLength << sep_endl;
	fs << ClassTag("ConvergeThreshold") << sep_semicolon << model.m_fConvergeThreshold << sep_endl;
	fs << ClassTag("SInTraining") << sep_semicolon << model.m_SInTraining << sep_endl;
	fs << ClassTag("TxInTraining") << sep_semicolon << model.m_TxInTraining << sep_endl;
	fs << ClassTag("TyInTraining") << sep_semicolon << model.m_TyInTraining << sep_endl;
	fs << ClassTag("ThetaInTraining") << sep_semicolon << model.m_ThetaInTraining << sep_endl;
	fs << ClassTag("LocalStructureLength") << sep_semicolon << model.m_iLocalStructLength << sep_endl;
	fs << ClassTag("TotLevel") << sep_semicolon << model.m_iTotLevel << sep_endl;
	fs << ClassTag("MaxStepPerLevel") << sep_semicolon << model.m_iMaxStepPerLevel << sep_endl;
	fs << ClassTag("m_ModelWithoutAlign") << sep_semicolon << model.m_ModelWithoutAlign << sep_endl;
	
	fs << model.m_ConnectModel << sep_endl;
	fs << (CShapeModel&)(model) << sep_endl;
	fs << model.m_ShapeModel << sep_endl;

    for (iLevel = 0; iLevel < model.m_iTotLevel; iLevel++)
    {
        for (iPoint = 0; iPoint < model.m_ConnectModel.PointNum(); iPoint++)
        {
            fs << model.m_vLocalStruct[iPoint * model.m_iTotLevel + iLevel] << sep_endl;
        }
    }
	return fs;
}

CInputDataFile& Input(CInputDataFile& fs, CASMMLSearchModel& model)
{
    int iLevel, iPoint;
    
    fs >> ClassTag("CASMMLSearchModel") >> sep_endl;
	fs >> ClassTag("LocalStructureType") >> sep_semicolon >> model.m_LoacalStructType >> sep_endl;
	fs >> ClassTag("SearchLength") >> sep_semicolon >> model.m_iSearchLength >> sep_endl;
	fs >> ClassTag("ConvergeThreshold") >> sep_semicolon >> model.m_fConvergeThreshold >> sep_endl;
	fs >> ClassTag("SInTraining") >> sep_semicolon >> model.m_SInTraining >> sep_endl;
	fs >> ClassTag("TxInTraining") >> sep_semicolon >> model.m_TxInTraining >> sep_endl;
	fs >> ClassTag("TyInTraining") >> sep_semicolon >> model.m_TyInTraining >> sep_endl;
	fs >> ClassTag("ThetaInTraining") >> sep_semicolon >> model.m_ThetaInTraining >> sep_endl;
	fs >> ClassTag("LocalStructureLength") >> sep_semicolon >> model.m_iLocalStructLength >> sep_endl;
	fs >> ClassTag("TotLevel") >> sep_semicolon >> model.m_iTotLevel >> sep_endl;
	fs >> ClassTag("MaxStepPerLevel") >> sep_semicolon >> model.m_iMaxStepPerLevel >> sep_endl;
	fs >> ClassTag("m_ModelWithoutAlign") >> sep_semicolon >> model.m_ModelWithoutAlign >> sep_endl;
		
	fs >> model.m_ConnectModel >> sep_endl;
	fs >> (CShapeModel&)(model) >> sep_endl;
	fs >> model.m_ShapeModel >> sep_endl;

	model.m_vLocalStruct.resize(model.m_iTotLevel * model.m_ConnectModel.PointNum());
    for (iLevel = 0; iLevel < model.m_iTotLevel; iLevel++)
    {
        for (iPoint = 0; iPoint < model.m_ConnectModel.PointNum(); iPoint++)
        {
            fs >> model.m_vLocalStruct[iPoint * model.m_iTotLevel + iLevel] >> sep_endl;
        }
    }
	model.SetAvgShape();

	return fs;
}

COutputDataFile& operator<<(COutputDataFile& fs, const CASMMLSearchModel& model)
{
	return Output(fs,model);
};

CInputDataFile& operator>>(CInputDataFile& fs, CASMMLSearchModel& model)
{
	return Input(fs,model);
}