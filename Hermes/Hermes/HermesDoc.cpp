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
//	*	CHermesDoc class. Implementation.
//	*
//
//
//
//		PURPOSE:
//
//	*		This is the application's document class. In the future, when we will build OLE fully into this app
//	*		it will perhaps end up being derived from COleDocument with CRichEditDoc in the somewhere.
//	*
//	*			https://docs.microsoft.com/en-us/cpp/mfc/reference/coledocument-class?view=vs-2017
//	*			https://docs.microsoft.com/en-us/cpp/mfc/reference/cricheditdoc-class?view=vs-2017
//	*
//	*		Worth considering:
//	*
//	*			https://docs.microsoft.com/en-us/cpp/mfc/reference/chtmleditview-class?view=vs-2017#chtmleditview
//	*
//	*		Initialization, Serialization and accessors implemented.
//	*
//	*	[SBROTHY@GMAIL.COM]
//	*
//


// HermesDoc.cpp : implementation of the CHermesDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Hermes.h"
#endif


#include "HermesDoc.h"
#include "CntrItem.h"
#include "HermesView.h"
#include "MainFrm.h"


#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHermesDoc

IMPLEMENT_DYNCREATE(CHermesDoc, CRichEditDoc)

BEGIN_MESSAGE_MAP(CHermesDoc, CRichEditDoc)
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnUpdateEditLinksMenu)
	ON_UPDATE_COMMAND_UI(ID_OLE_VERB_POPUP, &CHermesDoc::OnUpdateObjectVerbPopup)
	ON_COMMAND(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, &CRichEditDoc::OnUpdateObjectVerbMenu)
	ON_COMMAND(ID_FILE_SEND_MAIL, &CHermesDoc::OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, &CHermesDoc::OnUpdateFileSendMail)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CHermesDoc, CRichEditDoc)
END_DISPATCH_MAP()

// Note: we add support for IID_IHermes to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .IDL file.

// {bf8a6846-3e36-45a1-84fc-087fd0f4da95}
static const IID IID_IHermes =
{0xbf8a6846,0x3e36,0x45a1,{0x84,0xfc,0x08,0x7f,0xd0,0xf4,0xda,0x95}};

BEGIN_INTERFACE_MAP(CHermesDoc, CRichEditDoc)
	INTERFACE_PART(CHermesDoc, IID_IHermes, Dispatch)
END_INTERFACE_MAP()


// CHermesDoc construction/destruction

CHermesDoc::CHermesDoc() noexcept
	: m_To(_T("To")),
	m_From(_T("From")),
	m_Subject(_T("Subject"))

{
	ASSERT(m_To.vt == VT_BSTR);
	ASSERT(m_From.vt == VT_BSTR);
	ASSERT(m_Subject.vt == VT_BSTR);

	GUID g;
	::CoCreateGuid(&g);

	LPCWSTR lpsz = ::GUIDAsStringW(g);
	m_GUID = lpsz;
	TRACE(_T("Document created with ID: [%s]\n"), lpsz);
	ASSERT(m_GUID.vt  == VT_BSTR);

	// Use OLE compound files
	EnableCompoundFile();

	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();
}

CHermesDoc::~CHermesDoc()
{
	AfxOleUnlockApp();
}

BOOL CHermesDoc::OnNewDocument()
{
	TRACE(__T("BOOL CHermesDoc::OnNewDocument() >>\n"));

	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	AssertValid();
//	CDumpContext * pDC = new CDumpContext();
//	Dump(*pDC);
//	delete pDC;

	CMainFrame * pFrame = dynamic_cast<CMainFrame *>(::AfxGetMainWnd());
	CMailDetailView *pDetailView = pFrame->GetMailDetailView();

	pDetailView->UpdateData(FALSE);

	TRACE(__T("BOOL CHermesDoc::OnNewDocument() <<\n"));
	return TRUE;
}

CRichEditCntrItem* CHermesDoc::CreateClientItem(REOBJECT* preo) const
{
	return new CHermesCntrItem(preo, const_cast<CHermesDoc*>(this));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Accessors
CComVariant *CHermesDoc::GetSetTo(CComVariant *v)
{
	if (v)
	{
		ASSERT(v->vt == VT_BSTR);
		m_To = v->bstrVal;
	}

	ASSERT(m_To.vt == VT_BSTR);

	return &m_To;
}


///////////////////////////////////////////////////////////////////////////////
CComVariant *CHermesDoc::GetSetFrom(CComVariant *v)
{
	if (v)
	{
		ASSERT(v->vt == VT_BSTR);
		m_From = v->bstrVal;
	}

	ASSERT(m_From.vt == VT_BSTR);

	return &m_From;
}


///////////////////////////////////////////////////////////////////////////////
CComVariant *CHermesDoc::GetSetSubject(CComVariant *v)
{
	if (v)
	{
		ASSERT(v->vt == VT_BSTR);
		m_Subject = v->bstrVal;
	}

	ASSERT(m_Subject.vt == VT_BSTR);

	return &m_Subject;
}




// CHermesDoc serialization
void CHermesDoc::Serialize(CArchive& ar)
{
	CArchiveStream pStream(&ar);

	if (ar.IsStoring())
	{
		m_GUID.WriteToStream(&pStream);
		m_To.WriteToStream(&pStream);
		m_From.WriteToStream(&pStream);
		m_Subject.WriteToStream(&pStream);
		ar << m_CC.size();
		for (auto& it : m_CC) {
			it.WriteToStream(&pStream);
		}
		ar << m_BCC.size();
		for (auto& it : m_BCC) {
			it.WriteToStream(&pStream);
		}
	}
	else
	{
		m_GUID.ReadFromStream(&pStream);
		m_To.ReadFromStream(&pStream);
		m_From.ReadFromStream(&pStream);
		m_Subject.ReadFromStream(&pStream);

		size_t n;
		CComVariant v;

		m_CC.clear();
		ar >> n;
		for (size_t i = 0; i < n; i++)
		{
			v.ReadFromStream(&pStream);
			m_CC.push_back(v);
		}

		m_BCC.clear();
		ar >> n;
		for (size_t i = 0; i < n; i++)
		{
			v.ReadFromStream(&pStream);
			m_BCC.push_back(v);
		}
	}

	// Calling the base class CRichEditDoc enables serialization
	//  of the container document's COleClientItem objects.
	// [SBROTHY]: TODO: set CRichEditDoc::m_bRTF = FALSE if you are serializing as text
	CRichEditDoc::Serialize(ar);
}


#ifdef SHARED_HANDLERS

// Support for thumbnails
void CHermesDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CHermesDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CHermesDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CHermesDoc diagnostics

#ifdef _DEBUG
void CHermesDoc::AssertValid() const
{
	CRichEditDoc::AssertValid();

	ASSERT(m_To.vt == VT_BSTR);
	ASSERT(m_From.vt == VT_BSTR);
	ASSERT(m_Subject.vt == VT_BSTR);
	for (auto& it : m_CC)
		ASSERT(it.vt == VT_BSTR);
	for (auto& it : m_BCC)
		ASSERT(it.vt == VT_BSTR);
}

void CHermesDoc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG


// CHermesDoc commands
