// VisImagePump.h
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_IMSRC_VISIMAGEPUMP_H__
#define __VIS_IMSRC_VISIMAGEPUMP_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// We define this for now to support the released version of the
// CVisImagePump class.
// UNDONE:  Remove this definition sometime.
#define VIS_OLD_IMAGE_PUMP


// LATER:  Add support for multiple Sequences (and image members, one for each sequence)?


class CVisImagePumpBase : public CVisPump
{
public:
	bool ControlsContinuousGrab(void) const;
	void SetControlsContinuousGrab(bool f);

	
	// These methods just call the CVisSequenceBase methods with the same names.
	bool HasImageSource(void) const;
	const CVisImageSource& ImageSource(void) const;
	CVisImageSource& ImageSource(void);
	bool ConnectToSource(CVisImageSource& refimagesource,
			bool fModifyImEncoding = true, bool fUseContinuousGrab = true);
	void DisconnectFromSource(void);


protected:
	CVisImagePumpBase(CVisSequenceBase& refsequencebase,
			T_PfnPumpCallback pfnCallback, void *pvUser, bool fStartSuspended);
	

	// Called in Run() just before the thread is started running.
	virtual void OnRun(void);

	// Called in Pause() just before the thread is paused.
	// (Called after m_fIsProcessing is set to false but possibly before the
	// thread has stopped the current processing step.)
	virtual void OnPause(void);


private:
	CVisSequenceBase& m_refsequencebase;
	bool m_fControlsContinuousGrab;
};


template<class TPixel>
class CVisImagePump : public CVisImagePumpBase
{
public:
	typedef TPixel T_Pixel;
	typedef CVisImage<TPixel> T_Image;
	typedef CVisSequence<TPixel> T_Sequence;
	typedef bool (VISAPI *T_PfnImagePumpCallback)(
			CVisImagePump<TPixel> *pimagepump);


	CVisImagePump(T_PfnImagePumpCallback pfnCallback = 0, void *pvUser = 0,
			bool fStartSuspended = true, int nSequenceLenghthMax = 0,
			int evissequence = evissequenceDefault);

	// LATER:  We'll probably want a constructor that takes a CVisImageSource
	// object.

	virtual ~CVisImagePump(void);


	virtual void KillThread(void);

	const CVisSequence<TPixel> &Sequence(void) const;
	CVisSequence<TPixel> &Sequence(void);

	
	// We need to store the image in a member variable so that it will be
	// accessible to the callback function.  (The callback function is stored
	// in the base class, and we don't want to make an unsafe type cast.)
	const CVisImage<TPixel>& Image(void) const;
	CVisImage<TPixel>& Image(void);


#ifdef VIS_OLD_IMAGE_PUMP
	CVisImagePump(T_PfnImagePumpCallback pfnCallback, void *pvUser,
			bool fStartSuspended, bool fFindImageSource,
			int nSequenceLenghthMax = 0, int evissequence = evissequenceDefault);

	BOOL ResumeCapture(void);
	void SuspendCapture(void);
	bool CaptureActive(void) const;
	
	bool FindImageSource(class CWnd* pWndParent = 0, bool fAlwaysAsk = false);
	bool FindImageSource(const char *szImageSourceID);

	// LATER:  We'd probably like to be able to access or change the
	// CVisImageSource (and maybe the CVisSequence) via public methods.
	// We'll need to make sure that any methods that could change these
	// objects will not cause problems with the thread that uses them.

	void AddImage(const CVisImage<TPixel> & refimage);
#endif // VIS_OLD_IMAGE_PUMP

protected:
	// PumpCore is called repeatedly by the default implementation of
	// ThreadMain until the thread is killed or PumpCore returns false.
	// The default implementation attempts to Pop an image from the
	// Sequence and calls ProcessImage if an image could be Popped
	// from the Sequence.  Override this method to repeatedly perform
	// some other action in this thread.
	virtual bool PumpCore(void);


#ifdef VIS_OLD_IMAGE_PUMP
	// ProcessImage is called by the default implementation of PumpCore
	// when a new image is available.  While in the ProcessImage function,
	// the Image() method can be used to acess the new image.
	// The default implementation calls the callback function if the
	// pointer to the callback function is not NULL.  If false is returned,
	// the thread will exit.  Override this method to repeatedly perform
	// some other action each time a new image is available.
	virtual bool ProcessImage(void);
#endif // VIS_OLD_IMAGE_PUMP

	virtual bool ProcessImage(T_Image& refimage);


	// The following methods allow derived classes to access private data members.
	const CVisImage<TPixel>& ImageInternal(void) const;  // This doesn't assert that it's valid.
	CVisImage<TPixel>& ImageInternal(void);  // This doesn't assert that it's valid.


private:
	CVisSequence<TPixel> m_sequence;
	T_Image m_image;
};


#include "VisImagePump.inl"


#endif // __VIS_IMSRC_VISIMAGEPUMP_H__
