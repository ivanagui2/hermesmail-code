////////////////////////////////////////////////////////////////////////
// EUMAPI.CPP
//
// Main module for 32-bit Hermes MAPI DLL.  This common file
// is used for both versions, with the appropriate #ifdef's, of course.
////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
//#include <string.h>
//#include <afxwin.h>		// FORNOW, might be better to use precompiled stdafx


/////////////////////////////////////////////////////////////////////////////
// Library init

class CHermesMapiDLL : public CWinApp
{
public:
	virtual BOOL InitInstance(); // Initialization
	virtual int ExitInstance();  // Termination (WEP-like code)

	// nothing special for the constructor
	CHermesMapiDLL(LPCTSTR pszAppName) : CWinApp(pszAppName) { }
};

BOOL CHermesMapiDLL::InitInstance()
{
	// any DLL initialization goes here
#ifdef _DEBUG
	::OutputDebugStringW(L"EUMAPI32.DLL initializing\n");
#endif // _DEBUG

	SetDialogBkColor();     // grey dialogs in the DLL as well

	return TRUE;
}

int CHermesMapiDLL::ExitInstance()
{
	// any DLL termination goes here (WEP-like code)
	return CWinApp::ExitInstance();
}

CHermesMapiDLL  NEAR eudoraMapiDLL(_T("eumapi32.dll"));


