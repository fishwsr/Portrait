
// IndexedContour.cpp: implementation of the CIndexedContour class.
//
///////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "IndexedContour.h"

CIndexedContour::CIndexedContour(void)
{
    m_Type = 0;
    m_PointNum = 0;
    m_Index = NULL;
}

CIndexedContour::CIndexedContour(const int type, const int PointNum)
	:m_Type(type), m_PointNum(PointNum) 
{
	m_Index = new short[m_PointNum];
	memset(m_Index, 0, sizeof(short)*m_PointNum);
}

CIndexedContour::CIndexedContour(const CIndexedContour& ref)
: m_Type(ref.m_Type), m_PointNum(ref.m_PointNum)
{
	m_Index = new short[m_PointNum];
	memcpy(m_Index, ref.m_Index, sizeof(short)*m_PointNum);
}

const CIndexedContour& CIndexedContour::operator=(const CIndexedContour& ref)
{
	if ( this != &ref )
	{
		Clear();
		m_Type = ref.m_Type;
		m_PointNum = ref.m_PointNum;
		m_Index = new short[m_PointNum];
		memcpy(m_Index, ref.m_Index, sizeof(short)*m_PointNum);	
	}
	return *this;
}

bool CIndexedContour::operator ==(const CIndexedContour& ref) const
{
	if ( &ref==this ) return true;
	if ( m_Type!=ref.m_Type ) return false;
	if ( m_PointNum!=ref.m_PointNum ) return false;

	for ( int i=0; i<m_PointNum; i++ )
	{
		if ( m_Index[i]!=ref.m_Index[i] ) 
			return false;
	}

	return true;
}

CIndexedContour::~CIndexedContour(void)
{
    Clear();
}

/*
#ifdef DATA_TXT2BIN
bool CIndexedContour::TxtRead(FILE *fp)
{
    bool ret = false;

    int i;

    // Load CIndexedContour
    Clear();
    
	if (fscanf(fp, "%d %d", &m_Type, &m_PointNum) == EOF) goto exit;

    m_Index = new short[m_PointNum];
    if (!m_Index) goto exit;

	for (i = 0; i < m_PointNum; i++)
	{
		if (fscanf(fp, "%hd", &m_Index[i]) == EOF) goto exit;
	}

    ret = true;

exit:
    return ret;
}

bool CIndexedContour::TxtWrite(FILE *fp) const
{
	bool ret = false;

	int i;
	// Save CIndexedContour
	fprintf(fp, "%d %d \n", m_Type, m_PointNum);

	for (i = 0; i < m_PointNum; i++)
	{
		fprintf(fp, "%hd ", m_Index[i]);
	}
	fprintf(fp,"\n");
	ret = true;
	return ret;
}

bool CIndexedContour::BinRead(FILE *fp)
{
	bool ret = false;

	short tmp;
	Clear();

	if (fread(&tmp, sizeof(short), 1, fp) != 1) goto exit;
	m_Type = tmp;
    
	if (fread(&tmp, sizeof(short), 1, fp) != 1) goto exit;
	m_PointNum = tmp;

    m_Index = new short[m_PointNum];
	if (!m_Index) goto exit;

	if (fread(m_Index, sizeof(short), m_PointNum, fp) != m_PointNum) goto exit;

	ret = true;

exit:
	return ret;
}

bool CIndexedContour::BinWrite(FILE *fp) const
{
    bool ret = false;

    short tmp;

    tmp = m_Type;
    if (fwrite(&tmp, sizeof(short), 1, fp) != 1) goto exit;

    tmp = m_PointNum;
    if (fwrite(&tmp, sizeof(short), 1, fp) != 1) goto exit;
    
    if (fwrite(m_Index, sizeof(short), m_PointNum, fp) != m_PointNum) goto exit;

    ret = true;

exit:
    return ret;
}
#endif // DATA_TXT2BIN


BYTE* CIndexedContour::Decode(BYTE* buffer)
{
    m_Type = *((short *)buffer);
    buffer += sizeof(short);
    
    m_PointNum = *((short *)buffer);
    buffer += sizeof(short);
    
    m_Index = new short[m_PointNum];
    assert(m_Index);

    memcpy((void *)m_Index, buffer, m_PointNum * sizeof(short));
    buffer += m_PointNum * sizeof(short);
    
    return buffer;
}
*/

void CIndexedContour::Clear(void)
{
    if (m_Index)
    {
        delete []m_Index;
        m_Index = NULL;
    }
    m_Type = 0;
    m_PointNum = 0;
}

COutputDataFile& Output(COutputDataFile& fs, const CIndexedContour& c)
{
	int i;

	// Save CIndexedContour
	fs << c.m_Type << c.m_PointNum << sep_endl;
	for ( i=0; i<c.m_PointNum; i++ )
	{
		fs << int(c.m_Index[i]);
	}
	return fs;
}

CInputDataFile& Input(CInputDataFile& fs, CIndexedContour& c)
{
    c.Clear();

    // Load CIndexedContour
    fs >> c.m_Type >> c.m_PointNum >> sep_endl;
    c.m_Index = new short[c.m_PointNum];
	int tempi;
	for ( int i=0; i<c.m_PointNum; i++ )
	{
		fs >> tempi;
		c.m_Index[i] = tempi;
	}
	return fs;
}

COutputDataFile& operator<<(COutputDataFile& fs, const CIndexedContour& c)
{
	return Output(fs,c);
};

CInputDataFile& operator>>(CInputDataFile& fs, CIndexedContour& c)
{
	return Input(fs,c);
}

