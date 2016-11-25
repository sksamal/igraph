patt=$2
cd $1
#for file in $(ls test*.dot)
for file in $(ls "$patt"*.dot)
do
        pre=$(basename -s dot $file)
	for file1 in $(ls *"$file") 
	do
          pre1=$(basename -s dot $file1)
	  echo "Creating... $pre1"pdf
	  rm -rf "$pre1"pdf
          dot -Kfdp -n $file1 -Tpdf > "$pre1"pdf
	done
	rm -rf c"$pre"pdf
	pdfunite "$pre"pdf lbfs1-"$pre"pdf *lbfs2-"$pre"pdf *at-"$pre"pdf c"$pre"pdf
done
rm -rf maplbfs* remaplbfs*
pdfunite lbfs2-"$patt"*pdf cl"$patt".pdf
#pdfunite aa*.pdf caa.pdf
#pdfunite maa*.pdf cmaa.pdf
