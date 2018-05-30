// LDAP Directory Services 
//
// File:     export.h
// Author:   Mark K. Joseph, Ph.D.
// Version:  1.0
// Copyright 08/26/1997 Qualcomm Inc.  All Rights Reserved.
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
// Description:
// Defines the COM Object and interfaces to support IDSPQuery directory services.
//
//
#ifndef _EXPORT_H_
#define _EXPORT_H_


// -> supported flag values
#define EXPORT_AS_SIMPLE_TABLE  0x01

// -> convert an IDSPRecord object into several different presentation formats
//
HRESULT ExportToHTML(  IDSPRecord* pSearchHit, LPSTR pszFileName, IDirConfig* ConfigIO, BOOL OverWrite, DWORD flags );
HRESULT ExportToText(  IDSPRecord* pSearchHit, LPSTR pszFileName, IDirConfig* ConfigIO, BOOL OverWrite );
HRESULT ExportToVCard( IDSPRecord* pSearchHit, LPSTR pszFileName, IDirConfig* ConfigIO, BOOL OverWrite );


#endif


