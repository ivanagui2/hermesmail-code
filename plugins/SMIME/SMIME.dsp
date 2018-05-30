# Microsoft Developer Studio Project File - Name="SMIME" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SMIME - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SMIME.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SMIME.mak" CFG="SMIME - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SMIME - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SMIME - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/main/plugins/SMIME", BXKAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SMIME - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /base:"0x21500000" /subsystem:windows /dll /debug /machine:I386
# Begin Special Build Tool
OutDir=.\Release
SOURCE="$(InputPath)"
PostBuild_Desc=Copy DLL to Eudora Plugins directory
PostBuild_Cmds=copy  /v  $(OutDir)\SMIME.dll  C:\Eudora\Plugins\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SMIME - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# Begin Special Build Tool
OutDir=.\Debug
SOURCE="$(InputPath)"
PostBuild_Desc=Copy DLL to Eudora Plugins directory
PostBuild_Cmds=copy  /v  $(OutDir)\SMIME.dll  C:\Eudora\Plugins\ 
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "SMIME - Win32 Release"
# Name "SMIME - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\BUFTYPE.CPP
# End Source File
# Begin Source File

SOURCE=.\encoding.cpp
# End Source File
# Begin Source File

SOURCE=.\file1847.cpp
# End Source File
# Begin Source File

SOURCE=.\mimetype.cpp
# End Source File
# Begin Source File

SOURCE=.\RFC1847.CPP
# End Source File
# Begin Source File

SOURCE=.\RFC822.cpp
# End Source File
# Begin Source File

SOURCE=.\SetngDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SignTran.cpp
# End Source File
# Begin Source File

SOURCE=.\SMIME.cpp
# End Source File
# Begin Source File

SOURCE=.\SMIME.def
# End Source File
# Begin Source File

SOURCE=.\SMIME.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\BUFTYPE.H
# End Source File
# Begin Source File

SOURCE=.\CAPICOM.tlh
# End Source File
# Begin Source File

SOURCE=.\CAPICOM.tli
# End Source File
# Begin Source File

SOURCE=".\ems-win.h"
# End Source File
# Begin Source File

SOURCE=.\encoding.h
# End Source File
# Begin Source File

SOURCE=.\file1847.h
# End Source File
# Begin Source File

SOURCE=.\MIMETYPE.H
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\RFC1847.H
# End Source File
# Begin Source File

SOURCE=.\rfc822.h
# End Source File
# Begin Source File

SOURCE=.\SetngDlg.h
# End Source File
# Begin Source File

SOURCE=.\SMIME.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Plugin.ico
# End Source File
# Begin Source File

SOURCE=.\Res\sign.ico
# End Source File
# End Group
# End Target
# End Project
