#!/bin/bash

CMD_DIR=`dirname $0`

PROG=${1:-broyden}
CPU=${2:-3}

DATA_DIR="resultados"

METRICA="FLOPS_DP L3CACHE ENERGY"

TAMANHOS="32 64 128 256 512 1000 2000 4000 8000 9000 10000 20000"

mkdir -p ${DATA_DIR}

echo "performance" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor

make clean
make

for m in ${METRICA}
do
    LIKWID_LOG="${DATA_DIR}/${m}.log"
    rm -f ${LIKWID_LOG}
    touch ${LIKWID_LOG}

    for n in $TAMANHOS
    do
	LIKWID_OUT="${DATA_DIR}/${m}_${n}.txt"
    BROYDEN_OUT="${DATA_DIR}/${m}_${n}_sai.txt"
    rm -f ${BROYDEN_OUT}
	touch ${LIKWID_OUT}
    touch ${BROYDEN_OUT}
	echo "${m}: ./${PROG} $n" >/dev/tty
    likwid-perfctr -C ${CPU} -g ${m} -m ./${PROG} -o ${BROYDEN_OUT} $n >${LIKWID_OUT}
	    
	echo "===> N: ${n} <==" >> ${LIKWID_LOG}
	cat ${LIKWID_OUT} >> ${LIKWID_LOG}
    rm -f ${LIKWID_OUT}
    done
done

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor 

echo ""
echo ""
