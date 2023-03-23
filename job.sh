#!/bin/bash

#SBATCH --partition=main          # Partition (job queue)

#SBATCH --requeue                 # Return job to the queue if preempted

#SBATCH --job-name=xdd		  # Assign a short name to your job

#SBATCH --mem=128000		  # Real memory (RAM) required (MB)

#SBATCH --time=08:00:00           # Total run time limit (HH:MM:SS)

#SBATCH --nodes=1                 # Number of nodes you require

#SBATCH --ntasks=1                # Total # of tasks across all nodes

#SBATCH --cpus-per-task=1         # Cores per task (>1 if multithread tasks)

#SBATCH --output=slurm.%N.%j.out  # STDOUT output file

#SBATCH --error=slurm.%N.%j.err   # STDERR output file (optional)

#SBATCH --constraint=cascadelake

cd /scratch/jk1849/log1p
module purge
module load gcc
module load intel

export LD_LIBRARY_PATH=/home/jk1849/lib
export LIBRARY_PATH=$LD_LIBRARY_PATH
export CPLUS_INCLUDE_PATH=/home/jk1849/include
export C_INCLUDE_PATH=$CPLUS_INCLUDE_PATH

rm main
make main
srun ./main
