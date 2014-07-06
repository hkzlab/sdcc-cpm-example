REPLACE_TAG=REPLACE_ME_PLEASE

examples: $(BIN_DIR)/empty.com

examples-clean:
	rm -f $(BIN_DIR)/*.com
	rm -f $(BIN_DIR)/*.ihx
	rm -f $(BIN_DIR)/*.arf

$(BIN_DIR)/empty.com:	tools $(BIN_DIR)/empty.ihx
	$(LBIN_DIR)/load $(BIN_DIR)/empty

$(BIN_DIR)/empty.ihx:	libraries $(BIN_DIR)/empty.rel $(BIN_DIR)/empty.arf 
	$(CLD) $(CLD_FLAGS) -nf $(BIN_DIR)/empty.arf
	$(QUIET)$(MOVE) empty.ihx $(BIN_DIR)
	$(QUIET)$(MOVE) empty.map $(BIN_DIR)
	$(QUIET)$(MOVE) empty.noi $(BIN_DIR)

$(BIN_DIR)/empty.rel: $(ESRC_DIR)/empty/empty.c
	$(CCC) $(CCC_FLAGS) -o $(BIN_DIR) $(ESRC_DIR)/empty/empty.c

$(BIN_DIR)/empty.arf:	$(BIN_DIR)/generic.arf
	$(QUIET)$(SED) 's/$(REPLACE_TAG)/empty/' $(BIN_DIR)/generic.arf > $(BIN_DIR)/empty.arf 

$(BIN_DIR)/generic.arf:	
	$(QUIET)$(ECHO) Generating generic.arf
	$(QUIET)$(ECHO) -mjx > $(BIN_DIR)/generic.arf
	$(QUIET)$(ECHO) -i $(REPLACE_TAG).ihx >> $(BIN_DIR)/generic.arf
	$(QUIET)$(ECHO) -k $(COMPILER_LIBS) >> $(BIN_DIR)/generic.arf
	$(QUIET)$(ECHO) -l z80 >> $(BIN_DIR)/generic.arf
	$(QUIET)$(ECHO) $(BIN_DIR)/cpm0.rel >> $(BIN_DIR)/generic.arf
	$(QUIET)$(ECHO) $(BIN_DIR)/cpmbdos.rel >> $(BIN_DIR)/generic.arf
	$(QUIET)$(ECHO) $(BIN_DIR)/cprintf.rel >> $(BIN_DIR)/generic.arf
	$(QUIET)$(ECHO) $(BIN_DIR)/cpm_sysfunc.rel >> $(BIN_DIR)/generic.arf
	$(QUIET)$(ECHO) $(BIN_DIR)/ansi_term.rel >> $(BIN_DIR)/generic.arf
	$(QUIET)$(ECHO) $(BIN_DIR)/$(REPLACE_TAG).rel >> $(BIN_DIR)/generic.arf
	$(QUIET)$(ECHO) -e >> $(BIN_DIR)/generic.arf
