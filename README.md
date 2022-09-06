This file contains the steps we need to take to port to the new version of SALAM.
## Changes:
- Set the `bin/` directory of the arm compiler in `common/makefile`:
    ```
        CROSS_COMPILE_DIR = ~/arm_gcc/gcc-arm-none-eabi-10.3-2021.10/bin
    ```
- `config.yml` is added to all the benchmarks. The `systembuilder.py` read the config for each benchmark and creates the python files (`fs_$BENCHMARK.py` and `$BENCHMARK.py`) containing the setup of the accelerator. By defualt, the generated python files can be found in `config/SALAM/generated/` directory. 
- bash scripts (`runmulti.sh, runvector_stream.sh, runvector.sh`): Since each benchmark is using its own config file (`fs_$BENCHMARK.py` and `$BENCHMARK.py`), bash scripts are changed to run the setup for each benchmarks accordingly. For instance, `run_vector.py` is replaced by `fs_vector_dma.py`.
There is a commented line in the bash files:
    ```
    ${LAB_PATH}/SALAM-Configurator/systembuilder.py --sysName $BENCH --benchDir "benchmarks/${BENCH}"
    ```
    This line generates the python files for us. After the config file is generated, we manaully change it to read the input files (`m1.bin, m2.bin`).
- Reading from input files: Change the line 138 in `fs_$BENCHMARK.py` with:
    ```
    elif args.kernel is not None:
        test_sys.workload.object_file = binary(args.kernel)
        test_sys.workload.extras = [os.environ["LAB_PATH"]+"/benchmarks/inputs/m0.bin",os.environ["LAB_PATH"]+"/benchmarks/inputs/m1.bin"]
        test_sys.workload.extras_addrs = [0x80c00000,0x80c00000+8*8]
    ```
- Auto generated header: `systembuilder.py` creates a new header containing the SPM and DMA addresses. This header is stored in the benchmark directory under the name of `$BENCHMARK_clstr_hw_defines.h`. We include this header in the `defines.h` of every benchmark.


## Create a cluster cache:
1. Go to `config/SALAM/generated/$BENCHMARK.py`, to line 27. Add `cache_size=’1MB’` to `connect_caches()`
2. Add `clstr.top.acp = clstr.coherency_bus.cpu_side_ports` to top config.
3. Go to `src/hwacc/AccCluster.py` line 64 : make it: `if(cache_size!=0):` (this step is done in the gem5-SALAM side)
4. Go to `system_validation.sh` file and comment out *`${M5_PATH}/SALAM-Configurator/systembuilder.py --sysName $BENCH --benchDir "benchmarks/test-cases/${BENCH}"`*
5. Run the bash script corresponding to the cache benchmark.

## Streaming:
### Stream from/to DRAM:
- In the `config.yml`:
    - add a DMA with type = `Stream`. (see `benchmarks/vector_stream1/config.yml`) 
    - Add `streamOut` to the accelerator that receives the data from dram
    - Add `streamIn` to the accelerator that writes the data to the dram
- From `config/SALAM/generated/$BENCHMARK.py` copy the `clstr.streamdma.stream_addr` and put it in `defines.h` as `StreamIn` and `StreamOut`. Also, add 2 more defines that connects ACC1_OUT (`S1IN`) to `StreamIn` and ACC2_OUT to `StreamOut`.

### Between two accelerators:
- In the config.yml:
    - In the source accelerator define a variable with type stream.
    - Set the `streamSize, BufferSize`
    - `InCon: src Accelerator`
    - `OutCon: dst Accelerator`
- The generated header only has the InConnection. In the `defines.h` add another define to alias the output connection and input connection.