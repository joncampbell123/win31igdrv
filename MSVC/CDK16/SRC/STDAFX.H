// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1994 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and the
// Books Online documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#ifndef _DEBUG
#pragma warning(disable:4702)   // inline expansion finds unreachable code
#endif

// Set data macros.  MFC data macros set to their internal values, since
// we will be linking them into the DLL with the controls code.

#ifdef _WIN32
	#define AFX_CORE_DATA   AFX_DATA_EXPORT
	#define AFX_CORE_DATADEF
	#define AFX_OLE_DATA    AFX_DATA_EXPORT
	#define AFX_OLE_DATADEF

	#define AFX_CTL_DATA    AFX_DATA_EXPORT
	#define AFX_CTL_DATAMOD
	#define AFX_CTL_DATADEF
#else
	#define AFXAPI_DATA     __based(__segname("_DATA"))
	#define AFXAPIEX_DATA   __based(__segname("_DATA"))

	#define AFX_CTL_DATA    __based(__segname("_DATA"))
	#define AFX_CTL_DATAMOD __based(__segname("_DATA"))
	#define AFX_CTL_DATADEF
#endif

#define CURSORFILES
#define _AFX_NOFORCE_LIBS

#include <afxctl.h>         // MFC support for OLE Custom Controls

#include <windowsx.h>

#include <afxdlgs.h>
#include <afxext.h>
#include <ctype.h>
#include <stdlib.h>

#include "oleimpl.h"
#include "ctlimpl.h"
#include "ctltypes.h"
#include "ctlpwnd.h"
#include "ctlpages.h"

#include <afxpriv.h>
#if defined(_WIN32)
#undef _istleadbyte
#include <afximpl.h>
#include <afxdisp.h>
#else
#include <auxdata.h>
#endif

#ifndef RELEASE
#define RELEASE(p)  if ((p) != NULL) { (p)->Release(); (p) = NULL; };
#endif

#ifndef ASSERT_POINTER
#define ASSERT_POINTER(p, itf) \
	ASSERT(((p) != NULL) && AfxIsValidAddress((p), sizeof(itf), FALSE))
#endif

#ifndef ASSERT_NULL_OR_POINTER
#define ASSERT_NULL_OR_POINTER(p, itf) \
	ASSERT(((p) == NULL) || AfxIsValidAddress((p), sizeof(itf), FALSE))
#endif

#undef AFX_DATADEF
#define AFX_DATADEF AFX_CTL_DATADEF
#undef AFXAPP_DATA
#define AFXAPP_DATA AFX_CTL_DATAMOD
