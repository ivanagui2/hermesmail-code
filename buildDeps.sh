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
        export ${name}Url=$protocol://$server/$dlpath/$name-$version.tar.$suffix
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
        curl -L -o $filename "$url"
    else
        echo "$name already downloaded."
        echo
    fi
}

#Variables

hermesHome=`pwd`

echo "Home Directory = $hermesHome"

buildDepDir="$hermesHome/dependencies"

# Open Source Dependencies

echo
echo "OPEN SOURCE DEPENDENCIES"

defineDependency libexpat R_2_2_5 gz github.com libexpat/libexpat/archive $buildDepDir https

defineDependency libpng 1.6.34 gz ftp-osl.osuosl.org pub/libpng/src/libpng16 $buildDepDir ftp
echo "$libpngName - Used in Hermes Messenger"

defineDependency openssl 1.1.1-pre8 gz www.openssl.org source $buildDepDir https
echo "$opensslName - Used in Hermes Messenger"

defineDependency thunderbird 52.8.0 xz archive.mozilla.org thunderbird/releases/52.8.0/source $buildDepDir https 
echo "$thunderbirdName - Used in Hermes Messenger"

defineDependency xz 5.2.4 xz tukaani.org xz $buildDepDir https
echo "$xzName - Needed to support building the dependencies"

echo

# Download dependencies
mkdir -p ./$buildDepDir
cd ./$buildDepDir
echo "DOWNLOADING"

echo

downloadPackage $libexpatName $libexpatFilename $libexpatUrl
echo

downloadPackage $libpngName $libpngFilename $libpngUrl
echo

downloadPackage $opensslName $opensslFilename $opensslUrl
echo

downloadPackage $thunderbirdName $thunderbirdFilename $thunderbirdUrl
echo

downloadPackage $xzName $xzFilename $xzUrl
echo

# COMPILE DEPENDENCIES
echo "Compile dependencies..."

