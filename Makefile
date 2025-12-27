GENERATE_ALL_DOC := all_doc
GENERATE_DOX := dox_doc
GENERATE_REPORT := report_doc

CLEAN_ALL_DOC := clean
CLEAN_DOX_DOC := clean_dox
CLEAN_REPORT_DOC := clean_report

DOXYFILE := ./Doxyfile
DOXYDIR := ./doc/doxygen

REPORT_DIR := ./doc/report


.PHONY: $(GENERATE_ALL_DOC) $(GENERATE_DOX) $(CLEAN_ALL_DOC) $(CLEAN_DOX_DOC) help

$(GENERATE_ALL_DOC): $(GENERATE_DOX) $(GENERATE_REPORT)

$(GENERATE_DOX): $(DOXYFILE)
	@mkdir -p $(DOXYDIR)
	doxygen $(DOXYFILE)

$(GENERATE_REPORT): 
	make -C $(REPORT_DIR) all

#=================================================================================

$(CLEAN_ALL_DOC): $(CLEAN_DOX_DOC) $(CLEAN_REPORT)

$(CLEAN_DOX_DOC):
	rm -rf $(DOXYDIR)

$(CLEAN_REPORT_DOC):
	make -C $(REPORT_DIR) clean

help:
	$(info To generate all doc: make $(GENERATE_ALL_DOC))
	$(info To generate doxygen doc: make $(GENERATE_DOX))
	$(info To generate report doc: make $(GENERATE_REPORT))
	$(info ---------------------------------------------)
	$(info To clean all doc: make $(CLEAN_ALL_DOC))
	$(info To clean doxygen doc: make $(CLEAN_DOX_DOC))
	$(info To clean report doc: make $(CLEAN_REPORT_DOC))

include doc/report/Makefile
