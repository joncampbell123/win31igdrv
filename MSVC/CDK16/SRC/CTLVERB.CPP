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
// Signature for verb functions
typedef BOOL (AFX_MSG_CALL CCmdTarget::*AFX_PVERBFN)(LPMSG, HWND, LPCRECT);


/////////////////////////////////////////////////////////////////////////////
// Enumerator for OLEVERB


class CEnumOleVerb : public CEnumArray
{
public:
	CEnumOleVerb(const void FAR* pvEnum, UINT nSize) :

#ifdef _WIN32
		CEnumArray(sizeof(OLEVERB), pvEnum, nSize, TRUE) {}
#else
		CEnumArray(sizeof(OLEVERB), IID_IEnumOLEVERB, pvEnum, nSize, TRUE) {}
#endif

	~CEnumOleVerb();

protected:
	virtual BOOL OnNext(void FAR* pv);

#ifdef _WIN32
	DECLARE_INTERFACE_MAP()
#endif
};

#ifdef _WIN32
BEGIN_INTERFACE_MAP(CEnumOleVerb, CEnumArray)
	INTERFACE_PART(CEnumOleVerb, IID_IEnumOLEVERB, EnumVOID)
END_INTERFACE_MAP()
#endif

CEnumOleVerb::~CEnumOleVerb()
{
	if (m_pClonedFrom == NULL)
	{
		UINT iVerb;
		LPOLEVERB lpVerb = (LPOLEVERB)(VOID FAR *)m_pvEnum;
		for (iVerb = 0; iVerb < m_nSize; iVerb++)
			::_AfxCtlFreeString(lpVerb[iVerb].lpszVerbName, NULL);
	}
	// destructor will free the actual array (if it was not a clone)
}

BOOL CEnumOleVerb::OnNext(void FAR* pv)
{
	if (!CEnumArray::OnNext(pv))
		return FALSE;

	// outgoing OLEVERB requires the verb name to be copied
	//  (the caller has responsibility to free it)

	LPOLEVERB lpVerb = (LPOLEVERB)pv;
	if (lpVerb->lpszVerbName != NULL)
	{
		lpVerb->lpszVerbName = _AfxCtlCopyString(lpVerb->lpszVerbName, NULL);
		if (lpVerb->lpszVerbName == NULL)
			AfxThrowMemoryException();
	}
	// otherwise, copying worked...
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// OnEnumVerbs - Called by IOleObject::EnumVerbs

BOOL COleControl::OnEnumVerbs(LPENUMOLEVERB FAR* ppenumOleVerb)
{
	LPOLEVERB lpVerbList = NULL;
	LPOLEVERB lpVerbListNew;
	LPOLEVERB lpVerb;
	long nVerbs = 0;
	long nAlloc = 0;
	CString strVerbName;

	// walk the chain of message maps
	const AFX_MSGMAP* pMessageMap;

#ifdef _WIN32
	const
#endif
	AFX_MSGMAP_ENTRY FAR* lpEntry;

	for (pMessageMap = GetMessageMap();
		 (pMessageMap != NULL);
#ifdef _WIN32
		 pMessageMap = pMessageMap->pfnGetBaseMap())
#else
		 pMessageMap = pMessageMap->pBaseMessageMap)
#endif
	{
		// find all verb entries in the map that have non-negative IDs
		lpEntry = pMessageMap->lpEntries;
#ifdef _WIN32
		while ((lpEntry = AfxFindMessageEntry(lpEntry, 0xC002, 0, 1)) != NULL)
#else
		while ((lpEntry = _AfxFindMessageEntry(lpEntry, 0xC002, 1)) != NULL)
#endif
		{
			ASSERT(lpEntry != NULL);

			if (nVerbs == nAlloc)
			{
				// not enough space for new item -- allocate more
				lpVerbListNew = new OLEVERB[nVerbs + 10];
				nAlloc += 10;
				_fmemcpy(lpVerbListNew, lpVerbList, (size_t)(nVerbs * sizeof(OLEVERB)));
				delete [] lpVerbList;
				lpVerbList = lpVerbListNew;
			}

			// get the string for this item
			VERIFY(strVerbName.LoadString(lpEntry->nSig));
			ASSERT(strVerbName.GetLength() > 0);

			// add this item to the list
			ASSERT(nVerbs < nAlloc);
			lpVerb = &lpVerbList[nVerbs];
			lpVerb->lVerb = nVerbs;
			lpVerb->lpszVerbName =
				::_AfxCtlCopyString((LPTSTR)(LPCTSTR)strVerbName, NULL);
			lpVerb->fuFlags = 0;
			lpVerb->grfAttribs = OLEVERBATTRIB_ONCONTAINERMENU;
			++nVerbs;
			++lpEntry;
		}
	}

	if (nVerbs > 0)
	{
		// create and return the IEnumOLEVERB object
		CEnumOleVerb* pEnum = new CEnumOleVerb(lpVerbList, (UINT)nVerbs);
		*ppenumOleVerb = (IEnumOLEVERB FAR*)&pEnum->m_xEnumVOID;
	}
	else
	{
		// no verbs: return NULL
		*ppenumOleVerb = NULL;
	}

	return (nVerbs > 0);
}


/////////////////////////////////////////////////////////////////////////////
// OnDoVerb - Called by IOleObject::DoVerb

BOOL COleControl::OnDoVerb(LONG iVerb, LPMSG lpMsg, HWND hWndParent, LPCRECT lpRect)
{
#ifdef _WIN32
	const
#endif
	AFX_MSGMAP_ENTRY FAR* lpEntry = NULL;

	const AFX_MSGMAP* pMessageMap;
	long i = -1;

	for (pMessageMap = GetMessageMap();
		 (pMessageMap != NULL) && (lpEntry == NULL);
#ifdef _WIN32
		 pMessageMap = pMessageMap->pfnGetBaseMap())
#else
		 pMessageMap = pMessageMap->pBaseMessageMap)
#endif
	{
		if (iVerb < 0)  // Standard verb (negative index)
		{
#ifdef _WIN32
			lpEntry = AfxFindMessageEntry(pMessageMap->lpEntries, 0xC002, 0, (UINT)iVerb);
#else
			lpEntry = _AfxFindMessageEntry(pMessageMap->lpEntries, 0xC002, (UINT)iVerb);
#endif
		}
		else            // Non-standard verb (non-negative index)
		{
			lpEntry = pMessageMap->lpEntries;
#ifdef _WIN32
			while (((lpEntry = AfxFindMessageEntry(lpEntry, 0xC002, 0, 1)) != NULL) &&
#else
			while (((lpEntry = _AfxFindMessageEntry(lpEntry, 0xC002, 1)) != NULL) &&
#endif
				   (++i < iVerb))
			{
				++lpEntry;
			}
			ASSERT((lpEntry == NULL) || (i == iVerb));
		}
	}

	if (lpEntry == NULL)
		return FALSE;

	AFX_PVERBFN pfn = (AFX_PVERBFN)(lpEntry->pfn);
	if (! (this->*pfn)(lpMsg, hWndParent, lpRect))
		THROW (new CException);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
