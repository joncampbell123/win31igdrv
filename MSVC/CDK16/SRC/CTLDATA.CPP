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

extern CLIPFORMAT _cfPersistPropset;
extern CLIPFORMAT _cfConvertVBX;

/////////////////////////////////////////////////////////////////////////////
// AfxOleMatchPropsetClipFormat

BOOL _AfxOleMatchPropsetClipFormat(CLIPFORMAT cfFormat, LPCLSID lpFmtID)
{
	if (cfFormat == _cfPersistPropset)
	{
		*lpFmtID = CLSID_PersistPropset;
		return TRUE;
	}

	if (cfFormat == _cfConvertVBX)
	{
		*lpFmtID = CLSID_ConvertVBX;
		return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// COleControl::XDataObject


STDMETHODIMP_(ULONG) COleControl::XDataObject::AddRef()
{
	//
	//  Delegate to our exported AddRef.
	//

	METHOD_MANAGE_STATE(COleControl, DataObject)
	return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) COleControl::XDataObject::Release()
{
	//
	//  Delegate to our exported Release.
	//

	METHOD_MANAGE_STATE(COleControl, DataObject)
	return (ULONG)pThis->ExternalRelease();
}


STDMETHODIMP COleControl::XDataObject::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	//
	//  Delegate to our exported QueryInterface.
	//

	METHOD_MANAGE_STATE(COleControl, DataObject)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP COleControl::XDataObject::GetData(LPFORMATETC pformatetcIn,
							LPSTGMEDIUM pmedium )
{
	METHOD_MANAGE_STATE(COleControl, DataObject)
	return pThis->m_dataSource.m_xDataObject.GetData(pformatetcIn, pmedium);
}


STDMETHODIMP COleControl::XDataObject::GetDataHere(LPFORMATETC pformatetc,
							LPSTGMEDIUM pmedium )
{
	METHOD_MANAGE_STATE(COleControl, DataObject)
	return pThis->m_dataSource.m_xDataObject.GetDataHere(pformatetc, pmedium);
}


STDMETHODIMP COleControl::XDataObject::QueryGetData(LPFORMATETC pformatetc )
{
	METHOD_MANAGE_STATE(COleControl, DataObject)
	return pThis->m_dataSource.m_xDataObject.QueryGetData(pformatetc);
}


STDMETHODIMP COleControl::XDataObject::GetCanonicalFormatEtc(LPFORMATETC pformatetc,
							LPFORMATETC pformatetcOut)
{
	METHOD_MANAGE_STATE(COleControl, DataObject)
	return pThis->m_dataSource.m_xDataObject.GetCanonicalFormatEtc(pformatetc, pformatetcOut);
}


STDMETHODIMP COleControl::XDataObject::SetData(LPFORMATETC pformatetc, STGMEDIUM FAR * pmedium,
							BOOL fRelease)
{
	METHOD_MANAGE_STATE(COleControl, DataObject)
	return pThis->m_dataSource.m_xDataObject.SetData(pformatetc, pmedium, fRelease);
}


STDMETHODIMP COleControl::XDataObject::EnumFormatEtc(DWORD dwDirection,
							LPENUMFORMATETC FAR* ppenumFormatEtc)
{
	METHOD_MANAGE_STATE(COleControl, DataObject)
	return pThis->m_dataSource.m_xDataObject.EnumFormatEtc(dwDirection, ppenumFormatEtc);
}


STDMETHODIMP COleControl::XDataObject::DAdvise(FORMATETC FAR* pFormatetc, DWORD advf,
					LPADVISESINK pAdvSink, DWORD FAR* pdwConnection)
{
	METHOD_MANAGE_STATE(COleControl, DataObject)

	*pdwConnection = 0;

	if (pThis->m_pDataAdviseHolder == NULL &&
		CreateDataAdviseHolder(&pThis->m_pDataAdviseHolder) != NOERROR)
	{
		return ResultFromScode(E_OUTOFMEMORY);
	}
	ASSERT(pThis->m_pDataAdviseHolder != NULL);
	return pThis->m_pDataAdviseHolder->Advise(this, pFormatetc, advf, pAdvSink,
		pdwConnection);
}


STDMETHODIMP COleControl::XDataObject::DUnadvise(DWORD dwConnection)
{
	METHOD_MANAGE_STATE(COleControl, DataObject)

	if (pThis->m_pDataAdviseHolder == NULL)
		return ResultFromScode(E_FAIL);

	ASSERT(pThis->m_pDataAdviseHolder != NULL);
	return pThis->m_pDataAdviseHolder->Unadvise(dwConnection);
}


STDMETHODIMP COleControl::XDataObject::EnumDAdvise(LPENUMSTATDATA FAR* ppenumAdvise)
{
	METHOD_MANAGE_STATE(COleControl, DataObject)

	*ppenumAdvise = NULL;

	if (pThis->m_pDataAdviseHolder == NULL)
		return ResultFromScode(E_FAIL);

	ASSERT(pThis->m_pDataAdviseHolder != NULL);
	return pThis->m_pDataAdviseHolder->EnumAdvise(ppenumAdvise);
}


/////////////////////////////////////////////////////////////////////////////
// COleControl::CControlDataSource

BOOL COleControl::CControlDataSource::OnRenderGlobalData(
	LPFORMATETC lpFormatEtc, HGLOBAL* phGlobal)
{
	ASSERT_VALID(this);
	COleControl* pCtrl = (COleControl*)
		((BYTE*)this - offsetof(COleControl, m_dataSource));

	return pCtrl->OnRenderGlobalData(lpFormatEtc, phGlobal);
		// Note: COleDataSource has no implementation
}

BOOL COleControl::CControlDataSource::OnRenderFileData(
	LPFORMATETC lpFormatEtc, CFile* pFile)
{
	ASSERT_VALID(this);
	COleControl* pCtrl = (COleControl*)
		((BYTE*)this - offsetof(COleControl, m_dataSource));

	return pCtrl->OnRenderFileData(lpFormatEtc, pFile);
		// Note: COleDataSource has no implementation
}

BOOL COleControl::CControlDataSource::OnRenderData(
	LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium)
{
	ASSERT_VALID(this);
	COleControl* pCtrl = (COleControl*)
		((BYTE*)this - offsetof(COleControl, m_dataSource));

	if (pCtrl->OnRenderData(lpFormatEtc, lpStgMedium))
		return TRUE;

	return COleDataSource::OnRenderData(lpFormatEtc, lpStgMedium);
}

BOOL COleControl::CControlDataSource::OnSetData(LPFORMATETC lpFormatEtc,
	LPSTGMEDIUM lpStgMedium, BOOL bRelease)
{
	ASSERT_VALID(this);
	COleControl* pCtrl = (COleControl*)
		((BYTE*)this - offsetof(COleControl, m_dataSource));

	return pCtrl->OnSetData(lpFormatEtc, lpStgMedium, bRelease);
		// Note: COleDataSource has no implementation
}


/////////////////////////////////////////////////////////////////////////////
// COleControl overridables for IDataObject implementation


BOOL COleControl::OnRenderGlobalData(LPFORMATETC lpFormatEtc, HGLOBAL* phGlobal)
{
#ifndef _DEBUG
	UNUSED lpFormatEtc; // unused in retail builds
	UNUSED phGlobal;    // unused in retail builds
#endif

	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));
	ASSERT(AfxIsValidAddress(phGlobal, sizeof(HGLOBAL)));

	return FALSE;   // default does nothing
}

BOOL COleControl::OnRenderFileData(LPFORMATETC lpFormatEtc, CFile* pFile)
{
#ifndef _DEBUG
	UNUSED lpFormatEtc; // unused in retail builds
	UNUSED pFile;       // unused in retail builds
#endif

	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC)));
	ASSERT_VALID(pFile);

	return FALSE;   // default does nothing
}

BOOL COleControl::OnRenderData(
	LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));
	ASSERT(AfxIsValidAddress(lpStgMedium, sizeof(STGMEDIUM)));

	// default implementation does not support extended layout
	if (lpFormatEtc->lindex != -1)
		return FALSE;

	// default implementation supports CF_METAFILEPICT
	if (lpFormatEtc->cfFormat == CF_METAFILEPICT)
		return GetMetafileData(lpFormatEtc, lpStgMedium);

	// default implementation supports propset format
	CLSID fmtid;
	if (_AfxOleMatchPropsetClipFormat(lpFormatEtc->cfFormat, &fmtid))
	{
		return GetPropsetData(lpFormatEtc, lpStgMedium, fmtid);
	}

	return FALSE;   // cfFormat not supported
}

BOOL COleControl::OnSetData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium,
	BOOL bRelease)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));
	ASSERT(AfxIsValidAddress(lpStgMedium, sizeof(STGMEDIUM)));

	// default implementation supports propset format
	BOOL bSuccess = FALSE;
	CLSID fmtid;
	if (_AfxOleMatchPropsetClipFormat(lpFormatEtc->cfFormat, &fmtid))
	{
		bSuccess = SetPropsetData(lpFormatEtc, lpStgMedium, fmtid);

		if (bSuccess && bRelease)
			ReleaseStgMedium(lpStgMedium);
	}

	return bSuccess;
}


/////////////////////////////////////////////////////////////////////////////
// COleControl::SetInitialDataFormats

void COleControl::SetInitialDataFormats()
{
	// by default a COleControl supports CF_METAFILEPICT
	FORMATETC formatEtc;
	formatEtc.cfFormat = CF_METAFILEPICT;
	formatEtc.ptd = NULL;
	formatEtc.dwAspect = DVASPECT_CONTENT;
	formatEtc.lindex = -1;
	formatEtc.tymed = TYMED_MFPICT;
	m_dataSource.DelayRenderData(0, &formatEtc);

	// by default a COleControl supports persistent propset format
	// (GetData or SetData)
	formatEtc.cfFormat = _cfPersistPropset;
	formatEtc.ptd = NULL;
	formatEtc.dwAspect = DVASPECT_CONTENT;
	formatEtc.lindex = -1;
	formatEtc.tymed = TYMED_ISTREAM | TYMED_ISTORAGE;
	m_dataSource.DelayRenderData(0, &formatEtc);
	m_dataSource.DelaySetData(0, &formatEtc);

	// by default a COleControl supports VBX conversion propset format
	// (SetData only)
	formatEtc.cfFormat = _cfConvertVBX;
	m_dataSource.DelaySetData(0, &formatEtc);
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
