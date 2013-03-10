#pragma once

#ifndef __GXIMAGE_H
#define __GXIMAGE_H

/*//////////////////////////////////////////////////////////////////////////////
//
// File: gximage.h
//
// Copyright (C) 2000 Microsoft Corporation. All Rights Reserved.
//
// Description:
//    This include file defines a set of WINAPI entry points for manipulating
//  images as DirectDrawSurfaces.
//
//
//////////////////////////////////////////////////////////////////////////////*/

#include <ddraw.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GXWRAP_TORUS_X      0x1
#define GXWRAP_FOLD_X       0x2
#define GXWRAP_CLAMP_X      0x4
#define GXWRAP_MIRROR_X     0x8
#define GXWRAP_LEAK_X       0x10
#define GXWRAP_MASK         0xFF
#define GXWRAP_XFLAGBITS    16
#define GXWRAP_TORUS_Y      (GXWRAP_TORUS_X << GXWRAP_XFLAGBITS)
#define GXWRAP_FOLD_Y       (GXWRAP_FOLD_X << GXWRAP_XFLAGBITS)
#define GXWRAP_CLAMP_Y      (GXWRAP_CLAMP_X << GXWRAP_XFLAGBITS)
#define GXWRAP_MIRROR_Y     (GXWRAP_MIRROR_X << GXWRAP_XFLAGBITS)
#define GXWRAP_LEAK_Y       (GXWRAP_LEAK_X << GXWRAP_XFLAGBITS)

#define GXWRAP_LEAK         (GXWRAP_LEAK_X | GXWRAP_LEAK_Y)
#define GXWRAP_CLAMP        (GXWRAP_CLAMP_X | GXWRAP_CLAMP_Y)
#define GXWRAP_LEAKCLAMP    (GXWRAP_LEAK | GXWRAP_CLAMP)
#define GXWRAP_MIRROR       (GXWRAP_MIRROR_X | GXWRAP_MIRROR_Y)
#define GXWRAP_TORUS        (GXWRAP_TORUS_X | GXWRAP_TORUS_Y)
#define GXWRAP_FOLD         (GXWRAP_FOLD_X | GXWRAP_FOLD_Y)
#define GXWRAP_BURRITO      (GXWRAP_FOLD_X | GXWRAP_TORUS_Y)


///////////////////////////////////////////////////////////////////////////////
// Function: GXResample
//    This function copies a rectangle from a source surface and filters it
// into a rectangle in the destination surface.  If prDst or prSrc is NULL,
// the entire surface is assumed to be the rectangle size.
///////////////////////////////////////////////////////////////////////////////
extern HRESULT WINAPI 
GXResample(LPDIRECTDRAWSURFACE7 pddsDst, LPRECT prDst, 
           LPDIRECTDRAWSURFACE7 pddsSrc, LPRECT prSrc, UINT uFlags);

///////////////////////////////////////////////////////////////////////////////
// Function: GXComputeMipmapLevels
//    This function downsamples the topmost DDSurface into subsequent levels
// using a 9x9 windowed sinc filter.  If the width or height of a level is less
// than 16, linear interpolation is used.
///////////////////////////////////////////////////////////////////////////////
extern HRESULT WINAPI 
GXComputeMipmapLevels(LPDIRECTDRAWSURFACE7 pDDS, UINT uFlags);

///////////////////////////////////////////////////////////////////////////////
// Function: GXWriteToBMP
//    This function writes a rectangle of the given DirectDrawSurface out to
//  file in BMP format.
///////////////////////////////////////////////////////////////////////////////
extern HRESULT WINAPI 
GXWriteToBMP(LPDIRECTDRAWSURFACE7 pDDS, LPRECT prSrc, LPCSTR szFilename);

///////////////////////////////////////////////////////////////////////////////
// Function: GXConvertPixelFormat
//    This function copies one surface to another surface, performing a pixel
// format conversion when the pixel formats are different.  If the surfaces
// represent mipmap chains and prSrc is NULL, the entire chain is converted.
///////////////////////////////////////////////////////////////////////////////
extern HRESULT WINAPI 
GXConvertPixelFormat(LPDIRECTDRAWSURFACE7 pddsDst, DWORD dwXDst, DWORD dwYDst,
                     LPDIRECTDRAWSURFACE7 pddsSrc, LPRECT prSrc);

///////////////////////////////////////////////////////////////////////////////
// Function: GXCreateSurfaceFromStream
//    This function allocates a new DirectDraw surface based on the pDDSD, and
// initializes it with information from the stream.  If pDDSD is NULL, a
// default surface type is used.  If the stream contains a mipmap chain, the
// entire chain is loaded and initialized.
///////////////////////////////////////////////////////////////////////////////
extern HRESULT WINAPI 
GXCreateSurfaceFromStream(LPDIRECTDRAW7 pDD, IStream *pStream, 
                          LPDDSURFACEDESC2 pDDSD, DWORD dwFlags, 
                          LPDIRECTDRAWSURFACE7 *ppddsDst, IUnknown *pUnkOuter);

///////////////////////////////////////////////////////////////////////////////
// Function: GXCreateSurfaceFromFile
//    This function allocates a new DirectDraw surface based on the pDDSD, and
// initializes it with data from the given file.  This code uses the IE image
// filters, and can load common graphics formats such as gif, jpg, png, and
// bmp, as well as mipmap chains saved using GXSaveSurfaceToFile.
///////////////////////////////////////////////////////////////////////////////
extern HRESULT WINAPI 
GXCreateSurfaceFromFileW(LPDIRECTDRAW7 pDD, BSTR bstrFilename, 
                         LPDDSURFACEDESC2 pDDSD, DWORD dwFlags, 
                         LPDIRECTDRAWSURFACE7 *ppddsDst, IUnknown *pUnkOuter);
extern HRESULT WINAPI
GXCreateSurfaceFromFileA(LPDIRECTDRAW7 pDD, LPCSTR szFilename, 
                         LPDDSURFACEDESC2 pDDSD, DWORD dwFlags, 
                         LPDIRECTDRAWSURFACE7 *ppddsDst, IUnknown *pUnkOuter);
#ifdef _UNICODE
    #define GXCreateSurfaceFromFile CreateSurfaceFromFileW
#else
    #define GXCreateSurfaceFromFile CreateSurfaceFromFileA
#endif

///////////////////////////////////////////////////////////////////////////////
// Function: GXSaveSurfaceToFile
//    This function writes a DirectDraw surface to a file, using a proprietary
// format.  It can be used to store mipmap chains.
///////////////////////////////////////////////////////////////////////////////
extern HRESULT WINAPI
GXSaveSurfaceToFileW(LPDIRECTDRAWSURFACE7 pDDS, BSTR bstrFilename);
extern HRESULT WINAPI
GXSaveSurfaceToFileA(LPDIRECTDRAWSURFACE7 pDDS, LPCSTR szFilename);
#ifdef _UNICODE
    #define GXSaveSurfaceToFile SaveSurfaceToFileW
#else
    #define GXSaveSurfaceToFile SaveSurfaceToFileA
#endif

#ifdef __cplusplus
};
#endif
#endif  // #ifndef __GXIMAGE_H
