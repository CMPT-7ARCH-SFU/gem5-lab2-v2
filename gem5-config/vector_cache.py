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

def buildvector_cache_clstr(options, system, clstr):

	local_low = 0x10020000
	local_high = 0x10020080
	local_range = AddrRange(local_low, local_high)
	external_range = [AddrRange(0x00000000, local_low-1), AddrRange(local_high+1, 0xFFFFFFFF)]
	system.iobus.mem_side_ports = clstr.local_bus.cpu_side_ports
	clstr._connect_caches(system, options, l2coherent=True, cache_size='1kB')
	gic = system.realview.gic

	# Noncoherent DMA
	clstr.dma = NoncoherentDma(pio_addr=0x10020000, pio_size = 21, gic=gic, int_num=95)
	clstr.dma.cluster_dma = clstr.local_bus.cpu_side_ports
	clstr.dma.max_req_size = 64
	clstr.dma.buffer_size = 128
	clstr.dma.dma = clstr.coherency_bus.cpu_side_ports
	clstr.local_bus.mem_side_ports = clstr.dma.pio
	
	# vector_cache Definition
	acc = "vector_cache"
	ir = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/vector_cache/hw/vector_cache.ll"
	config = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/vector_cache/config.yml"
	clstr.vector_cache = CommInterface(devicename=acc, gic=gic, pio_addr=0x10020040, pio_size=64, int_num=68)
	AccConfig(clstr.vector_cache, ir, config)
	
	# vector_cache Config
	clstr.vector_cache.pio = clstr.local_bus.mem_side_ports
	clstr.vector_cache.enable_debug_msgs = False
	clstr.vector_cache.acp = clstr.coherency_bus.cpu_side_ports
	clstr.vector_cache.acp = clstr.coherency_bus.cpu_side_ports

def makeHWAcc(args, system):

	system.vector_cache_clstr = AccCluster()
	buildvector_cache_clstr(args, system, system.vector_cache_clstr)

