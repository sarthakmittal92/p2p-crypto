#!/bin/bash

echo "./sim -n 10 --slow 0.7 --low 0.7 --txn_interarrival_mean 100 --blk_interarrival_mean 600 --min_ngbrs 4 --max_ngbrs 8 --seed 0 --sim_time 10000 > ../outfile" >> ../commands.txt
./sim -n 10 --slow 0.7 --low 0.7 --txn_interarrival_mean 100 --blk_interarrival_mean 600 --min_ngbrs 4 --max_ngbrs 8 --seed 0 --sim_time 10000 > ../outfile
./files.sh $1