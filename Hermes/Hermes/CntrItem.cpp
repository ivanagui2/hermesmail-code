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

// CntrItem.cpp : implementation of the CHermesCntrItem class
//

#include "stdafx.h"
#include "Hermes.h"

#include "HermesDoc.h"
#include "HermesView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHermesCntrItem implementation

IMPLEMENT_SERIAL(CHermesCntrItem, CRichEditCntrItem, 0)

CHermesCntrItem::CHermesCntrItem(REOBJECT* preo, CHermesDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: add one-time construction code here
}

CHermesCntrItem::~CHermesCntrItem()
{
	// TODO: add cleanup code here
}


// CHermesCntrItem diagnostics

#ifdef _DEBUG
void CHermesCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CHermesCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

