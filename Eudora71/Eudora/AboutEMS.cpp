// AboutEMS.cpp : implementation file
//
// Copyright (c) 1995-2000 by QUALCOMM, Incorporated
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
#include "AboutEMS.h"
#include "ems-wglu.h"
#include "trnslate.h"
#include "eudora.h"

#include "DebugNewHelpers.h"

//------------------------------------------------------------------------------------------



/////////////////////////////////////////////////////////////////////////////
// CAboutPlugInDlg dialog


CAboutPlugInDlg::CAboutPlugInDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutPlugInDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutPlugInDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAboutPlugInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutPlugInDlg)
	DDX_Control(pDX, IDC_PLUGINS, m_PlugInList);
	DDX_Control(pDX, IDC_SETTINGS, m_Settings);
	//}}AFX_DATA_MAP
}

BOOL CAboutPlugInDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CTransAPI *ptrapi;
	CTransAPIArray *transAr = ((CEudoraApp *)AfxGetApp())->GetTranslators()->GetModules();
	int sz = transAr->GetSize();
	for(int i = 0;  i < sz; i++ )
	{
		ptrapi = transAr->GetAt(i);

		// Add Item To List Box
		if (ptrapi)
			m_PlugInList.AddString((LPCSTR)ptrapi);
	}
	if (sz > 0)
		m_PlugInList.SetCurSel(0);
	OnSelChange();
	return TRUE;  
}


void CAboutPlugInDlg::OnSelChange()
{
	int sel = m_PlugInList.GetCurSel();
	
	if (sel >= 0 && sel != LB_ERR)
	{
		CTransAPI* ptrapi = (CTransAPI *)m_PlugInList.GetItemData(sel);
		if (ptrapi->GetSettingsFunc())
		{
			m_Settings.EnableWindow(TRUE);
			return;
		}
	}
	
	m_Settings.EnableWindow(FALSE);

}

void CAboutPlugInDlg::OnSettings()
{
	m_PlugInList.Configure();
}


BEGIN_MESSAGE_MAP(CAboutPlugInDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutPlugInDlg)
	ON_LBN_SELCHANGE(IDC_PLUGINS, OnSelChange)
	ON_BN_CLICKED(IDC_SETTINGS, OnSettings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutPlugInDlg message handlers
/////////////////////////////////////////////////////////////////////////////
// CPlugInListBox
BEGIN_MESSAGE_MAP(CPlugInListBox, CListBox)
	//{{AFX_MSG_MAP(CPlugInListBox)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlugInListBox message handlers

int CPlugInListBox::CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/) 
{
	return 0;
}

void CPlugInListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if (NULL == pDC || NULL == pDC->m_hDC)
		return;
	CRect rect(lpDrawItemStruct->rcItem);
	
	int itemNumber = lpDrawItemStruct->itemID;
	CTransAPI* ptrapi = NULL;
	if (itemNumber >= 0)
		ptrapi = (CTransAPI *)lpDrawItemStruct->itemData;

	// Set up some color values
	COLORREF BackColor, ForeColor;
	
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		BackColor = GetSysColor(COLOR_HIGHLIGHT);
		ForeColor = GetSysColor(COLOR_HIGHLIGHTTEXT);
	}
	else
	{
		BackColor = GetSysColor(COLOR_WINDOW);
		ForeColor = GetSysColor(COLOR_WINDOWTEXT);
	}

	if (ptrapi)
	{  
		// Draw background
		CBrush BackBrush(BackColor);
		pDC->FillRect(&rect, &BackBrush);
	
		// scoot everything down
		rect.top += 2;  
		rect.left += 2;  

		HICON icn;
		ptrapi->GetIcon(icn);

#ifdef WIN32
		{
			//
			// Warning!  The GetIconInfo() call creates two bitmaps
			// that must be deleted before we return!
			//
			ICONINFO ii; 
			BITMAP bm;
			if (::GetIconInfo(icn, &ii))
			{
				ASSERT(ii.hbmMask);
				ASSERT(ii.hbmColor);
				GetObject(ii.hbmMask, sizeof(BITMAP), &bm); 
				if ( (bm.bmWidth == 32) && (bm.bmHeight == 32) )
					pDC->DrawIcon(rect.left, rect.top, icn);
				else
				{
					CDC MemDC;
					if (MemDC.CreateCompatibleDC(pDC))
					{
						// Get the bitmask
						GetObject(ii.hbmMask, sizeof(BITMAP), &bm);
						CBitmap* scaledBM = CBitmap::FromHandle(ii.hbmMask);
						if (scaledBM)
						{
							CBitmap *oldBM = MemDC.SelectObject(scaledBM);
							// Blt the bitmask
							pDC->StretchBlt(rect.left, rect.top,
											32, 32,
											&MemDC, 
											0, 0,
											bm.bmWidth, bm.bmHeight, 
											SRCAND);

							// Blt the colormask
							GetObject(ii.hbmColor, sizeof(BITMAP), &bm);
							CBitmap* coloredBM = CBitmap::FromHandle(ii.hbmColor);
							if (coloredBM)
							{
								CBitmap *old = MemDC.SelectObject(coloredBM);
								pDC->StretchBlt(rect.left, rect.top, 
												32, 32,
												&MemDC, 
												0, 0,
												bm.bmWidth, bm.bmHeight, 
												SRCINVERT);

								if(old)
									MemDC.SelectObject(oldBM);
							}
							// Restore 
							if (oldBM)
								MemDC.SelectObject(oldBM);
						}
					}
				}

				::DeleteObject(ii.hbmMask);
				::DeleteObject(ii.hbmColor);
			}
		}
 #else
		pDC->DrawIcon(rect.left, rect.top, icn);
 #endif
		// Draw name
		CString desc;
		ptrapi->GetDesc(desc);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(ForeColor);
		rect.left += 10 + 32;
		pDC->ExtTextOut(rect.left, rect.top + 8, 0, &rect, desc, desc.GetLength(), 0);
	}
}

void CPlugInListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight=36;

}

void CPlugInListBox::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct) 
{
	CListBox::DeleteItem(lpDeleteItemStruct);
}


void CPlugInListBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CListBox::OnLButtonDblClk(nFlags, point);
	Configure();
}

void CPlugInListBox::Configure()
{
	int sel = GetCurSel();
	if (sel >= 0 && sel != LB_ERR)
	{
		CTransAPI* ptrapi = (CTransAPI *)GetItemData(sel);
		if (ptrapi->GetSettingsFunc())
			ptrapi->Settings();
	}
	
}
	
