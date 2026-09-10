# **************************************************************************** #
# Prezentace k obhajobě IBP					 	                               #
# Autor:	Krupička Vojtěch	 xkrupi06                                      #
# Soubor:   xkrupi06.tex                                                       #
# Datum:    10. 06. 2009                                                       #
# Kódování: UTF-8                                                              #
# **************************************************************************** #

# Definice proměných pro překlad
DOC=xkrupi06
PACK=xkrupi06-fit.tar.gz

# Provedení vlastního překladu
all: $(DOC)

# Kompilace a vysázení výsledného dokumentu
$(DOC): $(DOC).tex
	latex $(DOC).tex
	latex $(DOC).tex
	dvips -ta4 $(DOC).dvi -o $(DOC).ps
	ps2pdf -sPAPERSIZE=a4 $(DOC).ps
	
# Smazání všech pomocných souborů, i výsledného dokumentu ve formátu PDF
clean:
	rm -f *.dvi *.ps *.aux *.pdf *.gz

# Vytvoření výsledného archívu
pack:
	tar cvzf $(PACK) $(DOC).tex Makefile *.eps
