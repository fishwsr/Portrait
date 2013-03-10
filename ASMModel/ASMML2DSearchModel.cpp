
// ASMML2DSearchModel.cpp: implementation of the CASMML2DSearchModel class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
//#include "VisImageIp.h"
//#include "VisImageLinearIp.h"
#include "ASMML2DSearchModel.h"


static char s_tmpbuf[256];

CASMML2DSearchModel::CASMML2DSearchModel(void) : CASMMLSearchModel()
{
    m_iLocalStructWidth = 0;
    m_iSearchWidth = 0;
    m_iUse2DStructNum = 0;
    m_iUse2DSearchNum = 0;
}

CASMML2DSearchModel::~CASMML2DSearchModel(void)
{

}

bool CASMML2DSearchModel::SearchOnePoint(int iPoint, double& desx, double& desy)
{
    double orgx, orgy; // the org point 
    double nx, ny;     // the search direction
    
    orgx = m_SearchShape.PointX(iPoint) / m_iCurLevelScale;
    orgy = m_SearchShape.PointY(iPoint) / m_iCurLevelScale;

    nx = (m_ShapeNormal.PointX(iPoint) );
    ny = (m_ShapeNormal.PointY(iPoint) );
    if (!m_ModelWithoutAlign)
    {
        nx *= m_S / m_SInTraining;
        ny *= m_S / m_SInTraining;
    }
    
    CASMPCALocalStruct& localstruct = LocalStruct(iPoint , m_iCurLevel);

    int iSearchPos;
    int iSearchPosY;
    CASMLocalProfile vCurrProfile;
    CASMLocalProfile LongProfile;

    double mincost = DBL_MAX;
    double minpos = 0.0;
    double minposy = 0.0;

    Get2DLongProfile(iPoint, LongProfile);

    int SearchWidth = 0;
    if (m_vUse2DSearch[iPoint] == 1)
    {
        SearchWidth = m_iSearchWidth;
    }

    // 2D search
    for (iSearchPosY =- SearchWidth; iSearchPosY <= SearchWidth; iSearchPosY++)
    {
        for (iSearchPos =- m_iSearchLength; iSearchPos <= m_iSearchLength; iSearchPos++)
        {
            // Get the current profile
            Get2DDetProfile(iPoint, LongProfile, vCurrProfile, iSearchPos, iSearchPosY);

            double currcost = localstruct.EvaluateLessThan(vCurrProfile, mincost);
            if (currcost < mincost)
            {
                mincost = currcost;
                minpos = iSearchPos;
                minposy = iSearchPosY;
            }
        }
    }

    desx = orgx + nx * minpos + ny * minposy;
    desy = orgy + ny * minpos - nx * minposy;

    desx *= m_iCurLevelScale;
    desy *= m_iCurLevelScale;

    if ( abs((int) minpos) <= m_iSearchLength / 2)
    {
        return true;
    }

    return false;
}


void CASMML2DSearchModel::Get2DLongProfile(int iPoint, CASMLocalProfile &profile)
{
    int length = m_iLocalStructLength + m_iSearchLength;
    int totlength = length * 2 + 1;
    int width = m_vUse2DLocalStruct[iPoint] != 0 ? m_iLocalStructWidth : 0;
    if (m_vUse2DSearch[iPoint] != 0)
    {
        width += m_iSearchWidth;
    }
    int totwidth = width * 2 + 1;
    
    profile.Resize(totlength * totwidth);
    
    double orgx, orgy; // the org point 
    double nx, ny;     // the search direction
    
    orgx = (m_SearchShape.PointX(iPoint)) / m_iCurLevelScale;
    orgy = (m_SearchShape.PointY(iPoint)) / m_iCurLevelScale;

    if (!m_ModelWithoutAlign)
    {   
        nx = (m_ShapeNormal.PointX(iPoint) * m_S) / m_SInTraining;
        ny = (m_ShapeNormal.PointY(iPoint) * m_S) / m_SInTraining;
    }
    else
    {
        nx = m_ShapeNormal.PointX(iPoint);
        ny = m_ShapeNormal.PointY(iPoint);
    }
    
    m_pSearchImg = &((*m_pImagePyramid)[m_iCurLevel]);

    CVisImageLinearIp ImageIp;
    ImageIp.SetSrc(m_pSearchImg);

    double oldx = orgx;
    double oldy = orgy;

    for (int iWidth = 0; iWidth < totwidth; iWidth++)
    {
        orgy = oldy - nx * (iWidth - width);
        orgx = oldx + ny * (iWidth - width);

        // the des point
        double x = orgx - nx * (length + 1 ) ;
        double y = orgy - ny * (length + 1 ) ;
        double olddata = ImageIp.EvaluateF(x, y);
        for (int iLength = 0; iLength < totlength; iLength++)
        {
            double curdata; // the des point
            x = orgx + nx * (iLength - length);
            y = orgy + ny * (iLength - length);

            curdata = ImageIp.EvaluateF(x, y); 
            if (m_LoacalStructType==0)
                profile[iWidth * totlength + iLength] = curdata - olddata;
            else
                profile[iWidth * totlength + iLength] = curdata;
            olddata = curdata;
        }
    }
    
}

void CASMML2DSearchModel::Get2DDetProfile(int iPoint, CASMLocalProfile &longprofile, CASMLocalProfile &profile, int detx, int dety)
{
    int longlength = m_iLocalStructLength + m_iSearchLength;
    int totlonglength = longlength * 2 + 1;
    int longwidth = m_vUse2DLocalStruct[iPoint] != 0 ? m_iLocalStructWidth : 0;
    if (m_vUse2DSearch[iPoint] != 0)
    {
        longwidth += m_iSearchWidth;
    }
    int totlongwidth = longwidth * 2 + 1;

    int length = m_iLocalStructLength;
    int totlength = 2 * length + 1;
    int width = m_vUse2DLocalStruct[iPoint] != 0 ? m_iLocalStructWidth : 0;
    int totwidth = 2 * width + 1;
    int SearchWidth = m_vUse2DSearch[iPoint] != 0 ? m_iSearchWidth : 0;

    profile.Resize(totwidth * totlength);

    for (int iWidth = 0; iWidth < totwidth; iWidth++)
    {
        memcpy(&profile[iWidth * totlength], &longprofile[totlonglength * (iWidth + SearchWidth + dety) + m_iSearchLength + detx], sizeof(double) * totlength);
    }

    profile.Normalize();
}   

COutputDataFile& Output(COutputDataFile& fs, const CASMML2DSearchModel& model)
{
	fs << ClassTag("CASMML2DSearchModel") << sep_endl;
	fs << (CASMMLSearchModel&) model;	
	fs << ClassTag("LocalStructWidth:") << model.m_iLocalStructWidth << sep_endl;
	fs << ClassTag("SearchWidth:") <<model.m_iSearchWidth << sep_endl;

	int i;
    int iNbPoint;
    iNbPoint = model.m_ConnectModel.PointNum();

	fs << ClassTag("Use2DStructNum") << model.m_iUse2DStructNum; 
	if (model.m_iUse2DStructNum > 0)
	{
		int tempi = 0;
		for (i = 0; i < iNbPoint; i++)
		{
			if (model.m_vUse2DLocalStruct[i] == 1)
			{
				fs << i;
				tempi++;
			}
		}
		if (tempi != model.m_iUse2DStructNum)
		{
			fs.SetError(1,"Use2DStructNum Error");
		}
    }

	fs << ClassTag("Use2DSearchNum") << model.m_iUse2DSearchNum;
	if (model.m_iUse2DSearchNum > 0 )
	{
		int tempi = 0;

		for (i = 0; i < iNbPoint; i++)
		{
			if (model.m_vUse2DSearch[i] == 1)
			{
				fs << i;
				tempi++;
			}
		}
		if (tempi != model.m_iUse2DSearchNum)
		{
			fs.SetError(1,"Use2DSearchNum Error");
		}
    }
	return fs;
}

CInputDataFile& Input(CInputDataFile& fs, CASMML2DSearchModel& model)
{
	fs >> ClassTag("CASMML2DSearchModel") >> sep_endl;
	fs >> (CASMMLSearchModel&) model;	
	fs >> ClassTag("LocalStructWidth:") >> model.m_iLocalStructWidth >> sep_endl;
	fs >> ClassTag("SearchWidth:") >>model.m_iSearchWidth >> sep_endl;

	int i;
    int iNbPoint;
    iNbPoint = model.m_ConnectModel.PointNum();

    model.m_vUse2DLocalStruct.resize(iNbPoint);
    model.m_vUse2DSearch.resize(iNbPoint);

    for (i = 0; i < iNbPoint; i++)
    {
        model.m_vUse2DLocalStruct[i] = 0;
        model.m_vUse2DSearch[i] = 0;
    }

	fs >> ClassTag("Use2DStructNum") >> model.m_iUse2DStructNum ; 
	for (i = 0; i < model.m_iUse2DStructNum; i++)
    {
		int tempi;
        fs >> tempi;
        model.m_vUse2DLocalStruct[tempi] = 1;
    }

	fs >> ClassTag("Use2DSearchNum") >> model.m_iUse2DSearchNum;
	for (i = 0; i < model.m_iUse2DSearchNum; i++)
    {
		int tempi;
		fs >> tempi;
        model.m_vUse2DSearch[tempi] = 1;
    }
	return fs;
}

COutputDataFile& operator<<(COutputDataFile& fs, const CASMML2DSearchModel& model)
{
	return Output(fs,model);
};

CInputDataFile& operator>>(CInputDataFile& fs, CASMML2DSearchModel& model)
{
	return Input(fs,model);
}