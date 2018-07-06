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
FOR ANY. DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

File revised by Jeff Prickett (kg4ygs@gmail.com) on July 4, 2018
    Removed a conditional debug code section
File revised by Jeff Prickett                    on July 6, 2018. 
    Removed references to Qualcomm's Shareware Manager.

*/

// UsgStatsView.h: interface for the CUsageStatisticsView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USGSTATSVIEW_H__2705E758_3BFD_11D4_8559_0008C7D3B6F8__INCLUDED_)
#define AFX_USGSTATSVIEW_H__2705E758_3BFD_11D4_8559_0008C7D3B6F8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "UsgStatsDoc.h"
#include "HtmlEditView.h"
#include "statmng.h"

#include "eugraph.h"

#include "QICommandClient.h"

extern UINT uRefreshUsageStatisticsWindow;

class CUsageStatisticsView : public CPaigeEdtView , public QICommandClient
{
	DECLARE_DYNCREATE(CUsageStatisticsView)

public:
	CUsageStatisticsView();
	virtual ~CUsageStatisticsView();

	CUsageStatisticsDoc* GetDocument();
	void RefreshUsageStatisticsWindow(long wParam = 0, long lParam = 0);
	void RefreshGraphicalStatistics(long wParam = 0, long lParam = 0);
	void ReloadData(short sSel, BOOL bMoreStatistics = FALSE);
	void SetPeriodSelection(short sSel);
	void ShowMoreStatistics(BOOL bMoreStatistics);

	void DrawGraph(_DEuGraph *pGraph, short sSeriesCount, CString csXString, short sXIntervals, long**ppData, CString csLegendString, short sLabelDisplayStartOffset, short sLabelDisplaySkipCount);

	virtual void			Clear();
	virtual void			ResetCaret();
	virtual void			HideCaret();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUsageStatisticsView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	void ReplaceSpaceFillers();
	static UpdateFT() { };

private:
	StatData*	m_pStatData;
	short		m_GraphStyles[3];
	COLORREF	m_GraphColors[3];
	_DEuGraph	m_Graph[STATCOUNT /* For tracking the basic statistics */	+ 2 /* For tracking detailed statistics */ ];

	ULONG		m_nTimerEventID;

	CString		m_csHourString;
	CString		m_csWeekString;
	CString		m_csMonthString;
	CString		m_csYearString;

	BOOL		m_bGraphOCXRegistered;

	void FilterString(CString &csHTML,const char *szBegin,const char *szEnd);

	// Generated message map functions
protected:
	//{{AFX_MSG(CUsageStatisticsView)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		//}}AFX_MSG

	afx_msg long OnRefreshUsageStatisticsWindow( WPARAM	wParam, LPARAM	lParam );
	afx_msg LRESULT OnDisplayChange(WPARAM, LPARAM);

	LPDISPATCH GetIDispatch(COleClientItem *pItem);
public:
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_USGSTATSVIEW_H__2705E758_3BFD_11D4_8559_0008C7D3B6F8__INCLUDED_)
