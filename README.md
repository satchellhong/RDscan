# RDscan
![bin](https://img.shields.io/badge/version-v1.0.1-green)

---------------------------------------------------------------------

## License terms<br>
RDscan software is freely available for academic purposes and a license is required for a commercial use. Please contact service@syntekabio.com regarding on the license.

---------------------------------------------------------------------
## Installation<br>
![tested](https://img.shields.io/badge/Tested-centos-blue)
![tested](https://img.shields.io/badge/Tested-ubuntu-blue)

### Prerequisites
1. gcc/g++
2. zlib<br>
`RHEL/Centos` : zlib-devel<br>
`Debian/Ubuntu` : zlib1g-dev

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
The rd_scan binary executable file is in bin/

__1. Single Sample__
### To run
```console
$ ./rd_scan_v1.0.1 –b [Bam file] -r [reference file] -i [input]
```
### To run in the Docker image
The file path should be an absolute path.
```console
$ docker run -it --rm -v /:/mnt paramost/rdscan –b /mnt/[Bam file] -r /mnt/[reference file] -i /mnt/[input]
```
__2. Paired Samples__
### To run
```console
$ ./rd_scan_v1.0.1 –b [Case bam file] -n [Control bam file] -r [reference file] -i [input]
```
### To run in the Docker image
The file path should be an absolute path.
```console
$ docker run -it --rm -v /:/mnt paramost/rdscan –b /mnt/[Case bam file] -n /mnt/[Control bam file] -r /mnt/[reference file] -i /mnt/[input]
```

__OPTIONs__

1. Input format<br>
`-f ['VCF' or 'ADIscan' or 'VARscan'], default='VCF'`
          

2. \# of Thread<br>
`-t [unsigned int], default=1`