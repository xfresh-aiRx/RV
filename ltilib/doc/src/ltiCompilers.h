/*
 * Copyright (C) 1998, 1999, 2000, 2001
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * 
 * This file is part of the LTI-Computer Vision Library (LTI-Lib)
 *
 * The LTI-Lib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License (LGPL)
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * The LTI-Lib is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public 
 * License along with the LTI-Lib; see the file LICENSE.  If
 * not, write to the Free Software Foundation, Inc., 59 Temple Place -
 * Suite 330, Boston, MA 02111-1307, USA.  
 */ 

#ifndef _LTI_COMPILERS
#define _LTI_COMPILERS

/** 

\page compilers Some Tips for the supported compilers
  
- \ref gcc
- \ref msvc

\section gcc Linux/GCC

\subsection gccReq Requirements

To compile the LTI-Lib using all its power, you will need the following:

- C++ Compiler GCC 2.95.3 or higher is required.  More than the
  compiler itself, the STL implementation in 2.95.3 is required,
  because some features and files (like \<sstream\>) are in 2.95.2
  still missing.  GCC 3.2 and pre-releases of GCC 3.3 are being
  intensively used.
  There have been reports of problems with the un-official 
  <a href="http://gcc.gnu.org/gcc-2.96.html">GCC 2.96</a>.  If you use
  the later compiler, we recomend to downgrade to 2.95.3 or to upgrade
  to the latest GCC 3.3 or higher release.
- Some viewer classes use the <a href="http://www.gtk.org">GTK</a>
  toolkit.  On your system you will need to have the developer's package
  installed.
- GNU Make version 3.79.1 or higher is required to build the library.
- \c autoconf version 2.52 or higher is required to build the \c
  configure script, used to detect which features are installed that
  the LTI-Lib can make use of.  The related \c aclocal must have version 1.5 or
  higher.
- Many Linux distributions provide the BASH shell as standard shell.  This
  fact is also assumed here and bash should be the shell you use while 
  calling the \c configure scripts and the \c make files.
- PERL is required to interpret some script files that simplify the development
  with/for the LTI-Lib.  All major Linux distributions have PERL packages,
  but you can also get it from
  <a href="http://www.perl.org/">here</a>.

You may also need:

- <a href="http://www.doxygen.org/">Doxygen</a>.  All the LTI-Lib is
  documented using this great tool.  If you want to generate the complete HTML
  documentation you will need it and also the 
  <a href="http://www.research.att.com/sw/tools/graphviz/">Graphviz</a>
  graph drawing software required by Doxygen to generate the class
  trees.
- LAPACK to use more efficiently some complex mathematical operations
  (e.g. eigenvalue/eigenvector computation, LDA). For this to be
  available you need to install the packages lapack, blas and the f2c
  compiler which should come with your distribution. See also \ref
  lapack.  The Fortran compiler with which the lapack was compiled is also 
  necessary (usually provided with the GCC complete package or as g77). 

\subsection gccCompile Compile the LTI-Lib

First, check if the <code>configure</code> script already exists in
your <code>ltilib/linux</code> directory.  If not, of if you are not sure that
this is the correct one, just execute in your BASH terminal

\code
cd ltilib/linux
make -f Makefile.cvs
\endcode

After that, you can configure your installation with

\code
./configure
\endcode

You can of course customize your installation with several options
(type ./configure --help for more information).

Now you can compile the debug and release libraries and testers with

\code
make 
\endcode

You can at this point make use of your local copy of the library.  
The <code>ltilib/tester/ltiTester.cpp</code> file can be used to try out small
programs.  You can create two executable test programs with:

\code
make dtester rtester
\endcode

This will create in <code>ltilib/bin</code> two executable files: 
- <code>dtester</code> is your program in the debug version, with all necessary
  debugging information, without optimizations and the most important thing,
  with all possible boundary check mechanisms activated (which make the code
  much slower as usual!).
- <code>rtester</code> is your tester program in the release version, which
  runs much faster than the debug one.
- the target <code>ptester</code> can also be used if you want to profile 
  your code.

Every <code>*.cpp</code> file in the <code>ltilib/linux/workbench</code>
directory will also be compiled and linked to the tester/rtester programs, so
that you can build more complex test applications.

You can also install the library in your system, assuming you have write
privileges for the <code>--prefix</code> directory given in the
<code>configure</code> script (if nothing was given, \c /usr/local is the 
default).  Just try:

\code
make install
\endcode

\subsection makefile The Makefile

The Makefile in <code>ltilib/linux/Makefile</code> provides many
options and targets.  Use the targets as usual, i.e. just type
\code
make target
\endcode 
in your xterm, konsole, or whatsoever bash-terminal you are using. Using make without target is equivalent to calling target all-debug and all-release, thus building the library and testers in debug and release version.

The most useful targets are:
- \b dtester to compile the tester application in debug mode.
- \b rtester to compile the tester application in release mode.
- \b ptester to compile the tester application in profile mode.
- \b bins to compile the debug and release versions of the tester application.
- \b libd to compile the complete library in debug version.
- \b libr to compile the complete library in release version.
- \b libp to compile the complete library in profile version.
- \b libsd to compile the math library, the library without viewers and the
  complete library in debug version.
- \b libsr to compile the math library, the library without viewers and the
  complete library in release version.
- \b libsp to compile the math library, the library without viewers and the
  complete library in profile version.
- \b all-debug like libd and dtester.
- \b all-release like libr and rtester.
- \b all-profile like libp and ptester.
- \b clean to remove all temporary object files, i.e. the libraries and
  tester applications will \e not be removed.
- \b clean-libs to remove all libraries, but not the temporary object files.
- \b clean-docs to remove all documentation generated by Doxygen.
- \b clean-all to remove all generated data: documentation, object files, 
  libraries, even the backup files generated by Emacs (*~).
- \b distclean like clean-all and additionally removes the Makefiles.
- \b install to install the library in the \a prefix specified to the
  \c configure script (type <code>./configure --help</code> for more 
  information).
- \b doxydoc to generate the documentation using Doxygen.  It assumes your 
  system has the latest Doxygen version and the Graphviz tool \c dot.
- \b dep to generate the dependencies.  You will need this target each time
  you add new files to the library and you notice your file is not compiled 
  even if something changed that your file directly or indirectly includes.

With the variable \c DEBUGEXC set to \c -D_DEBUG_EXCEPTION you can control if
the lti::exceptions will write to std::err a text message that should help
you to debug your programs.  However, this can (or should) be set in your
<code>extrapaths.mk</code> file, in order to survive your next "configure"
(see the next section for more information).

\subsubsection gccAddLibs Using external libraries and header files

For the use of many frame-grabbers and other hardware related classes, it
is usually required that some special hardware drivers or hardware related
libraries can be linked with the LTI-Lib.  For these and many other
special cases it is desirable to have a simple way to indicate:

- Additional include paths, where special header files can be found
- Additional library paths, where specific libraries can be found
- Additional libraries, that should be linked with the LTI-Lib in order
  to provide specific functionality.

Include paths are required by almost any frame-grabber or camera utility,
except if you provide that all headers can be found in the standard system
include paths.

Library paths are only necessary if you also specify some libraries to be
linked with, which are not located in the standard library directories
(e.g. /usr/lib or /usr/local/lib).

The mechanism provided by the LTI-Lib should "survive" a ./configure, since
otherwise you would need to make changes to the Makefile  each time you get a
newer library version or you just want to reconfigure some options.

You just need to create a file called \e "extrapaths.mk", in which following
variables are expected:

-  EXTRAINCLUDEPATH for additional include paths
-  EXTRALIBPATH     for additional library paths
-  EXTRALIBS        for additional libraries

For example, to use the lti::microEnablePulnix frame-grabber/camera
functor, you should include in "extrapaths.mk" following three lines
\code
EXTRAINCLUDEPATH = -I/usr/src/menable/include
EXTRALIBPATH = -L/usr/src/menable/lib
EXTRALIBS =  -lpulnixchanneltmc6700 -lmenable
\endcode
      
Please note the standard GCC flags used to indicate:
- \c -I path to headers
- \c -L path to libraries
- \c -l libraries.

There is a template for such a file in the linux directory: \e "extrapaths_template.mk". It contains examples for the Philips ToUCam and for Pulnix + MicroEnable framegrabber.

\subsection gccExam Examples

In the directory <code>ltilib/examples</code> you can find several 
directories with simple code examples using the LTI-Lib.  You can copy any
Makefile in a project of your own, in order to use the LTI-Lib without
much overhead.

<hr>   
  
\section msvc Microsoft Visual C++
  
\subsection vcReq Requirements

The LTI-Lib makes use of some C++ features and system functionality that
makes following requirements necessary:

- <b>Microsoft Visual C++</b>.  The LTI-Lib was modified in order to compile
  with the Visual C++ Version 6.0.  Visual C++ .NET 2002 was not able to deal
  with some ANSI compatible C++ template constructions widely used in the
  LTI-Lib, and is therefore not supported.  Version .NET 2003 can compile
  the LTI-Lib without problems.
- <b>GTK+</b>.  In order to provide a system independent widget set, 
  the GTK+ Library was chosen.  
  These are required only for the visualization classes,
  and you could modify your projects to avoid their use, but all default 
  projects will assume you want to exploit them.
  You can get the original GTK+ libraries from
  <A HREF="http://www.gimp.org/~tml/gimp/win32/downloads.html/">here</A>.
  You will need the three toolkit layers: glib, gdk and gtk.  At this time,
  we use the version 1.3 of the toolkit.  You also find sources and binaries of
  GTK within the Windows Installer Package.
- <b>ws2_32.lib</b> If not already in your system, just search the Internet for
  it.  It is easy to find.  (Also provided in the Installer)
- <b>psapi.lib</b> This should come with your Visual C++, but can also
  be found on the Internet. (Also provided in the Installer)
- <b>pdh.lib</b> The Performance Data Helper library can be found 
  <a href="http://support.microsoft.com/default.aspx?scid=kb;en-us;Q284996">
  here</a>, or search the Microsoft Knowledge Base for article "Q284996".
  (Also provided in the Installer)
- <a href="http://www.activestate.com/Products/ActivePerl/">PERL</a> is
  required to create automatically the library and some example project files.
  (It can also be found in one of the Installer Packages)

If you use the LTI-Lib Installer for Windows all required libraries can be 
directly installed from there.

\subsection Project creation

A project that uses the LTI-Lib can either be created manually or you can 
let a PERL-script do the job for you. Nevertheless you should read the 
section about the manual creation of a project to see which settings are 
required.

There are always two possibilities to work with the LTI-Lib: either as 
source code or as static library. The first method is required if you want 
to make changes to existing library sources. The second is preferable if you 
only want to use the library or to develop new classes. 


\subsection vcAutomatic Automatic creation of a new project
 
In <code>ltilib\\win</code> you will find the folders
<code>console_src</code>, <code>console_lib</code> and <code>tester</code>.

Each contains a PERL script that generates a console project (.dsp and .dsw
files) with default settings. 
If the project files already exist, then the
scripts will only perform an update. A relative path to the sources is
assumed, i.e. if you want to move your project's location, you will have to
change settings manually. 

The <code>console_lib</code> project assumes you have already created and
installed the library.
To do this go to the directory <code>ltilib\\win\\buildLib</code> and
execute the batch file <code>buildLib.bat</code>.  Don't forget to set up your
environment variables using the <code>VCVARS32.BAT</code> file provided in
your Visual C++ distribution (usually in directory <code>VC98\\Bin</code>.
  
These scripts assume you have installed the GTK+ libraries.  We recommend you 
to install them, in order to use the visualization classes of the LTI-Lib.
Using the Windows Installer, this is simply a check box in the offered
packages.

For example, let us assume, you are in the installation path of the library.
You can create a simple tester project with:
\code
cd ltilib\win\tester
perl tester.pl --new
\endcode

After that, you can open the <code>tester.dsw</code> file with the
Visual Studio and compile both, debug and release versions, as usual.

\subsection vcManual Manual creation of a new project.

The following instructions apply, if you create a new project and want to 
adjust settings manually.
  
-  <B>Working with sources:</B>
   Include all the LTI-Lib sources directly in your project.
   -  Add all LTI-Lib sources to the file view of your project workspace.
   -  Indicate under Project->Settings->C/C++->Preprocessor for all 
      configurations the directory path to all the %lti-headers. The
      directories you should provide are <code>ltilib\\src\\basics,
      ltilib\\src\\imgProc, ltilib\\src\\ioBasics, ltilib\\src\\io,
      ltilib\\src\\io\\png, ltilib\\src\\io\\jpeg, ltilib\\src\\basics,
      ltilib\\src\\math, ltilib\\src\\viewer, ltilib\\src\\system</code>,
      where <code>ltilib</code> should be replaced with the absolute or 
      relative path of your ltilib directory.
   -  Activate Project->Settings->C/C++->C++ Language:Enable RTTI.
   -  You will also need to enlarge the heap used by the compiler with the
      option /Zm200.  To to this just type this option in the small edit
      box at the bottom of the dialog window in 
      Project->Settings->C/C++->General.
-  <B>Working with static library:</B>
   compile the LTI-Lib to a static library (i.e. a *.lib file) and link that
   to your project. 
   -  Call the <code>buildLib.bat</code> batch file located in 
      <code>ltilib\\win\\buildLib</code> (Perl must be installed!). 
      This will build the libraries in <code>ltilib\\lib</code>.
      Additionally all header-files will be collected and placed in 
      <code>ltilib\\lib\\headerFiles</code>.
   -  Under Project->Settings->Link->General add <code>ltilib.lib</code>
      to the debug configuration and <code>rltilib.lib</code> to the release
      configuration.
   -  Under Project->Settings->C/C++->Preprocessor indicate the
      directory path to <code>ltilib\\lib\\headerFiles</code> for all
      configurations.
   -  Activate Project->Settings->C/C++->C++ Language:Enable RTTI.
   -  Note: by default the lib is built with the option /MD resp. 
      /MDd, i.e. the multi-threaded-dll run-time libraries are used. 
      This means that MFC must be linked dynamically. However, if you 
      need to use MFC in a static lib (not recommended), then you must 
      set the variable <code>v_buildDLLversion=0</code> in the Perl script 
      <code>createMakefile.pl</code> beforehand. 
-  <B>General:</B> The lti::viewer and other visualization classes are
   based on the GNU GIMP-Toolkit
   (<A HREF="http://www.gtk.org">GTK</A>). To use it, you will 
   need to access its header files and static linked libraries, 
   and you will need to grant your system access to the DLL-libraries.  
   You can get the original GTK+ libraries from
   <A HREF="http://www.gimp.org/~tml/gimp/win32/downloads.html/">here</A>,
   or if you
   are in the <code>techinfo.rwth-aachen.de</code> domain, you can get a
   (maybe outdated) package with all required libraries from 
   <A HREF="http://tintas.techinfo.rwth-aachen.de/software/xwinlibs.tar.gz">here</A>.
   If you can get this last file, unpack it into the folder 
   <code>ltilib\\win</code>.  Otherwise, you should get the glib, gdk 
   and gtk libraries from 
   <A HREF="http://www.gimp.org/~tml/gimp/win32/downloads.html/">here</A>,
   and install them in the directories <code>ltilib\\win\\gtk+</code>,
   <code>ltilib\\win\\gtk+\\gdk</code> and <code>ltilib\\win\\glib</code>.
   -  Indicate under Project->Settings->C/C++->Preprocessor for all
      configurations the gtk directories. Those are <code>ltilib\\win\\gtk+,
      ltilib\\win\\gtk+\\gdk, ltilib\\win\\glib</code>
   -  under Project->Settings->Link->Input you can specify
      the path to the gtk-*.lib files, e.g. <code>ltilib\\win\\lib</code>
   -  under Project->Settings->Link->General specify the following
      libraries to be linked with:  
      <code>glib-1.3.lib, gdk-1.3.lib, gtk-1.3.lib, gmodule-1.3.lib,
      gthread-1.3.lib</code>
   -  also the following libraries are required 
      <code>ws2_32.lib</code>, <code>psapi.lib</code> and
      <code>pdh.lib</code>, with their respective DLLs.
      The last one (performance data helper) can be found 
      <a href="http://support.microsoft.com/default.aspx?scid=kb;en-us;Q284996">
      here</a>, or search the Microsoft Knowledge Base for article "Q284996".
   -  don't forget to copy the *.dll libraries in your SYSTEM
      directory, or to specify the directory <code>ltilib\\win\\lib</code>
      in your PATH-variable. The required seven GTK+ related
      DLL-Libraries are:
      <code>gdk-1.3.dll, glib-1.3.dll, gmodule-1.3.dll,
      gnu-intl.dll, gthread-1.3.dll, gtk-1.3.dll, pthread.dll</code>.  And
      the <code>pdh.dll</code> is required to.  

   The lti::processInfo class requires for Windows the libraries
   pdh.lib and pdh.dll, which are assumed to be in your default path.
   The lti::url class requires the libraries ws2_32.lib and ws2_32.dll,
   which should be already in your default path.

   Also check the LTI-Lib Tutorial which contains many screenshots of
   the installer for windows.

*/

#endif

