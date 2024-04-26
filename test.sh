#!/bin/bash
#usage: ./perf.sh log

#set -e

S=2
N=10
M=${PERF_REPEAT:-500}

{ read -r -d '' prog_end || true; } <<EOF
END {
	s = 0;
	nout = int(5*i/100);
	for(k = 1; k < i-nout; k++){
		d = a[k] - a[0];
		s += d*d;
	}
	rms = sqrt(s/i-nout-1));
	nmd = a[int(i/2)] - a[0];
	printf "Ntrial = %d ; Min = %.3f + %.3f clc/call; Median-Min = %.3f clc/call; Max = %.3f clc/call;\n", i, a[0], rms, nmd, a[i-1];
}
EOF

prog_bar () {
	local T=$1 i=$2
	local p=$(( i * 100 / T))
	local j=$(( T - i ))
	printf "[%s%s] %3d %%\r" "${str_hsh:0:${i}}" "${str_dot:0:${j}}" $p
	if (( i == T)); then
		echo
	fi
}

collect_perf_stat () {
	echo -n "" > $LOG_FILE
	prog_bar $S 0
	local i=1
	while [ $i -le $S ]; do
		perf stat -e cycles -x " " ./perf $PERF_ARGS &>> $LOG_FILE
		prog_bar $S $i
		i=$(( i + 1 ))
	done
}

process_perf_stat () {
	sort -g -k 1 $LOG_FILE | awk "/cycles/{a[i++]=\$1/(${N}*${M});} ${prog_end}"
}

perfom_perf_stat () {
	perf stat -e cycles -x " " ./perf $PERF_ARGS 2>&1 | awk "/cycles/{print \$1/(${N}*${M})}"
}

proc_perf () {
	collect_perf_stat
	process_perf_stat
}

#############

LOG_FILE="$(mktemp log-file.XXXXXX)"
RANDOMS_FILE="$(mktemp rand-file.XXXXXX)"

f=$1
u="$(echo src/${f}f.c)"

if [ -f "$u" ]; then
	dir="${u%/*}"
else
	echo "function not found: %f"
	exit 2
fi

CFLAGS="-O3 -march=native"
ROUNDING_MATH="-frounding-math"
export CFLAGS ROUNDING_MATH

if [ -n "$LIBM" ]; then
	BACKUP_LIBM="$LIBM"
	unset LIBM
fi

i=1
str_hsh=""
str_dot=""
while [ $i -le $S ]; do
	str_hsh="${str_hsh}#"
	str_dot="${str_dot}."
	i=$(( i + 1 ))
done

cd $dir

make -s -C test clean
make -s -C test perf

./perf --file ${RANDOMS_FILE} --count ${N} --reference # makes random file

PERF_ARGS="--file ${RANDOMS_FILE} --count ${N} --repeat ${M}"

proc_perf
PERF_ARGS="${PERF_ARGS} --libc"
proc_perf

has_symbol () {
	[ "$(nm "$LIBM" | while read a b c; do if [ "$c" = "$f" ]; then echo OK; return; fi; done | wc -l)" -ge 1 ]
}

export LIBM="$BACKUP_LIBM"
