include lib/Makefile

test: lib/lib.bc
	llvm-as test.ll -o test.bc
	llvm-link test.bc lib/lib.bc -o test.bc
	clang test.bc -o test

clean: 
	rm *.bc test
	rm lib/*.bc