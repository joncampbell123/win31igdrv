// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1994 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and the
// Books Online documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

/////////////////////////////////////////////////////////////////////////////
// AFXCTL.H - MFC OLE Custom Control support

#ifndef __AFXCTL_H__
#define __AFXCTL_H__

// Classes declared in this file

//CWinApp
	class COleControlModule;        // Module housekeeping for an .OCX

//COleObjectFactory
	class COleObjectFactoryEx;      // Object factory with more reg support

class CFontHolder;                  // For manipulating font objects
class CPictureHolder;               // For manipulating picture objects

//CCmdTarget
	class CConnectionPoint;         // For exposing a connection point

//CWnd
	class COleControl;              // OLE Custom Control

//CDialog
	class COlePropertyPage;         // OLE Property page

class CPropExchange;                // Abstract base for property exchange


#ifdef __AFXWIN_H__
#error When including AFXCTL.H, do not also include AFXWIN.H
#endif

#ifdef __AFXOLE_H__
#error When including AFXCTL.H, do not also include AFXOLE.H
#endif

#ifdef __AFX_H__
#error When including AFXCTL.H, do not also include AFX.H
#endif

#ifdef __AFXDISP_H__
#error When including AFXCTL.H, do not also include AFXDISP.H
#endif

/////////////////////////////////////////////////////////////////////////////
// Win32 libraries - must appear before AFX.H is included

#ifdef _WIN32
#ifndef _AFX_NOFORCE_LIBS
	#ifdef _UNICODE
		#ifdef _DEBUG
			#pragma comment(lib, "oc30ud.lib")
			#pragma comment(lib, "ocs30ud.lib")
		#else
			#pragma comment(lib, "oc30u.lib")
			#pragma comment(lib, "ocs30u.lib")
		#endif
	#else
		#pragma comment(lib, "mfcans32.lib")

		#ifdef _DEBUG
			#pragma comment(lib, "oc30d.lib")
			#pragma comment(lib, "ocs30d.lib")
		#else
			#pragma comment(lib, "oc30.lib")
			#pragma comment(lib, "ocs30.lib")
		#endif

	#endif
#endif
#endif

#include <afxwin.h>
#include <afxole.h>
#include <olectl.h>

#if _MFC_VER < 0x0251
	#error 16 Bit OLE Custom Controls require MFC 2.51
	#error Please install Visual C++ 1.51
#endif

// Set structure packing
#ifdef _WIN32
#pragma pack(push, _AFX_PACKING)
#endif

/////////////////////////////////////////////////////////////////////////////
// Macros not defined in MFC 2.5

#ifndef _WIN32
#define INIT_INTERFACE_PART(theClass, localClass)
#endif

/////////////////////////////////////////////////////////////////////////////
// New parameter types

#define VTS_COLOR           VTS_I4      // OLE_COLOR
#define VTS_XPOS_PIXELS     VTS_I4      // OLE_XPOS_PIXELS
#define VTS_YPOS_PIXELS     VTS_I4      // OLE_YPOS_PIXELS
#define VTS_XSIZE_PIXELS    VTS_I4      // OLE_XSIZE_PIXELS
#define VTS_YSIZE_PIXELS    VTS_I4      // OLE_YSIZE_PIXELS
#define VTS_XPOS_HIMETRIC   VTS_I4      // OLE_XPOS_HIMETRIC
#define VTS_YPOS_HIMETRIC   VTS_I4      // OLE_YPOS_HIMETRIC
#define VTS_XSIZE_HIMETRIC  VTS_I4      // OLE_XSIZE_HIMETRIC
#define VTS_YSIZE_HIMETRIC  VTS_I4      // OLE_YSIZE_HIMETRIC
#define VTS_TRISTATE        VTS_I2      // OLE_TRISTATE
#define VTS_OPTEXCLUSIVE    VTS_BOOL    // OLE_OPTEXCLUSIVE

#define VTS_PCOLOR          VTS_PI4     // OLE_COLOR*
#define VTS_PXPOS_PIXELS    VTS_PI4     // OLE_XPOS_PIXELS*
#define VTS_PYPOS_PIXELS    VTS_PI4     // OLE_YPOS_PIXELS*
#define VTS_PXSIZE_PIXELS   VTS_PI4     // OLE_XSIZE_PIXELS*
#define VTS_PYSIZE_PIXELS   VTS_PI4     // OLE_YSIZE_PIXELS*
#define VTS_PXPOS_HIMETRIC  VTS_PI4     // OLE_XPOS_HIMETRIC*
#define VTS_PYPOS_HIMETRIC  VTS_PI4     // OLE_YPOS_HIMETRIC*
#define VTS_PXSIZE_HIMETRIC VTS_PI4     // OLE_XSIZE_HIMETRIC*
#define VTS_PYSIZE_HIMETRIC VTS_PI4     // OLE_YSIZE_HIMETRIC*
#define VTS_PTRISTATE       VTS_PI2     // OLE_TRISTATE*
#define VTS_POPTEXCLUSIVE   VTS_PBOOL   // OLE_OPTEXCLUSIVE*

#define VTS_FONT            VTS_DISPATCH    // IFontDispatch*
#define VTS_PICTURE         VTS_DISPATCH    // IPictureDispatch*

#ifdef _WIN32
#define VTS_HANDLE          VTS_I4      // OLE_HANDLE
#define VTS_PHANDLE         VTS_PI4     // OLE_HANDLE*
#else
#define VTS_HANDLE          VTS_I2      // OLE_HANDLE
#define VTS_PHANDLE         VTS_PI2     // OLE_HANDLE*
#endif

/////////////////////////////////////////////////////////////////////////////
//  Module state and data macros.

#ifndef _WIN32
	#undef AFX_MODULE_STATE
	#define AFX_MODULE_STATE AFX_APPDATA_MODULE
#endif

#ifndef _AFXCTL
	#undef AFX_MODULE_STATE
	#define AFX_MODULE_STATE void
#endif

#ifdef AFX_CTL_DATA
// Building OC.DLL.
#define _afxModuleAddrThis AfxGetBaseModuleContext()
#else // !AFX_CTL_DATA
// Building a control.
AFX_MODULE_STATE* AFXAPI AfxGetControlModuleContext();
#define _afxModuleAddrThis AfxGetControlModuleContext()

#ifdef _WIN32
	#define AFX_CTL_DATA        AFX_DATA_IMPORT
	#define AFX_CTL_DATAMOD
	#define AFX_CTL_DATADEF
#else
	#define AFX_CTL_DATA        _far
	#define AFX_CTL_DATAMOD     _far
	#define AFX_CTL_DATADEF
#endif
#endif  // !AFX_CTL_DATA

#ifdef _WIN32
#define AFX_CTL_DATA32 AFX_CTL_DATA
#define AFX_CTL_DATA16
#else
#define AFX_CTL_DATA32
#define AFX_CTL_DATA16 AFX_CTL_DATA
#endif

#ifndef AFX_DATADEF
#define AFX_DATADEF
#endif

/////////////////////////////////////////////////////////////////////////////
// COleControlModule - base class for .OCX module
//  This object is statically linked into the control.

class COleControlModule : public CWinApp
{
	DECLARE_DYNAMIC(COleControlModule)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
};

// MFC data definition for data exported from the runtime DLL

#undef AFX_DATA
#define AFX_DATA AFX_CTL_DATA32
#undef AFXAPP_DATA
#define AFXAPP_DATA AFX_CTL_DATA16

/////////////////////////////////////////////////////////////////////////////
// Connection maps

struct AFX_CONNECTIONMAP_ENTRY
{
	const void FAR* piid;   // the interface id (IID)
	size_t nOffset;         // offset of the interface vtable from m_unknown
};

struct AFX_CONNECTIONMAP
{
	AFX_CONNECTIONMAP FAR* pMapBase;     // NULL indicates root class
	AFX_CONNECTIONMAP_ENTRY FAR* pEntry; // map for this class
};

#define DECLARE_CONNECTION_MAP() \
private: \
	static AFX_DATA AFX_CONNECTIONMAP_ENTRY BASED_CODE _connectionEntries[]; \
protected: \
	static AFX_DATA AFX_CONNECTIONMAP BASED_CODE connectionMap; \
	virtual AFX_CONNECTIONMAP FAR* GetConnectionMap() const;

#define BEGIN_CONNECTION_PART(theClass, localClass) \
	class FAR X##localClass : public CConnectionPoint \
	{ \
	public: \
		X##localClass() \
			{ m_nOffset = offsetof(theClass, m_x##localClass); } \
		LPCONNECTIONPOINTCONTAINER GetContainer() \
			{ METHOD_MANAGE_STATE(theClass, localClass) \
				return pThis->GetConnectionPointContainer(); }

#define CONNECTION_IID(iid) \
		REFIID GetIID() { return iid; }

#define END_CONNECTION_PART(localClass) \
	} m_x##localClass; \
	friend class X##localClass;

#define BEGIN_CONNECTION_MAP(theClass, theBase) \
	AFX_CONNECTIONMAP FAR* theClass::GetConnectionMap() const \
		{ return &theClass::connectionMap; } \
	AFX_DATADEF AFX_CONNECTIONMAP BASED_CODE theClass::connectionMap = \
		{ &(theBase::connectionMap), theClass::_connectionEntries, }; \
	AFX_DATADEF AFX_CONNECTIONMAP_ENTRY BASED_CODE theClass::_connectionEntries[] = \
	{

#define BEGIN_CONNECTION_MAP_(theClass) \
	AFX_CONNECTIONMAP FAR* theClass::GetConnectionMap() const \
		{ return &theClass::connectionMap; } \
	AFX_DATADEF AFX_CONNECTIONMAP BASED_CODE theClass::connectionMap = \
		{ NULL, theClass::_connectionEntries, }; \
	AFX_DATADEF AFX_CONNECTIONMAP_ENTRY BASED_CODE theClass::_connectionEntries[] = \
	{

#define CONNECTION_PART(theClass, iid, localClass) \
		{ &iid, offsetof(theClass, m_x##localClass) }, \

#define END_CONNECTION_MAP() \
		{ NULL, (size_t)-1 } \
	}; \


/////////////////////////////////////////////////////////////////////////////
// CConnectionPoint

class CConnectionPoint : public CCmdTarget
{
// Constructors
public:
	CConnectionPoint();

// Operations
	const CPtrArray* GetConnections();

// Overridables
	virtual LPCONNECTIONPOINTCONTAINER GetContainer() = 0;
	virtual REFIID GetIID() = 0;
	virtual void OnAdvise(BOOL bAdvise);
	virtual int GetMaxConnections();

// Implementation
	~CConnectionPoint();

protected:
	size_t m_nOffset;
	AFX_MODULE_STATE* m_pModuleState;
	CPtrArray m_Connections;

// Interface Maps
public:
	BEGIN_INTERFACE_PART(ConnPt, IConnectionPoint)
		INIT_INTERFACE_PART(CConnectionPoint, ConnPt)
		STDMETHOD(GetConnectionInterface)(IID FAR* pIID);
		STDMETHOD(GetConnectionPointContainer)(
			IConnectionPointContainer FAR* FAR* ppCPC);
		STDMETHOD(Advise)(LPUNKNOWN pUnkSink, DWORD FAR* pdwCookie);
		STDMETHOD(Unadvise)(DWORD dwCookie);
		STDMETHOD(EnumConnections)(LPENUMCONNECTIONS FAR* ppEnum);
	END_INTERFACE_PART(ConnPt)
};


/////////////////////////////////////////////////////////////////////////////
// Connection helper functions

BOOL AFXAPI AfxConnectionAdvise(LPUNKNOWN pUnkSrc, REFIID iid,
	LPUNKNOWN pUnkSink, BOOL bRefCount, DWORD FAR* pdwCookie);

BOOL AFXAPI AfxConnectionUnadvise(LPUNKNOWN pUnkSrc, REFIID iid,
	LPUNKNOWN pUnkSink, BOOL bRefCount, DWORD dwCookie);


/////////////////////////////////////////////////////////////////////////////
// Event maps

enum AFX_EVENTMAP_FLAGS
{
	afxEventCustom = 0,
	afxEventStock = 1,
};

struct AFX_EVENTMAP_ENTRY
{
	AFX_EVENTMAP_FLAGS flags;
	DISPID dispid;
	LPTSTR pszName;
	BYTE pbParams[16];
};

struct AFX_EVENTMAP
{
	AFX_EVENTMAP FAR* lpBaseEventMap;
	AFX_EVENTMAP_ENTRY FAR* lpEntries;
};

#define DECLARE_EVENT_MAP() \
private: \
	static AFX_DATA AFX_EVENTMAP_ENTRY BASED_CODE _eventEntries[]; \
protected: \
	static AFX_DATA AFX_EVENTMAP BASED_CODE eventMap; \
	virtual AFX_EVENTMAP FAR* GetEventMap() const;

#define BEGIN_EVENT_MAP(theClass, baseClass) \
	AFX_EVENTMAP FAR* theClass::GetEventMap() const \
	{ return &eventMap; } \
	AFX_DATADEF AFX_EVENTMAP BASED_CODE theClass::eventMap = \
		{ &(baseClass::eventMap), theClass::_eventEntries }; \
	AFX_DATADEF AFX_EVENTMAP_ENTRY BASED_CODE theClass::_eventEntries[] = \
	{

#define END_EVENT_MAP() \
		{ afxEventCustom, DISPID_UNKNOWN, NULL, NULL }, \
	};

#define EVENT_CUSTOM(pszName, pfnFire, vtsParams) \
	{ afxEventCustom, DISPID_UNKNOWN, _T(pszName), vtsParams },

#define EVENT_CUSTOM_ID(pszName, dispid, pfnFire, vtsParams) \
	{ afxEventCustom, dispid, _T(pszName), vtsParams },

#define EVENT_PARAM(vtsParams) (BYTE FAR*)(vtsParams)


/////////////////////////////////////////////////////////////////////////////
// Stock events

#define EVENT_STOCK_CLICK() \
	{ afxEventStock, DISPID_CLICK, _T("Click"), VTS_NONE },

#define EVENT_STOCK_DBLCLICK() \
	{ afxEventStock, DISPID_DBLCLICK, _T("DblClick"), VTS_NONE },

#define EVENT_STOCK_KEYDOWN() \
	{ afxEventStock, DISPID_KEYDOWN, _T("KeyDown"), VTS_PI2 VTS_I2 },

#define EVENT_STOCK_KEYPRESS() \
	{ afxEventStock, DISPID_KEYPRESS, _T("KeyPress"), VTS_PI2 },

#define EVENT_STOCK_KEYUP() \
	{ afxEventStock, DISPID_KEYUP, _T("KeyUp"), VTS_PI2 VTS_I2 },

#define EVENT_STOCK_MOUSEDOWN() \
	{ afxEventStock, DISPID_MOUSEDOWN, _T("MouseDown"), \
	  VTS_I2 VTS_I2 VTS_XPOS_PIXELS VTS_YPOS_PIXELS },

#define EVENT_STOCK_MOUSEMOVE() \
	{ afxEventStock, DISPID_MOUSEMOVE, _T("MouseMove"), \
	  VTS_I2 VTS_I2 VTS_XPOS_PIXELS VTS_YPOS_PIXELS },

#define EVENT_STOCK_MOUSEUP() \
	{ afxEventStock, DISPID_MOUSEUP, _T("MouseUp"), \
	  VTS_I2 VTS_I2 VTS_XPOS_PIXELS VTS_YPOS_PIXELS },

#define EVENT_STOCK_ERROREVENT() \
	{ afxEventStock, DISPID_ERROREVENT, _T("Error"), \
	  VTS_I2 VTS_PBSTR VTS_SCODE VTS_BSTR VTS_BSTR VTS_I4 VTS_PBOOL },

// Shift state values for mouse and keyboard events
#define SHIFT_MASK      0x01
#define CTRL_MASK       0x02
#define ALT_MASK        0x04

// Button values for mouse events
#define LEFT_BUTTON     0x01
#define RIGHT_BUTTON    0x02
#define MIDDLE_BUTTON   0x04

/////////////////////////////////////////////////////////////////////////////
// Stock properties

#define VTS_STOCK "\0Stock Property"

#define DISP_STOCKPROP_BACKCOLOR() \
	DISP_PROPERTY_PARAM_ID(COleControl, "BackColor", DISPID_BACKCOLOR, \
			COleControl::GetBackColor, COleControl::SetBackColor, \
			VT_COLOR, VTS_STOCK)

#define DISP_STOCKPROP_BORDERSTYLE() \
	DISP_PROPERTY_PARAM_ID(COleControl, "BorderStyle", DISPID_BORDERSTYLE, \
			COleControl::GetBorderStyle, COleControl::SetBorderStyle, \
			VT_I2, VTS_STOCK)

#define DISP_STOCKPROP_CAPTION() \
	DISP_PROPERTY_PARAM_ID(COleControl, "Caption", DISPID_CAPTION, \
			COleControl::GetText, COleControl::SetText, VT_BSTR, VTS_STOCK)

#define DISP_STOCKPROP_ENABLED() \
	DISP_PROPERTY_PARAM_ID(COleControl, "Enabled", DISPID_ENABLED, \
			COleControl::GetEnabled, COleControl::SetEnabled, \
			VT_BOOL, VTS_STOCK)

#define DISP_STOCKPROP_FONT() \
	DISP_PROPERTY_PARAM_ID(COleControl, "Font", DISPID_FONT, \
			COleControl::GetFont, COleControl::SetFont, VT_FONT, VTS_STOCK)

#define DISP_STOCKPROP_FORECOLOR() \
	DISP_PROPERTY_PARAM_ID(COleControl, "ForeColor", DISPID_FORECOLOR, \
			COleControl::GetForeColor, COleControl::SetForeColor, \
			VT_COLOR, VTS_STOCK)

#define DISP_STOCKPROP_HWND() \
	DISP_PROPERTY_PARAM_ID(COleControl, "hWnd", DISPID_HWND, \
			COleControl::GetHwnd, SetNotSupported, VT_HANDLE, VTS_STOCK)

#define DISP_STOCKPROP_TEXT() \
	DISP_PROPERTY_PARAM_ID(COleControl, "Text", DISPID_TEXT, \
			COleControl::GetText, COleControl::SetText, VT_BSTR, VTS_STOCK)


/////////////////////////////////////////////////////////////////////////////
// Stock methods

#define DISP_STOCKFUNC_REFRESH() \
	DISP_FUNCTION_ID(COleControl, "Refresh", DISPID_REFRESH, \
			COleControl::Refresh, VT_EMPTY, VTS_NONE)

#define DISP_STOCKFUNC_DOCLICK() \
	DISP_FUNCTION_ID(COleControl, "DoClick", DISPID_DOCLICK, \
			COleControl::DoClick, VT_EMPTY, VTS_NONE)


/////////////////////////////////////////////////////////////////////////////
// Message map extensions for OLE verbs

#ifdef _WIN32

#define ON_STDOLEVERB(iVerb, memberFxn) \
	{ 0xC002, 0, (UINT)iVerb, (UINT)iVerb, (UINT)-1, \
		(AFX_PMSG)(BOOL (AFX_MSG_CALL CCmdTarget::*)(LPMSG, HWND, LPCRECT))memberFxn },

#define ON_OLEVERB(idsVerbName, memberFxn) \
	{ 0xC002, 0, 1, 1, idsVerbName, \
		(AFX_PMSG)(BOOL (AFX_MSG_CALL CCmdTarget::*)(LPMSG, HWND, LPCRECT))memberFxn },

#else // WIN16

#define ON_STDOLEVERB(iVerb, memberFxn) \
	{ 0xC002, (UINT)iVerb, (UINT)-1, \
		(AFX_PMSG)(BOOL (AFX_MSG_CALL CCmdTarget::*)(LPMSG, HWND, LPCRECT))memberFxn },

#define ON_OLEVERB(idsVerbName, memberFxn) \
	{ 0xC002, 1, idsVerbName, \
		(AFX_PMSG)(BOOL (AFX_MSG_CALL CCmdTarget::*)(LPMSG, HWND, LPCRECT))memberFxn },

#endif

/////////////////////////////////////////////////////////////////////////////
// COleObjectFactoryEx - base class for extended object factories

class COleObjectFactoryEx : public COleObjectFactory
{
	DECLARE_DYNAMIC(COleObjectFactoryEx)

// Constructors
public:
	COleObjectFactoryEx(REFCLSID clsid, CRuntimeClass* pRuntimeClass,
		BOOL bMultiInstance, LPCTSTR lpszProgID);

// Operations
	static BOOL PASCAL UpdateRegistryAll(BOOL bRegister);
	COleObjectFactory* GetNextFactory();
	BOOL IsLicenseValid();

// Overridables
	virtual BOOL UpdateRegistry(BOOL bRegister) = 0;
	virtual BOOL VerifyUserLicense();
	virtual BOOL GetLicenseKey(DWORD dwReserved, BSTR FAR* pbstrKey);
	virtual BOOL VerifyLicenseKey(BSTR bstrKey);

// Implementation
protected:
	AFX_MODULE_STATE* m_pModuleState;
	BYTE m_bLicenseChecked;
	BYTE m_bLicenseValid;

// Interface Maps
	BEGIN_INTERFACE_PART(ClassFactory2, IClassFactory2)
		INIT_INTERFACE_PART(COleObjectFactoryEx, ClassFactory2)
		STDMETHOD(CreateInstance)(LPUNKNOWN, REFIID, LPVOID FAR*);
		STDMETHOD(LockServer)(BOOL);
		STDMETHOD(GetLicInfo)(LPLICINFO);
		STDMETHOD(RequestLicKey)(DWORD, BSTR FAR*);
		STDMETHOD(CreateInstanceLic)(LPUNKNOWN, LPUNKNOWN, REFIID, BSTR,
			LPVOID FAR*);
	END_INTERFACE_PART(ClassFactory2)

	DECLARE_INTERFACE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// Macros for object factory and class ID

#define BEGIN_OLEFACTORY(class_name) \
protected: \
	class class_name##Factory : public COleObjectFactoryEx \
	{ \
	public: \
		class_name##Factory(REFCLSID clsid, CRuntimeClass* pRuntimeClass, \
			BOOL bMultiInstance, LPCTSTR lpszProgID) : \
				COleObjectFactoryEx(clsid, pRuntimeClass, bMultiInstance, \
				lpszProgID) {} \
		virtual BOOL UpdateRegistry(BOOL);

#define END_OLEFACTORY(class_name) \
	}; \
	friend class class_name##Factory; \
	static AFX_DATA class_name##Factory AFXAPP_DATA factory; \
public: \
	static AFX_DATA const GUID CDECL BASED_CODE guid; \
	virtual HRESULT GetClassID(LPCLSID pclsid);

#define DECLARE_OLECREATE_EX(class_name) \
	BEGIN_OLEFACTORY(class_name) \
	END_OLEFACTORY(class_name)

#define IMPLEMENT_OLECREATE_EX(class_name, external_name, \
			l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	static const TCHAR BASED_CODE _szProgID_##class_name[] = _T(external_name); \
	AFX_DATADEF class_name::class_name##Factory AFXAPP_DATA class_name::factory( \
		class_name::guid, RUNTIME_CLASS(class_name), FALSE, \
		_szProgID_##class_name); \
	const AFX_DATADEF GUID CDECL BASED_CODE class_name::guid = \
		{ l, w1, w2, { b1, b2, b3, b4, b5, b6, b7, b8 } }; \
	HRESULT class_name::GetClassID(LPCLSID pclsid) \
		{ *pclsid = guid; return NOERROR; }

/////////////////////////////////////////////////////////////////////////////
// Macros for type library information

class CTypeLibCache
{
public:
	static CTypeLibCache* PASCAL GetTypeLibCache(const GUID FAR* ptlid);

public:
	CTypeLibCache(const GUID FAR* ptlid);
	~CTypeLibCache();

	void Lock();
	void Unlock();
	BOOL Lookup(LCID lcid, LPTYPELIB FAR* pptlib);
	void Cache(LCID lcid, LPTYPELIB ptlib);
	BOOL LookupTypeInfo(LCID lcid, REFGUID guid, LPTYPEINFO FAR* pptinfo);
	void CacheTypeInfo(LCID lcid, REFGUID guid, LPTYPEINFO ptinfo);

protected:
	const GUID FAR* m_ptlid;
	LCID m_lcid;
	LPTYPELIB m_ptlib;
	GUID m_guidInfo;
	LPTYPEINFO m_ptinfo;
	long m_cRef;
};

#define DECLARE_OLETYPELIB(class_name) \
	protected: \
		virtual HRESULT GetTypeLib(LCID, LPTYPELIB FAR*); \
		virtual CTypeLibCache* GetTypeLibCache();

#define IMPLEMENT_OLETYPELIB(class_name, tlid, wVerMajor, wVerMinor) \
	HRESULT class_name::GetTypeLib(LCID lcid, LPTYPELIB FAR* ppTypeLib) \
	{ return ::LoadRegTypeLib(tlid, wVerMajor, wVerMinor, lcid, ppTypeLib); } \
	CTypeLibCache* class_name::GetTypeLibCache() \
	{ if (m_pTypeLibCache == NULL) \
		m_pTypeLibCache = CTypeLibCache::GetTypeLibCache(&tlid); \
	  return m_pTypeLibCache; }


/////////////////////////////////////////////////////////////////////////////
// Macros for type name and misc status

#define DECLARE_OLECTLTYPE(class_name) \
	virtual UINT GetUserTypeNameID(); \
	virtual DWORD GetMiscStatus();

#define IMPLEMENT_OLECTLTYPE(class_name, idsUserTypeName, dwOleMisc) \
	UINT class_name::GetUserTypeNameID() { return idsUserTypeName; } \
	DWORD class_name::GetMiscStatus() { return dwOleMisc; }


/////////////////////////////////////////////////////////////////////////////
// Macros for property page IDs

#define DECLARE_PROPPAGEIDS(class_name) \
	protected: \
		virtual LPCLSID GetPropPageIDs(ULONG& cPropPages);

#define BEGIN_PROPPAGEIDS(class_name, count) \
	static CLSID NEAR _rgPropPageIDs_##class_name[count]; \
	static ULONG NEAR _cPropPages_##class_name = (ULONG)-1; \
	LPCLSID class_name::GetPropPageIDs(ULONG& cPropPages) { \
		if (_cPropPages_##class_name == (ULONG)-1) { \
			_cPropPages_##class_name = count; \
			LPCLSID pIDs = _rgPropPageIDs_##class_name; \
			ULONG iPageMax = count; \
			ULONG iPage = 0;

#define PROPPAGEID(clsid) \
			ASSERT(iPage < iPageMax); \
			if (iPage < iPageMax) \
				pIDs[iPage++] = clsid;

#define END_PROPPAGEIDS(class_name) \
			ASSERT(iPage == iPageMax); \
		} \
		cPropPages = _cPropPages_##class_name; \
		return _rgPropPageIDs_##class_name; }


/////////////////////////////////////////////////////////////////////////////
// CFontHolder - helper class for dealing with font objects

class CFontHolder
{
// Constructors
public:
	CFontHolder(LPPROPERTYNOTIFYSINK pNotify);

// Attributes
	LPFONT m_pFont;

// Operations
	void InitializeFont(
			const FONTDESC FAR* pFontDesc = NULL,
			LPDISPATCH pFontDispAmbient = NULL);
	void SetFont(LPFONT pNewFont);
	void ReleaseFont();
	HFONT GetFontHandle();
	HFONT GetFontHandle(long cyLogical, long cyHimetric);
	CFont* Select(CDC* pDC, long cyLogical, long cyHimetric);
	BOOL GetDisplayString(CString& strValue);
	LPFONTDISP GetFontDispatch();
	void QueryTextMetrics(LPTEXTMETRIC lptm);

// Implementation
public:
	~CFontHolder();

protected:
	DWORD m_dwConnectCookie;
	LPPROPERTYNOTIFYSINK m_pNotify;
};


/////////////////////////////////////////////////////////////////////////////
// CPictureHolder - helper class for dealing with picture objects

class CPictureHolder
{
// Constructors
public:
	CPictureHolder();
	~CPictureHolder();

// Attributes
	LPPICTURE m_pPict;

// Operations
	BOOL CreateEmpty();

	BOOL CreateFromBitmap(UINT idResource);
	BOOL CreateFromBitmap(CBitmap* pBitmap, CPalette* pPal = NULL,
		BOOL bTransferOwnership = TRUE);
	BOOL CreateFromBitmap(HBITMAP hbm, HPALETTE hpal = NULL,
		BOOL bTransferOwnership = FALSE);

	BOOL CreateFromMetafile(HMETAFILE hmf, int xExt, int yExt,
		BOOL bTransferOwnership = FALSE);

	BOOL CreateFromIcon(UINT idResource);
	BOOL CreateFromIcon(HICON hIcon, BOOL bTransferOwnership = FALSE);

	short GetType();
	BOOL GetDisplayString(CString& strValue);
	LPPICTUREDISP GetPictureDispatch();
	void SetPictureDispatch(LPPICTUREDISP pDisp);
	void Render(CDC* pDC, const CRect& rcRender, const CRect& rcWBounds);
};


/////////////////////////////////////////////////////////////////////////////
// COleControl - base class for a control implemented in C++ with MFC

class COleControl : public CWnd
{
	DECLARE_DYNAMIC(COleControl)

// Constructors
public:
	COleControl();

// Operations

	// Initialization
	void SetInitialSize(int cx, int cy);
	void InitializeIIDs(const IID* piidPrimary, const IID* piidEvents);

	// Invalidating
	void InvalidateControl(LPCRECT lpRect = NULL);

	// Modified flag
	BOOL IsModified();
	void SetModifiedFlag(BOOL bModified = TRUE);

	// Drawing operations
	void DoSuperclassPaint(CDC* pDC, const CRect& rcBounds);

	// Property exchange
	BOOL ExchangeExtent(CPropExchange* pPX);
	void ExchangeStockProps(CPropExchange* pPX);
	BOOL ExchangeVersion(CPropExchange* pPX, DWORD dwVersionDefault,
		BOOL bConvert = TRUE);
	BOOL IsConvertingVBX();

	// Stock methods
	void Refresh();
	void DoClick();

	// Stock properties
	OLE_COLOR GetBackColor();
	void SetBackColor(OLE_COLOR);
	short GetBorderStyle();
	void SetBorderStyle(short);
	BOOL GetEnabled();
	void SetEnabled(BOOL);
	CFontHolder& InternalGetFont();
	LPFONTDISP GetFont();
	void SetFont(LPFONTDISP);
	OLE_COLOR GetForeColor();
	void SetForeColor(OLE_COLOR);
	OLE_HANDLE GetHwnd();
	const CString& InternalGetText();
	BSTR GetText();
	void SetText(LPCTSTR);

	// Using colors
	COLORREF TranslateColor(OLE_COLOR clrColor, HPALETTE hpal = NULL);

	// Using fonts
	CFont* SelectStockFont(CDC* pDC);
	CFont* SelectFontObject(CDC* pDC, CFontHolder& fontHolder);
	void GetStockTextMetrics(LPTEXTMETRIC lptm);
	void GetFontTextMetrics(LPTEXTMETRIC lptm, CFontHolder& fontHolder);

	// Client site access
	LPOLECLIENTSITE GetClientSite();

	// Generic ambient property access
	BOOL GetAmbientProperty(DISPID dispid, VARTYPE vtProp, void* pvProp);
	BOOL WillAmbientsBeValidDuringLoad();

	// Specific ambient properties
	OLE_COLOR AmbientBackColor();
	CString AmbientDisplayName();
	LPFONTDISP AmbientFont();
	OLE_COLOR AmbientForeColor();
	LCID AmbientLocaleID();
	CString AmbientScaleUnits();
	short AmbientTextAlign();
	BOOL AmbientUserMode();
	BOOL AmbientUIDead();
	BOOL AmbientShowGrabHandles();
	BOOL AmbientShowHatching();

	// Firing events
	void FireEvent(DISPID dispid, BYTE FAR* pbParams, ...);

	// Firing functions for stock events
	void FireKeyDown(USHORT* pnChar, short nShiftState);
	void FireKeyUp(USHORT* pnChar, short nShiftState);
	void FireKeyPress(USHORT* pnChar);
	void FireMouseDown(short nButton, short nShiftState,
		OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y);
	void FireMouseUp(short nButton, short nShiftState,
		OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y);
	void FireMouseMove(short nButton, short nShiftState,
		OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y);
	void FireClick();
	void FireDblClick();
	void FireError(SCODE scode, LPCTSTR lpszDescription, UINT nHelpID = 0);

	// Changing size and/or rectangle
	BOOL GetRectInContainer(LPRECT lpRect);
	BOOL SetRectInContainer(LPCRECT lpRect);
	void GetControlSize(int* pcx, int* pcy);
	BOOL SetControlSize(int cx, int cy);

	// Window management
	void RecreateControlWindow();

	// Modal dialog operations
	void PreModalDialog(HWND hWndParent = NULL);
	void PostModalDialog(HWND hWndParent = NULL);

	// Data binding operations
	void BoundPropertyChanged(DISPID dispid);
	BOOL BoundPropertyRequestEdit(DISPID dispid);

	// Dispatch exceptions
	void ThrowError(SCODE sc, UINT nDescriptionID, UINT nHelpID = -1);
	void ThrowError(SCODE sc, LPCTSTR pszDescription = NULL, UINT nHelpID = 0);
	void GetNotSupported();
	void SetNotSupported();
	void SetNotPermitted();

	// Communication with the control site
	void ControlInfoChanged();
	BOOL LockInPlaceActive(BOOL bLock);
	LPDISPATCH GetExtendedControl();
	void TransformCoords(POINTL FAR* lpptlHimetric,
		POINTF FAR* lpptfContainer, DWORD flags);

	// Simple frame
	void EnableSimpleFrame();

// Overridables
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual void OnDraw(
				CDC* pDC, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void OnDrawMetafile(CDC* pDC, const CRect& rcBounds);

	// Class ID (implemented by IMPLEMENT_OLECREATE_EX macro)
	virtual HRESULT GetClassID(LPCLSID pclsid) = 0;

	// For customizing the default messages on the status bar
	virtual void GetMessageString(UINT nID, CString& rMessage) const;

	// Display of error events to user
	virtual void DisplayError(SCODE scode, LPCTSTR lpszDescription,
		LPCTSTR lpszSource, LPCTSTR lpszHelpFile, UINT nHelpID);

	// IOleObject notifications
	virtual void OnSetClientSite();
	virtual BOOL OnSetExtent(LPSIZEL lpSizeL);

	// IOleInPlaceObject notifications
	virtual BOOL OnSetObjectRects(LPCRECT lpRectPos, LPCRECT lpRectClip);

	// Event connection point notifications
	virtual void OnEventAdvise(BOOL bAdvise);

	// Override to hook firing of Click event
	virtual void OnClick(USHORT iButton);

	// Override to get character after key events have been processed.
	virtual void OnKeyDownEvent(USHORT nChar, USHORT nShiftState);
	virtual void OnKeyUpEvent(USHORT nChar, USHORT nShiftState);
	virtual void OnKeyPressEvent(USHORT nChar);

	// Change notifications
	virtual void OnBackColorChanged();
	virtual void OnBorderStyleChanged();
	virtual void OnEnabledChanged();
	virtual void OnTextChanged();
	virtual void OnFontChanged();
	virtual void OnForeColorChanged();

	// IOleControl notifications
	virtual void OnGetControlInfo(LPCONTROLINFO pControlInfo);
	virtual void OnMnemonic(LPMSG pMsg);
	virtual void OnAmbientPropertyChange(DISPID dispid);
	virtual void OnFreezeEvents(BOOL bFreeze);

	// In-place activation
	virtual HMENU OnGetInPlaceMenu();
	virtual void OnShowToolBars();
	virtual void OnHideToolBars();

	// IViewObject
	virtual BOOL OnGetColorSet(DVTARGETDEVICE FAR* ptd, HDC hicTargetDev,
				LPLOGPALETTE FAR* ppColorSet);

	// IDataObject - see COleDataSource for a description of these overridables
	virtual BOOL OnRenderGlobalData(LPFORMATETC lpFormatEtc, HGLOBAL* phGlobal);
	virtual BOOL OnRenderFileData(LPFORMATETC lpFormatEtc, CFile* pFile);
	virtual BOOL OnRenderData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium);
	virtual BOOL OnSetData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium,
		BOOL bRelease);

	// Verbs
	virtual BOOL OnEnumVerbs(LPENUMOLEVERB FAR* ppenumOleVerb);
	virtual BOOL OnDoVerb(LONG iVerb, LPMSG lpMsg, HWND hWndParent, LPCRECT lpRect);
	virtual BOOL OnEdit(LPMSG lpMsg, HWND hWndParent, LPCRECT lpRect);
	virtual BOOL OnProperties(LPMSG lpMsg, HWND hWndParent, LPCRECT lpRect);

	// IPerPropertyBrowsing overrides
	virtual BOOL OnGetDisplayString(DISPID dispid, CString& strValue);
	virtual BOOL OnMapPropertyToPage(DISPID dispid, LPCLSID lpclsid,
		BOOL* pbPageOptional);
	virtual BOOL OnGetPredefinedStrings(DISPID dispid,
		CStringArray* pStringArray, CDWordArray* pCookieArray);
	virtual BOOL OnGetPredefinedValue(DISPID dispid, DWORD dwCookie,
		VARIANT FAR* lpvarOut);

	// Window procedure
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

// Implementation
	~COleControl();

#ifdef _DEBUG
	void AssertValid() const;
	void Dump(CDumpContext& dc) const;
#endif // _DEBUG

protected:
	// Friend classes
	friend class CReflectorWnd;
	friend class CControlFrameWnd;

	// Interface hook for primary automation interface
	LPUNKNOWN GetInterfaceHook(const void* piid);

	// Shutdown
	virtual void OnFinalRelease();

	// Window management
	BOOL CreateControlWindow(HWND hWndParent, const CRect& rcPos);
	void CreateWindowForSubclassedControl();
	BOOL IgnoreWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam,
		LRESULT* plResult);

	// Serialization
	HRESULT SaveState(IStream* pStm);
	HRESULT LoadState(IStream* pStm);
	virtual void Serialize(CArchive& ar);

	// Drawing
	void DrawContent(CDC* pDC, CRect& rc);
	void DrawMetafile(CDC* pDC, CRect& rc);
	BOOL GetMetafileData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium);

	// IDataObject formats
	void SetInitialDataFormats();
	BOOL GetPropsetData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium,
		REFCLSID fmtid);
	BOOL SetPropsetData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium,
		REFCLSID fmtid);

	// Type library
	virtual HRESULT GetTypeLib(LCID, LPTYPELIB FAR*);
	virtual CTypeLibCache* GetTypeLibCache();
	HRESULT GetTypeInfoOfGuid(LCID, REFGUID, LPTYPEINFO FAR*);

	// Connection point container
	LPCONNECTIONPOINTCONTAINER GetConnectionPointContainer();
	virtual LPCONNECTIONPOINT GetConnectionHook(REFIID iid);

	// Events
	static AFX_DATA AFX_EVENTMAP_ENTRY BASED_CODE _eventEntries[];
	virtual AFX_EVENTMAP FAR* GetEventMap() const;
	static AFX_DATA AFX_EVENTMAP BASED_CODE eventMap;
	AFX_EVENTMAP_ENTRY FAR* GetEventMapEntry(LPCTSTR pszName,
		DISPID* pDispid) const;
	void FireEventV(DISPID dispid, BYTE FAR* pbParams, va_list argList);

	// Stock events
	void KeyDown(USHORT* pnChar);
	void KeyUp(USHORT* pnChar);
	void ButtonDown(USHORT iButton, UINT nFlags, CPoint point);
	void ButtonUp(USHORT iButton, UINT nFlags, CPoint point);
	void ButtonDblClk(USHORT iButton, UINT nFlags, CPoint point);

	// Masks to identify which stock events and properties are used
	void InitStockEventMask();
	void InitStockPropMask();

	// Support for subclassing a Windows control
	BOOL ContainerReflectsMessages();
	CWnd* GetOuterWindow() const;       // m_pReflect if any, otherwise this
	void OnReflectorDestroyed();

	// Aggregation of default handler
	BOOL OnCreateAggregates();

	// State change notifications
	void SendAdvise(UINT uCode);

	// Non-in-place activation
	HRESULT OnOpen(BOOL bTryInPlace, LPMSG pMsg);
	void ResizeOpenControl(int cx, int cy);
	virtual CControlFrameWnd* CreateFrameWindow();
	void ResizeFrameWindow(int cx, int cy);
	void OnFrameClose();
	HRESULT OnHide();

	// In-place activation
	HRESULT OnActivateInPlace(BOOL bUIActivate, LPMSG pMsg);
	void ForwardActivationMsg(LPMSG pMsg);
	void AddFrameLevelUI();
	void RemoveFrameLevelUI();
	BOOL BuildSharedMenu();
	void DestroySharedMenu();

	// Property sheet
	virtual LPCLSID GetPropPageIDs(ULONG& cPropPages);

	// IOleObject implementation
	CString& GetUserType();
	virtual UINT GetUserTypeNameID() = 0;
	virtual DWORD GetMiscStatus() = 0;

	// Data members
	AFX_MODULE_STATE* m_pModuleState;   // Pointer to our module's state
	const IID* m_piidPrimary;           // IID for control automation
	const IID* m_piidEvents;            // IID for control events
	CTypeLibCache* m_pTypeLibCache;     // Type library cache
	DWORD m_dwVersionLoaded;            // Version number of loaded state
	COleDispatchDriver m_ambientDispDriver; // Driver for ambient properties
	DWORD m_dwStockEventMask;           // Which stock events are used?
	DWORD m_dwStockPropMask;            // Which stock properties are used?
	ULONG m_cEventsFrozen;              // Event freeze count (>0 means frozen)
	CControlFrameWnd* m_pWndOpenFrame;  // Open frame window.
	CString m_strFrameTitle;            // Title string for open frame window.
	CRect m_rcPos;                      // Control's position rectangle
	CRect m_rcBounds;                   // Bounding rectangle for drawing
	long m_cxExtent;                    // Control's width in HIMETRIC units
	long m_cyExtent;                    // Control's height in HIMETRIC units
	class CReflectorWnd* m_pReflect;    // Reflector window
	UINT m_nIDTracking;                 // Tracking command ID or string IDS
	UINT m_nIDLastMessage;              // Last displayed message string IDS
	BYTE m_bAutoMenuEnable;             // Disable menu items without handlers?
	BYTE m_bFinalReleaseCalled;         // Are we handling the final Release?
	BYTE m_bModified;                   // "Dirty" bit.
	BYTE m_bCountOnAmbients;            // Can we count on Ambients during load?
	BYTE m_iButtonState;                // Which buttons are down?
	BYTE m_iDblClkState;                // Which buttons involved in dbl click?
	BYTE m_bInPlaceActive;              // Are we in-place active?
	BYTE m_bUIActive;                   // Are we UI active?
	BYTE m_bPendingUIActivation;        // Are we about to become UI active?
	BYTE m_bOpen;                       // Are we open (non-in-place)?
	BYTE m_bChangingExtent;             // Extent is currently being changed
	BYTE m_bConvertVBX;                 // VBX conversion in progress
	BYTE m_bSimpleFrame;                // Simple frame support
	BYTE m_bTranslatingAccelerator;     // Currently inside TranslateAcclerator?
	BYTE m_bUIDead;                     // UIDead ambient property value

	// Stock properties
	OLE_COLOR m_clrBackColor;           // BackColor
	OLE_COLOR m_clrForeColor;           // ForeColor
	CString m_strText;                  // Text/Caption
	CFontHolder m_font;                 // Font
	HFONT m_hFontPrev;                  // Previously selected font object
	short m_sBorderStyle;               // BorderStyle
	BOOL m_bEnabled;                    // Enabled

	// Shared OLE menu data
	OLEMENUGROUPWIDTHS m_menuWidths;
	HMENU m_hSharedMenu;
	HOLEMENU m_hOleMenu;

	// Default Handler aggregation
	LPUNKNOWN m_pDefIUnknown;
	LPOLEOBJECT m_pDefIOleObject;
	LPPERSISTSTORAGE m_pDefIPersistStorage;
	LPVIEWOBJECT m_pDefIViewObject;
	LPOLECACHE m_pDefIOleCache;
#ifdef OLE2ANSI
	LPUNKNOWN m_pUnkWrapper;
#endif

	// OLE client site interfaces
	LPOLECLIENTSITE m_pClientSite;          // Client site
	LPOLEINPLACESITE m_pInPlaceSite;        // In-place site
	LPOLECONTROLSITE m_pControlSite;        // Control site
	LPOLEADVISEHOLDER m_pOleAdviseHolder;   // Advise holder
	LPDATAADVISEHOLDER m_pDataAdviseHolder; // Data advise holder
	LPSIMPLEFRAMESITE m_pSimpleFrameSite;   // Simple frame site

	// OLE in-place activation info
	LPOLEINPLACEFRAME m_pInPlaceFrame;
	OLEINPLACEFRAMEINFO m_frameInfo;
	LPOLEINPLACEUIWINDOW m_pInPlaceDoc;
	CString m_strUserTypeName;

	// Implementation of IDataObject
	// CControlDataSource implements OnRender reflections to COleControl
	class CControlDataSource : public COleDataSource
	{
	protected:
		virtual BOOL OnRenderGlobalData(LPFORMATETC lpFormatEtc, HGLOBAL* phGlobal);
		virtual BOOL OnRenderFileData(LPFORMATETC lpFormatEtc, CFile* pFile);
		virtual BOOL OnRenderData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium);

		virtual BOOL OnSetData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium,
			BOOL bRelease);
	};
	CControlDataSource m_dataSource;
	friend class CControlDataSource;

// Message Maps
protected:
	//{{AFX_MSG(COleControl)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnInitMenuPopup(CMenu*, UINT, BOOL);
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg LRESULT OnSetMessageString(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnterIdle(UINT nWhy, CWnd* pWho);
	afx_msg void OnCancelMode();
	afx_msg void OnPaint(CDC* pDC);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int  OnMouseActivate(CWnd *pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg  void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

#ifdef _WIN32
	afx_msg LRESULT OnOcmCtlColorBtn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOcmCtlColorDlg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOcmCtlColorEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOcmCtlColorListBox(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOcmCtlColorMsgBox(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOcmCtlColorScrollBar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOcmCtlColorStatic(WPARAM wParam, LPARAM lParam);
#else
	afx_msg LRESULT OnOcmCtlColor(WPARAM wParam, LPARAM lParam);
#endif
	DECLARE_MESSAGE_MAP()

// Interface Maps
protected:
	// IPersistStorage
	BEGIN_INTERFACE_PART(PersistStorage, IPersistStorage)
		INIT_INTERFACE_PART(COleControl, PersistStorage)
		STDMETHOD(GetClassID)(LPCLSID);
		STDMETHOD(IsDirty)();
		STDMETHOD(InitNew)(LPSTORAGE);
		STDMETHOD(Load)(LPSTORAGE);
		STDMETHOD(Save)(LPSTORAGE, BOOL);
		STDMETHOD(SaveCompleted)(LPSTORAGE);
		STDMETHOD(HandsOffStorage)();
	END_INTERFACE_PART(PersistStorage)

	// IPersistStreamInit
	BEGIN_INTERFACE_PART(PersistStreamInit, IPersistStreamInit)
		INIT_INTERFACE_PART(COleControl, PersistStreamInit)
		STDMETHOD(GetClassID)(LPCLSID);
		STDMETHOD(IsDirty)();
		STDMETHOD(Load)(LPSTREAM);
		STDMETHOD(Save)(LPSTREAM, BOOL);
		STDMETHOD(GetSizeMax)(ULARGE_INTEGER FAR *);
		STDMETHOD(InitNew)();
	END_INTERFACE_PART(PersistStreamInit)

	// IOleObject
	BEGIN_INTERFACE_PART(OleObject, IOleObject)
		INIT_INTERFACE_PART(COleControl, OleObject)
		STDMETHOD(SetClientSite)(LPOLECLIENTSITE);
		STDMETHOD(GetClientSite)(LPOLECLIENTSITE FAR*);
		STDMETHOD(SetHostNames)(LPCTSTR, LPCTSTR);
		STDMETHOD(Close)(DWORD);
		STDMETHOD(SetMoniker)(DWORD, LPMONIKER);
		STDMETHOD(GetMoniker)(DWORD, DWORD, LPMONIKER FAR*);
		STDMETHOD(InitFromData)(LPDATAOBJECT, BOOL, DWORD);
		STDMETHOD(GetClipboardData)(DWORD, LPDATAOBJECT FAR*);
		STDMETHOD(DoVerb)(LONG, LPMSG, LPOLECLIENTSITE, LONG, HWND, LPCRECT);
		STDMETHOD(EnumVerbs)(IEnumOLEVERB FAR* FAR*);
		STDMETHOD(Update)();
		STDMETHOD(IsUpToDate)();
		STDMETHOD(GetUserClassID)(CLSID FAR*);
		STDMETHOD(GetUserType)(DWORD, LPTSTR FAR*);
		STDMETHOD(SetExtent)(DWORD, LPSIZEL);
		STDMETHOD(GetExtent)(DWORD, LPSIZEL);
		STDMETHOD(Advise)(LPADVISESINK, LPDWORD);
		STDMETHOD(Unadvise)(DWORD);
		STDMETHOD(EnumAdvise)(LPENUMSTATDATA FAR*);
		STDMETHOD(GetMiscStatus)(DWORD, LPDWORD);
		STDMETHOD(SetColorScheme)(LPLOGPALETTE);
	END_INTERFACE_PART(OleObject)

	// IViewObject2
	BEGIN_INTERFACE_PART(ViewObject, IViewObject2)
		INIT_INTERFACE_PART(COleControl, ViewObject)
		STDMETHOD(Draw)(DWORD, LONG, void FAR*, DVTARGETDEVICE FAR*, HDC, HDC,
			LPCRECTL, LPCRECTL, BOOL (CALLBACK*)(DWORD), DWORD);
		STDMETHOD(GetColorSet)(DWORD, LONG, void FAR*, DVTARGETDEVICE FAR*,
			HDC, LPLOGPALETTE FAR*);
		STDMETHOD(Freeze)(DWORD, LONG, void FAR*, DWORD FAR*);
		STDMETHOD(Unfreeze)(DWORD);
		STDMETHOD(SetAdvise)(DWORD, DWORD, LPADVISESINK);
		STDMETHOD(GetAdvise)(DWORD FAR*, DWORD FAR*, LPADVISESINK FAR*);
		STDMETHOD(GetExtent) (DWORD, LONG, DVTARGETDEVICE FAR*, LPSIZEL);
	END_INTERFACE_PART(ViewObject)

	// IDataObject
	BEGIN_INTERFACE_PART(DataObject, IDataObject)
		INIT_INTERFACE_PART(COleControl, DataObject)
		STDMETHOD(GetData)(LPFORMATETC, LPSTGMEDIUM);
		STDMETHOD(GetDataHere)(LPFORMATETC, LPSTGMEDIUM);
		STDMETHOD(QueryGetData)(LPFORMATETC);
		STDMETHOD(GetCanonicalFormatEtc)(LPFORMATETC, LPFORMATETC);
		STDMETHOD(SetData)(LPFORMATETC, LPSTGMEDIUM, BOOL);
		STDMETHOD(EnumFormatEtc)(DWORD, LPENUMFORMATETC FAR*);
		STDMETHOD(DAdvise)(LPFORMATETC, DWORD, LPADVISESINK, LPDWORD);
		STDMETHOD(DUnadvise)(DWORD);
		STDMETHOD(EnumDAdvise)(LPENUMSTATDATA FAR*);
	END_INTERFACE_PART(DataObject)

	// IOleInPlaceObject
	BEGIN_INTERFACE_PART(OleInPlaceObject, IOleInPlaceObject)
		INIT_INTERFACE_PART(COleControl, OleInPlaceObject)
		STDMETHOD(GetWindow)(HWND FAR*);
		STDMETHOD(ContextSensitiveHelp)(BOOL);
		STDMETHOD(InPlaceDeactivate)();
		STDMETHOD(UIDeactivate)();
		STDMETHOD(SetObjectRects)(LPCRECT, LPCRECT);
		STDMETHOD(ReactivateAndUndo)();
	END_INTERFACE_PART(OleInPlaceObject)

	// IOleInPlaceActiveObject
	BEGIN_INTERFACE_PART(OleInPlaceActiveObject, IOleInPlaceActiveObject)
		INIT_INTERFACE_PART(COleControl, OleInPlaceActiveObject)
		STDMETHOD(GetWindow)(HWND FAR*);
		STDMETHOD(ContextSensitiveHelp)(BOOL);
		STDMETHOD(TranslateAccelerator)(LPMSG);
		STDMETHOD(OnFrameWindowActivate)(BOOL);
		STDMETHOD(OnDocWindowActivate)(BOOL);
		STDMETHOD(ResizeBorder)(LPCRECT, LPOLEINPLACEUIWINDOW, BOOL);
		STDMETHOD(EnableModeless)(BOOL);
	END_INTERFACE_PART(OleInPlaceActiveObject)

	// IDispatch
	BEGIN_INTERFACE_PART(Dispatch, IDispatch)
		INIT_INTERFACE_PART(COleControl, Dispatch)
		STDMETHOD(GetTypeInfoCount)(unsigned int FAR* pctinfo);
		STDMETHOD(GetTypeInfo)(unsigned int itinfo, LCID lcid,
			ITypeInfo FAR* FAR* pptinfo);
		STDMETHOD(GetIDsOfNames)(REFIID riid, LPTSTR FAR* rgszNames,
			unsigned int cNames, LCID lcid, DISPID FAR* rgdispid);
		STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid,
			unsigned short wFlags, DISPPARAMS FAR* pdispparams,
			VARIANT FAR* pvarResult, EXCEPINFO FAR* pexcepinfo,
			unsigned int FAR* puArgErr);
	END_INTERFACE_PART(Dispatch)

	// IOleCache
	BEGIN_INTERFACE_PART(OleCache, IOleCache)
		INIT_INTERFACE_PART(COleControl, OleCache)
		STDMETHOD(Cache)(LPFORMATETC, DWORD, LPDWORD);
		STDMETHOD(Uncache)(DWORD);
		STDMETHOD(EnumCache)(LPENUMSTATDATA FAR*);
		STDMETHOD(InitCache)(LPDATAOBJECT);
		STDMETHOD(SetData)(LPFORMATETC, STGMEDIUM FAR*, BOOL);
	END_INTERFACE_PART(OleCache)

	// IOleControl
	BEGIN_INTERFACE_PART(OleControl, IOleControl)
		INIT_INTERFACE_PART(COleControl, OleControl)
		STDMETHOD(GetControlInfo)(LPCONTROLINFO pCI);
		STDMETHOD(OnMnemonic)(LPMSG pMsg);
		STDMETHOD(OnAmbientPropertyChange)(DISPID dispid);
		STDMETHOD(FreezeEvents)(BOOL bFreeze);
	END_INTERFACE_PART(OleControl)

	// IProvideClassInfo
	BEGIN_INTERFACE_PART(ProvideClassInfo, IProvideClassInfo)
		INIT_INTERFACE_PART(COleControl, ProvideClassInfo)
		STDMETHOD(GetClassInfo)(LPTYPEINFO FAR* ppTypeInfo);
	END_INTERFACE_PART(ProvideClassInfo)

	// ISpecifyPropertyPages
	BEGIN_INTERFACE_PART(SpecifyPropertyPages, ISpecifyPropertyPages)
		INIT_INTERFACE_PART(COleControl, SpecifyPropertyPages)
		STDMETHOD(GetPages)(CAUUID FAR*);
	END_INTERFACE_PART(SpecifyPropertyPages)

	// IPerPropertyBrowsing
	BEGIN_INTERFACE_PART(PerPropertyBrowsing, IPerPropertyBrowsing)
		INIT_INTERFACE_PART(COleControl, PerPropertyBrowsing)
		STDMETHOD(GetDisplayString)(DISPID dispid, BSTR FAR* lpbstr);
		STDMETHOD(MapPropertyToPage)(DISPID dispid, LPCLSID lpclsid);
		STDMETHOD(GetPredefinedStrings)(DISPID dispid,
			CALPOLESTR FAR* lpcaStringsOut, CADWORD FAR* lpcaCookiesOut);
		STDMETHOD(GetPredefinedValue)(DISPID dispid, DWORD dwCookie,
			VARIANT FAR* lpvarOut);
	END_INTERFACE_PART(PerPropertyBrowsing)

	// IConnectionPointContainer
	BEGIN_INTERFACE_PART(ConnPtContainer, IConnectionPointContainer)
		INIT_INTERFACE_PART(COleControl, ConnPtContainer)
		STDMETHOD(EnumConnectionPoints)(LPENUMCONNECTIONPOINTS FAR* ppEnum);
		STDMETHOD(FindConnectionPoint)(
			REFIID iid, LPCONNECTIONPOINT FAR* ppCP);
	END_INTERFACE_PART(ConnPtContainer)

	// IPropertyNotifySink for font updates (not exposed via QueryInterface)
	BEGIN_INTERFACE_PART(FontNotification, IPropertyNotifySink)
		INIT_INTERFACE_PART(COleControl, FontNotification)
		STDMETHOD(OnChanged)(DISPID dispid);
		STDMETHOD(OnRequestEdit)(DISPID dispid);
	END_INTERFACE_PART(FontNotification)

	DECLARE_INTERFACE_MAP()

// Connection maps
protected:
	// Connection point for events
	BEGIN_CONNECTION_PART(COleControl, EventConnPt)
		virtual void OnAdvise(BOOL bAdvise);
		virtual REFIID GetIID();
	END_CONNECTION_PART(EventConnPt)

	// Connection point for property notifications
	BEGIN_CONNECTION_PART(COleControl, PropConnPt)
		CONNECTION_IID(IID_IPropertyNotifySink)
	END_CONNECTION_PART(PropConnPt)

	DECLARE_CONNECTION_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// Registry functions

BOOL AFXAPI AfxOleRegisterTypeLib(HINSTANCE hInstance, REFGUID tlid,
	LPCTSTR pszFileName = NULL, LPCTSTR pszHelpDir = NULL);

BOOL AFXAPI AfxOleUnregisterTypeLib(REFGUID tlid);

BOOL AFXAPI AfxOleRegisterControlClass(HINSTANCE hInstance, REFCLSID clsid,
	LPCTSTR pszProgID, UINT idTypeName, UINT idBitmap, BOOL bInsertable,
	DWORD dwMiscStatus, REFGUID tlid, WORD wVerMajor, WORD wVerMinor);

BOOL AFXAPI AfxOleUnregisterClass(REFCLSID clsid, LPCTSTR pszProgID);

BOOL AFXAPI AfxOleRegisterPropertyPageClass(HINSTANCE hInstance,
	REFCLSID clsid, UINT idTypeName);


/////////////////////////////////////////////////////////////////////////////
// Licensing functions

BOOL AFXAPI AfxVerifyLicFile(HINSTANCE hInstance, LPCTSTR pszLicFileName,
	LPCTSTR pszLicFileContents, UINT cch=-1);


/////////////////////////////////////////////////////////////////////////////
// CPropExchange - Abstract base class for property exchange

class CPropExchange
{
// Operations
public:
	BOOL IsLoading();
	DWORD GetVersion();
	BOOL ExchangeVersion(DWORD& dwVersionLoaded, DWORD dwVersionDefault,
		BOOL bConvert);

	virtual BOOL ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
				void* pvProp, const void* pvDefault = NULL) = 0;
	virtual BOOL ExchangeBlobProp(LPCTSTR pszPropName, HGLOBAL* phBlob,
				HGLOBAL hBlobDefault = NULL) = 0;
	virtual BOOL ExchangeFontProp(LPCTSTR pszPropName, CFontHolder& font,
				const FONTDESC FAR* pFontDesc,
				LPFONTDISP pFontDispAmbient) = 0;
	virtual BOOL ExchangePersistentProp(LPCTSTR pszPropName,
				LPUNKNOWN FAR* ppUnk, REFIID iid, LPUNKNOWN pUnkDefault) = 0;

// Implementation
protected:
	CPropExchange();
	BOOL m_bLoading;
	DWORD m_dwVersion;
};


/////////////////////////////////////////////////////////////////////////////
// Property-exchange (PX_) helper functions

BOOL PX_Short(CPropExchange* pPX, LPCTSTR pszPropName, short& sValue);

BOOL PX_Short(CPropExchange* pPX, LPCTSTR pszPropName, short& sValue,
	short sDefault);

BOOL PX_UShort(CPropExchange* pPX, LPCTSTR pszPropName, USHORT& usValue);

BOOL PX_UShort(CPropExchange* pPX, LPCTSTR pszPropName, USHORT& usValue,
	USHORT usDefault);

BOOL PX_Long(CPropExchange* pPX, LPCTSTR pszPropName, long& lValue);

BOOL PX_Long(CPropExchange* pPX, LPCTSTR pszPropName, long& lValue,
	long lDefault);

BOOL PX_ULong(CPropExchange* pPX, LPCTSTR pszPropName, ULONG& ulValue);

BOOL PX_ULong(CPropExchange* pPX, LPCTSTR pszPropName, ULONG& ulValue,
	ULONG ulDefault);

BOOL PX_Color(CPropExchange* pPX, LPCTSTR pszPropName, OLE_COLOR& clrValue);

BOOL PX_Color(CPropExchange* pPX, LPCTSTR pszPropName, OLE_COLOR& clrValue,
	OLE_COLOR clrDefault);

BOOL PX_Bool(CPropExchange* pPX, LPCTSTR pszPropName, BOOL& bValue);

BOOL PX_Bool(CPropExchange* pPX, LPCTSTR pszPropName, BOOL& bValue,
	BOOL bDefault);

BOOL PX_String(CPropExchange* pPX, LPCTSTR pszPropName, CString& strValue);

BOOL PX_String(CPropExchange* pPX, LPCTSTR pszPropName, CString& strValue,
	const CString& strDefault);

BOOL PX_Currency(CPropExchange* pPX, LPCTSTR pszPropName, CY& cyValue);

BOOL PX_Currency(CPropExchange* pPX, LPCTSTR pszPropName, CY& cyValue,
	CY cyDefault);

BOOL PX_Float(CPropExchange* pPX, LPCTSTR pszPropName, float& floatValue);

BOOL PX_Float(CPropExchange* pPX, LPCTSTR pszPropName, float& floatValue,
	float floatDefault);

BOOL PX_Double(CPropExchange* pPX, LPCTSTR pszPropName, double& doubleValue);

BOOL PX_Double(CPropExchange* pPX, LPCTSTR pszPropName, double& doubleValue,
	double doubleDefault);

BOOL PX_Blob(CPropExchange* pPX, LPCTSTR pszPropName, HGLOBAL& hBlob,
	HGLOBAL hBlobDefault = NULL);

BOOL PX_Font(CPropExchange* pPX, LPCTSTR pszPropName, CFontHolder& font,
	const FONTDESC FAR* pFontDesc = NULL,
	LPFONTDISP pFontDispAmbient = NULL);

BOOL PX_Picture(CPropExchange* pPX, LPCTSTR pszPropName,
	CPictureHolder& pict);

BOOL PX_Picture(CPropExchange* pPX, LPCTSTR pszPropName,
	CPictureHolder& pict, CPictureHolder& pictDefault);

BOOL PX_IUnknown(CPropExchange* pPX, LPCTSTR pszPropName, LPUNKNOWN& pUnk,
	REFIID iid, LPUNKNOWN pUnkDefault = NULL);

BOOL PX_VBXFontConvert(CPropExchange* pPX, CFontHolder& font);


/////////////////////////////////////////////////////////////////////////////
// Structures used by COlePropertyPage

typedef struct tagAFX_PPFIELDSTATUS
{
	UINT    nID;
	BOOL    bDirty;

} AFX_PPFIELDSTATUS;


/////////////////////////////////////////////////////////////////////////////
// Property Page Dialog Class

class COlePropertyPage : public CDialog
{
	DECLARE_DYNAMIC(COlePropertyPage)

// Constructors
public:
	COlePropertyPage(UINT idDlg, UINT idCaption);

// Operations
	LPDISPATCH FAR* GetObjectArray(ULONG FAR* pnObjects);
	void SetModifiedFlag(BOOL bModified = TRUE);
	BOOL IsModified();
	LPPROPERTYPAGESITE GetPageSite();
	void SetDialogResource(HGLOBAL hDialog);
	void SetPageName(LPCTSTR lpszPageName);
	void SetHelpInfo(LPCTSTR lpszDocString, LPCTSTR lpszHelpFile = NULL,
		DWORD dwHelpContext = 0);

	BOOL GetControlStatus(UINT nID);
	BOOL SetControlStatus(UINT nID, BOOL bDirty);
	void IgnoreApply(UINT nID);

	int MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL,
			UINT nType = MB_OK);
	// note that this is a non-virtual override of CWnd::MessageBox()

// Overridables
	virtual void OnSetPageSite();
	virtual void OnObjectsChanged();
	virtual BOOL OnHelp(LPCTSTR lpszHelpDir);
	virtual BOOL OnInitDialog();
	virtual BOOL OnEditProperty(DISPID dispid);

// Implementation

	// DDP_ property get/set helper routines
	BOOL SetPropText(LPCTSTR pszPropName, BYTE &Value);
	BOOL GetPropText(LPCTSTR pszPropName, BYTE* pValue);
	BOOL SetPropText(LPCTSTR pszPropName, int &Value);
	BOOL GetPropText(LPCTSTR pszPropName, int* pValue);
	BOOL SetPropText(LPCTSTR pszPropName, UINT &Value);
	BOOL GetPropText(LPCTSTR pszPropName, UINT* pValue);
	BOOL SetPropText(LPCTSTR pszPropName, long &Value);
	BOOL GetPropText(LPCTSTR pszPropName, long* pValue);
	BOOL SetPropText(LPCTSTR pszPropName, DWORD &Value);
	BOOL GetPropText(LPCTSTR pszPropName, DWORD* pValue);
	BOOL SetPropText(LPCTSTR pszPropName, CString &Value);
	BOOL GetPropText(LPCTSTR pszPropName, CString* pValue);
	BOOL SetPropText(LPCTSTR pszPropName, float &Value);
	BOOL GetPropText(LPCTSTR pszPropName, float* pValue);
	BOOL SetPropText(LPCTSTR pszPropName, double &Value);
	BOOL GetPropText(LPCTSTR pszPropName, double* pValue);
	BOOL SetPropCheck(LPCTSTR pszPropName, int Value);
	BOOL GetPropCheck(LPCTSTR pszPropName, int* pValue);
	BOOL SetPropRadio(LPCTSTR pszPropName, int Value);
	BOOL GetPropRadio(LPCTSTR pszPropName, int* pValue);
	BOOL SetPropIndex(LPCTSTR pszPropName, int Value);
	BOOL GetPropIndex(LPCTSTR pszPropName, int* pValue);
	CMapWordToOb m_DDPmap;      // Map of member vars to pending DDP data

	// Destructors
	~COlePropertyPage();

protected:
	LRESULT WindowProc(UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	BOOL PreTranslateMessage(LPMSG lpMsg);
	virtual void OnFinalRelease();
	void CleanupObjectArray();
	static BOOL CALLBACK EXPORT EnumChildProc(HWND hWnd, LPARAM lParam);
	static BOOL CALLBACK EXPORT EnumControls(HWND hWnd, LPARAM lParam);

	// Member data
protected:
	AFX_MODULE_STATE* m_pModuleState;

private:
	BOOL m_bDirty;
	UINT m_idDlg;
	UINT m_idCaption;
	CString m_strPageName;
	SIZE m_sizePage;
	CString m_strDocString;
	CString m_strHelpFile;
	DWORD m_dwHelpContext;
	LPPROPERTYPAGESITE m_pPageSite;

	LPDISPATCH FAR* m_ppDisp;   // Array of IDispatch pointers, used to
								// access the properties of each control

	LPDWORD m_pAdvisors;        // Array of connection tokens used by
								// IConnecitonPoint::Advise/UnAdvise.

	BOOL m_bPropsChanged;       // IPropertyNotifySink::OnChanged has been
								// called, but not acted upon yet.

	ULONG m_nObjects;           // Objects in m_ppDisp, m_ppDataObj, m_pAdvisors

	BOOL m_bInitializing;       // TRUE if the contents of the fields of
								// the dialog box are being initialized

	int m_nControls;            // Number of fields on this property page

	AFX_PPFIELDSTATUS* m_pStatus;   // Array containing information on
									// which fields are dirty

	CWordArray m_IDArray;       // Array containing information on which
								// controls to ignore when deciding if
								// the apply button is to be enabled

	HGLOBAL m_hDialog;          // Handle of the dialog resource


// Interface Maps
protected:
	BEGIN_INTERFACE_PART(PropertyPage, IPropertyPage2)
		INIT_INTERFACE_PART(COlePropertyPage, PropertyPage)
		STDMETHOD(SetPageSite)(LPPROPERTYPAGESITE);
		STDMETHOD(Activate)(HWND, LPCRECT, BOOL);
		STDMETHOD(Deactivate)();
		STDMETHOD(GetPageInfo)(LPPROPPAGEINFO);
		STDMETHOD(SetObjects)(ULONG, LPUNKNOWN FAR*);
		STDMETHOD(Show)(UINT);
		STDMETHOD(Move)(LPCRECT);
		STDMETHOD(IsPageDirty)();
		STDMETHOD(Apply)();
		STDMETHOD(Help)(LPCTSTR);
		STDMETHOD(TranslateAccelerator)(LPMSG);
		STDMETHOD(EditProperty)(DISPID);
	END_INTERFACE_PART(PropertyPage)

	BEGIN_INTERFACE_PART(PropNotifySink, IPropertyNotifySink)
		INIT_INTERFACE_PART(COlePropertyPage, PropNotifySink)
		STDMETHOD(OnRequestEdit)(DISPID);
		STDMETHOD(OnChanged)(DISPID);
	END_INTERFACE_PART(PropNotifySink)

	DECLARE_INTERFACE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// Property Page Dialog Data Exchange routines

// simple text operations
void AFXAPI DDP_Text(CDataExchange*pDX, int id, BYTE& member, LPCTSTR pszPropName);
void AFXAPI DDP_Text(CDataExchange*pDX, int id, int& member, LPCTSTR pszPropName);
void AFXAPI DDP_Text(CDataExchange*pDX, int id, UINT& member, LPCTSTR pszPropName);
void AFXAPI DDP_Text(CDataExchange*pDX, int id, long& member, LPCTSTR pszPropName);
void AFXAPI DDP_Text(CDataExchange*pDX, int id, DWORD& member, LPCTSTR pszPropName);
void AFXAPI DDP_Text(CDataExchange*pDX, int id, float& member, LPCTSTR pszPropName);
void AFXAPI DDP_Text(CDataExchange*pDX, int id, double& member, LPCTSTR pszPropName);
void AFXAPI DDP_Text(CDataExchange*pDX, int id, CString& member, LPCTSTR pszPropName);
void AFXAPI DDP_Check(CDataExchange*pDX, int id, int& member, LPCTSTR pszPropName);
void AFXAPI DDP_Radio(CDataExchange*pDX, int id, int& member, LPCTSTR pszPropName);
void AFXAPI DDP_LBString(CDataExchange* pDX, int id, CString& member, LPCTSTR pszPropName);
void AFXAPI DDP_LBStringExact(CDataExchange* pDX, int id, CString& member, LPCTSTR pszPropName);
void AFXAPI DDP_LBIndex(CDataExchange* pDX, int id, int& member, LPCTSTR pszPropName);
void AFXAPI DDP_CBString(CDataExchange* pDX, int id, CString& member, LPCTSTR pszPropName);
void AFXAPI DDP_CBStringExact(CDataExchange* pDX, int id, CString& member, LPCTSTR pszPropName);
void AFXAPI DDP_CBIndex(CDataExchange* pDX, int id, int& member, LPCTSTR pszPropName);
void AFXAPI DDP_PostProcessing(CDataExchange *pDX);

////////////////////////////////////////////////////////////////////////////
// AfxOleTypeMatchGuid - Tests whether a given TYPEDESC matches a type with a
// given GUID, when all aliases have been expanded.

BOOL AFXAPI AfxOleTypeMatchGuid(LPTYPEINFO pTypeInfo,
	TYPEDESC FAR* pTypeDesc, REFGUID guidType, ULONG cIndirectionLevels);

/////////////////////////////////////////////////////////////////////////////
// Inline function declarations

#ifdef _AFX_ENABLE_INLINES
#define _AFXCTL_INLINE inline
#include <afxctl.inl>
#endif

// Reset MFC data definitions

#ifdef _WIN32
#undef AFX_DATA
#define AFX_DATA
#else
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR
#endif

// Reset structure packing to its previous state
#ifdef _WIN32
#pragma pack(pop)
#endif

#endif // __AFXCTL_H__
