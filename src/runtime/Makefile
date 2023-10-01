C_FILES := $(wildcard *.c)
TEST_FILES := $(wilcard tests/*.c)

libbask_runtime.so: $(C_FILES)
	clang  -shared -o $@ $^ -g3
	sudo rm -f /usr/lib/bask/$@
	sudo cp $@ /usr/lib/$@
	sudo cp -r ./include/* /usr/include/bask
	
clean:
	rm -f *.so