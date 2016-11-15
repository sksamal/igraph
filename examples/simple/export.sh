	file=$1
        pre=$(basename -s dot $file)
        echo "Creating... $pre"pdf
        dot -Kfdp -n $file -Tpdf > "$pre"pdf

