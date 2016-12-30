#!/bin/bash
#SBATCH --time=07:00:00          # Run time in hh:mm:ss
#SBATCH --mem-per-cpu=9500mb
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --job-name=atgen
#SBATCH --error=/work/deogun/ssamal/igraph/examples/simple/atgen-%j.err
#SBATCH --output=/work/deogun/ssamal/igraph/examples/simple/atgen-%j.out

module load compiler/gcc/5.4
./igraph_atrandomgen
