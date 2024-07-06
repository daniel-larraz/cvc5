FROM arm64v8/ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
  automake \
  cmake \
  g++ \
  libcln-dev \
  libgmp-dev \
  libtool \
  python3 \
  python3-pip \
  python3-venv \
  && rm -rf /var/lib/apt/lists/*

COPY . /cvc5
WORKDIR /cvc5

RUN ./configure.sh --auto-download --gpl --cln --cocoa --glpk
WORKDIR /cvc5/build

RUN make -j`nproc`

