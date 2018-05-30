// HeaderView.cpp : implementation file
//
// Copyright (c) 1993-2001 by QUALCOMM, Incorporated
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

#include "stdafx.h"

#include "headervw.h"
#include "PooterButton.h"

#include <afxcmn.h>
#include <afxrich.h>

#include "resource.h"
#include "rs.h"
#include "helpxdlg.h"
#include "doc.h"
#include "cursor.h"
#include "fileutil.h"
#include "summary.h"
#include "msgdoc.h"
#include "compmsgd.h"
#include "mdichild.h"
#include "msgframe.h"
#include "bmpcombo.h"
#include "3dformv.h"
#include "mainfrm.h"
#include "spell.h"
#include "BossProtector.h"
#include "header.h"		// for MIME.H
#include "mime.h"		// for TextReader class
#include "QCProtocol.h"
#include "QCRecipientCommand.h"
#include "tocdoc.h"
#include "utils.h"
#include "guiutils.h"
#include "address.h"
#include "eudora.h"
#include "progress.h"
#include "sendmail.h"
#include "changeq.h"
#include "mainfrm.h"
#include "filtersd.h"
#include "tocview.h"
#include "saveas.h"
#include "msgutils.h"
#include "pop.h"
#include "password.h"
#include "persona.h"
#include "msgopts.h"
#include "AutoCompleteSearcher.h"
#include "CompMessageFrame.h"
#include "PgCompMsgView.h"
#include "QCSharewareManager.h"

#include "PgStyleUtils.h"
#include "PaigeStyle.h"
#include "SpellDialog.h"

#include "ems-wglu.h"
#include "trnslate.h"  
#include "nickdoc.h"
#include "MoodWatch.h"


#include "QCCommandActions.h"
#include "QCCommandStack.h"
extern QCCommandStack		g_theCommandStack;

#include "DebugNewHelpers.h"


// our own unique little windows message for scrolling stuff
UINT wmScrollToShowCaret = RegisterWindowMessage( "wmScrollToShowCaret" );

//
// Statics.
//

// HACK -- for now, we keep our own font info. This prevents us from changing
// our display font when Eudora's Screen Font changes.
int			CHeaderView::s_nObjectCount = 0;
CFontInfo	CHeaderView::s_editFont;
CFontInfo	CHeaderView::s_labelFont( FW_BOLD );

BOOL		CHeaderView::m_fShortcutTableInitialized = FALSE;
char		CHeaderView::m_ShortcutTable[];

int splitY = 0;
extern CPersonality g_Personalities;

enum {AUTO_HEADER_SPELL_TIMER =111, AUTO_HEADER_BP_MAIL_TIMER};
#define SPELL_INTERVAL 500    // auto spell check timer interval (mSec)
//for Boss Protector
#define BP_INTERVAL 500

/////////////////////////////////////////////////////////////////////////////
// CHeaderView

IMPLEMENT_DYNCREATE(CHeaderView, CFormView)


CHeaderView::CHeaderView()
    : CFormView(CHeaderView::IDD)
{
    //{{AFX_DATA_INIT(CHeaderView)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

    // instantiate a 'HeaderField' object for each static/edit combo
    // in our dialog template (e.g. "To:", "From:", etc...), and add them
    // to our linked list. We attach these to the form later.

    m_LastChar = 0;

    for ( int i = IDC_HDRFLD_EDIT1; i <= IDC_HDRFLD_EDIT6; i++ ) {
	m_headerFields.AddTail( DEBUG_NEW CHeaderField );
    }

    fUpdating = FALSE;
    fSizing = FALSE;
	m_nMoodScore = MSM_MOOD_UNKNOWN;
    m_headerRows = 88;
    m_extraRows = 0;
    CtlBrush.CreateSolidBrush( GetSysColor( COLOR_WINDOW ) );
    m_CurrentHeader = DONT_KNOW;
	m_BadNames = DEBUG_NEW AutoCompList(_T("BadAdds.lst"));
	m_BadNames->SetMaxSize(-1);
	m_bJustQueue = FALSE;
	m_pPgCompMsgView = NULL;
}

CHeaderView::~CHeaderView()
{
	if (m_BadNames)
		delete m_BadNames;
	// delete our list of HeaderField objects
	CHeaderField* pField;
	for ( int i = IDC_HDRFLD_EDIT6; i >= IDC_HDRFLD_EDIT1; i-- ) 
	{
		pField = m_headerFields.RemoveTail();
		ASSERT( pField != NULL );
		/*if (pField && i  == IDC_HDRFLD_EDIT3)
			pField->KillSpellTimer();*/
		delete pField;
	}
}


void CHeaderView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHeaderView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHeaderView, CFormView)
    //{{AFX_MSG_MAP(CHeaderView)
    ON_WM_CTLCOLOR()
    ON_WM_SIZE()
    ON_COMMAND(ID_FILE_OPEN_ATTACHMENT, OnOpenAttachment)
    ON_UPDATE_COMMAND_UI(ID_FILE_OPEN_ATTACHMENT, OnUpdateOpenAttachment)
    ON_EN_KILLFOCUS(IDC_HDRFLD_EDIT1, OnKillFocusTo)
    ON_EN_KILLFOCUS(IDC_HDRFLD_EDIT3, OnKillFocusHeader)// Subject
    ON_EN_KILLFOCUS(IDC_HDRFLD_EDIT4, OnKillFocusCc)	// Cc
    ON_EN_KILLFOCUS(IDC_HDRFLD_EDIT5, OnKillFocusBcc)	// Bcc
    ON_EN_KILLFOCUS(IDC_HDRFLD_EDIT6, OnKillFocusAttachments)	// Attach
    ON_EN_SETFOCUS(IDC_HDRFLD_EDIT1, OnSetFocusTo)
    ON_EN_SETFOCUS(IDC_HDRFLD_EDIT2, OnSetFocusFrom)
    ON_EN_SETFOCUS(IDC_HDRFLD_EDIT3, OnSetFocusSubject)
    ON_EN_SETFOCUS(IDC_HDRFLD_EDIT4, OnSetFocusCc)
    ON_EN_SETFOCUS(IDC_HDRFLD_EDIT5, OnSetFocusBcc)
    ON_EN_SETFOCUS(IDC_HDRFLD_EDIT6, OnSetFocusAttachments)
	ON_WM_CREATE()
    ON_WM_DESTROY( )
	ON_WM_TIMER()

    //}}AFX_MSG_MAP
    ON_CONTROL_RANGE(EN_UPDATE, IDC_HDRFLD_EDIT1, IDC_HDRFLD_EDIT6, OnUpdateEdit)
    ON_CONTROL_RANGE(EN_CHANGE, IDC_HDRFLD_EDIT1, IDC_HDRFLD_EDIT6, OnChangeEdit)
    ON_REGISTERED_MESSAGE( wmScrollToShowCaret, OnScrollToShowCaret )
    ON_MESSAGE(	WM_CONTEXTMENU, OnContextMenu )
    ON_UPDATE_COMMAND_UI(ID_MESSAGE_ATTACHFILE, OnUpdateMessageAttachFile)
    ON_COMMAND_EX_RANGE( QC_FIRST_COMMAND_ID, QC_LAST_COMMAND_ID, OnDynamicCommand )
    ON_UPDATE_COMMAND_UI_RANGE( QC_FIRST_COMMAND_ID, QC_LAST_COMMAND_ID, OnUpdateDynamicCommand )
    ON_UPDATE_COMMAND_UI( IDC_FONT_COMBO, OnUpdateFontCombo )
    ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTEASQUOTATION, OnUpdateEditPasteAsQuote)
    ON_BN_CLICKED(IDC_HDRFLD_LABEL2, OnFromPooter)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeaderView diagnostics

#ifdef _DEBUG
void CHeaderView::AssertValid() const
{
	CFormView::AssertValid();
}

void CHeaderView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHeaderView message handlers

void CHeaderView::OnFromPooter()
{
	if (!(g_Personalities.GetPersonaCount() > 1L))
		return;

	CMenu* pMenu = CMainFrame::QCGetMainFrame()->GetMessageChangePersonalityMenu();

	if (pMenu)
	{
		CRect theRect;
		CHeaderField* pFld = GetHeaderCtrl(HEADER_FROM);
		pFld->GetLabelWindowRect(&theRect);

		pMenu->TrackPopupMenu(TPM_LEFTALIGN, theRect.left - 1, theRect.bottom + 1, AfxGetMainWnd());
	}
}


void CHeaderView::OnKillFocusTo()
{
	OnKillFocusRecipient(IDC_HDRFLD_EDIT1);
}

void CHeaderView::OnKillFocusCc()
{
	OnKillFocusRecipient(IDC_HDRFLD_EDIT4);
}

void CHeaderView::OnKillFocusBcc()
{
	OnKillFocusRecipient(IDC_HDRFLD_EDIT5);
}

void CHeaderView::OnKillFocusAttachments()
{
	// If a boxe is popped up, get rid of them.
//	CHeaderField* pField = (CHeaderField*)GetDlgItem(IDC_HDRFLD_EDIT6);
//	if (pField && pField->m_ACListBox)
//		pField->m_ACListBox->KillACListBox();
}

void CHeaderView::OnKillFocusRecipient(UINT nID)
{
	CHeaderField* pField = (CHeaderField*)GetDlgItem(nID);
	// If any boxes are popped up, get rid of them.
	if (pField && pField->m_ACListBox)
		pField->m_ACListBox->KillACListBox();


	if (GetIniShort(IDS_INI_AUTO_EXPAND_NICKNAMES) &&
		pField->IsKindOf(RUNTIME_CLASS(CHeaderField)))
	{
		CCompMessageDoc* doc = GetDocument();
		CSummary* Sum;
	
		if (doc && (Sum = doc->m_Sum))
		{
			// auto-expansion must be done on a per-personality basis
			CString	homie =	g_Personalities.GetCurrent();
			g_Personalities.SetCurrent( Sum->GetPersona() );

			int TextLen = pField->GetWindowTextLength();
			char* Text = DEBUG_NEW char[TextLen + 1];
			int NumCopied = pField->GetWindowText(Text, TextLen + 1);
			Text[NumCopied] = 0;
		
			char* ExpandedText = ExpandAliases(Text, TRUE, FALSE);

			bool done = false;
			char* end = NULL;
			char *floater = Text;
			while (!done && floater < (Text + strlen(Text)) )
			{
				end = FindAddressEnd(floater, FALSE, TRUE);
				int length = end? end - floater : 0;

				if (length == 0)
				{
					done = true;  
					break;
				}
				else
				{
					char *address = DEBUG_NEW char[(end - floater) +1];
					strncpy(address, floater, (end - floater));
					address[(end - floater)] = 0;
					const char *FoundBad = m_BadNames->FindACStartingWith(address);
					if (!FoundBad || (strcmp(FoundBad, address) != 0))
					{
						AutoCompList* pACL = GetAutoCompList();
						if (pACL)
						{
							if (!pACL->Contains(address))
								m_AutoArray.Add(address);
							pACL->Add(address, false, true);
						}
					}
					delete [] address;
					floater = end+1;
				}
			}

			if (ExpandedText && (strcmp(ExpandedText, Text) != 0))
			{
				pField->SetExpanded(TRUE);	// We don't grab names from expanded fields 
											// because we don't know where they came from.		
			}

			if (ExpandedText)
			{
				// update the controls
				pField->SetWindowText(ExpandedText);
				OnUpdateEdit(pField->GetDlgCtrlID());
				delete [] ExpandedText;
			}

			delete [] Text;

			// revert back to previous personality
			g_Personalities.SetCurrent( homie );
		}
		pField->m_bMoodDirty = TRUE;
		pField->m_bBPDirty = TRUE;
		pField->m_bBPSpeakDirty = TRUE;
		pField->m_bBPWarnDlgDirty = TRUE;
	}
	else //Not AutoExpanded.
	{
		int TextLen = pField->GetWindowTextLength();
		char* Text = DEBUG_NEW char[TextLen + 1];
		int NumCopied = pField->GetWindowText(Text, TextLen + 1);
		Text[NumCopied] = 0;

		char* end = NULL;
		bool done = false;
		char *floater = Text;
		while (!done && floater < (Text + strlen(Text)) )
		{
			end = FindAddressEnd(floater, FALSE, TRUE);
			int length = end? end - floater : 0;

			if (length == 0)
			{
				done = true;  
				break;
			}
			else
			{
				char *address = DEBUG_NEW char[(end - floater) +1];
				strncpy(address, floater, (end - floater));
				address[(end - floater)] = 0;

				AutoCompList* pACL = GetAutoCompList();
				if (pACL)
				{
					if (!pACL->Contains(address))
						m_AutoArray.Add(address);
					pACL->Add(address, false, true);
				}

				delete [] address;
				floater = end+1;
			}
		}	
		pField->m_bMoodDirty = TRUE;
		pField->m_bBPDirty = TRUE;
		pField->m_bBPSpeakDirty = TRUE;
		pField->m_bBPWarnDlgDirty = TRUE;
		delete [] Text;
	}
	// call the generic CHeaderView kill focus routine
	CHeaderView::OnKillFocusHeader();
}

void CHeaderView::OnKillFocusHeader()
{
    CCompMessageDoc* doc = GetDocument();
    CSummary* Sum;
    char buf[256];
    CString To;
    CString Subject;
	
    if (doc && (Sum = doc->m_Sum))
    {
	GetHeaderCtrl(HEADER_TO)->GetWindowText(To);
	if (To.IsEmpty())
	    GetHeaderCtrl(HEADER_BCC)->GetWindowText(To);
	GetHeaderCtrl(HEADER_SUBJECT)->GetWindowText(Subject);

	GetRealName(To.GetBuffer(To.GetLength()));
	To.ReleaseBuffer();

	//use _snprint so as not to overflow the buffer..
	_snprintf(buf, sizeof(buf), CRString(IDS_SUM_TITLE_FORMAT),
		  (const char*) (To.IsEmpty()? CRString(IDS_NO_RECIPIENT) : To),
		  (Sum->m_Date[0]? ", " : ""), Sum->m_Date,
		  (const char*) (Subject.IsEmpty()? CRString(IDS_NO_SUBJECT) : Subject));

	// BOG: snprintf does not do this for you---amazing how many people
	// don't know that!
	buf[sizeof(buf) - 1] = '\0';

	if (doc->GetTitle() != buf)
	{
	    doc->ReallySetTitle(buf);
	    ((CMainFrame*)AfxGetMainWnd())->QCUpdateTab((SECWorksheet*)GetParentFrame());
	}
    }
}

void CHeaderView::OnSetFocusTo()
{
	m_CurrentHeader = HEADER_TO;

	if ( UsingPaidFeatureSet() )
	{
		// Set the BP mail timer
		SetTimer(AUTO_HEADER_BP_MAIL_TIMER, BP_INTERVAL, NULL);
	}
}

void CHeaderView::OnSetFocusFrom()
{
	m_CurrentHeader = HEADER_FROM;
}

void CHeaderView::OnSetFocusSubject()
{
	CHeaderField *	pHeaderField = GetHeaderCtrl(HEADER_SUBJECT);
	if (pHeaderField)
		pHeaderField->SetSpellTimer();

	m_CurrentHeader = HEADER_SUBJECT;

	// Tell the associated PgCompMsgView (if any) to set the mood timer
	// because we're focused.
	if (m_pPgCompMsgView)
		m_pPgCompMsgView->SetMoodTimer();
}

void CHeaderView::OnSetFocusCc()
{
	m_CurrentHeader = HEADER_CC;

	if ( UsingPaidFeatureSet() )
	{
		// Set the BP mail timer
		SetTimer(AUTO_HEADER_BP_MAIL_TIMER, BP_INTERVAL, NULL);
	}
}

void CHeaderView::OnSetFocusBcc()
{
	m_CurrentHeader = HEADER_BCC;

	if ( UsingPaidFeatureSet() )
	{
		// Set the BP mail timer
		SetTimer(AUTO_HEADER_BP_MAIL_TIMER, BP_INTERVAL, NULL);
	}
}

void CHeaderView::OnSetFocusAttachments()
{
	m_CurrentHeader = HEADER_ATTACHMENTS;
}


// OnCtlColor:
// Sets the background color and text color for the header view window
// and all of its child controls.

HBRUSH CHeaderView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
	pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	return (GetSysColorBrush(COLOR_WINDOW));
}


void CHeaderView::OnInitialUpdate() 
{
	CDC		theDC;
	LONG	lMaxLabelWidth;
	CRect	theRect;
	CRect	theLabelRect;
	CSize	theSize;
	LONG	lNewLabelWidth;
	LONG	lNewEditLeft;
	CFont*	pOldFont;
	LONG	lNewHeight;
	LONG	lYIncrement;
	LONG	lTopSpace;

	theDC.CreateCompatibleDC( NULL );

	// subclass all of the static/edit pairs in our header form. this code
	// depends on us having reserved the correct number of slots in our
	// constructor. get that right, and everything else just happens.

	CHeaderField* pField;
	POSITION pos = m_headerFields.GetHeadPosition();
		
	int iLabel = IDC_HDRFLD_LABEL1;
	int iEdit = IDC_HDRFLD_EDIT1;

	lMaxLabelWidth = 0;

	// select the label font
	pOldFont = theDC.SelectObject( &s_labelFont );

	for (/*MANTSH*/ ; pos; iLabel++, iEdit++ ) {

		pField = m_headerFields.GetNext( pos );
		ASSERT( pField != NULL );
		if ( pField ) {
			pField->Attach( this, iLabel, iEdit );
		}

		// get the width of the label
		theSize = theDC.GetTextExtent( pField->GetLabelText() );
	
		// set the max label width if approriate
		if ( lMaxLabelWidth < theSize.cx )
		{
			lMaxLabelWidth = theSize.cx;
		}
	}

	// reset the font
	theDC.SelectObject( pOldFont );

	// add space
	lMaxLabelWidth += 3;

	// get the new height
	lNewHeight = s_editFont.CellHeight();

	// find the position of the first label & edit controls
	pField = m_headerFields.GetHead();
	
	// get the edit control rect
	pField->GetWindowRect( &theRect );
	ScreenToClient( &theRect );

	// get the label control rect
	pField->GetLabelWindowRect( &theLabelRect );	
	ScreenToClient( &theLabelRect );
	
	// get the space at the top
	lTopSpace = theRect.top;

	// get the new x position of the edit control
	lNewEditLeft = theLabelRect.left + lMaxLabelWidth + ( theRect.left - theLabelRect.right );
	
	// get the new width of the edit control
	lNewLabelWidth = theRect.Width() - ( lNewEditLeft - theRect.left );
					
	for (	lYIncrement = 0, pos = m_headerFields.GetHeadPosition(), iLabel = IDC_HDRFLD_LABEL1, iEdit = IDC_HDRFLD_EDIT1; 
			pos != NULL ; 
			iLabel++, iEdit++ ) 
	{
		pField = m_headerFields.GetNext( pos );
		
		// get the edit control rect
		pField->GetWindowRect( &theRect );
		ScreenToClient( &theRect );

		// set the new size & position
		theRect.left = lNewEditLeft;
		theRect.right = lNewEditLeft + lNewLabelWidth - 1;
		theRect.top += lYIncrement;
		theRect.bottom += lYIncrement;
		
		// get the static rect
		pField->GetLabelWindowRect( &theLabelRect );		
		ScreenToClient( &theLabelRect );

		// set the new size and position
		theLabelRect.right = theLabelRect.left + lMaxLabelWidth + 1;
		theLabelRect.top += lYIncrement;
		theLabelRect.bottom += lYIncrement;

		// add to the increment
		lYIncrement += lNewHeight - theLabelRect.Height();

		// set the new height
		theRect.bottom += lNewHeight - theLabelRect.Height();
		theLabelRect.bottom += lNewHeight - theLabelRect.Height();
		
		// resize and move
		pField->MoveLabelWindow( theLabelRect );
		pField->MoveWindow( theRect );
	}

	// note that much of the header field code is dependent on the resources
	// being sequentially numbered. this was not really required, except in the
	// case of our command routing, so we might as well take advantage of it
	// elswhere. remember, when adding resources...
	//
	// *** PRESERVE THE NUMBERING SCHEME ***
	
	// do a little bone-head math, and set the size of the pane
	// that contains the header view.

	splitY = /*lTopSpace +*/ theRect.bottom;
	CSplitterWnd* pSplit = (CSplitterWnd*) GetParent();
	pSplit->SetRowInfo( 0, splitY, 0 );

	// set the "fUpdating" flag, because RecalcLayout causes a WM_SIZE
	// before we are ready to handle it. Dis is bedy, bedy bad!
	fUpdating = TRUE;
	pSplit->RecalcLayout();
	fUpdating = FALSE;

	GetClientRect( &theRect );
	m_docSize.cx = 0;
	m_docSize.cy = theRect.Size().cy;

	m_lMaxDocSize = m_docSize.cy + ( 3 * s_editFont.CellHeight() );

	m_pageSize.cx = 0;
	m_pageSize.cy = m_docSize.cy / 3;

	m_lineSize.cx = 0;
	m_lineSize.cy = m_docSize.cy / s_editFont.CellHeight();

	SetScrollSizes( MM_TEXT, m_docSize, m_pageSize, m_lineSize );

	// get the header
	SetHeaderFromDoc();

	// if the message has been addressed and the subject is null,
	// move the focus to the subject line
	BOOL bSetActive = FALSE;
	for(int i=HEADER_TO; i < HEADER_IN_REPLY_TO; i++)
	{
		if(IsHeaderInvalid(i))
		{
			SetFocusToHeader(i);
			bSetActive = TRUE;
			break;
		}

	}

	if (!bSetActive && ( GetText( HEADER_TO ) != "" )  &&
		 ( GetText( HEADER_SUBJECT ) == "" ) ) 
	{
		SetFocusToHeader( HEADER_SUBJECT );
		bSetActive = TRUE;
	}

	//
	// Initialize the shortcut key table (one time only).
	//
	if (! CHeaderView::m_fShortcutTableInitialized)
	{
		for (int i = 0; i < MAXHEADERS; i++)
			m_ShortcutTable[i] = GetShortcutLetter(i);

		CHeaderView::m_fShortcutTableInitialized = TRUE;
	}

	// If we can edit all headers, then enable tabstop for From:
	if (GetIniShort(IDS_INI_EDIT_ALL_HEADERS))
		GetDlgItem(IDC_HDRFLD_EDIT2)->ModifyStyle(0, WS_TABSTOP);

	//Only do this if we've set focus to a particular header
	if(bSetActive)
		GetParentFrame()->SetActiveView(this, TRUE);

	// Now go and see if any fields have been screwed with, if so then we dirty the field
	// So that Autocompletion doesn't work on these fields because we wouldn't want to add
	// a bunch of names just because you did a reply to all.
	// Instead we turn it off for that field and then specifically add certain types,
	// like the from address and things added from directory services.
	CHeaderField* pHeaderField = GetHeaderCtrl(HEADER_TO);
	if (pHeaderField)
	{
	
		if (pHeaderField->GetText() != "")
		{
			m_BadNames->PullNamesFromStringAndAdd(pHeaderField->GetText());
			pHeaderField->SetExpanded(true);
		}
		if (GetIniShort(IDS_INI_POPUP_NAMECOMPLETER) != 0)
		pHeaderField->m_DoDropDown = true;

		pHeaderField->m_ACListBox = DEBUG_NEW_NOTHROW AutoCompleterListBox(pHeaderField);
		if (pHeaderField->m_ACListBox)
			pHeaderField->m_ACListBox->SetHotRollover(false);
	}
	pHeaderField = GetHeaderCtrl(HEADER_CC);
	if (pHeaderField)
	{

		if (pHeaderField->GetText() != "")
		{
			m_BadNames->PullNamesFromStringAndAdd(pHeaderField->GetText());
			pHeaderField->SetExpanded(true);
		}
		if (GetIniShort(IDS_INI_POPUP_NAMECOMPLETER) != 0)
			pHeaderField->m_DoDropDown = true;

		pHeaderField->m_ACListBox = DEBUG_NEW_NOTHROW AutoCompleterListBox(pHeaderField);
		if (pHeaderField->m_ACListBox)
			pHeaderField->m_ACListBox->SetHotRollover(false);
	}
	pHeaderField = GetHeaderCtrl(HEADER_BCC);
	if (pHeaderField)
	{
	
		if (pHeaderField->GetText() != "")
		{
			m_BadNames->PullNamesFromStringAndAdd(pHeaderField->GetText());
			pHeaderField->SetExpanded(true);
		}
		if (GetIniShort(IDS_INI_POPUP_NAMECOMPLETER) != 0)
			pHeaderField->m_DoDropDown = true;

		pHeaderField->m_ACListBox = DEBUG_NEW_NOTHROW AutoCompleterListBox(pHeaderField);
		if (pHeaderField->m_ACListBox)
			pHeaderField->m_ACListBox->SetHotRollover(false);
	}

	pHeaderField = GetHeaderCtrl(HEADER_SUBJECT);
	if (pHeaderField)
	{
		pHeaderField->SetSpellTimer();
		//Workaround so the subject will get spell checked first time
		// when editing queued messages
		CWnd *pOldWnd = pHeaderField->SetFocus();
		if (pOldWnd) 
			pOldWnd->SetFocus();
	}

	if ( UsingPaidFeatureSet() )
	{
		// Set the BP mail timer (probably not necessary, because we'll do this
		// when headers get focus, but just in case).
		SetTimer( AUTO_HEADER_BP_MAIL_TIMER, BP_INTERVAL, NULL );
	}

	CCompMessageFrame * pFrame = DYNAMIC_DOWNCAST( CCompMessageFrame, GetParentFrame() );

	if (pFrame)
		m_pPgCompMsgView = pFrame->GetPgCompMsgView();

	//Disabled File completion. This can be a project for someone else.
//	pHeaderField = GetHeaderCtrl(HEADER_ATTACHMENTS);
//	if (pHeaderField)
//	{
//		if (GetIniShort(IDS_INI_POPUP_FILECOMPLETER) != 0)
//		{
//			pHeaderField->m_DoDropDown = true;
//			pHeaderField->m_ACListBox = new AutoCompleterListBox(pHeaderField);
//			if (pHeaderField->m_ACListBox)
//				pHeaderField->m_ACListBox->SetHotRollover(false);
//		}
//	}
}

void CHeaderView::OnUpdateEdit( UINT nID )
{
	// this routine is not re-entrant.
	if ( fUpdating ) {
		return;
	}

	fUpdating = TRUE;

	// get the object that sent this message
	CHeaderField* pField = (CHeaderField*) GetDlgItem( nID );

	// get the current number of lines
	int nLines = pField->GetLineCount();
	int nLinesChanged = 0;

	// if there is more text than the edit can display, we grow it. if there
	// there is more room than we need, we shrink the control.

	int maxRows = m_lMaxDocSize;
	int dontCare;
	((CSplitterWnd*)GetParent())->GetRowInfo( 0, m_headerRows, dontCare );

	CHeaderField::Direction ndgDir = CHeaderField::NdgNil;
	if ( pField->m_numLines < nLines ) {

		// displayed lines are less than the actual number
		ndgDir = CHeaderField::NdgDown;
		nLinesChanged = nLines - pField->m_numLines;
		pField->Grow( nLinesChanged );
		m_docSize.cy += nLinesChanged * s_editFont.CellHeight();
		int fudge = m_headerRows - m_docSize.cy;
		BOOL goodFudge = ( fudge < 20 && fudge > -20 );

		if ( (m_headerRows >= splitY ) || goodFudge ) {
			if ( m_headerRows < maxRows ) {
				m_headerRows = m_docSize.cy;
			}

			((CSplitterWnd*)GetParent())->SetRowInfo( 0, m_headerRows, 0 );
			((CSplitterWnd*)GetParent())->RecalcLayout();
		}

		SetScrollSizes( MM_TEXT, m_docSize, m_pageSize, m_lineSize );
	}
	else if ( nLines < pField->m_numLines ) {

		// displayed lines are greater than the actual number
		ndgDir = CHeaderField::NdgUp;
		nLinesChanged = pField->m_numLines - nLines;
		pField->Shrink( nLinesChanged );
		m_docSize.cy -= nLinesChanged * s_editFont.CellHeight();
		SetScrollSizes( MM_TEXT, m_docSize, m_pageSize, m_lineSize );

		if ( m_headerRows > m_docSize.cy )//&& m_headerRows <= maxRows ) {
		{
			m_headerRows -= nLinesChanged * s_editFont.CellHeight();
			
			if ( m_headerRows < splitY ) {
				m_headerRows = splitY;
			}
			
			((CSplitterWnd*)GetParent())->SetRowInfo( 0, m_headerRows, 0 );
			((CSplitterWnd*)GetParent())->RecalcLayout();
		}
	}

	// we must adjust the position of any fields below the one
	// we just moved.
	if ( ndgDir != CHeaderField::NdgNil ) {

		// find the current (changed) field. note that, quite unbelievably,
		// a 'Find' followed by a 'GetNext' does not return the next field!
		// this, of course, SUCKS!!
		POSITION pos = m_headerFields.Find( pField );
		m_headerFields.GetNext( pos );

		CHeaderField* pNextField;
		while ( pos ) {
			pNextField = m_headerFields.GetNext( pos );
			ASSERT( pNextField != NULL );
			pNextField->Noodge( ndgDir, pField->LineHeight() * nLinesChanged );
		}

		// update the entire Header view. if we were truly cool, we would
		// only invalidate the effected area. maybe later...
		Invalidate();
	}

	// must... not... forget... this... number. (kudos to Adam West)
	pField->m_numLines = nLines;


//	if (m_DoAutoComplete == 1)
//		m_DoAutoComplete++;
//	else if (m_DoAutoComplete == 2 && GetIniShort(IDS_INI_DO_NN_AUTOCOMPLETE) &&
//			(nID == IDC_HDRFLD_EDIT1 || nID == IDC_HDRFLD_EDIT4 || nID == IDC_HDRFLD_EDIT5)) // To CC and BCC Only Please.
//	{
//		CHeaderField* TheField = GetHeaderCtrl(GetCurrentHeader());
//		TheField->FinishNNAutoComplete();
//		m_DoAutoComplete = false;
//	}
//	else if (m_DoAutoComplete == 2 && GetIniShort(IDS_INI_DO_NN_AUTOCOMPLETE) && nID == IDC_HDRFLD_EDIT6)
//	{
//		CHeaderField* TheField = GetHeaderCtrl(GetCurrentHeader());
//		TheField->FinishFilenameAutoComplete();
//		m_DoAutoComplete = false;		
//	}
//
	fUpdating = FALSE;
}

void CHeaderView::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == AUTO_HEADER_BP_MAIL_TIMER)
	{
		if ( UsingPaidFeatureSet() )
		{
			if ( IsHeaderBPDirty() )
			{
				KillTimer( AUTO_HEADER_BP_MAIL_TIMER );

				//////////////////////////////////
				//do a BP check on all headers here
				//////////////////////////////////
				bool bBPWarning = DoBPCheck();
				CCompMessageFrame* pFrame = DYNAMIC_DOWNCAST(CCompMessageFrame, GetParentFrame());
				if (pFrame)
				{
					pFrame->SetBPWarning(bBPWarning);
					pFrame->PostMessage(WM_USER_UPDATE_IMMEDIATE_SEND);
				}

				if (bBPWarning) 
					ValidateFields(false, true);

				SetHeaderBPDirty(FALSE);			

				// If we're still focussed set the timer again
				CWnd *		pFocusWnd = CWnd::GetFocus();
				CWnd *		pParentOfFocusWnd = pFocusWnd ? pFocusWnd->GetParent() : NULL;
				if ( (pFocusWnd == this) || (pParentOfFocusWnd == this) )
					SetTimer( AUTO_HEADER_BP_MAIL_TIMER, BP_INTERVAL, NULL );
			}
		}
		else
		{
			KillTimer(AUTO_HEADER_BP_MAIL_TIMER);
		}
	}
}

//Description: This function will add the text in the header to the TAE engine.
// It then checks for the offsets returned by TAE will draw the lines differently
//for H-Word and non-H Word.
//Input : pointer the CMoodWatch class passed by the CompMsgView.
int CHeaderField::AddFieldMoodText(CMoodWatch *pMoodWatch)
{
	if(pMoodWatch)
	{
		TAEAllMatches sTaeAllMatches;
		int nNumofLines = GetLineCount();
		char strData[1024];
		ClearAllMoodDirtyPairs();

		for(int nLineNum = 0; nLineNum<nNumofLines;nLineNum++)
		{
			memset(strData,0,1024);
			if(GetLine(nLineNum,strData,LineLength(nLineNum)))
			{
				//TAE_IGNORESAFETEXT was added to not consider the part of the text for scoring if it not have any
				//flaming words. So in this case we ignore the header if it does not have any flaming words.
				if(pMoodWatch->AddText(strData,strlen(strData),&sTaeAllMatches,
					TAE_IGNORESAFETEXT))
				{
					if(GetIniShort(IDS_INI_MOOD_MAIL_CHECK)
						&& GetIniShort(IDS_INI_MOOD_SHOW_COMP_BADWORDS))
					{
						DrawLine(&sTaeAllMatches, nLineNum);
						//Invalidate();
					}
					TAEFreeAllMatches(&sTaeAllMatches);
				}
			}
		}
		/*if(IsMoodDirty())
			Invalidate();*/
	}
	return 1;
}

//This function actually draws the line in the edit box for MOOD bad words
int  CHeaderField::DrawLine(TAEAllMatches *pTaeAllMatches, int nLineNum)
{
	if (!pTaeAllMatches)
		return 0;
	//CClientDC clientDC(this);
	TAEMatch *ptaematch ;
	for (int i = 0; i < pTaeAllMatches->iNumMatches; i++)
	{
		ptaematch = (struct TAEMatch *)(pTaeAllMatches->ptaematches + i);
		if (ptaematch)
		{
			int nWordType = MOOD_NON_H_WORD;
			if (ptaematch->nCollection % 2 == 0)
				nWordType = MOOD_H_WORD;
			CPoint StartPt = PosFromChar(ptaematch->lStart);	
			CPoint EndPt = PosFromChar(ptaematch->lStart+
							ptaematch->lLength-1);
			CString strTemp = GetText();
			strTemp = strTemp.GetAt(ptaematch->lStart+ptaematch->lLength-1);
			//To draw the last char of the bad word
			CClientDC dc(this);
			CSize LastCharSize = dc.GetTextExtent(strTemp);
			EndPt.x+=LastCharSize.cx;
			//
			//int Height = GetMessageFont().CellHeight()-3;
			int Height = LastCharSize.cy-3;
			StartPt.y+=(nLineNum*Height)+(nLineNum*3)+Height; 
			EndPt.y+= (nLineNum*Height)+(nLineNum*3)+Height;
			QueueDirtyPair(ID_MOOD_PAIR,StartPt, EndPt, nWordType);
		}
	}
	return 1;
}

//This function actually draws the line in the edit box for misspelled words
int CHeaderField::DrawMisspelledLine(int nStartOffset, int nEndOffset)
{
	CPoint StartPt = PosFromChar(nStartOffset);	
	CPoint EndPt = PosFromChar(nEndOffset);
	/*CString strTemp = strTemp.GetAt(nEndOffset);
	//To draw the last char of the bad word
	CClientDC dc(this);
	CSize LastCharSize = dc.GetTextExtent(strTemp);
	EndPt.x+=LastCharSize.cx;*/
	
	//int Height = GetMessageFont().CellHeight()-3;
	CClientDC dc(this);
	CSize szStr = dc.GetTextExtent(GetText()); 
	int Height = szStr.cy-3;
	/*StartPt.y+=(nLineNum*Height)+Height; 
	EndPt.y+= (nLineNum*Height)+Height;*/
	StartPt.y+=Height; 
	EndPt.y+= Height;

	QueueDirtyPair(ID_SPELL_PAIR, StartPt, EndPt);
	return 1;
}

//This function actually draws the line in the edit box for BP addresses
int CHeaderField::DrawBPAddressesLine(int nStartOffset, int nEndOffset)
{
	CPoint StartPt = PosFromChar(nStartOffset);	
	CPoint EndPt = PosFromChar(nEndOffset);
	CString szStr = GetText();
	szStr = szStr.GetAt(nEndOffset);
	//To draw the last char of the BP address
	CClientDC dc(this);
	CSize LastCharSize = dc.GetTextExtent(szStr);
	EndPt.x+=LastCharSize.cx;
	int Height = LastCharSize.cy-3;
	StartPt.y+=Height; 
	EndPt.y+= Height;

	QueueDirtyPair(ID_BP_PAIR, StartPt, EndPt);
	return 1;
}

void CHeaderField::OnPaint() 
{
	CRect rect;
	GetUpdateRect(&rect);

	CEdit::OnPaint();
	CClientDC dc(this);
	dc.SetBoundsRect(&rect,DCB_ACCUMULATE);
	
	//This is code is for  drawing the Misspelled words
	if (m_pSpellDirtyPair)
	{
		HeaderDirtyPairPtr pDirtyPair = m_pSpellDirtyPair;
		while(pDirtyPair)
		{
			CPoint StartPt(pDirtyPair->LineStartX,pDirtyPair->LineStartY);
		   PGS_DrawUnderline( dc, &StartPt,
				   pDirtyPair->LineEndX-pDirtyPair->LineStartX,
				   kSpellUnderline);
			pDirtyPair= pDirtyPair->next;
		}
	}

	//This is code is for  drawing BP addresses
	if (m_pBPDirtyPair)
	{
		HeaderDirtyPairPtr pDirtyPair = m_pBPDirtyPair;
		while(pDirtyPair)
		{
			CPoint StartPt(pDirtyPair->LineStartX,pDirtyPair->LineStartY);
		    PGS_DrawUnderline( dc, &StartPt,
				   pDirtyPair->LineEndX-pDirtyPair->LineStartX,
				   kBPUnderline);
			pDirtyPair= pDirtyPair->next;
		}
	}

	//This is code is for  drawing the Mood bad words
	if(GetIniShort(IDS_INI_MOOD_MAIL_CHECK)
		&& GetIniShort(IDS_INI_MOOD_SHOW_COMP_BADWORDS))
	{
		//COLORREF lineColor;
		//CPen newPen;
		//CPoint begin,end;
		int nMoodScore = ((CHeaderView*)GetParent())->GetMoodScore();
	    if ( m_pMoodDirtyPair && nMoodScore >1 && nMoodScore < 5 ) 
		{
			HeaderDirtyPairPtr pDirtyPair = m_pMoodDirtyPair;
			while ( pDirtyPair )
			{
				CPoint StartPt(pDirtyPair->LineStartX,pDirtyPair->LineStartY);

				switch(pDirtyPair->nMoodWordType)
				{
					case MOOD_H_WORD:
						//lineColor = 0x000000FF;//for now red color
					   PGS_DrawUnderline( dc, &StartPt,
						   pDirtyPair->LineEndX-pDirtyPair->LineStartX,
						   kHWordUnderline);

						break;
					case MOOD_NON_H_WORD:
						//lineColor = RGB(0,255,0);//for now Green Color
					   PGS_DrawUnderline( dc, &StartPt,
						   pDirtyPair->LineEndX-pDirtyPair->LineStartX,
						   kNonHWordUnderline);
						break;
					default:
						return;
				}
				pDirtyPair= pDirtyPair->next;

			}
		}
	}
}

void CHeaderField::ClearAllDirtyPairs()
{
	ClearAllMoodDirtyPairs();
	ClearAllSpellDirtyPairs();
	ClearAllBPDirtyPairs();
}

void CHeaderField::ClearAllMoodDirtyPairs()
{
	CPoint begin,end;
	int nWordType;
	while ( DequeueDirtyPair(ID_MOOD_PAIR,&begin, &end,&nWordType ) )
	{ }
}
void CHeaderField::ClearAllSpellDirtyPairs()
{
	CPoint begin,end;
	int nWordType;
	while ( DequeueDirtyPair(ID_SPELL_PAIR,&begin, &end,&nWordType ) )
	{ }
}

void CHeaderField::ClearAllBPDirtyPairs()
{
	CPoint begin,end;
	int nWordType;
	while ( DequeueDirtyPair(ID_BP_PAIR,&begin, &end,&nWordType ) )
	{ }
}

void CHeaderField::OnTimer(UINT nIDEvent)//OnDoInteractiveSpellCheck() 
{
	if (nIDEvent == AUTO_HEADER_SPELL_TIMER && UsingFullFeatureSet() && 
		GetDlgCtrlID() == IDC_HDRFLD_EDIT3 &&
		m_bSpellDirty && !GetIniShort( IDS_INI_INTERACTIVE_SPELL_CHECK )&&
		!m_bServicingSpellTimer)
	{
		m_bServicingSpellTimer = true;
			DoSpellCheck();
		m_bServicingSpellTimer = false;

	}
}

void CHeaderField::DoSpellCheck()
{
	KillSpellTimer();
	ClearAllSpellDirtyPairs();
	CSpell Spell(TRUE);
	Spell.Check(this,0,FALSE,TRUE);
	Invalidate();
	m_bSpellDirty = FALSE;

	if ( CWnd::GetFocus() == this )
		SetSpellTimer();
}

void CHeaderField::SetSpellTimer()
{
	if ( UsingFullFeatureSet() )
	{
		UINT nTimer = SetTimer( AUTO_HEADER_SPELL_TIMER, SPELL_INTERVAL, NULL );
		ASSERT(nTimer);
	}
	else
	{
		KillSpellTimer();
	}
}

void CHeaderField::KillSpellTimer()
{
   KillTimer(AUTO_HEADER_SPELL_TIMER);
}


bool CHeaderField::DoBPCheck(CStringList * pAddressList /* = NULL */)
{
	ClearAllBPDirtyPairs();
	CBossProtector BossProtector;
	bool bBPWarning = BossProtector.Check(this, 0, pAddressList);
	Invalidate();
	m_bBPDirty = FALSE; 
	return bBPWarning;
	
}

void CHeaderField::OnDestroy() 
{
	if (GetDlgCtrlID() == IDC_HDRFLD_EDIT3)
		KillSpellTimer();
	CEdit::OnDestroy();
}


void CHeaderField::QueueDirtyPair(int nPairType,
								  CPoint StartPt, CPoint EndPt, int nWordType /*=0*/  )
{
    HeaderDirtyPairPtr pRng = DEBUG_NEW_NOTHROW HeaderDirtyPair;

    if ( pRng ) {
		pRng->LineStartX = StartPt.x;
		pRng->LineStartY =StartPt.y;
		pRng->LineEndX = EndPt.x;
		pRng->LineEndY = EndPt.y;

		pRng->nMoodWordType = nWordType;
        
		pRng->next = NULL;
		
		HeaderDirtyPairPtr pDirtyPair= NULL;
		if (nPairType == ID_MOOD_PAIR)
			pDirtyPair = m_pMoodDirtyPair;
		else if(nPairType == ID_SPELL_PAIR)
			pDirtyPair = m_pSpellDirtyPair;
		else if(nPairType == ID_BP_PAIR)
			pDirtyPair = m_pBPDirtyPair;

        if ( pDirtyPair ) {
            HeaderDirtyPairPtr pTrv = pDirtyPair;
            for ( ; pTrv->next; pTrv = pTrv->next )
                ;
            pTrv->next = pRng;
        }
        else
        {
			if (nPairType == ID_MOOD_PAIR)
				m_pMoodDirtyPair=pRng;
			else if(nPairType == ID_SPELL_PAIR)
				m_pSpellDirtyPair=pRng;
			else if(nPairType == ID_BP_PAIR)
				m_pBPDirtyPair=pRng;
			//pDirtyPair = pRng;
		}
    }
}

bool CHeaderField::DequeueDirtyPair(int nPairType,
									CPoint* StartPt, CPoint* EndPt, int* nWordType )
{
    bool bRet = false;
	HeaderDirtyPairPtr pDirtyPair=NULL;
	if (nPairType == ID_MOOD_PAIR)
		pDirtyPair = m_pMoodDirtyPair;
	else if(nPairType == ID_SPELL_PAIR)
		pDirtyPair = m_pSpellDirtyPair;
	else if(nPairType == ID_BP_PAIR)
		pDirtyPair = m_pBPDirtyPair;

    if ( pDirtyPair ) {
        HeaderDirtyPairPtr pRng = pDirtyPair;
		if (nPairType == ID_MOOD_PAIR)
			m_pMoodDirtyPair=pRng->next;
		else if(nPairType == ID_SPELL_PAIR)
			m_pSpellDirtyPair=pRng->next;
		else if(nPairType == ID_BP_PAIR)
			m_pBPDirtyPair=pRng->next;
        StartPt->x = pRng->LineStartX;
        StartPt->y = pRng->LineStartY;
        EndPt->x = pRng->LineStartX;
        EndPt->y = pRng->LineStartY;
		if (nWordType)
			*nWordType = pRng->nMoodWordType; 
        delete pRng;
        bRet = true;
    }
    else {
		StartPt->x=0;
		StartPt->y=0;		
		EndPt->x= 0;		
		EndPt->y= 0;		
		*nWordType=0;
    }	
    return bRet;
}

// If we are doing spell check from Menu command we need to ignore the loss
// of focus so that we can correctly highlight any misspelled words.
void CHeaderField::OnKillFocus(CWnd* pNewWnd) 
{
	if ( (GetDlgCtrlID() != IDC_HDRFLD_EDIT3) ||
		 !pNewWnd || !pNewWnd->IsKindOf(RUNTIME_CLASS(CSpellDialog)) )
	{
		CEdit::OnKillFocus(pNewWnd);
	}
}

void CHeaderField::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int StartOffset, EndOffset;
	if(IsMissSpelledWord(point, StartOffset, EndOffset))
	{
		if (StartOffset >= 0 && EndOffset >=0)
		{
			SetSel(StartOffset,EndOffset);
			CString strText = GetText();
			CString strWord = strText.Mid(StartOffset, EndOffset-StartOffset);
			char SpellWord[256];
			strcpy (SpellWord,strWord.GetBuffer(0));
			CSpell SpellCheck;
			SpellCheck.Popup(this,SpellWord,point);
		}
	}
	else
		CEdit::OnRButtonDown(nFlags, point);
	//m_bSpellDirty = TRUE;

}

BOOL CHeaderField::IsMissSpelledWord (CPoint point, int& StartOffset, int& EndOffset)
{
   	int Height = GetMessageFont().CellHeight();
	if (m_pSpellDirtyPair )
	{
		HeaderDirtyPairPtr pDirtyPair = m_pSpellDirtyPair;
		while(pDirtyPair)
		{
		   CPoint TopLeftPt(pDirtyPair->LineStartX,pDirtyPair->LineStartY-Height);
		   CPoint BottomRightPt(pDirtyPair->LineEndX,pDirtyPair->LineEndY);
		   if ((point.x >= TopLeftPt.x) && (point.x <= BottomRightPt.x) &&
			   (point.y >= TopLeftPt.y) && (point.y <= BottomRightPt.y))
		   {
			   StartOffset = CharFromPos(CPoint(pDirtyPair->LineStartX,pDirtyPair->LineStartY));
			   EndOffset = CharFromPos(BottomRightPt);
			   return true;
		   }
		   pDirtyPair= pDirtyPair->next;
		}
	}
	return false;
}

void CHeaderField::FinishFilenameAutoComplete()
{
	int Index = LineFromChar();

	int LineOffset = LineIndex();

	char buf[256];
	char Partial[256];
	Partial[0] = 0;
	unsigned int Len;
	
	// Grab the line that the caret is in

	if ((Len = GetLine(Index, buf, sizeof(buf)-1)) <= 0 || !*buf)
		return;

	buf[Len] = 0;

	int SelStart, SelEnd;

	GetSel(SelStart, SelEnd);

	char* Start = buf + SelStart - LineOffset;

	// Find the beginning of the string if no text is selected
	if (SelStart == SelEnd)
	{
		while (Start > buf && Start[-1] != ',')
			Start--;
	}

	// Skip by leading space
	while (*Start == ' ')
		Start++;

	Len = strlen(Start);

	if (strchr(Start, '\\') == NULL)
		return;		// Don't start autocomp til they've done a c:\ or something.

	CStringList* Matches = NULL;

	AutoCompList* pACL = GetAutoCompList();
	if (pACL)
		Matches = pACL->FindListOfFilesStartingWith(Start);
	
	// Here we want to first check the history list for matches then we go on to the addressbook. 

	if (Matches && Matches->GetCount() != 0)
	{
		CString firstMatch = Matches->GetHead();
		char * ToMatch = DEBUG_NEW char[firstMatch.GetLength()+1];
		strcpy(ToMatch, firstMatch);

		char * floater = ToMatch;
		char * floater2 = Start;
		while (((*floater == tolower(*floater2))||(*floater == toupper(*floater2)))  && *floater2 != 0)
		{
			floater++;
			floater2++;
		}
		
		strncpy(Partial, floater, sizeof(Partial));
		delete [] ToMatch;

		if (Matches->GetCount() >= 2)
		{
			m_ACListBox->KillACListBox();
			m_ACListBox->DoACListBox();
			for (int i=0; i < Matches->GetCount(); i++)
			{
				CString temp = Matches->GetAt(Matches->FindIndex(i));

				char *tempChars = DEBUG_NEW char[temp.GetLength()+1];				
				strcpy(tempChars, temp);

				m_ACListBox->AddToACListBox(tempChars, 0);
				delete [] tempChars;
			}
			m_ACListBox->InitList();
		}
		else
			m_ACListBox->KillACListBox();

		delete Matches;
	}
	else 
		m_ACListBox->KillACListBox();

	if (Partial[0])
	{
		ReplaceSel(Partial);
		SetSel((int)(strlen(Partial)+SelStart), (int)SelStart);
	}

}

// OnChangeEdit:
// Called anytime the contents of one of our fields changes

void CHeaderView::OnChangeEdit( UINT nID )
{
	// set the modified flag of our document to TRUE
	GetDocument()->SetModifiedFlag();
	if (nID == IDC_HDRFLD_EDIT3 ) 
	{
		CWnd* pWnd = GetDlgItem(IDC_HDRFLD_EDIT3);
		if (pWnd)
		{
			((CHeaderField*)pWnd)->m_bMoodDirty = TRUE;
			((CHeaderField*)pWnd)->m_bSpellDirty = TRUE;
		}
	}

	if ( (nID == IDC_HDRFLD_EDIT1) || (nID == IDC_HDRFLD_EDIT4) || (nID == IDC_HDRFLD_EDIT5) )
	{
		CWnd* pWnd = GetDlgItem(nID);
		if (pWnd)
		{
			((CHeaderField*)pWnd)->m_bBPDirty = TRUE;
			((CHeaderField*)pWnd)->m_bBPSpeakDirty = TRUE;
			((CHeaderField*)pWnd)->m_bBPWarnDlgDirty = TRUE;
		}
	}
}


// OnScrollToShowCaret:
// Responds to a windows message registered by us, telling the HeaderView
// to scroll until caret is visible.

LONG CHeaderView::OnScrollToShowCaret( UINT uCmd, LONG lParam )
{
	ScrollToShowCaret();
	return 0;
}


void CHeaderView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	COleDataObject clipBoardData;
	VERIFY( clipBoardData.AttachClipboard() );

	if ( clipBoardData.IsDataAvailable( CF_HDROP ) ) {
		pCmdUI->Enable( TRUE );
		return;
	}
}


//////////////////////////////////////////////////////////////////////////////
// CHeaderView implimentation


// IsEmpty:
// Returns whether or not the specified field is empty

BOOL CHeaderView::IsEmpty
(
	int	header		// zero-based index of Header field
)
{
	CHeaderField* pField = NULL;
	POSITION pos = m_headerFields.GetHeadPosition();
	for ( int i = 0; i <= header; i++ ) {
		pField = m_headerFields.GetNext( pos );
	}

	ASSERT( pField != NULL );
	int lineLength = pField->LineLength( 0 );
	return (lineLength != 0);
}


// SaveToDoc:
// Updates the document with the value of each Header field

void CHeaderView::SaveToDoc()
{
	CCompMessageDoc* pDoc = (CCompMessageDoc*) GetDocument();
	ASSERT( pDoc != NULL );
	ASSERT( pDoc->IsKindOf(RUNTIME_CLASS(CCompMessageDoc)) );

	int nFields = m_headerFields.GetCount();
	for ( int i = 0; i < nFields; i++ ) {
		pDoc->m_Headers[i] = GetText( i );
	}
}


// GetText:
// Gets the contents of the specified Header field

CString CHeaderView::GetText
(
	int header		// zero-based index of Header field
)
{
	CHeaderField* pField = NULL;
	POSITION pos = m_headerFields.GetHeadPosition();
	for ( int i = 0; i <= header; i++ ) {
		pField = m_headerFields.GetNext( pos );
	}

	ASSERT( pField != NULL );
	CString theString = "";
	pField->GetWindowText( theString );
	return theString;
}


BOOL CHeaderView::GetText
(
	int			nHeader,		// (I) index of desired field
	CString&	labelText,		// (O) storage for the label text
	CString&	fieldText		// (O) storage for the field text
)
{
	// find the specified field
	CHeaderField* pField = NULL;
	POSITION pos = m_headerFields.GetHeadPosition();
	for ( int i = 0; i <= nHeader; i++ ) {
		pField = m_headerFields.GetNext( pos );
	}

	ASSERT( pField != NULL );
	BOOL fRet = FALSE;

	// got the field? Get the text.
	if ( pField ) {
		labelText = pField->GetLabelText();
		fieldText = pField->GetText();

		// the field label text may contain an ampersand for providing
		// the 'hotkey underscore'. we need to remove this.

		// lock down the CString buffer (so much for object orientation)
		LPTSTR pTStr = labelText.GetBuffer( 64 );

		// find the ampersand
		while ( (*pTStr != _T('&')) && (*pTStr != _T('\0')) ) {
			pTStr++;
		}

		// if we have one, overwright it with the rest of the string. this
		// code would be pretty sucky if we didn't know that the ampersand
		// could never be the last character!
		if ( *pTStr == _T('&') ) {
			LPTSTR pTStrNext = pTStr + 1;
			strcpy( pTStr, pTStrNext );
		}

		// unlock the CString buffer
		labelText.ReleaseBuffer();
		fRet = TRUE;
	}

	return fRet;	// hasty amoebas!
}


// SetText:
// Sets the contents of the specified Header field

void CHeaderView::SetText
(
	int header,		// zero-based index of Header field
	const char* szText
)
{
	CHeaderField* pField;
	POSITION pos = m_headerFields.GetHeadPosition();
	for ( int i = 0; i < NumHeaders; i++ ) 
	{
		pField = m_headerFields.GetNext( pos );
		if ( i == header )
		{
			pField->SetWindowText(szText);
			OnUpdateEdit( pField->GetDlgCtrlID() );
		}
	}
}


char CHeaderView::GetShortcutLetter(int nHeader)	//(i) index of desired field
{
	// find the specified field
	CHeaderField* pField = NULL;
	POSITION pos = m_headerFields.GetHeadPosition();
	for ( int i = 0; i <= nHeader; i++ )
		pField = m_headerFields.GetNext( pos );

	ASSERT( pField != NULL );

	// got the field? Get the label text.
	if ( pField ) 
	{
		CString labelText = pField->GetLabelText();

		int ampersand_idx = labelText.Find('&');
		if (-1 == ampersand_idx)
			return '\0';	// no shortcut key
		else if (ampersand_idx + 1 >= labelText.GetLength())
		{
			ASSERT(0);		// ampersand is last character?  oops.
			return '\0';
		}
		else
		{
			char shortcut = labelText[ampersand_idx + 1];
			shortcut = (char)toupper(shortcut);
			if (shortcut >= 'A' && shortcut <= 'Z')
				return shortcut;

			//
			// Making a very tenuous assumption here that shortcut
			// keys in all localized versions of Eudora resolve to
			// basic English letter.
			//
			ASSERT(0);
		}
	}

	return '\0';
}


BOOL CHeaderView::IsHeaderInvalid(int nHeader)		
{
	// find the specified field
	CHeaderField* pField = NULL;
	POSITION pos = m_headerFields.GetHeadPosition();
	BOOL bReturn = FALSE;
	for ( int i = 0; i <= nHeader; i++ ) 
	{
		pField = m_headerFields.GetNext( pos );
	}

	ASSERT( pField != NULL );

	// got the field? Get the text.
	if ( pField ) 
		bReturn = pField->IsInvalid();

	return bReturn;
		
}

//Check if the fields have changed 
int CHeaderView::OnCheckSpelling(BOOL bAutoCheck/*=FALSE*/)
{
	// find the specified field
	CHeaderField* pField = NULL;
	POSITION pos = m_headerFields.GetHeadPosition();
	if (pos)
	{
		for ( int i = IDC_HDRFLD_EDIT1; i <= IDC_HDRFLD_EDIT6; i++ ) 
		{
			pField = m_headerFields.GetNext( pos );
			if ( pField && pField->GetDlgCtrlID() == IDC_HDRFLD_EDIT3 )
			{
				return pField->OnCheckSpelling(bAutoCheck);
			}
		}
	}
	return 0;
}

BOOL CHeaderView::IsSelSubject()
{
	CHeaderField* pField = NULL;
	POSITION pos = m_headerFields.GetHeadPosition();

	if (pos)
	{
		for ( int i = IDC_HDRFLD_EDIT1; i <= IDC_HDRFLD_EDIT6; i++ ) 
		{
			pField = m_headerFields.GetNext( pos );
			if ((i==IDC_HDRFLD_EDIT3) && pField )
			{
				DWORD sel = pField->GetSel();
				if (sel == 0 || (HIWORD(sel)==LOWORD(sel)))
					return FALSE;
				else
					return TRUE;
			}
		}
	}

	return FALSE;
}

//Check if the fields have changed 
BOOL CHeaderView::IsHeaderMoodDirty()
{
	// find the specified field
	CHeaderField* pField = NULL;
	POSITION pos = m_headerFields.GetHeadPosition();
	BOOL bReturn = FALSE;
	if (pos)
	{
		for ( int i = IDC_HDRFLD_EDIT1; i <= IDC_HDRFLD_EDIT6; i++ ) 
		{
			pField = m_headerFields.GetNext( pos );
			if ( pField )
			{
				bReturn = pField->IsMoodDirty();
				if (bReturn) return bReturn;		
			}
		}
	}
	return 0;
}

//check if the header fields have changed 
BOOL CHeaderView::IsHeaderBPDirty()
{
	// find the specified field
	CHeaderField* pField = NULL;
	POSITION pos = m_headerFields.GetHeadPosition();
	BOOL bReturn = FALSE;
	if (pos)
	{
		for ( int i = IDC_HDRFLD_EDIT1; i <= IDC_HDRFLD_EDIT6; i++ ) 
		{
			pField = m_headerFields.GetNext( pos );
			if ( pField )
			{
				int ctrlID = pField->GetDlgCtrlID();
				if( (ctrlID == IDC_HDRFLD_EDIT1) || (ctrlID == IDC_HDRFLD_EDIT4) || (ctrlID == IDC_HDRFLD_EDIT5) ) {
					bReturn = pField->IsBPDirty();
					if (bReturn) 
						return bReturn;		
				}
			}
		}
	}
	return 0;
}

//check if the header fields have changed 
BOOL CHeaderView::IsHeaderBPSpeakDirty()
{
	// find the specified field
	CHeaderField* pField = NULL;
	POSITION pos = m_headerFields.GetHeadPosition();
	BOOL bReturn = FALSE;
	if (pos)
	{
		for ( int i = IDC_HDRFLD_EDIT1; i <= IDC_HDRFLD_EDIT6; i++ ) 
		{
			pField = m_headerFields.GetNext( pos );
			if ( pField )
			{
				int ctrlID = pField->GetDlgCtrlID();
				if( (ctrlID == IDC_HDRFLD_EDIT1) || (ctrlID == IDC_HDRFLD_EDIT4) || (ctrlID == IDC_HDRFLD_EDIT5) ) {
					bReturn = pField->IsBPSpeakDirty();
					if (bReturn) 
						return bReturn;		
				}
			}
		}
	}
	return 0;
}

//check if the header fields have changed 
BOOL CHeaderView::IsHeaderBPWarnDlgDirty()
{
	// find the specified field
	CHeaderField* pField = NULL;
	POSITION pos = m_headerFields.GetHeadPosition();
	BOOL bReturn = FALSE;
	if (pos)
	{
		for ( int i = IDC_HDRFLD_EDIT1; i <= IDC_HDRFLD_EDIT6; i++ ) 
		{
			pField = m_headerFields.GetNext( pos );
			if ( pField )
			{
				int ctrlID = pField->GetDlgCtrlID();
				if( (ctrlID == IDC_HDRFLD_EDIT1) || (ctrlID == IDC_HDRFLD_EDIT4) || (ctrlID == IDC_HDRFLD_EDIT5) ) {
					bReturn = pField->IsBPWarnDlgDirty();
					if (bReturn) 
						return bReturn;		
				}
			}
		}
	}
	return 0;
}

int  CHeaderView::ValidateFields(bool bMoodCheck, bool bBPWarning)
{
	// find the specified field
	CHeaderField* pField = NULL;
	POSITION pos = m_headerFields.GetHeadPosition();
	if (pos)
	{
		for ( int i = IDC_HDRFLD_EDIT1; i <= IDC_HDRFLD_EDIT6; i++ ) 
		{
			pField = m_headerFields.GetNext( pos );
			if ( pField )
			{
				if(bMoodCheck && (pField->m_pMoodDirtyPair || pField->IsMoodDirty()))
						pField->Invalidate();
				else if(bBPWarning && (pField->m_pBPDirtyPair || pField->IsBPDirty()))
						pField->Invalidate();
			}
		}
	}
	return 1;
}

void CHeaderView::SetHeaderMoodDirty(bool bStatus)
{
	// find the specified field
	CHeaderField* pField = NULL;
	POSITION pos = m_headerFields.GetHeadPosition();
	if (pos)
	{
		for ( int i = IDC_HDRFLD_EDIT1; i <= IDC_HDRFLD_EDIT6; i++ ) 
		{
			pField = m_headerFields.GetNext( pos );
			if ( pField )
			{
				pField->SetMoodDirty(bStatus);
			}
		}
	}
}

void CHeaderView::SetHeaderBPDirty(bool bStatus)
{
	// find the specified field
	CHeaderField* pField = NULL;
	POSITION pos = m_headerFields.GetHeadPosition();
	if (pos)
	{
		for ( int i = IDC_HDRFLD_EDIT1; i <= IDC_HDRFLD_EDIT6; i++ ) 
		{
			pField = m_headerFields.GetNext( pos );
			if ( pField )
			{
				pField->SetBPDirty(bStatus);
			}
		}
	}
}

void CHeaderView::SetHeaderBPSpeakDirty(bool bStatus)
{
	// find the specified field
	CHeaderField* pField = NULL;
	POSITION pos = m_headerFields.GetHeadPosition();
	if (pos)
	{
		for ( int i = IDC_HDRFLD_EDIT1; i <= IDC_HDRFLD_EDIT6; i++ ) 
		{
			pField = m_headerFields.GetNext( pos );
			if ( pField )
			{
				pField->SetBPSpeakDirty(bStatus);
			}
		}
	}
}


void CHeaderView::SetHeaderBPWarnDlgDirty(bool bStatus)
{
	// find the specified field
	CHeaderField* pField = NULL;
	POSITION pos = m_headerFields.GetHeadPosition();
	if (pos)
	{
		for ( int i = IDC_HDRFLD_EDIT1; i <= IDC_HDRFLD_EDIT6; i++ ) 
		{
			pField = m_headerFields.GetNext( pos );
			if ( pField )
			{
				pField->SetBPWarnDlgDirty(bStatus);
			}
		}
	}
}

int CHeaderView::AddMoodText(CMoodWatch *pMoodWatch, int nHeaderField)
{
	if(pMoodWatch)
	{
		CHeaderField* pField = NULL;
		POSITION pos = m_headerFields.GetHeadPosition();
		for(int j =0; j<=nHeaderField;j++)
		{
			pField = m_headerFields.GetNext( pos );
		}
		if(pField)
		{
			pField->AddFieldMoodText(pMoodWatch);
		}
	}
	return 1;
}

bool CHeaderView::DoBPCheck(CStringList * pAddressList /* = NULL */)
{
		CHeaderField* pField = NULL;
		bool bBPWarning = false;
		POSITION pos = m_headerFields.GetHeadPosition();
		for(int j =0; j<MAXHEADERS; j++)
		{
			pField = m_headerFields.GetNext( pos );
			if(pField)
			{
				int ctrlID = pField->GetDlgCtrlID();
				if( (ctrlID == IDC_HDRFLD_EDIT1) || (ctrlID == IDC_HDRFLD_EDIT4) || (ctrlID == IDC_HDRFLD_EDIT5) )
					if(pField->DoBPCheck(pAddressList))
						bBPWarning = true;		
			}
		}
	return bBPWarning;
}

// GetHeaderCtrl:
// Returns a pointer to the specified CHeaderField object.

CHeaderField* CHeaderView::GetHeaderCtrl( int nField )
{
	// our return value (presume failure)
	CHeaderField* pField = NULL;

	// make sure caller is in the ballpark
	if ( nField >= 0 && nField < MAXHEADERS ) {
		
		// find fields location in the list
		POSITION pos = m_headerFields.FindIndex( nField );

		if ( pos ) {
			
			// get a pointer to the field
			pField = m_headerFields.GetAt( pos );
		}
	}

	// most likely indicates a bad 'nField'
	ASSERT( pField );
	return pField;
}

void CHeaderView::SetFocusToHeader( int nField )
{
    TRACE1("CHeaderView::SetFocusToHeader(%d)\n", nField);

    // make sure caller is in the ballpark
    ASSERT( nField >= 0 && nField < MAXHEADERS ); 
    // find fields location in the list
    POSITION pos = m_headerFields.FindIndex( nField );
    if ( pos )
    {
        // get a pointer to the field
        CHeaderField* pField = m_headerFields.GetAt( pos );
        ASSERT(pField != NULL);
        ASSERT(pField->IsKindOf(RUNTIME_CLASS(CHeaderField)));

        //
        // 'm_hWndFocus' is a undocumented, protected member variable
        // of CFormView which stores the HWND of the last known
        // control which had the focus.  Darn the torpedoes and overwrite
        // it here so that we can force the focus to the control that
        // we want.
        //
        m_hWndFocus = pField->GetSafeHwnd();

        // make sure the header view is the active pane
        ((CSplitterWnd*)GetParent())->SetActivePane(0,0);
    }
}


BOOL CHeaderView::SetFocusToHeaderWithShortcut( int nChar )
{
    TRACE1("CHeaderView::SetFocusToHeaderWithShortcut(%c)\n", nChar);
    ASSERT(nChar != '\0');

    // see if we are popping-up a pooter
    if ( ProcessPooters( nChar ) )
        return TRUE;

    ASSERT(CHeaderView::m_fShortcutTableInitialized);
    for (int i = 0; i < MAXHEADERS; i++) {
        if (m_ShortcutTable[i] == nChar) {
            SetFocusToHeader(i);
            return TRUE;
        }
    }

    return FALSE;    // no matching shortcut
}


inline BOOL CHeaderView::ProcessPooters( int nVirtKey )
{
    if ( (nVirtKey == 'R' || nVirtKey == 'r') && !GetIniShort(IDS_INI_EDIT_ALL_HEADERS) ) {
        OnFromPooter();
        return TRUE;
    }

    return FALSE;
}
BOOL CHeaderView::DoFindFirst(const char* szSearch, BOOL bMatchCase, BOOL bWholeWord, BOOL bSelect)
{
	CString search_str(szSearch);
	BOOL bFound = false;
	int startIndex = 0;
	int endIndex = 0;
	//
	// Kick off the search, first looking through each header in turn.
	// If there is no match in a header, then continue the search in
	// the message body.
	//
	for (int i = 0; i < MAXHEADERS; i++)
	{
		//
		// Get the text to scan in this pass, searching from the last
		// find position.
		//
		CString msg_text;

		CHeaderField *phdrField =  GetHeaderCtrl(i);

		msg_text = phdrField->GetText();
		

		const char* p_msgtext = msg_text;

		if (! bMatchCase)
		{
			msg_text.MakeLower();
			search_str.MakeLower();
		}
		
		//
		// Do the search...
		//
		char* p_match = strstr(p_msgtext, search_str);
		if (p_match)
		{
			//
			// Got a hit.
			//
			bFound = TRUE;
			startIndex = p_match - p_msgtext;
			endIndex = startIndex + search_str.GetLength();

			//
			// Got a hit in the header, so we need to grab the
			// proper edit control and tell the edit control
			// exactly where to do the selection.
			//
			CHeaderField* p_editctrl = GetHeaderCtrl(i);
			if (p_editctrl != NULL)
			{

				//
				// The following call is a very special hack
				// to fool the CFormView-derived object to set
				// the focus where we want it and not where it
				// thinks it wants it.
				// 
				SetFocusToHeader(i);
				// set selection to match

				// Clear all selection
				for(int j = 0; j <= MAXHEADERS; j++)
				{
					CEdit* p_edit = GetHeaderCtrl(i);
					if (p_edit != NULL)
					{
						ASSERT(p_edit->IsKindOf(RUNTIME_CLASS(CEdit)));
						p_edit->SetSel(0, 0);							// deselect all
					}
				}
				p_editctrl->SetFocus();
				p_editctrl->SetSel(startIndex, endIndex, FALSE);


			}
			return TRUE;
			
		}

	}
	return FALSE;
}	


BOOL CHeaderView::DoFindNext(const char* szSearch, BOOL bMatchCase, BOOL bWholeWord, BOOL bSelect)
{
	int curHead = GetCurrentHeader();
	CString search_str(szSearch);
	BOOL bFound = false;
	int startIndex = 0;
	int endIndex = 0;
	int dummy_sel_x = 0;
	int dummy_sel_y = 0;
	CString msg_text;

	for (int i = curHead; i <= MAXHEADERS; i++)
	{
		CHeaderField *phdrField =  GetHeaderCtrl(i);

		if(!phdrField)
			return FALSE;

		msg_text = phdrField->GetText();
		phdrField->GetSel(dummy_sel_x, dummy_sel_y);

		const char* p_msgtext = msg_text;

		ASSERT((unsigned)dummy_sel_y <= strlen(p_msgtext));

		if ((unsigned)dummy_sel_y > strlen(p_msgtext))
			dummy_sel_y = 0;

		p_msgtext += dummy_sel_y;

		if (! bMatchCase)
		{
			msg_text.MakeLower();
			search_str.MakeLower();
		}
		
		//
		// Do the search...
		//
		char* p_match = strstr(p_msgtext, search_str);
		if (p_match)
		{
			//
			// Got a hit.
			//
			bFound = TRUE;
			startIndex = p_match - msg_text;
			endIndex = startIndex + search_str.GetLength();

			//
			// Got a hit in the header, so we need to grab the
			// proper edit control and tell the edit control
			// exactly where to do the selection.
			//
			CHeaderField* p_editctrl = GetHeaderCtrl(i);
			if (p_editctrl != NULL)
			{

				//
				// The following call is a very special hack
				// to fool the CFormView-derived object to set
				// the focus where we want it and not where it
				// thinks it wants it.
				// 
				SetFocusToHeader(i);

				// set selection to match

				// Clear all selection
				for(int j = 0; j <= MAXHEADERS; j++)
				{
					CEdit* p_edit = GetHeaderCtrl(i);
					if (p_edit != NULL)
					{
						ASSERT(p_edit->IsKindOf(RUNTIME_CLASS(CEdit)));
						p_edit->SetSel(0, 0);							// deselect all
					}
				}
				p_editctrl->SetFocus();
				p_editctrl->SetSel(startIndex, endIndex, FALSE);


			}
			return TRUE;
			
		}

	}

	for (i = 0; i < curHead; i++)
	{

		CHeaderField *phdrField =  GetHeaderCtrl(i);

		msg_text = phdrField->GetText();

		const char* p_msgtext = msg_text;

		if (! bMatchCase)
		{
			msg_text.MakeLower();
			search_str.MakeLower();
		}
		
		//
		// Do the search...
		//
		char* p_match = strstr(p_msgtext, search_str);
		if (p_match)
		{
			//
			// Got a hit.
			//
			bFound = TRUE;
			startIndex = p_match - msg_text;
			endIndex = startIndex + search_str.GetLength();

			//
			// Got a hit in the header, so we need to grab the
			// proper edit control and tell the edit control
			// exactly where to do the selection.
			//
			CHeaderField* p_editctrl = GetHeaderCtrl(i);
			if (p_editctrl != NULL)
			{

				//
				// The following call is a very special hack
				// to fool the CFormView-derived object to set
				// the focus where we want it and not where it
				// thinks it wants it.
				// 
				SetFocusToHeader(i);
				// set selection to match

				// Clear all selection
				for(int j = 0; j <= MAXHEADERS; j++)
				{
					CEdit* p_edit = GetHeaderCtrl(i);
					if (p_edit != NULL)
					{
						ASSERT(p_edit->IsKindOf(RUNTIME_CLASS(CEdit)));
						p_edit->SetSel(0, 0);							// deselect all
					}
				}
				p_editctrl->SetFocus();
				p_editctrl->SetSel(startIndex, endIndex, FALSE);


			}
			return TRUE;
			
		}

	}

	return FALSE;
}


// GetDocument:
// Returns a pointer to our associated document object
//
// would have just defined this in the header, but our 'include' scheme is so
// messed up right now, that it was just more trouble than it was worth.

CCompMessageDoc* CHeaderView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCompMessageDoc)));
	return (CCompMessageDoc*) m_pDocument;
}


// SetHeaderFromDoc:
// Initializes the Header view from data contained in the document

void CHeaderView::SetHeaderFromDoc()
{
	CCompMessageDoc* pDoc = (CCompMessageDoc*) GetDocument();
	CSummary* Sum = pDoc->m_Sum;
	ASSERT( pDoc != NULL );
	ASSERT( pDoc->IsKindOf(RUNTIME_CLASS(CCompMessageDoc)) );
	const BOOL bEditAllHeaders = GetIniShort(IDS_INI_EDIT_ALL_HEADERS);

	CHeaderField* pField;
	POSITION pos = m_headerFields.GetHeadPosition();
	for ( int i = 0; i < NumHeaders; i++ ) {
		pField = m_headerFields.GetNext( pos );
		pField->SetWindowText(pDoc->m_Headers[i]);
		//Set the invalid flag as well, i.e, if a field is invalid and unable to send a message because of it
		pField->SetAsInvalid(pDoc->m_HeadersInvalidFlag[i]);
		if (Sum->CantEdit() || ((i == HEADER_FROM || i == HEADER_ATTACHMENTS) && !bEditAllHeaders))
		{
			pField->SetReadOnly( TRUE );
		}

		OnUpdateEdit( pField->GetDlgCtrlID() );
	}
}


int CHeaderView::GetNumberOfAddrInHeader()
{
	CHeaderField* pField;
	POSITION pos = m_headerFields.GetHeadPosition();
	int numAddr = 0;
	for ( int i = 0; i < NumHeaders; i++ ) {

		pField = m_headerFields.GetNext( pos );
		
		int ctrlID = pField->GetDlgCtrlID();
		int num = 0;
		if( (ctrlID == IDC_HDRFLD_EDIT1) || (ctrlID == IDC_HDRFLD_EDIT4) || (ctrlID == IDC_HDRFLD_EDIT5) ) {
			int TextLen = pField->GetWindowTextLength();
			char* Text = DEBUG_NEW char[TextLen + 1];
			int NumCopied = pField->GetWindowText(Text, TextLen + 1);
			Text[NumCopied] = 0;
		
			char* ExpandedText = ExpandAliases(Text, TRUE, FALSE);

			bool done = false;
			char* end = NULL;
			char *floater = ExpandedText;
			while (!done && floater < (ExpandedText + strlen(ExpandedText)) ) {
				end = FindAddressEnd(floater, FALSE, FALSE);
				int length = end? end - floater : 0;

				if (length == 0) {
					done = true;  
					break;
				} else {
					num++;
					floater = end+1;
				}
			}

			delete [] ExpandedText;
		}


		numAddr += num;
	}
	return numAddr;
}

//////////////////////////////////////////////////////////////////////////////
// CHeaderField

IMPLEMENT_DYNCREATE(CHeaderField, AutoCompCEdit)

BEGIN_MESSAGE_MAP(CHeaderField, AutoCompCEdit)
	//{{AFX_MSG_MAP(CHeaderField)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_SYSKEYDOWN()
	ON_COMMAND(ID_EDIT_CHECKSPELLING, OnCheckSpelling)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_KILLFOCUS()
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CHeaderField:
// Default constructor. Objects built using this constructor must be
// "attached" before use by calling CHeaderField::Attach.

CHeaderField::CHeaderField()
{
	m_pWndParent = NULL;
	m_pwndLabel = DEBUG_NEW CStatic;
	m_numLines = 1;
	m_bInvalid = FALSE;
	m_beenExpanded = false;
	m_ACListBox = NULL;
	m_DoDropDown = false;
	m_bMoodDirty = FALSE;
	m_bSpellDirty = TRUE;
	m_bBPDirty = TRUE;
	m_bBPSpeakDirty = TRUE;
	m_bBPWarnDlgDirty = TRUE;
	m_pMoodDirtyPair = NULL;
	m_pSpellDirtyPair = NULL;
	m_pBPDirtyPair = NULL;
	PGS_InitDrawGlobals();
	m_bServicingSpellTimer = false;
}

// ~CHeaderField()
// Destructor for CHeaderField objects.

CHeaderField::~CHeaderField()
{
	delete m_ACListBox;
	m_ACListBox = NULL;
	ClearAllDirtyPairs();
	delete m_pwndLabel;
}

void CHeaderField::SetExpanded(bool value)
{
	if (value)
		m_beenExpanded = true;
	else
		m_beenExpanded = false;
}

void CHeaderField::GetACListWindowRect(LPRECT rct)
{//TODO: Make this return a rectangle around the text to be completed so the list of choices is undet eh thing being typed.
	GetWindowRect(rct);	
}

void CHeaderField::OnACListDisplayed()
{
	if (m_ACListBox)
	{
		m_ACListBox->SetListToControl(TRUE);
//		if (ExactMatch > 0)
//		{
//			m_ACListBox->SetCheck(ExactMatch);
//			m_ACListBox->SetControlToItem(ExactMatch);
//		}
//		else
//		{
//			m_ACListBox->SetCheck(0);
//			m_ACListBox->SetControlToItem(0);
//		}
	}
}

bool CHeaderField::GetExpanded()
{
	return m_beenExpanded;
}

void CHeaderField::OnACListSelectionMade()
{
	if (m_ACListBox)
	{
		m_ACListBox->SetControlToItem(-1, false);
	}

	int Start,End;
	GetSel(Start, End);
	SetSel((Start > End) ? Start :End , (Start > End) ? Start :End );	
}

//////////////////////////////////////////////////////////////////////////////
// CHeaderField operations

// Attach:
// Subclasses a Static/Edit control combo. Used when the object has been
// built using the default constructor.

BOOL CHeaderField::Attach
(
    CHeaderView*    pWndParent,    // our parent window (CHeaderView)
    UINT            idcStatic,     // id of Static control to subclass
    UINT            idcEdit        // id of Edit control to subclass
)
{
    BOOL fRet = FALSE;    // return value (assume failure)

    // don't support re-attachment
    ASSERT( m_pWndParent == NULL );
    ASSERT( pWndParent != NULL );
    m_pWndParent = pWndParent;

    long numPersonae = g_Personalities.GetPersonaCount();
    if ( (idcStatic == IDC_HDRFLD_LABEL2) && (numPersonae > 1) ) {

        // the "From" field is a "pooter" button
        delete m_pwndLabel;
        m_pwndLabel = DEBUG_NEW PooterButton;
    }

    // subclass our associated label (static control)
    fRet = m_pwndLabel->SubclassDlgItem( idcStatic, m_pWndParent );
    if ( fRet ) {
                
        // subclass the edit
        fRet = SubclassDlgItem( idcEdit, m_pWndParent );
    }

    if ( fRet ) {
                
        // set the field font
        SetFont( &CHeaderView::GetEditFont() );

        // set the field label font
        m_pwndLabel->SetFont( &CHeaderView::GetLabelFont() );
    }
        
    return fRet;
}


// Noodge:
// Here it is. The butt-stoopid "Noodge" method. It don't know how to move
// on the diagonal, just up, down, right, left. Pretty lame stuff.

void CHeaderField::Noodge
(
	Direction	ndgDir,		// direction to nudge field
	int			ndgBy		// amount to nudge by (logical units)
)
{
	ASSERT(m_pWndParent != NULL);
	ASSERT(m_pwndLabel->GetParent());

	// get the rect of the label
	CRect rcStatic;
	m_pwndLabel->GetWindowRect( &rcStatic );

	// get the rect of the Edit control
	CRect rcEdit;
	GetWindowRect( &rcEdit );

	// convert the 'rects' to client coordinates
	GetParent()->ScreenToClient( &rcStatic );
	GetParent()->ScreenToClient( &rcEdit );

	// adjust the control rects for the move we want to make
	switch ( ndgDir ) {
		case CHeaderField::NdgUp:
			rcStatic.OffsetRect( 0, -ndgBy );
			rcEdit.OffsetRect( 0, -ndgBy );
			break;

		case CHeaderField::NdgDown:
			rcStatic.OffsetRect( 0, ndgBy );
			rcEdit.OffsetRect( 0, ndgBy );
			break;

		case CHeaderField::NdgLeft:
			rcStatic.OffsetRect( -ndgBy, 0 );
			rcEdit.OffsetRect( -ndgBy, 0 );
			break;

		case CHeaderField::NdgRight:
			rcStatic.OffsetRect( ndgBy, 0 );
			rcEdit.OffsetRect( ndgBy, 0 );
			break;

		default:
			TRACE( "Bad Stuff is happening in 'CHeaderField::Noodge'!\n" );
			ASSERT(0);	// we've been passed a bogus 'ndgDir' value
	}

	// reposition the controls
	m_pwndLabel->MoveWindow( &rcStatic );
	MoveWindow( &rcEdit );
}


// Grow:
// Grows the edit control by the specified number of lines. Line height is
// determined by the characteristics of the current font.

void CHeaderField::Grow( int nLines )
{
	// grow our edit control by nLines
	CRect theRect;
	GetWindowRect( &theRect );
	GetParent()->ScreenToClient( &theRect );
	theRect.bottom += LineHeight() * nLines;
	MoveWindow( &theRect );
}


// Shrink:
// Shrinks the edit control by the specified number of lines.
//
// neither Grow(), nor this routine are truly robust. They could both be
// doing some bounds checking 'n' such, but it really isn't needed for the
// header stuff, so we'll keep it simple and move on.

void CHeaderField::Shrink( int nLines )
{
	// shrink our edit control by nLines
	CRect theRect;
	GetWindowRect( &theRect );
	GetParent()->ScreenToClient( &theRect );
	theRect.bottom -= LineHeight() * nLines;
	MoveWindow( &theRect );
	if(GetDlgCtrlID() == IDC_HDRFLD_EDIT3 && !GetIniShort( IDS_INI_INTERACTIVE_SPELL_CHECK )
		&&!m_bServicingSpellTimer)
		DoSpellCheck();
	m_bMoodDirty= true;
	m_bBPDirty= true;
	m_bBPSpeakDirty = true;
	m_bBPWarnDlgDirty = true;
}


// LineHeight:
// Returns the height of a line in logical units. This value will be in pixels
// for MM_TEXT mapping mode (the default).

int CHeaderField::LineHeight()
{
//	return MessageFont.CellHeight();
	return CHeaderView::GetEditFont().CellHeight();
}


void CHeaderView::OnSize(UINT nType, int cx, int cy) 
{
	fSizing = TRUE;
	
	// call the base implimentation
	CFormView::OnSize(nType, cx, cy);
	
	// get our new client rect
	CRect clientRect;
	GetClientRect( &clientRect );

	// resize all the fields to the 'right' dimension of the client rect,
	// minus a little fudge.
	CRect fieldRect;
	int fudgePixels = 20;

	// get the first field from our list
	POSITION pos = m_headerFields.GetHeadPosition();

	CHeaderField* pField = NULL;
	while ( pos ) {
		
		// in spite of the method name, this starts at the head position
		pField = m_headerFields.GetNext( pos );

		// if this gets called before OnInitialUpdate, fields are invalid
		if ( !pField->GetSafeHwnd() ) {
			break;
		}

		// resize the field
		pField->GetWindowRect( &fieldRect );
		ScreenToClient( &fieldRect );
		fieldRect.right = clientRect.right - fudgePixels;
		pField->MoveWindow( &fieldRect );
		OnUpdateEdit( pField->GetDlgCtrlID() );
	}

	fSizing = FALSE;
}


BOOL CHeaderView::ScrollToShowCaret()
{
	int CellHeight = s_editFont.CellHeight();
	POINT point(GetCaretPos());
	RECT CtrlRect, rect;
	int DeltaY = 0;

	CWnd* Ctrl = GetFocus();
	Ctrl->GetWindowRect(&CtrlRect);
	GetWindowRect(&rect);
	int WindowLine = CtrlRect.top + point.y;
	if (WindowLine < rect.top)
		DeltaY = WindowLine - rect.top;
	else if (WindowLine > rect.bottom - CellHeight)
		DeltaY = WindowLine - (rect.bottom - CellHeight);
		
	if (DeltaY)
	{
//		int Round = (DeltaY > 0? 1 : -1);
//		DeltaY /= CellHeight;
//		DeltaY += Round;
		return (OnScroll(MAKEWORD(-1, SB_THUMBTRACK), GetScrollPos(SB_VERT) + DeltaY));
	}

	return (FALSE);
}


CString CHeaderField::GetText()
{
	CString theText;
	GetWindowText( theText );
	return theText;
}


CString CHeaderField::GetLabelText()
{
	CString theText;
	m_pwndLabel->GetWindowText( theText );
	return theText;
}

void CHeaderField::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	m_bSpellDirty = TRUE;
	m_bMoodDirty = TRUE;
	m_bBPDirty = TRUE;
	m_bBPSpeakDirty = TRUE;
	m_bBPWarnDlgDirty = TRUE;
	if (nChar == VK_BACK || nChar == VK_TAB || nChar == VK_ESCAPE)
	{
		if (m_ACListBox) // And kill timer
			m_ACListBox->KillACListBox();

		AutoCompCEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}
	else if (nChar == ',')
	{
		if (m_ACListBox && m_ACListBox->ContainsItems()) // And kill timer
		m_ACListBox->SelectionMade();	
		AutoCompCEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	AutoCompCEdit::OnChar(nChar, nRepCnt, nFlags);

	const bool bDoAutoComplete = UsingFullFeatureSet() && GetIniShort(IDS_INI_DO_NN_AUTOCOMPLETE) && m_ACListBox;
	if (bDoAutoComplete)
	{
		if (nChar == ',')
		{
			CHeaderField* AttachField = ((CHeaderView*)GetParent())->GetHeaderCtrl(HEADER_ATTACHMENTS);
			if (AttachField != this)
			{
				m_ACListBox->SelectionMade();
			}
		}
		else if (nChar == '\\')
		{
			CHeaderField* AttachField = ((CHeaderView*)GetParent())->GetHeaderCtrl(HEADER_ATTACHMENTS);
			if (AttachField == this)
			{
				m_ACListBox->SelectionMade();
			}
		}
	}

	if (m_ACListBox)
		m_ACListBox->KillACListBox();

	//Set Timer
	if (bDoAutoComplete)
		FinishNNAutoComplete();
}

// OnKeyDown:
// Handles the WM_KEYDOWN Windows message
//
// Right now, we just use this for navigation, allowing the user to TAB from
// the last header field into the message body, and SHIFT-TAB backwards
// through the header fields.
// JES: Now we also use it for autocompletion of nicknames in the to field.

void CHeaderField::OnKeyDown
(
	UINT	nChar,		// vk code of the key that was pressed
	UINT	nRepCnt,	// repeat count for keyboard auto-repeat
	UINT	nFlags		// varies: OEM scan code, extended key code, etc...
)
{
	int ctrlID = GetDlgCtrlID();
	BOOL fScrollToShowCaret = TRUE;
	CSplitterWnd*	pSplitter;

	if (nChar == VK_DELETE)
    {
		m_bMoodDirty = TRUE;
		m_bSpellDirty = TRUE;
		m_bBPDirty = TRUE;
		m_bBPSpeakDirty = TRUE;
		m_bBPWarnDlgDirty = TRUE;
	}
	if ( m_ACListBox && m_ACListBox->ContainsItems() && (nChar == VK_UP || nChar == VK_DOWN || nChar == VK_DELETE || nChar == 188) )
	{
		short ControlKey = GetKeyState(VK_CONTROL);
		if (!(ControlKey & 0xF000))
		{
			if (nChar == VK_UP)
				m_ACListBox->OnArrowKey(true, true);
			else if (nChar == VK_DOWN)
				m_ACListBox->OnArrowKey(false, true);
			else if (nChar == VK_DELETE)
			{
				m_ACListBox->OnDeleteKey();
				m_bMoodDirty = TRUE;
				m_bSpellDirty = TRUE;
				m_bBPDirty = TRUE;
				m_bBPSpeakDirty = TRUE;
				m_bBPWarnDlgDirty = TRUE;
			}
		}
		else
		{
			if (nChar == VK_DOWN || nChar == 188 || nChar == VK_UP)
				if (m_ACListBox)
					m_ACListBox->TogglePoppedUpState();
		}
	}
	
//	if ( ( (nChar == 188 && (ctrlID == IDC_HDRFLD_EDIT1 || ctrlID == IDC_HDRFLD_EDIT4 ||
//		ctrlID == IDC_HDRFLD_EDIT5)) || (nChar == 220 && ctrlID == IDC_HDRFLD_EDIT6) ) &&
//		(m_ACListBox && m_ACListBox->ContainsItems() == TRUE))	
//	{		// You're in the To: Cc: or Bcc: Fields and you type a comma, or your in the attachments field and you type a backslash
//		m_ACListBox->SelectionMade();
//	}
	else if ( nChar == VK_TAB )
	{
		pSplitter = (CSplitterWnd*) m_pWndParent->GetParent();

		if ( AsyncShiftDown() ) 
		{
			if ( ctrlID == IDC_HDRFLD_EDIT1 )
			{
				// set active splitter pane to message body
				pSplitter->SetActivePane( 1, 0 );
				
				// don't scroll the view when we are switching to the
				// message body.
				fScrollToShowCaret = FALSE;
			}
			else
			{
				// go to the previous control
				m_pWndParent->PostMessage( WM_NEXTDLGCTL, 1, 0 );
			}
		}
		else
		{
			if ((ctrlID == IDC_HDRFLD_EDIT5 && !GetIniShort(IDS_INI_EDIT_ALL_HEADERS)) ||
				 ctrlID == IDC_HDRFLD_EDIT6)
			{
				pSplitter->SetActivePane( 1, 0 );
				
				// don't scroll the view when we are switching to the
				// message body.
				fScrollToShowCaret = FALSE;
			}
			else 
				m_pWndParent->PostMessage( WM_NEXTDLGCTL, 0, 0 );
		}
	}
	else
	{
		// pass everything else to our base class
		AutoCompCEdit::OnKeyDown( nChar, nRepCnt, nFlags );
	}

	if ( fScrollToShowCaret )
	{
		// send HeaderView our registered 'scroll-to-show-caret' message
		m_pWndParent->PostMessage( wmScrollToShowCaret, 0, 0 );
	}
}


void CHeaderField::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // Turn message-switching hotkeys into ID_PREVIOUS_MESSAGE/ID_NEXT_MESSAGE
    if (nChar == VK_LEFT || nChar == VK_UP || nChar == VK_RIGHT || nChar == VK_DOWN) {
        BOOL CtrlDown = (GetKeyState(VK_CONTROL) < 0);
        BOOL AltArrows = GetIniShort(IDS_INI_ALT_ARROWS) != 0;
                
        if (AltArrows && !CtrlDown) {
            GetParentFrame()->SendMessage(WM_COMMAND, (nChar == VK_LEFT || nChar == VK_UP)?
                                          ID_PREVIOUS_MESSAGE : ID_NEXT_MESSAGE);
            return;
        }
    }

    AutoCompCEdit::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CHeaderField::OnCheckSpelling()
{
	OnCheckSpelling(FALSE);
}



int CHeaderField::OnCheckSpelling(BOOL bAutoCheck/*=FALSE*/)
{
	// Shareware: Reduced feature mode does not allow spell checking
	if (UsingFullFeatureSet())
	{
		// FULL FEATURE mode
		CSpell Spell(TRUE);

		if (GetDlgCtrlID() == IDC_HDRFLD_EDIT3 ) 
		{
			int bRetVal = Spell.Check(this,0,bAutoCheck);
			((CHeaderView*)GetParent())->SetJustQueue(Spell.m_bJustQueue);
			m_bSpellDirty = TRUE;
			return bRetVal;
		}
	}
	return 0;
}

int CHeaderField::CheckSpelling()
{
	int ret = 0;

	// Shareware: Reduced feature mode does not allow spell checking
	if (UsingFullFeatureSet())
	{
		// FULL FEATURE mode
		CSpell Spell(TRUE);

		
		int stCh, endCh;
		this->GetSel(stCh, endCh);

		// See if there's a selection made?
		if (stCh != endCh)
			ret = Spell.Check(this);
		// Otherwise check subject + body
		else
		{
			QCProtocol* pProtocol = QCProtocol::QueryProtocol( QCP_SPELL, m_pWndParent->GetDocument()->GetView() );

			if( pProtocol )
			{
				ret = pProtocol->CheckSpelling( FALSE );
			}
		}
	}
	
	return ret;
}


LRESULT CHeaderField::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result=0;
	if (GetDlgCtrlID() == IDC_HDRFLD_EDIT6)
	{
		switch (message)
		{
		case WM_KEYDOWN:
		case WM_CHAR:
			// Backspace and Delete keys are allowed for Attachments header
			// Cuz of scan codes the period key is coming thru in WM_CHAR as VK_DELETE
			// so separate the sucker out, so we don't allow the placement of a period
			// in that field the real delete key doesn't send WM_CHAR anyhow
			// Fixes VI:4792 - jm
			if ((message == WM_KEYDOWN && wParam != VK_BACK && wParam != VK_DELETE)
				|| (message == WM_CHAR && wParam != VK_BACK))
				break;
			// Fall through
			
		case WM_CUT:
		case WM_CLEAR:
			// Do normal stuff when EditAllHeaders is on
			if (GetIniShort(IDS_INI_EDIT_ALL_HEADERS))
				break;

			SetReadOnly(FALSE);
			SelectAttachments();
			Result = AutoCompCEdit::WindowProc(message, wParam, lParam);
			SetReadOnly(TRUE);
			// May wind up with text left, so make sure selection is ok
			SelectAttachments();

			return (Result);
			break;

		case WM_LBUTTONUP :
			// Do normal stuff when EditAllHeaders is on
			if (GetIniShort(IDS_INI_EDIT_ALL_HEADERS))
				break;

			SelectAttachments();
			break;

		case WM_LBUTTONDBLCLK :
			GetParent()->SendMessage(WM_COMMAND, ID_FILE_OPEN_ATTACHMENT, 0);
			break;

		}
	}
	/*if (GetDlgCtrlID() == IDC_HDRFLD_EDIT3)
	{
		switch (message)
		{
			case WM_CUT:
			case WM_CLEAR:
				m_bSpellDirty = TRUE; 
		}
	}*/

	return (AutoCompCEdit::WindowProc(message, wParam, lParam));
}


BOOL CHeaderField::PreTranslateMessage(MSG* pMsg)
{
    if ( pMsg->message == WM_SYSKEYDOWN ) {
        int nVirtKey = (int) pMsg->wParam;
        UINT nFlags = HIWORD(pMsg->lParam);    // low word is repeat count

        if ( (nFlags & 0xE000) == 0x2000 ) {
            if ( nVirtKey != VK_MENU )
                if ( m_pWndParent->ProcessPooters( nVirtKey ) )
                    return TRUE;
        }
    }

//
// The following is for the autocompleter listbox
//
    if ( m_ACListBox && pMsg->message == WM_KEYDOWN &&
         (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_TAB || pMsg->wParam == VK_RIGHT) &&
		 m_ACListBox->ContainsItems()) {       

		BOOL bReturnValue = FALSE;

		if (pMsg->wParam == VK_RETURN)
		{
			bReturnValue = TRUE;
			m_ACListBox->ForceSelection();
		}

		m_ACListBox->SelectionMade();

		return bReturnValue;
    }
    else if (m_ACListBox && m_ACListBox->ShouldShow() &&
             (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_RBUTTONDOWN ||
              (pMsg->message == WM_KEYDOWN && pMsg->wParam != VK_UP && pMsg->wParam != VK_DOWN
               && pMsg->wParam != VK_SHIFT && pMsg->wParam != VK_CONTROL && pMsg->wParam != 220
               && pMsg->wParam != VK_DELETE && pMsg->wParam != 188 ) ) ) {

        // If a listbox exists and it has something in it and you press either mouse button
        // or a key handled by the list.
        m_ACListBox->KillACListBox();
    }
	
//
// End stuff for Autocompleter.
//

    return AutoCompCEdit::PreTranslateMessage( pMsg );
}


void CHeaderField::FinishNNAutoComplete()
{

	if (!m_ACListBox)
		return;

	int Index = LineFromChar();

	int LineOffset = LineIndex();

	char *buf = DEBUG_NEW char[256];
	
	char Partial[256];
	Partial[0] = 0;
	unsigned int Len;
	
	// Grab the line that the caret is in
				
	if ( ((Len = GetLine(Index, buf, 255)) <= 0 || !*buf) )
		return;

	buf[Len] = 0;

	int SelStart, SelEnd;

	GetSel(SelStart, SelEnd);

	// Unless you're at the end then you get no autocompletion.
	if (((int)strlen(buf) + LineOffset) < SelStart || buf[SelStart - LineOffset] != 0)
	{
		delete [] buf;
		return;
	}

	char* Start = buf + SelStart - LineOffset;

	// Find the beginning of the string if no text is selected
	if (SelStart == SelEnd)
	{
		while (Start > buf && Start[-1] != ',')
			Start--;
	}

	if (Start[-1] != ',' && Index > 0)	// It's possible the rest of this entry is on the previous line.
	{
		bool done = false;
		// JOSH: ToDO: Make this get the previous line and make buf contain whole entry,
		// not just that line's part of that entry.
		char *buf2 = DEBUG_NEW char[300];
		int curIndex = Index - 1;

		while (!done && curIndex >= 0)
		{
			Len = GetLine(curIndex, buf2, 299);
			if (Len <= 0 || !*buf2)
				break;
			else
			{
				buf2[Len] = 0;
				curIndex--;

				char * floater = strrchr(buf2, ',');
				if (floater)
				{
					done = true;
					char *concat = DEBUG_NEW char [strlen(buf)+strlen(floater+1)+1];
					strcpy(concat, floater+1);
					strcat(concat, buf);
					char *temp = buf;
					buf = concat;
					delete [] temp;
				}
				else
				{
					char *concat = DEBUG_NEW char [strlen(buf)+strlen(buf2)+1];
					strcpy(concat, buf2);
					strcat(concat, buf);
					char *temp = buf;
					buf = concat;
					delete [] temp;					
				}
			}
		}
		delete [] buf2;
		Start = buf;
	}

	// Skip by leading space
	while (*Start == ' ' || *Start == '\r', *Start == '\n')
		Start++;

	ASSERT(g_Nicknames != NULL);
	Len = strlen(Start);
	BOOL keep_going = true;		// flag to terminate all loops from inner loop

	// Here we want to first check the history list for matches then we go on to the addressbook. 

	CStringList* AddressBookMatches = NULL;
	CStringList* PluginBookMatches = NULL;
	CStringList* AutoCompMatches = NULL;
	CObArray AdrMatchArr;

	if (GetIniLong(IDS_INI_DO_HIST_COMP))
	{
		AutoCompList* pACL = GetAutoCompList();
		if (pACL)
			AutoCompMatches = pACL->FindListOfACStartingWith(Start);
	}

	if (GetIniLong(IDS_INI_DO_AB_COMP) != 0)
	{
		AddressBookMatches = DEBUG_NEW CStringList();

		char* trueStart = Start;
		while (*trueStart == ' ' && ( trueStart < (Start + strlen(Start)) ) )
			trueStart++;

		POSITION pos = NULL;

		for (CNicknameFile* NickFile = g_Nicknames->GetFirstNicknameFile();
			 keep_going && (NickFile != NULL);
			 NickFile = g_Nicknames->GetNextNicknameFile())
		{
				// if it is a plugin nickfile we'll populate later
				 // If the user wants it
				 if(NickFile->IsPluginNickFile())
					 continue;

			while (keep_going && ((pos = NickFile->FindNicknameStartingWith(trueStart, pos)) != NULL))
			{
				CNickname* nn = NickFile->GetAt(pos);
				nn->SetNickContainingFile(NickFile->m_Filename);
				
				if (!nn)
					continue;
				
				const char* Name = nn->GetName();
				
				if (strlen(Name) >= Len)
				{
					AddressBookMatches->AddTail(Name);
					AdrMatchArr.Add(nn);
				}
			}
		}
		ASSERT(AddressBookMatches->GetCount() == AdrMatchArr.GetSize());
	}


	// For plugin Nickname Files.
	if (GetIniLong(IDS_INI_DO_PLUG_COMP) != 0)
	{
		PluginBookMatches = DEBUG_NEW CStringList();
		char* trueStart = Start;
		while (*trueStart == ' ' && ( trueStart < (Start + strlen(Start)) ) )
			trueStart++;

		POSITION pos = NULL;

		for (CNicknameFile* NickFile = g_Nicknames->GetFirstNicknameFile();
			 keep_going && (NickFile != NULL);
			 NickFile = g_Nicknames->GetNextNicknameFile())
		{
				// if it is a plugin nickfile we'll populate later
				 // If the user wants it
				 if(!NickFile->IsPluginNickFile())
					 continue;

			while (keep_going && ((pos = NickFile->FindNicknameStartingWith(trueStart, pos)) != NULL))
			{
				CNickname* nn = NickFile->GetAt(pos);
				
				if (!nn)
					continue;
				
				const char* Name = nn->GetName();
				
				if (strlen(Name) >= Len)
				{
					PluginBookMatches->AddTail(Name);
				}
			}
		}

	}

	int NumEntries = 0;

	if (AutoCompMatches)
		NumEntries += AutoCompMatches->GetCount();

	if (AddressBookMatches)
		NumEntries += AddressBookMatches->GetCount();

	if(PluginBookMatches)
		NumEntries += PluginBookMatches->GetCount();

	m_ACListBox->KillACListBox();

	if (NumEntries > 0)
	{
		m_ACListBox->DoACListBox();	
		if (AutoCompMatches && !GetIniLong(IDS_INI_AUTOCOMP_AB_FIRST))
		{
			for (int i=0; i < AutoCompMatches->GetCount(); i++)
			{
				CString temp = AutoCompMatches->GetAt(AutoCompMatches->FindIndex(i));

				char *tempChars = DEBUG_NEW char[temp.GetLength()+1];				
				strcpy(tempChars, temp);

				m_ACListBox->AddToACListBox(tempChars, 0);
				delete [] tempChars;
			}
		}

		if (AddressBookMatches)
		{
			for (int i=0; i < AddressBookMatches->GetCount(); i++)
			{
				CString temp = AddressBookMatches->GetAt(AddressBookMatches->FindIndex(i));

				char *tempChars = DEBUG_NEW char[temp.GetLength()+1];				
				strcpy(tempChars, temp);

				m_ACListBox->AddToACListBox(tempChars, 1, (DWORD)(CNickname*) AdrMatchArr.GetAt(i));
				delete [] tempChars;
			}
		}
		
		if (PluginBookMatches)
		{
			for (int i=0; i < PluginBookMatches->GetCount(); i++)
			{
				CString temp = PluginBookMatches->GetAt(PluginBookMatches->FindIndex(i));

				char *tempChars = DEBUG_NEW char[temp.GetLength()+1];				
				strcpy(tempChars, temp);

				m_ACListBox->AddToACListBox(tempChars, 3);
				delete [] tempChars;
			}
		}

		if (AutoCompMatches && GetIniLong(IDS_INI_AUTOCOMP_AB_FIRST))
		{
			for (int i=0; i < AutoCompMatches->GetCount(); i++)
			{
				CString temp = AutoCompMatches->GetAt(AutoCompMatches->FindIndex(i));

				char *tempChars = DEBUG_NEW char[temp.GetLength()+1];				
				strcpy(tempChars, temp);

				m_ACListBox->AddToACListBox(tempChars, 0);
				delete [] tempChars;
			}
		}

		m_ACListBox->InitList(m_DoDropDown);
	}

	delete [] buf;
	delete AutoCompMatches;

	delete AddressBookMatches;
	delete PluginBookMatches;
}


//////////////////////////////////////////////////////////////////////////////
// Attachment specific stuff
//

void CHeaderField::SelectAttachments()
{
	ASSERT(GetDlgCtrlID() == IDC_HDRFLD_EDIT6);
	
	DWORD Range;
	WORD Start, End;

	// Figure out what's selected, and make sure Start is before End
	Range	= GetSel();
	Start	= min(LOWORD(Range), HIWORD(Range));
	End		= max(LOWORD(Range), HIWORD(Range));
	
	// Grab the text
	CString Text;
	GetWindowText(Text);
	int Len = Text.GetLength();
	const char* t = Text;
	const char* p;

	if (Start == End)
	{
		// No range is selected, so select whatever attachment the caret is on
		if (Start && (End == Len || t[Start - 1] != ';'))
			Start--;
		End++;
	}

	// Find start and beginning of filename(s)
	for (p = (t + Start); Start > 0 && p[-1] != ';'; p--, Start--) {}
	for (p = (t + End); End < Len && (p[-1] != ';' && !(p[-1] == ' ' && p[-2] == ';')); p++, End++) {}
	
	// If there's a space after the filename(s) then use it,
	// otherwise if there's a space before the filename(s) then use it
	if (End < Len && t[End] == ' ')
		End++;
	else if (Start > 0 && t[Start - 1] == ' ')
		Start--;

	// Select the filename(s)
	SetSel(Start, End, FALSE);
}

void CHeaderView::AddAttachment(const char* Filename)
{
	CHeaderField* pField = GetHeaderCtrl(HEADER_ATTACHMENTS);

	CString NewText(Filename);
	
	// Put space between filenames
	if (pField->GetWindowTextLength())
		NewText = ' ' + NewText;
	
	pField->SetSel(-1, -1, TRUE);
	pField->ReplaceSel(NewText + ';');
		
	GetDocument()->SetModifiedFlag();
}

void CHeaderView::OnOpenAttachment()
{
	char* Text;
	char* Filename;
	char* End;
	char* Semi;
	int StartSel, EndSel;
	int Len;
	
	CHeaderField* pField = GetHeaderCtrl(HEADER_ATTACHMENTS);

	Len = pField->GetWindowTextLength();
	if (!Len || !(Text = DEBUG_NEW char[Len + 2]))
		return;
	
	pField->GetWindowText(Text, Len+1);
	pField->GetSel(StartSel, EndSel);
	if (Text[StartSel] == ' ')
		StartSel++;
	Len = EndSel - StartSel;
	strncpy(Text, Text + StartSel, Len);
	Text[Len] = 0;
	Filename = Text;
	End = Filename + strlen(Filename);
	for (; Filename < End; Filename = Semi)
	{
		if (Semi = strchr(Filename, ';'))
		{
			*Semi++ = 0;
			if (*Semi == ' ')
				Semi++;
		}
		else
			Semi = End;
		
		DoFileCommand(Filename, 0, false);
	}
	
	delete [] Text;
}


void CHeaderView::OnUpdateOpenAttachment(CCmdUI* pCmdUI)
{
//	CHeaderField* pField = GetHeaderCtrl(HEADER_ATTACHMENTS);
//	pCmdUI->Enable(GetCurrentEdit() ==pField && pField->GetWindowTextLength());
}

long CHeaderView::OnContextMenu(WPARAM wParam, LPARAM lParam)
{
	TRACE( "CHeaderView::OnContextMenu\n" );
	return GetDocument()->DoContextMenu( this, wParam, lParam);
}


BOOL CHeaderView::PreTranslateMessage(MSG* pMsg)
{
	// is this a right click on a child window?
	if (	( ::IsChild( GetSafeHwnd(), pMsg->hwnd ) == TRUE ) &&
			( pMsg->message == WM_RBUTTONUP ) )
	{
		// yep - grab it
		PostMessage( pMsg->message, pMsg->wParam, pMsg->lParam );
		return TRUE;
    }
	else
    {    
		// nope - process normally
		return CFormView::PreTranslateMessage( pMsg );
	}
}


void CHeaderView::OnUpdateMessageAttachFile(CCmdUI* pCmdUI) 
{
	char	attachFileString[255];
	CMenu*	messageMenu = CMenu::FromHandle(CMainFrame::QCGetMainFrame()->GetMessageMenu());
	
	// The Attach File Item can be Attach File or Attach to New Message depending what's on top
	if (messageMenu->GetMenuString(ID_MESSAGE_ATTACHFILE, attachFileString,
		sizeof(attachFileString), MF_BYCOMMAND))
	{
		CRString fileText(IDS_ATTACH_FILE_TEXT);

		if (stricmp(attachFileString, fileText))
		{
			messageMenu->ModifyMenu(ID_MESSAGE_ATTACHFILE, 
									MF_BYCOMMAND | MF_STRING, 
									ID_MESSAGE_ATTACHFILE, 
									fileText);
		}
	}

	pCmdUI->Enable( TRUE );
}

void CHeaderView::OnUpdateFontCombo( CCmdUI* pCmdUI ) 
{
	pCmdUI->Enable( FALSE );
}

BOOL CHeaderView::OnDynamicCommand(
UINT uID )
{
	QCCommandObject*		pCommand;
	COMMAND_ACTION_TYPE		theAction;	

	if( ! g_theCommandStack.GetCommand( ( WORD ) uID, &pCommand, &theAction ) )
	{
		return FALSE;
	}

	if( ( pCommand == NULL ) || !theAction )
	{
		return FALSE;
	}

	if( theAction == CA_INSERT_RECIPIENT )
	{
		CMainFrame* p_mainframe = (CMainFrame *) AfxGetMainWnd();
		ASSERT( p_mainframe );
		if (!p_mainframe) return TRUE;

		ASSERT_KINDOF( QCRecipientCommand, pCommand );
		CString Name = ( ( QCRecipientCommand* ) pCommand )->GetName();

		char Recip[256];
		int RecipOffset = 1;
				
		Recip[0] = ',';
		strcpy(Recip + 1, Name);
				
		char buf[256];
		CWnd* wnd = p_mainframe->GetFocus();
		if( wnd && wnd->IsKindOf( RUNTIME_CLASS( AutoCompCEdit ) ) )
		{
			AutoCompCEdit* Edit = (AutoCompCEdit*)wnd;

			int LineNum, LineStart, SelStart, Start, Len;
			LineNum = Edit->LineFromChar();
			LineStart = Edit->LineIndex(LineNum);
			SelStart = LOWORD(Edit->GetSel());
			Start, Len;
						
			Len = Edit->GetLine(LineNum, buf, sizeof(buf));
			Start = min(Len, SelStart - LineStart - 1);
			for (; Start >= 0; Start--)
			{
				char c = buf[Start];
				if (c && !isspace((int)(unsigned char)c))
				{
					if (c != ',')
						RecipOffset = 0;
					break;
				}
			}

			if (!ShiftDown())
				Edit->SendMessage(EM_REPLACESEL, TRUE, (LPARAM)Recip + RecipOffset);
			else
			{
				char *expandAl = ExpandAliases(Recip + RecipOffset);
				if (!RecipOffset)
					Edit->SendMessage(EM_REPLACESEL, TRUE, (LPARAM)",");
				Edit->SendMessage(EM_REPLACESEL, TRUE, (LPARAM)expandAl);
				delete [] expandAl;
			}
		}

		return TRUE;
	}		

	return FALSE;
}

void CHeaderView::OnUpdateDynamicCommand(
CCmdUI* pCmdUI)
{
	QCCommandObject*	pCommand;
	COMMAND_ACTION_TYPE	theAction;	

	if( pCmdUI->m_pSubMenu == NULL )
	{
		if( g_theCommandStack.Lookup( ( WORD ) ( pCmdUI->m_nID ), &pCommand, &theAction ) )
		{
			if( theAction == CA_INSERT_RECIPIENT )
			{
				pCmdUI->Enable( TRUE );
				return;
			}
			else if( theAction == CA_TRANSMISSION_PLUGIN ) 
			{
				CCompMessageDoc	*pDoc = NULL;
				CSummary		*pSummary = NULL;
				VERIFY( pDoc = (CCompMessageDoc*) GetDocument() );
				VERIFY( pSummary = pDoc->m_Sum );
				
				ASSERT( pDoc != NULL );
				ASSERT( pDoc->IsKindOf(RUNTIME_CLASS(CCompMessageDoc)) );

				pCmdUI->Enable( !pSummary->CantEdit() );
				return;
			}
		}
	}

	pCmdUI->ContinueRouting();
}


void CHeaderView::OnEditPaste() 
{
	COleDataObject clipboardData;
	VERIFY( clipboardData.AttachClipboard() );

	// if this is file(s) on the clipboard, we'll pass this on to
	// our parent frame.
	if ( clipboardData.IsDataAvailable( CF_HDROP ) ) {

		// suck out the HDROP and delegate to OnDropFiles.
		STGMEDIUM stgMedium;
		clipboardData.GetData( CF_HDROP, &stgMedium );

		CWnd* pPF = GetParentFrame();
		pPF->SendMessage( WM_DROPFILES,(WPARAM)stgMedium.hGlobal );
	}
	else {

		// forward the message to the header field that has focus
		::SendMessage( ::GetFocus(), WM_PASTE, 0, 0 );
	}
}


void CHeaderView::OnUpdateEditPasteAsQuote( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( FALSE );
}


int CHeaderView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFormView::OnCreate(lpCreateStruct) == -1)
        return -1;
	
	if (s_nObjectCount == 0)
	{
		LOGFONT		theLF;
		
		GetMessageFont().GetLogFont( &theLF );

		// get the logical screen resolution
		CDC			theDC;
		theDC.CreateCompatibleDC( NULL );
		LONG		lVertRes = theDC.GetDeviceCaps(LOGPIXELSY);
		
		// get the height of the font in points
		LONG		lPtSize = ( ( abs( theLF.lfHeight ) * 72 ) + ( lVertRes >> 1 ) ) / lVertRes;

		// create the fonts
		if ( !s_editFont.CreateFont(theLF.lfFaceName, lPtSize, true) )
			return -1;

		if ( !s_labelFont.CreateFont(theLF.lfFaceName, lPtSize, true) )
		{
			BOOL	bDeleteObjectGood = s_editFont.DeleteObject();
			ASSERT(bDeleteObjectGood);

			return -1;
		}
	}
		
	++s_nObjectCount;

	return 0;
}


void CHeaderView::OnDestroy()
{		
	CCompMessageDoc	*pDoc = NULL;
	VERIFY(pDoc = (CCompMessageDoc*) GetDocument());
	int nAutoSize = m_AutoArray.GetSize();

	if (nAutoSize)
	{
		AutoCompList* pACL = GetAutoCompList();
		if (pACL)
		{
			if (pDoc->m_HasBeenSaved)
				pACL->SaveToDisk();
			else
			{
				for (int j = 0; j < nAutoSize; j++)
					pACL->Remove(m_AutoArray[j]);
			}
		}
	}

	m_AutoArray.RemoveAll();

    --s_nObjectCount;

	if (s_nObjectCount == 0)
	{
		BOOL	bDeleteObjectGood = s_editFont.DeleteObject();
		ASSERT(bDeleteObjectGood);

		bDeleteObjectGood = s_labelFont.DeleteObject();
		ASSERT(bDeleteObjectGood);
	}

	CFormView::OnDestroy();
}


