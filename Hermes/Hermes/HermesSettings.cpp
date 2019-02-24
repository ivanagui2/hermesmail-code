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
//	*	CHermesSettings class. Implementation.
//	*
//	*
//	*	PURPOSE:
//	*
//	*		Initialization, persistence and automatic accssors implementedted.
//	*		
//	*
//	*	[SBROTHY@GMAIL.COM]
//	*
//

//////////////////////////////////////////////////////////////////////////////// 
// CHermesSettings Implementation BEF [SBROTHY]


#include "stdafx.h"
#include "HermesSettings.h"
#include "resource.h"


// Implement class serialization [SBROTHY]
IMPLEMENT_SERIAL(CHermesSettings, CObject, VERSIONABLE_SCHEMA | 2)



//////////////////////////////////////////////////////////////////////////////// 
// CHermesSettings Construction / Destruction  [SBROTHY]
//
//
//	Adding a new setting. It's a pretty byzantine process but that's the price for automation. This works flawlessly for the simpler values.
//	I have yet to figure out how to store structuresa in CComVariant. COLORREF is the size of a DWORD, that could maybe be done, but LOGFONT is another matter...
//
//	If not already there, in PropertiesWnd.cpp, void CPropertiesWnd::InitPropList() add the group to which you want the setting to belong:
//
//	eg.:
//
//		CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Eudora"));
//
//	Here, the group "Eudora" has already been created.
//
//  to add a new persistent setting to this group first declare the CComVariant in the CHermesSettings header file, like:
//
//			CComVariant		m_strServerIncoming;
//
//	Make sure it's initialized in the constructor initialization list (or for more more complicated settingds in the body of the CHermesSettings constructor.
//
//	Add a new value (in this case SERVERINCOMING) in the FUNCTIONPONTERKEYS enum in the CHmeresSettings header.
//
//	Add an accessor for the new setting along with the rest like so.:
//
//			CComVariant *GetSetServerIncoming(CComVariant *v = nullptr);
//
//	Implement it in the CHermesSettings source file as:
//			
//			CComVariant *CHermesSettings::GetSetServerIncoming(CComVariant *v)
//			{
//				if (v)
//					m_strServerIncoming = v->bstrVal;
//				return &m_strServerIncoming;
//			}
//
//	Add it to the void CHermesSettings::MapFunctionPointers() function as 
//
//		m_FunctionMap[SERVERINCOMING] = &CHermesSettings::GetSetServerIncominig;
//
//
//	Make sure it's addded to the void CHermesSettings::Serialize(CArchive& archive) function like
//		
//				m_strServerIncoming.WriteToStream(&pStream);
//	and
//				m_strServerIncoming.ReadFromStream(&pStream);
//
//
//	Back in PropertiesWnd.cpp add the new settings as:
//
//				cv = pSettings->GetSetServerIncoming();
//				CMFCPropertyGridProperty *pServerIncoming = new CMFCPropertyGridProperty(_T("Server Incoming"), (_variant_t)(cv->bstrVal), _T("Server incoming"));
//				pServerIncoming->SetData(SERVERINCOMING);
//				pGroup4->AddSubItem(pServerIncoming);
//
//	Because of the pServerIncoming->SetData(SERVERINCOMING); call, the 
//
//			LRESULT CPropertiesWnd::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
//
//	will automatically be able to use the SERVERINCOMING key to get the pointer to the proper accessor and call it when the value changes.
//
//	Settings are automatically loaded on startup and saved on exit.
//
//			void CPropertiesWnd::OnProperties2()
//
//	corresponds tro the toolbutton with the little lightning on. This will save all the settings on demand. 
//
//
//	Make sure to delete the settings file when adding or removing settings or the Serialiable Write and Read will ofcourse not match.



CHermesSettings::CHermesSettings()
	:		m_b3DLook(VARIANT_TRUE),
			m_strCaption(_T("Caption")),
			m_strBorder(_T("Thin")),
			m_uiWindowHeight((unsigned short)250),
			m_uiWindowWidth((unsigned short)150),
			m_bUseSystemFont(VARIANT_FALSE),
			m_strName(_T("Hermes")),
			m_strRealName(_T("")),
			m_strEMail(_T("")),
			m_strServerIncoming(_T("")),
			m_strUserName(_T("")),
			m_strSMTPServer(_T("")),
			m_bAllowAuth(VARIANT_TRUE),
			m_bAutoOpenMailDetail(VARIANT_TRUE),
			m_bAutoOpenNewDocument(VARIANT_TRUE),
			m_strMailServer(_T("")),
			m_strMailServerUserName(_T("")),
			m_uiMailCheckInterval((unsigned short)3),
			m_bCheckWithoutNetworkConnection(VARIANT_FALSE),
			m_bCheckWithoutBattery(VARIANT_TRUE),
			m_bSendOnCheck(VARIANT_TRUE),
			m_uiSecureSockets(RSTARTTLS),
			m_uiServerConfig(POP),
			m_bLeaveMailOnServer(VARIANT_FALSE),
			m_uiDeleteFromServerAfterNumDays((unsigned short)0),
			m_bDeleteFromServerWhenEmptiedFromTrash(VARIANT_FALSE),
			m_uiSkipMessagesOverNumKilos((unsigned short)40),
			m_bOffLine(VARIANT_FALSE),
			m_uiAuthType(PASSWORDS),
			m_rgbWindowColor((DWORD)RGB(210, 192, 254))
{
	TRACE(_T("CHermesSettings::CHermesSettings() >>\n"));

	TRACE_VARIANTTYPE(&m_rgbWindowColor);

	MapFunctionPointers();


	CFont* font = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&m_lf);
	_tcscpy_s(m_lf.lfFaceName, _T("Arial"));
	m_Font.vt = VT_RECORD | VT_BYREF;
	m_Font.pvRecord = &m_lf;

	//TVARIANT(m_Font);

	TRACE(_T("CHermesSettings::CHermesSettings() <<\n"));
}


CHermesSettings::~CHermesSettings()

{

}


//////////////////////////////////////////////////////////////////////////////// 
// Auxililarity implementation [SBROTHY]


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Save Settings [SBROTHY]
//
//
//	Save the settings.
//
//	07/02/19	-	SBROTHY	-	Not all control paths made sense. Fixed.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHermesSettings::Save()
{
	TRACE(_T("BOOL CHermesSettings::Save() >>\n"));

	CString strFileName;
	BOOL bNameValid = strFileName.LoadString(IDS_STRING_SETTINGS_FILE);
	ASSERT(bNameValid);

	CFile f;
	BOOL bFound = f.Open(strFileName, CFile::modeWrite | CFile::modeCreate);

	if (!bFound)
	{
		TCHAR *szErrMsg = ::GetErrMsg(::GetLastError(), _T("CFile::Open: "));
		TRACE(_T("[%s]\n"), szErrMsg);
		::MessageBox(NULL, szErrMsg, _T("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	try
	{
		CArchive ar(&f, CArchive::store);
		Serialize(ar);
		ar.Close();
		f.Close();
	}
	catch (CException *e)
	{
		SYSError(_T("CArchive: "), FALSE);
		//CDumpContext *pDC = new CDumpContext(NULL);
//		e->Dump(*pDC);
//		delete pDC;
		e->ReportError();
		e->Delete();
		return FALSE;
	}


	TRACE(_T("BOOL CHermesSettings::Save() <<\n"));

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Load Settings [SBROTHY]
//
//
//	Load the settings.
//
//	07/02/19	-	SBROTHY	-	Not all control paths made sense. Fixed.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHermesSettings::Load()
{
	TRACE(_T("BOOL CHermesSettings::Load() >>\n"));

	CString strFileName;
	BOOL bNameValid = strFileName.LoadString(IDS_STRING_SETTINGS_FILE);
	ASSERT(bNameValid);
	if (!bNameValid)
	{
		::SYSError(_T("CString::LoadString: "));
		return FALSE;
	}

	CFile f;
	BOOL bFound = FALSE;
	try
	{
		bFound = f.Open(strFileName, CFile::modeRead);
	}
	catch (CException *e)
	{
		SYSError(_T("CArchive: "), FALSE);
//		CDumpContext *pDC = new CDumpContext(NULL);
//		e->Dump(*pDC);
//		delete pDC;
		e->ReportError();
		e->Delete();
		return FALSE;
	}

	if (!bFound)
	{
		TCHAR *szErrMsg = ::GetErrMsg(::GetLastError(), _T("CFile::Open: "));
		TRACE(_T("[%s]\n"), szErrMsg);
		::MessageBox(NULL, szErrMsg, _T("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	try
	{
		CArchive ar(&f, CArchive::load);
		Serialize(ar);
		ar.Close();
		f.Close();
	}
	catch (CException *e)
	{
		SYSError(_T("CArchive: "), FALSE);
//		CDumpContext *pDC = new CDumpContext(NULL);
//		e->Dump(*pDC);
//		delete pDC;
		e->ReportError();
		e->Delete();
		return FALSE;
	}

	TRACE(_T("BOOL CHermesSettings::Load() <<\n"));

	return TRUE;
}


/////////////////////////////////////////// /////////////////////////////////////////////////////////////////
// MapFunctionPointers()
//
//	Map each settings value to it's corresponding accessor.
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHermesSettings::MapFunctionPointers()
{
	m_FunctionMap[THREEDLOOK] = &CHermesSettings::GetSet3DLook;
	m_FunctionMap[CAPTION] = &CHermesSettings::GetSetCaption;
	m_FunctionMap[BORDER] = &CHermesSettings::GetSetBorder;
	m_FunctionMap[WINDOWHEIGHT] = &CHermesSettings::GetSetWindowHeight;
	m_FunctionMap[WINDOWWIDTH] = &CHermesSettings::GetSetWindowWidth;
	m_FunctionMap[FONT] = &CHermesSettings::GetSetFont;
	m_FunctionMap[USESYSTEMFONT] = &CHermesSettings::GetSetUseSystemFont;
	m_FunctionMap[NAME] = &CHermesSettings::GetSetRealName;
	m_FunctionMap[WINDOWCOLOR] = &CHermesSettings::GetSetWindowColor;
	m_FunctionMap[EMAIL] = &CHermesSettings::GetSetEMail;
	m_FunctionMap[SERVERINCOMING] = &CHermesSettings::GetSetServerIncoming;
	m_FunctionMap[USERNAME] = &CHermesSettings::GetSetUserName;
	m_FunctionMap[SMTPSERVER] = &CHermesSettings::GetSetSMTPServer;
	m_FunctionMap[ALLOWAUTH] = &CHermesSettings::GetSetAllowAuth;
	m_FunctionMap[AUTOOPENMAILDETAIL] = &CHermesSettings::GetSetAutoOpenMailDetail;
	m_FunctionMap[AUTOOPENNEWDOCUMENT] = &CHermesSettings::GetSetAutoOpenNewDocument;
	m_FunctionMap[MAILSERVER] = &CHermesSettings::GetSetMailServer;
	m_FunctionMap[MAILSERVERUSERNAME] = &CHermesSettings::GetSetMailServerUserName;
	m_FunctionMap[MAILCHECKINTERVAL] = &CHermesSettings::GetSetMailCheckInterval;
	m_FunctionMap[CHECKWITHOUTNETWORKCONNECTION] = &CHermesSettings::GetSetCheckWithoutNetworkConnection;
	m_FunctionMap[CHECKWITHOUTBATTERY] = &CHermesSettings::GetSetCheckWithoutBattery;
	m_FunctionMap[SENDONCHECK] = &CHermesSettings::GetSetSendOnCheck;
	m_FunctionMap[SECURESOCKETS] = &CHermesSettings::GetSetSecureSockets;
	m_FunctionMap[_SERVERCONFIG] = &CHermesSettings::GetSetServerConfig;
	m_FunctionMap[LEAVEMAILONSERVER] = &CHermesSettings::GetSetLeaveMailOnServer;
	m_FunctionMap[DELETEFROMSERVERAFTERNUMDAYS] = &CHermesSettings::GetSetDeleteFromServerAfterNumDays;
	m_FunctionMap[DELETEFROMSERVERWHENEMPTIEDFROMTRASH] = &CHermesSettings::GetSetDeleteFromServerWhenEmptiedFromTrash;
	m_FunctionMap[SKIPMESSAGESOVERNUMKILOS] = &CHermesSettings::GetSetSkipMessagesOverNumKilos;
	m_FunctionMap[OFFLINE] = &CHermesSettings::GetSetOffLine;
	m_FunctionMap[AUTHTYPE] = &CHermesSettings::GetSetAuthType;
}


////////////////////////////////////
PFUNCTIONMAP CHermesSettings::GetFunctionMap()
{
	return &m_FunctionMap;
}

////////////////////////////////////
// MFC implement serialization helper [SBROTHY]
void CHermesSettings::Serialize(CArchive& ar)
{
	CArchiveStream pStream(&ar);

	if (ar.IsStoring())
	{
		m_b3DLook.WriteToStream(&pStream);
		m_strCaption.WriteToStream(&pStream);
		m_strBorder.WriteToStream(&pStream);
		m_uiWindowHeight.WriteToStream(&pStream);
		m_uiWindowWidth.WriteToStream(&pStream);
//		m_Font.WriteToStream(&pStream);
		m_bUseSystemFont.WriteToStream(&pStream);
		m_strName.WriteToStream(&pStream);
		m_rgbWindowColor.WriteToStream(&pStream);
		m_strRealName.WriteToStream(&pStream);
		m_strEMail.WriteToStream(&pStream);
		m_strServerIncoming.WriteToStream(&pStream);
		m_strUserName.WriteToStream(&pStream);
		m_strSMTPServer.WriteToStream(&pStream);
		m_bAllowAuth.WriteToStream(&pStream);
		m_bAutoOpenMailDetail.WriteToStream(&pStream);
		m_bAutoOpenNewDocument.WriteToStream(&pStream);
		m_bCheckWithoutBattery.WriteToStream(&pStream);
		m_strMailServer.WriteToStream(&pStream);
		m_strMailServerUserName.WriteToStream(&pStream);
		m_uiMailCheckInterval.WriteToStream(&pStream);
		m_bCheckWithoutNetworkConnection.WriteToStream(&pStream);
		m_bCheckWithoutBattery.WriteToStream(&pStream);
		m_bSendOnCheck.WriteToStream(&pStream);
		m_uiSecureSockets.WriteToStream(&pStream);
		m_uiServerConfig.WriteToStream(&pStream);
		m_bLeaveMailOnServer.WriteToStream(&pStream);
		m_uiDeleteFromServerAfterNumDays.WriteToStream(&pStream);
		m_bDeleteFromServerWhenEmptiedFromTrash.WriteToStream(&pStream);
		m_uiSkipMessagesOverNumKilos.WriteToStream(&pStream);
		m_bOffLine.WriteToStream(&pStream);
		m_uiAuthType.WriteToStream(&pStream);
	}
	else
	{
		m_b3DLook.ReadFromStream(&pStream);
		m_strCaption.ReadFromStream(&pStream);
		m_strBorder.ReadFromStream(&pStream);
		m_uiWindowHeight.ReadFromStream(&pStream);
		m_uiWindowWidth.ReadFromStream(&pStream);
		//		m_Font.ReadFromStream(&pStream);
		m_bUseSystemFont.ReadFromStream(&pStream);
		m_strName.ReadFromStream(&pStream);
		m_rgbWindowColor.ReadFromStream(&pStream);
		m_strRealName.ReadFromStream(&pStream);
		m_strEMail.ReadFromStream(&pStream);
		m_strServerIncoming.ReadFromStream(&pStream);
		m_strUserName.ReadFromStream(&pStream);
		m_strSMTPServer.ReadFromStream(&pStream);
		m_bAllowAuth.ReadFromStream(&pStream);
		m_bAutoOpenMailDetail.ReadFromStream(&pStream);
		m_bAutoOpenNewDocument.ReadFromStream(&pStream);
		m_bCheckWithoutBattery.ReadFromStream(&pStream);
		m_strMailServer.ReadFromStream(&pStream);
		m_strMailServerUserName.ReadFromStream(&pStream);
		m_uiMailCheckInterval.ReadFromStream(&pStream);
		m_bCheckWithoutNetworkConnection.ReadFromStream(&pStream);
		m_bCheckWithoutBattery.ReadFromStream(&pStream);
		m_bSendOnCheck.ReadFromStream(&pStream);
		m_uiSecureSockets.ReadFromStream(&pStream);
		m_uiServerConfig.ReadFromStream(&pStream);
		m_bLeaveMailOnServer.ReadFromStream(&pStream);
		m_uiDeleteFromServerAfterNumDays.ReadFromStream(&pStream);
		m_bDeleteFromServerWhenEmptiedFromTrash.ReadFromStream(&pStream);
		m_uiSkipMessagesOverNumKilos.ReadFromStream(&pStream);
		m_bOffLine.ReadFromStream(&pStream);
		m_uiAuthType.ReadFromStream(&pStream);
	}

	CObject::Serialize(ar);
}


//////////////////////////////////////////////////////////////////////////////// 
// Automatic accessors [SBROTHY]
//

////////////////////////////////////
CComVariant *CHermesSettings::GetSet3DLook(CComVariant *v)
{
	if (v)
	{
		ASSERT(v->vt == VT_BOOL);
		m_b3DLook = v->boolVal;
	}
	return &m_b3DLook;
}

////////////////////////////////////
CComVariant *CHermesSettings::GetSetCaption(CComVariant *v)
{
	if (v)
	{
		ASSERT(v->vt == VT_BSTR);
		m_strCaption = v->bstrVal;
	}
	return &m_strCaption;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetBorder(CComVariant *v)
{
	if (v)
	{
		ASSERT(v->vt == VT_BSTR);
		m_strBorder = v->bstrVal;
	}
	return &m_strBorder;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetWindowHeight(CComVariant *v)
{
	if (v)
	{
		ASSERT(v->vt == VT_UI2);
		m_uiWindowHeight = v->uiVal;
	}
	return &m_uiWindowHeight;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetWindowWidth(CComVariant *v)
{
	if (v)
	{
		ASSERT(v->vt == VT_UI2);
		m_uiWindowWidth = v->uiVal;
	}
	return &m_uiWindowWidth;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetFont(CComVariant *v)
{
/*
	if (v)
		m_Font = v->pvRecord;
	return &m_Font;
*/
	return NULL;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetUseSystemFont(CComVariant *v)
{
	if (v)
	{
		ASSERT(v->vt = VT_BOOL);
		m_bUseSystemFont = v->boolVal;
	}
	return &m_bUseSystemFont;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetName(CComVariant *v)
{
	if (v)
	{
		ASSERT(v->vt == VT_BSTR);
		m_strName = v->bstrVal;
	}
	return &m_strName;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetWindowColor(CComVariant *v)
{
	if (v)
	{
		ASSERT(v->vt == VT_I4);
		m_rgbWindowColor = v->lVal;
	}
	return &m_rgbWindowColor;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetRealName(CComVariant *v)
{
	if (v)
		m_strRealName = v->bstrVal;
	return &m_strRealName;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetEMail(CComVariant *v)
{
	if (v)
		m_strEMail = v->bstrVal;
	return &m_strEMail;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetServerIncoming(CComVariant *v)
{
	if (v)
		m_strServerIncoming = v->bstrVal;
	return &m_strServerIncoming;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetUserName(CComVariant *v)
{
	if (v)
		m_strUserName = v->bstrVal;
	return &m_strUserName;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetSMTPServer(CComVariant *v)
{
	if (v)
		m_strSMTPServer = v->bstrVal;
	return &m_strSMTPServer;
}



////////////////////////////////////
CComVariant *CHermesSettings::GetSetAllowAuth(CComVariant *v)
{
	if (v)
	{
		TRACE_VARIANTTYPE(v);
		ASSERT(v->vt == VT_BOOL);
		m_bAllowAuth = v->boolVal;
	}
	return &m_bAllowAuth;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetAutoOpenMailDetail(CComVariant *v)
{
	if (v)
	{
		TRACE_VARIANTTYPE(v);
		ASSERT(v->vt == VT_BOOL);
		m_bAutoOpenMailDetail = v->boolVal;
	}
	return &m_bAutoOpenMailDetail;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetAutoOpenNewDocument(CComVariant *v)
{
	if (v)
	{
		TRACE_VARIANTTYPE(v);
		ASSERT(v->vt == VT_BOOL);
		m_bAutoOpenNewDocument = v->boolVal;
	}
	return &m_bAutoOpenNewDocument;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetMailServer(CComVariant *v)
{
	if (v)
		m_strMailServer = v->bstrVal;
	return &m_strMailServer;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetMailServerUserName(CComVariant *v)
{
	if (v)
		m_strMailServerUserName = v->bstrVal;
	return &m_strMailServerUserName;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetMailCheckInterval(CComVariant *v)
{
	if (v)
		m_uiMailCheckInterval = v->uiVal;
	return &m_uiMailCheckInterval;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetCheckWithoutBattery(CComVariant *v)
{
	if (v)
	{
		TRACE_VARIANTTYPE(v);
		ASSERT(v->vt == VT_BOOL);
		m_bCheckWithoutBattery = v->boolVal;
	}   
	return &m_bCheckWithoutBattery;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetCheckWithoutNetworkConnection(CComVariant *v)
{
	if (v)
	{
		TRACE_VARIANTTYPE(v);
		ASSERT(v->vt == VT_BOOL);
		m_bCheckWithoutNetworkConnection = v->boolVal;
	}
	return &m_bCheckWithoutNetworkConnection;
}



////////////////////////////////////
CComVariant *CHermesSettings::GetSetSendOnCheck(CComVariant *v)
{
	if (v)
	{
		TRACE_VARIANTTYPE(v);
		ASSERT(v->vt == VT_BOOL);
		m_bSendOnCheck= v->boolVal;
	}
	return &m_bSendOnCheck;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetSecureSockets(CComVariant *v)
{
	if (v)
	{
		TRACE_VARIANTTYPE(v);
		ASSERT(v->vt == VT_UI2);
		TRACE("%ld\n", v->uiVal);
		m_uiSecureSockets = v->uiVal;
	}
	return &m_uiSecureSockets;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetServerConfig(CComVariant *v)
{
	if (v)
	{
		TRACE_VARIANTTYPE(v);
		ASSERT(v->vt == VT_UI2);
		TRACE("%ld\n", v->uiVal);
		m_uiServerConfig = v->uiVal;
	}
	return &m_uiServerConfig;
}


////////////////////////////////////
CComVariant	*CHermesSettings::GetSetLeaveMailOnServer(CComVariant *v)
{
	if (v)
	{
		TRACE_VARIANTTYPE(v);
		ASSERT(v->vt == VT_BOOL);
		m_bLeaveMailOnServer = v->boolVal;
	}
	return &m_bLeaveMailOnServer;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetDeleteFromServerAfterNumDays(CComVariant *v)
{
	if (v)
		m_uiDeleteFromServerAfterNumDays = v->uiVal;
	return &m_uiDeleteFromServerAfterNumDays;

}


////////////////////////////////////
CComVariant	*CHermesSettings::GetSetDeleteFromServerWhenEmptiedFromTrash(CComVariant *v)
{
	if (v)
	{
		TRACE_VARIANTTYPE(v);
		ASSERT(v->vt == VT_BOOL);
		m_bDeleteFromServerWhenEmptiedFromTrash = v->boolVal;
	}
	return &m_bDeleteFromServerWhenEmptiedFromTrash;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetSkipMessagesOverNumKilos(CComVariant *v)
{
	if (v)
		m_uiSkipMessagesOverNumKilos = v->uiVal;
	return &m_uiSkipMessagesOverNumKilos;

}


////////////////////////////////////
CComVariant	*CHermesSettings::GetSetOffLine(CComVariant *v)
{
	if (v)
	{
		TRACE_VARIANTTYPE(v);
		ASSERT(v->vt == VT_BOOL);
		m_bOffLine = v->boolVal;
	}
	return &m_bOffLine;
}


////////////////////////////////////
CComVariant *CHermesSettings::GetSetAuthType(CComVariant *v)
{
	if (v)
	{
		TRACE_VARIANTTYPE(v);
		ASSERT(v->vt == VT_UI2);
		TRACE("%ld\n", v->uiVal);
		m_uiAuthType = v->uiVal;
	}
	return &m_uiAuthType;
}



#ifdef _DEBUG
void CHermesSettings::AssertValid() const
{
	CObject::AssertValid();
	ASSERT(m_b3DLook.vt == VT_BOOL);
	ASSERT(m_strCaption.vt == VT_BSTR);
	ASSERT(m_strBorder.vt == VT_BSTR);
	ASSERT(m_uiWindowHeight.vt == VT_UI2);
	ASSERT(m_uiWindowWidth.vt == VT_UI2);
	ASSERT(m_bUseSystemFont.vt == VT_BOOL);
	ASSERT(m_strName.vt == VT_BSTR);
	ASSERT(m_rgbWindowColor.vt == VT_I4);
}


void CHermesSettings::Dump(CDumpContext& dc) const
{
	// We skip the CDumpContext because it clutters the output to be of no use.
	TRACE(_T("m_b3DLook: [%d]\n"), m_b3DLook);
	TRACE(_T("m_strCaption: [%s]\n"), m_strCaption);
	TRACE(_T("m_strBorder: [%s]\n"), (LPCTSTR)m_strCaption.bstrVal);
	TRACE(_T("m_uiWindowHeight: [%d]\n"), m_uiWindowHeight.uiVal);
	TRACE(_T("m_uiWindowWidth: [%d]\n"), m_uiWindowWidth.uiVal);
	TRACE(_T("m_bUseSystemFont: [%d]\n"), m_bUseSystemFont.boolVal);
	TRACE(_T("m_strName: [%s]\n"), (LPCTSTR)m_strName.bstrVal);
	TRACE(_T("m_rgbWindowColor: [%d]"), m_rgbWindowColor.lVal);
}

#endif //_DEBUG



// CHermesSettings Implementation EOF [SBROTHY]
////////////////////////////////////////////////////////////////////////////////


