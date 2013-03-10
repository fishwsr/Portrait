
// ASMLocalProfile.cpp: implementation of the CASMLocalProfile class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ASMModel.h"
#include "ASMLocalProfile.h"


CASMLocalProfile::CASMLocalProfile(void)
{

}

CASMLocalProfile::~CASMLocalProfile(void)
{

}

//  We normalise the sample by dividing through by the sum of the absolute 
//  element value.
void CASMLocalProfile::Normalize(void)
{
    double sum = 0.0;
    int iSize = Length();
    
    for (int i = 0; i < iSize; i++)
    {
        sum += fabs((*this)[i]);
    }

    if (sum < 0.000001) sum = 1.0;
    double scale = (1.0 / sum) * iSize;
    
    for (i = 0; i < iSize; i++)
    {
        (*this)[i] *= scale;
    }
}
