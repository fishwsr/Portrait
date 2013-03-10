// VisPictureFileHandler.h
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved

#pragma once

struct SVisFileDescriptor;

class CVisPictureFileHandler : public CVisFileHandler
{
// @access Public members
public:
	//------------------------------------------------------------------
	// @group Constructor and destructor


	// @cmember
   	// 
	CVisPictureFileHandler(void);

	// @cmember
   	// 
	~CVisPictureFileHandler(void);


	//------------------------------------------------------------------
	// @group Virtual functions

	// @cmember
	// Does the handler support images of the specified pixel type with
	// the specifed number of bands?
    virtual BOOL SupportsPixelType(int evispixfmt, int nbands);

	// @cmember
	// Does the specified extension match the extension of a file type
	// supported by this file handler?
    virtual BOOL MatchExtension(const char *szFiletype);

	// @cmember
	// Should the stream be opened outside of this file handler.
	virtual BOOL ExternallyOpenStream(void);

	// @cmember
 	// Attempt to read the file header.  Return TRUE is successful.
 	// (This may return FALSE or throw an exception if not successful.)
    virtual BOOL ReadHeader(SVisFileDescriptor &fd, class CVisImageBase &img);

	// @cmember
  	// Attempt to read the file body.  Return TRUE is successful.
 	// (This may return FALSE or throw an exception if not successful.)
    virtual BOOL ReadBody(SVisFileDescriptor &fd, class CVisImageBase &img);

	// @cmember
  	// Attempt to write the file header.  Return TRUE is successful.
 	// (This may return FALSE or throw an exception if not successful.)
    virtual BOOL WriteHeader(SVisFileDescriptor &fd, class CVisImageBase &img);

	// @cmember
   	// Attempt to write the file body.  Return TRUE is successful.
 	// (This may return FALSE or throw an exception if not successful.)
    virtual BOOL WriteBody(SVisFileDescriptor &fd, class CVisImageBase &img);


// @access:(IMPL) Private members
private:
	interface IPicture		*m_pIPicture;
}; 

