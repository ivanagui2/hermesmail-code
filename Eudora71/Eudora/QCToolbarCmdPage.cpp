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
    Removed references to the Stingray Toolkit and replaced them with references
    to the Hermes UI Toolkit.

*/    

// QCToolBarCmdPage.cpp : implementation file
//

#include "stdafx.h"
#include "QCToolBarCmdPage.h"
#include "QCCustomizeToolBar.h"


#include "DebugNewHelpers.h"


/////////////////////////////////////////////////////////////////////////////
// QCToolBarCmdPage property page

IMPLEMENT_DYNAMIC(QCToolBarCmdPage, HRMToolBarCmdPage)

QCToolBarCmdPage::QCToolBarCmdPage( UINT uID, UINT uTitleID ) : HRMToolBarCmdPage( uID, uTitleID )
{
	//{{AFX_DATA_INIT(QCToolBarCmdPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

QCToolBarCmdPage::~QCToolBarCmdPage()
{
}

void QCToolBarCmdPage::DoDataExchange(CDataExchange* pDX)
{
	HRMToolBarCmdPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(QCToolBarCmdPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(QCToolBarCmdPage, HRMToolBarCmdPage)
	//{{AFX_MSG_MAP(QCToolBarCmdPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// QCToolBarCmdPage message handlers

BOOL QCToolBarCmdPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// Calc. where to create the toolbars on the page by positioning them
	// slightly inside the IDC_TOOLBAR_FRAME static control.
	WINDOWPLACEMENT wp;
	wp.length = sizeof(wp);
	GetDlgItem(IDC_TOOLBAR_FRAME)->GetWindowPlacement(&wp);

	CRect rect = wp.rcNormalPosition;
	int x = 6;			// in DBU
	int y = 8;
	HRMConvertDialogUnitsToPixels(*(GetFont()), x, y);
	rect.DeflateRect(x, y);

	int nID = AFX_IDW_TOOLBAR + 0xff;
	for (int i = 0; i < m_btnGroups.GetSize(); i++, nID--)
	{
		// Create and initialise the toolbar
		HRMBtnGroup* pGroup = (HRMBtnGroup*)m_btnGroups[i];
		HRMCustomizeToolBar* pToolBar = DEBUG_NEW QCCustomizeToolBar;
		pGroup->pToolBar = pToolBar;

		DWORD dwStyle = CBRS_BORDER_3D | CBRS_SIZE_DYNAMIC | CBRS_FLOATING;
		DWORD dwExStyle = 0L;
		if (m_pManager && m_pManager->FlyByEnabled())
			dwStyle |= CBRS_FLYBY;

		if (m_pManager && m_pManager->CoolLookEnabled())
			dwExStyle |= CBRS_EX_COOLBORDERS | CBRS_EX_GRIPPER;

		if(!pToolBar->CreateEx(dwExStyle, this, dwStyle, nID))
			return FALSE;

		if (m_pManager)
		{
			// Initialise bitmap for toolbar
			m_pManager->SetToolBarInfo(pToolBar);

			pToolBar->SetManager(m_pManager);

			// Set owner - used for flyby help
			pToolBar->SetOwner(m_pManager->GetFrameWnd());
		}

		if (!pToolBar->SetButtons(pGroup->lpBtnIDs, pGroup->nBtnCount))
			return FALSE;

		// Position and size the new toolbar.
		CRect r = rect;
		pToolBar->SetWindowPos(&wndTop, r.left, r.top, r.Width(), r.Height(), SWP_DRAWFRAME);
		pToolBar->DoSize(r);

		// All toolbars are hidden except the very first one.
		pToolBar->ShowWindow(i == 0? SW_SHOWNA : SW_HIDE);
	}

	return TRUE;
}


BOOL QCToolBarCmdPage::OnSetActive()
{
	INT iPos;
	QCCustomizeToolBar*	pBar;
	HRMBtnGroup*		pBtnGroup;

	if(m_pManager)
	{
		// Put all toolbars into config mode.
		m_pManager->EnableConfigMode(TRUE);

		// Re-enable the main frame - so toolbar buttons can be dragged
		m_pManager->EnableMainFrame();

		// Don't allow buttons to be dropped on me.
		m_pManager->SetNoDropWnd(GetParent());

		// for the same reason as below, we need to update any bitmap changes		
		for( iPos = 0; iPos < m_btnGroups.GetSize(); iPos ++ )
		{
			pBtnGroup = ( HRMBtnGroup* ) m_btnGroups[ iPos ];		
			pBar = ( QCCustomizeToolBar* ) ( pBtnGroup->pToolBar );		
			if( pBar->IsKindOf( RUNTIME_CLASS( QCCustomizeToolBar ) ) )
			{
				m_pManager->SetToolBarInfo( pBar );
			}
		}
	}

	// EnableConfigMode will only work on those toolbars returned by
	// the main frames GetControlBar function. Since my custom toolbars do
	// not have the main frame as a parent, they will not be in the list.
	// I therefore do this by hand.
	
#ifdef ORIGINAL
	CListBox* pList = (CListBox*) GetDlgItem(IDC_TOOLBAR_LIST);
	int nIndex = pList->GetCurSel();
	if(nIndex != LB_ERR)
	{
		((HRMBtnGroup*) m_btnGroups[nIndex])->pToolBar->EnableConfigMode(TRUE);
		m_nCurSel = nIndex;
	}
#endif
		
	return TRUE;
}


UINT QCToolBarCmdPage::TranslateID(
UINT	uID )
{
	return uID;
}
