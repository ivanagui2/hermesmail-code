// DockBar.cpp : implementation file
//
// Copyright (c) 1999-2000 by QUALCOMM, Incorporated
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

#include "stdafx.h"

#include "resource.h"
#include "DockBar.h"
#include "AdWazooBar.h"
#include "QCSharewareManager.h"

#include "DebugNewHelpers.h"

/////////////////////////////////////////////////////////////////////////////
// QCDockBar

QCDockBar::QCDockBar()
{
}

QCDockBar::~QCDockBar()
{
}


/////////////////////////////////////////////////////////////////////////////
// FindControlBarLocation [public]
//
// Returns in what column and row a given control bar is located.
/////////////////////////////////////////////////////////////////////////////
bool QCDockBar::FindControlBarLocation(SECControlBar* in_pBar, int & out_nCol, int & out_nRow)
{
	bool	bFound = false;

	out_nCol = 0;
	out_nRow = -1;
	
	for (int nPos = 0; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar *		pBar = GetDockedControlBar(nPos);

		if (pBar == in_pBar)
		{
			bFound = true;
			break;
		}

		void* pVoid = m_arrBars[nPos];
		if (pBar != NULL && pBar->IsVisible())
		{
			out_nCol++;
		}
		else if (pBar == NULL && pVoid == NULL)
			// end of row because pBar == NULL
		{
			out_nCol = 0;
			out_nRow++;
		}
	}

	return bFound;
}



BEGIN_MESSAGE_MAP(QCDockBar, SECDockBar)
	//{{AFX_MSG_MAP(QCDockBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// QCDockBar message handlers
