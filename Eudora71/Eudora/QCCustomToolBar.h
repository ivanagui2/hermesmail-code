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

// QCCustomToolBar.h: interface for the QCCustomToolBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QCCUSTOMTOOLBAR_H__6AF0C4E2_F166_11D0_9801_00805FD2F268__INCLUDED_)
#define AFX_QCCUSTOMTOOLBAR_H__6AF0C4E2_F166_11D0_9801_00805FD2F268__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "QICommandClient.h"
#include "SafetyPal.h"

class QCCustomToolBar : public HRMCustomToolBar, public QICommandClient 
{
public:
	QCCustomToolBar();
	virtual ~QCCustomToolBar();

	DECLARE_DYNCREATE( QCCustomToolBar )

	// Returns TRUE if this toolbar accepts dropped buttons.
	virtual BOOL  AcceptDrop() const;

	// Handles a button drop
	virtual void DropButton(CRect& dropRect, HRMStdBtn* pDragBtn, BOOL bAdd);

	virtual BOOL RemoveButton(int nIndex, BOOL bNoUpdate = FALSE, INT* pIndex = NULL );
	virtual BOOL DestroyWindow();

	virtual void GetBarInfoEx(HRMControlBarInfo* pInfo);

	void SetSource( UINT iPosition, UINT uSrcCmdID )
	{
		m_btns[ iPosition ]->m_ulData = uSrcCmdID;
	}

	void SetButtonData(ULONG ulData, int nIndex = 0)
	{
		m_btns[nIndex]->m_ulData = ulData;
	}

	void SaveCustomInfo(
	LPCSTR	szSectionName );

	void LoadCustomInfo(
	LPCSTR	szSection );

	void LoadInYourFacePlugins();

	BOOL SpecialToolTipCaseHandled(WORD wCommandID, CString& strToolTip);

	virtual void Notify( 
	QCCommandObject*	pObject,
	COMMAND_ACTION_TYPE	theAction,
	void*				pData = NULL );

	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );

	virtual BOOL ConvertOldStuff();

	BOOL CreateEx(DWORD dwExStyle, CWnd* pParentWnd, DWORD dwStyle = 
				WS_VISIBLE | WS_CHILD | CBRS_TOP, UINT nID = AFX_IDW_TOOLBAR, 
				LPCTSTR lpszTitle = NULL);

	void ResetImageOffsets();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

protected:
	CSafetyPalette m_Palette;
protected:
	//{{AFX_MSG(QCCustomToolBar)
	afx_msg LRESULT OnMyQueryNewPalette(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnSysColorChange();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual HRMStdBtn* CreateSeparator(HRMCustomToolBar* pToolBar = NULL);
};

#endif // !defined(AFX_QCCUSTOMTOOLBAR_H__6AF0C4E2_F166_11D0_9801_00805FD2F268__INCLUDED_)
