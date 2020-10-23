#ifndef DETERMINE_SEQ_H_
#define DETERMINE_SEQ_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "./bio_files.h"

using namespace std;


struct MUTATION
{
	unsigned long ulStartPos;
	unsigned long ulEndPos;
	char cOp;	//I:indel, D:delete, M:mismatch
	string sSeq;
};

struct MUTATION_PER_READ
{
	vector<MUTATION> vMutation;	
};

struct AGG_MUTATION
{
	MUTATION Mutation;
	unsigned long ulCntMutation;
	vector<unsigned long> vulRelativeReadIdx;
};

struct DIFF
{
	vector<uint8_t> v8CntMismatch;
	vector<uint8_t> v8CntMatch;
	vector<uint8_t> v8CntInsert;
	vector<uint8_t> v8CntInsertRegion;
	vector<uint8_t> v8CntDelete;
	vector<uint8_t> v8CntDeleteRegion;
	vector<uint8_t> v8CntSoftClip;
	vector<uint8_t> v8CntHardClip;

	unsigned long ulCntMutation;
	vector<MUTATION_PER_READ> vMutationPerRead;
	
	
	vector<AGG_MUTATION> vAggMutation;	//distinct mutation per each bamfile
};

struct ALIGN_INFO
{
	//for each read
	vector<unsigned long> vulStartPos;
	vector<unsigned long> vulEndPos;
};


class CDETERMINE_SEQ
{
private:
	CLIST_FILE m_ListFile;
	
	vector<double> m_vdAveDepth;
	
	vector<CFA_FILE> m_vFaFile;
	vector<CBAM_FILE> m_vBamFile;
	vector<DIFF> m_vDiff;

	vector<double> m_vdScore;
	vector<ALIGN_INFO> m_vAlign;
	vector<string> m_vsSeq;

	vector<string> m_sResult;
	vector<string> m_sNewResult;

public:
	bool ReadListFile(string sListBamFile);
	bool ReadBamFile();
	bool GetReadInfo(unsigned long ulExtraLen);
	bool CalcScore(unsigned long ulExtraLen, unsigned long ulMismatchCutoff, double dDivider);
	bool ExtractHighScoreAllele(unsigned long ulExtraLen, unsigned long ulMismatchCutoff, double dScoreCutoff);	
	bool OutputAll(unsigned long ulExtraLen, unsigned long ulMismatchCutoff);

	bool OutputHigh();
private:
	double ScoreFunction(unsigned long ulLenZero, double dDivider);
	


};










#endif
