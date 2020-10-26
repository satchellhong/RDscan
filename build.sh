#!/bin/bash
os=`cat /etc/os-release 2> /dev/null | awk '
NR==1{if($0~/.*[Cc][Ee][Nn][Tt][Oo][Ss].*/){print '1';}
else if($0~/.*[Uu][Bb][Uu][Nn][Tt][Uu].*/){print '2'}}
'`
if [ -z "$os" ]; then
	os=`cat /etc/issue.net 2> /dev/null | awk '
	NR==1{if($0~/.*[Cc][Ee][Nn][Tt][Oo][Ss].*/){print '1';}
	else if($0~/.*[Uu][Bb][Uu][Nn][Tt][Uu].*/){print '2'}}
	'`
fi

chsudo=""
if [[ $os == 1 ]]; then
	echo "CentOS"
	if [ $EUID -ne 0 ]; then
		chsudo="sudo "
	fi
	iscpp=`g++ --version 2> /dev/null`	
	if [ -z "$iscpp" ]; then
		echo "${chsudo}yum update -y"
		${chsudo}yum update -y
		echo "${chsudo}yum install -y gcc-c++"
		${chsudo}yum install -y gcc-c++
	fi	
	iszlib=`yum list installed zlib-devel 2>/dev/null | awk 'END{if($0~/^zlib-devel/)print 1;else print 0}'`
	if [[ $iszlib == 0 ]]; then
		echo "${chsudo}yum install -y zlib-devel"
		${chsudo}yum install -y zlib-devel	
	fi
	
elif [[ $os == 2 ]]; then
	echo "Ubuntu"
	if [ $EUID -ne 0 ]; then
		chsudo="sudo "
	fi
	iscpp=`g++ --version 2> /dev/null`	
	if [ -z "$iscpp" ]; then
		echo "${chsudo}apt-get update -y"
		${chsudo}apt-get update -y
		echo "${chsudo}apt-get install -y build-essential"
		${chsudo}apt-get install -y build-essential
	fi
	iszlib=`dpkg -s zlib1g-dev 2>/dev/null || echo 0`
	if [[ $iszlib == 0 ]]; then
		echo "${chsudo}apt-get install -y zlib1g-dev"
		${chsudo}apt-get install -y zlib1g-dev
	fi
else
	echo "[WARNING] Cannot configure operating system!"
fi

rootdir=`pwd`
mkdir -p ${rootdir}/rdscan/lib

# make libs
cd ${rootdir}/rdscan/header/alglib
make clean
make
cd ${rootdir}/rdscan/header/
make clean
make	

# make run file
cd ${rootdir}/rdscan/src
make clean
make
