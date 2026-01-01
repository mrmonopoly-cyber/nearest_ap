TEX_SRC := report.tex

PLOT_DIR := ./plot

all: clean report

plots:
	@mkdir -p $(PLOT_DIR)
	./make_plots.sh

report: $(TEX_SRC) plots
	latexmk -pdf $(TEX_SRC)

clean:
	rm -f *.aux *.toc *.log *.out *.pdf *.blg *.bbl *.dvi *.fls *.fdb* *.xcp
	rm -rf $(PLOT_DIR)
