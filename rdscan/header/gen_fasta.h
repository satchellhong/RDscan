#ifndef GEN_FASTA_H_
#define GEN_FASTA_H_

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

class CGEN_FASTA
{
private:
	string m_sChr;
	string m_sStartPos;
	string m_sEndPos;

	string m_sRefSeq;
	string m_sRefPreSeq;
	string m_sRefPostSeq;

	vector<string> m_vsId;
	vector<string> m_vsSeq;

public:
	bool ReadSeqFrqFile(string sSeqFrqFile);
	bool GenerateFasta(string sOutputPath, unsigned long ulExtraLen);



};





#endif
