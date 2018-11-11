all: functions.pdf laplace.pdf

functions.pdf: functions.tex
	pdflatex $^ $@

laplace.pdf: laplace.tex
	pdflatex $^ $@

clean:
	rm *.log
	rm *.pdf
	rm *.aux
	rm *.out
