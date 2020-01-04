:: YOU HAVE TO CHANGE SOME SETTINGS IN THE CONFIGURATION SECTION BELOW BEFORE
:: THIS SCRIPT WORKS!!!

:: CONFIGURATION SECTION ------------------------------------------------------
@echo off
@cls

:: 1. Comment the next two lines to enable the script--------------------------
::@echo You have to configure the script before it works correctly!
::goto :END

:: 2. Setting Visual Studio .NET environment ----------------------------------
:: change the path to vsvars32.bat to match your machine, it is normally-------
:: located in the common7\tools-directory of the Visual Studio home directory--
call "C:\Program Files\Microsoft Visual Studio 9.0\Common7\Tools\vsvars32.bat"

:: 3. Configure the CVS settings if you like to use the -co (check out) option-
::   a. set path to CVS program for command line execution---------------------
::   b. set home directory where CVS stores your passwords---------------------
::   c. set CVSROOT to the ltilib repository-----------------------------------
set Path=%Path%;"C:\Programme\WinCVS 1.2\"
set HOME=MyDrive:\MyCVSHome
set CVSROOT=:pserver:user@cvsserver:/repositorydirectoy

:: 4. Choose the ltilib configurations you would like to build-----------------
:: setting a configuration to "0" disables the build --------------------------
set MFC_Shared_Debug=0
set MFC_Shared_Release=0
set LTI_Lib_NET_Debug=0
set LTI_Lib_NET_Release=0
set Win32_Debug=1
set Win32_Release=1

:: END CONFIGURATION SECTION --------------------------------------------------

:: START OF SCRIPT ------------------------------------------------------------
set success=1
set scriptName=%0

:: Get commandline options ----------------------------------------------------
set clear=0
set co=0
set nomake=0
set noheaders=0
set help=0
set dir=0

:CLLOOP
  set valid=0
  if "%1"=="" goto :CLEXIT
  if "%1"=="-clear"     set clear=1     & set valid=1
  if "%1"=="-co"        set co=1        & set valid=1
  if "%1"=="-nomake"    set nomake=1    & set valid=1
  if "%1"=="-noheaders" set noheaders=1 & set valid=1
  if "%1"=="-help"      set help=1      & set valid=1
  if "%1"=="-h"         set help=1      & set valid=1
  if NOT "%1"=="-dir" GOTO :NO_DIR
    shift 
    set dir=1
    set dirname=%1
    set valid=1
  :NO_DIR
  
  if "%valid%"=="1" goto :OPTION_OK
    @echo Invalid option: %1
    @echo.
    @echo For help please type: %scriptName% -h
    goto END
  :OPTION_OK  
  
  shift
  goto :CLLOOP
:CLEXIT
:: ----------------------------------------------------------------------------


:: Print Help -----------------------------------------------------------------
if %help%==0 goto :ENDHELP
@echo Builds the ltilib. Creates makefile and collects headerfiles beforehand.
@echo.
@echo Usage: %scriptName% [-clear] [-co] [-nomake] [-noheaders] [-dir "dirname"]
@echo.
@echo   -clear            deletes all existing object files (forces complete rebuild)
@echo   -co               check out lib from repository before compiling
@echo   -nomake           don't create Visual .Net project
@echo   -noheaders        don't collect headerfiles
@echo   -dir "dirname"    copy lib to specified directory, if successful
goto :END
:ENDHELP
:: ----------------------------------------------------------------------------

:: ----------------------------------------------------------------------------
:: Clear existing configurations if the option -clear was specified------------
:: ----------------------------------------------------------------------------
@echo.
@echo.
if %clear%==0 goto :ENDCLEAR
  @echo Clearing existing configurations...
  @echo.
  @echo.
  @echo -- Clear existing configurations ...
  devenv ltilib.sln /clean "MFC Shared Debug"
  devenv ltilib.sln /clean "MFC Shared Release"
  devenv ltilib.sln /clean "LTI-Lib .NET Debug"
  devenv ltilib.sln /clean "LTI-Lib .NET Release"
  devenv ltilib.sln /clean "WIN32 Debug"
  devenv ltilib.sln /clean "WIN32 Release"
:ENDCLEAR

:: ----------------------------------------------------------------------------
:: Check out new sources if -co was specified----------------------------------
:: ----------------------------------------------------------------------------
if %co%==0 goto :ENDCO
  @cd ..\..
  @echo.
  @echo.
  @echo Updating sources from CVS repository ...

  cvs -d %CVSROOT% update -d
  if ERRORLEVEL 1 goto :CHECKOUTFAILED
  goto :CHECKOUTOK
  :CHECKOUTFAILED
  @echo Could not check out new sources.
  :CHECKOUTOK
  @cd win\buildLib_net
:ENDCO

:: ----------------------------------------------------------------------------
:: Create new Visual Studio project if -nomake was NOT specified---------------
:: ----------------------------------------------------------------------------
if %nomake%==1 goto :ENDNOMAKE
  @echo.
  @echo.
  @perl buildLib.net.pl 
:ENDNOMAKE

:: ----------------------------------------------------------------------------
:: Collect header files in \lib\headerFiles if -noheaders was NOT specified----
:: ----------------------------------------------------------------------------
if %noheaders%==1 goto :ENDNOHEADERS
  @echo.
  @echo.
  @perl collectHeaders.pl 
:ENDNOHEADERS

@echo.
@echo.

:: ----------------------------------------------------------------------------
:: Build debug version of ltilib with MFC shared library support
:: ----------------------------------------------------------------------------
if "%MFC_Shared_Debug%"=="0" GOTO :MFC_SHARED_DEBUG_END
@echo Trying to build debug version of ltilib with MFC shared library support...
devenv ltilib.sln /build "MFC Shared Debug"
@if ERRORLEVEL 1 goto :MFC_SHARED_DEBUG_NOT_OK
goto :MFC_SHARED_DEBUG_OK
  :MFC_SHARED_DEBUG_NOT_OK
  @echo.
  @echo Error while building MFC Shared Debug version!!!
  set success=0
  goto :BUILD_SECTION_END
  :MFC_SHARED_DEBUG_OK
  @echo MFC Shared Debug version built successfully!
:MFC_SHARED_DEBUG_END

:: ----------------------------------------------------------------------------
:: Build release version of ltilib with MFC shared library support
:: ----------------------------------------------------------------------------
if "%MFC_Shared_Release%"=="0" GOTO :MFC_SHARED_RELEASE_END
@echo Trying to build release version of ltilib with MFC shared library support...
devenv ltilib.sln /build "MFC Shared Release"
@if ERRORLEVEL 1 goto :MFC_SHARED_RELEASE_NOT_OK
goto :MFC_SHARED_RELEASE_OK
  :MFC_SHARED_RELEASE_NOT_OK
  @echo.
  @echo Error while building MFC Shared Release version!!!
  set success=0
  goto :BUILD_SECTION_END
  :MFC_SHARED_RELEASE_OK
  @echo MFC Shared Release version built successfully!
:MFC_SHARED_RELEASE_END

:: ----------------------------------------------------------------------------
:: Build debug version of ltilib with MFC static library support
:: ----------------------------------------------------------------------------
if "%LTI_Lib_NET_Debug%"=="0" GOTO :LTI_Lib_NET_Debug_END
@echo Trying to build debug version of ltilib with dynamic runtime support...
devenv ltilib.sln /build "LTI-Lib .NET Debug"
@if ERRORLEVEL 1 goto :LTI_Lib_NET_Debug_NOT_OK
goto :LTI_Lib_NET_Debug_OK
  :LTI_Lib_NET_Debug_NOT_OK
  @echo.
  @echo Error while building LTI-Lib .NET Debug version!!!
  set success=0
  goto :BUILD_SECTION_END
  :LTI_Lib_NET_Debug_OK
  @echo LTI-Lib .NET Debug version built successfully!
:LTI_Lib_NET_Debug_END

:: ----------------------------------------------------------------------------
:: Build release version of ltilib with MFC static library support
:: ----------------------------------------------------------------------------
if "%LTI_Lib_NET_Release%"=="0" GOTO :LTI_Lib_NET_Release_END
@echo Trying to build release version of ltilib with dynamic runtime support...
devenv ltilib.sln /build "LTI-Lib .NET Release"
@if ERRORLEVEL 1 goto :LTI_Lib_NET_Release_NOT_OK
goto :LTI_Lib_NET_Release_OK
  :LTI_Lib_NET_Release_NOT_OK
  @echo.
  @echo Error while building LTI-Lib .NET Release version!!!
  set success=0
  goto :BUILD_SECTION_END
  :LTI_Lib_NET_Release_OK
  @echo LTI-Lib .NET Release version built successfully!
:LTI_Lib_NET_Release_END

:: ----------------------------------------------------------------------------
:: Build debug version of ltilib for WIN32
:: ----------------------------------------------------------------------------
if "%Win32_Debug%"=="0" GOTO :WIN32_DEBUG_END
@echo Trying to build debug version of ltilib for WIN32...
devenv ltilib.sln /build "WIN32 Debug"
@if ERRORLEVEL 1 goto :WIN32_DEBUG_NOT_OK
goto :WIN32_DEBUG_OK
  :WIN32_DEBUG_NOT_OK
  @echo.
  @echo Error while building WIN32 Debug version!!!
  set success=0
  goto :BUILD_SECTION_END
  :WIN32_DEBUG_OK
  @echo WIN32 Debug version built successfully!
:WIN32_DEBUG_END

:: ----------------------------------------------------------------------------
:: Build release version of ltilib for WIN32
:: ----------------------------------------------------------------------------
if "%Win32_Release%"=="0" GOTO :WIN32_RELEASE_END
@echo Trying to build release version of ltilib for WIN32...
devenv ltilib.sln /build "WIN32 Release"
@if ERRORLEVEL 1 goto :WIN32_RELEASE_NOT_OK
goto :WIN32_RELEASE_OK
  :WIN32_RELEASE_NOT_OK
  @echo.
  @echo Error while building WIN32 Release version!!!
  set success=0
  goto :BUILD_SECTION_END
  :WIN32_RELEASE_OK
  @echo WIN32 Release version built successfully!
:WIN32_RELEASE_END

:BUILD_SECTION_END

:: ----------------------------------------------------------------------------
:: Copy the lib if desired and the build was successfull-----------------------
:: ----------------------------------------------------------------------------
if "%dir%"=="0" GOTO :END
  @echo.
  @echo.
  @echo -- Copy lib to %dirname%

if "%success%"=="1" goto :COPY_LIB
  @echo.
  @echo Build was not successful. Not copying lib!
goto :END

:COPY_LIB
  @echo.
  echo Checking directory %dirname%
  mkdir %dirname%\lib 
  if NOT exist %dirname%\lib goto :DIR_NOT_OK
  @echo Clearing existing files in %dirname%\lib
  @if exist %dirname%\lib del /Q /S %dirname%\lib\*.* >NUL 2>NUL
  @echo Copying lib to %dirname% ...
  xcopy ..\..\lib %dirname%\lib /y/s/i/r/k
  @echo Finished copying!
  goto :DIR_OK
  :DIR_NOT_OK
  @echo Couldn't create directory for copying lib
  GOTO :END
:DIR_OK
GOTO :END
:: ----------------------------------------------------------------------------


:: The END --------------------------------------------------------------------
:END
@echo.
@echo Done.
@echo.
:: ----------------------------------------------------------------------------
