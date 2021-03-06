// DEBUG.CPP
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
    Removed program mode references from the debug logging code.
    Removed a reference to IDS_INI_MODE as the program now only has one more, Pro. 

*/

#include "stdafx.h"
#include "resource.h"

#include <strsafe.h>

#include "debug.h"

#include "QCUtils.h"

//#include "fileutil.h"  //EudoraDir

#include "rs.h"



#include "DebugNewHelpers.h"


// I had to make the following change to get this to link in debug mode (Pete Maclean 4-Sep-2018)
#undef DEBUG_NEW_NOTHROW
#define DEBUG_NEW_NOTHROW new(std::nothrow)

bool		QCLogFileMT::s_bOutputEudoraStatusInfoNow = true;
DebugMaskType	QCLogFileMT::DebugMask = 0;
long		QCLogFileMT::DebugLogSize;
const time_t QCLogFileMT::kInitialTime = 1;
time_t		QCLogFileMT::MarkerTime = kInitialTime;
JJFileMT*	QCLogFileMT::s_pLogFile = NULL;

CString		QCLogFileMT::g_strLogFileName;
CString		QCLogFileMT::g_strEudoraDir;
CCriticalSection QCLogFileMT::m_Guard;
BOOL		QCLogFileMT::m_bIsNT = FALSE;

CString QCLogFileMT::m_AssertFmtStr = "ASSERT FAILED: [\"%s\"] %s (%d)";


// In enabling this function for Hermes so that the original Eudora executable could work with a newly built
// QCUtils.dll (using MSVC version 14), I found that I had to add an extra parameter.  My guess is that the
// version of MSVC used to build Eudora.exe used a different calling convention for static class methods that
// included an instance reference!  When the main executable is rebuilt for Hermes, this [dwIgnore] parameter
// should be removed.  Pete Maclean, 9-Nov-2018.

BOOL QCLogFileMT::InitDebug(
	DWORD dwIgnore,			// [in] ??? artefact of old MSVC??
	DWORD dwMask,			// [in] debugging options (set of DEBUG_MASK_xxx values)
	DWORD dwSize,			// [in] size for the log file in KB
	LPCTSTR strEudoraDir,	// [in] the Eudora directory
	LPCTSTR strLogFileName)	// [in] the (unqualified) name for the Eudora log file
{
#if _DEBUG
//	__debugbreak();

	char szDebug[512];
	StringCchPrintfA(szDebug, _countof(szDebug),
		"QCLogFileMT::InitDebug(Mask = 0x%X, Size = %u, EudoraDirectory = \"%s\", LogFileName = \"%s\"\r\n",
		(unsigned int)dwMask, (unsigned int)dwSize, strEudoraDir, strLogFileName);
	::OutputDebugStringA(szDebug);
#endif

	ASSERT(::IsMainThreadMT());
	
	DebugMask = dwMask;
	DebugLogSize = dwSize;
	g_strEudoraDir = strEudoraDir;

	OSVERSIONINFO osInfo;
	osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (GetVersionEx(&osInfo) && osInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
		m_bIsNT = TRUE;
	
	// No need to do anything if there are no conditions to log
	if (DebugMask)
	{
		if (!s_pLogFile)
		{
			s_pLogFile = DEBUG_NEW_NOTHROW JJFileMT;
			if(!s_pLogFile) return FALSE;
		}

		char Filename[_MAX_PATH + 1];
		//wsprintf(Filename, "%s%s", (const char*)EudoraDir, GetIniString(IDS_INI_DEBUG_LOG_FILE_NAME));
		wsprintfA(Filename, "%s%s", strEudoraDir, strLogFileName);

		g_strLogFileName = Filename;

		//if (FAILED(s_pLogFile->Open(Filename, O_CREAT | O_APPEND | O_WRONLY)))
		if (FAILED(s_pLogFile->Open(g_strLogFileName, O_CREAT | O_APPEND | O_WRONLY)))
		{
			// Format the error string (just like JJFile does)
			CString		strError;
			char szWriting[16] = "writing";
			char szDiagnostic[256] = { 0 };

			::LoadStringA(NULL, IDS_ERR_FILE_OPEN_WRITING, szWriting, _countof(szWriting));

			::AfxFormatString2(strError, IDS_ERR_FILE_OPEN,
				static_cast<const char *>(g_strLogFileName), szWriting);

			// Format the error message (just like JJFile does)
			int nError = s_pLogFile->GetLastError_();
			if (nError < 0 || nError > 36)
				nError = 37;		// unknown error

			const int IDS_FILE_BASE_ERROR = 8500;

			CString		strErrorMessage;

			::LoadStringA(NULL, IDS_FILE_BASE_ERROR + nError, szDiagnostic, _countof(szDiagnostic));

			strErrorMessage.Format(IDS_DOS_ERR_FORMAT, static_cast<const char *>(strError), szDiagnostic, nError);

			// Combine the file error message with message explaining that Eudora
			// can't work without the ability to write to Eudora.log.
			CString		strMessage;

			strMessage.Format( IDS_ERR_OPENING_DEBUG_LOG,
							   static_cast<const char *>(strErrorMessage),
							   strLogFileName );

			// Display the error message before we return FALSE and Eudora quits
			MessageBox(NULL, strMessage, NULL, MB_OK | MB_ICONERROR);

			delete s_pLogFile;
			s_pLogFile = NULL;

			return (FALSE);
		}
	}

	return (TRUE);
}


void QCLogFileMT::NotifyIniChange(UINT nStringNum, long nValue)
{
	switch (nStringNum)
	{
		case IDS_INI_DEBUG_LOG_LEVEL:
			s_bOutputEudoraStatusInfoNow = ( DebugMask != static_cast<DebugMaskType>(nValue) );
			DebugMask = nValue;
			break;

	}
}


void QCLogFileMT::CleanupDebug()
{
	WriteDebugLog(DebugMask, "Logging shutdown");
	delete s_pLogFile;
	s_pLogFile = NULL;
}


#ifdef TRUE //_DEBUG

// Place null terminated character data into the log file
void PutDebugLog(DebugMaskType ID, LPCTSTR Buffer, int Length /* = -1*/)
{
	if ((QCLogFileMT::DebugMask & ID) == 0 || !QCLogFileMT::s_pLogFile)
		return;
	QCLogFileMT::WriteDebugLog(ID, Buffer, Length);
}

#endif


BOOL QCLogFileMT::DebugMaskSet(DebugMaskType Mask)
{ 
	return ((DebugMask & Mask) != 0); 
}


void QCLogFileMT::PutLineHeader(DebugMaskType ID)
{
	char DigitString[32];
	time_t Now = time(NULL);

	// Close and reopen the file every minute so that when running under OSes like Win 3.1 the file gets updated
	ASSERT(s_pLogFile);
	if ((Now - MarkerTime) % 60 == 0)
	{
		s_pLogFile->Close();

		CString strFilename("???");
		{
			BSTR bstrFilename = NULL;
			if (SUCCEEDED(s_pLogFile->GetFName(&bstrFilename)))
			{
				strFilename = bstrFilename;
				::SysFreeString(bstrFilename);
			}
		}

		if (FAILED(s_pLogFile->Open(strFilename, O_CREAT | O_APPEND | O_WRONLY)))
		{
			delete s_pLogFile;
			s_pLogFile = NULL;
			return;
		}
	}

	if ( s_bOutputEudoraStatusInfoNow || (Now > MarkerTime + 1200) )
	{
		if (MarkerTime == kInitialTime)
		{
			// Make it more obvious where Eudora is actually launching
			if ( FAILED(s_pLogFile->PutLine()) ) return;
		}

		// Write out the time and version of Eudora
		if (FAILED(s_pLogFile->PutLine(ctime(&Now), 24))) return;

		// The version string is something like "Version 7.1.0.9"
		char szVersion[32] = { 0 };
		::LoadStringA(NULL, IDS_VERSION, szVersion, _countof (szVersion));

		if (FAILED(s_pLogFile->PutLine(szVersion))) return;

		// Write out the log level
		sprintf(DigitString, "LogLevel %d (0x%X)", QCLogFileMT::DebugMask, QCLogFileMT::DebugMask);
		if (FAILED(s_pLogFile->PutLine(DigitString))) return;

		MarkerTime = Now;
		s_bOutputEudoraStatusInfoNow = false;
	}

	//First write the thread ID
	if (::IsMainThreadMT())
	{
		strcpy(DigitString, "MAIN ");
	}
	else
	{
		if (m_bIsNT)
			wsprintfA(DigitString, "%-4.0u ", ::GetCurrentThreadId());  //reuse the digitstring
		else
			wsprintfA(DigitString, "%-4.0X ", ::GetCurrentThreadId() & 0xFFFF);  //reuse the digitstring
	}
	if (FAILED(s_pLogFile->Put(DigitString))) return;
	

	wsprintfA(DigitString, "%5d", ID);
	if (FAILED(s_pLogFile->Put(DigitString))) return;
	if (FAILED(s_pLogFile->Put(":"))) return;
	wsprintfA(DigitString, "%2ld", (Now - MarkerTime) / 60);
	if (FAILED(s_pLogFile->Put(DigitString))) return;
	if (FAILED(s_pLogFile->Put("."))) return;
	wsprintfA(DigitString, "%02ld", (Now - MarkerTime) % 60);
	if (FAILED(s_pLogFile->Put(DigitString))) return;
	if (FAILED(s_pLogFile->Put(" "))) return;

	UINT uDebugID = 0;

	switch (ID)
	{
	case DEBUG_MASK_TRANS:		uDebugID = IDS_DEBUG_SENT;		break;
	case DEBUG_MASK_RCV:		uDebugID = IDS_DEBUG_RECEIVED;	break;
	case DEBUG_MASK_DIALOG:		uDebugID = IDS_DEBUG_DIALOG;	break;
	case DEBUG_MASK_TRANS_BASIC:	
	case DEBUG_MASK_TRANS_ADV:	uDebugID = IDS_DEBUG_TRANS;	break;
	}

	if (uDebugID != 0)
	{
		char szDebug[32] = { 0 };

		::LoadStringA(NULL, uDebugID, szDebug, _countof(szDebug));

		if (szDebug[0] != 0)
		{
			s_pLogFile->Put(szDebug);
		}
	}
}



void QCLogFileMT::PutLineTrailer(DebugMaskType ID)
{
	if (ID == DEBUG_MASK_TRANS || ID == DEBUG_MASK_RCV || ID == DEBUG_MASK_DIALOG)
		s_pLogFile->PutLine("\"");
	else
		s_pLogFile->PutLine();
}

void QCLogFileMT::WriteAssertLog(LPCTSTR pExpr, LPCTSTR pFileName, int nLineNum)
{
	// If no options are set, then ignore this call
	if (((DebugMask & DEBUG_MASK_ASSERT) == 0) || (!s_pLogFile))
		return;

	char str[1024];
	const int nLen = sprintf(str, m_AssertFmtStr, pExpr, pFileName, nLineNum);

	WriteDebugLog(DEBUG_MASK_ASSERT, str, nLen);
}

//
// Outputs a single line.  Input is a null terminated string.
//
//void QCLogFileMT::PutDebugLog(DebugMaskType ID, LPCTSTR Buffer, int Length /*= -1*/)
void QCLogFileMT::WriteDebugLog(DebugMaskType ID, LPCTSTR Buffer, int Length /*= -1*/)
{
	if (!Buffer || !*Buffer)
		return;

	//
	// Normally, you should only call this if you have a known
	// good logfile object.  However, in the startup case, it is
	// possible to get an error when trying to open the this logfile.
	// The problem is that the code tries to log a logfile open error
	// (your basic chicken/egg problem).  Therefore, the solution
	// seems to be to bounce attempts to write to an invalid logfile
	// here at a low level.
	//
	if ((s_pLogFile == NULL) || (S_FALSE == s_pLogFile->IsOpen()))
		return;

	if (Length < 0)
		Length = strlen(Buffer);

	// If no options are set, then ignore this call
	if ((DebugMask & ID) == 0 || !s_pLogFile)
		return;

	CSingleLock lock(&m_Guard, TRUE);

	BOOL WasCR = FALSE;
	BOOL StartNewLine = TRUE;
	for (; Length; Buffer++, Length--)
	{
		if (StartNewLine)
		{
			PutLineHeader(ID);
			StartNewLine = FALSE;
		}
		if (*Buffer == '\r')
		{
			if (FAILED(s_pLogFile->Put("\\r"))) return;
			WasCR = TRUE;
		}
		else if (*Buffer == '\n')
		{
			if (FAILED(s_pLogFile->Put("\\n"))) return;
			PutLineTrailer(ID);
			StartNewLine = TRUE;
			WasCR = FALSE;
		}
		else
		{
			if (WasCR)
			{
				PutLineTrailer(ID);
				WasCR = FALSE;
				PutLineHeader(ID);
			}
			if (*Buffer >= ' ' && *Buffer <= 0x7F)	{ s_pLogFile->Put(*Buffer); }
			else if (*Buffer == '\t')				{ s_pLogFile->Put("\\t"); }
			else if (*Buffer == '\b')				{ s_pLogFile->Put("\\b"); }
			else if (*Buffer == '\f')				{ s_pLogFile->Put("\\f"); }
			else if (*Buffer == '\007')				{ s_pLogFile->Put("\\g"); }
			else
			{
				if (FAILED(s_pLogFile->Put('\\'))) return;
				if (FAILED(s_pLogFile->Put((char)((((*Buffer) >> 6) & 0x01) + '0')))) return;
				if (FAILED(s_pLogFile->Put((char)((((*Buffer) >> 3) & 0x03) + '0')))) return;
				if (FAILED(s_pLogFile->Put((char)((((*Buffer) >> 0) & 0x03) + '0')))) return;
			}
		}
	}

	if (!StartNewLine)
		PutLineTrailer(ID);
	s_pLogFile->Flush();

	struct stat statbuf;
	s_pLogFile->Stat(&statbuf);
	if (statbuf.st_size > (DebugLogSize * 1024))
	{
		char Filename[_MAX_PATH + 1];
		char szLogFileNameOld[_MAX_FNAME] = { 0 };

		// Load the name for old log files (e.g. "eudorlog.old")
		::LoadStringA(NULL, IDS_DEBUG_LOG_FILE_NAME_OLD, szLogFileNameOld, _countof(szLogFileNameOld));

		strcpy(Filename, g_strEudoraDir);
		strcat(Filename, szLogFileNameOld);
		::FileRemoveMT(Filename);
		s_pLogFile->Rename(Filename);
		s_pLogFile->Close();
		//strcpy(Filename + EudoraDirLen, GetIniString(IDS_INI_DEBUG_LOG_FILE_NAME));
		//s_pLogFile->Open(Filename, O_CREAT | O_WRONLY | O_TRUNC);
		s_pLogFile->Open(g_strLogFileName, O_CREAT | O_WRONLY | O_TRUNC);

		// Make sure new log file starts with the logging info
		s_bOutputEudoraStatusInfoNow = true;
	}
}
