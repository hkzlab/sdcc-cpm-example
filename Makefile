include common.mk

all: tools libraries
clean:	tools-clean libraries-clean 

include tools.mk # Local tools
include libraries.mk # Libraries
include examples.mk # Examples

