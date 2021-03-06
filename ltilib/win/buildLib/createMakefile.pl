#!usr/bin/perl -w

# this perl-scipt works with ActiveState perl (http://www.ActiveState.com/)
# to create a makefile for compiling from command line with Microsoft nmake 
# (, which is part of Visual Studio 6.0). 

# include packages{
  use File::Find; # use file::Find package for searching all sourcefiles
# }

# load configuration file
open(IN,"../../src/basics/ltiWinConfig.h") || die "Cannot find ltiWinConfig.h";
@config=<IN>;
close(IN);

$have_gtk=grep {/^\#\s*define\s+HAVE_GTK/} @config;
$have_libz=grep {/^\#\s*define\s+HAVE_LIBZ/} @config;
$have_local_png=grep {/^\#\s*define\s+HAVE_LOCALPNG/} @config;
$have_local_jpg=grep {/^\#\s*define\s+HAVE_LOCALJPEG/} @config;


# define some global variables {
$main::v_projectName="ltilib";              # the name of the project
$main::v_sourceDir="..\\..\\src";           # the directory that contains all sources
$main::v_includes="/I \".\" /I \"..\\..\\src\\basics\" /I \"..\\..\\src\\math\" /I \"..\\..\\src\\io\" /I \"..\\..\\src\\imgProc\" /I \"..\\..\\src\\system\" /I \"..\\..\\src\\viewer\" /I \"..\\..\\src\\classifiers\" /I \"..\\..\\src\\ioBasics\"";

if ($have_local_png) {
    $main::v_includes.=" /I \"..\\..\\src\\io\\png\"";
}
if ($have_local_jpg) {
    $main::v_includes.=" /I \"..\\..\\src\\io\\jpeg\"";
}
if ($have_gtk) {
    $main::v_includes.=" /I \"..\\gtk+\" /I \"..\\gtk+\\gdk\" /I \"..\\glib\"";
}
if ($have_libz) {
    $main::v_includes.=" /I \"..\\lib\"";
}
print $main::v_includes;
                                            # include directories

$main::v_rObjDir=".\\..\\..\\obj\\release"; # directory to store all intermediary files for release config
$main::v_dObjDir=".\\..\\..\\obj\\debug";   # directory to store all intermediary files for debug config

$main::v_rLibDir="..\\..\\lib";             # directory to store release .lib
$main::v_rLibName="r$v_projectName.lib";    # name of release .lib

$main::v_dLibDir="..\\..\\lib";             # directory to store debug .lib
$main::v_dLibName="$v_projectName.lib";     # name of debug .lib

$main::v_deleteOldObjFiles=1;               # specifies if existing object files should be erased
$main::v_buildDLLversion=1;                 # specifies if the Multithreaded-DLL should be built 
                                            # instead of the Multithreaded
# }


# subroutine for searching filenames of sources -------------------------------
sub mySearch {
  my($Path) = $ARGV[0];
  my($FileName) = $_;
  my($all) = $File::Find::name;
  return unless -f;
  return unless (lc($FileName) =~ /^lti/ ) ;  # find only files beginning with lti...
#  if ($FileName =~ /Include/ ) {
#    return if ($FileName !~ /ltiIncludeHeaders/);
#  }
  return unless ($FileName =~ /\.cpp$/ ) ;     # select files with suffix .cpp
  $all =~ tr/\//\\/;                          # replace slashes with backslashes
  # print $`." -> ".$all."\n";
  print $all."\n";
  push (@main::v_fileList,$`);                # push filenames into v_fileList
  push (@main::v_filePathList,$all);          # push filepaths (<dir>\<name>) into v_filePathList
}

# subroutine for sorting filenames of sources ---------------------------------
sub mySort {
#  lc($a) cmp lc($b);                          # alphabetic sort
}

# perl starts here! -----------------------------------------------------------
print "\n";
print "------------------------------------------------------------------------------- \n";
print "$0: creates makefile for building lib\n";
print "with sources from \"$main::v_sourceDir\". \n";
print "All the following files will be considered.\n";
print "---\n";

File::Find::find(\&mySearch, $main::v_sourceDir);
#@main::v_fileList=sort mySort @main::v_fileList;
($Second,$Minute,$Hour,$DayOfMonth,$Month,$Year)=localtime(time);
$Month++;
$Year+=1900;

# open file and write makefile
open(OUT,">$v_projectName.mak");


# -----------------------------------------------------------------------------
# start makefile --------------------------------------------------------------
print OUT "# Generated by Perl Script \"$0\" NMAKE File for \"$v_projectName\". ";
print OUT sprintf("Created %0.2d.%0.2d.%d at %0.2d:%0.2d:%0.2d\n",$DayOfMonth,$Month,$Year,$Hour,$Minute,$Second);
print OUT "!IF \"\$(CFG)\" == \"\"\n";
print OUT "CFG=$v_projectName - Win32 Debug\n";
print OUT "!MESSAGE No configuration specified. Defaulting to $v_projectName - Win32 Debug.\n";
print OUT "!ENDIF \n";
print OUT "\n";


# release configuration -------------------------------------------------------
print OUT "!IF \"\$(CFG)\" != \"$v_projectName - Win32 Release\" && \"\$(CFG)\" != \"$v_projectName - Win32 Debug\"\n";
print OUT "!MESSAGE Invalid configuration \"\$(CFG)\" specified.\n";
print OUT "!MESSAGE You can specify a configuration when running NMAKE\n";
print OUT "!MESSAGE by defining the macro CFG on the command line. For example:\n";
print OUT "!MESSAGE \n";
print OUT "!MESSAGE NMAKE /f \"$v_projectName.mak\" CFG=\"$v_projectName - Win32 Debug\"\n";
print OUT "!MESSAGE \n";
print OUT "!MESSAGE Possible choices for configuration are:\n";
print OUT "!MESSAGE \n";
print OUT "!MESSAGE \"$v_projectName - Win32 Release\" (based on \"Win32 (x86) Static Library\")\n";
print OUT "!MESSAGE \"$v_projectName - Win32 Debug\" (based on \"Win32 (x86) Static Library\")\n";
print OUT "!MESSAGE \n";
print OUT "!ERROR An invalid configuration is specified.\n";
print OUT "!ENDIF \n";
print OUT "\n";

print OUT "!IF \"\$(OS)\" == \"Windows_NT\"\n";
print OUT "NULL=\n";
print OUT "!ELSE \n";
print OUT "NULL=nul\n";
print OUT "!ENDIF \n";
print OUT "\n";

print OUT "CPP=cl.exe\n";
print OUT "RSC=rc.exe\n";
print OUT "\n";

print OUT "!IF  \"\$(CFG)\" == \"$v_projectName - Win32 Release\"\n";
print OUT "\n";

print OUT "OUTDIR=$v_rObjDir\n";
print OUT "INTDIR=$v_rObjDir\n";
print OUT "\n";
print OUT "ALL : \"$v_rLibDir\\$v_rLibName\"";
print OUT "\n";
print OUT "\n";
print OUT "\n";

if ($v_deleteOldObjFiles > 0) {
  print OUT "CLEAN :\n";
  foreach $file (@main::v_fileList) {
    print OUT "\t-\@erase \"\$(INTDIR)\\".$file.".obj\"\n";
  }
  print OUT "\t-\@erase \"\$(INTDIR)\\vc60.idb\"\n";
  print OUT "\t-\@erase \"\$(INTDIR)\\vc60.pdb\"\n";  
  print OUT "\t-\@erase \"$v_rLibDir\\$v_rLibName\"\n";
  print OUT "\n";
}

print OUT "\"\$(OUTDIR)\" :\n";
print OUT "    if not exist \"\$(OUTDIR)/\$(NULL)\" mkdir \"\$(OUTDIR)\"\n";
print OUT "\n";

if ($main::v_buildDLLversion > 0) {
  print OUT "CPP_PROJ=/nologo /MD /W3 /GR /GX /O2 /Zi ".$v_includes." /D \"_WIN32\" /D \"NDEBUG\" /D \"_MBCS\" /D \"_LIB\" /Fo\"\$(INTDIR)\\\\\" /Fd\"\$(INTDIR)\\\\\" /FD /Zm200 /c \n";  
}
else {
  print OUT "CPP_PROJ=/nologo /MT /W3 /GR /GX /O2 /Zi ".$v_includes." /D \"_WIN32\" /D \"NDEBUG\" /D \"_MBCS\" /D \"_LIB\" /Fo\"\$(INTDIR)\\\\\" /Fd\"\$(INTDIR)\\\\\" /FD /Zm200 /c \n";
}
print OUT "BSC32=bscmake.exe\n";
print OUT "BSC32_FLAGS=/nologo /o\"\$(OUTDIR)\\$v_projectName.bsc\" \n";
print OUT "BSC32_SBRS= \\";
print OUT "\n";
print OUT "\n";

print OUT "LIB32=link.exe -lib\n";
print OUT "LIB32_FLAGS=/nologo /out:\"$v_dLibDir\\$v_rLibName\" \n";
print OUT "LIB32_OBJS= \\\n";
foreach $file (@main::v_fileList) {
  unless (@main::v_fileList[scalar(@main::v_fileList)-1] eq $file) {print OUT "\t\"\$(INTDIR)\\".$file.".obj\" \\\n";}
  else {print OUT "\t\"\$(INTDIR)\\".$file.".obj\"\n";}
}
print OUT "\n";

print OUT "\"$v_dLibDir\\$v_rLibName\" : \"\$(OUTDIR)\" \$(DEF_FILE) \$(LIB32_OBJS)\n";
print OUT "    \$(LIB32) \@<<\n";
print OUT "  \$(LIB32_FLAGS) \$(DEF_FLAGS) \$(LIB32_OBJS)\n";
print OUT "<<\n";


# debug configuration ---------------------------------------------------------
print OUT "\n";
print OUT "!ELSEIF  \"\$(CFG)\" == \"$v_projectName - Win32 Debug\"\n";
print OUT "\n";

print OUT "OUTDIR=$v_dObjDir\n";
print OUT "INTDIR=$v_dObjDir\n";
print OUT "\n";
print OUT "ALL : \"$v_dLibDir\\$v_dLibName\"";
print OUT "\n";
print OUT "\n";
print OUT "\n";

if ($v_deleteOldObjFiles > 0) {
  print OUT "CLEAN :\n";
  foreach $file (@main::v_fileList) {
    print OUT "\t-\@erase \"\$(INTDIR)\\".$file.".obj\"\n";
  }
  print OUT "\t-\@erase \"\$(INTDIR)\\vc60.idb\"\n";
  print OUT "\t-\@erase \"\$(INTDIR)\\vc60.pdb\"\n";
  print OUT "\t-\@erase \"$v_dLibDir\\$v_dLibName\"\n";
  print OUT "\n";
}

print OUT "\"\$(OUTDIR)\" :\n";
print OUT "    if not exist \"\$(OUTDIR)/\$(NULL)\" mkdir \"\$(OUTDIR)\"\n";
print OUT "\n";

if ($main::v_buildDLLversion > 0) {
  print OUT "CPP_PROJ=/nologo /MDd /W3 /Gm /GR /GX /Zi /Od ".$v_includes." /D \"_WIN32\" /D \"_DEBUG\" /D \"_MBCS\" /D \"_LIB\" /Fo\"\$(INTDIR)\\\\\" /Fd\"\$(INTDIR)\\\\\" /FD /Zm200 /GZ /c \n";
}
else {
  print OUT "CPP_PROJ=/nologo /MTd /W3 /Gm /GR /GX /Zi /Od ".$v_includes." /D \"_WIN32\" /D \"_DEBUG\" /D \"_MBCS\" /D \"_LIB\" /Fo\"\$(INTDIR)\\\\\" /Fd\"\$(INTDIR)\\\\\" /FD /Zm200 /GZ /c \n";
}
print OUT "BSC32=bscmake.exe\n";
print OUT "BSC32_FLAGS=/nologo /o\"\$(OUTDIR)\\$v_projectName.bsc\" \n";
print OUT "BSC32_SBRS= \\\n";
print OUT "\n";

print OUT "LIB32=link.exe -lib\n";
print OUT "LIB32_FLAGS=/nologo /out:\"$v_dLibDir\\$v_dLibName\" \n";
print OUT "LIB32_OBJS= \\\n";
foreach $file (@main::v_fileList) {
  unless (@main::v_fileList[scalar(@main::v_fileList)-1] eq $file) {print OUT "\t\"\$(INTDIR)\\".$file.".obj\" \\\n";}
  else {print OUT "\t\"\$(INTDIR)\\".$file.".obj\"\n";}
}
print OUT "\n";

print OUT "\"$v_dLibDir\\$v_dLibName\" : \"\$(OUTDIR)\" \$(DEF_FILE) \$(LIB32_OBJS)\n";
print OUT "    \$(LIB32) \@<<\n";
print OUT "  \$(LIB32_FLAGS) \$(DEF_FLAGS) \$(LIB32_OBJS)\n";
print OUT "<<\n";
print OUT "\n";
print OUT "!ENDIF \n";
print OUT "\n";


# source browse files ---------------------------------------------------------
print OUT ".c{\$(INTDIR)}.obj::\n";
print OUT "   \$(CPP) \@<<\n";
print OUT "   \$(CPP_PROJ) \$< \n";
print OUT "<<\n";
print OUT "\n";
print OUT ".cpp{\$(INTDIR)}.obj::\n";
print OUT "   \$(CPP) \@<<\n";
print OUT "   \$(CPP_PROJ) \$< \n";
print OUT "<<\n";
print OUT "\n";
print OUT ".cxx{\$(INTDIR)}.obj::\n";
print OUT "   \$(CPP) \@<<\n";
print OUT "   \$(CPP_PROJ) \$< \n";
print OUT "<<\n";
print OUT "\n";
print OUT ".c{\$(INTDIR)}.sbr::\n";
print OUT "   \$(CPP) \@<<\n";
print OUT "   \$(CPP_PROJ) \$< \n";
print OUT "<<\n";
print OUT "\n";
print OUT ".cpp{\$(INTDIR)}.sbr::\n";
print OUT "   \$(CPP) \@<<\n";
print OUT "   \$(CPP_PROJ) \$< \n";
print OUT "<<\n";
print OUT "\n";
print OUT ".cxx{\$(INTDIR)}.sbr::\n";
print OUT "   \$(CPP) \@<<\n";
print OUT "   \$(CPP_PROJ) \$< \n";
print OUT "<<\n";
print OUT "\n";
print OUT "\n";

print OUT "!IF \"\$(NO_EXTERNAL_DEPS)\" != \"1\"\n";
print OUT "!IF EXISTS(\"$v_projectName.dep\")\n";
print OUT "!INCLUDE \"$v_projectName.dep\"\n";
print OUT "!ELSE \n";
print OUT "!MESSAGE Warning: cannot find \"$v_projectName.dep\"\n";
print OUT "!ENDIF \n";
print OUT "!ENDIF \n";
print OUT "\n";
print OUT "\n";
print OUT "!IF \"\$(CFG)\" == \"$v_projectName - Win32 Release\" || \"\$(CFG)\" == \"$v_projectName - Win32 Debug\"\n";

foreach $file (@main::v_fileList) {
  $x=0;
  $tfile="";
  while ($main::v_filePathList[$x] !~ /\\$file.cpp/) {
    $x++;
  }
  $tfile=$v_filePathList[$x];
  $tfile =~ tr/\//\\/;

  print OUT "SOURCE=".$tfile."\n";
  print OUT "\n";
  print OUT "\"\$(INTDIR)\\".$file.".obj\" \"\$(INTDIR)\\".$file.".sbr\" : \$(SOURCE) \"\$(INTDIR)\"\n";
  print OUT "\t\$(CPP) \$(CPP_PROJ) \$(SOURCE)\n";
  print OUT "\n";
  print OUT "\n";
}
print OUT "\n";
print OUT "!ENDIF \n";
print OUT "\n";

# end makefile ----------------------------------------------------------------
# -----------------------------------------------------------------------------

# close output file
close(OUT);

print "\n";
