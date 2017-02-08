patt=$1
./clean.sh $patt
./igraph_atread $patt > "$patt".txt
ssh ssamal@cse.unl.edu igraph/clean.sh igraph $patt
scp *"$patt"* ssamal@cse.unl.edu:~/igraph/
ssh ssamal@cse.unl.edu igraph/convert.sh igraph $patt
