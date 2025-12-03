PROTO_FILES := $(wildcard *.proto)
ODIR := out
PWD := $(shell pwd)
NANOPB_SETUP_DIR := nanopb
NANOPBDIR := nanopb_src
PROTOC = $(NANOPBDIR)/generator-bin/nanopb_generator
NANOPB_URL := https://github.com/nanopb/nanopb/releases/download/nanopb-0.4.9.1/nanopb-0.4.9.1-linux-x86.tar.gz
NANOPB_TAR += nanopb.tar.gz

setup_protobuf:
	wget  $(NANOPB_URL) -O$(NANOPB_TAR)
	tar -xzf $(NANOPB_TAR)
	mv nanopb-* $(NANOPB_SETUP_DIR)
	mkdir -p $(NANOPBDIR)
	cp  $(NANOPB_SETUP_DIR)/pb*.c $(NANOPBDIR)
	cp  $(NANOPB_SETUP_DIR)/pb*.h $(NANOPBDIR)
	cp -r $(NANOPB_SETUP_DIR)/generator-bin $(NANOPBDIR)
	rm -rf $(NANOPB_TAR) $(NANOPB_SETUP_DIR)

build_protobuf_messages:
	$(info $(PWD) files: $(PROTO_FILES))
	@mkdir -p $(ODIR)
	$(PROTOC) $(PROTO_FILES) --output-dir=$(ODIR)

clean_protobuf_messages:
	rm -rf $(ODIR)

clean_setup_protobuf:
	rm -rf $(NANOPB_SETUP_DIR)
	rm -rf pb_*.c pb*.h
	rm -rf $(NANOPB_TAR)
	rm -rf generator-bin
	rm -rf $(NANOPBDIR)

clean_all:
	clean_protobuf_messages
	clean_setup
