//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
//
//
//	CHermesCntrItem class. Declaration.
//
//	PURPOSE:
//	The amount of OLE I managed to sneak in without everything exploding in my face. Purpose so far unidentified, but
//	is's basically part of OLE. This is the first step for an external server or client to access automated parts of 
//	our app.
//
//
//	[SBROTHY@GMAIL.COM]
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// CntrItem.h : interface of the CHermesCntrItem class
//

#pragma once

class CHermesDoc;
class CHermesView;

class CHermesCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CHermesCntrItem)

// Constructors
public:
	CHermesCntrItem(REOBJECT* preo = nullptr, CHermesDoc* pContainer = nullptr);
		// Note: pContainer is allowed to be null to enable IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-null document pointer

// Attributes
public:
	CHermesDoc* GetDocument()
		{ return reinterpret_cast<CHermesDoc*>(CRichEditCntrItem::GetDocument()); }
	CHermesView* GetActiveView()
		{ return reinterpret_cast<CHermesView*>(CRichEditCntrItem::GetActiveView()); }

// Implementation
public:
	~CHermesCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

