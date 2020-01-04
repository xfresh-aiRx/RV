@echo off
@cls
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
@echo   -nomake           don't create makefile
@echo   -noheaders        don't collect headerfiles
@echo   -dir "dirname"    copy lib to specified directory, if successful
goto :END
:ENDHELP
:: ----------------------------------------------------------------------------


:: Process options ------------------------------------------------------------
@echo.
@echo.
@echo -- Logfile for automatic build of LtiLib --

if %clear%==0 goto :ENDCLEAR
  @echo.
  @echo.
  @echo -- Deleting existing object files ...
  @if exist ..\..\obj\debug del /Q ..\..\obj\debug\*.*
  @if exist ..\..\obj\release del /Q ..\..\obj\release\*.* 
:ENDCLEAR

if %co%==0 goto :ENDCO
  @cd ..\..
  @echo.
  @echo.
  @echo -- Updating sources from CVS repository ...
  set CVSROOT=:pserver:ltilib@tintas.techinfo.rwth-aachen.de:/home/cvsroot/lti
  cvs -d :pserver:ltilib@tintas.techinfo.rwth-aachen.de:/home/cvsroot/lti update -d
  @cd win\buildLib
:ENDCO

if %nomake%==1 goto :ENDNOMAKE
  @echo.
  @echo.
  @perl createMakefile.pl 
:ENDNOMAKE

if %noheaders%==1 goto :ENDNOHEADERS
  @echo.
  @echo.
  @perl collectHeaders.pl 
:ENDNOHEADERS
:: ----------------------------------------------------------------------------


:: Build the lib --------------------------------------------------------------
@echo.
@echo.
@echo -- Building Debug Version ...
NMAKE /NOLOGO /S /K /F "ltilib.mak" CFG="ltilib - Win32 Debug" /X "debug.log"
@if ERRORLEVEL 1 goto :DEBUG_NOT_OK
goto :DEBUG_OK
  :DEBUG_NOT_OK
  @echo.
  @echo Error while building debug version!!!
  @echo Check path for nmake and mspdb60.dll.
  set success=0
  goto :ENDDEBUG
  :DEBUG_OK
  @echo Debug version built successfully!
:ENDDEBUG

@echo.
@echo.
@echo -- Building Release Version ...
NMAKE /NOLOGO /S /K /F "ltilib.mak" CFG="ltilib - Win32 Release" /X "release.log"
@if ERRORLEVEL 1 goto :RELEASE_NOT_OK
goto :RELEASE_OK
  :RELEASE_NOT_OK
  @echo.
  @echo Error while building release version!!!
  @echo Check path for nmake and mspdb60.dll.
  set success=0
  goto :ENDRELEASE
  :RELEASE_OK
  @echo Release version built successfully!
:ENDRELEASE
:: ----------------------------------------------------------------------------


:: Copy the lib (if desired) --------------------------------------------------
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
  mkdir %dirname% >NUL 2>NUL
  if NOT exist %dirname% goto :DIR_NOT_OK
    @echo Clearing existing files in %dirname%
    @if exist %dirname%\obj del /Q /S %dirname%\obj\*.* >NUL 2>NUL
    @if exist %dirname%\lib del /Q /S %dirname%\lib\*.* >NUL 2>NUL
    @echo Copying lib to %dirname% ...
    xcopy ..\..\obj %dirname%\obj /d/s/e/c/i/r/h/k >NUL 2>NUL
    xcopy ..\..\lib %dirname%\lib /d/s/e/c/i/r/h/k >NUL 2>NUL
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
@echo --
@echo.
:: ----------------------------------------------------------------------------
