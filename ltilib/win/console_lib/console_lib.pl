#!/usr/bin/perl -w

# this perl-scipt works with ActiveState perl (http://www.ActiveState.com/)
# to create the console based project file for Microsoft Visual C++ 6.0

# include packages{
  use File::Find; # use file::Find package for searching all sourcefiles
# }

# load configure script
open(IN,"../../src/basics/ltiWinConfig.h") || die "Cannot find ltiWinConfig.h";
@config=<IN>;
close(IN);

$have_gtk=grep {/^\#\s*define\s+HAVE_GTK/} @config;
$have_libz=grep {/^\#\s*define\s+HAVE_LIBZ/} @config;

# define some global variables {
$main::v_projectName="console_lib";         # the name of the project
$main::v_rDir="Release";                    # directory for release config
$main::v_dDir="Debug";                      # directory for debug config
$main::v_libName="ltilib";                  # name of the library

$main::v_rLib="rltilib.lib";                # name of the static release library
$main::v_dLib="ltilib.lib";                 # name of the static debug library


# additional includes and libs (gtk stuff, zlib, libpng, libjpeg)
@main::v_additionalIncludeDirs=("..\\lib", "..\\..\\lib\\headerFiles");
if ( $have_gtk ) {
  push(@main::v_additionalIncludeDirs, ("..\\gtk+", "..\\gtk+\\gdk", "..\\glib"));
}
                                            
# additional library directories
@main::v_additionalLibDirs=("..\\lib", "..\\..\\lib");

@main::v_additionalLibs=("ws2_32.lib", "psapi.lib", "pdh.lib");
if ( $have_gtk ) {
  push(@main::v_additionalLibs, ("glib-1.3.lib", "gdk-1.3.lib", "gtk-1.3.lib", "gmodule-1.3.lib", "gthread-1.3.lib"));
}
if ( $have_libz ) {
  push(@main::v_additionalLibs, "zlib.lib");
}

# }

# global variables
# @main::v_previousSources                  # keep sources of existing project

# subroutine for reading existing file ----------------------------------------
# previous ltilib sources -> (@main::v_previousSources) 
sub readExistingFile {
  if(-e "$v_projectName.dsp") { 
    open(IN,"<$v_projectName.dsp");            # open existing dsp. file
    my($count)=0;
    while(defined($i = <IN>)) { 
      if ($i =~ /^# Begin Group/ ) {
        push (@main::v_previousSources,$i);
        $count++;
      }
      elsif ($count!=0) {
        if ($i =~ /^# Begin Group/ ) {
          $count++;
        }
        elsif ($i =~ /^# End Group/) {
          $count--;
        }
        push (@main::v_previousSources,$i);          
      }
    }
    close(IN);
    return 1;                                 # file exists
  }
  return 0;                                   # file did'n exist
}

# perl starts here! -----------------------------------------------------------
print "\n";
print "--------------------------------------------------------------------\n";
print "$0\n";
print "Creates .dsp and .dsw files for project \"$main::v_projectName\"\n";
print "using static $main::v_libName-libraries\n\n";
print "--------------------------------------------------------------------\n";

$fileExists=&readExistingFile();

# -----------------------------------------------------------------------------
# open dsp file for writing
open(OUT,">$v_projectName.dsp");
# start dsp -------------------------------------------------------------------
print "Applying default settings for \"$v_projectName.dsp\"\n";
print OUT "# Microsoft Developer Studio Project File - Name=\"$v_projectName\" - Package Owner=<4>\n";
print OUT "# Microsoft Developer Studio Generated Build File, Format Version 6.00\n";
print OUT "# ** DO NOT EDIT **\n";
print OUT "\n";

print OUT "# TARGTYPE \"Win32 (x86) Console Application\" 0x0103\n";
print OUT "\n";
print OUT "CFG=$main::v_projectName - Win32 Debug\n";
print OUT "!MESSAGE This is not a valid makefile. To build this project using NMAKE,\n";
print OUT "!MESSAGE use the Export Makefile command and run\n";
print OUT "!MESSAGE \n";
print OUT "!MESSAGE NMAKE /f \"$main::v_projectName.mak\".\n";
print OUT "!MESSAGE \n";
print OUT "!MESSAGE You can specify a configuration when running NMAKE\n";
print OUT "!MESSAGE by defining the macro CFG on the command line. For example:\n";
print OUT "!MESSAGE \n";
print OUT "!MESSAGE NMAKE /f \"$main::v_projectName.mak\" CFG=\"$main::v_projectName - Win32 Debug\"\n";
print OUT "!MESSAGE \n";
print OUT "!MESSAGE Possible choices for configuration are:\n";
print OUT "!MESSAGE \n";
print OUT "!MESSAGE \"$main::v_projectName - Win32 Release\" (based on \"Win32 (x86) Console Application\")\n";
print OUT "!MESSAGE \"$main::v_projectName - Win32 Debug\" (based on \"Win32 (x86) Console Application\")\n";
print OUT "!MESSAGE \n";
print OUT "\n";
print OUT "# Begin Project\n";
print OUT "# PROP AllowPerConfigDependencies 0\n";
print OUT "# PROP Scc_ProjName \"\"\n";
print OUT "# PROP Scc_LocalPath \"\"\n";
print OUT "CPP=cl.exe\n";
print OUT "RSC=rc.exe\n";
print OUT "\n";
# release configuration -------------------------------------------------------
print OUT "!IF  \"\$(CFG)\" == \"$main::v_projectName - Win32 Release\"\n";
print OUT "\n";
print OUT "# PROP BASE Use_MFC 0\n";
print OUT "# PROP BASE Use_Debug_Libraries 0\n";
print OUT "# PROP BASE Output_Dir \"$main::v_rDir\"\n";
print OUT "# PROP BASE Intermediate_Dir \"$main::v_rDir\"\n";
print OUT "# PROP BASE Target_Dir \"\"\n";
print OUT "# PROP Use_MFC 0\n";
print OUT "# PROP Use_Debug_Libraries 0\n";
print OUT "# PROP Output_Dir \"$main::v_rDir\"\n";
print OUT "# PROP Intermediate_Dir \"$main::v_rDir\"\n";
print OUT "# PROP Ignore_Export_Lib 0\n";
print OUT "# PROP Target_Dir \"\"\n";
print OUT "# ADD BASE CPP /nologo /W3 /GX /O2 /D \"WIN32\" /D \"NDEBUG\" /D \"_CONSOLE\" /D \"_MBCS\" /YX /FD /c\n";
print OUT "# ADD CPP /nologo /MD /W3 /GR /GX /O2 ";
  foreach $line (@main::v_additionalIncludeDirs) {
    print OUT "/I \"$line\" ";
  }
  print OUT "/D \"WIN32\" /D \"NDEBUG\" /D \"_CONSOLE\" /D \"_MBCS\" /FD /Zm200 /c\n";
print OUT "# SUBTRACT CPP /YX\n";
print OUT "# ADD BASE RSC /l 0x407 /d \"NDEBUG\"\n";
print OUT "# ADD RSC /l 0x407 /d \"NDEBUG\"\n";
print OUT "BSC32=bscmake.exe\n";
print OUT "# ADD BASE BSC32 /nologo\n";
print OUT "# ADD BSC32 /nologo\n";
print OUT "LINK32=link.exe\n";
print OUT "# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386\n";
print OUT "# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ";
  foreach $line (@main::v_additionalLibs) {
    print OUT "$line ";
  }
  print OUT $main::v_rLib." ";
  print OUT "/nologo /subsystem:console /machine:I386 ";
  foreach $line (@main::v_additionalLibDirs) {
    print OUT "/libpath:\"$line\" ";
  }
  print OUT "\n";
print OUT "\n";
# debug configuration ---------------------------------------------------------
print OUT "!ELSEIF  \"\$(CFG)\" == \"$main::v_projectName - Win32 Debug\"\n";
print OUT "\n";
print OUT "# PROP BASE Use_MFC 0\n";
print OUT "# PROP BASE Use_Debug_Libraries 1\n";
print OUT "# PROP BASE Output_Dir \"$main::v_dDir\"\n";
print OUT "# PROP BASE Intermediate_Dir \"$main::v_dDir\"\n";
print OUT "# PROP BASE Target_Dir \"\"\n";
print OUT "# PROP Use_MFC 0\n";
print OUT "# PROP Use_Debug_Libraries 1\n";
print OUT "# PROP Output_Dir \"$main::v_dDir\"\n";
print OUT "# PROP Intermediate_Dir \"$main::v_dDir\"\n";
print OUT "# PROP Ignore_Export_Lib 0\n";
print OUT "# PROP Target_Dir \"\"\n";
print OUT "# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D \"WIN32\" /D \"_DEBUG\" /D \"_CONSOLE\" /D \"_MBCS\" /YX /FD /GZ /c\n";
print OUT "# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od ";
  foreach $line (@main::v_additionalIncludeDirs) {
    print OUT "/I \"$line\" ";
  }
  print OUT "/D \"WIN32\" /D \"_DEBUG\" /D \"_CONSOLE\" /D \"_MBCS\" /FD /GZ /Zm200 /c\n";
print OUT "# SUBTRACT CPP /YX\n";
print OUT "# ADD BASE RSC /l 0x407 /d \"_DEBUG\"\n";
print OUT "# ADD RSC /l 0x407 /d \"_DEBUG\"\n";
print OUT "BSC32=bscmake.exe\n";
print OUT "# ADD BASE BSC32 /nologo\n";
print OUT "# ADD BSC32 /nologo\n";
print OUT "LINK32=link.exe\n";
print OUT "# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept\n";
print OUT "# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ";
  foreach $line (@main::v_additionalLibs) {
    print OUT "$line ";
  }
  print OUT $main::v_dLib." ";  
  print OUT "/nologo /subsystem:console /debug /machine:I386 /pdbtype:sept ";
  foreach $line (@main::v_additionalLibDirs) {
    print OUT "/libpath:\"$line\" ";
  }
  print OUT "\n";
print OUT "\n";
print OUT "!ENDIF \n";
print OUT "\n";

# general ---------------------------------------------------------------------
print OUT "# Begin Target\n";
print OUT "\n";
print OUT "# Name \"$main::v_projectName - Win32 Release\"\n";
print OUT "# Name \"$main::v_projectName - Win32 Debug\"\n";
if ($fileExists!=0) {
  foreach $line (@main::v_previousSources) {
    print OUT $line;
  }  
}
else {
  print OUT "# Begin Group \"Source Files\"\n";
  print OUT "\n";
  print OUT "# PROP Default_Filter \"cpp;c;cxx;rc;def;r;odl;idl;hpj;bat\"\n";
  print OUT "# Begin Source File\n";
  print OUT "\n";
  print OUT "SOURCE=.\\console_lib.cpp\n";
  print OUT "# End Source File\n";
  print OUT "# End Group\n";
  print OUT "# Begin Group \"Header Files\"\n";
  print OUT "\n";
  print OUT "# PROP Default_Filter \"h;hpp;hxx;hm;inl\"\n";
  print OUT "# End Group\n";
  print OUT "# Begin Group \"Resource Files\"\n";
  print OUT "\n";
  print OUT "# PROP Default_Filter \"ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe\"\n";
  print OUT "# End Group\n";
}

print OUT "# End Target\n";
print OUT "# End Project\n";
# finish dsp ------------------------------------------------------------------
close(OUT);
# -----------------------------------------------------------------------------


# -----------------------------------------------------------------------------
# open dsw file for writing
open(OUT,">$v_projectName.dsw");
# start dsw -------------------------------------------------------------------
print OUT "Microsoft Developer Studio Workspace File, Format Version 6.00\n";
print OUT "# WARNING: DO NOT EDIT OR DELETE THIS WORKSPACE FILE!\n";
print OUT "\n";
print OUT "###############################################################################\n";
print OUT "\n";
print OUT "Project: \"$main::v_projectName\"=.\\$main::v_projectName.dsp - Package Owner=<4>\n";
print OUT "\n";
print OUT "Package=<5>\n";
print OUT "{{{\n";
print OUT "}}}\n";
print OUT "\n";
print OUT "Package=<4>\n";
print OUT "{{{\n";
print OUT "}}}\n";
print OUT "\n";
print OUT "###############################################################################\n";
print OUT "\n";
print OUT "Global:\n";
print OUT "\n";
print OUT "Package=<5>\n";
print OUT "{{{\n";
print OUT "}}}\n";
print OUT "\n";
print OUT "Package=<3>\n";
print OUT "{{{\n";
print OUT "}}}\n";
print OUT "\n";
print OUT "###############################################################################\n";
print OUT "\n";
# finish dsw ------------------------------------------------------------------
close(OUT);
# -----------------------------------------------------------------------------

print "\nCall \"$v_projectName.dsw to use project.\"\n";
print "Please press enter.";
$input = <STDIN>;
