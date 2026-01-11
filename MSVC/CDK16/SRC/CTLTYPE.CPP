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

/////////////////////////////////////////////////////////////////////////////
//
// AfxOleTypeMatchGuid:  Tests whether a given TYPEDESC matches a type with a
// given GUID, when all aliases have been expanded.
//
// Example usage:
//
//  if (AfxOleTypeMatchGuid(pTypeInfo, &pVarDesc->elemdescVar.tdesc,
//              IID_IFontDisp, 1))
//  {
//      //  Property described by pVarDesc has type IFontDisp*
//  }

BOOL AFXAPI EXPORT AfxOleTypeMatchGuid(
		LPTYPEINFO pTypeInfo,
		TYPEDESC FAR* pTypeDesc,
		REFGUID guidType,
		ULONG cIndirectionLevels)
{
	ASSERT(pTypeInfo != NULL);
	ASSERT(pTypeDesc != NULL);
	ASSERT(cIndirectionLevels >= 0);

	LPTYPEINFO pTypeInfoRef = NULL;

	BOOL bMatch = FALSE;

	switch (pTypeDesc->vt)
	{
	case VT_USERDEFINED:
		//
		//  It's an alias:
		//  Expand the alias and try to match.
		//
		if (SUCCEEDED(pTypeInfo->GetRefTypeInfo(
									pTypeDesc->hreftype,
									&pTypeInfoRef)))
		{
			ASSERT(pTypeInfoRef != NULL);
			LPTYPEATTR pTypeAttr = NULL;
			if (SUCCEEDED(pTypeInfoRef->GetTypeAttr(&pTypeAttr)))
			{
				ASSERT(pTypeAttr != NULL);

				//
				//  If we've dereferenced the correct number of times,
				//  test the GUIDs for equality.
				//
				if (cIndirectionLevels == 0)
					bMatch = IsEqualGUID(pTypeAttr->guid, guidType);

				if ((bMatch == FALSE) &&
					(pTypeAttr->typekind == TKIND_ALIAS))
				{
					//
					//  GUIDs didn't match, but type expanded to another alias!
					//
					bMatch = AfxOleTypeMatchGuid(pTypeInfoRef,
									&pTypeAttr->tdescAlias, guidType,
									cIndirectionLevels);
				}

				pTypeInfoRef->ReleaseTypeAttr(pTypeAttr);
			}

			pTypeInfoRef->Release();
		}
		break;

	case VT_PTR:
		//
		//  It's a pointer:
		//  Dereference and try to match with one less level of indirection.
		//
		ASSERT(pTypeDesc->lptdesc != NULL);
		bMatch = AfxOleTypeMatchGuid(pTypeInfo, pTypeDesc->lptdesc, guidType,
						cIndirectionLevels - 1);
		break;
	}

	return bMatch;
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
