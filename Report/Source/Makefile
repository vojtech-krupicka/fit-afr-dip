# makefile pro preklad LaTeX verze Bc. prace
# (c) 2008 Michal Bidlo
# E-mail: bidlom AT fit vutbr cz
#===========================================
# upravil: Vojtìch Krupièka, xkrupi06 AT stud.fit.vutbr.cz, 20.5.2009

# skript pro pøevod obrázkù do PDF
FIG=./convert.sh

# název výstupního souboru pro WIS
CO=xkrupi06

# název výstupního souboru pro TISK
COP=xkrupi06_print

# Pøíkaz 'make'
all: convert $(CO).pdf

# pøevod obrázkù
convert:
	chmod 777 $(FIG)
	$(FIG)

# verze pro tisk
print: $(COP).pdf
	
pdf: $(CO).pdf

# verze pro WIS
$(CO).ps: $(CO).dvi
	dvips $(CO)

$(CO).pdf: clean
	pdflatex $(CO)
	bibtex $(CO)
	pdflatex $(CO)
	pdflatex $(CO)

$(CO).dvi: $(CO).tex $(CO).bib
	latex $(CO)
	bibtex $(CO)
	latex $(CO)
	latex $(CO)
	
# verze pro TISK
$(COP).ps: $(COP).dvi
	dvips $(COP)

$(COP).pdf: clean
	pdflatex $(COP)
	bibtex $(COP)
	pdflatex $(COP)
	pdflatex $(COP)

$(COP).dvi: $(COP).tex $(COP).bib
	latex $(COP)
	bibtex $(COP)
	latex $(COP)
	latex $(COP)

# vygeneruje pouze desky
desky:
#	latex desky
#	dvips desky
#	dvipdf desky
	pdflatex desky

# vyma¾e doèasná data
clean:
	rm -f *.dvi *.log $(CO).blg $(CO).bbl $(CO).toc *.aux $(CO).out $(CO).lof
	rm -f *.dvi *.log $(COP).blg $(COP).bbl $(COP).toc *.aux $(COP).out $(COP).lof
	rm -f *~
  
# vyma¾e doèasná data vcetne PDF souboru
uninstall:
	rm -f *.dvi *.log $(CO).blg $(CO).bbl $(CO).toc *.aux $(CO).out $(CO).lof
	rm -f $(CO).pdf
	rm -f *.dvi *.log $(COP).blg $(COP).bbl $(COP).toc *.aux $(COP).out $(COP).lof
	rm -f $(COP).pdf
	rm -f *~

# zabalí soubory do archivu
pack:
	tar czvf bp-xkrupi06.tar.gz *.tex *.bib *.bst *.cls ./fig/* ./content/* Makefile Changelog *.sh
