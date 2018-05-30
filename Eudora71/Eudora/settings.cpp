// SETTINGS.CPP
//
// Routines for the Settings dialog
//
// Copyright (c) 1994-2001 by QUALCOMM, Incorporated
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

#include <afxcmn.h>
#include <afxrich.h>
#include <ctype.h>
#include <dos.h>
#include <mmsystem.h>
#include <vfw.h>

#include "QCUtils.h"

#include "shlobj.h"
#include "resource.h"
#include "helpxdlg.h"
#include "rs.h"
#include "cursor.h"
#include "fileutil.h"
#include "guiutils.h"
#include "address.h"
#include "password.h"
#include "jjnet.h"
#include "QCRasConnection.h"
#include "utils.h"
#include "font.h"
#include "eudora.h"
#include "doc.h"
#include "msgdoc.h"
#include "compmsgd.h"
#include "CompMessageFrame.h"
#include "mdichild.h"
#include "msgframe.h"
#include "mainfrm.h"
#include "msgutils.h"
#include "ddeclien.h"
#include "persona.h"
#include "summary.h"
#include "tocdoc.h"
#include "tocframe.h"
#include "acapdlg.h"
#include "Automation.h"
#include "QCLabelDirector.h"
#include "QCSignatureDirector.h"
#include "QCStationeryDirector.h"
#include "SafetyPal.h"
#include "TaskStatusView.h"
#include "EudoraMsgs.h"
#include "QCTaskManager.h"
#include "QCSharewareManager.h"
#include "statmng.h"
#include "ExtLaunchMgr.h"
#include "SSLConnectionDlg.h"
#include "QCMailboxTreeCtrl.h"
#include "SelectMBox.h"
#include "ContentConcentrator.h"
#include "SearchManager.h"
#include "JunkMail.h"
#include "nickdoc.h"
#include "BossProtector.h"
#include "TextToSpeech.h"

#ifdef IMAP4
#include "QCMailboxDirector.h"
extern QCMailboxDirector g_theMailboxDirector;
#endif

// For SHBrowseForFolders new functionality
#ifndef BIF_USENEWUI
#define BIF_USENEWUI 0x0040
#endif

#include "settings.h"

#include "DebugNewHelpers.h"

// --------------------------------------------------------------------------

void AFXAPI AfxSetWindowText(HWND hWndCtrl, LPCTSTR lpszNew); 

// --------------------------------------------------------------------------

extern QCSignatureDirector	g_theSignatureDirector;
extern QCStationeryDirector	g_theStationeryDirector;

// --------------------------------------------------------------------------

class CCategory : public CObject
{
public:
	CString		m_Name;
	UINT		m_DlgID;
	HINSTANCE	m_hDll;
};

/////////////////////////////////////////////////////////////////////////////
// CSettingsListBox

CSettingsListBox::CSettingsListBox()
{
}

CSettingsListBox::~CSettingsListBox()
{
}

BEGIN_MESSAGE_MAP(CSettingsListBox, CListBox)
	//{{AFX_MSG_MAP(CSettingsListBox)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSettingsListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
}

int CSettingsListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
	return (-1);
}

void CSettingsListBox::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct)
{
	CCategory* Cat = (CCategory*)lpDeleteItemStruct->itemData;
	
	delete Cat;
}

void CSettingsListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CCategory* Cat = (CCategory*)lpDIS->itemData;
	CRect rect(lpDIS->rcItem);

	// Draw focus rect
	if ((lpDIS->itemAction & ODA_FOCUS) == ODA_FOCUS)
	{
		pDC->DrawFocusRect(&rect);
		return;
	}
	
	// Set up some color values
	COLORREF BackColor, ForeColor;
	if (lpDIS->itemState & ODS_SELECTED)
	{
		BackColor = ::GetSysColor(COLOR_HIGHLIGHT);
		ForeColor = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	}
	else
	{
		BackColor = ::GetSysColor(COLOR_WINDOW);
		ForeColor = ::GetSysColor(COLOR_WINDOWTEXT);
	}
	
	// Draw background
	CBrush BackBrush(BackColor);
	pDC->FillRect(&rect, &BackBrush);
	
	UINT nFormat = DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX;
	if (!GetIniShort(IDS_INI_SHOW_CATEGORY_ICONS))
		rect.left += 2;
	else
	{
		// Draw icon
		rect.top += 5;
		int IconWidth = GetSystemMetrics(SM_CXICON);
		int IconHeight = GetSystemMetrics(SM_CYICON);

		HICON hIcon = ::LoadIcon(Cat->m_hDll, MAKEINTRESOURCE( Cat->m_DlgID ));
		if (!hIcon)
			hIcon = QCLoadIcon( MAKEINTRESOURCE( IDD_SETTINGS_FIRST_CAT ) );
		if (hIcon)
		{
			pDC->DrawIcon((rect.Width() / 2) - (IconWidth / 2), rect.top + 1, hIcon);
			::DestroyIcon(hIcon);
		}
		rect.top += IconHeight;
		nFormat |= DT_CENTER;
	}

	// Draw name
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(ForeColor);
	pDC->DrawText(Cat->m_Name, Cat->m_Name.GetLength(), &rect, nFormat);
}

/////////////////////////////////////////////////////////////////////////////
// CSettingsListBox message handlers

void CSettingsListBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	static DWORD sLastKeyTime = 0;
	static TCHAR sKeyBuffer[32];
	static UINT sNumBufferChars = 0;
	const DWORD NewKeyTime = GetTickCount();

	if (nChar < ' ')
		sLastKeyTime = 0;
	else
	{
		if (NewKeyTime > sLastKeyTime + 1000)
			sNumBufferChars = 0;
		sLastKeyTime = NewKeyTime;
		if (sNumBufferChars < (sizeof(sKeyBuffer)/sizeof(sKeyBuffer[0])))
		{
			sKeyBuffer[sNumBufferChars++] = (TCHAR)nChar;
			const int Count = GetCount();
			int Start = GetCurSel();
			if (Start < 0 || Start >= Count)
				Start = 0;
			int i = sNumBufferChars > 1? -1 : (Start + 1) % Count;
			while (i != Start)
			{
				if (i == -1)
					i = Start;

				CCategory* Cat = (CCategory*)GetItemDataPtr(i);
				if (Cat && Cat != (CCategory*)-1)
				{
					if (strnicmp(Cat->m_Name, sKeyBuffer, sNumBufferChars) == 0)
					{
						((CSettingsDialog*)GetParent())->SmartSetSel(i);
						break;
					}
				}

				i = (i + 1) % Count;
			}
		}
	}

	CListBox::OnChar(nChar, nRepCnt, nFlags);
}


/////////////////////////////////////////////////////////////////////////////
// CLabelColorButton

CLabelColorButton::CLabelColorButton()
{
}

CLabelColorButton::~CLabelColorButton()
{
}

void CLabelColorButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rect(lpDIS->rcItem);
	
	// Draw focus rect
	if ((lpDIS->itemAction & ODA_FOCUS) == ODA_FOCUS)
	{
		pDC->DrawFocusRect(&rect);
		return;
	}
	
	// The control doesn't change its look for selection
	if (!(lpDIS->itemAction & ODA_DRAWENTIRE))
		return;

	// Draw background
//	CBrush Brush(GetSysColor(COLOR_HIGHLIGHT));
	CBrush Brush(m_Color);
	pDC->SelectObject(&Brush);
	pDC->SelectStockObject(BLACK_PEN);
	pDC->Rectangle(&rect);
	pDC->SelectStockObject(BLACK_BRUSH);
	
	// Draw text
	CString Text;
	GetWindowText(Text);
	pDC->SetBkMode(TRANSPARENT);
//	pDC->SetTextColor(m_Color);
	pDC->SetTextColor(GetSysColor(COLOR_WINDOW));
	pDC->DrawText(Text, Text.GetLength(), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	// Draw focus rect
	if (lpDIS->itemState & ODS_FOCUS)
		pDC->DrawFocusRect(&rect);
}

BEGIN_MESSAGE_MAP(CLabelColorButton, CButton)
	//{{AFX_MSG_MAP(CLabelColorButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLabelColorButton message handlers



/////////////////////////////////////////////////////////////////////////////
// CSettingsDialog dialog

int CSettingsDialog::m_Sel = -1;

CSettingsDialog::CSettingsDialog(CWnd* pParent /*=NULL*/)
	: CHelpxDlg(CSettingsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_DialogHWND = NULL;
	m_StatCombo = NULL;
	m_Static = NULL;

	m_NoDefaultStr.LoadString(IDS_NO_DEFAULT);
	m_NoRelayStr.LoadString (IDS_NO_RELAY_DEFINED);
	m_bACAPBusy = FALSE;
	m_VoiceIndex = -1;
	m_VoiceGUID = GetIniString(IDS_INI_BP_SPEAK);
	m_bHasSpeechEngine = false;
	m_hNewMailSound = NULL;
}

CSettingsDialog::~CSettingsDialog()
{
	SetIniShort(IDS_INI_LAST_SETTINGS_CATEGORY, short(m_Sel));
	FlushINIFile();
	QCLabelDirector::GetLabelDirector()->Write(); // Flush label info to INI

	if ( m_StatCombo )
	{
		delete m_StatCombo;
		m_StatCombo = NULL;
	}

	if ( m_Static )
	{
		delete m_Static;
		m_Static = NULL;
	}

	if ( m_DialogHWND )
	{
		if ( ::IsWindow( m_DialogHWND ) )
			::DestroyWindow(m_DialogHWND);
		m_DialogHWND = NULL;
	}

	if (m_hNewMailSound)
	{
		MCIWndStop(m_hNewMailSound);
		MCIWndDestroy(m_hNewMailSound);
	}
}

BOOL CSettingsDialog::OnInitDialog()
{
	CHelpxDlg::OnInitDialog();

	SetCategoryHeight();

	m_bCallImapFunctions	=	false ;
	
	UINT nID = IDD_SETTINGS_FIRST_CAT + 1;

	// Go through the category dialogs sequentially through resource ID
	for (;; nID++)
	{
		//If not full feature then dont add Mood Watch, Statistics, Auto-Completion & Spell-Checking categories
		if( nID == IDD_SETTINGS_MOODMAIL || 
			nID == IDD_SETTINGS_JUNK || 
			nID == IDD_SETTINGS_JUNK_EXTRAS || 
			nID == IDD_SETTINGS_CON_CON ||
			nID == IDD_SETTINGS_STATISTICS || 
			nID == IDD_SETTINGS_FIELD_COMP ||
			nID == IDD_SETTINGS_SPELL || 
			nID == IDD_SETTINGS_FIND_MESSAGES )
		{
			if(!UsingFullFeatureSet()) continue;
		}	

		if( nID == IDD_SETTINGS_BOSS_PROTECTOR ) {
			if (!UsingPaidFeatureSet()) 
				continue;
		}


#ifndef COMMERCIAL
		if (nID != IDD_SETTINGS_LABELS && 
			nID != IDD_SETTINGS_SPELL )
#endif // COMMERCIAL
		{
			LPCSTR DlgName = MAKEINTRESOURCE(nID);
			HINSTANCE hResInst = QCFindResourceHandle(DlgName, RT_DIALOG);
			if (!FindResource( hResInst, DlgName, RT_DIALOG))
				break;
			// Create a temporary dialog to get some info from it, then destroy it
			HWND hWnd = ::CreateDialog(hResInst, DlgName, m_hWnd, NULL);
			if (hWnd)
			{
				CCategory* Cat = DEBUG_NEW CCategory;
				Cat->m_hDll = hResInst;
				Cat->m_DlgID = nID;
				CWnd* Wnd = CWnd::FromHandle(hWnd);
				Wnd->GetWindowText(Cat->m_Name);
				Wnd->DestroyWindow();
				m_Categories.AddString((LPCSTR)Cat);
			}
		}
	}

	// find custom dialogs (in plug-ins)
	for ( nID = IDD_SETTINGS_ORLIST_START; nID <= IDD_SETTINGS_ORLIST_END; nID++)
	{
		// check all resource DLLs
		LPCSTR		DlgName = MAKEINTRESOURCE(nID);
		CPtrList *	pResList = ((CEudoraApp *)AfxGetApp())->GetResListPtr();
		POSITION	pos;

		for( pos = pResList->GetHeadPosition(); pos != NULL; )
		{
			HINSTANCE hResInst = (HINSTANCE)pResList->GetNext( pos );

			if ( hResInst && ::FindResource( hResInst, DlgName, RT_DIALOG))
			{
				// Create a temporary dialog to get some info from it, then destroy it
				HWND hWnd = ::CreateDialog(hResInst, DlgName, m_hWnd, NULL);
				if (hWnd)
				{
					CCategory* Cat = DEBUG_NEW CCategory;
					Cat->m_hDll = hResInst;
					Cat->m_DlgID = nID;
					CWnd* Wnd = CWnd::FromHandle(hWnd);
					Wnd->GetWindowText(Cat->m_Name);
					Wnd->DestroyWindow();
					m_Categories.AddString((LPCSTR)Cat);
				}
			}
		}
	}

	CenterWindow();
	
	// Size controls
    OnSize(SIZE_RESTORED, -1, -1);
	
	//
	// Select the previously selected category.
	//
	{
		int nLastCategory = 0;
		if (-1 == m_Sel)
		{
			nLastCategory = int(GetIniShort(IDS_INI_LAST_SETTINGS_CATEGORY));
		
			// Set last category to 0 in case starting out at a particular
			// category causes a crash.  If we successfully exit the dialog,
			// then the last category will be set properly.
			SetIniShort(IDS_INI_LAST_SETTINGS_CATEGORY, 0);
			FlushINIFile();
		}
		else
		{
			nLastCategory = m_Sel;

			// ASSERT, but there's at least one known way that this can happen - when
			// in light mode less categories are available so a previous index might
			// fall out of range now (even if it doesn't fall out of range it may not
			// match up with the previously selected category but c'est la vie).
			ASSERT(nLastCategory >= 0 && nLastCategory < m_Categories.GetCount());
		}

		if (nLastCategory < 0 || nLastCategory >= m_Categories.GetCount())
			nLastCategory = 0;		// INI value out of range, so ignore it

		m_Categories.SetCurSel(nLastCategory);
	}

	//
	// Force m_Sel to be a different value so the controls get filled in.
	//
	m_Sel = -1;
	OnSelChange();		// resets m_Sel to current selection
	
	// In order to get the category controls immediately after the Category list box,
	// the Category list box has to be last in the tab order.  But we want the Category
	// list box to have the focus when we start up, so set the focus and return FALSE
	// so that Windows doesn't try and set it to the focus to the first control.
	GotoDlgCtrl(&m_Categories);

	//save the initial value that the user had for the option
	//so that when the user hits OK we can test if the value has changed

	bHideABookFromVirusBefore = GetIniShort(IDS_INI_HIDE_ABOOK_FROM_VIRUS);


	return (FALSE);
}

void CSettingsDialog::SetCategoryHeight()
{
    // Set item height of the category list box to include the height of the text,
    // plus the height of the icon if we're showing them
    int Height = HIWORD(GetDialogBaseUnits()) + 2;
    if (GetIniShort(IDS_INI_SHOW_CATEGORY_ICONS))
    	Height += GetSystemMetrics(SM_CYICON) + 4;
	m_Categories.SetItemHeight(0, Height);
}

void CSettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpxDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsDialog)
	DDX_Control(pDX, IDC_SETTINGS_CAT_LIST, m_Categories);
	//}}AFX_DATA_MAP


	// Task cols will be updated if this becomes true
	bool bTaskStatusColChanged = false;

	// Some changes require windows to be redisplayed.  This BOOL records whether or not
	// that needs to happen, and if so it does the redisplay after going through all of
	// the controls.
	BOOL VisibleFontChanged = FALSE;
	
	// Some changes require mailbox date fields to be redisplayed.  This BOOL records whether or not
	// that needs to happen, and if so it does the redisplay after going through all of
	// the controls.
	BOOL DateFormatChanged = FALSE;

	// If we're saving the info it's because we're switching to a different category or
	// leaving the Settings dialog.  In either case, save the info, and attach the controls
	// back to the created sub-dialog so that they can be destroyed later.
	if (pDX->m_bSaveAndValidate)
	{
		CWnd* Wnd = CWnd::FromHandle(m_DialogHWND);
		CWnd* Ctrl;
		CWnd* Next;
		char Text[256];
		UINT nID, IniID;
		UINT ServerLoginID = 0;
		BOOL bPOPServerEmpty = TRUE;
		BOOL bLoginNameEmpty = TRUE;

		if (*GetIniString(IDS_INI_POP_SERVER))
			bPOPServerEmpty = FALSE;
		if (*GetIniString(IDS_INI_LOGIN_NAME))
			bLoginNameEmpty = FALSE;

		// First go through and validate some of the controls (mostly numerics)
		for (Ctrl = GetWindow(GW_CHILD); Ctrl; Ctrl = Ctrl->GetNextWindow())
		{
			nID = Ctrl->GetDlgCtrlID();
			IniID = nID & 0x7FFF;
			switch (IniID)
			{
			case IDS_INI_REAL_NAME:
			case IDS_INI_DOMAIN_QUALIFIER:
			case IDS_INI_LABEL_TEXT_1:
			case IDS_INI_LABEL_TEXT_2:
			case IDS_INI_LABEL_TEXT_3:
			case IDS_INI_LABEL_TEXT_4:
			case IDS_INI_LABEL_TEXT_5:
			case IDS_INI_LABEL_TEXT_6:
			case IDS_INI_LABEL_TEXT_7:
			case IDS_INI_IMAP_TRASH_MBOXNAME:
				TRY
				{
					CString value;
					DDX_Text(pDX, nID, value);
					DDV_MaxChars(pDX, value, 128);
				}
				CATCH_ALL(e)
				{
					// We got an invalid entry, so rethrow the exception and let
					// UpdateData() deal with it
					THROW_LAST();
					return;
				}
				END_CATCH_ALL
				break;

			case IDS_INI_CHECK_FOR_MAIL:
			case IDS_INI_MESSAGE_WIDTH:
			case IDS_INI_MESSAGE_LINES:
			case IDS_INI_TAB_STOP:
			case IDS_INI_AUTO_OK_TIMEOUT:
			case IDS_INI_BIG_MESSAGE_THRESHOLD:
			case IDS_INI_KERB_BUFF_SIZE:
			case IDS_INI_DEBUG_LOG_LEVEL:
			case IDS_INI_DEBUG_LOG_FILE_SIZE:
			case IDS_INI_LEAVE_ON_SERVER_DAYS:
			case IDS_INI_OLD_DATE_HOURS:
			case IDS_INI_ANCIENT_DATE_HOURS:
			case IDS_INI_IMAP_MAXSIZE:
			case IDS_INI_AUTO_SAVE_MSGS_EVERY:
			case IDS_INI_MAX_RECENT_MAILBOX:
				// Don't try to validate an empty setting as the MFC validation code will complain
				Ctrl->GetWindowText(Text, sizeof(Text));
				if (*Text)
				{
					TRY
					{
						DWORD value;
						DDX_Text(pDX, nID, value);
						DDV_MinMaxDWord(pDX, value, 0, MAXSHORT);					
 						if (nID == IDS_INI_LEAVE_ON_SERVER_DAYS && value == 0)
						{
							CWnd* DeleteFromServer = GetDlgItem(IDS_INI_DELETE_MAIL_FROM_SERVER);
							if ((DeleteFromServer && IsDlgButtonChecked(IDS_INI_DELETE_MAIL_FROM_SERVER)) ||
								(!DeleteFromServer && GetIniShort(IDS_INI_DELETE_MAIL_FROM_SERVER)))
							{
								// Can't have LMOS X days set to 0 if Delete from server is checked
								ErrorDialog(IDS_ERR_SERVER_DELETE_X_DAYS);
								pDX->PrepareEditCtrl(nID);
								pDX->Fail();
								break;
							}
						}
						else if (nID == IDS_INI_CHECK_FOR_MAIL)
						{
							int MinInterval = GetIniShort(IDS_MIN_MAIL_CHECK_INTERVAL);
							if ( value && ( (int)value < MinInterval ) )
							{
								// Mail check interval is lower than allowed
								ErrorDialog(IDS_ERR_CHECK_FOR_MAIL_INTERVAL, MinInterval);
								pDX->PrepareEditCtrl(nID);
								pDX->Fail();
							}
						}
					}
					CATCH_ALL(e)
					{
						// We got an invalid entry, so rethrow the exception and let
						// UpdateData() deal with it
						THROW_LAST();
						return;
					}
					END_CATCH_ALL
				}
				break;
				
			case IDS_INI_AUTOMATION_ENABLED:
				if (IsDlgButtonChecked(IDS_INI_AUTOMATION_ENABLED))
				{
					if (!gbAutomationEnabled)
					{
						// Automation was just enabled
						AutomationEnable();
						// Make automation immediately available from
						// this application
						AutomationRegisterFactory();
					}
				}
				else
				{
					if (gbAutomationEnabled)
					{
						// Automation was just disabled
						AutomationDisable();
						AutomationUnregisterFactory();
					}
				}
				break;
			case IDC_ACAP_BUTTON:
				//Don't allow the user to switch if we're busy doing acap stuff...
				if(m_bACAPBusy)
					pDX->Fail();
				break;
			case IDS_INI_POP_ACCOUNT:
				{
					// A simple check of the POP Account for an '@' and at least one character
					// before and after the '@'
					Ctrl->GetWindowText(Text, sizeof(Text));
					char* at = strchr(Text, '@');
					if (!at || at == &Text[0] || !at[1])
					{
						// Bad entry, so tell the user about it, and then call Fail()
						ErrorDialog(IDS_ERR_POP_ACCOUNT);
						pDX->PrepareEditCtrl(nID);
						pDX->Fail();
					}
				}
				break;

			case IDS_INI_POP_SERVER:
			case IDS_INI_LOGIN_NAME:
				// Either both POP Server and Login Name are filled in,
				// or neither should be filled in.
				Ctrl->GetWindowText(Text, sizeof(Text)) ;
				if (Text[0] == 0 || !ServerLoginID)
					ServerLoginID = nID;
				if (IniID == IDS_INI_POP_SERVER)
					bPOPServerEmpty = (Text[0] == 0);
				else
					bLoginNameEmpty = (Text[0] == 0);
				break;

			// see if we need to set the dirty flag (m_bCallImapFunctions)
			// depending on whether the user has changed from POP to IMAP 
			// or vice versa
			case IDS_INI_USES_IMAP:
				{
					if( ((CButton*)Ctrl)->GetCheck() == 1) 
					{
						m_bCallImapFunctions = true ;
					}
					else { // user wants pop
						if( m_bOrigPOP	==	false) {
							
							// user is changing IMAP to POP
							// bring up warning dialog
							char szMsg[ MAX_PATH];
							QCLoadString( IDS_CHANGE_IMAP_TO_POP, szMsg, sizeof( szMsg)) ;
							if( AfxMessageBox( szMsg, MB_YESNO) == IDNO) {

								// Ok User dont want to kill him/her self
								pDX->PrepareCtrl(nID);
								pDX->Fail();
							}
							else {
								m_bCallImapFunctions = true ; //they really want to change IMAP to POP
							}

						}
					}
				}
				break;


			case IDS_INI_MAPI_USE_NEVER:
			case IDS_INI_MAPI_USE_EUDORA:
			case IDS_INI_MAPI_USE_ALWAYS:
				if (!IsDlgButtonChecked(nID))
					break;
				if (IniID == IDS_INI_MAPI_USE_NEVER)
				{
					if (MAPIUninstall() > CMapiInstaller::STATUS_NOT_INSTALLED)
					{
						pDX->PrepareCtrl(nID);
						pDX->Fail();
					}
				}
				else
				{
					if (MAPIInstall() > CMapiInstaller::STATUS_ALREADY_INSTALLED)
					{
						pDX->PrepareCtrl(nID);
						pDX->Fail();
					}
				}
				break;

			case IDS_INI_NETSCAPE_MAILTO:
				{
					const BOOL bRegister = IsDlgButtonChecked(nID);
					if (bRegister != GetIniShort(IniID))
					{
						CDDEClient dde_client;
						dde_client.RegisterNetscapeProtocol("mailto", bRegister);
						dde_client.RegisterNetscapeProtocol("x-eudora-option", bRegister);
					}
				}
				break;
			}

		}

		// ServerLoginID will contian the ID of either the POP Server control or the
		// Login Name control.  If both settings are not empty or both are not filled,
		// then warn the user about the problem.
		if (ServerLoginID && (bPOPServerEmpty != bLoginNameEmpty))
		{
			ErrorDialog(IDS_ERR_POP_SERVER);
			pDX->PrepareEditCtrl(ServerLoginID);
			pDX->Fail();
		}

		if (GetIniShort(IDS_INI_OPTIONS_HIDE_CURSOR))
			::ShowCursor(FALSE);
		
		// Now go through all of the controls, saving info to the appropriate INI entries
		for (Ctrl = GetWindow(GW_CHILD); Ctrl; Ctrl = Next)
		{
			Next = Ctrl->GetNextWindow();
			nID = Ctrl->GetDlgCtrlID();
			IniID = nID & 0x7FFF;

			if (IniID >= 10000 && IniID < 20000 )
			{
				DWORD Style = Ctrl->GetStyle() & 0xF;

				if (IniID >= IDS_INI_LABEL_COLOR_1 && IniID <= IDS_INI_LABEL_COLOR_7)
				{
					const UINT nLabelIdx = (IniID - IDS_INI_LABEL_COLOR_1) + 1;
					QCLabelDirector::GetLabelDirector()->SetLabel(nLabelIdx, ((CLabelColorButton*)Ctrl)->m_Color);
				}
				else if ( (IniID >= IDS_INI_STATISTICS_COLOR_CURRENT && IniID <= IDS_INI_STATISTICS_COLOR_AVERAGE) )
				{
					SetIniLong(IniID,(long) ((CLabelColorButton*)Ctrl)->m_Color);
				}
				else if(IniID == IDS_INI_BP_COLOR)
				{
					long red, green, blue;
					CString txt;
					COLORREF clr;
					clr = ((CLabelColorButton*)Ctrl)->m_Color;
					red = (clr & 0xFF);
					green = ((clr >> 8) & 0xFF);
					blue = ((clr >> 16) & 0xFF);
		
					txt.Format("%ld,%ld,%ld", red, green, blue);
					SetIniString(IniID, txt);

				}
				else if(IniID == IDS_INI_BP_SPEAK)
				{
					if(m_bHasSpeechEngine) {
						m_VoiceIndex = ( ( CComboBox* ) Ctrl )->GetCurSel();

						if( m_VoiceIndex != CB_ERR )
						{
							m_VoiceGUID = (char *)( ( CComboBox* ) Ctrl )->GetItemDataPtr(m_VoiceIndex);

							if( m_VoiceGUID.CompareNoCase ( GetIniString( IniID ) ) )
							{
								SetIniString( IniID, m_VoiceGUID );
							}
						}
					}
				}
				else if ((IniID >= IDS_INI_STATISTICS_GRAPH_CURRENT && IniID <= IDS_INI_STATISTICS_GRAPH_AVERAGE) ||
							IniID == IDS_INI_SSL_RECEIVE_USE || IniID == IDS_INI_SSL_SEND_USE)
				{						
					INT i = ( ( CComboBox* ) Ctrl )->GetCurSel();

					if( ( i >= 0 ) && ( i != GetIniShort( IniID ) ) )
					{
						SetIniShort( IniID, ( short ) i );
					}
				}				
				else if(	( IniID == IDS_INI_SCREEN_FONT ) ||
							( IniID == IDS_INI_PRINTER_FONT ) ||
							( IniID == IDS_INI_MESSAGE_FONT ) ||
							( IniID == IDS_INI_MESSAGE_FIXED_FONT ))
				{						
					INT i = ( ( CComboBox* ) Ctrl )->GetCurSel();

					if( i != CB_ERR )
					{
						CString szText;

						( ( CComboBox* ) Ctrl )->GetLBText( i, szText );
						
						if( szText.CompareNoCase ( GetIniString( IniID ) ) )
						{
							SetIniString( IniID, szText );
							
							if( IniID != IDS_INI_PRINTER_FONT )
							{
								VisibleFontChanged = TRUE;
						}	}
					}
				}
				else if (	( IniID == IDS_INI_SF_SIZE ) ||
							( IniID == IDS_INI_PF_SIZE ) ||
							( IniID == IDS_INI_MESSAGE_FONT_SIZE )   )
				{
					INT i = ( ( CComboBox* ) Ctrl ) ->GetCurSel();

					if( ( i >= 0 ) && ( i != GetIniShort( IniID ) ) )
					{
						SetIniShort( IniID, ( short ) i );
					}

					if( IniID != IDS_INI_PF_SIZE )
					{
						VisibleFontChanged = TRUE;
					}
				}
				else if ( (IniID >= IDS_INI_CC_PREVIEW_PROFILE) && (IniID <= IDS_INI_CC_MULTIPLE_PROFILE) )
				{
					// Get the selected item
					CComboBox *		pComboBox = reinterpret_cast<CComboBox *>(Ctrl);

					if ( pComboBox->IsWindowEnabled() )
					{
						int				nSelectedItem = pComboBox->GetCurSel();
						
						if (nSelectedItem != CB_ERR)
						{
							// Get the text for the selected item
							CString		szSelectedItemText;
							pComboBox->GetLBText(nSelectedItem, szSelectedItemText);
							
							// Go ahead and always call SetIniString, because it will make sure
							// that the value is new before bothering to do anything.
							SetIniString(IniID, szSelectedItemText);
						}
					}
				}
				else if (IniID == IDS_INI_WHO_GETS_CTRL_J)
				{
					if ( IsDlgButtonChecked( IDS_INI_WHO_GETS_CTRL_J ) )
						SetIniShort( IDS_INI_WHO_GETS_CTRL_J, CTRL_J_JUNK );
					else
						SetIniShort( IDS_INI_WHO_GETS_CTRL_J, CTRL_J_FILTER );
					CMainFrame::QCGetMainFrame()->InitJunkMenus();
				}
				else if ((Style == BS_AUTOCHECKBOX || Style == BS_AUTORADIOBUTTON) &&
					IniID != IDS_INI_STATIONERY &&
					IniID != IDS_INI_RELAY_PERSONA &&
					IniID != IDS_INI_SIGNATURE_NAME &&
					IniID != IDS_INI_AUTO_CONNECTION_NAME)
				{
					UINT Old = GetIniShort(IniID);
					UINT New = IsDlgButtonChecked(nID);
					
					char *eolchar = NULL;
					char *nexteolchar = NULL;
					char ControlText[64];
					Ctrl->GetWindowText((char *)&ControlText, sizeof(ControlText)-1);
					eolchar = strchr(ControlText, '\n');

					if (eolchar)	// We put in an eol so the game's afoot.
					{
						eolchar++;
						nexteolchar = strchr(eolchar, '\n');
					}

					if (nexteolchar)	// We found 'em both. Good, then we've got a candidate for the \nRES_ID\nBIT_VALUE thing
					{
						nexteolchar++;
						int ResValue;
						int BitValue;

						ResValue = atoi(eolchar);
						BitValue = atoi(nexteolchar);

						LONG FullValue = GetIniLong(ResValue);

						if (New)		// Box was checked
						{
							FullValue = FullValue | BitValue;
						}
						else
						{
							FullValue = FullValue & ~BitValue;
						}

						SetIniLong(ResValue, FullValue);

						// Notify anyone who cares that an INI value has changed.
						// (Note that this obviously only handles a very few cases
						// for settings dialog changes. Add further support for
						// notifying of settings dialog changes as needed).
						NotifyIniUpdates(ResValue);
					}
					else
					{
						// If the high bit of the ID is set, then we want the saved setting
						// to be the opposite of the control value
						if (nID & 0x8000)
							New = !New;
						SetIniShort(IniID, short(New));

						// Notify anyone who cares that an INI value has changed.
						// (Note that this obviously only handles a very few cases
						// for settings dialog changes. Add further support for
						// notifying of settings dialog changes as needed).
						NotifyIniUpdates(IniID);

						if (Old != New)
						{
							switch (IniID)
							{
							case IDS_INI_SHOW_CATEGORY_ICONS:
								SetCategoryHeight();
								m_Categories.SetTopIndex(m_Categories.GetCaretIndex());
								m_Categories.InvalidateRect(NULL);
								break;
								
							case IDS_INI_SHOW_TOOLBAR:
								CFrameWnd* MainWnd;
								MainWnd = (CFrameWnd*)AfxGetMainWnd();
								MainWnd->ShowControlBar(MainWnd->GetControlBar(AFX_IDW_TOOLBAR), New, FALSE);
								MainWnd->RecalcLayout();
								break;
								
							case IDS_INI_SHOW_STATUS_BAR:
								MainWnd = (CFrameWnd*)AfxGetMainWnd();
								MainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR)->ShowWindow(New != NULL);
								MainWnd->RecalcLayout();
								break;

							case IDS_INI_SHOW_TOOLTIPS:
								{
									UINT tempID;

									// First Toggle tips on the Mainfrm toolbar
									CMainFrame* p_mainframe = (CMainFrame*) AfxGetMainWnd();
									ASSERT(p_mainframe->IsKindOf(RUNTIME_CLASS(CMainFrame)));
									p_mainframe->EnableToolTips(New);
								
									// Go through the mdi children and toggle the tips on the comp and read messages
									CWnd *mdiWnd = p_mainframe->GetActiveFrame();
									while (mdiWnd)
									{
										tempID = ((CMDIChild *)mdiWnd)->GetTemplateID();
										if (tempID)
										{
											switch (tempID)
											{
											case IDR_READMESS:
											case IDR_COMPMESS:
												{
													//
													// Now that child windows have multiple
													// toolbars, we need to delegate handling to
													// comp and read message window classes to
													// handle this "internally", just like with
													// CMainFrame above.
													//
													CMessageFrame* p_frame = (CMessageFrame *) mdiWnd;
													ASSERT(p_frame->IsKindOf(RUNTIME_CLASS(CMessageFrame)));
													p_frame->EnableTooltips(New);
												}
												break;
											}
										}
										mdiWnd = mdiWnd->GetWindow(GW_HWNDNEXT);
									}
								}
								break;

							case IDS_INI_MDI_TASKBAR:
								{
									// Set visibility of the Mainfrm task bar
									CMainFrame* p_mainframe = (CMainFrame *) AfxGetMainWnd();
									ASSERT_KINDOF(QCWorkbook, p_mainframe);
									p_mainframe->ShowMDITaskBar(New);
								}
								break;

							case IDS_INI_MAILBOX_PREVIEW_PANE:
							{
								POSITION pos = TocTemplate->GetFirstDocPosition();
								while (pos != NULL)
								{
									CTocDoc* doc = (CTocDoc*)TocTemplate->GetNextDoc(pos);
									ASSERT_KINDOF(CTocDoc, doc);
									CView* View = ( CView* )doc->GetView();
									if (View)
									{
										CTocFrame* pFrame = ( CTocFrame* ) View->GetParentFrame();
										pFrame->ShowHideSplitter();
									}
								}
							}
							break;
								
							case IDS_INI_SAVE_PASSWORD:
								if (Old)
								{
									// If turning Save Password off, then erase passwords in INI file
									g_Personalities.EraseAllPasswords();
									SetIniString(IDS_INI_SAVE_DIALUP_PASSWORD_TEXT);
								}
								else
								{
									// If turning Save Password on, then save passwords in INI file
									g_Personalities.SaveAllPasswords();
								}
								break;
								
							case IDS_INI_IMMEDIATE_SEND:
								{
									POSITION pos = CompMessageTemplate->GetFirstDocPosition();
									while (pos)
									{
										CCompMessageDoc* doc =
								    		(CCompMessageDoc*)CompMessageTemplate->GetNextDoc(pos);
										ASSERT_KINDOF(CCompMessageDoc, doc);
										CView* View;
										if (doc && (View = doc->GetView()))
										{
											CFrameWnd* pFrameWnd = View->GetParentFrame();
											CCompMessageFrame* pCompFrame = DYNAMIC_DOWNCAST(CCompMessageFrame, pFrameWnd);
											if (pCompFrame)
												pCompFrame->PostMessage(WM_USER_UPDATE_IMMEDIATE_SEND);
										}
									}
								}
								break;
						
							case IDS_INI_CONNECT_OFFLINE:
								
								if ( !New )
								{
									break;
								}

							case IDS_INI_CONNECT_WINSOCK:
								/*
							case IDS_INI_ASYNC_DATABASE:
							case IDS_INI_ASYNC_WINSOCK:
								if (NetConnection)
								{
									delete NetConnection;
									NetConnection = NULL;
								}*/
								break;
							
							case IDS_INI_MBOX_SHOW_STATUS:
							case IDS_INI_MBOX_SHOW_PRIORITY:
							case IDS_INI_MBOX_SHOW_ATTACHMENT:
							case IDS_INI_MBOX_SHOW_LABEL:
							case IDS_INI_MBOX_SHOW_SENDER:
							case IDS_INI_MBOX_SHOW_DATE:
							case IDS_INI_MBOX_SHOW_SIZE:
							case IDS_INI_MBOX_SHOW_SERVER_STATUS:
							case IDS_INI_MBOX_SHOW_MOOD:
							case IDS_INI_MBOX_SHOW_JUNK:
							case IDS_INI_MBOX_SHOW_SUBJECT:
							case IDS_INI_SHOW_MAILBOX_LINES:
							case IDS_INI_MOOD_MAIL_CHECK:
							case IDS_INI_USE_JUNK_MAILBOX:
								VisibleFontChanged = TRUE;
								break;

							case IDS_INI_AGE_SENSITIVE_DATE_DISPLAY:
							case IDS_INI_FIXED_DATE_DISPLAY:
							case IDS_INI_SENDER_TIME_DISPLAY:
							case IDS_INI_LOCAL_TIME_DISPLAY:
								DateFormatChanged = TRUE;
								break;

							case IDS_INI_TASKSTATUS_SHOWCOL_TASK:
							case IDS_INI_TASKSTATUS_SHOWCOL_PERSONA:
							case IDS_INI_TASKSTATUS_SHOWCOL_STATUS:
							case IDS_INI_TASKSTATUS_SHOWCOL_DETAILS:
							case IDS_INI_TASKSTATUS_SHOWCOL_PROGRESS:
							{
								bTaskStatusColChanged = true;
							}
							break;

							// Changing the authentication settings in either the "Incoming Mail" panel
							// or the "Kerberos" panel may cause a change in the other panel.  Handle
							// those cases here.
							case IDS_INI_AUTH_KERB:
							{
								if (New)
								{
									// Kerberos was turned on in "Incoming Mail" panel: turn on Kerberos authentication
									// in "Kerberos" panel.
									SetIniShort(IDS_INI_KERB_ON, 1);
									SetIniShort(IDS_INI_KERB_OFF, 0);
								}
								else
								{
									// A non-Kerberos authentication was turned on in "Incoming Mail" panel: turn off
									// Kerberos authentication in "Kerberos" panel.
									SetIniShort(IDS_INI_KERB_ON, 0);
									SetIniShort(IDS_INI_KERB_OFF, 1);
								}
							}
							break;

							case IDS_INI_KERB_ON:
							{
								if (New)
								{
									// Kerberos was turned on in "Kerberos" panel: turn on Kerberos authentication
									// and turn all other authentications off in "Incoming Mail" panel.
									SetIniShort(IDS_INI_AUTH_PASS, 0);
									SetIniShort(IDS_INI_AUTH_APOP, 0);
									SetIniShort(IDS_INI_AUTH_RPA, 0);
									SetIniShort(IDS_INI_AUTH_KERB, 1);
								}
								else
								{
									// Kerberos was turned off in Kerberos panel: turn on password authentication
									// and turn all other authentications off in "Incoming Mail" panel.
									SetIniShort(IDS_INI_AUTH_PASS, 1);
									SetIniShort(IDS_INI_AUTH_APOP, 0);
									SetIniShort(IDS_INI_AUTH_RPA, 0);
									SetIniShort(IDS_INI_AUTH_KERB, 0);
								}
							}
							break;

							case IDS_INI_SHOW_ONLY_IN_TRAY:
							case IDS_INI_ALWAYS_SHOW_TRAY_ICON:
							{
								CMainFrame* p_mainframe = (CMainFrame *) AfxGetMainWnd();
								p_mainframe->SetIcon(-1, -1, TRUE);
							}
							break;

							} // End Switch(IniID)
						} // Old != New
					}
				}
				else
				{
					Ctrl->GetWindowText(Text, sizeof(Text));
				
					switch (IniID)
					{
					case IDS_INI_CHECK_FOR_MAIL:
						int NewCMI;
						NewCMI = atoi(Text);
						if (GetIniShort(IDS_INI_CHECK_FOR_MAIL) != NewCMI)
						{
// !!! WE NEED TO PUT SOMETHING IN HERE TO SET THE NEXT TIME TO CHECK MAIL !!!
							unsigned long newTime = 0U;
							if (NewCMI)
							{
								NewCMI = __max(NewCMI, GetIniShort(IDS_MIN_MAIL_CHECK_INTERVAL));
								newTime = time(NULL) + (60 * NewCMI);
							}
							g_pApp->SetNextMailCheck(newTime);
							g_pApp->UpdateMailCheckLabel();
						}
						break;
						
					case IDS_INI_BIG_MESSAGE_THRESHOLD:
						// Big Message Threshold is in K
						long Num;
						Num = atol(Text) * 1024L;
						wsprintf(Text, "%ld", Num);
						break;
					
					
					case IDS_INI_MANUAL_JUNK_SCORE:
					{
						int		 iUserJunkScore = 0;
						Ctrl->GetWindowText(Text, sizeof(Text));
						iUserJunkScore = atoi(Text);
						if (iUserJunkScore > 100)
						{
							iUserJunkScore = 100;
						}
						else if (iUserJunkScore < 0)
						{
							iUserJunkScore = 0;
						}
						sprintf(Text, "%d", iUserJunkScore);
						break;
					}
					case IDS_INI_JUNK_MIN_AGEOFF_SCORE:
					{
						int		 iMinAgeoffScore = 0;
						Ctrl->GetWindowText(Text, sizeof(Text));
						iMinAgeoffScore = atoi(Text);
						if (iMinAgeoffScore > 100)
						{
							iMinAgeoffScore = 100;
						}
						else if (iMinAgeoffScore < 0)
						{
							iMinAgeoffScore = 0;
						}
						sprintf(Text, "%d", iMinAgeoffScore);
						break;
					}
					case IDS_INI_JUNK_AGEOFF_DAYS:
					{
						int		 iAgeOffDays = 0;
						Ctrl->GetWindowText(Text, sizeof(Text));
						iAgeOffDays = atoi(Text);
						if (iAgeOffDays < 1)
						{
							iAgeOffDays = 1;
						}
						sprintf(Text, "%d", iAgeOffDays);
						break;
					}

					case IDS_INI_CC_MULTIPLE_MAX_NUM:
					{
						int		nConConMultipleMessagesMax = atoi(Text);
						bool	bIllegalValue = false;

						if (nConConMultipleMessagesMax < 0)
						{
							bIllegalValue = true;
						}
						else
						{
							int		nStringLength = strlen(Text);

							for (int i = 0; i < nStringLength; i++)
							{
								if ( (Text[i] < '0') || (Text[i] > '9') )
								{
									bIllegalValue = true;
									break;
								}
							}
						}

						if (bIllegalValue)
						{
							//	Revert to the default value
							CString		szDefaultSettingValue;
							
							GetDefaultIniSetting(IDS_INI_CC_MULTIPLE_MAX_NUM, szDefaultSettingValue);

							if ( szDefaultSettingValue.GetLength() < sizeof(Text) )
								strcpy( Text, static_cast<LPCSTR>(szDefaultSettingValue) );
						}
						break;
					}

					case IDS_INI_POP_ACCOUNT:
					case IDS_INI_REAL_NAME:
					case IDS_INI_RETURN_ADDRESS:
					case IDS_INI_LOGIN_NAME:
					case IDS_INI_POP_SERVER:
							
						// If POP Account, Real Name, Return Address change, clear out ReturnAddress
						// so that the next time GetReturnAddress() is called it gets regenerated
						const char* OldValue;
						OldValue = GetIniString(IniID);
						if (strcmp(OldValue, Text))
						{
							ReturnAddress.Empty();
							// If this is the POP Account changing, then clear out the password
#ifndef DELPHI
							if (IniID == IDS_INI_POP_ACCOUNT) {
								ClearPassword();
								
								// go and reflect this change in the POP Server and LoginName fields
										char szTemp[ 256];
										strcpy( szTemp, Text) ; // just in case.....
										char* pszAtSign = strrchr(szTemp, '@');
										if( pszAtSign) {
											char szTemp1[ 128];
											strcpy( szTemp1, pszAtSign + 1) ;
											*pszAtSign = 0 ;
											SetIniString( IDS_INI_LOGIN_NAME, szTemp) ;
											SetIniString( IDS_INI_POP_SERVER, szTemp1) ;
										}
							}
#endif						
							//Remove this once the POP_ACCOUNT field finally disappers
							if( IniID == IDS_INI_LOGIN_NAME || IniID == IDS_INI_POP_SERVER) {
								//ok, reflect the change in the POP_ACCOUNT
								CString strTemp ;
								if( IniID == IDS_INI_POP_SERVER) {
										strTemp = GetIniString( IDS_INI_LOGIN_NAME) ;
										strTemp += "@" ;
										strTemp += Text;
								}
								if( IniID == IDS_INI_LOGIN_NAME) {
									strTemp = Text ;
									strTemp += "@" ;
									strTemp += GetIniString( IDS_INI_POP_SERVER) ;
								}
								SetIniString( IDS_INI_POP_ACCOUNT, strTemp) ;
								ClearPassword() ;
							}
						}
						break;

					case IDS_INI_STATIONERY:
//FORNOW					case IDS_INI_SIGNATURE_NAME:
						if ( strcmp( Text, m_NoDefaultStr ) == 0 )
							Text[ 0 ] = '\0';	// save an empty string
						break;

					case IDS_INI_RELAY_PERSONA:
						if ( strcmp( Text, m_NoRelayStr) == 0 )
							Text[ 0 ] = '\0';	// save an empty string
						break;

					case IDS_INI_RECENT_DATE_FORMAT:
					case IDS_INI_OLD_DATE_FORMAT:
					case IDS_INI_ANCIENT_DATE_FORMAT:
					case IDS_INI_OLD_DATE_HOURS:
					case IDS_INI_ANCIENT_DATE_HOURS:
						if (strcmp(GetIniString(IniID), Text))
							DateFormatChanged = TRUE;
						break;

					case IDS_INI_LABEL_TEXT_1:
					case IDS_INI_LABEL_TEXT_2:
					case IDS_INI_LABEL_TEXT_3:
					case IDS_INI_LABEL_TEXT_4:
					case IDS_INI_LABEL_TEXT_5:
					case IDS_INI_LABEL_TEXT_6:
					case IDS_INI_LABEL_TEXT_7:
					{
						const UINT nLabelIdx = (IniID - IDS_INI_LABEL_TEXT_1) + 1;
						QCLabelDirector::GetLabelDirector()->SetLabel(nLabelIdx, (LPCSTR)Text);
					}
					break;

					}

					switch (IniID)
					{
						case IDS_INI_ACAP_PASSWORD:
						case IDS_INI_LABEL_TEXT_1:
						case IDS_INI_LABEL_TEXT_2:
						case IDS_INI_LABEL_TEXT_3:
						case IDS_INI_LABEL_TEXT_4:
						case IDS_INI_LABEL_TEXT_5:
						case IDS_INI_LABEL_TEXT_6:
						case IDS_INI_LABEL_TEXT_7:
							break; // Don't write these out

						case IDS_INI_MIN_SCORE_TO_JUNK:
						{
							// Write out the slider position.
							CSliderCtrl		*pSlider = (CSliderCtrl*)GetDlgItem(IDS_INI_MIN_SCORE_TO_JUNK);
							if (pSlider)
							{
								int			iPos = pSlider->GetPos();
								// If the user set the threshold below 25 show them
								// the "We feel your pain" dialog.
								if (iPos < 25)
								{
									if (GetIniShort(IDS_INI_OPTIONS_HIDE_CURSOR))
										::ShowCursor(TRUE);
									if (AlertDialog(IDD_LOW_JUNK_THRESHHOLD_DIALOG) == IDOK)
									{
										// User chose to keep the minimum of 25.
										iPos = 25;
									}
									if (GetIniShort(IDS_INI_OPTIONS_HIDE_CURSOR))
										::ShowCursor(FALSE);
								}
								SetIniShort(IDS_INI_MIN_SCORE_TO_JUNK, (short)iPos);
							}
						}
						break;

						case IDS_INI_MAX_RECENT_MAILBOX:
						{
							BOOL bChanged = FALSE;
							int NewMRM;
							NewMRM = atoi(Text);
							if (GetIniShort(IDS_INI_MAX_RECENT_MAILBOX) != NewMRM)
								bChanged = TRUE;
							SetIniLong(IDS_INI_MAX_RECENT_MAILBOX, NewMRM);
							if (bChanged)
								QCMailboxDirector::UpdateRecentMailboxList(NULL, FALSE);
						}
						break;

						default:
							SetIniString(IniID, Text);
					}
				}
			}

			if ( Ctrl != Wnd 					&&
				 IniID != IDOK					&&
				 IniID != IDHELP					&&
				 IniID != IDCANCEL				&&
				 IniID != IDC_SETTINGS_CAT_LIST	&&
				 IniID != IDC_SETTINGS_CAT_TITLE )
				Ctrl->SetParent(Wnd);
		}

		if (bTaskStatusColChanged) // Check if they changed the task settings
		{
			ASSERT(QCGetTaskStatusView());

			if (QCGetTaskStatusView())
				QCGetTaskStatusView()->PostMessage(msgTaskViewUpdateCols);
		}

		if( VisibleFontChanged )
			SetupFonts();

		if (DateFormatChanged)
		{
			POSITION pos = TocTemplate->GetFirstDocPosition();
			
			while (pos != NULL)
			{
				CTocDoc* doc = (CTocDoc*)TocTemplate->GetNextDoc(pos);
				if (doc && doc->IsKindOf(RUNTIME_CLASS(CTocDoc)))
					doc->ReformatDateFields();
				else
					ASSERT(FALSE);
			}
		}

		if (GetIniShort(IDS_INI_OPTIONS_HIDE_CURSOR))
			::ShowCursor(TRUE);
	}

	// flush and reset the ini cache
	FlushINIFile();
	RemoveAllFromCache();
}


// Fills in a combo box with a string holding items separated by \n
// and selects the current value from the INI file
void CSettingsDialog::FillInCombo(CComboBox* Combo, UINT IniID, UINT StringID)
{
	char Items[256];
	
	GetIniString(StringID, Items, sizeof(Items));
	
	for (char* Start = Items; *Start; )
	{
		char* Separator = strchr(Start, '\n');
		if (Separator)
			*Separator = 0;
			
		Combo->AddString(Start);
		Start += strlen(Start);
		if (Start == Separator)
			Start++;
	}
	
	GetIniString(IniID, Items, sizeof(Items));
	int Index = Combo->FindStringExact(-1, Items);
	if (Index >= 0)
		Combo->SetCurSel(Index);
}

void CSettingsDialog::FillInSignatures( CComboBox* Combo )
{
	char szEntry[ 80 ];

	if ( ! Combo ) return;

	Combo->ResetContent();
	Combo->AddString( m_NoDefaultStr );	// always have a blank option

	g_theSignatureDirector.FillComboBox( Combo );

	//
	// First try -- Bypass the INI cache and check the raw INI settings 
	// file for a signature setting.  If there is no signature setting,
	// then morph the signature setting to STANDARD.TXT.
	//
	char szKey[80];
	QCLoadString( IDS_INI_SIGNATURE_NAME, szKey, sizeof( szKey ) );
	g_Personalities.GetProfileString( "", szKey, "NotThereAtAll", szEntry, sizeof( szEntry ) );
	if ( strcmp( szEntry, "NotThereAtAll" ) == 0 )
	{
		//
		// User has no signature setting in the INI file, so force
		// the STANDARD signature.
		//
		strcpy( szEntry, CRString(IDS_STANDARD_SIGNATURE) );
	}
	else
	{
		//
		// User has a signature setting in the INI file, so honor it.
		//
		GetIniString(IDS_INI_SIGNATURE_NAME, szEntry, sizeof(szEntry));
	}

	int Index = Combo->FindStringExact( -1, szEntry );
	if ( Index >= 0 )
		Combo->SetCurSel( Index );
	else
		Combo->SetCurSel( 0 );
}

void CSettingsDialog::FillInStationery( CComboBox* Combo, UINT id )
{
	char szEntry[ 80 ] = "";

	if ( ! Combo ) return;

	Combo->ResetContent();
	Combo->AddString( m_NoDefaultStr );	// always have a blank option

	g_theStationeryDirector.FillComboBox( Combo );

	// set the selection to the current selection
	GetIniString( IDS_INI_STATIONERY, szEntry, sizeof( szEntry ) );

	int Index = Combo->FindStringExact( -1, szEntry );
	if ( Index >= 0 )
		Combo->SetCurSel( Index );
	else
		Combo->SetCurSel( 0 );
}

void CSettingsDialog::FillInPersonalities( CComboBox* Combo )
{
	char szEntry[ 80 ] = "";

	if ( ! Combo ) return;

	Combo->ResetContent();
	Combo->AddString( m_NoRelayStr );	// always have a blank option

	g_Personalities.FillComboBox( Combo );

	// set the selection to the current selection
	GetIniString( IDS_INI_RELAY_PERSONA, szEntry, sizeof( szEntry ) );

	int Index = Combo->FindStringExact( -1, szEntry );
	if ( Index >= 0 )
		Combo->SetCurSel( Index );
	else
		Combo->SetCurSel( 0 );
}



BEGIN_MESSAGE_MAP(CSettingsDialog, CHelpxDlg)
	//{{AFX_MSG_MAP(CSettingsDialog)
	ON_LBN_SELCHANGE(IDC_SETTINGS_CAT_LIST, OnSelChange)
	ON_CBN_SELCHANGE(IDS_INI_STATIONERY, OnStationerySelChange)
	ON_CBN_SELCHANGE(IDS_INI_RELAY_PERSONA, OnRelaySelChange)
	ON_BN_CLICKED(IDC_ACAP_BUTTON, OnAcapButton)
	ON_BN_CLICKED(IDS_INI_USES_POP, OnPopServer)
	ON_BN_CLICKED(IDS_INI_USES_IMAP, OnImapServer)
	ON_BN_CLICKED(IDS_INI_MOOD_MAIL_CHECK, OnMoodMailCheck)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSettingsDialog message handlers

void CSettingsDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDialog::OnPaint();
}

void CSettingsDialog::OnSize(UINT nType, int cx, int cy)
{
	// Don't call CHelpxDlg::OnSize() when we're initially setting the Form size because
	// this is called from OnInitDialog, and CHelpxDlg::OnSize() just calls Default(), which
	// would cause another WM_INITDIALOG message to be sent and so recursing forever
	if (cx != -1 || cy != -1)
		CHelpxDlg::OnSize(nType, cx, cy);

	// Don't do any of this if the controls aren't created yet, or the window is being minimized
	if (m_Categories.m_hWnd == NULL || nType == SIZE_MINIMIZED)
		return;

	CRect rect, Crect;

	GetClientRect(&rect);
	m_Categories.GetWindowRect(&Crect);
	ScreenToClient(&Crect);
	m_CtrlOffset.x = Crect.right + LOWORD(::GetDialogBaseUnits());
	m_CtrlOffset.y = Crect.top;
}

bool CSettingsDialog::SmartSetSel(int NewSel)
{
	if (m_Categories.GetCurSel() == NewSel)
		return false;

	m_Categories.SetCurSel(NewSel);
	OnSelChange();

	return true;
}

void CSettingsDialog::OnSelChange()
{
	int Sel = m_Categories.GetCurSel();

	// We had previously selected something else, so update the data in this category
	if (m_DialogHWND)
	{
		// UpdateData() fails when there's an invalid entry, so clean up by reselecting the
		// previous category
		if ( !UpdateData() )
		{
			m_Categories.SetCurSel(m_Sel);
			return;
		}

		if ( m_StatCombo )
		{
			delete m_StatCombo;
			m_StatCombo = NULL;
		}

		if ( m_Static )
		{
			delete m_Static;
			m_Static = NULL;
		}

		if ( ::IsWindow( m_DialogHWND ) )
			::DestroyWindow(m_DialogHWND);
		m_DialogHWND = NULL;

		// make sure we have a clean canvas
		CRect DlgRect, CtrlRect;
		GetClientRect(&DlgRect);
		m_Categories.GetWindowRect(&CtrlRect);
		ScreenToClient(&CtrlRect);
		DlgRect.left = CtrlRect.right + 1;
		GetDlgItem(IDOK)->GetWindowRect(&CtrlRect);
		ScreenToClient(&CtrlRect);
		DlgRect.bottom = CtrlRect.top - 1;
		InvalidateRect(&DlgRect);
	}

	m_Sel = Sel;

	CCategory* Cat = (CCategory*)m_Categories.GetItemDataPtr(Sel);
	if (!Cat || Cat == (CCategory*)-1)
	{
		ASSERT(FALSE);
		return;
	}

	// Create a dialog for the selected category.  We save this as an HWND because we don't
	// have a C++ structure for it, and we can't save the return from a CWnd::FromHandle()
	// call because it's only temporary.
	LPCSTR DlgName = MAKEINTRESOURCE( Cat->m_DlgID );
	HINSTANCE hResInst = Cat->m_hDll;
	m_DialogHWND = ::CreateDialog( hResInst, DlgName, m_hWnd, NULL );
	ASSERT(m_DialogHWND);
	if (!m_DialogHWND)
		return;
	CWnd* Wnd = CWnd::FromHandle(m_DialogHWND);

	// Go through each control, initializing with the current setting,
	// and moving to the main dialog in the correct place.
	// Windows adds the new controls to the beginning of the child list,
	// so we need to add the child controls in reverse to get the order of
	// movement between controls to work properly.
	if (GetIniShort(IDS_INI_OPTIONS_HIDE_CURSOR))
		::ShowCursor(FALSE);

	UINT HasLMOS = 0;
	CWnd* Ctrl = Wnd->GetWindow(GW_CHILD);
	if (Ctrl)
		Ctrl = Ctrl->GetWindow(GW_HWNDLAST);
	CWnd* Prev;
	for (; Ctrl; Ctrl = Prev)
	{
		Prev = Ctrl->GetNextWindow(GW_HWNDPREV);

		UINT nID = Ctrl->GetDlgCtrlID();
		UINT IniID = nID & 0x7FFF;
		
		if (IniID == IDS_INI_LEAVE_MAIL_ON_SERVER)
			HasLMOS = nID;

		if (IniID >= 10000 && IniID < 20000)
		{
			DWORD Style = Ctrl->GetStyle() & 0xF;
			if (IniID >= IDS_INI_LABEL_COLOR_1 && IniID <= IDS_INI_LABEL_COLOR_7)
			{
				CLabelColorButton& LCB = m_LabelColor[IniID - IDS_INI_LABEL_COLOR_1];
				LCB.SubclassWindow(Ctrl->m_hWnd);

				const UINT nLabelIdx = (IniID - IDS_INI_LABEL_COLOR_1) + 1;
				LCB.m_Color = (QCLabelDirector::GetLabelDirector()->GetLabelColor(nLabelIdx));
			}
			else if (IniID == IDS_INI_STATIONERY )
			{
				// create a combo owned by CSettingsDialog so CBNotification messages can be routed
				CRect theRect( 10000, 10000, 10001, 10001 );

				// add a control to get an accelerator key to work
				char tmp[ 80 ];
				::GetDlgItemText( m_DialogHWND, IDC_STATIONERY_LABEL, tmp, sizeof( tmp ) );
				m_Static = DEBUG_NEW_MFCOBJ_NOTHROW CStatic;
				if ( m_Static )
					m_Static->Create( tmp, WS_CHILD | WS_VISIBLE, theRect, this );

				Ctrl->GetWindowRect(&theRect);
				Wnd->ScreenToClient(&theRect);
				theRect += m_CtrlOffset;
				theRect.bottom += (theRect.bottom - theRect.top ) * 4;	// size must include dropdown
				m_StatCombo = DEBUG_NEW_MFCOBJ_NOTHROW CComboBox;
				if ( m_StatCombo )
				{
					m_StatCombo->Create( CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_VSCROLL | WS_TABSTOP,
						theRect, this, IDS_INI_STATIONERY );

					// set the font to match the rest of the dialog
					CFont * pFont = Ctrl->GetFont();
					m_StatCombo->SetFont( pFont, TRUE );

					m_StatCombo->ShowWindow( SW_SHOW );
					// Setting the focus here is bad because it steals the focus from the dialog and
					// if the user is arrowing through the entries this derails that process. -dwiggins
					// m_StatCombo->SetFocus();
					Ctrl = (CWnd *)m_StatCombo;

					FillInStationery( (CComboBox*)Ctrl, IniID );
				}
			}
			else if (IniID == IDS_INI_RELAY_PERSONA )
			{
				// create a combo owned by CSettingsDialog so CBNotification messages can be routed
				CRect theRect( 10000, 10000, 10001, 10001 );

				// add a control to get an accelerator key to work
				char tmp[ 80 ];
				::GetDlgItemText( m_DialogHWND, IDC_RELAY_PERSONA_LABEL, tmp, sizeof( tmp ) );
				m_Static = DEBUG_NEW_MFCOBJ_NOTHROW CStatic;
				if ( m_Static )
					m_Static->Create( tmp, WS_CHILD | WS_VISIBLE, theRect, this );

				Ctrl->GetWindowRect(&theRect);
				Wnd->ScreenToClient(&theRect);
				theRect += m_CtrlOffset;
				theRect.bottom += (theRect.bottom - theRect.top ) * 4;	// size must include dropdown
				m_StatCombo = DEBUG_NEW_MFCOBJ_NOTHROW CComboBox;
				if ( m_StatCombo )
				{
					m_StatCombo->Create( CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_VSCROLL | WS_TABSTOP,
						theRect, this, IDS_INI_RELAY_PERSONA);

					// set the font to match the rest of the dialog
					CFont * pFont = Ctrl->GetFont();
					m_StatCombo->SetFont( pFont, TRUE );

					m_StatCombo->ShowWindow( SW_SHOW );
					m_StatCombo->SetFocus();
					Ctrl = (CWnd *)m_StatCombo;

					FillInPersonalities( (CComboBox*)Ctrl );
				}

				// make sure the list box keeps focus
				m_Categories.SetFocus();
			}
			else if (IniID >= IDS_INI_STATISTICS_COLOR_CURRENT && IniID <= IDS_INI_STATISTICS_COLOR_AVERAGE)
			{
				CLabelColorButton& LCB = m_GraphColor[IniID - IDS_INI_STATISTICS_COLOR_CURRENT];
				LCB.SubclassWindow(Ctrl->m_hWnd);

				LCB.m_Color = GetIniLong(IniID);
				
			}
			else if (IniID == IDS_INI_BP_COLOR)
			{
				CLabelColorButton& LCB = m_GraphColor[IniID - IDS_INI_BP_COLOR];
				LCB.SubclassWindow(Ctrl->m_hWnd);
				LCB.m_Color = CBossProtector::GetBPColorCode();
			
			} 
			else if(IniID == IDS_INI_BP_SPEAK)
			{
				//populate the combo 
				CComboBox *		pComboBox = reinterpret_cast<CComboBox *>(Ctrl);
				bool bRes = FillComboWithVoices(pComboBox, m_VoiceGUID, &m_VoiceIndex);
				if(!bRes) {
					m_bHasSpeechEngine = false;
					//set the voice INI option to an empty string
					SetIniString(IDS_INI_BP_SPEAK, "");
					//if speech engine can not be initialized, disable the combo
					pComboBox->EnableWindow(FALSE);
				} else {
					m_bHasSpeechEngine = true;
					pComboBox->SetCurSel(m_VoiceIndex);
				}

			}
			else if (IniID >= IDS_INI_STATISTICS_GRAPH_CURRENT && IniID <= IDS_INI_STATISTICS_GRAPH_AVERAGE)
			{
				
				CString csGraphTypes,csStringToDisplay;
				csGraphTypes.LoadString(IDS_STATISTICS_GRAPH_TYPES);

				int nMatchLoc = 0;
				
				while(nMatchLoc != -1)
				{
					nMatchLoc = csGraphTypes.Find(';');
					csStringToDisplay = csGraphTypes.Left(nMatchLoc);
					csGraphTypes = csGraphTypes.Right(csGraphTypes.GetLength() - (nMatchLoc + 1));
					if (! csStringToDisplay.IsEmpty() )
						( ( CComboBox* ) Ctrl )->AddString( csStringToDisplay );
				}

				INT i = GetIniShort ( IniID );

				( ( CComboBox* ) Ctrl )->SetCurSel( i );

			}
			else if (IniID == IDS_INI_AUTO_CONNECTION_NAME )
			{
				QCRasLibrary::FillRasConnectionNamesCombo( (CComboBox*)Ctrl, ::GetIniString(IDS_INI_AUTO_CONNECTION_NAME));
			}
			else if (IniID == IDS_INI_SIGNATURE_NAME )
			{
				FillInSignatures( (CComboBox*)Ctrl );
			}
			else if(	( IniID == IDS_INI_SF_SIZE ) ||
						( IniID == IDS_INI_PF_SIZE ) ||
						( IniID == IDS_INI_MESSAGE_FONT_SIZE ) )
			{
				( ( CComboBox* ) Ctrl )->AddString( CRString( IDS_FONT_SMALLEST ) );
				( ( CComboBox* ) Ctrl )->AddString( CRString( IDS_FONT_SMALL ) );
				( ( CComboBox* ) Ctrl )->AddString( CRString( IDS_FONT_MEDIUM ) );
				( ( CComboBox* ) Ctrl )->AddString( CRString( IDS_FONT_LARGE ) );
				( ( CComboBox* ) Ctrl )->AddString( CRString( IDS_FONT_LARGEST ) );
				
				UINT u = GetIniShort( IniID ); 

				if( u >= FONTSIZE_COUNT )
				{
					u = 1;
				}

				( ( CComboBox* ) Ctrl )->SetCurSel( u );
			}
			else if(	( IniID == IDS_INI_SCREEN_FONT ) ||
						( IniID == IDS_INI_PRINTER_FONT ) ||
						( IniID == IDS_INI_MESSAGE_FONT ) ||
						( IniID == IDS_INI_MESSAGE_FIXED_FONT ) )
			{
				CStringArray	theFontNames;
				INT				i;
				FONT_TYPE_ENUM	enType = DONT_CARE;

				if ( IniID == IDS_INI_MESSAGE_FONT )
					enType = PROPORTIONAL_ONLY;

				if ( IniID == IDS_INI_MESSAGE_FIXED_FONT )
					enType = FIXED_ONLY;

				EnumFontFaces( theFontNames, enType );
				
				for( i = 0; i < theFontNames.GetSize(); i++ )
				{
					( ( CComboBox* ) Ctrl )->AddString( theFontNames[ i ] );
				}

				CString Text( GetIniString( IniID ) );

				if( ( i = ( ( CComboBox* ) Ctrl )->FindStringExact( CB_ERR, Text ) ) == CB_ERR )
				{
					if( ( i = ( ( CComboBox* ) Ctrl )->FindString( CB_ERR, Text ) ) == CB_ERR )
					{
						( ( CComboBox* ) Ctrl )->SetCurSel( 0 );
					}
				}

				( ( CComboBox* ) Ctrl )->SetCurSel( i );

			}
			else if (IniID == IDS_INI_SSL_RECEIVE_USE || IniID == IDS_INI_SSL_SEND_USE)
			{
				CComboBox* pComboBox = (CComboBox*)Ctrl;
				pComboBox->AddString(CRString(IDS_SSL_NEVER));
				pComboBox->AddString(CRString(IDS_SSL_IFAVAILABLE));
				pComboBox->AddString(CRString(IDS_SSL_ALTERNATEPORT));
				pComboBox->AddString(CRString(IDS_SSL_ALWAYS));
				pComboBox->SetCurSel(GetIniShort(IniID));
			}
			else if ( (IniID >= IDS_INI_CC_PREVIEW_PROFILE) && (IniID <= IDS_INI_CC_MULTIPLE_PROFILE) )
			{
				CComboBox *		pComboBox = reinterpret_cast<CComboBox *>(Ctrl);

				// Add the special "None" entry first
				CRString		szNoneProfileEntry(IDS_CON_CON_PROFILE_NONE);
				pComboBox->AddString(szNoneProfileEntry);

				// Get the profile names from the Content Concentrator
				CStringList		stringList;
				ContentConcentrator::Instance()->GetProfileNames(stringList);

				if (stringList.GetCount() == 0)
				{
					//	No profiles - select "None" and disable the combo box
					pComboBox->SetCurSel(0);
					pComboBox->EnableWindow(FALSE);
				}
				else
				{
					//	Profiles - enable the combo box
					pComboBox->EnableWindow(TRUE);
					
					// Add the profile names
					POSITION		pos = stringList.GetHeadPosition();
					while (pos)
						pComboBox->AddString( stringList.GetNext(pos) );

					// Get the previously selected profile
					CString			szCurrentProfile;
					GetIniString(IniID, szCurrentProfile);

					// Select the previously selected profile unless we no longer have
					// an entry for it, in which case select "None"
					if (pComboBox->SelectString(-1, szCurrentProfile) == CB_ERR)
						pComboBox->SetCurSel(0);
				}
			}
			// Special handling for ctrl-j preference; set if 1, clear if 2...junk
			else if (IniID == IDS_INI_WHO_GETS_CTRL_J)
			{
				int curVal = GetIniShort(IDS_INI_WHO_GETS_CTRL_J);

				if (curVal < CTRL_J_JUNK || curVal > CTRL_J_FILTER)
				{
					ASSERT(0);  	// we should have set this at startup!
					curVal = CTRL_J_JUNK;
				}
				
				Wnd->CheckDlgButton(nID, curVal == CTRL_J_JUNK);
			}
			else if (Style == BS_AUTOCHECKBOX || Style == BS_AUTORADIOBUTTON)
			{
				// Put on your Hack-Goggles(tm). You can find a sickness bag in the back of the seat in front of you.
				////////////////////////////////////
				// Here's where we handle checkboxes and I suppose Radio buttons that turn off
				// and on the bits of a longword INI entry. The most obvious case is that of logging where each bit represents
				// a logging action. The way this works is this: You create a control with an ID that doesn't exist(or it does, that's not important)
				// and you name the id something like "Log Received messages         \n10604\n1" the \n's are the delimiter just
				// because it's used else where. The spaces are to push the \nxxxxx off the edge of the control.
				// The first number is the real INI entry you want to modify and the second number is the value you want to be ORed with the value.
				// Just watch...
				/////////////////////////

				char *eolchar = NULL;
				char *nexteolchar = NULL;
				char ControlText[64];
				Ctrl->GetWindowText((char *)&ControlText, sizeof(ControlText)-1);
				eolchar = strchr(ControlText, '\n');

				if (eolchar)	// We put in an eol so the game's afoot.
				{
					eolchar++;
					nexteolchar = strchr(eolchar, '\n');
				}

				if (nexteolchar)	// We found 'em both. Good, then we've got a candidate for the \nRES_ID\nBIT_VALUE thing
				{
					nexteolchar++;
					int ResValue;
					int BitValue;

					ResValue = atoi(eolchar);
					BitValue = atoi(nexteolchar);

					LONG FullValue = GetIniLong(ResValue);

					if ((FullValue & BitValue) == BitValue)		// This could not be the case if we were to have multi-bit bitflags.
					{
						Wnd->CheckDlgButton(nID, TRUE);

					}
					else
					{
						Wnd->CheckDlgButton(nID, FALSE);
					}
				}
				else
				{

					// If the high bit of the ID is set, then we want the control value
					// to be the opposite of the saved setting
					int Value = GetIniShort(IniID);
					if (nID & 0x8000)
						Value = !Value;
					if (Value)
						Wnd->CheckDlgButton(nID, TRUE);
					if (IniID == IDS_INI_SAVE_PASSWORD && GetIniShort(IDS_DISABLE_SAVE_PASSWORD))
					{
						SetIniShort(IniID, 0);
						Ctrl->EnableWindow(FALSE);
					}

					// Toolbars can be hidden by closing, so check its state for proper setting
					if (IniID == IDS_INI_SHOW_TOOLBAR)
					{
						BOOL vis = ((CFrameWnd*)AfxGetMainWnd())->GetControlBar(AFX_IDW_TOOLBAR)->IsVisible();
						Wnd->CheckDlgButton(nID, vis);
						SetIniShort(IniID, short(vis));
					}
					else if ( IniID == IDS_INI_USE_BIDENT_ALWAYS )
					{
						if( !IsAtLeastMSHTML4_71() )
						{
							Wnd->CheckDlgButton( IDS_INI_USE_BIDENT_ALWAYS, FALSE );
							Ctrl->EnableWindow( FALSE );
						}
					}
					else if (IDS_INI_TASKSTATUS_SHOWCOL_PERSONA == IniID)
					{
						// Task status persona column not visible when you have only one personality
						const BOOL bMultiPers = (g_Personalities.GetPersonaCount() > 1);
						Ctrl->EnableWindow(bMultiPers);
					}
					else if ( IniID == IDS_INI_INTERACTIVE_SPELL_CHECK ) {
					    if ( GetIniShort( IDS_INI_INTERACTIVE_SPELL_CHECK ) )
						Wnd->CheckDlgButton( IniID, TRUE );
					    else
						Wnd->CheckDlgButton( IDC_RADIO_AUTOSPELL, TRUE );
					}
				}
			}
			else if ((IniID >= IDS_INI_LABEL_TEXT_1) && (IniID <= IDS_INI_LABEL_TEXT_7))
			{
				const UINT nLabelIdx = (IniID - IDS_INI_LABEL_TEXT_1) + 1;
				Ctrl->SetWindowText(QCLabelDirector::GetLabelDirector()->GetLabelText(nLabelIdx));
			}
			else if ((IDS_INI_POP_SERVER == IniID) || (IDS_INI_SMTP_SERVER == IniID))
			{
				CString Text(GetIniString(IniID));
				Ctrl->SetWindowText(Text);
			}
			else
			{
				CString Text(GetIniString(IniID));
				if (!Text.IsEmpty())
				{
					if (IniID == IDS_INI_BIG_MESSAGE_THRESHOLD)
					{
						// Big Message Threshold is in K
						char NumBuf[16];
						long Num = atol(Text) / 1024L;
						ltoa(Num, NumBuf, 10);
						Text = NumBuf;
					}
					Ctrl->SetWindowText(Text);
				}
			}
			
			if (IniID == IDS_INI_RETURN_ADDRESS &&
				GetIniShort(IDS_DISABLE_EDIT_RETURN_ADDRESS))
			{
				Ctrl->EnableWindow(FALSE);
			}

			if (IniID == IDS_INI_DONOT_CHECK_BATTERY)
			{
				 // disabling if not on a notebook in DC (battery) mode 
				 // using Win95.
				 SYSTEM_POWER_STATUS systemPower;
				 if (GetSystemPowerStatus(&systemPower))
					Ctrl->EnableWindow(TRUE);
				 else
					Ctrl->EnableWindow(FALSE);
			}
		}

		if ( Ctrl )
		{
			if ( IniID != IDS_INI_STATIONERY && IniID != IDS_INI_RELAY_PERSONA )
			{
				CRect Crect;
				Ctrl->GetWindowRect(&Crect);
				Wnd->ScreenToClient(&Crect);
		
				Crect += m_CtrlOffset;
				Ctrl->SetWindowPos(NULL, Crect.left, Crect.top, 0, 0,
					SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);

				Ctrl->SetParent(this);
			}
		}
	}

#ifdef COMMERCIAL
	// make sure all those context-sensitive checkboxes are set up
	if ( Cat->m_DlgID == IDD_SETTINGS_COMPOSING )
	{
		// change the zorder so the created controls are in the right place
		HWND hPrevWnd = ::GetDlgItem( m_hWnd, IDS_INI_SMTP_SERVER );
		HWND hNextWnd = m_Static->m_hWnd;

		::SetWindowPos( hNextWnd, hPrevWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW );

		hPrevWnd = hNextWnd;
		hNextWnd = m_StatCombo->m_hWnd;

		::SetWindowPos( hNextWnd, hPrevWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW );
	}
#endif //COMMERCIAL

	// This will disable the control after IDS_INI_SPELL_ON_QUEUE
	if ( Cat->m_DlgID == IDD_SETTINGS_SPELL )
	{
		BOOL Enable = IsDlgButtonChecked(IDS_INI_SPELL_ON_QUEUE);
		CWnd* Ctrl = GetDlgItem(IDS_INI_SPELL_ON_QUEUE_WARN);
		if (Ctrl)
			Ctrl->EnableWindow(Enable);
	}

	// This will check whether the dominant account is POP or IMAP
	if ( Cat->m_DlgID == IDD_SETTINGS_INCOMING )
	{
		m_bInitedServType = false;
		if( GetIniShort( IDS_INI_USES_POP)) // POP
		{
			m_bOrigPOP	=	true;
			OnPopServer();
			m_nOriginalServType = SERV_POP;
		}
		else // IMAP
		{
			m_bOrigPOP	=	false;
			OnImapServer();
			m_nOriginalServType = SERV_IMAP;
		}
		m_bInitedServType = true;
	}
	if(Cat->m_DlgID == IDD_SETTINGS_MOODMAIL)
	{
		OnMoodMailCheck();
	}
	if(Cat->m_DlgID == IDD_SETTINGS_JUNK)
	{
		// When setting up Junk panel, set slider ticks at 25, 50 and 75% (0 and 100 already there).
		CSliderCtrl		*pSlider = (CSliderCtrl*)GetDlgItem(IDS_INI_MIN_SCORE_TO_JUNK);
		if (pSlider)
		{
			pSlider->SetTic(25);
			pSlider->SetTic(50);
			pSlider->SetTic(75);
			int			iPos = GetIniShort(IDS_INI_MIN_SCORE_TO_JUNK);
			pSlider->SetPos(iPos);
		}
	}
	if (Cat->m_DlgID == IDD_SETTINGS_FIND_MESSAGES)
	{
		CWnd *		pWnd = GetDlgItem(IDC_INDEX_NOW);
		
		if (pWnd)
		{
			if ( SearchManager::Instance()->IsDoingFullScan() )
				pWnd->SetWindowText( CRString(IDS_STOP_INDEXING_NOW) );
			else
				pWnd->SetWindowText( CRString(IDC_INDEX_NOW) );
		}
	}
	if(Cat->m_DlgID == IDD_SETTINGS_MAILBOXES)
	{
		CWnd *pWnd = GetDlgItem(IDS_INI_MBOX_SHOW_MOOD);
		if (pWnd)
		{
			if(GetIniShort(IDS_INI_MOOD_MAIL_CHECK) && UsingFullFeatureSet())
				pWnd->EnableWindow(TRUE);
			else
				pWnd->EnableWindow(FALSE);
		}
		pWnd = GetDlgItem(IDS_INI_MBOX_SHOW_JUNK);
		if (pWnd)
		{
			if (UsingFullFeatureSet())
				pWnd->EnableWindow(TRUE);
			else
				pWnd->EnableWindow(FALSE);
		}
	}
	// Shareware: Do whatever is needed depending on the feature set
	if (!UsingFullFeatureSet())
	{
		// REDUCED FEATURE mode
		switch (Cat->m_DlgID)
		{
			case IDD_SETTINGS_REPLYING:
			{
				CWnd *pCtrl = GetDlgItem(IDS_INI_REPLY_AUTOFCC);
				ASSERT(pCtrl);
				if (pCtrl)
					pCtrl->EnableWindow(FALSE);

				pCtrl = GetDlgItem(IDS_INI_MULT_REPLIES_FOR_MULT_SEL);
				ASSERT(pCtrl);
				if (pCtrl)
					pCtrl->EnableWindow(FALSE);
			}
			break;
			case IDD_SETTINGS_COMPOSING:
			{
				CWnd *pCtrl = GetDlgItem( IDS_INI_INLINE_SIGNATURE );
				ASSERT( pCtrl );
				if ( pCtrl )
					pCtrl->EnableWindow( FALSE );
			}
			break;

			default:
			{
				// Do nothing
			}
		}
	}
	
	// If this dialog has the Leave mail on server checkbox, then some other
	// controls may need to be disabled if it's turned off
	if (HasLMOS && !IsDlgButtonChecked(HasLMOS))
		SendMessage(WM_COMMAND, MAKEWPARAM(HasLMOS, BN_CLICKED), 0);
	
	if (GetIniShort(IDS_INI_OPTIONS_HIDE_CURSOR))
		::ShowCursor(TRUE);
}

// Obsolete by SHBrowseForFolder -jm
/*
// Needed to get the control IDs for the the controls to hide
#include <dlgs.h>

class CDirDialog : public CFileDialog
{
// Construction
public:
	CDirDialog(UINT nID, CWnd* pParentWnd);

// Attributes
public:
		
// Operations
public:
	BOOL OnInitDialog();

// Implementation
protected:
	CString m_Title;
	CString m_StartDir;
	CString m_Filter;
};

CDirDialog::CDirDialog(UINT nID, CWnd* pParentWnd) :
	CFileDialog(TRUE, NULL, "A", OFN_HIDEREADONLY, NULL, pParentWnd)
{
	m_ofn.Flags &= ~ ( OFN_SHOWHELP | OFN_EXPLORER );
	
	m_Title.LoadString(IDS_FILE_DIR_TITLE);
	m_ofn.lpstrTitle = m_Title;
	
	CWnd* Ctrl = pParentWnd->GetDlgItem(nID);
	if (Ctrl)
		Ctrl->GetWindowText(m_StartDir);
	if (m_StartDir.IsEmpty())
		m_StartDir = EudoraDir;
	m_ofn.lpstrInitialDir = m_StartDir;
	
	m_Filter.LoadString(IDS_ALL_FILES_FILTER);
	for (char* s = m_Filter.GetBuffer(1); *s; s++)
	{
		if (*s == '|')
			*s = 0;
	}
	m_ofn.lpstrFilter = m_Filter;
}

BOOL CDirDialog::OnInitDialog()
{
	if (!CFileDialog::OnInitDialog())
		return (FALSE);
		
	CWnd* FileEdit = GetDlgItem(edt1);
	if (FileEdit)
		FileEdit->ShowWindow(SW_HIDE);
	SetDlgItemText(IDOK, CRString(IDS_USE_DIRECTORY));
		
	return (TRUE);
}
*/

/////////////////////////////////////////////////////////////////////////////
// CFontDlg dialog

class CFontDlg : public CFontDialog
{
// Construction
public:
	CFontDlg(UINT nID, CWnd* pParentWnd);
	~CFontDlg();

// Dialog Data
	//{{AFX_DATA(CFontDlg)
	//}}AFX_DATA

// Attributes
public:
	const UINT m_ID;
	const BOOL m_IsScreen;
	const UINT m_SizeID;
		
// Operations
public:
	BOOL OnInitDialog();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFontDlg)
	afx_msg void OnPaint();
	afx_msg void OnSelChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	BOOL m_MovedSampleText;
	int m_MoveLeft;
};


/////////////////////////////////////////////////////////////////////////////
// CFontDlg dialog



BOOL CSettingsDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (GET_WM_COMMAND_CMD(wParam, lParam) == BN_CLICKED)
	{
		UINT IniID = GET_WM_COMMAND_ID(wParam, lParam) & 0x7FFF;
		switch (IniID)
		{
		case IDHELP:
			{
				CCategory* Cat = (CCategory*)m_Categories.GetItemDataPtr(m_Sel);
				LaunchURLWithQuery(NULL, ACTION_SETTINGSHELP, Cat->m_Name, NULL, NULL, NULL, NULL, ACTION_SETTINGSHELP_PARAM_LABEL);
				// Move the focus to the OK button
				GetDlgItem(IDOK)->SetFocus();
				break;
			}
		case IDS_INI_NEW_MAIL_SOUND:
			{
				CString StartDir(GetIniString(IDS_INI_NEW_MAIL_SOUND));
				CRString SoundTitle(IDS_SOUND_DIALOG_TITLE);
				CRString SoundFilter(IDS_SOUND_FILTER);
				
				if (!StartDir.IsEmpty())
				{
					int Index = StartDir.ReverseFind(SLASH);
					if (Index == -1)
						StartDir = EudoraDir;
					else
						StartDir.GetBufferSetLength(Index);
				}
				
				CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
					SoundFilter, this);
				dlg.m_ofn.Flags &= ~OFN_SHOWHELP;
				dlg.m_ofn.lpstrTitle = SoundTitle;
				dlg.m_ofn.lpstrInitialDir = StartDir;

				if (dlg.DoModal() == IDOK)
				{
					CString selSnd = dlg.GetPathName();
					SetDlgItemText(GET_WM_COMMAND_ID(wParam, lParam), selSnd);
					
					if (m_hNewMailSound)
					{
						MCIWndStop(m_hNewMailSound);
						MCIWndDestroy(m_hNewMailSound);
					}
					m_hNewMailSound = AsyncPlayMedia(selSnd);
				}

			}
			break;
		
		case IDC_USE_DEFAULT_SOUND:
			{
				SetDlgItemText(IDS_INI_NEW_MAIL_SOUND, "");
				HINSTANCE hInst = ::QCFindResourceHandle( MAKEINTRESOURCE(IDW_NEW_MAIL_WAVE), "WAVE" );
				PlaySound(MAKEINTRESOURCE(IDW_NEW_MAIL_WAVE), hInst, SND_ASYNC | SND_NODEFAULT | SND_RESOURCE);
				PurgeIniSetting(IDS_INI_NEW_MAIL_SOUND);
			}
			break;

/*		case IDS_PLAY_SOUND:
			{
				CString selSnd;
				GetDlgItemText(IDS_INI_NEW_MAIL_SOUND, selSnd);
				if (!selSnd.IsEmpty())
					sndPlaySound(selSnd, SND_ASYNC | SND_NODEFAULT);
			}
			break;
*/


			
		case IDS_INI_AUTO_RECEIVE_DIR:
			{
				// Obsolete by SHBrowseForFolder -jm
			/*	CDirDialog dlg(GET_WM_COMMAND_ID(wParam, lParam), this);
				// We want to set only the path, not including the file name
				if (dlg.DoModal() == IDOK)
				{
					SetDlgItemText(GET_WM_COMMAND_ID(wParam, lParam),
						dlg.GetPathName().GetBufferSetLength(dlg.m_ofn.nFileOffset - 1));
				}
			*/

				BROWSEINFO bi;
				char szPath[_MAX_PATH];
				CString szTitle;
				
				
				szTitle.LoadString(IDS_FILE_DIR_TITLE);

				
				bi.hwndOwner = m_DialogHWND;
				bi.pidlRoot = NULL;
				bi.pszDisplayName = NULL;
				bi.lpszTitle = (LPCSTR) szTitle;
				bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI; 
				bi.lpfn = 	BrowseCallbackProc;
				CWnd* Ctrl = this->GetDlgItem(GET_WM_COMMAND_ID(wParam, lParam));
				CString StartDir;
				
				if (Ctrl)
					Ctrl->GetWindowText(StartDir);
				if (StartDir.IsEmpty())
				{
					CFileStatus status;
					StartDir = EudoraDir + "Attach";
					if(!CFile::GetStatus(StartDir, status))
						StartDir = EudoraDir;
				}
				bi.lParam = (LPARAM)(LPCSTR) StartDir;

				CoInitialize(NULL);
				LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

				if(pidl != NULL)
				{
					SHGetPathFromIDList(pidl, szPath);

					LPMALLOC pMalloc;
					SHGetMalloc(&pMalloc);
					pMalloc->Free(pidl);
				
					int lastchar = strlen(szPath);
					szPath[lastchar] = '\0';

					SetDlgItemText(GET_WM_COMMAND_ID(wParam, lParam), szPath);
					
					CExternalLaunchManager::ReplacePathWithNew((LPCTSTR) StartDir, szPath);

				}
				CoUninitialize();


			}
			break;
			
//		case IDS_INI_SCREEN_FONT:
//		case IDS_INI_PRINTER_FONT:
//			{
//				CFontDlg dlg(GET_WM_COMMAND_ID(wParam, lParam), this);
//				if (dlg.DoModal() == IDOK)
//				{
//					char SizeBuf[8];
//					CString Text;
//					int Size;
//					
///					if (dlg.m_cf.Flags & CF_NOFACESEL)
//						Text = GetIniString(dlg.m_ID);
///					else
//						Text = dlg.GetFaceName();
//					
//					if (dlg.m_cf.Flags & CF_NOSIZESEL)
//						Size = GetIniShort(dlg.m_SizeID);
//					else
//						Size = (dlg.GetSize() + 5) / 10;
					
					//
					// Append the font size to the font name and update
					// the text on the font name button.
					//
//					wsprintf(SizeBuf, "%d", Size);
//					Text += ',';
//					Text += SizeBuf;
//					SetDlgItemText(dlg.m_ID, Text);
//				}
//			}
//			break;
		
		case IDS_INI_LABEL_COLOR_1:
		case IDS_INI_LABEL_COLOR_2:
		case IDS_INI_LABEL_COLOR_3:
		case IDS_INI_LABEL_COLOR_4:
		case IDS_INI_LABEL_COLOR_5:
		case IDS_INI_LABEL_COLOR_6:
		case IDS_INI_LABEL_COLOR_7:
			{
				CLabelColorButton& LCB = m_LabelColor[IniID - IDS_INI_LABEL_COLOR_1];
				CColorDialog dlg(LCB.m_Color,  CC_RGBINIT, this);
				dlg.clrSavedCustom[0] = LCB.m_Color;
					
				dlg.m_cc.Flags &= ~CC_SHOWHELP;
				if (dlg.DoModal() == IDOK)
				{
					LCB.m_Color = dlg.GetColor();
					LCB.Invalidate(FALSE);
				}
			}
			break;

		case IDS_INI_SPELL_ON_QUEUE:
			{
				// This will disable the control after IDS_INI_SPELL_ON_QUEUE
				BOOL Enable = IsDlgButtonChecked(IDS_INI_SPELL_ON_QUEUE);
				CWnd* Ctrl = GetDlgItem(IDS_INI_SPELL_ON_QUEUE_WARN);
				if (Ctrl)
					Ctrl->EnableWindow(Enable);
			}
			break;

		case IDC_DIALUP_ENTRY_NEW:
			{
				CString strNewEntry;
				if(!QCRasLibrary::IsReady())
				{
					ErrorDialog(IDS_STRING8204);
					break;
				}

				CStringList oldEntries;
				QCRasLibrary::GetRasConnectionList(oldEntries);

				if( QCRasLibrary::m_pfnRasCreatePhonebookEntry(m_hWnd, NULL) == 0)
				{
					//Fill the Phonebook entries combobox again
					CStringList newEntries;
					const  char *pszEntry = NULL; 
					QCRasLibrary::GetRasConnectionList(newEntries);

					POSITION newpos = newEntries.GetHeadPosition();
					POSITION oldpos;
					bool bFound = false;
					while(newpos && !bFound)
					{
						pszEntry = newEntries.GetNext(newpos);
						oldpos = oldEntries.Find( pszEntry);
						if(!oldpos)
						{
							strNewEntry = pszEntry;
							bFound = true;
						}
					}
					
					if(strNewEntry != "") //found new entry
					{
						//set the index
						CComboBox *pComboBox = static_cast<CComboBox *>(GetDlgItem(IDS_INI_AUTO_CONNECTION_NAME));
						int nIndex = pComboBox->AddString(strNewEntry);
				
						//set the index to the newly created entry name
						//int nIndex = pComboBox->FindStringExact(-1, strNewEntry);
						if (nIndex != CB_ERR)
							pComboBox->SetCurSel(nIndex);
						
					}

					
				}
				
			}
			break;
		case IDC_DIALUP_ENTRY_EDIT:
			{
				CComboBox *pCombo = static_cast<CComboBox *>(GetDlgItem(IDS_INI_AUTO_CONNECTION_NAME));
				CString strEntry;
				if(pCombo)
				{
					int i = pCombo->GetCurSel();

					if( i != CB_ERR )
					{
						pCombo->GetLBText( i, strEntry );
					}					
				}
				if(strEntry != "")
				{
					if(QCRasLibrary::IsReady())
					{
						QCRasLibrary::m_pfnRasEditPhonebookEntry(m_hWnd, NULL, strEntry);
					}
				}

			}
			break;

		case IDS_INI_IMAP_TRASH_MBOXNAME:
			if ( SelectTrashMailbox() == TRUE )
			{

			}
		
			break; 


// We're not going to do this
//		case IDS_INI_LEAVE_MAIL_ON_SERVER:
//			{
//				// Enable/disable the next 8 controls because they depend on LMOS
//				BOOL Enable = IsDlgButtonChecked(GET_WM_COMMAND_ID(wParam, lParam));
//				CWnd* Ctrl = GetDlgItem(GET_WM_COMMAND_ID(wParam, lParam));
//				for (int i = 8; i > 0 && Ctrl; i--)
//				{
//					Ctrl = Ctrl->GetNextWindow();
//					if (Ctrl)
//						Ctrl->EnableWindow(Enable);
//				}
//			}
//			break;


		case IDS_INI_STATISTICS_COLOR_CURRENT:
		case IDS_INI_STATISTICS_COLOR_PREVIOUS:
		case IDS_INI_STATISTICS_COLOR_AVERAGE:
			{
				CLabelColorButton& LCB = m_GraphColor[IniID - IDS_INI_STATISTICS_COLOR_CURRENT];
				CColorDialog dlg(LCB.m_Color,  CC_RGBINIT, this);
				dlg.clrSavedCustom[0] = LCB.m_Color;
					
				dlg.m_cc.Flags &= ~CC_SHOWHELP;
				if (dlg.DoModal() == IDOK)
				{
					LCB.m_Color = dlg.GetColor();
					LCB.Invalidate(FALSE);
				}
			}
			break;
		case IDS_INI_BP_COLOR:
			{
				CLabelColorButton& LCB = m_GraphColor[IniID - IDS_INI_BP_COLOR];
				CColorDialog dlg(LCB.m_Color,  CC_RGBINIT, this);
				dlg.clrSavedCustom[0] = LCB.m_Color;
					
				dlg.m_cc.Flags &= ~CC_SHOWHELP;
				if (dlg.DoModal() == IDOK)
				{
					LCB.m_Color = dlg.GetColor();
					LCB.Invalidate(FALSE);
				}
			}
			break;
		case IDC_STATISTICS_RESET:
			{
				TCHAR szDate[64];
				USGetStartTime(szDate);
				if (WarnYesNoDialog(0,IDS_STATISTICS_RESET_WARN,szDate) == IDOK)
				{
					// Reset the statistics
					ResetStatistics();
				}

			}
			break;

		// This is the setting for Tools->Options->Extra Warnings->Launch a program externally.
		case IDS_INI_WARN_EXTERNAL_LAUNCH_PROGRAM:
			{
				SetIniShort(IDS_INI_WARN_EXTERNAL_LAUNCH_PROGRAM, (short)IsDlgButtonChecked(IDS_INI_WARN_EXTERNAL_LAUNCH_PROGRAM));				
				CExternalLaunchManager::RecacheDataFromRegistry();		
			}
			break;

		case IDC_LAST_SSL_RECEIVE:
		case IDC_LAST_SSL_SEND:
			{
				CSSLConnectionDlg dlg;
				CString Protocol("SMTP");
				CString DominantPersona;

				if (IniID == IDC_LAST_SSL_RECEIVE)
				{
					if (GetIniShort(IDS_INI_USES_POP))
					{
						Protocol = _T("POP");
					}
					else
					{
						Protocol = _T("IMAP");
						// The ConnectionInfo map expects IMAP to have a personality of "<Dominant>" not "".
						DominantPersona = CRString(IDS_DOMINANT);
					}
				}

				dlg.Run(DominantPersona, Protocol);
			}
			break;

		case IDC_INDEX_NOW:
			{
				CWnd *	pWnd = GetDlgItem(IDC_INDEX_NOW);
				
				if ( SearchManager::Instance()->IsDoingFullScan() )
				{
					SearchManager::Instance()->StopScanning();
					pWnd->SetWindowText( CRString(IDC_INDEX_NOW) );
				}
				else
				{
					SearchManager::Instance()->StartScanning();
					pWnd->SetWindowText( CRString(IDS_STOP_INDEXING_NOW) );
				}
			}
			break;

		case IDC_REINDEX:
			{
				//	Initiate re-indexing, but only if the user is really sure.
				int			nResponse = WarnOneOptionCancelDialogWithDefButton(0, IDS_REINDEX_MSG, IDC_REINDEX, IDCANCEL);

				if (nResponse != IDCANCEL)
					SearchManager::Instance()->ReIndex();
			}
			break;
		}
	}

	// fix the dreaded Personality GPF
	if ( ! lParam || ::IsWindow( (HWND)lParam ) )
		return (CHelpxDlg::OnCommand(wParam, lParam));
	else
		return TRUE;
}
// Setting Start Dir in SHBrowseForFolders
int CALLBACK CSettingsDialog::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch(uMsg)
	{
		case BFFM_INITIALIZED:
			::SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
		break;
		//    not interested in other cases
 
	}
 
	return 0;
}
 

void CSettingsDialog::OnOK()
{
	CRString		sDominant(IDS_DOMINANT);

	// first let MFC take its course, so that it calls DoDataExchange()
	// Once that is done then go and check if we need to call the IMAP
	// functions
	CDialog::OnOK() ;

	// check if we need to call the IMAP functions

	if( m_bCallImapFunctions) {
#ifdef IMAP4
		g_theMailboxDirector.ModifyPersona (sDominant);
#endif
	}

	// Finally go and update the personality Wazooooooooooooooooooooooo
	// because user might have changed one or more settings
	CPersonalityView *pView	= CMainFrame::QCGetMainFrame()->GetActivePersonalityView() ;
	if (pView)
		pView->PopulateView() ;
		
	//we have hit OK
	//handle the option - Hide address books from simple viruses
	//make sure that if the user has changed the address book extension from txt to nnt or vice versa
	//we have to change all the address book file extensions
	//NOTE : Change only the NNDBase file and the files in the nickname directory
	//do not mess with the shared address books
	//this option does not have any effect on the shared address books

	//check if the option has recently changed by checking against the value that was read in during startup

	short bHideABookFromVirusAfter = GetIniShort(IDS_INI_HIDE_ABOOK_FROM_VIRUS);
	bool bChangeABookExt = (bHideABookFromVirusBefore != bHideABookFromVirusAfter);
	if(bChangeABookExt) {
			
		if(bHideABookFromVirusAfter == 1)  //if change to NNT extension
			RenameABookExtensions(CRString(IDS_NICK_DBASE_NAME), CRString(IDS_NICK_SAFE_DBASE_NAME), CRString(IDS_NICK_FILE_EXTENSION), CRString(IDS_SAFE_NICK_FILE_EXTENSION));
		else if(bHideABookFromVirusAfter == 0) //if change to txt extensions
			RenameABookExtensions(CRString(IDS_NICK_SAFE_DBASE_NAME), CRString(IDS_NICK_DBASE_NAME), CRString(IDS_SAFE_NICK_FILE_EXTENSION), CRString(IDS_NICK_FILE_EXTENSION));

		UpdateNicknameWithNewContainingNickfile();
	}
	
	CBossProtector::Build();
}


void CSettingsDialog::OnStationerySelChange()
{
	// used to diable the checkbox controls
}


void CSettingsDialog::OnRelaySelChange()
{
	// used to diable the checkbox controls
}



/////////////////////////////////////////////////////////////////////
////   ACAP CODE BELOW ---- BEWARE


void CSettingsDialog::OnAcapButton()
{
	CAcapSettings *acapSettings = DEBUG_NEW CAcapSettings;
	CString strServer;
	CString strUser;
	CString strPassword;

	GetDlgItemText(IDS_INI_ACAP_SERVER, strServer);
	GetDlgItemText(IDS_INI_ACAP_USER_ID, strUser);
	GetDlgItemText(IDS_INI_ACAP_PASSWORD, strPassword);
	
	CWnd* CtrlACAPButton = GetDlgItem(IDC_ACAP_BUTTON);
	CWnd* CtrlOKButton = GetDlgItem(IDOK);
	CWnd* CtrlCANCELButton = GetDlgItem(IDCANCEL);

	if (CtrlACAPButton)
		CtrlACAPButton->EnableWindow(FALSE);
	if (CtrlOKButton)
		CtrlOKButton->EnableWindow(FALSE);
	if (CtrlCANCELButton)
		CtrlCANCELButton->EnableWindow(FALSE);
	
	//Don't let the user switch option panels while we're doing this...
	m_bACAPBusy = TRUE;
	BeginWaitCursor();
	acapSettings->RetrieveSettings(strServer, strUser, strPassword);

	delete acapSettings;
	
	CtrlACAPButton->EnableWindow(TRUE);
	CtrlOKButton->EnableWindow(TRUE);
	CtrlCANCELButton->EnableWindow(TRUE);
	EndWaitCursor();
	m_bACAPBusy = FALSE;
}

bool CSettingsDialog::HasActiveTasks(ServType nServ)
{
	if(!m_bInitedServType)
		return false;

	//if we only switch to a different server type
	if(nServ != m_nOriginalServType)
	{
		//always dominant personality
		if( QCGetTaskManager()->GetTaskCount(CRString(IDS_DOMINANT)) != 0)
		{
			AfxMessageBox(CRString(IDS_CANT_CHANGE_PERSONA));
			
			const bool bPOP = (m_nOriginalServType == SERV_POP);
			const bool bIMAP = (m_nOriginalServType == SERV_IMAP);

			((CButton *) GetDlgItem(IDS_INI_USES_POP))->SetCheck(bPOP);
			((CButton *) GetDlgItem(IDS_INI_USES_IMAP))->SetCheck(bIMAP);

			return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////
//Code for showing and hiding the various controls in the "Incoming Mail" Dialog

void CSettingsDialog::OnPopServer()
{
	if( HasActiveTasks(SERV_POP))
		return;

	ShowServerSpecificControls(SERV_POP);
}

// --------------------------------------------------------------------------

void CSettingsDialog::OnImapServer()
{
	if( HasActiveTasks(SERV_IMAP))
		return;

	ShowServerSpecificControls(SERV_IMAP);
}

void CSettingsDialog::OnMoodMailCheck()
{
	if(((CButton*)GetDlgItem(IDS_INI_MOOD_MAIL_CHECK))->GetCheck())
	{
		EnableMoodControls(TRUE);
	}
	else
	{
		EnableMoodControls(FALSE);
	}
}
// --------------------------------------------------------------------------

void CSettingsDialog::ShowControl(int nID, bool bShow)
{
	GetDlgItem(nID)->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
}

// --------------------------------------------------------------------------

void CSettingsDialog::EnableControl(int nID, bool bEnable)
{
	GetDlgItem(nID)->EnableWindow(bEnable ? TRUE : FALSE);
}

// --------------------------------------------------------------------------

void CSettingsDialog::ShowServerSpecificControls(ServType nServ)
{
	const bool bPOP = (nServ == SERV_POP);
	const bool bIMAP = (nServ == SERV_IMAP);

	// Explicitly set the button otherwise when you tab over it'll select the POP button no matter what.
	((CButton *)GetDlgItem(IDS_INI_USES_IMAP))->SetCheck(bIMAP);
	((CButton *)GetDlgItem(IDS_INI_USES_POP))->SetCheck(bPOP);

	ShowControl(IDS_INI_AUTH_KERB, true);
	ShowControl(IDS_INI_AUTH_APOP, true);

	// POP controls
	ShowControl(IDS_INI_SKIP_BIG_MESSAGES, bPOP);
	ShowControl(IDS_INI_BIG_MESSAGE_THRESHOLD, bPOP);
	ShowControl(IDS_INI_LEAVE_MAIL_ON_SERVER, bPOP);
	ShowControl(IDS_INI_DELETE_MAIL_FROM_SERVER, bPOP);
	ShowControl(IDS_INI_LEAVE_ON_SERVER_DAYS, bPOP);
	ShowControl(IDS_INI_SERVER_DELETE, bPOP);
	ShowControl(IDS_INI_AUTH_RPA, bPOP);
	ShowControl(IDS_INI_CONNECT_OFFLINE, bPOP);
	ShowControl(IDC_STAT_INCOMING_DAYS, bPOP);
	ShowControl(IDC_STAT_INCOMING_KSIZE, bPOP);

	// IMAP controls
	ShowControl(IDS_INI_IMAP_PREFIX, bIMAP);
	ShowControl(IDS_INI_IMAP_OMITATTACH, bIMAP);
	ShowControl(IDS_INI_IMAP_MINDNLOAD, bIMAP);
	ShowControl(IDS_INI_IMAP_MAXSIZE, bIMAP);
	ShowControl(IDC_STAT_INCOMING_MAILBPREFIX, bIMAP);
	ShowControl(IDC_STAT_INCOMING_DOWNLOAD, bIMAP);
	ShowControl(IDC_STAT_INCOMING_K, bIMAP);
	ShowControl(IDC_STAT_IDELETE_MSG, bIMAP);
	ShowControl(IDS_INI_IMAP_MARK_DELETED, bIMAP);
	ShowControl(IDS_INI_IMAP_XFERTOTRASH, bIMAP);
	ShowControl(IDS_INI_IMAP_TRASH_MBOXNAME,	bIMAP);

	// Change the text of the "APOP" to "CRAM-MD5" for IMAP
	GetDlgItem(IDS_INI_AUTH_APOP)->SetWindowText(bIMAP ? "&Cram-MD5" : "&APOP");

	VerifyAuthCheck();
}

// --------------------------------------------------------------------------

void CSettingsDialog::VerifyAuthCheck()
{
	// This check is bogus if the Options dialog itself isn't visible
	if (IsWindowVisible())
	{
		bool bSetCheck = true;
		int nCheckedID = GetCheckedRadioButton(IDS_INI_AUTH_PASS, IDS_INI_AUTH_RPA);

		if (nCheckedID != 0)
		{
			if (GetDlgItem(nCheckedID)->IsWindowVisible() == TRUE)
				bSetCheck = false;
			else
				((CButton*)GetDlgItem(nCheckedID))->SetCheck(0); // Uncheck hidden check
		}

		if (bSetCheck)
			((CButton*)GetDlgItem(IDS_INI_AUTH_PASS))->SetCheck(1);
	}
}


//----------------------------------------------------------------------------
#include "Controls.h"

BOOL CSettingsDialog::SelectTrashMailbox()
{
	BOOL bModified = false;
	CRect	rect;

	// This applies only to  is not an IMAP personality.
	// 
	CRString szPersName (IDS_DOMINANT);

	if ( !g_Personalities.IsImapPersona (szPersName) )
	{
		ASSERT (0);
		return FALSE;
	}

	// Create an invisible CMailboxButton over the current one.
	//
	CWnd* pWnd = GetDlgItem(IDS_INI_IMAP_TRASH_MBOXNAME);
	if (!pWnd)
		return FALSE;
	
	pWnd->GetWindowRect(&rect);

	ScreenToClient(&rect);

	CMailboxButton mboxButton;

	mboxButton.Create ( "", BS_PUSHBUTTON | WS_TABSTOP, 
							rect, this, IDS_INI_IMAP_TRASH_MBOXNAME + 1);

	if (IsWindow(mboxButton.m_hWnd))
	{
		bModified = mboxButton.SelectMailbox(szPersName);

		if (bModified)
		{
			// Get the new selection.
			//
			if ( mboxButton.m_bIsImap )
			{
				AfxSetWindowText(pWnd->m_hWnd, mboxButton.m_SelectedImapName);
			}
			else
			{
				bModified = FALSE;
			}
		}
	}

	return (bModified); // We return true if the mailbox was changed
}

void CSettingsDialog::EnableMoodControls(BOOL nStatus)
{
	CWnd *pWnd;
	pWnd = GetDlgItem(IDS_INI_MOOD_MAIL_MIGHTOFFENSE);
	if (pWnd)
		pWnd->EnableWindow(nStatus);

	pWnd = GetDlgItem(IDS_INI_MOOD_MAIL_PROBABLYOFFENSE);
	if (pWnd)
		pWnd->EnableWindow(nStatus);

	pWnd = GetDlgItem(IDS_INI_MOOD_MAIL_FIRE);
	if (pWnd)
		pWnd->EnableWindow(nStatus);

	pWnd = GetDlgItem(IDS_INI_MOOD_MAIL_NEVER);
	if (pWnd)
		pWnd->EnableWindow(nStatus);

	pWnd = GetDlgItem(IDS_INI_MOOD_MAIL_CHECK_ORIGINAL);
	if (pWnd)
		pWnd->EnableWindow(nStatus);

	pWnd = GetDlgItem(IDS_INI_MOOD_MAIL_DELAY_MIGHTOFFENSE);
	if (pWnd)
		pWnd->EnableWindow(nStatus);

	pWnd = GetDlgItem(IDS_INI_MOOD_MAIL_DELAY_PROBABLYOFFENSE);
	if (pWnd)
		pWnd->EnableWindow(nStatus);

	pWnd = GetDlgItem(IDS_INI_MOOD_MAIL_DELAY_FIRE);
	if (pWnd)
		pWnd->EnableWindow(nStatus);

	pWnd = GetDlgItem(IDS_INI_MOOD_MAIL_DELAY_NEVER);
	if (pWnd)
		pWnd->EnableWindow(nStatus);

	pWnd = GetDlgItem(IDS_INI_MOOD_MAIL_INCOMING);
	if (pWnd)
		pWnd->EnableWindow(nStatus);

	pWnd = GetDlgItem(IDS_INI_MOODCHECK_BACKGROUND);
	if (pWnd)
		pWnd->EnableWindow(nStatus);
}
