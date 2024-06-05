#!/bin/bash

DATA_FILE=mst_random.jsonl
REPEAT=1
SETS=3
WARMUPS=1
ITERATIONS=3


if [ -f $INPUT_FILE ]
then
    mkdir -p results
    rm -f results/$DATA_FILE
    
    for n in 1000 2000 5000 10000 20000 50000 100000 200000 500000 1000000
    do
        echo "Benchmark MST with $n vertices"...
        for i in $(eval echo {0..$(($SETS - 1))})
        do
            echo "Set $i" 
            for _ in $(eval echo {1..$REPEAT})
            do
                ./../build/benchmark_mst $WARMUPS $ITERATIONS dataset/mst/random/mst_${n}_${i}.txt --json >> results/$DATA_FILE || exit
            done
        done
    done
fi
