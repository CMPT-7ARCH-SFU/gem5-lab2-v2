#!/bin/bash
FLAGS="HWACC,LLVMRuntime,StreamDma"
DEBUG="false"
PRINT_TO_FILE="false"

while getopts ":f:dp" opt
	do
		case $opt in
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
				exit 1
				;;
		esac
done

if [ "${DEBUG}" == "true" ]; then
	BINARY="ddd --gdb --args ${M5_PATH}/build/ARM/gem5.debug"
else
	BINARY="${M5_PATH}/build/ARM/gem5.opt"
fi

KERNEL=$LAB_PATH/benchmarks/vector_stream1/host/main.elf
SYS_OPTS="--mem-size=4GB \
          --kernel=$KERNEL \
          --disk-image=$M5_PATH/baremetal/common/fake.iso \
          --machine-type=VExpress_GEM5_V1 \
          --dtb-file=none --bare-metal \
          --cpu-type=DerivO3CPU"
CACHE_OPTS="--caches --l2cache --acc_cache"

OUTDIR=BM_ARM_OUT/vector_stream1
BENCH=vector_stream1
# ${LAB_PATH}/SALAM-Configurator/systembuilder.py --sysName $BENCH --benchDir "benchmarks/${BENCH}"

# RUN_SCRIPT="$BINARY $DEBUG_FLAGS --outdir=$OUTDIR \
# 			configs/SALAM/generated/fs_$BENCH.py $SYS_OPTS \
# 			--accpath=$LAB_PATH/benchmarks \
# 			--accbench=$BENCH $CACHE_OPTS"
RUN_SCRIPT="$BINARY $DEBUG_FLAGS --outdir=$OUTDIR \
			gem5-config/fs_$BENCH.py $SYS_OPTS \
			--accpath=$LAB_PATH/benchmarks \
			--accbench=$BENCH $CACHE_OPTS"

if [ "${PRINT_TO_FILE}" == "true" ]; then
	mkdir -p $OUTDIR
	$RUN_SCRIPT > ${OUTDIR}/debug-trace.txt
else
	$RUN_SCRIPT
fi

# Debug Flags List
#
# IOAcc
# ClassDetail
# CommInterface
# ComputeUnit
# LLVMInterface
# ComputeNode
# LLVMRegister
# LLVMOp
# LLVMParse
# LLVMGEP
# LLVMRuntime == ComputeNode + LLVMRegister + LLVMOp + LLVMParse
# NoncoherentDma - bfs, fft, gemm, md-knn, nw, spmv


