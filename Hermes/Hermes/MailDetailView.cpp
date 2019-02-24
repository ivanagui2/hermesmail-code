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
//*
//	*	CMailDetailView class. Implementation.
//	*
//	*
//	*	PURPOSE:
//	*
//	*		This the one and only window handling the data neccessary for every mail document (From, To, Subject, Bcc (vector or similar). Cc (vector or similar), and attachments.
//	*		The basic idea is that each view and each document will have a unique ID, identifying them to this window.
//	*
//	*		Serialization happens back and forth directly into the CHermesDoc. No need for saying it complicates matters.
//	*
//	*
//	*	[SBROTHY@GMAIL.COM]
//	*
//


#include "stdafx.h"
#include "MainFrm.h"
#include "Resource.h"
#include "Hermes.h"
#include "HermesDoc.h"
#include "HermesView.h"


class CMailDetailViewMenuButton : public CMFCToolBarMenuButton
{
	DECLARE_SERIAL(CMailDetailViewMenuButton)

public:
	CMailDetailViewMenuButton(HMENU hMenu = nullptr) noexcept : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CMailDetailViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMailDetailView::CMailDetailView() noexcept
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

CMailDetailView::~CMailDetailView()
{
}

BEGIN_MESSAGE_MAP(CMailDetailView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
END_MESSAGE_MAP()

/*
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMailDetailView::OnButtonSave)
*/

void CMailDetailView::OnButtonSave()
{
	TRACE(_T("void CMailDetailView::RemoveCaptionButtons() >>\n"));

	TRACE(_T("void CMailDetailView::RemoveCaptionButtons() <<\n"));
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CClassView message handlers


int CMailDetailView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Dlg.Create(IDD_FORMVIEW, this);
	m_Dlg.ShowWindow(SW_SHOW);

	// Load images:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CMailDetailViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CMailDetailViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CMailDetailViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != nullptr)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	return 0;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMailDetailView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMailDetailView::OnContextMenu(CWnd* pWnd, CPoint point)
{
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMailDetailView::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_Dlg.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMailDetailView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: Not sure if necessary, but better safe than sorry
void CMailDetailView::Reset()
{
/*
	if(!IsWindowVisible())
		return;
	if(!IsVisible() || !IsPaneVisible())
		return;
*/
	CEdit *pEdit = nullptr;

	pEdit = reinterpret_cast<CEdit *>(m_Dlg.GetDlgItem(IDC_EDIT_TO));
	ASSERT(pEdit);
	pEdit->Clear();

	pEdit = reinterpret_cast<CEdit *>(m_Dlg.GetDlgItem(IDC_EDIT_FROM));
	ASSERT(pEdit);
	pEdit->Clear();

	pEdit = reinterpret_cast<CEdit *>(m_Dlg.GetDlgItem(IDC_EDIT_SUBJECT));
	ASSERT(pEdit);
	pEdit->Clear();

	CComboBox *pComboBox = reinterpret_cast<CComboBox *>(m_Dlg.GetDlgItem(IDC_COMBO_GUIDS));
	ASSERT(pComboBox);
	pComboBox->ResetContent();

	// [SBROTHY]: TODO: Fix missing fields.
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	void CMailDetailView::DoDataExchange(CDataExchange* pDX)
//
//		TODO: Clean up this function. A lot of superflous heap allocation is going on.
//		TODO: Do what validation we can.
//		TODO: Fix missing fields.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMailDetailView::DoDataExchange(CDataExchange* pDX)
{
	CHermesDoc *pHermesDoc = nullptr;

	CHermesView *pView = reinterpret_cast<CHermesView *>(CHermesView::GetCurrentView());
	if (!pView)
	{
		DocsPtrList list;

		CHermesApp * pHermesApp = dynamic_cast<CHermesApp *>(::AfxGetApp());
		ASSERT(pHermesApp);
		pHermesApp->GetAllDocuments(list);

		if (list.IsEmpty())
		{
			// [SBROTHY]: Nothing for us to do. Really? Really?
			return;
		}

		TRACE(_T("Doc list size: %d\n"), list.GetSize());

		POSITION pos = list.GetHeadPosition();
		while (pos != NULL)
		{
			CDocument *pDoc = list.GetNext(pos);
			pHermesDoc = reinterpret_cast<CHermesDoc *>(pDoc);
		}
	}
	else
	{
		pHermesDoc = pView->GetDocument();
	}

	ASSERT(pHermesDoc);
	pHermesDoc->AssertValid();

	CEdit *pEdit = nullptr;
	CComVariant v, *pv;
	CString s, *ps;

	if (pDX->m_bSaveAndValidate)
	{
		pEdit = reinterpret_cast<CEdit *>(m_Dlg.GetDlgItem(IDC_EDIT_TO));
		pEdit->GetWindowTextW(s);
		TRACE("IDC_EDIT_TO: [%s]", s);
		v = s;
		pHermesDoc->GetSetTo(&v);

		pEdit = reinterpret_cast<CEdit *>(m_Dlg.GetDlgItem(IDC_EDIT_FROM));
		pEdit->GetWindowTextW(s);
		TRACE("IDC_EDIT_FROM: [%s]", s);
		v = s;
		pHermesDoc->GetSetFrom(&v);

		pEdit = reinterpret_cast<CEdit *>(m_Dlg.GetDlgItem(IDC_EDIT_SUBJECT));
		pEdit->GetWindowTextW(s);
		TRACE("IDC_EDIT_SUBJECT: [%s]", s);
		v = s;
		pHermesDoc->GetSetSubject(&v);

		// [SBROTHY]: TODO: Fix missing fields.
	}
	else // [SBROTHY]: Update controls from document data
	{
		Reset();

		pv = pHermesDoc->GetID();
		ASSERT(pv);
		ASSERT(pv->vt == VT_BSTR);
		TRACE_VARIANTTYPE(pv);
		ps = new CString(pv->bstrVal, SysStringLen(pv->bstrVal));

		CString s;
		s.Format(_T("DOC\tID [%s]"), *ps);
		delete ps;

		TRACE(_T("SETTING DOC ID: %s"), s);

		CComboBox *pComboBox = reinterpret_cast<CComboBox *>(m_Dlg.GetDlgItem(IDC_COMBO_GUIDS));
		ASSERT(pComboBox);
		pComboBox->AddString(s);

		pv = pHermesDoc->GetSetTo();
		ASSERT(pv);
		TRACE_VARIANTTYPE(pv);
		ASSERT(pv->vt == VT_BSTR);

		ps = new CString(pv->bstrVal, SysStringLen(pv->bstrVal));
		pEdit = reinterpret_cast<CEdit *>(m_Dlg.GetDlgItem(IDC_EDIT_TO));
		pEdit->SetWindowTextW(*ps);
		delete ps;

		pv = pHermesDoc->GetSetFrom();
		ASSERT(pv);
		ASSERT(pv->vt == VT_BSTR);
		ps = new CString(pv->bstrVal, SysStringLen(pv->bstrVal));
		pEdit = reinterpret_cast<CEdit *>(m_Dlg.GetDlgItem(IDC_EDIT_FROM));
		pEdit->SetWindowTextW(*ps);
		delete ps;

		pv = pHermesDoc->GetSetSubject();
		ASSERT(pv);
		ASSERT(pv->vt == VT_BSTR);
		ps = new CString(pv->bstrVal, SysStringLen(pv->bstrVal));
		pEdit = reinterpret_cast<CEdit *>(m_Dlg.GetDlgItem(IDC_EDIT_SUBJECT));
		pEdit->SetWindowTextW(*ps);
		pHermesDoc->SetTitle(*ps);
		delete ps;

		// [SBROTHY]: TODO: Fix missing fields.
	}
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMailDetailView::BespokeUpdateData(CHermesDoc *pHermesDoc, BOOL bSaveAndValidate)
{
	TRACE(_T("BOOL CMailDetailView::BespokeUpdateData(CHermesDoc *pHermesDoc, BOOL bSaveAndValidate) >>\n"));

	ASSERT(pHermesDoc);
	ASSERT(pHermesDoc->IsKindOf(RUNTIME_CLASS(CHermesDoc)));

	CEdit *pEdit = nullptr;
	CComVariant v, *pv;
	CString s, *ps;


	pv = pHermesDoc->GetID();
	ASSERT(pv);
	ASSERT(pv->vt == VT_BSTR);
	TRACE_VARIANTTYPE(pv);
	ps = new CString(pv->bstrVal, SysStringLen(pv->bstrVal));
	TRACE(_T("[bSaveAndValidate=%d] of DOCUMENT ID: [%s]\n"), bSaveAndValidate, *ps);
	delete ps;

	if (bSaveAndValidate) // [SBROTHY]: Update document data from controls
	{
		pEdit = reinterpret_cast<CEdit *>(m_Dlg.GetDlgItem(IDC_EDIT_TO));
		pEdit->GetWindowTextW(s);
		TRACE("IDC_EDIT_TO: [%s]", s);
		v = s;
		pHermesDoc->GetSetTo(&v);

		pEdit = reinterpret_cast<CEdit *>(m_Dlg.GetDlgItem(IDC_EDIT_FROM));
		pEdit->GetWindowTextW(s);
		TRACE("IDC_EDIT_FROM: [%s]", s);
		v = s;
		pHermesDoc->GetSetFrom(&v);

		pEdit = reinterpret_cast<CEdit *>(m_Dlg.GetDlgItem(IDC_EDIT_SUBJECT));
		pEdit->GetWindowTextW(s);
		TRACE("IDC_EDIT_SUBJECT: [%s]", s);
		v = s;
		pHermesDoc->GetSetSubject(&v);

		// [SBROTHY]: TODO: Fix missing fields.
	}
	else // [SBROTHY]: Update controls from document data
	{
		Reset();

		pv = pHermesDoc->GetID();
		ASSERT(pv);
		ASSERT(pv->vt == VT_BSTR);
		TRACE_VARIANTTYPE(pv);
		ps = new CString(pv->bstrVal, SysStringLen(pv->bstrVal));

		CString s;
		s.Format(_T("DOC\tID [%s]"), *ps);
		delete ps;

		TRACE(_T("SETTING DOC ID: %s"), s);

		CComboBox *pComboBox = reinterpret_cast<CComboBox *>(m_Dlg.GetDlgItem(IDC_COMBO_GUIDS));
		ASSERT(pComboBox);
		pComboBox->AddString(s);

		pv = pHermesDoc->GetSetTo();
		ASSERT(pv);
		TRACE_VARIANTTYPE(pv);
		ASSERT(pv->vt == VT_BSTR);

		ps = new CString(pv->bstrVal, SysStringLen(pv->bstrVal));
		pEdit = reinterpret_cast<CEdit *>(m_Dlg.GetDlgItem(IDC_EDIT_TO));
		pEdit->SetWindowTextW(*ps);
		delete ps;

		pv = pHermesDoc->GetSetFrom();
		ASSERT(pv);
		ASSERT(pv->vt == VT_BSTR);
		ps = new CString(pv->bstrVal, SysStringLen(pv->bstrVal));
		pEdit = reinterpret_cast<CEdit *>(m_Dlg.GetDlgItem(IDC_EDIT_FROM));
		pEdit->SetWindowTextW(*ps);
		delete ps;

		pv = pHermesDoc->GetSetSubject();
		ASSERT(pv);
		ASSERT(pv->vt == VT_BSTR);
		ps = new CString(pv->bstrVal, SysStringLen(pv->bstrVal));
		pEdit = reinterpret_cast<CEdit *>(m_Dlg.GetDlgItem(IDC_EDIT_SUBJECT));
		pEdit->SetWindowTextW(*ps);
		pHermesDoc->SetTitle(*ps);
		delete ps;

		// [SBROTHY]: TODO: Fix missing fields.
	}


	TRACE(_T("BOOL CMailDetailView::BespokeUpdateData(CHermesDoc *pHermesDoc, BOOL bSaveAndValidate) <<<\n"));

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMailDetailView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CMailDetailViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CMailDetailViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != nullptr)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMailDetailView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMailDetailView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("Add member function..."));
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMailDetailView::OnClassAddMemberVariable()
{
	// TODO: Add your command handler code here
}

void CMailDetailView::OnClassDefinition()

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	// TODO: Add your command handler code here
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMailDetailView::OnClassProperties()
{
	// TODO: Add your command handler code here
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMailDetailView::OnNewFolder()
{
	AfxMessageBox(_T("New Folder..."));
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMailDetailView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMailDetailView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMailDetailView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Locked */);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMailDetailView::ShowPane(BOOL bShow, BOOL bDelay, BOOL bActivate)
{
	TRACE(_T("void ShowPane(BOOL bShow, BOOL bDelay, BOOL bActivate) >>\n"));

	if (bShow)
	{
		UpdateData(FALSE);
	}
	else
	{
		TRACE(_T("PANE IS BEING HIDDEN\n"));
	}

	CDockablePane::ShowPane(bShow, bDelay, bActivate);
	TRACE(_T("void ShowPane(BOOL bShow, BOOL bDelay, BOOL bActivate) <<\n"));
}
