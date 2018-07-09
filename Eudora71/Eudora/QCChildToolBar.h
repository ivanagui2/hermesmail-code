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

File revised by Jeff Prickett (kg4ygs@gmail.com) on July 9, 2018
    Removed references to the Stingray Toolkit and replaced them with references
    to the Hermes UI Toolkit.

*/    

// QCChildToolBar.h: interface for the QCChildToolBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QCCHILDTOOLBAR_H__577A4910_1351_11D1_8415_00805FD2F268__INCLUDED_)
#define AFX_QCCHILDTOOLBAR_H__577A4910_1351_11D1_8415_00805FD2F268__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "tbarcust.h"
#include "SafetyPal.h"

class QCChildToolBar : public HRMCustomToolBar  
{
protected:

    //{{AFX_MSG(QCChildToolBar)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint pt);
	afx_msg LRESULT OnMyQueryNewPalette(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnSysColorChange();
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

public:
	
	DECLARE_DYNCREATE(QCChildToolBar)

	virtual BOOL InitButtons();
	
	virtual BOOL AcceptDrop() const;
	
	QCChildToolBar();
	
	virtual ~QCChildToolBar();
	
	void EnableDocking(DWORD dwDockStyle);
	
	void* GetButton(
	INT iIndex );
	
	INT Height() { return m_szTbarDockHorz.cy; }

	BOOL GetDragMode() const;
	BOOL DragButton(int nIndex, CPoint point);

	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );
	void AddTranslatorButtons(int &startPos);

	BOOL CreateEx(DWORD dwExStyle, CWnd* pParentWnd, DWORD dwStyle, UINT nID, LPCTSTR lpszTitle);
protected:
	CSafetyPalette m_Palette;
	DWORD			m_nLastIdleProcessing;
};

#endif // !defined(AFX_QCCHILDTOOLBAR_H__577A4910_1351_11D1_8415_00805FD2F268__INCLUDED_)
