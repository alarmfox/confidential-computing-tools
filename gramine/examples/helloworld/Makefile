CFLAGS=-Wall -Wextra -O0
GRAMINE_LOG_LEVEL?=debug

run: build
	gramine-sgx helloworld	

.PHONY: build
build: helloworld.manifest.sgx helloworld.sig

helloworld.sig helloworld.manifest.sgx &: helloworld.manifest
	gramine-sgx-sign \
		--manifest $< \
		--output $<.sgx
	gramine-manifest-check $<.sgx

helloworld.manifest: helloworld.manifest.template helloworld
	gramine-manifest \
		-Dlog_level=$(GRAMINE_LOG_LEVEL) \
		$< $@
	gramine-manifest-check $@

helloworld: helloworld.o

helloworld.o: helloworld.c

.PHONY: check
check: all
	$(GRAMINE) helloworld > OUTPUT
	echo "Hello, world" | diff OUTPUT -
	@echo "[ Success ]"

.PHONY: clean
clean:
	$(RM) *.sig *.manifest.sgx *.manifest helloworld.o helloworld OUTPUT

.PHONY: distclean
distclean: clean
