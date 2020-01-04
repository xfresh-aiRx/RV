#!/usr/bin/perl
# $Id: buildlib.net.pl,v 1.3 2005/06/09 11:13:36 libuda Exp $

# this perl-scipt works with perl to search and
# collect all c++files from a specified directory
# and add them to a Visual Studio.NET project file.

# include packages{
  use File::Find; # use file::Find package for searching all sourcefiles
# }

# define some global variables {
@main::v_addIncDirs=("..\\lapack","..\\jpeg","..\\png","..\\gtk+","..\\gtk+\\gdk","..\\glib");  # list where all static include directories are collected manually.

$main::v_sourceDir="../../src";                     # the directory that contains all sources
$main::v_sourceDirLamath="../../misc/lamath";        # the directory that contains all sources needed for lapack
$main::v_destFile="../buildlib_net/ltilib.vcproj";        # the project file to be created
$main::v_templateHeader="buildlib.net.header.tmpl";     # the template to insert before all files
$main::v_templateFilterHead="buildlib.net.filter.head.tmpl";    # the template to insert before all files
$main::v_templateC="buildlib.net.c.tmpl";           # the template for every .c file found
$main::v_templateH="buildlib.net.h.tmpl";           # the template for every .h file found
$main::v_templateFilterFoot="buildlib.net.filter.foot.tmpl";    # the template to insert after all files
$main::v_templateFooter="buildlib.net.footer.tmpl";     # the template to insert after all files
@main::v_fileList=();                       # list where all files are collected automatically
@main::v_includeDirs=@main::v_addIncDirs;           # list where all include directories are collted autom.
@main::v_openFilters=();                    # list where all open filters are collected
@main::v_projectContent=();                 # list where the script collect the .vcproj data
@main::v_headerTempl=();                    # the template storage to insert before all files
@main::v_filterHead=();                     # the template storage to insert a new filter
@main::v_cTempl=();                     # the template storage for every .c file found
@main::v_hTempl=();                     # the template storage for every .h file found
@main::v_filterFoot=();                     # the template storage to insert end a filter
@main::v_footerTempl=();                    # the template storage to insert after all files
# }

# subroutine for searching filenames of sources -------------------------------
sub mySearch {
  my($Path) = $ARGV[0];
  my($FileName) = $_;
  my($all) = $File::Find::name;
  return unless -f;
  return unless ((lc($FileName)=~/\.h$/)&&(lc($FileName)=~/^lti/)||(lc($FileName)=~/\.cpp$/)&&(lc($FileName)=~/^lti/));
  return if (($all =~ /\_template\./) || ($all =~ /\_inline\./));
  push (@main::v_fileList,$all);
}

# perl starts here! -----------------------------------------------------------
print "\n";
print "------------------------------------------------------------------- \n";
print "$0: creates a brand new Microsoft Visual.NET-project file called:\n";
print "$main::v_destFile for every single .cpp or .h file found in:\n";
print "$main::v_sourceDir and $main::v_sourceDirLamath\n";
print "------------------------------------------------------------------- \n";

File::Find::find(\&mySearch, $main::v_sourceDir);                       # search for our files
File::Find::find(\&mySearch, $main::v_sourceDirLamath);                       # search for our files
                                                # read .h template
open(HTEMPLFILE,"<$main::v_templateH") || die "Cannot open $main::v_templateH for reading!\n";
while(<HTEMPLFILE>) {
  push(@main::v_templateH,$_);
}
close(HTEMPLFILE);

                                                # read .c template
open(CTEMPLFILE,"<$main::v_templateC") || die "Cannot open $main::v_templateC for reading!\n";
while(<CTEMPLFILE>) {
  push(@main::v_templateC,$_);
}
close(CTEMPLFILE);

open(FLTHDTEMPLFILE,"<$main::v_templateFilterHead") || die "Cannot open $main::v_templateFilterHead for reading!\n";
while(<FLTHDTEMPLFILE>) {
  push(@main::v_filterHead,$_);
}
close(FLTHDTEMPLFILE);

open(FLTFTTEMPLFILE,"<$main::v_templateFilterFoot") || die "Cannot open $main::v_templateFilterFoot for reading!\n";
while(<FLTFTTEMPLFILE>) {
  push(@main::v_filterFoot,$_);
}
close(FLTFTTEMPLFILE);


foreach $file (sort @main::v_fileList) {                            # for every file found
  my($dir)=$file;                                       # extract its
  $dir =~ s/(.*)[\/,\\](.*)/$1/;                                # directory
  my($openFilters)=join("/", @main::v_openFilters);                     # make @openFilters compareable
  if ($dir =~ /$main::v_sourceDir/) {
    $openFilters=join("", ("$main::v_sourceDir", "$openFilters"?"/":"", "$openFilters"));
  }
  else {
    $openFilters=join("", ("$main::v_sourceDirLamath", "$openFilters"?"/":"", "$openFilters"));
  }
  my(@toClose)=split("/",$openFilters);                             # now extract every single directoryname
  my(@toOpen)=split("/",$dir);                                  # for path and filters
  while(scalar(@toClose)>0 && scalar(@toOpen)>0 && $toClose[0] eq $toOpen[0]) {         # and remove identical items.
    shift(@toClose); shift(@toOpen);
  }
  foreach $closeFilter (reverse @toClose) {                         # now close unused filters after
    $wasOpen=pop (@main::v_openFilters);                            # checking whether our stack is consistent
    die "Code error: trying to close filter $closeFilter instead of $wasOpen!\n" unless ($wasOpen eq $closeFilter);
    foreach (@main::v_filterFoot) {                             # by inserting the filter-closing template
      my($line)=$_;                                     # every line of the appropiate template
      $line=~s/\$FILTER/$closeFilter/g;                             # replace the $FILTER placeholders
      push (@main::v_projectContent,$line);                         # and store the modified line.
    }
  }
  foreach $openFilter (@toOpen) {                               # now open every newly needed filter
    push (@main::v_openFilters, $openFilter);                           # but not without remembering each one opened
    foreach (@main::v_filterHead) {                             # by inserting the filter-opening template
      my($line)=$_;                                     # every line of the appropiate template
      $line=~s/\$FILTER/$openFilter/g;                              # replace the $FILTER placeholders
      push (@main::v_projectContent,$line);                         # and store the modified line.
    }
  }
  $file=~y/\//\\/;                                      # make paths dos compatible
  $dir=~y/\//\\/;                                       # make paths dos compatible
  if(lc($file)=~/\.h$/) {                                   # check if it is a header file
    push (@main::v_includeDirs, $dir) unless                            # and remember it's directory
        join("", grep($dir eq $main::v_includeDirs[$_], 0..@main::v_includeDirs));  # if we don't know it already.
    foreach (@main::v_templateH) {                              # Now take for the file
      my($line)=$_;                                     # every line of the appropiate template,
      $line=~s/\$FILE/$file/g;                                  # replace the $FILE placeholders
      push (@main::v_projectContent,$line);                         # and store the modified line.
    }
  } else {                                          # if it's no header file found,
    foreach (@main::v_templateC) {                              # for every line of its template
      my($line)=$_;
      $line=~s/\$FILE/$file/g;                                  # replace the $FILE placeholders
      push (@main::v_projectContent,$line);                         # and store the modified lines.
    }
  }
}

foreach $closeFilter (reverse @main::v_openFilters) {                       # now close unused filters after
  foreach (@main::v_filterFoot) {                               # by inserting the filter-closing template
    my($line)=$_;                                       # every line of the appropiate template
    $line=~s/\$FILTER/$closeFilter/g;                               # replace the $FILTER placeholders
    push (@main::v_projectContent,$line);                           # and store the modified line.
  }
}

$INCDIRS=join(',',sort @main::v_includeDirs);                           # Now create a list of directories containing header files.

open(OUT, ">$main::v_destFile") || die "Cannot open $main::v_destFile for writing!\n";      # Create the project file with
# read header template
open(HEADER,"<$main::v_templateHeader") || die "Cannot open $main::v_templateHeader for reading!\n";
  while(<HEADER>) {                                     # every line if the project header
    s/\$INCDIRS/$INCDIRS/g;                                 # with the $INCDIRS placeholder replaced
    print OUT $_;                                       # and store it to the file.
  }
close(HEADER);                                          # That's it for the project header

foreach (@main::v_projectContent) {                             # Now write the collected file information
  s/\$INCDIRS/$INCDIRS/g;                                   # but don't forget to replace the $INCDIRS placeholders!
  print OUT $_;
}

open(FOOTER,"<$main::v_templateFooter") || die "Cannot open $main::v_templateFooter for reading!\n";
  while(<FOOTER>) {
    s/\$INCDIRS/$INCDIRS/g;                                 # And again the same for the project footer...
    print OUT $_;
  }
close(FOOTER);                                          # Finally close all remaining files
close(OUT);

exit 0;                                             # and be happy!
