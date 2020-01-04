
# Makefile for the JNI to the LTI library.
# You need the variable AXIOMROOT set to where the root of the
# Axiom source tree is. The Java source files are expected in
# $(AXIOMROOT)/java/sources/de/rwth_aachen/techinfo/ltilib
# You also need to manually create a link for jniVector.h and
# jniMatrix.h (This should be done automatically).
# For the JNI to work, you have to add the directories in which
# liblti.so and libltiMatrix.so and libltiVector.so reside to
# your LD_LIBRARY_PATH.

include config.mk

# Names of subdirectories
SUBDIRS = sources jni

ifeq "$(BUILDRELEASE)" "YES"
  LTILIBS=libsr
else
ifeq "$(BUILDRELEASE)" "NO"
  LTILIBS=libsd
else
  LTILIBS=libsr libsd
endif
endif

all: liblti subdirs cd

cd: $(AXCLSFILES)

doc:
	cd docs && make all

dep:
	cd jni && make dep

clean: subdirs

config:
	./configure

newconfig:
	rm config.mk ; ./configure

liblti:
	cd ../../linux && make -k $(LTILIBS)

jar:
	cd classes && jar cvmf ../manifest ../ltilib.jar de LTIlib.properties
