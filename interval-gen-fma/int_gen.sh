#!/bin/bash

# usage: ./main.sh log2

if [ -z "$1" ]; then
	echo "usage: ./main.sh <function>"
	exit 2
fi

make $1

if [ ! -f "$1" ]; then
	echo "exec doesn't exist"
	make clean
	exit 2
fi

./${1} /filer/tmp1/jk1849/oracle34/oracle_${1}f /filer/tmp1/jk1849/

make clean
