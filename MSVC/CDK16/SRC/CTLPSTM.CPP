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

#ifdef AFXCTL_PROP_SEG
#pragma code_seg(AFXCTL_PROP_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// COleControl::XPersistStreamInit

STDMETHODIMP_(ULONG) COleControl::XPersistStreamInit::AddRef()
{
	METHOD_MANAGE_STATE(COleControl, PersistStreamInit)

	// Delegate to our exported AddRef.
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleControl::XPersistStreamInit::Release()
{
	METHOD_MANAGE_STATE(COleControl, PersistStreamInit)

	// Delegate to our exported Release.
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP COleControl::XPersistStreamInit::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_MANAGE_STATE(COleControl, PersistStreamInit)

	// Delegate to our exported QueryInterface.
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP COleControl::XPersistStreamInit::GetClassID(LPCLSID lpClassID)
{
	METHOD_MANAGE_STATE(COleControl, PersistStreamInit)

	// Delegate to our main implementation of GetClassID.
	return pThis->GetClassID(lpClassID);
}

STDMETHODIMP COleControl::XPersistStreamInit::IsDirty()
{
	METHOD_MANAGE_STATE(COleControl, PersistStreamInit)

	// Return NOERROR (S_OK) if modified, and S_FALSE otherwise.
	return pThis->m_bModified ? NOERROR : ResultFromScode(S_FALSE);
}

STDMETHODIMP COleControl::XPersistStreamInit::Load(LPSTREAM pStm)
{
	METHOD_MANAGE_STATE(COleControl, PersistStreamInit)

	// Delegate to LoadState.
	HRESULT hr = pThis->LoadState(pStm);

	// Create window for subclassed control, if necessary.
	pThis->CreateWindowForSubclassedControl();

	return hr;
}

STDMETHODIMP COleControl::XPersistStreamInit::Save(LPSTREAM pStm, BOOL fClearDirty)
{
	METHOD_MANAGE_STATE(COleControl, PersistStreamInit)

	HRESULT hr = NOERROR;

	// Delegate to SaveState.
	hr = pThis->SaveState(pStm);

	// Bookkeeping:  Clear the dirty flag, if requested.
	if (fClearDirty)
		pThis->m_bModified = FALSE;

	return hr;
}

STDMETHODIMP COleControl::XPersistStreamInit::GetSizeMax(ULARGE_INTEGER FAR*)
{
	return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP COleControl::XPersistStreamInit::InitNew()
{
	METHOD_MANAGE_STATE(COleControl, PersistStreamInit)

	// Delegate to OnResetState.
	pThis->OnResetState();

	// Unless IOleObject::SetClientSite is called after this, we can
	// count on ambient properties being available while loading.
	pThis->m_bCountOnAmbients = TRUE;

	// Create window for subclassed control, if necessary.
	pThis->CreateWindowForSubclassedControl();

	return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
