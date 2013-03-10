// VisMFCTypes.inl
//
// Copyright ?1992-2000 Microsoft Corporation, All Rights Reserved


#ifdef VIS_IS_USING_MFC

template<class TDocTemplate>
inline CVisDocTemplate<TDocTemplate>::CVisDocTemplate(UINT nIDResource,
		CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass,
		CRuntimeClass* pViewClass)
  : TDocTemplate( nIDResource, pDocClass, pFrameClass, pViewClass )
{
}

template<class TDocTemplate>
inline CDocTemplate::Confidence CVisDocTemplate<TDocTemplate>::MatchDocType(
		LPCTSTR lpszPathName, CDocument*& rpDocMatch)
{
	ASSERT(lpszPathName != NULL);
	rpDocMatch = NULL;

	// go through all documents
	POSITION pos = GetFirstDocPosition();
	while (pos != NULL)
	{
		CDocument* pDoc = GetNextDoc(pos);
		if (AfxComparePath(pDoc->GetPathName(), lpszPathName))
		{
			// already open
			rpDocMatch = pDoc;
			return yesAlreadyOpen;
		}
	}

	// see if it matches our default suffix
	CString strFilterExt;
	if (GetDocString(strFilterExt, CDocTemplate::filterExt) &&
	  !strFilterExt.IsEmpty())
	{
		// This is where this method and the MFC supplied method differ
        // in the case of multiple file extensions, the ';' needs to be
        // used as a token and each ';' seperated extension needs to
        // be tested seperately.
		LPCTSTR lpszDot = _tcsrchr(lpszPathName, '.');
        LPCTSTR lpext   = (LPCTSTR)strFilterExt; 
        LPTSTR  lpsemi  = (LPTSTR)lpext;
        while( 1 )
        {
     		ASSERT(*lpext == '.');

            if( lpsemi = strchr( lpsemi, ';' ) )
            {
                *lpsemi = '\0';
                lpsemi++;
            }
            
            if (lpszDot != NULL && lstrcmpi(lpszDot, lpext) == 0)
			    return yesAttemptNative; // extension matches, looks like ours
            
            if( !lpsemi || !(lpext = strchr( lpsemi, '.' )) )
            {
                break;
            }
        }
	}

	// otherwise we will guess it may work
	return yesAttemptForeign;
}

#endif // VIS_IS_USING_MFC
