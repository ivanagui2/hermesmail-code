// RegTransReader.h
//
// MIMEReader class for handling EMSAPI-like registration code
// attachment translations. Parsing of information is done
// utilizing rfc 822 code.

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

NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY�S PATENT RIGHTS ARE GRANTED BY THIS 
LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
�AS IS� AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

File revised by Jeff Prickett (kg4ygs@gmail.com) on July 4, 2018
    Removed dependency on Qualcomm's Shareware Manager.

*/

#ifndef _RegInfoReader_h_
#define _RegInfoReader_h_

#include "FileLineReader.h"
#include "LineReader.h"
#include "MIMEReader.h"
#include "stdafx.h"


class CTLAddress;
class JJFile;
class MIMEState;
class CProfileConfirmationDialog;


//==============================================================================
// RegInfoReader
//
// Class for doing EMSAPI translations that an ON_ARRIVAL translator
// has said to do now
//
class RegInfoReader : public MIMEReader
{
  public:
	typedef enum
	{
		rs_Unknown = 0,
		rs_Invalid = 0,
		rs_Reg,
		rs_Profile
	} EudoraInfoEnum;

	static CProfileConfirmationDialog *		s_pProfileConfirmationDlg;

	static EudoraInfoEnum	GetRegInfoFromLineReader(
								LineReader *			pLineReader,
								bool					shouldWriteOutput,
								JJFile *				pOutputFile,
								CString &				outFirstName,
								CString &				outLastName,
								CString &				outRegCode,
								CString *				outProfileID,
								CString *				outMailedTo,
								bool *					outDeleteProfileID,
								CString *				outDistributorID,
								bool *					outEudoraNeedsRegistration);

	static bool				GetInfoFromRegCodeStartupFile(
								const char *			szFilePath,
								CString &				outFirstName,
								CString &				outLastName,
								CString &				outRegCode,
								CString &				outDistributorID,
								bool &					outEudoraNeedsRegistration,
								int *					outRegCodeMonth = NULL);

	BoundaryType			ReadIt(
								CObArray &				MimeStates,
								char*					buf,
								LONG					bSize);

  protected:
	static void				DisplayProfileIDConfirmationDialog(
								UINT				nMessageID,
								const char *		szFirstName,
								const char *		szLastName,
								const char *		szMailedTo,
								const char *		szProfileID);
	
	bool					IsMailedToUser(
								CString &				szMailedTo);

	static EudoraInfoEnum	InternalGetInfoFromAttachment(
								const char *			szFilePath,
								CString &				outFirstName,
								CString &				outLastName,
								CString &				outRegCode,
								CString &				outProfileID,
								CString &				outMailedTo,
								bool &					outDeleteProfileID)
								{
									return InternalGetInfoFromFile( szFilePath, outFirstName, outLastName,
																	outRegCode, &outProfileID,
																	&outMailedTo, &outDeleteProfileID, NULL, NULL );
								}
		
	static EudoraInfoEnum	InternalGetInfoFromFile(
									const char *			szFilePath,
									CString &				outFirstName,
									CString &				outLastName,
									CString &				outRegCode,
									CString *				outProfileID,
									CString *				outMailedTo,
									bool *					outDeleteProfileID,
									CString *				outDistributorID,
									bool *					outEudoraNeedsRegistration);
};


// RegInfoLineReader
//
// Class which provides a line reader that detects a given mime boundary
// and returns 0 bytes read to indicate this.
class RegInfoLineReader : public LineReader
{
  public:
					RegInfoLineReader(
						LineReader *		inLineReader,
						MIMEState *			inParentMS)
						:	m_LineReader(inLineReader), m_ParentMS(inParentMS), m_BoundaryType(btEndOfMessage)
						{ ASSERT(m_LineReader != NULL); }
	virtual			~RegInfoLineReader() {}

	virtual long	ReadLine(char* buf, LONG bSize);

	BoundaryType	GetBoundaryType() const { return m_BoundaryType; }

  protected:
	LineReader *	m_LineReader;
	MIMEState *		m_ParentMS;
	BoundaryType	m_BoundaryType;
};


// RegInfoFileLineReader
//
// Class which provides a file line reader that returns an extra blank line.
// This avoid problems with Lex822State::LexFill adding ".CRLF" to the buffer
// to indicate the end of the header area, which previously caused the last
// line to be interpreted incorrectly when the file did not end with a CRLF.
class RegInfoFileLineReader : public FileLineReader
{
  public:
					RegInfoFileLineReader()
						:	bReadEOF(false) {}
	virtual			~RegInfoFileLineReader() {}

	virtual long	ReadLine(char* buf, LONG bufSize);

  protected:
	bool			bReadEOF;
};



#endif // #ifndef _RegInfoReader_h_
