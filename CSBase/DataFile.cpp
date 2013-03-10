
// DataFile.cpp: implementation of the CDataFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//typedef enum _Seperator{
//  blank,
//	comma,              // ,
//	colon,              // :
//	semicolon,          // ;
//	endl 	
//}Seperator;
const char * StrSeperator[5]= {
	" ",
	",",
	";",
	":",
	"\n"
};

ClassTag::ClassTag(const char * pstr)
{
	m_pStr = pstr;
}

const char * ClassTag::TagStr() const
{
	return m_pStr;
}


CDataFile::CDataFile(bool bTxtMode)
:m_iErrCode(0), m_pf(NULL), m_bTxtMode(bTxtMode), m_SepVector(sep_endl)
{
}

CDataFile::~CDataFile()
{

}

COutputDataFile::~COutputDataFile()
{
	if (m_pf)
	{
		fclose(m_pf);
	}
}

CInputDataFile::~CInputDataFile()
{
	if (m_pf)
	{
		fclose(m_pf);
	}
}


COutputDataFile::COutputDataFile(const char * fname, bool bTxtMode)
:CDataFile(bTxtMode)
{
	if (m_bTxtMode)
	{
		m_pf = fopen(fname, "wt");
	}
	else
	{
		m_pf = fopen(fname, "wb");	
	}
	if (!m_pf)
	{
		SetError(1);
	}
}
	
CInputDataFile::CInputDataFile(const char * fname, bool bTxtMode)
:CDataFile(bTxtMode)
{
	if (m_bTxtMode)
	{
		m_pf = fopen(fname, "rt");
	}
	else
	{
		m_pf = fopen(fname, "rb");
	}
	if (!m_pf)
	{
		SetError(2);
	}
}

COutputDataFile& COutputDataFile::operator<< (bool b)
{
	if (m_pf)
	{
		int tempi;
		tempi = b ? 1 : 0;
		(*this) << tempi;
	}
	return *this;
}

COutputDataFile& COutputDataFile::operator<< (char c)
{
	if (m_pf)
	{
		if (m_bTxtMode)
		{
			if ( 0 > fprintf(m_pf, "%c ",c))
			{
				SetError(3);
			}
		}
		else
		{
			if ( 1 != fwrite(&c, sizeof(char), 1, m_pf))
			{
				SetError(4);
			}
		}
	}
	return *this;
}

CInputDataFile& CInputDataFile::operator>>(char& c)
{
	if (m_pf)
	{
		if (m_bTxtMode)
		{
			int tempi;
			if ( EOF == fscanf(m_pf, "%c",&tempi))
			{
				SetError(5);
			}
			c = tempi;
		}
		else
		{
			if ( 1 != fread(&c, sizeof(char), 1, m_pf))
			{
				SetError(6);
			}
		}
	}
	return *this;
}

CInputDataFile& CInputDataFile::operator>> (bool& b)
{
	if (m_pf)
	{
		int tempi;
		(*this) >> tempi;
		b = (tempi == 1);
	}
	return *this;
}

COutputDataFile& COutputDataFile::operator<< (int i)
{
	if (m_pf)
	{
		if (m_bTxtMode)
		{
			if ( 0 > fprintf(m_pf, "%d ",i))
			{
				SetError(7);
			}
		}
		else
		{
			if ( 1 != fwrite(&i, sizeof(int), 1, m_pf))
			{
				SetError(8);
			}
		}
	}
	return *this;
}

COutputDataFile& COutputDataFile::operator<< (long l)
{
	if (m_pf)
	{
		if (m_bTxtMode)
		{
			if ( 0 > fprintf(m_pf, "%ld ",l))
			{
				SetError(7);
			}
		}
		else
		{
			if ( 1 != fwrite(&l, sizeof(long), 1, m_pf))
			{
				SetError(8);
			}
		}
	}
	return *this;
}

CInputDataFile& CInputDataFile::operator>> (int& i)
{
	if (m_pf)
	{
		if (m_bTxtMode)
		{
			if ( EOF ==  fscanf(m_pf, "%d ",&i))
			{
				SetError(9);
			}
		}
		else
		{
			if ( 1 != fread(&i, sizeof(int), 1, m_pf))
			{
				SetError(10);
			}
		}
	}
	return *this;
}

CInputDataFile& CInputDataFile::operator>> (long& l)
{
	if (m_pf)
	{
		if (m_bTxtMode)
		{
			if ( EOF ==  fscanf(m_pf, "%ld ",&l))
			{
				SetError(9);
			}
		}
		else
		{
			if ( 1 != fread(&l, sizeof(long), 1, m_pf))
			{
				SetError(10);
			}
		}
	}
	return *this;
}

COutputDataFile& COutputDataFile::operator<< (float f)
{
	if (m_pf)
	{
		if (m_bTxtMode)
		{
			if ( 0 > fprintf(m_pf, "%e ",f))
			{
				SetError(11);
			}
		}
		else
		{
			if ( 1 != fwrite(&f, sizeof(float), 1, m_pf))
			{
				SetError(12);
			}
		}
	}
	return *this;
}

CInputDataFile& CInputDataFile::operator>> (float& f)
{
	if (m_pf)
	{
		if (m_bTxtMode)
		{
			double tempf;
			if ( EOF ==  fscanf(m_pf, "%le ",&tempf))
			{
				SetError(13);
			}
			f = (float)tempf;
		}
		else
		{
			if ( 1 != fread(&f, sizeof(float), 1, m_pf))
			{
				SetError(14);
			}
		}
	}
	return *this;
}

COutputDataFile& COutputDataFile::operator<< (double d)
{
	if (m_pf)
	{
		if (m_bTxtMode)
		{
			if ( 0 > fprintf(m_pf, "%.20lf ",d))
			{
				SetError(15);
			}
		}
		else
		{
			if ( 1 != fwrite(&d, sizeof(double), 1, m_pf))
			{
				SetError(16);
			}
		}
	}
	return *this;
}

CInputDataFile& CInputDataFile::operator>>(double& d)
{
	if (m_pf)
	{
		if (m_bTxtMode)
		{
			if ( EOF ==  fscanf(m_pf, "%lf", &d))
			{
				SetError(17);
			}
		}
		else
		{
			if ( 1 != fread(&d, sizeof(double), 1, m_pf))
			{
				SetError(18);
			}
		}
	}
	return *this;
}

COutputDataFile& COutputDataFile::operator<<(const char * str)
{
	if (m_pf)
	{
		int len = strlen(str);
		if (m_bTxtMode)
		{
			if ( 0 > fprintf(m_pf, "%s ", str))
			{
				SetError(19);
			}
		}
		else
		{
			(*this)<<len;
			if ( len != fwrite(str, sizeof(char), len, m_pf))
			{
				SetError(20);
			}
		}
	}
	return *this;
}

CInputDataFile& CInputDataFile::operator>>(char * str)
{
	if (m_pf)
	{
		if (m_bTxtMode)
		{
			if ( EOF ==  fscanf(m_pf, "%s", str))
			{
				SetError(21);
			}
		}
		else
		{
			int len;
			(*this)>>len;
			if ( len != fread(str, sizeof(char), len, m_pf))
			{
				SetError(22);
			}
		}
	}
	return *this;
}

COutputDataFile& COutputDataFile::operator<<(Seperator sep)
{
	if (m_pf)	
	{
		if (m_bTxtMode)
		{
			(*this)<<StrSeperator[sep];
		}
	}
	return *this;
}

CInputDataFile& CInputDataFile::operator>>(Seperator sep)
{
	char tempbuf[1024];

	if (m_pf)	
	{
		if (m_bTxtMode)
		{
			if ((sep!=sep_space)&&(sep!=sep_endl))
			{
				(*this) >> tempbuf;
				if ( 0 != strcmp(tempbuf, StrSeperator[sep]))
				{
					SetError(23);
				}
			}
		}
	}
	return *this;
}

COutputDataFile& COutputDataFile::operator<< (const ClassTag& tag)
{
	if (m_pf)
	{
		if (m_bTxtMode)
		{
			(*this)<<tag.TagStr();
		}
	}
	return (*this);
}

CInputDataFile& CInputDataFile::operator>> (const ClassTag& tag)
{
	char tempbuf[1024];
	if (m_pf)
	{
		if (m_bTxtMode)
		{
			(*this)>>tempbuf;
			if ( 0 != strcmp(tempbuf, tag.TagStr()))
			{
				SetError(24);
			}
		}
	}
	return (*this);
}

COutputDataFile& COutputDataFile::operator<< (short si)
{
	if (m_pf)
	{
		if (m_bTxtMode)
		{
			if ( 0 > fprintf(m_pf, "%d ",si))
			{
				SetError(7);
			}
		}
		else
		{
			if ( 1 != fwrite(&si, sizeof(short), 1, m_pf))
			{
				SetError(8);
			}
		}
	}
	return *this;
}

CInputDataFile& CInputDataFile::operator>> (short& si)
{
	if (m_pf)
	{
		if (m_bTxtMode)
		{
			int tempi;
			if ( 0 > fscanf(m_pf, "%d",&tempi))
			{
				SetError(7);
			}
			si = tempi;
		}
		else
		{
			if ( 1 != fread(&si, sizeof(short), 1, m_pf))
			{
				SetError(8);
			}
		}
	}
	return *this;
}

COutputDataFile& operator<<(COutputDataFile& ofs, const CVisDVector& vec)
{
	if (!ofs.HasError())
	{
		int len = vec.Length();
		ofs<<len;

		for (int i=0; i<len; i++)
			ofs<<vec[i];

		//ofs<<sep_endl;
	}
	return ofs;
}

CInputDataFile& operator>>(CInputDataFile& ifs, CVisDVector& vec)
{
	if (!ifs.HasError())
	{
		int len;
		ifs>>len;

		vec.Resize(len);
		for (int i=0; i<len; i++)
			ifs>>vec[i];

		//ifs>>sep_endl;
	}
	return ifs;
}

COutputDataFile& operator<<(COutputDataFile& ofs, const CVisDMatrix& mat)
{
	if (!ofs.HasError())
	{
		int NCol, NRow;
		NRow = mat.NRows();
		NCol = mat.NCols();
		ofs << NRow << NCol << sep_endl;
		
		for (int r = 0; r < NRow; r++ )
		{
			for (int c = 0; c < NCol; c++)
			{
				ofs << mat[r][c];
			}
			if (r!=NRow)
			{
				ofs<< sep_endl;
			}
		}
	}
	return ofs;
}

CInputDataFile& operator>>(CInputDataFile& ifs, CVisDMatrix& mat)
{
	if (!ifs.HasError())
	{
		int NCol, NRow;
		ifs>> NRow >> NCol >> sep_endl;
		mat.Resize(NRow, NCol);

		for (int r = 0; r < NRow; r++ )
		{
			for (int c = 0; c < NCol; c++ )
			{
				ifs >> mat[r][c];
			}
			if ( r != NRow)
			{
				ifs >> sep_endl;
			}
		}
	}
	return ifs;
}

/////////////////// MyCountTime ////////////////
CCountingTime::CCountingTime()
{
	Reset();
}

CCountingTime::~CCountingTime()
{
}

double CCountingTime::GetHour()
{
	return GetMin() / 60.;		
}

double CCountingTime::GetMin()
{
	endtime = clock();
	return GetSec() / 60.;
}

double CCountingTime::GetSec()
{
	endtime = clock();
	return (endtime - sttime) / CLOCKS_PER_SEC;
}

void CCountingTime::Reset()
{
	sttime = clock();
	endtime = sttime;
}

