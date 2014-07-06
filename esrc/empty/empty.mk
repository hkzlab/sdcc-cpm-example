example-empty: $(BIN_DIR)/empty.com

$(BIN_DIR)/empty.com:	tools $(BIN_DIR)/empty.ihx
	$(LBIN_DIR)/load $(BIN_DIR)/empty

$(BIN_DIR)/empty.ihx:	libraries $(BIN_DIR)/empty.rel $(BIN_DIR)/empty.arf 
	$(CLD) $(CLD_FLAGS) -nf $(BIN_DIR)/empty.arf

$(BIN_DIR)/empty.rel: $(ESRC_DIR)/empty/empty.c
	$(CCC) $(CCC_FLAGS) -o $(BIN_DIR) $(ESRC_DIR)/empty/empty.c

$(BIN_DIR)/empty.arf:	$(BIN_DIR)/generic.arf
	$(QUIET)$(SED) 's/$(REPLACE_TAG)/empty/' $(BIN_DIR)/generic.arf > $(BIN_DIR)/empty.arf 
