// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1994 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and the
// Books Online documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"

#ifdef AFXCTL_FACT_SEG
#pragma code_seg(AFXCTL_FACT_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW


/////////////////////////////////////////////////////////////////////////////
// AfxVerifyLicFile - Checks that a license file exists and contains a
//    specific byte pattern.

BOOL AFXAPI AfxVerifyLicFile(HINSTANCE hInstance, LPCTSTR pszLicFileName,
	LPCTSTR pszLicFileContents, UINT cch)
{
	// Assume the worst...
	BOOL bVerified = FALSE;

	// Look for license file in same directory as this DLL.
	TCHAR szPathName[_MAX_PATH];
	::GetModuleFileName(hInstance, szPathName, _MAX_PATH);
	LPTSTR pszFileName = _tcsrchr(szPathName, '\\') + 1;
	_tcscpy(pszFileName, pszLicFileName);

	LPSTR pszKey = NULL;
	LPBYTE pbContent = NULL;

	TRY
	{
		// Open file, read content and compare.

		CFile file(szPathName, CFile::modeRead);

		if (cch == -1)
			cch = _tcslen(pszLicFileContents);

#ifdef _UNICODE
		pszKey = new char[cch * 2];
		wcstombs(pszKey, pszLicFileContents, cch * 2);
#else
		pszKey = (LPSTR)pszLicFileContents;
#endif

		pbContent = new BYTE[cch];
		file.Read(pbContent, cch);

		if (_fmemcmp(pszKey, pbContent, (size_t)cch) == 0)
			bVerified = TRUE;
	}
	END_TRY

#ifdef _UNICODE
	delete [] pszKey;
#endif

	delete [] pbContent;

	return bVerified;
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
