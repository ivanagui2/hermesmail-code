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

File revised by Jeff Prickett (kg4ygs@gmail.com) on July 8, 2018
    Removed references to the Stingray Toolkit and replace them with references
    to the Hermes UI Toolkit.

*/    

// QCMbxCustomizeToolbarPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "QCCommandActions.h"
#include "QCCommandStack.h"
#include "QCMailboxCommand.h"
#include "QCMailboxDirector.h"
#include "QCMbxCustomizeToolbarPage.h"
#include "rs.h"


//  Commentedout this #include to a nonexistent file. Should it turn up again and prove useful,
//  we can  uncomment it. I'm not optimistic  about it though. (8/9/18. soren <sbrothy@gmail.com>)
//#include "DebugNewHelpers.h"


extern QCCommandStack		g_theCommandStack;
extern QCMailboxDirector	g_theMailboxDirector;

static UINT BASED_CODE theMailboxButtons[] =
{
	ID_MAILBOX_USER,
	ID_TRANSFER_USER
};

		
static UINT BASED_CODE theInButtons[] =
{
	ID_MAILBOX_IN,
	ID_TRANSFER_USER
};

		
static UINT BASED_CODE theOutButtons[] =
{
	ID_MAILBOX_OUT,
	ID_TRANSFER_USER
};

		
static UINT BASED_CODE theJunkButtons[] =
{
	ID_MAILBOX_JUNK,
	ID_TRANSFER_USER
};

		
static UINT BASED_CODE theTrashButtons[] =
{
	ID_MAILBOX_TRASH,
	ID_TRANSFER_USER
};

		
/////////////////////////////////////////////////////////////////////////////
// QCMailboxCustomizeToolbarPage property page

IMPLEMENT_DYNCREATE(QCMailboxCustomizeToolbarPage, QCToolBarCmdPage)

QCMailboxCustomizeToolbarPage::QCMailboxCustomizeToolbarPage() : QCToolBarCmdPage(QCMailboxCustomizeToolbarPage::IDD, IDR_MAILBOXES)
{
	//{{AFX_DATA_INIT(QCMailboxCustomizeToolbarPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

QCMailboxCustomizeToolbarPage::~QCMailboxCustomizeToolbarPage()
{
}

void QCMailboxCustomizeToolbarPage::DoDataExchange(CDataExchange* pDX)
{
	QCToolBarCmdPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(QCMailboxCustomizeToolbarPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


UINT QCMailboxCustomizeToolbarPage::TranslateID(UINT uID)
{
	COMMAND_ACTION_TYPE theAction;

	switch (uID)
	{
	case ID_MAILBOX_USER:	theAction = CA_OPEN;		break;
	case ID_TRANSFER_USER:	theAction = CA_TRANSFER_TO;	break;
	default:
		return uID;
	}

	HTREEITEM hItem = m_theTreeCtrl.GetSelectedItem();
	if (!hItem)
	{
		ASSERT(0);
		return 0;
	}

	QCMailboxCommand* pCommand =
		((QCMailboxTreeCtrl::CMboxTreeItemData*)m_theTreeCtrl.GetItemData(hItem))->m_pCommand;
	if (!pCommand)
	{
		ASSERT(0);
		return 0;
	}

	UINT uNewID = g_theCommandStack.FindOrAddCommandID(pCommand, theAction);

	return uNewID;
}


BEGIN_MESSAGE_MAP(QCMailboxCustomizeToolbarPage, QCToolBarCmdPage)
	//{{AFX_MSG_MAP(QCMailboxCustomizeToolbarPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY( TVN_SELCHANGED, IDC_TREE_CTRL, OnSelChanged )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// QCMailboxCustomizeToolbarPage message handlers

BOOL QCMailboxCustomizeToolbarPage::OnInitDialog() 
{
	CStatic*	pLabel;
	CRString	szTitle( IDR_MAILBOXES );

	m_nCurSel = 0;

	DefineBtnGroup(_T("In"), NUMELEMENTS(theInButtons), theInButtons);
	DefineBtnGroup(_T("Out"), NUMELEMENTS(theOutButtons), theOutButtons);
	DefineBtnGroup(_T("Junk"), NUMELEMENTS(theJunkButtons), theJunkButtons);
	DefineBtnGroup(_T("Trash"), NUMELEMENTS(theTrashButtons), theTrashButtons);
	DefineBtnGroup(_T("Mailboxes"), NUMELEMENTS(theMailboxButtons), theMailboxButtons);

	QCToolBarCmdPage::OnInitDialog();
	
	pLabel = ( CStatic* ) GetDlgItem( IDC_TREE_ITEMS_STATIC );
	
	if( pLabel != NULL )
	{
		pLabel->SetWindowText( 	szTitle );	
	}
	
	m_theTreeCtrl.SubclassDlgItem( IDC_TREE_CTRL, this );
	m_theTreeCtrl.Init();
	g_theMailboxDirector.InitializeMailboxTreeControl( &m_theTreeCtrl, 0, NULL );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void QCMailboxCustomizeToolbarPage::OnSelChanged(
NMHDR*		pNMHDR, 
LRESULT*	pResult) 
{
	QCMailboxTreeCtrl::ItemType		theItemType;
	NM_TREEVIEW*					pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	INT								iIndex;
	HTREEITEM						hItem;
	CTreeCtrl*						pTree;
	CString							szName;
	HRMCustomToolBar*				pToolBar;
	UINT							nStyle;

	*pResult = 0;

	hItem = pNMTreeView->itemNew.hItem;

	pTree = ( CTreeCtrl* ) GetDlgItem( IDC_TREE_CTRL );
	
	if( pTree == NULL )
	{
		ASSERT( 0 );
		return;
	}

	// Is this really a sel change?
	if (pNMTreeView->action == TVC_UNKNOWN)
		return;

	theItemType = ( ( QCMailboxTreeCtrl::CMboxTreeItemData* ) m_theTreeCtrl.GetItemData( hItem ) )->m_itemType;

	if(m_nCurSel != LB_ERR)
	{
		pToolBar = ((HRMBtnGroup*) m_btnGroups[m_nCurSel])->pToolBar;
		
		pToolBar->EnableConfigMode(FALSE);

		for( iIndex = 0; iIndex < pToolBar->GetBtnCount(); iIndex ++ )
		{
			nStyle = pToolBar->GetButtonStyle(iIndex) | TBBS_DISABLED;
			pToolBar->SetButtonStyle( iIndex, nStyle);
		}
		
		m_nCurSel = LB_ERR;
	}

	switch( theItemType )
	{
		case QCMailboxTreeCtrl::ITEM_IN_MBOX:
			szName = "In";	
		break;
		case QCMailboxTreeCtrl::ITEM_OUT_MBOX:
			szName = "Out";	
		break;
		case QCMailboxTreeCtrl::ITEM_JUNK_MBOX:
			szName = "Junk";	
		break;
		case QCMailboxTreeCtrl::ITEM_TRASH_MBOX:
			szName = "Trash";	
		break;
		case QCMailboxTreeCtrl::ITEM_USER_MBOX:
			szName = "Mailboxes";	
		break;
		case QCMailboxTreeCtrl::ITEM_IMAP_MAILBOX:
			szName = "Mailboxes";
		break;

		default:
			return;
	}
	
	// The current selection has changed. I need to show the newly selected
	// toolbar
	for( iIndex = 0; iIndex < m_btnGroups.GetSize(); iIndex ++)
	{
		if( szName == ((HRMBtnGroup*) m_btnGroups[iIndex])->lpszTitle )
		{
			break;
		}
	}

	if( iIndex == m_btnGroups.GetSize() )
	{
		ASSERT( 0 );
		return;
	}

	for(int i=0; i<m_btnGroups.GetSize(); i++)
	{
		HRMCustomizeToolBar* pBar = ((HRMBtnGroup*) m_btnGroups[i])->pToolBar;
		if(i == iIndex)
		{
			pBar->EnableConfigMode(TRUE);
			pBar->ShowWindow(SW_SHOW);
			m_nCurSel = i;
		}
		else
			pBar->ShowWindow(SW_HIDE);
	}
}



