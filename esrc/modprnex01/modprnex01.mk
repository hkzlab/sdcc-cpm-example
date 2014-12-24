example-modprnex01: $(BIN_DIR)/modprnex01.com

$(BIN_DIR)/modprnex01.com:	tools $(BIN_DIR)/modprnex01.ihx
	$(LBIN_DIR)/load $(BIN_DIR)/modprnex01

$(BIN_DIR)/modprnex01.ihx:	libraries $(BIN_DIR)/modprnex01.rel $(BIN_DIR)/modprnex01.arf 
	$(CLD) $(CLD_FLAGS) -nf $(BIN_DIR)/modprnex01.arf

$(BIN_DIR)/modprnex01.rel: $(ESRC_DIR)/modprnex01/modprnex01.c
	$(CCC) $(CCC_FLAGS) -o $(BIN_DIR) $(ESRC_DIR)/modprnex01/modprnex01.c

$(BIN_DIR)/modprnex01.arf:	$(BIN_DIR)/generic.arf
	$(QUIET)$(SED) 's/$(REPLACE_TAG)/modprnex01/' $(BIN_DIR)/generic.arf > $(BIN_DIR)/modprnex01.arf 
