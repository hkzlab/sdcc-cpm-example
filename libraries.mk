# Here begins the actual creation of destination files
libraries: $(BIN_DIR)/cprintf.rel $(BIN_DIR)/cpm0.rel $(BIN_DIR)/cpmbdos.rel $(BIN_DIR)/ansi_term.rel $(BIN_DIR)/cpm_sysfunc.rel 

libraries-clean:
	rm $(BIN_DIR)/*

$(BIN_DIR)/cprintf.rel: $(SYSLIB_SRC_DIR)/cprintf.c
	$(CCC) $(CCC_FLAGS) -o $(BIN_DIR) $(SYSLIB_SRC_DIR)/cprintf.c

$(BIN_DIR)/cpm_sysfunc.rel: $(SYSLIB_SRC_DIR)/cpm_sysfunc.c
	$(CCC) $(CCC_FLAGS) -o $(BIN_DIR) $(SYSLIB_SRC_DIR)/cpm_sysfunc.c

$(BIN_DIR)/ansi_term.rel: $(SYSLIB_SRC_DIR)/ansi_term.c
	$(CCC) $(CCC_FLAGS) -o $(BIN_DIR) $(SYSLIB_SRC_DIR)/ansi_term.c

$(BIN_DIR)/cpmbdos.rel:	$(SRC_DIR)/cpm/cpmbdos.c
	$(CCC) $(CCC_FLAGS) -o $(BIN_DIR) $(SRC_DIR)/cpm/cpmbdos.c

# Build CP/M-80 Command File Structure files
$(BIN_DIR)/cpm0.rel: $(CPM_SRC_DIR)/cpm0.rel
	$(CAS) $(CAS_FLAGS) $(BIN_DIR)/cpm0.o $(CPM_SRC_DIR)/cpm0.s
	$(QUIET)$(COPY) $(CPM_SRC_DIR)/cpm0.rel $(BIN_DIR)
	$(QUIET)$(COPY) $(CPM_SRC_DIR)/cpm0.lst $(BIN_DIR)
