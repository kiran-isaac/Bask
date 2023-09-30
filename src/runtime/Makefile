C_FILES := $(wildcard *.c)


bask_runtime.so: $(C_FILES)
	clang  -shared -o $@ $^

test:
		

clean:
	rm *.so
