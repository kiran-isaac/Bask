C_FILES := $(wildcard *.c)
LL_FILES_FROM_C_FILES := $(patsubst %.c, %.ll, $(C_FILES))
LL_FILES := $(wildcard *.ll) $(LL_FILES_FROM_C_FILES)
BC_FILES := $(patsubst %.ll, %.bc, $(LL_FILES))

libbask.bc : libbask.ll
	llvm-as  $< -o libbask.bc

libbask.ll: $(LL_FILES)
	llvm-link -S $^ -o $@

%.ll: %.c
	clang -S -emit-llvm $< -o $@

clean:
	rm -f *.bc
	rm -f libbask.ll
	rm -f $(LL_FILES_FROM_C_FILES)
