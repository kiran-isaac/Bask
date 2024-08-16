SOURCES=$(wildcard *.c)
BUILD_LOCATION=../build/io
OBJECTS=$(patsubst %.c,$(BUILD_LOCATION)/%.bc,$(SOURCES))

$(shell mkdir -p $(BUILD_LOCATION))

all: $(OBJECTS)
	llvm-link -o ../build/io.bc $(OBJECTS)

$(BUILD_LOCATION)/%.bc: %.c
	clang -c -emit-llvm -o $@ $<