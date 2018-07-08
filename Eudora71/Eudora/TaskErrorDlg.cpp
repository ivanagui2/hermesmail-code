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

*/

// TaskErrorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "winuser.h" // system colors
#include "TaskErrorMT.h"

#include "TaskErrorDlg.h"

#include "DebugNewHelpers.h"


/////////////////////////////////////////////////////////////////////////////
// CTaskErrorDlg dialog


CTaskErrorDlg::CTaskErrorDlg(CTaskErrorMT *pErr, CWnd* pParent /*=NULL*/)
	: CDialog(CTaskErrorDlg::IDD, pParent), m_pError(pErr), m_bInitialized(false)
{
	//{{AFX_DATA_INIT(CTaskErrorDlg)
	//}}AFX_DATA_INIT
}


void CTaskErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaskErrorDlg)
	DDX_Control(pDX, IDC_TIME_STATIC, m_TimeStatic);
	DDX_Control(pDX, IDOK, m_OKBtn);
	DDX_Control(pDX, IDC_DETAILS_BTN, m_DetailsBtn);
	DDX_Control(pDX, IDC_TITLE_STATIC, m_TitleStatic);
	DDX_Control(pDX, IDC_HRMTXT_STATIC, m_HRMTxtStatic);
	DDX_Control(pDX, IDC_PERSONA_STATIC, m_PersonaStatic);
	DDX_Control(pDX, IDC_MAINTXT_STATIC, m_MainTxtStatic);
	DDX_Control(pDX, IDC_ERROR_RICHEDIT, m_ErrorRichEdit);
	DDX_Control(pDX, IDC_PERSONA_RICHEDIT, m_PersonaRichEdit);
	DDX_Control(pDX, IDC_TITLE_RICHEDIT, m_TitleRichEdit);
	DDX_Control(pDX, IDC_STATUS_RICHEDIT, m_StatusRichEdit);
	DDX_Control(pDX, IDC_INFO_RICHEDIT, m_InfoRichEdit);
	DDX_Control(pDX, IDC_TIME_RICHEDIT, m_TimeRichEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaskErrorDlg, CDialog)
	//{{AFX_MSG_MAP(CTaskErrorDlg)
	ON_BN_CLICKED(IDC_DETAILS_BTN, OnDetailsBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskErrorDlg message handlers

BOOL CTaskErrorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (!m_bInitialized)
	{
		m_ErrorRichEdit.SetTargetDevice(NULL, 0); // Word-wrap only the main error text

		COLORREF bkClr = GetSysColor(COLOR_BTNFACE);

		// Set the background color for the rich-edit controls
		m_ErrorRichEdit.SetBackgroundColor(FALSE, bkClr);
		m_PersonaRichEdit.SetBackgroundColor(FALSE, bkClr);
		m_TitleRichEdit.SetBackgroundColor(FALSE, bkClr);
		m_StatusRichEdit.SetBackgroundColor(FALSE, bkClr);
		m_InfoRichEdit.SetBackgroundColor(FALSE, bkClr);
		m_TimeRichEdit.SetBackgroundColor(FALSE, bkClr);

		// Set the text for each control
		m_ErrorRichEdit.SetWindowText(m_pError->GetErrText());
		m_PersonaRichEdit.SetWindowText(m_pError->GetPersona());
		m_TitleRichEdit.SetWindowText(m_pError->GetTitle());
		m_StatusRichEdit.SetWindowText(m_pError->GetMainText());
		m_InfoRichEdit.SetWindowText(m_pError->GetSecondaryText());

		CTime ErrTime = m_pError->GetTimeStamp();
		CString sTime = ErrTime.Format("%#I:%M:%S %p on %B %#d");

		m_TimeRichEdit.SetWindowText(sTime);

		// Hide all 'details' controls
		m_TitleStatic.ShowWindow(SW_HIDE);
		m_SecTxtStatic.ShowWindow(SW_HIDE);
		m_PersonaStatic.ShowWindow(SW_HIDE);
		m_MainTxtStatic.ShowWindow(SW_HIDE);
		m_TimeStatic.ShowWindow(SW_HIDE);

		m_PersonaRichEdit.ShowWindow(SW_HIDE);
		m_TitleRichEdit.ShowWindow(SW_HIDE);
		m_StatusRichEdit.ShowWindow(SW_HIDE);
		m_InfoRichEdit.ShowWindow(SW_HIDE);
		m_TimeRichEdit.ShowWindow(SW_HIDE);

		// Save the size of the full dialog
		GetWindowRect((LPRECT)m_InitialDlgRct);

		// Resize the dialog, based on the OK button (move bottom up)
		CRect wndRct, okRct;
		GetWindowRect(wndRct);
		m_OKBtn.GetWindowRect(okRct);
		wndRct.bottom = okRct.bottom + 10;
		MoveWindow(wndRct);

		// Prevent multiple inits
		m_bInitialized = true;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaskErrorDlg::OnDetailsBtn() 
{	
	// Hide the 'details' button
	m_DetailsBtn.ShowWindow(SW_HIDE);

	// Show all 'details' controls
	m_TitleStatic.ShowWindow(SW_SHOW);
	m_SecTxtStatic.ShowWindow(SW_SHOW);
	m_PersonaStatic.ShowWindow(SW_SHOW);
	m_MainTxtStatic.ShowWindow(SW_SHOW);
	m_TimeStatic.ShowWindow(SW_SHOW);

	m_PersonaRichEdit.ShowWindow(SW_SHOW);
	m_TitleRichEdit.ShowWindow(SW_SHOW);
	m_StatusRichEdit.ShowWindow(SW_SHOW);
	m_InfoRichEdit.ShowWindow(SW_SHOW);
	m_TimeRichEdit.ShowWindow(SW_SHOW);

	CRect wndRct, okRct;
	
	GetWindowRect(wndRct);
	wndRct.bottom = wndRct.top + m_InitialDlgRct.Height();
	
	m_OKBtn.GetWindowRect(okRct);
	int offset = wndRct.bottom - okRct.bottom - 10;
	okRct.OffsetRect(0, offset);
	
	m_OKBtn.GetParent()->ScreenToClient(okRct);

	MoveWindow(wndRct); // Screen-relative
	m_OKBtn.MoveWindow(okRct); // Parent client-relative
}
