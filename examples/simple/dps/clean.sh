for file in $(ls rand*_[0-9]_*_0.dot)
do
        pre=$(basename $file _0.dot)
	echo "Cleaning $pre ..."
	for file1 in $(ls *"$pre"_[0-9]{.,_}*)
	do
	  echo "Removing $file1 ..."
	  rm -rf $file1
	done
done
