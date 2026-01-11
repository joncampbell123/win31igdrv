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
// COleControl::XOleCache


STDMETHODIMP_(ULONG) COleControl::XOleCache::AddRef()
{
	//
	//  Delegate to our exported AddRef.
	//

	METHOD_MANAGE_STATE(COleControl, OleCache)
	return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) COleControl::XOleCache::Release()
{
	//
	//  Delegate to our exported Release.
	//

	METHOD_MANAGE_STATE(COleControl, OleCache)
	return (ULONG)pThis->ExternalRelease();
}


STDMETHODIMP COleControl::XOleCache::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	//
	//  Delegate to our exported QueryInterface.
	//

	METHOD_MANAGE_STATE(COleControl, OleCache)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP COleControl::XOleCache::Cache(LPFORMATETC lpFormatetc, DWORD advf, LPDWORD lpdwConnection)
{
	METHOD_MANAGE_STATE(COleControl, OleCache)
	ASSERT_VALID(pThis);

	return pThis->m_pDefIOleCache->Cache(lpFormatetc, advf, lpdwConnection);
}


STDMETHODIMP COleControl::XOleCache::Uncache(DWORD dwConnection)
{
	METHOD_MANAGE_STATE(COleControl, OleCache)
	ASSERT_VALID(pThis);

	return pThis->m_pDefIOleCache->Uncache(dwConnection);
}


STDMETHODIMP COleControl::XOleCache::EnumCache(LPENUMSTATDATA FAR* ppenumStatData)
{
	METHOD_MANAGE_STATE(COleControl, OleCache)
	ASSERT_VALID(pThis);

	return pThis->m_pDefIOleCache->EnumCache(ppenumStatData);
}


STDMETHODIMP COleControl::XOleCache::InitCache(LPDATAOBJECT pDataObject)
{
	METHOD_MANAGE_STATE(COleControl, OleCache)
	ASSERT_VALID(pThis);

	return pThis->m_pDefIOleCache->InitCache(pDataObject);
}


STDMETHODIMP COleControl::XOleCache::SetData(LPFORMATETC pformatetc, STGMEDIUM FAR* pmedium, BOOL fRelease)
{
	METHOD_MANAGE_STATE(COleControl, OleCache)
	ASSERT_VALID(pThis);

	return pThis->m_pDefIOleCache->SetData(pformatetc, pmedium, fRelease);
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
