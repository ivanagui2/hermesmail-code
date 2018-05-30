// QCPOPSettings.h
//
// Copyright (c) 1997-2000 by QUALCOMM, Incorporated
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


#ifndef _QCPOPSETTINGS_H_
#define _QCPOPSETTINGS_H_

#include "sslsettings.h"

enum AuthenticationType
{
	AUTH_POP = 10,
	AUTH_KERBEROS = 100,		// Kerberos 4
	AUTH_APOP = 200,
	AUTH_RPA = 300,
	AUTH_KERBEROS_5 = 400		// Kerberos 5
};

enum FetchType
{
	FETCH_NORMAL = 1,			// fetch all messages (non-LMOS), fetch unread messages (LMOS)
	FETCH_MSG_STATUS = 2,		// fetch unread messages via brute force inspection of message status
	FETCH_LAST = 3				// use POP "LAST" command to determine last unread message
};


class QCNetworkSettings;


class QCPOPSettings {

public:
	QCPOPSettings(const char* persona, unsigned long ulBitFlags);
	virtual ~QCPOPSettings();

	
	int GrabSettings();

	bool UsePOPCAPA(){ return m_bUsePOPCAPA; }
	bool UsePOPMangle(){ return m_bUsePOPMangle; }

	AuthenticationType GetAuthenticationType(){ return m_AuthType; }
	FetchType GetFetchType(){ return m_FetchType; }
	const CString& GetPersona(){ return m_szPersona; }
	const char * GetPOPAccount(){ return m_strPOPAccount; }
	bool DoLMOS(){ return m_bLeaveMailOnServer; }
	bool DeleteMailAfterSomeDays(){ return m_bDeleteMailAfterSomeDays; }
	int  GetDaysToLeaveOnServer() { return m_nDaysToLeaveOnServer; }

	bool DoSendOnCheck(){ return m_bSendOnCheck; }
	bool UsePOPSend(){ return m_bUsePOPSend; }
	bool OpenInMailbox(){ return m_bOpenInMailbox; }
	const char *GetBadPass(){ return m_strBadPass; }
	const char *GetPasswordOKWordList(){ return m_strPasswordOKWordList; }
	bool SavePass(){ return m_bSavePassword; }
	const CString & GetSavePass(){ return m_strSavePassword; }
	bool ConnectOffline(){ return  m_bConnectOffline; }

	int GetSpoolErrorAction() const { return m_nSpoolErrorAction; }
	bool Alert(){ return m_bAlert; }
	bool Sound(){ return m_bSound; }

	CString& GetLMOSEntryName(){ return m_strLMOSEntryName; }
	CString& GetAttachDir(){ return m_strAttachDir; }
	bool SkipBigMessages(){ return m_bSkipBigMessages; }
	int  GetBigMsgThreshold(){ return m_BigMsgThreshold; }
	QCNetworkSettings *GetNetSettings(){ return m_pNetSettings; }


	//Get flags for GetMail
	bool Fetching(){ return m_bFetching; }
	bool Sending(){ return m_bSending; }
	bool Deleting(){ return m_bDeleting; }
	bool NeedPOP(){ return m_bNeedPOP; }
	unsigned long GetBitFlags(){ return m_ulBitFlags; }
	int GetPort(){ return m_nPopPort; }
	int GetUpdateFrequency(){ return m_nUpdateFreq;}
    bool Filtermail() { return m_bFilterMail;}
    QCSSLSettings m_SSLSettings;
private:

	QCNetworkSettings *m_pNetSettings;
	CString m_szPersona;

	int m_nPopPort;
	int m_nUpdateFreq;
	
	//Server Flags 
	unsigned long m_ulBitFlags;
	bool m_bFetching;
	bool m_bSending;
	bool m_bDeleting;
	bool m_bNeedPOP;

	bool m_bUsePOPCAPA;				//IDS_INI_USE_POP_CAPA
	bool m_bUsePOPMangle;			//IDS_INI_USE_POP_MANGLE
	
	CString m_strPOPAccount;		//IDS_INI_POP_ACCOUNT
	CString m_strLMOSEntryName;
	bool m_bFirstUnreadNormal;		//IDS_INI_FIRST_UNREAD_NORMAL
	bool m_bUsePOPLast;				//IDS_INI_USE_POP_LAST

	//Msg Fetch type & LMOS
	enum FetchType m_FetchType;
	bool m_bSkipBigMessages;		//IDS_INI_SKIP_BIG_MESSAGES
	int m_BigMsgThreshold;			//IDS_INI_BIG_MESSAGE_THRESHOLD
	CString m_strAttachDir;			//IDS_INI_AUTO_RECEIVE_DIR

	//LMOS
	bool m_bLeaveMailOnServer;		//IDS_INI_LEAVE_MAIL_ON_SERVER
	bool m_bDeleteMailAfterSomeDays;//IDS_INI_DELETE_MAIL_FROM_SERVER
	int m_nDaysToLeaveOnServer;		//IDS_INI_LEAVE_ON_SERVER_DAYS

	bool m_bSendOnCheck;			//IDS_INI_SEND_ON_CHECK
	bool m_bUsePOPSend;				//IDS_INI_USE_POP_SEND

	CString m_strBadPass;			//IDS_INI_BAD_PASSWORD_STRING
	CString m_strPasswordOKWordList;
	
	bool m_bSavePassword;			//IDS_INI_SAVE_PASSWORD
	CString m_strSavePassword;		//IDS_INI_SAVE_PASSWORD_TEXT
	bool m_bConnectOffline;			//IDS_INI_CONNECT_OFFLINE
	
	bool m_bAuthKerb;				//IDS_INI_AUTH_KERB
	bool m_bAuthAPOP;				//IDS_INI_AUTH_APOP
	bool m_bAuthRPA;				//IDS_INI_AUTH_RPA
	bool m_bAuthKerb5;				//IDS_INI_AUTH_GSSAPI
	enum AuthenticationType m_AuthType;


	int m_nSpoolErrorAction;		//IDS_INI_SPOOL_ERROR_ACTION
	bool m_bAlert;					//IDS_INI_ALERT
	bool m_bOpenInMailbox;			//IDS_INI_OPEN_IN_MAILBOX
	bool m_bSound;					//IDS_INI_SOUND
	CString m_strKerberosFakePassword;    //IDS_KERB_FAKE_PASSWORD
	bool m_bKerbersoSetUserName;	//IDS_INI_KERBEROS_SET_USERNAME
	CString m_strKerberosService;	//IDS_INI_KERB_SERVICE
	CString m_strKerberosRealm;		//IDS_INI_KERB_REALM
	CString m_strKerberosFormat;	//IDS_INI_KERB_FORMAT
	bool m_bFilterMail;				//IDS_INI_FILTER_INCOMING_MAIL
};



#endif