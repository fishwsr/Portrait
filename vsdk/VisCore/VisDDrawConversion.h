// VisDDrawConversion.h
//
// Copyright © 2000 Microsoft Corporation, All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
#pragma once

VisCoreExport bool VISAPI VisAliasDDSurfaceInImage(IDirectDrawSurface* pIDDS, 
													CVisImageBase& refimagebase);

VisCoreExport bool VISAPI VisCopyDDSurfacePixelsToImage(IDirectDrawSurface* pIDDS, 
														CVisImageBase& refimagebase);

VisCoreExport IDirectDrawSurface* VISAPI VisPIDDSAliasFromImage(const CVisImageBase& refimagebase);

VisCoreExport bool VISAPI VisCopyImagePixelsToDDSurface(const CVisImageBase& refimagebase, 
														IDirectDrawSurface** ppIDDS);
#ifdef VIS_USE_DXMEDIA

VisCoreExport bool VISAPI VisCopyImagePixelsToDXSurface(const CVisImageBase& refimagebase, 
														IDXSurface** ppIDXS);

VisCoreExport bool VISAPI VisCopyDXSurfacePixelsToImage(IDXSurface* pIDXS, 
														CVisImageBase& refimagebase);

#endif // VIS_USE_DXMEDIA

