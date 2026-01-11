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


#define GetConnectionPtr(pTarget, pEntry) \
	(LPCONNECTIONPOINT)((char*)pTarget + pEntry->nOffset + \
			offsetof(CConnectionPoint, m_xConnPt))


/////////////////////////////////////////////////////////////////////////////
// CEnumConnections

class CEnumConnPoints : public CEnumArray
{
public:
	CEnumConnPoints(const void FAR* pvEnum, UINT nSize);
	~CEnumConnPoints();
	void AddConnPoint(LPCONNECTIONPOINT pConnPt);

protected:
	virtual BOOL OnNext(void FAR* pv);

	UINT m_nMaxSize;    // number of items allocated (>= m_nSize)

#ifdef _WIN32
	DECLARE_INTERFACE_MAP()
#endif
};

#ifdef _WIN32
BEGIN_INTERFACE_MAP(CEnumConnPoints, CEnumArray)
	INTERFACE_PART(CEnumConnPoints, IID_IEnumConnectionPoints, EnumVOID)
END_INTERFACE_MAP()
#endif


CEnumConnPoints::CEnumConnPoints(const void FAR* pvEnum, UINT nSize) :
#ifdef _WIN32
	CEnumArray(sizeof(LPCONNECTIONPOINT), pvEnum, nSize, TRUE)
#else
	CEnumArray(sizeof(LPCONNECTIONPOINT), IID_IEnumConnectionPoints,
		pvEnum, nSize, TRUE)
#endif
{
	m_nMaxSize = 0;
}


CEnumConnPoints::~CEnumConnPoints()
{
	if (m_pClonedFrom == NULL)
	{
		UINT iCP;
		LPCONNECTIONPOINT FAR* ppCP =
			(LPCONNECTIONPOINT FAR*)(VOID FAR *)m_pvEnum;
		for (iCP = 0; iCP < m_nSize; iCP++)
			RELEASE(ppCP[iCP]);
	}
	// destructor will free the actual array (if it was not a clone)
}


BOOL CEnumConnPoints::OnNext(void FAR* pv)
{
	if (!CEnumArray::OnNext(pv))
		return FALSE;

	// outgoing connection point needs to be AddRef'ed
	//  (the caller has responsibility to release it)

	((LPCONNECTIONPOINT)pv)->AddRef();
	return TRUE;
}


void CEnumConnPoints::AddConnPoint(LPCONNECTIONPOINT pConnPt)
{
	ASSERT(m_nSize <= m_nMaxSize);

	if (m_nSize == m_nMaxSize)
	{
		// not enough space for new item -- allocate more
		LPCONNECTIONPOINT FAR* pListNew = new LPCONNECTIONPOINT[m_nSize+2];
		m_nMaxSize += 2;
		if (m_nSize > 0)
			_fmemcpy(pListNew, m_pvEnum, m_nSize*sizeof(LPCONNECTIONPOINT));
		delete m_pvEnum;
#ifdef _WIN32
		m_pvEnum = (BYTE*)pListNew;
#else
		m_pvEnum = (char*)pListNew;
#endif
	}

	// add this item to the list
	ASSERT(m_nSize < m_nMaxSize);
	((LPCONNECTIONPOINT FAR*)m_pvEnum)[m_nSize] = pConnPt;
	pConnPt->AddRef();
	++m_nSize;
}


/////////////////////////////////////////////////////////////////////////////
// Connection map

BEGIN_CONNECTION_MAP_(COleControl)
	CONNECTION_PART(COleControl, IID_IPropertyNotifySink, PropConnPt)
END_CONNECTION_MAP()


/////////////////////////////////////////////////////////////////////////////
// CConnectionPoint

CConnectionPoint::CConnectionPoint() : m_pModuleState(_afxModuleAddrCurrent)
{
}


CConnectionPoint::~CConnectionPoint()
{
	int cConnections = m_Connections.GetSize();
	LPUNKNOWN pConnection;

	while (cConnections-- > 0)
	{
		pConnection = (LPUNKNOWN)m_Connections.GetAt(cConnections);
		if (pConnection != NULL)
			pConnection->Release();
	}
}


const CPtrArray* CConnectionPoint::GetConnections()
{
	ASSERT_VALID(this);
	return &m_Connections;
}


void CConnectionPoint::OnAdvise(BOOL bAdvise)
{
	ASSERT_VALID(this);

	// May be overridden by subclass.
	UNUSED bAdvise;
}


int CConnectionPoint::GetMaxConnections()
{
	ASSERT_VALID(this);

	// May be overridden by subclass.
	return -1;
}


STDMETHODIMP_(ULONG) CConnectionPoint::XConnPt::AddRef()
{
	// Delegate to our exported AddRef.
	METHOD_MANAGE_STATE(CConnectionPoint, ConnPt)
	return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) CConnectionPoint::XConnPt::Release()
{
	// Delegate to our exported Release.
	METHOD_MANAGE_STATE(CConnectionPoint, ConnPt)
	return (ULONG)pThis->ExternalRelease();
}


STDMETHODIMP CConnectionPoint::XConnPt::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_MANAGE_STATE(CConnectionPoint, ConnPt)
	ASSERT_POINTER(ppvObj, LPVOID);

	if (IsEqualIID(iid, IID_IUnknown) ||
		IsEqualIID(iid, IID_IConnectionPoint))
	{
		*ppvObj = this;
		AddRef();
		return NOERROR;
	}

	return ResultFromScode(E_NOINTERFACE);
}


STDMETHODIMP CConnectionPoint::XConnPt::GetConnectionInterface(IID FAR* pIID)
{
	METHOD_MANAGE_STATE(CConnectionPoint, ConnPt)
	ASSERT_POINTER(pIID, IID);

	*pIID = pThis->GetIID();
	return NOERROR;
}


STDMETHODIMP CConnectionPoint::XConnPt::GetConnectionPointContainer(
	IConnectionPointContainer FAR* FAR* ppCPC)
{
	METHOD_MANAGE_STATE(CConnectionPoint, ConnPt)
	ASSERT_POINTER(ppCPC, LPCONNECTIONPOINT);

	if ((*ppCPC = pThis->GetContainer()) != NULL)
	{
		return NOERROR;
	}

	return ResultFromScode(E_FAIL);
}


STDMETHODIMP CConnectionPoint::XConnPt::Advise(
	LPUNKNOWN pUnkSink, DWORD FAR* pdwCookie)
{
	METHOD_MANAGE_STATE(CConnectionPoint, ConnPt)
	ASSERT_POINTER(pUnkSink, IUnknown);
	ASSERT_POINTER(pdwCookie, DWORD);

	if (pUnkSink == NULL)
		return ResultFromScode(E_POINTER);

	LPUNKNOWN lpInterface;

	int cMaxConn = pThis->GetMaxConnections();
	if ((cMaxConn >=0) && (pThis->m_Connections.GetSize() == cMaxConn))
	{
		return ResultFromScode(CONNECT_E_ADVISELIMIT);
	}

	if (SUCCEEDED(pUnkSink->QueryInterface(pThis->GetIID(),
			(LPVOID FAR*)&lpInterface)))
	{
		pThis->m_Connections.Add(lpInterface);
		int iSink = pThis->m_Connections.GetSize() - 1;
		pThis->OnAdvise(TRUE);
		lpInterface = (LPUNKNOWN)pThis->m_Connections.GetAt(iSink);
		*pdwCookie = (DWORD)lpInterface;
		return NOERROR;
	}

	return ResultFromScode(E_NOINTERFACE);
}


STDMETHODIMP CConnectionPoint::XConnPt::Unadvise(DWORD dwCookie)
{
	METHOD_MANAGE_STATE(CConnectionPoint, ConnPt)

	LPUNKNOWN pUnkSink;

	int i;
	int cConnections = pThis->m_Connections.GetSize();
	for (i = 0; i < cConnections; i++)
	{
		pUnkSink = (LPUNKNOWN)(pThis->m_Connections.GetAt(i));
		if ((DWORD)pUnkSink == dwCookie)
		{
			pUnkSink->Release();
			pThis->m_Connections.RemoveAt(i);
			pThis->OnAdvise(FALSE);
			return NOERROR;
		}
	}

	return ResultFromScode(CONNECT_E_NOCONNECTION);
}


STDMETHODIMP CConnectionPoint::XConnPt::EnumConnections(
	LPENUMCONNECTIONS FAR* ppEnum)
{
	UNUSED ppEnum;
	METHOD_MANAGE_STATE(CConnectionPoint, ConnPt)
	return ResultFromScode(E_NOTIMPL);
}


/////////////////////////////////////////////////////////////////////////////
// COleControl::XProvideClassInfo


STDMETHODIMP_(ULONG) COleControl::XProvideClassInfo::AddRef()
{
	// Delegate to our exported AddRef.
	METHOD_MANAGE_STATE(COleControl, ProvideClassInfo)
	return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) COleControl::XProvideClassInfo::Release()
{
	// Delegate to our exported Release.
	METHOD_MANAGE_STATE(COleControl, ProvideClassInfo)
	return (ULONG)pThis->ExternalRelease();
}


STDMETHODIMP COleControl::XProvideClassInfo::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	// Delegate to our exported QueryInterface.
	METHOD_MANAGE_STATE(COleControl, ProvideClassInfo)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP COleControl::XProvideClassInfo::GetClassInfo(
	LPTYPEINFO FAR* ppTypeInfo)
{
	METHOD_MANAGE_STATE(COleControl, ProvideClassInfo)

	CLSID clsid;
	pThis->GetClassID(&clsid);

	return pThis->GetTypeInfoOfGuid(0, clsid, ppTypeInfo);
}


/////////////////////////////////////////////////////////////////////////////
// COleControl::GetConnectionPointContainer

LPCONNECTIONPOINTCONTAINER COleControl::GetConnectionPointContainer()
{
	LPCONNECTIONPOINTCONTAINER pCPC = NULL;
	if (SUCCEEDED((HRESULT)ExternalQueryInterface(&IID_IConnectionPointContainer,
			(LPVOID FAR*)&pCPC)))
	{
		ASSERT(pCPC != NULL);
	}

	return pCPC;
}


/////////////////////////////////////////////////////////////////////////////
// COleControl::XConnPtContainer


STDMETHODIMP_(ULONG) COleControl::XConnPtContainer::AddRef()
{
	// Delegate to our exported AddRef.
	METHOD_MANAGE_STATE(COleControl, ConnPtContainer)
	return (ULONG)pThis->ExternalAddRef();
}


STDMETHODIMP_(ULONG) COleControl::XConnPtContainer::Release()
{
	// Delegate to our exported Release.
	METHOD_MANAGE_STATE(COleControl, ConnPtContainer)
	return (ULONG)pThis->ExternalRelease();
}


STDMETHODIMP COleControl::XConnPtContainer::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	// Delegate to our exported QueryInterface.
	METHOD_MANAGE_STATE(COleControl, ConnPtContainer)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}


STDMETHODIMP COleControl::XConnPtContainer::EnumConnectionPoints(
	LPENUMCONNECTIONPOINTS FAR* ppEnum)
{
	METHOD_MANAGE_STATE(COleControl, ConnPtContainer)

	CEnumConnPoints* pEnum = NULL;

	TRY
	{
		pEnum = new CEnumConnPoints(NULL, 0);

		// Add event handler connection point (not in the map)
		pEnum->AddConnPoint((LPCONNECTIONPOINT)((char*)&pThis->m_xEventConnPt +
				offsetof(CConnectionPoint, m_xConnPt)));

		// walk the chain of connection maps
		AFX_CONNECTIONMAP FAR* pMap = pThis->GetConnectionMap();
		AFX_CONNECTIONMAP_ENTRY FAR* pEntry;

		while (pMap != NULL)
		{
			pEntry = pMap->pEntry;

			while (pEntry->piid != NULL)
			{
				pEnum->AddConnPoint(GetConnectionPtr(pThis, pEntry));
				++pEntry;
			}
			pMap = pMap->pMapBase;
		}
	}
	CATCH (CException, e)
	{
		delete pEnum;
		pEnum = NULL;
	}
	END_CATCH

	if (pEnum != NULL)
	{
		// create and return the IEnumConnectionPoints object
		*ppEnum = (IEnumConnectionPoints FAR*)&pEnum->m_xEnumVOID;
	}
	else
	{
		// no connection points: return NULL
		*ppEnum = NULL;
	}

	return (pEnum != NULL) ? NOERROR : ResultFromScode(CONNECT_E_NOCONNECTION);
}


LPCONNECTIONPOINT COleControl::GetConnectionHook(REFIID iid)
{
	if ((m_piidEvents != NULL) && IsEqualIID(iid, *m_piidEvents))
		return (LPCONNECTIONPOINT)((char*)&m_xEventConnPt +
				offsetof(CConnectionPoint, m_xConnPt));
	else
		return NULL;
}


STDMETHODIMP COleControl::XConnPtContainer::FindConnectionPoint(
	REFIID iid, LPCONNECTIONPOINT FAR* ppCP)
{
	METHOD_MANAGE_STATE(COleControl, ConnPtContainer)
	ASSERT(ppCP != NULL);

	if ((*ppCP = pThis->GetConnectionHook(iid)) != NULL)
	{
		(*ppCP)->AddRef();
		return NOERROR;
	}

	AFX_CONNECTIONMAP FAR* pMap = pThis->GetConnectionMap();
	AFX_CONNECTIONMAP_ENTRY FAR* pEntry;

	while (pMap != NULL)
	{
		pEntry = pMap->pEntry;

		while (pEntry->piid != NULL)
		{
			if (IsEqualIID(iid, *(IID*)(pEntry->piid)))
			{
				*ppCP = GetConnectionPtr(pThis, pEntry);
				(*ppCP)->AddRef();
				return NOERROR;
			}
			++pEntry;
		}
		pMap = pMap->pMapBase;
	}

	return ResultFromScode(E_NOINTERFACE);
}


/////////////////////////////////////////////////////////////////////////////
// COleControl::OnEventAdvise - called by XEventConnPt::OnAdvise

void COleControl::OnEventAdvise(BOOL)
{
	// May be overridden by subclass
}


/////////////////////////////////////////////////////////////////////////////
// COleControl::XEventConnPt

void COleControl::XEventConnPt::OnAdvise(BOOL bAdvise)
{
	METHOD_MANAGE_STATE(COleControl, EventConnPt)
	pThis->OnEventAdvise(bAdvise);
}


REFIID COleControl::XEventConnPt::GetIID()
{
	METHOD_MANAGE_STATE(COleControl, EventConnPt)
	if (pThis->m_piidEvents != NULL)
		return *(pThis->m_piidEvents);
	else
		return GUID_NULL;
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
