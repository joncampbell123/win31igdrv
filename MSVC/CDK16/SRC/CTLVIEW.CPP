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
// COleControl overridables for IViewObject implementation

BOOL COleControl::OnGetColorSet(DVTARGETDEVICE FAR*, HDC, LPLOGPALETTE FAR*)
{
	// Can be overridden by subclass
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// COleControl::XViewObject


STDMETHODIMP_(ULONG) COleControl::XViewObject::AddRef()
{
	//
	//  Delegate to our exported AddRef.
	//

	METHOD_MANAGE_STATE(COleControl, ViewObject)
	return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) COleControl::XViewObject::Release()
{
	//
	//  Delegate to our exported Release.
	//

	METHOD_MANAGE_STATE(COleControl, ViewObject)
	return (ULONG)pThis->ExternalRelease();
}


STDMETHODIMP COleControl::XViewObject::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	//
	//  Delegate to our exported QueryInterface.
	//

	METHOD_MANAGE_STATE(COleControl, ViewObject)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP COleControl::XViewObject::Draw(DWORD dwDrawAspect, LONG lindex,
					void FAR* pvAspect, DVTARGETDEVICE FAR * ptd,
					HDC hicTargetDev,
					HDC hdcDraw,
#if (_MFC_VER >= 0x0300)
					const RECTL FAR *lprcBounds,
					const RECTL FAR *lprcWBounds,
#else
					LPCRECTL lprcBounds,
					LPCRECTL lprcWBounds,
#endif
					BOOL (CALLBACK * pfnContinue)(DWORD),
					DWORD dwContinue)
{
	METHOD_MANAGE_STATE(COleControl, ViewObject)

	HRESULT hResult = NOERROR;

	AfxLockTempMaps();

	CRect rc((int)lprcBounds->left, (int)lprcBounds->top,
			 (int)lprcBounds->right, (int)lprcBounds->bottom);

	//  Convert from rectangle from logical to device coordinates,
	//  save DC state, and switch to MM_TEXT mode.  After drawing,
	//  restore DC state.

	switch (dwDrawAspect)
	{
	case DVASPECT_CONTENT:
		if (GetDeviceCaps(hdcDraw, TECHNOLOGY) == DT_METAFILE)
		{
			// If attributes DC is NULL, create one, based on ptd.
			HDC hAttribDC = hicTargetDev;
			if (hicTargetDev == NULL)
				hAttribDC = ::_AfxCtlCreateDC(ptd);

			// Draw into the metafile DC.
			CMetaFileDC dc;
			dc.Attach(hdcDraw);
			dc.SetAttribDC(hAttribDC);

			pThis->DrawMetafile(&dc, rc);

			dc.SetAttribDC(NULL);
			dc.Detach();

			// If we created an attributes DC, delete it now.
			if (hicTargetDev == NULL)
				::DeleteDC(hAttribDC);
		}
		else
		{
			CDC* pDC = CDC::FromHandle(hdcDraw);
			pThis->DrawContent(pDC, rc);
		}
		break;

	default:
		hResult = pThis->m_pDefIViewObject->Draw(
				dwDrawAspect, lindex, pvAspect, ptd, hicTargetDev, hdcDraw,
				lprcBounds, lprcWBounds, pfnContinue, dwContinue);
	}

	AfxUnlockTempMaps();
	return hResult;
}


STDMETHODIMP COleControl::XViewObject::GetColorSet(DWORD dwDrawAspect,
					LONG lindex, void FAR*, DVTARGETDEVICE FAR* ptd,
					HDC hicTargetDev, LPLOGPALETTE FAR* ppColorSet)
{
	METHOD_MANAGE_STATE(COleControl, ViewObject)

	HRESULT hr = ResultFromScode(E_FAIL);

	if ((dwDrawAspect == DVASPECT_CONTENT) && (lindex == -1) &&
		pThis->OnGetColorSet(ptd, hicTargetDev, ppColorSet))
	{
		hr = NOERROR;
	}

	return hr;
}


STDMETHODIMP COleControl::XViewObject::Freeze(DWORD, LONG, void FAR*,
					DWORD FAR*)
{
	return ResultFromScode(E_NOTIMPL);
}


STDMETHODIMP COleControl::XViewObject::Unfreeze(DWORD)
{
	return ResultFromScode(E_NOTIMPL);
}


STDMETHODIMP COleControl::XViewObject::SetAdvise(DWORD aspects, DWORD advf,
					LPADVISESINK pAdvSink)
{
	METHOD_MANAGE_STATE(COleControl, ViewObject)
	return pThis->m_pDefIViewObject->SetAdvise(aspects, advf, pAdvSink);
}


STDMETHODIMP COleControl::XViewObject::GetAdvise(DWORD FAR* pAspects, DWORD FAR* pAdvf,
					LPADVISESINK FAR* ppAdvSink)
{
	METHOD_MANAGE_STATE(COleControl, ViewObject)
	return pThis->m_pDefIViewObject->GetAdvise(pAspects, pAdvf, ppAdvSink);
}


STDMETHODIMP COleControl::XViewObject::GetExtent(DWORD dwDrawAspect, LONG,
								 DVTARGETDEVICE FAR*, LPSIZEL lpsizel)
{
	METHOD_MANAGE_STATE(COleControl, ViewObject)
	return pThis->m_xOleObject.GetExtent(dwDrawAspect, lpsizel);
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
