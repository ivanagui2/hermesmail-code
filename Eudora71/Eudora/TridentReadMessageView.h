// TridentReadMessageView.h : interface of the CTridentReadMessageView class
//
// Copyright (c) 1997-2001 by QUALCOMM, Incorporated
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

//


#ifndef TRIDENTREADMESSAGEVIEW_H
#define TRIDENTREADMESSAGEVIEW_H

#include "TridentView.h"

class CReadMessageDoc;
class CReadMessageFrame;

class CTridentReadMessageView : public CTridentView
{
	BSTR				m_bstrBookmark;
	CReadMessageFrame*	m_pParentFrame;

	void FillBlockFormatCombo();

	BOOL SplitTagOff(
	IHTMLElement*	pElement,
	UINT			uStringID );

	BOOL ExpandTag(
	IHTMLElement*	pElement,
	UINT			uStringID );

	BOOL WrapTag(
	LPCSTR	szTagOn,
	LPCSTR	szTagOff );

protected: // create from serialization only
	CTridentReadMessageView();
	DECLARE_DYNCREATE(CTridentReadMessageView)

public:
	CReadMessageDoc* GetDocument();

	BOOL IsEditing();
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTridentReadMessageView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTridentReadMessageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTridentReadMessageView)
	afx_msg void OnEditBold();
	afx_msg void OnUpdateEditBold(CCmdUI* pCmdUI);
	afx_msg void OnEditItalic();
	afx_msg void OnUpdateEditItalic(CCmdUI* pCmdUI);
	afx_msg void OnEditUnderline();
	afx_msg void OnUpdateEditUnderline(CCmdUI* pCmdUI);
	afx_msg void OnEditStrikeout();
	afx_msg void OnUpdateEditStrikeout(CCmdUI* pCmdUI);
	afx_msg void OnFilePrint();
	afx_msg void OnEditCheckspelling();
	afx_msg void OnUpdateEditCheckspelling(CCmdUI* pCmdUI);
	afx_msg void OnEditTextFont();
	afx_msg void OnUpdateEditTextFont(CCmdUI* pCmdUI);
	afx_msg void OnEditTextPlain();
	afx_msg void OnUpdateEditTextPlain(CCmdUI* pCmdUI);
	afx_msg void OnEditTextSizeHumongous();
	afx_msg void OnEditTextSizeLarge();
	afx_msg void OnEditTextSizeLarger();
	afx_msg void OnEditTextSizeMedium();
	afx_msg void OnEditTextSizeSmall();
	afx_msg void OnEditTextSizeVeryLarge();
	afx_msg void OnEditTextSizeVerySmall();
	afx_msg void OnColor0();
	afx_msg void OnColor1();
	afx_msg void OnColor10();
	afx_msg void OnColor11();
	afx_msg void OnColor12();
	afx_msg void OnColor13();
	afx_msg void OnColor14();
	afx_msg void OnColor15();
	afx_msg void OnColor16();
	afx_msg void OnColor2();
	afx_msg void OnColor3();
	afx_msg void OnColor4();
	afx_msg void OnColor5();
	afx_msg void OnColor6();
	afx_msg void OnColor7();
	afx_msg void OnColor8();
	afx_msg void OnColor9();
	afx_msg void OnLastTextColor();
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnBlkfmtBullettedList();
	afx_msg void OnUpdateBlkfmtBullettedList(CCmdUI* pCmdUI);
	afx_msg void OnBlkfmtH1();
	afx_msg void OnUpdateBlkfmtH1(CCmdUI* pCmdUI);
	afx_msg void OnBlkfmtH2();
	afx_msg void OnUpdateBlkfmtH2(CCmdUI* pCmdUI);
	afx_msg void OnBlkfmtH3();
	afx_msg void OnUpdateBlkfmtH3(CCmdUI* pCmdUI);
	afx_msg void OnBlkfmtH4();
	afx_msg void OnUpdateBlkfmtH4(CCmdUI* pCmdUI);
	afx_msg void OnBlkfmtH5();
	afx_msg void OnUpdateBlkfmtH5(CCmdUI* pCmdUI);
	afx_msg void OnBlkfmtH6();
	afx_msg void OnUpdateBlkfmtH6(CCmdUI* pCmdUI);
	afx_msg void OnBlkfmtNormal();
	afx_msg void OnUpdateBlkfmtNormal(CCmdUI* pCmdUI);
	afx_msg void OnEditInsertLink();
	afx_msg void OnUpdateEditInsertLink(CCmdUI* pCmdUI);
	afx_msg void OnEditInsertPicture();
	afx_msg void OnUpdateEditInsertPicture(CCmdUI* pCmdUI);
	afx_msg void OnEditInsertHr();
	afx_msg void OnUpdateEditInsertHr(CCmdUI* pCmdUI);
	afx_msg void OnBlkfmtFixedwidth();
	afx_msg void OnUpdateBlkfmtFixedwidth(CCmdUI* pCmdUI);
	afx_msg void OnEditPasteAsQuotation();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnBgcolor();
	afx_msg void OnUpdateBgcolor(CCmdUI* pCmdUI);

	afx_msg void OnRightAlign();
	afx_msg void OnUpdateRightAlign(CCmdUI* pCmdUI);

	afx_msg void OnLeftAlign();
	afx_msg void OnUpdateLeftAlign(CCmdUI* pCmdUI);

	afx_msg void OnCenterAlign();
	afx_msg void OnUpdateCenterAlign(CCmdUI* pCmdUI);

	afx_msg void OnIndent();
	afx_msg void OnUpdateIndent(CCmdUI* pCmdUI);
	afx_msg void OnOutdent();
	afx_msg void OnUpdateOutdent(CCmdUI* pCmdUI);

	afx_msg void OnClearFormatting();
	afx_msg void OnUpdateClearFormatting(CCmdUI* pCmdUI);

	afx_msg void OnDelete();
	afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);

	afx_msg void OnUndo();
	afx_msg void OnUpdateUndo(CCmdUI* pCmdUI);
	afx_msg void OnRedo();
	afx_msg void OnUpdateRedo(CCmdUI* pCmdUI);

	afx_msg void OnCut();
	afx_msg void OnUpdateCut(CCmdUI* pCmdUI);
	afx_msg void OnPaste();
	afx_msg void OnUpdatePaste(CCmdUI* pCmdUI);

	afx_msg void OnFilePrintPreview();

	afx_msg void OnFgcolor();
	afx_msg void OnUpdateFgcolor(CCmdUI* pCmdUI);

	afx_msg void OnUpdateEditMode(CCmdUI* pCmdUI);

	void DoBlockFmtChange( LPCSTR szBlockFmt );
	void UpdateBlockFmt(CCmdUI* pCmdUI, LPCSTR szBlockFmt );

	afx_msg void OnFontChange();
	afx_msg void OnUpdateFont(CCmdUI* pCmdUI);

	afx_msg void OnBlahBlahBlah();
	afx_msg void OnFixedFont();

	void SetBgColor( COLORREF color );
	void SetFgColor( COLORREF color );

	afx_msg void OnUpdateSize(CCmdUI* pCmdUI);

	void ClearUndoStack();

	static UINT		s_umsgReloadAfterDoneEditing;

public:
	virtual HRESULT SaveInfo();
	
    virtual BOOL GetAllText(CString& szText);

	virtual BOOL SetSelectedText(const char* szText, bool bSign);
	virtual BOOL SetAllText(const char* szText, bool bSign);
	virtual BOOL SetSelectedHTML(const char* szHTML, bool bSign);
	virtual BOOL SetAllHTML(const char* szHTML, bool bSign);

	virtual BOOL GetMessageAsHTML(CString& msg, BOOL IncludeHeaders = FALSE);
	virtual BOOL GetMessageAsText(CString& msg, BOOL IncludeHeaders = FALSE); 

	bool					Reload(
									bool				in_bShouldConcentrate,
									bool				in_bIsBlahBlahBlah,
									bool				in_bSilenceFileErrors = false);
	afx_msg long			OnReloadAfterDoneEditing(WPARAM wParam, LPARAM lParam);
	afx_msg void			OnEditMode();

	virtual CTocDoc *		GetTocDoc();

	virtual int				GetNumMessageDocs() const;
	virtual CMessageDoc *	GetMessageDoc(int in_nMessageIndex = 0, bool * out_bLoadedDoc = NULL, CSummary ** out_ppSummary = NULL);
	virtual bool			IsNoMessageToDisplayAllowed() const;
	virtual void			SetTitleIfNecessary(CMessageDoc * in_pDoc);
	virtual BOOL			UseFixedFont() const;
	virtual bool			ShouldShowAllHeaders() const;
	virtual ContentConcentrator::ContextT	GetContentConcentratorContext(bool in_bIsBlahBlahBlah = false) const;

	virtual void			WriteHeaders(
									CFile &				theFile,
									CMessageDoc *		pDoc,
									const CString &		szHeaders,
									BOOL				bShowTabooHeaders);

	BOOL GetMessageBody(CString& szMsg, BOOL bAsHTML);

	void SetBodyText(LPCSTR szBody);
};

#ifndef _DEBUG  // debug version in CarrotView.cpp
inline CReadMessageDoc* CTridentReadMessageView::GetDocument()
   { return (CReadMessageDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
#endif
