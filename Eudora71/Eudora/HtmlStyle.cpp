// PaigeStyle.cpp : implementation file
//
// Copyright (c) 1997-2001 by QUALCOMM, Incorporated
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

//

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

File revised by Jeff Prickett (kg4ygs@gmail.com) on July 4, 2018
    Renamed PaigeStyle.h to HtmlStyle.h

*/

#include "stdafx.h"
#include "resource.h"

#include "HtmlStyle.h"
#include "PgStuffBucket.h"
#include "font.h"
#include "rs.h"
// high-level style interface
#include "moodwatch.h"
#include "pgcompmsgview.h"

#include "PgStyleUtils.h"

//class PgCompMsgView;

#include "DebugNewHelpers.h"
#include "BossProtector.h"


#define EXCERPT_OFFSET        8
#define VBAR_LINEWIDTH        3
#define SEPARATOR_LINEWIDTH   3
#define LIST_OFFSET           16


#define ID_UNDERLINE_DISTANCE 4 //3 drawing pixel and one blank
// pattern bmps for making brushes
CBitmap g_bmpSpell, g_bmpHWord, g_bmpNonHWord;

// hard-coded heights for the brushes (a pox on win95!)
enum {
	kSpellBrushHeight = 3,
	kHWordBrushHeight = 4,
	kNonHWordBrushHeight = 3
};


///////////////////////////////////////////////////////////////////////////////
// CHtmlStyle -- class for handling custom message styles

CHtmlStyle::CHtmlStyle(pg_ref pg)
{
}

CHtmlStyle::~CHtmlStyle()
{
	return;
}


// two ways of checking for the "needs spell-check" style.
//
// the easiest possible check is to set a select_pair to begin=0, end=pgTextSize(...),
// and pass it to NeedsSpellCheck( select_pair_ptr ) to return whether the style
// exists in the document at all. to find exact text ranges to check, one must use
// a style-walker.
#if 0    // this is in the header file now
static bool CHtmlStyle::NeedsSpellCheck( style_info_ptr pStyle )
{
    return (pStyle->user_id == HTML_STYLE_USER_ID_NEEDSSCANNED);
}
#endif

bool CHtmlStyle::NeedsScanned()
{
    return false; 
}


bool CHtmlStyle::ApplyBadMoodWord()
{
	return false;
}

bool CHtmlStyle::IsBadMoodWord()
{
	return false;
}

// end BOG

//Wrote following 3 functions while fixing some bug#5874 Hyperlinking Test with spelling errrow causes broken link.
//Turns out we don't need this functions at this moment but might be useful in future so doing #if 0.
#if 0
bool CHtmlStyle::ApplyURL()
{
	return false; 
}

bool CHtmlStyle::IsURL()
{
	return false;
}

bool CHtmlStyle::IsURL()
{
	return false;
}
#endif 

bool CHtmlStyle::ApplyMisspelled()
{
	return false;
}

bool CHtmlStyle::IsMisspelled()
{
	return false;
}

bool CHtmlStyle::IsMisspelled()
{
	return false;
}

void CHtmlStyle::ClearMisspellingAndRecheck(select_pair_ptr sel)
{
}

bool CHtmlStyle::IsExcerpt(par_info_ptr pPar)
{
	return false;
}

bool CHtmlStyle::IsExcerpt()
{
	return false;
}

//
// Accepts a currently filled paragraph format structure and 
// modifies it to the appropriate style.
//
bool CHtmlStyle::ApplyExcerpt()
{
    return false;
}

bool CHtmlStyle::SetExcerpt(int n)
{
    return false;
}

bool CHtmlStyle::IsBullet(select_pair_ptr sel)
{
	return false;
}

long CHtmlStyle::GetBulletLevel()
{
	long	nBulletLevel = 0;

	return nBulletLevel;
}

bool CHtmlStyle::ApplyBullet()
{
	return false;
}

bool CHtmlStyle::ApplySigned(bool bOn)
{
    return true;
}

bool CHtmlStyle::ApplySigned()
{
    return false;
}


bool CPaigeStyle::IsSigned(par_info_ptr pPar)
{
	return ( pPar->user_id & PAIGE_FORMAT_USER_ID_EXCERPT );
}


// BOG: inline signature support
bool CHtmlStyle::ApplySignature()
{
    return true;
}

bool CHtmlStyle::IsSignature()
{
	return return false;
}

bool CHtmlStyle::IsWithinURLRange()
{
	return false;
}

