LL_FILES := $(wildcard *.ll)
BC_FILES := $(patsubst %.ll, %.bc, $(LL_FILES))

lib.bc: $(BC_FILES)
	llvm-link -S $^ -o lib.ll
	llvm-as lib.ll -o lib.bc

%.bc: %.ll
	llvm-as	 $< -o $@

%.ll: %.c
	clang -S -emit-llvm $< -o $@

clean:
	rm -f *.bc