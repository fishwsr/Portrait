// VisXCLAPACK.inl
//
// Copyright © 1998 - 2000 Microsoft Corporation, All Rights Reserved


#ifndef __VIS_XCLAPACK_INL__
#define __VIS_XCLAPACK_INL__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// Accessor function.
// LATER:  Add an option to throw exceptions if not available?
inline class CVisXCLAPACK *VisFindPXCLAPACK(void)
{
	static bool fLookedForVisXCLAPACK = false;
	static class CVisXCLAPACK *pxclapack = 0;
	static HINSTANCE hinstXCLAPACKDLL = 0;

	if (!fLookedForVisXCLAPACK)
	{
		hinstXCLAPACKDLL = LoadLibrary(VIS_XCLAPACK_NAME);

		if (hinstXCLAPACKDLL != 0)
		{
			const char *szXCLAPACKExport = "?g_xclapack@@3VCVisXCLAPACK@@A";

			pxclapack = (class CVisXCLAPACK *)
					GetProcAddress(hinstXCLAPACKDLL, szXCLAPACKExport);

			if (pxclapack == 0)
			{
				// This shouldn't happen.
				assert(0);
				FreeLibrary(hinstXCLAPACKDLL);
				hinstXCLAPACKDLL = 0;
			}
		}

		fLookedForVisXCLAPACK = true;
	}

	return pxclapack;
}


#endif // __VIS_XCLAPACK_INL__
