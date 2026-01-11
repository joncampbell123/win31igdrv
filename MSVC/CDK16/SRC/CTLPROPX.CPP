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


#ifdef _DEBUG
  #ifdef _WIN32
  #define ASSERT_BUFFER_VALID(p, cb, bWrite) \
	ASSERT(AfxIsValidAddress(p, cb, bWrite))
  #else
  #define ASSERT_BUFFER_VALID(p, cb, bWrite) \
	if ((cb) <= 0xffff) ASSERT(AfxIsValidAddress(p, (UINT)(cb), bWrite))
  #endif
#else
#define ASSERT_BUFFER_VALID(p, cb, bWrite)
#endif


/////////////////////////////////////////////////////////////////////////////
// _AfxGetArchiveStream

LPSTREAM _AfxGetArchiveStream(CArchive& ar)
{
	// Obtain direct access to the archive's LPSTREAM.

	ar.Flush();
	LPSTREAM pstm = NULL;
	CFile* pFile = ar.GetFile();
	ASSERT(pFile != NULL);
	ASSERT(pFile->IsKindOf(RUNTIME_CLASS(COleStreamFile)));

	if (pFile->IsKindOf(RUNTIME_CLASS(COleStreamFile)))
		pstm = ((COleStreamFile*)pFile)->m_lpStream;

	if (pstm == NULL)
		AfxThrowArchiveException(CArchiveException::generic);

	ASSERT_POINTER(pstm, IStream);

	return pstm;
}


/////////////////////////////////////////////////////////////////////////////
// _AfxInitBlob

BOOL _AfxInitBlob(HGLOBAL* phDst, void* pvSrc)
{
	BOOL bSuccess = FALSE;
	ULONG cb;

	if ((cb = *(long*)pvSrc) > 0)
	{
		ASSERT_BUFFER_VALID(pvSrc, sizeof(cb) + cb, TRUE);

		*phDst = GlobalAlloc(GMEM_MOVEABLE, sizeof(cb) + cb);
		if (*phDst != NULL)
		{
			void* pvDst = GlobalLock(*phDst);
			if (pvDst != NULL)
			{
#ifdef _WIN32
				_fmemcpy(pvDst, pvSrc, sizeof(cb) + cb);
#else
				hmemcpy(pvDst, pvSrc, (ULONG)sizeof(cb) + cb);
#endif
				bSuccess = TRUE;
				GlobalUnlock(*phDst);
			}
		}
	}
	return bSuccess;
}


/////////////////////////////////////////////////////////////////////////////
// _AfxCopyBlob

BOOL _AfxCopyBlob(HGLOBAL* phDst, HGLOBAL hSrc)
{
	BOOL bSuccess = FALSE;

	void* pvSrc = GlobalLock(hSrc);
	if (pvSrc != NULL)
	{
		bSuccess = _AfxInitBlob(phDst, pvSrc);
		GlobalUnlock(hSrc);
	}
	return bSuccess;
}


/////////////////////////////////////////////////////////////////////////////
// _AfxCopyPropValue

BOOL _AfxCopyPropValue(VARTYPE vtProp, void* pvDest, const void * pvSrc)
{
	ASSERT(AfxIsValidAddress(pvDest, 1));

	if (pvSrc != NULL)
	{
		ASSERT(AfxIsValidAddress(pvSrc, 1, FALSE));

		switch (vtProp)
		{
		case VT_I2:
			*(short*)pvDest = *(short*)pvSrc;
			break;

		case VT_I4:
			*(long*)pvDest = *(long*)pvSrc;
			break;

		case VT_BOOL:
			*(BOOL*)pvDest = *(BOOL*)pvSrc;
			break;

		case VT_BSTR:
			*(CString*)pvDest = *(CString*)pvSrc;
			break;

		case VT_CY:
			*(CY*)pvDest = *(CY*)pvSrc;
			break;

		case VT_R4:
			memcpy( pvDest, pvSrc, sizeof(float) );
			// *(_AFXFLOAT FAR*)pvDest = *(_AFXFLOAT FAR*)pvSrc;
			break;

		case VT_R8:
			memcpy( pvDest, pvSrc, sizeof(double) );
			// *(_AFXDOUBLE FAR*)pvDest = *(_AFXDOUBLE FAR*)pvSrc;
			break;

		default:
			return FALSE;
		}
	}

	return (pvSrc != NULL);
}


/////////////////////////////////////////////////////////////////////////////
// COleControl::ExchangeExtent

BOOL COleControl::ExchangeExtent(CPropExchange* pPX)
{
	// Save extent
	SIZEL szl;
	szl.cx = m_cxExtent;
	szl.cy = m_cyExtent;

	if (PX_Long(pPX, _T("_ExtentX"), szl.cx) &&
		PX_Long(pPX, _T("_ExtentY"), szl.cy))
	{
		if ((pPX->IsLoading()) &&
			((m_cxExtent != szl.cx) || (m_cyExtent != szl.cy)))
		{
			m_xOleObject.SetExtent(DVASPECT_CONTENT, &szl);
		}

		return TRUE;
	}

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// COleControl::ExchangeVersion

BOOL COleControl::ExchangeVersion(CPropExchange* pPX, DWORD dwVersionDefault,
	BOOL bConvert)
{
	return pPX->ExchangeVersion(m_dwVersionLoaded, dwVersionDefault, bConvert);
}


/////////////////////////////////////////////////////////////////////////////
// CPropExchange member functions

CPropExchange::CPropExchange() :
	m_dwVersion(0)
{
}


BOOL CPropExchange::IsLoading()
{
	return m_bLoading;
}


DWORD CPropExchange::GetVersion()
{
	return m_dwVersion;
}


BOOL CPropExchange::ExchangeVersion(DWORD& dwVersionLoaded,
	DWORD dwVersionDefault, BOOL bConvert)
{
	BOOL bSuccess;

	if (m_bLoading)
	{
		bSuccess = PX_ULong(this, _T("_Version"), m_dwVersion,
			dwVersionDefault);
		dwVersionLoaded = m_dwVersion;
	}
	else
	{
		m_dwVersion = bConvert ? dwVersionDefault : dwVersionLoaded;
		bSuccess = PX_ULong(this, _T("_Version"), m_dwVersion);
	}

	return bSuccess;
}


/////////////////////////////////////////////////////////////////////////////
// CArchivePropExchange member functions

CArchivePropExchange::CArchivePropExchange(CArchive& ar) :
	m_ar(ar)
{
	ASSERT_POINTER(&ar, CArchive);

	m_bLoading = m_ar.IsLoading();
}


BOOL CArchivePropExchange::ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
		void* pvProp, const void* pvDefault)
{
#ifndef _DEBUG
	UNUSED pszPropName;     // unused in retail builds
	UNUSED pvDefault;       // unused in retail builds
#endif

	ASSERT(AfxIsValidString(pszPropName));
	ASSERT(AfxIsValidAddress(pvProp, 1, FALSE));
	ASSERT((pvDefault == NULL) || AfxIsValidAddress(pvDefault, 1, FALSE));

	if (m_bLoading)
	{
		switch (vtProp)
		{
		case VT_I2:
			m_ar >> *(WORD*)pvProp;
			break;

		case VT_I4:
			m_ar >> *(long*)pvProp;
			break;

		case VT_BOOL:
			*(BOOL*)pvProp = 0;
			m_ar >> *(BYTE*)pvProp;
			break;

		case VT_BSTR:
			m_ar >> *(CString*)pvProp;
			break;

		case VT_CY:
			m_ar >> ((CY*)pvProp)->Lo;
			m_ar >> ((CY*)pvProp)->Hi;
			break;

		case VT_R4:
			m_ar >> *(float*)pvProp;
			break;

		case VT_R8:
			m_ar >> *(double*)pvProp;
			break;
		}
	}
	else
	{
		switch (vtProp)
		{
		case VT_I2:
			m_ar << *(WORD*)pvProp;
			break;

		case VT_I4:
			m_ar << *(long*)pvProp;
			break;

		case VT_BOOL:
			m_ar << *(BYTE*)pvProp;
			break;

		case VT_BSTR:
			m_ar << *(CString*)pvProp;
			break;

		case VT_CY:
			m_ar << ((CY*)pvProp)->Lo;
			m_ar << ((CY*)pvProp)->Hi;
			break;

		case VT_R4:
			m_ar << *(float*)pvProp;
			break;

		case VT_R8:
			m_ar << *(double*)pvProp;
			break;
		}
	}

	return TRUE;
}


BOOL CArchivePropExchange::ExchangeBlobProp(LPCTSTR pszPropName,
	HGLOBAL* phBlob, HGLOBAL hBlobDefault)
{
#ifndef _DEBUG
	UNUSED pszPropName;     // unused in retail builds
#endif
	UNUSED hBlobDefault;

	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(phBlob, HGLOBAL);

	LPSTREAM pstm = _AfxGetArchiveStream(m_ar);

	BOOL bSuccess = FALSE;
	long cb;

	if (m_bLoading)
	{
		if (*phBlob != NULL)
		{
			GlobalFree(*phBlob);
			*phBlob = NULL;
		}

		if (FAILED(pstm->Read((BYTE*)&cb, sizeof(cb), NULL)))
			cb = 0;

		ASSERT(cb >= 0);

		if (cb > 0)
		{
			*phBlob = GlobalAlloc(GMEM_MOVEABLE, sizeof(cb) + cb);
			if (*phBlob != NULL)
			{
				void* pvBlob = GlobalLock(*phBlob);
				if (pvBlob != NULL)
				{
					*(long*)pvBlob = cb;
					bSuccess = SUCCEEDED(pstm->Read(((BYTE*)pvBlob)+sizeof(cb),
						cb, NULL));
				}
				GlobalUnlock(*phBlob);
			}
		}
		else
		{
			bSuccess = (cb == 0);
		}
	}
	else
	{
		if (*phBlob != NULL)
		{
			void* pvBlob = GlobalLock(*phBlob);
			if (pvBlob != NULL)
			{
				cb = *(long*)pvBlob;
				ASSERT_BUFFER_VALID(pvBlob, sizeof(cb)+cb, FALSE);
				bSuccess = SUCCEEDED(pstm->Write(pvBlob,
					(ULONG)sizeof(cb) + cb, NULL));
				GlobalUnlock(*phBlob);
			}
		}
		if (!bSuccess)
		{
			m_ar << (ULONG)0;
		}
	}

	return bSuccess;
}


BOOL _AfxPeekAtClassIDInStream(LPSTREAM pstm, LPCLSID lpClassID)
{
	// Read the class ID, then restore the seek pointer.
	LARGE_INTEGER li;
	li.LowPart = (DWORD)(-(long)sizeof(CLSID));
	li.HighPart = -1;

	return (SUCCEEDED(ReadClassStm(pstm, lpClassID)) &&
		SUCCEEDED(pstm->Seek(li, STREAM_SEEK_CUR, NULL)));
}


BOOL CArchivePropExchange::ExchangePersistentProp(LPCTSTR pszPropName,
		LPUNKNOWN FAR* ppUnk, REFIID iid, LPUNKNOWN pUnkDefault)
{
#ifndef _DEBUG
	UNUSED pszPropName;     // unused in retail builds
#endif

	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(ppUnk, LPUNKNOWN);
	ASSERT_NULL_OR_POINTER(pUnkDefault, IUnknown);

	LPSTREAM pstm = _AfxGetArchiveStream(m_ar);

	BOOL bSuccess = FALSE;
	BYTE bFlag = 0x00;

	if (m_bLoading)
	{
		RELEASE(*ppUnk);
		*ppUnk = NULL;

		if (SUCCEEDED(pstm->Read(&bFlag, sizeof(bFlag), NULL)) &&
			(bFlag != 0xff))
		{
			CLSID clsid;
			if (_AfxPeekAtClassIDInStream(pstm, &clsid))
			{
				// If class ID is null, don't load anything.
				if (IsEqualCLSID(clsid, GUID_NULL))
				{
					// Skip past class ID.
					::ReadClassStm(pstm, &clsid);
					bSuccess = TRUE;
				}
				// Special case: load the picture directly.
				else if (IsEqualCLSID(clsid, CLSID_StdPicture))
				{
					bSuccess =
						SUCCEEDED(::ReadClassStm(pstm, &clsid)) &&
						SUCCEEDED(::OleLoadPicture(pstm, 0, FALSE, iid,
							(LPVOID FAR*)ppUnk));
				}
				// Load the object.
				else
				{
					bSuccess = SUCCEEDED(::OleLoadFromStream(pstm, iid,
						(LPVOID FAR*)ppUnk));
				}
			}
		}
		else
		{
			// Use default value.
			if (pUnkDefault != NULL)
			{
				bSuccess = SUCCEEDED(pUnkDefault->QueryInterface(iid,
					(LPVOID FAR*)ppUnk));
			}
			else
			{
				bSuccess = TRUE;
			}
		}
	}
	else
	{
		ASSERT_NULL_OR_POINTER(*ppUnk, IUnknown);

		LPPERSISTSTREAM pps = NULL;

		// Check if *ppUnk and pUnkDefault are the same thing.  If so, don't
		// bother saving the object; just write a special flag instead.

		LPUNKNOWN pI1 = NULL;

		if (*ppUnk == pUnkDefault)
		{
			bFlag = 0xff;
			pstm->Write(&bFlag, sizeof(bFlag), NULL);
			bSuccess = TRUE;
		}
		else if ((*ppUnk != NULL) && (pUnkDefault != NULL) &&
			SUCCEEDED((*ppUnk)->QueryInterface(iid, (LPVOID FAR*)&pI1)))
		{
			ASSERT_POINTER(pI1, IUnknown);

			LPUNKNOWN pI2 = NULL;

			if (SUCCEEDED(pUnkDefault->QueryInterface(iid, (LPVOID FAR*)&pI2)))
			{
				ASSERT_POINTER(pI2, IUnknown);

				if (pI1 == pI2)
				{
					bFlag = 0xff;
					pstm->Write(&bFlag, sizeof(bFlag), NULL);
					bSuccess = TRUE;
				}

				pI2->Release();
			}

			pI1->Release();
		}

		if (! bSuccess)
		{
			if (*ppUnk != NULL)
			{
				if (SUCCEEDED((*ppUnk)->QueryInterface(IID_IPersistStream,
					(LPVOID FAR*)&pps)))
				{
					ASSERT_POINTER(pps, IPersistStream);

					bSuccess =
						SUCCEEDED(pstm->Write(&bFlag, 1, NULL)) &&
						SUCCEEDED(::OleSaveToStream(pps, pstm));
					pps->Release();
				}
			}
			else
			{
				// If no object, write null class ID.
				CLSID clsid = GUID_NULL;
				bSuccess =
					SUCCEEDED(pstm->Write(&bFlag, 1, NULL)) &&
					SUCCEEDED(WriteClassStm(pstm, clsid));
			}
		}
	}

	if (!bSuccess)
		AfxThrowArchiveException(CArchiveException::generic);

	return TRUE;
}


BOOL CArchivePropExchange::ExchangeFontProp(LPCTSTR pszPropName,
		CFontHolder& font, const FONTDESC FAR* pFontDesc,
		LPFONTDISP pFontDispAmbient)
{
#ifndef _DEBUG
	UNUSED pszPropName;     // unused in retail builds
#endif

	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&font, CFontHolder);
	ASSERT_NULL_OR_POINTER(pFontDesc, FONTDESC);
	ASSERT_NULL_OR_POINTER(pFontDispAmbient, IFontDisp);

	LPSTREAM pstm = _AfxGetArchiveStream(m_ar);

	BOOL bSuccess = FALSE;
	BYTE bFlag = 0x00;
	LPFONT pFont;

	if (m_bLoading)
	{
		if (SUCCEEDED(pstm->Read(&bFlag, sizeof(bFlag), NULL)) &&
			(bFlag != 0xff))
		{
			pFont = _AfxCreateFontFromStream(pstm);
			if (pFont != NULL)
			{
				font.SetFont(pFont);
				bSuccess = TRUE;
			}
		}

		if (!bSuccess)
			font.InitializeFont(pFontDesc, pFontDispAmbient);
	}
	else
	{
		pFont = font.m_pFont;

		if (pFont != NULL)
		{
			// If same as ambient font, write 0xff for the flag
			LPFONT pFontAmbient;
			if ((pFontDispAmbient != NULL) &&
				SUCCEEDED(pFontDispAmbient->QueryInterface(IID_IFont,
					(LPVOID FAR*)&pFontAmbient)))
			{
				ASSERT_POINTER(pFontAmbient, IFont);
				if (pFont->IsEqual(pFontAmbient) == S_OK)
				{
					bFlag = 0xff;
					pstm->Write(&bFlag, sizeof(bFlag), NULL);
					bSuccess = TRUE;
				}
				RELEASE(pFontAmbient);
			}

			if (! bSuccess)
			{
				LPPERSISTSTREAM pps = NULL;

				if (SUCCEEDED(pFont->QueryInterface(IID_IPersistStream,
					(LPVOID FAR*)&pps)))
				{
					ASSERT_POINTER(pps, IPersistStream);
					bSuccess =
						SUCCEEDED(pstm->Write(&bFlag, sizeof(bFlag), NULL)) &&
						SUCCEEDED(::OleSaveToStream(pps, pstm));
					pps->Release();
				}
			}
		}

		if (! bSuccess)
		{
			bFlag = 0xff;
			pstm->Write(&bFlag, sizeof(bFlag), NULL);
			bSuccess = TRUE;
		}
	}

	return bSuccess;
}


/////////////////////////////////////////////////////////////////////////////
// CResetPropExchange member functions

CResetPropExchange::CResetPropExchange()
{
	m_bLoading = TRUE;
}


BOOL CResetPropExchange::ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
		void* pvProp, const void* pvDefault)
{
#ifndef _DEBUG
	UNUSED pszPropName;     // unused in retail builds
#endif

	ASSERT(AfxIsValidString(pszPropName));
	ASSERT(AfxIsValidAddress(pvProp, 1, FALSE));
	ASSERT((pvDefault == NULL) || AfxIsValidAddress(pvDefault, 1, FALSE));

	return _AfxCopyPropValue(vtProp, pvProp, pvDefault);
}

BOOL CResetPropExchange::ExchangeBlobProp(LPCTSTR pszPropName,
	HGLOBAL* phBlob, HGLOBAL hBlobDefault)
{
#ifndef _DEBUG
	UNUSED pszPropName;     // unused in retail builds
#endif

	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(phBlob, HGLOBAL);

	if (*phBlob != NULL)
	{
		GlobalFree(*phBlob);
		*phBlob = NULL;
	}

	if (hBlobDefault != NULL)
		_AfxCopyBlob(phBlob, hBlobDefault);

	return TRUE;
}

BOOL CResetPropExchange::ExchangePersistentProp(LPCTSTR pszPropName,
		LPUNKNOWN FAR* ppUnk, REFIID iid, LPUNKNOWN pUnkDefault)
{
#ifndef _DEBUG
	UNUSED pszPropName;     // unused in retail builds
#endif

	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(ppUnk, LPUNKNOWN);
	ASSERT_NULL_OR_POINTER(pUnkDefault, IUnknown);

	BOOL bSuccess = TRUE;

	RELEASE(*ppUnk);

	if (pUnkDefault != NULL)
	{
		bSuccess = SUCCEEDED(pUnkDefault->QueryInterface(iid,
			(LPVOID FAR*)ppUnk));
	}

	return bSuccess;
}

BOOL CResetPropExchange::ExchangeFontProp(
				LPCTSTR pszPropName,
				CFontHolder& font,
				const FONTDESC FAR* pFontDesc,
				LPFONTDISP pFontDispAmbient)
{
#ifndef _DEBUG
	UNUSED pszPropName;     // unused in retail builds
#endif

	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&font, CFontHolder);
	ASSERT_NULL_OR_POINTER(pFontDesc, FONTDESC);
	ASSERT_NULL_OR_POINTER(pFontDispAmbient, IFontDisp);

	font.InitializeFont(pFontDesc, pFontDispAmbient);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// PX_ functions

BOOL PX_Short(CPropExchange* pPX, LPCTSTR pszPropName, short& sValue)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&sValue, short);

	return pPX->ExchangeProp(pszPropName, VT_I2, &sValue);
}

BOOL PX_Short(CPropExchange* pPX, LPCTSTR pszPropName, short& sValue, short sDefault)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&sValue, short);

	return pPX->ExchangeProp(pszPropName, VT_I2, &sValue, &sDefault);
}

BOOL PX_UShort(CPropExchange* pPX, LPCTSTR pszPropName, USHORT& usValue)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&usValue, USHORT);

	return pPX->ExchangeProp(pszPropName, VT_I2, &usValue);
}

BOOL PX_UShort(CPropExchange* pPX, LPCTSTR pszPropName, USHORT& usValue,
	USHORT usDefault)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&usValue, USHORT);

	return pPX->ExchangeProp(pszPropName, VT_I2, &usValue, &usDefault);
}

BOOL PX_Long(CPropExchange* pPX, LPCTSTR pszPropName, long& lValue)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&lValue, long);

	return pPX->ExchangeProp(pszPropName, VT_I4, &lValue);
}

BOOL PX_Long(CPropExchange* pPX, LPCTSTR pszPropName, long& lValue, long lDefault)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&lValue, long);

	return pPX->ExchangeProp(pszPropName, VT_I4, &lValue, &lDefault);
}

BOOL PX_ULong(CPropExchange* pPX, LPCTSTR pszPropName, ULONG& ulValue)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&ulValue, ULONG);

	return pPX->ExchangeProp(pszPropName, VT_I4, &ulValue);
}

BOOL PX_ULong(CPropExchange* pPX, LPCTSTR pszPropName, ULONG& ulValue,
	ULONG ulDefault)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&ulValue, ULONG);

	return pPX->ExchangeProp(pszPropName, VT_I4, &ulValue, &ulDefault);
}

BOOL PX_Color(CPropExchange* pPX, LPCTSTR pszPropName, OLE_COLOR& clrValue)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&clrValue, OLE_COLOR);

	return pPX->ExchangeProp(pszPropName, VT_I4, &clrValue);
}

BOOL PX_Color(CPropExchange* pPX, LPCTSTR pszPropName, OLE_COLOR& clrValue,
	OLE_COLOR clrDefault)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&clrValue, OLE_COLOR);

	return pPX->ExchangeProp(pszPropName, VT_I4, &clrValue, &clrDefault);
}

BOOL PX_Bool(CPropExchange* pPX, LPCTSTR pszPropName, BOOL& bValue)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&bValue, BOOL);

	return pPX->ExchangeProp(pszPropName, VT_BOOL, &bValue);
}

BOOL PX_Bool(CPropExchange* pPX, LPCTSTR pszPropName, BOOL& bValue, BOOL bDefault)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&bValue, BOOL);

	return pPX->ExchangeProp(pszPropName, VT_BOOL, &bValue, &bDefault);
}

BOOL PX_String(CPropExchange* pPX, LPCTSTR pszPropName, CString& strValue)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&strValue, CString);

	return pPX->ExchangeProp(pszPropName, VT_BSTR, &strValue);
}

BOOL PX_String(CPropExchange* pPX, LPCTSTR pszPropName, CString& strValue, const CString& strDefault)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&strValue, CString);

	return pPX->ExchangeProp(pszPropName, VT_BSTR, &strValue, &strDefault);
}

BOOL PX_Currency(CPropExchange* pPX, LPCTSTR pszPropName, CY& cyValue)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&cyValue, CY);

	return pPX->ExchangeProp(pszPropName, VT_CY, &cyValue);
}

BOOL PX_Currency(CPropExchange* pPX, LPCTSTR pszPropName, CY& cyValue, CY cyDefault)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&cyValue, CY);

	return pPX->ExchangeProp(pszPropName, VT_CY, &cyValue, &cyDefault);
}

BOOL PX_Float(CPropExchange* pPX, LPCTSTR pszPropName, float& floatValue)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&floatValue, float);

	return pPX->ExchangeProp(pszPropName, VT_R4, &floatValue);
}

BOOL PX_Float(CPropExchange* pPX, LPCTSTR pszPropName, float& floatValue, float floatDefault)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&floatValue, float);

	return pPX->ExchangeProp(pszPropName, VT_R4, &floatValue, &floatDefault);
}

BOOL PX_Double(CPropExchange* pPX, LPCTSTR pszPropName, double& doubleValue)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&doubleValue, double);

	return pPX->ExchangeProp(pszPropName, VT_R8, &doubleValue);
}

BOOL PX_Double(CPropExchange* pPX, LPCTSTR pszPropName, double& doubleValue, double doubleDefault)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&doubleValue, double);

	return pPX->ExchangeProp(pszPropName, VT_R8, &doubleValue, &doubleDefault);
}

BOOL PX_Blob(CPropExchange* pPX, LPCTSTR pszPropName, HGLOBAL& hBlob,
	HGLOBAL hBlobDefault)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&hBlob, HGLOBAL);

	return pPX->ExchangeBlobProp(pszPropName, &hBlob, hBlobDefault);
}

BOOL PX_Font(
		CPropExchange* pPX,
		LPCTSTR pszPropName,
		CFontHolder& font,
		const FONTDESC FAR* pFontDesc,
		LPFONTDISP pFontDispAmbient)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&font, CFontHolder);
	ASSERT_NULL_OR_POINTER(pFontDesc, FONTDESC);
	ASSERT_NULL_OR_POINTER(pFontDispAmbient, IFontDisp);

	return pPX->ExchangeFontProp(pszPropName, font, pFontDesc, pFontDispAmbient);
}

BOOL PX_IUnknown(CPropExchange* pPX, LPCTSTR pszPropName, LPUNKNOWN& pUnk,
	REFIID iid, LPUNKNOWN pUnkDefault)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&pUnk, LPUNKNOWN);
	ASSERT_NULL_OR_POINTER(pUnk, IUnknown);
	ASSERT_NULL_OR_POINTER(pUnkDefault, IUnknown);

	return pPX->ExchangePersistentProp(pszPropName, &pUnk, iid, pUnkDefault);
}

BOOL PX_Picture(CPropExchange* pPX, LPCTSTR pszPropName, CPictureHolder& pict)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&pict, CPictureHolder);

	LPUNKNOWN& pUnk = (LPUNKNOWN&)pict.m_pPict;
	return PX_IUnknown(pPX, pszPropName, pUnk, IID_IPicture);
}

BOOL PX_Picture(CPropExchange* pPX, LPCTSTR pszPropName, CPictureHolder& pict,
	CPictureHolder& pictDefault)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&pict, CPictureHolder);

	LPUNKNOWN& pUnk = (LPUNKNOWN&)pict.m_pPict;
	return PX_IUnknown(pPX, pszPropName, pUnk, IID_IPicture, pictDefault.m_pPict);
}

BOOL PX_VBXFontConvert(CPropExchange* pPX, CFontHolder& font)
{
	ASSERT_POINTER(pPX, CPropExchange);
	ASSERT_POINTER(&font, CFontHolder);

	if (font.m_pFont != NULL)
	{
		CString strName;
		CY cySize;
		BOOL bFlag;

		if (PX_String(pPX, _T("FontName"), strName))
		{
			BSTR bstrName;
			bstrName = SysAllocString(strName);
			font.m_pFont->put_Name(bstrName);
			SysFreeString(bstrName);
		}

		if (PX_Currency(pPX, _T("FontSize"), cySize))
			font.m_pFont->put_Size(cySize);

		if (PX_Bool(pPX, _T("FontBold"), bFlag))
			font.m_pFont->put_Bold(bFlag);

		if (PX_Bool(pPX, _T("FontItalic"), bFlag))
			font.m_pFont->put_Italic(bFlag);

		if (PX_Bool(pPX, _T("FontUnderline"), bFlag))
			font.m_pFont->put_Underline(bFlag);

		if (PX_Bool(pPX, _T("FontStrikethru"), bFlag))
			font.m_pFont->put_Strikethrough(bFlag);
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
