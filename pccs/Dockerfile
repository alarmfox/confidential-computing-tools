FROM ubuntu:24.04 AS build
WORKDIR /tmp

# dependencies
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
  git \
  curl \
  python-is-python3 \
  python3 \
  python3-pip \
  python3-setuptools \
  openssl \
  libssl-dev \
  build-essential

# configure intel sgx sdk
RUN curl -fsSLo /usr/share/keyrings/intel-sgx-deb.asc https://download.01.org/intel-sgx/sgx_repo/ubuntu/intel-sgx-deb.key && \
  echo "deb [arch=amd64 signed-by=/usr/share/keyrings/intel-sgx-deb.asc] https://download.01.org/intel-sgx/sgx_repo/ubuntu noble main" \
  | tee /etc/apt/sources.list.d/intel-sgx.list

# install sgx sdk
RUN curl -O https://download.01.org/intel-sgx/sgx-dcap/1.22/linux/distro/ubuntu24.04-server/sgx_linux_x64_sdk_2.25.100.3.bin
RUN chmod +x sgx_linux_x64_sdk_2.25.100.3.bin
RUN ./sgx_linux_x64_sdk_2.25.100.3.bin --prefix /opt/intel/

# get source code
RUN git clone --recurse-submodules --depth=1 --branch DCAP_1.21 https://github.com/intel/SGXDataCenterAttestationPrimitives dcap

# build PCKCertSelection
WORKDIR /tmp/dcap/tools/PCKCertSelection

RUN . /opt/intel/sgxsdk/environment && \
  make && \
  mkdir /tmp/dcap/QuoteGeneration/pccs/lib && \
  cp /tmp/dcap/tools/PCKCertSelection/out/libPCKCertSelection.so /tmp/dcap/QuoteGeneration/pccs/lib

RUN RUN curl -sL https://deb.nodesource.com/setup_21.5 | bash -
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install nodejs npm

WORKDIR /tmp/dcap/QuoteGeneration/pccs

RUN npm install

FROM node:21.5
WORKDIR /app

COPY --from=build /tmp/dcap/QuoteGeneration/pccs  .

ENTRYPOINT [ "node", "pccs_server.js" ]
