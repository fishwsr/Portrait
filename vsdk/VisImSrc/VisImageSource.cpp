// VisImageSource.cpp
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#include "VisImSrcPch.h"
#include "VisImSrcProj.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



VisImSrcExport CVisImageSource::CVisImageSource(const CVisImageSource& refimagesource)
  : m_pIVisImSrcDevice(refimagesource.m_pIVisImSrcDevice),
	m_pIVisImSrcSettings(refimagesource.m_pIVisImSrcSettings),
	m_pSequenceList(refimagesource.m_pSequenceList),
	m_strID(refimagesource.m_strID),
	m_strName(refimagesource.m_strName)
{
	assert(&refimagesource != 0);

	if (m_pIVisImSrcDevice != 0)
		m_pIVisImSrcDevice->AddRef();

	if (m_pIVisImSrcSettings != 0)
		m_pIVisImSrcSettings->AddRef();

	if (m_pSequenceList != 0)
		m_pSequenceList->AddRef();
}


VisImSrcExport CVisImageSource& CVisImageSource::operator=(
		const CVisImageSource& refimagesource)
{
	assert(&refimagesource != 0);

	if (m_pIVisImSrcDevice != 0)
		m_pIVisImSrcDevice->Release();

	m_pIVisImSrcDevice = refimagesource.m_pIVisImSrcDevice;
	if (m_pIVisImSrcDevice != 0)
		m_pIVisImSrcDevice->AddRef();

	if ((m_pIVisImSrcSettings != 0)
			&& (m_pIVisImSrcSettings != refimagesource.m_pIVisImSrcSettings))
	{
		m_pIVisImSrcSettings->SaveSettings();
	}

	if (m_pIVisImSrcSettings != 0)
		m_pIVisImSrcSettings->Release();

	m_pIVisImSrcSettings = refimagesource.m_pIVisImSrcSettings;
	if (m_pIVisImSrcSettings != 0)
		m_pIVisImSrcSettings->AddRef();

	if (m_pSequenceList != 0)
		m_pSequenceList->Release();

	m_pSequenceList = refimagesource.m_pSequenceList;
	if (m_pSequenceList != 0)
		m_pSequenceList->AddRef();

	m_strID = refimagesource.m_strID;
	m_strName = refimagesource.m_strName;

	return *this;
}


VisImSrcExport CVisImageSource::~CVisImageSource(void)
{
	if (m_pIVisImSrcDevice != 0)
	{
		m_pIVisImSrcDevice->Release();
		m_pIVisImSrcDevice = 0;
	}

	if (m_pIVisImSrcSettings != 0)
	{
		m_pIVisImSrcSettings->SaveSettings();
		m_pIVisImSrcSettings->Release();
		m_pIVisImSrcSettings = 0;
	}

	if (m_pSequenceList != 0)
	{
		m_pSequenceList->Release();
		m_pSequenceList = 0;
	}
}



VisImSrcExport bool CVisImageSource::IsValid(void) const
{
	bool fRet;

	if ((this != 0)
			&& (m_pIVisImSrcDevice != 0)
			&& (m_pIVisImSrcSettings != 0)
			&& (m_pSequenceList != 0))
	{
		fRet = true;
	}
	else
	{
		fRet = false;
	}

	return fRet;
}



EVisImEncoding CVisImageSource::ImEncodingGetPixFmt(int evispixfmt)
{
	EVisImEncoding evisimencoding = evisimencodingUnknown;

	switch (evispixfmt & evispixfmtStructureMask)
	{
	case evispixfmtGray:
		if ((evispixfmt & (evispixfmtUnsignedMask | evispixfmtNumericTypeMask))
				== (evispixfmtSigned | evispixfmtChar))
		{
			evisimencoding = evisimencodingGray8bit;
		}
		else if ((evispixfmt & (evispixfmtUnsignedMask | evispixfmtNumericTypeMask))
				== (evispixfmtSigned | evispixfmtShort))
		{
			evisimencoding = evisimencodingGray16bit;
		}
		break;

	case evispixfmtRGBA:
		if ((evispixfmt & (evispixfmtUnsignedMask | evispixfmtNumericTypeMask))
				== (evispixfmtSigned | evispixfmtChar))
		{
			evisimencoding = evisimencodingRGBA32;
		}
		else if ((evispixfmt & (evispixfmtUnsignedMask | evispixfmtNumericTypeMask))
				== (evispixfmtSigned | evispixfmtShort))
		{
			evisimencoding = evisimencodingRGBA64;
		}
		break;

	case evispixfmtRGB:
		if ((evispixfmt & (evispixfmtUnsignedMask | evispixfmtNumericTypeMask))
				== (evispixfmtSigned | evispixfmtChar))
		{
			evisimencoding = evisimencodingRGB24;
		}
		else if ((evispixfmt & (evispixfmtUnsignedMask | evispixfmtNumericTypeMask))
				== (evispixfmtSigned | evispixfmtShort))
		{
			evisimencoding = evisimencodingRGB48;
		}
		break;

	case evispixfmtYUVA:
		if ((evispixfmt & (evispixfmtUnsignedMask | evispixfmtNumericTypeMask))
				== (evispixfmtSigned | evispixfmtChar))
		{
			evisimencoding = evisimencodingYUVA32;
		}
		else if ((evispixfmt & (evispixfmtUnsignedMask | evispixfmtNumericTypeMask))
				== (evispixfmtSigned | evispixfmtShort))
		{
			evisimencoding = evisimencodingYUVA64;
		}
		break;

	case evispixfmtYUV:
		if ((evispixfmt & (evispixfmtUnsignedMask | evispixfmtNumericTypeMask))
				== (evispixfmtSigned | evispixfmtChar))
		{
			evisimencoding = evisimencodingYUV24;
		}
		else if ((evispixfmt & (evispixfmtUnsignedMask | evispixfmtNumericTypeMask))
				== (evispixfmtSigned | evispixfmtShort))
		{
			evisimencoding = evisimencodingYUV48;
		}
		break;

	case evispixfmtImEncoding:
		evisimencoding = (EVisImEncoding) (evispixfmt & 0x3f);
		if (evisimencoding >= evisimencodingLim)
			evisimencoding = evisimencodingUnknown;
		break;
	}

	return evisimencoding;
}


VisImSrcExport bool CVisImageSource::IsPreferredPixFmt(EVisPixFmt evispixfmt) const
{
	assert(IsValid());
	
	bool fRet;

	EVisImEncoding evisimencoding = ImEncodingGetPixFmt(evispixfmt);

	if ((evisimencoding != evisimencodingUnknown)
			&& (m_pIVisImSrcDevice->IsPreferredImEncoding(evisimencoding) == S_OK))
	{
		fRet = true;
	}
	else
	{
		fRet = false;
	}

	return fRet;
}


VisImSrcExport bool CVisImageSource::IsSupportedPixFmt(EVisPixFmt evispixfmt) const
{
	assert(IsValid());
	
	bool fRet = false;

	CVisImageFromImSrcFrame imagefromframe(evispixfmt);

	int cimencoding = imagefromframe.CImEncodingSupported();
	for (int i = 0; i < cimencoding; ++i)
	{
		EVisImEncoding evisimencodingT = imagefromframe.ImEncodingSupportedI(i);

		if ((m_pIVisImSrcDevice->IsPreferredImEncoding(evisimencodingT)
						== ResultFromScode(S_OK))
				|| (m_pIVisImSrcDevice->IsSupportedImEncoding(evisimencodingT)
						== ResultFromScode(S_OK)))
		{
			fRet = true;
			break;
		}
	}

	return fRet;
}


VisImSrcExport bool CVisImageSource::UseContinuousGrab(void) const
{
	assert(IsValid());
	
	LONG fUseContinuousGrab = false;

	m_pIVisImSrcSettings->GetImSrcValue(evisimsrcFContinuousGrab,
			&fUseContinuousGrab);

	return ((fUseContinuousGrab) ? true : false);
}


VisImSrcExport void CVisImageSource::SetUseContinuousGrab(bool f)
{
	// LATER:  What if one sequence tries to turn off continuous grabbing when other
	// sequences want to keep continuous grabbing on?
	assert(IsValid());

	if (f != UseContinuousGrab())
	{
		m_pIVisImSrcSettings->SetImSrcValue(evisimsrcFContinuousGrab, f);
	}

	bool fHaveContinuousGrab = UseContinuousGrab();
	
	m_pSequenceList->SetWantContinuousGrab(f, fHaveContinuousGrab);
}


VisImSrcExport bool CVisImageSource::AddSequence(class CVisSequenceBase& refsequence,
		bool fModifyImEncoding, bool fUseContinuousGrab)
{
	bool fRet;

	CVisImageSource *pimagesourcePrev = refsequence.m_pimagesource;
	assert(pimagesourcePrev != this);

	if (pimagesourcePrev != 0)
	{
		pimagesourcePrev->RemoveSequence(refsequence);
		delete pimagesourcePrev;
	}

	if (IsValid())
	{
		fRet = m_pSequenceList->FAddSequence(&refsequence, fModifyImEncoding);

		if (fRet)
		{
			switch (refsequence.m_evissequence & evissequenceMaskSourceChangesMaxLength)
			{
			case evissequenceSourceCanChangeMaxLength:
				if (refsequence.m_fCalledSetLengthMax)
					break;
				// Missing break is intentional.
			case evissequenceSourceAlwaysChangesMaxLength:
				refsequence.SetLengthMax(0);
				break;
			}

			refsequence.m_pimagesource = new CVisImageSource(*this);
			// LATER:  What if this fails.  (OOM)

			if (fUseContinuousGrab)
				SetUseContinuousGrab(fUseContinuousGrab);
		}
	}
	else
	{
		fRet = false;
	}

	return fRet;
}

VisImSrcExport void CVisImageSource::RemoveSequence(
		class CVisSequenceBase& refsequence)
{
	assert(refsequence.m_pimagesource == this);

	if (IsValid())
	{
		m_pSequenceList->RemoveSequence(&refsequence);
	}

	refsequence.m_pimagesource = 0;
}


// (These correspond to the methods used in AciveMovie.)
VisImSrcExport void CVisImageSource::GetImSrcRange(EVisImSrc evisimsrc, long *plMin,
		long *plMax, long *pdlStepping, long *plDefault, long *plFlags) const
{
	assert(IsValid());
	
	if (FAILED(m_pIVisImSrcSettings->GetImSrcRange(evisimsrc, plMin, plMax,
			pdlStepping, plDefault, plFlags)))
	{
		if (plMin != 0)
			*plMin = 0;
		if (plMax != 0)
			*plMax = 0;
		if (pdlStepping != 0)
			*pdlStepping = 0;
		if (plDefault != 0)
			*plDefault = 0;
		if (plFlags != 0)
			*plFlags = 0;
	}
}


VisImSrcExport const char *CVisImageSource::SzID(void)
{
	assert(IsValid());

	if (m_strID.empty())
	{
		char szBuf[256];
		LONG cb = sizeof(szBuf);
		if (SUCCEEDED(m_pIVisImSrcDevice->GetSzID(szBuf, &cb)))
		{
			m_strID = szBuf;
		}
		else if (SUCCEEDED(m_pIVisImSrcDevice->GetSzID(0, &cb)))
		{
			// Attempt to handle lond strings with this special-case code.
			char *szT = new char[cb];
			if (szT != 0)
			{
				if (SUCCEEDED(m_pIVisImSrcDevice->GetSzID(szT, &cb)))
				{
					m_strID = szT;
				}

				delete szT;
			}
		}
	}
	
	return m_strID.c_str();
}

VisImSrcExport const char *CVisImageSource::SzName(void)
{
	assert(IsValid());
	
	if (m_strName.empty())
	{
		char szBuf[256];
		LONG cb = sizeof(szBuf);
		if (SUCCEEDED(m_pIVisImSrcDevice->GetSzName(szBuf, &cb)))
		{
			m_strName = szBuf;
		}
		else if (SUCCEEDED(m_pIVisImSrcDevice->GetSzName(0, &cb)))
		{
			// Attempt to handle lond strings with this special-case code.
			char *szT = new char[cb];
			if (szT != 0)
			{
				if (SUCCEEDED(m_pIVisImSrcDevice->GetSzName(szT, &cb)))
				{
					m_strName = szT;
				}

				delete szT;
			}
		}
	}
	
	return m_strName.c_str();
}


VisImSrcExport CVisImageSource::CVisImageSource(
		IVisImSrcDevice *pIVisImSrcDevice,
		IVisImSrcSettings *pIVisImSrcSettings)
  : m_pIVisImSrcDevice(pIVisImSrcDevice),
	m_pIVisImSrcSettings(pIVisImSrcSettings),
	m_pSequenceList(0),
	m_strID(),
	m_strName()
{
	if (m_pIVisImSrcDevice != 0)
	{
		m_pIVisImSrcDevice->AddRef();

		if (m_pIVisImSrcSettings != 0)
		{
			m_pIVisImSrcSettings->AddRef();
		}
		else
		{
			m_pIVisImSrcDevice->QueryInterface(IID_IVisImSrcSettings,
					(void **) &m_pIVisImSrcSettings);
			assert(m_pIVisImSrcSettings != 0);

			m_pSequenceList = new CVisSequenceList(pIVisImSrcDevice,
					m_pIVisImSrcSettings);
		}
	}
}

VisImSrcExport void CVisImageSource::DeleteFromSequence(
		CVisSequenceBase& refsequence)
{
	RemoveSequence(refsequence);
	delete this;
}

VisImSrcExport void CVisImageSource::NeedImage(
		CVisSequenceBase& refsequence)
{
	GrabNextInBackground();
}
