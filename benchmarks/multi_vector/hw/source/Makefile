TARGET=vector2.ll vector.ll top.ll
%.o : %.c
	clang -emit-llvm -fno-slp-vectorize -fno-vectorize -fno-unroll-loops -fno-inline -fno-builtin -emit-llvm -c -o $@ $<
%.bc : %.o
	opt -O1  -mem2reg -simplifycfg -loops -loop-unroll -o $@ $<
%.ll : %.bc
	llvm-dis -o $@ $<
build : $(TARGET)
	mkdir -p ../ir/
	mv $^ ../ir/
clean:
	rm -rf ../ir/*.ll

