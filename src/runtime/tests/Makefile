C_FILES := $(wildcard *.c)

test: $(C_FILES)
	clang $^ -lbask_runtime -g3 -Wall

clean:
	rm *.o
