// DEBUG.H
//
// Routines for writing info to the debug file
//

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

File revised by Jeff Prickett (kg4ygs@gmail.com) on July 4, 2018

*/

#ifndef _EUDORA_DEBUG_H_
#define _EUDORA_DEBUG_H_


#include "afxmt.h"
typedef DWORD DebugMaskType;

// Logging bitmask values
const DebugMaskType DEBUG_MASK_SEND		= 0x01;	// Log sending of a message
const DebugMaskType DEBUG_MASK_RCVD		= 0x02;	// Log receipt of a message
const DebugMaskType DEBUG_MASK_NAV		= 0x04;	// Serial login navagation
const DebugMaskType DEBUG_MASK_DIALOG	= 0x08;	// Dialog message logging
const DebugMaskType DEBUG_MASK_PROG		= 0x10;	// Progress box logging
const DebugMaskType DEBUG_MASK_TRANS	= 0x20;	// All bytes Tx
const DebugMaskType DEBUG_MASK_RCV		= 0x40;	// All bytes Rx

const DebugMaskType DEBUG_MASK_TOC_CORRUPT = 0x80;	// Check a TOC bug

const DebugMaskType DEBUG_MASK_TRANS_BASIC	= 0x100;	// Basic Translator Stuff
const DebugMaskType DEBUG_MASK_TRANS_ADV	= 0x200;	// Advanced Translator Stuff

const DebugMaskType DEBUG_MASK_FILTERS		= 0x400;	// Filter actions

const DebugMaskType DEBUG_MASK_ASSERT		= 0x800;	// Log ASSERTs & VERIFYs
const DebugMaskType DEBUG_MASK_PLIST		= 0x1000;	// Playlist stuff

const DebugMaskType DEBUG_MASK_LMOS			= 0x2000;	// LMOS stuff

const DebugMaskType DEBUG_MASK_SEARCH		= 0x4000;	// Searching and search indexing

const DebugMaskType DEBUG_MASK_MISC			= 0x8000;	// Miscellaneous .. This can be used in cases, where the developer does not 
														// want/need to create new log levels. Baically if you need to log information once or so. 

class JJFileMT;

class AFX_EXT_CLASS QCLogFileMT
{

public:
	// Load debug values from EUDORA.INI
	static BOOL InitDebug(DWORD dwMask, DWORD dwSize, LPCTSTR strEudoraDir, LPCTSTR strLogFileName);

	// Notify INI change
	static void NotifyIniChange(UINT nStringNum, long nValue);

	// Cleanup left over structures.
	static void CleanupDebug();

	static void PutLineHeader(DebugMaskType ID);
	static void PutLineTrailer(DebugMaskType ID);
	static void WriteDebugLog(DebugMaskType ID, LPCTSTR Buffer, int Length = -1);
	static void WriteAssertLog(LPCTSTR pExpr, LPCTSTR pFileName, int nLineNum);

	// TRUE indicates that at least one of the mask bits are set
	static BOOL DebugMaskSet(DebugMaskType Mask);
	
public:
	static bool			s_bOutputEudoraStatusInfoNow;
	static DebugMaskType	DebugMask;
	static JJFileMT*	s_pLogFile;

private:
	static const time_t kInitialTime;
	static long			DebugLogSize;
	static time_t		MarkerTime;
	
	static CString g_strLogFileName;
	static CString g_strEudoraDir;
	static CString m_AssertFmtStr;

	static CCriticalSection m_Guard;

	static BOOL m_bIsNT;

};


#ifdef TRUE //_DEBUG

__declspec( dllexport )  void PutDebugLog(DebugMaskType ID, LPCTSTR Buffer, int Length = -1);

#else
// Place null terminated character data into the log file
inline __declspec( dllexport )  void PutDebugLog(DebugMaskType ID, LPCTSTR Buffer, int Length = -1)
{
	if ((QCLogFileMT::DebugMask & ID) == 0 || !QCLogFileMT::s_pLogFile)
		return;
	QCLogFileMT::WriteDebugLog(ID, Buffer, Length);
}

#endif


#endif // _EUDORA_DEBUG_H_
