FROM ubuntu:20.04

WORKDIR /workspace

# install dependencies
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \ 
  gnupg2 \
  curl \
  make \
  build-essential

RUN echo 'deb [arch=amd64] https://download.01.org/intel-sgx/sgx_repo/ubuntu focal main' | tee /etc/apt/sources.list.d/intel-sgx.list
RUN curl https://download.01.org/intel-sgx/sgx_repo/ubuntu/intel-sgx-deb.key | apt-key add -

RUN echo "deb http://apt.llvm.org/focal/ llvm-toolchain-focal-11 main" | tee /etc/apt/sources.list.d/llvm-toolchain-focal-11.list
RUN curl https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -

RUN echo "deb [arch=amd64] https://packages.microsoft.com/ubuntu/20.04/prod focal main" | tee /etc/apt/sources.list.d/msprod.list
RUN curl https://packages.microsoft.com/keys/microsoft.asc | apt-key add -

# install openenclave-sdk
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install clang-11 \
  libssl-dev \
  gdb \
  libsgx-enclave-common \
  libsgx-quote-ex \
  libprotobuf17 \
  libsgx-dcap-ql \
  libsgx-dcap-ql-dev \
  az-dcap-client \
  open-enclave

RUN echo "source /opt/openenclave/share/openenclave/openenclaverc" >> /root/.bashrc

# download open-enclave-sdk for samples
RUN curl -L -o /tmp/v0.19.8.tar.gz https://github.com/openenclave/openenclave/archive/refs/tags/v0.19.8.tar.gz

RUN tar xvf /tmp/v0.19.8.tar.gz  
RUN mv openenclave-0.19.8 openenclave

# configure AESM - Architectural Enclaves Service Manager
RUN echo "#!/bin/sh \n \
  set -e \n \
  killall -q aesm_service || true \n \
  AESM_PATH=/opt/intel/sgx-aesm-service/aesm LD_LIBRARY_PATH=/opt/intel/sgx-aesm-service/aesm exec /opt/intel/sgx-aesm-service/aesm/aesm_service --no-syslog \n\
  " >> /restart_aesm.sh

RUN mkdir -p /var/run/aesmd
RUN chmod +x /restart_aesm.sh

# add user to sgx_prv to access remote attestation primitives
RUN groupadd sgx_prv
RUN usermod -aG sgx_prv root

ENTRYPOINT ["/bin/sh", "-c"]
CMD ["/restart_aesm.sh ; exec /bin/bash"]
