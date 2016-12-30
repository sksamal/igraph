
for file in $(ls rand{[0-9],[0-9][0-9]}_[0-9]_*.dot.edg)
do
 base=$(basename $file .dot.edg)
 echo $base
 ../igraph_dpread $base > "$base".txt
 case "$?" in
   1)
     out="ham1";;
   2) 
     out="nonham2";;
   3) 
     out="iss3";;
   4)
     out="iss4";;
   5) 
     out="hamu5";;
   6)
     out="nonhamu6";;
   *)
     out="err";;
 esac
 echo $base is $out
 mv "$base".dot.edg "$base".txt "$base"_0.dot {lbfs2,maplbfs2}-"$base"_0.dot hdp-"$base"_0.dot $out
 echo "Cleaning $base ..."
 for file1 in $(ls *"$base"_[0-9]{.,_}*)
  do
  echo "Removing $file1 ..."
  rm -rf $file1
  done
# exit
done
