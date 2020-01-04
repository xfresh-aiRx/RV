
# Makefile for the JNI to the LTI library.
# You also need to manually create a link for jniVector.h and
# jniMatrix.h (This should be done automatically).
# For the JNI to work, you have to add the directories in which
# liblti.so and libltiMatrix.so and libltiVector.so reside to
# your LD_LIBRARY_PATH.
# $Id: Makefile,v 1.38 2004/11/22 15:43:02 sltilib Exp $

include config.mk

RELEASE = YES

all: Makefile.jni jniltilib

Makefile.jni: $(LTIBASE)/linux/Makefile.jni
	( cd $(LTIBASE)/linux && make -f Makefile.cvs config ) ; \
        cp $(LTIBASE)/linux/Makefile.jni .

include Makefile.jni

LTIJAVASRC = $(LTISRC)/de/rwth_aachen/techinfo/ltilib

dep: javacode
	$(CXX) -MM $(CXXINCLUDE) -I.. $(JNIINCLUDE) $(JNISRC) > Dependencies

clean:
	rm $(TARGETDIR)/*.so $(TARGETDIR)/*.a *.o jni*.h

clean-all: clean
	rm Makefile.jni

javacode:
	cd $(LTIJAVASRC) && make -k all || true

EXTRAOBJS =
EXTRALIBS = -lpthread

# JNI build targets

TARGETS = libltiInterface.so
STATICTARGETS = libltiInterface.a
TARGETDIR = ../lib

TARGETFILES = $(patsubst %,$(TARGETDIR)/%,$(TARGETS))
STATICTARGETFILES = $(patsubst %,$(TARGETDIR)/%,$(STATICTARGETS))

JNIOBJFILES = $(patsubst %.cpp,%.o,$(JNILIBSRC))

jniltilib: javacode $(JNILIBSRC) $(JNIHEADERS) $(TARGETFILES) $(STATICTARGETFILES)

# compile rule
%.o: %.cpp
	$(CC) -o $@ $<

$(TARGETFILES): $(TARGETDIR)/liblti%.so: $(JNIOBJFILES) mathObject.o libBase.o $(LTILIBFILE)
	$(LN) -o $@ $(JNIOBJFILES) mathObject.o libBase.o $(EXTRAOBJS) -L.. $(EXTRALIBS) -l$(LTILIB) $(LIBS)

# do a static link so clients may include the interface into their own
# shared libraries
$(STATICTARGETFILES): $(TARGETDIR)/liblti%.a: $(JNIOBJFILES) mathObject.o libBase.o $(LTILIBFILE)
	ar rs $@ $(JNIOBJFILES) mathObject.o libBase.o $(EXTRAOBJS)

$(JNILIBSRC): %.cpp: %.h
	checkImpl $*

include Dependencies


debug:
	@echo "LTIBASE = $(LTIBASE)" ; \
        echo "VPATH = $(VPATH)" ; \
        echo "LAPACKLIB = $(LAPACKLIB)" ; \
        echo "LAPACKDIR = $(LAPACKDIR)" ; \
        echo "LAPACKDEFS = $(LAPACKDEFS)" ; \
        echo "CXXINCLUDE = $(CXXINCLUDE)"








