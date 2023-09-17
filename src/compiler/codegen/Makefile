include lib/Makefile

test: test.ll lib/lib.bc
	llvm-as test.ll -o test.bc
	llvm-link test.bc lib/lib.bc -o test.bc
	clang test.bc -o test

stringtest: stringtest.ll lib/lib.bc
	llvm-as stringtest.ll -o stringtest.bc
	llvm-link stringtest.bc lib/lib.bc -o stringtest.bc
	clang stringtest.bc -o stringtest

clean: 
	rm *.bc test