#!/bin/bash
#SBATCH --time=07:00:00          # Run time in hh:mm:ss
#SBATCH --mem-per-cpu=3500mb
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --job-name=atrun
#SBATCH --error=/work/deogun/ssamal/igraph/examples/simple/atrun-%j.err
#SBATCH --output=/work/deogun/ssamal/igraph/examples/simple/atrun-%j.out

module load compiler/gcc/5.4
echo $SLURM_JOBID 
#ssh ssamal@cse.unl.edu mkdir -p ~/igraph/%j
