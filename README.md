# RDscan
![bin](https://img.shields.io/badge/version-v1.0.0-green)

---------------------------------------------------------------------

## License terms<br>
RDscan software is freely available for academic purposes and a license is required for a commercial use. Please contact service@syntekabio.com regarding on the license.

---------------------------------------------------------------------
## Installation<br>
![tested](https://img.shields.io/badge/Tested-centos:6-blue)
![tested](https://img.shields.io/badge/Tested-ubuntu:16-blue)

Since the precompiled libraries in lib/ folder are made in CentOS environment, we recommend you to make file in CentOS.<br>
Or, you can simply use the compiled exe file located in bin/rdscan_v1.0.0<br>

### Prerequisites
```console
$ yum install gcc gcc-c++ make 
$ yum install zlib-devel
```

### Make
```console
$ git clone https://github.com/SyntekabioTools/RDscan.git

$ cd ./v1.0.0/src/

$ make
```

## User Guide<br>
The rd_scan file is in bin/

__1. Single Sample__
```console
$ ./rd_scan –b [Bam file] -r [reference file] -i [input]
```
__2. Paired Samples__
```console
$ ./rd_scan –b [Case bam file] -n [Control bam file] -r [reference file] -i [input]
```

__OPTIONs__

1. Input format<br>
<code>-f ['VCF' or 'ADIscan' or 'VARscan'], default='VCF'</code>
          

2. \# of Thread<br>
<code>-t [unsigned int], default=1</code>