// EudoraRes.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

//  Commentedout this #include to a nonexistent file. Should it turn up again and prove useful,
//  we can  uncomment it. I'm not optimistic  about it though. (8/9/18. soren <sbrothy@gmail.com>)
//#include "DebugNewHelpers.h"


static AFX_EXTENSION_MODULE EudoraResDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("EudoraRes.dll Initializing!\n");
		
		AfxInitExtensionModule(EudoraResDLL, hInstance);

		DEBUG_NEW CDynLinkLibrary(EudoraResDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("EudoraRes.dll Terminating!\n");
	}
	return 1;
}
