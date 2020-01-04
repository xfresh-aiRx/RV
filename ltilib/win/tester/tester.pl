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
$main::v_projectName="tester";        # the name of the project
$main::v_rDir="..\\..\\obj\\release"; # directory for release config
$main::v_dDir="..\\..\\obj\\debug";   # directory for debug config
$main::v_libName="ltilib";            # name of the library

$main::v_sourceDir="..\\..\\src";     # the directory that contains all sources

# additional includes and libs (gtk stuff, zlib, libpng, libjpeg)
@main::v_additionalIncludeDirs=("..\\lib");
if ( $have_gtk ) {
  push(@main::v_additionalIncludeDirs, ("..\\gtk+", "..\\gtk+\\gdk", "..\\glib"));
}
                                           
# additional library directories
@main::v_additionalLibDirs=("..\\lib");

@main::v_additionalLibs=("ws2_32.lib", "psapi.lib", "pdh.lib");
if ( $have_gtk ) {
  push(@main::v_additionalLibs, ("glib-1.3.lib", "gdk-1.3.lib", "gtk-1.3.lib", "gmodule-1.3.lib", "gthread-1.3.lib"));
}
if ( $have_libz ) {
  push(@main::v_additionalLibs, "zlib.lib");
}


                                            # additional libraries
# keep settings or sources from existing dsp-file
$main::v_keepSettings=1;                    # settings will be taken, if ==1
$main::v_keepSources=0;                     # ltilib-sources to include will be taken, if ==1

# }

# global variables
# #elem = #file
  # @main::v_filePathList;      # the full file paths, i.e. srcdir\subdir\file
  # @main::v_fileList;          # only the file names
  # @main::v_dirList;           # the dir names srcdir\subdir
  # @main::v_subdirList;        # only the subdirs dir names (without srcdir or file)
# #elem = #subdirs
# @main::v_includeDirs;         # the include directories, i.e. srcdir\subdir 

# @main::v_previousSettings     # settings of existing project (without lti sources)
# @main::v_previousLtiSources   # ltilib sources of existing project

# subroutine for retrieving full paths of files (v_filePathList) --------------
sub mySearch {
  my($Path) = $ARGV[0];
  my($FileName) = lc($_);                     # lower char
  my($all) = $File::Find::name;
  return unless -f;                           # continue, if is file
  return unless ($FileName =~ /^lti/ ) ;      # find only files beginning with lti...
                                              # ... and ending with .cpp or .h
  return unless (($FileName =~ /\.cpp$/ )||($FileName =~ /\.h$/ ));
  $all =~ tr/\//\\/;                          # replace slashes/ with backslashes\
  push (@main::v_filePathList,$all);          # push filepaths (<dir>\<name>) into v_filePathList
}

# subroutine for sorting ------------------------------------------------------
sub mySort {
  lc($a) cmp lc($b);                          # alphabetic sort
}

# subroutine for extracting file names and dirs (v_fileList and v_dirList) ----
sub getFileList {
  foreach $line (@main::v_filePathList) {
    $a = $line;
    $a =~ s/\\[^\\]*$//;                      # trim right of last backslash
    push(@main::v_dirList,$a);

    $a = $line;    
    $a =~ s/.*\\([^\\]*)$/$1/;                # trim left of last backslash
    push(@main::v_fileList,$a);
  }
}

# subroutine for extracting dirs names (v_subdirList) -------------------------
sub getSubdirList {
  foreach $line (@main::v_dirList) {
    my($b) = substr($line,length($v_sourceDir)+1);  # extract subdir 
    push (@main::v_subdirList,$b);
  }
}

# subroutine for extracting the required include dirs (v_includeDirs)----------
sub getIncludeDirs {
  foreach $line (@main::v_dirList) {
    my($found)=0;
    foreach $a (@main::v_includeDirs) { # same as v_dirList, except that
      if ($a eq $line) {                # entries are unique
        $found=1;
      }
    }
    if ($found==0) {
      push (@main::v_includeDirs,$line);
    }
  }
  @main::v_includeDirs=sort mySort @main::v_includeDirs;  
}

# subroutine for reading existing file ----------------------------------------
# previous settings       -> (@main::v_previousSettings) 
# previous ltilib sources -> (@main::v_previousLtiSources) 
sub readExistingFile {
  if (($main::v_keepSettings!=0)||($main::v_keepSources!=0)) {
    if(-e "$v_projectName.dsp") { 
      open(IN,"<$v_projectName.dsp");            # open existing dsp. file
      my($count)=0;
      while(defined($i = <IN>)) { 
        if ($i =~ /^# Begin Group \"$main::v_libName\"/ ) {
          push (@main::v_previousLtiSources,$i);
          $count++;
        }
        elsif ($count!=0) {
          if ($i =~ /^# Begin Group/ ) {
            $count++;
          }
          elsif ($i =~ /^# End Group/) {
            $count--;
          }
          push (@main::v_previousLtiSources,$i);          
        }
        elsif ($count==0) {
          push (@main::v_previousSettings,$i);
        }
      }
      close(IN);
      if (scalar(@main::v_previousSettings)>0) {
        return 1;                               # success
      }
    }
    return 0;                                   # file did'n exist
  }
  else {
    return 0;
  }
}

# perl starts here! -----------------------------------------------------------


my $forceNew=0; # 0: interactive mode, 1: new, 2: keep

if ( $ARGV[0] ) {
  # argument list not empty
    if ($ARGV[0] eq "--new") {
        $forceNew=1;
    } elsif ($ARGV[0] eq "--keep") {
        $forceNew=2;
    } elsif ($ARGV[0] eq "--help") {
        print "\n";
        print "Usage:\n\n         perl tester.pl [options]\n\n";
        print "Options:\n";
        print "         --new : ignore previous settings and create all new\n";
        print "         --keep: keep all previous settings\n";
        print "         --help: show this help\n";
        exit 1;
    }
}

# find all file paths and sort
File::Find::find(\&mySearch, $main::v_sourceDir);
@main::v_filePathList=sort mySort @main::v_filePathList;
# fill the other lists
getFileList();
getSubdirList();
getIncludeDirs();

print "\n";
print "--------------------------------------------------------------------\n";
print "$0\n";
print "Creates .dsp and .dsw files for project \"$main::v_projectName\"\n\n";
print "------------------- ------------------------------------------------\n";

# -----------------------------------------------------------------------------
# open dsp file for writing (modify if existing)
$fileExists=&readExistingFile();
if ($fileExists!=0) {
    if ($forceNew==0) { #interactive mode
        print "$v_projectName.dsp already exists.\n";
        print "Keep project settings (enter=";
        print $main::v_keepSettings; 
        print ", q=exit)?";
        $input = <STDIN>; chop($input);
        $abort = "q";
        if ($input eq $abort) {exit};
        if ($input ne "") {$main::v_keepSettings=$input};
        # 
        print "Keep source settings (enter=";
        print $main::v_keepSources;
        print ", q=exit)?";
        $input = <STDIN>; chop($input);
        if ($input eq $abort) {exit};
        if ($input ne "") {$main::v_keepSources=$input};
        #
        print "\n";
        print "\n";
    } elsif ($forceNew==1) {# force new
        $main::v_keepSources=0;
        $main::v_keepSettings=0;
    } else { #keep all settings
        $main::v_keepSources=1;
        $main::v_keepSettings=1;
    }
}
open(OUT,">$v_projectName.dsp");
# start dsp -------------------------------------------------------------------
if (($fileExists!=0)&&($main::v_keepSettings!=0)) { # modify existing file (keep project settings)-------
  print "Taking settings from existing file \"$v_projectName.dsp\"\n";
  my($found)=0;
  foreach $line (@main::v_previousSettings) {
    if ($line =~ /^# End Target/) { # write lines, until you find this tag
      $found=1;
    }
    elsif ($found==0) {
      print OUT $line;
    }
  }
}
else {  # create default project settings -------------------------------------
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
    foreach $line (@main::v_includeDirs) {
      print OUT "/I \"$line\" ";
    }
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
  print OUT "# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386\n";
  print OUT "# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ";
    foreach $line (@main::v_additionalLibs) {
      print OUT "$line ";
    }
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
    foreach $line (@main::v_includeDirs) {
      print OUT "/I \"$line\" ";
    }
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
  print OUT "# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept\n";
  print OUT "# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ";
    foreach $line (@main::v_additionalLibs) {
      print OUT "$line ";
    }
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
  print OUT "# Begin Group \"Source Files\"\n";
  print OUT "\n";
  print OUT "# PROP Default_Filter \"cpp;c;cxx;rc;def;r;odl;idl;hpj;bat\"\n";
  print OUT "# Begin Source File\n";
  print OUT "\n";
  print OUT "SOURCE=..\\..\\tester\\main.cpp\n";
  print OUT "# End Source File\n";
  print OUT "# Begin Source File\n";
  print OUT "\n";
  print OUT "SOURCE=..\\..\\tester\\ltiTester.cpp\n";
  print OUT "# End Source File\n";
  print OUT "# Begin Source File\n";
  print OUT "\n";
  print OUT "SOURCE=..\\..\\tester\\ltiTester.h\n";
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
} # end of else ---------------------------------------------------------------

# write lti source files ------------------------------------------------------
if ($main::v_keepSources!=0) {
  if (scalar(@main::v_previousLtiSources)==0) {
    print "Adding no $main::v_libName-sources to \"$v_projectName.dsp\"\n";    
  }
  else {
    print "Taking $main::v_libName-sources from existing file \"$v_projectName.dsp\"\n";
    foreach $line (@main::v_previousLtiSources) {
      print OUT $line;
    }
  }
}
else {
  print "Adding all $main::v_libName-sources from $main::v_sourceDir\n";
  # write some info to stdout
  print "\nDirectories ... \n\n";
  foreach $line (@main::v_includeDirs) { print $line."\n"; }
  print "\nFiles ... \n\n";
  for($i = 0; $i<scalar(@main::v_filePathList); $i++) { 
    print $main::v_dirList[$i]."\\";
    print $main::v_fileList[$i]."\n";
  }
  print "\nFound "; print scalar(@main::v_filePathList); print " files ...\n\n";

  print OUT "# Begin Group \"$main::v_libName\"\n";
  print OUT "\n";
  print OUT "# PROP Default_Filter \"\"\n";
  my($actSrcDir)=$main::v_sourceDir; 
  for($i = 0; $i<scalar(@main::v_subdirList); $i++) {
    $line=$main::v_subdirList[$i];
    if ($line ne $actSrcDir) {
      if ($actSrcDir ne $main::v_sourceDir) {
        print OUT "# End Group\n";
      }
      $actSrcDir = $line;
      print OUT "# Begin Group \"$actSrcDir\"\n";
      print OUT "\n";
      print OUT "# PROP Default_Filter \"\"\n";
    }
    print OUT "# Begin Source File\n";
    print OUT "\n";
    print OUT "SOURCE=$main::v_filePathList[$i]\n";
    print OUT "# End Source File\n";
  }
  print OUT "# End Group\n";
  print OUT "# End Group\n";  # $main::v_libName
}
# source files ----------------------------------------------------------------
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
if ($forceNew == 0) {
  print "Please press enter.";
  $input = <STDIN>;
}
