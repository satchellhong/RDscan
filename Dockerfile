FROM ubuntu:18.04

ENV DEBIAN_FRONTEND=noninteractive

# copy git repository into the image
RUN mkdir -p /rdscan
COPY . /rdscan/
WORKDIR /rdscan/

RUN apt-get update -y \
&& apt-get install -y build-essential zlib1g-dev

RUN bash build.sh

ENTRYPOINT ["/rdscan/rdscan/bin/rd_scan_v1.0.1"]
