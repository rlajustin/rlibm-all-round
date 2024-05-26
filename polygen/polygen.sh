#!/bin/bash

# usage: ./polygen.sh log2

LOG_FILE="$(echo $1_generating_logs)"

if [ -z "$1" ]; then
	echo "usage: ./polygen.sh <function>"
	exit 2
fi

if [ ! -f "/filer/tmp1/jk1849/sorted_$1" ]; then
	echo "no interval file exists"
	exit 2
fi

make clean
make piecewise
./piecewise /filer/tmp1/jk1849/sorted_$1 > $LOG_FILE

env VERSION_CONTROL=numbered mv --backup=t $LOG_FILE logs/FINISHED_$1
make clean

