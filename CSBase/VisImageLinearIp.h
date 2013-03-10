
// VisImageLinearIp.h: interface for the CVisImageLinearIp class.
//
///////////////////////////////////////////////////////////////////////////


#ifndef __MINIVIS_IMAGELINEARIP_H__
#define __MINIVIS_IMAGELINEARIP_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CSBASE_API CVisImageLinearIp : public CVisImageIp
{
public:
    CVisImageLinearIp(void) : CVisImageIp() { }
    virtual ~CVisImageLinearIp(void) { }

public:
    virtual double EvaluateF(double x, double y)
    {
        int x1 = (int)floor(x);
        int y1 = (int)floor(y);
        int x2 = x1 + 1;
        int y2 = y1 + 1;
        double dx = x - x1;
        double dy = y - y1;
    
        if (InImage(x1, y1) && InImage(x2, y2))
        {
            BYTE px1y1 = m_pSrc->Pixel(x1, y1);
            BYTE px1y2 = m_pSrc->Pixel(x1, y2);
            BYTE px2y2 = m_pSrc->Pixel(x2, y2);
            BYTE px2y1 = m_pSrc->Pixel(x2, y1);
            return (px1y1 * (1 - dx) + px2y1 * dx) * (1 - dy) + (px1y2 * (1 - dx) + px2y2 * dx) * dy;
        }
    
        return 0.0;
    }
};


#endif // __MINIVIS_IMAGELINEARIP_H__
