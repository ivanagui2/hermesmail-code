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

defineDependency() {
    local name=$1
    local version=$2
    local suffix=$3
    local server=$4
    local dlpath=$5
    local builddir=$6
    local protocol=$7

    echo $name
    echo $name
    export ${name}Name=$name
    export ${name}Version=$version
    export ${name}Dir=$name-$version
    export ${name}BuildDir=$builddir/$name-$version
    export ${name}Tarball=$name-$version.tar
    export ${name}Suffix=$suffix
    export ${name}Filename=$name-$version.tar.$suffix
    export ${name}Server=$server
    export ${name}Path=$dlpath
    if ! [ $name = libexpat ]; then
        if ![ $name = googletest ]; then
            export ${name}Url=$protocol://$server/$dlpath/$name-$version.tar.$suffix
        else
            export ${name}Url=http://$server/$dlpath/release-$version.tar.gz
        fi
    else
        export ${name}Url=$protocol://$server/$dlpath/$version.tar.$suffix
    fi
}

downloadPackage() {
    local name=$1
    local filename=$2
    local url=$3

    echo $name
    echo $filename
    echo $url
    
    echo "Downloading $name"
    if ! [ -f $filename ]; then
        echo "\"$url\""
        curl -L -o $filename "$url"
    else
        echo "$name already downloaded."
        echo
    fi
}

#Variables

hermesHome=.

echo "Home Directory = $hermesHome"

buildDepDir="./opensourcelibs"

# Open Source Dependencies
echo
echo "OPEN SOURCE DEPENDENCIES"

defineDependency firefox  61.0.source xz archive.mozilla.org pub/firefox/releases/61.0/source $buildDepDir https 
echo "$firefoxName"

defineDependency googletest 1.8.0 gz github.com google/googletest/archive $buildDepsDir https 
echo "$googletestName"

defineDependency libexpat R_2_2_5 gz github.com libexpat/libexpat/archive $buildDepDir https
echo "$libexpatName"

defineDependency libpng 1.6.34 gz ftp-osl.osuosl.org pub/libpng/src/libpng16 $buildDepDir ftp
echo "$libpngName"

defineDependency mailcore2 0.6.3 gz github.com MailCore/mailcore2/archive $buildDepDir https
echo

defineDependency openssl 1.1.1-pre8 gz www.openssl.org source $buildDepDir https
echo "$opensslName" 

# defineDependency zlib 1.2.11 gz zlib.net "" $buildDepDir http 
# echo "$zlib"

if ! [ -f $filename ]; then
    echo "http://zlib.net/zlib-1.2.11.tar.gz"
    curl -L -o zlib-1.2.11.tar.gz "http://zlib.net/zlib-1.2.11.tar.gz"
else
    echo "$name already downloaded."
    echo
fi


# Download dependencies
mkdir -p ./$buildDepDir
cd ./$buildDepDir
pwd
echo "DOWNLOADING"

echo

downloadPackage $firefoxName $firefoxFilename $firefoxUrl
echo

downloadPackage $googletestName $googletestFilename $googletestUrl
echo

downloadPackage $libexpatName $libexpatFilename $libexpatUrl
echo

downloadPackage $libpngName $libpngFilename $libpngUrl
echo

downloadPackage $mailcore2Name $mailcore2Filename $mailcore2Url
echo

downloadPackage $opensslName $opensslFilename $opensslUrl
echo

