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

def buildmulti_vector_cache1_clstr(options, system, clstr):

	local_low = 0x10020000
	local_high = 0x1006b280
	local_range = AddrRange(local_low, local_high)
	external_range = [AddrRange(0x00000000, local_low-1), AddrRange(local_high+1, 0xFFFFFFFF)]
	system.iobus.mem_side_ports = clstr.local_bus.cpu_side_ports
	clstr._connect_caches(system, options, l2coherent=True, cache_size='32kB')
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
	ir = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/multi_vector_cache1/hw/top.ll"
	config = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/multi_vector_cache1/config.yml"
	clstr.top = CommInterface(devicename=acc, gic=gic, pio_addr=0x10020040, pio_size=64, int_num=68)
	AccConfig(clstr.top, ir, config)
	
	# multi_vector1 Definition
	acc = "multi_vector1"
	ir = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/multi_vector_cache1/hw/multi_vector1.ll"
	config = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/multi_vector_cache1/config.yml"
	clstr.multi_vector1 = CommInterface(devicename=acc, gic=gic, pio_addr=0x10020080, pio_size=64)
	AccConfig(clstr.multi_vector1, ir, config)
	
	# vector2 Definition
	acc = "vector2"
	ir = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/multi_vector_cache1/hw/vector2.ll"
	config = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/multi_vector_cache1/config.yml"
	clstr.vector2 = CommInterface(devicename=acc, gic=gic, pio_addr=0x10045980, pio_size=64)
	AccConfig(clstr.vector2, ir, config)
	
	# top Config
	clstr.top.local = clstr.local_bus.cpu_side_ports
	clstr.top.pio = clstr.local_bus.mem_side_ports
	clstr.top.enable_debug_msgs = False
	clstr.top.acp = clstr.coherency_bus.cpu_side_ports
	# multi_vector1 Config
	clstr.multi_vector1.pio = clstr.local_bus.mem_side_ports
	clstr.multi_vector1.enable_debug_msgs = False
	
	# MATRIX1 (Variable)
	addr = 0x100200c0
	spmRange = AddrRange(addr, addr + 0xc800)
	clstr.matrix1 = ScratchpadMemory(range = spmRange)
	clstr.matrix1.conf_table_reported = False
	clstr.matrix1.ready_mode = False
	clstr.matrix1.reset_on_scratchpad_read = True
	clstr.matrix1.read_on_invalid = False
	clstr.matrix1.write_on_valid = True
	clstr.matrix1.port = clstr.local_bus.mem_side_ports
	
	# Connecting MATRIX1 to multi_vector1
	for i in range(2):
		clstr.multi_vector1.spm = clstr.matrix1.spm_ports
	
	# MATRIX2 (Variable)
	addr = 0x1002c900
	spmRange = AddrRange(addr, addr + 0xc800)
	clstr.matrix2 = ScratchpadMemory(range = spmRange)
	clstr.matrix2.conf_table_reported = False
	clstr.matrix2.ready_mode = False
	clstr.matrix2.reset_on_scratchpad_read = True
	clstr.matrix2.read_on_invalid = False
	clstr.matrix2.write_on_valid = True
	clstr.matrix2.port = clstr.local_bus.mem_side_ports
	
	# Connecting MATRIX2 to multi_vector1
	for i in range(2):
		clstr.multi_vector1.spm = clstr.matrix2.spm_ports
	
	# MATRIX3 (Variable)
	addr = 0x10039140
	spmRange = AddrRange(addr, addr + 0xc800)
	clstr.matrix3 = ScratchpadMemory(range = spmRange)
	clstr.matrix3.conf_table_reported = False
	clstr.matrix3.ready_mode = False
	clstr.matrix3.reset_on_scratchpad_read = True
	clstr.matrix3.read_on_invalid = False
	clstr.matrix3.write_on_valid = True
	clstr.matrix3.port = clstr.local_bus.mem_side_ports
	
	# Connecting MATRIX3 to multi_vector1
	for i in range(2):
		clstr.multi_vector1.spm = clstr.matrix3.spm_ports
	
	# vector2 Config
	clstr.vector2.pio = clstr.local_bus.mem_side_ports
	clstr.vector2.enable_debug_msgs = False
	
	# V2_MAT1 (Variable)
	addr = 0x100459c0
	spmRange = AddrRange(addr, addr + 0xc800)
	clstr.v2_mat1 = ScratchpadMemory(range = spmRange)
	clstr.v2_mat1.conf_table_reported = False
	clstr.v2_mat1.ready_mode = False
	clstr.v2_mat1.reset_on_scratchpad_read = True
	clstr.v2_mat1.read_on_invalid = False
	clstr.v2_mat1.write_on_valid = True
	clstr.v2_mat1.port = clstr.local_bus.mem_side_ports
	
	# Connecting V2_MAT1 to vector2
	for i in range(2):
		clstr.vector2.spm = clstr.v2_mat1.spm_ports
	
	# V2_MAT2 (Variable)
	addr = 0x10052200
	spmRange = AddrRange(addr, addr + 0xc800)
	clstr.v2_mat2 = ScratchpadMemory(range = spmRange)
	clstr.v2_mat2.conf_table_reported = False
	clstr.v2_mat2.ready_mode = False
	clstr.v2_mat2.reset_on_scratchpad_read = True
	clstr.v2_mat2.read_on_invalid = False
	clstr.v2_mat2.write_on_valid = True
	clstr.v2_mat2.port = clstr.local_bus.mem_side_ports
	
	# Connecting V2_MAT2 to vector2
	for i in range(2):
		clstr.vector2.spm = clstr.v2_mat2.spm_ports
	
	# V2_MAT3 (Variable)
	addr = 0x1005ea40
	spmRange = AddrRange(addr, addr + 0xc800)
	clstr.v2_mat3 = ScratchpadMemory(range = spmRange)
	clstr.v2_mat3.conf_table_reported = False
	clstr.v2_mat3.ready_mode = False
	clstr.v2_mat3.reset_on_scratchpad_read = True
	clstr.v2_mat3.read_on_invalid = False
	clstr.v2_mat3.write_on_valid = True
	clstr.v2_mat3.port = clstr.local_bus.mem_side_ports
	
	# Connecting V2_MAT3 to vector2
	for i in range(2):
		clstr.vector2.spm = clstr.v2_mat3.spm_ports
	
def makeHWAcc(args, system):

	system.multi_vector_cache1_clstr = AccCluster()
	buildmulti_vector_cache1_clstr(args, system, system.multi_vector_cache1_clstr)

