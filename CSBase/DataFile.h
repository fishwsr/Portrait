
// DataFile.h: interface for the CDataFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAFILE_H__A9B9E725_CF67_46EE_95A9_C74451659B53__INCLUDED_)
#define AFX_DATAFILE_H__A9B9E725_CF67_46EE_95A9_C74451659B53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef enum _Seperator{
	sep_space, 
	sep_comma,              // ,
	sep_colon,              // :
	sep_semicolon,          // ;
	sep_endl 	
}Seperator;

class CSBASE_API ClassTag{
public:
	ClassTag(const char * pstr);
	const char * TagStr() const;
private:
	const char * m_pStr;
};


class CSBASE_API CDataFile  
{
public:
	CDataFile(bool bTxtMode = true);
	virtual ~CDataFile();
	bool HasError() const {return (m_iErrCode!=0)||(m_pf==NULL);};
	int ErrorCode() const {return m_iErrCode;};
	void SetError(int errcode = 1, const char * errmsg = "Unknown" ) {m_iErrCode = errcode; printf("\n Error: %d, %s", errcode, errmsg );};

	void SetSepVector(Seperator sep) { m_SepVector = sep; };
	const Seperator& SepVector() {return m_SepVector; };
protected:
	Seperator m_SepVector;
	FILE * m_pf;
	bool m_bTxtMode;
	int  m_iErrCode;
};

class CSBASE_API COutputDataFile : public CDataFile
{
public:
	COutputDataFile(const char * fname, bool bTxtMode = true);
	virtual ~COutputDataFile();

	virtual COutputDataFile& operator<< (bool b);
	virtual COutputDataFile& operator<< (char c);
	virtual COutputDataFile& operator<< (short si);
	virtual COutputDataFile& operator<< (int i);
	virtual COutputDataFile& operator<< (long l);
	virtual COutputDataFile& operator<< (float f);
	virtual COutputDataFile& operator<< (double d);
	
	virtual COutputDataFile& operator<< (const char * str);
	virtual COutputDataFile& operator<< (Seperator sep);
	virtual COutputDataFile& operator<< (const ClassTag& tag);
};

class CSBASE_API CInputDataFile : public CDataFile
{
public:
	CInputDataFile(const char * fname, bool bTxtMode = true);
	virtual ~CInputDataFile();

	virtual CInputDataFile& operator>> (bool& b);
	virtual CInputDataFile& operator>> (char& c);
	virtual CInputDataFile& operator>> (int& i);
	virtual CInputDataFile& operator>> (long& l);
	virtual CInputDataFile& operator>> (short& si);
	virtual CInputDataFile& operator>> (float& f);
	virtual CInputDataFile& operator>> (double& d);

	virtual CInputDataFile& operator>> (char * str);
	virtual CInputDataFile& operator>> (Seperator sep);
	virtual CInputDataFile& operator>> (const ClassTag& tag);
};

CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const CVisDVector& vec);
CSBASE_API COutputDataFile& operator<<(COutputDataFile& ofs, const CVisDMatrix& mat);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, CVisDVector& vec);
CSBASE_API CInputDataFile& operator>>(CInputDataFile& ifs, CVisDMatrix& mat);

template<class TClass>
COutputDataFile& operator<<(COutputDataFile& ofs, const std::vector<TClass>& vData)
{
	int nData = vData.size();
	ofs << nData << sep_endl;
	for (int i = 0; i< nData; i++)
	{
		ofs << vData[i] << ofs.SepVector();
	}
	return ofs;
}

template<class TClass>
CInputDataFile& operator>>(CInputDataFile& ifs, std::vector<TClass>& vData)
{
	int nData;
	ifs >> nData >> sep_endl;
	vData.resize(nData);
	
	for (int i = 0; i< nData; i++)
	{
		ifs >> vData[i] >> ifs.SepVector();
	}
	return ifs;
}

template<class TClass>
void SaveToFile(const char * fname, const TClass& data, bool bTxtMode = true)
{
	COutputDataFile ofs(fname, bTxtMode);
	ofs << data;
}

template<class TClass>
void LoadFromFile(const char * fname, TClass& data, bool bTxtMode = true)
{
	CInputDataFile ifs(fname, bTxtMode);
	if (!ifs.HasError())
	{
		ifs >> data;
	}
}

class CSBASE_API CCountingTime
{
public:
	CCountingTime();
	virtual ~CCountingTime();
	
	void    Reset();
	double  GetSec();
	double  GetMin();
	double  GetHour();	
	
protected:
	
	clock_t sttime;
	clock_t endtime;
};

#endif // !defined(AFX_DATAFILE_H__A9B9E725_CF67_46EE_95A9_C74451659B53__INCLUDED_)
