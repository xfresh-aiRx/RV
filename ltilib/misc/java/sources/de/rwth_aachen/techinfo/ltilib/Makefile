
# Makefile for the Java part of the JNI classes to the LTI library.
# You need to have your AXIOMROOT variable set to the root of the
# Axiom source tree.

include config.mk

SUBDIRS = 

LTILIBDIR=$(LTICLS)/de/rwth_aachen/techinfo/ltilib

JNIHEADERS = ./jniMatrix.h ./jniVector.h ./jniLAFunctors.h \
             ./jniIteratingFunctors.h ./jniUnaryFunctors.h \
             ./jniStatFunctors.h ./jniRectLocation.h

JNIHFILES = $(patsubst ./%,$(LTIJNI)/%,$(JNIHEADERS))

debug: 
	@echo JNIHFILES = $(JNIHFILES) ; \
        echo JNIHEADERS = $(JNIHEADERS) ; \
        echo LTISRCFILES = $(LTISRCFILES) ; \
        echo LTISRCDIR = $(LTISRCDIR) ; \
        echo LTICLSFILES = $(LTICLSFILES) 


all: cd $(JNIHFILES) subdirs

$(JNIHFILES): $(LTIJNI)/jni%.h: $(LTICLSDIR)/%.class
	cd $(LTICLSDIR) && $(JAVAH) $@ de.rwth_aachen.techinfo.ltilib.$* && \
        touch $@

cd: $(LTICLSFILES)

# # vector dependencies

# jniVector.h: $(LTILIBDIR)/Vector.class
# 	$(JAVAH) $(LTIJNI)/jniVector.h de.rwth_aachen.techinfo.ltilib.Vector


# # matrix dependencies

# jniMatrix.h: $(LTILIBDIR)/Matrix.class
# 	$(JAVAH)  $(LTIJNI)/jniMatrix.h de.rwth_aachen.techinfo.ltilib.Matrix


# # functor dependencies

# jniFunctors.h: $(LTILIBDIR)/Functors.class
# 	$(JAVAH) $(LTIJNI)/jniFunctors.h de.rwth_aachen.techinfo.ltilib.Functors

# jniLAFunctors.h: $(LTILIBDIR)/LAFunctors.class
# 	$(JAVAH)  $(LTIJNI)/jniLAFunctors.h de.rwth_aachen.techinfo.ltilib.LAFunctors

# jniIteratingFunctors.h: $(LTILIBDIR)/IteratingFunctors.class
# 	$(JAVAH)  $(LTIJNI)/jniIteratingFunctors.h \
#                  de.rwth_aachen.techinfo.ltilib.IteratingFunctors

# jniUnaryFunctors.h: $(LTILIBDIR)/UnaryFunctors.class
# 	$(JAVAH)  $(LTIJNI)/jniUnaryFunctors.h \
#                  de.rwth_aachen.techinfo.ltilib.UnaryFunctors

# test program

test: $(LTILIBDIR)/LtiTest.class
	java -classpath $(LTICLS) de.rwth_aachen.techinfo.ltilib.LtiTest

clean: 
	rm $(LTICLSFILES) 2>/dev/null || true	






