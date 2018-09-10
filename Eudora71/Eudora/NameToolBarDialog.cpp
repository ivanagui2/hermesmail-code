// NameToolBarDialog.cpp : implementation file
//

#include "stdafx.h"
#include "eudora.h"
#include "NameToolBarDialog.h"

//  Commentedout this #include to a nonexistent file. Should it turn up again and prove useful,
//  we can  uncomment it. I'm not optimistic  about it though. (8/9/18. soren <sbrothy@gmail.com>)
#include "DebugNewHelpers.h"


/////////////////////////////////////////////////////////////////////////////
// CNameToolBarDialog dialog


CNameToolBarDialog::CNameToolBarDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNameToolBarDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNameToolBarDialog)
	m_szName = _T("");
	//}}AFX_DATA_INIT
}


void CNameToolBarDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNameToolBarDialog)
	DDX_Text(pDX, IDC_TOOL_BAR_NAME, m_szName);
	DDV_MaxChars(pDX, m_szName, 15);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNameToolBarDialog, CDialog)
	//{{AFX_MSG_MAP(CNameToolBarDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNameToolBarDialog message handlers
