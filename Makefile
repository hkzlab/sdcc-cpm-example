include common.mk

all: tools libraries examples \
	example-empty fileop-empty example-modprnex01

clean:	tools-clean libraries-clean examples-clean 
	rm -f $(BIN_DIR)/*

include tools.mk # Local tools
include libraries.mk # Libraries
include examples.mk # Examples

include esrc/empty/empty.mk
include esrc/fileop/fileop.mk
include esrc/modprnex01/modprnex01.mk

