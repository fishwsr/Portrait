///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISENUMPIXEL
//
// @module VisEnumPixel.cpp |
//
// Non-inline functions for objects used to enumerate pixels in images.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisEnumPixel\.h> <l VisEnumPixel\.inl>
//
// <nl>
// @index | VISENUMPIXEL
//
///////////////////////////////////////////////////////////////////////////


// This should be the first preprocessor statement in the file so that
// automatic precompiled headers work correctly.
#include "VisCorePch.h"
#include "VisCoreProj.h"

#include "VisEnumPixel.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG



////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisEnumPixel | CVisEnumPixel |
//
// Constructor.
//
// @syntax CVisEnumPixel(const CVisImageBase& image, int band = -1,
//		int dxBorder = 0, int dyBorder = 0,
//		EVisPad pad = evispadNone);
// @syntax CVisEnumPixel(const CVisImageBase& image, const RECT& rect,
//		int band = -1, int dxBorder = 0, int dyBorder = 0,
//		EVisPad pad = evispadNone);
// @syntax CVisEnumPixel(const CVisImageBase& image, int xLeft, int yTop,
//		int xRight, int yBottom, int band = -1,
//		int dxBorder = 0, int dyBorder = 0,
//		EVisPad pad = evispadNone);
// @syntax CVisEnumPixel(const CVisEnumPixel& refenumpixel);
// @syntax CVisEnumPixel(void);
//
// @parm const CVisImageBase& | refImage |
// The image whose pixels are to be enumerated.  This object will make
// a copy of the image (that shares the same pixel memory), so <p image>
// may be safely destroyed before this enumerator is destroyed, and
// changes to the pixels in <p image> will change the pixels enumerated
// by this object.
//
// @parm const RECT& | rect |
// The rectangle to be enumerated.
//
// @parm int | xLeft |
// Left column of the rectangle to be enumerated.
//
// @parm int | yTop |
// Top row of the rectangle to be enumerated.
//
// @parm int | xRight |
// Right column of the rectangle to be enumerated.  (This column defines
// the right of the enumeration but is not included in the enumeration.)
//
// @parm int | yBottom |
// Bottom row of the rectangle to be enumerated.  (This row defines the
// bottom of the enumeration but is not included in the enumeration.)
//
// @parmopt int | band | - 1 |
// Band to be enumerated.  If -1 is given, all bands will be enumerated.
//
// @parmopt int | dxBorder | 0 |
// Future border option.  Not supported in the beta release.
// (Users should always use the default value for this parameter.)
//
// @parmopt int | dyBorder | 0 |
// Future border option.  Not supported in the beta release.
// (Users should always use the default value for this parameter.)
//
// @parmopt EVisPad | pad | evispadNone |
// Future padding option.  Not supported in the beta release.
// (Users should always use the default value for this parameter.)
//
// @parm const CVisEnumPixel& | refenumpixel |
// Reference to a <c CVisEnumPixel> object to copy.
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport CVisEnumPixel::CVisEnumPixel(const CVisImageBase& refImage, int band,
		int dxBorder, int dyBorder, EVisPad pad)
  : m_pImage(0),
	m_rect(refImage.Rect()),
	m_pbCur(0),
	m_dpbCur(0),
	m_dxBorder(dxBorder),
	m_dyBorder(dyBorder),
	m_band(band),
	m_cbPixelCur(refImage.CbPixel()),
	m_cbColCur(refImage.CbCol()),
	m_cbRowCur(refImage.CbRow()),
	m_cbColEnumCur(0),
	m_pbRowLim(0),
	m_pbSubimageLim(0),
	m_pfnNextSubsection(FinishedWithAllSubsections)
{
	VisUnreferenced(pad);

	// LATER:  Images with Borders.
	assert(m_dxBorder == 0);
	assert(m_dyBorder == 0);

	// Get a copy of the image.
	m_pImage = refImage.Clone();
}

VisCoreExport CVisEnumPixel::CVisEnumPixel(const CVisImageBase& refImage,
		const RECT& rect, int band, int dxBorder, int dyBorder,
		EVisPad pad)
  : m_pImage(0),
	m_rect(rect),
	m_pbCur(0),
	m_dpbCur(0),
	m_dxBorder(dxBorder),
	m_dyBorder(dyBorder),
	m_band(band),
	m_cbPixelCur(refImage.CbPixel()),
	m_cbColCur(refImage.CbCol()),
	m_cbRowCur(refImage.CbRow()),
	m_cbColEnumCur(0),
	m_pbRowLim(0),
	m_pbSubimageLim(0),
	m_pfnNextSubsection(FinishedWithAllSubsections)
{
	VisUnreferenced(pad);

	// LATER:  Images with Borders.
	assert(m_dxBorder == 0);
	assert(m_dyBorder == 0);

	// Get a copy of the image.
	m_pImage = refImage.Clone();
}

VisCoreExport CVisEnumPixel::CVisEnumPixel(const CVisImageBase& refImage, int xLeft,
		int yTop, int xRight, int yBottom, int band, int dxBorder,
		int dyBorder, EVisPad pad)
  : m_pImage(0),
	m_rect(xLeft, yTop, xRight, yBottom),
	m_pbCur(0),
	m_dpbCur(0),
	m_dxBorder(dxBorder),
	m_dyBorder(dyBorder),
	m_band(band),
	m_cbPixelCur(refImage.CbPixel()),
	m_cbColCur(refImage.CbCol()),
	m_cbRowCur(refImage.CbRow()),
	m_cbColEnumCur(0),
	m_pbRowLim(0),
	m_pbSubimageLim(0),
	m_pfnNextSubsection(FinishedWithAllSubsections)
{
	VisUnreferenced(pad);

	// LATER:  Images with Borders.
	assert(m_dxBorder == 0);
	assert(m_dyBorder == 0);

	// Get a copy of the image.
	m_pImage = refImage.Clone();
}

VisCoreExport CVisEnumPixel::CVisEnumPixel(const CVisEnumPixel& refenumpixel)
  : m_pImage(0),
	m_rect(refenumpixel.m_rect),
	m_pbCur(refenumpixel.m_pbCur),
	m_dpbCur(refenumpixel.m_dpbCur),
	m_dxBorder(refenumpixel.m_dxBorder),
	m_dyBorder(refenumpixel.m_dyBorder),
	m_band(refenumpixel.m_band),
	m_cbPixelCur(refenumpixel.m_cbPixelCur),
	m_cbColCur(refenumpixel.m_cbColCur),
	m_cbRowCur(refenumpixel.m_cbRowCur),
	m_cbColEnumCur(refenumpixel.m_cbColEnumCur),
	m_pbRowLim(refenumpixel.m_pbRowLim),
	m_pbSubimageLim(refenumpixel.m_pbSubimageLim),
	m_pfnNextSubsection(refenumpixel.m_pfnNextSubsection)
{
	if (refenumpixel.m_pImage != 0)
		m_pImage = refenumpixel.m_pImage->Clone();
}

VisCoreExport CVisEnumPixel::CVisEnumPixel(void)
  : m_pImage(0),
	m_pfnNextSubsection(FinishedWithAllSubsections)
{
}



////////////////////////////////////////////////////////////////////////////
//
// @mfunc CVisEnumPixel& | CVisEnumPixel | operator= |
//
// Assignment operator.
//
// @parm const CVisEnumPixel& | refenumpixel |
// Reference to a <c CVisEnumPixel> object to copy.
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport CVisEnumPixel& CVisEnumPixel::operator=(const CVisEnumPixel& refenumpixel)
{
	if (m_pImage != 0)
		delete m_pImage;

	m_pImage = refenumpixel.m_pImage;
	m_rect = refenumpixel.m_rect;
	m_pbCur = refenumpixel.m_pbCur;
	m_dpbCur = refenumpixel.m_dpbCur;
	m_dxBorder = refenumpixel.m_dxBorder;
	m_dyBorder = refenumpixel.m_dyBorder;
	m_band = refenumpixel.m_band;
	m_cbPixelCur = refenumpixel.m_cbPixelCur;
	m_cbColCur = refenumpixel.m_cbColCur;
	m_cbRowCur = refenumpixel.m_cbRowCur;
	m_cbColEnumCur = refenumpixel.m_cbColEnumCur;
	m_pbRowLim = refenumpixel.m_pbRowLim;
	m_pbSubimageLim = refenumpixel.m_pbSubimageLim;
	m_pfnNextSubsection = refenumpixel.m_pfnNextSubsection;

	if (refenumpixel.m_pImage != 0)
		m_pImage = refenumpixel.m_pImage->Clone();

	return *this;
}


////////////////////////////////////////////////////////////////////////////
//
// @mfunc | CVisEnumPixel | ~CVisEnumPixel |
//
// Destructor.
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport CVisEnumPixel::~CVisEnumPixel(void)
{
	if (m_pImage != 0)
	{
		delete m_pImage;
		m_pImage = 0;
	}
}

////////////////////////////////////////////////////////////////////////////
//
// @mfunc:(IMPL) void | CVisEnumPixel | StartCenter |
//
// Start enumerating pixels in the center part of the image.  (If there
// are no borders, all pixels are in the center part of the image.)
//
////////////////////////////////////////////////////////////////////////////
VisCoreExport void CVisEnumPixel::StartCenter(void)
{
	// LATER:  Images with Borders.
	assert(m_dxBorder == 0);
	assert(m_dyBorder == 0);
	assert(m_cbColCur == m_pImage->CbCol());
	assert(m_cbRowCur == m_pImage->CbRow());

	// LATER:  Special case of MR == AR can be done with pbRowLim
	// == pbImageLim.

	if (m_band == -1)
	{
		// Enumerate pixels in all bands.
		if (m_pImage->IsValid())
			m_pbCur = m_pImage->PbFirstPixelInRow(m_rect.top);
		else
			m_pbCur = 0;
		m_dpbCur = m_cbPixelCur;
	}
	else
	{
		// Enumerate pixels in the specified band.
		if (m_pImage->IsValid())
			m_pbCur = m_pImage->PbPixel(m_rect.left, m_rect.top, m_band);
		else
			m_pbCur = 0;
		m_dpbCur = m_cbColCur;
	}

	m_cbColEnumCur = m_rect.Width() * m_pImage->CbCol();
	m_pbRowLim = m_pbCur + m_cbColEnumCur;
	m_pbSubimageLim = m_pbRowLim + m_rect.Height() * m_pImage->CbRow();
}


#ifdef LATER
inline void CVisEnumPixel::StartTop(void)
{
	// LATER:  m_dxBorder == m_dyBorder == 1 and image size are hard-coded.
	m_pbRowLim = m_pbBuffer + 2 * g_kcCol + 3;
	m_cbRowCur = g_kcCol + 2;
	m_cbColEnumCur = g_kcCol;

	m_pbSubimageLim = m_pbRowLim + m_cbRowCur;
	m_pbCur = m_pbRowLim - m_cbColEnumCur;

	m_pfnNextSubsection = StartBottomSubsection;
	CopyTopSliceFromImageToBuffer(m_pbImage, m_pbBuffer);
}

inline void CVisEnumPixel::StartBottom(void)
{
	// LATER:  m_dxBorder == m_dyBorder == 1 and image size are hard-coded.
	m_pbRowLim = m_pbBuffer + 2 * g_kcCol + 3;

	m_pbSubimageLim = m_pbRowLim + m_cbRowCur;
	m_pbCur = m_pbRowLim - m_cbColEnumCur;

	m_pfnNextSubsection = StartLeftSubsection;
	CopyBottomSliceFromImageToBuffer(m_pbImage, m_pbBuffer);
}

inline void CVisEnumPixel::StartLeft(void)
{
	// LATER:  m_dxBorder == m_dyBorder == 1 and image size are hard-coded.
	m_pbRowLim = m_pbBuffer + 5;
	m_cbRowCur = 3;
	m_cbColEnumCur = 1;

	m_pbSubimageLim = m_pbRowLim + (g_kcRow - 2) * m_cbRowCur;
	m_pbCur = m_pbRowLim - m_cbColEnumCur;

	m_pfnNextSubsection = StartRightSubsection;
	CopyLeftSliceFromImageToBuffer(m_pbImage, m_pbBuffer);
}

inline void CVisEnumPixel::StartRight(void)
{
	// LATER:  m_dxBorder == m_dyBorder == 1 and image size are hard-coded.
	m_pbRowLim = m_pbBuffer + 5;

	m_pbSubimageLim = m_pbRowLim + (g_kcRow - 2) * m_cbRowCur;
	m_pbCur = m_pbRowLim - m_cbColEnumCur;

	m_pfnNextSubsection = StartCenterSubsection;
	CopyRightSliceFromImageToBuffer(m_pbImage, m_pbBuffer);
}


VisCoreExport void __fastcall CVisEnumPixel::StartBottomSubsection(
		CVisEnumPixel *pEnumPixel)
{
	pEnumPixel->StartBottom():
}

VisCoreExport void __fastcall CVisEnumPixel::StartLeftSubsection(
		CVisEnumPixel *pEnumPixel)
{
	pEnumPixel->StartLeft():
}

VisCoreExport void __fastcall CVisEnumPixel::StartRightSubsection(
		CVisEnumPixel *pEnumPixel)
{
	pEnumPixel->StarRight():
}

VisCoreExport void __fastcall CVisEnumPixel::StartCenterSubsection(
		CVisEnumPixel *pEnumPixel)
{
	pEnumPixel->StartCenter():
}
#endif // LATER

VisCoreExport void __fastcall CVisEnumPixel::FinishedWithAllSubsections(
		CVisEnumPixel *pEnumPixel)
{
	pEnumPixel->m_pbCur = 0;
}
