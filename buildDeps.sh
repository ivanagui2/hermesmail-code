#!/bin/sh

# HERMES MESSENGER SOFTWARE LICENSE AGREEMENT | Hermes Messenger Client Source 
# Code
# Copyright (c) 2018, Hermes Messenger Development Team. All rights reserved.
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted (subject to the limitations in the disclaimer 
# below) provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, 
# this list of conditions and the following disclaimer.
# Redistributions in binary form must reproduce the above copyright notice, this 
# list of conditions and the following disclaimer in the documentation and/or 
# other materials provided with the distribution.
#
# Neither the name of Hermes Messenger nor the names of its contributors
# may be used to endorse or promote products derived from this software without 
# specific prior written permission.
#
# NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY’S PATENT RIGHTS ARE GRANTED BY 
# THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
# CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Author - Jeff Prickett <kg4ygs@gmail.com>

#Variables
buildDepDir="dependencies"

# Open Source Dependencies

echo "OPEN SOURCE DEPENDENCIES"

expatName="expat"
expatVersion="2.2.5"
expatUrl="https://github.com/libexpat/libexpat/releases/download/R_2_2_5/expat-2.2.5.tar.bz2"
expatSuffix="tar.bz2"
expatFilename="$expatName-$expatVersion.$expatSuffix"
expatDir="$expatName-$expatVersion"
echo $expatName

libPngName=libpng
libPngVersion="1.6.34"
libPngUrl="ftp://ftp-osl.osuosl.org/pub/libpng/src/libpng16/libpng-1.6.34.tar.gz"
libPngSuffix="tar.gz"
libPngFilename="$libPngName-$libPngVersion.$libPngSuffix"
libPngDir=$libPngName-$libPngVersion
echo $libPngName

opensslName=openssl
opensslVersion=1.1.1-pre8
opensslUrl="https://www.openssl.org/source/openssl-1.1.1-pre8.tar.gz"
opensslSuffix="tar.gz"
opensslFilename="$opensslName-$opensslVersion.$opensslSuffix"
opensslDir="$opensslName-$opensslVersion"
echo $opensslName

thunderbirdName=thunderbird
thunderbirdVersion=52.8.0
thunderbirdUrl="https://archive.mozilla.org/pub/thunderbird/releases/52.8.0/source/thunderbird-52.8.0.source.tar.xz"
thunderbirdSuffix="tar.xz"
thunderbirdFilename=$thunderbirdName-$thunderbirdVersion.$thunderbirdSuffix
thunderbirdDir=$thunderbirdName-$thunderbirdVersion
echo $thunderbirdName

xzName=xz
xzVersion="5.2.4"
xzUrl="https://tukaani.org/xz/xz-5.2.4.tar.bz2"
xzSuffix="tar.bz2"
xzFilename=$xzName-$xzVersion.$xzSuffix
xzDir=$xzName-$xzVersion
echo $xzName

