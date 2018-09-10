// FONT.CPP
//
// Routines for dealing with fonts, especially the user-defined Screen Font
//


#include "stdafx.h"

#include "resource.h"
#include "rs.h"
#include "font.h"



#include "DebugNewHelpers.h"


int theFontSizeMap[FONTSIZE_COUNT] = { 9, 10, 12, 14, 16 };

CFontInfo ScreenFont;
CFontInfo ScreenFontBold(FW_BOLD);
CFontInfo ScreenFontItalic(FW_NORMAL, TRUE);
CFontInfo MessageFont;
CFontInfo MessageFixedFont;
CFontInfo PrinterFont;
CFontInfo PrinterFontBold(FW_BOLD);

static ENUMLOGFONT SelectedFont;

enum
{
	EF_ALL_FONTS,
	EF_ONE_FONT
};

static FONT_TYPE_ENUM s_FontTypeEnum = DONT_CARE;

// EnumAllFonts
// Windows CALLback function for finding info about available fonts
//
int CALLBACK EnumAllFonts(const ENUMLOGFONT *lf, const NEWTEXTMETRIC *tm, int FontType, LPARAM WhichFunction)
{
	if (!lf)
		return (0);

	if (WhichFunction == EF_ONE_FONT)
	{
		SelectedFont = *lf;
		return (0);
	}

	return (1);
}


BOOL CFontInfo::SetTextMetrics()
{
	return (GetTextMetrics(this, m_tm));
}


static const TCHAR DisplayText[] = "DISPLAY";

BOOL GetTextMetrics(HFONT hFont, TEXTMETRIC& tm)
{
 	CDC dc;
 	
	VERIFY(dc.CreateIC(DisplayText, NULL, NULL, NULL));
	VERIFY(::SelectObject(dc.m_hDC, hFont));
	
	return (dc.GetTextMetrics(&tm));
}

int TextWidth(CFont* Font, LPCTSTR Text)
{
 	if (!Text)
 		return (0);
 		
 	CDC dc;
 	CRect rect;
	rect.SetRectEmpty();
 	
	VERIFY(dc.CreateIC(DisplayText, NULL, NULL, NULL));
	VERIFY(dc.SelectObject(Font));
	dc.DrawText(Text, strlen(Text), &rect, DT_CALCRECT | DT_SINGLELINE | DT_NOPREFIX);
#ifdef unix
	// DrawText is not returning the Rect under windu2
	if (rect.Width() == 0) return strlen(Text)*((CFontInfo *)Font)->CharWidth();	
#endif
	
	return (rect.Width());
}

CFontInfo::CFontInfo(int Weight /*= FW_NORMAL*/, BOOL Italic /*= FALSE*/)
{
	m_Weight = Weight;
	m_Italic = Italic;
}

int CFontInfo::TextWidth(LPCTSTR Text)
{
	return (::TextWidth(this, Text));
}

//Can create a font for display or for some other device.  Defaults to the diplay if no other device specified.
//Uses the values stored in the ini file to determine the font size.
BOOL CFontInfo::CreateFontFromMapping(LPCTSTR Name, int nUnMappedSize, bool bDeleteOldFont, CDC* pDC /*= NULL*/)
{
	if ( nUnMappedSize < 0 || nUnMappedSize >= FONTSIZE_COUNT )
		nUnMappedSize = 1;

	int FontSize = theFontSizeMap[ nUnMappedSize ];

	return CreateFont(Name, FontSize, bDeleteOldFont, pDC);
}

BOOL CFontInfo::CreateFont(LPCTSTR Name, int FontSize, bool bDeleteOldFont, CDC* pDC /*= NULL*/)
{
	BOOL bStatus = FALSE;
	BOOL bGotDC = FALSE;
	HFONT OldFont = (HFONT) Detach();

	if ( FontSize <= 0 ) FontSize = 10;

	if ( !pDC ) {
		pDC = CDC::FromHandle( ::GetDC(NULL) );
		bGotDC = TRUE;
	}

	int YPerInch = pDC->GetDeviceCaps( LOGPIXELSY );

	LOGFONT lf;
	memset( &lf, '\0', sizeof(LOGFONT) );
	strcpy( lf.lfFaceName, Name );
	lf.lfHeight = -((FontSize * YPerInch) / 72);
	lf.lfWeight = m_Weight;
	lf.lfItalic = (BYTE) m_Italic;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfOutPrecision = OUT_TT_PRECIS;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfCharSet = DEFAULT_CHARSET;

	if ( CreateFontIndirect( &lf ) )
	{
		bStatus = TRUE;

		if ( bDeleteOldFont && (OldFont != NULL) )
		{
			BOOL	bDeleteObjectGood = ::DeleteObject(OldFont);
			ASSERT(bDeleteObjectGood);
		}
	}
	else
	{
		Attach( OldFont ? OldFont : ::GetStockObject(SYSTEM_FIXED_FONT) );
	}

	if ( bGotDC )
		::ReleaseDC( NULL, pDC->m_hDC );

	SetTextMetrics();
	return bStatus;
}

// SetWindowFont
// Resize all window objects of this window that use the config font
//
BOOL SetWindowFont(CWnd* win, HFONT OldFont, CFont* NewFont)
{
	if (!win)
		return (FALSE);

	win = win->GetWindow(GW_CHILD);
	for (; win; win = win->GetWindow(GW_HWNDNEXT))
	{
		CWnd* child = win->GetWindow(GW_CHILD);
		CFont* TheFont = win->GetFont();
		if (child)
			SetWindowFont(win, OldFont, NewFont);
		if (TheFont && TheFont->m_hObject == OldFont)
			win->SetFont(NewFont, TRUE);
	}

	return (TRUE);
}

 
BOOL SetupFonts()
{
	CWnd* MainWnd = AfxGetMainWnd();
	HFONT OldFont;
	BOOL Status;
	CString Name;
	int Size;

	// Screen font
	OldFont = (HFONT)ScreenFont.m_hObject;
	Name = GetIniString(IDS_INI_SCREEN_FONT);
	Size = GetIniShort(IDS_INI_SF_SIZE);

	if ( Size < 0 || Size >= FONTSIZE_COUNT )
		Size = 1;
	
	Size = theFontSizeMap[ Size ];

	if ( Status = ScreenFont.CreateFont(Name, Size, false) )
	{
		Status = ScreenFontBold.CreateFont(Name, Size, true);
		Status = ScreenFontItalic.CreateFont(Name, Size, true);
	}

	if (Status && ScreenFont.m_hObject != OldFont)
	{
		SetWindowFont(MainWnd, OldFont, &ScreenFont);

		// Delete the old Screen font if necessary
		if (OldFont != NULL)
		{
			BOOL	bDeleteObjectGood = ::DeleteObject(OldFont);
			ASSERT(bDeleteObjectGood);
		}
	}

	// Fixed Message font
	OldFont = (HFONT)MessageFixedFont.m_hObject;
	Name = GetIniString(IDS_INI_MESSAGE_FIXED_FONT);
	Size = GetIniShort(IDS_INI_MESSAGE_FONT_SIZE);

	if ( Size < 0 || Size >= FONTSIZE_COUNT )
		Size = 1;
	
	Size = theFontSizeMap[ Size ];
	Status = MessageFixedFont.CreateFont(Name, Size, false);

	if (Status && MessageFixedFont.m_hObject != OldFont)
	{
		SetWindowFont(MainWnd, OldFont, &MessageFixedFont);

		// Delete the old Fixed Message font if necessary
		if (OldFont != NULL)
		{
			BOOL	bDeleteObjectGood = ::DeleteObject(OldFont);
			ASSERT(bDeleteObjectGood);
		}
	}

	// Message font
	OldFont = (HFONT)MessageFont.m_hObject;
	Name = GetIniString(IDS_INI_MESSAGE_FONT);
	Size = GetIniShort(IDS_INI_MESSAGE_FONT_SIZE);

	if ( Size < 0 || Size >= FONTSIZE_COUNT )
		Size = 1;
	
	Size = theFontSizeMap[ Size ];
	Status = MessageFont.CreateFont(Name, Size, false);

	if (Status && MessageFont.m_hObject != OldFont)
	{
		SetWindowFont(MainWnd, OldFont, &MessageFont);

		// Delete the old Message font if necessary
		if (OldFont != NULL)
		{
			BOOL	bDeleteObjectGood = ::DeleteObject(OldFont);
			ASSERT(bDeleteObjectGood);
		}
	}

	
	// Printer font regular and bold
	Name = GetIniString(IDS_INI_PRINTER_FONT);
	Size = GetIniShort(IDS_INI_PF_SIZE);

	if ( Size < 0 || Size >= FONTSIZE_COUNT )
		Size = 1;
	
	Size = theFontSizeMap[ Size ];
	Status = PrinterFont.CreateFont(Name, Size, true);
	Status = PrinterFontBold.CreateFont(Name, Size, true);

	return (Status);
}

CFontInfo& GetMessageFont()
{
	if (GetIniShort(IDS_INI_USE_PROPORTIONAL_AS_DEFAULT))
		return MessageFont;

	return MessageFixedFont;
}

CFontInfo& GetMessageProportionalFont()
{
	return MessageFont;
}

CFontInfo& GetMessageFixedFont()
{
	return MessageFixedFont;
}


BOOL CALLBACK EnumFontFamProc(
LPLOGFONT lplf, 
LPNEWTEXTMETRIC lpntm, 
DWORD FontType, 
LPVOID lParam)   
{
	CStringArray*	pArray;
	CString			szFontName;

	pArray = ( CStringArray* ) lParam;
	szFontName = lplf->lfFaceName;
	switch ( s_FontTypeEnum )
	{
	case FIXED_ONLY :
		if ( ( lpntm->tmPitchAndFamily & 0x1 ) == 0 )
			pArray->Add( szFontName );
		break;

	case PROPORTIONAL_ONLY :
		if ( ( lpntm->tmPitchAndFamily & 0x1 ) == 1 )
			pArray->Add( szFontName );
		break;

	default :
		pArray->Add( szFontName );
	}

	return 1;
}
 

void EnumFontFaces(
CStringArray& theArray,
FONT_TYPE_ENUM enType /* = DONT_CARE */ )
{
	s_FontTypeEnum = enType;

	CDC theDC;

	theDC.CreateCompatibleDC( NULL );

	EnumFontFamilies(	theDC.m_hDC, 
						NULL, 
						( FONTENUMPROC ) EnumFontFamProc, 
 						( LONG ) &theArray );

	s_FontTypeEnum = DONT_CARE; // make sure previous behavior is maintained
}


void UpdateFontSettings()
{
	CString			szName;
	CStringArray	thePropFontArray;
	CStringArray	theFixedFontArray;
	INT				i;

	// verify the message fonts if proportional by default
	// We check it because IDS_INI_SCREEN_FONT is the prop and fixed
	// font in 3.x.  We don't check if fixed is on by default because it's new

	if( GetIniShort( IDS_INI_USE_PROPORTIONAL_AS_DEFAULT ) )
	{
		szName = GetIniString( IDS_INI_SCREEN_FONT );
		EnumFontFaces( thePropFontArray, PROPORTIONAL_ONLY );
		EnumFontFaces( theFixedFontArray, FIXED_ONLY );

		for( i = thePropFontArray.GetSize() - 1; i >= 0; i-- )
		{
			if( szName.CompareNoCase( thePropFontArray[ i ] ) == 0 )
			{
				break;
			}
		}

		if( i >= 0 )
		{
			// set the font name 
			SetIniString( IDS_INI_MESSAGE_FONT, szName );
			// set the printer font name
			SetIniString( IDS_INI_PRINTER_FONT, szName );
		}
		else
		{
			// now see if it's in the fixed width
			for( i = theFixedFontArray.GetSize() - 1; i >= 0; i-- )
			{
				if( szName.CompareNoCase( theFixedFontArray[ i ] ) == 0 )
				{
					break;
				}
			}
			
			if( i >= 0 )
			{
				// it's in the fixed list so turn fixed on by default
				SetIniShort( IDS_INI_USE_PROPORTIONAL_AS_DEFAULT, 0 );
				SetIniString( IDS_INI_MESSAGE_FIXED_FONT, szName );
				SetIniString( IDS_INI_PRINTER_FONT, szName );
			}
		}
	}
}