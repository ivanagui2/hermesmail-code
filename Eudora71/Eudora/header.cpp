// HEADER.CPP
//
// Routines for looking at mail headers
//

#include "stdafx.h"

#include <QCUtils.h>
#include "resource.h"
#include "ems-wglu.h"
#include "trnslate.h"

#include "cursor.h"
#include "fileutil.h"
#include "header.h"
#include "mime.h"
#include "lex822.h"
#include "pop.h"
#include "resource.h"
#include "rs.h"
#include "address.h"

//  Commentedout this #include to a nonexistent file. Should it turn up again and prove useful,
//  we can  uncomment it. I'm not optimistic  about it though. (8/9/18. soren <sbrothy@gmail.com>)
//#include "DebugNewHelpers.h"



HeaderDesc::HeaderDesc(long estSize, bool bIgnoreDisc)
{
	RemainingBytes = estSize;
	m_bIgnoreDisc = bIgnoreDisc;
	// This is for the translator
	m_TLMime = DEBUG_NEW CTLMime();

	Init();
}

HeaderDesc::~HeaderDesc()
{
	while (Attributes.IsEmpty() == FALSE)
		delete Attributes.RemoveHead();

	delete (m_TLMime);
}

void HeaderDesc::Init()
{
	state = ExpectHeaderName;
	hFound = hLimit;
	*contentEnco = *receivedDate = *messageID = *status = *subj = *who = 0;
	// Initialize the type and subtype, version
	strcpy(contentType, LPCTSTR(CRString(IDS_MIME_TEXT)));
	strcpy(contentSubType, LPCTSTR(CRString(IDS_MIME_PLAIN)));
	strcpy(mimeVersion, LPCTSTR(CRString(IDS_MIME_VERSION)));


	if(!m_bIgnoreDisc)
	{
	g_pPOPInboxFile->Tell(&lDiskStart);
	ASSERT(lDiskStart >= 0);
	lDiskEnd = lDiskStart;
	}

	grokked = TRUE;
	isMIME = FALSE;
	isMHTML = FALSE;
	hasRich = FALSE;
	isPartial = FALSE;
	isDispNotifyTo = FALSE;

	contentID[0] = '\0';
	contentLocation[0] = '\0';
	contentBase[0] = '\0';

	// Initialize Plain
	m_TLMime->CreateMimeType();
}

int HeaderDesc::Read(LineReader* pLR, bool bWriteHeader /*= true*/)
{
	const int IntHeaderSize = 32;
	char interesting[hLimit][IntHeaderSize];
	short h;
	bool bL822Param = !m_bIgnoreDisc;
	if(bWriteHeader)
		bL822Param = !m_bIgnoreDisc;
	else
		bL822Param = bWriteHeader;

	Lex822State l822s(pLR,bL822Param);
	Token822Enum tokenType;
	long lLastNewline = 0;
	long lastParam;

#define EXPECT_TEXT l822s.State = CollectText; state = ExpectText;

	// Grab the interesting header labels
	strcpy(interesting[hContentType], CRString(IDS_MIME_HEADER_CONTENT_TYPE));
	strcpy(interesting[hContentEncoding], CRString(IDS_MIME_HEADER_CONTENT_ENCODING));
	strcpy(interesting[hContentDisposition], CRString(IDS_MIME_HEADER_CONTENT_DISP));
	strcpy(interesting[hContentDescription], CRString(IDS_MIME_CONTENT_DESCRIPTION));
	strcpy(interesting[hContentBase], CRString(IDS_MIME_HEADER_CONTENT_BASE));
	strcpy(interesting[hContentID], CRString(IDS_MIME_HEADER_CONTENT_ID));
	strcpy(interesting[hContentLocation], CRString(IDS_MIME_HEADER_CONTENT_LOCATION));
	strcpy(interesting[hStatus], CRString(IDS_HEADER_STATUS));
	strcpy(interesting[hMimeVersion],CRString(IDS_MIME_HEADER_VERSION));
	strcpy(interesting[hReceived], CRString(IDS_POP_RECEIVED_HEADER));
	strcpy(interesting[hMessageID],CRString(IDS_POP_MESSAGE_ID_HEADER));
	strcpy(interesting[hSubject],CRString(IDS_HEADER_SUBJECT));
	strcpy(interesting[hWho], CRString(IDS_HEADER_FROM));
	strcpy(interesting[hDispNotifyTo], CRString(IDS_MIME_HEADER_READR));

	
    // Remove the colons
	for (h = hContentType; h < hLimit; h++)
    	interesting[h][strlen(interesting[h]) - 1] = 0;
	if(!m_bIgnoreDisc)
	{
		g_pPOPInboxFile->Tell(&lDiskStart);
		ASSERT(lDiskStart >= 0);
		lLastNewline = lDiskEnd = lDiskStart;
	}
	// Read the header
	for (tokenType = l822s.Lex822(); ; tokenType = l822s.Lex822())
	{
		// Write the token to disk
		tokenType = l822s.WriteHeaderToken(tokenType);
		if (tokenType == Comment)
			tokenType = LinearWhite;
		
		// First, we handle a few special token types
		switch (tokenType)
		{
		// any of these things terminate the header collection
		case EndOfHeader:
		case EndOfMessage:
		case ErrorToken:
			goto out;
			break;
			
		// If we have a return, we've hit the end of a header field the header
		// can only legally end in ExpectText or ExpectSem or ExpectVersion
		case Special:
			if (l822s.Token[0] == '\r')
				break;		// Skip first part of newline
			else if (l822s.Token[0] == '\n')
			{
				if (state != ExpectText && state != ExpectSem && state != ExpectVersion)
					grokked = FALSE;
				state = ExpectHeaderName;
				if(!m_bIgnoreDisc)
				{
				g_pPOPInboxFile->Tell(&lLastNewline);
				ASSERT(lLastNewline >= 2);
				lLastNewline -= 2;
				}
				break;
			}
			// Fall through
				
		default:
			// We handle the remaining tokens on a state-by-state basis
			switch (state)
			{
			//********************************
			case ExpectHeaderName:
				switch (tokenType)
				{
				case LinearWhite:
					break;	// Ignore
							
				case Atom:
					for (h = 0; h < hLimit; h++)
						if (!stricmp(interesting[h], (char*)l822s.Token))
							break;
					hFound = (InterestHeaderEnum)h;
					state = ExpectColon;
					if (h > hMimeVersion)
						grokked = FALSE;
					break;
							
				default:
					grokked = FALSE;
					EXPECT_TEXT;
					break;
				}
				break;
						
			//********************************
			case ExpectColon:
				switch (tokenType)
				{
				case LinearWhite:
					break; // ignore
							
				case Special:
					if (l822s.Token[0] == ':')
					{
						switch (hFound)
						{
						case hContentType:
							state = ExpectType;
							break;
						case hContentEncoding:
							state = ExpectEnco;
							break;
						case hContentDisposition:
							strncpy(attributeName,
								CRString(IDS_MIME_HEADER_CONTENT_DISP),
								sizeof(attributeName) - 1);

							state = ExpectValue;
							break;
						case hMimeVersion:
							state = ExpectVersion;
							break;
						default:
							if (hFound > hMimeVersion)
								grokked = FALSE;
							EXPECT_TEXT;
							break;
						}
					}
					break;
							
				default:
					grokked = FALSE;
					EXPECT_TEXT;
					break;
				}
				break;
			//********************************
			case ExpectText:
				switch (hFound)
				{
				case hStatus:
					strncpy(status, ::TrimWhitespaceMT((char*)l822s.Token), sizeof(status) - 1);
					status[sizeof(status) - 1] = 0;
					break;
				case hReceived:
					{
						char* comma = (char *) strchr((const char*)l822s.Token, ',');
						if (comma && !*receivedDate)
						{
							strncpy(receivedDate, ::TrimWhitespaceMT(comma + 1), sizeof(receivedDate) - 1);
							receivedDate[sizeof(receivedDate) - 1] = 0;
						}
					}
					break;
				case hMessageID:
					strncpy(messageID, ::TrimWhitespaceMT((char*)l822s.Token), sizeof(messageID) - 1);
					messageID[sizeof(messageID) - 1] = 0;
					break;
				case hSubject:
					strncpy(subj, ::TrimWhitespaceMT((char*)l822s.Token), sizeof(subj) - 1);
					subj[sizeof(subj) - 1] = 0;
					break;
				case hWho:
					strncpy(who, GetRealName((char*)l822s.Token), sizeof(who) - 1);
					who[sizeof(who) - 1] = 0;
					break;
				case hDispNotifyTo:
					isDispNotifyTo = TRUE;
					break;
				case hContentBase:
					strncpy(contentBase, ::TrimWhitespaceMT((char*)l822s.Token), sizeof(contentBase) - 1);
					contentBase[sizeof(contentBase) - 1] = 0;
					break;
				case hContentID:
					strncpy(contentID, ::TrimWhitespaceMT((char*)l822s.Token), sizeof(contentID) - 1);
					contentID[sizeof(contentID) - 1] = 0;
					break;
				case hContentLocation:
					strncpy(contentLocation, ::TrimWhitespaceMT((char*)l822s.Token), sizeof(contentLocation) - 1);
					contentLocation[sizeof(contentLocation) - 1] = 0;
					break;
				}
				break;

			//********************************
			case ExpectEnco:
				switch (tokenType)
				{
				case LinearWhite:
					break;	// Ignore
				default:
					strncpy(contentEnco, ::TrimWhitespaceMT((char*)l822s.Token),
						sizeof(contentEnco) - 1);
					contentEnco[sizeof(contentEnco) - 1] = 0;
					// If we know the encoding, get rid of it
					// that is, if it's safe to do so.
					if(!m_bIgnoreDisc)
					{
					if (g_bAttachmentDecodeEnabled && FindMIMEDecoder(contentEnco, FALSE))
						 g_pPOPInboxFile->Seek(lLastNewline);
					}
					EXPECT_TEXT;
					break;
				}
				break;

			//********************************
			case ExpectVersion:
				switch (tokenType)
				{
				case LinearWhite:
					break;	// Ignore
				default:
					isMIME = CompareRStringI(IDS_MIME_VERSION, (char*)l822s.Token, 0) == 0;
					EXPECT_TEXT;
					break;
				}
				break;
		
			//********************************
			case ExpectType:
				if (tokenType == LinearWhite)
					break;	// skip this
				if (tokenType == Atom || tokenType == QText)
				{
					strncpy(contentType, ::TrimWhitespaceMT((char*)l822s.Token),
						sizeof(contentType) - 1);
					contentType[sizeof(contentType) - 1] = 0;
					
					m_TLMime->ChangeInfo(NULL,contentType,NULL);

					state = ExpectSlash;
				}
				else
				{
					grokked = FALSE;
					EXPECT_TEXT;
				}
				break;
							
			//********************************
			case ExpectSlash:
				if (tokenType == LinearWhite)
					break;	// Ignore
				if (tokenType == Special && l822s.Token[0] == '/')
				{
					state = ExpectSubType;
					break;
				}
				else
				{
					grokked = FALSE;
					EXPECT_TEXT;
				}
				break;
						
			//********************************
			case ExpectSubType:
				if (tokenType == LinearWhite)
					break;	// Skip this
				if (tokenType == Atom || tokenType == QText)
				{
					strncpy(contentSubType, ::TrimWhitespaceMT((char*)l822s.Token),
						sizeof(contentSubType) - 1);
					contentSubType[sizeof(contentSubType) - 1] = 0;

					m_TLMime->ChangeInfo(NULL,NULL,contentSubType);

					state = ExpectSem;
					if (!CompareRStringI(IDS_MIME_PARTIAL, contentSubType))
						isPartial = TRUE;
					else
						isPartial = FALSE;
					if (!CompareRStringI(IDS_MIME_RICHTEXT, contentSubType) ||
						!CompareRStringI(IDS_MIME_ENRICHED, contentSubType) )
						hasRich = TRUE;  //hasRich = g_AnyRich = TRUE;
					if (!CompareRStringI(IDS_MIME_HTML, contentSubType) )
						isMHTML = TRUE;  //isMHTML = g_AnyHTML = TRUE;
					if(!m_bIgnoreDisc)
					{
					g_pPOPInboxFile->Tell(&lastParam);
					ASSERT(lastParam >= 0);
					}
				}
				else
				{
					grokked = FALSE;
					EXPECT_TEXT;
				}
				break;
					
			//********************************
			case ExpectSem:
				if (tokenType == LinearWhite)
					break;
				if (tokenType == Special && l822s.Token[0] == ';')
				{
					state = ExpectAttribute;
					break;
				}
				else
				{
					grokked = FALSE;
					EXPECT_TEXT;
				}
				break;
						
			//********************************
			case ExpectAttribute:
				if (tokenType == LinearWhite)
					break;	// Skip this
				if (tokenType == Atom)
				{
					strncpy(attributeName, ::TrimWhitespaceMT((char*)l822s.Token),
						sizeof(attributeName) - 1);
					attributeName[sizeof(attributeName) - 1] = 0;
					state = ExpectEqual;
				}
				else
				{
					grokked = FALSE;
					EXPECT_TEXT;
				}
				break;
		
			//********************************
			case ExpectEqual:
				if (tokenType == LinearWhite)
					break;
				if (tokenType == Special && l822s.Token[0] == '=')
				{
					state = ExpectValue;
					break;
				}
				else
				{
					grokked = FALSE;
					EXPECT_TEXT;
				}
				break;
						
			//********************************
			case ExpectValue:
				if (tokenType == LinearWhite)
					break;	// skip this
				if (tokenType == Atom || tokenType == QText)
				{
					if (!AddAttribute((char *)l822s.Token))
					{
						tokenType = ErrorToken;
						goto out;
					}
					state = ExpectSem;
					if(!m_bIgnoreDisc)
						{
						g_pPOPInboxFile->Tell(&lastParam);
						ASSERT(lastParam >= 0);
						}
				}
				else
				{
					grokked = FALSE;
					EXPECT_TEXT;
				}
				break;
			}
		}
	}
	
out:
	// did we finish cleanly?
	if (state != ExpectHeaderName && state != ExpectSem && state != ExpectText)
		grokked = FALSE;
	if(!m_bIgnoreDisc)
	{
	// where we ended up
	g_pPOPInboxFile->Tell(&lDiskEnd);
	ASSERT(lDiskEnd >= 0);
	}
	return (tokenType);
}

int HeaderDesc::AddAttribute(const char* Value)
{

	AttributeElement* AE = DEBUG_NEW_MFCOBJ_NOTHROW AttributeElement(attributeName, Value);
	if (!AE)
		return (FALSE);
 
	
	// This is a hack, we really should be parsing out the disposition into its own field
	if (!CompareRStringI(IDS_MIME_HEADER_CONTENT_DISP, attributeName))
		m_TLMime->ChangeInfo(NULL, NULL, NULL, Value);

	// This is a hack, we really should be parsing out the disposition into its own field
	if (!CompareRStringI(IDS_MIME_CONTENT_DISP_FILENAME, attributeName))
		m_TLMime->AddContentParam(attributeName, Value);

	else
		m_TLMime->AddParam(attributeName, Value);


	if (Attributes.AddTail(AE))
		return (TRUE);
	else
		return (FALSE);
}

AttributeElement* HeaderDesc::GetAttribute(const char* Name)
{
	if (!Name || !*Name)
	{
		ASSERT(FALSE);
		return (NULL);
	}

	POSITION pos = Attributes.GetHeadPosition();
	while (pos)
	{
		AttributeElement* AE = (AttributeElement*)Attributes.GetNext(pos);
		if (AE->m_Name.CompareNoCase(Name) == 0)
			return (AE);
	}

	return (NULL);
}

