GRAMINE_LOG_LEVEL ?= error
ARCH_LIBDIR ?= /lib/$(shell $(CC) -dumpmachine)

.PHONY: build
build: sealing.manifest.sgx sealing.sig 

CFLAGS += -Wall -std=c11 $(shell pkg-config --cflags secret_prov_gramine)
LDFLAGS += -Wl,--enable-new-dtags $(shell pkg-config --libs secret_prov_gramine)

app: main.c
	$(CC) $< $(CFLAGS) $(LDFLAGS) -o $@

sealing.manifest.sgx sealing.sig &: sealing.manifest
	gramine-sgx-sign \
		--manifest $< \
		--output $<.sgx

sealing.manifest: sealing.manifest.template
	gramine-manifest \
		-Dlog_level=$(GRAMINE_LOG_LEVEL) \
		-Darch_libdir=$(ARCH_LIBDIR) \
		$< >$@

.PHONY: dcap
dcap: ssl/server.crt server_dcap

# SSL data: key and x.509 self-signed certificate
ssl/server.crt: ssl/ca_config.conf
	openssl genrsa -out ssl/ca.key 2048
	openssl req -x509 -new -nodes -key ssl/ca.key -sha256 -days 1024 -out ssl/ca.crt -config ssl/ca_config.conf
	openssl genrsa -out ssl/server.key 2048
	openssl req -new -key ssl/server.key -out ssl/server.csr -config ssl/ca_config.conf
	openssl x509 -req -days 360 -in ssl/server.csr -CA ssl/ca.crt -CAkey ssl/ca.key -CAcreateserial -out ssl/server.crt

server_dcap: server.c
	$(CC) $< $(CFLAGS) $(LDFLAGS) -Wl,--no-as-needed -lsgx_urts -lsecret_prov_verify_dcap -pthread -o $@

.PHONY: clean
clean:
	$(RM) *.manifest *.manifest.sgx *.sig app server_dcap
