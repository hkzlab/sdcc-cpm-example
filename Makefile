COMPILER_PREFIX = /opt/local/libexec/sdcc29/
COMPILER_LIBS = $(COMPILER_PREFIX)/share/sdcc/lib/z80/
CCC = $(COMPILER_PREFIX)/bin/sdcc
CAS = $(COMPILER_PREFIX)/bin/as-z80
CLD = $(COMPILER_PREFIX)/bin/link-z80

CC = gcc

ECHO = echo
COPY = cp
MOVE = mv

SRC_DIR = src/
CPM_SRC_DIR = $(SRC_DIR)/cpm
BIN_DIR = bin/

LSRC_DIR = lsrc/
LBIN_DIR = lbin/

INCLUDE_DIR = $(SRC_DIR)/include

CCC_FLAGS = -c -mz80 -D__SDCC__=1 -I $(INCLUDE_DIR)
CAS_FLAGS = -plosff 
CLD_FLAGS = 

all: $(BIN_DIR)/hello.com

$(BIN_DIR)/hello.com:	$(LBIN_DIR)/load $(BIN_DIR)/hello.hex	
	$(LBIN_DIR)/load	$(BIN_DIR)/hello

$(BIN_DIR)/hello.hex:	$(BIN_DIR)/hello.ihx
	$(COPY)	$(BIN_DIR)/hello.ihx $(BIN_DIR)/hello.hex

$(BIN_DIR)/hello.ihx:	$(BIN_DIR)/hello.rel $(BIN_DIR)/hello.arf $(BIN_DIR)/cpm0.rel $(BIN_DIR)/cpmbdos.rel $(BIN_DIR)/cprintf.rel
	$(CLD) $(CLD_FLAGS) -nf $(BIN_DIR)/hello.arf
	$(MOVE) hello.ihx $(BIN_DIR)
	$(MOVE) hello.map $(BIN_DIR)
	$(MOVE) hello.sym $(BIN_DIR)

$(BIN_DIR)/hello.rel: $(SRC_DIR)/hello.c
	$(CCC) $(CCC_FLAGS) -o $(BIN_DIR) $(SRC_DIR)/hello.c

$(BIN_DIR)/hello.arf:	
	$(ECHO) -mjx > $(BIN_DIR)/hello.arf
	$(ECHO) -i hello.ihx >> $(BIN_DIR)/hello.arf
	$(ECHO) -k $(COMPILER_LIBS) >> $(BIN_DIR)/hello.arf
	$(ECHO) -l z80 >> $(BIN_DIR)/hello.arf
	$(ECHO) $(BIN_DIR)/cpm0.rel >> $(BIN_DIR)/hello.arf
	$(ECHO) $(BIN_DIR)/hello.rel >> $(BIN_DIR)/hello.arf
	$(ECHO) $(BIN_DIR)/cpmbdos.rel >> $(BIN_DIR)/hello.arf
	$(ECHO) $(BIN_DIR)/cprintf.rel >> $(BIN_DIR)/hello.arf
	$(ECHO) -e >> $(BIN_DIR)/hello.arf

$(BIN_DIR)/cprintf.rel: $(SRC_DIR)/cprintf.c
	$(CCC) $(CCC_FLAGS) -o $(BIN_DIR) $(SRC_DIR)/cprintf.c

# Build CP/M-80 Command File Structure files
$(BIN_DIR)/cpm0.rel: $(CPM_SRC_DIR)/cpm0.rel
	$(CAS) $(CAS_FLAGS) $(BIN_DIR)/cpm0.o $(CPM_SRC_DIR)/cpm0.s
	$(COPY) $(CPM_SRC_DIR)/cpm0.rel $(BIN_DIR)
	$(COPY) $(CPM_SRC_DIR)/cpm0.lst $(BIN_DIR)

$(BIN_DIR)/cpmbdos.rel: $(CPM_SRC_DIR)/cpmbdos.rel
	$(CAS) $(CAS_FLAGS) $(BIN_DIR)/cpmbdos.o $(CPM_SRC_DIR)/cpmbdos.s
	$(COPY) $(CPM_SRC_DIR)/cpmbdos.rel $(BIN_DIR)
	$(COPY) $(CPM_SRC_DIR)/cpmbdos.lst $(BIN_DIR)

# Build Host tools
$(LBIN_DIR)/load: $(LSRC_DIR)/load.c
	$(CC) $(LSRC_DIR)/load.c -o $(LBIN_DIR)/load

clean:
	rm $(BIN_DIR)/*
	rm $(LBIN_DIR)/*
