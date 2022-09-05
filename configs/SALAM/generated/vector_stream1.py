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

def buildvector_stream_clstr(options, system, clstr):

	local_low = 0x10020000
	local_high = 0x10020500
	local_range = AddrRange(local_low, local_high)
	external_range = [AddrRange(0x00000000, local_low-1), AddrRange(local_high+1, 0xFFFFFFFF)]
	system.iobus.mem_side_ports = clstr.local_bus.cpu_side_ports
	clstr._connect_caches(system, options, l2coherent=True)
	gic = system.realview.gic

	# Stream DMA
	clstr.streamdma = StreamDma(pio_addr=0x10020000, status_addr=0x10020040, pio_size = 32, gic=gic, max_pending = 32)
	clstr.streamdma.stream_addr = 0x10020000 + 32
	clstr.streamdma.stream_size = 128
	clstr.streamdma.pio_delay = '1ns'
	clstr.streamdma.rd_int = 210
	clstr.streamdma.wr_int = 211
	clstr.streamdma.dma = clstr.coherency_bus.cpu_side_ports
	clstr.local_bus.mem_side_ports = clstr.streamdma.pio
	
	# Noncoherent DMA
	clstr.dma = NoncoherentDma(pio_addr=0x10020080, pio_size = 21, gic=gic, int_num=95)
	clstr.dma.cluster_dma = clstr.local_bus.cpu_side_ports
	clstr.dma.max_req_size = 64
	clstr.dma.buffer_size = 128
	clstr.dma.dma = clstr.coherency_bus.cpu_side_ports
	clstr.local_bus.mem_side_ports = clstr.dma.pio
	
	# top Definition
	acc = "top"
	ir = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/vector_stream1/hw/top.ll"
	config = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/vector_stream1/config.yml"
	clstr.top = CommInterface(devicename=acc, gic=gic, pio_addr=0x100200c0, pio_size=64, int_num=68)
	AccConfig(clstr.top, ir, config)
	
	# s1 Definition
	acc = "s1"
	ir = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/vector_stream1/hw/S1.ll"
	config = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/vector_stream1/config.yml"
	clstr.s1 = CommInterface(devicename=acc, gic=gic, pio_addr=0x10020100, pio_size=64)
	AccConfig(clstr.s1, ir, config)
	
	# s2 Definition
	acc = "s2"
	ir = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/vector_stream1/hw/S2.ll"
	config = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/vector_stream1/config.yml"
	clstr.s2 = CommInterface(devicename=acc, gic=gic, pio_addr=0x10020400, pio_size=64)
	AccConfig(clstr.s2, ir, config)
	
	# s3 Definition
	acc = "s3"
	ir = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/vector_stream1/hw/S3.ll"
	config = "/localhome/mha157/Desktop/gem5-lab2/benchmarks/vector_stream1/config.yml"
	clstr.s3 = CommInterface(devicename=acc, gic=gic, pio_addr=0x100204c0, pio_size=64)
	AccConfig(clstr.s3, ir, config)
	
	# top Config
	clstr.top.local = clstr.local_bus.cpu_side_ports
	clstr.top.pio = clstr.local_bus.mem_side_ports
	clstr.top.enable_debug_msgs = False
	
	# s1 Config
	clstr.s1.pio = clstr.local_bus.mem_side_ports
	clstr.s1.stream = clstr.streamdma.stream_out
	clstr.s1.enable_debug_msgs = False
	
	# S1Buffer (Variable)
	addr = 0x10020140
	spmRange = AddrRange(addr, addr + 0x200)
	clstr.s1buffer = ScratchpadMemory(range = spmRange)
	clstr.s1buffer.conf_table_reported = False
	clstr.s1buffer.ready_mode = True
	clstr.s1buffer.reset_on_scratchpad_read = True
	clstr.s1buffer.read_on_invalid = False
	clstr.s1buffer.write_on_valid = True
	clstr.s1buffer.port = clstr.local_bus.mem_side_ports
	
	# Connecting S1Buffer to S1
	for i in range(10):
		clstr.s1.spm = clstr.s1buffer.spm_ports
	
	# S1Out (Stream Variable)
	addr = 0x10020380
	clstr.s1out = StreamBuffer(stream_address = addr, status_address= 0x100203c0, stream_size = 8, buffer_size = 8)
	clstr.s1.stream = clstr.s1out.stream_in
	clstr.s2.stream = clstr.s1out.stream_out
	
	
	# s2 Config
	clstr.s2.pio = clstr.local_bus.mem_side_ports
	clstr.s2.enable_debug_msgs = False
	
	# S2Out (Stream Variable)
	addr = 0x10020440
	clstr.s2out = StreamBuffer(stream_address = addr, status_address= 0x10020480, stream_size = 8, buffer_size = 8)
	clstr.s2.stream = clstr.s2out.stream_in
	clstr.s3.stream = clstr.s2out.stream_out
	
	
	# s3 Config
	clstr.s3.pio = clstr.local_bus.mem_side_ports
	clstr.s3.stream = clstr.streamdma.stream_in
	clstr.s3.enable_debug_msgs = False
	
def makeHWAcc(args, system):

	system.vector_stream_clstr = AccCluster()
	buildvector_stream_clstr(args, system, system.vector_stream_clstr)

