#ifndef PILE_UP_H_
#define PILE_UP_H_

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <stdexcept>
#include <iomanip>
#include <tr1/unordered_map>
#include "stdlib.h"
#include <algorithm>
#include <numeric>
#include <ctime>
#include <time.h>

#include "./alglib.h"
#include "./bam/bam.h"
#include "./bio_files.h"
#include "./util.h"
#include "./suffix_array.h"


#define EXTRA_LEN	500

using namespace std;

class CRD;

struct DIST_THREAD
{
	CRD *RD;
	int nId;
	int nSIdx;
	int nEIdx;
};


struct VCF
{
	//raw data
	vector<string> vsHeader;
	string sFormat;
	vector<string> vsChr;
	vector<int>	vnPos;
	vector<string> vsId;
	vector<string> vsRef;
	vector<string> vsAlt;
	vector<string> vsQual;
	vector<string> vsFilter;
	vector<string> vsInfo;
	vector<string> vsEtc;
	
};

struct RDscan
{
	vector<double> vdCorr;			//pearson corr
	vector<double> vdPval;		//pvalue
	vector<double> vdVafT;
	vector<double> vdVafN;
	vector<int> vnVarT;
	vector<int> vnVarN;
};

class CRD
{
public:
	bool m_bIsDebug;
	int m_nStatus;				//1:vcf, 2:adiscan, 3:strelka, 4:varscan
	string m_sBamFile;
	string m_sBamFileN;
	string m_sRefFile;
	string m_sInputFile;
	string m_sOutputFile;
	int m_nCntThread;

	CFA_FILE m_FaFile;			//reference file
	
	RDscan m_rdscan;			//rdscan (output)

	//for util
	clock_t m_clockStart;
	clock_t m_clockEnd;
	struct timespec m_tspecStart;
	struct timespec m_tspecEnd;

public:
	CRD(int, string, string, string, string, string, int, bool);
	bool ReadInput();
	bool CalcDist();
	bool Report();
	
	int PrintCommonInfo();

private:
	// input
	VCF m_vcf;
	//ADIscan m_adiscan;
	//VARscan m_varscan;


	// read input files
	bool ReadVcf();
//	bool ReadAdiscan();
//	bool ReadVarscan();
	
	// calc dist
	void* AlleleDist(int);
	static void *AlleleDist_helper(void *object)
	{
		DIST_THREAD *my = (DIST_THREAD*)object;
		my->RD->AlleleDist(my->nId);
	}
	int ConvertChrToTid(string, bam_header_t*);
	bool GetReadAlign(int &, int &, string &, int, string, string, bam1_t*);
	bool CheckRepeat(string, string, string, int, int &, int &);
	bool AlleleCount(bam_index_t *, bamFile &, int, int, string, string, int, int, int &, vector<int> &, vector<int> &, vector<int> &, vector<int> &, vector<string> &, vector<string> &);
	//double CalcCoef(string sChr, vector<string> &, vector<int> &, vector<int> &);
	bool FilterRead(string, vector<int> &, vector<int> &, vector<int> &, vector<int> &, vector<string> &, vector<string> &, int &, int &, int &, int &);

	// report 
	bool ReportVcf();
//	bool ReportAdiscan();
//	bool ReportVarscan();


	// library
	bool Parsing(vector<string> &, string, string);
	bool CalcPearsonCorr(int*, int*, double&, double&);

	//util
	void SetStartTime()
	{		
		//m_clockStart = clock();
		clock_gettime(CLOCK_REALTIME, &m_tspecStart);
	};
	int ViewStatus(int nCurr, int nTotal, string sId, bool bIsNoDel=false);
	string to_string(int);
	string to_string(unsigned long);
	string to_string(uint16_t);
	string to_string(double);
};


#endif

