FROM ubuntu:latest

WORKDIR /tmp/wireshark

RUN apt-get update && \
  apt-get upgrade -y && \
  DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential pkg-config libglib2.0-dev \
    python3 python3-dev python3-pip \
    wireshark libwireshark-dev libwiretap-dev \
    less vim git && \
  apt-get autoremove && \
  pip3 install cython
