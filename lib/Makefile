SOURCES=$(wildcard */*.c)
BUILD_FOLDER=build
OBJECTS=$(patsubst %.c,$(BUILD_FOLDER)/%.bc,$(SOURCES))
LLVM_LINK=llvm-link
# All headers are bask files not in the build folder
HEADERS=$(filter-out $(wildcard build/*.bsk),$(wildcard */*.bsk))

# All folders except build and without a slash at the end
MODULES=$(filter-out build/,$(filter-out %/,$(dir $(wildcard */))))

$(info $(OBJECTS))

all: $(OBJECTS) $(MODULES)
	llvm-link -o $(BUILD_FOLDER)/lib.bc $(OBJECTS)
	cp $(HEADERS) $(BUILD_FOLDER)

IOSOURCES=$(wildcard io/*.c)
IOOBJECTS=$(patsubst %.c,$(BUILD_FOLDER)/%.bc,$(IOSOURCES))

io: $(IOOBJECTS)
	llvm-link -o $(BUILD_FOLDER)/io.bc $(IOOBJECTS)
	cp $(wildcard io/*.bsk) $(BUILD_FOLDER)

$(BUILD_FOLDER)/%.bc: %.c
	mkdir -p $(dir $@)
	clang -c -emit-llvm -o $@ $<

$(MODULES): %: $(BUILD_FOLDER)/%.bc
	MODULESOURCE=$(wildcard $@/*.c) 
	MODULEOBJECT=$(BUILD_FOLDER)/$@.bc
	clang -c -emit-llvm -o $(MODULEOBJECT) $(MODULESOURCE)