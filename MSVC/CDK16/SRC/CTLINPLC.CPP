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
// COleControl::XOleInPlaceObject


STDMETHODIMP_(ULONG) COleControl::XOleInPlaceObject::AddRef()
{
	// Delegate to our exported AddRef.
	METHOD_MANAGE_STATE(COleControl, OleInPlaceObject)
	return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) COleControl::XOleInPlaceObject::Release()
{
	// Delegate to our exported Release.
	METHOD_MANAGE_STATE(COleControl, OleInPlaceObject)
	return (ULONG)pThis->ExternalRelease();
}


STDMETHODIMP COleControl::XOleInPlaceObject::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	// Delegate to our exported QueryInterface.
	METHOD_MANAGE_STATE(COleControl, OleInPlaceObject)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP COleControl::XOleInPlaceObject::GetWindow(HWND FAR* lphwnd)
{
	METHOD_MANAGE_STATE(COleControl, OleInPlaceObject)

	if (pThis->m_bInPlaceActive)
	{
		*lphwnd = pThis->GetOuterWindow()->m_hWnd;
		return NOERROR;
	}
	else
	{
		*lphwnd = NULL;
		return ResultFromScode(E_FAIL);
	}
}


STDMETHODIMP COleControl::XOleInPlaceObject::ContextSensitiveHelp(BOOL fEnterMode)
{
	UNUSED fEnterMode;
	METHOD_MANAGE_STATE(COleControl, OleInPlaceObject)
	return ResultFromScode(E_NOTIMPL);
}


STDMETHODIMP COleControl::XOleInPlaceObject::InPlaceDeactivate()
{
	METHOD_MANAGE_STATE(COleControl, OleInPlaceObject)

	if (! pThis->m_bInPlaceActive)
		return NOERROR;

	pThis->m_bInPlaceActive = FALSE;

	if (pThis->m_bUIActive)
		UIDeactivate();

	// hide the window
	pThis->OnHide();
	pThis->m_pInPlaceSite->OnInPlaceDeactivate();

	return NOERROR;
}


STDMETHODIMP COleControl::XOleInPlaceObject::UIDeactivate()
{
	METHOD_MANAGE_STATE(COleControl, OleInPlaceObject)

	if (! pThis->m_bUIActive)
		return NOERROR;

	pThis->m_bUIActive = FALSE;

	if (pThis->m_pInPlaceDoc != NULL)
		pThis->m_pInPlaceDoc->SetActiveObject(NULL, NULL);
	pThis->m_pInPlaceFrame->SetActiveObject(NULL, NULL);
	pThis->RemoveFrameLevelUI();
	pThis->DestroySharedMenu();

	pThis->m_pInPlaceSite->OnUIDeactivate(FALSE);

	return NOERROR;
}


STDMETHODIMP COleControl::XOleInPlaceObject::SetObjectRects(LPCRECT lprcPosRect,
					LPCRECT lprcClipRect)
{
	METHOD_MANAGE_STATE(COleControl, OleInPlaceObject)

	return pThis->OnSetObjectRects(lprcPosRect, lprcClipRect) ?
			NOERROR :
			ResultFromScode(E_FAIL);
}


BOOL COleControl::OnSetObjectRects(LPCRECT lprcPosRect, LPCRECT lprcClipRect)
{
	UNUSED lprcClipRect;    // Note: Clip rectangle is ignored.

	CopyRect(&m_rcPos, lprcPosRect);
	CWnd* pWndOuter = GetOuterWindow();
	if ((pWndOuter != NULL) && (pWndOuter->m_hWnd != NULL))
		pWndOuter->MoveWindow(m_rcPos);

	return TRUE;
}


STDMETHODIMP COleControl::XOleInPlaceObject::ReactivateAndUndo()
{
	METHOD_MANAGE_STATE(COleControl, OleInPlaceObject)
	return ResultFromScode(E_NOTIMPL);
}


/////////////////////////////////////////////////////////////////////////////
// COleControl::XOleInPlaceActiveObject


STDMETHODIMP_(ULONG) COleControl::XOleInPlaceActiveObject::AddRef()
{
	// Delegate to our exported AddRef.
	METHOD_MANAGE_STATE(COleControl, OleInPlaceActiveObject)
	return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) COleControl::XOleInPlaceActiveObject::Release()
{
	// Delegate to our exported Release.
	METHOD_MANAGE_STATE(COleControl, OleInPlaceActiveObject)
	return (ULONG)pThis->ExternalRelease();
}


STDMETHODIMP COleControl::XOleInPlaceActiveObject::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	// Delegate to our exported QueryInterface.
	METHOD_MANAGE_STATE(COleControl, OleInPlaceActiveObject)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP COleControl::XOleInPlaceActiveObject::GetWindow(HWND FAR* lphwnd)
{
	METHOD_MANAGE_STATE(COleControl, OleInPlaceActiveObject)
	return pThis->m_xOleInPlaceObject.GetWindow(lphwnd);
}


STDMETHODIMP COleControl::XOleInPlaceActiveObject::ContextSensitiveHelp(BOOL fEnterMode)
{
	UNUSED fEnterMode;
	METHOD_MANAGE_STATE(COleControl, OleInPlaceActiveObject)
	return ResultFromScode(E_NOTIMPL);
}


STDMETHODIMP COleControl::XOleInPlaceActiveObject::TranslateAccelerator(
	LPMSG lpmsg)
{
	METHOD_MANAGE_STATE(COleControl, OleInPlaceActiveObject)

	HRESULT hr;

	// Prevent this function from being called recursively.
	if (pThis->m_bTranslatingAccelerator)
		return ResultFromScode(S_FALSE);

	// Give the control the first chance.
	if (pThis->PreTranslateMessage(lpmsg))
		return NOERROR;

	// Give the site a chance.
	if (pThis->m_pControlSite != NULL)
	{
		hr = pThis->m_pControlSite->TranslateAccelerator(lpmsg, 0);
		if (hr == NOERROR)
			return NOERROR;
	}

	// Give the containing frame the last chance.
	OLEINPLACEFRAMEINFO frameInfo = pThis->m_frameInfo;
	pThis->m_bTranslatingAccelerator = TRUE;
	hr = ::OleTranslateAccelerator(pThis->m_pInPlaceFrame,
		&frameInfo, lpmsg);
	pThis->m_bTranslatingAccelerator = FALSE;

	return hr;
}


STDMETHODIMP COleControl::XOleInPlaceActiveObject::OnFrameWindowActivate(
	BOOL fActivate)
{
	UNUSED fActivate;

	METHOD_MANAGE_STATE(COleControl, OleInPlaceActiveObject)

	// This is where we would do border shading if we were a normal OLE object.
	// But controls don't shade their borders, so we don't need to do anything.

	return NOERROR;
}


STDMETHODIMP COleControl::XOleInPlaceActiveObject::OnDocWindowActivate(
	BOOL fActivate)
{
	METHOD_MANAGE_STATE(COleControl, OleInPlaceActiveObject)

	if (fActivate)
		pThis->AddFrameLevelUI();
	else
		pThis->OnHideToolBars();

	return NOERROR;
}


STDMETHODIMP COleControl::XOleInPlaceActiveObject::ResizeBorder(
	LPCRECT lprectBorder, LPOLEINPLACEUIWINDOW lpUIWindow, BOOL fFrameWindow)
{
	UNUSED lprectBorder;
	UNUSED lpUIWindow;
	UNUSED fFrameWindow;

	METHOD_MANAGE_STATE(COleControl, OleInPlaceActiveObject)
	return NOERROR;
}


STDMETHODIMP COleControl::XOleInPlaceActiveObject::EnableModeless(BOOL fEnable)
{
	UNUSED fEnable;

	METHOD_MANAGE_STATE(COleControl, OleInPlaceActiveObject)
	return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
