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
// AfxConnectionAdvise

BOOL AFXAPI AfxConnectionAdvise(LPUNKNOWN pUnkSrc, REFIID iid,
	LPUNKNOWN pUnkSink, BOOL bRefCount, DWORD FAR* pdwCookie)
{
	ASSERT_POINTER(pUnkSrc, IUnknown);
	ASSERT_POINTER(pUnkSink, IUnknown);
	ASSERT_POINTER(pdwCookie, DWORD);

	BOOL bSuccess = FALSE;

	LPCONNECTIONPOINTCONTAINER pCPC;

	if (SUCCEEDED(pUnkSrc->QueryInterface(
					IID_IConnectionPointContainer,
					(LPVOID FAR*)&pCPC)))
	{
		ASSERT_POINTER(pCPC, IConnectionPointContainer);

		LPCONNECTIONPOINT pCP;

		if (SUCCEEDED(pCPC->FindConnectionPoint(iid, &pCP)))
		{
			ASSERT_POINTER(pCP, IConnectionPoint);

			if (SUCCEEDED(pCP->Advise(pUnkSink, pdwCookie)))
				bSuccess = TRUE;

			pCP->Release();

			// The connection point just AddRef'ed us.  If we don't want to
			// keep this reference count (because it would prevent us from
			// being deleted; our reference count wouldn't go to zero), then
			// we need to cancel the effects of the AddRef by calling
			// Release.

			if (bSuccess && !bRefCount)
				pUnkSink->Release();
		}

		pCPC->Release();
	}

	return bSuccess;
}


/////////////////////////////////////////////////////////////////////////////
// AfxConnectionAdvise

BOOL AFXAPI AfxConnectionUnadvise(LPUNKNOWN pUnkSrc, REFIID iid,
	LPUNKNOWN pUnkSink, BOOL bRefCount, DWORD dwCookie)
{
	ASSERT_POINTER(pUnkSrc, IUnknown);
	ASSERT_POINTER(pUnkSink, IUnknown);

	// When we call Unadvise, the connection point will Release us.  If we
	// didn't keep the reference count when we called Advise, we need to
	// AddRef now, to keep our reference count consistent.  Note that if
	// the Unadvise fails, then we need to undo this extra AddRef by
	// calling Release before we return.

	if (!bRefCount)
		pUnkSink->AddRef();

	BOOL bSuccess = FALSE;

	LPCONNECTIONPOINTCONTAINER pCPC;

	if (SUCCEEDED(pUnkSrc->QueryInterface(
					IID_IConnectionPointContainer,
					(LPVOID FAR*)&pCPC)))
	{
		ASSERT_POINTER(pCPC, IConnectionPointContainer);

		LPCONNECTIONPOINT pCP;

		if (SUCCEEDED(pCPC->FindConnectionPoint(iid, &pCP)))
		{
			ASSERT_POINTER(pCP, IConnectionPoint);

			if (SUCCEEDED(pCP->Unadvise(dwCookie)))
				bSuccess = TRUE;

			pCP->Release();
		}

		pCPC->Release();
	}

	// If we failed, undo the earlier AddRef.

	if (!bRefCount && !bSuccess)
		pUnkSink->Release();

	return bSuccess;
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
