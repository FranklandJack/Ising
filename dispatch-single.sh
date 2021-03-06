#!/bin/bash
#
# SGE (Sun Grid Engine) submission script for a single run of the
# mcpi.py Monte Carlo Python code.
#
# To submit this job to be run on the cluster, type:
#
# qsub ./dispatch-single.sh [number_of_trials] [output_filename]
#
# at the command line from the directory this script lives in.
#
# Both arguments are optional; defaults of 1,000,000 trials and a
# file called pi-1e6.dat will be created
# if not specified.
#
# The output file always lands in a directory called data/
# (you should not include data/ as part of your filename)
#
#####################################################################
#
# Lines beginning with '#$' below provide arguments the qsub command,
# and save you typing them out each time.
#
# (1) Run the job in the same directory that qsub is issued
#
#$ -cwd
#
# (2) Give the job a meaningful name (this will appear in qstat listings)
#
#$ -N mcpi
#
# (3) Send a notification email when your job begins (b) and ends (e)
# being run
#
#$ -m be
#
# (4) Set the queue that the job should run on. As undergraduates, you have
# access to the sopa.1.day queue.
#
#$ -q sopa.1.day
#
# (5) Give an estimate of how long the job should run for, expressed as
# hours:minutes:seconds. This can be up to 24 hours on the sopa.1.day queue.
# NB: your job will be halted when this time expires, so it is best to slightly
# over-estimate.
#
#$ -l h_rt=0:15:00

mkdir -p data # This makes sure the data directory can be written to
./mcpi.py -t ${1:-1000000} -o data/${2:-pi-1e6.dat}

# The ${n:-default} syntax looks for the n'th argument supplied to the
# script, and substitutes default if this is empty.
