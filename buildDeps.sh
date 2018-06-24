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

echo
echo "OPEN SOURCE DEPENDENCIES"

expatName="expat"
expatVersion="2.2.5"
expatSuffix="tar.gz"
expatFilename="$expatName-$expatVersion.$expatSuffix"
expatServer=github.com
expatPath=libexpat/libexpat/releases/download/R_2_2_5
expatUrl="https://$expatServer/$expatPath/$expatFilename"
expatDir="$expatName-$expatVersion"
echo "$expatName - Used in Hermes Messenger"

libIconvName=libiconv
libIconvVersion=1.15
libIconvSuffix=tar.gz
libIconvFilename=$libIconvName-$libIconvVersion.$libIconvSuffix
libIconvServer=ftp.gnu.org
libIconvPath=pub/gnu/libiconv
libIconvUrl=ftp://$libIconvServer/$libIconvPath/$libIconvFilename
libIconvDir=$libIconvName-$libIconvVersion
echo "$libIconvName - Needed to support building the dependencies"
echo $libIconvUrl


libPngName=libpng
libPngVersion="1.6.34"
libPngSuffix="tar.gz"
libPngFilename="$libPngName-$libPngVersion.$libPngSuffix"
libPngServer=ftp-osl.osuosl.org
libPngPath=pub/$libPngName/src/${libPngName}16
libPngUrl="ftp://$libPngServer/$libPngPath/$libPngFilename"
libPngDir=$libPngName-$libPngVersion
echo "$libPngName - Used in Hermes Messenger"

opensslName=openssl
opensslVersion=1.1.1-pre8
opensslSuffix="tar.gz"
opensslFilename="$opensslName-$opensslVersion.$opensslSuffix"
opensslServer=www.openssl.org
opensslPath=source
opensslUrl="https://$opensslServer/$opensslPath/$opensslFilename"
opensslDir="$opensslName-$opensslVersion"
echo "$opensslName - Used in Hermes Messenger"

pkgconfigName=pkgconfig
pkgconfigVersion=0.29.2
pkgconfigSuffix=tar.gz
pkgconfigFilename=$pkgconfigName-$pkgconfigVersion.$pkgconfigSuffix
pkgconfigServer=pkg-config.freedesktop.org
pkgconfigUrl=https://$pkgconfigServer/$pkgconfigPath/$pkgconfigFilename
pkgconfigDir=$pkgconfigName-$pkgconfigVersion
echo "$pkgconfigName - Needed to support building the dependencies"

thunderbirdName=thunderbird
thunderbirdVersion=52.8.0
thunderbirdSuffix="tar.xz"
thunderbirdServer=archive.mozilla.org
thunderbirdPath=pub/$thunderbirdName/releases/$thunderbirdVersion/source
thunderbirdFilename=$thunderbirdName-$thunderbirdVersion.$thunderbirdSuffix
thunderbirdUrl="https://$thunderbirdServer/$thunderbirdPath/$thunderbirdFilename"
thunderbirdDir=$thunderbirdName-$thunderbirdVersion
echo "$thunderbirdName - Used in Hermes Messenger"

wgetName=wget
wgetVersion=1.19.5
wgetSuffix=tar.gz
wgetServer=ftp.gnu.org
wgetPath=gnu/wget
wgetFilename=$wgetName-$wgetVersion.$wgetSuffix
wgetUrl=ftp://$wgetServer/$wgetPath/$wgetFilename
wgetDir=$wgetName-$wgetVersion
echo "$wgetName - Needed to support building the dependencies"

xzName=xz
xzVersion="5.2.4"
xzSuffix="tar.bz2"
xzServer=tukaani.org
xzFilename=$xzName-$xzVersion.$xzSuffix
xzUrl="https://$xzServer/$xzName/$xzFilenameName"
xzDir=$xzName-$xzVersion
echo "$xzName - Needed to support building the dependencies"

echo

# Download dependencies
mkdir -p ./$buildDepDir
cd ./$buildDepDir
echo "DOWNLOADING"

echo
echo "Downloading $expatName"
if ! [ -f $expatFilename ]; then
    curl -o -L $expatFilename "$expatUrl"
else
    echo "$expatName already downloaded."
    echo
fi

echo
echo "Downloading $libPngName"
if ! [ -f $libPngFilename ]; then
    curl -o $libPngFilename "$libPngUrl"
else
    echo "$libPngName already downloaded."
    echo
fi

echo
echo "Downloading $libIconvName"
if ! [ -f $libIconvFilename ]; then
    curl -o $libIconvFilename "$libIconvUrl"
else
    echo "$libIconv already downloaded"
fi

echo
echo "Downloading $opensslName"
if ! [ -f $opensslFilename ]; then
    curl -o $opensslFilename "$opensslUrl"
else
    echo "$opensslName already downloaded."
    echo
fi

echo
echo "Downloading $pkgconfigName"
if ! [ -f $pkgconfigFilename ]; then
    curl -o $pkgconfigFilename "$pkgconfigUrl"
else
    echo "$pkgconfigFilename already downloaded"
fi

echo
echo "Downloading $thunderbirdName"
if ! [ -f $thunderbirdFilename ]; then
    curl -o $thunderbirdFilename "$thunderbirdUrl"
else
    echo "$thunderbirdName already downloaded."
    echo
fi

echo
echo "Downloading $wgetName"
if ! [ -f $wgetFilename ]; then
    curl -o $wgetFilename "$wgetUrl"
else
    echo "$wgetName already downloaded."
fi

echo
echo "Downloading $xzName"
if ! [ -f $xzFilename ]; then
    curl -o $xzFilename "$xzUrl"
else
    echo "$xzName already downloaded."
    echo
fi

# COMPILE DEPENDENCIES



