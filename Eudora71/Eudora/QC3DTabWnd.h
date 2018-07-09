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

Files revised by Jeff Prickett (kg4ygs@gmail.com) on July 9, 2018
    Removed references to the Stingray Toolkit and replaced them with references 
    to the Hermes UI Toolkit.

*/

#ifndef __QC3DTABWND_H__
#define __QC3DTABWND_H__

// QC3DTabWnd.h : header file
//
// QC3DTabWnd
// QC customizations of the HRM3DTabWnd.


///////////////////////////////////////////////////////////////////////
// QC3DTabControl
//
///////////////////////////////////////////////////////////////////////
class QC3DTabControl : public HRM3DTabControl
{
	friend class QC3DTabWnd;
	DECLARE_DYNCREATE(QC3DTabControl)

// Construction
public:
	QC3DTabControl();
	virtual ~QC3DTabControl();

	HICON QCGetIcon(int nTabIndex) const;

	// Override virtuals from base class...
	virtual void ActivateTab(int nTab);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(QC3DTabControl)
protected:
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:

	// required implementation of a pure virtual
	//{{AFX_MSG(QC3DTabControl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:    
    // Needed to make this control an OLE data SOURCE (see OnMouseMove)
    COleDataSource m_OleDataSource;
	CPoint m_SavedMouseDownPoint;
	int m_nSavedTabIndex;
	enum
	{
		MOUSE_IDLE,
		MOUSE_DOWN_ON_TAB,
		MOUSE_IS_MOVING,
		MOUSE_IS_DRAGGING
	} m_MouseState;
};


///////////////////////////////////////////////////////////////////////
// QC3DTabWnd
//
///////////////////////////////////////////////////////////////////////
class QC3DTabWnd : public HRM3DTabWnd
{
	DECLARE_DYNCREATE(QC3DTabWnd)

// Construction
public:
	QC3DTabWnd();
	virtual ~QC3DTabWnd();

// Attributes
public:

// Operations
public:

	HICON QCGetIcon(int nTabIndex) const;
	BOOL QCTabHit(int nTabIndex, const CPoint& ptScreen);

	void SetTabLocation(DWORD dwTabLocation);
	DWORD GetTabLocation() const;
	void ShowTabs(BOOL bShowTabs);
	BOOL TabsAreVisible() const;
	void SetDraw3DBorder(BOOL bNew) { m_Draw3DBorder = bNew; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(QC3DTabWnd)
protected:
	//}}AFX_VIRTUAL

	virtual BOOL CreateTabCtrl(DWORD dwStyle, UINT nID);
    virtual void RecalcLayout();

// Implementation
public:

	// Generated message map functions
protected:
	BOOL m_bShowTabs;
	BOOL m_Draw3DBorder;

	// required implementation of a pure virtual
	//{{AFX_MSG(QC3DTabWnd)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__QC3DTABWND_H__
