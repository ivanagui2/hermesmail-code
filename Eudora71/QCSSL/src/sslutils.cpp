// sslutils.cpp
//
// Implementation file for miscellaneous SSL utility routines.
//

#include "stdafx.h"

#include <afx.h>

//
// Loads a string from the DLL's resources.
//
// This function is not actually used!

bool GetResourceString(UINT StringID, char * buffer, int maxbuf)
{
	// Set this so the resource gets loaded from the DLL's resources.
	//
	// Corrected the name of the DLL here.  Was "QCSll.dll"!  (Pete Maclean 4-Sep-2018)
	HINSTANCE hInstance = ::GetModuleHandleW(L"QCSSL.dll");
	int nLen = ::LoadStringA(hInstance, StringID, buffer, maxbuf);
	return (nLen > 0);
}
