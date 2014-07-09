fileop-empty: $(BIN_DIR)/fileop.com

$(BIN_DIR)/fileop.com:	tools $(BIN_DIR)/fileop.ihx
	$(LBIN_DIR)/load $(BIN_DIR)/fileop

$(BIN_DIR)/fileop.ihx:	libraries $(BIN_DIR)/fileop.rel $(BIN_DIR)/fileop.arf 
	$(CLD) $(CLD_FLAGS) -nf $(BIN_DIR)/fileop.arf

$(BIN_DIR)/fileop.rel: $(ESRC_DIR)/fileop/fileop.c
	$(CCC) $(CCC_FLAGS) -o $(BIN_DIR) $(ESRC_DIR)/fileop/fileop.c

$(BIN_DIR)/fileop.arf:	$(BIN_DIR)/generic.arf
	$(QUIET)$(SED) 's/$(REPLACE_TAG)/fileop/' $(BIN_DIR)/generic.arf > $(BIN_DIR)/fileop.arf 
