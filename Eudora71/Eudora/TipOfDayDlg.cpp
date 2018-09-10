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

NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY�S PATENT RIGHTS ARE GRANTED BY THIS 
LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
�AS IS� AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

File revised by Jeff Prickett (kg4ygs@gmail.com) on July 8, 2018
    Removed references to the Stingray Toolokit and replaced them with references
    to the Hermes UI Toolkit.

*/        

// QCTipOfDayDlg.cpp : implementation file


#include "stdafx.h"

#include "resource.h"
#include "rs.h"
#include "TipOfDayDlg.h"


#include "DebugNewHelpers.h"



BEGIN_MESSAGE_MAP(QCTipOfDayDlg, HRMTipOfDay)
	//{{AFX_MSG_MAP(QCTipOfDayDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// QCTipOfDayDlg dialog

QCTipOfDayDlg::QCTipOfDayDlg
(
	const TCHAR * lpszTipFileName,
	int nTip /*= 1*/,
	BOOL bShowAtStart /*= 1*/,
	CWnd* pParentWnd /*= NULL*/
) :
	HRMTipOfDay(lpszTipFileName, (const TCHAR *) CRString(IDS_DID_YOU_KNOW), nTip,
	            bShowAtStart, pParentWnd)
{
	//{{AFX_DATA_INIT(QCTipOfDayDlg)
	//}}AFX_DATA_INIT
}


QCTipOfDayDlg::~QCTipOfDayDlg()
{
	// nothing to do
}


BOOL QCTipOfDayDlg::OnInitDialog()
{
	HRMTipOfDay::OnInitDialog();

	//
	// Hide the Help button.
	//
	CWnd* pButton = GetDlgItem(IDC_TOD_HELP_BUTTON);
	if (pButton)
		pButton->ShowWindow(SW_HIDE);
	else
	{
		ASSERT(0);
	}

	//
	// Change the text on the OK button to "Close"
	//
	pButton = GetDlgItem(IDC_TOD_OK_BUTTON);
	if (pButton)
		pButton->SetWindowText(CRString(IDS_CLOSE));
	else
	{
		ASSERT(0);
	}

	SetTipIcon(IDR_COMPMESS);

	if (m_fontLeadIn.CreateFont(20,
								8,
								0,
								0,
								FW_BOLD,
								0,
								0,
								0,
								DEFAULT_CHARSET,
								OUT_DEFAULT_PRECIS,
								CLIP_DEFAULT_PRECIS,
								DEFAULT_QUALITY,
								TMPF_TRUETYPE | FF_ROMAN, //DEFAULT_PITCH,
								_T("Times New Roman")))
	{
		SetLeadInFont(&m_fontLeadIn);
	}

	if (m_fontTip.CreatePointFont(90, "Arial"))
		SetTipFont(&m_fontTip);

	CenterWindow();

    return TRUE;  // return TRUE  unless you set the focus to a control
}

