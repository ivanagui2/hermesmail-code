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

File revised by Jeff Prickett (kg4ygs@gmail.com) on July 9, 2018
    Removed references to the Stingray Toolkit and replaced them with references
    to the Hermes UI Toolkit.

*/    


// QCCustomizeGeneral.cpp: implementation of the QCCustomizeGeneral class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "eudora.h"
#include "QCCustomizeGeneralPage.h"


#include "DebugNewHelpers.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(QCCustomizeGeneralPage, QCToolBarCmdPage)

QCCustomizeGeneralPage::QCCustomizeGeneralPage() : 
	QCToolBarCmdPage(	IDD_GENERAL_TOOLBAR_CUSTOMIZATION, IDS_GENERAL_TITLE )
{
	//{{AFX_DATA_INIT(QCCustomizeGeneralPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

QCCustomizeGeneralPage::~QCCustomizeGeneralPage()
{

}


BEGIN_MESSAGE_MAP(QCCustomizeGeneralPage, QCToolBarCmdPage)
	//{{AFX_MSG_MAP(QCCustomizeGeneralPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void QCCustomizeGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	QCToolBarCmdPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(QCCustomizeGeneralPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// QCCustomizeGeneralPage message handlers

BOOL QCCustomizeGeneralPage::OnInitDialog() 
{
	// Must have at least 1 btn group defined.
	ASSERT(m_btnGroups.GetSize() > 0);
	
	QCToolBarCmdPage::OnInitDialog();
	CListBox* pList = (CListBox*) GetDlgItem(IDC_TOOLBAR_LIST);
	
	ASSERT( pList );

	for(int i=0; i<m_btnGroups.GetSize(); i++)
	{
		// Create and initialise the toolbar
		HRMBtnGroup* pGroup = (HRMBtnGroup*) m_btnGroups[i];
		// Add the name of this toolbar to the listbox
		pList->AddString(pGroup->lpszTitle);
	}

	pList->SetCurSel(0);

	return TRUE;
}


