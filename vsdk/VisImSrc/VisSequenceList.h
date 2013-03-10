////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISIMAGEQUEUELIST
//
// @module VisSequenceList.h |
//
// This file defines an internal class that is used to keep a list of
// sequences connected to an image source and make sure that images from
// the image source device are given to the sequences.  Most users of the
// library will not need to use this class.
//
// <nl>
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l VisSequenceList\.inl> <l VisSequenceList\.cpp>
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_IMSRC_VISIMAGEQUEUELIST_H__
#define __VIS_IMSRC_VISIMAGEQUEUELIST_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



#include <vector>


class CVisSequenceList
{
public:
	VisImSrcExport CVisSequenceList(IVisImSrcDevice *pIVisImSrcDevice,
			IVisImSrcSettings *pIVisImSrcSettings);
	VisImSrcExport ~CVisSequenceList(void);

	inline unsigned long AddRef(void);
	inline unsigned long Release(void);

	VisImSrcExport bool FAddSequence(class CVisSequenceBase *psequence,
			bool fModifyImEncoding);
	VisImSrcExport void RemoveSequence(class CVisSequenceBase *psequence);

	// @cmember
	// Should we allow volatile image buffers in the images that are
	// stored in sequences?  If so, the pixel data for images in
	// sequences could become invalid after a short time.  If we choose
	// not to allow volatile images, we may need to copy pixel data
	// when adding images to sequences.
	inline bool AllowVolatileImages(void) const;

	// Indicate whether we shouldallow volatile image buffers in the images
	// that are stored in sequences.
	inline void SetAllowVolatileImages(bool f);

	// @cmember
	// Should we  continuously grab images in the background?  This may increase
	// the rate at which we can get images from the image source, but
	// it may also use up significant processor time, especially if
	// we don't need a high frame rate or if the driver that is grabbing
	// the images spends a lot of time in a high-priority thread.
	inline bool WantContinuousGrab(void) const;

	// @cmember
	// Indicate whether we should continuously grab images in the background.
	inline void SetWantContinuousGrab(bool fWantContinuousGrab,
			bool fHaveContinuousGrab);

	inline int NSequences(void) const;
	inline class CVisSequenceBase *PSequenceGetI(int i);

	VisImSrcExport void GrabIfNeeded(bool fAddingSequence = false);
	inline void GrabNextInBackground(void);

protected:
	static VisImSrcExport void STDAPICALLTYPE StaticFrameCallback(
			IVisImSrcDevice *pIVisImSrcDevice,
			SVisImSrcFrame *pimsrcframe, void *pvUser);
	VisImSrcExport void FrameCallback(IVisImSrcDevice *pIVisImSrcDevice,
			SVisImSrcFrame *pimsrcframe);

	static VisImSrcExport unsigned __stdcall StaticThreadMainGrabber(
			void *pvSequenceList);
	VisImSrcExport unsigned ThreadMainGrabber(void);

	static VisImSrcExport unsigned __stdcall StaticThreadMainProcessFrame(
			void *pvSequenceList);
	VisImSrcExport unsigned ThreadMainProcessFrame(void);

private:
	unsigned long m_cRef;
	EVisPixFmt m_evispixfmt;
	IVisImSrcDevice *m_pIVisImSrcDevice;
	IVisImSrcSettings *m_pIVisImSrcSettings;
	std::vector<class CVisSequenceBase *> m_vectorSequenceList;
	DWORD m_dwIDThreadGrabber;
	HANDLE m_hThreadGrabber;
	HANDLE m_hSemaphoreGrabber;
	DWORD m_dwIDThreadProcessFrame;
	HANDLE m_hThreadProcessFrame;
	HANDLE m_hSemaphoreProcessFrame;
	CRITICAL_SECTION m_criticalsectionSequenceList;
	CRITICAL_SECTION m_criticalsectionFrame;
	SVisImSrcFrame m_imsrcframeCur;
	bool m_fWantContinuousGrab;
	bool m_fHaveContinuousGrab;
	bool m_fWantFrame;
	bool m_fKillThreadGrabber;
	bool m_fAllowVolatileImages;
	bool m_fKillThreadProcessFrame;
	bool m_fValidFrameCur;

	VisImSrcExport void KillThreadGrabber(void);
	VisImSrcExport void KillThreadProcessFrame(void);

	VisImSrcExport void DiscardFrame(SVisImSrcFrame *pimsrcframe);
	VisImSrcExport void ProcessFrame(SVisImSrcFrame *pimsrcframe);
};


#endif // __VIS_IMSRC_VISIMAGEQUEUELIST_H__
