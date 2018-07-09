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

*/

#ifndef _QCGraphics_h_
#define _QCGraphics_h_

#include "SecImage.h"
#include "SecJpeg.h"

typedef struct structQCMetaFileInfo
{
	HMETAFILE hm;
	long width;
	long height;
} QCMetaFileInfo;

class CBitmap;
bool CreateJpegFromIcon(HICON hIcon, LPCSTR filename, COLORREF BackColor);
bool CreateJpegFromBitmap( CBitmap* pBitmap, LPCSTR filename );
HRMImage* LoadImage(LPCSTR pURL, int TransparencySysColor = COLOR_3DFACE);
bool MetafileFromImage(LPCSTR pURL, QCMetaFileInfo* pMFI, int TransparencySysColor = COLOR_3DFACE);
bool IsEudoraPaletteCurrent(void);
bool BlitToDevice(HRMImage* pImage, CDC* pCDC);

typedef enum
{
	IF_CANT_HANDLE = 0,
	IF_BMP,
	IF_JPEG,
	IF_PNG,
	IF_GIF
} ImageFormatType;

bool IsValidImageFileQuickCheck(const char * in_szFullPath);
ImageFormatType CanHandleImageInternally(LPCTSTR FileName, BOOL bDisplayFileErrors = TRUE);


class QCJpeg : public HRMJpeg
{
public:
	BOOL DoSaveImage(CFile* pFile);
};

class QCPng : public HRMDib
{
public:
	QCPng(int in_nSystemTransparencySysColor = COLOR_3DFACE);
	QCPng(COLORREF in_crTransparent);
	virtual BOOL LoadImage(CFile* pFile);
	virtual BOOL LoadImage(LPCTSTR lpszFileName);
};

class QCImage : public HRMDib
{
public:
	QCImage(int in_nSystemTransparencySysColor = COLOR_3DFACE);
	QCImage(COLORREF in_crTransparent);
	virtual BOOL LoadImage(CFile* pFile);
	virtual BOOL LoadImage(LPCTSTR lpszFileName);
};

#endif // _QCGraphics_h_

