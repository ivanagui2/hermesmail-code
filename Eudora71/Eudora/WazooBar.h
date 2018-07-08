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
    Removed references to the Stringray Toolkit and replaced them with references
    to the Hermes UI Toolkit.

*/    

#ifndef __WAZOOBAR_H__
#define __WAZOOBAR_H__

// WazooBar.h : header file
//
// CWazooBar
// Specific implementation of a resizable SECControlBar.

#include "QC3DTabWnd.h"


///////////////////////////////////////////////////////////////////////
// CWazooBarOleDropTarget
//
// Private class that overrides CView-based implementation of 
// COleDropTarget to work with CWazooBar objects.
///////////////////////////////////////////////////////////////////////
class CWazooBarOleDropTarget : public COleDropTarget
{
	friend class CWazooBar;		// private class

private:
	CWazooBarOleDropTarget()
	{
	}
	virtual ~CWazooBarOleDropTarget()
	{
	}

public:
	// Overridables
	virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
								   DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
								  DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
						DROPEFFECT dropEffect, CPoint point);
	virtual void OnDragLeave(CWnd* pWnd);
	virtual DROPEFFECT OnDropEx(CWnd* pWnd, COleDataObject* pDataObject,
								DROPEFFECT dropDefault, DROPEFFECT dropList,
								CPoint point);
//	virtual DROPEFFECT OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point);

private:
	CWazooBarOleDropTarget(const CWazooBarOleDropTarget&);	// not implemented
	void operator=(const CWazooBarOleDropTarget&);		// not implemented
};


/////////////////////////////////////////////////////////////////////////////
// CWazooBar control bar

class CWazooWnd;
class CWazooBarMgr;

///////////////////////////////////////////////////////////////////////
// CWazooBar
//
///////////////////////////////////////////////////////////////////////
class CWazooBar : public CControlBar
{
	friend class CWazooBarOleDropTarget;				// mi casa es su casa
	friend class CWazooBarMgr;

	DECLARE_DYNCREATE(CWazooBar)

// Construction
protected:
	CWazooBar() {}		// not used!
public:
	CWazooBar(CWazooBarMgr* pManager);
	virtual ~CWazooBar();

	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, UINT nID,
		DWORD dwStyle, DWORD dwExStyle, const RECT& rect, CWnd* pParentWnd = NULL,
		CCreateContext* pContext = NULL);
	virtual BOOL Create(CWnd* pParentWnd, LPCTSTR lpszWindowName, DWORD dwStyle,
		DWORD dwExStyle, UINT nID, CCreateContext* pContext = NULL);

	// Bit mask flags for 'nFlags' value saved to INI file
	enum
	{
		FLAGS_VISIBLE	= 0x1,	// Is visible MDI child window
		FLAGS_ACTIVE	= 0x2	// Is active MDI child window
	};

// Attributes
public:

// Operations
public:

	BOOL AddWazooWindow(CWazooWnd* pWazooWnd, const CPoint& ptScreen);
	BOOL RemoveWazooWindow(CWazooWnd* pWazooWnd);
	BOOL ShuffleWazooWindow(CWazooWnd* pWazooWnd, const CPoint& ptScreen);
	CWazooWnd* GetActiveWazooWindow();
	void SetActiveWazooWindow();
	BOOL ContainsWazooWindow(CRuntimeClass* pRuntimeClass);
	BOOL UnlinkFromManager();
	int GetWazooCount();

	CWnd* QCTabHitTest(const CPoint& ptScreen, BOOL& bSelected, int& nIndex);

	virtual BOOL SetDefaultWazooConfig(int nBar);
	virtual BOOL LoadWazooConfigFromIni();
	virtual BOOL SaveWazooConfigToIni();

	void RefreshWindowTitle();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Overrides
public:
	virtual BOOL ActivateWazooWindow(CRuntimeClass* pRuntimeClass);
	virtual void OnBarFloat();
	virtual void OnBarMDIFloat();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWazooBar)
protected:
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:

	// required implementation of a pure virtual
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	
	// override base class implementation
	virtual void OnExtendContextMenu(CMenu* pMenu);

	BOOL CreateWazooWindow(CRuntimeClass* pRuntimeClass);

	QC3DTabWnd m_wndTab;
	CWazooBarMgr* m_pManager;
	int m_LastActiveTabOnShutdown;

	enum
	{
		AUTOACTIVATE_TIMER_EVENT_ID = 1001
	};

	//{{AFX_MSG(CWazooBar)
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCmdHRMMDIFloat();
	afx_msg void OnTabsOnTop();
	afx_msg void OnTabsOnBottom();
	afx_msg void OnTabsOnLeft();
	afx_msg void OnTabsOnRight();
	afx_msg void OnTabsShowSingleTab();
	afx_msg LRESULT OnTabSelect(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL GetNextParam(CString& strParams, CString& strParam) const;

	//
	// Registered object for OLE Drag and Drop callbacks.
	//
	CWazooBarOleDropTarget m_dropTarget;

	UINT m_idAutoActivateTimer;		// for tab drag and drop auto-activation
};

#endif //__WAZOOBAR_H__
