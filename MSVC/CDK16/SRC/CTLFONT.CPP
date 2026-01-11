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
// CFontHolder

EXPORT CFontHolder::CFontHolder(LPPROPERTYNOTIFYSINK pNotify) :
	m_pFont(NULL),
	m_dwConnectCookie(0),
	m_pNotify(pNotify)
{
	ASSERT_NULL_OR_POINTER(pNotify, IPropertyNotifySink);
}

EXPORT CFontHolder::~CFontHolder()
{
	ReleaseFont();
}

void EXPORT CFontHolder::ReleaseFont()
{
	if ((m_pFont != NULL) && (m_pNotify != NULL))
	{
		AfxConnectionUnadvise(m_pFont, IID_IPropertyNotifySink, m_pNotify,
			FALSE, m_dwConnectCookie);
	}

	RELEASE(m_pFont);
}


static const FONTDESC NEAR _fdDefault =
	{ sizeof(FONTDESC), _T("MS Sans Serif"), FONTSIZE(12), FW_NORMAL,
	  DEFAULT_CHARSET, FALSE, FALSE, FALSE };


void EXPORT CFontHolder::InitializeFont(const FONTDESC FAR* pFontDesc,
	LPDISPATCH pFontDispAmbient)
{
	ASSERT_NULL_OR_POINTER(pFontDesc, FONTDESC);
	ASSERT_NULL_OR_POINTER(pFontDispAmbient, IDispatch);
#ifdef _DEBUG
	if (pFontDesc != NULL)
		ASSERT(pFontDesc->cbSizeofstruct == sizeof(FONTDESC));
#endif

	// Release any previous font, in preparation for creating a new one.
	ReleaseFont();

	LPFONT pFontAmbient;
	LPFONT pFontNew = NULL;

	if ((pFontDispAmbient != NULL) &&
		SUCCEEDED(pFontDispAmbient->QueryInterface(IID_IFont,
				(LPVOID FAR*)&pFontAmbient)))
	{
		ASSERT_POINTER(pFontAmbient, IFont);

		// Make a clone of the ambient font.
		pFontAmbient->Clone(&pFontNew);
		pFontAmbient->Release();
	}
	else
	{
		// Create the font.
		if (pFontDesc == NULL)
			pFontDesc = &_fdDefault;

		if (FAILED(OleCreateFontIndirect((LPFONTDESC)pFontDesc, IID_IFont,
				(LPVOID FAR *)&pFontNew)))
			pFontNew = NULL;
	}

	// Setup advisory connection and find dispatch interface.
	if (pFontNew != NULL)
		SetFont(pFontNew);
}


BOOL _AfxIsSameFont(CFontHolder& font, const FONTDESC FAR* pFontDesc,
	LPFONTDISP pFontDispAmbient)
{
	if (font.m_pFont == NULL)
		return FALSE;

	BOOL bSame = FALSE;

	if (pFontDispAmbient != NULL)
	{
		LPFONT pFontAmbient;
		if (SUCCEEDED(pFontDispAmbient->QueryInterface(IID_IFont,
			(LPVOID FAR*)&pFontAmbient)))
		{
			ASSERT_POINTER(pFontAmbient, IFont);
			bSame = pFontAmbient->IsEqual(font.m_pFont) == S_OK;
			pFontAmbient->Release();
		}
	}
	else
	{
		if (pFontDesc == NULL)
			pFontDesc = &_fdDefault;

		bSame = TRUE;
		BOOL bFlag;

		font.m_pFont->get_Italic(&bFlag);
		bSame = (bFlag == pFontDesc->fItalic);

		if (bSame)
		{
			font.m_pFont->get_Underline(&bFlag);
			bSame = (bFlag == pFontDesc->fUnderline);
		}

		if (bSame)
		{
			font.m_pFont->get_Strikethrough(&bFlag);
			bSame = (bFlag == pFontDesc->fStrikethrough);
		}

		if (bSame)
		{
			short sCharset;
			font.m_pFont->get_Charset(&sCharset);
			bSame = (sCharset == pFontDesc->sCharset);
		}

		if (bSame)
		{
			short sWeight;
			font.m_pFont->get_Weight(&sWeight);
			bSame = (sWeight == pFontDesc->sWeight);
		}

		if (bSame)
		{
			CURRENCY cy;
			font.m_pFont->get_Size(&cy);
			bSame = (memcmp(&cy, &pFontDesc->cySize, sizeof(CURRENCY)) == 0);
		}

		if (bSame)
		{
			BSTR bstrName;
			font.m_pFont->get_Name(&bstrName);
			CString strName1(bstrName);
			CString strName2(pFontDesc->lpstrName);
			bSame = (strName1 == strName2);
			SysFreeString(bstrName);
		}
	}

	return bSame;
}




HFONT EXPORT CFontHolder::GetFontHandle()
{
	// Assume a screen DC for logical/himetric ratio.
	CDC dc;
	dc.CreateCompatibleDC(NULL);
	return GetFontHandle((long)dc.GetDeviceCaps(LOGPIXELSY), HIMETRIC_PER_INCH);
}

HFONT EXPORT CFontHolder::GetFontHandle(long cyLogical, long cyHimetric)
{
	HFONT hFont = NULL;

	if ((m_pFont != NULL) &&
		SUCCEEDED(m_pFont->SetRatio(cyLogical, cyHimetric)) &&
		SUCCEEDED(m_pFont->get_hFont(&hFont)))
	{
		ASSERT(hFont != NULL);
	}

	return hFont;
}

CFont* EXPORT CFontHolder::Select(CDC* pDC, long cyLogical, long cyHimetric)
{
	ASSERT_POINTER(pDC, CDC);

	HFONT hFont = NULL;

	if (m_pFont != NULL)
		hFont = GetFontHandle(cyLogical, cyHimetric);

	if (hFont != NULL)
	{
		if ((pDC->m_hAttribDC != pDC->m_hDC) &&
			(pDC->m_hAttribDC != NULL))
		{
			::SelectObject(pDC->m_hAttribDC, hFont);
		}

		return CFont::FromHandle((HFONT)::SelectObject(pDC->m_hDC, hFont));
	}

	return NULL;
}


void EXPORT CFontHolder::QueryTextMetrics(LPTEXTMETRIC lptm)
{
	ASSERT(lptm != NULL);

	if (m_pFont != NULL)
	{
		m_pFont->QueryTextMetrics(lptm);
	}
	else
	{
		memset(lptm, 0, sizeof(TEXTMETRIC));
	}
}


LPFONTDISP EXPORT CFontHolder::GetFontDispatch()
{
	LPFONTDISP pFontDisp = NULL;

	if ((m_pFont != NULL) &&
		SUCCEEDED(m_pFont->QueryInterface(IID_IFontDisp, (LPVOID*)&pFontDisp)))
	{
		ASSERT_POINTER(pFontDisp, IFontDisp);
	}

	return pFontDisp;
}


void EXPORT CFontHolder::SetFont(LPFONT pFontNew)
{
	ASSERT_NULL_OR_POINTER(pFontNew, IFont);

	if (m_pFont != NULL)
		ReleaseFont();

	m_pFont = pFontNew;

	if (m_pNotify != NULL)
	{
		AfxConnectionAdvise(m_pFont, IID_IPropertyNotifySink, m_pNotify,
			FALSE, &m_dwConnectCookie);
	}
}


BOOL EXPORT CFontHolder::GetDisplayString(CString& strValue)
{
	return strValue.LoadString(AFX_IDS_DISPLAYSTRING_FONT);
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
