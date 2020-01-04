# GNU MAKE FILE

# tex,eps and pdf files
TEXFILES=$(wildcard ./*.tex)
EPSFILES=$(wildcard ./fig/*.eps)
PDFFILES=$(patsubst %.eps,./fig/%.pdf,$(notdir $(EPSFILES)))

# implicit rules for the debug version
./fig/%.pdf : ./fig/%.eps
	@echo "Converting $< to pdf" ; \
        epstopdf --outfile=$@ $< 

all: ps two

ps: DevelopersGuide.ps

pdf: DevelopersGuide.pdf

two: DevelopersGuide2.ps
	rm -f DevelopersGuide2.ps.gz
	gzip DevelopersGuide2.ps

booklet: Booklet.ps
	rm -f Booklet.ps.gz
	gzip Booklet.ps

DevelopersGuide.ps: DevelopersGuide.dvi
	dvips -t a4 DevelopersGuide

DevelopersGuide2.ps: DevelopersGuide.ps
	psnup -2 DevelopersGuide.ps > DevelopersGuide2.ps

Booklet.ps: DevelopersGuide.ps
	echo "Generating booklet..."
	psbook DevelopersGuide.ps DevelopersGuide_pre.ps
	psnup -2 DevelopersGuide_pre.ps Booklet_pre.ps
	pstops -pa4 '2:0,1U(21cm,29.7cm)' Booklet_pre.ps Booklet.ps
	rm Booklet_pre.ps
	rm DevelopersGuide_pre.ps

DevelopersGuide.dvi: $(TEXFILES)
	echo "Running latex..."
	@latex DevelopersGuide
	echo "Running makeindex..."
	@makeindex DevelopersGuide
	echo "Running bibtex..."
	@bibtex DevelopersGuide
	echo "Rerunning latex...."
	@latex DevelopersGuide
	echo "Rerunning latex...."
	@latex DevelopersGuide
	echo "Done!"

DevelopersGuide.pdf: $(TEXFILES) $(PDFFILES)
	echo "Running pdflatex..."
	@pdflatex DevelopersGuide
	echo "Running makeindex..."
	@makeindex DevelopersGuide
	echo "Running bibtex..."
	@bibtex DevelopersGuide
	echo "Rerunning pdflatex...."
	@pdflatex DevelopersGuide
	echo "Rerunning pdflatex...."
	@pdflatex DevelopersGuide
	echo "Done!"

clean:
	rm -f *.ps *.dvi *.aux *.toc *.idx *.ind *.ilg *.log *.out *.bbl *.blg *.lot *.pdf *.*~



