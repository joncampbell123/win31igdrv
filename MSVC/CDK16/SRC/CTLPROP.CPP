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
// Stock property mask

#define STOCKPROP_BACKCOLOR     0x00000001
#define STOCKPROP_CAPTION       0x00000002
#define STOCKPROP_FONT          0x00000004
#define STOCKPROP_FORECOLOR     0x00000008
#define STOCKPROP_TEXT          0x00000010
#define STOCKPROP_BORDERSTYLE   0x00000020
#define STOCKPROP_ENABLED       0x00000040

static const DISPID BASED_CODE _rgdispidStockProps[] =
{
	DISPID_BACKCOLOR,
	DISPID_CAPTION,
	DISPID_FONT,
	DISPID_FORECOLOR,
	DISPID_TEXT,
	DISPID_BORDERSTYLE,
	DISPID_ENABLED,
	DISPID_UNKNOWN,
};

void COleControl::InitStockPropMask()
{
	const DISPID* pDispID = _rgdispidStockProps;
	const AFX_DISPMAP_ENTRY FAR* pEntry;
	DWORD dwMaskBit = 1;

	while (*pDispID != DISPID_UNKNOWN)
	{
		if (((pEntry = GetDispEntry(*pDispID)) != NULL) &&
#if (_MFC_VER >= 0x300)
			(pEntry->lpszParams != NULL) &&
			(memcmp(pEntry->lpszParams, VTS_STOCK, sizeof(VTS_STOCK)) == 0))
#else
			(memcmp(pEntry->pbParams, VTS_STOCK, sizeof(VTS_STOCK)) == 0))
#endif
		{
			m_dwStockPropMask |= dwMaskBit;
		}

		dwMaskBit <<= 1;
		++pDispID;
	}
}


/////////////////////////////////////////////////////////////////////////////
// Property exchange for stock properties

void COleControl::ExchangeStockProps(CPropExchange* pPX)
{
	BOOL bLoading = pPX->IsLoading();
	DWORD dwPersistMask = m_dwStockPropMask;

	PX_ULong(pPX, _T("_StockProps"), dwPersistMask);

	if (m_dwStockPropMask & (STOCKPROP_CAPTION | STOCKPROP_TEXT))
	{
		CString strText;

		if (dwPersistMask & (STOCKPROP_CAPTION | STOCKPROP_TEXT))
		{
			if (! bLoading)
				strText = InternalGetText();
			if (m_dwStockPropMask & STOCKPROP_CAPTION)
				PX_String(pPX, _T("Caption"), strText, _T(""));
			if (m_dwStockPropMask & STOCKPROP_TEXT)
				PX_String(pPX, _T("Text"), strText, _T(""));
		}
		if (bLoading)
		{
			TRY
				SetText(strText);
			END_TRY
		}
	}

	if (m_dwStockPropMask & STOCKPROP_FORECOLOR)
	{
		if (dwPersistMask & STOCKPROP_FORECOLOR)
			PX_Color(pPX, _T("ForeColor"), m_clrForeColor, AmbientForeColor());
		else if (bLoading)
			m_clrForeColor = AmbientForeColor();
	}

	if (m_dwStockPropMask & STOCKPROP_BACKCOLOR)
	{
		if (dwPersistMask & STOCKPROP_BACKCOLOR)
			PX_Color(pPX, _T("BackColor"), m_clrBackColor, AmbientBackColor());
		else if (bLoading)
			m_clrBackColor = AmbientBackColor();
	}

	if (m_dwStockPropMask & STOCKPROP_FONT)
	{
		LPFONTDISP pFontDispAmbient = AmbientFont();
		BOOL bChanged = TRUE;

		if (dwPersistMask & STOCKPROP_FONT)
			bChanged = PX_Font(pPX, _T("Font"), m_font, NULL, pFontDispAmbient);
		else if (bLoading)
			m_font.InitializeFont(NULL, pFontDispAmbient);

		if (bLoading && bChanged)
			OnFontChanged();

		RELEASE(pFontDispAmbient);
	}

	if (m_dwStockPropMask & STOCKPROP_BORDERSTYLE)
	{
		short sBorderStyle = m_sBorderStyle;

		if (dwPersistMask & STOCKPROP_BORDERSTYLE)
			PX_Short(pPX, _T("BorderStyle"), m_sBorderStyle, 0);
		else if (bLoading)
			m_sBorderStyle = 0;

		if (sBorderStyle != m_sBorderStyle)
			RecreateControlWindow();
	}

	if (m_dwStockPropMask & STOCKPROP_ENABLED)
	{
		BOOL bEnabled = m_bEnabled;

		if (dwPersistMask & STOCKPROP_ENABLED)
			PX_Bool(pPX, _T("Enabled"), m_bEnabled, TRUE);
		else if (bLoading)
			m_bEnabled = TRUE;

		if ((bEnabled != m_bEnabled) && (m_hWnd != NULL))
			EnableWindow(m_bEnabled);
	}
}


/////////////////////////////////////////////////////////////////////////////
// Get/Set/OnChanged functions for stock properties

//  BackColor property

OLE_COLOR COleControl::GetBackColor()
{
	return m_clrBackColor;
}

void COleControl::SetBackColor(OLE_COLOR clrBackColor)
{
	// Is the property changing?
	if (m_clrBackColor == clrBackColor)
		return;

	COLORREF cr;
	if (FAILED(::OleTranslateColor(clrBackColor, NULL, &cr)))
		ThrowError(CTL_E_INVALIDPROPERTYVALUE,
			AFX_IDP_E_INVALIDPROPERTYVALUE);

	if (! BoundPropertyRequestEdit(DISPID_BACKCOLOR))
		SetNotPermitted();

	m_clrBackColor = clrBackColor;
	m_bModified = TRUE;
	OnBackColorChanged();

	BoundPropertyChanged(DISPID_BACKCOLOR);
}

void COleControl::OnBackColorChanged()
{
	//  Can be overridden by subclass
	InvalidateControl();
}

//  BorderStyle property

short COleControl::GetBorderStyle()
{
	return m_sBorderStyle;
}

void COleControl::SetBorderStyle(short sBorderStyle)
{
	// Is the property changing?
	if (m_sBorderStyle == sBorderStyle)
		return;

	if (! BoundPropertyRequestEdit(DISPID_BORDERSTYLE))
		SetNotPermitted();

	m_sBorderStyle = sBorderStyle;
	m_bModified = TRUE;
	RecreateControlWindow();
	OnBorderStyleChanged();

	BoundPropertyChanged(DISPID_BORDERSTYLE);
}

void COleControl::OnBorderStyleChanged()
{
	//  Can be overridden by subclass
	InvalidateControl();
}

//  Text and Caption properties

const CString& COleControl::InternalGetText()
{
	if (m_hWnd != NULL)
	{
		if (GetWindowTextLength() > 0)
			GetWindowText(m_strText);
		else
			m_strText = _T("");
	}

	return m_strText;
}


BSTR COleControl::GetText()
{
#if defined(WIN32)
	CString sTemp;
	sTemp = InternalGetText();

	return sTemp.AllocSysString();
#else
	return SysAllocString(InternalGetText());
#endif
}

void COleControl::SetText(LPCTSTR pszText)
{
	TCHAR szText[256];

	if (pszText == NULL)
	{
		_tcscpy(szText, _T(""));
	}
	else
	{
		ASSERT(AfxIsValidString(pszText));

		_tcsncpy(szText, pszText, 255); // Truncate to 255 characters
		szText[255] = (TCHAR)'\0';
	}

	LRESULT lResult;
	if (m_hWnd != NULL)
		lResult = SendMessage(WM_SETTEXT, 0, (LPARAM)(LPTSTR)szText);
	else
		lResult = OnSetText(0, (LPARAM)szText);

	if (lResult == -1)
		SetNotSupported();
}

LRESULT COleControl::OnSetText(WPARAM wParam, LPARAM lParam)
{
	ASSERT((LPCTSTR)lParam != NULL);
	ASSERT(AfxIsValidString((LPCTSTR)lParam));

	// Is the property changing?
	if (m_strText == (LPCTSTR)lParam)
		return 0;

	if (m_dwStockPropMask & STOCKPROP_CAPTION)
		if (! BoundPropertyRequestEdit(DISPID_CAPTION))
			return -1;

	if (m_dwStockPropMask & STOCKPROP_TEXT)
		if (! BoundPropertyRequestEdit(DISPID_TEXT))
			return -1;

	LRESULT lResult = 0;
	m_strText = (LPCTSTR)lParam;
	m_bModified = TRUE;

	if (m_hWnd != NULL)
		lResult = CWnd::DefWindowProc(WM_SETTEXT, wParam, lParam);

	OnTextChanged();

	if (m_dwStockPropMask & STOCKPROP_CAPTION)
		BoundPropertyChanged(DISPID_CAPTION);

	if (m_dwStockPropMask & STOCKPROP_TEXT)
		BoundPropertyChanged(DISPID_TEXT);

	return lResult;
}

void COleControl::OnTextChanged()
{
	//  Can be overridden by subclass
	InvalidateControl();
}

//  Enabled property

BOOL COleControl::GetEnabled()
{
	return m_bEnabled;
}

void COleControl::SetEnabled(BOOL bEnabled)
{
	// Is the property changing?
	if (m_bEnabled == bEnabled)
		return;

	if (! BoundPropertyRequestEdit(DISPID_ENABLED))
		SetNotPermitted();

	m_bEnabled = bEnabled;
	m_bModified = TRUE;
	if (m_hWnd != NULL)
		EnableWindow(m_bEnabled);
	OnEnabledChanged();

	BoundPropertyChanged(DISPID_ENABLED);
}

void COleControl::OnEnabledChanged()
{
	//  Can be overridden by subclass
	InvalidateControl();
}

//  Font property

CFontHolder& COleControl::InternalGetFont()
{
	return m_font;
}

LPFONTDISP COleControl::GetFont()
{
	return m_font.GetFontDispatch();
}

void COleControl::SetFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) ||
		   AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	m_font.InitializeFont(NULL, pFontDisp);
	m_bModified = TRUE;
	OnFontChanged();

	BoundPropertyChanged(DISPID_FONT);
}

void COleControl::OnFontChanged()
{
	//  Can be overridden by subclass

	// Send WM_SETFONT to control's window
	if ((m_hWnd != NULL) &&
		(m_dwStockPropMask & STOCKPROP_FONT) &&
		(GetSuperWndProcAddr() != CWnd::GetSuperWndProcAddr()))
	{
		HFONT hFontPrev = (HFONT)SendMessage(WM_GETFONT, 0, 0);

		CFontHolder& font = InternalGetFont();

		if (font.m_pFont != NULL)
		{
			HFONT hFont = font.GetFontHandle();
			font.m_pFont->AddRefHfont(hFont);
			SendMessage(WM_SETFONT, (WPARAM)hFont, 0);

			if (m_hFontPrev != NULL)
			{
				ASSERT(hFontPrev == m_hFontPrev);
				font.m_pFont->ReleaseHfont(hFontPrev);
			}
			m_hFontPrev = hFont;
		}
		else
		{
			SendMessage(WM_SETFONT, NULL, 0);
			m_hFontPrev = NULL;
		}
	}

	// Invalidate the control
	InvalidateControl();
}

//  ForeColor property

OLE_COLOR COleControl::GetForeColor()
{
	return m_clrForeColor;
}

void COleControl::SetForeColor(OLE_COLOR clrForeColor)
{
	// Is the property changing?
	if (m_clrForeColor == clrForeColor)
		return;

	COLORREF cr;
	if (FAILED(::OleTranslateColor(clrForeColor, NULL, &cr)))
		ThrowError(CTL_E_INVALIDPROPERTYVALUE,
			AFX_IDP_E_INVALIDPROPERTYVALUE);

	if (! BoundPropertyRequestEdit(DISPID_FORECOLOR))
		SetNotPermitted();

	m_clrForeColor = clrForeColor;
	m_bModified = TRUE;
	OnForeColorChanged();

	BoundPropertyChanged(DISPID_FORECOLOR);
}

void COleControl::OnForeColorChanged()
{
	//  Can be overridden by subclass
	InvalidateControl();
}

//  hWnd property

OLE_HANDLE COleControl::GetHwnd()
{
	return (OLE_HANDLE)((m_bInPlaceActive || m_bOpen) ? m_hWnd : NULL);
}


/////////////////////////////////////////////////////////////////////////////
// COleControl::XFontNotification


STDMETHODIMP_(ULONG) COleControl::XFontNotification::AddRef()
{
	//  Delegate to our exported AddRef.
	METHOD_MANAGE_STATE(COleControl, FontNotification)
	return 1;
}


STDMETHODIMP_(ULONG) COleControl::XFontNotification::Release()
{
	//  Delegate to our exported Release.
	METHOD_MANAGE_STATE(COleControl, FontNotification)
	return 0;
}


STDMETHODIMP COleControl::XFontNotification::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_MANAGE_STATE(COleControl, FontNotification)
	ASSERT(ppvObj != NULL);

	if (IsEqualIID(iid, IID_IUnknown) ||
		IsEqualIID(iid, IID_IPropertyNotifySink))
	{
		*ppvObj = this;
		return NOERROR;
	}

	return ResultFromScode(E_NOINTERFACE);
}


STDMETHODIMP COleControl::XFontNotification::OnChanged(DISPID)
{
	METHOD_MANAGE_STATE(COleControl, FontNotification)
	pThis->OnFontChanged();
	pThis->m_bModified = TRUE;

	pThis->BoundPropertyChanged(DISPID_FONT);

	return NOERROR;
}

STDMETHODIMP COleControl::XFontNotification::OnRequestEdit(DISPID)
{
	METHOD_MANAGE_STATE(COleControl, FontNotification)
	return NOERROR;
}


CFont* COleControl::SelectStockFont(CDC* pDC)
{
	return SelectFontObject(pDC, m_font);
}


CFont* COleControl::SelectFontObject(CDC* pDC, CFontHolder& fontHolder)
{
	return fontHolder.Select(pDC, m_rcBounds.Height(), m_cyExtent);
}

void COleControl::GetStockTextMetrics(LPTEXTMETRIC lptm)
{
	m_font.QueryTextMetrics(lptm);
}

void COleControl::GetFontTextMetrics(LPTEXTMETRIC lptm, CFontHolder& fontHolder)
{
	fontHolder.QueryTextMetrics(lptm);
}


/////////////////////////////////////////////////////////////////////////////
// Ambient property access

BOOL COleControl::GetAmbientProperty(DISPID dwDispID, VARTYPE vtProp, void* pvProp)
{
	BOOL bSuccess = FALSE;

	if (m_ambientDispDriver.m_lpDispatch != NULL)
	{
		TRY
		{
			m_ambientDispDriver.GetProperty(dwDispID, vtProp, pvProp);
			bSuccess = TRUE;
		}
		END_TRY
	}

	return bSuccess;
}

OLE_COLOR COleControl::AmbientBackColor()
{
	OLE_COLOR clrBackColor;
	if (! GetAmbientProperty(DISPID_AMBIENT_BACKCOLOR, VT_COLOR, &clrBackColor))
		clrBackColor = GetSysColor(COLOR_WINDOW);
	return clrBackColor;
}

CString COleControl::AmbientDisplayName()
{
	CString strDisplayName;
	GetAmbientProperty(DISPID_AMBIENT_DISPLAYNAME, VT_BSTR, &strDisplayName);
	return strDisplayName;
}

LPFONTDISP COleControl::AmbientFont()
{
	//
	//  Note: Caller MUST Release the font!
	//

	LPFONTDISP pDisp;
	if (! GetAmbientProperty(DISPID_AMBIENT_FONT, VT_DISPATCH, &pDisp))
		pDisp = NULL;
	return pDisp;
}

OLE_COLOR COleControl::AmbientForeColor()
{
	OLE_COLOR clrForeColor;
	if (! GetAmbientProperty(DISPID_AMBIENT_FORECOLOR, VT_COLOR, &clrForeColor))
		clrForeColor = GetSysColor(COLOR_WINDOWTEXT);
	return clrForeColor;
}

LCID COleControl::AmbientLocaleID()
{
	LCID lcid;
	if (! GetAmbientProperty(DISPID_AMBIENT_LOCALEID, VT_I4, &lcid))
		lcid = 0;
	return lcid;
}

CString COleControl::AmbientScaleUnits()
{
	CString strScaleUnits;
	GetAmbientProperty(DISPID_AMBIENT_SCALEUNITS, VT_BSTR, &strScaleUnits);
	return strScaleUnits;
}

short COleControl::AmbientTextAlign()
{
	short iTextAlign;
	if (! GetAmbientProperty(DISPID_AMBIENT_LOCALEID, VT_I2, &iTextAlign))
		iTextAlign = 0;
	return iTextAlign;
}

BOOL COleControl::AmbientUserMode()
{
	BOOL bUserMode;
	if (! GetAmbientProperty(DISPID_AMBIENT_USERMODE, VT_BOOL, &bUserMode))
		bUserMode = TRUE;
	return bUserMode;
}

BOOL COleControl::AmbientUIDead()
{
	BOOL bUIDead;
	if (! GetAmbientProperty(DISPID_AMBIENT_UIDEAD, VT_BOOL, &bUIDead))
		bUIDead = FALSE;
	return bUIDead;
}

BOOL COleControl::AmbientShowGrabHandles()
{
	BOOL bShowGrab;
	if (! GetAmbientProperty(DISPID_AMBIENT_SHOWGRABHANDLES, VT_BOOL, &bShowGrab))
		bShowGrab = TRUE;
	return bShowGrab;
}

BOOL COleControl::AmbientShowHatching()
{
	BOOL bShowHatch;
	if (! GetAmbientProperty(DISPID_AMBIENT_SHOWHATCHING, VT_BOOL, &bShowHatch))
		bShowHatch = TRUE;
	return bShowHatch;
}


/////////////////////////////////////////////////////////////////////////////
// Placeholder functions for read-only or write-only properties

void COleControl::SetNotSupported()
{
	ThrowError(CTL_E_SETNOTSUPPORTED, AFX_IDP_SET_NOT_SUPPORTED);
}

void COleControl::GetNotSupported()
{
	ThrowError(CTL_E_GETNOTSUPPORTED, AFX_IDP_SET_NOT_SUPPORTED);
}


/////////////////////////////////////////////////////////////////////////////
// COleControl::XPerPropertyBrowsing

STDMETHODIMP_(ULONG) COleControl::XPerPropertyBrowsing::AddRef()
{
	//  Delegate to our exported AddRef.
	METHOD_MANAGE_STATE(COleControl, PerPropertyBrowsing)
	return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) COleControl::XPerPropertyBrowsing::Release()
{
	//  Delegate to our exported Release.
	METHOD_MANAGE_STATE(COleControl, PerPropertyBrowsing)
	return (ULONG)pThis->ExternalRelease();
}


STDMETHODIMP COleControl::XPerPropertyBrowsing::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	//  Delegate to our exported QueryInterface.
	METHOD_MANAGE_STATE(COleControl, PerPropertyBrowsing)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP COleControl::XPerPropertyBrowsing::GetDisplayString(
	DISPID dispid, BSTR FAR* lpbstr)
{
	METHOD_MANAGE_STATE(COleControl, PerPropertyBrowsing)

	ASSERT_NULL_OR_POINTER(lpbstr, BSTR);

	CString strValue;
	BOOL bSuccess = pThis->OnGetDisplayString(dispid, strValue);

	if (lpbstr != NULL)
		*lpbstr = (bSuccess ? strValue.AllocSysString() : NULL);

	return bSuccess ? NOERROR : ResultFromScode(S_FALSE);
}


BOOL COleControl::OnGetDisplayString(DISPID dispid, CString& strValue)
{
	TRY
	{
		switch (dispid)
		{
		case DISPID_FONT:
			return m_font.GetDisplayString(strValue);
		}
	}
	END_TRY

	return FALSE;
}


STDMETHODIMP COleControl::XPerPropertyBrowsing::MapPropertyToPage(
	DISPID dispid, LPCLSID lpclsid)
{
	METHOD_MANAGE_STATE(COleControl, PerPropertyBrowsing)

	ASSERT_NULL_OR_POINTER(lpclsid, CLSID);

	CLSID clsid = GUID_NULL;
	BOOL bPageOptional = FALSE;
	BOOL bSuccess = pThis->OnMapPropertyToPage(dispid, &clsid, &bPageOptional);

	if (lpclsid != NULL)
		*lpclsid = (bSuccess ? clsid : GUID_NULL);

	return ResultFromScode(bSuccess ? (bPageOptional ? S_OK : S_FALSE) :
			PERPROP_E_NOPAGEAVAILABLE);
}


BOOL COleControl::OnMapPropertyToPage(DISPID dispid, LPCLSID lpclsid,
	BOOL* pbPageOptional)
{
	switch (dispid)
	{
	case DISPID_FONT:
		*lpclsid = CLSID_CFontPropPage;
		*pbPageOptional = TRUE;
		return TRUE;

	case DISPID_BACKCOLOR:
	case DISPID_FORECOLOR:
		*lpclsid = CLSID_CColorPropPage;
		*pbPageOptional = TRUE;
		return TRUE;
	}

	return FALSE;
}


STDMETHODIMP COleControl::XPerPropertyBrowsing::GetPredefinedStrings(
	DISPID dispid, CALPOLESTR FAR* lpcaStringsOut, CADWORD FAR* lpcaCookiesOut)
{
	METHOD_MANAGE_STATE(COleControl, PerPropertyBrowsing)

	ASSERT_POINTER(lpcaStringsOut, CALPOLESTR);
	ASSERT_POINTER(lpcaCookiesOut, CADWORD);

	CStringArray stringArray;
	CDWordArray cookieArray;

	BOOL bSuccess = pThis->OnGetPredefinedStrings(dispid, &stringArray,
		&cookieArray);

	if (bSuccess)
	{
		// Allocate and fill arrays to return.

		ASSERT(stringArray.GetSize() == cookieArray.GetSize());

		int iElem = 0;
		LPTSTR lpszCopy;
		ULONG cElems = stringArray.GetSize();

		lpcaStringsOut->pElems = (LPOLESTR FAR*)_AfxCtlMalloc(
			sizeof(LPOLESTR) * cElems);

		if (lpcaStringsOut->pElems == NULL)
			return ResultFromScode(E_OUTOFMEMORY);

		lpcaCookiesOut->pElems = (DWORD FAR*)_AfxCtlMalloc(
			sizeof(DWORD FAR*) * cElems);

		if (lpcaCookiesOut->pElems == NULL)
		{
			_AfxCtlFree(lpcaStringsOut->pElems);
			return ResultFromScode(E_OUTOFMEMORY);
		}

		lpcaStringsOut->cElems = cElems;
		lpcaCookiesOut->cElems = cElems;

		for (iElem = 0; iElem < (int)cElems; iElem++)
		{
			lpszCopy = _AfxCtlCopyString(
							(LPTSTR)(LPCTSTR)(stringArray.GetAt(iElem)),
							NULL);

			if (lpszCopy == NULL)
			{
				// Cleanup everything allocated so far...

				while (--iElem >= 0)
					_AfxCtlFreeString(lpcaStringsOut->pElems[iElem], NULL);

				_AfxCtlFree(lpcaCookiesOut->pElems);
				_AfxCtlFree(lpcaStringsOut->pElems);

				return ResultFromScode(E_OUTOFMEMORY);
			}

			lpcaStringsOut->pElems[iElem] = (LPOLESTR)lpszCopy;
			lpcaCookiesOut->pElems[iElem] = cookieArray.GetAt(iElem);
		}
	}

	return bSuccess ? NOERROR : ResultFromScode(S_FALSE);
}

BOOL COleControl::OnGetPredefinedStrings(DISPID dispid,
	CStringArray* pStringArray, CDWordArray* pCookieArray)
{
	UNUSED dispid;
	UNUSED pStringArray;
	UNUSED pCookieArray;

	// May be overridden by subclass.
	return FALSE;
}

STDMETHODIMP COleControl::XPerPropertyBrowsing::GetPredefinedValue(
	DISPID dispid, DWORD dwCookie, VARIANT FAR* lpvarOut)
{
	METHOD_MANAGE_STATE(COleControl, PerPropertyBrowsing)

	ASSERT_POINTER(lpvarOut, VARIANT);

	return pThis->OnGetPredefinedValue(dispid, dwCookie, lpvarOut) ?
		NOERROR :
		ResultFromScode(E_FAIL);
}

BOOL COleControl::OnGetPredefinedValue(DISPID dispid, DWORD dwCookie,
	VARIANT FAR* lpvarOut)
{
	UNUSED dispid;
	UNUSED dwCookie;
	UNUSED lpvarOut;

	// May be overridden by subclass.
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
