for file in $(ls atgraph?.dot.edg atgraph???.dot.edg)
do
        pre=$(basename $file .dot.edg)
	echo "Cleaning $pre ..."
	for file1 in $(ls *"$pre"[0-9]{.,_}*)
	do
	  echo "Removing $file1 ..."
	  rm -rf $file1
	done
done
