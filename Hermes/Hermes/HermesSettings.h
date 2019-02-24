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
//	
//	
// CHermesSettings class. Declaration.
//
//
//	PURPOSE:
//
//	This is where all the app's settings are kept and persisted.
//
//			https://docs.microsoft.com/en-us/cpp/mfc/reference/coledocument-class?view=vs-2017
//			https://docs.microsoft.com/en-us/cpp/mfc/reference/cricheditdoc-class?view=vs-2017
//
//		Worth considering:
//
//			https://docs.microsoft.com/en-us/cpp/mfc/reference/chtmleditview-class?view=vs-2017#chtmleditview
//
//	[SBROTHY@GMAIL.COM]
//
//

////////////////////////////////////////////////////////////////////////////////
// CHermesSettings Header BEF [SBROTHY]

//
//	TODO: Look into std::function
//
//	Heavily modified default file. So far it´s an ugly mix of C and C++. Only excuse is too little time.
//
// TODO: Make file fully C++
//
// [SBT]

#pragma once

#include "stdafx.h"

using namespace std;	// [SBROTHY]: Throw away the std:: syntax from vector and map to avoid clutter.

class CHermesSettings;		// [SBROTHY]: Forward declaration 


// [SBROTHY]: Pointer to automatic accessor
typedef CComVariant* (CHermesSettings::*MethodPtr)(CComVariant *);
//

// [SBROTHY]: Map of callback accesssor functions.
typedef map<DWORD_PTR, MethodPtr> FUNCTIONMAP, *PFUNCTIONMAP;
//

// [SBROTHY]: The keys to all settings in the automatic callback accessor map
typedef enum : DWORD_PTR /* Force DWORD_PTR type into enum  [SBROTHY] */ {	 
	THREEDLOOK = 0u,
	CAPTION,
	BORDER,
	WINDOWHEIGHT,
	WINDOWWIDTH,
	FONT,
	USESYSTEMFONT,
	NAME,
	WINDOWCOLOR,
	REALNAME,
	EMAIL,
	SERVERINCOMING,
	USERNAME,
	SMTPSERVER,
	ALLOWAUTH,
	AUTOOPENMAILDETAIL,
	AUTOOPENNEWDOCUMENT,
	MAILSERVER,
	MAILSERVERUSERNAME,
	MAILCHECKINTERVAL,
	CHECKWITHOUTNETWORKCONNECTION,
	CHECKWITHOUTBATTERY,
	SENDONCHECK,
	SECURESOCKETS,
	_SERVERCONFIG,
	LEAVEMAILONSERVER,
	DELETEFROMSERVERAFTERNUMDAYS,
	DELETEFROMSERVERWHENEMPTIEDFROMTRASH,
	SKIPMESSAGESOVERNUMKILOS,
	OFFLINE,
	AUTHTYPE
} FUNCTIONPONTERKEYS, *PFUNCTIONPONTERKEYS;


// [SBROTHY]: Dialog Frame type. We were born with this. It might go suddenly.
typedef enum : unsigned short /* Force unsigned short type [SBROTHY] */ {		
	NONE = 0,
	THIN,
	RESIZABLE,
	DLGFRAME
} BORDERVALUE, *PBORDERVALUE;

// [SBROTHY]: Secure socket types
typedef enum : unsigned short /* Force unsigned short type [SBROTHY] */  {
	NEVER = 0,
	IASTARTTLS,
	RALTERNATEPORT,
	RSTARTTLS
} SECURESOCKETSONRECEIVE, *PSECURESOCKETSONRECEIVE;

// [SBROTHY]:TODO: make this a boost::bimap. We want to be able to look up both ways.
// [SBROTHY]:TODO: Reason for this ridiculous name is that we already have an enum value above called SECURESOCKETS. It should really be the othre way around
typedef map<unsigned short, CString> _SECURESOCKETS, *P_SECURESOCKETS; 

// [SBROTHY]:TODO: Initialize these static settings in source files instead in order to make them i18n ready
static const _SECURESOCKETS s_SecureSocketNames = {
	pair<unsigned short, CString>(NEVER, _T("Never")),
	pair<unsigned short, CString>(IASTARTTLS, _T("If available, Start TLS")),
	pair<unsigned short, CString>(RALTERNATEPORT, _T("Required, Alternate port")),
	pair<unsigned short, CString>(RSTARTTLS, _T("Required, STARTTLS"))
};

// [SBROTHY]: Server config
typedef enum : unsigned short /* Force unsigned short type [SBROTHY] */  {
	POP = 0,
	IMAP
} SERVERCONFIG, *PSERVERCONFIG;

// [SBROTHY]:TODO: make this a boost::bimap. We want to be able to look up both ways.
// [SBROTHY]:TODO: Reason for this ridiculous name is that we already have an enum value above called SECURESOCKETS. It should really be the other way around
typedef map<unsigned short, CString> _SERVERCONFIGS, *P_SERVERCONFIGS;

// [SBROTHY]:TODO: Initialize all these static settings in source files instead in order to make them i18n ready
static const _SERVERCONFIGS s_ServerConfigNames = {
	pair<unsigned short, CString>(POP, _T("POP")),
	pair<unsigned short, CString>(IMAP, _T("IMAP"))
};

// [SBROTHY]: Authentication style
// [SBROTHY]:TODO: make this a boost::bimap. We want to be able to look up both ways.
typedef enum : unsigned short /* Force unsigned short type [SBROTHY] */ {
	PASSWORDS = 0,
	KERBEROS,
	APOP,
	RPA
} AUTHSTYLE, *PAUTHSTYLE;


typedef map<unsigned short, CString> AUTHTYPES, *PAUTHTYPES;

// [SBROTHY]: TODO: Initialize these static settings in source files instead in order to make them i18n ready
static const AUTHTYPES s_AuthTypeNames = {
	pair<unsigned short, CString>(PASSWORDS, _T("Passwords")),
	pair<unsigned short, CString>(KERBEROS, _T("Kerberos")),
	pair<unsigned short, CString>(APOP, _T("APOP")),
	pair<unsigned short, CString>(RPA, _T("RPA"))
};



////////////////////////////////////////////////////////////////////////////////
// CHermesSettings class declaration [SBROTHY]

class CHermesSettings : public CObject
{
	// Declare class serializable [SBROTHY]
	DECLARE_SERIAL(CHermesSettings)

public:
	CHermesSettings();
	virtual ~CHermesSettings();

// Attributes [SBROTHY]
protected:
	FUNCTIONMAP		m_FunctionMap;

	CComVariant		m_b3DLook;
	CComVariant		m_strCaption;
	CComVariant		m_strBorder;
	CComVariant		m_uiWindowHeight;
	CComVariant		m_uiWindowWidth;

	LOGFONT			m_lf;
	CComVariant		m_Font;
	CComVariant		m_bUseSystemFont;

	CComVariant		m_strName;
	CComVariant		m_rgbWindowColor;


	CComVariant		m_strRealName;
	CComVariant		m_strEMail;
	CComVariant		m_strServerIncoming;
	CComVariant		m_strUserName;
	CComVariant		m_strSMTPServer;
	CComVariant		m_bAllowAuth;
	CComVariant		m_bAutoOpenMailDetail;
	CComVariant		m_bAutoOpenNewDocument;

	CComVariant		m_strMailServer;
	CComVariant		m_strMailServerUserName;
	CComVariant		m_uiMailCheckInterval;

	CComVariant		m_bCheckWithoutNetworkConnection;
	CComVariant		m_bCheckWithoutBattery;
	CComVariant		m_bSendOnCheck;
	CComVariant		m_uiSecureSockets;
	CComVariant		m_uiServerConfig;

	CComVariant		m_bLeaveMailOnServer;
	CComVariant		m_uiDeleteFromServerAfterNumDays;
	CComVariant		m_bDeleteFromServerWhenEmptiedFromTrash;
	CComVariant		m_uiSkipMessagesOverNumKilos;
	CComVariant		m_bOffLine;
	CComVariant		m_uiAuthType;

// Auxililarity implementation [SBROTHY]
protected:
	void MapFunctionPointers();

public:
	BOOL Save();
	BOOL Load();


// MFC declare serialization helper [SBROTHY]
	void Serialize(CArchive& archive);

	

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Universal MethodFunc type for auto-mapping and auto-callback [SBROTHY]
//
//	Making these inline may seem tempting, but you cannot take the address of an inline function. So out of necessity, they're not.
//

	CComVariant *GetSet3DLook(CComVariant *v = nullptr);
	CComVariant *GetSetCaption(CComVariant *v = nullptr);
	CComVariant *GetSetBorder(CComVariant *v = nullptr);
	CComVariant *GetSetWindowHeight(CComVariant *v = nullptr);
	CComVariant *GetSetWindowWidth(CComVariant *v = nullptr);
	CComVariant *GetSetFont(CComVariant *v = nullptr);
	CComVariant *GetSetUseSystemFont(CComVariant *v = nullptr);
	CComVariant *GetSetName(CComVariant *v = nullptr);
	CComVariant *GetSetWindowColor(CComVariant *v = nullptr);
	CComVariant *GetSetRealName(CComVariant *v = nullptr);
	CComVariant *GetSetEMail(CComVariant *v = nullptr);
	CComVariant *GetSetServerIncoming(CComVariant *v = nullptr);
	CComVariant *GetSetUserName(CComVariant *v = nullptr);
	CComVariant *GetSetSMTPServer(CComVariant *v = nullptr);
	CComVariant *GetSetAllowAuth(CComVariant *v = nullptr);
	CComVariant *GetSetAutoOpenMailDetail(CComVariant *v = nullptr);
	CComVariant *GetSetAutoOpenNewDocument(CComVariant *v = nullptr);
	CComVariant *GetSetMailServer(CComVariant *v = nullptr);
	CComVariant *GetSetMailServerUserName(CComVariant *v = nullptr);
	CComVariant *GetSetMailCheckInterval(CComVariant *v = nullptr);
	CComVariant *GetSetCheckWithoutNetworkConnection(CComVariant *v = nullptr);
	CComVariant *GetSetCheckWithoutBattery(CComVariant *v = nullptr);
	CComVariant *GetSetSendOnCheck(CComVariant *v = nullptr);
	CComVariant *GetSetSecureSockets(CComVariant *v = nullptr);
	CComVariant *GetSetServerConfig(CComVariant *v = nullptr);
	CComVariant	*GetSetLeaveMailOnServer(CComVariant *v = nullptr);
	CComVariant *GetSetDeleteFromServerAfterNumDays(CComVariant *v = nullptr);
	CComVariant	*GetSetDeleteFromServerWhenEmptiedFromTrash(CComVariant *v = nullptr);
	CComVariant *GetSetSkipMessagesOverNumKilos(CComVariant *v = nullptr);
	CComVariant *GetSetOffLine(CComVariant *v = nullptr);
	CComVariant *GetSetAuthType(CComVariant *v = nullptr);


//////////////////////////////////////////////////////////////////////////////////////
// Get the generic accessors [SBROTHY]
	PFUNCTIONMAP GetFunctionMap(); // See typedef map at beginning of file.
};


// CHermesSettings Header EOF [SBROTHY]
////////////////////////////////////////////////////////////////////////////////
