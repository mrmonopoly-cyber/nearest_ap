TEX_SRC ?= ./report.tex

all: $(TEX_SRC)
	latex -output-format=pdf $(TEX_SRC)
clean:
	rm -f *.aux *.toc *.log *.out *.pdf
