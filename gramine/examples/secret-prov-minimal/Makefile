ARCH_LIBDIR ?= /lib/$(shell $(CC) -dumpmachine)

GRAMINE_LOG_LEVEL = error
CFLAGS += -O0 
CFLAGS += -fPIE
LDFLAGS += -pie

.PHONY: all
all: app dcap

.PHONY: app
app: ssl/server.crt client.manifest.sgx client.sig

.PHONY: dcap
dcap: ssl/server.crt server_dcap

############################# SSL DATA DEPENDENCY #############################

# SSL data: key and x.509 self-signed certificate
ssl/server.crt: ssl/ca_config.conf
	openssl genrsa -out ssl/ca.key 2048
	openssl req -x509 -new -nodes -key ssl/ca.key -sha256 -days 1024 -out ssl/ca.crt -config ssl/ca_config.conf
	openssl genrsa -out ssl/server.key 2048
	openssl req -new -key ssl/server.key -out ssl/server.csr -config ssl/ca_config.conf
	openssl x509 -req -days 360 -in ssl/server.csr -CA ssl/ca.crt -CAkey ssl/ca.key -CAcreateserial -out ssl/server.crt

CFLAGS += -Wall -std=c11 $(shell pkg-config --cflags secret_prov_gramine)
LDFLAGS += -Wl,--enable-new-dtags $(shell pkg-config --libs secret_prov_gramine)

server_dcap: server.c
	$(CC) $< $(CFLAGS) $(LDFLAGS) -Wl,--no-as-needed -lsgx_urts -lsecret_prov_verify_dcap -pthread -o $@

client: client.c
	$(CC) $< $(CFLAGS) $(LDFLAGS) -o $@

client.manifest: client.manifest.template client
	gramine-manifest \
		-Dlog_level=$(GRAMINE_LOG_LEVEL) \
		-Darch_libdir=$(ARCH_LIBDIR) \
		$< > $@

client.manifest.sgx client.sig &: client.manifest
	gramine-sgx-sign \
		--manifest $< \
		--output $<.sgx

.PHONY: clean
clean:
	$(RM) OUTPUT
	$(RM) client server_* *.sig *.manifest.sgx *.manifest

.PHONY: distclean
distclean: clean
	$(RM) -f ssl/ca.* ssl/server.*

