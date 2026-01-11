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

#ifdef AFXCTL_FACT_SEG
#pragma code_seg(AFXCTL_FACT_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

#define GUID_CCH	39	// Characters in string form of guid, including '\0'


inline BOOL _AfxRegDeleteKeySucceeded(LONG error)
{
#ifdef _WIN32
	return (error == ERROR_SUCCESS) || (error == ERROR_BADKEY) ||
		(error == ERROR_FILE_NOT_FOUND);
#else
	return (error == ERROR_SUCCESS) || (error == ERROR_BADKEY);
#endif
}


// Under Win32, a reg key may not be deleted unless it is empty.
// Thus, to delete a tree,  one must recursively enumerate and
// delete all of the sub-keys.
// Under Win16, RegDeleteKey does this for you.

LONG _AfxRecursiveRegDeleteKey(HKEY hParentKey, LPTSTR szKeyName)
{
#ifndef _WIN32
	return RegDeleteKey(hParentKey, szKeyName);
#else
	DWORD   dwIndex = 0L;
	TCHAR   szSubKeyName[256];
	HKEY    hCurrentKey;
	DWORD   dwResult;

	if ((dwResult = RegOpenKey(hParentKey, szKeyName, &hCurrentKey)) == ERROR_SUCCESS)
	{
		// remove all subkeys of the key to delete
		while ((dwResult = RegEnumKey(hCurrentKey, 0, szSubKeyName, 255)) == ERROR_SUCCESS)
		{
			OutputDebugString(szSubKeyName);
			OutputDebugString(_T("\r\n"));
			if ((dwResult = _AfxRecursiveRegDeleteKey(hCurrentKey, szSubKeyName)) != ERROR_SUCCESS)
				break;
		}

		// if all went well, we should now be able to delete the requested key
		if (dwResult == ERROR_NO_MORE_ITEMS)
			dwResult = RegDeleteKey(hParentKey, szKeyName);
	}

	RegCloseKey(hCurrentKey);
	return dwResult;
#endif
}

BOOL AFXAPI AfxOleRegisterHelperEx(LPCTSTR FAR* rglpszRegister,
	LPCTSTR FAR* rglpszSymbols, int nSymbols, BOOL bReplace, HKEY hkey)
{
	ASSERT(hkey != NULL);
	ASSERT(rglpszRegister != NULL);
	ASSERT(nSymbols == 0 || rglpszSymbols != NULL);

	CString strKey;
	CString strValue;

	while (*rglpszRegister != NULL)
	{
		LPCTSTR lpszKey = *rglpszRegister++;
		LPCTSTR lpszValue = lpszKey + _tcslen(lpszKey) + 1;

		AfxFormatStrings(strKey, lpszKey, rglpszSymbols, nSymbols);
		AfxFormatStrings(strValue, lpszValue, rglpszSymbols, nSymbols);

		if ((hkey == HKEY_CLASSES_ROOT) && strKey.IsEmpty())
		{
			TRACE1("Warning: skipping empty key '%Fs'\n", lpszKey);
			continue;
		}

		if (!bReplace)
		{
			TCHAR szBuffer[256];
			LONG lSize = sizeof(szBuffer)/sizeof(*szBuffer);
			if (::RegQueryValue(hkey, strKey, szBuffer, &lSize) ==
				ERROR_SUCCESS)
			{
#ifdef _DEBUG
				if (strValue != szBuffer)
				{
					TRACE3("Warning: Leaving value '%Fs' for key '%Fs' in registry\n\tintended value was '%Fs'\n",
						(LPCTSTR)szBuffer, (LPCTSTR)strKey, (LPCTSTR)strValue);
				}
#endif
				continue;
			}
		}

		if (::RegSetValue(hkey, strKey, REG_SZ, strValue, 0)
			!= ERROR_SUCCESS)
		{
			TRACE2("Error: failed setting key '%Fs' to value '%Fs'\n",
				(LPCTSTR)strKey, (LPCTSTR)strValue);
			return FALSE;
		}
	}

	return TRUE;
}


BOOL AFXAPI AfxOleRegisterTypeLib(HINSTANCE hInstance, REFGUID tlid,
	LPCTSTR pszFileName, LPCTSTR pszHelpDir)
{
	BOOL bSuccess = FALSE;
	UNUSED tlid;

	CString strPathName;
	::GetModuleFileName(hInstance, strPathName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strPathName.ReleaseBuffer();

	// If a filename was specified, replace final component of path with it.
	if (pszFileName != NULL)
	{
		int iBackslash = strPathName.ReverseFind(_T('\\'));
		if (iBackslash != -1)
			strPathName = strPathName.Left(iBackslash+1);
		strPathName += pszFileName;
	}

	LPTYPELIB ptlib = NULL;
	if (SUCCEEDED(LoadTypeLib((LPTSTR)(LPCTSTR)strPathName, &ptlib)))
	{
		ASSERT_POINTER(ptlib, ITypeLib);

		LPTLIBATTR pAttr;
		GUID tlidActual = GUID_NULL;

		if (SUCCEEDED(ptlib->GetLibAttr(&pAttr)))
		{
			ASSERT_POINTER(pAttr, TLIBATTR);
			tlidActual = pAttr->guid;
			ptlib->ReleaseTLibAttr(pAttr);
		}

		// Check that the guid of the loaded type library matches
		// the tlid parameter.
		ASSERT(IsEqualGUID(tlid, tlidActual));

		if (IsEqualGUID(tlid, tlidActual))
		{
			// Register the type library.
			if (SUCCEEDED(RegisterTypeLib(ptlib,
					(LPTSTR)(LPCTSTR)strPathName, (LPTSTR)pszHelpDir)))
				bSuccess = TRUE;
		}

		RELEASE(ptlib);
	}

	return bSuccess;
}


#ifdef _WIN32
#define TYPELIBWIN   _T("win32")
#define TYPELIBWIN_2 _T("win16")
#else
#define TYPELIBWIN   _T("win16")
#define TYPELIBWIN_2 _T("win32")
#endif

BOOL AFXAPI AfxOleUnregisterTypeLib(REFGUID tlid)
{
	// Format typelib guid as a string
	TCHAR szTypeLibID[GUID_CCH];
	int cchGuid = ::StringFromGUID2(tlid, szTypeLibID, GUID_CCH);

	ASSERT(cchGuid == GUID_CCH);	// Did StringFromGUID2 work?
	if (cchGuid != GUID_CCH)
		return FALSE;

	TCHAR szKey[_MAX_PATH];

	// check to see if a win16 key is found when unregistering win32 (or vice versa)
	wsprintf(szKey, _T("TYPELIB\\%s\\1.0\\0\\%s"), szTypeLibID, TYPELIBWIN_2);
	HKEY hkey;
	BOOL bSurgical = RegOpenKey(HKEY_CLASSES_ROOT, szKey, &hkey) == ERROR_SUCCESS;

	LONG error;
	if( bSurgical )
	{
		// Only remove the keys specific to this version of the control,
		// leaving things in tact for the other version.
		// Note that a dangling HELPDIR key would be inevitable.
		wsprintf(szKey, _T("TYPELIB\\%s\\1.0\\0\\%s"), szTypeLibID, TYPELIBWIN);
		error = RegDeleteKey(HKEY_CLASSES_ROOT, szKey);
	}
	else
	{
		// No other versions of this control were detected,
		// so go ahead and remove the control completely.
		wsprintf(szKey, _T("TYPELIB\\%s"), szTypeLibID);
		error = _AfxRecursiveRegDeleteKey(HKEY_CLASSES_ROOT, szKey);
	}

	return _AfxRegDeleteKeySucceeded(error);
}


static LPCTSTR BASED_CODE rglpszCtrlProgID[] =
{
	_T("\0") _T("%1"),
	_T("CLSID\0") _T("%2"),
	NULL
};

static LPCTSTR BASED_CODE rglpszCtrlClassID[] =
{
	_T("\0") _T("%1"),
	_T("ProgID\0") _T("%2"),
#ifdef _WIN32
	_T("InprocServer32\0") _T("%3"),
	_T("ToolboxBitmap32\0") _T("%3, %4"),
#else
	_T("InprocServer\0") _T("%3"),
	_T("ToolboxBitmap\0") _T("%3, %4"),
#endif
	_T("MiscStatus\0") _T("0"),
	_T("MiscStatus\\1\0") _T("%5"),
	_T("Control\0") _T(""),
	_T("TypeLib\0") _T("%6"),
	_T("Version\0") _T("%7"),
	NULL
};


BOOL AFXAPI AfxOleRegisterControlClass(HINSTANCE hInstance,
	REFCLSID clsid, LPCTSTR pszProgID, UINT idTypeName, UINT idBitmap,
	BOOL bInsertable, DWORD dwMiscStatus, REFGUID tlid, WORD wVerMajor,
	WORD wVerMinor)
{
	BOOL bSuccess = FALSE;

	// Format class ID as a string
	TCHAR szClassID[GUID_CCH];
	int cchGuid = ::StringFromGUID2(clsid, szClassID, GUID_CCH);

	ASSERT(cchGuid == GUID_CCH);	// Did StringFromGUID2 work?
	if (cchGuid != GUID_CCH)
		return FALSE;

	// Format typelib guid as a string
	TCHAR szTypeLibID[GUID_CCH];
	cchGuid = ::StringFromGUID2(tlid, szTypeLibID, GUID_CCH);

	ASSERT(cchGuid == GUID_CCH);	// Did StringFromGUID2 work?
	if (cchGuid != GUID_CCH)
		return FALSE;

	TCHAR szPathName[_MAX_PATH];
	::GetModuleFileName(hInstance, szPathName, _MAX_PATH);

	CString strTypeName;
	if (! strTypeName.LoadString(idTypeName))
	{
		ASSERT(FALSE);  // Name string not present in resources
		strTypeName = szClassID; // Use Class ID instead
	}

	TCHAR szBitmapID[_MAX_PATH];
	_itot(idBitmap, szBitmapID, 10);

	TCHAR szMiscStatus[_MAX_PATH];
	_ltot(dwMiscStatus, szMiscStatus, 10);

	// Format version string as "major.minor"
	TCHAR szVersion[_MAX_PATH];
	wsprintf(szVersion, _T("%d.%d"), wVerMajor, wVerMinor);

	//  Attempt to open registry keys.
	HKEY hkeyClassID = NULL;
	HKEY hkeyProgID = NULL;

	TCHAR szKey[_MAX_PATH];
	wsprintf(szKey, _T("CLSID\\%s"), szClassID);
	if (::RegCreateKey(HKEY_CLASSES_ROOT, szKey, &hkeyClassID) != ERROR_SUCCESS)
		goto Error;
	if (::RegCreateKey(HKEY_CLASSES_ROOT, pszProgID, &hkeyProgID) != ERROR_SUCCESS)
		goto Error;

	ASSERT(hkeyClassID != NULL);
	ASSERT(hkeyProgID != NULL);

	LPCTSTR rglpszSymbols[7];
	rglpszSymbols[0] = strTypeName;
	rglpszSymbols[1] = szClassID;
	bSuccess = AfxOleRegisterHelperEx(rglpszCtrlProgID, rglpszSymbols, 2, TRUE,
		hkeyProgID);

	if (!bSuccess)
		goto Error;

	rglpszSymbols[1] = pszProgID;
	rglpszSymbols[2] = szPathName;
	rglpszSymbols[3] = szBitmapID;
	rglpszSymbols[4] = szMiscStatus;
	rglpszSymbols[5] = szTypeLibID;
	rglpszSymbols[6] = szVersion;
	bSuccess = AfxOleRegisterHelperEx(rglpszCtrlClassID, rglpszSymbols, 7, TRUE,
		hkeyClassID);

	if (!bSuccess)
		goto Error;

	if (bInsertable)
	{
		bSuccess =
			(::RegSetValue(hkeyProgID, _T("Insertable"), REG_SZ, _T(""), 0) ==
				ERROR_SUCCESS) &&
			(::RegSetValue(hkeyClassID, _T("Insertable"), REG_SZ, _T(""), 0) ==
				ERROR_SUCCESS);
	}

Error:
	if (hkeyProgID != NULL)
		::RegCloseKey(hkeyProgID);

	if (hkeyClassID != NULL)
		::RegCloseKey(hkeyClassID);

	return bSuccess;
}

#ifdef _WIN32
#define INPROCSERVER   _T("InprocServer32")
#define INPROCSERVER_2 _T("InprocServer")
#define TOOLBOXBITMAP  _T("ToolboxBitmap32")
#else
#define INPROCSERVER   _T("InprocServer")
#define INPROCSERVER_2 _T("InprocServer32")
#define TOOLBOXBITMAP  _T("ToolboxBitmap")
#endif

BOOL AFXAPI AfxOleUnregisterClass(REFCLSID clsid, LPCTSTR pszProgID)
{
	// Format class ID as a string
	TCHAR szClassID[GUID_CCH];
	int cchGuid = ::StringFromGUID2(clsid, szClassID, GUID_CCH);

	ASSERT(cchGuid == GUID_CCH);	// Did StringFromGUID2 work?
	if (cchGuid != GUID_CCH)
		return FALSE;

	TCHAR szKey[_MAX_PATH];

	// check to see if a 16-bit InprocServer key is found when unregistering
	// 32-bit (or vice versa).
	wsprintf(szKey, _T("CLSID\\%s\\%s"), szClassID, INPROCSERVER_2);
	HKEY hkey;
	BOOL bSurgical = RegOpenKey(HKEY_CLASSES_ROOT, szKey, &hkey) == ERROR_SUCCESS;

	long error;
	BOOL bRetCode = TRUE;
	if( bSurgical )
	{
		// Only remove the keys specific to this version of the control,
		// leaving things in tact for the other version.
		wsprintf(szKey, _T("CLSID\\%s\\%s"), szClassID, INPROCSERVER);
		error = RegDeleteKey(HKEY_CLASSES_ROOT, szKey);
		bRetCode = bRetCode && _AfxRegDeleteKeySucceeded(error);

		wsprintf(szKey, _T("CLSID\\%s\\%s"), szClassID, TOOLBOXBITMAP);
		error = RegDeleteKey(HKEY_CLASSES_ROOT, szKey);
		bRetCode = bRetCode && _AfxRegDeleteKeySucceeded(error);
	}
	else
	{
		// No other versions of this control were detected,
		// so go ahead and remove the control completely.
		wsprintf(szKey, _T("CLSID\\%s"), szClassID);
		error = _AfxRecursiveRegDeleteKey(HKEY_CLASSES_ROOT, szKey);
		bRetCode = bRetCode && _AfxRegDeleteKeySucceeded(error);

		if (pszProgID != NULL)
		{
			error = _AfxRecursiveRegDeleteKey(HKEY_CLASSES_ROOT, (LPTSTR)pszProgID);
			bRetCode = bRetCode && _AfxRegDeleteKeySucceeded(error);
		}
	}

	return bRetCode;
}


static LPCTSTR BASED_CODE rglpszPropPageClass[] =
{
	_T("\0") _T("%1"),
#ifdef _WIN32
	_T("InprocServer32\0") _T("%2"),
#else
	_T("InprocServer\0") _T("%2"),
#endif
	NULL
};

BOOL AFXAPI  AfxOleRegisterPropertyPageClass(HINSTANCE hInstance,
	REFCLSID clsid, UINT idTypeName)
{
	BOOL bSuccess = FALSE;

	// Format class ID as a string
	TCHAR szClassID[GUID_CCH];
	int cchGuid = ::StringFromGUID2(clsid, szClassID, GUID_CCH);

	ASSERT(cchGuid == GUID_CCH);	// Did StringFromGUID2 work?
	if (cchGuid != GUID_CCH)
		return FALSE;

	TCHAR szPathName[_MAX_PATH];
	::GetModuleFileName(hInstance, szPathName, _MAX_PATH);

	CString strTypeName;
	if (! strTypeName.LoadString(idTypeName))
	{
		ASSERT(FALSE);  // Name string not present in resources
		strTypeName = szClassID; // Use Class ID instead
	}

	HKEY hkeyClassID = NULL;

	TCHAR szKey[_MAX_PATH];
	wsprintf(szKey, _T("CLSID\\%s"), szClassID);
	if (::RegCreateKey(HKEY_CLASSES_ROOT, szKey, &hkeyClassID) != ERROR_SUCCESS)
		goto Error;

	LPCTSTR rglpszSymbols[2];
	rglpszSymbols[0] = strTypeName;
	rglpszSymbols[1] = szPathName;
	bSuccess = AfxOleRegisterHelperEx(rglpszPropPageClass, rglpszSymbols,
		2, TRUE, hkeyClassID);

Error:
	if (hkeyClassID != NULL)
		::RegCloseKey(hkeyClassID);

	return bSuccess;
}


static LPCTSTR BASED_CODE rglpszClsClassID[] =
{
	_T("\0") _T("%1"),
	_T("ProgID\0") _T("%2"),
#ifdef _WIN32
	_T("InprocServer32\0") _T("%3"),
#else
	_T("InprocServer\0") _T("%3"),
#endif
	NULL
};

BOOL AFXAPI _AfxOleRegisterClass(HINSTANCE hInstance, REFCLSID clsid,
	LPCTSTR pszProgID, UINT idTypeName)
{
	BOOL bSuccess = FALSE;

	// Format class ID as a string
	TCHAR szClassID[GUID_CCH];
	int cchGuid = ::StringFromGUID2(clsid, szClassID, GUID_CCH);

	ASSERT(cchGuid == GUID_CCH);	// Did StringFromGUID2 work?
	if (cchGuid != GUID_CCH)
		return FALSE;

	TCHAR szPathName[_MAX_PATH];
	::GetModuleFileName(hInstance, szPathName, _MAX_PATH);

	CString strTypeName;
	if (! strTypeName.LoadString(idTypeName))
	{
		ASSERT(FALSE);  // Name string not present in resources
		strTypeName = szClassID; // Use Class ID instead
	}

	// Attempt to open registry keys.
	HKEY hkeyClassID = NULL;
	HKEY hkeyProgID = NULL;

	TCHAR szKey[_MAX_PATH];
	wsprintf(szKey, _T("CLSID\\%s"), szClassID);
	if (::RegCreateKey(HKEY_CLASSES_ROOT, szKey, &hkeyClassID) != ERROR_SUCCESS)
		goto Error;
	if (::RegCreateKey(HKEY_CLASSES_ROOT, pszProgID, &hkeyProgID) != ERROR_SUCCESS)
		goto Error;

	ASSERT(hkeyClassID != NULL);
	ASSERT(hkeyProgID != NULL);

	LPCTSTR rglpszSymbols[3];
	rglpszSymbols[0] = strTypeName;
	rglpszSymbols[1] = szClassID;
	AfxOleRegisterHelperEx(rglpszCtrlProgID, rglpszSymbols, 2, TRUE, hkeyProgID);

	rglpszSymbols[1] = pszProgID;
	rglpszSymbols[2] = szPathName;
	AfxOleRegisterHelperEx(rglpszClsClassID, rglpszSymbols, 3, TRUE, hkeyClassID);

	bSuccess = TRUE;

Error:
	if (hkeyProgID != NULL)
		::RegCloseKey(hkeyProgID);

	if (hkeyClassID != NULL)
		::RegCloseKey(hkeyClassID);

	return bSuccess;
}


/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
