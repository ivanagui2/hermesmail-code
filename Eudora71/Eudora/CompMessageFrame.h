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

File revised by Jeff Prickett (kg4ygs@gmail.com) on July 17, 2018
    Removed references to the Stingray Toolkit and replaced them with references
    to the Hermes UI Toolkit.

*/    

// CompMessageFrame.h: interface for the CCompMessageFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPMESSAGEFRAME_H__7EA9D343_E0EE_11D0_97ED_00805FD2F268__INCLUDED_)
#define AFX_COMPMESSAGEFRAME_H__7EA9D343_E0EE_11D0_97ED_00805FD2F268__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "msgframe.h"
#include "QICommandClient.h"
#include "QCProtocol.h"

#define WM_USER_UPDATE_IMMEDIATE_SEND WM_USER+222

class CHeaderView;
class PgCompMsgView;
class QCChildToolBar;
class HRMToolBarManager;
class QCToolBarManagerWithBM;

class CCompMessageFrame : public CMessageFrame, public QICommandClient, public QCProtocol
{
	QCChildToolBar*		m_pFormattingToolBar;
	QCChildToolBar*		m_pToolBar;
	QCToolBarManagerWithBM*	m_pToolBarManager;
	CMenu				m_theTranslatorMenu;
	CStringArray		m_thePropertyList;
	int					m_nHeaderHeight;
	bool				m_bBPWarning;

	void SelectTranslators(const char *hdr);

	DECLARE_DYNCREATE(CCompMessageFrame)

// Generated message map functions
protected:
	//{{AFX_MSG(CCompMessageFrame)
	afx_msg void OnDestroy();
	afx_msg void OnDropFiles( HDROP );
	afx_msg void OnUpdateFileSaveAsStationery(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCheckSpelling(CCmdUI* pCmdUI);
	afx_msg void OnCheckSpelling();
	afx_msg void OnUpdateInsertSystemConfiguration(CCmdUI* pCmdUI);
	afx_msg void OnInsertSystemConfiguration();
	afx_msg void OnUpdateEditFindFindText(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditFindFindTextAgain(CCmdUI* pCmdUI);
	//}}AFX_MSG

	afx_msg LONG OnFindReplace(WPARAM wParam, LPARAM lParam);

	afx_msg void OkIfEditable(CCmdUI* pCmdUI);
	afx_msg void OnMoodMailUI(CCmdUI* pCmdUI);

	afx_msg void OnPriorityChange();
	afx_msg void OnSignatureChange();
	afx_msg void OnEncodingChange();
	afx_msg void OnQuotedPrintable();
	afx_msg void OnTextAsDocument();
	afx_msg void OnWordWrap();
	afx_msg void OnTabsInBody();
	afx_msg void OnKeepCopies();
	afx_msg void OnReturnReceipt();
	afx_msg long OnAddAttachment( WPARAM	wParam, LPARAM	lParam );
	afx_msg long OnSetSignature( WPARAM	wParam, LPARAM	lParam );
	afx_msg long OnSetPriority( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnUserUpdateImmediateSend( WPARAM wParam, LPARAM lParam );
	afx_msg void OnToggleHeader();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:

	DECLARE_MESSAGE_MAP()

// Quick! What is the access specifier of the following methods?
// Here's a hint: it's not public.

	CCompMessageFrame();
	virtual ~CCompMessageFrame();

	virtual BOOL OnCreateClient( 
	LPCREATESTRUCT	lpcs,
	CCreateContext* pContext);

public:
	CHeaderView* GetHeaderView();
	PgCompMsgView *			GetPgCompMsgView() const;
	BOOL AddAttachment( const char* Filename );

	// oooh, a public afx_msg!!
	afx_msg void OnAttachFile();
	void EnableTooltips( BOOL bEnable );

	void SetCheck( UINT nCommandID, BOOL bChecked );
	BOOL GetCheck( UINT nCommandID );

	void SetBPWarning(bool bBPWarning) { m_bBPWarning = bBPWarning; }
	bool IsBPWarning() { return m_bBPWarning; }

	void Notify( 
	QCCommandObject*	pCommand,
	COMMAND_ACTION_TYPE	theAction,
	void*				pData); 

	BOOL OnDynamicCommand(
	UINT uID );

	void OnUpdateDynamicCommand(
	CCmdUI* pCmdUI);
	
	CString GetSelectedTranslators();
	QCChildToolBar* GetFormatToolbar(); 
	QCChildToolBar* GetToolbar(); 

	//Printing releated
	BOOL QuietPrint();

};

#endif // !defined(AFX_COMPMESSAGEFRAME_H__7EA9D343_E0EE_11D0_97ED_00805FD2F268__INCLUDED_)
