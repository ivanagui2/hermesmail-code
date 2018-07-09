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

File revised by Jeff Prickett (kg4ygs@gmail.com) on July 8, 2018.
    Removed references to the Stingray Toolkit and replaced them with references
    to the Hermes UI Toolkit.

*/    

// QCToolBarManager.h: interface for the QCToolBarManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QCTOOLBARMANAGER_H__A46998C2_ED9C_11D0_9800_00805FD2F268__INCLUDED_)
#define AFX_QCTOOLBARMANAGER_H__A46998C2_ED9C_11D0_9800_00805FD2F268__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class QCToolBarManagerWithBM : public HRMToolBarManager  
{
public:
	DECLARE_DYNAMIC( QCToolBarManagerWithBM )

	QCToolBarManagerWithBM( CFrameWnd* pFrame = NULL );
	virtual ~QCToolBarManagerWithBM();

	virtual BOOL AddButtonImage( CBitmap& theBitmap, UINT uID );
	virtual void CopyButtonImage( UINT uCopyButtonIndex, UINT uID );
	virtual void CopyButtonImage( CImageList *pImageList, int imageOffset, UINT uID, BOOL drawEdge = FALSE);
	virtual void DeleteBitmapAt( UINT iIndex );
	virtual void DeleteAllUnusedBitmaps();

protected:
};

class QCToolBarManager : public QCToolBarManagerWithBM  
{
public:
	CFrameWnd* GetFrameWnd() { return m_pFrameWnd; }
	QCToolBarManager( CFrameWnd* pFrame = NULL );

	DECLARE_DYNAMIC( QCToolBarManager )

	virtual ~QCToolBarManager();

	virtual BOOL LoadToolBarResource();
	virtual BOOL LargeButtonsEnabled();
	virtual BOOL QCEnableLargeBtns(BOOL bEnable=TRUE, BOOL bForceSwitch=FALSE);		// c_dalew 10/11/99 - See implementation.
	virtual void QCLoadState(LPCTSTR lpszProfileName);								// c_dalew 10/11/99 - See implementation.
	virtual void LoadState(LPCTSTR lpszProfileName);
	virtual void OnCustomize();
	virtual void SaveState(LPCTSTR lpszProfileName);
	
	virtual HRMCustomToolBar* CreateUserToolBar(LPCTSTR lpszTitle = NULL,
												DWORD dwStyle = -1,
												DWORD dwExStyle = -1);
	BOOL ConvertOldStuff();
	virtual CControlBar* DynCreateControlBar(HRMControlBarInfo* pBarInfo);
};

#endif // !defined(AFX_QCTOOLBARMANAGER_H__A46998C2_ED9C_11D0_9800_00805FD2F268__INCLUDED_)
