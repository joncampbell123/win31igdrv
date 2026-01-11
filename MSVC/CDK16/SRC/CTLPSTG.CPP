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

static LPCTSTR s_szContents = _T("Contents");


/////////////////////////////////////////////////////////////////////////////
// COleControl::XPersistStorage


STDMETHODIMP_(ULONG) COleControl::XPersistStorage::AddRef()
{
	// Delegate to our exported AddRef.
	METHOD_MANAGE_STATE(COleControl, PersistStorage)
	return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) COleControl::XPersistStorage::Release()
{
	// Delegate to our exported Release.
	METHOD_MANAGE_STATE(COleControl, PersistStorage)
	return (ULONG)pThis->ExternalRelease();
}


STDMETHODIMP COleControl::XPersistStorage::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	// Delegate to our exported QueryInterface.
	METHOD_MANAGE_STATE(COleControl, PersistStorage)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP COleControl::XPersistStorage::GetClassID(LPCLSID lpClassID)
{
	// Delegate to our main implementation of GetClassID.
	METHOD_MANAGE_STATE(COleControl, PersistStorage)
	return pThis->GetClassID(lpClassID);
}


STDMETHODIMP COleControl::XPersistStorage::IsDirty()
{
	// Return NOERROR (S_OK) if modified, and S_FALSE otherwise.
	METHOD_MANAGE_STATE(COleControl, PersistStorage)

	BOOL fDefModified = (pThis->m_pDefIPersistStorage->IsDirty() == NOERROR);
	return (fDefModified || pThis->m_bModified) ? NOERROR : ResultFromScode(S_FALSE);
}


STDMETHODIMP COleControl::XPersistStorage::InitNew(LPSTORAGE pStg)
{
	METHOD_MANAGE_STATE(COleControl, PersistStorage)

	pThis->m_pDefIPersistStorage->InitNew(pStg);

	// Delegate to OnResetState.
	pThis->OnResetState();

	// Unless IOleObject::SetClientSite is called after this, we can
	// count on ambient properties being available while loading.
	pThis->m_bCountOnAmbients = TRUE;

	// Create window for subclassed control, if necessary.
	pThis->CreateWindowForSubclassedControl();

	return NOERROR;
}


STDMETHODIMP COleControl::XPersistStorage::Load(LPSTORAGE pStg)
{
	ASSERT(pStg != NULL);
	METHOD_MANAGE_STATE(COleControl, PersistStorage)

	CLIPFORMAT cf;
	HRESULT hr;
	CLSID fmtid;

	hr = ::ReadFmtUserTypeStg(pStg, &cf, NULL);

	if (SUCCEEDED(hr) && _AfxOleMatchPropsetClipFormat(cf, &fmtid))
	{
		// Load the property set data
		FORMATETC formatEtc;
		STGMEDIUM stgMedium;
		formatEtc.cfFormat = cf;
		formatEtc.ptd = NULL;
		formatEtc.dwAspect = DVASPECT_CONTENT;
		formatEtc.lindex = -1;
		formatEtc.tymed = TYMED_ISTORAGE;
		stgMedium.tymed = TYMED_ISTORAGE;
		stgMedium.pstg = pStg;
		stgMedium.pUnkForRelease = NULL;
		hr = pThis->SetPropsetData(&formatEtc, &stgMedium, fmtid) ?
			NOERROR : ResultFromScode(E_FAIL);
	}
	else
	{
		// Open the "Contents" stream of the supplied storage object, and
		// then delegate to same implementation as IPersistStreamInit::Load.
		LPSTREAM pStm = NULL;
		hr = pStg->OpenStream(
							s_szContents, NULL,
							STGM_READ | STGM_SHARE_EXCLUSIVE,
							0, &pStm);

		ASSERT(FAILED(hr) || pStm != NULL);

		if (pStm != NULL)
		{
			// Delegate to LoadState.
			hr = pThis->LoadState(pStm);
			pStm->Release();
		}
	}

	// Delegate to default handler (for cache).
	pThis->m_pDefIPersistStorage->Load(pStg);

	// Create window for subclassed control, if necessary.
	pThis->CreateWindowForSubclassedControl();

	return hr;
}


STDMETHODIMP COleControl::XPersistStorage::Save(
	LPSTORAGE pStg, BOOL fSameAsLoad)
{
	ASSERT(pStg != NULL);
	METHOD_MANAGE_STATE(COleControl, PersistStorage)

	// Create a "Contents" stream on the supplied storage object, and
	// then delegate to the implementation of IPersistStreamInit::Save.

	//  Don't bother saving if destination is up-to-date.
	if (fSameAsLoad && !pThis->m_bModified)
		return NOERROR;

	LPSTREAM pStm = NULL;
	HRESULT hr = pStg->CreateStream(
						s_szContents,
						STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
						0, 0, &pStm);

	ASSERT(FAILED(hr) || pStm != NULL);

	if (pStm != NULL)
	{
		// Delegate to SaveState.
		hr = pThis->SaveState(pStm);

		// Bookkeeping:  Clear the dirty flag, if storage is same.
		if (fSameAsLoad)
			pThis->m_bModified = FALSE;

		pStm->Release();
	}

	// Delegate to default handler (for cache).
	pThis->m_pDefIPersistStorage->Save(pStg, fSameAsLoad);
	return hr;
}


STDMETHODIMP COleControl::XPersistStorage::SaveCompleted(LPSTORAGE pStgSaved)
{
	METHOD_MANAGE_STATE(COleControl, PersistStorage)
	return pThis->m_pDefIPersistStorage->SaveCompleted(pStgSaved);
}


STDMETHODIMP COleControl::XPersistStorage::HandsOffStorage()
{
	METHOD_MANAGE_STATE(COleControl, PersistStorage)
	return pThis->m_pDefIPersistStorage->HandsOffStorage();
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
