#pragma once

#ifndef __GXIMAGEBUILDER_H
#define __GXIMAGEBUILDER_H

/*//////////////////////////////////////////////////////////////////////////////
//
// File: gximagebuilder.h
//
// Copyright (C) 1998 - 2000 Microsoft Corporation. All Rights Reserved.
//
// Description:
//    This include file defines an interface for conveniently manipulating
//  images as DirectDrawSurfaces.
//
//
//////////////////////////////////////////////////////////////////////////////*/

#include <gxcolor.h>
#include <gximage.h>
#include <ddraw.h>
#include <objbase.h>

extern "C" HRESULT WINAPI GXImageBuilderCreate(class IGXImageBuilder **ppIB);

// {6872C732-C322-4df6-A0B9-446270050EA8}
DEFINE_GUID(IID_IGXImageBuilder, 
    0x6872c732, 0xc322, 0x4df6, 0xa0, 0xb9, 0x44, 0x62, 0x70, 0x5, 0xe, 0xa8);

#define GXFILL_SOLID        0

//------------------------------------------------------------------------------
// Class: IGXImageBuilder
//------------------------------------------------------------------------------
class IGXImageBuilder : public IUnknown
{
public:
    // initialization and file i/o
    STDMETHOD(Init)(UINT nWidth, UINT nHeight, LPDIRECTDRAW7 pDD, 
                UINT uWrapMode) PURE;
    STDMETHOD(InitFromFile)(LPCSTR szFilename, LPDIRECTDRAW7 pDD, 
                UINT uWrapMode) PURE;
    STDMETHOD(InitFromDDS)(LPDIRECTDRAWSURFACE7 pDDS, LPDIRECTDRAW7 pDD, 
                UINT uWrapMode) PURE;
    STDMETHOD(WriteToBMP)(LPRECT prSrc, LPCSTR szFilename) PURE;
    STDMETHOD(ExportDDS)(LPDDSURFACEDESC2 pDDSD, LPDIRECTDRAW7 pDD, 
                LPDIRECTDRAWSURFACE7 *ppDDSImage) PURE;

    // data access
    STDMETHOD_(GXPackedRGBA,GetPixel)(UINT iRow, UINT iCol) PURE;
    STDMETHOD_(VOID,SetPixel)(UINT iRow, UINT iCol, GXPackedRGBA uPixel) PURE;
    STDMETHOD_(UINT,GetWidth)() PURE;
    STDMETHOD_(UINT,GetHeight)() PURE;
    STDMETHOD(BltFastToSurface)(DWORD dwXDst, DWORD dwYDst, 
                LPDIRECTDRAWSURFACE7 pddsDst, LPRECT prSrc, DWORD dwFlags) PURE;
    STDMETHOD(BltFast)(DWORD dwXDst, DWORD dwYDst, LPDIRECTDRAWSURFACE7 pddsSrc, 
                LPRECT prSrc, DWORD dwFlags) PURE;

    // image manipulation
    STDMETHOD(Resize)(UINT nWidth, UINT nHeight, LPDIRECTDRAW7 pDD) PURE;
    STDMETHOD(Crop)(LPRECT prDst, LPDIRECTDRAW7 pDD) PURE;
    STDMETHOD(Fill)(LPRECT prDst, GXPackedRGBA uColorA, GXPackedRGBA uColorB, 
                UINT uFlags) PURE;
};

#endif  // #ifndef __GXIMAGEBUILDER_H
