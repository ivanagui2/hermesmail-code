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

File revised on by Jeff Prickett (kg4ygs@gmail.com) on July 8, 2018
    Changed the code to reference the Hermes UI Toolkit as opposed to the
    Stringray Toolkit.
*/    


#if !defined(AFX_WAZOOWND_H__AA6D0E51_A6C9_11D0_97B4_00805FD2F268__INCLUDED_)
#define AFX_WAZOOWND_H__AA6D0E51_A6C9_11D0_97B4_00805FD2F268__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// WazooWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWazooWnd window

extern UINT umsgActivateWazoo;		// registered Windows message


class CWazooWnd : public CWnd
{
	DECLARE_DYNAMIC(CWazooWnd)

// Construction
public:
	CWazooWnd(UINT nID);

// Attributes
public:

	UINT GetID() const
		{ return m_nID; }

	HICON GetTabIconHandle() const
		{ return m_hIcon; }

	CString GetTabLabel() const
		{ return m_strTabLabel; }

	BOOL HasActiveFocus() const;

	static void SaveInitialChildFocus(const CWnd* pChildWithFocus);

	virtual void OnActivateWazoo();
	virtual void OnDeactivateWazoo();

// Operations
public:

	void	DoContextMenu(
	CWnd*	pWnd, 
	CPoint	point,
	HMENU	hMenu );

	CControlBar* GetParentControlBar() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWazooWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWazooWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWazooWnd)
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	//}}AFX_MSG
	afx_msg void OnInitMenuPopup(CMenu* pMenu, UINT, BOOL bSysMenu);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnCmdHRMAllowDocking();
	afx_msg void OnCmdHRMHide();
	afx_msg void OnCmdHRMMDIFloat();
	afx_msg void OnCmdUpdateHRMAllowDocking(CCmdUI* pCmdUI);
	afx_msg LRESULT OnCommandHelp(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	BOOL SaveChildWithFocus();

	UINT	m_nID;				// ID for Wazoo window
	CString m_strTabLabel;		// tab label string for Wazoo tab
	HICON	m_hIcon;			// HICON for Wazoo tab
	HWND	m_hwndFocus;		// last child window that had keyboard focus
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAZOOWND_H__AA6D0E51_A6C9_11D0_97B4_00805FD2F268__INCLUDED_)
