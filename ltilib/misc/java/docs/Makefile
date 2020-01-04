

include config.mk

# Names of subdirectories
SUBDIRS = 

all: doc

cd: doc

doc:
	find $(LTISRC) -name *.java -exec perl -e 'while (<>) { /^\s*package\s+([^;]+);/; print $$1,"\n"; }' {} \; | sort | uniq > packages.txt ; \
        javadoc -J-Xmx64M -sourcepath $(LTISRC) -link http://tintas/external/jdk-docs/api @packages.txt

clean:
	rm -r de



