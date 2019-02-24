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
//*
//	*	stdafx implementation file.
//	*
//	*
//	*	PURPOSE:
//	*
//	*		This file implements function and/or classes that are used frequently but changed INfrequently. This serves as our Precompiled Header (PCH)
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
// stdafx implemetation BEF [SBROTHY]
//
// TODO: Some of the smaller functions in here could perhaps benefit from being inline.
//
//

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// stdafx.cpp : source file that includes just the standard includes
// Hermes.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GetErrMsg (generic)
TCHAR * GetErrMsg(const DWORD dwErrorCode, const LPCTSTR lpszFunction)
{
	TRACE(_T("void PrintError(DWORD dwErrorCode, LPCTSTR lpszFunction, BOOL bExitProcess) >>\n"));

	static TCHAR szErrMsg[MAX_PATH];	// [SBROTHY]: TODO: MAX_PATH must be enough but, then again it might not.

	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(
		LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)
	);

	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dwErrorCode, lpMsgBuf);

	::lstrcpy(szErrMsg, (LPCTSTR)lpDisplayBuf);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);

	TRACE(_T("void PrintError(DWORD dwErrorCode, LPCTSTR lpszFunction, BOOL bExitProcess) <<\n"));

	return szErrMsg;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PrintError (generic)
void PrintError(const DWORD dwErrorCode, const LPCTSTR lpszFunction, const BOOL bExitProcess)
{
	TRACE(_T("void PrintError(DWORD dwErrorCode, LPCTSTR lpszFunction, BOOL bExitProcess) >>\n"));

	MessageBox(NULL, GetErrMsg(dwErrorCode, lpszFunction), _T("Error"), MB_OK | MB_ICONERROR);

	if (bExitProcess)
	{
		::MessageBox(NULL, _T("Forcibly exiting application...."), _T("Full Stop."), MB_OK | MB_ICONSTOP);
		::ExitProcess(dwErrorCode);
	}

	TRACE(_T("void PrintError(DWORD dwErrorCode, LPCTSTR lpszFunction, BOOL bExitProcess) <<\n"));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PrintError GetLastError()
void SYSError(const LPCTSTR lpszFunction, const BOOL bExitProcess)
{
	TRACE(_T("void SYSError(LPCTSTR lpszFunction, BOOL bExitProcess) >>\n"));

	PrintError(::GetLastError(), lpszFunction, bExitProcess);

	TRACE(_T("void SYSError(LPCTSTR lpszFunction, BOOL bExitProcess) <<\n"));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PrintError WSAGetLastError()
void WSAError(const LPCTSTR lpszFunction, const BOOL bExitProcess)
{
	TRACE(_T("void WSAError(LPCTSTR lpszFunction, BOOL bExitProcess) >>\n"));

	PrintError(::WSAGetLastError(), lpszFunction, bExitProcess);

	TRACE(_T("void WSAError(LPCTSTR lpszFunction, BOOL bExitProcess) << \n"));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GUIDAsString - return a GUID as a string
//
// The char * return is a static manipulative variable.

char *GUIDAsString(GUID guid)
{
	static char tmp[39];

	sprintf_s(tmp, "{%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

	return tmp;
}


//////////////////////////
// This one... 
// 
TCHAR *GUIDAsStringW(GUID guid)
{
	static TCHAR tmp[39];

	
///	_tcprintf_s
	
		swprintf(tmp, 39, _T("{%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}"),
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

	return tmp;
}



/*
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GUIDAsString
TCHAR * GUIDAsString(GUID &g)
{
	static TCHAR clsID[39];

	const HRESULT hr = ::CLSIDFromString((LPOLESTR)clsID, &g);
	if (S_OK != hr)
	{
		::PrintError(hr, _T("::StringFromCLSID: "));
		TRACE(_T("%s\n"), ::GetErrMsg(hr, _T("::StringFromCLSID: ")));
		return NULL;
	}
	
	return clsID;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// StringAsGUID

GUID *StringAsGUID(TCHAR *lpszOleStr)
{
	static GUID g;

	const HRESULT hr = CLSIDFromString(lpszOleStr, &g);
	if (S_OK != hr)
	{
		::PrintError(hr, _T("::CLSIDFromString: "));
		TRACE(_T("%s\n"), ::GetErrMsg(hr, _T("::CLSIDFromString: ")));
		return NULL;
	}

	return &g;
}
*/



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LoadImage()
// Load png and jpeg at least....
BOOL MyLoadImage(CString path, CBitmap &b)
{
	CImage image;
	HRESULT hr = image.Load(path); // just change extension to load jpg
	if (S_OK != hr)
	{
		PrintError(hr, _T("CImage::Load: "));
		return FALSE;
	}

	HBITMAP hBitmap = NULL;
	try 
	{
		hBitmap = image.Detach();
	}
	catch (CException *e)
	{
		SYSError(_T("CImage::Detach: "));
//		e->Dump(*new CDumpContext(NULL));
		e->ReportError();
		e->Delete();
		return FALSE;
	}
	catch (...)
	{
		SYSError(_T("CImage::Detach: "), FALSE);
		return FALSE;
	}

	if(!b.Attach(hBitmap))
	{
		SYSError(_T("Bitmap::Attach: "));
		DeleteObject(hBitmap);

		return FALSE;
	}
	ASSERT(hBitmap);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LoadBitmapFromBMPFile - Well duh 
//
//  __IN		szFileNamne
//	__OUT		phBitMap
//	__OUT		phPalette
//
//	The caller must destroy these.
BOOL LoadBitmapFromBMPFile(LPTSTR szFileName, HBITMAP *phBitmap, HPALETTE *phPalette)
{
	BITMAP  bm;

	// Use LoadImage() to get the image loaded into a DIBSection
	*phBitmap = (HBITMAP)LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if (*phBitmap == NULL)
	{
		return FALSE;
	}


	// Get the color depth of the DIBSection
	GetObject(*phBitmap, sizeof(BITMAP), &bm);
	// If the DIBSection is 256 color or less, it has a color table
	if ((bm.bmBitsPixel * bm.bmPlanes) <= 8)
	{
		HDC           hMemDC;
		HBITMAP       hOldBitmap;
		RGBQUAD       rgb[256];
		LPLOGPALETTE  pLogPal;
		WORD          i;

		// Create a memory DC and select the DIBSection into it
		hMemDC = CreateCompatibleDC(NULL);
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, *phBitmap);
		// Get the DIBSection's color table
		GetDIBColorTable(hMemDC, 0, 256, rgb);
		// Create a palette from the color tabl
		pLogPal = (LOGPALETTE *)malloc(sizeof(LOGPALETTE) + (256 * sizeof(PALETTEENTRY)));
		pLogPal->palVersion = 0x300;
		pLogPal->palNumEntries = 256;
		for (i = 0; i < 256; i++)
		{
			pLogPal->palPalEntry[i].peRed = rgb[i].rgbRed;
			pLogPal->palPalEntry[i].peGreen = rgb[i].rgbGreen;
			pLogPal->palPalEntry[i].peBlue = rgb[i].rgbBlue;
			pLogPal->palPalEntry[i].peFlags = 0;
		}
		*phPalette = CreatePalette(pLogPal);
		// Clean up
		free(pLogPal);
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
	}

	return TRUE;
}


#ifdef _DEBUG

void TRACE_VARIANTTYPE(const CComVariant *pv)
{
	ASSERT(pv);

	if (pv == NULL && pv->vt == NULL)
	{
		return;
	}

	VARTYPE vt = pv->vt;

	switch (vt)
	{
	case VT_NULL:
		TRACE(_T("\tVT_NULL\n"));
		break;
	case VT_EMPTY:
		TRACE(_T("\tVT_EMPTY\n"));
		break;
	case VT_I2:
		TRACE(_T("\tVT_I2\n"));
		break;
	case VT_I4:
		TRACE(_T("\tVT_I4\n"));
		break;
	case VT_R4:
		TRACE(_T("\tVT_R4\n"));
		break;
	case VT_R8:
		TRACE(_T("\tVT_R8\n"));
		break;
	case VT_CY:
		TRACE(_T("\tVT_CY\n"));
		break;
	case VT_DATE:
		TRACE(_T("\tVT_DATE\n"));
		break;
	case VT_BSTR:
		TRACE(_T("\tVT_BSTR\n"));
		break;
	case VT_DISPATCH:
		TRACE(_T("\tVT_DISPATCH\n"));
		break;
	case VT_ERROR:
		TRACE(_T("\tVT_ERROR\n"));
		break;
	case VT_BOOL:
		TRACE(_T("\tVT_BOOL\n"));
		break;
	case VT_VARIANT:
		TRACE(_T("\tVT_VARIANT\n"));
		break;
	case VT_UNKNOWN:
		TRACE(_T("\tVT_UNKNOWN\n"));
		break;
	case VT_DECIMAL:
		TRACE(_T("\tVT_DECIMAL\n"));
		break;
	case VT_I1:
		TRACE(_T("\tVT_I1\n"));
		break;
	case VT_UI1:
		TRACE(_T("\tVT_UI1\n"));
		break;
	case VT_UI2:
		TRACE(_T("\tVT_UI2\n"));
		break;
	case VT_UI4:
		TRACE(_T("\tVT_UI4\n"));
		break;
	case VT_I8:
		TRACE(_T("\tVT_I8\n"));
		break;
	case VT_UI8:
		TRACE(_T("\tVT_UI8\n"));
		break;
	case VT_INT:
		TRACE(_T("\tVT_INT\n"));
		break;
	case VT_UINT:
		TRACE(_T("\tVT_UINT\n"));
		break;
	case VT_HRESULT:
		TRACE(_T("\tVT_HRESULT\n"));
		break;
	case VT_PTR:
		TRACE(_T("\tVT_PTR\n"));
		break;
	case VT_SAFEARRAY:
		TRACE(_T("\tVT_SAFEARRAY\n"));
		break;
	case VT_CARRAY:
		TRACE(_T("\tVT_CARRAY\n"));
		break;
	case VT_USERDEFINED:
		TRACE(_T("\tVT_USERDEFINED\n"));
		break;
	case VT_LPSTR:
		TRACE(_T("\tVT_LPSTR\n"));
		break;
	case VT_LPWSTR:
		TRACE(_T("\tVT_LPWSTR\n"));
		break;
	case VT_RECORD:
		TRACE(_T("\tVT_RECORD\n"));
		break;
	case VT_INT_PTR:
		TRACE(_T("\tVT_INT_PTR\n"));
		break;
	case VT_UINT_PTR:
		TRACE(_T("\tVT_UINT_PTR\n"));
		break;
	case VT_ARRAY:
		TRACE(_T("\tVT_ARRAY\n"));
		break;
	case VT_BYREF:
		TRACE(_T("\tVT_BYREF\n"));
		break;

	default:
		TRACE(_T("\t[UNDEFINED]\n"));
		break;
	};
}




void TRACE_IMAGELIST(const CImageList &imgList)
{
	const int numImages = imgList.GetImageCount();

	IMAGEINFO inf;

	for (int n = 0; n < numImages; n++)
	{
		memset(&inf, 0, sizeof(inf));
		imgList.GetImageInfo(n, &inf);
 		TRACE("IMAGE %d:", n);
		TRACE_IMAGEINFO(&inf);
   	}
}
#endif //  _DEBUG


///////////////////////////////////////////////////
/* Build COLORREF from CString
COLORREF ColorRefFromCString(const CString & strValue)
{
	MyColor colorData;
	unsigned short wTempValues[3];

	unsigned short & wRed = wTempValues[0];
	unsigned short & wGreen = wTempValues[1];
	unsigned short & wBlue = wTempValues[2];

	sscanf(reinterpret_cast<LPCSTR>(strValue), "RGB(%hu,%hu,%hu)", &wRed, &wGreen, &wBlue);

	colorData.Blue = static_cast<unsigned char>(wBlue);
	colorData.Green = static_cast<unsigned char>(wGreen);
	colorData.Red = static_cast<unsigned char>(wRed);
	colorData.Alpha = 0;

	return reinterpret_cast<COLORREF &>(colorData);
}
*/

/*
CHermesSettings * GetAppSettings()
{
	TRACE("CHermesSettings * GetAppSettings() >>\n");

	CWinApp * pApp = ::AfxGetApp();
	ASSERT(pApp);
	CWinAppEx *pAppEx = reinteroret_cast(CWinAppEx *)pApp;
	CHermesApp *pHermesApp = reinteroret_cast<CHermesApp *>(pAppEx);
	CHermesSettings *settings = pHermesApp->GetSettings();
	ASSERT(settings);

	// DEBUG
	TRACE("CHermesSettings * GetAppSettings() <<\n");

	return settings;
}

*/





/*
BOOL SaveComToArchive(CArchive& ar, IPersistStreamInit* pObject)
{
	// Note: This is a pseudo code - it will not compile!

	// Create IStream


	HGLOBAL handle = GlobalAlloc(GMEM_FIXED, sizeof(*pObject));
	if(NULL == handle)
	{
		::PrintError(L"GlobalAlloc: ");
		return;
	)

	LPSTREAM pStream;
	HRESULT err = CreateStreamOnHGlobal(::GlobalAlloc(), TRUE, pStream);
	if (err != S_OK)
	{
		SYSErr(err);
		return FALSE;
	}


	// Write COM object's class UUID!
	pStream->WriteObjectClassUUID(pObject->GetClassUUID());

	// save the object
	pObject->Save(pStream);

	// Get the memory block maintained by the IStream
	HGLOBAL hMem;
	hMem = pStream->GetGlobalMemory();

	// Copy the block on the CArchive
	ar.Write(hMem, size_of_hMem_block);

	// Release the IStream
	pStream->Release;
}

*/


// stdafx implemetation EOF [SBROTHY]
////////////////////////////////////////////////////////////////////////////////






