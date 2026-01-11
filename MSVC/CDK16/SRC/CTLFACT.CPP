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

#if !defined(_WIN32)
#include "auxvars.h"
#endif

#ifdef AFXCTL_FACT_SEG
#pragma code_seg(AFXCTL_FACT_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW


/////////////////////////////////////////////////////////////////////////////
// COleObjectFactoryEx implementation

BEGIN_INTERFACE_MAP(COleObjectFactoryEx, COleObjectFactory)
	INTERFACE_PART(COleObjectFactoryEx, IID_IClassFactory, ClassFactory2)
	INTERFACE_PART(COleObjectFactoryEx, IID_IClassFactory2, ClassFactory2)
END_INTERFACE_MAP()


COleObjectFactoryEx::COleObjectFactoryEx(REFCLSID clsid,
	CRuntimeClass* pRuntimeClass, BOOL bMultiInstance, LPCTSTR lpszProgID) :
	COleObjectFactory(clsid, pRuntimeClass, bMultiInstance, lpszProgID)
{
	m_pModuleState = _afxModuleAddrCurrent;
	m_bLicenseChecked = FALSE;
	m_bLicenseValid = FALSE;
}


BOOL PASCAL COleObjectFactoryEx::UpdateRegistryAll(BOOL bRegister)
{
	//  NOTE: This is dangerous, but necessary to get at the factory's
	//  protected m_pNextFactory member.
#ifdef _WIN32
	CDynLinkLibrary* pDLL = AfxGetCoreState()->m_pFirstDLL;
	ASSERT(pDLL != NULL);
	COleObjectFactoryEx* pFactory = (COleObjectFactoryEx*) pDLL->m_pFirstSharedFactory;
#else
	COleObjectFactoryEx* pFactory = (COleObjectFactoryEx*) _afxFirstFactory;
#endif
	while (pFactory != NULL)
	{
		if (pFactory->IsKindOf(RUNTIME_CLASS(COleObjectFactoryEx)))
			if (!pFactory->UpdateRegistry(bRegister))
				return FALSE;

		pFactory = (COleObjectFactoryEx*)pFactory->GetNextFactory();
	}

	return TRUE;
}


BOOL COleObjectFactoryEx::IsLicenseValid()
{
	if (! m_bLicenseChecked)
	{
		m_bLicenseValid = (BYTE)VerifyUserLicense();
		m_bLicenseChecked = TRUE;
	}

	return m_bLicenseValid;
}


BOOL COleObjectFactoryEx::VerifyUserLicense()
{
	// May be overridden by subclass
	return TRUE;
}


BOOL COleObjectFactoryEx::GetLicenseKey(DWORD, BSTR FAR*)
{
	// May be overridden by subclass
	return FALSE;
}


BOOL COleObjectFactoryEx::VerifyLicenseKey(BSTR bstrKey)
{
	// May be overridden by subclass

	BOOL bLicensed = FALSE;
	BSTR bstr = NULL;

	if ((bstrKey != NULL) && GetLicenseKey(0, &bstr))
	{
		ASSERT(bstr != NULL);

		UINT cch = SysStringLen(bstr);
		if ((cch == SysStringLen(bstrKey)) &&
			(memcmp(bstr, bstrKey, cch) == 0))
		{
			bLicensed = TRUE;
		}

		SysFreeString(bstr);
	}

	return bLicensed;
}


STDMETHODIMP_(ULONG) COleObjectFactoryEx::XClassFactory2::AddRef()
{
	METHOD_MANAGE_STATE(COleObjectFactoryEx, ClassFactory2)
	ASSERT_VALID(pThis);
	return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) COleObjectFactoryEx::XClassFactory2::Release()
{
	METHOD_MANAGE_STATE(COleObjectFactoryEx, ClassFactory2)
	ASSERT_VALID(pThis);
	return (ULONG)pThis->ExternalRelease();
}


STDMETHODIMP COleObjectFactoryEx::XClassFactory2::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_MANAGE_STATE(COleObjectFactoryEx, ClassFactory2)
	ASSERT_VALID(pThis);
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP COleObjectFactoryEx::XClassFactory2::CreateInstance(
	IUnknown FAR* pUnkOuter, REFIID riid, LPVOID FAR* ppvObject)
{
	METHOD_MANAGE_STATE(COleObjectFactoryEx, ClassFactory2)
	ASSERT_VALID(pThis);

	if (! pThis->IsLicenseValid())
		return ResultFromScode(CLASS_E_NOTLICENSED);

	return pThis->m_xClassFactory.CreateInstance(pUnkOuter, riid, ppvObject);
}


STDMETHODIMP COleObjectFactoryEx::XClassFactory2::LockServer(BOOL fLock)
{
	METHOD_MANAGE_STATE(COleObjectFactoryEx, ClassFactory2)
	ASSERT_VALID(pThis);
	return pThis->m_xClassFactory.LockServer(fLock);
}


STDMETHODIMP COleObjectFactoryEx::XClassFactory2::GetLicInfo(
	LPLICINFO pLicInfo)
{
	METHOD_MANAGE_STATE(COleObjectFactoryEx, ClassFactory2)
	ASSERT_VALID(pThis);

	BSTR bstr = NULL;
	pLicInfo->fLicVerified = pThis->IsLicenseValid();
	pLicInfo->fRuntimeKeyAvail = pThis->GetLicenseKey(0, &bstr);
	if (bstr != NULL)
		SysFreeString(bstr);

	return NOERROR;
}


STDMETHODIMP COleObjectFactoryEx::XClassFactory2::RequestLicKey(
	DWORD dwReserved, BSTR FAR* pbstrKey)
{
	METHOD_MANAGE_STATE(COleObjectFactoryEx, ClassFactory2)
	ASSERT_VALID(pThis);

	ASSERT(pbstrKey != NULL);

	*pbstrKey = NULL;

	if (pThis->IsLicenseValid())
	{
		if (pThis->GetLicenseKey(dwReserved, pbstrKey))
			return NOERROR;
		else
			return ResultFromScode(E_FAIL);
	}
	else
		return ResultFromScode(CLASS_E_NOTLICENSED);
}


STDMETHODIMP COleObjectFactoryEx::XClassFactory2::CreateInstanceLic(
	LPUNKNOWN pUnkOuter, LPUNKNOWN, REFIID riid, BSTR bstrKey,
	LPVOID FAR* ppvObject)
{
	METHOD_MANAGE_STATE(COleObjectFactoryEx, ClassFactory2)
	ASSERT_VALID(pThis);

	if ((! pThis->VerifyLicenseKey(bstrKey)) && (! pThis->IsLicenseValid()))
		return ResultFromScode(CLASS_E_NOTLICENSED);

	return pThis->m_xClassFactory.CreateInstance(pUnkOuter, riid, ppvObject);
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

IMPLEMENT_DYNAMIC(COleObjectFactoryEx, COleObjectFactory)
