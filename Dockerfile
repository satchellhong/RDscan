FROM ubuntu:18.04

ENV DEBIAN_FRONTEND=noninteractive

# copy git repository into the image
RUN mkdir -p /rdscan
COPY . /rdscan/
WORKDIR /rdscan/

RUN bash build.sh

ENTRYPOINT ["/rdscan/rdscan/bin/rd_scan"]
