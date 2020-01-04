# Microsoft Developer Studio Project File - Name="RV03" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=RV03 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RV03.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RV03.mak" CFG="RV03 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RV03 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "RV03 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RV03 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../../obj/release"
# PROP BASE Intermediate_Dir "../../obj/release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../obj/release"
# PROP Intermediate_Dir "../../obj/release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../../src/viewer" /I "." /I "../../src/" /I "../../src/math" /I "../../src/basics" /I "../../src/io" /I "../../src/io/png" /I "../../src/io/jpeg" /I "../../src/imgProc" /I "../../src/system" /I "../gtk+" /I "../gtk+/gdk" /I "../glib" /I "../../src/classifiers" /D "_WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /FD /I /usr/local/src/gtk+" /I /usr/local/src/glib" /I /usr/local/src/gtk+/gdk" " " " /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 psapi.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib glib-1.3.lib gdk-1.3.lib gtk-1.3.lib gmodule-1.3.lib gthread-1.3.lib /nologo /subsystem:console /machine:I386 /out:"../../bin/rtester.exe" /libpath:"../lib/" /libpath:"/usr/local/src/gtk+/gtk/" /libpath:"/usr/local/src/gtk+/gdk/" /libpath:"/usr/local/src/glib"

!ELSEIF  "$(CFG)" == "RV03 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../../obj/debug"
# PROP BASE Intermediate_Dir "../../obj/debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug"
# PROP Intermediate_Dir "debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W2 /Gm /GR /GX /ZI /Od /I "c:/LTILib/src/io/jpeg" /I "c:/LTILib/src/viewer" /I "c:/LTILib/src/" /I "c:/LTILib/src/math" /I "c:/LTILib/src/basics" /I "c:/LTILib/src/io" /I "c:/LTILib/src/io/png" /I "c:/LTILib/src/imgProc" /I "c:/LTILib/src/system" /I "c:/LTILib/win/gtk+" /I "c:/LTILib/win/gtk+/gdk" /I "c:/LTILib/win/gtk+/gtk" /I "c:/LTILib/win/glib" /I "c:/LTILib/src/classifiers" /I "c:\LTILib/src/ioBasics" /D "_WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /I /usr/local/src/gtk+" /I /usr/local/src/glib" /I /usr/local/src/gtk+/gdk" /GZ " " " /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"RV03.exe" /pdbtype:sept /libpath:"../lib/" /libpath:"/usr/local/src/gtk+/gtk/" /libpath:"/usr/local/src/gtk+/gdk/" /libpath:"/usr/local/src/glib"

!ENDIF 

# Begin Target

# Name "RV03 - Win32 Release"
# Name "RV03 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\RV03.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\RV03.h
# End Source File
# End Group
# Begin Group "Libraries"

# PROP Default_Filter ".lib"
# Begin Source File

SOURCE=..\..\..\lib\ltilib.lib
# End Source File
# Begin Source File

SOURCE=..\..\..\win\lib\zlib.lib
# End Source File
# Begin Source File

SOURCE="..\..\..\win\lib\gdk-1.3.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\win\lib\glib-1.3.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\win\lib\gmodule-1.3.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\win\lib\gthread-1.3.lib"
# End Source File
# Begin Source File

SOURCE="..\..\..\win\lib\gtk-1.3.lib"
# End Source File
# Begin Source File

SOURCE=..\..\..\win\lib\PSAPI.LIB
# End Source File
# Begin Source File

SOURCE=..\..\..\win\lib\WS2_32.LIB
# End Source File
# End Group
# End Target
# End Project
