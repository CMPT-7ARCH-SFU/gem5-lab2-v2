KERN=S3.c S2.c S1.c top.c
build: $(FILES)
	clang -O1 -S -fno-slp-vectorize -fno-vectorize -fno-unroll-loops -fno-inline -fno-builtin -emit-llvm $(KERN)

clean:
	rm -f *.ll
