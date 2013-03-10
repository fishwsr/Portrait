// XmlFile.h: interface for the CXmlFile class.
//
//////////////////////////////////////////////////////////////////////


#include "xmlParserSimple.h"
#include "../Portrait EditingDlg.h"	// Added by ClassView

#if !defined(AFX_XMLFILE_H__E8470068_0E21_4F84_BFCC_BD969783365C__INCLUDED_)
#define AFX_XMLFILE_H__E8470068_0E21_4F84_BFCC_BD969783365C__INCLUDED_

#if _MSC_VER > 100
#pragma once
#endif // _MSC_VER > 1000

class CXmlFile  
{
public:
	CXmlFile();
	virtual ~CXmlFile();
	
public:
	char ReverseConvertChar2Char(char chSrc);
	void DecodeBase64(char *pDes, const char *pSrc, int nLength);
	char ConvertChar2Char(char chSrc);
	void EncodeBase64(char *pDes,  char *pSrc, int nLength);
	unsigned short* CString2Short(CString sFilename);

	void AddBitmapAttribute(XMLNode & node, CvvImage &img);
	void ReadBitmapAttribute2(XMLNode & node, CString sSaveFilename);

	inline void CXmlFile::AddIntegerAttribute(XMLNode & node, char *name, int value)
	{
		CString s;
		s.Format("%d", value);
		node.addAttribute(stringDup(_T(name)), stringDup(s));
	}

	inline void CXmlFile::AddDoubleAttribute(XMLNode & node, char *name, double value)
	{
		CString s;
		s.Format("%.6f", value);
		node.addAttribute(stringDup(_T(name)), stringDup(s));
	}

	void SaveXmlTemplate(CString sFilename, CvvImage &img);
	void LoadXmlTemplate(CString sFilename, CvvImage &img);

	void LoadFeaturePoint(XMLNode & ndFather);
	void LoadSourceImages(XMLNode & ndFather);
	void LoadGUIState(XMLNode &ndFather);
	void LoadTemplateInfo(XMLNode &ndFather);
	void ReadXml(CString sFilename);

	void SaveFeaturePoints(XMLNode & ndFather);
	void SaveSourceImages(XMLNode & ndFather, bool bSaveSSrc);
	void SaveGUIState(XMLNode &ndFather);
	void SaveTemplateInfo(XMLNode &ndFather);
	void SaveAsXml(CString sFilename);

	vector<CPoint> m_vPnt105; 
	vector<dPoint> m_vPnt91;
	vector<dPoint> m_vPntBgContour;
	vector<dPoint> m_vPntSkinContour;
	vector<dPoint> m_vPntHairContour;
	vector<dPoint> m_vPntClothContour;
	vector<dPoint> m_vPntLEarContour;
	vector<dPoint> m_vPntREarContour;

	int m_nBackground;
	int m_nSkin;
	int m_nHair;
	int m_nCloth;
	int m_nEar;
	int m_nHairIndex;
	int m_nClothIndex;
	int m_nCompIDs[8];

	CString m_sSrcImgFilename;

};

#endif // !defined(AFX_XMLFILE_H__E8470068_0E21_4F84_BFCC_BD969783365C__INCLUDED_)
