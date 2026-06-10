#!/bin/bash

CMD_DIR=`dirname $0`

PROG=${1:-broyden}
CPU=${2:-3}

DATA_DIR="resultados"

METRICA="FLOPS_DP L3CACHE ENERGY"

TAMANHOS="32 64 128 256 512 1000 "

mkdir -p ${DATA_DIR}

echo "performance" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor

make clean
make

for m in ${METRICA}
do
    LIKWID_LOG="${DATA_DIR}/${m}.log"
    touch ${LIKWID_LOG}

    for n in $TAMANHOS
    do
	LIKWID_OUT="${DATA_DIR}/${m}_${n}.txt"
	touch ${LIKWID_OUT}
	echo "${m} ./${PROG} $n" >/dev/tty
    likwid-perfctr -C ${CPU} -g ${m} -o ${LIKWID_OUT} -m ./${PROG} -o sai.txt $n
	    
	echo "===> N: ${n} <==" >> ${LIKWID_LOG}
	cat ${LIKWID_OUT} >> ${LIKWID_LOG}
    done

   # ${CMD_DIR}/genplot.py < ${LIKWID_LOG} > ${LIKWID_LOG%%.log}.csv
done

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor 

echo ""
echo ""
