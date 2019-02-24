//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Cop't 1991-2005 Qualcomm, 2019 HERMES, use of this software is governed by these terms
// 
//	Copyright 2019 HERMES Team
// 
//	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met :
//
//	1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
//	
//	2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
//
//	3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
//
//	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
//	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;  OR BUSINESS INTERRUPTION) 
//	HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
//	EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// HermesView.h : interface of the CHermesView class
//

#pragma once

class CHermesCntrItem;

class CHermesView : public CRichEditView
{
protected: // create from serialization only
	CHermesView() noexcept;
	DECLARE_DYNCREATE(CHermesView)

// Attributes
public:
	// [SBROTHY]: We want a globlly unique identifier fior ID to the MailDetailView. (The dockable window of which there, Insh Alllah, will be only one.)
	// [SBROTHY]: It´s kept in a CComvariant for both consistency and for easy saving and loading with the rest.
	CComVariant	m_GUID;
	CComVariant *GetID() { return &m_GUID; }

	CHermesDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	// [SBROTHY]: This is kinda of a dirty hack. Used in another function: void CHermesView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
	// [SBROTHY]: This is more of a problem than a solution. Exactly because the CDocakblePanes do not play well with MFC message routing. Appearently CDockablePane 
	// [SBROTHY]: is a pane (sic! pun intended) in the proverbial a... CDocakablePane and some of the CMFCToolBar classes and the-like is a little too cutting edge it may seem.
	static CView *m_pCurrentView;


// Implementation
public:
	virtual ~CHermesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	static CView * GetCurrentView() { return m_pCurrentView; }

protected:

// Generated message map functions
protected:
	afx_msg void OnDestroy();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in HermesView.cpp
inline CHermesDoc* CHermesView::GetDocument() const
   { return reinterpret_cast<CHermesDoc*>(m_pDocument); }
#endif

