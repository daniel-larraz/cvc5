FROM arm64v8/ubuntu:20.04

ARG OTHER_OPTS
ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
  automake \
  cmake \
  g++ \
  libcln-dev \
  libgmp-dev \
  libtool \
  openjdk-8-jdk \
  python3 \
  python3-pip \
  python3-venv \
  && rm -rf /var/lib/apt/lists/*

COPY . /cvc5
WORKDIR /cvc5

RUN ./configure.sh --auto-download --prefix=./build/install \
    --gpl --cln --cocoa --glpk ${OTHER_OPTS}
WORKDIR /cvc5/build

RUN make -j`nproc`
RUN make install