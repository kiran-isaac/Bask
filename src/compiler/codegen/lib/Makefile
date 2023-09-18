LL_FILES := $(wildcard *.ll)
BC_FILES := $(patsubst %.ll, %.bc, $(LL_FILES))

lib.bc: $(BC_FILES)
	llvm-link $^ -o lib.bc

test:
	clang -S -emit-llvm test1.c
	clang -S -emit-llvm test2.c

%.bc: %.ll
	llvm-as	 $< -o $@

%.ll: %.c
	clang -S -emit-llvm $< -o $@

clean:
	rm -f *.bc