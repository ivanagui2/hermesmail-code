//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Cop't 1991-2005 Qualcomm, 2019 HERMES, use of this software is governed by these terms
// 
//	Copyright 2019 HERMES Team
// 
//	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met :
//
//	1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
//	
//	2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
//
//	3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
//
//	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
//	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;  OR BUSINESS INTERRUPTION) 
//	HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
//	EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	CChildFrm class. Declaration.
//
//
//	PURPOSE:
//
//	This is the frame(window) around the child view in this MDI application.
//
//[SBROTHY@GMAIL.COM]
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ChildFrm.h : interface of the CChildFrame class
//

#pragma once

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame() noexcept;

// Attributes
protected:
	CSplitterWndEx m_wndSplitter;
public:

// Operations
public:

// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};
