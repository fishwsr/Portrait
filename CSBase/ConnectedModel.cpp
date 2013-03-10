
// ConnectedModel.cpp: implementation of the CConnectedModel class.
//
///////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ConnectedModel.h"

CConnectedModel::CConnectedModel(void)
{
    m_PointNum = 0;
	m_vContours.clear();
}

CConnectedModel::CConnectedModel(const CConnectedModel& ref)
{
	m_PointNum = ref.m_PointNum;
	m_vContours = ref.m_vContours;
}

CConnectedModel::~CConnectedModel(void)
{
    Clear();
}

void CConnectedModel::Clear(void)
{
    m_PointNum = 0;
	m_vContours.clear();
}

bool CConnectedModel::operator==(const CConnectedModel& ref) const
{
	if ( &ref==this ) return true;
	if ( m_PointNum!=ref.m_PointNum ) return false;
	if ( m_vContours.size()!=ref.m_vContours.size() ) return false;

	for ( int i=0; i<m_vContours.size(); i++ )
	{
		if ( !(m_vContours[i]==ref.m_vContours[i]) )
			return false;
	}

	return true;
}

/*
#ifdef DATA_TXT2BIN
bool CConnectedModel::TxtRead(FILE *fp)
{
    bool ret = false;

    int i;

    Clear();

	int m_ContourNum;
    // Load CConnectedModel
	if (fscanf(fp, "%d %d", &m_PointNum, &m_ContourNum) == EOF) goto exit;

    m_vContours.resize(m_ContourNum);

    // Init CIndexedContour
	for (i = 0; i < m_ContourNum; i++)
	{
		if (!m_vContours[i].TxtRead(fp)) goto exit;
	}

    ret = true;

exit:
    return ret;
}

bool CConnectedModel::TxtWrite(FILE *fp) const
{
	bool ret = false;

	int i;

	// Save CConnectedModel
	fprintf(fp, "%d %d\n", PointNum(), ContourNum());
	
	// Save CIndexedContour
	for (i = 0; i < ContourNum(); i++)
	{
		if (!m_vContours[i].TxtWrite(fp)) goto exit;
	}

	ret = true;

exit:
	return ret;
}

bool CConnectedModel::BinRead(FILE *fp)
{
	bool ret = false;

	Clear();

	int i;
	short tmp;
	int m_ContourNum;

	if (fread(&tmp, sizeof(short), 1, fp) != 1) goto exit;
	m_PointNum = tmp;

	if (fread(&tmp, sizeof(short), 1, fp) != 1) goto exit;
	m_ContourNum = tmp;

	m_vContours.resize(m_ContourNum);

	for (i = 0; i < m_ContourNum; i++)
	{
		if (!m_vContours[i].BinRead(fp)) goto exit;
	}

	ret = true;

exit:
	return ret;
}

bool CConnectedModel::BinWrite(FILE *fp) const
{
    bool ret = false;

    int i;
    short tmp;
	int m_ContourNum;

    tmp = PointNum();
    if (fwrite(&tmp, sizeof(short), 1, fp) != 1) goto exit;

    tmp = m_ContourNum = ContourNum();
    if (fwrite(&tmp, sizeof(short), 1, fp) != 1) goto exit;

	for (i = 0; i < m_ContourNum; i++)
	{
		if (!m_vContours[i].BinWrite(fp)) goto exit;
	}

    ret = true;

exit:
    return ret;
}
#endif // DATA_TXT2BIN


BYTE* CConnectedModel::Decode(BYTE* buffer)
{
    m_PointNum = *((short *)buffer);
    buffer += sizeof(short);
    
    int m_ContourNum = *((short *)buffer);
    buffer += sizeof(short);
    
    m_vContours.resize(m_ContourNum);
    
	for (int i = 0; i < m_ContourNum; i++)
	{
		buffer = m_vContours[i].Decode(buffer);
	}
    
    return buffer;
}
*/

COutputDataFile& Output(COutputDataFile& fs, const CConnectedModel& con)
{
	fs << ClassTag("[ConnectedModel]") << sep_endl;

	fs << con.PointNum() << con.ContourNum() << sep_endl;

	// Save CIndexedContour
	for (int i = 0; i < con.ContourNum(); i++)
	{
		fs << con.Contour(i) << sep_endl;
	}

	return fs;
}

COutputDataFile& operator<<(COutputDataFile& ifs, const CConnectedModel& con)
{

	return Output(ifs,con);
}


CInputDataFile& Input(CInputDataFile& fs, CConnectedModel& con)
{
	fs >> ClassTag("[ConnectedModel]") >> sep_endl;

	int nContourNum;
	fs >> con.m_PointNum >> nContourNum >> sep_endl;
	con.m_vContours.resize(nContourNum);
	
	// Save CIndexedContour
	for (int i = 0; i < con.ContourNum(); i++)
	{
		fs >> con.Contour(i) >> sep_endl;
	}

	return fs;
}

CInputDataFile& operator>>(CInputDataFile& fs, CConnectedModel& con)
{
	return Input(fs,con);
}

void GetShapeNormal(const CConnectedModel& conmodel, const CShape& orgShape, CShape& normalShape)
{
	int iNbContour = conmodel.ContourNum();
    assert(iNbContour >= 1);
	assert(conmodel.PointNum() == orgShape.PointNum());
    normalShape = orgShape;

    // for each contour
    for (int iContour = 0; iContour < iNbContour; iContour++)
    {
		if ( conmodel.Contour(iContour).PointNum()==1 )
		{
			normalShape.PointX(conmodel.Contour(iContour).Index(0)) = sqrt(2) * 0.5;
			normalShape.PointY(conmodel.Contour(iContour).Index(0)) = sqrt(2) * 0.5;
			continue;
		}
		else if ( conmodel.Contour(iContour).PointNum()==2 )
		{
			normalShape.PointX(conmodel.Contour(iContour).Index(0)) = sqrt(2) * 0.5;
			normalShape.PointY(conmodel.Contour(iContour).Index(0)) = sqrt(2) * 0.5;
			normalShape.PointX(conmodel.Contour(iContour).Index(1)) = sqrt(2) * 0.5;
			normalShape.PointY(conmodel.Contour(iContour).Index(1)) = sqrt(2) * 0.5;
			continue;
		}

        CIndexedContour& curContour = const_cast<CIndexedContour&>(conmodel.Contour(iContour));
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
            dx1 = orgShape.PointX(iCurPoint) - orgShape.PointX(iPrevPoint);
            dy1 = orgShape.PointY(iCurPoint) - orgShape.PointY(iPrevPoint);
            dx2 = orgShape.PointX(iNextPoint) - orgShape.PointX(iCurPoint);
            dy2 = orgShape.PointY(iNextPoint) - orgShape.PointY(iCurPoint);
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
			
			if ( (curContour.Type() == CONTOUR_OPEN) && (iPointOfContour == 0) ) 
			{
				dx = -dx;
				dy = -dy;
			}

            // save normal to shapenormal
            normalShape.PointX(iCurPoint) = dx;
            normalShape.PointY(iCurPoint) = dy;
        }
    }
}

void GetCurveNormal(const CShape& orgShape, CShape& normalShape, bool bClosed)
{
    normalShape = orgShape;

    // for each contour
    {
        int iNbPointOfContour = orgShape.PointNum();
        int iCurPoint, iPrevPoint, iNextPoint;
        // for each indexed point of a contour
        for (int iPointOfContour = 0; iPointOfContour < iNbPointOfContour; iPointOfContour++)
        {
            // first get the index of prev/current/next points 
            iCurPoint = iPointOfContour; //Get Current point index
            //If current point is the the first point of a contour
            if (iPointOfContour == 0) 
            {
                if (!bClosed) 
                {
                    iPrevPoint = iNextPoint = iPointOfContour + 1;
                }
                else // CONTOUR_CLOSED
                {
                    iNextPoint = iPointOfContour + 1;
                    iPrevPoint = iNbPointOfContour - 1;
                }
            }
            else if (iPointOfContour == iNbPointOfContour - 1)
            {
                if (!bClosed) 
                {
                    iPrevPoint = iNextPoint = iPointOfContour - 1;
                }
                else // CONTOUR_CLOSED
                {
                    iPrevPoint = iPointOfContour - 1;
                    iNextPoint = 0;
                }
            }
            else
            {
                iPrevPoint = iPointOfContour - 1;
                iNextPoint = iPointOfContour + 1; 
            }
            // calc the normal of current point 
            double EPS = 0.00001;
            double dx1, dy1, dx2, dy2, tempf;
            // get the vector (dx1,dy1) = currentpoint -> prev point
            //                (dx2,dy2) = next point   -> currentpoint  
            dx1 = orgShape.PointX(iCurPoint) - orgShape.PointX(iPrevPoint);
            dy1 = orgShape.PointY(iCurPoint) - orgShape.PointY(iPrevPoint);
            dx2 = orgShape.PointX(iNextPoint) - orgShape.PointX(iCurPoint);
            dy2 = orgShape.PointY(iNextPoint) - orgShape.PointY(iCurPoint);
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
            if ((!bClosed) && ((iPointOfContour == 0) || (iPointOfContour == iNbPointOfContour - 1)))
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
			
			if ( (!bClosed) && (iPointOfContour == 0) ) 
			{
				dx = -dx;
				dy = -dy;
			}

            // save normal to shapenormal
            normalShape.PointX(iCurPoint) = dx;
            normalShape.PointY(iCurPoint) = dy;
        }
    }
}