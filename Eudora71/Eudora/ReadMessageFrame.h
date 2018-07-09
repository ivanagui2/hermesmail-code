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

// ReadMessageFrame.h: interface for the CReadMessageFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_READMESSAGEFRAME_H__7EA9D342_E0EE_11D0_97ED_00805FD2F268__INCLUDED_)
#define AFX_READMESSAGEFRAME_H__7EA9D342_E0EE_11D0_97ED_00805FD2F268__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "msgframe.h"
#include "QCOleDropSource.h"
#include "QCProtocol.h"

class HRMToolBarManager;
class QCChildToolBar;
class CReadMessageToolBar;

class CReadMessageFrame : public CMessageFrame, public QCProtocol
{
    COleDataSource			m_OleDataSource;
	QCChildToolBar*			m_pFormattingToolBar;
	CReadMessageToolBar*	m_pToolBar;
	HRMToolBarManager*		m_pToolBarManager;
	
	DECLARE_DYNCREATE(CReadMessageFrame)

protected:

	//{{AFX_MSG(CReadMessageFrame)
	afx_msg void OnUpdateEditFindFindText(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditFindFindTextAgain(CCmdUI* pCmdUI);
	//}}AFX_MSG

	afx_msg LONG OnFindReplace(WPARAM wParam, LPARAM lParam);

	afx_msg void OnDeleteFromServer();
	afx_msg void OnDownloadFromServer();
	afx_msg void OnBlahBlahBlah();
	afx_msg void OnEditMode();
	afx_msg void OnChangeSubject();
	afx_msg void OnPriorityChange();
	afx_msg void OnDelete();
	afx_msg long OnSetPriority(WPARAM wParam, LPARAM lParam);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint ptScreen);
	afx_msg long OnButtonSetCheck(WPARAM wParam, LPARAM lParam);

	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual BOOL OnCmdMsg(
	UINT				nID, 
	int					nCode, 
	void*				pExtra, 
	AFX_CMDHANDLERINFO*	pHandlerInfo );

	afx_msg BOOL OnDynamicCommand(
	UINT uID );
	
	afx_msg void OnUpdateDynamicCommand(
	CCmdUI* pCmdUI);
	
	afx_msg void OnUpdateAlwaysEnabled(
	CCmdUI*	pCmdUI);

#ifdef IMAP4
	afx_msg void OnUpdateUndelete(CCmdUI* pCmdUI);
	afx_msg void UnDelete();
#endif // IMAP4

public:

	DECLARE_MESSAGE_MAP()

	CReadMessageFrame();
	virtual ~CReadMessageFrame();

	virtual BOOL OnCreateClient( 
	LPCREATESTRUCT	lpcs,
	CCreateContext* pContext);

	QCChildToolBar* GetFormatToolbar();
	QCChildToolBar* GetToolbar();

public:
	BOOL UseFixedWidthFont();
	
	BOOL GetCheck( UINT nCommandID );
	BOOL GetFormattingCheck( UINT nCommandID );

	CComboBox* GetFontCombo();

	BOOL QuietPrint();

	void EnableTooltips( BOOL bEnable );

	BOOL GetSubject(
	CString& szText );

	void ZoomWindow();

	void KillOriginalView();
};

#endif // !defined(AFX_READMESSAGEFRAME_H__7EA9D342_E0EE_11D0_97ED_00805FD2F268__INCLUDED_)
