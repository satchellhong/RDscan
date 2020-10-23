FROM ubuntu:18.04

ENV DEBIAN_FRONTEND=noninteractive

# copy git repository into the image
RUN mkdir -p /run
COPY . /run/
WORKDIR /run/

RUN ./build.sh

WORKDIR /

ENTRYPOINT ["/run/rdscan/bin/rd_scan"]
