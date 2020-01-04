
include config.mk

# Names of subdirectories
SUBDIRS = de

PROPSRC = $(wildcard ./*.properties)
PROPDEST = $(PROPSRC:./%.properties=$(LTICLSDIR)/%.properties)
#PROPDEFAULTSRC = $(wildcard $(AXCLSDIR)/*_en.properties)
#PROPDEFAULT = $(PROPDEFAULTSRC:./%_en.properties=$(AXCLSDIR)/%.properties)

all: cd subdirs 

cd: $(AXCLSFILES) $(PROPDEST)

doc: subdirs

clean: subdirs


$(PROPDEST): $(LTICLSDIR)/%.properties: ./%.properties
	@if [ ! -d $(LTICLSDIR) ]; then \
          mkdir $(LTICLSDIR) ; \
        fi ; \
        cp -v $< $@



