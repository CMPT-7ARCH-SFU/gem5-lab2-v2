#!/bin/bash
# Uncomment if you want trace. Dumps txt trace. Do not enable for large programs.
FLAGS="DeviceMMR,LLVMInterface,AddrRanges,NoncoherentDma,RuntimeCompute"
# NoncoherentDma only dumps the copy from and to host
#FLAGS="NoncoherentDma"
BENCH=""
DEBUG="false"
PRINT_TO_FILE="false"

while getopts ":b:f:dp" opt
	do
		case $opt in
			b )
				BENCH=${OPTARG}
				;;
			d )
				DEBUG="true"
				;;
			p )
				PRINT_TO_FILE="true"
				;;
			f )
				FLAGS+=",${OPTARG}"
				;;
			* )
				echo "Invalid argument: ${OPTARG}"
				echo "Usage: $0 -b BENCHMARK (-f DEBUGFLAG) (-p) (-d)"
				exit 1
				;;
		esac
done

if [ "${BENCH}" == "" ]; then
	echo "No benchmark specified."
	echo "Usage: $0 -b BENCHMARK (-f DEBUGFLAG) (-p) (-d)"
	exit 2
fi

if [ "${DEBUG}" == "true" ]; then
	BINARY="ddd --gdb --args ${M5_PATH}/build/ARM/gem5.debug"
else
	BINARY="${M5_PATH}/build/ARM/gem5.opt"
fi

KERNEL=$LAB_PATH/benchmarks/$BENCH/host/main.elf
SYS_OPTS="--mem-size=4GB \
          --kernel=$KERNEL \
          --disk-image=$M5_PATH/baremetal/common/fake.iso \
          --machine-type=VExpress_GEM5_V1 \
          --dtb-file=none --bare-metal \
          --cpu-type=DerivO3CPU"
# With acc cache
CACHE_OPTS="--caches --l2cache --acc_cache"

# Script to start up full system simulation
# --debug-flags=$FLAGS

OUTDIR=BM_ARM_OUT/$BENCH
DEBUG_FLAGS=""

if [ "${FLAGS}" != "" ]; then
	DEBUG_FLAGS+="--debug-flags="
	DEBUG_FLAGS+=$FLAGS
fi
# ${LAB_PATH}/SALAM-Configurator/systembuilder.py --sysName $BENCH --benchDir "benchmarks/${BENCH}"

# RUN_SCRIPT="$BINARY $DEBUG_FLAGS --outdir=$OUTDIR \
# 			configs/SALAM/generated/fs_$BENCH.py $SYS_OPTS \
# 			--accpath=$LAB_PATH/benchmarks \
# 			--accbench=$BENCH $CACHE_OPTS"
RUN_SCRIPT="$BINARY $DEBUG_FLAGS --outdir=$OUTDIR \
			$LAB_PATH/gem5-config/fs_$BENCH.py $SYS_OPTS \
			--accpath=$LAB_PATH/benchmarks \
			--accbench=$BENCH $CACHE_OPTS"

# RUN_SCRIPT="$BINARY $DEBUG_FLAGS --outdir=$OUTDIR \
# 			gem5-config/run_vector.py $SYS_OPTS \
# 			--accpath=$LAB_PATH/benchmarks \
# 			--accbench=$BENCH $CACHE_OPTS"

echo $RUN_SCRIPT

if [ "${PRINT_TO_FILE}" == "true" ]; then
	mkdir -p $OUTDIR
	$RUN_SCRIPT > ${OUTDIR}/debug-trace.txt
else
	$RUN_SCRIPT
fi

# Debug Flags List
#
# CommInterface
# DeviceMMR
# Runtime
# LLVMInterface
# LLVMParse
# RuntimeCompute
# AddrRanges





