#!usr/bin/perl -w

# this perl-scipt works with ActiveState perl (http://www.ActiveState.com/)
# to search and collect all headerfiles from a specified directory
#

# include packages{
  use File::Find; # use file::Find package for searching all sourcefiles
  use File::Copy; # use File::Copy package for copying files
  use File::Path; # use File::Path package for creating/removing directories
# }

# define some global variables {

$main::v_sourceDir="..\\..\\src";           # the directory that contains all sources
$main::v_destDir="..\\..\\lib";             # the destination directory. 
                                            # headerfiles will be stored in the subdirectory "headersFiles"

# }

# subroutine for searching filenames of sources -------------------------------
sub mySearch {
  my($Path) = $ARGV[0];
  my($FileName) = $_;
    my($all) = $File::Find::name;
  return unless -f;
  return unless ((lc($FileName)=~/\.h$/)&&(lc($FileName)=~/^lti/));
  push (@main::v_fileList,$FileName);
  push (@main::v_filePathList,$all);
}

# perl starts here! -----------------------------------------------------------
print "\n";
print "------------------------------------------------------------------- \n";
print "$0: collects header files from \"$main::v_sourceDir\":\n";
print "All following files will be copied to \"$main::v_destDir\".\n";
print "All without an asterisk (*) will be included in \"ltilib.h\" \n";
print "---\n";

File::Find::find(\&mySearch, $main::v_sourceDir);
($Second,$Minute,$Hour,$DayOfMonth,$Month,$Year)=localtime(time);
$Month++;
$Year+=1900;

open(OUT,">$main::v_destDir\\"."ltilib.h");

print OUT "// This headerfile was generated automatically by Perl Script \"$0\" ";
print OUT sprintf("on %0.2d.%0.2d.%d at %0.2d:%0.2d:%0.2d\n",$DayOfMonth,$Month,$Year,$Hour,$Minute,$Second);
print OUT "// It includes all headerfiles of the LTI-Lib.\n";
print OUT "\n";
print OUT "\n";

print OUT "#ifndef _LTI_HEADERS_H_\n";
print OUT "#define _LTI_HEADERS_H_\n";
print OUT "\n";

# Make directory if not existing yet
# Delete files in directory, if already existing
if (!(-e "$main::v_destDir\\headerFiles")) {
  File::Path::mkpath("$main::v_destDir\\headerFiles",0);
}  
else {
  File::Path::rmtree("$main::v_destDir\\headerFiles",0,1);
  sleep 3;
  File::Path::mkpath("$main::v_destDir\\headerFiles",0); 
}
if (!(-e "$main::v_destDir\\headerFiles")) {
  die "Couldn't create directory\"$main::v_destDir\\headerFiles\" !\n";  
}
  
foreach $nr (0..scalar(@main::v_filePathList)-1) {
  $filePath=$main::v_filePathList[$nr];
  $file=$main::v_fileList[$nr];
  
  File::Copy::copy($filePath,"$main::v_destDir\\headerFiles\\".$file);
  $tempStr = "* $file";
  
  unless (($filePath =~ /\_template\./) || ($file =~ /\_inline\./)) {
    print OUT "\#include \"$file\"\n";
    $tempStr = "$file";
  }
  print "$tempStr\n";
}
print "\n";

print OUT "\n";
print OUT "#endif\n";

close(OUT);
