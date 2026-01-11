// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1994 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and the
// Books Online documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdctl.h"

#pragma warning(disable:4103)
#include <initguid.h>
#include <olectlid.h>
#pragma warning(default:4103)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

IMPLEMENT_DYNAMIC(COleControlModule, CWinApp)

/////////////////////////////////////////////////////////////////////////////
// COleControlModule::InitInstance

BOOL COleControlModule::InitInstance()
{
	COleObjectFactory::RegisterAll();
	SetDialogBkColor();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// COleControlModule::ExitInstance

int COleControlModule::ExitInstance()
{
#ifndef _WIN32
	if (_AfxGetAppData()->appOleState != NULL)
#endif
		COleObjectFactory::RevokeAll();
	return CWinApp::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
//  Module state

static AFX_MODULE_STATE NEAR g_sModuleData;
static AFX_MODULE_STATE* psSavePrevious = NULL;

AFX_MODULE_STATE* AFXAPI AfxGetControlModuleContext()
{
	return &g_sModuleData;
}

/////////////////////////////////////////////////////////////////////////////
// Library initialization and cleanup

#ifdef _WIN32
/////////////////////////////////////////////////////////////////////////////
// DllMain for control

static AFX_EXTENSION_MODULE controlDLL;

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		CWinApp* pApp = AfxGetApp();

		ASSERT(psSavePrevious != NULL); // ControlModuleInit no called!

		// fail early if can't get app state or thread state
		if (AfxGetAppState() == NULL || AfxGetThreadState() == NULL)
			goto failure;       // Init Failed

		// Initialize DLL's instance(/module) not the app's
		if (!AfxWinInit(hInstance, NULL, &afxChNil, 0))
		{
			AfxWinTerm();
			goto failure;       // Init Failed
		}

		// initialize the single instance DLL
		if (pApp != NULL && !pApp->InitInstance())
		{
			pApp->ExitInstance();
			AfxWinTerm();
			goto failure;       // Init Failed
		}

		if (!AfxInitExtensionModule(controlDLL, hInstance))
		{
			pApp->ExitInstance();
			AfxWinTerm();
			goto failure;       // Init Failed
		}
		else
		{
			// wire up this DLL into the resource chain
			//  (In the Win32 version it is OK to create this in DllMain)
			CDynLinkLibrary* pDLL = new CDynLinkLibrary(controlDLL);
			pDLL->m_bSystem = TRUE;
		}

		// Pop initialized module data back into our global.
		AfxPopModuleContext(psSavePrevious, TRUE);
		return 1;   // ok

failure:

		// Pop control's module data.
		AfxPopModuleContext(psSavePrevious, TRUE);
		return 0;   // failed
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		// Push this control's context, and leave it there until all
		// destructors have happened.

		psSavePrevious = AfxPushModuleContext(_afxModuleAddrThis);
		ASSERT(psSavePrevious != NULL);

		CWinApp* pApp = AfxGetApp();    // Push state before getting app.

#ifdef _DEBUG
		// Check for missing AfxLockTempMap calls
		if (AfxGetThreadState()->m_nTempMapLock != 0)
			TRACE1("Warning: Temp map lock count non-zero (%ld).\n",
				AfxGetThreadState()->m_nTempMapLock);
#endif

		if (pApp != NULL)
			pApp->ExitInstance();

		AFX_CORE_STATE* pCoreState = AfxGetCoreState();

		// free the DLL info blocks for this module.
		CDynLinkLibrary* pDLLNext;
		CDynLinkLibrary* pDLL = pCoreState->m_pFirstDLL;
		CDynLinkLibrary* pDLLEnd =
			AfxGetBaseModuleContext()->m_coreState.m_pFirstDLL;
		pCoreState->m_pFirstDLL = NULL;
		for (/*nothing*/; pDLL != pDLLEnd; pDLL = pDLLNext)
		{
			pDLLNext = pDLL->m_pNextDLL;    // save next pointer before delete
			delete pDLL;
		}

		// Terminate the library before destructors are called
		AfxWinTerm();
	}

	return 1;   // ok
}

#else   //!_WIN32
/////////////////////////////////////////////////////////////////////////////
// LibMain for the control.

static AFX_EXTENSION_MODULE NEAR controlDLL = { NULL, NULL };

extern "C"
int CALLBACK LibMain(HINSTANCE hInstance, WORD, WORD, LPSTR lpszCmdLine)
{
	ASSERT(psSavePrevious != NULL); // ControlModuleInit not called!

	// Initialize DLL's instance(/module) not the app's
	if (!AfxWinInit(hInstance, NULL, lpszCmdLine, 0))
		goto failure;       // Init Failed

	// initialize the single instance DLL
	if (AfxGetApp() != NULL && !AfxGetApp()->InitInstance())
	{
		AfxGetApp()->ExitInstance();
		goto failure;
	}

	// Shared initialization
	AfxInitExtensionModule(controlDLL, hInstance);
	ASSERT(_AfxGetAppData()->pFirstDLL != NULL);    // runtime should be first.
	new CDynLinkLibrary(controlDLL);   // will add to list

	// Pop initialized module data back into our global.
	AfxPopModuleContext(psSavePrevious, TRUE);
	return 1;   // ok

failure:

	AfxWinTerm();

	// Pop control's module data.
	AfxPopModuleContext(psSavePrevious, TRUE);
	return 0;   // failed
}

/////////////////////////////////////////////////////////////////////////////
//  _WEP for the control
//      Called by the C-runtime's WEP before global destructors.

extern "C" int CALLBACK _WEP(int)
{
	// Push this control's context, and leave it there until all
	// destructors have happened.

	psSavePrevious = AfxPushModuleContext(_afxModuleAddrThis);
	ASSERT(psSavePrevious != NULL);

	if (AfxGetApp() != NULL)
		AfxGetApp()->ExitInstance();

	// Now it is safe to cleanup the library.
	AfxWinTerm();

	return 0;
}
#endif //!_WIN32

#ifndef _WIN32
extern HRESULT AfxDllGetClassObject(REFCLSID, REFIID, LPVOID FAR*);
extern HRESULT AfxDllCanUnloadNow(void);
#endif

extern "C"
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	return AfxDllGetClassObject(rclsid, riid, ppv);
}

extern "C"
STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	return AfxDllCanUnloadNow();
}

/////////////////////////////////////////////////////////////////////////////
//  AFX_CTL_BOUNDARY
//      This is the object that gets constructed first, and destructed last.
//      I makes sure that the control is initialized correctly before any
//      other code is run, and also that its state stays loaded for complete
//      destruction.

class AFX_CTL_BOUNDARY
{
public:
	AFX_CTL_BOUNDARY();
	~AFX_CTL_BOUNDARY();

#ifndef _WIN32
private:
	HINSTANCE m_hExplicit;
#endif
};

AFX_CTL_BOUNDARY::AFX_CTL_BOUNDARY()
{
#ifndef _WIN32
	// Win16 has a bug that allows implicitly use DLLs to unload before the
	// user has unloaded fully.  So the boundary object explicitly references
	// the runtime DLL to keep it around for the life of the control.

#ifdef _DEBUG
	static TCHAR BASED_CODE szOleControlRuntimeLib[] = _T("OC25D.DLL");
#else
	static TCHAR BASED_CODE szOleControlRuntimeLib[] = _T("OC25.DLL");
#endif

	m_hExplicit = LoadLibrary(szOleControlRuntimeLib);
#endif

	// Push the control's module data into the AppData for initialization.
	AFX_MODULE_STATE* pModuleData = _afxModuleAddrThis;
	pModuleData->m_pID = pModuleData;
#ifdef _WIN32
	pModuleData->m_coreState.m_pFirstDLL =
		AfxGetBaseModuleContext()->m_coreState.m_pFirstDLL;
#else
	pModuleData->pFirstDLL = AfxGetBaseModuleContext()->pFirstDLL;
#endif

	psSavePrevious = AfxPushModuleContext(pModuleData);
	ASSERT(psSavePrevious != NULL);
}

AFX_CTL_BOUNDARY::~AFX_CTL_BOUNDARY()
{
#ifndef _WIN32
	// Delete the dialog brush for this control.
	HBRUSH hbr = _AfxGetAppData()->appDlgBkBrush;
	if (hbr != NULL)
	{
		::DeleteObject(hbr);
		_AfxGetAppData()->appDlgBkBrush = NULL;
	}
#endif

	// Do a full copy pop of the control's context for the dtor.
	AfxPopModuleContext(psSavePrevious, TRUE);

#ifndef _WIN32
	ASSERT(m_hExplicit > HINSTANCE_ERROR);  // Control shouldn't load without DLL.
	FreeLibrary(m_hExplicit);
#endif
}

#pragma warning(disable: 4147)  // disable warning about using init_seg
#pragma warning(disable: 4073)  // disable warning about using init_seg
#pragma init_seg(lib)

static AFX_CTL_BOUNDARY NEAR afxOleControlBoundary; // Bounding ctor/dtor.

// NOTE: Do NOT place any new object allocations after this point.  If you
//  do they will be constructed before all other objects -- probably not
//  what you want!

/////////////////////////////////////////////////////////////////////////////
