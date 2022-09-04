import m5
from m5.objects import *
from m5.util import *
from configparser import ConfigParser
from HWAccConfig import *

def makeHWAcc(options, system):
    # Specify the path to the config file for an accelerator
    # acc_config = <Absolute path to the config file>
    acc_path = options.accpath + "/" + options.accbench + "/"

############################# Creating the Accelerator Cluster #################################
    # Create a new Accelerator Cluster
    system.acctest  = AccCluster()
    local_low       = 0x2F000000
    local_high      = 0x2FFFFFFF
    local_range     = AddrRange(local_low, local_high)
    external_range  = [AddrRange(0x00000000, local_low-1),
                       AddrRange(local_high+1, 0xFFFFFFFF)]
    system.acctest._attach_bridges(system, local_range, external_range)
 #   system.acctest._connect_caches(system, options, l2coherent=True)
    system.acctest._connect_caches(system, options, l2coherent=True, cache_size = "32kB")


     # Add accelerator top to the cluster
    acc_bench = options.accpath + "/" + options.accbench + "/" + "hw/ir/" + "top.ll"
    # Specify the path to the config file for an accelerator
    # acc_config = <Absolute path to the config file>
    acc_config = options.accpath + "/" + options.accbench + "/" + "hw/configs/" + "top.ini"


    system.acctest.top = CommInterface(devicename="top")

    AccConfig(system.acctest.top, acc_config, acc_bench)
    system.acctest.top.gic = system.realview.gic
    system.acctest._connect_hwacc(system.acctest.top)
    system.acctest.top.local = system.acctest.local_bus.slave
    system.acctest.top.acp = system.acctest.coherency_bus.slave


    ############################# Adding Accelerators to Cluster ##################################

    for acc_name in ["vector", "vector2"]:
        # Add accelerator A1 to the cluster
        acc_bench = options.accpath + "/" + options.accbench + "/" + "hw/ir/" + acc_name + ".ll"
        # Specify the path to the config file for an accelerator
        # acc_config = <Absolute path to the config file>
        acc_config = options.accpath + "/" + options.accbench + "/" + "hw/configs/" + acc_name + ".ini"

        print(acc_config)

        setattr(system.acctest, acc_name, CommInterface(devicename=acc_name))
        ACC = getattr(system.acctest,acc_name)
        AccConfig(ACC, acc_config, acc_bench)

        # Add an SPM attribute to the cluster
        setattr(system.acctest, acc_name+"_spm", ScratchpadMemory())
        ACC_SPM = getattr(system.acctest,acc_name + "_spm")
        AccSPMConfig(ACC, ACC_SPM, acc_config)
        system.acctest._connect_spm(ACC_SPM)


        # Connect the accelerator to the system's interrupt controller
        ACC.gic = system.realview.gic

        # Connect HWAcc to cluster buses
        system.acctest._connect_hwacc(ACC)
        ACC.local = system.acctest.local_bus.slave
        ACC.acp = system.acctest.coherency_bus.slave

        # Enable display of debug messages for the accelerator
        ACC.enable_debug_msgs = True


#     ############################# A2 ##################################
#     # Specify the path to the benchmark file for an accelerator

#     ############################# Adding Accelerators to Cluster ##################################

#     # Add accelerator A2 to the cluster
#     acc_A2_bench = options.accpath + "/" + options.accbench  + "/hw/ir/" + "vector2.ll"

#    #  # Specify the path to the config file for an accelerator
#    #  # acc_config = <Absolute path to the config file>
#     acc_A2_config = options.accpath + "/" + options.accbench + "/hw/configs/" + "vector2.ini"


#     system.acctest.acc_A2 = CommInterface(devicename="vector2")
#     AccConfig(system.acctest.acc_A2, acc_A2_config, acc_A2_bench)


#     # Add an SPM for the accelerator
#     system.acctest.acc_spm_A2 = ScratchpadMemory()
#     AccSPMConfig(system.acctest.acc_A2, system.acctest.acc_spm_A2, acc_A2_config)
#     system.acctest._connect_spm(system.acctest.acc_spm_A2)

    # Add an SPM for the accelerator
    # addr = 0x2F200000
    # spmRange = AddrRange(addr, addr+8096)
    # system.acctest.acc_A2_Buffer = ScratchpadMemory(range=spmRange)
    # system.acctest.acc_A2_Buffer.conf_table_reported = False
    # system.acctest.acc_A2_Buffer.ready_mode=True
    # system.acctest.acc_A2_Buffer.port = system.acctest.local_bus.master
    # for i in range(8):
    #     system.acctest.acc_A2.spm = system.acctest.acc_A2_Buffer.spm_ports



    # Connect the accelerator to the system's interrupt controller
    # system.acctest.acc_A2.gic = system.realview.gic

    # # Connect HWAcc to cluster buses
    # system.acctest._connect_hwacc(system.acctest.acc_A2)
    # system.acctest.acc_A2.local = system.acctest.local_bus.slave
    # system.acctest.acc_A2.acp = system.acctest.coherency_bus.slave

    # # Enable display of debug messages for the accelerator
    # system.acctest.acc_A2.enable_debug_msgs = True
    
    ################################## Adding DMAs to Cluster #####################################
    # Add DMA devices to the cluster and connect them
    system.acctest.dma = NoncoherentDma(pio_addr=0x2ff00000, pio_size=24, gic=system.realview.gic, max_pending=32, int_num=95)
    system.acctest._connect_cluster_dma(system, system.acctest.dma)
    # system.acctest.dma.dma = system.membus.slave
    # system.acctest.dma.pio = system.acctest.local_bus.master

    system.acctest.stream_dma_0 = StreamDma(pio_addr=0x2ff10000, pio_size=32, gic=system.realview.gic, max_pending=32)
    system.acctest.stream_dma_0.stream_in = system.acctest.top.stream
    system.acctest.stream_dma_0.stream_out = system.acctest.top.stream
    system.acctest.stream_dma_0.stream_addr=0x2ff10020
    system.acctest.stream_dma_0.stream_size=8
    system.acctest.stream_dma_0.pio_delay = '1ns'
    system.acctest.stream_dma_0.rd_int = 210
    system.acctest.stream_dma_0.wr_int = 211
    system.acctest._connect_dma(system, system.acctest.stream_dma_0)

    system.acctest.stream_dma_1 = StreamDma(pio_addr=0x2ff20000, pio_size=32, gic=system.realview.gic, max_pending=32)
    system.acctest.stream_dma_1.stream_in = system.acctest.top.stream
    system.acctest.stream_dma_1.stream_out = system.acctest.top.stream
    system.acctest.stream_dma_1.stream_addr=0x2ff20020
    system.acctest.stream_dma_1.stream_size=8
    system.acctest.stream_dma_1.pio_delay = '1ns'
    system.acctest.stream_dma_1.rd_int = 212
    system.acctest.stream_dma_1.wr_int = 213
    system.acctest._connect_dma(system, system.acctest.stream_dma_1)

