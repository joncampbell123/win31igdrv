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

#ifdef AFXCTL_PAGE_SEG
#pragma code_seg(AFXCTL_PAGE_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW


struct AFX_DDPDATA : public CObject
{
	AFX_DDPDATA(LPVOID lpHandler, int nCtrlId, LPVOID lpMember,
		UINT nType, LPCTSTR lpszOleName);

	LPVOID  m_lpHandler;
	int     m_nCtrlId;
	LPVOID  m_lpMember;
	UINT    m_nType;
	LPCTSTR m_lpszOleName;
};


AFX_DDPDATA::AFX_DDPDATA(LPVOID lpHandler, int nCtrlId, LPVOID lpMember,
					UINT nType, LPCTSTR lpszOleName ) :
	m_lpHandler(lpHandler),
	m_nCtrlId(nCtrlId),
	m_lpMember(lpMember),
	m_nType(nType),
	m_lpszOleName(lpszOleName)
{
}


#define MAX_CLASS_NAME  100

struct NotifyInfo
{
	LPCTSTR     szClassName;        // Name of control's class
	WORD        wNotifyCode;        // Notification code
};

#define MAX_TEXT    1024


#ifdef _WIN32
struct DialogBoxHeader { // From MSDN Article "Win32 Binary Resource Formats"
	DWORD   lStyle;
	DWORD   lExtendedStyle;
	WORD    bNumberOfItems;
	WORD    x, y;
	WORD    cx, cy;
	WCHAR   szMenuName;
	// Var length "WCHAR []" or "{ (WORD)-1, (WORD)MAKEINTRESOURCE() }" records
};
#else
struct DialogBoxHeader {
	LONG    lStyle;
	BYTE    bNumberOfItems;
	SHORT   x, y;
	SHORT   cx, cy;
	char    szMenuName;
	// Var length "char []" or "{ (char)-1, (WORD)MAKEINTRESOURCE() }" records
};
#endif

BEGIN_INTERFACE_MAP(COlePropertyPage, CDialog)
	INTERFACE_PART(COlePropertyPage, IID_IPropertyPage2, PropertyPage)
	INTERFACE_PART(COlePropertyPage, IID_IPropertyPage, PropertyPage)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ConvertDialogUnitsToPixels

static void ConvertDialogUnitsToPixels(LPCTSTR pszFontFace, WORD wFontSize,
	int cxDlg, int cyDlg, SIZE* pSizePixel)
{
	// Attempt to create the font to be used in the dialog box
	UINT cxSysChar, cySysChar;
	LOGFONT lf;
	HDC hDC = ::GetDC(NULL);
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = (SHORT)-MulDiv(wFontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	lf.lfWeight = FW_BOLD;

	_tcscpy((TCHAR *) lf.lfFaceName, pszFontFace);

	// try to create the new font, return FALSE if not possible, otherwise keep
	// pointer to the old font and select the new one

	HFONT hNewFont = CreateFontIndirect(&lf);
	if (hNewFont != NULL)
	{

		HFONT hFontOld = (HFONT) SelectObject(hDC, hNewFont);

		// Calculate the average character width and height
		//
		// If it's a variable pitch font, calculate the average pitch ourselves
		// rather than using the average char width windows provides -- don't know
		// exactly why we do this, but dlgedit did, so we do!   - ggh 06/22/92

		TEXTMETRIC tm;
		GetTextMetrics(hDC, &tm);
		cySysChar = tm.tmHeight;

		if (tm.tmPitchAndFamily & 0x01)
		{
			int i;                                              // counter
			TCHAR szAveCharWidth[52];        // array to hold A-Z and a-z

			for (i = 0; i < 26; i++)
			{
				szAveCharWidth[i] = (TCHAR)(i + 'a');
				szAveCharWidth[i + 26] = (TCHAR)(i + 'A');
			}
#ifdef _WIN32
			SIZE sizeExtent;
			GetTextExtentPoint(hDC, szAveCharWidth, 52, &sizeExtent);
			cxSysChar = sizeExtent.cx / 26;
#else
			cxSysChar = LOWORD( GetTextExtent( hDC, szAveCharWidth, 52 )) / 26;
#endif
			cxSysChar = (cxSysChar + 1) / 2;
		}
		else
		{
			cxSysChar = tm.tmAveCharWidth;
		}

		SelectObject(hDC, hFontOld);
		DeleteObject(hNewFont);
	}
	else
	{
		// Could not create the font so just use the system's values
		cxSysChar = LOWORD(GetDialogBaseUnits());
		cySysChar = HIWORD(GetDialogBaseUnits());
	}

	::ReleaseDC(NULL, hDC);

	//  Translate dialog units to pixels
	pSizePixel->cx = MulDiv(cxDlg, cxSysChar, 4);
	pSizePixel->cy = MulDiv(cyDlg, cySysChar, 8);
}

/////////////////////////////////////////////////////////////////////////////
// GetDialogSize

static void GetDialogSize(HGLOBAL hDialog, SIZE* pSize)
{
	ASSERT_POINTER(pSize, SIZE);

	UINT cx, cy;

	// Set the default dialog font face and size
	TCHAR szFontFace[128] = _T("System");
	WORD wFontSize = 8;

	// Set a default page size just in case we
	// cannot obtain the page size from the dialog
	// resource template
	cx = 250;
	cy = 62;

	if (hDialog != NULL)
	{
		// Attempt to lock the resource
		DialogBoxHeader FAR* pDlg = (DialogBoxHeader FAR*)LockResource(hDialog);
		if (pDlg != NULL)
		{
			// We managed to load the dialog resource template so now get
			// the size of the dialog box
			cx = pDlg->cx;
			cy = pDlg->cy;

#ifdef _WIN32
			// Skip over the menu name, classname, and caption fields
			// In WIN32, all resources string are stored in UNICODE WCHARs
			WORD * pb = (WORD *)&pDlg->szMenuName;

			// Skip past the menu name or menu resource id
			if (*pb == (WORD)-1 )
				pb += 2;
			else
				while( *pb++ );

			// Skip past the class name or class resource id
			if (*pb == (WORD)-1 )
				pb += 2; // WORDS
			else
				while( *pb++ );

			// Skip past the Dialog szCaption (WCHAR)
			while (*pb++);

			// Does the dialog resource template contain font information
			if (pDlg->lStyle & DS_SETFONT)
			{
				// It does contain font information so extract it
				wFontSize = *pb++;
#ifdef _UNICODE
				// Copy over the WIDE char font facename
				_tcscpy( szFontFace, pb );
#else
				// Convert the WIDE char font face to MBCS
				BOOL bUsedDefaultChar = FALSE;
				WideCharToMultiByte( CP_ACP, 0 /*??*/,
									 (LPCWSTR)pb,
									 wcslen((LPCWSTR)pb),
									 (LPSTR)szFontFace,
									 min(wcslen((LPCWSTR)pb),
										 sizeof(szFontFace) ),
									 " ", (LPBOOL)&bUsedDefaultChar );

#endif
			}
#else
			// Skip over the menu name, classname, and caption fields
			BYTE *pb = (BYTE *)&pDlg->szMenuName;

			if (*((BYTE *) pb) == (BYTE)-1)
				pb += 3; // BYTS
			else
				while (*pb++);  // Do Nothing

			while (*pb++);
			while (*pb++);

			// Does the dialog resource template contain font information
			if (pDlg->lStyle & DS_SETFONT)
			{
				// It does contain font information so extract it
				wFontSize = *(WORD*)pb;
				pb += sizeof(WORD);
				_tcscpy( szFontFace, (TCHAR*)pb );
			}
#endif

			// Unlock the resource
			UnlockResource(hDialog);
		}
	}

	ConvertDialogUnitsToPixels(szFontFace, wFontSize, cx, cy, pSize);
}

/////////////////////////////////////////////////////////////////////////////
// COlePropertyPage::COlePropertyPage

COlePropertyPage::COlePropertyPage(UINT idDlg, UINT idCaption) :
	m_bDirty(FALSE),
	m_idCaption(idCaption),
	m_idDlg(idDlg),
	m_pPageSite(NULL),
	m_ppDisp(NULL),
	m_pAdvisors(NULL),
	m_bPropsChanged(FALSE),
	m_nObjects(0),
	m_bInitializing(TRUE),
	m_nControls(0),
	m_pStatus(NULL),
	m_hDialog(NULL),
	m_dwHelpContext(0)
{
	m_pModuleState = _afxModuleAddrCurrent;

	// m_lpDialogTemplate is needed later by CWnd::ExecuteDlgInit
	m_lpDialogTemplate = MAKEINTRESOURCE(m_idDlg);

	// Keep this DLL loaded at least until this object is deleted.
	AfxOleLockApp();
}

void COlePropertyPage::OnSetPageSite()
{
	// Load the caption from resources
	CString strCaption;
	if (! strCaption.LoadString(m_idCaption))
		strCaption.LoadString(AFX_IDS_PROPPAGE_UNKNOWN);
	SetPageName(strCaption);

	// Try to load the dialog resource template into memory and get its size
	m_sizePage.cx = 0;
	m_sizePage.cy = 0;
	HINSTANCE hInstRes = AfxFindResourceHandle(MAKEINTRESOURCE(m_idDlg),
		RT_DIALOG);
	HRSRC hRes = ::FindResource(hInstRes, MAKEINTRESOURCE(m_idDlg), RT_DIALOG);
	if (hRes != NULL)
	{
		SetDialogResource(::LoadResource(hInstRes, hRes));
	}
}

BOOL COlePropertyPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// If we're running on a DBCS-enabled system, use the System font,
	// to guarantee that all characters can be displayed.
	if (GetSystemMetrics(SM_DBCSENABLED))
	{
		HFONT hFontSystem = (HFONT)GetStockObject(SYSTEM_FONT);
		SendMessage(WM_SETFONT, (WPARAM)hFontSystem, 0);
		HWND hWnd = ::GetTopWindow(m_hWnd);
		while (hWnd != NULL)
		{
			::SendMessage(hWnd, WM_SETFONT, (WPARAM)hFontSystem, 0);
			hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT);
		}
	}

	return 0;
}

BOOL COlePropertyPage::PreTranslateMessage(LPMSG lpMsg)
{
	// Don't let CDialog process the Return key or Escape key.
	if ((lpMsg->message == WM_KEYDOWN) &&
		((lpMsg->wParam == VK_RETURN) || (lpMsg->wParam == VK_ESCAPE)))
	{
		// Special case: if control with focus is an edit control with
		// ES_WANTRETURN style, let it handle the Return key.

		TCHAR szClass[10];
		CWnd* pWndFocus = GetFocus();
		if ((lpMsg->wParam == VK_RETURN) &&
			((pWndFocus = GetFocus()) != NULL) &&
			IsChild(pWndFocus) &&
			(pWndFocus->GetStyle() & ES_WANTRETURN) &&
			GetClassName(pWndFocus->m_hWnd, szClass, 10) &&
			(_tcsicmp(szClass, _T("EDIT")) == 0))
		{
			pWndFocus->SendMessage(WM_CHAR, lpMsg->wParam, lpMsg->lParam);
			return TRUE;
		}

		return FALSE;
	}

	// If it's a WM_SYSKEYDOWN, temporarily replace the hwnd in the
	// message with the hwnd of our first control, and try to handle
	// the message for ourselves.

	BOOL bHandled;

	if ((lpMsg->message == WM_SYSKEYDOWN) && !::IsChild(m_hWnd, lpMsg->hwnd))
	{
		HWND hWndSave = lpMsg->hwnd;
		lpMsg->hwnd = ::GetWindow(m_hWnd, GW_CHILD);
		bHandled = CDialog::PreTranslateMessage(lpMsg);
		lpMsg->hwnd = hWndSave;
	}
	else
	{
		bHandled = CDialog::PreTranslateMessage(lpMsg);
	}

	return bHandled;
}

void COlePropertyPage::CleanupObjectArray()
{
	if (m_pAdvisors)
	{
		for (ULONG nObject = 0; nObject < m_nObjects; nObject++)
			AfxConnectionUnadvise(m_ppDisp[nObject], IID_IPropertyNotifySink,
				&m_xPropNotifySink, FALSE, m_pAdvisors[nObject]);
		delete [] m_pAdvisors;
		m_pAdvisors = NULL;
	}
	if (m_ppDisp)
	{
		for (ULONG nObject = 0; nObject < m_nObjects; nObject++)
			RELEASE(m_ppDisp[nObject]);
		delete [] m_ppDisp;
		m_ppDisp = NULL;
	}
}


COlePropertyPage::~COlePropertyPage()
{
	// Remember to free the resource we loaded!
	if (m_hDialog != NULL)
		FreeResource(m_hDialog);

	if (m_pStatus != NULL)
	{
		delete [] m_pStatus;
		m_pStatus = NULL;
	}

	RELEASE(m_pPageSite);
	CleanupObjectArray();
	AfxOleUnlockApp();
}

void COlePropertyPage::OnFinalRelease()
{
	if (m_hWnd != NULL)
		DestroyWindow();

	delete this;
}

LRESULT COlePropertyPage::WindowProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	AFX_MANAGE_STATE(m_pModuleState);

	// Forward WM_SYSCOMMAND messages to the frame for translation
	if ((msg == WM_SYSCOMMAND) && (m_pPageSite != NULL))
	{
		if (m_pPageSite->TranslateAccelerator((LPMSG)GetCurrentMessage())
			== NOERROR)
		{
			return 0;
		}
	}

	return CDialog::WindowProc(msg, wParam, lParam);
}

BOOL COlePropertyPage::OnHelp(LPCTSTR)
{
	// May be overridden by subclass.
	return FALSE;
}

BOOL COlePropertyPage::OnEditProperty(DISPID)
{
	// May be overridden by subclass.
	return FALSE;
}

void COlePropertyPage::OnObjectsChanged()
{
	// May be overridden by subclass.
}

LPDISPATCH FAR* COlePropertyPage::GetObjectArray(ULONG FAR* pnObjects)
{
	ASSERT_POINTER(pnObjects, ULONG);

	if (pnObjects != NULL)
		*pnObjects = m_nObjects;

	return m_ppDisp;
}

void COlePropertyPage::SetModifiedFlag(BOOL bModified)
{
	if ((m_bDirty && !bModified) || (!m_bDirty && bModified))
	{
		m_bDirty = bModified;

		if (m_pPageSite != NULL)
		{
			DWORD flags = 0;
			if (bModified)
				flags |= PROPPAGESTATUS_DIRTY;

			m_pPageSite->OnStatusChange(flags);
		}
	}
}

BOOL COlePropertyPage::IsModified()
{
	return m_bDirty;
}

void COlePropertyPage::SetPageName(LPCTSTR lpszPageName)
{
	ASSERT(AfxIsValidString(lpszPageName));
	m_strPageName = lpszPageName;
}

void COlePropertyPage::SetDialogResource(HGLOBAL hDialog)
{
	m_hDialog = hDialog;
	GetDialogSize(m_hDialog, &m_sizePage);
}

void COlePropertyPage::SetHelpInfo(LPCTSTR lpszDocString,
	LPCTSTR lpszHelpFile, DWORD dwHelpContext)
{
	ASSERT((lpszDocString == NULL) || AfxIsValidString(lpszDocString));
	ASSERT((lpszHelpFile == NULL) || AfxIsValidString(lpszHelpFile));

	m_strDocString = lpszDocString;
	m_strHelpFile = lpszHelpFile;
	m_dwHelpContext = dwHelpContext;
}

LPPROPERTYPAGESITE COlePropertyPage::GetPageSite()
{
	return m_pPageSite;
}

#ifndef _WIN32
#define AfxGetSafeOwner _AfxGetSafeOwner
#endif

int COlePropertyPage::MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption,
	UINT nType)
{
	// Try to find a better owner than the page itself, which may cause the
	// wrong window to be disabled.

	if (lpszCaption == NULL)
		lpszCaption = AfxGetAppName();
	return ::MessageBox(AfxGetSafeOwner(NULL), lpszText, lpszCaption, nType);
}

/////////////////////////////////////////////////////////////////////////////
// COlePropertyPage::XPropertyPage


STDMETHODIMP_(ULONG) COlePropertyPage::XPropertyPage::AddRef()
{
	// Delegate to our exported AddRef.
	METHOD_MANAGE_STATE(COlePropertyPage, PropertyPage)
	return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) COlePropertyPage::XPropertyPage::Release()
{
	// Delegate to our exported Release.
	METHOD_MANAGE_STATE(COlePropertyPage, PropertyPage)
	return (ULONG)pThis->ExternalRelease();
}


STDMETHODIMP COlePropertyPage::XPropertyPage::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	// Delegate to our exported QueryInterface.
	METHOD_MANAGE_STATE(COlePropertyPage, PropertyPage)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP COlePropertyPage::XPropertyPage::SetPageSite(
	LPPROPERTYPAGESITE pPageSite)
{
	METHOD_MANAGE_STATE(COlePropertyPage, PropertyPage)
	ASSERT_VALID(pThis);
	ASSERT_POINTER(pPageSite, IPropertyPageSite);

	pThis->m_pPageSite = pPageSite;
	pThis->m_pPageSite->AddRef();           // Bump the reference count

	pThis->OnSetPageSite();

	return NOERROR;
}


STDMETHODIMP COlePropertyPage::XPropertyPage::Activate(HWND hWndParent,
	LPCRECT pRect, BOOL)
{
	METHOD_MANAGE_STATE(COlePropertyPage, PropertyPage)
	ASSERT_VALID(pThis);
	ASSERT_NULL_OR_POINTER(pRect, RECT);

	BOOL bSuccess = FALSE;  // Did we successfully create the dialog box

	if (pThis->m_hDialog != NULL)
	{
		// We've already loaded the dialog template into memory so just
		// create it!

		void FAR* lpDialogTemplate = LockResource(pThis->m_hDialog);
		if (lpDialogTemplate != NULL)
		{
			bSuccess = pThis->CreateIndirect(lpDialogTemplate, CWnd::FromHandle(hWndParent));
			UnlockResource(pThis->m_hDialog);
		}
		else
			bSuccess = pThis->Create(pThis->m_idDlg, CWnd::FromHandle(hWndParent));
	}
	else
		bSuccess = pThis->Create(pThis->m_idDlg, CWnd::FromHandle(hWndParent));

	// Were we successful in creating the dialog box!
	if (bSuccess)
	{
		pThis->MoveWindow(pRect);       //  Force page to fill area given by frame *
		pThis->m_bInitializing = TRUE;
		pThis->UpdateData(FALSE);
		pThis->m_bInitializing = FALSE;

		if (pThis->m_pStatus != NULL)
		{
			delete [] pThis->m_pStatus;
			pThis->m_pStatus = NULL;
		}

		pThis->m_nControls = 0;
		::EnumChildWindows(pThis->GetSafeHwnd(), (WNDENUMPROC) COlePropertyPage::EnumChildProc, (LPARAM) pThis);

		if (pThis->m_nControls > 0)
			pThis->m_pStatus = new AFX_PPFIELDSTATUS [UINT(pThis->m_nControls)];

		pThis->m_nControls = 0;
		EnumChildWindows(pThis->GetSafeHwnd(), (WNDENUMPROC) COlePropertyPage::EnumControls, (LPARAM) pThis);

		return NOERROR;
	}

	return ResultFromScode(E_FAIL);
}

BOOL CALLBACK EXPORT COlePropertyPage::EnumChildProc(HWND, LPARAM lParam)
{
	COlePropertyPage* pDlg = (COlePropertyPage*) lParam;
	ASSERT_POINTER(pDlg, COlePropertyPage);

	pDlg->m_nControls++;
	return TRUE;
}

BOOL CALLBACK EXPORT COlePropertyPage::EnumControls(HWND hWnd, LPARAM lParam)
{
	COlePropertyPage* pDlg = (COlePropertyPage*) lParam;
	ASSERT_POINTER(pDlg, COlePropertyPage);
	ASSERT(pDlg->m_pStatus != NULL);

	pDlg->m_pStatus[pDlg->m_nControls].nID = (UINT)::GetDlgCtrlID(hWnd);
	pDlg->m_pStatus[pDlg->m_nControls].bDirty = FALSE;
	pDlg->m_nControls++;

	return TRUE;
}

STDMETHODIMP COlePropertyPage::XPropertyPage::Deactivate()
{
	METHOD_MANAGE_STATE(COlePropertyPage, PropertyPage)
	ASSERT_VALID(pThis);

	pThis->DestroyWindow();

	return NOERROR;
}


static LPTSTR _SafeCopyString(LPCTSTR psz)
{
	if ((psz != NULL) && (_tcslen(psz) > 0))
		return _AfxCtlCopyString((LPTSTR)psz, NULL);

	return NULL;
}


STDMETHODIMP COlePropertyPage::XPropertyPage::GetPageInfo(
	LPPROPPAGEINFO pPageInfo)
{
	METHOD_MANAGE_STATE(COlePropertyPage, PropertyPage)
	ASSERT_VALID(pThis);
	ASSERT_POINTER(pPageInfo, PROPPAGEINFO);

	pPageInfo->pszTitle = _SafeCopyString(pThis->m_strPageName);
	pPageInfo->size = pThis->m_sizePage;
	pPageInfo->pszDocString = _SafeCopyString(pThis->m_strDocString);
	pPageInfo->pszHelpFile = _SafeCopyString(pThis->m_strHelpFile);
	pPageInfo->dwHelpContext = pThis->m_dwHelpContext;
	return NOERROR;
}


STDMETHODIMP COlePropertyPage::XPropertyPage::SetObjects(
		ULONG cObjects, LPUNKNOWN FAR* ppUnk)
{
	METHOD_MANAGE_STATE(COlePropertyPage, PropertyPage)
	ASSERT_VALID(pThis);

	pThis->CleanupObjectArray();

	if (cObjects != 0)
	{
		ASSERT(AfxIsValidAddress(ppUnk, sizeof(LPUNKNOWN) * (int)cObjects, FALSE));

		pThis->m_ppDisp = new LPDISPATCH [(UINT)cObjects];
		pThis->m_pAdvisors = new DWORD [(UINT)cObjects];
		for (ULONG nObject = 0; nObject < cObjects; nObject++)
		{
			HRESULT hr = ppUnk[nObject]->QueryInterface(IID_IDispatch,
				(VOID FAR* FAR*)&(pThis->m_ppDisp[nObject]));
			if (SUCCEEDED(hr))
			{
				AfxConnectionAdvise(ppUnk[nObject], IID_IPropertyNotifySink,
					&pThis->m_xPropNotifySink, FALSE,
					&pThis->m_pAdvisors[nObject]);
			}
			else
			{
				return hr;
			}
		}
	}

	pThis->m_nObjects = cObjects;

	// No painting during the data update.
	BOOL bLock = (pThis->m_hWnd != NULL) && pThis->IsWindowVisible();
	if (bLock)
		::LockWindowUpdate(pThis->m_hWnd);

	pThis->OnObjectsChanged();

	//  If window exists, update the data in its fields.
	if (cObjects != 0 && pThis->m_hWnd != NULL)
		pThis->UpdateData(FALSE);

	if (bLock)
		::LockWindowUpdate(NULL);

	return NOERROR;
}


STDMETHODIMP COlePropertyPage::XPropertyPage::Show(UINT nCmdShow)
{
	METHOD_MANAGE_STATE(COlePropertyPage, PropertyPage)
	ASSERT_VALID(pThis);

	pThis->ShowWindow(nCmdShow);
	if (nCmdShow == SW_SHOWNORMAL)
		pThis->SetFocus();
	return NOERROR;
}


STDMETHODIMP COlePropertyPage::XPropertyPage::Move(LPCRECT pRect)
{
	METHOD_MANAGE_STATE(COlePropertyPage, PropertyPage)
	ASSERT_VALID(pThis);
	ASSERT_POINTER(pRect, RECT);

	pThis->MoveWindow(pRect);
	return NOERROR;
}


STDMETHODIMP COlePropertyPage::XPropertyPage::IsPageDirty()
{
	METHOD_MANAGE_STATE(COlePropertyPage, PropertyPage)
	ASSERT_VALID(pThis);

	return pThis->m_bDirty ? NOERROR : ResultFromScode(S_FALSE);
}


STDMETHODIMP COlePropertyPage::XPropertyPage::Apply()
{
	METHOD_MANAGE_STATE(COlePropertyPage, PropertyPage)
	ASSERT_VALID(pThis);

	if (pThis->UpdateData(TRUE))
	{
		pThis->m_bDirty = FALSE;

		// Set the dirty status of all the controls on this page to false.
		if (pThis->m_pStatus != NULL)
		{
			for (int nControl = 0; nControl < pThis->m_nControls; nControl++)
				pThis->m_pStatus[nControl].bDirty = FALSE;
		}

		return NOERROR;
	}

	return ResultFromScode(E_FAIL);
}

STDMETHODIMP COlePropertyPage::XPropertyPage::Help(LPCTSTR lpszHelpDir)
{
	METHOD_MANAGE_STATE(COlePropertyPage, PropertyPage)
	ASSERT_VALID(pThis);
	ASSERT((lpszHelpDir == NULL) || AfxIsValidString(lpszHelpDir));

	if (!pThis->OnHelp(lpszHelpDir))
		return ResultFromScode(S_FALSE);
	else
		return NOERROR;
}

BOOL _AfxAtEndOfTabList(CDialog* pDlg, UINT nCmd)
{
	if ((pDlg->SendMessage(WM_GETDLGCODE) &
		(DLGC_WANTALLKEYS | DLGC_WANTMESSAGE | DLGC_WANTTAB)) == 0)
	{
		CWnd* pCtl = CWnd::GetFocus();
		if (pDlg->IsChild(pCtl))
		{
			// Get top level child for controls with children, like combo.
			while (pCtl->GetParent() != pDlg)
			{
				pCtl = pCtl->GetParent();
				ASSERT_VALID(pCtl);
			}

			do
			{
				if ((pCtl = pCtl->GetWindow(nCmd)) == NULL)
					return TRUE;
			}
			while ((pCtl->GetStyle() & (WS_DISABLED | WS_TABSTOP)) != WS_TABSTOP);
		}
	}

	return FALSE;
}

STDMETHODIMP COlePropertyPage::XPropertyPage::TranslateAccelerator(LPMSG lpMsg)
{
	METHOD_MANAGE_STATE(COlePropertyPage, PropertyPage)
	ASSERT_VALID(pThis);
	ASSERT_POINTER(lpMsg, MSG);

	BOOL bHandled = FALSE;

	if (lpMsg->message == WM_KEYDOWN && lpMsg->wParam == VK_TAB &&
		GetKeyState(VK_CONTROL) >= 0)
	{
		if (pThis->IsChild(CWnd::GetFocus()))
		{
			// We already have the focus.  Let's determine whether we should
			// pass focus up to the frame.

			if (_AfxAtEndOfTabList(pThis, GetKeyState(VK_SHIFT) < 0 ?
				GW_HWNDPREV : GW_HWNDNEXT))
			{
				// fix for default button border
				DWORD dwDefID = pThis->GetDefID();
				if (HIWORD(dwDefID) == DC_HASDEFID)
				{
					CWnd *pDefBtn = pThis->GetDlgItem(LOWORD(dwDefID));
					if (pDefBtn != NULL && pDefBtn->IsWindowEnabled())
						pThis->GotoDlgCtrl(pDefBtn);
				}

				// Pass focus to the frame by letting the page site handle
				// this message.
				if (pThis->m_pPageSite)
					bHandled =
						pThis->m_pPageSite->TranslateAccelerator(lpMsg) ==
						NOERROR;
			}
		}
		else
		{
			// We don't already have the focus.  The frame is passing the
			// focus to us.

			// Set the focus to the first tabstop in the page.
			if (GetKeyState(VK_SHIFT) >= 0)
				pThis->SetFocus();
			// Set the focus to the last tabstop in the page.
			else
			{
				CWnd* pWnd = pThis->GetTopWindow();
				ASSERT_VALID(pWnd);
				pWnd = pWnd->GetWindow(GW_HWNDLAST);
				while (pWnd != NULL)
				{
					if ((pWnd->GetStyle() & (WS_DISABLED | WS_TABSTOP)) ==
						WS_TABSTOP)
					{
						pThis->GotoDlgCtrl(pWnd);
						break;
					}

					pWnd = pWnd->GetWindow(GW_HWNDPREV);
				}
			}

			// Message was handled here here.
			bHandled = TRUE;
		}
	}

	// If message was not handled here, call PreTranslateMessage
	return (bHandled || pThis->PreTranslateMessage(lpMsg)) ?
		NOERROR :
		ResultFromScode(S_FALSE);
}

STDMETHODIMP COlePropertyPage::XPropertyPage::EditProperty(DISPID dispid)
{
	METHOD_MANAGE_STATE(COlePropertyPage, PropertyPage)
	ASSERT_VALID(pThis);

	return pThis->OnEditProperty(dispid) ? NOERROR : ResultFromScode(E_NOTIMPL);
}

/////////////////////////////////////////////////////////////////////////////
// COlePropertyPage::XPropNotifySink


STDMETHODIMP_(ULONG) COlePropertyPage::XPropNotifySink::AddRef()
{
	return 1;
}


STDMETHODIMP_(ULONG) COlePropertyPage::XPropNotifySink::Release()
{
	return 0;
}


STDMETHODIMP COlePropertyPage::XPropNotifySink::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	if (IsEqualIID(iid, IID_IPropertyNotifySink) ||
		IsEqualIID(iid, IID_IUnknown))
	{
		*ppvObj = this;
		return NOERROR;
	}
	else
	{
		*ppvObj = NULL;
		return ResultFromScode(E_NOINTERFACE);
	}
}


STDMETHODIMP COlePropertyPage::XPropNotifySink::OnRequestEdit(DISPID)
{
	return NOERROR;
}

STDMETHODIMP COlePropertyPage::XPropNotifySink::OnChanged(DISPID)
{
	METHOD_MANAGE_STATE(COlePropertyPage, PropNotifySink)

	// Since UpdateData() might recurse here, use PostMessage
	// to notify ourselves to update the data later
	pThis->m_bPropsChanged = TRUE;
	if (pThis->m_hWnd != NULL)
		pThis->PostMessage(WM_COMMAND, (WPARAM) -1, (LPARAM)0);
	return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// Handle control notifications

#if !defined(ENTRIES)
#define ENTRIES(a)  (sizeof(a)/sizeof(*a))
#endif

static TCHAR BASED_CODE _szEdit[] =     _T("Edit");
static TCHAR BASED_CODE _szButton[] =   _T("Button");
static TCHAR BASED_CODE _szComboBox[] = _T("ComboBox");
static TCHAR BASED_CODE _szListBox[] =  _T("ListBox");

static const NotifyInfo BASED_CODE NotifyList[] = {
	{ _szEdit,      EN_CHANGE },
	{ _szButton,    BN_CLICKED },
	{ _szButton,    BN_DOUBLECLICKED },
	{ _szComboBox,  CBN_EDITCHANGE },
	{ _szComboBox,  CBN_SELCHANGE },
	{ _szComboBox,  CBN_SELENDOK },
	{ _szListBox,   LBN_SELCHANGE },
};

static const NotifyInfo BASED_CODE UpdateList[] = {
	{ _szEdit,      EN_KILLFOCUS },
	{ _szButton,    BN_CLICKED },
	{ _szButton,    BN_DOUBLECLICKED },
	{ _szComboBox,  CBN_SELENDOK },
	{ _szComboBox,  CBN_SELENDCANCEL },
	{ _szListBox,   LBN_SELCHANGE },
};

BOOL _AfxIsRadioButton(HWND hWnd)
{
	DWORD dwButtonStyle = GetWindowStyle(hWnd) & 0x0000000FL;
	return ((dwButtonStyle == BS_RADIOBUTTON) ||
			(dwButtonStyle == BS_AUTORADIOBUTTON));
}

BOOL COlePropertyPage::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Let the base class process the message first
	BOOL bSuccess = CDialog::OnCommand(wParam, lParam);

	// Are we just initializing the dialog box, or do we have no objects?
	if (m_bInitializing || m_ppDisp == NULL)
		return bSuccess;

#if defined(_WIN32)
	// For 32 bit!
	WORD wID = LOWORD(wParam);
	HWND hWndCtl = (HWND)lParam;
	WORD wNotifyCode = HIWORD(wParam);
#else
	// For 16 bit!
	WORD wID = (WORD)wParam;
	HWND hWndCtl = (HWND) LOWORD(lParam);
	WORD wNotifyCode = HIWORD(lParam);
#endif

	// Process special async data changed notification
	if ( wID == (WORD)-1 && !hWndCtl )
	{
		// Sent by COlePropertyPage::XPropNotifySink::OnChanged
		if (m_bPropsChanged)
		{
			UpdateData(FALSE);
			m_bPropsChanged = FALSE;
		}
		return bSuccess;
	}

	DWORD flags = 0;

	if (hWndCtl != NULL)
	{
		BOOL bIgnoreControl = FALSE;
		for (int count = 0; count < m_IDArray.GetSize(); count++)
		{
			WORD wNotID = m_IDArray.GetAt(count);
			if (wID == wNotID)
				bIgnoreControl = TRUE;
		}

		if ( !bIgnoreControl )
		{
			TCHAR szClassName[MAX_CLASS_NAME];

			// We have a control message - check type of control and message
			::GetClassName(hWndCtl, (LPTSTR)szClassName, MAX_CLASS_NAME);

			for (int iNotify = 0; iNotify < ENTRIES(NotifyList); iNotify++)
			{
				if (_tcscmp(NotifyList[iNotify].szClassName, szClassName) == 0
				 && NotifyList[iNotify].wNotifyCode == wNotifyCode )
				{
					if ((_tcscmp(szClassName, _szButton) == 0) &&
						_AfxIsRadioButton(hWndCtl))
					{
						// Special case for radio buttons:
						// mark first button in group

						while ((hWndCtl != NULL) &&
								! (GetWindowStyle(hWndCtl) & WS_GROUP))
						{
							hWndCtl = ::GetWindow(hWndCtl, GW_HWNDPREV);
						}

						// First button in group must have WS_GROUP style,
						// and must be a radio button.
						ASSERT(hWndCtl != NULL);
						ASSERT(_AfxIsRadioButton(hWndCtl));

						// Mark first radio button as dirty
						if (hWndCtl != NULL)
							wID = (WORD)::GetWindowID(hWndCtl);
					}

					// Control has been modified
					m_bDirty = TRUE;
					SetControlStatus(wID, TRUE);

					flags = PROPPAGESTATUS_DIRTY;
					break;
				}
			}

			if (m_bDirty)
			{
				for (int iNotify=0; iNotify < ENTRIES(UpdateList); iNotify++)
				{
					if (_tcscmp(UpdateList[iNotify].szClassName, szClassName)==0 &&
						UpdateList[iNotify].wNotifyCode == wNotifyCode &&
						GetControlStatus(wID))
					{
						flags |= PROPPAGESTATUS_VALIDATE;
					}
				}
			}
		}
	}

	if (flags != 0)
	{
		ASSERT(m_pPageSite != NULL);
		m_pPageSite->OnStatusChange(flags);
	}

	return bSuccess;
}

void COlePropertyPage::IgnoreApply(UINT nID)
{
	m_IDArray.Add((WORD)nID);
}

BOOL COlePropertyPage::GetControlStatus(UINT nID)
{
	for (int nControl = 0; nControl < m_nControls; nControl++)
		if (m_pStatus[nControl].nID == nID)
			return m_pStatus[nControl].bDirty;

	// If we couldn't find the control - assume it is dirty
	return TRUE;
}

BOOL COlePropertyPage::SetControlStatus(UINT nID, BOOL bDirty)
{
	for (int nControl = 0; nControl < m_nControls; nControl++)
		if (m_pStatus[nControl].nID == nID)
		{
			m_pStatus[nControl].bDirty = bDirty;
			return TRUE;
		}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////
// Function Templates using the Preprocessor
// [This should be replaced with C++ Templates when the 16 bit compiler allows]

#define DEFINE_GET_SET_PROP(ctype,vttype) \
BOOL COlePropertyPage::SetPropText(LPCTSTR pszPropName, ctype& data ) \
{ \
	COleDispatchDriver PropDispDriver; \
	BOOL bResult = FALSE; \
	for (ULONG i = 0; i < m_nObjects; i++) \
	{ \
		DISPID dwDispID; \
		if (SUCCEEDED(m_ppDisp[i]->GetIDsOfNames(IID_NULL, (LPTSTR FAR*)&pszPropName, 1, 0, &dwDispID))) \
		{ \
			PropDispDriver.AttachDispatch(m_ppDisp[i], FALSE); \
			PropDispDriver.SetProperty(dwDispID, vttype, data ); \
			PropDispDriver.DetachDispatch(); \
			bResult = TRUE; \
		} \
	} \
	return bResult; \
} \
BOOL COlePropertyPage::GetPropText(LPCTSTR pszPropName, ctype *data ) \
{ \
	COleDispatchDriver PropDispDriver; \
	BOOL bSuccess = FALSE; \
	for (ULONG i = 0; i < m_nObjects; i++) \
	{ \
		DISPID dwDispID; \
		if (SUCCEEDED(m_ppDisp[i]->GetIDsOfNames(IID_NULL, (LPTSTR FAR*)&pszPropName, 1, 0, &dwDispID))) \
		{ \
			ctype dataTemp; \
			static ctype NEAR nil; \
			PropDispDriver.AttachDispatch(m_ppDisp[i], FALSE); \
			PropDispDriver.GetProperty(dwDispID, vttype, &dataTemp); \
			PropDispDriver.DetachDispatch(); \
			if (i == 0) *data = dataTemp; \
			if (*data != dataTemp) *data = nil; \
			bSuccess = TRUE; \
		} \
	} \
	return bSuccess; \
}

/////////////////////////////////////////////////////////////////////////////
// DDP_ property get/set helpers

DEFINE_GET_SET_PROP( BYTE, VT_UI1 );
#ifdef _WIN32
DEFINE_GET_SET_PROP( int, VT_I4 );
DEFINE_GET_SET_PROP( UINT, VT_UI4 );
#else
DEFINE_GET_SET_PROP( int, VT_I2 );
DEFINE_GET_SET_PROP( UINT, VT_UI2 );
#endif
DEFINE_GET_SET_PROP( long, VT_I4 );
DEFINE_GET_SET_PROP( DWORD, VT_UI4 );
DEFINE_GET_SET_PROP( float, VT_R4 );
DEFINE_GET_SET_PROP( double, VT_R8 );
DEFINE_GET_SET_PROP( CString, VT_BSTR );

BOOL COlePropertyPage::SetPropCheck(LPCTSTR pszPropName, int Value)
{
	COleDispatchDriver PropDispDriver;
	BOOL bResult = FALSE;
	BOOL bValue;

	if (Value == 1)
		bValue = TRUE;
	else
		bValue = FALSE;         // default to off

	// Set the properties for all the objects
	for (ULONG i = 0; i < m_nObjects; i++)
	{
		DISPID dwDispID;

		// Get the Dispatch ID for the property and if successful set the value of the property
		if (SUCCEEDED(m_ppDisp[i]->GetIDsOfNames(IID_NULL, (LPTSTR FAR*)&pszPropName, 1, 0, &dwDispID)))
		{
			// Set property
			PropDispDriver.AttachDispatch(m_ppDisp[i], FALSE);
			PropDispDriver.SetProperty(dwDispID, VT_BOOL, bValue);
			PropDispDriver.DetachDispatch();
			bResult = TRUE;
		}
	}
	return bResult;
}

BOOL COlePropertyPage::GetPropCheck(LPCTSTR pszPropName, int* pValue)
{
	COleDispatchDriver PropDispDriver;
	BOOL bSuccess = FALSE;

	// Check the property values for all the objects
	for (ULONG i = 0; i < m_nObjects; i++)
	{
		DISPID dwDispID;

		// Get the Dispatch ID for the property and if successful get the value of the property
		if (SUCCEEDED(m_ppDisp[i]->GetIDsOfNames(IID_NULL, (LPTSTR FAR*)&pszPropName, 1, 0, &dwDispID)))
		{
			// Get property
			BOOL bTemp = FALSE;
			int tempValue;

			PropDispDriver.AttachDispatch(m_ppDisp[i], FALSE);
			PropDispDriver.GetProperty(dwDispID, VT_BOOL, &bTemp);
			PropDispDriver.DetachDispatch();

			// Convert boolean value to check box equivalent
			if (bTemp)
				tempValue = 1;
			else
				tempValue = 0;

			// Handle special case for first object
			if (i == 0)
				*pValue = tempValue;

			// If the current check value is not the same as the one just retrieved then
			// set the current check value to the indeterminate state.
			if (tempValue != *pValue)
				*pValue = 2;

			bSuccess = TRUE;
		}
	}
	return bSuccess;
}

BOOL COlePropertyPage::SetPropRadio(LPCTSTR pszPropName, int Value)
{
	COleDispatchDriver PropDispDriver;
	BOOL bSuccess = FALSE;

	// Set the properties for all the objects
	for (ULONG i = 0; i < m_nObjects; i++)
	{
		DISPID dwDispID;

		// Get the Dispatch ID for the property and if successful set the value of the property
		if (SUCCEEDED(m_ppDisp[i]->GetIDsOfNames(IID_NULL, (LPTSTR FAR*)&pszPropName, 1, 0, &dwDispID)))
		{
			// Set property
			PropDispDriver.AttachDispatch(m_ppDisp[i], FALSE);
			PropDispDriver.SetProperty(dwDispID, VT_I2, Value);
			PropDispDriver.DetachDispatch();
			bSuccess = TRUE;
		}
	}
	return bSuccess;
}

BOOL COlePropertyPage::GetPropRadio(LPCTSTR pszPropName, int* pValue)
{
	COleDispatchDriver PropDispDriver;
	BOOL bSuccess = FALSE;

	// Check the property values for all the objects
	for (ULONG i = 0; i < m_nObjects; i++)
	{
		DISPID dwDispID;

		// Get the Dispatch ID for the property and if successful get the value of the property
		if (SUCCEEDED(m_ppDisp[i]->GetIDsOfNames(IID_NULL, (LPTSTR FAR*)&pszPropName, 1, 0, &dwDispID)))
		{
			int nTemp;

			// Get property
			PropDispDriver.AttachDispatch(m_ppDisp[i], FALSE);
			PropDispDriver.GetProperty(dwDispID, VT_I2, &nTemp);
			PropDispDriver.DetachDispatch();

			// Handle special case for first object
			if (i == 0)
				*pValue = nTemp;

			// Compare the current radio value with the one just retrieved then
			// if they are different then set the radio value to -1, so that no
			// radio buttons will be checked.
			if (nTemp != *pValue)
				*pValue = -1;

			bSuccess = TRUE;
		}
	}
	return bSuccess;
}

BOOL COlePropertyPage::SetPropIndex(LPCTSTR pszPropName, int Value)
{
	return SetPropRadio(pszPropName, Value);
}

BOOL COlePropertyPage::GetPropIndex(LPCTSTR pszPropName, int* pValue)
{
	return GetPropRadio(pszPropName, pValue);
}


/////////////////////////////////////////////////////////////////////////////
// DDP_Begin data exchange routines (Should be C++ templated someday!)

#define DEFINE_DDP_(group,ctype,vtype) \
void AFXAPI DDP_End##group(CDataExchange* pDX, int, \
						   ctype *member, LPCTSTR pszPropName ) \
{ \
	ASSERT((pDX->m_pDlgWnd)->IsKindOf(RUNTIME_CLASS(COlePropertyPage))); \
	COlePropertyPage* propDialog = (COlePropertyPage*)(pDX->m_pDlgWnd); \
	if (pDX->m_bSaveAndValidate) \
		propDialog->SetProp##group(pszPropName, *member ); \
} \
void AFXAPI DDP_##group(CDataExchange* pDX, int nCtrlId, \
						ctype &member, LPCTSTR pszPropName ) \
{ \
	ASSERT(AfxIsValidString(pszPropName)); \
	ASSERT((pDX->m_pDlgWnd)->IsKindOf(RUNTIME_CLASS(COlePropertyPage))); \
	COlePropertyPage* propDialog = (COlePropertyPage*)(pDX->m_pDlgWnd); \
	if (pDX->m_bSaveAndValidate) /*Are we Saving?*/ \
	{ \
		if ( propDialog->GetControlStatus(nCtrlId) ) /*Is Control Dirty?*/ \
		{ \
			void (AFXAPI *pfv)(CDataExchange*,int,ctype*,LPCTSTR) = \
				 DDP_End##group; \
			AFX_DDPDATA *pDDP = new AFX_DDPDATA( (void *)pfv, nCtrlId, \
												 (void*)&member, (UINT)vtype, \
												 pszPropName ); \
			propDialog->m_DDPmap.SetAt( (WORD)(LONG)&member, pDDP ); \
		} \
	} \
	else /* Loading data from properties! */ \
	{ \
			propDialog->GetProp##group(pszPropName, &member); \
			propDialog->SetControlStatus(nCtrlId,FALSE); \
	} \
}

/////////////////////////////////////////////////////////////////////////////
// DDP Functions (Pseudo Template Generation)

DEFINE_DDP_(Text,BYTE,VT_UI1);
#ifdef _WIN32
DEFINE_DDP_(Text,int,VT_I4);
DEFINE_DDP_(Text,UINT,VT_UI4);
#else
DEFINE_DDP_(Text,int,VT_I2);
DEFINE_DDP_(Text,UINT,VT_UI2);
#endif
DEFINE_DDP_(Text,long,VT_I4);
DEFINE_DDP_(Text,DWORD,VT_UI4);
DEFINE_DDP_(Text,float,VT_R4);
DEFINE_DDP_(Text,double,VT_R8);
DEFINE_DDP_(Text,CString,VT_BSTR);

#ifdef _WIN32
DEFINE_DDP_(Check,BOOL,VT_I4);
DEFINE_DDP_(Radio,int,VT_I4);
#else
DEFINE_DDP_(Check,BOOL,VT_I2);
DEFINE_DDP_(Radio,int,VT_I2);
#endif

//////////////////////////////////////////////////////////////////////////////
// DDP Deferred Property Write Handler

void AFXAPI DDP_PostProcessing(CDataExchange*pDX)
{
	if ( pDX->m_bSaveAndValidate )
	{
		WORD            wKey;
		CMapWordToOb    &m_DDPmap =
			((COlePropertyPage *)pDX->m_pDlgWnd)->m_DDPmap;
		AFX_DDPDATA     *pDDP = NULL;
		POSITION pos = m_DDPmap.GetStartPosition();
		while( pos )
		{
			m_DDPmap.GetNextAssoc( pos, wKey, (CObject *&)pDDP);
			TRY
			{
				switch( pDDP->m_nType )
				{
					case    VT_I1:
					{
						typedef void (AFXAPI *PFV)(CDataExchange *, int,
												   char *, LPCTSTR );
						(*(PFV)pDDP->m_lpHandler)(pDX, pDDP->m_nCtrlId,
												  (char *)pDDP->m_lpMember,
												  pDDP->m_lpszOleName );
						break;
					}
					case    VT_UI1:
					{
						typedef void (AFXAPI *PFV)(CDataExchange *, int,
												   BYTE *, LPCTSTR );
						(*(PFV)pDDP->m_lpHandler)(pDX, pDDP->m_nCtrlId,
												  (BYTE *)pDDP->m_lpMember,
												  pDDP->m_lpszOleName );

						break;
					}
					case    VT_I2:
					{
						typedef void (AFXAPI *PFV)(CDataExchange *, int,
												   short *, LPCTSTR );
						(*(PFV)pDDP->m_lpHandler)(pDX, pDDP->m_nCtrlId,
												  (short *)pDDP->m_lpMember,
												  pDDP->m_lpszOleName );
						break;
					}
					case    VT_UI2:
					{
						typedef void (AFXAPI *PFV)(CDataExchange *, int,
												   WORD *, LPCTSTR );
						(*(PFV)pDDP->m_lpHandler)(pDX, pDDP->m_nCtrlId,
												  (WORD *)pDDP->m_lpMember,
												  pDDP->m_lpszOleName );
						break;
					}
					case    VT_I4:
					{
						typedef void (AFXAPI *PFV)(CDataExchange *, int,
												   long *, LPCTSTR );
						(*(PFV)pDDP->m_lpHandler)(pDX, pDDP->m_nCtrlId,
												  (long *)pDDP->m_lpMember,
												  pDDP->m_lpszOleName );
						break;
					}
					case    VT_UI4:
					{
						typedef void (AFXAPI *PFV)(CDataExchange *, int,
												   DWORD *, LPCTSTR );
						(*(PFV)pDDP->m_lpHandler)(pDX, pDDP->m_nCtrlId,
												  (DWORD *)pDDP->m_lpMember,
												  pDDP->m_lpszOleName);
						break;
					}
					case    VT_R4:
					{
						typedef void (AFXAPI *PFV)(CDataExchange *, int,
												   float *, LPCTSTR );
						(*(PFV)pDDP->m_lpHandler)(pDX, pDDP->m_nCtrlId,
												  (float *)pDDP->m_lpMember,
												  pDDP->m_lpszOleName);
						break;
					}
					case    VT_R8:
					{
						typedef void (AFXAPI *PFV)(CDataExchange *, int,
												   double *, LPCTSTR );
						(*(PFV)pDDP->m_lpHandler)(pDX, pDDP->m_nCtrlId,
												  (double *)pDDP->m_lpMember,
												  pDDP->m_lpszOleName );
						break;
					}
					case    VT_BSTR:
					{
						typedef void (AFXAPI *PFV)(CDataExchange *, int,
												   CString *, LPCTSTR );
						(*(PFV)pDDP->m_lpHandler)(pDX, pDDP->m_nCtrlId,
												  (CString *)pDDP->m_lpMember,
												  pDDP->m_lpszOleName );
						break;
					}
					default:
						// Unknown Data Type!
						ASSERT(FALSE);
						break;
				}
			}
			CATCH_ALL(e)

			{
				// Ignore all exceptions from the called functions!(Workaround)
				// Particularly from attempts to SetProperty read-only props.
			}
			END_CATCH_ALL
			delete pDDP;
		}
		m_DDPmap.RemoveAll();
	}
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

IMPLEMENT_DYNAMIC(COlePropertyPage, CDialog)
