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
// Calls to IPropertyNotifySink


void COleControl::BoundPropertyChanged(DISPID dispid)
{
	const CPtrArray* pConnections = m_xPropConnPt.GetConnections();
	ASSERT(pConnections != NULL);

	int i;
	int cConnections = pConnections->GetSize();
	LPPROPERTYNOTIFYSINK pPropNotifySink;

	for (i = 0; i < cConnections; i++)
	{
		pPropNotifySink = (LPPROPERTYNOTIFYSINK)(pConnections->GetAt(i));
		ASSERT(pPropNotifySink != NULL);
		pPropNotifySink->OnChanged(dispid);
	}
}


BOOL COleControl::BoundPropertyRequestEdit(DISPID dispid)
{
	const CPtrArray* pConnections = m_xPropConnPt.GetConnections();
	ASSERT(pConnections != NULL);

	int i;
	int cConnections = pConnections->GetSize();
	LPPROPERTYNOTIFYSINK pPropNotifySink;

	for (i = 0; i < cConnections; i++)
	{
		pPropNotifySink = (LPPROPERTYNOTIFYSINK)(pConnections->GetAt(i));
		ASSERT(pPropNotifySink != NULL);
		if (pPropNotifySink->OnRequestEdit(dispid) != NOERROR)
			return FALSE;
	}

	// All of the sinks said yes, so it's ok.
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// COleControl::SetNotPermitted - call when BoundPropertyRequestEdit fails

void COleControl::SetNotPermitted()
{
	ThrowError(CTL_E_SETNOTPERMITTED, AFX_IDP_SET_NOT_SUPPORTED);
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
