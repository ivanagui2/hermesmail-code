// CURSOR.CPP
//
// Cursor class because MFC forgot one

#include "stdafx.h"

#include "cursor.h"
#include "utils.h"

//  Commentedout this #include to a nonexistent file. Should it turn up again and prove useful,
//  we can  uncomment it. I'm not optimistic  about it though. (8/9/18. soren <sbrothy@gmail.com>)
#include "DebugNewHelpers.h"



static BOOL OverOurWindow()
{
	POINT pt;
	CWnd* MainWindow = AfxGetMainWnd();
	HWND hwnd;

	GetCursorPos(&pt);
	hwnd = WindowFromPoint(pt);
#ifdef unix	// if hwnd==0, warning will follow
	if (hwnd)
#endif
	if (MainWindow && IsChild(MainWindow->m_hWnd, hwnd))
		return (TRUE);

	return (FALSE);
}


int CCursor::m_Count = 0;
BOOL CCursor::m_bIgnore = FALSE;

CCursor::CCursor(LPCSTR CursorName /*= IDC_WAIT*/)
{
	m_PrevCursor = NULL;
	m_SetCursor = FALSE;

	if ( ! m_bIgnore && OverOurWindow() )
	{
		HCURSOR NewCursor = ::LoadCursor(NULL, CursorName);
		if ( ! NewCursor )
			NewCursor = QCLoadCursor(CursorName);
		if ( NewCursor )
		{
			m_PrevCursor = SetCursor(NewCursor);
			m_SetCursor = TRUE;
		}
	}

	m_Count++;
}

CCursor::~CCursor()
{
	Restore();
	m_Count--;
}

void CCursor::Restore()
{
	if ( m_SetCursor && OverOurWindow())
	{
		SetCursor(m_PrevCursor);
		m_PrevCursor = NULL;
		m_SetCursor = FALSE;
	}
}

void CCursor::Ignore( BOOL bIgnore )
{
	m_bIgnore = bIgnore;
}
