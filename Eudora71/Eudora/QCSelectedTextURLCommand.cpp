// QCSelectedTextURLCommand.cpp: implementation of the QCSelectedTextURLCommand class.
//
// Copyright (c) 1997-2002 by QUALCOMM, Incorporated
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
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "guiutils.h"

#include "QCSelectedTextURLCommand.h"

//  Commentedout this #include to a nonexistent file. Should it turn up again and prove useful,
//  we can  uncomment it. I'm not optimistic  about it though. (8/9/18. soren <sbrothy@gmail.com>)
#include "DebugNewHelpers.h"



IMPLEMENT_DYNAMIC(QCSelectedTextURLCommand, QCCommandObject)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

QCSelectedTextURLCommand::QCSelectedTextURLCommand(const char* URLFormat) :
	QCCommandObject(NULL), m_URLFormat(URLFormat)
{
}

QCSelectedTextURLCommand::~QCSelectedTextURLCommand()
{
}

void QCSelectedTextURLCommand::Execute(COMMAND_ACTION_TYPE theAction, void* pData /*= NULL*/)
{
	CString ResultURL;

	ResultURL.Format(m_URLFormat, pData);

	LaunchURL(ResultURL);
}

UINT QCSelectedTextURLCommand::GetFlyByID(COMMAND_ACTION_TYPE)
{
	return 0;
}

CString	QCSelectedTextURLCommand::GetToolTip(COMMAND_ACTION_TYPE)
{
	return m_URLFormat;
}
