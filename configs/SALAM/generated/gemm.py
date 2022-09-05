import m5
from m5.objects import *
from m5.util import *
from configparser import ConfigParser
from HWAccConfig import *

class L1Cache(Cache):
	assoc = 2
	tag_latency = 2
	data_latency = 2
	response_latency = 2
	mshrs = 4
	tgts_per_mshr = 20
	
def __init__(self, size, options=None):
		self.size = size
		super(L1Cache, self).__init__()
		pass

def buildgemm_clstr(options, system, clstr):

	local_low = 0x10020000
	local_high = 0x10038180
	local_range = AddrRange(local_low, local_high)
	external_range = [AddrRange(0x00000000, local_low-1), AddrRange(local_high+1, 0xFFFFFFFF)]
	system.iobus.mem_side_ports = clstr.local_bus.cpu_side_ports
	clstr._connect_caches(system, options, l2coherent=False)
	gic = system.realview.gic

	# Noncoherent DMA
	clstr.dma = NoncoherentDma(pio_addr=0x10020000, pio_size = 21, gic=gic, int_num=95)
	clstr.dma.cluster_dma = clstr.local_bus.cpu_side_ports
	clstr.dma.max_req_size = 64
	clstr.dma.buffer_size = 128
	clstr.dma.dma = clstr.coherency_bus.cpu_side_ports
	clstr.local_bus.mem_side_ports = clstr.dma.pio
	
	# top Definition
	acc = "top"
	ir = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/gemm/hw/top.ll"
	config = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/gemm/config.yml"
	clstr.top = CommInterface(devicename=acc, gic=gic, pio_addr=0x10020040, pio_size=64, int_num=68)
	AccConfig(clstr.top, ir, config)
	
	# gemm Definition
	acc = "gemm"
	ir = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/gemm/hw/gemm.ll"
	config = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/gemm/config.yml"
	clstr.gemm = CommInterface(devicename=acc, gic=gic, pio_addr=0x10020080, pio_size=64)
	AccConfig(clstr.gemm, ir, config)
	
	# top Config
	clstr.top.local = clstr.local_bus.cpu_side_ports
	clstr.top.pio = clstr.local_bus.mem_side_ports
	clstr.top.enable_debug_msgs = False
	
	# gemm Config
	clstr.gemm.pio = clstr.local_bus.mem_side_ports
	clstr.gemm.enable_debug_msgs = False
	
	# MATRIX1 (Variable)
	addr = 0x100200c0
	spmRange = AddrRange(addr, addr + 0x8000)
	clstr.matrix1 = ScratchpadMemory(range = spmRange)
	clstr.matrix1.conf_table_reported = False
	clstr.matrix1.ready_mode = False
	clstr.matrix1.reset_on_scratchpad_read = True
	clstr.matrix1.read_on_invalid = False
	clstr.matrix1.write_on_valid = True
	clstr.matrix1.port = clstr.local_bus.mem_side_ports
	
	# Connecting MATRIX1 to gemm
	for i in range(2):
		clstr.gemm.spm = clstr.matrix1.spm_ports
	
	# MATRIX2 (Variable)
	addr = 0x10028100
	spmRange = AddrRange(addr, addr + 0x8000)
	clstr.matrix2 = ScratchpadMemory(range = spmRange)
	clstr.matrix2.conf_table_reported = False
	clstr.matrix2.ready_mode = False
	clstr.matrix2.reset_on_scratchpad_read = True
	clstr.matrix2.read_on_invalid = False
	clstr.matrix2.write_on_valid = True
	clstr.matrix2.port = clstr.local_bus.mem_side_ports
	
	# Connecting MATRIX2 to gemm
	for i in range(2):
		clstr.gemm.spm = clstr.matrix2.spm_ports
	
	# MATRIX3 (Variable)
	addr = 0x10030140
	spmRange = AddrRange(addr, addr + 0x8000)
	clstr.matrix3 = ScratchpadMemory(range = spmRange)
	clstr.matrix3.conf_table_reported = False
	clstr.matrix3.ready_mode = False
	clstr.matrix3.reset_on_scratchpad_read = True
	clstr.matrix3.read_on_invalid = False
	clstr.matrix3.write_on_valid = True
	clstr.matrix3.port = clstr.local_bus.mem_side_ports
	
	# Connecting MATRIX3 to gemm
	for i in range(2):
		clstr.gemm.spm = clstr.matrix3.spm_ports
	
def makeHWAcc(args, system):

	system.gemm_clstr = AccCluster()
	buildgemm_clstr(args, system, system.gemm_clstr)

