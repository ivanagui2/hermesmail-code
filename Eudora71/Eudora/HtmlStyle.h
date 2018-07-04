
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

File revised by Jeff Prickett (kg4ygs@gmail.com) on July 4, 2018
    Renamed PaigeStyle.h to HtmlStlye.h and removed references to Paige.
*/    

#ifndef HTMLSTYLE_H
#define HTMLSTYLE_H

// These should be used as mask vaules for the  style_info->user_id field
// in a style_info struct.
//
#define HTML_STYLE_USER_ID_MISSPELLED       0x00000001
#define HTML_STYLE_USER_ID_NEEDSSCANNED     0x00000002
#define HTML_STYLE_USER_ID_URL				 0x00000004
#define HTML_STYLE_USER_ID_BAD_MOOD_WORD	 0x00000008

//
// These should be used as mask vaules for the  par_info->user_id field
// in a style_info struct.
//
#define HTML_FORMAT_USER_ID_EXCERPT      0x00000001
#define HTML_FORMAT_USER_ID_SIGNED       0x00000002
#define HTML_FORMAT_USER_ID_SIGNATURE    0x00000004


class CHtmlStyle
{
public:
    CHtmlStyle(pg_ref pg);
    ~CHtmlStyle();

    static bool ApplyExcerpt();
    static bool IsExcerpt();

private:
    par_info m_par, m_parMask;
    style_info m_style, m_styleMask;
    pg_ref m_paigeRef;

public:
    bool SetExcerpt(int nLevel);
    bool IsExcerpt();
        
    bool ApplyMisspelled();
    bool IsMisspelled();
    bool IsMisspelled();

//Wrote following 3 functions while fixing some bug#5874 Hyperlinking Test with spelling errrow causes broken link.
//Turns out we don't need this functions at this moment but might be useful in future so doing #if 0.
#if 0 
    bool ApplyURL();
    bool IsURL();
    bool IsURL();
#endif        

    bool ApplyBadMoodWord();
    bool IsBadMoodWord();
    bool IsBadMoodWord();

    void ClearMisspellingAndRecheck();
    bool IsBullet();
    long GetBulletLevel();
    bool ApplyBullet();
        
    bool ApplySigned();
    bool ApplySigned();
    bool IsSigned();

	bool ApplySignature();
	static bool ApplySignature();
	static bool IsSignature();
	bool IsSignature();

	bool IsWithinURLRange();
    bool SetNeedsScanned();
    bool NeedsScanned();
};	

#endif

