// VisSequence.h
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


// LATER:  Insert-like Push methods?


#ifndef __VIS_CORE_SEQUENCE_H__
#define __VIS_CORE_SEQUENCE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include <deque>
#include <vector>


class CVisImageSourceBase;
class CVisImageSource;


enum EVisSequence
{
	evissequenceDefault							= 0,

	evissequencePushCanRemoveOldImages			= 0,
	evissequencePushNeverRemovesOldImages		= 0x01,
	evissequenceMaskPushRemovesOldImages		= 0x01,

	evissequenceInsertNeverRemovesOldImages		= 0,
	evissequenceInsertCanRemoveOldImages		= 0x02,
	evissequenceMaskInsertRemovesOldImages		= 0x02,

	evissequenceSourceCanChangeMaxLength		= 0,
	evissequenceSourceNeverChangesMaxLength		= 0x04,
	evissequenceSourceAlwaysChangesMaxLength	= 0x08,
	evissequenceMaskSourceChangesMaxLength		= 0x0c,

	// UNDONE (in Rel1):  This option is not supported yet.
	evissequenceReadAlwaysChangesMaxLength		= 0,
	evissequenceReadCanChangeMaxLength			= 0x10,
	evissequenceReadNeverChangesMaxLength		= 0x20,
	evissequenceMaskReadChangesMaxLength		= 0x30,
// UNDONE (in Rel1):  Make sure we check the DelayRead flag works as expected.
	evissequenceDontDelayRead					= 0,
	evissequenceDelayReadUntilNeeded			= 0x40,
	evissequenceMaskDelayRead					= 0x40,

	evissequenceDontRenameImagesWhenWriting		= 0,
	evissequenceRenameImagesWhenWriting			= 0x100,
	evissequenceMaskRenameImagesWhenWriting		= 0x100,

	evissequenceDontWriteImagesWhenWriting		= 0,
	evissequenceWriteImagesWhenWriting			= 0x200,
	evissequenceMaskWriteImagesWhenWriting		= 0x200,

	evissequenceDontLimitMemoryUsage			= 0,
	evissequenceLimitMemoryUsage			    = 0x400,
	evissequenceMaskLimitMemoryUsage			= 0x400,
};


// Helper structure only used internally.
// (It should be a protected class member, but that would make it harder
// to use with STL.)
struct SVisSequenceCallbackInfo
{
	void *m_pfnCallback;
	void *m_pvUser;
	bool m_fCallOnChange;
	bool m_fCallWhenDying;

	inline bool operator< (const SVisSequenceCallbackInfo& ref) const
		{ return (memcmp(this, &ref, sizeof(SVisSequenceCallbackInfo)) < 0); }
	inline bool operator==(const SVisSequenceCallbackInfo& ref) const
		{ return (memcmp(this, &ref, sizeof(SVisSequenceCallbackInfo)) == 0); }
};

class CVisSequenceBase
{
public:
    //------------------------------------------------------------------
	// @group Type definitions

	typedef CVisImageBase		T_Image;
	typedef void (VISAPI *T_PfnCallback)
			(CVisSequenceBase& refsequence, void *pvUser);


    //------------------------------------------------------------------
	// @group Constants

	enum { ecmsTimeoutDefault = 0 };


    //------------------------------------------------------------------
	// @group Attributes

	int Length(void) const;
	int LengthMax(void) const;

	VisCoreExport void SetLengthMax(int n);
    VisCoreExport void SetMRULength( int n);

	bool IsEmpty() const;
	bool IsFull() const;

	int SequenceOptions(void) const;

	void SetSequenceOptions(int evissequence);


    //------------------------------------------------------------------
	// @group Image Source for the sequence

	// We use pointers to implement these methods, so that the image sequence
	// code does not depend on the image source code.
	bool HasImageSource(void) const;
	const CVisImageSource& ImageSource(void) const;
	CVisImageSource& ImageSource(void);

	// @cmember
	// Attempt to connect this image sequence to an image source.  Returns
	// true if the this image sequence was connected to the image source,
	// false otherwise.
	//
	// Since image sequences can be connected to at most
	// one source at a time, this method will first disconnect this
	// image sequence from its current image source (if it is connected to
	// an image source) and then attempt to connect this image sequence
	// to the specified image source.
	//
	// This method could fail if the image source specified if not valid
	// or if the pixel type of this sequence can not be used with the
	// image source.  (The only pixel types supported by image sources
	// in the beta are <c BYTE>, <c unsigned long>, <c CVisRGBABytePixel>,
	// <c CVisYUVABytePixel>, and <c CVisYUVAUShortPixel>.  All image
	// sequences connected to an image source must use the same pixel type.)
	VisCoreExport bool ConnectToSource(CVisImageSource& refimagesource,
			bool fModifyImEncoding = true, bool fUseContinuousGrab = true);

	// @cmember
	// Disconnect this image sequence from an image source, if it is
	// connected to one.
	VisCoreExport void DisconnectFromSource(void);


    //------------------------------------------------------------------
	// @group Synchronized image access

	// These methods do synchronize threads accessing the sequence.
	// With internal thread synchronization, these can't be const methods.
	// LATER:  Not sure if these should be public or protected.  For now,
	// they are public, but they may be made protected in future releases.
	VisCoreExport bool GetFront(CVisImageBase& refimage, DWORD cmsTimeout);
	VisCoreExport bool GetBack(CVisImageBase& refimage, DWORD cmsTimeout);
	VisCoreExport bool GetAt(int i, CVisImageBase& refimage);

    //------------------------------------------------------------------
	// @group Image access based on the std::deque methods

	VisCoreExport void Clear();

	VisCoreExport void Erase(int i);
	VisCoreExport void Erase(int iFirst, int iLim);

	// Helper function that can be used to remove empty images from
	// the sequence.  This will return true if images were removed from
	// the sequence, false otherwise.
	VisCoreExport bool RemoveEmptyImages(void);

	// Warning:  These methods do not synchronize threads accessing the
	// sequence.  They may not work right (and may cause access violations) if
	// called when other threads are modifying the sequence.  Bad things could
	// also happen if the image specified is not in the sequence.
	// LATER:  Not sure if these should be public or protected.  For now,
	// they are public, but they may be made protected in future releases.
	VisCoreExport const CVisImageBase& Front(void) const;
	VisCoreExport CVisImageBase& Front(void);
	VisCoreExport const CVisImageBase& Back(void) const;
	VisCoreExport CVisImageBase& Back(void);
	VisCoreExport const CVisImageBase& At(int i) const;
	VisCoreExport CVisImageBase& At(int i);
	VisCoreExport const CVisImageBase& operator[](int i) const;
	VisCoreExport CVisImageBase& operator[](int i);

	// LATER:  Some other methods, like Assign(n), could (and probably
	// should) also be made public.


    //------------------------------------------------------------------
	// @group The current image

	int ICur(void) const;
	VisCoreExport void SetICur(int i);
	VisCoreExport void SetFirst(void);
	VisCoreExport void SetLast(void);
	VisCoreExport void SetNext(void);
	VisCoreExport void SetPrev(void);

	const CVisImageBase& Cur(void) const;
	CVisImageBase& Cur(void);

	bool GetCur(CVisImageBase& refimage);


    //------------------------------------------------------------------
	// @group Changing the order of images in the sequence

	// These methods return true if the order of images in the sequence
	// was changed.

    bool MoveForward(bool fToFront = true);
    VisCoreExport bool MoveForward(int i, bool fToFront = true);

    bool MoveBackward(bool fToBack = true);
    VisCoreExport bool MoveBackward(int i, bool fToBack = true);

    bool Swap(int i);
    VisCoreExport bool Swap(int i, int j);


    //------------------------------------------------------------------
	// @group EVisPixFmt and C++ type information

	EVisPixFmt PixFmt(void) const;
	virtual const type_info& PixelTypeInfo(void) const;
	virtual const type_info& ImageTypeInfo(void) const;
	virtual VisCoreExport const type_info& ObjectTypeInfo(int iLevel = - 1)
			const;

	virtual CVisSequenceBase *Clone(bool fCopyThis = true) const;

    //------------------------------------------------------------------
	// @group Sequence I/O methods

	// Read options and image file name information from a file.
	// This will throw an exception if there's an error reading the file.
	// Warning:  The format of this file may change in future releases.
	void ReadSequenceInfo(const char *szFilename,
			bool fReadFiles = false, bool fChangeOptionsAndLengthMax = true);

	// Write options and image file name information to a file.
	// This will throw an exception if there's an error writing the file.
	// Warning:  The format of this file may change in future releases.
	void WriteSequenceInfo(const char *szFilename,
			bool fWriteFiles = false);

	// Read options and image file name information from a file.
	// This will return false and the sequence will be unchanged if there's
	// an error reading the file.
	// Warning:  The format of this file may change in future releases.
	// LATER:  Do we want an option to only add files that can be read?
	bool FReadSequenceInfo(const char *szFilename,
			bool fReadFiles = false, bool fChangeOptionsAndLengthMax = true);

	// Write options and image file name information to a file.
	// This will return false if there's an error writing the file.
	// Warning:  The format of this file may change in future releases.
	bool FWriteSequenceInfo(const char *szFilename,
			bool fWriteFiles = false);

	// This will throw an exception if there's an error reading the files.
	VisCoreExport void ReadFiles(bool fOnlyIfNoImage = true);

	// This will throw an exception if there's an error writing the files.
	VisCoreExport void WriteFiles(void);

	// This will return false if there's an error reading the files, but
	// it will try to read every file before returning.
	// If fRemoveBadImages is true, images that can't be read will be
	// removed from the sequence.
	VisCoreExport bool FReadFiles(bool fOnlyIfNoImage = true,
			bool fRemoveBadImages = false);
	VisCoreExport bool FReadFiles(int first, int num, 
            bool fOnlyIfNoImage = true, bool fRemoveBadImages = false);

	// This will return false if there's an error writing the files, but
	// it will try to write every file before returning.
	VisCoreExport bool FWriteFiles(void);


	// Add a file to the end of the sequence.
	// LATER:  Should we have push / insert variations?
	VisCoreExport void AddFile(const char *szFilename,
			bool fReadFile = false);

	// Add a file to the end of the sequence.  If fReadFile is true and
	// the file can not be read, it will not be added to the sequence.
	// Returns true if the file was added to the sequence.
	// LATER:  Should we have push / insert variations?
	VisCoreExport bool FAddFile(const char *szFilename,
			bool fReadFile = false);


	// This method changes the names that will be used when files are read
	// or written, but it does not make affect the file system.
	// LATER:  We might want to add an option to affect the file system
	// by moving or deleting files when their names are changed.
	VisCoreExport void RenameFiles(const char *szBase,
			const char *szExtension, int cchDigits = 4);


	// This could return 0 if no working directory has been set.
	VisCoreExport const char *WorkingDirectory(void) const;

	VisCoreExport void SetWorkingDirectory(const char *szPath);

    // Methods used to control stream ( .avi, .. ) i/o  
    VisCoreExport void ReadStream  ( const char* filename );
    VisCoreExport void InsertStream( const char* filename, int pos );
    VisCoreExport void AppendStream( const char* filename );
    VisCoreExport void WriteStream ( const char* filename,
                                     UINT uBitCount = 32,
                                     bool fCompressorDlg = FALSE, 
                                     bool fAddSilentAudio = FALSE );

	//------------------------------------------------------------------
	// @group Property List for user-defined properties

	const CVisPropList& PropList(void) const;

	CVisPropList& PropList(void);


    //------------------------------------------------------------------
	// SDStream I / O

	// Note:  These I/O functions assume that other threads are not attempting
	// to modify the sequence while it is being read or written.

	virtual VisCoreExport void ReadValue(CVisSDIStream& refsdistream);
	virtual VisCoreExport void WriteValue(CVisSDOStream& refsdostream) const;

	void VisCoreExport BuildPropList(CVisPropList& refproplist);

	// Helper funciton
	void SDOFindTypes(CVisSDOStream& refsdostream) const;

	// These two methods are overridden in derived classes to give type
	// information (about the derived classes) to the property list code.
	virtual VisCoreExport CVisPropTypeInfoBase *PropTypeInfoNew(void) const;
	virtual VisCoreExport CVisPropTypeInfoBase *PropTypeInfoLookup(void) const;

	// LATER:  Methods to read / write a video sequence.


    //------------------------------------------------------------------
	// @group Old self-describing stream (low-level) I/O

	// LATER:  We might want to add Image Source information.
	enum FieldType { eftName, eftProperties, eftImages, eftEnd};

	// @cmember
	// Read or write a field.  Returns a description of the field.
    const char *ReadWriteField(class CVisSDStream& s, int field_id);

	// @cmember
	// Read or write properties to a self-describing stream.
	VisCoreExport virtual void SDReadWriteProperties(class CVisSDStream& s,
			bool fAddComma = false);


    //------------------------------------------------------------------
	// @group Notification callback functions

	// Notification callbacks (called when sequence is changed or destroyed).
	VisCoreExport void AddCallback(T_PfnCallback pfnCallback, void *pvUser = 0,
			bool fCallOnChange = true, bool fCallWhenDying = true);
	VisCoreExport void RemoveCallback(T_PfnCallback pfnCallback);

	bool IsDying(void) const;


	//------------------------------------------------------------------
	// @group Default settings for all sequences

	// When reading from streams, should we always use the AVI stream
	// handler?
	static VisCoreExport bool AlwaysUseAVIStreamHandler(void);
	static VisCoreExport void SetAlwaysUseAVIStreamHandler(bool f);

	// LATER:  Default sequence options (EVisSequence values)
//	static VisCoreExport int DefaultOptions(void);
//	static VisCoreExport void SetDefaultOptions(int wOptions);


	// Constant used to specify the depth of this object (for use with
	// ObjectTypeInfo).
	enum { eilevelSequenceBase = 0 };
	enum { eiLevel = 0 };

protected:
#ifdef VIS_INCLUDE_SDSTREAM
	friend class CVisSDObject< CVisSequenceBase >;
#endif
	// UNDONE (in Rel1):  Remove default values when no longer required for
	// self-describing streams.
	VisCoreExport CVisSequenceBase(EVisPixFmt evispixfmt = evispixfmtUnknown,
			int evissequence = evissequenceDefault);
	VisCoreExport CVisSequenceBase& operator=(
			const CVisSequenceBase& refsequence);
	virtual VisCoreExport ~CVisSequenceBase(void);


	// The default implementations of these methods call the notification
	// callback functions.  The SequenceIsDying method should be called by
	// the destructor in any derived class.
	// UNDONE:  Check that we do this in our derived classes.
	virtual void OnSequenceChanged(void);
	virtual void SequenceIsDying(void);


	void Push(void);
	void Push(const CVisImageBase& refimage);
	void Push(const CVisShape& refshape);
	void Push(const char *szName);
	bool Pop(void);
	bool Pop(DWORD cmsTimeout);
	bool Pop(CVisImageBase& refimage, DWORD cmsTimeout);

	void PushFront(void);
	void PushFront(const CVisImageBase& refimage);
	void PushFront(const CVisShape& refshape);
	VisCoreExport void PushFront(const char *szName);
	VisCoreExport bool PopFront(void);
	VisCoreExport bool PopFront(DWORD cmsTimeout);
	VisCoreExport bool PopFront(CVisImageBase& refimage, DWORD cmsTimeout);

	void PushBack(void);
	void PushBack(const CVisImageBase& refimage);
	void PushBack(const CVisShape& refshape);
	VisCoreExport void PushBack(const char *szName);
	VisCoreExport bool PopBack(void);
	VisCoreExport bool PopBack(DWORD cmsTimeout);
	VisCoreExport bool PopBack(CVisImageBase& refimage, DWORD cmsTimeout);

	VisCoreExport void Assign(int n);
	VisCoreExport void Assign(int n, const CVisImageBase& refimage);
	VisCoreExport void Assign(int n, const CVisShape& refshape);
	VisCoreExport void Assign(int n, const char *szName);
	VisCoreExport void Assign(const CVisSequenceBase& refsequence,
			int iFirst = 0, int iLim = - 1);
#ifdef LATER // Build problems
	VisCoreExport void Assign(CVisSequenceBase& refsequence,
			int iFirst = 0, int iLim = - 1);
#endif // LATER

	// Should the name of this method be something like SetLength?
	// Why does this take an object while Assign takes a reference?
	VisCoreExport void Resize(int n);
	VisCoreExport void Resize(int n, const CVisImageBase& refimage);
	VisCoreExport void Resize(int n, const CVisShape& refshape);
	VisCoreExport void Resize(int n, const char *szName);

	VisCoreExport void Insert(int i);
	VisCoreExport void Insert(int i, int n);
	VisCoreExport void Insert(int i, const CVisShape& refshape);
	VisCoreExport void Insert(int i, int n, const CVisShape& refshape);
	VisCoreExport void Insert(int i, const CVisImageBase& refimage);
	VisCoreExport void Insert(int i, int n, const CVisImageBase& refimage);
	VisCoreExport void Insert(int i, const char *szName);
	VisCoreExport void Insert(int i, int n, const char *szName);
	VisCoreExport void Insert(int i, const CVisSequenceBase& refsequence,
			int iFirst, int iLim);
#ifdef LATER // Build problems
	void Insert(int i, CVisSequenceBase& refsequence,
			int iFirst, int iLim);
#endif // LATER

	VisCoreExport void Swap(CVisSequenceBase& refsequence);

	virtual CVisImageBase *PImageNew(void) const;
	virtual CVisImageBase *PImageNew(const CVisShape& refshape) const;
	virtual CVisImageBase *PImageNew(const CVisImageBase& refimage) const;

	void CopyPImageSource(const CVisImageSource *pimagesource);

#ifdef VIS_INCLUDE_SDSTREAM
	void ReadWriteImages(CVisSDStream& s);
#endif
	void ReadFrontIfNeeded(void) const;
	void ReadBackIfNeeded(void) const;
	VisCoreExport void ReadIfNeeded(int iimage) const;

	// The following methods are used to notify derived classes when the
	// index of the current image or the order of images in the sequence
	// is changed.
	virtual VisCoreExport void OnICurChanged(void);
	virtual VisCoreExport void OnOrderChanged(void);


	virtual VisCoreExport void PreReadValue(CVisSDIStream& refsdistream);
	virtual VisCoreExport void PostReadValue(CVisSDIStream& refsdistream);

	virtual VisCoreExport void PreWriteValue(CVisSDOStream& refsdostream)
			const;
	virtual VisCoreExport void PostWriteValue(CVisSDOStream& refsdostream)
			const;

    virtual VisCoreExport void OpenStreams(UINT mode = OF_READ);

	VisCoreExport void ReadImageList(CVisSDIStream& refsdistream);
	VisCoreExport void WriteImageList(CVisSDOStream& refsdostream) const;

    VisCoreExport void ReadImageFile  (CVisImageBase& refimage);
    VisCoreExport void FReadImageFile (CVisImageBase& refimage);
    VisCoreExport void WriteImageFile (CVisImageBase& refimage) const;
    VisCoreExport void FWriteImageFile(CVisImageBase& refimage) const;

private:
	friend CVisImageSource;
	friend class CVisSequenceList;
//	friend bool CVisImageSource::AddNewSequence(
//		CVisSequenceBase& refsequence);

	enum
	{
		eihSemaphoreImagesAvailable = 0,
		eihSemaphoreInBlock,
		eihLim
	};

	enum { ecSemaphoreLim = (1 << 28) };

	enum { enTrueLengthMax = ecSemaphoreLim };

    struct SVisSequenceMemUsageEl
    {
        CVisImageBase          *m_pImage;
        SVisSequenceMemUsageEl *m_pNext;
        SVisSequenceMemUsageEl *m_pPrev;
    };

    struct SVisSequenceDequeEl
    {
        CVisImageBase          *m_pImage;
        SVisSequenceMemUsageEl *m_pMemUsage;
    
    	SVisSequenceDequeEl()
    	{
    		m_pImage    = NULL;
            m_pMemUsage = NULL;
    	};
    };

	typedef std::deque<SVisSequenceDequeEl>			T_Deque;
	typedef std::vector<SVisSequenceCallbackInfo>	T_Rgcallback;

	int m_evissequence;
	int m_nLengthMax;
	int m_iCur;
	EVisPixFmt m_evispixfmt;
	T_Deque m_deque;
	CVisImageSource *m_pimagesource;
	HANDLE m_rgh[eihLim];
	HANDLE m_hEventNotInBlock;
	CVisImageBase *m_pimageCur;  // Used with zero-length sequences.
	std::string m_strWorkingDirectory;
	int m_cThreadInBlock;
	int m_cWaitingForImage;
	CVisPropList m_proplist;
	T_Rgcallback m_rgcallback;
	bool m_IsDying;
	bool m_HasDied;
	bool m_fValidSequence;
	bool m_fCalledSetLengthMax;

    SVisSequenceMemUsageEl* m_pMemHead;
    SVisSequenceMemUsageEl* m_pMemTail;
    int                     m_cMemUsageSize;
	int                     m_cMemUsageMax;


	// When reading from streams, should we always use the AVI stream
	// handler?
	static bool s_fAlwaysUseAVIStreamHandler;

	// LATER:  Default sequence options (EVisSequence values)
//	static int s_evissequenceDefault;


	// LATER:  We could make these const methods by moving the m_cThreadInBlock
	// variable to an array outside of this class.
	bool FEnterBlock(bool fWriteBlock, DWORD cmsTimeout = INFINITE);
	bool FEnterReadBlock(DWORD cmsTimeout = INFINITE);
	VisCoreExport bool FEnterWriteBlock(DWORD cmsTimeout = INFINITE);
	bool FEnterReadBlockWithImage(DWORD cmsTimeout = INFINITE);
	VisCoreExport bool FEnterWriteBlockWithImage(DWORD cmsTimeout = INFINITE);
	void LeaveBlock(bool fWriteBlock);
	void LeaveReadBlock(void);
	VisCoreExport void LeaveWriteBlock(void);

	void NeedImage(void);
	VisCoreExport void ImageAdded(int cImage = 1);
	VisCoreExport void ImageRemoved(int cImage = 1);

	VisCoreExport bool FPushFrontPImage(CVisImageBase *pimage);
	VisCoreExport bool FPushBackPImage(CVisImageBase *pimage);

	VisCoreExport void DeleteFront(bool fChangeCount = true);
	VisCoreExport void DeleteBack(bool fChangeCount = true);
	VisCoreExport void DeleteImage(int i, bool fChangeCount = true );
	VisCoreExport void DeleteImage(SVisSequenceDequeEl& sDequeEl, 
                                   bool fChangeCount = true );
	VisCoreExport void DeleteImages(void);
	VisCoreExport void DeleteImages(int iFirst, int iLim);

    VisCoreExport SVisSequenceDequeEl NewDequeEl( CVisImageBase* pimage );
    VisCoreExport void InitDequeEl( SVisSequenceDequeEl&, CVisImageBase* pimage );

    VisCoreExport SVisSequenceMemUsageEl* NewMemUsageEl( CVisImageBase* pImage );
    VisCoreExport void EmptyMemUsage( SVisSequenceMemUsageEl* );
    VisCoreExport void RemoveMemUsage( SVisSequenceMemUsageEl* pEl );
    VisCoreExport void MoveToMRU( SVisSequenceMemUsageEl* pEl );
    VisCoreExport bool UpdateMRU( SVisSequenceDequeEl& sDequeEl );

	VisCoreExport void InitSynchObjects(void);

	bool PushCanRemoveOldImages(void) const;
	bool InsertCanRemoveOldImages(void) const;
};


template<class TPixel>
class CVisSequence : public CVisSequenceBase
{
public:
    //------------------------------------------------------------------
	// @group Type definitions

	typedef TPixel					T_Pixel;
	typedef CVisImage<TPixel>		T_Image;
	typedef class CVisSequenceBase	T_BaseSequence;


    //------------------------------------------------------------------
	// @group Constructors, assignement operator, and destructors

	// Note:  Use nLengthMax == - 1 to place no limits on the sequence length.
	CVisSequence(int nLength = - 1, int evissequence = evissequenceDefault);
	CVisSequence(int nLength, const T_Image& refimage,
			int evissequence = evissequenceDefault);
	CVisSequence(int nLength, const CVisShape& refshape,
			int evissequence = evissequenceDefault);
	CVisSequence(const CVisSequence<TPixel>& refsequence,
			int iFirst = 0, int iLim = - 1);
	CVisSequence(const CVisSequence<TPixel>& refsequence,
			int iFirst, int iLim, int evissequence);
	CVisSequence<TPixel>& operator=(
			const CVisSequence<TPixel>& refsequence);
	virtual ~CVisSequence(void);


    //------------------------------------------------------------------
	// @group Synchronized image access

	// These methods do synchronize threads accessing the sequence.
	// With internal thread synchronization, these can't be const methods.
	bool GetFront(T_Image& refimage, DWORD cmsTimeout = ecmsTimeoutDefault);
	bool GetBack(T_Image& refimage, DWORD cmsTimeout = ecmsTimeoutDefault);
	bool GetAt(int i, T_Image& refimage, DWORD cmsTimeout = ecmsTimeoutDefault);


    //------------------------------------------------------------------
	// @group Methods based on the std::deque methods

	// Warning:  These methods do not synchronize threads accessing the
	// sequence.  They may not work right (and may cause access violations) if
	// called when other threads are modifying the sequence.  Bad things could
	// also happen if the image specified is not in the sequence.
	const T_Image& Front(void) const;
	T_Image& Front(void);
	const T_Image& Back(void) const;
	T_Image& Back(void);
	const T_Image& At(int i) const;
	T_Image& At(int i);
	const T_Image& operator[](int i) const;
	T_Image& operator[](int i);

	void Push(void);
	void Push(const T_Image& refimage);
	void Push(const CVisShape& refshape);
	void Push(const char *szName);
	bool Pop(void);
	bool Pop(DWORD cmsTimeout);
	bool Pop(T_Image& refimage, DWORD cmsTimeout = ecmsTimeoutDefault);

	void PushFront(void);
	void PushFront(const T_Image& refimage);
	void PushFront(const CVisShape& refshape);
	void PushFront(const char *szName);
	bool PopFront(void);
	bool PopFront(DWORD cmsTimeout);
	bool PopFront(T_Image& refimage, DWORD cmsTimeout = ecmsTimeoutDefault);

	void PushBack(void);
	void PushBack(const T_Image& refimage);
	void PushBack(const CVisShape& refshape);
	void PushBack(const char *szName);
	bool PopBack(void);
	bool PopBack(DWORD cmsTimeout);
	bool PopBack(T_Image& refimage, DWORD cmsTimeout = ecmsTimeoutDefault);

	void Assign(int n);
	void Assign(int n, const T_Image& refimage);
	void Assign(int n, const CVisShape& refshape);
	void Assign(int n, const char *szName);
	void Assign(const CVisSequence<TPixel>& refsequence,
			int iFirst = 0, int iLim = - 1);
#ifdef LATER // Build problems
	void Assign(CVisSequence<TPixel>& refsequence,
			int iFirst = 0, int iLim = - 1);
#endif // LATER

	// Should the name of this method be something like SetLength?
	// Why does this take an object while Assign takes a reference?
	void Resize(int n);
	void Resize(int n, const T_Image& refimage);
	void Resize(int n, const CVisShape& refshape);
	void Resize(int n, const char *szName);

	void Insert(int i);
	void Insert(int i, int n);
	void Insert(int i, const CVisShape& refshape);
	void Insert(int i, int n, const CVisShape& refshape);
	void Insert(int i, const T_Image& refimage);
	void Insert(int i, int n, const T_Image& refimage);
	void Insert(int i, const char *szName);
	void Insert(int i, int n, const char *szName);
	void Insert(int i, const CVisSequence<TPixel>& refsequence,
			int iFirst = 0, int iLim = - 1);
#ifdef LATER // Build problems
	void Insert(int i, CVisSequence<TPixel>& refsequence,
			int iFirst = 0, int iLim = - 1);
#endif // LATER

    bool Swap(int i);
    bool Swap(int i, int j);
	void Swap(CVisSequence<TPixel>& refsequence);


    //------------------------------------------------------------------
	// @group The current image

	const T_Image& Cur(void) const;
	T_Image& Cur(void);

	bool GetCur(T_Image& refimage);


    //------------------------------------------------------------------
	// @group C++ type information

	virtual const type_info& PixelTypeInfo(void) const;
	virtual const type_info& ImageTypeInfo(void) const;
	virtual const type_info& ObjectTypeInfo(int iLevel = - 1) const;

	virtual CVisSequenceBase *Clone(bool fCopyThis = true) const;


    //------------------------------------------------------------------
	// SDStream I / O

	// These two methods are overridden in derived classes to give type
	// information (about the derived classes) to the property list code.
	virtual CVisPropTypeInfoBase *PropTypeInfoNew(void) const;
	virtual CVisPropTypeInfoBase *PropTypeInfoLookup(void) const;


	// Constant used to specify the depth of this object (for use with
	// ObjectTypeInfo).
	enum { eilevelSequence = eilevelSequenceBase + 1 };
	enum { eiLevel = T_BaseSequence::eiLevel + 1 };

protected:
	virtual CVisImageBase *PImageNew(void) const;
	virtual CVisImageBase *PImageNew(const CVisShape& refshape) const;
	virtual CVisImageBase *PImageNew(const CVisImageBase& refimage) const;

private:
	static EVisPixFmt EVisPixFmtGet(void);
};


// This will call virtual functions to write the correct information
// for derived classes.
#ifdef VIS_INCLUDE_SDSTREAM
CVisSDStream& operator<<(CVisSDStream& s, CVisSequenceBase& refsequence);
#endif


class CVisImageSourceBase
{
public:
	// These methods are used by the CVisImagePump class.
	virtual bool UseContinuousGrab(void) const = 0;
	virtual void SetUseContinuousGrab(bool f) = 0;
	
protected:
	friend CVisSequenceBase;
	virtual bool AddSequence(CVisSequenceBase& refsequence,
			bool fModifyImEncoding = true, bool fUseContinuousGrab = true) = 0;
	virtual void DeleteFromSequence(CVisSequenceBase& refsequence) = 0;
	virtual void NeedImage(CVisSequenceBase& refsequence) = 0;
};


#ifndef VIS_ONLY_DECLARE_PROPLIST

VIS_DECLARE_PROP_LIST_CLASS_VIRTUAL(VisCoreExportDeclspec, CVisSequenceBase)

#ifndef VIS_NO_SD_STREAM

// We need to use a macro for each I/O function because we want to
// use the class property list to find type information but use
// class ReadValue / WriteValue methods when reading and writing.
VIS_DECLARE_SDI_FIND_TYPES_PL(VisCoreExportDeclspec, CVisSequenceBase)
VIS_DECLARE_SDI_READ_VALUE_USING_CLASS_METHOD(VisCoreExportDeclspec, CVisSequenceBase)
VIS_DECLARE_SDI_READ_OBJ_DEFAULT(VisCoreExportDeclspec, CVisSequenceBase)
VIS_DECLARE_SDI_READ_OP_DEFAULT(VisCoreExportDeclspec, CVisSequenceBase)

VIS_DECLARE_SDO_FIND_TYPES_PL(VisCoreExportDeclspec, CVisSequenceBase)
VIS_DECLARE_SDO_WRITE_VALUE_USING_CLASS_METHOD(VisCoreExportDeclspec, CVisSequenceBase)
VIS_DECLARE_SDO_WRITE_OBJ_DEFAULT(VisCoreExportDeclspec, CVisSequenceBase)
VIS_DECLARE_SDO_WRITE_OP_DEFAULT(VisCoreExportDeclspec, CVisSequenceBase)

#endif // !VIS_NO_SD_STREAM

#endif // VIS_ONLY_DECLARE_PROPLIST



typedef CVisSequence<signed char>				CVisCharSequence;
typedef CVisSequence<short>						CVisShortSequence;
typedef CVisSequence<int>						CVisIntSequence;
typedef CVisSequence<long>						CVisLongSequence;

typedef CVisSequence<unsigned char>				CVisByteSequence;
typedef CVisSequence<unsigned char>				CVisUCharSequence;
typedef CVisSequence<unsigned short>			CVisUShortSequence;
typedef CVisSequence<unsigned int>				CVisUIntSequence;
typedef CVisSequence<unsigned long>				CVisULongSequence;

typedef CVisSequence<float>						CVisFloatSequence;
typedef CVisSequence<double>					CVisDoubleSequence;


typedef CVisSequence<CVisRGBACharPixel>			CVisRGBACharSequence;
typedef CVisSequence<CVisRGBAShortPixel>		CVisRGBAShortSequence;
typedef CVisSequence<CVisRGBAIntPixel>			CVisRGBAIntSequence;
typedef CVisSequence<CVisRGBALongPixel>			CVisRGBALongSequence;

typedef CVisSequence<CVisRGBABytePixel>			CVisRGBAByteSequence;
typedef CVisSequence<CVisRGBAUCharPixel>		CVisRGBAUCharSequence;
typedef CVisSequence<CVisRGBAUShortPixel>		CVisRGBAUShortSequence;
typedef CVisSequence<CVisRGBAUIntPixel>			CVisRGBAUIntSequence;
typedef CVisSequence<CVisRGBAULongPixel>		CVisRGBAULongSequence;

typedef CVisSequence<CVisRGBAFloatPixel>		CVisRGBAFloatSequence;
typedef CVisSequence<CVisRGBADoublePixel>		CVisRGBADoubleSequence;


typedef CVisSequence<CVisYUVACharPixel>			CVisYUVACharSequence;
typedef CVisSequence<CVisYUVAShortPixel>		CVisYUVAShortSequence;
typedef CVisSequence<CVisYUVAIntPixel>			CVisYUVAIntSequence;
typedef CVisSequence<CVisYUVALongPixel>			CVisYUVALongSequence;

typedef CVisSequence<CVisYUVABytePixel>			CVisYUVAByteSequence;
typedef CVisSequence<CVisYUVAUCharPixel>		CVisYUVAUCharSequence;
typedef CVisSequence<CVisYUVAUShortPixel>		CVisYUVAUShortSequence;
typedef CVisSequence<CVisYUVAUIntPixel>			CVisYUVAUIntSequence;
typedef CVisSequence<CVisYUVAULongPixel>		CVisYUVAULongSequence;

typedef CVisSequence<CVisYUVAFloatPixel>		CVisYUVAFloatSequence;
typedef CVisSequence<CVisYUVADoublePixel>		CVisYUVADoubleSequence;



#include "VisSequence.inl"

#endif // __VIS_CORE_SEQUENCE_H__

