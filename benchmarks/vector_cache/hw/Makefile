KERN=vector_cache.c
build: $(FILES)
	clang -O1 -S -fno-slp-vectorize -fno-vectorize -fno-unroll-loops -fno-inline -fno-builtin -emit-llvm $(KERN)

clean:
	rm -f *.ll
