C_FILES := $(wildcard *.c)

test: $(C_FILES)
	clang $^ -lbask_runtime -g3

clean:
	rm *.o
