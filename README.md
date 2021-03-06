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
1. gcc/g++ >= 4.8.5
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
```shell
$ docker pull paramost/rdscan
```
or
```shell
$ cd <where rdscan Dockerfile is>
$ docker build -t paramost/rdscan -f Dockerfile .
```

## User Guide<br>
The rd_scan binary executable file is in bin/

__1. Single Sample__
#### To run
```console
$ ./rd_scan_v1.0.1 –b [Bam file] -r [reference fa file] -i [input vcf] -o [output vcf]
```
#### To run in the Docker image
The file path should be an absolute path.
```console
$ docker run -it --rm -v /:/mnt paramost/rdscan –b /mnt/[Bam file] -r /mnt/[reference fa file] -i /mnt/[input vcf] -o /mnt/[output vcf]
```
__2. Paired Samples__
#### To run
```console
$ ./rd_scan_v1.0.1 –b [tumor bam file] -n [normal bam file] -r [reference fa file] -i [input vcf] -o [output vcf]
```
#### To run in the Docker image
The file path should be an absolute path.
```console
$ docker run -it --rm -v /:/mnt paramost/rdscan –b /mnt/[tumor bam file] -n /mnt/[normal bam file] -r /mnt/[reference fa file] -i /mnt/[input] -o /mnt/[output vcf]
```

__OPTIONs__

#### \# of Thread<br>
`-t [unsigned int], default=1`
