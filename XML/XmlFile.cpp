// XmlFile.cpp: implementation of the CXmlFile class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "../Portrait Editing.h"
#include "XmlFile.h"
#include "xmlParserSimple.h"
#include <iostream>
#include <fstream>
#include <atlbase.h>

using std::ofstream;
using std::endl;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXmlFile::CXmlFile()
{
	m_nBackground = 0;
	m_nSkin = 0;
	m_nHair = 0;
	m_nCloth = 0;
	m_nEar = 0;
	m_nHairIndex = 0;
	m_nClothIndex = 0;

	for(int i = 0; i < 8; i++)
	{
		m_nCompIDs[i] = 0;
	}
}

CXmlFile::~CXmlFile()
{

}

void CXmlFile::SaveAsXml(CString sFilename)
{
	try
	{
		ofstream ofs(sFilename);

		XMLNode ndTop = XMLNode::createXMLTopNode();
		XMLNode ndRoot = ndTop.addChild(stringDup(_T("xml")), 1);
		ndRoot.addAttribute(stringDup(_T("version")), stringDup(_T("1.0")));
		ndRoot.addAttribute(stringDup(_T("encoding")), stringDup(_T("UTF-8")));
		ndRoot.addAttribute(stringDup(_T("standalone")), stringDup(_T("no")));

		XMLNode ndP2AOData = ndRoot.addChild(stringDup(_T("P2AOData")));
		CTime t = CTime::GetCurrentTime();
		ndP2AOData.addAttribute(stringDup(_T("date")), stringDup(t.Format("%m/%d/%y")));
		ndP2AOData.addAttribute(stringDup(_T("time")), stringDup(t.Format("%H:%M:%S")));

		SaveSourceImages(ndP2AOData, false);
		SaveFeaturePoints(ndP2AOData);
		SaveGUIState(ndP2AOData);
		SaveTemplateInfo(ndP2AOData);

		char *pStrXML = ndTop.createXMLString(0);
		ofs << pStrXML << endl;
		free(pStrXML);
		ofs.close();
	}
	catch (...)	{
		AfxMessageBox("保存视软艺术工程文件失败！请重试。");
	}

}

void CXmlFile::SaveGUIState(XMLNode &ndFather)
{
	XMLNode ndGUIState = ndFather.addChild(stringDup(_T("GUIState")));
	AddIntegerAttribute(ndGUIState, "Background", m_nBackground);
	AddIntegerAttribute(ndGUIState, "Skin", m_nHair);
	AddIntegerAttribute(ndGUIState, "Hair", m_nHair);
	AddIntegerAttribute(ndGUIState, "Cloth", m_nCloth);
	AddIntegerAttribute(ndGUIState, "Ear", m_nEar);
}

void CXmlFile::SaveTemplateInfo(XMLNode &ndFather)
{
	XMLNode ndTemplate = ndFather.addChild(stringDup(_T("Template")));
	AddIntegerAttribute(ndTemplate, "HairIndex", m_nHairIndex);
	AddIntegerAttribute(ndTemplate, "ClothIndex", m_nClothIndex);
	AddIntegerAttribute(ndTemplate, "HairID", m_nCompIDs[0]);
	AddIntegerAttribute(ndTemplate, "EyebrowID", m_nCompIDs[1]);
	AddIntegerAttribute(ndTemplate, "EyeID", m_nCompIDs[2]);
	AddIntegerAttribute(ndTemplate, "NoseID", m_nCompIDs[3]);
	AddIntegerAttribute(ndTemplate, "MouthID", m_nCompIDs[4]);
	AddIntegerAttribute(ndTemplate, "ClothID", m_nCompIDs[5]);
	AddIntegerAttribute(ndTemplate, "EarID", m_nCompIDs[6]);
	AddIntegerAttribute(ndTemplate, "FaceContourID", m_nCompIDs[7]);
}


void CXmlFile::SaveSourceImages(XMLNode & ndFather, bool bSaveSSrc)
{
	XMLNode ndSrc = ndFather.addChild(stringDup(_T("Src")));
	CvvImage imgSrc;
	imgSrc.Load(m_sSrcImgFilename);
	AddBitmapAttribute(ndSrc, imgSrc);

}



void CXmlFile::SaveFeaturePoints(XMLNode & ndFather)
{
	//105
	XMLNode ndPoint105 = ndFather.addChild(stringDup(_T("Point105")));
	int size = 105;
	AddIntegerAttribute(ndPoint105, "size", size);

	for (int j = 0; j < size; j++)
	{
		XMLNode ndPoint = ndPoint105.addChild(stringDup(_T("Point")));

		AddIntegerAttribute(ndPoint, "x", m_vPnt105[j].x);
		AddIntegerAttribute(ndPoint, "y", m_vPnt105[j].y);
	}

	//91
	XMLNode ndPoint91 = ndFather.addChild(stringDup(_T("Point91")));
	size = 91;
	AddIntegerAttribute(ndPoint91, "size", size);

	for (j = 0; j < size; j++)
	{
		XMLNode ndPoint = ndPoint91.addChild(stringDup(_T("Point")));

		AddDoubleAttribute(ndPoint, "x", m_vPnt91[j].x);
		AddDoubleAttribute(ndPoint, "y", m_vPnt91[j].y);
	}

	//background
	XMLNode ndPointBg = ndFather.addChild(stringDup(_T("PointBackground")));
	size = m_vPntBgContour.size();
	AddIntegerAttribute(ndPointBg, "size", size);

	for (j = 0; j < size; j++)
	{
		XMLNode ndPoint = ndPointBg.addChild(stringDup(_T("Point")));

		AddDoubleAttribute(ndPoint, "x", m_vPntBgContour[j].x);
		AddDoubleAttribute(ndPoint, "y", m_vPntBgContour[j].y);
	}

	//skin
	XMLNode ndPointSkin = ndFather.addChild(stringDup(_T("PointSkin")));
	size = m_vPntSkinContour.size();
	AddIntegerAttribute(ndPointSkin, "size", size);

	for (j = 0; j < size; j++)
	{
		XMLNode ndPoint = ndPointSkin.addChild(stringDup(_T("Point")));

		AddDoubleAttribute(ndPoint, "x", m_vPntSkinContour[j].x);
		AddDoubleAttribute(ndPoint, "y", m_vPntSkinContour[j].y);
	}

	//hair
	XMLNode ndPointHair = ndFather.addChild(stringDup(_T("PointHair")));
	size = m_vPntHairContour.size();
	AddIntegerAttribute(ndPointHair, "size", size);

	for (j = 0; j < size; j++)
	{
		XMLNode ndPoint = ndPointHair.addChild(stringDup(_T("Point")));

		AddDoubleAttribute(ndPoint, "x", m_vPntHairContour[j].x);
		AddDoubleAttribute(ndPoint, "y", m_vPntHairContour[j].y);
	}

	//cloth
	XMLNode ndPointCloth = ndFather.addChild(stringDup(_T("PointCloth")));
	size = m_vPntClothContour.size();
	AddIntegerAttribute(ndPointCloth, "size", size);

	for (j = 0; j < size; j++)
	{
		XMLNode ndPoint = ndPointCloth.addChild(stringDup(_T("Point")));

		AddDoubleAttribute(ndPoint, "x", m_vPntClothContour[j].x);
		AddDoubleAttribute(ndPoint, "y", m_vPntClothContour[j].y);
	}

	//left ear
	XMLNode ndPointLEar = ndFather.addChild(stringDup(_T("PointLEar")));
	size = m_vPntLEarContour.size();
	AddIntegerAttribute(ndPointLEar, "size", size);

	for (j = 0; j < size; j++)
	{
		XMLNode ndPoint = ndPointLEar.addChild(stringDup(_T("Point")));

		AddDoubleAttribute(ndPoint, "x", m_vPntLEarContour[j].x);
		AddDoubleAttribute(ndPoint, "y", m_vPntLEarContour[j].y);
	}

	//right ear
	XMLNode ndPointREar = ndFather.addChild(stringDup(_T("PointREar")));
	size = m_vPntREarContour.size();
	AddIntegerAttribute(ndPointREar, "size", size);

	for (j = 0; j < size; j++)
	{
		XMLNode ndPoint = ndPointREar.addChild(stringDup(_T("Point")));

		AddDoubleAttribute(ndPoint, "x", m_vPntREarContour[j].x);
		AddDoubleAttribute(ndPoint, "y", m_vPntREarContour[j].y);
	}
}


void CXmlFile::ReadXml(CString sFilename)
{
	try
	{
		XMLNode ndP2AOData = XMLNode::openFileHelper(sFilename, _T("P2AOData"));

		LoadSourceImages(ndP2AOData);
		LoadFeaturePoint(ndP2AOData);
		LoadGUIState(ndP2AOData);
		LoadTemplateInfo(ndP2AOData);
	}
	catch (...)
	{
		AfxMessageBox("打开视软艺术工程文件失败！可能该文件已经损坏，请重试。");
	}

}


void CXmlFile::LoadSourceImages(XMLNode & ndFather)
{
	XMLNode ndSrc = ndFather.getChildNode("Src");
	ReadBitmapAttribute2(ndSrc, m_sSrcImgFilename);
}


void CXmlFile::LoadFeaturePoint(XMLNode & ndFather)
{
	//105
	int iStrokeNode = 0;
	XMLNode ndPoint105 = ndFather.getChildNode("Point105", &iStrokeNode);
	int size = atoi(ndPoint105.getAttribute("size"));

	int iPointNode = 0;
	m_vPnt105.clear();
	for (int j = 0; j < size; j++)
	{
		XMLNode ndPoint = ndPoint105.getChildNode("Point", &iPointNode);
		
		CPoint opt;
		opt.x = atoi(ndPoint.getAttribute("x"));
		opt.y = atoi(ndPoint.getAttribute("y"));
		m_vPnt105.push_back(opt);
	}

	//91
	iStrokeNode = 0;
	XMLNode ndPoint91 = ndFather.getChildNode("Point91", &iStrokeNode);
	size = atoi(ndPoint91.getAttribute("size"));

	iPointNode = 0;
	m_vPnt91.clear();
	for (j = 0; j < size; j++)
	{
		XMLNode ndPoint = ndPoint91.getChildNode("Point", &iPointNode);
		
		dPoint opt;
		opt.x = atof(ndPoint.getAttribute("x"));
		opt.y = atof(ndPoint.getAttribute("y"));
		m_vPnt91.push_back(opt);
	}

	//background
	iStrokeNode = 0;
	XMLNode ndPointBg = ndFather.getChildNode("PointBackground", &iStrokeNode);
	size = atoi(ndPointBg.getAttribute("size"));

	iPointNode = 0;
	m_vPntBgContour.clear();
	for (j = 0; j < size; j++)
	{
		XMLNode ndPoint = ndPointBg.getChildNode("Point", &iPointNode);
		
		dPoint opt;
		opt.x = atof(ndPoint.getAttribute("x"));
		opt.y = atof(ndPoint.getAttribute("y"));
		m_vPntBgContour.push_back(opt);
	}

	//skin
	iStrokeNode = 0;
	XMLNode ndPointSkin = ndFather.getChildNode("PointSkin", &iStrokeNode);
	size = atoi(ndPointSkin.getAttribute("size"));

	iPointNode = 0;
	m_vPntSkinContour.clear();
	for (j = 0; j < size; j++)
	{
		XMLNode ndPoint = ndPointSkin.getChildNode("Point", &iPointNode);
		
		dPoint opt;
		opt.x = atof(ndPoint.getAttribute("x"));
		opt.y = atof(ndPoint.getAttribute("y"));
		m_vPntSkinContour.push_back(opt);
	}

	//hair
	iStrokeNode = 0;
	XMLNode ndPointHair = ndFather.getChildNode("PointHair", &iStrokeNode);
	size = atoi(ndPointHair.getAttribute("size"));

	iPointNode = 0;
	m_vPntHairContour.clear();
	for (j = 0; j < size; j++)
	{
		XMLNode ndPoint = ndPointHair.getChildNode("Point", &iPointNode);
		
		dPoint opt;
		opt.x = atof(ndPoint.getAttribute("x"));
		opt.y = atof(ndPoint.getAttribute("y"));
		m_vPntHairContour.push_back(opt);
	}

	//cloth
	iStrokeNode = 0;
	XMLNode ndPointCloth = ndFather.getChildNode("PointCloth", &iStrokeNode);
	size = atoi(ndPointCloth.getAttribute("size"));

	iPointNode = 0;
	m_vPntClothContour.clear();
	for (j = 0; j < size; j++)
	{
		XMLNode ndPoint = ndPointCloth.getChildNode("Point", &iPointNode);
		
		dPoint opt;
		opt.x = atof(ndPoint.getAttribute("x"));
		opt.y = atof(ndPoint.getAttribute("y"));
		m_vPntClothContour.push_back(opt);
	}

	//left ear
	iStrokeNode = 0;
	XMLNode ndPointLEar = ndFather.getChildNode("PointLEar", &iStrokeNode);
	size = atoi(ndPointLEar.getAttribute("size"));

	iPointNode = 0;
	m_vPntLEarContour.clear();
	for (j = 0; j < size; j++)
	{
		XMLNode ndPoint = ndPointLEar.getChildNode("Point", &iPointNode);
		
		dPoint opt;
		opt.x = atof(ndPoint.getAttribute("x"));
		opt.y = atof(ndPoint.getAttribute("y"));
		m_vPntLEarContour.push_back(opt);
	}

	//right ear
	iStrokeNode = 0;
	XMLNode ndPointREar = ndFather.getChildNode("PointREar", &iStrokeNode);
	size = atoi(ndPointREar.getAttribute("size"));

	iPointNode = 0;
	m_vPntREarContour.clear();
	for (j = 0; j < size; j++)
	{
		XMLNode ndPoint = ndPointREar.getChildNode("Point", &iPointNode);
		
		dPoint opt;
		opt.x = atof(ndPoint.getAttribute("x"));
		opt.y = atof(ndPoint.getAttribute("y"));
		m_vPntREarContour.push_back(opt);
	}
}


void CXmlFile::AddBitmapAttribute(XMLNode & node, CvvImage &img)
{
	AddIntegerAttribute(node, "width", img.Width());
	AddIntegerAttribute(node, "height", img.Height());

	int nLength = img.GetImage()->widthStep * img.Height();
	AddIntegerAttribute(node, "length", nLength);
	char *pBuffer = img.GetImage()->imageData;


	int nBase64Length = nLength * 2 + 1;
	char *pBufBase64 = new char[nBase64Length];
	EncodeBase64(pBufBase64, pBuffer, nLength);	
	node.addAttribute(stringDup(_T("data")), stringDup(pBufBase64));

	delete[] pBufBase64;		
}



void CXmlFile::ReadBitmapAttribute2(XMLNode & node, CString sSaveFilename)
{
	int width = atoi(node.getAttribute(_T("width")));
	int height = atoi(node.getAttribute(_T("height")));
	int nLength = atoi(node.getAttribute(_T("length")));

	CvvImage pImg;
	pImg.Create(width, height, 24);

	const char *test = reinterpret_cast<LPCSTR>(node.getAttribute(_T("data")));

	char *pBuf = new char[nLength];
	DecodeBase64(pBuf, test, nLength);
	
	int nWidthStep = pImg.GetImage()->widthStep;
	for (int h = 0; h < height; h++)
	{
		for (int w  = 0; w < width; w++)
		{
			int nPos = h * nWidthStep + w * 3;
			pImg.GetImage()->imageData[nPos] = pBuf[nPos];
			pImg.GetImage()->imageData[nPos + 1] = pBuf[nPos + 1];
			pImg.GetImage()->imageData[nPos + 2] = pBuf[nPos + 2];
		}
	}

	delete[] pBuf;
	pImg.Save(sSaveFilename);
}


void CXmlFile::LoadGUIState(XMLNode &ndFather)
{
	XMLNode ndGUIState = ndFather.getChildNode("GUIState");
	m_nBackground = atoi(ndGUIState.getAttribute("Background"));
	m_nSkin = atoi(ndGUIState.getAttribute("Skin"));
	m_nHair = atoi(ndGUIState.getAttribute("Hair"));
	m_nCloth = atoi(ndGUIState.getAttribute("Cloth"));
	m_nEar = atoi(ndGUIState.getAttribute("Ear"));
}

void CXmlFile::LoadTemplateInfo(XMLNode &ndFather)
{
	XMLNode ndTemplate = ndFather.getChildNode("Template");
	m_nHairIndex = atoi(ndTemplate.getAttribute("HairIndex"));
	m_nClothIndex = atoi(ndTemplate.getAttribute("ClothIndex"));
	m_nCompIDs[0] = atoi(ndTemplate.getAttribute("HairID"));
	m_nCompIDs[1] = atoi(ndTemplate.getAttribute("EyebrowID"));
	m_nCompIDs[2] = atoi(ndTemplate.getAttribute("EyeID"));
	m_nCompIDs[3] = atoi(ndTemplate.getAttribute("NoseID"));
	m_nCompIDs[4] = atoi(ndTemplate.getAttribute("MouthID"));
	m_nCompIDs[5] = atoi(ndTemplate.getAttribute("ClothID"));
	m_nCompIDs[6] = atoi(ndTemplate.getAttribute("EarID"));
	m_nCompIDs[7] = atoi(ndTemplate.getAttribute("FaceContourID"));
}

unsigned short* CXmlFile::CString2Short(CString sFilename)
{
	unsigned short *sTemp = new unsigned short[sFilename.GetLength()];
	for (int i = 0; i < sFilename.GetLength(); i++ )
		sTemp[i] = (unsigned short)sFilename.GetAt(i);
	return sTemp;
}

void CXmlFile::EncodeBase64(char *pDes, char *pSrc, int nLength)
{
	for (int i = 0; i < nLength; i++)
	{
		BYTE bt = pSrc[i];
		
		char bt1 = bt % 64;
		char ch1 = ConvertChar2Char(bt1);
		pDes[i * 2 + 1] = ch1;
		bt /= 64;

		char bt2 = bt % 64;
		char ch2 = ConvertChar2Char(bt2);
		pDes[i * 2] = ch2;
	}

	pDes[nLength * 2] = '\0';
}

char CXmlFile::ConvertChar2Char(char chSrc)
{
	BYTE bt = (BYTE)chSrc;
	
	if ( (bt >= 0) && (bt < 26) )
		return  'A' + bt;
	else if ( (bt >= 26) && (bt < 52) )
		return 'a' + bt-26;
	else if ( (bt >= 52) && (bt < 62) )
		return '0' + bt-52;
	else if ( bt < 63)
		return ',';
	else
		return '.';


}

void CXmlFile::DecodeBase64(char *pDes, const char *pSrc, int nLength)
{
	for (int i = 0; i < nLength; i++)
	{
		BYTE bt1 = pSrc[i * 2];
		BYTE bt2 = pSrc[i * 2 + 1];

		int ch1 = ReverseConvertChar2Char(bt1);
		int ch2 = ReverseConvertChar2Char(bt2);

		pDes[i] = ch1 * 64 + ch2;
		
	}
}



char CXmlFile::ReverseConvertChar2Char(char chSrc)
{
	BYTE bt = (BYTE)chSrc;
	
	if ( (bt >= 'A') && (bt <= 'Z') )
		return  0 + bt - 'A';
	else if ( (bt >= 'a') && (bt <= 'z') )
		return 26 + bt - 'a';
	else if ( (bt >= '0') && (bt <= '9') )
		return 52 + bt - '0';
	else if ( bt == ',')
		return  62;
	else
		return 63;
}

void CXmlFile::SaveXmlTemplate(CString sFilename, CvvImage &img)
{
	try
	{
		ofstream ofs(sFilename);

		XMLNode ndTop = XMLNode::createXMLTopNode();
		XMLNode ndRoot = ndTop.addChild(stringDup(_T("xml")), 1);
		ndRoot.addAttribute(stringDup(_T("version")), stringDup(_T("1.0")));
		ndRoot.addAttribute(stringDup(_T("encoding")), stringDup(_T("UTF-8")));
		ndRoot.addAttribute(stringDup(_T("standalone")), stringDup(_T("no")));

		XMLNode ndP2AOData = ndRoot.addChild(stringDup(_T("Template")));
		XMLNode ndSrc = ndP2AOData.addChild(stringDup(_T("Src")));

		AddBitmapAttribute(ndSrc, img);
	
		char *pStrXML = ndTop.createXMLString(0);
		ofs << pStrXML << endl;
		free(pStrXML);
		ofs.close();
	}
	catch (...)	{
		AfxMessageBox("保存模板文件失败！请重试。");
	}

}

void CXmlFile::LoadXmlTemplate(CString sFilename, CvvImage &img)
{
	try
	{
		XMLNode ndP2AOData = XMLNode::openFileHelper(sFilename, _T("Template"));
		XMLNode ndSrc = ndP2AOData.getChildNode("Src");
		
		int width = atoi(ndSrc.getAttribute(_T("width")));
		int height = atoi(ndSrc.getAttribute(_T("height")));
		int nLength = atoi(ndSrc.getAttribute(_T("length")));

		img.Create(width, height, 24);

		const char *test = reinterpret_cast<LPCSTR>(ndSrc.getAttribute(_T("data")));

		char *pBuf = new char[nLength];
		DecodeBase64(pBuf, test, nLength);
		
		int nWidthStep = img.GetImage()->widthStep;
		for (int h = 0; h < height; h++)
		{
			for (int w  = 0; w < width; w++)
			{
				int nPos = h * nWidthStep + w * 3;
				img.GetImage()->imageData[nPos] = pBuf[nPos];
				img.GetImage()->imageData[nPos + 1] = pBuf[nPos + 1];
				img.GetImage()->imageData[nPos + 2] = pBuf[nPos + 2];
			}
		}

		delete[] pBuf;
	}
	catch (...)
	{
		AfxMessageBox("打开模板文件失败！可能该文件已经损坏，请重试。");
	}
}


