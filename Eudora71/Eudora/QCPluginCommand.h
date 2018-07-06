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

/*

HERMES MESSENGER SOFTWARE LICENSE AGREEMENT | Hermes Messenger Client Source Code
Copyright (c) 2018, Hermes Messenger Development Team. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted (subject to the limitations in the disclaimer below) provided that 
the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list 
of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this 
list of conditions and the following disclaimer in the documentation and/or 
other materials provided with the distribution.

Neither the name of Hermes Messenger nor the names of its contributors
may be used to endorse or promote products derived from this software without 
specific prior written permission.

NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY’S PATENT RIGHTS ARE GRANTED BY THIS 
LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
“AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

File revised by Jeff Prickett (kg4ygs@gmail.com) on July 6, 2018
    Removed references to Qualcomm's Shareware Manager.

*/    

// QCPluginCommand.h: interface for the QCPluginCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QCPLUGINCOMMAND_H__7BB106C2_C6FC_11D0_97D1_00805FD2F268__INCLUDED_)
#define AFX_QCPLUGINCOMMAND_H__7BB106C2_C6FC_11D0_97D1_00805FD2F268__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "QCCommandObject.h"

class QCPluginDirector;
class QCProtocol;
class CSpecial;

extern enum COMMAND_ACTION_TYPE;


struct TRANSLATE_DATA
{
	QCProtocol*	m_pProtocol;
	CView*		m_pView;
	BOOL		m_bBuildAddresses;

	TRANSLATE_DATA() { m_pProtocol = NULL; m_pView = NULL; m_bBuildAddresses = FALSE; }
};


struct TOGGLE_DATA
{
	CMenu*		m_pMenu;
	UINT		m_uID;
	UINT		m_uFlags;
	CString*	m_pszProperties;
	BOOL		m_ForceCheck;

	TOGGLE_DATA() { m_pMenu = NULL; m_uID = m_uFlags = 0; m_pszProperties = NULL; m_ForceCheck = FALSE; }
};

class QCPluginCommand : public QCCommandObject  
{
	void*				m_pPluginObject;
	CString				m_szName;
	COMMAND_ACTION_TYPE	m_theType;
public:
	int					m_ImageOffset;

private:

	void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct);
	void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );

public:
	
	QCPluginCommand(
	QCPluginDirector*	pDirector,
	void*				pPluginObject,
	LPCSTR				szName,
	COMMAND_ACTION_TYPE	theType	);
	
	virtual ~QCPluginCommand();

	virtual void	Execute(
	COMMAND_ACTION_TYPE	theAction,
	void*				pData = NULL );

	const CString&	GetName() { return m_szName; }
	COMMAND_ACTION_TYPE	GetType() { return m_theType; }

	void GetTransmissionID( 
	LONG*	plModuleID, 
	LONG*	plTranslatorID);

	CSpecial *		GetSpecialObject() const;

	virtual CString	GetToolTip(
	COMMAND_ACTION_TYPE theAction );

	BOOL GetIcon(HICON &icn);
	BOOL InYourFace();
	BOOL IsDefaultOn();
	BOOL IsOnCompletion();


DECLARE_DYNAMIC( QCPluginCommand )
};

#endif // !defined(AFX_QCPLUGINCOMMAND_H__7BB106C2_C6FC_11D0_97D1_00805FD2F268__INCLUDED_)
