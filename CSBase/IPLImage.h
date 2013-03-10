#if !defined(__IPLIMAGE_H__)
#define __IPLIMAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ipl/ipl.h"
//#include "VisCore.h"
#include "VisImageProc.h"
//#include "VisMatrix.h"

class CSBASE_API CIPLImage : public IplImage  
{
public:
	CIPLImage();
	CIPLImage(CVisImageBase &imageBase);

	virtual ~CIPLImage(){};

	void CreateFromVisImage( CVisImageBase &imageBase );
	
	void CopyToVisImage( CVisImageBase &imageBase );
	void CopyToIplImage( IplImage** ppiplImage );

	CIPLImage& operator=(CVisImageBase &imageBase );
	
	operator IplImage*()
	{
		return this;
	}

	void SetROI( int coi, int xOffset, int yOffset, int nWidth, int nHeight);

	IplROI &GetROI(void)
	{
		return m_ROI;
	}

	void ClearROI(void)
	{
		roi = NULL;
	}

	void SetBorderMode( int mode, int border, int constVal);
	
	void ClearBorderMode(void)
	{
		ZeroMemory( BorderMode, 4 * sizeof(int));
		ZeroMemory( BorderConst, 4 * sizeof(int));
	}

private:
	IplROI m_ROI;
};

#endif // !defined(__IPLIMAGE_H__)
