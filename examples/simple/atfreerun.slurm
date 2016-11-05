#!/bin/bash
#SBATCH --time=07:00:00          # Run time in hh:mm:ss
#SBATCH --mem-per-cpu=3500mb
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --job-name=atrun
#SBATCH --error=/work/deogun/ssamal/igraph/examples/simple/atrun-%j.err
#SBATCH --output=/work/deogun/ssamal/igraph/examples/simple/atrun-%j.out

module load compiler/gcc/5.4
ssh ssamal@cse.unl.edu "mkdir -p ~/igraph/$SLURM_JOBID"
echo "Git status:" > stats.txt
git status -uno >> stats.txt
git log -1 >> stats.txt
echo "Running at " >> stats.txt
date >> stats.txt
scp stats.txt ssamal@cse.unl.edu:~/igraph/$SLURM_JOBID
./build runat $SLURM_JOBID
echo "Successfully completed" >> stats.txt
scp stats.txt ssamal@cse.unl.edu:~/igraph/$SLURM_JOBID
rm -f aa*.txt *.dot
