
// VisImageIp.h: interface for the CVisImageIp class.
//
///////////////////////////////////////////////////////////////////////////


#ifndef __MINIVIS_IMAGEIP_H__
#define __MINIVIS_IMAGEIP_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CSBASE_API CVisImageIp
{
public:
    CVisImageIp(void) { m_pSrc = NULL; }
    virtual ~CVisImageIp(void) { }

public:
    void SetSrc(CVisGrayByteImage *pSrc) { m_pSrc = pSrc; }

protected:
    bool InImage(int x, int y)
    {
        if (m_pSrc) return m_pSrc->ContainsPoint(x, y);
        else return false;
    }

public:
    virtual double EvaluateF(double x, double y) = 0;

    BYTE EvaluateB(double x, double y)
    {
		double tempf = EvaluateF(x, y);
        return tempf>255?255:(tempf<0?0:(BYTE)tempf);
    }

protected:
    CVisGrayByteImage *m_pSrc;
};


#endif // __MINIVIS_IMAGEIP_H__
