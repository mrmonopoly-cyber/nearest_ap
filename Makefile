GENERATE_ALL_DOC := all_doc
GENERATE_DOX := dox_doc

CLEAN_ALL_DOC := clean
CLEAN_DOX_DOC := clean_dox

DOXYFILE := ./Doxyfile
DOXYDIR := ./doc/doxygen

.PHONY: $(GENERATE_ALL_DOC) $(GENERATE_DOX) $(CLEAN_ALL_DOC) $(CLEAN_DOX_DOC) help

$(GENERATE_ALL_DOC): $(GENERATE_DOX)

$(GENERATE_DOX): $(DOXYFILE)
	@mkdir -p $(DOXYDIR)
	doxygen $(DOXYFILE)

$(CLEAN_ALL_DOC): $(CLEAN_DOX_DOC)

$(CLEAN_DOX_DOC):
	rm -rf $(DOXYDIR)

help:
	$(info To generate all doc: make $(GENERATE_ALL_DOC))
	$(info To generate doxygen: make $(GENERATE_DOX))
	$(info ---------------------------------------------)
	$(info To clean all doc: make $(CLEAN_ALL_DOC))
	$(info To clean doxygen doc: make $(CLEAN_DOX_DOC))
