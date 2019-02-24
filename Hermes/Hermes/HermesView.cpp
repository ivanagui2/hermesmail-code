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

// HermesView.cpp : implementation of the CHermesView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Hermes.h"
#endif

#include "HermesDoc.h"
#include "CntrItem.h"
#include "resource.h"
#include "HermesView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHermesView

IMPLEMENT_DYNCREATE(CHermesView, CRichEditView)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CHermesView Message Map
BEGIN_MESSAGE_MAP(CHermesView, CRichEditView)
	ON_WM_DESTROY()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CHermesView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// static intialization of current view pointer
CView* CHermesView::m_pCurrentView = NULL;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CHermesView construction/destruction
CHermesView::CHermesView() noexcept
{
	EnableActiveAccessibility();
	// TODO: add construction code here

	GUID g;
	::CoCreateGuid(&g);

	LPCWSTR lpsz = ::GUIDAsStringW(g);
	m_GUID = lpsz;
	TRACE(_T("DOC ID: %s\n"), lpsz);
	ASSERT(m_GUID.vt == VT_BSTR);


}

CHermesView::~CHermesView()
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHermesView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRichEditView::PreCreateWindow(cs);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHermesView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();


	// Set the printing margins (720 twips = 1/2 inch)
	SetMargins(CRect(720, 720, 720, 720));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHermesView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if (m_pCurrentView == NULL)
		m_pCurrentView = pActivateView;
	else if (pActivateView == m_pCurrentView)
		return;
	else
		m_pCurrentView = pActivateView;

	CMainFrame *pFrame = reinterpret_cast<CMainFrame *>(::AfxGetMainWnd());
	ASSERT(pFrame);
	CMailDetailView *pMailDetailView = pFrame->GetMailDetailView();

	CHermesView *pCurrentView = reinterpret_cast<CHermesView *>(pActivateView);
	ASSERT(pCurrentView);
	ASSERT(pCurrentView->IsKindOf(RUNTIME_CLASS(CHermesView)));


	CHermesView *pPreviousView = reinterpret_cast<CHermesView *>(pDeactiveView);
	ASSERT(pPreviousView);
	ASSERT(pPreviousView->IsKindOf(RUNTIME_CLASS(CHermesView)));

	pMailDetailView->BespokeUpdateData(pPreviousView->GetDocument(), FALSE);
	pMailDetailView->BespokeUpdateData(pCurrentView->GetDocument());
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	void CHermesView::DoDataExchange(CDataExchange* pDX)
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHermesView::DoDataExchange(CDataExchange* pDX)
{
	if (pDX->m_bSaveAndValidate)
	{
	}
	else // [SBROTHY]: Update controls from document data
	{
	}
}




// CHermesView printing


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHermesView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHermesView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHermesView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != nullptr && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == nullptr);
   }
   CRichEditView::OnDestroy();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHermesView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHermesView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CHermesView diagnostics

#ifdef _DEBUG
void CHermesView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CHermesView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHermesDoc* CHermesView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHermesDoc)));
	return (CHermesDoc*)m_pDocument;
}
#endif //_DEBUG


// CHermesView message handlers
