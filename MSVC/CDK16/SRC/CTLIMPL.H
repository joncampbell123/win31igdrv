// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1994 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and the
// Books Online documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef __CTLIMPL_H__
#define __CTLIMPL_H__

// MFC data definition for data exported from the runtime DLL

#undef AFX_DATA
#define AFX_DATA AFX_CTL_DATA32
#undef AFXAPP_DATA
#define AFXAPP_DATA AFX_CTL_DATA16

#define UNUSED      // usage: UNUSED formal_arg

/////////////////////////////////////////////////////////////////////////////
// Codes for COleControl::SendAdvise
//......Code.........................Method called
#define OBJECTCODE_SAVED          0  //IOleAdviseHolder::SendOnSave
#define OBJECTCODE_CLOSED         1  //IOleAdviseHolder::SendOnClose
#define OBJECTCODE_RENAMED        2  //IOleAdviseHolder::SendOnRename
#define OBJECTCODE_SAVEOBJECT     3  //IOleClientSite::SaveObject
#define OBJECTCODE_DATACHANGED    4  //IDataAdviseHolder::SendOnDataChange
#define OBJECTCODE_SHOWWINDOW     5  //IOleClientSite::OnShowWindow(TRUE)
#define OBJECTCODE_HIDEWINDOW     6  //IOleClientSite::OnShowWindow(FALSE)
#define OBJECTCODE_SHOWOBJECT     7  //IOleClientSite::ShowObject
#define OBJECTCODE_VIEWCHANGED    8  //IOleAdviseHolder::SendOnViewChange


/////////////////////////////////////////////////////////////////////////////
// Typedefs

typedef LPVOID FAR* LPLPVOID;


/////////////////////////////////////////////////////////////////////////////
// Macros

#undef RELEASE
#define RELEASE(p)  if ((p) != NULL) { (p)->Release(); (p) = NULL; };

#define ASSERT_POINTER(p, type) \
	ASSERT(((p) != NULL) && AfxIsValidAddress((p), sizeof(type), FALSE))

#define ASSERT_NULL_OR_POINTER(p, type) \
	ASSERT(((p) == NULL) || AfxIsValidAddress((p), sizeof(type), FALSE))

#define HIMETRIC_PER_INCH   2540
#define MAP_PIX_TO_LOGHIM(x,ppli)   MulDiv(HIMETRIC_PER_INCH, (x), (ppli))
#define MAP_LOGHIM_TO_PIX(x,ppli)   MulDiv((ppli), (x), HIMETRIC_PER_INCH)


////////////////////////////////////////////////////////////////////////////
// The DLL object

class COleControlDll : public CWinApp
{
public:
	COleControlDll(LPCTSTR pszAppName) : CWinApp(pszAppName) { }

	virtual BOOL InitInstance();
	virtual int ExitInstance();
};

/////////////////////////////////////////////////////////////////////////////
// Functions

BOOL _AfxOleMatchPropsetClipFormat(CLIPFORMAT cfFormat, LPCLSID lpFmtID);
BOOL _AfxCopyPropValue(VARTYPE vtProp, void* pvDest, const void * pvSrc);
BOOL _AfxPeekAtClassIDInStream(LPSTREAM pstm, LPCLSID lpClassID);
BOOL _AfxIsSameFont(CFontHolder& font, const FONTDESC FAR* pFontDesc,
	LPFONTDISP pFontDispAmbient);
BOOL _AfxInitBlob(HGLOBAL* phDst, void* pvSrc);
BOOL _AfxCopyBlob(HGLOBAL* phDst, HGLOBAL hSrc);
LPFONT _AfxCreateFontFromStream(LPSTREAM);

#ifndef _WIN32
#define _AfxCtlMalloc OleStdMalloc
#define _AfxCtlRealloc OleStdRealloc
#define _AfxCtlFree OleStdFree
#define _AfxCtlCreateDC OleStdCreateDC
#define _AfxCtlDeleteTargetDevice OleStdDeleteTargetDevice
#define _AfxCtlCopyTargetDevice OleStdCopyTargetDevice
#define _AfxCtlCopyFormatEtc OleStdCopyFormatEtc
#define _AfxCtlFreeString OleStdFreeString
#define _AfxCtlCopyString OleStdCopyString
#endif

STDAPI_(LPVOID) _AfxCtlMalloc(ULONG ulSize);
STDAPI_(LPVOID) _AfxCtlRealloc(LPVOID pmem, ULONG ulSize);
STDAPI_(void) _AfxCtlFree(LPVOID pmem);
STDAPI_(HDC) _AfxCtlCreateDC(DVTARGETDEVICE FAR* ptd);
STDAPI_(BOOL) _AfxCtlDeleteTargetDevice(DVTARGETDEVICE FAR* ptd);
STDAPI_(DVTARGETDEVICE FAR*) _AfxCtlCopyTargetDevice(DVTARGETDEVICE FAR* ptdSrc);
STDAPI_(BOOL) _AfxCtlCopyFormatEtc(LPFORMATETC petcDest, LPFORMATETC petcSrc);
STDAPI_(void) _AfxCtlFreeString(LPTSTR lpsz, LPMALLOC lpMalloc);
STDAPI_(LPTSTR) _AfxCtlCopyString(LPTSTR lpszSrc, LPMALLOC lpMalloc);

long _RoundDiv(long, long);
STDAPI_(void) _XformRectInPixelsToHimetric(HDC, LPRECT, LPRECT);
STDAPI_(void) _XformRectInHimetricToPixels(HDC, LPRECT, LPRECT);
STDAPI_(void) _XformSizeInPixelsToHimetric(HDC, LPSIZEL, LPSIZEL);
STDAPI_(void) _XformSizeInHimetricToPixels(HDC, LPSIZEL, LPSIZEL);
STDAPI_(int) _XformWidthInHimetricToPixels(HDC, int);
STDAPI_(int) _XformWidthInPixelsToHimetric(HDC, int);
STDAPI_(int) _XformHeightInHimetricToPixels(HDC, int);
STDAPI_(int) _XformHeightInPixelsToHimetric(HDC, int);
STDAPI_(int) _AfxCtlSetDCToDrawInHimetricRect(HDC, LPRECT, LPRECT, LPRECT, LPRECT);
STDAPI_(int) _AfxCtlResetOrigDC(HDC, int, LPRECT, LPRECT);

/////////////////////////////////////////////////////////////////////////////
// CControlFrameWnd - used for a control's "open" (non-in-place) state.

class CControlFrameWnd : public CWnd
{
public:
	CControlFrameWnd(COleControl* pCtrl);
	virtual BOOL Create(LPCTSTR pszTitle);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual void PostNcDestroy();

	COleControl* m_pCtrl;

	//{{AFX_MSG(CControlFrameWnd)
	afx_msg void OnClose();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CReflectorWnd - reflects window messages to a subclassed control.

class CReflectorWnd : public CWnd
{
public:
	CReflectorWnd() : m_pCtrl(NULL) { }

	BOOL Create(const CRect& rect, HWND hWndParent);
	void SetControl(COleControl* pCtrl);

protected:
	virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();

	COleControl* m_pCtrl;
};


/////////////////////////////////////////////////////////////////////////////
// CParkingWnd - "parking space" for not-yet-activated subclassed controls

class CParkingWnd : public CWnd
{
public:
	CParkingWnd()
		{ CreateEx(0, _T("AfxWnd"), NULL, WS_OVERLAPPED, 0, 0, 1, 1, NULL, 0); }
};


/////////////////////////////////////////////////////////////////////////////
//  Property setting

typedef struct tagSECTIONHEADER
{
	DWORD       cbSection ;
	DWORD       cProperties ;  // Number of props.
} SECTIONHEADER, FAR *LPSECTIONHEADER ;

typedef struct tagPROPERTYIDOFFSET
{
	DWORD       propertyID;
	DWORD       dwOffset;
} PROPERTYIDOFFSET, FAR * LPPROPERTYIDOFFSET;

typedef struct tagPROPHEADER
{
	WORD        wByteOrder ;    // Always 0xFFFE
	WORD        wFormat ;       // Always 0
	DWORD       dwOSVer ;       // System version
	CLSID       clsID ;         // Application CLSID
	DWORD       cSections ;     // Number of sections (must be at least 1)
} PROPHEADER, FAR *LPPROPHEADER ;

typedef struct tagFORMATIDOFFSET
{
	GUID        formatID;
	DWORD       dwOffset;
} FORMATIDOFFSET, FAR * LPFORMATIDOFFSET;


/////////////////////////////////////////////////////////////////////////////
// CProperty

class CProperty : public CObject
{
	friend class CPropertySet ;
	friend class CPropertySection ;

public:
// Construction
	CProperty( void ) ;
	CProperty( DWORD dwID, const LPVOID pValue, DWORD dwType ) ;

// Attributes
	BOOL    Set( DWORD dwID, const LPVOID pValue, DWORD dwType ) ;
	BOOL    Set( const LPVOID pValue, DWORD dwType ) ;
	BOOL    Set( const LPVOID pValue ) ;
	LPVOID  Get( DWORD* pcb ) ;     // Returns pointer to actual value
	LPVOID  Get( void ) ;           // Returns pointer to actual value
	DWORD   GetType( void ) ;       // Returns property type
	void    SetType( DWORD dwType ) ;
	DWORD   GetID( void ) ;
	void    SetID( DWORD dwPropID ) ;

	LPVOID  GetRawValue( void ) ;   // Returns pointer internal value (may
									// include size information)
// Operations
	BOOL    WriteToStream( IStream* pIStream ) ;
	BOOL    ReadFromStream( IStream* pIStream ) ;

private:
	DWORD       m_dwPropID ;
	DWORD       m_dwType ;
	LPVOID      m_pValue ;

#ifndef _WIN32
	HGLOBAL     m_hGlobal;
#endif

	LPVOID  AllocValue(ULONG cb);
	void    FreeValue();

public:
	~CProperty() ;
} ;


/////////////////////////////////////////////////////////////////////////////
// CPropertySection

class CPropertySection : public CObject
{
	friend class CPropertySet ;
	friend class CProperty ;

public:
// Construction
	CPropertySection( void ) ;
	CPropertySection( CLSID FormatID ) ;

// Attributes
	CLSID   GetFormatID( void ) ;
	void    SetFormatID( CLSID FormatID ) ;

	BOOL    Set( DWORD dwPropID, LPVOID pValue, DWORD dwType ) ;
	BOOL    Set( DWORD dwPropID, LPVOID pValue ) ;
	LPVOID  Get( DWORD dwPropID, DWORD* pcb ) ;
	LPVOID  Get( DWORD dwPropID ) ;
	void    Remove( DWORD dwPropID ) ;
	void    RemoveAll() ;

	CProperty* GetProperty( DWORD dwPropID ) ;
	void AddProperty( CProperty* pProp ) ;

	DWORD   GetSize( void ) ;
	DWORD   GetCount( void ) ;
	CObList* GetList( void ) ;

	BOOL    GetID( LPCTSTR pszName, DWORD* pdwPropID ) ;
	BOOL    SetName( DWORD dwPropID, LPCTSTR pszName ) ;

	BOOL    SetSectionName( LPCTSTR pszName );
	LPCTSTR GetSectionName( void );

// Operations
	BOOL    WriteToStream( IStream* pIStream ) ;
	BOOL    ReadFromStream( IStream* pIStream, LARGE_INTEGER liPropSet ) ;
	BOOL    WriteNameDictToStream( IStream* pIStream ) ;
	BOOL    ReadNameDictFromStream( IStream* pIStream ) ;

private:
// Implementation
	CLSID           m_FormatID ;
	SECTIONHEADER   m_SH ;
	// List of properties (CProperty)
	CObList         m_PropList ;
	// Dictionary of property names
	CMapStringToPtr m_NameDict ;
	CString         m_strSectionName;

public:
	~CPropertySection();
} ;


/////////////////////////////////////////////////////////////////////////////
// CPropertySet

class CPropertySet : public CObject
{
	friend class CPropertySection ;
	friend class CProperty ;

public:
// Construction
	CPropertySet( void ) ;
	CPropertySet( CLSID clsID )  ;

// Attributes
	BOOL    Set( CLSID FormatID, DWORD dwPropID, LPVOID pValue, DWORD dwType ) ;
	BOOL    Set( CLSID FormatID, DWORD dwPropID, LPVOID pValue ) ;
	LPVOID  Get( CLSID FormatID, DWORD dwPropID, DWORD* pcb ) ;
	LPVOID  Get( CLSID FormatID, DWORD dwPropID ) ;
	void    Remove( CLSID FormatID, DWORD dwPropID ) ;
	void    Remove( CLSID FormatID ) ;
	void    RemoveAll( ) ;

	CProperty* GetProperty( CLSID FormatID, DWORD dwPropID ) ;
	void AddProperty( CLSID FormatID, CProperty* pProp ) ;
	CPropertySection* GetSection( CLSID FormatID ) ;
	CPropertySection* AddSection( CLSID FormatID ) ;
	void AddSection( CPropertySection* psect ) ;

	WORD    GetByteOrder( void ) ;
	WORD    GetFormatVersion( void ) ;
	void    SetFormatVersion( WORD wFmtVersion ) ;
	DWORD   GetOSVersion( void ) ;
	void    SetOSVersion( DWORD dwOSVer ) ;
	CLSID   GetClassID( void ) ;
	void    SetClassID( CLSID clsid ) ;
	DWORD   GetCount( void ) ;
	CObList* GetList( void ) ;

// Operations
	BOOL    WriteToStream( IStream* pIStream ) ;
	BOOL    ReadFromStream( IStream* pIStream ) ;

// Implementation
private:
	PROPHEADER      m_PH ;
	CObList         m_SectionList ;

public:
	~CPropertySet();
} ;


/////////////////////////////////////////////////////////////////////////////
// CArchivePropExchange - for persistence in an archive.

class CArchivePropExchange : public CPropExchange
{
// Constructors
public:
	CArchivePropExchange(CArchive& ar);

// Operations
	virtual BOOL ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
				void* pvProp, const void* pvDefault = NULL);
	virtual BOOL ExchangeBlobProp(LPCTSTR pszPropName, HGLOBAL* phBlob,
				HGLOBAL hBlobDefault = NULL);
	virtual BOOL ExchangeFontProp(LPCTSTR pszPropName, CFontHolder& font,
				const FONTDESC FAR* pFontDesc, LPFONTDISP pFontDispAmbient);
	virtual BOOL ExchangePersistentProp(LPCTSTR pszPropName,
				LPUNKNOWN FAR* ppUnk, REFIID iid, LPUNKNOWN pUnkDefault);

// Implementation
protected:
	CArchive& m_ar;
};


/////////////////////////////////////////////////////////////////////////////
// CResetPropExchange - for resetting property state to defaults.

class CResetPropExchange : public CPropExchange
{
// Constructors
public:
	CResetPropExchange(void);

// Operations
	virtual BOOL ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
				void* pvProp, const void* pvDefault = NULL);
	virtual BOOL ExchangeBlobProp(LPCTSTR pszPropName, HGLOBAL* phBlob,
				HGLOBAL hBlobDefault = NULL);
	virtual BOOL ExchangeFontProp(LPCTSTR pszPropName, CFontHolder& font,
				const FONTDESC FAR* pFontDesc, LPFONTDISP pFontDispAmbient);
	virtual BOOL ExchangePersistentProp(LPCTSTR pszPropName,
				LPUNKNOWN FAR* ppUnk, REFIID iid, LPUNKNOWN pUnkDefault);
};


/////////////////////////////////////////////////////////////////////////////
// CPropsetPropExchange - for persistence in a property set.

class CPropsetPropExchange : public CPropExchange
{
// Constructors
public:
	CPropsetPropExchange(CPropertySection& psec, LPSTORAGE lpStorage,
		BOOL bLoading);

// Operations
	virtual BOOL ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
				void* pvProp, const void* pvDefault = NULL);
	virtual BOOL ExchangeBlobProp(LPCTSTR pszPropName, HGLOBAL* phBlob,
				HGLOBAL hBlobDefault = NULL);
	virtual BOOL ExchangeFontProp(LPCTSTR pszPropName, CFontHolder& font,
				const FONTDESC FAR* pFontDesc, LPFONTDISP pFontDispAmbient);
	virtual BOOL ExchangePersistentProp(LPCTSTR pszPropName,
				LPUNKNOWN FAR* ppUnk, REFIID iid, LPUNKNOWN pUnkDefault);

// Implementation
	CPropertySection& m_psec;
	LPSTORAGE m_lpStorage;
	DWORD m_dwPropID;
};


/////////////////////////////////////////////////////////////////////////////
// COleDispatchExceptionEx - dispatch exception that includes an SCODE

class COleDispatchExceptionEx : public COleDispatchException
{
public:
	COleDispatchExceptionEx(LPCTSTR lpszDescription, UINT nHelpID, SCODE sc);
};


/////////////////////////////////////////////////////////////////////////////
// ANSI and Unicode versions of OLE APIs

#ifdef _WIN32
#ifdef OLE2ANSI
#define OleCreatePropertyFrame          OleCreatePropertyFrameA
#define OleCreatePropertyFrameIndirect  OleCreatePropertyFrameIndirectA
#define OleCreateFontIndirect           OleCreateFontIndirectA
#else
#define OleCreatePropertyFrame          OleCreatePropertyFrameW
#define OleCreatePropertyFrameIndirect  OleCreatePropertyFrameIndirectW
#define OleCreateFontIndirect           OleCreateFontIndirectW
#endif

STDAPI OleCreatePropertyFrame(HWND hwndOwner, UINT x, UINT y,
	LPCOLESTR lpszCaption, ULONG cObjects, LPUNKNOWN FAR* ppUnk, ULONG cPages,
	LPCLSID pPageClsID, LCID lcid, DWORD dwReserved, LPVOID pvReserved);

STDAPI OleCreatePropertyFrameIndirect(LPOCPFIPARAMS lpParams);

STDAPI OleCreateFontIndirect(LPFONTDESC lpFontDesc, REFIID riid,
	LPVOID FAR* lplpvObj);

#endif // _WIN32


/////////////////////////////////////////////////////////////////////////////
// ANSI wrappers for OLE interfaces

#ifdef OLE2ANSI
STDAPI Ole2AnsiWFromA(REFIID, LPUNKNOWN, LPUNKNOWN*);
STDAPI Ole2AnsiAFromW(REFIID, LPUNKNOWN, LPUNKNOWN*);
#endif


/////////////////////////////////////////////////////////////////////////////
// Reset MFC data definitions

#ifdef _WIN32
#undef AFX_DATA
#define AFX_DATA
#else
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR
#endif

/////////////////////////////////////////////////////////////////////////////
// 16-bit segment tuning

#if defined(_M_I86MM) || defined(_M_I86LM)  // far code only
#ifndef _WIN32

#define AFXCTL_CORE1_SEG    "AFXCTL_CORE1_TEXT"     // core functionality
#define AFXCTL_CORE2_SEG    "AFXCTL_CORE2_TEXT"     // core functionality
#define AFXCTL_CORE3_SEG    "AFXCTL_CORE3_TEXT"     // core functionality
#define AFXCTL_COMM1_SEG    "AFXCTL_COMM1_TEXT"     // common objects
#define AFXCTL_COMM2_SEG    "AFXCTL_COMM2_TEXT"     // common objects
#define AFXCTL_PAGE_SEG     "AFXCTL_PAGE_TEXT"      // property page things
#define AFXCTL_PROP_SEG     "AFXCTL_PROP_TEXT"      // property data & exchange
#define AFXCTL_FRAME_SEG    "AFXCTL_FRAME_TEXT"     // property frame things
#define AFXCTL_FACT_SEG     "AFXCTL_FACT_TEXT"      // reg/factory/licensing
#define AFXCTL_DEBUG_SEG    "AFXCTL_DEBUG_TEXT"     // inlines

#endif
#endif

#endif  //__CTLIMPL_H__
