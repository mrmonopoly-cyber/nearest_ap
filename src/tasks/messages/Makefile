PROTO_FILES := $(wildcard *.proto)
ODIR := out
PWD := $(shell pwd)

build_protobuf_messages:
	$(info $(PWD) files: $(PROTO_FILES))
	@mkdir -p $(ODIR)
	protoc $(PROTO_FILES) --cpp_out=$(ODIR)

clean_protobuf_messages:
	rm -rf $(ODIR)
