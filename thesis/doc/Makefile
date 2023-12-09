MAKEFLAGS += --silent

all: Bibliography.bib  htwcd Bericht.tex Glossar.tex Selbststaendigkeitserklaerung.tex
	@pdflatex Bericht.tex  >/dev/null
	@makeglossaries Bericht >/dev/null
	@biber Bericht >/dev/null
	@pdflatex Bericht.tex 2> /dev/null >/dev/null
	rm -f Bericht.acn Bericht.aux Bericht.bcf Bericht.glo Bericht.ist Bericht.lof Bericht.log Bericht.lot Bericht.out Bericht.run.xml Bericht.toc texput.log Bericht.bbl Bericht.blg Bericht.acr Bericht.alg Bericht.glg Bericht.gls Bericht.lol


clean:
	rm -f Bericht.pdf
