AC_DEFUN([AC_CXX_MUTABLE],
[AC_CACHE_CHECK(whether the compiler supports the mutable keyword,
ac_cv_cxx_mutable,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
class A { mutable int i;
          public:
          int f (int n) const { i = n; return i; }
        };
],[A a; return a.f (1);],
 ac_cv_cxx_mutable=yes, ac_cv_cxx_mutable=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_mutable" = yes; then
  AC_DEFINE(HAVE_MUTABLE,,[define if the compiler supports the mutable keyword])
fi
])


AC_DEFUN([AC_CXX_NAMESPACES],
[AC_CACHE_CHECK(whether the compiler implements namespaces,
ac_cv_cxx_namespaces,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([namespace Outer { namespace Inner { int i = 0; }}],
                [using namespace Outer::Inner; return i;],
 ac_cv_cxx_namespaces=yes, ac_cv_cxx_namespaces=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_namespaces" = yes; then
  AC_DEFINE(HAVE_NAMESPACES,,[define if the compiler implements namespaces])
fi
])


AC_DEFUN([AC_CXX_BOOL],
[AC_CACHE_CHECK(whether the compiler recognizes bool as a built-in type,
ac_cv_cxx_bool,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
int f(int  x){return 1;}
int f(char x){return 1;}
int f(bool x){return 1;}
],[bool b = true; return f(b);],
 ac_cv_cxx_bool=yes, ac_cv_cxx_bool=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_bool" = yes; then
  AC_DEFINE(HAVE_BOOL,,[define if bool is a built-in type])
fi
])

AC_DEFUN([AC_CXX_CONST_CAST],
[AC_CACHE_CHECK(whether the compiler supports const_cast<>,
ac_cv_cxx_const_cast,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE(,[int x = 0;const int& y = x;int& z = const_cast<int&>(y);return z;],
 ac_cv_cxx_const_cast=yes, ac_cv_cxx_const_cast=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_const_cast" = yes; then
  AC_DEFINE(HAVE_CONST_CAST,,[define if the compiler supports const_cast<>])
fi
])

AC_DEFUN([AC_CXX_DYNAMIC_CAST],
[AC_CACHE_CHECK(whether the compiler supports dynamic_cast<>,
ac_cv_cxx_dynamic_cast,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <typeinfo>
class Base { public : Base () {} virtual void f () = 0;};
class Derived : public Base { public : Derived () {} virtual void f () {} };],[
Derived d; Base& b=d; return dynamic_cast<Derived*>(&b) ? 0 : 1;],
 ac_cv_cxx_dynamic_cast=yes, ac_cv_cxx_dynamic_cast=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_dynamic_cast" = yes; then
  AC_DEFINE(HAVE_DYNAMIC_CAST,,[define if the compiler supports dynamic_cast<>])
fi
])

AC_DEFUN([AC_CXX_EXCEPTIONS],
[AC_CACHE_CHECK(whether the compiler supports exceptions,
ac_cv_cxx_exceptions,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE(,[try { throw  1; } catch (int i) { return i; }],
 ac_cv_cxx_exceptions=yes, ac_cv_cxx_exceptions=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_exceptions" = yes; then
  AC_DEFINE(HAVE_EXCEPTIONS,,[define if the compiler supports exceptions])
fi
])

AC_DEFUN([AC_CXX_HAVE_STD],
[AC_CACHE_CHECK(whether the compiler supports ISO C++ standard library,
ac_cv_cxx_have_std,
[AC_REQUIRE([AC_CXX_NAMESPACES])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <iostream>
#include <map>
#include <iomanip>
#include <cmath>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[return 0;],
 ac_cv_cxx_have_std=yes, ac_cv_cxx_have_std=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_std" = yes; then
  AC_DEFINE(HAVE_STD,,[define if the compiler supports ISO C++ standard library])
fi
])

AC_DEFUN([AC_CXX_HAVE_STL],
[AC_CACHE_CHECK(whether the compiler supports Standard Template Library,
ac_cv_cxx_have_stl,
[AC_REQUIRE([AC_CXX_NAMESPACES])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <list>
#include <deque>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[list<int> x; x.push_back(5);
list<int>::iterator iter = x.begin(); if (iter != x.end()) ++iter; return 0;],
 ac_cv_cxx_have_stl=yes, ac_cv_cxx_have_stl=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_stl" = yes; then
  AC_DEFINE(HAVE_STL,,[define if the compiler supports Standard Template Library])
fi
])

AC_DEFUN([AC_CXX_STREAMBUF_HAS_FD],
[AC_CACHE_CHECK(whether the std::streambuf has member fd(),
ac_cv_cxx_streambuf_has_fd,
[AC_REQUIRE([AC_CXX_NAMESPACES])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <fstream>
                 #include <iostream>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[ifstream in; in.rdbuf()->fd();],
 ac_cv_cxx_streambuf_has_fd=yes, ac_cv_cxx_streambuf_has_fd=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_streambuf_has_fd" = yes; then
  AC_DEFINE(HAVE_STREAMBUF_FD,,[define if the std::streambuf has member fd()])
fi
])

AC_DEFUN([AC_CXX_IFSTREAM_HAS_FD_CTOR],
[AC_CACHE_CHECK(whether std::ifstream has a constructor taking a file descriptor,
ac_cv_cxx_ifstream_has_fd_ctor,
[AC_REQUIRE([AC_CXX_NAMESPACES])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <fstream>
                 #include <unistd.h>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[int fd=STDIN_FILENO; ifstream in(fd); ],
 ac_cv_cxx_ifstream_has_fd_ctor=yes, ac_cv_cxx_ifstream_has_fd_ctor=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_ifstream_has_fd_ctor" = yes; then
  AC_DEFINE(HAVE_IFSTREAM_FD_CTOR,,[define if std::ifstrean has constructor taking a file descriptor])
fi
])


AC_DEFUN([AC_CXX_RTTI],
[AC_CACHE_CHECK(whether the compiler supports Run-Time Type Identification,
ac_cv_cxx_rtti,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <typeinfo>
class Base { public :
             Base () {}
             virtual int f () { return 0; }
           };
class Derived : public Base { public :
                              Derived () {}
                              virtual int f () { return 1; }
                            };
],[Derived d;
Base *ptr = &d;
return typeid (*ptr) == typeid (Derived);
],
 ac_cv_cxx_rtti=yes, ac_cv_cxx_rtti=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_rtti" = yes; then
  AC_DEFINE(HAVE_RTTI,,
            [define if the compiler supports Run-Time Type Identification])
fi
])

# Modified version from "fortran.m4" to avoid crashing of configure due to
# abscence of fortran compiler.  
# The LTI-Lib can work without Fortran, we just need to disable 
# all Fortran-based features (LAPACK)

# _AC_F77_NAME_MANGLING
# ---------------------
# Test for the name mangling scheme used by the Fortran 77 compiler.
#
# Sets ac_cv_f77_mangling. The value contains three fields, separated
# by commas:
#
# lower case / upper case:
#    case translation of the Fortran 77 symbols
# underscore / no underscore:
#    whether the compiler appends "_" to symbol names
# extra underscore / no extra underscore:
#    whether the compiler appends an extra "_" to symbol names already
#    containing at least one underscore
#
AC_DEFUN([_AC_F77_NAME_MANGLING],
[AC_REQUIRE([AC_F77_LIBRARY_LDFLAGS])dnl
AC_REQUIRE([AC_F77_DUMMY_MAIN])dnl
AC_CACHE_CHECK([for Fortran 77 name-mangling scheme],
               ac_cv_f77_mangling,
[AC_LANG_PUSH(Fortran 77)dnl
AC_COMPILE_IFELSE(
[      subroutine foobar()
      return
      end
      subroutine foo_bar()
      return
      end],
[mv conftest.$ac_objext cf77_test.$ac_objext

  AC_LANG_PUSH(C)dnl

  ac_save_LIBS=$LIBS
  LIBS="cf77_test.$ac_objext $LIBS $FLIBS"

  ac_success=no
  for ac_foobar in foobar FOOBAR; do
    for ac_underscore in "" "_"; do
      ac_func="$ac_foobar$ac_underscore"
      AC_LINK_IFELSE([AC_LANG_CALL([], [$ac_func])],
                     [ac_success=yes; break 2])
    done
  done

  if test "$ac_success" = "yes"; then
     case $ac_foobar in
        foobar)
           ac_case=lower
           ac_foo_bar=foo_bar
           ;;
        FOOBAR)
           ac_case=upper
           ac_foo_bar=FOO_BAR
           ;;
     esac

     ac_success_extra=no
     for ac_extra in "" "_"; do
        ac_func="$ac_foo_bar$ac_underscore$ac_extra"
        AC_LINK_IFELSE([AC_LANG_CALL([], [$ac_func])],
                       [ac_success_extra=yes; break])
     done

     if test "$ac_success_extra" = "yes"; then
	ac_cv_f77_mangling="$ac_case case"
        if test -z "$ac_underscore"; then
           ac_cv_f77_mangling="$ac_cv_f77_mangling, no underscore"
	else
           ac_cv_f77_mangling="$ac_cv_f77_mangling, underscore"
        fi
        if test -z "$ac_extra"; then
           ac_cv_f77_mangling="$ac_cv_f77_mangling, no extra underscore"
	else
           ac_cv_f77_mangling="$ac_cv_f77_mangling, extra underscore"
        fi
      else
	ac_cv_f77_mangling="unknown"
      fi
  else
     ac_cv_f77_mangling="unknown"
  fi

  LIBS=$ac_save_LIBS
  AC_LANG_POP(C)dnl
  rm -f cf77_test* conftest*])],[ac_cv_f77_mangling="unknown"])
AC_LANG_POP(Fortran 77)dnl
])
])# _AC_F77_NAME_MANGLING

AC_DEFUN([ACX_F2C], 
[AC_CACHE_CHECK(whether you have the f2c library and header file,
acx_f2c_ok,
[AC_LANG_SAVE
 AC_LANG_C
 save_LIBS="$LIBS"; LIBS="-lf2c -lm"
 AC_TRY_LINK([extern int e_wsfe(); 
int MAIN__() { e_wsfe(); }],[],
 acx_f2c_ok=yes, acx_f2c_ok=no)
 AC_CHECK_HEADER(f2c.h, acx_f2c_header_ok=yes, acx_f2c_header_ok=no)
 AC_LANG_RESTORE
])
LIBS="$save_LIBS"
if test "x$acx_f2c_ok" = "xyes" -a "x$acx_f2c_header_ok" = "xyes"  ; then
  F2C_LIBS="-lf2c"
else
  F2C_LIBS=""
fi
AC_SUBST(F2C_LIBS)
])


AC_DEFUN([ACX_LAPACK], [

fortran_ok=no

AC_LANG_PUSH(Fortran 77)
AC_COMPILE_IFELSE([AC_LANG_PROGRAM(,[[]])],fortran_ok=yes,fortran_ok=no)
AC_LANG_POP(Fortran 77)

if test "x$fortran_ok" = "xyes" ; then

ACX_F2C

if test "x$acx_f2c_ok" = "xyes" -a "x$acx_f2c_header_ok" = "xyes"  ; then

AC_F77_LIBRARY_LDFLAGS
AC_F77_WRAPPERS
ACX_BLAS
acx_lapack_ok=no

AC_ARG_WITH(lapack,
        [AC_HELP_STRING([--with-lapack=<lib>], [use LAPACK library <lib>])])
case $with_lapack in
        yes | "") ;;
        no) acx_lapack_ok=disable ;;
        -* | */* | *.a | *.so | *.so.* | *.o) LAPACK_LIBS="$with_lapack" ;;
        *) LAPACK_LIBS="-l$with_lapack" ;;
esac

# Get fortran linker name of LAPACK function to check for.
AC_F77_FUNC(cheev)

# We cannot use LAPACK if BLAS is not found
if test "x$acx_blas_ok" != xyes; then
        acx_lapack_ok=noblas
fi

# First, check LAPACK_LIBS environment variable
if test "x$LAPACK_LIBS" != x; then
        save_LIBS="$LIBS"; LIBS="$LAPACK_LIBS $BLAS_LIBS $LIBS $FLIBS"
        AC_MSG_CHECKING([for $cheev in $LAPACK_LIBS])
        AC_TRY_LINK_FUNC($cheev, [acx_lapack_ok=yes], [LAPACK_LIBS=""])
        AC_MSG_RESULT($acx_lapack_ok)
        LIBS="$save_LIBS"
        if test "x$acx_lapack_ok" != yes; then
                LAPACK_LIBS=""
        fi
fi

# LAPACK linked to by default?  (is sometimes included in BLAS lib)
if test $acx_lapack_ok = no; then
        save_LIBS="$LIBS"; LIBS="$LIBS $BLAS_LIBS $FLIBS"
        AC_CHECK_FUNC($cheev, [acx_lapack_ok=yes])
        LIBS="$save_LIBS"
fi

# Generic LAPACK library?
for lapack in lapack lapack_rs6k; do
        if test $acx_lapack_ok = no; then
                save_LIBS="$LIBS"; LIBS="$BLAS_LIBS $LIBS"
                AC_CHECK_LIB($lapack, $cheev,
                    [acx_lapack_ok=yes; LAPACK_LIBS="-l$lapack"], [], [$FLIBS])
                LIBS="$save_LIBS"
        fi
done

if test "x$acx_f2c_ok" != xyes; then
        acx_lapack_ok=nof2c
fi

AC_SUBST(LAPACK_LIBS)

else

fortran_ok=nof2c
LAPACK_LIBS=""
BLAS_LIBS=""
AC_SUBST(LAPACK_LIBS)
AC_SUBST(BLAS_LIBS)

fi

else

LAPACK_LIBS=""
BLAS_LIBS=""
AC_SUBST(LAPACK_LIBS)
AC_SUBST(BLAS_LIBS)

fi

# Finally, execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
if test "x$acx_lapack_ok" = xyes; then
        ifelse([$1],,AC_DEFINE(HAVE_LAPACK,1,[Define if you have LAPACK library.]),[$1])
        :
else
        acx_lapack_ok=no
        $2
fi
])dnl ACX_LAPACK

AC_DEFUN([ACX_BLAS], [
AC_PREREQ(2.50)

AC_F77_LIBRARY_LDFLAGS
acx_blas_ok=no

AC_ARG_WITH(blas,
        [AC_HELP_STRING([--with-blas=<lib>], [use BLAS library <lib>])])
case $with_blas in
        yes | "") ;;
        no) acx_blas_ok=disable ;;
        -* | */* | *.a | *.so | *.so.* | *.o) BLAS_LIBS="$with_blas" ;;
        *) BLAS_LIBS="-l$with_blas" ;;
esac

# Get fortran linker names of BLAS functions to check for.
AC_F77_FUNC(sgemm)
AC_F77_FUNC(dgemm)

acx_blas_save_LIBS="$LIBS"
LIBS="$LIBS $FLIBS"

# First, check BLAS_LIBS environment variable
if test $acx_blas_ok = no; then
if test "x$BLAS_LIBS" != x; then
        save_LIBS="$LIBS"; LIBS="$BLAS_LIBS $LIBS"
        AC_MSG_CHECKING([for $sgemm in $BLAS_LIBS])
        AC_TRY_LINK_FUNC($sgemm, [acx_blas_ok=yes], [BLAS_LIBS=""])
        AC_MSG_RESULT($acx_blas_ok)
        LIBS="$save_LIBS"
fi
fi

# BLAS linked to by default?  (happens on some supercomputers)
if test $acx_blas_ok = no; then
        save_LIBS="$LIBS"; LIBS="$LIBS"
        AC_CHECK_FUNC($sgemm, [acx_blas_ok=yes])
        LIBS="$save_LIBS"
fi

# BLAS in ATLAS library? (http://math-atlas.sourceforge.net/)
if test $acx_blas_ok = no; then
        AC_CHECK_LIB(atlas, ATL_xerbla,
                [AC_CHECK_LIB(f77blas, $sgemm,
                [AC_CHECK_LIB(cblas, cblas_dgemm,
                        [acx_blas_ok=yes
                         BLAS_LIBS="-lcblas -lf77blas -latlas"],
                        [], [-lf77blas -latlas])],
                        [], [-latlas])])
fi

# BLAS in PhiPACK libraries? (requires generic BLAS lib, too)
if test $acx_blas_ok = no; then
        AC_CHECK_LIB(blas, $sgemm,
                [AC_CHECK_LIB(dgemm, $dgemm,
                [AC_CHECK_LIB(sgemm, $sgemm,
                        [acx_blas_ok=yes; BLAS_LIBS="-lsgemm -ldgemm -lblas"],
                        [], [-lblas])],
                        [], [-lblas])])
fi

# BLAS in Alpha CXML library?
if test $acx_blas_ok = no; then
        AC_CHECK_LIB(cxml, $sgemm, [acx_blas_ok=yes;BLAS_LIBS="-lcxml"])
fi

# BLAS in Alpha DXML library? (now called CXML, see above)
if test $acx_blas_ok = no; then
        AC_CHECK_LIB(dxml, $sgemm, [acx_blas_ok=yes;BLAS_LIBS="-ldxml"])
fi

# BLAS in Sun Performance library?
if test $acx_blas_ok = no; then
        if test "x$GCC" != xyes; then # only works with Sun CC
                AC_CHECK_LIB(sunmath, acosp,
                        [AC_CHECK_LIB(sunperf, $sgemm,
                                [BLAS_LIBS="-xlic_lib=sunperf -lsunmath"
                                 acx_blas_ok=yes],[],[-lsunmath])])
        fi
fi

# BLAS in SCSL library?  (SGI/Cray Scientific Library)
if test $acx_blas_ok = no; then
        AC_CHECK_LIB(scs, $sgemm, [acx_blas_ok=yes; BLAS_LIBS="-lscs"])
fi

# BLAS in SGIMATH library?
if test $acx_blas_ok = no; then
        AC_CHECK_LIB(complib.sgimath, $sgemm,
                     [acx_blas_ok=yes; BLAS_LIBS="-lcomplib.sgimath"])
fi

# BLAS in IBM ESSL library? (requires generic BLAS lib, too)
if test $acx_blas_ok = no; then
        AC_CHECK_LIB(blas, $sgemm,
                [AC_CHECK_LIB(essl, $sgemm,
                        [acx_blas_ok=yes; BLAS_LIBS="-lessl -lblas"],
                        [], [-lblas $FLIBS])])
fi

# Generic BLAS library?
if test $acx_blas_ok = no; then
        AC_CHECK_LIB(blas, $sgemm, [acx_blas_ok=yes; BLAS_LIBS="-lblas"])
fi

AC_SUBST(BLAS_LIBS)

LIBS="$acx_blas_save_LIBS"

# Finally, execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
if test x"$acx_blas_ok" = xyes; then
        ifelse([$1],,AC_DEFINE(HAVE_BLAS,1,[Define if you have a BLAS library.]),[$1])
        :
else
        acx_blas_ok=no
        $2
fi
])dnl ACX_BLAS

# Configure paths for GTK+
# Owen Taylor     97-11-3

dnl AM_PATH_GTK([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND [, MODULES]]]])
dnl Test for GTK, and define GTK_CFLAGS and GTK_LIBS
dnl
AC_DEFUN([AM_PATH_GTK],
[dnl 
dnl Get the cflags and libraries from the gtk-config script
dnl
AC_ARG_WITH(gtk,[  --without-gtk           Disable use of GTK+ Widget Library ],
        disable_gtk=yes,disable_gtk=no)
AC_ARG_WITH(gtk-prefix,[  --with-gtk-prefix=PFX   Prefix where GTK is installed (optional)],
            gtk_config_prefix="$withval", gtk_config_prefix="")
AC_ARG_WITH(gtk-exec-prefix,[  --with-gtk-exec-prefix=PFX Exec prefix where GTK is installed (optional)],
            gtk_config_exec_prefix="$withval", gtk_config_exec_prefix="")
AC_ARG_ENABLE(gtktest, [  --disable-gtktest       Do not try to compile and run a test GTK program],
		    , enable_gtktest=yes)

  for module in . $4
  do
      case "$module" in
         gthread) 
             gtk_config_args="$gtk_config_args gthread"
         ;;
      esac
  done

  if test x$gtk_config_exec_prefix != x ; then
     gtk_config_args="$gtk_config_args --exec-prefix=$gtk_config_exec_prefix"
     if test x${GTK_CONFIG+set} != xset ; then
        GTK_CONFIG=$gtk_config_exec_prefix/bin/gtk-config
     fi
  fi
  if test x$gtk_config_prefix != x ; then
     gtk_config_args="$gtk_config_args --prefix=$gtk_config_prefix"
     if test x${GTK_CONFIG+set} != xset ; then
        GTK_CONFIG=$gtk_config_prefix/bin/gtk-config
     fi
  fi

  AC_PATH_PROG(GTK_CONFIG, gtk-config, no)
  min_gtk_version=ifelse([$1], ,0.99.7,$1)
  AC_MSG_CHECKING(for GTK - version >= $min_gtk_version)
  AC_SUBST(gtk_lib)
  gtk_lib=no
  no_gtk=""
  if test "x$disable_gtk" = "xyes" ; then
    no_gtk=yes
  elif test "$GTK_CONFIG" = "no" ; then
    no_gtk=yes
  else
    GTK_CFLAGS=`$GTK_CONFIG $gtk_config_args --cflags`
    GTK_LIBS=`$GTK_CONFIG $gtk_config_args --libs`
    gtk_config_major_version=`$GTK_CONFIG $gtk_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    gtk_config_minor_version=`$GTK_CONFIG $gtk_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    gtk_config_micro_version=`$GTK_CONFIG $gtk_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_gtktest" = "xyes" ; then
      ac_save_CXXFLAGS="$CXXFLAGS"
      ac_save_LIBS="$LIBS"
      CXXFLAGS="$CFLAGS $GTK_CFLAGS"
      LIBS="$GTK_LIBS $LIBS"
dnl
dnl Now check if the installed GTK is sufficiently new. (Also sanity
dnl checks the results of gtk-config to some extent
dnl
      rm -f conf.gtktest
      AC_TRY_RUN([
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

int 
main ()
{
  int major, minor, micro;
  char *tmp_version;

  system ("touch conf.gtktest");

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = g_strdup("$min_gtk_version");
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
     printf("%s, bad version string\n", "$min_gtk_version");
     exit(1);
   }

  if ((gtk_major_version != $gtk_config_major_version) ||
      (gtk_minor_version != $gtk_config_minor_version) ||
      (gtk_micro_version != $gtk_config_micro_version))
    {
      printf("\n*** 'gtk-config --version' returned %d.%d.%d, but GTK+ (%d.%d.%d)\n", 
             $gtk_config_major_version, $gtk_config_minor_version, $gtk_config_micro_version,
             gtk_major_version, gtk_minor_version, gtk_micro_version);
      printf ("*** was found! If gtk-config was correct, then it is best\n");
      printf ("*** to remove the old version of GTK+. You may also be able to fix the error\n");
      printf("*** by modifying your LD_LIBRARY_PATH enviroment variable, or by editing\n");
      printf("*** /etc/ld.so.conf. Make sure you have run ldconfig if that is\n");
      printf("*** required on your system.\n");
      printf("*** If gtk-config was wrong, set the environment variable GTK_CONFIG\n");
      printf("*** to point to the correct copy of gtk-config, and remove the file config.cache\n");
      printf("*** before re-running configure\n");
    } 
#if defined (GTK_MAJOR_VERSION) && defined (GTK_MINOR_VERSION) && defined (GTK_MICRO_VERSION)
  else if ((gtk_major_version != GTK_MAJOR_VERSION) ||
	   (gtk_minor_version != GTK_MINOR_VERSION) ||
           (gtk_micro_version != GTK_MICRO_VERSION))
    {
      printf("*** GTK+ header files (version %d.%d.%d) do not match\n",
	     GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION);
      printf("*** library (version %d.%d.%d)\n",
	     gtk_major_version, gtk_minor_version, gtk_micro_version);
    }
#endif /* defined (GTK_MAJOR_VERSION) ... */
  else
    {
      if ((gtk_major_version > major) ||
        ((gtk_major_version == major) && (gtk_minor_version > minor)) ||
        ((gtk_major_version == major) && (gtk_minor_version == minor) && (gtk_micro_version >= micro)))
      {
        return 0;
       }
     else
      {
        printf("\n*** An old version of GTK+ (%d.%d.%d) was found.\n",
               gtk_major_version, gtk_minor_version, gtk_micro_version);
        printf("*** You need a version of GTK+ newer than %d.%d.%d. The latest version of\n",
	       major, minor, micro);
        printf("*** GTK+ is always available from ftp://ftp.gtk.org.\n");
        printf("***\n");
        printf("*** If you have already installed a sufficiently new version, this error\n");
        printf("*** probably means that the wrong copy of the gtk-config shell script is\n");
        printf("*** being found. The easiest way to fix this is to remove the old version\n");
        printf("*** of GTK+, but you can also set the GTK_CONFIG environment to point to the\n");
        printf("*** correct copy of gtk-config. (In this case, you will have to\n");
        printf("*** modify your LD_LIBRARY_PATH enviroment variable, or edit /etc/ld.so.conf\n");
        printf("*** so that the correct libraries are found at run-time))\n");
      }
    }
  return 1;
}
],, no_gtk=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CXXFLAGS="$ac_save_CXXFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi
  if test "x$no_gtk" = x ; then
     AC_MSG_RESULT(yes)
     gtk_lib=yes
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "x$disable_gtk" = "xyes" ; then
       echo "You requested to disable GTK+."
     elif test "$GTK_CONFIG" = "no" ; then
       echo "*** The gtk-config script installed by GTK could not be found"
       echo "*** If GTK was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the GTK_CONFIG environment variable to the"
       echo "*** full path to gtk-config."
     else
       if test -f conf.gtktest ; then
        :
       else
          echo "*** Could not run GTK test program, checking why..."
          CFLAGS="$CFLAGS $GTK_CFLAGS"
          LIBS="$LIBS $GTK_LIBS"
          AC_TRY_LINK([
#include <gtk/gtk.h>
#include <stdio.h>
],      [ return ((gtk_major_version) || (gtk_minor_version) || (gtk_micro_version)); ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding GTK or finding the wrong"
          echo "*** version of GTK. If it is not finding GTK, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"
          echo "***"
          echo "*** If you have a RedHat 5.0 system, you should remove the GTK package that"
          echo "*** came with the system with the command"
          echo "***"
          echo "***    rpm --erase --nodeps gtk gtk-devel" ],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means GTK was incorrectly installed"
          echo "*** or that you have moved GTK since it was installed. In the latter case, you"
          echo "*** may want to edit the gtk-config script: $GTK_CONFIG" ])
          CFLAGS="$ac_save_CFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     GTK_CFLAGS=""
     GTK_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(GTK_CFLAGS)
  AC_SUBST(GTK_LIBS)
  AC_SUBST(gtk_lib)
  rm -f conf.gtktest
])

## Portability defines that help interoperate with classic and modern autoconfs
ifdef([AC_TR_SH],[
define([AC_TR_SH_REUSE],[AC_TR_SH([$1])])
define([AC_TR_CPP_REUSE],[AC_TR_CPP([$1])])
], [
define([AC_TR_SH_REUSE],
       [patsubst(translit([[$1]], [*+], [pp]), [[^a-zA-Z0-9_]], [_])])
define([AC_TR_CPP_REUSE],
       [patsubst(translit([[$1]],
                          [*abcdefghijklmnopqrstuvwxyz],
                          [PABCDEFGHIJKLMNOPQRSTUVWXYZ]),
                 [[^A-Z0-9_]], [_])])
])

AC_DEFUN([AC_C_COMPILE_VALUE], [
  pushdef([ac_c_compile_value],
    AC_TR_SH_REUSE([ac_cv_c_compile_value_$2]))dnl
  ac_c_compile_value_expand="$1"
  AC_CACHE_CHECK([value of $1 by analyzing object code],
                 ac_c_compile_value, [
    save_CFLAGS="$CFLAGS"
    CFLAGS="$CFLAGS -c -o conftest.o"
    AC_TRY_COMPILE([$3
      #include <stddef.h>
      #include <stdint.h>
      #include <stdlib.h>
      #define COMPILE_VALUE $ac_c_compile_value_expand
      #define HEX_DIGIT(n)      ((n) >= 10 ? 'a' + (n) - 10 : '0' + (n))
      char object_code_block[] = {
        '\n', 'e', '4', 'V', 'A',
        '0', 'x',
        (char) HEX_DIGIT((((COMPILE_VALUE / 16) / 16) / 16) % 16),
        (char) HEX_DIGIT(((COMPILE_VALUE / 16) / 16) % 16),
        (char) HEX_DIGIT((COMPILE_VALUE / 16) % 16),
        (char) HEX_DIGIT(COMPILE_VALUE % 16),
        'Y', '3', 'p', 'M', '\n'
      };],
      [],
      [ac_c_compile_value=`
        typeset -i n=\`sed -ne 's/^e4VA0x\(.*\)Y3pM$/0x\1/p' < conftest.o\`;
        echo $n`],
      [ac_c_compile_value=0])
    CFLAGS="$save_CFLAGS"])
  AC_DEFINE_UNQUOTED(AC_TR_CPP_REUSE(compile_value_$2),
                     [$[]ac_c_compile_value],
                     [$1])
  popdef([ac_c_compile_value])dnl
])

AC_DEFUN(
        [AC_ENSURE_GNU_MAKE], [ AC_CACHE_CHECK( for GNU make,_cv_gnu_make_command,
                _cv_gnu_make_command='' ;
dnl Search all the common names for GNU make
                for a in "$MAKE" make gmake gnumake ; do
                        if test -z "$a" ; then continue ; fi ;
                        if  ( sh -c "$a --version" 2> /dev/null | grep GNU  2>&1 > /dev/null ) ;  then
                                _cv_gnu_make_command=$a ;
                                break;
                        fi
                done ;
        ) ;
dnl If there was a GNU version, then set @ifGNUmake@ to the empty string, '#' otherwise
        if test  "x$_cv_gnu_make_command" != "x"  ; then
                ifGNUmake='' ;
        else
                ifGNUmake='#' ;
                AC_MSG_ERROR("GNU make is required but not found.");
        fi
        AC_SUBST(ifGNUmake)
] )



AC_DEFUN([AC_LIB_VERSIONS_CHECK],[
        AC_ARG_ENABLE(debug,[  --disable-debug         Disable the compilation of the debug libraries ],
        enable_debug=$enableval,enable_debug=yes)
        AC_ARG_ENABLE(release,[  --disable-release       Disable the compilation of the release libraries ],
        enable_release=$enableval,enable_release=yes)

        if test "x$enable_debug" = "xyes" ; then
          LIB_DEBUG=all-debug
        else
          LIB_DEBUG=
        fi
        AC_SUBST(LIB_DEBUG)

        if test "x$enable_release" = "xyes" ; then
          LIB_RELEASE=all-release
        else
          LIB_RELEASE=
        fi
        AC_SUBST(LIB_RELEASE)
])

dnl --------------------------------------------------------------------------
dnl Processor check
dnl --------------------------------------------------------------------------
AC_DEFUN([AC_PROCESSOR_CHECK],[

        dnl Get minor and major versions of the compiler used

        if test "x$CXX" = "x" ; then
          CXX=g++;
        fi

        _ac_cxx_major=`$CXX --version | sed -e 's/[[^0-9]]*\([[0-9]][[0-9]]*\)\..*/\1/;' | egrep '^[[0-9]]+$' `

        _ac_cxx_minor=`$CXX --version | sed -e 's/[[^0-9]]*[[0-9]]*\.\([[0-9]][[0-9]]*\)\..*/\1/;' | egrep '^[[0-9]]+$' `

        dnl -----------------------------------------------------------
        dnl | Try to guess which processor is being used, by means of |
        dnl | /proc/cpuinfo                                           |
        dnl -----------------------------------------------------------
        
        if test -d /proc; then
          AC_DEFINE(HAVE_PROC,1,
             [define if the unix/linux system has a /proc directory structure])

          dnl ----------------------------------------------------------------
          dnl | For Intel/AMD processors a line "model name" is defined with |
          dnl | a string identifying the CPU, but PowerPC (Mac) doesn't do   |
          dnl | that, so that we need some complex checking here.
          dnl ----------------------------------------------------------------
        
          AC_MSG_CHECKING( for local CPU)

	  dnl some old grep versions do not understand -m1
	  _ac_grep_opt=-m1
	  if ! grep --help | grep -q " \-m" ; then
	    _ac_grep_opt=""
	  fi

          _ac_cpu_vendor=`cat /proc/cpuinfo | grep $_ac_grep_opt "vendor_id" | sed -e 's/.*: \(.*\)/\1/;'`

          if echo $_ac_cpu_vendor | fgrep -q "Intel" || echo $_ac_cpu_vendor | fgrep -q "AMD" ; then
            dnl Mainstream: Intel or AMD processors expected

            _ac_arch_gcc2="-march=i686"
            _ac_arch_gcc3x="-march=i686"
            _ac_arch_valgrind="-march=i586"

            _ac_cpu_orig=`cat /proc/cpuinfo | grep $_ac_grep_opt "model name" | sed -e 's/.*: \(.*\)/\1/;'`

            if echo $_ac_cpu_orig | fgrep -q "Pentium III" ; then
              _ac_arch_gcc3x="-march=pentium3"
            elif echo $_ac_cpu_orig | fgrep -q "Pentium(R) 4" ; then
              _ac_arch_gcc3x="-march=pentium4"
            elif echo $_ac_cpu_orig | fgrep -q "Pentium" ; then
              _ac_arch_gcc2="-march=i586"
              _ac_arch_gcc3x="-march=i586"
            elif echo $_ac_cpu_orig | fgrep -q "Athlon(tm) 64" ; then
              if test "x$_ac_cxx_major" = "x3" ; then
                if test $_ac_cxx_minor -ge 4 ; then
                  _ac_arch_gcc3x="-march=k8"
                else
                  _ac_arch_gcc3x="-march=athlon-xp -m64"
                fi
              fi             
            elif echo $_ac_cpu_orig | fgrep -q "Athlon(TM) XP" ; then
              _ac_arch_gcc3x="-march=athlon-xp"
            elif echo $_ac_cpu_orig | fgrep -q "Athlon" ; then
              _ac_arch_gcc3x="-march=athlon"
            elif echo $_ac_cpu_orig | fgrep -q "K6" ; then
              _ac_arch_gcc2="-march=i586"
              _ac_arch_gcc3x="-march=k6"
            fi

            AC_MSG_RESULT($_ac_cpu_orig ($_ac_cpu_vendor))

            AC_CACHE_CHECK( for Intel MMX,_ac_mmx, 
            [if ( sh -c "grep mmx /proc/cpuinfo" 2>&1 > /dev/null ) ; then
              _ac_mmx=yes            
            else 
              _ac_mmx=no
            fi])
            if test "$_ac_mmx" = yes; then
              AC_DEFINE(HAVE_MMX,1,[define if the processor has intel mmx])
            fi

            AC_CACHE_CHECK( for Intel SSE,_ac_sse,
            [if ( sh -c "grep sse /proc/cpuinfo" 2>&1 > /dev/null ) ; then
              _ac_sse=yes            
            else 
              _ac_sse=no
            fi])
            if test "$_ac_sse" = yes; then
              AC_DEFINE(HAVE_SSE,1,[define if processor has intel SSE])  
            fi
  
            AC_CACHE_CHECK( for Intel SSE2,_ac_sse2,
            [if ( sh -c "grep sse2 /proc/cpuinfo" 2>&1 > /dev/null ) ; then
              _ac_sse2=yes
            else 
              _ac_sse2=no
            fi])
            if test "$_ac_sse2" = yes; then
              AC_DEFINE(HAVE_SSE2,1,[define if processor has intel SSE2])
            fi

            AC_CACHE_CHECK( for AMD 3D-Now,_ac_3dnow,
            [if ( sh -c "grep 3dnow /proc/cpuinfo" 2>&1 > /dev/null ) ; then
              _ac_3dnow=yes            
            else 
              _ac_3dnow=no
            fi])
            if test "$_ac_3dnow" = yes; then
              AC_DEFINE(HAVE_3DNOW,1,[define if processor has AMD 3DNow])
            fi

          else
            dnl --------------------------
            dnl No Intel or AMD Processor!
            dnl --------------------------

            _ac_arch_gcc2=""
            _ac_arch_gcc3x=""
            _ac_arch_valgrind=""

            dnl if no vendor_id was found, it is possibly a Mac/PowerPC or a
            dnl Sparc processor.

            _ac_cpu_orig=`cat /proc/cpuinfo | grep $_ac_grep_opt "^cpu" | sed -e 's/.*: \(.*\)/\1/;'`

            if echo $_ac_cpu_orig | grep -q "^740." ; then
              _ac_arch_gcc2="-mcpu=common"
              _ac_arch_gcc3x="-mcpu=7400"
            elif echo $_ac_cpu_orig | grep -q "^74." ; then
              _ac_arch_gcc2="-mcpu=common"
              _ac_arch_gcc3x="-mcpu=740"
            elif echo $_ac_cpu_orig | grep -q "^745." ; then
              _ac_arch_gcc2="-mcpu=common"
              _ac_arch_gcc3x="-mcpu=7450"
            elif echo $_ac_cpu_orig | grep -q "^75." ; then
              _ac_arch_gcc2="-mcpu=common"
              _ac_arch_gcc3x="-mcpu=750"
            elif echo $_ac_cpu_orig | grep -q "^601" ; then
              _ac_arch_gcc2="-mcpu=601"
              _ac_arch_gcc3x="-mcpu=601"
            elif echo $_ac_cpu_orig | grep -q "^602" ; then
              _ac_arch_gcc2="-mcpu=602"
              _ac_arch_gcc3x="-mcpu=602"
            elif echo $_ac_cpu_orig | grep -q "^603e" ; then
              _ac_arch_gcc2="-mcpu=603e"
              _ac_arch_gcc3x="-mcpu=603e"
            elif echo $_ac_cpu_orig | grep -q "^603" ; then
              _ac_arch_gcc2="-mcpu=603"
              _ac_arch_gcc3x="-mcpu=603"
            elif echo $_ac_cpu_orig | grep -q "^604e" ; then
              _ac_arch_gcc2="-mcpu=604e"
              _ac_arch_gcc3x="-mcpu=604e"
            elif echo $_ac_cpu_orig | grep -q "^62." ; then
              _ac_arch_gcc2="-mcpu=620"
              _ac_arch_gcc3x="-mcpu=620"
            elif echo $_ac_cpu_orig | grep -q "^63." ; then
              _ac_arch_gcc2="-mcpu=630"
              _ac_arch_gcc3x="-mcpu=630"
            elif echo $_ac_cpu_orig | grep -q "^403" ; then
              _ac_arch_gcc2="-mcpu=403"
              _ac_arch_gcc3x="-mcpu=403"
            elif echo $_ac_cpu_orig | grep -q "^505" ; then
              _ac_arch_gcc2="-mcpu=505"
              _ac_arch_gcc3x="-mcpu=505"
            elif echo $_ac_cpu_orig | grep -q "^801" ; then
              _ac_arch_gcc2="-mcpu=801"
              _ac_arch_gcc3x="-mcpu=801"
            elif echo $_ac_cpu_orig | grep -q "^821" ; then
              _ac_arch_gcc2="-mcpu=821"
              _ac_arch_gcc3x="-mcpu=821"
            elif echo $_ac_cpu_orig | grep -q "^823" ; then
              _ac_arch_gcc2="-mcpu=823"
              _ac_arch_gcc3x="-mcpu=823"
            elif echo $_ac_cpu_orig | grep -q "^86." ; then
              _ac_arch_gcc2="-mcpu=860"
              _ac_arch_gcc3x="-mcpu=860"
            fi

            AC_MSG_RESULT($_ac_cpu_orig ($_ac_cpu_vendor))


            AC_CACHE_CHECK( for PowerPC Altivec,_ac_altivec,
            [if ( sh -c "grep -q altivec /proc/cpuinfo" ) ; then
              _ac_altivec=yes            
            else 
              _ac_altivec=no
            fi])
            if test "$_ac_altivec" = yes; then
              AC_DEFINE(HAVE_ALTIVEC,1,[define if processor has Altivec])  
              _ac_arch_gcc3x="$_ac_arch_gcc3x -maltivec -mabi=altivec"
            fi

            if test "x$_ac_cpu_orig" == x ; then
              echo
              echo "Unknown processor in /proc/cpuinfo"
              echo "Please send the output of '/proc/cpuinfo', 'uname -m' and"
              echo "'uname -p' to one of the mailing lists or forums in"
              echo "the LTI-Lib homepage: http://ltilib.sourceforge.net."
              echo
              echo "As workaround you can define the environment variable"
              echo "CXXFLAGS to contain the proper values."
            fi

            _ac_cpu_vendor=`uname -m`
            
          fi 

          AC_DEFINE_UNQUOTED(PROCESSOR,$_ac_cpu_orig,[define processor type])
          AC_DEFINE_UNQUOTED(PROCESSOR_VENDOR,$_ac_cpu_vendor,
                             [define processor vendor])

          AC_ARG_WITH(valgrind,
                      [AC_HELP_STRING([--with-valgrind],
                      [compile for valgrind])],
                      [_ac_archd_gcc2=$_ac_arch_valgrind
                       _ac_archd_gcc3x=$_ac_arch_valgrind],
                      [_ac_archd_gcc2=$_ac_arch_gcc2
                       _ac_archd_gcc3x=$_ac_arch_gcc3x])

          dnl maybe the user indicates directly what he wants!
          if echo "x$CXXFLAGS" | fgrep -q "march=" ; then
            _ac_arch_gcc3x=`echo $CXXFLAGS | sed -e 's/.*march=\([[^ ]]*\).*/-march=\1/;' `
            _ac_archd_gcc3x=$_ac_arch_gcc3x
            _ac_arch_gcc2=$_ac_arch_gcc3x
            _ac_archd_gcc2=$_ac_arch_gcc2
          fi

          _ac_cpuarch_gcc2="$_ac_arch_gcc2"
          _ac_cpuarch_gcc3x="$_ac_arch_gcc3x"
          _ac_cpuarchd_gcc2="$_ac_archd_gcc2"
          _ac_cpuarchd_gcc3x="$_ac_archd_gcc3x"

          AC_SUBST(_ac_cxx_major)
          AC_SUBST(_ac_cxx_minor)
          AC_SUBST(_ac_cpuarch_gcc2)
          AC_SUBST(_ac_cpuarch_gcc3x)
          AC_SUBST(_ac_cpuarchd_gcc2)
          AC_SUBST(_ac_cpuarchd_gcc3x)

        else

          dnl no /proc/ found
          AC_DEFINE(PROCESSOR,["unknown"],[define processor type])
          AC_DEFINE(PROCESSOR_VENDOR,["unknown"],[define processor vendor])
        
          _ac_cpuarch_gcc2=""
          _ac_cpuarch_gcc3x=""
          _ac_cpuarchd_gcc2=""
          _ac_cpuarchd_gcc3x=""

          AC_SUBST(_ac_cxx_major)
          AC_SUBST(_ac_cxx_minor)
          AC_SUBST(_ac_cpuarch_gcc2)
          AC_SUBST(_ac_cpuarch_gcc3x)
          AC_SUBST(_ac_cpuarchd_gcc2)
          AC_SUBST(_ac_cpuarchd_gcc3x)

        fi
])

AC_DEFUN([AC_IMAGE_LIBS],
[if echo $LIBS | fgrep "jpeg" ; then
   JPEGLIB=1
 else
   JPEGLIB=
 fi
 if echo $LIBS | fgrep "png" ; then
   PNGLIB=1
 else
   PNGLIB=
 fi
 AC_SUBST(JPEGLIB)
 AC_SUBST(PNGLIB)
])

AC_DEFUN([AC_GET_PLATFORM_OPTS],
[if ( uname -s | fgrep "CYGWIN" ) ; then
   OSCXXOPTS=
 else
   OSCXXOPTS=-fpic
 fi
 AC_SUBST(OSCXXOPTS)
])

AC_DEFUN([AC_TYPE_SIZE_CHECK],
   [AC_MSG_CHECKING([for size of types])
   AC_MSG_RESULT([])
    AC_TRY_RUN([#include <stdio.h>
int main() {  
  /* Check for size of fixed point types in bytes under following assumtions:
   *
   * sizeof(char)=1
   * sizeof(char) <= sizeof(short) <= sizeof(int) <= sizeof(long)
   * sizeof(unsigned xyz) = sizeof(signed xyz)
   */ 

  unsigned char  uchar;

  int i;
  int sizeofchar;
  int sizeofshort;
  int sizeofint;
  int sizeoflong;
  int sizeoflonglong;
  char sizes[128];

  for (i=0;i<128;++i) {
    sizes[i]=0;
  }

  /*
   * check for size of char
   */
  for (i=0,uchar=1;uchar!=0;uchar=uchar<<1,++i);
  
  /*
   * size of types in bytes
   */
  sizeofchar=i/8;  
  sizeofshort=sizeof(unsigned short)*i/8;
  sizeofint=sizeof(unsigned int)*i/8;
  sizeoflong=sizeof(unsigned long)*i/8;
  sizeoflonglong=sizeof(unsigned long long)*i/8;

  sizes[sizeoflonglong] = 'm';
  sizes[sizeoflong]     = 'l';
  sizes[sizeofint]      = 'i';
  sizes[sizeofshort]    = 's';
  sizes[sizeofchar]     = 'c';

  if (sizes[1] == 0) {
    exit(1);
  } else {
    switch(sizes[1]) {
      case 'c':
        printf("BYTE char\n");       
        break;
      case 's':
        printf("BYTE short int\n");
        break;
      case 'i':
        printf("BYTE int\n");
        break;
      case 'l':
        printf("BYTE long int\n");
        break;
    }
  }

  if (sizes[2] == 0) {
    exit(1);
  } else {
    switch(sizes[2]) {
      case 'c':
        printf("WORD16 char\n");       
        break;
      case 's':
        printf("WORD16 short int\n");
        break;
      case 'i':
        printf("WORD16 int\n");
        break;
      case 'l':
        printf("WORD16 long int\n");
        break;
    }
  }

  if (sizes[4] == 0) {
    exit(1);
  } else {
    switch(sizes[4]) {
      case 'c':
        printf("WORD32 char\n");       
        break;
      case 's':
        printf("WORD32 short int\n");
        break;
      case 'i':
        printf("WORD32 int\n");
        break;
      case 'l':
        printf("WORD32 long int\n");
        break;
    }
  }

  if (sizes[8] == 0) {
    exit(1);
  } else {
    switch(sizes[8]) {
      case 'c':
        printf("WORD64 char\n");       
        break;
      case 's':
        printf("WORD64 short int\n");
        break;
      case 'i':
        printf("WORD64 int\n");
        break;
      case 'l':
        printf("WORD64 long int\n");
        break;
      case 'm':
        printf("WORD64 long long\n");
        break;
    }
  }

  return 0;}
],
  [ac_try=./conftest$ac_exeext
   _ac_byte=`$ac_try | grep BYTE | sed -e 's/BYTE \(.*\)/\1/;'`
   _ac_word16=`$ac_try | grep WORD16 | sed -e 's/WORD16 \(.*\)/\1/;'`
   _ac_word32=`$ac_try | grep WORD32 | sed -e 's/WORD32 \(.*\)/\1/;'`
   _ac_word64=`$ac_try | grep WORD64 | sed -e 's/WORD64 \(.*\)/\1/;'`
   AC_DEFINE_UNQUOTED(_LTI_BYTE,`echo $_ac_byte`,define type for byte)
   AC_DEFINE_UNQUOTED(_LTI_WORD16,`echo $_ac_word16`,define type for two-byte)
   AC_DEFINE_UNQUOTED(_LTI_WORD32,`echo $_ac_word32`,define type for four-bytes)
   AC_DEFINE_UNQUOTED(_LTI_WORD64,`echo $_ac_word64`,define type for eight-bytes)],
  [AC_DEFINE(_LTI_BYTE,char,define type for byte)
   AC_DEFINE(_LTI_WORD16,short int,define type for two-bytes)
   AC_DEFINE(_LTI_WORD32,int,define type for four-bytes)
   AC_DEFINE(_LTI_WORD64,long long,define type for eight-bytes)
   AC_MSG_RESULT([failed])])
   ])
