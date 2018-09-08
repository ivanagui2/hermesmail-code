/* Copyright (c) 2016, Computer History Museum 
All rights reserved. 
Redistribution and use in source and binary forms, with or without modification, are permitted (subject to 
the limitations in the disclaimer below) provided that the following conditions are met: 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
   disclaimer in the documentation and/or other materials provided with the distribution. 
 * Neither the name of Computer History Museum nor the names of its contributors may be used to endorse or promote products 
   derived from this software without specific prior written permission. 
NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE 
COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH 
DAMAGE. */

/*

HERMES MESSENGER SOFTWARE LICENSE AGREEMENT | Hermes Messenger Client Source Code
Copyright (c) 2018, Hermes Messenger Development Team. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted (subject to the limitations in the disclaimer below) provided that 
the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list 
of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this 
list of conditions and the following disclaimer in the documentation and/or 
other materials provided with the distribution.

Neither the name of Hermes Messenger nor the names of its contributors
may be used to endorse or promote products derived from this software without 
specific prior written permission.

NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY’S PATENT RIGHTS ARE GRANTED BY THIS 
LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
“AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

File revised by Jeff Prickett (kg4ygs@gmail.com) on July 6, 2018
    Removed references to Qualcomm's Shareware Manager.

*/    

// SearchDoc.cpp : implementation file
//
 
#include "stdafx.h"
#include "resource.h"
#include "rs.h"
#include "summary.h" 
#include "doc.h"
#include "mdichild.h"
#include "tocdoc.h"
#include "msgframe.h"
#include "compmsgd.h"
#include "guiutils.h"
#include "progress.h"
#include "mainfrm.h"
#include "SearchView.h"
#include "cursor.h"
#include "ReadMessageDoc.h"
#include "QCProtocol.h"
#include "QCSharewareManager.h"
#include "fileutil.h"


#ifdef IMAP4
#include "ImapMailbox.h"
#endif // IMAP4

#include "SearchDoc.h"

// --------------------------------------------------------------------------

//  Commentedout this #include to a nonexistent file. Should it turn up again and prove useful,
//  we can  uncomment it. I'm not optimistic  about it though. (8/9/18. soren <sbrothy@gmail.com>)
//#include "DebugNewHelpers.h"


// --------------------------------------------------------------------------

// One global SearchDoc to store user info that will be carried over to next
//  invoke of window. View should populate from the doc.
CSearchDoc	*gSearchDoc = NULL;  

// --------------------------------------------------------------------------
// CSearchDoc

IMPLEMENT_DYNCREATE(CSearchDoc, CDoc)


BEGIN_MESSAGE_MAP(CSearchDoc, CDocument)
	//{{AFX_MSG_MAP(CSearchDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// --------------------------------------------------------------------------
// CSearchDoc [protected, constructor]
//
////////////////////////////////////////////////////////////////////////
CSearchDoc::CSearchDoc()
{
	ASSERT(gSearchDoc == NULL);
	gSearchDoc = this;
}

////////////////////////////////////////////////////////////////////////
// ~CSearchDoc [protected, destructor]
//
////////////////////////////////////////////////////////////////////////
CSearchDoc::~CSearchDoc()
{
	gSearchDoc = NULL;
}


CSearchView * CSearchDoc::GetSearchView()
{
	CSearchView *	pSearchView = NULL;
	CView *			pView = NULL;
	POSITION		pos = GetFirstViewPosition();   
	if (pos != NULL)  
		pView = GetNextView(pos);

	ASSERT_VALID(pView);
	ASSERT_KINDOF(CSearchView, pView);

	if ( pView && pView->IsKindOf(RUNTIME_CLASS(CSearchView)) )
		pSearchView = dynamic_cast<CSearchView *>(pView);

	return pSearchView;
}


void CSearchDoc::OnFileSave()
{
/*
	if ( m_strPathName.GetLength() )
		OnSaveDocument(m_strPathName);
	else
		OnFileSaveAs();
*/

	//	For now always make "Save" behave like "Save As...". Since we continue
	//	to be limited to one "Find Messages" window I'm not sure it makes sense
	//	to save over the last saved search when the user next saves. This may
	//	make it slightly more painful for a user to slightly tweak and re-save
	//	a search, but at least the user will know exactly what is being saved.
	OnFileSaveAs();
}


// If we get here looking for whether or not to enable the File->Save As...
// menu item, then we don't want to enable it.  This has to be done because
// CDocument has a default handler that we don't want called.
void CSearchDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}


void CSearchDoc::OnFileSaveAs()
{
	CString		szSavedSearchDir = EudoraDir + CRString(IDS_IDX_SRCH_MGR_SAVED_SEARCHES_DIR);

	//	Check to see if it exists
	BOOL		bDirectoryExists = FileExistsMT(szSavedSearchDir);

	//	Create it if it doesn't exist
	if (!bDirectoryExists)
		bDirectoryExists = CreateDirectory(szSavedSearchDir, NULL);

	if (bDirectoryExists)
	{
		CString			strSuggestedFileName;
		const char *	szSuggestedFileName = NULL;

		CSearchView *	pSearchView = GetSearchView();

		if (pSearchView)
		{
			//	Get the suggested file name that represents the first line of the search
			pSearchView->GetSuggestedFileName(strSuggestedFileName);
			if ( !strSuggestedFileName.IsEmpty() )
				szSuggestedFileName = strSuggestedFileName;
		}
		
		CFileDialog theDlg(	FALSE, CRString(IDS_SAVED_SEARCH_EXTENSION),
							szSuggestedFileName, 
							OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
							CRString(IDS_XML_FILE_FILTER), NULL );

		theDlg.m_ofn.lpstrInitialDir = szSavedSearchDir;

		if ( theDlg.DoModal() == IDOK )
			OnSaveDocument( m_strPathName = theDlg.GetPathName() );	
	}
	else
	{
		ErrorDialog(IDS_ERR_DIR_CREATE, szSavedSearchDir);
	}
}


////////////////////////////////////////////////////////////////////////
// OnNewDocument [protected, virtual]
//
////////////////////////////////////////////////////////////////////////
BOOL CSearchDoc::OnNewDocument()
{		
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(CRString(IDS_SEARCH_TITLE));
	return TRUE;
}


////////////////////////////////////////////////////////////////////////
// OnCloseDocument [protected, virtual]
//
////////////////////////////////////////////////////////////////////////
void
CSearchDoc::OnCloseDocument()
{   
	//
	// Setting CDocument::m_bAutoDelete to FALSE forces MFC to bypass
	// the destruction of this object.  This allows the same CSearchDoc
	// object to live across multiple invocations of the CSearchView 
	// dialog.  However, it does force us to "manually" delete the
	// global 'gSearchDoc' object (i.e. ourselves) when we shutdown the
	// program.
	//
	this->m_bAutoDelete = FALSE;
	CDoc::OnCloseDocument();
}


BOOL
CSearchDoc::OnSaveDocument(const char * pszPathName)
{
	CSearchView *	pSearchView = GetSearchView();

	if (pSearchView)
		pSearchView->WriteCurrentSearchCriteria(pszPathName);

	return TRUE;
}

