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
// COleControl::XOleObject


STDMETHODIMP_(ULONG) COleControl::XOleObject::AddRef()
{
	//
	//  Delegate to our exported AddRef.
	//

	METHOD_MANAGE_STATE(COleControl, OleObject)
	return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) COleControl::XOleObject::Release()
{
	//
	//  Delegate to our exported Release.
	//

	METHOD_MANAGE_STATE(COleControl, OleObject)
	return (ULONG)pThis->ExternalRelease();
}


STDMETHODIMP COleControl::XOleObject::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	//
	//  Delegate to our exported QueryInterface.
	//

	METHOD_MANAGE_STATE(COleControl, OleObject)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP COleControl::XOleObject::SetClientSite(LPOLECLIENTSITE pClientSite)
{
	METHOD_MANAGE_STATE(COleControl, OleObject)

	ASSERT_NULL_OR_POINTER(pClientSite, IOleClientSite);

	// maintain reference counts
	if (pClientSite != NULL)
		pClientSite->AddRef();
	RELEASE(pThis->m_pClientSite);
	pThis->m_pClientSite = pClientSite;

	LPVOID pInterface;

	// Initialize pointer to ambient property dispinterface.
	pThis->m_ambientDispDriver.ReleaseDispatch();
	if (pClientSite != NULL &&
		SUCCEEDED(pClientSite->QueryInterface(IID_IDispatch, &pInterface)))
	{
		ASSERT(pInterface != NULL);
		pThis->m_ambientDispDriver.AttachDispatch((LPDISPATCH) pInterface);
	}

	// Initialize pointer to in-place site.
	if (pClientSite == NULL ||
		FAILED(pClientSite->QueryInterface(IID_IOleInPlaceSite, &pInterface)))
	{
		pInterface = NULL;
	}

	RELEASE(pThis->m_pInPlaceSite);
	pThis->m_pInPlaceSite = (LPOLEINPLACESITE) pInterface;

	// Initialize pointer to control site
	if (pClientSite == NULL ||
		FAILED(pClientSite->QueryInterface(IID_IOleControlSite, &pInterface)))
	{
		pInterface = NULL;
	}

	RELEASE(pThis->m_pControlSite);
	pThis->m_pControlSite = (LPOLECONTROLSITE) pInterface;

	// Initialize pointer to simple frame site
	if (pClientSite == NULL || !pThis->m_bSimpleFrame ||
		FAILED(pClientSite->QueryInterface(IID_ISimpleFrameSite, &pInterface)))
	{
		pInterface = NULL;
	}

	RELEASE(pThis->m_pSimpleFrameSite);
	pThis->m_pSimpleFrameSite = (LPSIMPLEFRAMESITE) pInterface;

#ifndef _WIN32
	// Load language specific DLL
	if (pClientSite != NULL && !_AfxGetAppData()->bLangDLLInit)
	{
		LCID lcid = pThis->AmbientLocaleID();

		_AfxSetResourceLocale(lcid);
	}
#endif

	// Let the control run its own code here.
	pThis->OnSetClientSite();

	pThis->m_bUIDead = (BYTE)(pThis->AmbientUIDead());

	// Unless IPersist*::Load or IPersist*::InitNew is called after this,
	// we can't count on ambient properties being available while loading.
	pThis->m_bCountOnAmbients = FALSE;

	return NOERROR;
}


STDMETHODIMP COleControl::XOleObject::GetClientSite(LPOLECLIENTSITE FAR* ppClientSite)
{
	METHOD_MANAGE_STATE(COleControl, OleObject)

	ASSERT_POINTER(ppClientSite, LPOLECLIENTSITE);

	LPOLECLIENTSITE pClientSite = pThis->m_pClientSite;
	*ppClientSite = pClientSite;
	if (pClientSite != NULL)
		pClientSite->AddRef();

	return (pClientSite != NULL) ? NOERROR : ResultFromScode(E_FAIL);
}


STDMETHODIMP COleControl::XOleObject::SetHostNames(LPCTSTR szContainerApp,
	LPCTSTR szContainerObj)
{
	UNUSED szContainerApp;
	UNUSED szContainerObj;
	METHOD_MANAGE_STATE(COleControl, OleObject)
	return NOERROR;
}


STDMETHODIMP COleControl::XOleObject::Close(DWORD dwSaveOption)
{
	METHOD_MANAGE_STATE(COleControl, OleObject)

	if (pThis->m_bInPlaceActive)
		pThis->m_xOleInPlaceObject.InPlaceDeactivate();

	if (((dwSaveOption == OLECLOSE_SAVEIFDIRTY) || (dwSaveOption == OLECLOSE_PROMPTSAVE)) &&
		pThis->m_bModified)
	{
		pThis->SendAdvise(OBJECTCODE_SAVEOBJECT);
		pThis->SendAdvise(OBJECTCODE_SAVED);
	}

	return NOERROR;
}


STDMETHODIMP COleControl::XOleObject::SetMoniker(DWORD, LPMONIKER)
{
	METHOD_MANAGE_STATE(COleControl, OleObject)
	return ResultFromScode(E_NOTIMPL);
}


STDMETHODIMP COleControl::XOleObject::GetMoniker(DWORD, DWORD, LPMONIKER FAR*)
{
	METHOD_MANAGE_STATE(COleControl, OleObject)
	return ResultFromScode(E_NOTIMPL);
}


STDMETHODIMP COleControl::XOleObject::InitFromData(LPDATAOBJECT, BOOL, DWORD)
{
	METHOD_MANAGE_STATE(COleControl, OleObject)
	return ResultFromScode(E_NOTIMPL);
}


STDMETHODIMP COleControl::XOleObject::GetClipboardData(DWORD, LPDATAOBJECT FAR*)
{
	METHOD_MANAGE_STATE(COleControl, OleObject)
	return ResultFromScode(E_NOTIMPL);
}


STDMETHODIMP COleControl::XOleObject::DoVerb(LONG iVerb,
				LPMSG lpmsg,
				LPOLECLIENTSITE pActiveSite,
				LONG lindex,
				HWND hwndParent,
				LPCRECT lprcPosRect)
{
	METHOD_MANAGE_STATE(COleControl, OleObject)

	TRY
	{
		if (pThis->OnDoVerb(iVerb, lpmsg, hwndParent, lprcPosRect))
			return NOERROR;                 // Custom verb succeeded
	}
	CATCH (CException, e)
	{
		return ResultFromScode(E_FAIL);     // Custom verb failed
	}
	END_CATCH

	// Custom verb not found, invoke standard verb instead.

	HRESULT hr;

	switch (iVerb)
	{
	case OLEIVERB_HIDE:
		pThis->m_xOleInPlaceObject.UIDeactivate();
		return pThis->OnHide();


	case OLEIVERB_SHOW:
		return pThis->OnOpen(TRUE, lpmsg);      // Try in-place first

	case OLEIVERB_UIACTIVATE:
	case OLEIVERB_INPLACEACTIVATE:
		if (pThis->m_bOpen)
			return ResultFromScode(OLE_E_NOT_INPLACEACTIVE);    // Already open

		// FALL THRU

	case OLEIVERB_PRIMARY:
		CopyRect(&pThis->m_rcPos, lprcPosRect);
		return pThis->OnActivateInPlace((iVerb != OLEIVERB_INPLACEACTIVATE), lpmsg);

	case OLEIVERB_OPEN:
		return pThis->OnOpen(FALSE, lpmsg);     //  Don't try in-place

	case OLEIVERB_PROPERTIES:
		return pThis->OnProperties(lpmsg, hwndParent, lprcPosRect) ?
				NOERROR :
				ResultFromScode(E_FAIL);

	default:
		// negative verbs not understood should return E_NOTIMPL
		if (iVerb < 0)
			return ResultFromScode(E_NOTIMPL);

		// positive verb not processed --
		//  according to OLE 2.0 spec, primary verb should be executed
		//  instead.
		if (SUCCEEDED(hr = DoVerb(OLEIVERB_PRIMARY, lpmsg, pActiveSite,
								lindex, hwndParent, lprcPosRect)))
			return ResultFromScode(OLEOBJ_S_INVALIDVERB);
		else
			return hr;
	}
}


STDMETHODIMP COleControl::XOleObject::EnumVerbs(LPENUMOLEVERB FAR* ppenumOleVerb)
{
	METHOD_MANAGE_STATE(COleControl, OleObject)
	return pThis->OnEnumVerbs(ppenumOleVerb) ?
		NOERROR :
		ResultFromScode(OLEOBJ_E_NOVERBS);
}


STDMETHODIMP COleControl::XOleObject::Update()
{
	METHOD_MANAGE_STATE(COleControl, OleObject)
	return NOERROR;
}


STDMETHODIMP COleControl::XOleObject::IsUpToDate()
{
	METHOD_MANAGE_STATE(COleControl, OleObject)
	return NOERROR;
}


STDMETHODIMP COleControl::XOleObject::GetUserClassID(CLSID FAR* pClsid)
{
	METHOD_MANAGE_STATE(COleControl, OleObject)
	return pThis->GetClassID(pClsid);
}


STDMETHODIMP COleControl::XOleObject::GetUserType(DWORD dwFormOfType,
	LPTSTR FAR* ppszUserType)
{
	UNUSED dwFormOfType;
	METHOD_MANAGE_STATE(COleControl, OleObject)
	ASSERT_POINTER(ppszUserType, LPTSTR);
	*ppszUserType = _AfxCtlCopyString((LPTSTR)(LPCTSTR)pThis->GetUserType(),
		NULL);
	return NOERROR;
}


CString& COleControl::GetUserType()
{
	if (m_strUserTypeName.IsEmpty())
	{
		TRY
		{
			m_strUserTypeName.LoadString(GetUserTypeNameID());
		}
		END_TRY
	}
	return m_strUserTypeName;
}


STDMETHODIMP COleControl::XOleObject::SetExtent(DWORD dwDrawAspect, LPSIZEL lpsizel)
{
	METHOD_MANAGE_STATE(COleControl, OleObject)

	if (dwDrawAspect & DVASPECT_CONTENT)
		return pThis->OnSetExtent(lpsizel) ? NOERROR : ResultFromScode(E_FAIL);
	else
		return ResultFromScode(E_NOTIMPL);
}


BOOL COleControl::OnSetExtent(LPSIZEL lpSizeL)
{
	if (m_bChangingExtent)  // Prevent infinite recursion!
		return FALSE;

	m_bChangingExtent = TRUE;

	// Update the control's extent.
	m_cxExtent = lpSizeL->cx;
	m_cyExtent = lpSizeL->cy;

	// Mark the control dirty and force a repaint.
	SetModifiedFlag();
	InvalidateControl();

	SIZEL szlPixels;
	_XformSizeInHimetricToPixels(NULL, lpSizeL, &szlPixels);

	CRect rectNew(m_rcPos);
	rectNew.right = rectNew.left + (int)szlPixels.cx;
	rectNew.bottom = rectNew.top + (int)szlPixels.cy;

	if ((m_pInPlaceSite != NULL) && m_bInPlaceActive)
	{
		// If the control is in-place active, tell the container to resize.
		m_pInPlaceSite->OnPosRectChange(&rectNew);
	}
	else if (m_bOpen)
	{
		// If the control is open, resize it.
		ResizeOpenControl((int)szlPixels.cx, (int)szlPixels.cy);
	}
	else
	{
		CopyRect(m_rcPos, rectNew);

		// Resize off-screen window, if any.
		if (m_hWnd != NULL)
			SetWindowPos(NULL, 0, 0, (int)szlPixels.cx, (int)szlPixels.cy,
				SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
	}

	m_bChangingExtent = FALSE;
	return TRUE;
}


STDMETHODIMP COleControl::XOleObject::GetExtent(DWORD dwDrawAspect, LPSIZEL lpsizel)
{
	METHOD_MANAGE_STATE(COleControl, OleObject)

	if (dwDrawAspect & DVASPECT_CONTENT)
	{
		lpsizel->cx = pThis->m_cxExtent;
		lpsizel->cy = pThis->m_cyExtent;
		return NOERROR;
	}

	return ResultFromScode(E_NOTIMPL);
}


STDMETHODIMP COleControl::XOleObject::Advise(LPADVISESINK pAdvSink, DWORD FAR* pdwConnection)
{
	//
	//  If no advise holder exists, create one.
	//  Then delegate this call to the advise holder.
	//

	METHOD_MANAGE_STATE(COleControl, OleObject)

	if (pThis->m_pOleAdviseHolder == NULL)
	{
		HRESULT hr;
		if (FAILED(hr = CreateOleAdviseHolder(&pThis->m_pOleAdviseHolder)))
			return hr;
	}

	return pThis->m_pOleAdviseHolder->Advise(pAdvSink, pdwConnection);
}


STDMETHODIMP COleControl::XOleObject::Unadvise(DWORD dwConnection)
{
	METHOD_MANAGE_STATE(COleControl, OleObject)

	if (pThis->m_pOleAdviseHolder != NULL)
		return pThis->m_pOleAdviseHolder->Unadvise(dwConnection);

	return ResultFromScode(E_FAIL);
}


STDMETHODIMP COleControl::XOleObject::EnumAdvise(LPENUMSTATDATA FAR* ppenumAdvise)
{
	METHOD_MANAGE_STATE(COleControl, OleObject)

	if (pThis->m_pOleAdviseHolder != NULL)
		return pThis->m_pOleAdviseHolder->EnumAdvise(ppenumAdvise);

	return ResultFromScode(E_FAIL);
}


STDMETHODIMP COleControl::XOleObject::GetMiscStatus(DWORD dwAspect, DWORD FAR* pdwStatus)
{
	METHOD_MANAGE_STATE(COleControl, OleObject)
	ASSERT_POINTER(pdwStatus, DWORD);

	if (dwAspect == DVASPECT_CONTENT)
		*pdwStatus = pThis->GetMiscStatus();
	else
		*pdwStatus = 0;

	return NOERROR;
}


STDMETHODIMP COleControl::XOleObject::SetColorScheme(LPLOGPALETTE lpLogpal)
{
	UNUSED lpLogpal;
	METHOD_MANAGE_STATE(COleControl, OleObject)
	return ResultFromScode(E_NOTIMPL);
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
