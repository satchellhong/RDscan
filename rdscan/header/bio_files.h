/**************************************************************************
*
*       Syntekabio, inc.
*
*  Copyright(c) 2016 Syntekabio, inc., 187, Techro 2-Gil, 
*  Yuseong-Gu, Daejeon, Korea.
*  All rights are reserved. No part of this work covered by the copyright
*  may be reproduced, stored in retrieval systems, in any form or by any
*  means, electronics, mechanical, photocopying, recording or otherwise,
*  without the prior permission of Syntekabio.
*
*  FILE NAME : bio_files.h
*  LAST MODIFIED DATE : 2016-01-30
*
*  AUTHOR : Sunho Lee
*  E-MAIL : shlee@syntekabio.com
*
*  DESCRIPTION : header file for library of reading bio files

**************************************************************************/

#ifndef BIO_FILES_H_
#define BIO_FILES_H_


//header files of .bam .sam
#include "./bam/bam2bcf.h"
#include "./bam/bam_endian.h"
#include "./bam/bam.h"
#include "./bam/bgzf.h"
#include "./bam/errmod.h"
#include "./bam/faidx.h"
#include "./bam/kaln.h"
#include "./bam/khash.h"
#include "./bam/knetfile.h"
#include "./bam/kprobaln.h"
#include "./bam/kseq.h"
#include "./bam/ksort.h"
#include "./bam/kstring.h"
#include "./bam/razf.h"
#include "./bam/sam.h"
#include "./bam/sam_header.h"
#include "./bam/sample.h"
#include "./bam/bcftools/bcf.h"

#include "./bam/bam_file.h"

//header files of .key .dp
#include "./synteka/key_dp_file.h"
#include "./synteka/list_file.h"
#include "./synteka/fa_file.h"
#include "./synteka/iupac_file.h"

#endif

