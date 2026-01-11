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

#ifdef AFXCTL_CORE1_SEG
#pragma code_seg(AFXCTL_CORE1_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

#ifndef _WIN32
#define AfxFreeTaskMem  _AfxFreeTaskMem
#define AfxMergeMenus   _AfxMergeMenus
#define AfxUnmergeMenus _AfxUnmergeMenus
#define AfxCancelModes  _AfxCancelModes
#define AfxLoadString   _AfxLoadString
#endif

#ifdef _WIN32
#define _afxTrackingMenu (AfxGetThreadState()->m_hTrackingMenu)
#else
extern HMENU _afxTrackingMenu;
#endif

// Special WM_PAINT message handler that includes the HDC
#ifdef _WIN32
#define ON_WM_PAINT_SPECIAL() \
	{ WM_PAINT, 0, 0, 0, AfxSig_vD, \
		(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(CDC*))OnPaint },
#else
#define ON_WM_PAINT_SPECIAL() \
	{ WM_PAINT, 0, AfxSig_vD, \
		(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(CDC*))OnPaint },
#endif


// Window to serve as "parking space" for not-yet-activated subclassed controls
extern CParkingWnd* _pWndParking;

static CParkingWnd* GetParkingWindow()
{
	if (_pWndParking == NULL)
	{
		TRY
		{
			_pWndParking = new CParkingWnd;
		}
		CATCH (CException, e)
		{
			if (_pWndParking)
				delete _pWndParking;
			_pWndParking = NULL;
		}
		END_CATCH
	}

	return _pWndParking;
}


/////////////////////////////////////////////////////////////////////////////
//
//  COleControl interface map
//
/////////////////////////////////////////////////////////////////////////////

BEGIN_INTERFACE_MAP(COleControl, CCmdTarget)
	INTERFACE_PART(COleControl, IID_IPersist, PersistStorage)
	INTERFACE_PART(COleControl, IID_IPersistStorage, PersistStorage)
	INTERFACE_PART(COleControl, IID_IPersistStreamInit, PersistStreamInit)
	INTERFACE_PART(COleControl, IID_IOleObject, OleObject)
	INTERFACE_PART(COleControl, IID_IViewObject, ViewObject)
	INTERFACE_PART(COleControl, IID_IViewObject2, ViewObject)
	INTERFACE_PART(COleControl, IID_IDataObject, DataObject)
	INTERFACE_PART(COleControl, IID_IOleInPlaceObject, OleInPlaceObject)
	INTERFACE_PART(COleControl, IID_IOleInPlaceActiveObject, OleInPlaceActiveObject)
	INTERFACE_PART(COleControl, IID_IDispatch, Dispatch)
	INTERFACE_PART(COleControl, IID_IOleCache, OleCache)
	INTERFACE_PART(COleControl, IID_IOleControl, OleControl)
	INTERFACE_PART(COleControl, IID_IProvideClassInfo, ProvideClassInfo)
	INTERFACE_PART(COleControl, IID_ISpecifyPropertyPages, SpecifyPropertyPages)
	INTERFACE_PART(COleControl, IID_IPerPropertyBrowsing, PerPropertyBrowsing)
	INTERFACE_PART(COleControl, IID_IConnectionPointContainer, ConnPtContainer)
END_INTERFACE_MAP()


/////////////////////////////////////////////////////////////////////////////
//
//  COleControl event map
//
/////////////////////////////////////////////////////////////////////////////

AFX_EVENTMAP BASED_CODE COleControl::eventMap = { NULL, NULL };


/////////////////////////////////////////////////////////////////////////////
//
//  COleControl message map
//
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(COleControl, CWnd)
	ON_WM_PAINT_SPECIAL()
	//{{AFX_MSG_MAP(COleControl)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CHAR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_INITMENUPOPUP()
	ON_WM_MENUSELECT()
	ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString)
	ON_WM_ENTERIDLE()
	ON_WM_CANCELMODE()
	ON_WM_SYSKEYDOWN()
	ON_WM_SYSKEYUP()
	ON_WM_MOUSEACTIVATE()
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_WM_NCCREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
#ifdef _WIN32
	ON_MESSAGE(OCM_CTLCOLORBTN, OnOcmCtlColorBtn)
	ON_MESSAGE(OCM_CTLCOLORDLG, OnOcmCtlColorDlg)
	ON_MESSAGE(OCM_CTLCOLOREDIT, OnOcmCtlColorEdit)
	ON_MESSAGE(OCM_CTLCOLORLISTBOX, OnOcmCtlColorListBox)
	ON_MESSAGE(OCM_CTLCOLORMSGBOX, OnOcmCtlColorMsgBox)
	ON_MESSAGE(OCM_CTLCOLORSCROLLBAR, OnOcmCtlColorScrollBar)
	ON_MESSAGE(OCM_CTLCOLORSTATIC, OnOcmCtlColorStatic)
#else
	ON_MESSAGE(OCM_CTLCOLOR, OnOcmCtlColor)
#endif
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
//
// COleControl implementation
//
/////////////////////////////////////////////////////////////////////////////

COleControl::COleControl() :
	m_font(&m_xFontNotification)
{
	m_pModuleState = _afxModuleAddrCurrent;

	m_pTypeLibCache = NULL;

	m_bFinalReleaseCalled = FALSE;
	m_bChangingExtent = FALSE;
	m_bTranslatingAccelerator = FALSE;
	m_bUIDead = FALSE;

	m_pReflect = NULL;

	m_pClientSite = NULL;
	m_pOleAdviseHolder = NULL;
	m_pDataAdviseHolder = NULL;
	m_pInPlaceSite = NULL;
	m_pInPlaceFrame = NULL;
	m_pInPlaceDoc = NULL;
	m_pControlSite = NULL;

	m_pDefIPersistStorage = NULL;
	m_pDefIViewObject = NULL;
	m_pDefIOleCache = NULL;

	m_hSharedMenu = NULL;
	m_hOleMenu = NULL;
	m_nIDTracking = 0;
	m_nIDLastMessage = 0;
	m_bAutoMenuEnable = TRUE;       // auto enable on by default

	m_bInPlaceActive = FALSE;
	m_bUIActive = FALSE;
	m_bOpen = FALSE;
	m_bPendingUIActivation = FALSE;
	m_pWndOpenFrame = NULL;

	m_dwVersionLoaded = 0;

	m_bModified = TRUE;

	m_iButtonState = 0;
	m_iDblClkState = 0;

	m_dwStockEventMask = 0;
	m_dwStockPropMask = 0;

	m_sBorderStyle = 0;
	m_bEnabled = TRUE;

	m_hFontPrev = NULL;

	m_cEventsFrozen = 0;

	m_bConvertVBX = FALSE;

	m_pSimpleFrameSite = NULL;
	m_bSimpleFrame = FALSE;

	SetInitialSize(100, 50);

	//
	//  Wire up aggregation and IDispatch support
	//
	EnableAggregation();
	EnableAutomation();

	SetInitialDataFormats();

	AfxOleLockApp();
}


COleControl::~COleControl()
{
	if (m_pReflect != NULL)
		m_pReflect->DestroyWindow();

	if (m_pWndOpenFrame != NULL)
		m_pWndOpenFrame->DestroyWindow();

	RELEASE(m_pClientSite);
	RELEASE(m_pOleAdviseHolder);
	RELEASE(m_pDataAdviseHolder);
	RELEASE(m_pInPlaceSite);
	RELEASE(m_pInPlaceFrame);
	RELEASE(m_pInPlaceDoc);
	RELEASE(m_pControlSite);
	RELEASE(m_pSimpleFrameSite);

#ifdef OLE2ANSI
	LPUNKNOWN pUnkControlling = m_pUnkWrapper;
#else
	LPUNKNOWN pUnkControlling = (LPUNKNOWN)m_pOuterUnknown;

	if (pUnkControlling == NULL)
		pUnkControlling = (LPUNKNOWN)&m_xInnerUnknown;

	m_dwRef++;  // Keep ref count from going to zero again.
#endif

	pUnkControlling->AddRef();
	RELEASE(m_pDefIPersistStorage);

	pUnkControlling->AddRef();
	RELEASE(m_pDefIViewObject);

	pUnkControlling->AddRef();
	RELEASE(m_pDefIOleCache);

	RELEASE(m_pDefIUnknown);

#ifdef OLE2ANSI
	RELEASE(m_pUnkWrapper);
#else
	m_dwRef--;
#endif

	AfxOleUnlockApp();
}


void COleControl::OnFinalRelease()
{
	if (! m_bFinalReleaseCalled)
	{
		m_bFinalReleaseCalled = TRUE;

		if (m_hWnd != NULL)
			DestroyWindow();

		GetTypeLibCache()->Unlock();
		delete this;
	}
}


LPUNKNOWN COleControl::GetInterfaceHook(const void* piid)
{
	ASSERT_POINTER(piid, IID);

	if ((m_piidPrimary != NULL) &&
		IsEqualIID(*m_piidPrimary, *(IID*)piid))
	{
		return GetInterface((void*)&IID_IDispatch);
	}

	return NULL;
}


void COleControl::SetInitialSize(int cx, int cy)
{
	SIZEL szlPixels;
	SIZEL szlHimetric;
	szlPixels.cx = cx;
	szlPixels.cy = cy;
	_XformSizeInPixelsToHimetric(NULL, &szlPixels, &szlHimetric);
	m_cxExtent = szlHimetric.cx;
	m_cyExtent = szlHimetric.cy;
}


void COleControl::InitializeIIDs(const IID* piidPrimary, const IID* piidEvents)
{
	m_piidPrimary = piidPrimary;
	m_piidEvents = piidEvents;

	//  This function performs initialization that must occur after
	//  construction of the most-derived class has happened.
	GetTypeLibCache()->Lock();

	//  Initialize the masks for stock events and properties.
	InitStockEventMask();
	InitStockPropMask();

#ifdef _DEBUG

	// Verify that the type library contains all the correct information.
	// If any of the following assertions fail, carefully check the IDs
	// in the control's .CPP file against those in its .ODL file.

	LPTYPEINFO pTypeInfo;
	HRESULT hr;
	CLSID clsid;

	GetClassID(&clsid);
	if (SUCCEEDED(hr = GetTypeInfoOfGuid(0, clsid, &pTypeInfo)))
		RELEASE(pTypeInfo);

	ASSERT(SUCCEEDED(hr));  // Class ID may be corrupted

	if (SUCCEEDED(hr = GetTypeInfoOfGuid(0, *m_piidPrimary, &pTypeInfo)))
		RELEASE(pTypeInfo);

	ASSERT(SUCCEEDED(hr));  // Primary dispatch interface ID may be corrupted

	if (SUCCEEDED(hr = GetTypeInfoOfGuid(0, *m_piidEvents, &pTypeInfo)))
		RELEASE(pTypeInfo);

	ASSERT(SUCCEEDED(hr));  // Event dispatch interface ID may be corrupted

#endif
}


#ifdef _DEBUG

void COleControl::AssertValid() const
{
	CWnd::AssertValid();
}

void _AfxDumpGuid(CDumpContext& dc, const GUID* pGuid)
{
	if (pGuid == NULL)
	{
		dc << "(NULL)";
		return;
	}

	TCHAR szGuid[40];
	::StringFromGUID2(*pGuid, szGuid, 40);
	dc << szGuid;
}

void _AfxDumpHex(CDumpContext& dc, DWORD dw)
{
	TCHAR szHex[10];
	wsprintf(szHex, _T("0x%08lx"), dw);
	dc << szHex;
}

void COleControl::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
	dc << "\nm_pModuleState = " << m_pModuleState;
	dc << "\nm_piidPrimary = ";
	_AfxDumpGuid(dc, m_piidPrimary);
	dc << "\nm_piidEvents =  ";
	_AfxDumpGuid(dc, m_piidEvents);
	dc << "\nm_dwVersionLoaded = ";
	_AfxDumpHex(dc, m_dwVersionLoaded);
	dc << "\nm_cEventsFrozen = " << m_cEventsFrozen;
	dc << "\nm_rcPos = " << m_rcPos;
	dc << "\nm_cxExtent = " << m_cxExtent;
	dc << "\nm_cyExtent = " << m_cyExtent;
	dc << "\nm_bFinalReleaseCalled = " << m_bFinalReleaseCalled;
	dc << "\nm_bModified = " << m_bModified;
	dc << "\nm_bCountOnAmbients = " << m_bCountOnAmbients;
	dc << "\nm_iButtonState = " << m_iButtonState;
	dc << "\nm_iDblClkState = " << m_iDblClkState;
	dc << "\nm_bInPlaceActive = " << m_bInPlaceActive;
	dc << "\nm_bUIActive = " << m_bUIActive;
	dc << "\nm_bPendingUIActivation = " << m_bPendingUIActivation;
	dc << "\nm_bOpen = " << m_bOpen;
	dc << "\nm_bChangingExtent = " << m_bChangingExtent;
	dc << "\nm_bConvertVBX = " << m_bConvertVBX;
	dc << "\nm_bSimpleFrame = " << m_bSimpleFrame;
	dc << "\nm_bTranslatingAccelerator = " << m_bTranslatingAccelerator;
	dc << "\nm_bUIDead = " << m_bUIDead;
	dc << "\nm_strUserTypeName = " << m_strUserTypeName;
}

#endif // _DEBUG


/////////////////////////////////////////////////////////////////////////////
// _AfxFillPSOnStack
//
// Windows has a bug in the WM_PAINT code for the Button control.  If the
// paint is a sub-classed paint, and if the display driver is a Win3.0
// display driver, then Windows calls IsRectEmpty, passing in the rectangle
// from the paint structure.  Since it was a sub-classed paint, and
// BeginPaint was not called, the rectangle struct passed to IsRectEmpty is
// uninitialized.  If IsRectEmpty returned True, then the control was not
// painted.  To work around the bug, we call FillPSOnStack before calling
// windows with the WM_PAINT.  This routine fills a buffer on the stack
// 0x80 bytes of consecutive values from 0 to 0x7f.  That way, if the
// rectangle falls anywhere within this buffer range, and the stack has not
// been modified by other means, then IsRectEmpty will return FALSE, so that
// the control will Paint.

void _AfxFillPSOnStack()
{
#ifndef _WIN32
#define WORDBUFSIZE 0x40
	WORD buf[WORDBUFSIZE];
	WORD i;
	WORD pat;

	for (i = 0, pat = 0x0100; i < WORDBUFSIZE; i++, pat += 0x0202)
		buf[i] = pat;
#endif
}


void COleControl::DoSuperclassPaint(CDC* pDC, const CRect& rcBounds)
{
	if (m_hWnd != NULL)
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		pDC->SetMapMode(MM_ANISOTROPIC);
		pDC->SetWindowOrg(0, 0);
		pDC->SetWindowExt(rcClient.Size());
		pDC->SetViewportOrg(rcBounds.left, rcBounds.top);
		pDC->SetViewportExt(rcBounds.Size());

		_AfxFillPSOnStack();
		::CallWindowProc(
				*GetSuperWndProcAddr(),
				m_hWnd, WM_PAINT, (WPARAM)(pDC->m_hDC), 0);
	}
}


BOOL COleControl::ContainerReflectsMessages()
{
	BOOL bReflect;

	if (!GetAmbientProperty(DISPID_AMBIENT_MESSAGEREFLECT, VT_BOOL, &bReflect))
		bReflect = FALSE;
	return bReflect;
}


BOOL COleControl::CreateControlWindow(HWND hWndParent, const CRect& rcPos)
{
	if (m_hWnd == NULL)
	{
		// If window doesn't exist, create it.

		// Test if:
		//  we're not subclassing a Windows control, or
		//  container reflects messages for us...

		DWORD dwStyle = WS_CHILD | WS_CLIPSIBLINGS;
		if (m_sBorderStyle)
			dwStyle |= WS_BORDER;
		if (!m_bEnabled)
			dwStyle |= WS_DISABLED;

		if ((GetSuperWndProcAddr() == CWnd::GetSuperWndProcAddr()) ||
			ContainerReflectsMessages())
		{
			// Just create the control's window.
			CreateEx(0, NULL, m_strText, dwStyle,
					rcPos.left, rcPos.top,
					rcPos.right - rcPos.left, rcPos.bottom - rcPos.top,
					hWndParent, 0);

			m_pReflect = NULL;

			if (m_hWnd != NULL)
			{
				ShowWindow(SW_SHOWNORMAL);
			}
		}
		else    // ...we're subclassing a Windows control.
		{
			if (m_pReflect == NULL)
			{
				// Create a window to reflect notification messages.
				m_pReflect = new CReflectorWnd;

				if (!m_pReflect->Create(rcPos, hWndParent))
				{
					// If m_pReflect->Create failed, then m_pReflect deleted itself.
					m_pReflect = NULL;
				}
			}
			else
			{
				// Reflector window already exists... just reparent it.
				if (m_pReflect->m_hWnd != NULL)
					::SetParent(m_pReflect->m_hWnd, hWndParent);
			}

			if (m_pReflect != NULL)
			{
				if (m_pReflect->m_hWnd != NULL)
				{
					// Create the control's window.
					CreateEx(0, NULL, m_strText, dwStyle,
						0, 0,
						rcPos.right - rcPos.left, rcPos.bottom - rcPos.top,
						m_pReflect->m_hWnd, 0);
				}

				if (m_hWnd != NULL)
				{
					//  Make both windows visible.
					m_pReflect->ShowWindow(SW_SHOWNORMAL);
					ShowWindow(SW_SHOWNORMAL);
				}
				else
				{
					// Window creation failed: cleanup.
					if (m_pReflect != NULL)
						m_pReflect->DestroyWindow();

					m_pReflect = NULL;
				}
			}
		}

		// Set the new window's font.
		OnFontChanged();
	}
	else
	{
		// If window does exist, reparent and reposition it.
		CWnd* pwndOuter = GetOuterWindow();
		ASSERT(pwndOuter != NULL);

		if (::GetParent(pwndOuter->m_hWnd) != hWndParent)
			::SetParent(pwndOuter->m_hWnd, hWndParent);

		pwndOuter->ShowWindow(SW_SHOWNORMAL);
		pwndOuter->MoveWindow(rcPos, FALSE);
	}

	ASSERT(m_hWnd != NULL);

	return (m_hWnd != NULL);
}


void COleControl::GetControlSize(int* pcx, int* pcy)
{
	ASSERT_POINTER(pcx, int);
	ASSERT_POINTER(pcy, int);

	SIZEL szlHimetric;
	SIZEL szlPixels;
	szlHimetric.cx = m_cxExtent;
	szlHimetric.cy = m_cyExtent;
	_XformSizeInHimetricToPixels(NULL, &szlHimetric, &szlPixels);
	*pcx = (int)szlPixels.cx;
	*pcy = (int)szlPixels.cy;
}

BOOL COleControl::SetControlSize(int cx, int cy)
{
	SIZEL szlPixels;
	SIZEL szlHimetric;
	szlPixels.cx = cx;
	szlPixels.cy = cy;
	_XformSizeInPixelsToHimetric(NULL, &szlPixels, &szlHimetric);
	return SUCCEEDED(m_xOleObject.SetExtent(DVASPECT_CONTENT, &szlHimetric));
}


void COleControl::ResizeOpenControl(int cx, int cy)
{
	CWnd* pWndOuter = GetOuterWindow();
	if ((pWndOuter != NULL) && (pWndOuter->m_hWnd != NULL))
		pWndOuter->SetWindowPos(NULL, 0, 0, cx, cy,
					SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	ResizeFrameWindow(cx, cy);
}


BOOL COleControl::SetRectInContainer(LPCRECT lpRect)
{
	if ((m_pInPlaceSite != NULL) && m_bInPlaceActive)
	{
		m_pInPlaceSite->OnPosRectChange(lpRect);
		return TRUE;
	}
	else if (m_bOpen)
	{
		ResizeOpenControl(lpRect->right - lpRect->left,
				lpRect->bottom - lpRect->top);
		return TRUE;
	}

	return FALSE;
}


void COleControl::OnResetState()
{
	CResetPropExchange px;
	DoPropExchange(&px);
	InvalidateControl();
}


void _AfxDrawBorder(CDC* pDC, CRect& rc, int cxBorder, int cyBorder, COLORREF cr)
{
	CBrush brush(cr);
	CBrush* pBrushSave = pDC->SelectObject(&brush);
	int cxRect = rc.Width();
	int cyRect = rc.Height();
	pDC->PatBlt(rc.left, rc.top, cxRect, cyBorder, PATCOPY);
	pDC->PatBlt(rc.left, rc.top, cxBorder, cyRect, PATCOPY);
	pDC->PatBlt(rc.left, rc.bottom - cyBorder, cxRect, cyBorder, PATCOPY);
	pDC->PatBlt(rc.right - cxBorder, rc.top, cxBorder, cyRect, PATCOPY);
	pDC->SelectObject(pBrushSave);
}


void COleControl::DrawContent(CDC* pDC, CRect& rc)
{
	//  Map into device coordinates.

	pDC->LPtoDP(&rc);
	int iSaveDC = pDC->SaveDC();
	pDC->SetViewportOrg(0, 0);
	pDC->SetWindowOrg(0, 0);
	pDC->SetMapMode(MM_TEXT);
	m_rcBounds = rc;

	if (m_sBorderStyle)
	{
		int cxBorder = GetSystemMetrics(SM_CXBORDER);
		int cyBorder = GetSystemMetrics(SM_CYBORDER);
		::_AfxDrawBorder(pDC, rc, cxBorder, cyBorder,
			GetSysColor(COLOR_WINDOWFRAME));
		rc.InflateRect(-cxBorder, -cyBorder);
	}

	OnDraw(pDC, rc, rc);
	pDC->RestoreDC(iSaveDC);
}


void COleControl::DrawMetafile(CDC* pDC, CRect& rc)
{
	int iSaveDC = pDC->SaveDC();
	m_rcBounds = rc;

	if (m_sBorderStyle)
	{
		int cxBorder = GetSystemMetrics(SM_CXBORDER);
		int cyBorder = GetSystemMetrics(SM_CYBORDER);
		::_AfxDrawBorder(pDC, rc, cxBorder, cyBorder,
			GetSysColor(COLOR_WINDOWFRAME));
		rc.InflateRect(-cxBorder, -cyBorder);
	}

	OnDrawMetafile(pDC, rc);
	pDC->RestoreDC(iSaveDC);
}


void COleControl::OnDraw(CDC*, const CRect&, const CRect&)
{
	// To be overridden by subclass.
}


void COleControl::OnDrawMetafile(CDC* pDC, const CRect& rcBounds)
{
	// By default, we draw into metafile the same way we would draw to the
	// screen.  This may be overridden by the subclass.

	OnDraw(pDC, rcBounds, rcBounds);
}


BOOL COleControl::GetMetafileData(LPFORMATETC lpFormatEtc,
		LPSTGMEDIUM lpStgMedium)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));
	ASSERT(AfxIsValidAddress(lpStgMedium, sizeof(STGMEDIUM)));
	ASSERT(lpStgMedium->tymed == TYMED_NULL);   // GetDataHere not valid
	ASSERT(lpStgMedium->pUnkForRelease == NULL);

	// medium must be TYMED_MFPICT -- cannot fill in existing HGLOBAL
	if (!(lpFormatEtc->tymed & TYMED_MFPICT) || lpStgMedium->hGlobal != NULL)
		return FALSE;

	// create appropriate memory metafile DC
	CMetaFileDC dc;
	if (!dc.Create())
		return FALSE;

	// create attribute DC according to lpFormatEtc->ptd
	HDC hAttribDC = ::_AfxCtlCreateDC(lpFormatEtc->ptd);
	dc.SetAttribDC(hAttribDC);

	// Paint directly into the metafile.
	int cx;
	int cy;
	GetControlSize(&cx, &cy);
	CRect rc(0, 0, cx, cy);
	dc.SetMapMode(MM_ANISOTROPIC);
	dc.SetWindowOrg(0, 0);
	dc.SetWindowExt(cx, cy);
	DrawMetafile(&dc, rc);

	// attribute DC is no longer necessary
	dc.SetAttribDC(NULL);
	::DeleteDC(hAttribDC);

	HMETAFILE hMF;
	hMF = (HMETAFILE)dc.Close();
	if (hMF == NULL)
		return FALSE;

	HGLOBAL hPict;
	if ((hPict = ::GlobalAlloc(GMEM_DDESHARE, sizeof(METAFILEPICT))) == NULL)
	{
		DeleteMetaFile(hMF);
		return FALSE;
	}
	LPMETAFILEPICT lpPict;
	if ((lpPict = (LPMETAFILEPICT)::GlobalLock(hPict)) == NULL)
	{
		DeleteMetaFile(hMF);
		::GlobalFree(hPict);
		return FALSE;
	}

	// set the metafile size
	lpPict->mm = MM_ANISOTROPIC;
	lpPict->hMF = hMF;
	lpPict->xExt = (int)m_cxExtent;
	lpPict->yExt = (int)m_cyExtent;

	// return the medium with the hGlobal to the METAFILEPICT
	::GlobalUnlock(hPict);
	lpStgMedium->hGlobal = hPict;
	lpStgMedium->tymed = TYMED_MFPICT;
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////
//
//  COleControl::OnCreateAggregates
//
//  Instantiates default handler as an aggregate, and obtains pointers
//  to its interfaces.
//
//////////////////////////////////////////////////////////////////////////////

BOOL COleControl::OnCreateAggregates()
{
	//
	//  If we're being aggregated, we want to pass the outer unknown
	//  to the object we're aggregating (the default handler).
	//  Otherwise, we pass our own "inner" unknown.
	//

	LPUNKNOWN pUnk = (LPUNKNOWN)m_pOuterUnknown;

	if (pUnk == NULL)
		pUnk = (LPUNKNOWN)&m_xInnerUnknown;

	HRESULT hr;

	CLSID clsid;
	GetClassID(&clsid);

	LPUNKNOWN pUnkControlling;

#ifdef OLE2ANSI

	// OLE2ANSI version of OleCreateDefaultHandler requires a "pre-wrapped"
	// interface for pUnkControlling.

	if (FAILED(::Ole2AnsiWFromA(IID_IUnknown, pUnk, &pUnkControlling)))
		return FALSE;

	// We don't want the wrapper to keep a reference count on us, so we
	// immediately decrement our reference count.
	pUnk->Release();

	// Keep the wrapper in a member variable for eventual cleanup.
	m_pUnkWrapper = pUnkControlling;

#else

	pUnkControlling = pUnk;

#endif

	// Note: This call will not increment pUnkControlling's reference count.
	hr = OleCreateDefaultHandler(
				clsid,
				pUnkControlling,
				IID_IUnknown,
				(LPLPVOID)&m_pDefIUnknown);

	if (FAILED(hr))
		return FALSE;

	// Note: For each of the following QueryInterface calls, we want to prevent
	// pUnkControlling's reference count from being incremented.  So, if the
	// call succeeds, we immediately force a decrement of the reference count.

	hr = m_pDefIUnknown->QueryInterface(
			IID_IPersistStorage, (LPLPVOID)&m_pDefIPersistStorage);

	if (FAILED(hr))
		return FALSE;

	pUnkControlling->Release();

	hr = m_pDefIUnknown->QueryInterface(
			IID_IViewObject, (LPLPVOID)&m_pDefIViewObject);

	if (FAILED(hr))
		return FALSE;

	pUnkControlling->Release();

	hr = m_pDefIUnknown->QueryInterface(
			IID_IOleCache, (LPLPVOID)&m_pDefIOleCache);

	if (FAILED(hr))
		return FALSE;

	pUnkControlling->Release();

	//  If we get this far, our attempt to aggregate on the default
	//  handler has succeeded!

	return TRUE;
}


void COleControl::InvalidateControl(LPCRECT lpRect)
{
	if (m_bInPlaceActive || m_bOpen)
		InvalidateRect(lpRect, TRUE);
	else
		SendAdvise(OBJECTCODE_VIEWCHANGED);

	SendAdvise(OBJECTCODE_DATACHANGED);
}


CWnd* COleControl::GetOuterWindow() const
{
	return (m_pReflect != NULL ? (CWnd*)m_pReflect : (CWnd*)this);
}


void COleControl::OnReflectorDestroyed()
{
	m_pReflect = NULL;
}


HRESULT COleControl::SaveState(IStream* pstm)
{
	HRESULT hr = NOERROR;

	TRY
	{
		//
		//  Delegate to the Serialize method.
		//

		COleStreamFile file(pstm);
		CArchive ar(&file, CArchive::store);
		Serialize(ar);
	}
	CATCH_ALL(e)
	{
		hr = ResultFromScode(E_FAIL);
	}
	END_CATCH_ALL

	return hr;
}


HRESULT COleControl::LoadState(IStream* pstm)
{
	HRESULT hr = NOERROR;

	TRY
	{
		//
		//  Delegate to the Serialize method.
		//

		COleStreamFile file(pstm);
		CArchive ar(&file, CArchive::load);
		Serialize(ar);
	}
	CATCH_ALL(e)
	{
		//
		//  The load failed.  Delete any partially-initialized state.
		//

		OnResetState();
		hr = ResultFromScode(E_FAIL);
	}
	END_CATCH_ALL

	// Clear the modified flag.
	m_bModified = FALSE;

	// Unless IOleObject::SetClientSite is called after this, we can
	// count on ambient properties being available while loading.
	m_bCountOnAmbients = TRUE;

	return hr;
}


void COleControl::SendAdvise(UINT uCode)
{
	// Calls the appropriate IOleClientSite or IAdviseSink member function
	// for various events such as closure, renaming, saving, etc.

	switch (uCode)
	{
	case OBJECTCODE_SAVED:
		if (m_pOleAdviseHolder != NULL)
			m_pOleAdviseHolder->SendOnSave();
		break;

	case OBJECTCODE_CLOSED:
		if (m_pOleAdviseHolder != NULL)
			m_pOleAdviseHolder->SendOnClose();
		break;

	case OBJECTCODE_SAVEOBJECT:
		if (m_bModified && m_pClientSite != NULL)
			m_pClientSite->SaveObject();
		break;

	case OBJECTCODE_DATACHANGED:
		//No flags are necessary here.
		if (m_pDataAdviseHolder != NULL)
			m_pDataAdviseHolder->SendOnDataChange(&m_xDataObject, 0, 0);
		break;

	case OBJECTCODE_SHOWWINDOW:
		if (m_pClientSite != NULL)
			m_pClientSite->OnShowWindow(TRUE);
		break;

	case OBJECTCODE_HIDEWINDOW:
		if (m_pClientSite != NULL)
			m_pClientSite->OnShowWindow(FALSE);
		break;

	case OBJECTCODE_SHOWOBJECT:
		if (m_pClientSite != NULL)
			m_pClientSite->ShowObject();
		break;

	case OBJECTCODE_VIEWCHANGED:
		if (m_pDefIViewObject != NULL)
		{
			DWORD aspects;
			DWORD advf;
			LPADVISESINK pAdvSink;

			if (SUCCEEDED(m_xViewObject.GetAdvise(&aspects, &advf, &pAdvSink)) &&
				(pAdvSink != NULL))
			{
				pAdvSink->OnViewChange(DVASPECT_CONTENT, -1);
				pAdvSink->Release();
			}
		}
		break;
	}
}


HRESULT COleControl::OnHide()
{
	CWnd* pWnd = m_bOpen ? m_pWndOpenFrame : GetOuterWindow();
	if (pWnd != NULL && pWnd->m_hWnd != NULL)
	{
		pWnd->ShowWindow(SW_HIDE);
		pWnd->SetParent(NULL);
	}

	RELEASE(m_pInPlaceFrame);
	RELEASE(m_pInPlaceDoc);

	if (m_bOpen)
		SendAdvise(OBJECTCODE_HIDEWINDOW);

	return NOERROR;
}


void COleControl::ResizeFrameWindow(int cxCtrl, int cyCtrl)
{
	m_pWndOpenFrame->SetWindowPos(NULL, 0, 0, 100, 100,
				SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	CRect rectClient;
	m_pWndOpenFrame->GetClientRect(&rectClient);
	CRect rectWindow;
	m_pWndOpenFrame->GetWindowRect(&rectWindow);
	int cx = cxCtrl + rectWindow.Width()  - rectClient.Width();
	int cy = cyCtrl + rectWindow.Height() - rectClient.Height();
	m_pWndOpenFrame->SetWindowPos(NULL, 0, 0, cx, cy,
				SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
}


HRESULT COleControl::OnOpen(BOOL bTryInPlace, LPMSG pMsg)
{
	if (!m_bOpen)
	{
		//  If not already open, try in-place activating.

		if (bTryInPlace && SUCCEEDED(OnActivateInPlace(TRUE, pMsg)))
			return NOERROR;

		//  If already in-place active, deactivate first.
		if (m_bInPlaceActive)
			m_xOleInPlaceObject.InPlaceDeactivate();

		//  Open a separate window.
		if (m_pWndOpenFrame == NULL)
		{
			//  Create frame window
			m_pWndOpenFrame = CreateFrameWindow();

			if (m_pWndOpenFrame == NULL)
				return ResultFromScode(E_FAIL);

			//  Size frame window to exactly contain the control.
			int cx;
			int cy;
			GetControlSize(&cx, &cy);
			ResizeFrameWindow(cx, cy);

			//  Create and/or reparent the control's window.
			CRect rectClient;
			m_pWndOpenFrame->GetClientRect(&rectClient);
			if (! CreateControlWindow(m_pWndOpenFrame->m_hWnd, rectClient))
				return ResultFromScode(E_FAIL);
		}

		m_bOpen = TRUE;
	}

	//  Make the frame window visible and activate it.
	ASSERT(m_pWndOpenFrame != NULL);
	m_pWndOpenFrame->ShowWindow(SW_SHOW);
	m_pWndOpenFrame->SetActiveWindow();
	SendAdvise(OBJECTCODE_SHOWWINDOW);

	return NOERROR;
}


CControlFrameWnd* COleControl::CreateFrameWindow()
{
	LPCTSTR pszFrameTitle;
	if (m_strFrameTitle.GetLength() > 0)
		pszFrameTitle = m_strFrameTitle;
	else
		pszFrameTitle = GetUserType();

	CControlFrameWnd* pWnd = new CControlFrameWnd(this);
	if (! pWnd->Create(pszFrameTitle))
	{
		// If Create failed, then frame window has deleted itself.
		pWnd = NULL;
	}

	return pWnd;
}


void COleControl::OnFrameClose()
{
	//  Reparent control to prevent its window from being destroyed.
	CWnd* pWnd = GetOuterWindow();

	if (pWnd != NULL)
	{
		pWnd->ShowWindow(SW_HIDE);
		pWnd->SetParent(NULL);
	}

	m_pWndOpenFrame = NULL;
	m_bOpen = FALSE;

	m_xOleObject.Close(OLECLOSE_SAVEIFDIRTY);

	SendAdvise(OBJECTCODE_HIDEWINDOW);
	SendAdvise(OBJECTCODE_CLOSED);
}


HRESULT COleControl::OnActivateInPlace(BOOL bUIActivate, LPMSG pMsg)
{
	if (m_bOpen)
	{
		m_pWndOpenFrame->SetActiveWindow();
		SendAdvise(OBJECTCODE_SHOWWINDOW);
		return NOERROR;
	}

	if (m_bOpen || (m_bInPlaceActive && !bUIActivate) || m_bUIActive)
		return NOERROR;

	HRESULT hr = ResultFromScode(E_FAIL);
	if (m_pInPlaceSite != NULL)
		hr = m_pInPlaceSite->CanInPlaceActivate();

	if (FAILED(hr) || (GetScode(hr) == S_FALSE))
	{
		//  Site doesn't allow in-place activation.
		return OnOpen(FALSE, pMsg);
	}

	if (! m_bInPlaceActive)
	{
		m_pInPlaceSite->OnInPlaceActivate();
		m_bInPlaceActive = TRUE;
	}

	HWND hwndParent = NULL;

	if (SUCCEEDED(m_pInPlaceSite->GetWindow(&hwndParent)))
	{
		RECT rcClip;
		m_frameInfo.cb = sizeof(OLEINPLACEFRAMEINFO);

		RELEASE(m_pInPlaceFrame);
		RELEASE(m_pInPlaceDoc);

		if (SUCCEEDED(m_pInPlaceSite->GetWindowContext(
						&m_pInPlaceFrame, &m_pInPlaceDoc,
						&m_rcPos, &rcClip, &m_frameInfo)))
		{
			ASSERT(m_pInPlaceFrame != NULL);

			if (CreateControlWindow(hwndParent, m_rcPos))
			{
				if (bUIActivate)
				{
					BuildSharedMenu();

					m_pInPlaceSite->OnUIActivate();
					m_bUIActive = TRUE;

					m_pInPlaceFrame->SetActiveObject(
						&m_xOleInPlaceActiveObject, NULL);

					if (m_pInPlaceDoc != NULL)
						m_pInPlaceDoc->SetActiveObject(
							&m_xOleInPlaceActiveObject, NULL);

					AddFrameLevelUI();

					if (m_hWnd != NULL)
						SetFocus();

				}

				// Pass thru the window message that caused us to be activated

				if ((m_hWnd != NULL) && (pMsg != NULL))
					ForwardActivationMsg(pMsg);

				//  Send appropriate notifications...

				SendAdvise(OBJECTCODE_SHOWOBJECT);

				return NOERROR;
			}
		}
	}

	RELEASE(m_pInPlaceFrame);
	RELEASE(m_pInPlaceDoc);

	return ResultFromScode(E_FAIL);
}


void COleControl::ForwardActivationMsg(LPMSG pMsg)
{
	UINT uMsg = pMsg->message;
	LPARAM lParam = pMsg->lParam;

	// For mouse messages, convert to this window's coordinates

	if ((uMsg >= WM_MOUSEFIRST) && (uMsg <= WM_MOUSELAST) && m_bEnabled)
	{
		POINT ptNew = pMsg->pt;
		ScreenToClient(&ptNew);
		lParam = MAKELONG((short)ptNew.x, (short)ptNew.y);
	}

	// Pass the message along to the control's window

	SendMessage(uMsg, pMsg->wParam, lParam);
}


HMENU COleControl::OnGetInPlaceMenu()
{
	// Default: no in-place menu
	return NULL;
}


BOOL COleControl::BuildSharedMenu()
{
	// This can be called more than once on mouse clicks
	if (m_hSharedMenu != NULL)
		return TRUE;

	HMENU hInPlaceMenu = OnGetInPlaceMenu();

	ASSERT(m_hSharedMenu == NULL);

	memset(&m_menuWidths, 0, sizeof m_menuWidths);

	if ((!m_bUIDead) && (hInPlaceMenu != NULL))
	{
		// Create shared menu
		if ((m_hSharedMenu = ::CreateMenu()) == NULL)
			return FALSE;

		// Start out by getting menu from container
		if (m_pInPlaceFrame->InsertMenus(m_hSharedMenu, &m_menuWidths) != NOERROR)
		{
			::DestroyMenu(m_hSharedMenu);
			m_hSharedMenu = NULL;
		}
		else
		{
			// Container shouldn't touch these
			ASSERT(m_menuWidths.width[1] == 0);
			ASSERT(m_menuWidths.width[3] == 0);
			ASSERT(m_menuWidths.width[5] == 0);

			// Only copy the popups if there is a menu loaded
			if (hInPlaceMenu != NULL)
			{
				// Insert our menu popups amongst the container menus
				AfxMergeMenus(CMenu::FromHandle(m_hSharedMenu),
					CMenu::FromHandle(hInPlaceMenu), &m_menuWidths.width[0], 1);
			}
		}
	}

	// Finally create the special OLE menu descriptor
	m_hOleMenu = ::OleCreateMenuDescriptor(m_hSharedMenu, &m_menuWidths);

	return m_hOleMenu != NULL;
}


void COleControl::DestroySharedMenu()
{
	HMENU hInPlaceMenu = NULL;

	if ((m_hSharedMenu != NULL) &&
		((hInPlaceMenu = OnGetInPlaceMenu()) != NULL))
	{
		// remove our menu popups from the shared menu
		AfxUnmergeMenus(CMenu::FromHandle(m_hSharedMenu), CMenu::FromHandle(hInPlaceMenu));

		// allow container to remove its items from the menu
		ASSERT(m_pInPlaceFrame != NULL);
		VERIFY(m_pInPlaceFrame->RemoveMenus(m_hSharedMenu) == NOERROR);

		// now destroy the menu
		::DestroyMenu(m_hSharedMenu);
		m_hSharedMenu = NULL;
	}

	if (m_hOleMenu != NULL)
	{
		VERIFY(::OleDestroyMenuDescriptor(m_hOleMenu) == NOERROR);
		m_hOleMenu = NULL;
	}
}


void COleControl::AddFrameLevelUI()
{
	m_pInPlaceFrame->SetMenu(m_hSharedMenu, m_hOleMenu, m_hWnd);
	OnShowToolBars();
}


void COleControl::RemoveFrameLevelUI()
{
	OnHideToolBars();

	// allow container to remove its items from the menu
	ASSERT(m_pInPlaceFrame != NULL);
	if (m_hSharedMenu != NULL)
		VERIFY(m_pInPlaceFrame->RemoveMenus(m_hSharedMenu) == NOERROR);
}


void COleControl::OnShowToolBars()
{
	// Default sets border space to empty.
	// When overriding, don't call this implementation.

	m_pInPlaceFrame->SetBorderSpace(NULL);
}


void COleControl::OnHideToolBars()
{
	// Default does nothing
}


BOOL COleControl::GetRectInContainer(LPRECT lpRect)
{
	if (m_bInPlaceActive)
		CopyRect(lpRect, &m_rcPos);

	return m_bInPlaceActive;
}


LPCLSID COleControl::GetPropPageIDs(ULONG& cPropPages)
{
	cPropPages = 0;
	return NULL;
}


BOOL COleControl::OnEdit(LPMSG lpMsg, HWND, LPCRECT lpRect)
{
	CopyRect(m_rcPos, lpRect);
	return SUCCEEDED(OnActivateInPlace(TRUE, lpMsg));
}


HWND _AfxGetTopLevelWindow(HWND hWnd)
{
	HWND hWndTop;

	do
	{
		hWndTop = hWnd;
		hWnd = ::GetParent(hWnd);
	}
	while (hWnd != NULL);

	return hWndTop;
}


BOOL COleControl::OnProperties(LPMSG, HWND hWndParent, LPCRECT)
{
	HRESULT hr;

	if ((m_pControlSite == NULL) ||
		FAILED(hr = m_pControlSite->ShowPropertyFrame()))
	{
		LPUNKNOWN pUnk = GetIDispatch(FALSE);
		HWND hWndTop = _AfxGetTopLevelWindow(hWndParent);

		LCID lcid = AmbientLocaleID();

		ULONG cPropPages;
		LPCLSID pclsidPropPages = GetPropPageIDs(cPropPages);

		RECT rectParent;
		RECT rectTop;
		::GetWindowRect(hWndParent, &rectParent);
		::GetWindowRect(hWndTop, &rectTop);

		PreModalDialog(hWndTop);

		hr = OleCreatePropertyFrame(hWndTop, rectParent.left - rectTop.left,
				rectParent.top - rectTop.top, GetUserType(), 1, &pUnk,
				cPropPages, pclsidPropPages, lcid, NULL, 0);

		PostModalDialog(hWndTop);
	}

	return SUCCEEDED(hr);
}


void COleControl::OnPaint(CDC* pDC)
{
	AfxLockTempMaps();

	CRect rcClient;
	GetClientRect(rcClient);
	m_rcBounds = rcClient;

	if (pDC != NULL)
	{
		// We were passed a device context: use it.
		int iSaveDC = pDC->SaveDC();
		OnDraw(pDC, rcClient, rcClient);
		pDC->RestoreDC(iSaveDC);
	}
	else
	{
		// Create a device context for painting.
		CPaintDC dc(this);
		OnDraw(&dc, rcClient, &dc.m_ps.rcPaint);
	}

	AfxUnlockTempMaps();
}


void COleControl::Serialize(CArchive& ar)
{
	CArchivePropExchange px(ar);
	DoPropExchange(&px);
	if (ar.IsLoading())
		InvalidateControl();
}


void COleControl::DoPropExchange(CPropExchange* pPX)
{
	ASSERT_POINTER(pPX, CPropExchange);

	ExchangeExtent(pPX);
	ExchangeStockProps(pPX);
}


/////////////////////////////////////////////////////////////////////////////
// Wrappers for IOleControlSite

void COleControl::ControlInfoChanged()
{
	if (m_pControlSite != NULL)
		m_pControlSite->OnControlInfoChanged();
}


BOOL COleControl::LockInPlaceActive(BOOL bLock)
{
	if (m_pControlSite != NULL)
		return SUCCEEDED(m_pControlSite->LockInPlaceActive(bLock));

	return FALSE;
}

LPDISPATCH COleControl::GetExtendedControl()
{
	LPDISPATCH pDispatch = NULL;
	if (m_pControlSite != NULL)
		m_pControlSite->GetExtendedControl(&pDispatch);

	return pDispatch;
}


void COleControl::TransformCoords(POINTL FAR* lpptlHimetric,
	POINTF FAR* lpptfContainer, DWORD flags)
{
	if ((m_pControlSite == NULL) ||
		(FAILED(m_pControlSite->TransformCoords(lpptlHimetric,
			lpptfContainer, flags))))
	{
		// Transformation failed, use the identity transformation

		if (flags & XFORMCOORDS_CONTAINERTOHIMETRIC)
		{
			lpptlHimetric->x = (long)lpptfContainer->x;
			lpptlHimetric->y = (long)lpptfContainer->y;
		}
		else
		{
			lpptfContainer->x = (float)lpptlHimetric->x;
			lpptfContainer->y = (float)lpptlHimetric->y;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// COleControl::XOleControl


STDMETHODIMP_(ULONG) COleControl::XOleControl::AddRef()
{
	//  Delegate to our exported AddRef.
	METHOD_MANAGE_STATE(COleControl, OleControl)
	return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) COleControl::XOleControl::Release()
{
	//  Delegate to our exported Release.
	METHOD_MANAGE_STATE(COleControl, OleControl)
	return (ULONG)pThis->ExternalRelease();
}


STDMETHODIMP COleControl::XOleControl::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	//  Delegate to our exported QueryInterface.
	METHOD_MANAGE_STATE(COleControl, OleControl)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP COleControl::XOleControl::GetControlInfo(LPCONTROLINFO pCI)
{
	METHOD_MANAGE_STATE(COleControl, OleControl)
	pThis->OnGetControlInfo(pCI);
	return NOERROR;
}


STDMETHODIMP COleControl::XOleControl::OnMnemonic(LPMSG pMsg)
{
	METHOD_MANAGE_STATE(COleControl, OleControl)
	pThis->OnMnemonic(pMsg);
	return NOERROR;
}


STDMETHODIMP COleControl::XOleControl::OnAmbientPropertyChange(DISPID dispid)
{
	METHOD_MANAGE_STATE(COleControl, OleControl)

	if (dispid == DISPID_AMBIENT_UIDEAD || dispid == DISPID_UNKNOWN)
		pThis->m_bUIDead = (BYTE)(pThis->AmbientUIDead());

	pThis->OnAmbientPropertyChange(dispid);
	return NOERROR;
}


STDMETHODIMP COleControl::XOleControl::FreezeEvents(BOOL bFreeze)
{
	METHOD_MANAGE_STATE(COleControl, OleControl)

	ULONG& cEventsFrozen = pThis->m_cEventsFrozen;

	if (bFreeze)
		++(cEventsFrozen);
	else
		--(cEventsFrozen);

	ASSERT(cEventsFrozen >= 0); // Should never go below zero!

	if ((cEventsFrozen == 1 && bFreeze) ||
		(cEventsFrozen == 0 && !bFreeze))
	{
		pThis->OnFreezeEvents(bFreeze);
	}

	return NOERROR;
}


void COleControl::OnGetControlInfo(LPCONTROLINFO pControlInfo)
{
	// Subclass may override

	pControlInfo->hAccel = NULL;
	pControlInfo->cAccel = 0;
	pControlInfo->dwFlags = 0;
}


void COleControl::OnMnemonic(LPMSG)
{
	// To be implemented by subclass
}


void COleControl::OnAmbientPropertyChange(DISPID)
{
	// To be implemented by subclass
}


void COleControl::OnFreezeEvents(BOOL)
{
	// To be implemented by subclass
}


void COleControl::OnSetClientSite()
{
	// Subclass can override to provide additional code to be run.
}


LPOLECLIENTSITE COleControl::GetClientSite()
{
	return m_pClientSite;
}


COLORREF COleControl::TranslateColor(OLE_COLOR clrColor, HPALETTE hpal)
{
	COLORREF cr = RGB(0x00,0x00,0x00);
	OleTranslateColor(clrColor, hpal, &cr);
	return cr;
}


void COleControl::Refresh()
{
	InvalidateControl();
	if (m_hWnd != NULL)
		UpdateWindow();
}


void COleControl::DoClick()
{
	OnClick(LEFT_BUTTON);
}


BOOL COleControl::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (m_pReflect != NULL)
		m_pReflect->SetControl(this);

	return CWnd::OnNcCreate(lpCreateStruct);
}


void COleControl::RecreateControlWindow()
{
	if (m_bInPlaceActive)
	{
		BOOL bUIActive = m_bUIActive;
		m_xOleInPlaceObject.InPlaceDeactivate();
		DestroyWindow();
		OnActivateInPlace(bUIActive, NULL);
	}
	else if (m_bOpen)
	{
		DestroyWindow();
		CRect rectClient;
		m_pWndOpenFrame->GetClientRect(&rectClient);
		CreateControlWindow(m_pWndOpenFrame->m_hWnd, rectClient);
	}
	else
	{
		HWND hWndParent = GetParkingWindow()->GetSafeHwnd();
		if (hWndParent != NULL)
		{
			DestroyWindow();
			int cx;
			int cy;
			GetControlSize(&cx, &cy);
			CRect rect(0, 0, cx, cy);
			CreateControlWindow(hWndParent, rect);
		}
	}
}


void COleControl::CreateWindowForSubclassedControl()
{
	if (GetSuperWndProcAddr() != CWnd::GetSuperWndProcAddr() &&
		(m_hWnd == NULL))
	{
		// If this is a subclassed control, we should create the window
		// for it now, in case the window is needed by the DoSuperclassPaint
		// implementation.

		HWND hWndParent = GetParkingWindow()->GetSafeHwnd();
		if (hWndParent != NULL)
		{
			SIZEL szlHimetric;
			SIZEL szlPixels;
			szlHimetric.cx = m_cxExtent;
			szlHimetric.cy = m_cyExtent;
			_XformSizeInHimetricToPixels(NULL, &szlHimetric, &szlPixels);
			CRect rcPos(0, 0, (int)szlPixels.cx, (int)szlPixels.cy);
			CreateControlWindow(hWndParent, rcPos);
		}
	}
}


int COleControl::OnMouseActivate(CWnd *pDesktopWnd, UINT nHitTest, UINT message)
{
	if (m_bInPlaceActive && !m_bUIActive)
		m_bPendingUIActivation = TRUE;

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message );
}


void COleControl::PreModalDialog(HWND hWndParent)
{
	if (m_pInPlaceFrame != NULL)
	{
		m_pInPlaceFrame->EnableModeless(FALSE);
	}
	else
	{
		HWND hWndTop = _AfxGetTopLevelWindow(hWndParent);
		if (hWndTop != NULL)
			::EnableWindow(_AfxGetTopLevelWindow(hWndParent), FALSE);
	}
}


void COleControl::PostModalDialog(HWND hWndParent)
{
	if (m_pInPlaceFrame != NULL)
	{
		m_pInPlaceFrame->EnableModeless(TRUE);
	}
	else
	{
		HWND hWndTop = _AfxGetTopLevelWindow(hWndParent);
		if (hWndTop != NULL)
			::EnableWindow(_AfxGetTopLevelWindow(hWndParent), TRUE);
	}
}


void COleControl::SetModifiedFlag(BOOL bModified)
{
	m_bModified = (BYTE)bModified;
}


BOOL COleControl::IsModified()
{
	return m_bModified;
}


BOOL COleControl::WillAmbientsBeValidDuringLoad()
{
	return m_bCountOnAmbients;
}


void COleControl::EnableSimpleFrame()
{
	m_bSimpleFrame = TRUE;
}


BOOL COleControl::IgnoreWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam,
	LRESULT* plResult)
{
	if (! m_bUIDead)
		return FALSE;

	switch (msg)
	{
	case WM_NCHITTEST:
		*plResult = HTNOWHERE;
		return TRUE;

	case WM_SETCURSOR:
		*plResult = ::SendMessage(::GetParent(m_hWnd), msg, wParam, lParam);
		return TRUE;
	}

	if ((msg >= WM_KEYFIRST) && (msg <= WM_KEYLAST))
	{
		*plResult = 0;
		return TRUE;
	}

	return FALSE;
}


LRESULT COleControl::WindowProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	DWORD dwCookie;
	LRESULT lResult;
	HRESULT hr;

	AFX_MANAGE_STATE(m_pModuleState);

	// Give the simple frame site the opportunity to filter the message
	if ((m_pSimpleFrameSite != NULL) &&
		SUCCEEDED(hr = m_pSimpleFrameSite->PreMessageFilter(
			m_hWnd, msg, wParam, lParam, &lResult, &dwCookie)))
	{
		if (hr == NOERROR)
		{
			if (! IgnoreWindowMessage(msg, wParam, lParam, &lResult))
				lResult = CWnd::WindowProc(msg, wParam, lParam);
			m_pSimpleFrameSite->PostMessageFilter(
				m_hWnd, msg, wParam, lParam, &lResult, dwCookie);
		}
	}
	else
	{
		if (! IgnoreWindowMessage(msg, wParam, lParam, &lResult))
			lResult = CWnd::WindowProc(msg, wParam, lParam);
	}

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
// Command prompts

void COleControl::OnInitMenuPopup(CMenu* pMenu, UINT, BOOL bSysMenu)
{
	AfxCancelModes(m_hWnd);

	if (bSysMenu)
		return;     // don't support system menu

	ASSERT(pMenu != NULL);
	// check the enabled state of various menu items

	CCmdUI state;
	state.m_pMenu = pMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// determine if menu is popup in top-level menu and set m_pOther to
	//  it if so (m_pParentMenu == NULL indicates that it is secondary popup)
	HMENU hParentMenu;
	if (_afxTrackingMenu == pMenu->m_hMenu)
		state.m_pParentMenu = pMenu;    // parent == child for tracking popup
	else
	{
		CWnd* pParent = GetTopLevelParent();
			// child windows don't have menus -- need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pMenu->m_hMenu)
				{
					// when popup is found, m_pParentMenu is containing menu
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
	  state.m_nIndex++)
	{
		state.m_nID = pMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // menu separator or invalid cmd - ignore it

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// possibly a popup menu, route to first item of that popup
			state.m_pSubMenu = pMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // first item of popup can't be routed to
			}
			state.DoUpdate(this, FALSE);    // popups are never auto disabled
		}
		else
		{
			// normal menu item
			// Auto enable/disable if frame window has 'm_bAutoMenuEnable'
			//    set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, m_bAutoMenuEnable && state.m_nID < 0xF000);
		}
	}
}

void COleControl::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU /*hSysMenu*/)
{
	// set the tracking state (update on idle)
	if (nFlags == 0xFFFF)
	{
		m_nIDTracking = AFX_IDS_IDLEMESSAGE;
		SendMessage(WM_SETMESSAGESTRING, (WPARAM)m_nIDTracking);
		ASSERT(m_nIDTracking == m_nIDLastMessage);
	}
	else if (nItemID == 0 ||
		nFlags & (MF_SEPARATOR|MF_POPUP|MF_MENUBREAK|MF_MENUBARBREAK))
	{
		// nothing should be displayed
		m_nIDTracking = 0;
	}
	else if (nItemID >= 0xF000 && nItemID < 0xF1F0) // max of 31 SC_s
	{
		// special strings table entries for system commands
		m_nIDTracking = ID_COMMAND_FROM_SC(nItemID);
		ASSERT(m_nIDTracking >= AFX_IDS_SCFIRST &&
			m_nIDTracking < AFX_IDS_SCFIRST + 31);
	}
	else if (nItemID >= AFX_IDM_FIRST_MDICHILD)
	{
		// all MDI Child windows map to the same help id
		m_nIDTracking = AFX_IDS_MDICHILD;
	}
	else
	{
		// track on idle
		m_nIDTracking = nItemID;
	}

	// when running in-place, it is necessary to cause a message to
	//  be pumped through the queue.
	if (m_nIDTracking != m_nIDLastMessage && GetParent() != NULL)
		PostMessage(WM_NULL);
}

void COleControl::GetMessageString(UINT nID, CString& rMessage) const
{
	// load appropriate string
	LPTSTR lpsz = rMessage.GetBuffer(255);
	if (AfxLoadString(nID, lpsz) != 0)
	{
		// first newline terminates actual string
		lpsz = _tcschr(lpsz, (TCHAR)'\n');
		if (lpsz != NULL)
			*lpsz = '\0';
	}
	else
	{
		// not found
		TRACE1("Warning: no message line prompt for ID 0x%04X.\n", nID);
	}
	rMessage.ReleaseBuffer();
}

LRESULT COleControl::OnSetMessageString(WPARAM wParam, LPARAM lParam)
{
	if (m_pInPlaceFrame != NULL)
	{
		LPCTSTR lpsz = NULL;
		CString strMessage;

		// set the message bar text
		if (lParam != 0)
		{
			ASSERT(wParam == 0);    // can't have both an ID and a string
			lpsz = (LPCTSTR)lParam; // set an explicit string
		}
		else if (wParam != 0)
		{
			// use the wParam as a string ID
			GetMessageString(wParam, strMessage);
			lpsz = strMessage;
		}

		// notify container of new status text
		m_pInPlaceFrame->SetStatusText(lpsz);
	}

	UINT nIDLast = m_nIDLastMessage;
	m_nIDLastMessage = (UINT)wParam;    // new ID (or 0)
	m_nIDTracking = (UINT)wParam;       // so F1 on toolbar buttons work
	return nIDLast;
}

void COleControl::OnEnterIdle(UINT nWhy, CWnd* /*pWho*/)
{
	if (nWhy != MSGF_MENU || m_nIDTracking == m_nIDLastMessage)
		return;

	SendMessage(WM_SETMESSAGESTRING, (WPARAM)m_nIDTracking);
	ASSERT(m_nIDTracking == m_nIDLastMessage);
}

/////////////////////////////////////////////////////////////////////////////
// COleControl::XSpecifyPropertyPages


STDMETHODIMP_(ULONG) COleControl::XSpecifyPropertyPages::AddRef()
{
	//  Delegate to our exported AddRef.
	METHOD_MANAGE_STATE(COleControl, SpecifyPropertyPages)
	return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) COleControl::XSpecifyPropertyPages::Release()
{
	//  Delegate to our exported Release.
	METHOD_MANAGE_STATE(COleControl, SpecifyPropertyPages)
	return (ULONG)pThis->ExternalRelease();
}


STDMETHODIMP COleControl::XSpecifyPropertyPages::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	//  Delegate to our exported QueryInterface.
	METHOD_MANAGE_STATE(COleControl, SpecifyPropertyPages)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP COleControl::XSpecifyPropertyPages::GetPages(CAUUID FAR* pPages)
{
	METHOD_MANAGE_STATE(COleControl, SpecifyPropertyPages)

	ASSERT(pPages != NULL);

	if (pPages == NULL)
		return ResultFromScode(E_POINTER);

	pPages->cElems = 0;
	pPages->pElems = NULL;

	HRESULT hr;
	ULONG cElems;
	LPCLSID pClassID = pThis->GetPropPageIDs(cElems);

	IMalloc* pMalloc = NULL;

	if ((pPages->pElems = (LPCLSID)(_AfxCtlMalloc(cElems * sizeof(CLSID)))) != NULL)
	{
		ASSERT(pPages->pElems != NULL);
		pPages->cElems = cElems;
		memcpy(pPages->pElems, pClassID, (int)(cElems * sizeof(CLSID)));
		hr = NOERROR;
	}
	else
	{
		hr = ResultFromScode(E_OUTOFMEMORY);
	}

	return hr;
}

void COleControl::OnDestroy()
{
	// Release hfont, if any.
	if (m_hFontPrev != NULL)
	{
		SendMessage(WM_SETFONT, (WPARAM)NULL, 0);
		InternalGetFont().m_pFont->ReleaseHfont(m_hFontPrev);
		m_hFontPrev = NULL;
	}

	CWnd::OnDestroy();
}

void COleControl::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	if (m_pControlSite != NULL)
		m_pControlSite->OnFocus(FALSE);
}

void COleControl::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	if (m_pControlSite != NULL)
		m_pControlSite->OnFocus(TRUE);
}

#ifdef _WIN32

LRESULT COleControl::OnOcmCtlColorBtn(WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(m_hWnd, WM_CTLCOLORBTN, wParam, lParam);
}

LRESULT COleControl::OnOcmCtlColorDlg(WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(m_hWnd, WM_CTLCOLORDLG, wParam, lParam);
}

LRESULT COleControl::OnOcmCtlColorEdit(WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(m_hWnd, WM_CTLCOLOREDIT, wParam, lParam);
}

LRESULT COleControl::OnOcmCtlColorListBox(WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(m_hWnd, WM_CTLCOLORLISTBOX, wParam, lParam);
}

LRESULT COleControl::OnOcmCtlColorMsgBox(WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(m_hWnd, WM_CTLCOLORMSGBOX, wParam, lParam);
}

LRESULT COleControl::OnOcmCtlColorScrollBar(WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(m_hWnd, WM_CTLCOLORSCROLLBAR, wParam, lParam);
}

LRESULT COleControl::OnOcmCtlColorStatic(WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(m_hWnd, WM_CTLCOLORSTATIC, wParam, lParam);
}

#else

LRESULT COleControl::OnOcmCtlColor(WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(m_hWnd, WM_CTLCOLOR, wParam, lParam);
}

#endif // _WIN32


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

IMPLEMENT_DYNAMIC(COleControl, CWnd)
