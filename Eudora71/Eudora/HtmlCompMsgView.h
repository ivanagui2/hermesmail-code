// PgCompMsgView.h
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
    Moved PgCompMsgView.h to HtmlCompMsgView.h and removed most protected and
    private function and variable definitions.

*/        

//

#if !defined(AFX_PGCOMPMSGVIEW_H__35E93281_E9CD_11D0_AC4F_00805FD2626C__INCLUDED_)
#define AFX_PGCOMPMSGVIEW_H__35E93281_E9CD_11D0_AC4F_00805FD2626C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PgCompMsgView.h : header file
//

#include "HtmlMsgView.h"
#include "headervw.h"
#include "MoodWatch.h"

class CMessageDoc;

/////////////////////////////////////////////////////////////////////////////
// HtmlCompMsgView view

class HtmlCompMsgView : public HtmlMsgView
{
protected:
	HtmlCompMsgView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(HtmlCompMsgView)

// Implementation
protected:
	virtual ~HtmlCompMsgView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
// Attributes
public:

// Operations
public:

	BOOL GetMessageAsHTML(CString& szText, BOOL includeHeaders);
	BOOL GetMessageAsText(CString& szText, BOOL includeHeaders);
	BOOL OnSignatureChanged();
	BOOL IsHeaderMoodDirty(){ return m_pHeader->IsHeaderMoodDirty();}
	void SetHeaderMoodDirty(bool bStatus){ m_pHeader->SetHeaderMoodDirty(bStatus);}
	BOOL IsPaigeMoodDirty(){ return m_bMoodMailDirty;};
	void SetPaigeMoodDirty(bool bStatus){ m_bMoodMailDirty = bStatus;}
	virtual int DoMoodMailCheck(CMoodWatch *pmoodwatch = NULL);
	void UpdateMoodMailButton(int nScore);
	BOOL IsHeaderBPDirty(){ return m_pHeader->IsHeaderBPDirty();}
	void SetHeaderBPDirty(bool bStatus){ m_pHeader->SetHeaderBPDirty(bStatus);}
	BOOL IsHeaderBPSpeakDirty(){ return m_pHeader->IsHeaderBPSpeakDirty();}
	BOOL IsHeaderBPWarnDlgDirty(){ return m_pHeader->IsHeaderBPWarnDlgDirty();}
	void SetHeaderBPSpeakDirty(bool bStatus){ m_pHeader->SetHeaderBPSpeakDirty(bStatus);}
	void SetHeaderBPWarnDlgDirty(bool bStatus){ m_pHeader->SetHeaderBPWarnDlgDirty(bStatus);}
	bool DoBPCheck(CStringList * pAddressList = NULL) { return m_pHeader->DoBPCheck(pAddressList); }
	void ShowMoodMailButton(BOOL bStatus);
	void SetMoodTimer();
	void KillMoodTimer();
	void UpdateBPButton(bool bBPWarning);
	int GetMoodScore (){return m_nScore;}
	void SetMoodScore (int nScore ) { m_nScore = nScore;}

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PgCompMsgView)
	public:
	virtual void OnInitialUpdate();
	virtual void OnDraw( CDC* pDC );
	//}}AFX_VIRTUAL

    protected:
        CHeaderView* m_pHeader;
        int m_nScore;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PGCOMPMSGVIEW_H__35E93281_E9CD_11D0_AC4F_00805FD2626C__INCLUDED_)
