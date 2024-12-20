CFLAGS=-Wall -Wextra -O0
GRAMINE_LOG_LEVEL?=debug

run: build
	gramine-sgx sqlite

.PHONY: build
build: sqlite.manifest.sgx sqlite.sig

sqlite.sig sqlite.manifest.sgx &: sqlite.manifest
	gramine-sgx-sign \
		--manifest $< \
		--output $<.sgx
	gramine-manifest-check $<.sgx

sqlite.manifest: sqlite.manifest.template sqlite
	gramine-manifest \
		-Dlog_level=$(GRAMINE_LOG_LEVEL) \
		$< $@
	gramine-manifest-check $@

sqlite: sqlite.o
	gcc -lsqlite3 -o $@ $<

sqlite.o: sqlite.c

.PHONY: clean
clean:
	$(RM) *.sig *.manifest.sgx *.manifest sqlite.o sqlite OUTPUT

.PHONY: distclean
distclean: clean
