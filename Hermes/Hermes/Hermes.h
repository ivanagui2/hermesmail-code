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

// Hermes.h : main header file for the Hermes application
//
#pragma once

#include "stdafx.h"
// [SBROTHY]: Include HermesSettings (options) 
#include "HermesSettings.h"

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CHermesApp:
// See Hermes.cpp for the implementation of this class
//


// [SBROTHY]: Typedef for getting all dopcument instances from the document server template
// [SBROTHY]: https://docs.microsoft.com/en-us/cpp/mfc/document-templates-and-the-document-view-creation-process?view=vs-2017
// 
typedef CTypedPtrList<CObList, CDocument*> DocsPtrList;
//



class CHermesApp : public CWinAppEx
{
public:
	CHermesApp() noexcept;

// Attributes
	CHermesSettings	*m_pSettings;

// Implementation
	CHermesSettings * GetSettings() const;
	void GetAllDocuments(DocsPtrList& listDocs);

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHermesApp theApp;
