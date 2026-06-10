#!/bin/bash

CMD_DIR=`dirname $0`

PROG=${1:-broyden}
tipo=${2:-avx}
CPU=${3:-3}

DATA_DIR="resultados"
TEMPOS="${DATA_DIR}/Tempos.csv"

METRICA="FLOPS_DP L3CACHE ENERGY"

TAMANHOS="32 64 128 256 512 1000 2000 4000 8000 9000 10000 20000"

mkdir -p ${DATA_DIR}

echo "performance" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor

make clean
make

for m in ${METRICA}
do
    LIKWID_LOG="${DATA_DIR}/${m}_${tipo}.log"
    rm -f ${TEMPOS}
    rm -f ${LIKWID_LOG}

    for n in $TAMANHOS
    do
	LIKWID_OUT="${DATA_DIR}/${m}_${tipo}_${n}.txt"
	
	echo "likwid-perfctr -O -C ${CPU} -g ${m} -o ${LIKWID_OUT} -m ./${PROG} -o sai.txt $n" >/dev/tty

	    
	echo "===> N: ${n} <==" >> ${LIKWID_LOG}
	cat ${LIKWID_OUT} >> ${LIKWID_LOG}
	rm -f ${LIKWID_OUT}
    done

   # ${CMD_DIR}/genplot.py < ${LIKWID_LOG} > ${LIKWID_LOG%%.log}.csv
done

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy${CPU}/scaling_governor 

echo ""
echo ""
