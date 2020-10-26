# RDscan
![bin](https://img.shields.io/badge/version-v1.0.1-green)

---------------------------------------------------------------------

## License terms<br>
RDscan software is freely available for academic purposes and a license is required for a commercial use. Please contact service@syntekabio.com regarding on the license.

---------------------------------------------------------------------
## Installation<br>
![tested](https://img.shields.io/badge/Tested-centos-blue)
![tested](https://img.shields.io/badge/Tested-ubuntu-blue)

There are two ways to install RDscan.

### 1. Build
```console
$ git clone https://github.com/satchellhong/RDscan.git
$ cd rdscan
$ ./build.sh
```

### 2. Docker Image
You can get the image from [here](https://hub.docker.com/repository/docker/paramost/rdscan)

rdscan
```console
$ docker pull paramost/rdscan
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