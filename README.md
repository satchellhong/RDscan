# RDscan
![bin](https://img.shields.io/badge/version-v1.1.0-green)

---------------------------------------------------------------------

## License terms<br>
RDscan software is freely available for academic purposes and a license is required for a commercial use. Please contact service@syntekabio.com regarding on the license.

---------------------------------------------------------------------
## Installation<br>
![tested](https://img.shields.io/badge/Tested-centos-blue)
![tested](https://img.shields.io/badge/Tested-ubuntu-blue)

### Download
Release version can be downloaded from here: https://github.com/satchellhong/RDscan/releases/download/paper-version/rd_scan_v1.1.0
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

## User Guide<br>
The rd_scan binary executable file is in bin/

__1. Single Sample__
#### To run
```console
$ ./rd_scan_v1.1.0 –b [Bam file] -r [reference fa file] -i [input vcf] -o [output vcf]
```

__2. Paired Samples__
#### To run
```console
$ ./rd_scan_v1.1.0 –b [tumor bam file] -n [normal bam file] -r [reference fa file] -i [input vcf] -o [output vcf]
```


__OPTIONs__

#### \# of Thread<br>
`-t [unsigned int], default=1`
