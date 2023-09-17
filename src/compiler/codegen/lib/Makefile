LL_FILES := $(wildcard *.ll)
BC_FILES := $(patsubst %.ll, %.bc, $(LL_FILES))

lib.bc: $(BC_FILES)
	llvm-link $^ -o lib.bc

%.bc: %.ll
	llvm-as	 $< -o $@