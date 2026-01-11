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

#ifdef AFXCTL_CORE3_SEG
#pragma code_seg(AFXCTL_CORE3_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// COleDispatchExceptionEx constructor

COleDispatchExceptionEx::COleDispatchExceptionEx(LPCTSTR lpszDescription,
	UINT nHelpID, SCODE sc) :
	COleDispatchException(lpszDescription, nHelpID, 0)
{
	m_scError = sc;
	m_wCode = 0;
}


/////////////////////////////////////////////////////////////////////////////
// COleControl::ThrowError

void COleControl::ThrowError(SCODE sc, UINT nDescriptionID, UINT nHelpID)
{
	TCHAR szBuffer[256];
#ifdef _WIN32
	AfxLoadString(nDescriptionID, szBuffer);
#else
	_AfxLoadString(nDescriptionID, szBuffer);
#endif
	if (nHelpID == -1)
		nHelpID = nDescriptionID;
	THROW(new COleDispatchExceptionEx(szBuffer, nHelpID, sc));
}

void COleControl::ThrowError(SCODE sc, LPCTSTR pszDescription, UINT nHelpID)
{
	THROW(new COleDispatchExceptionEx(pszDescription, nHelpID, sc));
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
