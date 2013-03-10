
// ASMPCALocalStruct.cpp: implementation of the CASMPCALocalStruct class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ASMModel.h"
#include "ASMPCALocalStruct.h"

CASMPCALocalStruct::CASMPCALocalStruct(void)
{
    m_TotLength = 0;
}

CASMPCALocalStruct::~CASMPCALocalStruct(void)
{

}

/*
bool CASMPCALocalStruct::ReadFile_HChen(FILE *fp)
{
    bool ret = false;

    // Load CASMPCALocalStruct
	if (fscanf(fp, "%d", &m_TotLength) == EOF) goto exit;

    // Init CPCA2Model
    if (!m_PCA2Model.ReadFile_HChen(fp)) goto exit;

    ret = true;

exit:
    return ret;
}

#ifdef DATA_TXT2BIN
bool CASMPCALocalStruct::TxtRead(FILE *fp)
{
    bool ret = false;

    // Load CASMPCALocalStruct
	if (fscanf(fp, "%d", &m_TotLength) == EOF) goto exit;

    // Init CPCA2Model
    if (!m_PCA2Model.TxtRead(fp)) goto exit;

    ret = true;

exit:
    return ret;
}

bool CASMPCALocalStruct::BinWrite(FILE *fp)
{
    bool ret = false;

    short tmp;

    tmp = m_TotLength;
    if (fwrite(&tmp, sizeof(short), 1, fp) != 1) goto exit;
    
    if (!m_PCA2Model.BinWrite(fp)) goto exit;

    ret = true;

exit:
    return ret;
}
#endif // DATA_TXT2BIN


BYTE* CASMPCALocalStruct::Decode(BYTE* buffer)
{
    m_TotLength = *((short *)buffer);
    buffer += sizeof(short);
    
    buffer = m_PCA2Model.Decode(buffer);
    
    return buffer;
}
*/

double CASMPCALocalStruct::EvaluateLessThan(CASMLocalProfile& srcprofile, double MinEnergy)
{
	assert(srcprofile.Length() == m_TotLength);
	return m_PCA2Model.EnergyLessThan(srcprofile, MinEnergy);
}

double CASMPCALocalStruct::LogP(CASMLocalProfile& srcprofile, double MaxLogP)
{
	assert(srcprofile.Length() == m_TotLength);
	return m_PCA2Model.LogP(srcprofile, MaxLogP);
}


COutputDataFile& Output(COutputDataFile& fs, const CASMPCALocalStruct& ls)
{
    fs << ls.m_TotLength << sep_endl;
	fs << ls.m_PCA2Model << sep_endl;
	return fs;
}

CInputDataFile& Input(CInputDataFile& fs, CASMPCALocalStruct& ls)
{
    fs >> ls.m_TotLength >> sep_endl;
	fs >> ls.m_PCA2Model >> sep_endl;
	return fs;
}

COutputDataFile& operator<<(COutputDataFile& fs, const CASMPCALocalStruct& ls)
{
	return Output(fs,ls);
};

CInputDataFile& operator>>(CInputDataFile& fs, CASMPCALocalStruct& ls)
{
	return Input(fs,ls);
}
