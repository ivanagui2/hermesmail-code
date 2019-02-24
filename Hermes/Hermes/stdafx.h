//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Cop't 1991-2005 Qualcomm, 2019 HERMES, use of this software is governed by these terms
// 
//	Copyright 2019 HERMES Team
// 
//	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met :
//
//	1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
//	
//	2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
//
//	3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
//
//	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
//	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;  OR BUSINESS INTERRUPTION) 
//	HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
//	EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	*	stdafx declaration file.
//	*
//	*
//	*	PURPOSE:
//	*
//	*		This file declares function and/or classes that are used frequently but changed INfrequently. This serves as our Precompiled Header (PCH)
//	*
//	*	TODO:
//	*
//	*		Boost will be included here. Both for it's REGEXes for validating emails as far as possible, and for that bimap [sic!] we´re gonna use. I'm sure there are lots of stuff we can get from free
//	*		from Boost. Everything from complex containers and CRC/encryption files
//	*
//	*	[SBROTHY@GMAIL.COM]
//	*
//


////////////////////////////////////////////////////////////////////////////////
// stdafx Header BEF [SBROTHY]
//
// Functions and or classes that are used frequently but changed infrequently are kept here.
//
// FILE HAS BEEN CHANGED HEAVILY.SYSTEM WIDE FUNCTIONS THAT CHANGE SELDOMLY BUT ARE USED HEAVILY ARE KEPT HERE. ERROR 
// HANDLERS AND THE-LIKE....
//
// TODO: Clean up in #includes that may or may not be needed anymore.
// TODO: Check that #defines that inluence how, for instance, string functions are handled may be needed here. Or indeed there may be too many.
//
// [SBROTHY] 01/01  2019//
//

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afx.h>
#include <atlbase.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxole.h>         // MFC OLE classes
#include <oleauto.h>
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#include <winver.h>


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars
#include <strsafe.h>			// StringCchPrintf
#include <afxsock.h>            // MFC socket extensions
#include <afxrich.h>            // MFC rich edit classes

// C++ includes. "using namespace std;" are put in files where they are needed. Not globally, as in here.
#include <vector>
#include <map>
#include <utility>
#include <functional>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>

// TODO: Boost will be included here.... [SBROTHY]


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif




//////////////////////////////////////////////////////////////////////////////////////////////////////
// Generic Error handlers
TCHAR * GetErrMsg(const DWORD dwErrorCode, const LPCTSTR lpszFunction);
void PrintError(const DWORD dwErrorCode, const LPCTSTR lpszFunction, const BOOL bExitProcess = FALSE);
void SYSError(const LPCTSTR lpszFunction, const BOOL bExitProcess = FALSE);
void WSAError(const LPCTSTR lpszFunction, const BOOL bExitProcess = FALSE);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Conversion from GUID to string and vice-versa.

// [SBROTHY]: I'm aware of two other methofs. This Shell .Dll from which the proper GUIDFromString and StringFromGUID can be GetProcedureAddressed from. 
// [SBROTHY]: I'm also aware od the CLSIDFromString and the StringfromCLSID functions. I'm just not sure they server our best interest. So we roll our own 
// [SBROTHY]: for the time being.

char *GUIDAsString(GUID guid);
TCHAR *GUIDAsStringW(GUID guid);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LoadBitmapFromBMPFile - Guess what this function does...
//
//  __IN		szFileNamne
//	__OUT		phBitMap
//	__OUT		phPalette
//
//	The caller must destroy these.
BOOL LoadBitmapFromBMPFile(LPTSTR szFileName, HBITMAP *phBitmap, HPALETTE *phPalette);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Load an image like for instance png or jpeg and return it as a bitmap
BOOL MyLoadImage(CString path, CBitmap &b);



// [SBROTHY]: Various TRACE functions....


#ifdef _DEBUG 

inline void TRACE_RECT(const RECT &pRect)
{
	TRACE("[left=%ld][top=%ld][right=%ld][bottom=%ld]", pRect.left, pRect.top, pRect.right, pRect.bottom);
}

void TRACE_IMAGEINFO(const IMAGEINFO *pInf)
{
	TRACE("[hbmImage=%ld]", pInf->hbmImage);
	TRACE("[hbmMask=%ld]", pInf->hbmMask);
	TRACE("[Unused1=%d]", pInf->Unused1);
	TRACE("[Unused2=%d]", pInf->Unused2);
	TRACE_RECT(pInf->rcImage);
	TRACE("\n");
}

///////////////////////////////////////////////////
// TRACE _variant_t vt type
//
// This one TRACEs the vt type of _variant_t
void TRACE_VARIANTTYPE(const CComVariant *pv);

void TRACE_IMAGELIST(const CImageList &imgList);


#endif // _DEBUG 


	///////////////////////////////////////////////////
	// Build COLORREF from CString
	//COLORREF ColorRefFromCString(const CString & strValue)

	// Yes. It´s the most annoying name in all of VSC++, so we redefine it
#define ODS OutputDebugString 




///////////////////////////////////////////////////

// Debug help macros

#ifdef _DEBUG
/*

void Dump(LPCREATESTRUCTA pCSA)
{
	TCHAR buf[MAX_PATH];

	LPCLIENTCREATESTRUCT c = (LPCLIENTCREATESTRUCT)pCSA->lpCreateParams;
	::_stprintf_s(buf, _T("HANDLE hWindowsMenu=[%d], UINT idFirstChild=[%d]\n"), (int)c->hWindowMenu, c->idFirstChild);

	::_stprintf_s(buf, _T("HINSTANCE hInstance=[%n]\n"), (int)pCSA->hInstance);
	TRACE(buf);

	::_stprintf_s(buf, _T("HMENU hMenu=[%d]\n"), (int)pCSA->hMenu);
	TRACE(buf);

	::_stprintf_s(buf, _T("HMENU hMenuParent=[%d]\n"), (int)pCSA->hwndParent);
	TRACE(buf);

	::_stprintf_s(buf, _T("int cs: [%d], int cy: [%d], int x: [%d], int y: [%d]\n"), pCSA->cx, pCSA->cy, pCSA->x, pCSA->y);
	TRACE(buf);

	::_stprintf_s(buf, _T("Long style: [%ld]"), pCSA->style);
	TRACE(buf);

	::_stprintf_s(buf, _T("LPCTSTR lpszName: [%s]\n"), (LPCTSTR)pCSA->lpszName);
	TRACE(buf);

	::_stprintf_s(buf, _T("LPCTSTR lpszClass: [%s]\n"), (LPCTSTR)pCSA->lpszClass);
	TRACE(buf);


	::_stprintf_s(buf, _T("DWORD dwExStyle: [%ui]\n"), pCSA->dwExStyle);
	TRACE(buf);
}

*/


#endif // :DEBUG


// stdafx Header EOF [SBROTHY]
////////////////////////////////////////////////////////////////////////////////

