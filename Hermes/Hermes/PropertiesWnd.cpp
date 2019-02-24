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

#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "Hermes.h"
#include "HermesSettings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Message Map
BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar
CPropertiesWnd::CPropertiesWnd() noexcept
{
	m_nComboHeight = 0;
}


/////////////////////////////////////////////////////////////////////////////
CPropertiesWnd::~CPropertiesWnd()
{
}


/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers
void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() -(m_nComboHeight+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}


/////////////////////////////////////////////////////////////////////////////
// OnCreate
int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}

	m_wndObjectCombo.AddString(_T("Application"));
	m_wndObjectCombo.AddString(_T("Properties Window"));
	m_wndObjectCombo.SetCurSel(0);

	CRect rectCombo;
	m_wndObjectCombo.GetClientRect (&rectCombo);

	m_nComboHeight = rectCombo.Height();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Locked */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// OnSize
void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}


/////////////////////////////////////////////////////////////////////////////
// void CPropertiesWnd::OnExpandAllProperties()
void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}


/////////////////////////////////////////////////////////////////////////////
// void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}


/////////////////////////////////////////////////////////////////////////////
// void CPropertiesWnd::OnSortProperties()
void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}


/////////////////////////////////////////////////////////////////////////////
// void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}


/////////////////////////////////////////////////////////////////////////////
// void CPropertiesWnd::OnProperties1()
void CPropertiesWnd::OnProperties1()
{
	// TODO: Add your command handler code here
}

/////////////////////////////////////////////////////////////////////////////
// void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Linked to lightening button. Save on demand.
//
//	Save the settings.
//
//	07/02/19	-	SBROTHY	-	Made explicit.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPropertiesWnd::OnProperties2()
{
	CHermesApp *pApp = reinterpret_cast<CHermesApp *>(::AfxGetApp());
	ASSERT(pApp);
	pApp->GetSettings()->Save();
}

/////////////////////////////////////////////////////////////////////////////
// void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPropertiesWnd::LoadI18NPropNames()
{
	TRACE("void CPropertiesWnd::LoadI18NPropNames() >> \n");

	// Apperance 
	ASSERT(m_strI18NPropNameAppearence.LoadString(IDS_I18N_PROP_NAME_APPEARENCE));

	ASSERT(m_strI18NPropName3DLook.LoadString(IDS_I18N_PROP_NAME_3DLOOK));
	ASSERT(m_strI18NPropName3DLookComment.LoadString(IDS_I18N_PROP_NAME_APPEARENCE_COMMENT));
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Properties list. [SBROTHY]
//
//
//	
//
//	07/02/19	-	SBROTHY	-	Tried to dissec into logical sections. Could probably use more work but, it's easier to wreck something than to fix something.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPropertiesWnd::InitPropList()
{
	CHermesApp *pApp = dynamic_cast<CHermesApp *>(::AfxGetApp());
	CHermesSettings *pSettings = pApp->GetSettings();
	ASSERT(pSettings);

	CComVariant *cv = nullptr;

	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Appearance"));

	////
	cv = pSettings->GetSet3DLook();
	//	ASSERT(cv);
	//	ASSERT(cv->vt == VT_BOOL);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("3D Look"), (_variant_t)(cv->boolVal == VARIANT_TRUE ? true : false), _T("Specifies the window's font will be non-bold and controls will have a 3D border")));
	pGroup1->SetData(THREEDLOOK);

	////
	cv = pSettings->GetSetCaption();
	//	ASSERT(cv);
	CMFCPropertyGridProperty *pCaption = new CMFCPropertyGridProperty(_T("Caption"), (_variant_t)(cv->bstrVal), _T("Specifies the text that will be displayed in the window's title bar"));
	pCaption->SetData(CAPTION);

	pGroup1->AddSubItem(pCaption);

	////
	cv = pSettings->GetSetBorder();
	//	ASSERT(cv);
	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Border"), (_variant_t)(cv->bstrVal), _T("One of: None, Thin, Resizable, or Dialog Frame"));
	pProp->AddOption(_T("None"));
	pProp->AddOption(_T("Thin"));
	pProp->AddOption(_T("Resizable"));
	pProp->AddOption(_T("Dialog Frame"));
	pProp->AllowEdit(FALSE);
	pProp->SetData(BORDER);

	pGroup1->AddSubItem(pProp);

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("Window Size"), 0, TRUE);

	////
	cv = pSettings->GetSetWindowHeight();
	//	ASSERT(cv);
	pProp = new CMFCPropertyGridProperty(_T("Height"), (_variant_t)(cv->uiVal), _T("Specifies the window's height"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pProp->SetData(WINDOWHEIGHT);
	pSize->AddSubItem(pProp);

	////
	cv = pSettings->GetSetWindowWidth();
	//	ASSERT(cv);
	pProp = new CMFCPropertyGridProperty(_T("Width"), (_variant_t)(cv->uiVal), _T("Specifies the window's width"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pProp->SetData(WINDOWWIDTH);
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

/*
	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Font"));
	CComVariant *pFont = pSettings->GetSetFont();
	LOGFONT* lf = (LOGFONT *)pFont->pvRecord;
	CFont* font = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(lf);
	_tcscpy_s(lf->lfFaceName, _T("Arial"));

	CMFCPropertyGridFontProperty *f = new CMFCPropertyGridFontProperty(_T("Font"), *lf, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the default font for the window"));
	f->SetData(FONT);
	pGroup2->AddSubItem(f);



	////
	cv = pSettings->GetSetUseSystemFont();
	//	ASSERT(cv);
	//	ASSERT(cv->vt == VT_BOOL);
	CMFCPropertyGridProperty * useSysFont = new CMFCPropertyGridProperty(_T("Use System Font"), (_variant_t)(cv->boolVal == VARIANT_TRUE ? true : false), _T("Specifies that the window uses MS Shell Dlg font"));
	useSysFont->SetData(USESYSTEMFONT);
	pGroup2->AddSubItem(useSysFont);

	m_wndPropList.AddProperty(pGroup2);
*/


	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Misc"));

	cv = pSettings->GetSetName();
	CMFCPropertyGridProperty *pName = new CMFCPropertyGridProperty(_T("Name"), (_variant_t)cv->bstrVal, _T("Some Name thingy"));
	//pName->Enable(FALSE);
	pName->SetData(NAME);
	pGroup3->AddSubItem(pName);


	cv = pSettings->GetSetWindowColor();
	//	RGB col = (RGB)(cv->lVal);
	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Window Color"), RGB(210, 192, 254), nullptr, _T("Specifies the default window color"));
	pColorProp->EnableOtherButton(_T("Other..."));
	pColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
	pColorProp->SetData(WINDOWCOLOR);
	pGroup3->AddSubItem(pColorProp);

	/*
	static const TCHAR szFilter[] = _T("Icon Files(*.ico)|*.ico|All Files(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Icon"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("Specifies the window icon")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Folder"), _T("c:\\")));
	*/

	m_wndPropList.AddProperty(pGroup3);

	InitPropListGettingStarted();
	InitPropListCheckingMail();
	InitPropListIncomingMail();
}


/////////////////////////////////////////////////////////////////////////////
// void CPropertiesWnd::InitPropListGettingStarted()

void CPropertiesWnd::InitPropListGettingStarted()
{
	CHermesApp *pApp = dynamic_cast<CHermesApp *>(::AfxGetApp());
	CHermesSettings *pSettings = pApp->GetSettings();
	ASSERT(pSettings);

	CComVariant *cv = nullptr;

	CMFCPropertyGridProperty* pGettingsStarted = new CMFCPropertyGridProperty(_T("Getting Started"));

	cv = pSettings->GetSetRealName();
	CMFCPropertyGridProperty *pRealName = new CMFCPropertyGridProperty(_T("Real Name"), (_variant_t)(cv->bstrVal), _T("User's real name"));
	pRealName->SetData(REALNAME);
	pGettingsStarted->AddSubItem(pRealName);

	cv = pSettings->GetSetEMail();
	CMFCPropertyGridProperty *pEMail = new CMFCPropertyGridProperty(_T("EMail Address"), (_variant_t)(cv->bstrVal), _T("User's EMail"));
	pEMail->SetData(EMAIL);
	pGettingsStarted->AddSubItem(pEMail);

	cv = pSettings->GetSetServerIncoming();
	CMFCPropertyGridProperty *pServerIncoming = new CMFCPropertyGridProperty(_T("Mail Server (Incoming)"), (_variant_t)(cv->bstrVal), _T("Incoming mail Server"));
	pServerIncoming->SetData(SERVERINCOMING);
	pGettingsStarted->AddSubItem(pServerIncoming);

	cv = pSettings->GetSetUserName();
	CMFCPropertyGridProperty *pUserName = new CMFCPropertyGridProperty(_T("User Name"), (_variant_t)(cv->bstrVal), _T("User's name"));
	pUserName->SetData(USERNAME);
	pGettingsStarted->AddSubItem(pUserName);

	cv = pSettings->GetSetSMTPServer();
	CMFCPropertyGridProperty *pSMTPServer = new CMFCPropertyGridProperty(_T("SMTP Server"), (_variant_t)(cv->bstrVal), _T("User's name"));
	pSMTPServer->SetData(SMTPSERVER);
	pGettingsStarted->AddSubItem(pSMTPServer);

	////
	cv = pSettings->GetSetAllowAuth();
	CMFCPropertyGridProperty * pAllowAuth = new CMFCPropertyGridProperty(_T("Allow Authorization"), (_variant_t)(cv->boolVal == VARIANT_TRUE ? true : false), _T("Specifies whether authorization is allowed"));
	pAllowAuth->SetData(ALLOWAUTH);
	pGettingsStarted->AddSubItem(pAllowAuth);

	////
	cv = pSettings->GetSetAutoOpenMailDetail();
	CMFCPropertyGridProperty * pAutoOpenMailDetail = new CMFCPropertyGridProperty(_T("Auto open mail detail"), (_variant_t)(cv->boolVal == VARIANT_TRUE ? true : false), _T("Open Mail Detail window on start"));
	pAutoOpenMailDetail->SetData(AUTOOPENMAILDETAIL);
	pGettingsStarted->AddSubItem(pAutoOpenMailDetail);

	////
	cv = pSettings->GetSetAutoOpenNewDocument();
	CMFCPropertyGridProperty * pAutoOpenNewDocument = new CMFCPropertyGridProperty(_T("Auto open new document"), (_variant_t)(cv->boolVal == VARIANT_TRUE ? true : false), _T("Open new default document on start"));
	pAutoOpenNewDocument->SetData(AUTOOPENNEWDOCUMENT);
	pGettingsStarted->AddSubItem(pAutoOpenNewDocument);


	m_wndPropList.AddProperty(pGettingsStarted);
}


/////////////////////////////////////////////////////////////////////////////
// void CPropertiesWnd::InitPropListCheckingMail()

void CPropertiesWnd::InitPropListCheckingMail()
{
	CHermesApp *pApp = dynamic_cast<CHermesApp *>(::AfxGetApp());
	CHermesSettings *pSettings = pApp->GetSettings();
	ASSERT(pSettings);

	CComVariant *cv = nullptr;

	CMFCPropertyGridProperty* pCheckingMail = new CMFCPropertyGridProperty(_T("Checking Mail"));

	cv = pSettings->GetSetMailServer();
	CMFCPropertyGridProperty *pMailServer = new CMFCPropertyGridProperty(_T("Mail Server"), (_variant_t)(cv->bstrVal), _T("Mail Server"));
	pMailServer->SetData(MAILSERVER);
	pCheckingMail->AddSubItem(pMailServer);

	cv = pSettings->GetSetMailServerUserName();
	CMFCPropertyGridProperty *pMailServerUserName = new CMFCPropertyGridProperty(_T("Mail Server User Name"), (_variant_t)(cv->bstrVal), _T("Mail Server User Name"));
	pMailServerUserName->SetData(MAILSERVERUSERNAME);
	pCheckingMail->AddSubItem(pMailServerUserName);


	////
	cv = pSettings->GetSetMailCheckInterval();
	//	ASSERT(cv);
	CMFCPropertyGridProperty *pMailCheckInterval = new CMFCPropertyGridProperty(_T("Minutes between mail checking"), (_variant_t)(cv->uiVal), _T(";ail checkinh Intretval"));
	pMailCheckInterval->EnableSpinControl(TRUE, 1, 20);
	pMailCheckInterval->SetData(MAILCHECKINTERVAL);
	pCheckingMail->AddSubItem(pMailCheckInterval);


	////
	cv = pSettings->GetSetCheckWithoutNetworkConnection();
	CMFCPropertyGridProperty * pCheckWithoutNetworkConnection = new CMFCPropertyGridProperty(_T("Check without network connection"), (_variant_t)(cv->boolVal == VARIANT_TRUE ? true : false), _T("Check without network connection"));
	pCheckWithoutNetworkConnection->SetData(CHECKWITHOUTBATTERY);
	pCheckingMail->AddSubItem(pCheckWithoutNetworkConnection);


	////
	cv = pSettings->GetSetCheckWithoutBattery();
	CMFCPropertyGridProperty * pCheckWithoutBattery = new CMFCPropertyGridProperty(_T("Check without battery"), (_variant_t)(cv->boolVal == VARIANT_TRUE ? true : false), _T("Check if not on battery"));
	pCheckWithoutBattery->SetData(CHECKWITHOUTBATTERY);
	pCheckingMail->AddSubItem(pCheckWithoutBattery);

	////
	cv = pSettings->GetSetSendOnCheck();
	CMFCPropertyGridProperty * pSendOnCheck = new CMFCPropertyGridProperty(_T("Send On Check"), (_variant_t)(cv->boolVal == VARIANT_TRUE ? true : false), _T("Send On Check"));
	pSendOnCheck->SetData(SENDONCHECK);
	pCheckingMail->AddSubItem(pSendOnCheck);

	////
	cv = pSettings->GetSetSecureSockets();
	wstring ws = s_SecureSocketNames.at(cv->uiVal);
	CMFCPropertyGridProperty * pSecureSockets = new CMFCPropertyGridProperty(_T("Secure Sockets"), (_variant_t)ws.c_str(), _T("Secure Sockets when receiving"));
	for (const pair<unsigned short, CString>& p : s_SecureSocketNames)
	{
		// use first and second
		pSecureSockets->AddOption(p.second);
	}
	pSecureSockets->AllowEdit(FALSE);
	pSecureSockets->SetData(SECURESOCKETS);
	pCheckingMail->AddSubItem(pSecureSockets);

	m_wndPropList.AddProperty(pCheckingMail);
}


/////////////////////////////////////////////////////////////////////////////
// void CPropertiesWnd::InitPropListIncomingMail()

void CPropertiesWnd::InitPropListIncomingMail()
{
	CHermesApp *pApp = dynamic_cast<CHermesApp *>(::AfxGetApp());
	CHermesSettings *pSettings = pApp->GetSettings();
	ASSERT(pSettings);

	CComVariant *cv = nullptr;


	CMFCPropertyGridProperty* pIncomingMail = new CMFCPropertyGridProperty(_T("Incoming Mail"));

	////
	cv = pSettings->GetSetServerConfig();
	CString ws = s_ServerConfigNames.at(cv->uiVal);


	CMFCPropertyGridProperty * pServerConfig = new CMFCPropertyGridProperty(_T("Server configuration"), (_variant_t)ws, _T("Server configuration"));
	for (const pair<unsigned short, CString>& p : s_ServerConfigNames)
	{
		// use first and second
		pServerConfig->AddOption(p.second);
	}
	pServerConfig->AllowEdit(FALSE);
	pServerConfig->SetData(_SERVERCONFIG);
	pIncomingMail->AddSubItem(pServerConfig);

	////
	cv = pSettings->GetSetLeaveMailOnServer();
	CMFCPropertyGridProperty * pLeaveMailOnServer = new CMFCPropertyGridProperty(_T("Leave mail on server"), (_variant_t)(cv->boolVal == VARIANT_TRUE ? true : false), _T("Leave mail on server"));
	pLeaveMailOnServer->SetData(LEAVEMAILONSERVER);
	pIncomingMail->AddSubItem(pLeaveMailOnServer);


	////
	cv = pSettings->GetSetDeleteFromServerAfterNumDays();
	//	ASSERT(cv);
	CMFCPropertyGridProperty *pDeleteFromServerAfterNumDays = new CMFCPropertyGridProperty(_T("Delete from server after"), (_variant_t)(cv->uiVal), _T("Delete from server after this number of days"));
	pDeleteFromServerAfterNumDays->EnableSpinControl(TRUE, 0, 30);
	pDeleteFromServerAfterNumDays->SetData(DELETEFROMSERVERAFTERNUMDAYS);
	pIncomingMail->AddSubItem(pDeleteFromServerAfterNumDays);


	////
	cv = pSettings->GetSetDeleteFromServerWhenEmptiedFromTrash();
	CMFCPropertyGridProperty * pDeleteFromServerWhenEmptiedFromTrash = new CMFCPropertyGridProperty(_T("Delete from server"), (_variant_t)(cv->boolVal == VARIANT_TRUE ? true : false), _T("Delete from server when emptied from trash"));
	pDeleteFromServerWhenEmptiedFromTrash->SetData(DELETEFROMSERVERWHENEMPTIEDFROMTRASH);
	pIncomingMail->AddSubItem(pDeleteFromServerWhenEmptiedFromTrash);

	////
	cv = pSettings->GetSetSkipMessagesOverNumKilos();
	CMFCPropertyGridProperty *pSkipMessagesOverNumKilos = new CMFCPropertyGridProperty(_T("Skip messages over kilobytes"), (_variant_t)(cv->uiVal), _T("Skip messages over this number of kilobytes"));
	pSkipMessagesOverNumKilos->EnableSpinControl(TRUE, 0, 1000);
	pSkipMessagesOverNumKilos->SetData(SKIPMESSAGESOVERNUMKILOS);
	pIncomingMail->AddSubItem(pSkipMessagesOverNumKilos);

	////
	cv = pSettings->GetSetOffLine();
	CMFCPropertyGridProperty * pOffLine = new CMFCPropertyGridProperty(_T("Offline"), (_variant_t)(cv->boolVal == VARIANT_TRUE ? true : false), _T("Specify OffLine status"));
	pOffLine->SetData(OFFLINE);
	pIncomingMail->AddSubItem(pOffLine);


	////
	cv = pSettings->GetSetAuthType();
	ws = s_AuthTypeNames.at(cv->uiVal);

	TRACE("server config ui: [%ld], str: [%s]\n", cv->uiVal, ws);

	CMFCPropertyGridProperty * pAuthType = new CMFCPropertyGridProperty(_T("Authentication Type"), (_variant_t)ws, _T("Authentication Type"));
	for (const pair<unsigned short, CString>& p : s_AuthTypeNames)
	{
		pAuthType->AddOption(p.second);
	}
	pAuthType->AllowEdit(FALSE);
	pAuthType->SetData(AUTHTYPE);
	pIncomingMail->AddSubItem(pAuthType);


	m_wndPropList.AddProperty(pIncomingMail);
}


	/*
	CMFCPropertyGridProperty* pCheckingMail = new CMFCPropertyGridProperty(_T("Hierarchy"));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("First sub-level"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("Second sub-level"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 1"), (_variant_t) _T("Value 1"), _T("This is a description")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 2"), (_variant_t) _T("Value 2"), _T("This is a description")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 3"), (_variant_t) _T("Value 3"), _T("This is a description")));

	pGroup4->Expand(FALSE);
	m_wndPropList.AddProperty(pGroup4);
*/

/////////////////////////////////////////////////////////////////////////////
// void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}


/////////////////////////////////////////////////////////////////////////////
// void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}


/////////////////////////////////////////////////////////////////////////////
// void CPropertiesWnd::SetPropListFont()

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}


/////////////////////////////////////////////////////////////////////////////
// OnPropertyChanged

#include <sstream>
#include <string>

using namespace std;

// TODO: PRIORITY HIGH: Stop making more settings before boost::bimap has been fecthed in. No reason to make more mess.
// TODO: PRIORITY HIGH: This must be enough for proof of concept. Also, before we get into too much cluttering of backwards std::map lookups.


LRESULT CPropertiesWnd::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	TRACE("LRESULT CPropertiesWnd::OnPropertyChanged(__in WPARAM wparam, __in LPARAM lparam) >>\n");

	CHermesApp *pApp = reinterpret_cast<CHermesApp *>(::AfxGetApp());
	ASSERT(pApp);
	CHermesSettings *pSettings = pApp->GetSettings();
	ASSERT(pSettings);

	// Parameters:
	// [in] wParam: the control ID of the CMFCPropertyGridCtrl that changed.
	// [in] lParam: pointer to the CMFCPropertyGridProperty that changed.

	// Return value:
	// Not used.

	// Cast the lparam to a property.
	CMFCPropertyGridProperty * pProperty = (CMFCPropertyGridProperty *)lParam;

	DWORD_PTR dwData = pProperty->GetData();
	PFUNCTIONMAP tmp = pSettings->GetFunctionMap();

	MethodPtr fpn = tmp->operator[](dwData);
	ASSERT(fpn);

	CComVariant g = pProperty->GetValue();
	TRACE_VARIANTTYPE(&g);

	// Some data members (the ints mapped to strings need backwards lookup to be set properly
	// TODO: This mess can be removed by using boost::bimap. As it is´we'll live with the mess though.
	// TODO: Perform reverse std::map lookup. This is where boost::bimap would be welcome. For now we do it manually...

	switch (dwData)
	{
	case SECURESOCKETS:
		ASSERT(g.vt == VT_BSTR);
		
		for (const pair<unsigned short, CString>& p : s_SecureSocketNames)
		{
			unsigned short num = p.first;
			CString s1 = p.second;
			CString s2 = (LPCTSTR)g.bstrVal;

			TRACE(_T("CString1: [%s], CString2: [%s]\n"), s1, s2);

			if (s1 == s2)
			{
				TRACE(_T("SELECTED: [%s], first: [%ld], second: [%s]\n"), s1, num, s2);
				g = p.first;
				break;
			}
		}
		break;
	case _SERVERCONFIG:
		ASSERT(g.vt == VT_BSTR);
		for (const pair<unsigned short, CString>& p : s_ServerConfigNames)
		{
			// use first and second
			unsigned short num = p.first;
			CString s1 = p.second;
			CString s2 = (LPCTSTR)g.bstrVal;

			TRACE(_T("CString1: [%s], CString2: [%s]\n"), s1, s2);

			if (s1 == s2)
			{
				TRACE("SELECTED: [%s], first: [%ld], second: [%s]\n", s1, num, s2);
				g = p.first;
				break;
			}
		}
		break;
	case AUTHTYPE:
		ASSERT(g.vt == VT_BSTR);
		for (const pair<unsigned short, CString>& p : s_AuthTypeNames)
		{
			// use first and second
			unsigned short num = p.first;
			CString s1 = p.second;
			CString s2 = (LPCTSTR)g.bstrVal;

			TRACE(_T("CString1: [%s], CString2: [%s]\n"), s1, s2);

			if (s1 == s2)
			{
				TRACE("SELECTED: [%s], first: [%ld], second: [%s]\n", s1, num, s2);
				g = p.first;
				break;
			}
		}
		break;
	default:
		break;
	};

	// Set the new value with the auto callback functions in the HermesSettings files.
	(pSettings->*fpn)(&g);

	TRACE("LRESULT CPropertiesWnd::OnPropertyChanged(__in WPARAM wparam, __in LPARAM lparam) <<\n");

	return 0;
}


