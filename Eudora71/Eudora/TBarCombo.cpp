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

File revised by Jeff Prickett (kg4ygs@gmail.com) on July 8, 2018
    Removed references to the Stingray Toolkit and replaced them with references
    to the Hermes UI Toolkit.

*/    

#include "stdafx.h"
#include "eudora.h"
#include "TBarCombo.h"

IMPLEMENT_BUTTON(CTBarComboBtn);

//  Commentedout this #include to a nonexistent file. Should it turn up again and prove useful,
//  we can  uncomment it. I'm not optimistic  about it though. (8/9/18. soren <sbrothy@gmail.com>)
//#include "DebugNewHelpers.h"


// WINDOWSX.H has a SubclassWindow macro which collides
#undef SubclassWindow


BOOL CTBarComboBtn::CreateWnd(CWnd* pParentWnd, DWORD dwStyle, CRect& rect, int nID)
{
	if(!(dwStyle & (CBS_DROPDOWNLIST | CBS_DROPDOWN)))
		dwStyle |= CBS_DROPDOWNLIST;

	// Create the combo box.
	BOOL bRes = CComboBox::Create(dwStyle, rect, pParentWnd, nID);
	ASSERT(bRes);
	if(bRes)
	{
		if(dwStyle & CBS_DROPDOWN)
		{
			// Subclass the child edit.
			CWnd* pWnd = GetWindow(GW_CHILD);
			if(pWnd != NULL)
			{
				VERIFY(m_edit.SubclassWindow(pWnd->m_hWnd));
				m_edit.m_pCombo = (HRMWndBtn*) this;

			}
		}

		OnFontCreateAndSet();
		AdjustSize();		// save latest and greatest size info
	}

	return bRes;
}


void CTBarComboBtn::OnFontCreateAndSet() {
	// set default font for this control for proper sizing
	m_font.CreateFont (8, 0, 0, 0, FW_NORMAL, 0, 0, 0,
        	   DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, 
			   CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, 
               DEFAULT_PITCH | FF_DONTCARE, _T("MS Sans Serif"));
   
	SetFont (&m_font,TRUE);
}

