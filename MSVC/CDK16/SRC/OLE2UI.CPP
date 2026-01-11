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


// _RoundDiv -- rounded long division

long _RoundDiv(long numer, long denom)
{
	ldiv_t result;
	long quotient;
	long absrem;

	// get the quotient and remainder using C runtime function
	result = ldiv(numer, denom);
	quotient = result.quot;
	absrem = labs(result.rem);

	// if the remainder is >= half the denominator, we need to round.
	if (labs(denom) - absrem <= absrem)
	if (quotient >= 0)
		quotient++;
	else
		quotient--;

	return quotient;
}


STDAPI_(LPVOID) _AfxCtlMalloc(ULONG ulSize)
{
	LPMALLOC pMalloc = NULL;
	void FAR* p = NULL;

	if (SUCCEEDED(CoGetMalloc(MEMCTX_TASK, &pMalloc)))
	{
		ASSERT(pMalloc != NULL);
		p = pMalloc->Alloc(ulSize);
		pMalloc->Release();
	}

	return p;
}


STDAPI_(void) _AfxCtlFree(LPVOID p)
{
	if (p == NULL)      // Free(NULL) is a no-op
		return;

	LPMALLOC pMalloc = NULL;

	if (SUCCEEDED(CoGetMalloc(MEMCTX_TASK, &pMalloc)))
	{
		ASSERT(pMalloc != NULL);
		pMalloc->Free(p);
		pMalloc->Release();
	}
}


STDAPI_(LPVOID) _AfxCtlRealloc(LPVOID p, ULONG ulSize)
{
	LPMALLOC pMalloc = NULL;
	void FAR* pNew = NULL;

	if (SUCCEEDED(CoGetMalloc(MEMCTX_TASK, &pMalloc)))
	{
		ASSERT(pMalloc != NULL);
		pNew = pMalloc->Realloc(p, ulSize);
		pMalloc->Release();
	}

	return pNew;
}


/* _AfxCtlFreeString
** ----------------
**    Free a string that was allocated with the currently active
**    IMalloc* allocator.
**
**    if the caller has the current IMalloc* handy, then it can be
**    passed as a argument, otherwise this function will retrieve the
**    active allocator and use it.
*/
STDAPI_(void) _AfxCtlFreeString(LPTSTR lpsz, LPMALLOC lpMalloc)
{
	BOOL fMustRelease = FALSE;

	if (! lpMalloc) {
		if (CoGetMalloc(MEMCTX_TASK, &lpMalloc) != NOERROR)
			return;
		fMustRelease = TRUE;
	}

	lpMalloc->Free(lpsz);

	if (fMustRelease)
		lpMalloc->Release();
}


/* _AfxCtlCopyString
** ----------------
**    Copy a string into memory allocated with the currently active
**    IMalloc* allocator.
**
**    if the caller has the current IMalloc* handy, then it can be
**    passed as a argument, otherwise this function will retrieve the
**    active allocator and use it.
*/
STDAPI_(LPTSTR) _AfxCtlCopyString(LPTSTR lpszSrc, LPMALLOC lpMalloc)
{
	LPTSTR lpszDest = NULL;
	BOOL fMustRelease = FALSE;
	UINT lSize = (_tcslen(lpszSrc)+1) * sizeof(TCHAR);

	if (! lpMalloc) {
		if (CoGetMalloc(MEMCTX_TASK, &lpMalloc) != NOERROR)
			return NULL;
		fMustRelease = TRUE;
	}

	lpszDest = (LPTSTR)lpMalloc->Alloc(lSize);

	if (lpszDest)
		_tcscpy(lpszDest, lpszSrc);

	if (fMustRelease)
		lpMalloc->Release();
	return lpszDest;
}


/*
 * _AfxCtlDeleteTargetDevice()
 *
 * Purpose:
 *
 * Parameters:
 *
 * Return Value:
 *    SCODE  -  S_OK if successful
 */
STDAPI_(BOOL) _AfxCtlDeleteTargetDevice(DVTARGETDEVICE FAR* ptd)
{
	if (ptd != NULL)
		_AfxCtlFree(ptd);

	return TRUE;
}


/*
 * _AfxCtlCopyTargetDevice()
 *
 * Purpose:
 *  duplicate a TARGETDEVICE struct. this function allocates memory for
 *  the copy. the caller MUST free the allocated copy when done with it
 *  using the standard allocator returned from CoGetMalloc.
 *  (_AfxCtlFree can be used to free the copy).
 *
 * Parameters:
 *  ptdSrc      pointer to source TARGETDEVICE
 *
 * Return Value:
 *    pointer to allocated copy of ptdSrc
 *    if ptdSrc==NULL then retuns NULL is returned.
 *    if ptdSrc!=NULL and memory allocation fails, then NULL is returned
 */
STDAPI_(DVTARGETDEVICE FAR*) _AfxCtlCopyTargetDevice(
	DVTARGETDEVICE FAR* ptdSrc)
{
	DVTARGETDEVICE FAR* ptdDest = NULL;

	if (ptdSrc == NULL)
		return NULL;

	if ((ptdDest = (DVTARGETDEVICE FAR*)_AfxCtlMalloc(ptdSrc->tdSize)) != NULL)
		_fmemcpy(ptdDest, ptdSrc, (size_t)ptdSrc->tdSize);

	return ptdDest;
}


/*
 * OleStdCopyFormatEtc()
 *
 * Purpose:
 *  Copies the contents of a FORMATETC structure. this function takes
 *  special care to copy correctly copying the pointer to the TARGETDEVICE
 *  contained within the source FORMATETC structure.
 *  if the source FORMATETC has a non-NULL TARGETDEVICE, then a copy
 *  of the TARGETDEVICE will be allocated for the destination of the
 *  FORMATETC (petcDest).
 *
 *  OLE2NOTE: the caller MUST free the allocated copy of the TARGETDEVICE
 *  within the destination FORMATETC when done with it
 *  using the standard allocator returned from CoGetMalloc.
 *  (OleStdFree can be used to free the copy).
 *
 * Parameters:
 *  petcDest      pointer to destination FORMATETC
 *  petcSrc       pointer to source FORMATETC
 *
 * Return Value:
 *    returns TRUE is copy is successful; retuns FALSE if not successful
 */

STDAPI_(BOOL) _AfxCtlCopyFormatEtc(LPFORMATETC petcDest, LPFORMATETC petcSrc)
{
	if ((petcDest == NULL) || (petcSrc == NULL))
		return FALSE;

	petcDest->cfFormat = petcSrc->cfFormat;
	petcDest->ptd      = _AfxCtlCopyTargetDevice(petcSrc->ptd);
	petcDest->dwAspect = petcSrc->dwAspect;
	petcDest->lindex   = petcSrc->lindex;
	petcDest->tymed    = petcSrc->tymed;

	return TRUE;
}


STDAPI_(int) _XformWidthInPixelsToHimetric(HDC hDC, int iWidthInPix)
{
	int     iXppli;     //Pixels per logical inch along width
	int     iWidthInHiMetric;
	BOOL    fSystemDC=FALSE;

	if (NULL==hDC)
	{
		hDC=GetDC(NULL);
		fSystemDC=TRUE;
	}

	iXppli = GetDeviceCaps (hDC, LOGPIXELSX);

	//We got pixel units, convert them to logical HIMETRIC along the display
	iWidthInHiMetric = MAP_PIX_TO_LOGHIM(iWidthInPix, iXppli);

	if (fSystemDC)
		ReleaseDC(NULL, hDC);

	return iWidthInHiMetric;
}


STDAPI_(int) _XformWidthInHimetricToPixels(HDC hDC, int iWidthInHiMetric)
{
	int     iXppli;     //Pixels per logical inch along width
	int     iWidthInPix;
	BOOL    fSystemDC=FALSE;

	if (NULL==hDC)
	{
		hDC=GetDC(NULL);
		fSystemDC=TRUE;
	}

	iXppli = GetDeviceCaps (hDC, LOGPIXELSX);

	//We got logical HIMETRIC along the display, convert them to pixel units
	iWidthInPix = MAP_LOGHIM_TO_PIX(iWidthInHiMetric, iXppli);

	if (fSystemDC)
		ReleaseDC(NULL, hDC);

	return iWidthInPix;
}


STDAPI_(int) _XformHeightInPixelsToHimetric(HDC hDC, int iHeightInPix)
{
	int     iYppli;     //Pixels per logical inch along height
	int     iHeightInHiMetric;
	BOOL    fSystemDC=FALSE;

	if (NULL==hDC)
	{
		hDC=GetDC(NULL);
		fSystemDC=TRUE;
	}

	iYppli = GetDeviceCaps (hDC, LOGPIXELSY);

	//* We got pixel units, convert them to logical HIMETRIC along the display
	iHeightInHiMetric = MAP_PIX_TO_LOGHIM(iHeightInPix, iYppli);

	if (fSystemDC)
		ReleaseDC(NULL, hDC);

	return iHeightInHiMetric;
}


STDAPI_(int) _XformHeightInHimetricToPixels(HDC hDC, int iHeightInHiMetric)
{
	int     iYppli;     //Pixels per logical inch along height
	int     iHeightInPix;
	BOOL    fSystemDC=FALSE;

	if (NULL==hDC)
	{
		hDC=GetDC(NULL);
		fSystemDC=TRUE;
	}

	iYppli = GetDeviceCaps (hDC, LOGPIXELSY);

	//* We got logical HIMETRIC along the display, convert them to pixel units
	iHeightInPix = MAP_LOGHIM_TO_PIX(iHeightInHiMetric, iYppli);

	if (fSystemDC)
		ReleaseDC(NULL, hDC);

	return iHeightInPix;
}


STDAPI_(void) _XformSizeInPixelsToHimetric(
	HDC hDC, LPSIZEL lpSizeInPix, LPSIZEL lpSizeInHiMetric)
{
	int     iXppli;     //Pixels per logical inch along width
	int     iYppli;     //Pixels per logical inch along height
	BOOL    fSystemDC=FALSE;

	if (NULL==hDC || GetDeviceCaps(hDC, LOGPIXELSX) == 0)
	{
		hDC=GetDC(NULL);
		fSystemDC=TRUE;
	}

	iXppli = GetDeviceCaps (hDC, LOGPIXELSX);
	iYppli = GetDeviceCaps (hDC, LOGPIXELSY);

	//We got pixel units, convert them to logical HIMETRIC along the display
	lpSizeInHiMetric->cx = (long)MAP_PIX_TO_LOGHIM((int)lpSizeInPix->cx, iXppli);
	lpSizeInHiMetric->cy = (long)MAP_PIX_TO_LOGHIM((int)lpSizeInPix->cy, iYppli);

	if (fSystemDC)
		ReleaseDC(NULL, hDC);

	return;
}


STDAPI_(void) _XformSizeInHimetricToPixels(
	HDC hDC, LPSIZEL lpSizeInHiMetric, LPSIZEL lpSizeInPix)
{
	int     iXppli;     //Pixels per logical inch along width
	int     iYppli;     //Pixels per logical inch along height
	BOOL    fSystemDC=FALSE;

	if (NULL==hDC || GetDeviceCaps(hDC, LOGPIXELSX) == 0)
	{
		hDC=GetDC(NULL);
		fSystemDC=TRUE;
	}

	iXppli = GetDeviceCaps (hDC, LOGPIXELSX);
	iYppli = GetDeviceCaps (hDC, LOGPIXELSY);

	//We got logical HIMETRIC along the display, convert them to pixel units
	lpSizeInPix->cx = (long)MAP_LOGHIM_TO_PIX((int)lpSizeInHiMetric->cx, iXppli);
	lpSizeInPix->cy = (long)MAP_LOGHIM_TO_PIX((int)lpSizeInHiMetric->cy, iYppli);

	if (fSystemDC)
		ReleaseDC(NULL, hDC);

	return;
}


/*
 * _XformRectInPixelsToHimetric
 * _XformRectInHimetricToPixels
 *
 * Purpose:
 *  Convert a rectangle between pixels of a given hDC and HIMETRIC units
 *  as manipulated in OLE.  If the hDC is NULL, then a screen DC is used
 *  and assumes the MM_TEXT mapping mode.
 *
 * Parameters:
 *  hDC             HDC providing reference to the pixel mapping.  If
 *                  NULL, a screen DC is used.
 *  lprcSrc         LPRECT providing the rectangle to convert.  This
 *                  contains pixels in _XformRectInPixelsToHimetric and
 *                  logical HiMetric units in the complement function.
 *  lprcDst         LPRECT providing the rectangle to receive converted units.
 *                  This contains pixels in _XformRectInPixelsToHimetric and
 *                  logical HiMetric units in the complement function.
 *
 * Return Value:
 *  None
 *
 * NOTE:
 *  When displaying on the screen, Window apps display everything enlarged
 *  from its actual size so that it is easier to read. For example, if an
 *  app wants to display a 1in. horizontal line, that when printed is
 *  actually a 1in. line on the printed page, then it will display the line
 *  on the screen physically larger than 1in. This is described as a line
 *  that is "logically" 1in. along the display width. Windows maintains as
 *  part of the device-specific information about a given display device:
 *      LOGPIXELSX -- no. of pixels per logical in along the display width
 *      LOGPIXELSY -- no. of pixels per logical in along the display height
 *
 *  The following formula converts a distance in pixels into its equivalent
 *  logical HIMETRIC units:
 *
 *      DistInHiMetric = (HIMETRIC_PER_INCH * DistInPix)
 *                      -------------------------------
 *                            PIXELS_PER_LOGICAL_IN
 *
 * Rect in Pixels (MM_TEXT):
 *
 *              0---------- X
 *              |
 *              |       1) ------------------ ( 2   P1 = (rc.left, rc.top)
 *              |       |                     |     P2 = (rc.right, rc.top)
 *              |       |                     |     P3 = (rc.left, rc.bottom)
 *              |       |                     |     P4 = (rc.right, rc.bottom)
 *                      |                     |
 *              Y       |                     |
 *                      3) ------------------ ( 4
 *
 *              NOTE:   Origin   = (P1x, P1y)
 *                      X extent = P4x - P1x
 *                      Y extent = P4y - P1y
 *
 *
 * Rect in Himetric (MM_HIMETRIC):
 *
 *
 *                      1) ------------------ ( 2   P1 = (rc.left, rc.top)
 *              Y       |                     |     P2 = (rc.right, rc.top)
 *                      |                     |     P3 = (rc.left, rc.bottom)
 *              |       |                     |     P4 = (rc.right, rc.bottom)
 *              |       |                     |
 *              |       |                     |
 *              |       3) ------------------ ( 4
 *              |
 *              0---------- X
 *
 *              NOTE:   Origin   = (P3x, P3y)
 *                      X extent = P2x - P3x
 *                      Y extent = P2y - P3y
 *
 *
 */

STDAPI_(void) _XformRectInPixelsToHimetric(
	HDC hDC, LPRECT lprcPix, LPRECT lprcHiMetric)
{
	int     iXppli;     //Pixels per logical inch along width
	int     iYppli;     //Pixels per logical inch along height
	int     iXextInPix=(lprcPix->right-lprcPix->left);
	int     iYextInPix=(lprcPix->bottom-lprcPix->top);
	BOOL    fSystemDC=FALSE;

	if (NULL==hDC || GetDeviceCaps(hDC, LOGPIXELSX) == 0)
	{
		hDC=GetDC(NULL);
		fSystemDC=TRUE;
	}

	iXppli = GetDeviceCaps (hDC, LOGPIXELSX);
	iYppli = GetDeviceCaps (hDC, LOGPIXELSY);

	ASSERT(iXppli > 0);
	ASSERT(iYppli > 0);

	//We got pixel units, convert them to logical HIMETRIC along the display
	//Note: don't use Muldiv, I got problems with overflow
	lprcHiMetric->right = (int)_RoundDiv((LONG)iXextInPix * HIMETRIC_PER_INCH, iXppli);
	lprcHiMetric->top   = (int)_RoundDiv((LONG)iYextInPix * HIMETRIC_PER_INCH, iYppli);

	lprcHiMetric->left    = 0;
	lprcHiMetric->bottom  = 0;

	if (fSystemDC)
		ReleaseDC(NULL, hDC);

	return;
}


/*
 * _AfxCtlCreateDC()
 *
 * Purpose:
 *
 * Parameters:
 *
 * Return Value:
 *    SCODE  -  S_OK if successful
 */
STDAPI_(HDC) _AfxCtlCreateDC(DVTARGETDEVICE FAR* ptd)
{
	HDC hdc=NULL;
	LPDEVNAMES lpDevNames;
	LPDEVMODE lpDevMode;
	LPTSTR lpszDriverName;
	LPTSTR lpszDeviceName;
	LPTSTR lpszPortName;

	if (ptd == NULL)
	{
		hdc = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
		goto errReturn;
	}

	lpDevNames = (LPDEVNAMES) ptd; // offset for size field

	if (ptd->tdExtDevmodeOffset == 0)
		lpDevMode = NULL;
	else
		lpDevMode = (LPDEVMODE) ((LPTSTR)ptd + ptd->tdExtDevmodeOffset);

	lpszDriverName = (LPTSTR) lpDevNames + ptd->tdDriverNameOffset;
	lpszDeviceName = (LPTSTR) lpDevNames + ptd->tdDeviceNameOffset;
	lpszPortName   = (LPTSTR) lpDevNames + ptd->tdPortNameOffset;

	hdc = CreateDC(lpszDriverName, lpszDeviceName, lpszPortName, lpDevMode);

errReturn:
	return hdc;
}


/*
 * _AfxCtlSetDCToDrawInHimetricRect
 *
 * Purpose:
 *  Setup the correspondence between the rect in pixels (Viewport) and
 *  the rect in HIMETRIC (Window) so that the proper scaling of
 *  coordinate systems will be calculated. set up both the Viewport and
 *  the window as follows:
 *
 *      1) ------------------ ( 2
 *      |                     |
 *      |                     |
 *      |                     |
 *      |                     |
 *      |                     |
 *      3) ------------------ ( 4
 *
 *      Origin   = P3
 *      X extent = P2x - P3x
 *      Y extent = P2y - P3y
 *
 * Parameters:
 *  hDC             HDC to affect
 *  lprcPix         LPRECT containing the pixel extents of DC
 *  lprcHiMetric    LPRECT to receive the himetric extents
 *  lprcWindowOld   LPRECT in which to preserve the window for _AfxCtlResetOrigDC
 *  lprcViewportOld LPRECT in which to preserver the viewport for _AfxCtlResetOrigDC
 *
 * Return Value:
 *  int             The original mapping mode of the DC.
 */
STDAPI_(int) _AfxCtlSetDCToDrawInHimetricRect(HDC hDC, LPRECT lprcPix,
	LPRECT lprcHiMetric, LPRECT lprcWindowOld, LPRECT lprcViewportOld)
{
	int nMapModeOld = SetMapMode(hDC, MM_ANISOTROPIC);
	BOOL fSystemDC = FALSE;

	if (NULL==hDC)
	{
		hDC=GetDC(NULL);
		fSystemDC=TRUE;
	}

	_XformRectInPixelsToHimetric(hDC, lprcPix, lprcHiMetric);

	SetWindowOrgEx(hDC, lprcHiMetric->left, lprcHiMetric->bottom,
		(LPPOINT)&lprcWindowOld->left);
	SetWindowExtEx(hDC, (lprcHiMetric->right-lprcHiMetric->left),
		(lprcHiMetric->top-lprcHiMetric->bottom), (LPSIZE)&lprcWindowOld->right);
	SetViewportOrgEx(hDC, lprcPix->left, lprcPix->bottom,
		(LPPOINT)&lprcViewportOld->left);
	SetViewportExtEx(hDC, (lprcPix->right-lprcPix->left),
		(lprcPix->top-lprcPix->bottom), (LPSIZE)&lprcViewportOld->right);

	if (fSystemDC)
		ReleaseDC(NULL, hDC);

	return nMapModeOld;
}

/*
 * _AfxCtlResetOrigDC
 *
 * Purpose:
 *  Restores a DC set to draw in himetric from _AfxCtlSetDCToDrawInHimetricRect.
 *
 * Parameters:
 *  hDC             HDC to restore
 *  nMapModeOld     int original mapping mode of hDC
 *  lprcWindowOld   LPRECT filled in _AfxCtlSetDCToDrawInHimetricRect
 *  lprcViewportOld LPRECT filled in _AfxCtlSetDCToDrawInHimetricRect
 *
 * Return Value:
 *  int             Same as nMapModeOld.
 */

STDAPI_(int) _AfxCtlResetOrigDC(HDC hDC, int nMapModeOld,
	LPRECT lprcWindowOld, LPRECT lprcViewportOld)
{
	POINT     pOld;

	SetMapMode(hDC, nMapModeOld);
	SetWindowOrgEx(hDC, lprcWindowOld->left, lprcWindowOld->top, &pOld);
	SetWindowExtEx(hDC, lprcWindowOld->right, lprcWindowOld->bottom,
		(LPSIZE)&pOld);
	SetViewportOrgEx(hDC, lprcViewportOld->left, lprcViewportOld->top, &pOld);
	SetViewportExtEx(hDC, lprcViewportOld->right, lprcViewportOld->bottom,
		(LPSIZE)&pOld);

	return nMapModeOld;
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
