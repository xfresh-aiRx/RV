#----------------------------------------------------------------
# project ....: LTI Digital Image/Signal Processing Library
# file .......: Examples Makefile
# authors ....: Pablo Alvarado, Jochen Wickel
# organization: LTI, RWTH Aachen
# creation ...: 09.02.2003
# revisions ..: $Id: Makefile,v 1.7 2004/07/12 10:49:18 alvarado Exp $
#----------------------------------------------------------------

MAKE=make

all:	
	@for i in */Makefile ; do \
	  a=`dirname $$i` ; \
	  echo "----------------------------------------------------------" ; \
	  echo "Building example `basename $$a`" ; \
	  echo "----------------------------------------------------------" ; \
	  if [ "`basename $$a`" != "template" ]; then \
	    $(MAKE) -C $$a Makefile ; \
	    $(MAKE) -C $$a ; \
	  fi ; \
	  echo " " ; \
	done

clean:
	@for i in */Makefile ; do \
	  a=`dirname $$i` ; \
	  echo "Cleaning $$a" ; \
	  $(MAKE) -s -C $$a clean ; \
	done

clean-all:
	@for i in */Makefile ; do \
	  a=`dirname $$i` ; \
	  echo "Cleaning all $$a" ; \
	  $(MAKE) -s -C $$a clean-all ; \
	done

copymake:
	@for i in */Makefile ; do \
	  a=`dirname $$i` ; \
	  if [ "`basename $$a`" != "template" ]; then \
	    echo "Copy template/Makefile to $$a" ; \
	    $(MAKE) -s -C $$a Makefile ; \
	  fi \
	done

forcecopymake:
	@for i in * ; do \
	  if test -d $$i ; then \
	    if test -f $$i/$$i.cpp ; then \
	      echo "Copy template/Makefile to $$i" ; \
	      cp template/Makefile $$i/Makefile ; \
	    fi \
	  fi \
	done
