for file in $(ls *.dot)
do
	pre=$(basename -s dot $file)
#	echo "$pre"pdf
	dot -Kfdp -n $file -Tpdf > "$pre"pdf
done
pdfunite aa*.pdf caa.pdf
pdfunite maa*.pdf cmaa.pdf

