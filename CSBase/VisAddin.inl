template<class pixelType>
inline void VisImageCopy(const CVisImage<pixelType> &srcImage, CVisImage<pixelType> &desImg)
{
	if ( srcImage.Width()!=desImg.Width() || srcImage.Height()!=desImg.Height() )
	{
		desImg.Deallocate();
		desImg.Allocate(srcImage.Shape());
	}
	srcImage.CopyPixelsTo(desImg);
};

template <class pixelType>
inline void VisImageZoom(const CVisImage<pixelType> &srcImage, CVisImage<pixelType> &desImg, double sx /*=1.*/, double sy /*=1.*/, TVisInterpolation interp /*=VIS_INTERP_CUBIC*/)
{
	int width = srcImage.Width() * sx;
	int height = srcImage.Height() * sy;
	
	if ( width<=0 || height<=0 ) return;
	
	if ( desImg.Width()!=width || desImg.Height()!=height )
	{
		desImg.Deallocate();
		desImg.Allocate(width, height);
	}

	VisZoom(srcImage, desImg, 1./sx, 0., 1./sy, 0., interp);
};

template<class pixelType>
inline void VisImageFlipHorizontal(CVisImage<pixelType> &in_image, CVisImage<pixelType> &out_image)
{
	int width = in_image.Width();
	int height = in_image.Height();
	if ( width<=0 || height<=0 ) return;

	out_image.Allocate(width, height);

	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			out_image.Pixel(x,y) = in_image.Pixel(width-1-x, y);
		}
	}
};

template<class pixelType>
inline void VisImageFlipVertical(CVisImage<pixelType> &in_image, CVisImage<pixelType> &out_image)
{
	int width = in_image.Width();
	int height = in_image.Height();
	if ( width<=0 || height<=0 ) return;

	out_image.Allocate(width, height);

	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			out_image.Pixel(x,y) = in_image.Pixel(x, height-1-y);
		}
	}
};