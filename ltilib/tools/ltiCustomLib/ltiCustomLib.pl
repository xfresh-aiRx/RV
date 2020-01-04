#!/usr/bin/perl
# $Id: ltiCustomLib.pl,v 1.5 2004/01/29 14:06:26 alvarado Exp $

use File::Find; # use File::Find package for searching all sourcefiles
use Cwd 'abs_path';

$ltiLibSrcPath="../..";						         		 # path to the ltilib (should be relative)
$ltiLibSrcPathEnv="LTILIB_SRC_PATH";						         # ENV identifier of the path to the ltilib
$ltiLibSrcPath=$ENV{$ltiLibSrcPathEnv} if(grep($ltiLibSrcPathEnv eq $_, keys %ENV));     # Give the environment precedence over the hardcoded path
@ltiLibCfgPaths=("$ltiLibSrcPath/src/basics/config.h",                                   # path to the ltilib's config.h
                 "$ltiLibSrcPath/src/basics/ltiWinConfig.h",
                 "/usr/local/include/ltilib/config.h");

$includeMarker='#include';							# prefix identifying cpp-includes
$ltiIncludePrefix='lti';						        # prefix identifying ltilib files
$idHAVE_LIBJPEG='^#define.*HAVE_LIBJPEG.*1$';					# RegExp identifying that libjpeg was found
$idHAVE_LIBPNG='^#define.*HAVE_LIBPNG.*1$';					# RegExp identifying that libpng was found
$idHAVE_LOCALJPEG='^#define.*HAVE_LOCALJPEG.*1$';                               # RegExp identifying that local jpeg was found
$idHAVE_LOCALPNG='^#define.*HAVE_LOCALPNG.*1$';                                 # RegExp identifying that local png was found
$HAVE_LIBJPEG=0;								# Flag if libjpeg was found
$HAVE_LIBPNG=0;									# Flag if libpng was found
$HAVE_LOCALJPEG=0;								# Flag if local jpeg routines were found
$HAVE_LOCALPNG=0;								# Flag if local png routines were found
@ltiLibList=();									# list of all ltilib sources or headers
@ltiUsedList=();								# list of needed sources from the ltilib
@sourcesToScan=();								# list of sources still to scan
@scannedSources=();								# list of sources we've already schedules for scanning
$onlyFilesToCome=0;								# prevent the script from processing any further options
$stripLibPath=0;								# instruct the script not to add the path to the ltiLib to all filenames
$expandLibPath=0;								# instruct the script to remove relative path elements
$printProjectFile=0;								# switch from raw file mode to MSVS.net(R) mode

# subroutine for searching filenames of sources -------------------------------
sub mySearch {
  my($Path) = $ARGV[0];
  my($FileName) = $_;
  my($all) = $File::Find::name;
  return unless -f;
  return unless ((lc($FileName)=~/\.h$/)&&(lc($FileName)=~/^lti/)||(lc($FileName)=~/\.cpp$/)&&(lc($FileName)=~/^lti/));
#  return if (($all =~ /\_template\./) || ($all =~ /\_inline\./));
  push (@ltiLibList,$all);
}

sub initLibList {
   File::Find::find(\&mySearch, $_[0]);                           # search for our files
}

sub analyseConfigFiles {									# This function scans the config.h's for available code
   my($configName);										# The name of the config.h found
   my($foundConfig);										# Flag whether a config.h was found
   my(@configLines);										# List containing the config.h lines
   $HAVE_LIBJPEG=0;										# Preinitialize all flags
   $HAVE_LIBPNG=0;										#
   $HAVE_LOCALJPEG=0;										#
   $HAVE_LOCALPNG=0;										#
												# Try to open the first available known config.h.
   while(scalar(@ltiLibCfgPaths) && ($configName=shift(@ltiLibCfgPaths)) && !($foundConfig=open(CONFIGFILE,"<$configName"))) {};
   if($foundConfig) {										# If one was successfully opened
     while (<CONFIGFILE>) {									# read the whole file
       push(@configLines, $_);									# into memory
     }
     close(CONFIGFILE);										# and close it afterwards.
     $HAVE_LIBJPEG=1 if(scalar(grep(/$idHAVE_LIBJPEG/, @configLines)));				# Set HAVE_LIBJPEG if the config file says so
     $HAVE_LIBPNG=1  if(scalar(grep(/$idHAVE_LIBPNG/, @configLines)));				# and do the same for HAVE_LIBPNG.
     $HAVE_LOCALJPEG=1 if((!$HAVE_LIBJPEG) && scalar(grep(/$idHAVE_LOCALJPEG/, @configLines)));	# Only check for local JPEG/PNG code
     $HAVE_LOCALPNG=1  if((!$HAVE_LIBPNG) && scalar(grep(/$idHAVE_LOCALPNG/, @configLines)));	# if the library code is not available.
     @configLines=();										# At last: clean up!
     $HAVE_LOCALJPEG=0 if($HAVE_LIBJPEG); $HAVE_LOCALPNG=0 if($HAVE_LIBPNG);			# Be absolutely sure! :)
   }
#       print "Analysis: libJPEG: $HAVE_LIBJPEG  libPNG: $HAVE_LIBPNG  locJPEG: $HAVE_LOCALJPEG  locPNG: $HAVE_LOCALPNG\n";
}

sub scheduleSource {					# This function remembers every file that was not already scanned
  my($sourceFile)=$_[0];
  unless( grep($sourceFile eq $_, @scannedSources) ) {	# if we did'nt already scan the file
    push(@sourcesToScan, $sourceFile);			# schedule it for later scanning
    push(@scannedSources, $sourceFile);			# and remember it to prevent rereading it again.
#                                                      print "       Scheduling $sourceFile ...\n";
  }
}

sub maybeRemeberFile {					# This function remembers every file which is yet unknown
  my($file)=$_[0];
  $_=$file;
  return 0 if($HAVE_LIBJPEG && /ltiJPEGFunctor/);
  return 0 if($HAVE_LIBPNG && /ltiPNGFunctor/);
  unless( grep($file eq $_, @ltiUsedList)) {		# if we don't already know the file provided
    if(open(TEMPFILE,"<$file")) {			# and the given file exists
      close(TEMPFILE);
      push(@ltiUsedList, $file);			# save it to tell the user later about it
#                                                      print "       Remembering $file ...\n";
      scheduleSource($file);				# and maybe schedule the cpp file for later scanning
    }
  }
}

sub scanSource {										# this function analyses the given source code file
  my($source)=$_[0];
  open(SOURCEFILE,"<$source") || die "Unable to scan $source"; 					# try to open the given file or fail
#                                                      print "       Scanning $source ...\n";
  my($sourceLine)="";
  while($sourceLine=<SOURCEFILE>) {								# as long as the file is not completely read
    if(substr($sourceLine, 0, length($includeMarker)) eq $includeMarker) {			# work on every line that includes other files.
      $sourceLine=substr($sourceLine,2+index($sourceLine, '<')+index($sourceLine, '"'));	# if an includion is found, extract the filename
      if(substr($sourceLine, 0, length($ltiIncludePrefix)) eq $ltiIncludePrefix) {		# and chech whether it is from the ltilib
        my($idx)=-1;
        my($incSuffix)="";
        my($incBody)="";
        if(($idx=1+index($sourceLine, '.h>')+index($sourceLine, '.h"'))>=0) {			# if so, check for a header file included
          $incSuffix=".h";									# and remember the suffix.
        } elsif(($idx=1+index($sourceLine, '.cpp>')+index($sourceLine, '.cpp"'))>=0) {		# if it's no header file try a cpp file
            $incSuffix=".cpp";									# and remeber if so.
        }
        if($idx>=0) {										# If the given file was either one type of cpp oder h
          $incBody=substr($sourceLine, 0, $idx);						# extract the filebase
          my($searchExp)=join("", ('/', $incBody, $incSuffix, '$'));				# make sure that the path in the ltilib ends with our filename
          foreach $includedFile (grep(/$searchExp/, @ltiLibList)) {				# and search for file+suffix in our source tree
             my($incBase)=substr($includedFile, 0, length($includedFile)-length($incSuffix));	# for every file found
             scheduleSource($includedFile);							# maybe schedule the included file for later scanning

             $_="$incBody$incSuffix";
             if(/ltiJPEGFunctor/) {								# If the code requires JPEG routines
               if($HAVE_LIBJPEG) {								# and the user has libjpeg
                   $searchExp='/ltiJPEGLibFunctor.cpp$';					# then find the appropiate code
                   my($altFile)=(grep(/$searchExp/, @ltiLibList))[0];				#
                   maybeRemeberFile($altFile);							# and remember it.
               } elsif($HAVE_LOCALJPEG) {							# If the code requires local available JPEG
                 maybeRemeberFile(join("", ($incBase, ".cpp")));				# simply remember to compile the local routines.
               }										#
             } elsif(/ltiPNGFunctor/) {								# Do the same for PNG includes
               if($HAVE_LIBPNG) {								# as we did for JPEG.
                   $searchExp='/ltiPNGLibFunctor.cpp$';						#
                   my($altFile)=(grep(/$searchExp/, @ltiLibList))[0];				#
                   maybeRemeberFile($altFile);							#
               } elsif($HAVE_LOCALPNG) {							#
                 maybeRemeberFile(join("", ($incBase, ".cpp")));				#
               }
             } else {										# Or for regular lti files, simply
               maybeRemeberFile(join("", ($incBase, ".cpp")));					# maybe remember it's cpp source
             }
          }
        }
      }
    }
  }
  close(SOURCEFILE);										# finally close the scanned source file
}

sub scanSources {			# this function scans all scheduled sources until there is no one left.
  while(scalar(@sourcesToScan)) {	# while there are unscanned sources
    scanSource(shift(@sourcesToScan));	# get the next source name and scan it.
  }
}

sub printUsedFiles {				# this function outputs the collected list of cpp sources the given source files require
  print join("\n", sort @ltiUsedList),"\n";	# no comment - I love comments :-)
}

sub mkPathsAbsolute {
  for $item (@ltiUsedList) {
    $item=abs_path($item);
  }
}

sub stripPathFromFiles {				# this function strips the $ltiLibSrcPath from all files found
  foreach $usedFile (@ltiUsedList) {			# for each file found
    $usedFile=~s/^$ltiLibSrcPath[\/\\](.*)/$1/g;	# match the regexp
  }							# and be happy :-)
}

sub filterForeignFiles {
  # @ltiUsedList=grep( (/^$ltiLibSrcPath[\\\/]src/ || /^$ltiLibSrcPath[\\\/]misc/) && !(/\_template\./ || /\_inline\./) , @ltiUsedList);
  @ltiUsedList=grep( (/^$ltiLibSrcPath[\\\/]src/ || /^$ltiLibSrcPath[\\\/]misc/), @ltiUsedList);
}

sub printHelp {				# for those who simply play around with it :-)
  print <<EOT
This cute little scrap of code just determines every file of
source code the files given in the command line depend on.

Usage  : stage1.pl { sourcefile.cpp | --help | -h | --strip | -s | --expand | -e | --.net | -n | -- }
 where :
         (--help  |-h) prints this cute little help text.
         (--strip |-s) strips the path to the ltiLib from the output.
         (--expand|-e) expands any relative paths to absolute ones.
         (--.net  |-n) generates a MSVS.net(R) project file.
         --            stops processing of the arguments above.
                       All further command line arguments will be treated as files.

Example: stage1.pl ~/myProject/myMain.cpp ~/myProject/myModule.h ~/myProject/myModule.cpp
         This will scan through the files myMain.cpp, myModule.h and myModule.h in ~/myProject
         and extract information about every single .cpp file from the ltilib which is needed
         to compile the given source files. The output will contain one line per file.

Environment: LTILIB_SRC_PATH has to point to the root directory of the ltiLib sources.

EOT
}

sub processArgument {								# This function checks the given cla for it's meaning
   my($commandLineArg)=$_[0];
   if($onlyFilesToCome) {							# if the argument is forced to be a filename
     scheduleSource($commandLineArg);						# then schedule the filename for scanning
   } else {									# if we still process clas the clever way
     if($commandLineArg eq "--help" || $commandLineArg eq "-h") {		# check, if the user wants help
       printHelp;								# if so, give him what he wants
     } elsif($commandLineArg eq "--strip" || $commandLineArg eq "-s") {		# does the user not want the whole file path?
       $stripLibPath=1;								# let's make a note about that
       $expandLibPath=0;							# this won't work with absolute paths
     } elsif($commandLineArg eq "--expand" || $commandLineArg eq "-e") {	# does the user not want the whole file path?
       $expandLibPath=1;							# let's make a note about that
       $stripLibPath=0;								# but stripping them will crash
     } elsif($commandLineArg eq "--.net" || $commandLineArg eq "-n") {		# what kind of output does the user want?
       $printProjectFile=1;							# and save his whishes.
     } elsif($commandLineArg eq "--") {						# check if this was the last option the user want to set
       $onlyFilesToCome=1;							# so force us to schedule the remaining clas as file names
     } else {									# if none of the known options fits
       scheduleSource($commandLineArg);						# simply schedule the cla for scanning.
     }
   }
}

sub checkSourceDir {								# this function chechs whether the given path is correct
   if(open(TEMPFILE,"<$ltiLibSrcPath/src/basics/ltiTypes.h")) {			# if we can open the test file
     close(TEMPFILE);								# simply close it again
   } else {									# but if not,
     die "ERROR: $ltiLibSrcPath does not point to a valid ltiLibraryPath\!";	# kill the script radically!
   }
}


# sub main {
   if(scalar(@ARGV)) {							# if the user provided any sources
     checkSourceDir;
     initLibList($ltiLibSrcPath);					# get me a complete list of the ltilib
     analyseConfigFiles;						# determine whether we have libPNG or libJPEG

# $HAVE_LIBJPEG=1;
# $HAVE_LOCALJPEG=0;
# $HAVE_LIBPNG=1;
# $HAVE_LOCALPNG=0;

     foreach $sourceFile (@ARGV) { processArgument($sourceFile) } ;	# schedule each filename provided by the user
     scanSources;							# and start scanning them including their dependencies.
     filterForeignFiles;
     mkPathsAbsolute if($expandLibPath);				# Strip the path to the ltilib from the files found if the user wants it.
     stripPathFromFiles if($stripLibPath);				# Strip the path to the ltilib from the files found if the user wants it.
     if($printProjectFile) {
       print "UNIMPLEMENTED¡\n";
     } else {
       printUsedFiles;							# Afterwards inform the user about the files he will need to compile
     }
  } else {								# if the user did not privide any sources
    printHelp;								# tell him that he's dumb. ]:-)
  }
#}
