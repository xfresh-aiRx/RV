## Makefile.am for gtk+/gtk

INCLUDES = @STRIP_BEGIN@ \
	-DG_LOG_DOMAIN=\"Gtk\"				\
	-DGTK_DISABLE_COMPAT_H				\
	-DGTK_COMPILATION				\
	-DGTK_EXE_PREFIX=\"$(exec_prefix)\"		\
	-DGTK_DATA_PREFIX=\"$(prefix)\"			\
	-DGTK_SYSCONFDIR=\"$(sysconfdir)\"		\
	-DGTK_LOCALEDIR=\"$(gtklocaledir)\"		\
	-I$(top_srcdir) -I../gdk			\
	-I$(top_srcdir)/gdk				\
	-I$(top_srcdir)/gdk/x11				\
	@GTK_DEBUG_FLAGS@				\
	@GTK_XIM_FLAGS@					\
	@GTK_LOCALE_FLAGS@				\
	@GLIB_CFLAGS@					\
	@x_cflags@					\
@STRIP_END@


#
# libraries to compile and install
#
lib_LTLIBRARIES = libgtk.la

if PLATFORM_WIN32
no_undefined = -no-undefined
endif

if OS_WIN32
gtk_symbols = -export-symbols gtk.def

gtk_win32res_lo = gtk-win32res.lo

gtk-win32res.lo : gtk.rc gtk.def $(libgtk_la_OBJECTS)
	$(top_srcdir)/build/win32/lt-compile-resource gtk.rc gtk-win32res.lo

install-data-local-libtool-import-lib:
	$(INSTALL) .libs/libgtk.dll.a $(DESTDIR)$(libdir)
uninstall-libtool-import-lib:
	-rm $(DESTDIR)$(libdir)/libgtk.dll.a
endif

# libtool stuff: set version and export symbols for resolving
libgtkincludedir = $(includedir)/gtk
libgtk_la_LDFLAGS = @STRIP_BEGIN@ \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)	\
	-export-dynamic						\
	$(no_undefined)						\
	$(gtk_symbols)						\
	@GLIB_LIBS@ 						\
	@x_ldflags@						\
	@x_libs@ 						\
	-lm							\
@STRIP_END@


libgtk_la_LIBADD = $(gtk_win32res_lo) ../gdk/libgdk.la

if MS_LIB_AVAILABLE
noinst_DATA = gtk.lib

install-data-local-ms-lib:
	$(INSTALL) gtk.lib $(DESTDIR)$(libdir)

uninstall-local-ms-lib:
	-rm $(DESTDIR)$(libdir)/gtk.lib
else
install-data-local-ms-lib:
uninstall-local-ms-lib:
endif

#
# setup source file variables
#
# GTK+ header files for public installation (non-generated, or generated
# by configure)

gtk_public_h_sources = @STRIP_BEGIN@ \
	gtk.h			\
	gtkaccelgroup.h		\
	gtkaccellabel.h		\
	gtkadjustment.h		\
	gtkalignment.h		\
	gtkarg.h		\
	gtkarrow.h		\
	gtkaspectframe.h	\
	gtkbin.h		\
	gtkbindings.h		\
	gtkbbox.h		\
	gtkbox.h		\
	gtkbutton.h		\
	gtkcalendar.h		\
	gtkcheckbutton.h	\
	gtkcheckmenuitem.h	\
	gtkclist.h		\
	gtkcolorsel.h		\
	gtkcombo.h		\
	gtkcompat.h		\
	gtkcontainer.h		\
	gtkctree.h		\
	gtkcurve.h		\
	gtkdata.h		\
	gtkdebug.h              \
	gtkdialog.h		\
	gtkdnd.h		\
	gtkdrawingarea.h	\
	gtkeditable.h           \
	gtkentry.h		\
	gtkenums.h		\
	gtkeventbox.h		\
	gtkfeatures.h		\
	gtkfilesel.h		\
	gtkfixed.h		\
	gtkfontsel.h		\
	gtkframe.h		\
	gtkgamma.h		\
	gtkgc.h			\
	gtkhandlebox.h		\
	gtkhbbox.h		\
	gtkhbox.h		\
	gtkhpaned.h		\
	gtkhruler.h		\
	gtkhscale.h		\
	gtkhscrollbar.h		\
	gtkhseparator.h		\
	gtkimage.h		\
	gtkinputdialog.h	\
	gtkinvisible.h		\
	gtkitem.h		\
	gtkitemfactory.h	\
	gtklabel.h		\
	gtklayout.h             \
	gtklist.h		\
	gtklistitem.h		\
	gtkmain.h		\
	gtkmenu.h		\
	gtkmenubar.h		\
	gtkmenufactory.h	\
	gtkmenuitem.h		\
	gtkmenushell.h		\
	gtkmisc.h		\
	gtknotebook.h		\
	gtkobject.h		\
	gtkoptionmenu.h		\
	gtkpacker.h		\
	gtkpaned.h		\
	gtkpixmap.h		\
	gtkplug.h		\
	gtkpreview.h		\
	gtkprivate.h		\
	gtkprogress.h		\
	gtkprogressbar.h	\
	gtkradiobutton.h	\
	gtkradiomenuitem.h	\
	gtkrange.h		\
	gtkrc.h			\
	gtkruler.h		\
	gtkscale.h		\
	gtkscrollbar.h		\
	gtkscrolledwindow.h	\
	gtkselection.h		\
	gtkseparator.h		\
	gtksignal.h		\
	gtksocket.h		\
	gtkspinbutton.h		\
	gtkstyle.h		\
	gtkstatusbar.h		\
	gtktable.h		\
	gtktearoffmenuitem.h    \
	gtktext.h		\
	gtkthemes.h		\
	gtktipsquery.h		\
	gtktogglebutton.h	\
	gtktoolbar.h		\
	gtktooltips.h		\
	gtktree.h		\
	gtktreeitem.h		\
	gtktypeutils.h		\
	gtkvbbox.h		\
	gtkvbox.h		\
	gtkviewport.h		\
	gtkvpaned.h		\
	gtkvruler.h		\
	gtkvscale.h		\
	gtkvscrollbar.h		\
	gtkvseparator.h		\
	gtkwidget.h		\
	gtkwindow.h		\
@STRIP_END@
# GTK+ header files that don't get installed
gtk_private_h_sources = @STRIP_BEGIN@ \
@STRIP_END@
# GTK+ C sources to build the library from
gtk_c_sources = @STRIP_BEGIN@ \
	gtkaccelgroup.c		\
	gtkaccellabel.c		\
	gtkadjustment.c		\
	gtkalignment.c		\
	gtkarg.c		\
	gtkarrow.c		\
	gtkaspectframe.c	\
	gtkbin.c		\
	gtkbindings.c		\
	gtkbbox.c		\
	gtkbox.c		\
	gtkbutton.c		\
	gtkcalendar.c		\
	gtkcheckbutton.c	\
	gtkcheckmenuitem.c	\
	gtkclist.c		\
	gtkcolorsel.c		\
	gtkcombo.c		\
	gtkcontainer.c		\
	gtkctree.c		\
	gtkcurve.c		\
	gtkdata.c		\
	gtkdialog.c		\
	gtkdnd.c		\
	gtkdrawingarea.c	\
	gtkeditable.c           \
	gtkentry.c		\
	gtkeventbox.c		\
	gtkfilesel.c		\
	gtkfixed.c		\
	gtkfontsel.c            \
	gtkframe.c		\
	gtkgamma.c		\
	gtkgc.c			\
	gtkhandlebox.c		\
	gtkhbbox.c		\
	gtkhbox.c		\
	gtkhpaned.c		\
	gtkhruler.c		\
	gtkhscale.c		\
	gtkhscrollbar.c		\
	gtkhseparator.c		\
	gtkimage.c		\
	gtkinputdialog.c	\
	gtkintl.h		\
	gtkinvisible.c		\
	gtkitem.c		\
	gtkitemfactory.c	\
	gtklabel.c		\
	gtklayout.c		\
	gtklist.c		\
	gtklistitem.c		\
	gtkmain.c		\
	gtkmarshal.c		\
	gtkmenu.c		\
	gtkmenubar.c		\
	gtkmenufactory.c	\
	gtkmenuitem.c		\
	gtkmenushell.c		\
	gtkmisc.c		\
	gtknotebook.c		\
	gtkobject.c		\
	gtkoptionmenu.c		\
	gtkpacker.c		\
	gtkpaned.c		\
	gtkpixmap.c		\
	gtkplug.c               \
	gtkpreview.c		\
	gtkprogress.c		\
	gtkprogressbar.c	\
	gtkradiobutton.c	\
	gtkradiomenuitem.c	\
	gtkrange.c		\
	gtkrc.c			\
	gtkruler.c		\
	gtkscale.c		\
	gtkscrollbar.c		\
	gtkscrolledwindow.c	\
	gtkselection.c		\
	gtkseparator.c		\
	gtksignal.c		\
	gtksocket.c             \
	gtkspinbutton.c		\
	gtkstyle.c		\
	gtkstatusbar.c		\
	gtktable.c		\
	gtktearoffmenuitem.c    \
	gtktext.c		\
	gtkthemes.c		\
	gtktipsquery.c		\
	gtktogglebutton.c	\
	gtktoolbar.c		\
	gtktooltips.c		\
	gtktree.c		\
	gtktreeitem.c		\
	gtktypeutils.c		\
	gtkvbbox.c		\
	gtkvbox.c		\
	gtkviewport.c		\
	gtkvpaned.c		\
	gtkvruler.c		\
	gtkvscale.c		\
	gtkvscrollbar.c		\
	gtkvseparator.c		\
	gtkwidget.c		\
	gtkwindow.c		\
	fnmatch.c		\
@STRIP_END@
# we use our own built_sources variable rules to avoid automake's
# BUILT_SOURCES oddities
# we generate frequently rebuild files piggyback on a stamp file, so sources
# depending on them only get rebuild when the built source actually changed
# content
#
# built sources that don't get installed
gtk_built_sources = @STRIP_BEGIN@ \
	stamp-gtk.defs		\
	stamp-gtktypebuiltins.h	\
	stamp-gtkmarshal.h	\
	gtktypebuiltins_vars.c	\
	gtktypebuiltins_ids.c	\
	gtktypebuiltins_evals.c	\
	gtkmarshal.c		\
	gtk.defs		\
@STRIP_END@
# built sources that get installed with the header files
gtk_built_public_sources = @STRIP_BEGIN@ \
	gtkmarshal.h		\
	gtktypebuiltins.h	\
@STRIP_END@
# non-header sources (headers should be specified in the above variables)
# that don't serve as direct make target sources, i.e. they don't have
# their own .lo rules and don't get publically installed
gtk_extra_sources = @STRIP_BEGIN@ \
	gtkcompat.h.in		\
	makeenums.pl		\
	makeenums.awk		\
	maketypes.awk		\
	makeenums.h		\
	gtkargcollector.c	\
	gtk-boxed.defs		\
	genmarshal.pl		\
	gtkmarshal.list		\
@STRIP_END@
# Extra headers that are used for enum type array/id generation
gdk_headers = @STRIP_BEGIN@ 	\
	../gdk/gdkcc.h		\
	../gdk/gdkcolor.h	\
	../gdk/gdkcursor.h     	\
	../gdk/gdkdnd.h		\
	../gdk/gdkdrawable.h	\
	../gdk/gdkevents.h	\
	../gdk/gdkfont.h	\
	../gdk/gdkgc.h		\
	../gdk/gdkim.h		\
	../gdk/gdkimage.h	\
	../gdk/gdkinput.h	\
	../gdk/gdkpixmap.h	\
	../gdk/gdkproperty.h	\
	../gdk/gdkregion.h	\
	../gdk/gdkrgb.h		\
	../gdk/gdkselection.h	\
	../gdk/gdktypes.h	\
	../gdk/gdkvisual.h	\
	../gdk/gdkwindow.h	\
@STRIP_END@

#
# setup GTK+ sources and their dependancies
#
libgtkinclude_HEADERS = $(gtk_public_h_sources) $(gtk_built_public_sources)
libgtk_la_SOURCES = $(gtk_c_sources)
MAINTAINERCLEANFILES = $(gtk_built_public_sources) $(gtk_built_sources)
EXTRA_HEADERS =
EXTRA_DIST = $(gtk_private_h_sources)
EXTRA_DIST += $(gtk_built_sources) $(gtk_built_public_sources) $(gtk_extra_sources)

#
# rules to generate built sources
#
# setup autogeneration dependancies
gen_sources = xgen-gdef xgen-gtbh xgen-gtbvc xgen-gtbic xgen-gtbec xgen-gmh xgen-gmc
CLEANFILES = $(gen_sources)
OLD_STAMP: $(gtk_built_public_sources) $(gtk_built_sources)
$(OBJECTS): OLD_STAMP 	# this is our oldest-source-stamp
# initial creation of the real stamp-* files
gtk.defs gtkmarshal.h gtktypebuiltins.h:          # never add deps here
	test -f "$(srcdir)/$@" || touch $(srcdir)/$@
# normal autogeneration rules
# all autogenerated files need to generated in the srcdir,
# so old versions get remade and are not confused with newer
# versions in the build dir. thus a development setup requires
# srcdir to be writable, passing --disable-rebuilds to
# ../configure will supress all autogeneration rules.
$(srcdir)/stamp-gtk.defs: @REBUILD@ makeenums.pl gtk.defs gtk-boxed.defs $(gtk_public_h_sources) $(gdk_headers)
	cd $(srcdir) \
	&& $(PERL) makeenums.pl defs $(gtk_public_h_sources) $(gdk_headers) > xgen-gdef \
	&& cat gtk-boxed.defs >> xgen-gdef \
	&& (cmp -s xgen-gdef gtk.defs || cp xgen-gdef gtk.defs) \
	&& rm -f xgen-gdef \
	&& echo timestamp > $(@F)
$(srcdir)/gtkmarshal.c: stamp-gtkmarshal.h
$(srcdir)/gtkmarshal.c $(srcdir)/stamp-gtkmarshal.h: @REBUILD@ genmarshal.pl gtkmarshal.list gtkmarshal.h
	cd $(srcdir) \
	&& $(PERL) genmarshal.pl gtkmarshal.list xgen-gmh xgen-gmc \
	&& (test -z "$(INDENT)" || $(INDENT) xgen-gmh) \
	&& (test -z "$(INDENT)" || $(INDENT) xgen-gmc) \
	&& cp xgen-gmc gtkmarshal.c \
	&& (cmp -s xgen-gmh gtkmarshal.h || cp xgen-gmh gtkmarshal.h) \
	&& rm -f xgen-gmh xgen-gmc xgen-gmh~ xgen-gmc~ \
	&& echo timestamp > stamp-gtkmarshal.h
$(srcdir)/stamp-gtktypebuiltins.h: @REBUILD@ maketypes.awk stamp-gtk.defs gtktypebuiltins.h
	cd $(srcdir) \
	&& $(AWK) -f maketypes.awk gtk.defs macros > xgen-gtbh \
	&& (cmp -s xgen-gtbh gtktypebuiltins.h || cp xgen-gtbh gtktypebuiltins.h) \
	&& rm -f xgen-gtbh \
	&& echo timestamp > $(@F)
$(srcdir)/gtktypebuiltins_vars.c: @REBUILD@ maketypes.awk stamp-gtk.defs
	cd $(srcdir) \
	&& $(AWK) -f maketypes.awk gtk.defs variables > xgen-gtbvc \
	&& cp xgen-gtbvc $(@F) \
	&& rm -f xgen-gtbvc
$(srcdir)/gtktypebuiltins_ids.c: @REBUILD@ maketypes.awk stamp-gtk.defs
	cd $(srcdir) \
	&& $(AWK) -f maketypes.awk gtk.defs entries > xgen-gtbic \
	&& cp xgen-gtbic $(@F) \
	&& rm -f xgen-gtbic
$(srcdir)/gtktypebuiltins_evals.c: @REBUILD@ makeenums.pl $(gtk_public_h_sources) $(gdk_headers)
	cd $(srcdir) \
	&& $(PERL) makeenums.pl arrays $(gtk_public_h_sources) $(gdk_headers) > xgen-gtbec \
	&& cp xgen-gtbec $(@F) \
	&& rm -f xgen-gtbec


gtkconfdir = $(sysconfdir)/gtk
gtkconf_DATA = gtkrc.bg gtkrc.el gtkrc.eo gtkrc.he gtkrc.hy gtkrc.ja \
	gtkrc.ko gtkrc.ru gtkrc.tr gtkrc.th gtkrc.uk gtkrc.iso-8859-2 \
	gtkrc.iso-8859-5 gtkrc.iso-8859-13 gtkrc.iso-8859-14 \
	gtkrc.iso-8859-15 gtkrc.zh_CN gtkrc.zh_TW \
	gtkrc.ka_GE.georgianacademy gtkrc.ka_GE.georgianps \
	gtkrc.vi_VN.tcvn gtkrc.vi_VN.viscii

# We create a dummy theme for the default GTK+ theme
install-data-local: install-data-local-ms-lib install-data-local-libtool-import-lib
	$(mkinstalldirs) $(DESTDIR)$(datadir)/themes/Default/gtk
	echo "# Empty gtkrc for default theme" > \
	    $(DESTDIR)$(datadir)/themes/Default/gtk/gtkrc
	$(mkinstalldirs) $(DESTDIR)$(gtkconfdir)
	cd $(DESTDIR)$(gtkconfdir) && \
	  for i in cs hr hu pl ro sk sl sq ; do \
	    rm -f gtkrc.$$i ; \
	    cp gtkrc.iso-8859-2 gtkrc.$$i ; \
	  done ; \
	  for i in bg_BG.iso88595 mk sr ru_RU.iso88595 ; do \
	    rm -f gtkrc.$$i ; \
	    cp gtkrc.iso-8859-5 gtkrc.$$i ; \
	  done ; \
	rm -f gtkrc.lt gtkrc.cy gtkrc.ga gtkrc.et gtkrc.ka \
	     gtkrc.vi_VN.viscii111 gtkrc.vi_VN.tcvn5712 gtkrc.vi ; \
	cp gtkrc.iso-8859-13 gtkrc.lt ; \
	cp gtkrc.iso-8859-14 gtkrc.cy ; \
	cp gtkrc.iso-8859-14 gtkrc.ga ; \
	cp gtkrc.iso-8859-15 gtkrc.et ; \
	cp gtkrc.ka_GE.georgianacademy gtkrc.ka ; \
	cp gtkrc.vi_VN.viscii gtkrc.vi_VN.viscii111 ; \
	cp gtkrc.vi_VN.tcvn gtkrc.vi_VN.tcvn5712 ; \
	cp gtkrc.vi_VN.tcvn5712 gtkrc.vi 

uninstall-local: uninstall-local-ms-lib uninstall-local-libtool-import-lib
	rm -f $(DESTDIR)$(datadir)/themes/Default/gtkrc

gtk.lib: libgtk.la gtk.def
	lib -name:libgtk-@LT_CURRENT_MINUS_AGE@.dll -def:gtk.def -out:$@

#
# test programs, not to be installed
#
noinst_PROGRAMS = testgtk testinput testselection testrgb testdnd simple # testthreads
DEPS = libgtk.la $(top_builddir)/gdk/libgdk.la
LDADDS = @STRIP_BEGIN@ \
	libgtk.la			\
	$(top_builddir)/gdk/libgdk.la	\
	@x_ldflags@			\
	@x_libs@			\
	@GDK_WLIBS@			\
	@GLIB_LIBS@			\
	@GTK_LIBS_EXTRA@		\
	-lm				\
@STRIP_END@
testgtk_DEPENDENCIES = $(DEPS)
testinput_DEPENDENCIES = $(DEPS)
testselection_DEPENDENCIES = $(DEPS)
testrgb_DEPENDENCIES = $(DEPS)
testdnd_DEPENDENCIES = $(DEPS)
simple_DEPENDENCIES = $(DEPS)
#testthreads_DEPENDENCIES = $(DEPS)
testgtk_LDADD = $(LDADDS)
testinput_LDADD = $(LDADDS)
testselection_LDADD = $(LDADDS)
testrgb_LDADD = $(LDADDS)
testdnd_LDADD = $(LDADDS)
simple_LDADD = $(LDADDS)
#testthreads_LDADD = $(LDADDS)

.PHONY: files test test-debug

files:
	@files=`ls $(DISTFILES) 2> /dev/null `; for p in $$files; do \
	  echo $$p; \
	done

test: testgtk
	builddir=`pwd`; cd $(top_builddir); top_builddir=`pwd`; \
	cd $$builddir; cd $(srcdir); \
	$(SHELL) $$top_builddir/libtool --mode=execute $$builddir/testgtk

test-debug: testgtk
	builddir=`pwd`; cd $(top_builddir); top_builddir=`pwd`; \
	cd $$builddir; cd $(srcdir); \
	$(SHELL) $$top_builddir/libtool --mode=execute gdb $$builddir/testgtk

EXTRA_DIST += @STRIP_BEGIN@ 	\
	testgtk.1 		\
	testgtkrc 		\
	testgtkrc2 		\
	circles.xbm		\
	line-arrow.xbm 		\
	line-wrap.xbm 		\
	tree_plus.xbm		\
	tree_minus.xbm          \
	3DRings.xpm		\
	FilesQueue.xpm		\
	Modeller.xpm		\
	check-y.xpm		\
	check-n.xpm		\
	marble.xpm		\
	tree_minus.xpm		\
	tree_plus.xpm		\
	test.xpm		\
	check-y.xpm		\
	check-n.xpm		\
	test.xpm		\
	gtk.def			\
	makefile.mingw		\
	makefile.msc		\
	$(gtkconf_DATA)		\
@STRIP_END@
