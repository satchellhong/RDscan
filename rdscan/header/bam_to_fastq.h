#ifndef BAM_TO_FASTQ_H_
#define BAM_TO_FASTQ_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#include "./bam/bam.h"

using namespace std;





class CBAM_TO_FASTQ
{
private :
	bam_header_t *m_header;
	bam_index_t *m_idx;
	string m_sChr;
	unsigned long m_ulStartPos;
	unsigned long m_ulEndPos;



public :
	bool Init();
	bool ConvertBamToFastQ(string sBamFile, string sChr, unsigned long ulStartPos, unsigned long ulEndPos);
	int ExtractReadInBound(bamFile finBam, uint32_t &nBlockLen, uint32_t *nMetaData, uint8_t *pcName,
                                         uint32_t *pnCigar, uint8_t *pcSeq, uint8_t *pcQual);
	bool DecodeSequence(string &sDecodeSeq, string &sDecodeQual, uint32_t nSeqLen, uint8_t *pcSeq, uint8_t *pcQual);

	uint32_t ConvertChrToTid(string sChr);
};










#endif

