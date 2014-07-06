tools:	$(LBIN_DIR)/load

tools-clean:
	rm -f $(LBIN_DIR)/*

$(LBIN_DIR)/load: $(LSRC_DIR)/load.c
	$(CC) $(LSRC_DIR)/load.c -o $(LBIN_DIR)/load
