 /**************************************************************************
  * *
  * *       Syntekabio, inc.
  * *
  * *  Copyright(c) 2018 Syntekabio, inc., 187, Techro 2-Gil, 
  * *  Yuseong-Gu, Daejeon, Korea.
  * *  All rights are reserved. No part of this work covered by the copyright
  * *  may be reproduced, stored in retrieval systems, in any form or by any
  * *  means, electronics, mechanical, photocopying, recording or otherwise,
  * *  without the prior permission of Syntekabio.
  * *
  * *  FILE NAME : main.cc
  * *  LAST MODIFIED DATE : 2018-12-29
  * *
  * *  AUTHOR : Sunho Lee
  * *  E-MAIL : shlee@syntekabio.com
  * *
  * *  DESCRIPTION : source file for RDscan
  * *
  * **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iomanip>
#include <iostream>
#include <getopt.h>
#include <stdexcept>

//#include "../header/bam_to_fastq.h"
//#include "../header/gen_fasta.h"
//#include "../header/determine_seq.h"
#include "../header/rdscan.h"
//#include "../header/fisher.h"


using namespace std;

int parse_opt(const int argc, char **argv, int &nStatus, string &sBamFile, string &sBamFileN, string &sRefFile, 
	string &sInputFile, string &sOutputFile, int &nCntThread, bool &bIsDebug);
bool manual()
{
	cout << "====================================== RDscan manual ====================================" << endl;
	cout << "1. COMMANDs"											<< endl;
	cout << "  (1) Single Sample" 									<< endl;
    cout << "      ./rd_scan -b ${Bam} -r ${Ref Fasta} -i ${Vcf} -o ${Output Vcf}" 	<< endl;
	cout << "  (2) Paired Samples" 									<< endl;
	cout << "      ./rd_scan -b ${Tumor Bam} -n ${Normal Bam} -r ${Ref Fasta} -i ${Vcf} -o ${Output Vcf}" 	<< endl;

	cout << endl;
	cout << "2. OPTIONs" << endl;
	cout << "  (1) # of Thread			-t	[unsigned int], default=1"				<< endl;
	cout << "============================================================================================" << endl << endl;

	return true;
}


int main(int argc, char* argv[])
{
	int nStatus = 1;			// 1:vcf 
	string sBamFile = "";			//bam file
	string sBamFileN = "";
	string sRefFile;
	string sInputFile;
	string sOutputFile;
	int nCntThread = 1;
	bool bIsDebug = false;


	try
	{
		parse_opt(argc, argv, nStatus, sBamFile, sBamFileN, sRefFile, sInputFile, sOutputFile, nCntThread, bIsDebug); 
		if(nStatus == 0)			manual();
		else
		{
			if(nStatus == 3 && sBamFileN == "")		throw std::logic_error("VARscan is available with paired samples");

			//construct class & init
			CRD RD(nStatus, sBamFile, sBamFileN, sRefFile, sInputFile, sOutputFile, nCntThread, bIsDebug);

			RD.PrintCommonInfo();
			cout << endl;

			//read files (ref, input)
			RD.ReadInput();
			
			//calculate read distribution
			RD.CalcDist();		
			
			//report
			RD.Report();
		}
	}
	catch (std::logic_error const& s)
	{
		cout << s.what();
		exit(EXIT_FAILURE);
	}
	catch (bad_alloc &ba)
	{
		cout << ba.what() << endl;
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		cout << "ERROR: Unexpected Exception" << endl;
		exit(EXIT_FAILURE);
	}


	return true;
}


int parse_opt(const int argc, char **argv, int &nStatus, string &sBamFile, string &sBamFileN, string &sRefFile, 
	string &sInputFile, string &sOutputFile, int &nCntThread, bool &bIsDebug)
{
	bool bBOption = false;
	bool bNOption = false;
	bool bROption = false;
	bool bIOption = false;
	bool bQOption = false;
	bool bOOption = false;

	struct option long_options[] =
	{
		{"bamFile", 		1, 0, 'b'},		//0
		{"bamFileN",		1, 0, 'n'},
		{"refFile",			1, 0, 'r'},		//1
		{"input",			1, 0, 'i'},		//3
		{"output",			1, 0, 'o'},
		{"input format",	1, 0, 'f'},		//4	
		{"num_thread", 		1, 0, 't'},		//8
		{0, 0, 0, 0}
	};

	int c;
	
	while (1) 
	{   
		int option_index = 0;
		c = getopt_long(argc, argv, "b:n:r:i:o:f:t:q", long_options, &option_index);
		if (c == -1)    break;
		char *pcEnd;
		switch (c) 
		{   
			case 'b':
				bBOption = true;
				sBamFile = optarg;
				break;
			case 'n':
				bNOption = true;
				sBamFileN = optarg;
				break;
			case 'r':
				bROption = true;
				sRefFile = optarg;
				break;
			case 'i':
				bIOption = true;
				sInputFile = optarg;
				break;
			case 'o':
				bOOption = true;
				sOutputFile = optarg;
				break;
			case 'f':
				/*
				if((string)optarg == "VCF")			nStatus = 1;
				else if((string)optarg == "ADIscan")	nStatus = 2;
				else if((string)optarg == "VARscan")	nStatus = 3;
				else							nStatus = 0;
				*/
				break;
			case 't':
				nCntThread = strtol(optarg, &pcEnd, 10);
				break;
			case 'q':
				bIsDebug = true;
				break;
		}
	}

	if(!bBOption || !bROption || !bIOption || !bOOption)		nStatus = 0;
	return true;
}










