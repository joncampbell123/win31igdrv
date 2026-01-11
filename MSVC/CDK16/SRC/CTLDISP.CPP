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

#ifdef AFXCTL_CORE2_SEG
#pragma code_seg(AFXCTL_CORE2_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// COleControl::GetTypeInfoOfGuid - Returns typeinfo

HRESULT COleControl::GetTypeInfoOfGuid(LCID lcid, REFGUID guid,
	LPTYPEINFO FAR* ppTypeInfo)
{
	CTypeLibCache* pTypeLibCache = GetTypeLibCache();
	LPTYPELIB pTypeLib = NULL;

	// If type info is already cached, just return it.
	if (pTypeLibCache->LookupTypeInfo(lcid, guid, ppTypeInfo))
	{
		return NOERROR;
	}
	else
	{
		// If type library isn't already cached, try to locate it.
		if (! pTypeLibCache->Lookup(lcid, &pTypeLib))
		{
			// First, try getting the subclass to load the type library
			// (normally this goes through LoadRegTypeLib).

			if (FAILED(GetTypeLib(lcid, &pTypeLib)))
			{
				// If that failed, try loading the type library from our own
				// resources.

				TCHAR szPath[_MAX_PATH];
				GetModuleFileName(AfxGetInstanceHandle(), szPath, _MAX_PATH);

				if (FAILED(LoadTypeLib(szPath, &pTypeLib)))
					pTypeLib = NULL;
			}

			pTypeLibCache->Cache(lcid, pTypeLib);
		}

		// If we got a type library, extract the requested type info.
		if (pTypeLib != NULL)
		{
			HRESULT hr = pTypeLib->GetTypeInfoOfGuid(guid, ppTypeInfo);
			pTypeLib->Release();
			pTypeLibCache->CacheTypeInfo(lcid, guid, *ppTypeInfo);
			return hr;
		}
	}

	return ResultFromScode(TYPE_E_CANTLOADLIBRARY);
}


/////////////////////////////////////////////////////////////////////////////
// COleControl::GetTypeLibCache - Return class-specific cache

CTypeLibCache* COleControl::GetTypeLibCache()
{
	//  Subclass must implement (typically via IMPLEMENT_OLETYPELIB macro)
	ASSERT(FALSE);
	return NULL;
}


/////////////////////////////////////////////////////////////////////////////
// COleControl::GetTypeLib - Return class-specific type library

HRESULT COleControl::GetTypeLib(LCID, LPTYPELIB FAR*)
{
	//  Subclass must implement (typically via IMPLEMENT_OLETYPELIB macro)
	ASSERT(FALSE);
	return ResultFromScode(E_FAIL);
}


/////////////////////////////////////////////////////////////////////////////
// CTypeLibCache

CTypeLibCache* PASCAL CTypeLibCache::GetTypeLibCache(const GUID FAR* ptlid)
{
	CMapPtrToPtr* pMap = AfxGetExtraDataMap();
	ASSERT_VALID(pMap);

	CTypeLibCache* pCache;
	if (!pMap->Lookup((void*)(const void*) ptlid, (void*&) pCache))
		pCache = new CTypeLibCache(ptlid);  // Adds itself to extra map

	return pCache;
}

CTypeLibCache::CTypeLibCache(const GUID FAR* ptlid)
{
	m_ptlid = ptlid;
	m_lcid = (LCID)-1;
	m_ptlib = NULL;
	m_guidInfo = GUID_NULL;
	m_ptinfo = NULL;
	m_cRef = 0;

	CMapPtrToPtr* pMap = AfxGetExtraDataMap();
	ASSERT_VALID(pMap);

#ifdef _DEBUG
	BOOL bEnable = AfxEnableMemoryTracking(FALSE);
#endif
	pMap->SetAt((void*)(const void*) m_ptlid, this);
#ifdef _DEBUG
	AfxEnableMemoryTracking(bEnable);
#endif
}

CTypeLibCache::~CTypeLibCache()
{
	RELEASE(m_ptinfo);
	RELEASE(m_ptlib);

	CMapPtrToPtr* pMap = AfxGetExtraDataMap();
	ASSERT_VALID(pMap);

	pMap->RemoveKey((void*)(const void*) m_ptlid);
}

void CTypeLibCache::Lock()
{
	ASSERT(m_cRef >= 0);
	++m_cRef;
}

void CTypeLibCache::Unlock()
{
	ASSERT(m_cRef > 0);

	if (--m_cRef == 0)
		delete this;
}

BOOL CTypeLibCache::Lookup(LCID lcid, LPTYPELIB FAR* pptlib)
{
	if ((m_lcid != -1) && (m_lcid == lcid))
	{
		ASSERT(m_ptlib != NULL);
		*pptlib = m_ptlib;
		m_ptlib->AddRef();
		return TRUE;
	}
	else
	{
		*pptlib = NULL;
		return FALSE;
	}
}

void CTypeLibCache::Cache(LCID lcid, LPTYPELIB ptlib)
{
	if (ptlib != NULL)
	{
		m_lcid = lcid;
		m_guidInfo = GUID_NULL;
		RELEASE(m_ptinfo);
		RELEASE(m_ptlib);
		m_ptlib = ptlib;
		m_ptlib->AddRef();
	}
}

BOOL CTypeLibCache::LookupTypeInfo(LCID lcid, REFGUID guid,
	LPTYPEINFO FAR* pptinfo)
{
	if ((m_lcid != -1) && (m_lcid == lcid) && IsEqualGUID(m_guidInfo, guid))
	{
		ASSERT(m_ptlib != NULL);
		ASSERT(m_ptinfo != NULL);
		*pptinfo = m_ptinfo;
		m_ptinfo->AddRef();
		return TRUE;
	}
	else
	{
		*pptinfo = NULL;
		return FALSE;
	}
}

void CTypeLibCache::CacheTypeInfo(LCID lcid, REFGUID guid, LPTYPEINFO ptinfo)
{
	if ((ptinfo != NULL) && (m_lcid == lcid))
	{
		m_guidInfo = guid;
		RELEASE(m_ptinfo);
		m_ptinfo = ptinfo;
		m_ptinfo->AddRef();
	}
}

/////////////////////////////////////////////////////////////////////////////
// COleControl::XDispatch


STDMETHODIMP_(ULONG) COleControl::XDispatch::AddRef()
{
	//
	//  Delegate to our exported AddRef.
	//

	METHOD_MANAGE_STATE(COleControl, Dispatch)
	return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) COleControl::XDispatch::Release()
{
	//
	//  Delegate to our exported Release.
	//

	METHOD_MANAGE_STATE(COleControl, Dispatch)
	return (ULONG)pThis->ExternalRelease();
}


STDMETHODIMP COleControl::XDispatch::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	//
	//  Delegate to our exported QueryInterface.
	//

	METHOD_MANAGE_STATE(COleControl, Dispatch)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP COleControl::XDispatch::GetTypeInfoCount(unsigned int FAR* pctinfo)
{
	METHOD_MANAGE_STATE(COleControl, Dispatch)

	ASSERT_POINTER(pctinfo, unsigned int);

	*pctinfo = 1;
	return NOERROR;
}


STDMETHODIMP COleControl::XDispatch::GetTypeInfo(
	  unsigned int itinfo,
	  LCID lcid,
	  ITypeInfo FAR* FAR* pptinfo)
{
	METHOD_MANAGE_STATE(COleControl, Dispatch)

	ASSERT_VALID(pThis);
	ASSERT_POINTER(pptinfo, LPTYPEINFO);

	if (itinfo == 0)
		return pThis->GetTypeInfoOfGuid(lcid, *pThis->m_piidPrimary, pptinfo);

	return ResultFromScode(E_INVALIDARG);
}


STDMETHODIMP COleControl::XDispatch::GetIDsOfNames(
	  REFIID riid,
	  LPTSTR FAR* rgszNames,
	  unsigned int cNames,
	  LCID lcid,
	  DISPID FAR* rgdispid)
{
	METHOD_MANAGE_STATE(COleControl, Dispatch)

	ASSERT_POINTER(rgszNames, char FAR*);
	ASSERT_POINTER(rgdispid, DISPID);

	// check arguments
	if (riid != IID_NULL)
		return ResultFromScode(DISP_E_UNKNOWNINTERFACE);

	HRESULT hr = ResultFromScode(E_FAIL);
	LPTYPEINFO ptinfo;

	if (lcid == 0)
	{
		hr = ((LPDISPATCH)&pThis->CCmdTarget::m_xDispatch)->GetIDsOfNames(
				riid, rgszNames, cNames, lcid, rgdispid);
	}
	else if (SUCCEEDED(hr = GetTypeInfo(0, lcid, &ptinfo)))
	{
		// For non-zero lcid, let typeinfo do the work
		ASSERT(ptinfo != NULL);
		hr = ptinfo->GetIDsOfNames(rgszNames, cNames, rgdispid);
		ptinfo->Release();

		if (GetScode(hr) == TYPE_E_ELEMENTNOTFOUND)
			hr = ResultFromScode(DISP_E_UNKNOWNNAME);
	}

	return hr;
}


STDMETHODIMP COleControl::XDispatch::Invoke(
	  DISPID dispidMember,
	  REFIID riid,
	  LCID lcid,
	  unsigned short wFlags,
	  DISPPARAMS FAR* pdispparams,
	  VARIANT FAR* pvarResult,
	  EXCEPINFO FAR* pexcepinfo,
	  unsigned int FAR* puArgErr)
{
	METHOD_MANAGE_STATE(COleControl, Dispatch)

	ASSERT_POINTER(pdispparams, DISPPARAMS);
	ASSERT_NULL_OR_POINTER(pvarResult, VARIANT);
	ASSERT_NULL_OR_POINTER(pexcepinfo, EXCEPINFO);
	ASSERT_NULL_OR_POINTER(puArgErr, unsigned int);

	return ((LPDISPATCH)&pThis->CCmdTarget::m_xDispatch)->Invoke(
			dispidMember,
			riid,
			lcid,
			wFlags,
			pdispparams,
			pvarResult,
			pexcepinfo,
			puArgErr);
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
