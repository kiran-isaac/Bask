# get all folders exept build
MODULES=$(filter-out build/, $(wildcard */))
SOURCES=$(wildcard */*.c)

# to build a module, we need to build all the .bc files in the module
# and then link them together

# build all modules
# this will build all the modules in the lib folder
all: $(MODULES)

# build a module. Run the Makefile in the module folder
# this will build all the .bc files in the module

# this is a pattern rule. It says that to build a module, we need to build all the .bc files in the module
# and then link them together
$(MODULES): 
	$(MAKE) -C $@ all

# clean all modules
clean:
	$(foreach module, $(MODULES), $(MAKE) -C $(module) clean;)
	rm -rf build