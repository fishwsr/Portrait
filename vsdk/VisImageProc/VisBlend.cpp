///////////////////////////////////////////////////////////////////////////
//
// NAME
//  VisBlend.cpp -- image blending and compositing routines
//
// DESCRIPTION
//  These routines are used for blending images, using either image
//  compositing operators (such as over), or using a weighted average
//  (A-buffer).  Eventually, we would also like to have a weighted
//  median or some other robust averaging operator.
//
//  Routines are also provided for premultiplying and unmultiplying
//  the alpha channel.
//
// SEE ALSO
//  VisBlend.h          header file with longer description
//
// BUGS
//   
//
// DESIGN
//  
//
// HISTORY
//  30-Jun-96  Richard Szeliski (szeliski) at Microsoft
//  Created.
//  30-Jun-98  Arno Schoedl (t-arnosc) at Microsoft
//  add normalization for float images
//
// Copyright © Microsoft Corp. 2000
//
///////////////////////////////////////////////////////////////////////////

#include "VisImageProcPch.h"
#include "VisImageProc.h"


#ifndef M_PI 
#define M_PI    3.1415926536
#endif // M_PI

#ifndef __max
#define __max(a, b)  (((a) > (b)) ? (a) : (b))
#define __min(a, b)  (((a) < (b)) ? (a) : (b))
#endif


// Caution (t-rdror, 8/10/99):  The version of RGBA_over called here works with premultiplied colors
// and assumes that each premultiplied color lies in the range [0, A], where A is the
// value of alpha for the corresponding pixel.  This assumption is true if each
// premultiplied color corresponds to a non-premultiplied color between 0 and 255.  
// For example, compositing RGBA pixels (0,100,0,100) over (0,255,0,255) gives (0,255,0,255),
// but compositing (0,101,0,100) over (0,255,0,255) gives (1,0,0,255)

void VisComposite(CVisRGBAByteImage &frgd, CVisRGBAByteImage &bkgd, CVisRGBAByteImage &comp,
                    EVisComposite op)
{
    // Assumes both images active regions are totally safe
    VisCheckConformal(&frgd, &bkgd, &comp, NULL);
    int nc = comp.Width() * comp.NBands();
    int nc8 = (nc & -8);
    CVisRGBABytePixel a_mask(0, 0, 0, 255), g_mask(0, 255, 0, 0);
    int ia_mask = *(int *) &a_mask;
    int ig_mask = *(int *) &g_mask;
    int frgd_cp = (&frgd != &comp);
    int bkgd_cp = (&bkgd != &comp);
    for (int i = 0; i < comp.Height(); i++) {
        int *f = (int *) frgd.PtrToFirstPixelInRow(i+frgd.Top());
        int *b = (int *) bkgd.PtrToFirstPixelInRow(i+bkgd.Top());
        int *c = (int *) comp.PtrToFirstPixelInRow(i+comp.Top());
        int odd = ((i ^ comp.Left() ^ comp.Top()) & 1);
        int j;
        switch (op) {

        case composite_over:
            for (j = 0; j < nc; j += 8, f += 8, b += 8, c += 8) {
                if (j < nc8) {
                    int f255 = (f[0] & f[1] & f[2] & f[3] & 
                               f[4] & f[5] & f[6] & f[7]);
                    if ((f255 & ia_mask) == ia_mask) {
                        if (frgd_cp)
                            memcpy(c, f, 8*sizeof(CVisRGBABytePixel));
                        continue;
                    }
                    int f000 = (f[0] | f[1] | f[2] | f[3] | 
                               f[4] | f[5] | f[6] | f[7]);
                    if ((f000  & ia_mask) == 0) {
                        if (bkgd_cp)
                            memcpy(c, b, 8*sizeof(CVisRGBABytePixel));
                        continue;
                    }
                }
                int kl = (j < nc8) ? 8 : nc - j;
                for (int k = 0; k < kl; k++) {
                    int f_mask = (f[k] & ia_mask);
                    if (f_mask == ia_mask || b[k] == 0)
                        c[k] = f[k];
                    else if (f_mask == 0)
                        c[k] = b[k];
                    else
                        c[k] = RGBA_over(f[k], b[k]);
                }
            }
            break;

        case composite_average:
            for (j = 0; j < nc; j += 8, f += 8, b += 8, c += 8) {
                if (j < nc8) {
                    int f000 = (f[0] | f[1] | f[2] | f[3] | 
                               f[4] | f[5] | f[6] | f[7]);
                    if ((f000  & ia_mask) == 0) {
                        if (bkgd_cp)
                            memcpy(c, b, 8*sizeof(CVisRGBABytePixel));
                        continue;
                    }
                    int f255 = (f[0] & f[1] & f[2] & f[3] & 
                               f[4] & f[5] & f[6] & f[7]);
                    if ((f255 & ia_mask) == ia_mask) {
                        for (int k = 0; k < 8; k++)
                            c[k] = VisPixelAverage4Bytes(f[k], b[k]);
                        continue;
                    }
                }
                int kl = (j < nc8) ? 8 : nc - j;
                for (int k = 0; k < kl; k++) {
                    int f_mask = (f[k] & ia_mask);
                    if (f_mask == 0)
                        c[k] = b[k];
                    else if (f_mask == ia_mask)
                        c[k] = VisPixelAverage4Bytes(f[k], b[k]);
                    else
                        c[k] = RGBA_over(VisPixelAverage4Bytes(f[k], 0), b[k]);
                }
            }
            break;

        case composite_screen:      // alternate pixels in checkerboard
            for (j = 0; j < nc; j += 8, f += 8, b += 8, c += 8) {
                if (j < nc8) {
                    int f000 = (f[0] | f[1] | f[2] | f[3] | 
                               f[4] | f[5] | f[6] | f[7]);
                    if ((f000  & ia_mask) == 0) {
                        if (bkgd_cp)
                            memcpy(c, b, 8*sizeof(CVisRGBABytePixel));
                        continue;
                    }
                    int f255 = (f[0] & f[1] & f[2] & f[3] & 
                               f[4] & f[5] & f[6] & f[7]);
                    if ((f255 & ia_mask) == ia_mask) {
                        for (int k = 0; k < 8; k++)
                            c[k] = ((k ^ odd) & 1) ? f[k] : b[k];
                        continue;
                    }
                }
                int kl = (j < nc8) ? 8 : nc - j;
                for (int k = 0; k < kl; k++) {
                    int f_mask = (f[k] & ia_mask);
                    if (f_mask == 0)
                        c[k] = b[k];
                    else if (f_mask == ia_mask)
                        c[k] = ((k ^ odd) & 1) ? f[k] : b[k];
                    else
                        c[k] = ((k ^ odd) & 1) ? RGBA_over(f[k], b[k]) : b[k];
                }
            }
            break;

        case composite_minimum:
          {
            CVisRGBABytePixel *fc = (CVisRGBABytePixel *) f;
            CVisRGBABytePixel *bc = (CVisRGBABytePixel *) b;
            CVisRGBABytePixel *cc = (CVisRGBABytePixel *) c;
            for (j = 0; j < nc; j++) {
                unsigned char fA = fc[j].A(), bA = bc[j].A();
                // Use only opaque pixels
                if (fA == 255 && bA == 255)
                    cc[j] = CVisRGBABytePixel(__min(fc[j].R(), bc[j].R()),
                                              __min(fc[j].G(), bc[j].G()),
                                              __min(fc[j].B(), bc[j].B()), 255);
                else if (fA == 255)
                    c[j] = f[j];
                else if (bA == 255)
                    c[j] = b[j];
                else
                    c[j] = 0;
            }
          }
            break;

        }
    }
}

//
//  For this code, we assume that all pixel addresses are the same,
//  i.e., that src may be a subimage of accumulator
//

void VisAccumulate(CVisRGBAByteImage &src, CVisRGBAIntImage &accumulator,
                     CVisByteImage &weight, int decrement)
{
    int nrows = src.Height(), ncols = src.Width()*src.NBands();
	for (int r = 0; r < nrows; r++) {
		CVisRGBABytePixel *s = &src.Pixel(src.Left(), r + src.Top());
		CVisRGBAIntPixel  *a = &accumulator.Pixel(src.Left(), r + src.Top());
	   if (! decrement) {
		if (&weight) {
			unsigned char *w = &weight.Pixel(src.Left(), r + src.Top());
			int *i = (int *) s;
			for (int c = 0; c < ncols; c++) {
#ifdef SLOW
				a[c].SetB(a[c].B() + s[c].B() * w[c]);
				a[c].SetG(a[c].G() + s[c].G() * w[c]);
				a[c].SetR(a[c].R() + s[c].R() * w[c]);
				a[c].SetA(a[c].A() + s[c].A() * w[c]);
#else
				// Optimized to halve the number of multiplies
				if (w[c] == 0 || i[c] == 0)
					continue;
				int p0 = (i[c]        & 0x00ff00ff) * w[c];
				int p1 = ((i[c] >> 8) & 0x00ff00ff) * w[c];
				a[c].SetB(a[c].B() + p0 & 0xffff);
				a[c].SetG(a[c].G() + p1 & 0xffff);
				a[c].SetR(a[c].R() + (p0 >> 16) & 0xffff);
				a[c].SetA(a[c].A() + (p1 >> 16) & 0xffff);
#endif
			}
		} else {
			for (int c = 0; c < ncols; c++) {
				a[c].SetB(a[c].B() + s[c].B());
				a[c].SetG(a[c].G() + s[c].G());
				a[c].SetR(a[c].R() + s[c].R());
				a[c].SetA(a[c].A() + s[c].A());
			}
		}
	   } else {
		if (&weight) {
			unsigned char *w = &weight.Pixel(src.Left(), r + src.Top());
			int *i = (int *) s;
			for (int c = 0; c < ncols; c++) {
#ifdef SLOW
				a[c].SetB(a[c].B() - s[c].B() * w[c]);
				a[c].SetG(a[c].G() - s[c].G() * w[c]);
				a[c].SetR(a[c].R() - s[c].R() * w[c]);
				a[c].SetA(a[c].A() - s[c].A() * w[c]);
#else
				// Optimized to halve the number of multiplies
				if (w[c] == 0 || i[c] == 0)
					continue;
				int p0 = (i[c]        & 0x00ff00ff) * w[c];
				int p1 = ((i[c] >> 8) & 0x00ff00ff) * w[c];
				a[c].SetB(a[c].B() - p0 & 0xffff);
				a[c].SetG(a[c].G() - p1 & 0xffff);
				a[c].SetR(a[c].R() - (p0 >> 16) & 0xffff);
				a[c].SetA(a[c].A() - (p1 >> 16) & 0xffff);
#endif
			}
		} else {
			for (int c = 0; c < ncols; c++) {
				a[c].SetB(a[c].B() - s[c].B());
				a[c].SetG(a[c].G() - s[c].G());
				a[c].SetR(a[c].R() - s[c].R());
				a[c].SetA(a[c].A() - s[c].A());
			}
		}
	   }
	}
}

void VisAccumulate(CVisRGBAByteImage &src, CVisRGBAFloatImage &accumulator,
                     CVisFloatImage &weight, int decrement)
{
    int nrows = src.Height(), ncols = src.Width()*src.NBands();
	for (int r = 0; r < nrows; r++) {
		CVisRGBABytePixel  *s = &src.Pixel(src.Left(), r + src.Top(), 0);
		CVisRGBAFloatPixel *a = &accumulator.Pixel(src.Left(), r + src.Top(), 0);
	   if (! decrement) {
		if (&weight) {
			float *w = &weight.Pixel(src.Left(), r + src.Top(), 0);
			int *i = (int *) s;
			for (int c = 0; c < ncols; c++) {
				a[c].SetB(a[c].B() + s[c].B() * w[c]);
				a[c].SetG(a[c].G() + s[c].G() * w[c]);
				a[c].SetR(a[c].R() + s[c].R() * w[c]);
				a[c].SetA(a[c].A() + s[c].A() * w[c]);
			}
		} else {
			for (int c = 0; c < ncols; c++) {
				a[c].SetB(a[c].B() + s[c].B());
				a[c].SetG(a[c].G() + s[c].G());
				a[c].SetR(a[c].R() + s[c].R());
				a[c].SetA(a[c].A() + s[c].A());
			}
		}
	   } else {
		if (&weight) {
			float *w = &weight.Pixel(src.Left(), r + src.Top(), 0);
			int *i = (int *) s;
			for (int c = 0; c < ncols; c++) {
				a[c].SetB(a[c].B() - s[c].B() * w[c]);
				a[c].SetG(a[c].G() - s[c].G() * w[c]);
				a[c].SetR(a[c].R() - s[c].R() * w[c]);
				a[c].SetA(a[c].A() - s[c].A() * w[c]);
			}
		} else {
			for (int c = 0; c < ncols; c++) {
				a[c].SetB(a[c].B() - s[c].B());
				a[c].SetG(a[c].G() - s[c].G());
				a[c].SetR(a[c].R() - s[c].R());
				a[c].SetA(a[c].A() - s[c].A());
			}
		}
	   }
	}
}

void VisAccumulate(CVisRGBAByteImage &src, CVisRGBAIntImage &accumulator,
                     unsigned char weight, int decrement)
{
	if(weight==0) return;
    int nrows = src.Height(), ncols = src.Width()*src.NBands();
	for (int r = 0; r < nrows; r++) {
		CVisRGBABytePixel *s = &src.Pixel(src.Left(), r + src.Top());
		CVisRGBAIntPixel  *a = &accumulator.Pixel(src.Left(), r + src.Top());
	   if (! decrement) {
		if (weight!=1) {
			int *i = (int *) s;
			for (int c = 0; c < ncols; c++) {
#ifdef SLOW
				a[c].SetB(a[c].B() + s[c].B() * weight);
				a[c].SetG(a[c].G() + s[c].G() * weight);
				a[c].SetR(a[c].R() + s[c].R() * weight);
				a[c].SetA(a[c].A() + s[c].A() * weight);
#else
				// Optimized to halve the number of multiplies
				if (i[c] == 0)
					continue;
				int p0 = (i[c]        & 0x00ff00ff) * weight;
				int p1 = ((i[c] >> 8) & 0x00ff00ff) * weight;
				a[c].SetB(a[c].B() + p0 & 0xffff);
				a[c].SetG(a[c].G() + p1 & 0xffff);
				a[c].SetR(a[c].R() + (p0 >> 16) & 0xffff);
				a[c].SetA(a[c].A() + (p1 >> 16) & 0xffff);
#endif
			}
		} else {
			for (int c = 0; c < ncols; c++) {
				a[c].SetB(a[c].B() + s[c].B());
				a[c].SetG(a[c].G() + s[c].G());
				a[c].SetR(a[c].R() + s[c].R());
				a[c].SetA(a[c].A() + s[c].A());
			}
		}
	   } else {
		if (&weight) {
			int *i = (int *) s;
			for (int c = 0; c < ncols; c++) {
#ifdef SLOW
				a[c].SetB(a[c].B() - s[c].B() * weight);
				a[c].SetG(a[c].G() - s[c].G() * weight);
				a[c].SetR(a[c].R() - s[c].R() * weight);
				a[c].SetA(a[c].A() - s[c].A() * weight);
#else
				// Optimized to halve the number of multiplies
				if (i[c] == 0)
					continue;
				int p0 = (i[c]        & 0x00ff00ff) * weight;
				int p1 = ((i[c] >> 8) & 0x00ff00ff) * weight;
				a[c].SetB(a[c].B() - p0 & 0xffff);
				a[c].SetG(a[c].G() - p1 & 0xffff);
				a[c].SetR(a[c].R() - (p0 >> 16) & 0xffff);
				a[c].SetA(a[c].A() - (p1 >> 16) & 0xffff);
#endif
			}
		} else {
			for (int c = 0; c < ncols; c++) {
				a[c].SetB(a[c].B() - s[c].B());
				a[c].SetG(a[c].G() - s[c].G());
				a[c].SetR(a[c].R() - s[c].R());
				a[c].SetA(a[c].A() - s[c].A());
			}
		}
	   }
	}
}

void VisAccumulate(CVisRGBAByteImage &src, CVisRGBAFloatImage &accumulator,
                     float weight, int decrement)
{
	if(weight==0) return;
    int nrows = src.Height(), ncols = src.Width()*src.NBands();
	for (int r = 0; r < nrows; r++) {
		CVisRGBABytePixel  *s = &src.Pixel(src.Left(), r + src.Top());
		CVisRGBAFloatPixel *a = &accumulator.Pixel(src.Left(), r + src.Top());
	   if (! decrement) {
		if (weight!=1) {
			int *i = (int *) s;
			for (int c = 0; c < ncols; c++) {
				a[c].SetB(a[c].B() + s[c].B() * weight);
				a[c].SetG(a[c].G() + s[c].G() * weight);
				a[c].SetR(a[c].R() + s[c].R() * weight);
				a[c].SetA(a[c].A() + s[c].A() * weight);
			}
		} else {
			for (int c = 0; c < ncols; c++) {
				a[c].SetB(a[c].B() + s[c].B());
				a[c].SetG(a[c].G() + s[c].G());
				a[c].SetR(a[c].R() + s[c].R());
				a[c].SetA(a[c].A() + s[c].A());
			}
		}
	   } else {
		if (weight!=1) {
			int *i = (int *) s;
			for (int c = 0; c < ncols; c++) {
				a[c].SetB(a[c].B() - s[c].B() * weight);
				a[c].SetG(a[c].G() - s[c].G() * weight);
				a[c].SetR(a[c].R() - s[c].R() * weight);
				a[c].SetA(a[c].A() - s[c].A() * weight);
			}
		} else {
			for (int c = 0; c < ncols; c++) {
				a[c].SetB(a[c].B() - s[c].B());
				a[c].SetG(a[c].G() - s[c].G());
				a[c].SetR(a[c].R() - s[c].R());
				a[c].SetA(a[c].A() - s[c].A());
			}
		}
	   }
	}
}

void VisAccumulate(CVisGrayByteImage &src, CVisImage<CVisGrayAIntPixel> &accumulator,
                     CVisByteImage &weight, int decrement)
{
    int nrows = src.Height(), ncols = src.Width()*src.NBands();
	for (int r = 0; r < nrows; r++) {
		CVisGrayBytePixel *s = &src.Pixel(src.Left(), r + src.Top());
		CVisGrayAIntPixel *a = &accumulator.Pixel(src.Left(), r + src.Top());
	   if (! decrement) {
		if (&weight) {
			unsigned char *w = &weight.Pixel(src.Left(), r + src.Top(), 0);
			int *i = (int *) s;
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() + s[c] * w[c]);
				a[c].SetA(a[c].A() + 255 * w[c]);
			}
		} else {
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() + s[c]);
				a[c].SetA(a[c].A() + 255);
			}
		}
	   } else {
		if (&weight) {
			unsigned char *w = &weight.Pixel(src.Left(), r + src.Top());
			int *i = (int *) s;
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() - s[c] * w[c]);
				a[c].SetA(a[c].A() - 255 * w[c]);
			}
		} else {
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() - s[c]);
				a[c].SetA(a[c].A() - 255);
			}
		}
	   }
	}
}

void VisAccumulate(CVisGrayByteImage &src, CVisImage<CVisGrayAFloatPixel> &accumulator,
                     CVisFloatImage &weight, int decrement)
{
    int nrows = src.Height(), ncols = src.Width()*src.NBands();
	for (int r = 0; r < nrows; r++) {
		CVisGrayBytePixel   *s = &src.Pixel(src.Left(), r + src.Top());
		CVisGrayAFloatPixel *a = &accumulator.Pixel(src.Left(), r + src.Top());
	   if (! decrement) {
		if (&weight) {
			float *w = &weight.Pixel(src.Left(), r + src.Top());
			int *i = (int *) s;
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() + s[c] * w[c]);
				a[c].SetA(a[c].A() + 255 * w[c]);
			}
		} else {
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() + s[c]);
				a[c].SetA(a[c].A() + 255);
			}
		}
	   } else {
		if (&weight) {
			float *w = &weight.Pixel(src.Left(), r + src.Top());
			int *i = (int *) s;
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() - s[c] * w[c]);
				a[c].SetA(a[c].A() - 255 * w[c]);
			}
		} else {
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() - s[c]);
				a[c].SetA(a[c].A() - 255);
			}
		}
	   }
	}
}

void VisAccumulate(CVisGrayByteImage &src, CVisImage<CVisGrayAIntPixel> &accumulator,
                     unsigned char weight, int decrement)
{
	if(weight==0) return;
    int nrows = src.Height(), ncols = src.Width()*src.NBands();
	for (int r = 0; r < nrows; r++) {
		CVisGrayBytePixel *s = &src.Pixel(src.Left(), r + src.Top());
		CVisGrayAIntPixel *a = &accumulator.Pixel(src.Left(), r + src.Top());
	   if (! decrement) {
		if (weight!=1) {
			int *i = (int *) s;
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() + s[c] * weight);
				a[c].SetA(a[c].A() + 255 * weight);
			}
		} else {
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() + s[c]);
				a[c].SetA(a[c].A() + 255);
			}
		}
	   } else {
		if (&weight) {
			int *i = (int *) s;
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() - s[c] * weight);
				a[c].SetA(a[c].A() - 255 * weight);
			}
		} else {
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() - s[c]);
				a[c].SetA(a[c].A() - 255);
			}
		}
	   }
	}
}

void VisAccumulate(CVisGrayByteImage &src, CVisImage<CVisGrayAFloatPixel> &accumulator,
                     float weight, int decrement)
{
	if(weight==0) return;
    int nrows = src.Height(), ncols = src.Width()*src.NBands();
	for (int r = 0; r < nrows; r++) {
		CVisGrayBytePixel   *s = &src.Pixel(src.Left(), r + src.Top());
		CVisGrayAFloatPixel *a = &accumulator.Pixel(src.Left(), r + src.Top());
	   if (! decrement) {
		if (weight!=1) {
			int *i = (int *) s;
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() + s[c] * weight);
				a[c].SetA(a[c].A() + 255 * weight);
			}
		} else {
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() + s[c]);
				a[c].SetA(a[c].A() + 255);
			}
		}
	   } else {
		if (weight!=1) {
			int *i = (int *) s;
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() - s[c] * weight);
				a[c].SetA(a[c].A() - 255 * weight);
			}
		} else {
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() - s[c]);
				a[c].SetA(a[c].A() - 255);
			}
		}
	   }
	}
}

void VisAccumulate(CVisGrayFloatImage &src, CVisImage<CVisGrayAFloatPixel> &accumulator,
                     float weight, int decrement)
{
	if(weight==0) return;

    int nrows = src.Height(), ncols = src.Width()*src.NBands();
	for (int r = 0; r < nrows; r++) {
		CVisGrayFloatPixel  *s = &src.Pixel(src.Left(), r + src.Top());
		CVisGrayAFloatPixel *a = &accumulator.Pixel(src.Left(), r + src.Top());
	   if (! decrement) {
		if (weight!=1) {
			int *i = (int *) s;
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() + s[c] * weight);
				a[c].SetA(a[c].A() + weight);
			}
		} else {
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() + s[c]);
				a[c].SetA(a[c].A() + 1);
			}
		}
	   } else {
		if (weight!=1) {
			int *i = (int *) s;
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() - s[c] * weight);
				a[c].SetA(a[c].A() - weight);
			}
		} else {
			for (int c = 0; c < ncols; c++) {
				a[c].SetI(a[c].I() - s[c]);
				a[c].SetA(a[c].A() - 1);
			}
		}
	   }
	}
}

void VisAccumulateNormalize(CVisRGBAIntImage &accumulator, CVisRGBAByteImage &dst,
                            int empty_value)
{
    int nrows = dst.Height(), ncols = dst.Width() * dst.NBands();
    VisCheckConformal(&accumulator, &dst);
	for (int r = 0; r < nrows; r++) {
		CVisRGBABytePixel  *d = dst.PtrToFirstPixelInRow(r + dst.Top());
		CVisRGBAIntPixel   *a = accumulator.PtrToFirstPixelInRow(r + dst.Top());
		for (int c = 0; c < ncols; c++) {
			// Set to opaque white if empty
			if (a[c].A() == 0)
				d[c].SetRGBA(empty_value, empty_value, empty_value, empty_value);
			else {
				int w = a[c].A();
				d[c].SetB((a[c].B() * 255 + w/2) / w );
				d[c].SetG((a[c].G() * 255 + w/2) / w );
				d[c].SetR((a[c].R() * 255 + w/2) / w );
				d[c].SetA(255);   // opaque
			}
		}
	}
}

void VisAccumulateNormalize(CVisRGBAFloatImage &accumulator, CVisRGBAByteImage &dst,
                            int empty_value)
{
    int nrows = dst.Height(), ncols = dst.Width() * dst.NBands();
    VisCheckConformal(&accumulator, &dst);
	for (int r = 0; r < nrows; r++) {
		CVisRGBABytePixel    *d = dst.PtrToFirstPixelInRow(r + dst.Top());
		CVisRGBAFloatPixel *a = accumulator.PtrToFirstPixelInRow(r + dst.Top());
		for (int c = 0; c < ncols; c++) {
			// Set to opaque white if empty
			if (a[c].A() == 0)
				d[c].SetRGBA(empty_value, empty_value, empty_value, empty_value);
			else {
				float w = 255.0f / a[c].A();
				d[c].SetB((int) (a[c].B() * w + 0.5f));
				d[c].SetG((int) (a[c].G() * w + 0.5f));
				d[c].SetR((int) (a[c].R() * w + 0.5f));
				d[c].SetA(255);   // opaque
			}
		}
	}
}

void VisAccumulateNormalize(CVisImage<CVisGrayAFloatPixel> &accumulator, CVisGrayFloatImage &dst,
                            int empty_value)
{
    int nrows = dst.Height(), ncols = dst.Width() * dst.NBands();
    VisCheckConformal(&accumulator, &dst);
	for (int r = 0; r < nrows; r++) {
		CVisGrayFloatPixel    *d = dst.PtrToFirstPixelInRow(r + dst.Top());
		CVisGrayAFloatPixel *a = accumulator.PtrToFirstPixelInRow(r + dst.Top());
		for (int c = 0; c < ncols; c++) {
			// Set to opaque white if empty
			if (a[c].A() == 0)
				d[c]=empty_value;
			else {
				d[c]=a[c].I() / a[c].A();
			}
		}
	}
}

//
//  City block distance map:
//      see Rosenfeld & Kak, Digital Picture Processing, pp. 352-357
//

CVisByteImage VisCityBlockDistance(CVisRGBAByteImage &src, int max_dist,
                                   int raised_cosine)
{
    CVisByteImage dst(src.Shape());
    int nrows = src.Height(), ncols = src.Width();

    // Forward sweep
    for (int r = 0; r < nrows; r++) {
        CVisRGBABytePixel     *s = &src.Pixel(src.Left(), r + src.Top());
        unsigned char *d = &dst.Pixel(src.Left(), r + src.Top());
        unsigned char *p = &dst.Pixel(src.Left(), r-(r>0) + src.Top());

        // First row: just set to totally opaque alpha value
        if (r == 0) {
            for (int c = 0; c < ncols; c++)
                d[c] = (s[c].A() == 255);
        }
        
        // Other rows: find minimum of left and up neighbors
        else {
            d[0] = (s[0].A() != 0);
            for (int c = 1; c < ncols; c++) {
                if (s[c].A()) {
                    int d1 = d[c-1], d2 = p[c];
                    int d3 = __min(d1, d2)+1;
                    d[c]   = __min(d3, max_dist);
                } else
                    d[c] = 0;
            }
        }
    }

    // Backward sweep
    for (r = nrows-1; r >= 0; r--) {
        CVisRGBABytePixel     *s = &src.Pixel(src.Left(), r + src.Top());
        unsigned char *d = &dst.Pixel(src.Left(), r + src.Top());
        unsigned char *p = &dst.Pixel(src.Left(), r+(r<nrows-1) + src.Top());

        // Last row: just set to totally opaque alpha value
        if (r == nrows-1) {
            for (int c = 0; c < ncols; c++)
                d[c] = (s[c].A() == 255);
        }
        
        // Other rows: find minimum of right and down neighbors
        else {
            d[ncols-1] = (s[ncols-1].A() != 0);
            for (int c = ncols-2; c >= 0; c--) {
                if (s[c].A()) {
                    int d1 = d[c+1], d2 = p[c];
                    int d3 = __min(d1, d2)+1;
                    int d4 = __min(d3, d[c]);
                    d[c] = __min(d4, max_dist);
                } else
                    d[c] = 0;
            }
        }
    }

    // Postprocess:  smooth out map by passing through a raised cosine
    if (raised_cosine) {
        int *lut = new int[max_dist+1];
        for (int i = 0; i <= max_dist; i++)
            lut[i] = (int) ceil(0.5 * max_dist * (1.0 - cos(i * M_PI / max_dist)));
        for (int r = 0; r < nrows; r++) {
            unsigned char *d = dst.PtrToFirstPixelInRow(r + src.Top());
            for (int c = 0; c < ncols; c++)
                d[c] = lut[d[c]];
        }
        delete lut;
    }

    return dst;
}


CVisFloatImage VisFloatCityBlockDistance(CVisRGBAByteImage &src, int max_dist,
                                         int raised_cosine)
{
    CVisByteImage dst(src.Shape());
    CVisFloatImage fdst(src.Shape());
    int nrows = src.Height(), ncols = src.Width();

    // Forward sweep
    for (int r = 0; r < nrows; r++) {
        CVisRGBABytePixel     *s = &src.Pixel(src.Left(), r + src.Top());
        unsigned char *d = &dst.Pixel(src.Left(), r + src.Top());
        unsigned char *p = &dst.Pixel(src.Left(), r-(r>0) + src.Top());

        // First row: just set to totally opaque alpha value
        if (r == 0) {
            for (int c = 0; c < ncols; c++)
                d[c] = (s[c].A() == 255);
        }
        
        // Other rows: find minimum of left and up neighbors
        else {
            d[0] = (s[0].A() != 0);
            for (int c = 1; c < ncols; c++) {
                if (s[c].A()) {
                    int d1 = d[c-1], d2 = p[c];
                    int d3 = __min(d1, d2)+1;
                    d[c]   = __min(d3, max_dist);
                } else
                    d[c] = 0;
            }
        }
    }

    // Backward sweep
    for (r = nrows-1; r >= 0; r--) {
        CVisRGBABytePixel     *s = &src.Pixel(src.Left(), r + src.Top());
        unsigned char *d = &dst.Pixel(src.Left(), r + src.Top());
        unsigned char *p = &dst.Pixel(src.Left(), r+(r<nrows-1) + src.Top());

        // Last row: just set to totally opaque alpha value
        if (r == nrows-1) {
            for (int c = 0; c < ncols; c++)
                d[c] = (s[c].A() == 255);
        }
        
        // Other rows: find minimum of right and down neighbors
        else {
            d[ncols-1] = (s[ncols-1].A() != 0);
            for (int c = ncols-2; c >= 0; c--) {
                if (s[c].A()) {
                    int d1 = d[c+1], d2 = p[c];
                    int d3 = __min(d1, d2)+1;
                    int d4 = __min(d3, d[c]);
                    d[c] = __min(d4, max_dist);
                } else
                    d[c] = 0;
            }
        }
    }

    // Postprocess:  smooth out map by passing through a raised cosine
    if (raised_cosine) {
        float *lut = new float[max_dist+1];
        for (int i = 0; i <= max_dist; i++)
        {
            float scale = 0.5 * (1.0 - cos(i * M_PI / max_dist)); 
            if (raised_cosine == 2) // special case: slow transition from edges
            {
                lut[i] = max_dist * scale * scale;
            }
            else 
            {
                lut[i] = max_dist * scale; 
            }
        }
        for (int r = 0; r < nrows; r++) {
            unsigned char *d = dst.PtrToFirstPixelInRow(r + src.Top());
            float *fd = fdst.PtrToFirstPixelInRow(r + src.Top());
            for (int c = 0; c < ncols; c++)
                fd[c] = lut[d[c]];
        }
        delete lut;
    }
    else
    {
        for (int r = 0; r < nrows; r++) {
            unsigned char *d = dst.PtrToFirstPixelInRow(r + src.Top());
            float *fd = fdst.PtrToFirstPixelInRow(r + src.Top());
            for (int c = 0; c < ncols; c++)
                fd[c] = (float) d[c];
        }
    }

    return fdst;
}

void VisPremultiplyAlpha(CVisRGBAByteImage &src, CVisRGBAByteImage &dst)
{
}

void VisUnmultiplyAlpha(CVisRGBAByteImage &src, CVisRGBAByteImage &dst)
{
}
