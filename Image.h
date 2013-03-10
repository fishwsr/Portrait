/***************************************************************/
/*文件名：     image.h               				           */
/*文件描述：                                                   */
/*相关文件：   global.h, global.cpp, image.cpp                 */
/*主要函数：                                                   */
/*提交日期：                        作  者：刘岩、山世光       */
/*编 辑 器： Visual C++                                        */
/*版权：哈尔滨工业大学智能接口实验室 中科院计算所数字媒体实验室*/
/*版权说明：任何使用必须保留版权信息和历史纪录信息			   */
/*修改纪录：												   */
/*日  期        版本        修改人         修改内容            */
/*03/22/1999    2.0         山世光         ---------           */
/*05/22/2000    3.0         山世光         ---------           */
/***************************************************************/

#ifndef	_IMAGE_H_
#define	_IMAGE_H_

class CImage : public CObject
{
	DECLARE_SERIAL(CImage)

private:
	DWORD Transform(WORD width);
	DWORD ByteNumForOneLine(WORD nDIBWidth, int nBpp);

public:
	BOOL		m_bIsDIB;
    CPalette*	m_pPal;	    
    BYTE*		m_pDib ;     // DIB data buffer
    DWORD		m_dwLength;  // total buffer length
    BYTE*       m_pData; 

public:
    CImage();
    virtual ~CImage();

	CImage(CImage& img, CSize newSize);
    CImage(CDC* pDC, int nBt = 0, BOOL bCompr = FALSE); // nBt = 0 means use default bits/pixel
    CImage(CImage& img, CPoint start, CSize size);
    CImage(CSize size, int NumColor=256, int Bits=8);
	CImage(CImage& img, float scale);
    CImage(const CImage& img);//copy constructor

    CImage& operator=(const CImage& img);

	void	Resize(int w, int h);
	void	ImageClear();

	BOOL	WriteToFile(CFile* pFile);
	BOOL	ReadFromFile(CFile* pFile);
    BOOL	WriteAsBMP(CFile* pFile);
	BOOL	WriteAsPGM(CFile* pFile);
	BOOL	WriteAsData(CFile* pFile);
	BOOL	WriteToFile(CString filename);
	BOOL	SaveAsText(CString	 filename, BOOL	bIsBinary = FALSE);

	BOOL	ReadFromFile(CString filename);
	BOOL	ReadFromPGM(CFile* pFile);
	BOOL	ReadFromBMP(CFile* pFile);
	BOOL	ReadFromIMG(CFile* pFile);
	BOOL	ReadFromDAT(CFile* pFile);
	BOOL	ReadFromJPG(CString filename);

	double	DistanceBetween(CImage* img);
	BOOL	VisualizeDataAsImage(double* pData, int cols, int rows);

    virtual void Serialize(CArchive &ar);

    CBitmap* MakeBitmap(CDC* pDC, CSize& bmSize); // bmsize returned
	BOOL	CreateDIBPalette();
	BOOL	CreateGreyPalette();
	BOOL	CreateGreyDib();

    BOOL	Display(CDC*, CPoint origin) const;
    BOOL	Stretch(CDC*, CPoint origin, CSize size) const;

	WORD	Bits() const;  // bits per pixel
    WORD	NumColors() const;//real colors 
    DWORD	Length() const;
    WORD	Height() const;
    WORD	Width() const;
    CSize	Size() const;
	CPalette* Palette() const;

	UINT	SetPalette(CDC* pDC,BOOL bBackground=FALSE) const;
	void	SetDIB(BOOL Flag=TRUE);
	BYTE*	Dib();
	BYTE*	Data();
	void	SetMonoColors(DWORD dwForeground, DWORD dwBackground);
    BOOL	GetMonoColors(DWORD& dwForeground, DWORD& dwBackground);

protected:
	LPBITMAPINFOHEADER	BMInfoHdPtr() const;
    LPBITMAPINFO		BMInfoPtr() const;
	RGBQUAD*			BMColorTblPtr() const;
    BOOL				AllocateMemory(BOOL bRealloc = FALSE);
	BYTE				Get8NeiborAver(BYTE* pGreyData, WORD w, WORD h, CPoint pos);

/////////the property of the image
public:
	BOOL		ValidImage(void);
	BYTE		MeanGrey();
	double		Contrast();
	double		CleanMeasure();
	double		NoiseMeasure();
	double		Entropy();
	double		ImageQuality();

public:
	static const int GREY;

/********************************************************************
///////////////////////////Process Algorithms////////////////////////
********************************************************************/
private:
	void HistogramEqualize(BYTE * newgrey, BYTE q_step = 1);
	BYTE GetBit(BYTE* pImageData,DWORD dwWidth,DWORD dwHeight,int x,int y);

public:
	void DisplayImage(CWnd *pWnd, double &dScale);
	BOOL CenterRotateInImagePlane(double angle, BOOL bClockWise);
	BOOL MirrorImage();
	/* transform*/
	BOOL	FFT( double* fr, double* fi, double* fm, double* fa);
	//color space transform
	BOOL	Mask5x5ConvFilter(int pMaskMatrix[5][5], int nNormalFactor);
	BOOL	Mask3x3ConvFilter(int pMaskMatrix[3][3], int nNormalFactor);
	BOOL	GetBImage(CImage* pBImg);
	BOOL	GetGImage(CImage* pGImg);
	BOOL	GetRImage(CImage* pRImg );
	BOOL	Convert2I_Image(void);
	BOOL	Convert2FaiImage();
	//add noise
	void	AddNoise(double	nNoiseMag=20.0);
	// smoothing operator
	void	Smoothing();
	void	MidFilter();
	void	ImageMidValueFilter(void);//color
	void	ImageSmoothing(void);//color
	// edge detector
	BOOL	ColorFaiImgCannyEdge(double sigma, BOOL bBinary);
	BOOL	ColorImageCannyEdgeDetector(double sigma, BOOL bBinary);
	void	SecondOrderEdgeDetect();
	void	Thin();
	void	Sobel();
	BOOL	Prewitt(int*	pnEdge, BYTE *pucDir);
	BOOL	Kirsch(int*	pnEdge, BYTE *pucDir);
	BOOL	CannyEdgeDetect(double sigma = 0.5, BOOL bBinary = FALSE);
	BOOL	YDirectionGradient();
	BOOL	XDirectionGradient();
	BOOL	LaplaceOperator(int*	pnEdge = NULL);
	// binary operator
	int		DajinThre();
	void	Binary(int t, BOOL bBackground=TRUE);
	// grey process
	void	GammaTransform(double lfGamma);
	void	LogProcess();
	void	GreyStratch();
	void	SegmentedGrayStretch(int nSegNum);
	BOOL	ImageEnhance();
	BOOL    GreyTranslation(int GreyBase);
	BOOL	ContrastEnhance(int factor);
	BOOL	IntensityAverage128();

	// historgram process
	BOOL	DisplayHistogram(CDC* pDC, CPoint pos);
	BOOL	NormalHistogram(BYTE step);
	void	ModifyHistogramTo(CImage& img, BYTE q_step = 1);
	void	IntraIlluminationNormal(BYTE q_step = 1);
};

#endif
