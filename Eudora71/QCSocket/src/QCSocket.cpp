// QCSocket.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "QCWorkerSocket.h"

//  Commentedout this #include to a nonexistent file. Should it turn up again and prove useful,
//  we can  uncomment it. I'm not optimistic  about it though. (8/9/18. soren <sbrothy@gmail.com>)
//#include "DebugNewHelpers.h"



static AFX_EXTENSION_MODULE QCSocketDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("QCSOCKET.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(QCSocketDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		DEBUG_NEW CDynLinkLibrary(QCSocketDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("QCSOCKET.DLL Terminating!\n");
		//Do some cleanup first SK
		QCWorkerSocket::RemoveAsyncThread();
			
		// Terminate the library before destructors are called
		AfxTermExtensionModule(QCSocketDLL);
	}
	return 1;   // ok
}
