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
*  FILE NAME : bam_file.h
*  LAST MODIFIED DATE : 2016-02-15
*
*  AUTHOR : Sunho Lee
*  E-MAIL : shlee@syntekabio.com
*
*  DESCRIPTION : header file for library of reading .bam files
*
**************************************************************************/



#ifndef BAM_H_
#define BAM_H_

#include <iostream>
#include <string>
#include <vector>
#include "bam.h"

#define SIZE_OF_READ_INFO 2000



using namespace std;

class CBAM_FILE
{
private :	//member variables
	string m_sBamFileName;		//bam file name
	unsigned long m_ulCntRead;	//# of reads

	vector<bam1_t*>	m_read;		//reads info
	bam_header_t *m_header;		//header info
	bam_index_t *m_idx;		//index file info
	bam_iter_t m_iter;

public :	//major member functions

	CBAM_FILE()
	{
		m_ulCntRead = 0;
	}
	//////////////////////////////////////////////////////////
	//
	//  read a bam file
	//
	//////////////////////////////////////////////////////////

	//read header and all contigs
	bool ReadBamFile(const string &sBamFileName);

	//read header and contigs which are aligned in "sRefName"
	bool ReadBamFile(const string &sBamFileName, const string &sRefName, 
				const unsigned long ulStartPos=1, const unsigned long ulEndPos=10000000000);

	//read header of .bam file
	bool ReadBamHeader(const string &sBamFileName);

	/////////////////////////////////////////////////////////
	//
	//  write a bam file
	//
	/////////////////////////////////////////////////////////

	

	/////////////////////////////////////////////////////////
	//
	//  get header info
	//
	/////////////////////////////////////////////////////////
	template <typename T>
	unsigned long GetRefLen(T idx)
	{
		if(idx < 0 || idx >= m_header->n_targets)    
		{  
			if(m_header->n_targets == 0 && idx == 0)	return 0;
			cout << "ERROR: input wrong idx in GetRefLen()" << endl;
			exit(EXIT_FAILURE);
		}   
		return m_header->target_len[idx];
	}

	unsigned long GetCntRef()
	{
		return m_header->n_targets;
	}

	////////////////////////////////////////////////////////
	//
	//  get read info
	//
	////////////////////////////////////////////////////////
	inline unsigned long GetReadCnt(){	return m_ulCntRead;	}
	inline string GetBamFileName(){		return m_sBamFileName;	}
	//get reference id
	template <typename T>
	T GetTidNumForRead(T idx){
		if(idx < 0 || idx >= m_ulCntRead){  
			cout << "ERROR: input wrong idx in GetTidNumForRead()" << endl;
			exit(EXIT_FAILURE);
		} 
		return m_read[idx]->core.tid;
	}
	//get start position
	template <typename T>
	T GetStartPosForRead(T idx){
		if(idx < 0 || idx >= m_ulCntRead){  
			cout << "ERROR: input wrong idx in GetStartPosForRead()" << endl;
			exit(EXIT_FAILURE);
		} 
		return m_read[idx]->core.pos;
	}
	//get bin
	template <typename T>
	T GetBinForRead(T idx){
		if(idx < 0 || idx >= m_ulCntRead){  
			cout << "ERROR: input wrong idx in GetBinForRead()" << endl;
			exit(EXIT_FAILURE);
		} 
		return m_read[idx]->core.bin;
	}
	//get quality score
	template <typename T>
	T GetQualForRead(T idx){
		if(idx < 0 || idx >= m_ulCntRead){  
			cout << "ERROR: input wrong idx in GetQualForRead()" << endl;
			exit(EXIT_FAILURE);
		} 
		return m_read[idx]->core.qual;
	}
	//get length of read name
	template <typename T>
	T GetNameLenForRead(T idx){
		if(idx < 0 || idx >= m_ulCntRead){  
			cout << "ERROR: input wrong idx in GetNameLenForRead()" << endl;
			exit(EXIT_FAILURE);
		} 
		return m_read[idx]->core.l_qname;
	}
	//get read name
	template <typename T>
	string GetNameForRead(T idx){
		if(idx < 0 || idx >= m_ulCntRead){  
			cout << "ERROR: input wrong idx in GetNameForRead()" << endl;
			exit(EXIT_FAILURE);
		} 
		char temp[1000];
		memcpy(temp, m_read[idx]->data, m_read[idx]->core.l_qname*sizeof(char));
		string result(temp);
		
		return result;
	}

	//get sequence 
	template <typename T>
	string GetSeqForRead(T idx){
		if(idx < 0 || idx >= m_ulCntRead){  
			cout << "ERROR: input wrong idx in GetSeqForRead()" << endl;
			exit(EXIT_FAILURE);
		} 
		uint8_t seq[1000];
		memcpy(seq, m_read[idx]->data + m_read[idx]->core.n_cigar*4 + m_read[idx]->core.l_qname, 
			(m_read[idx]->core.l_qseq+1)/2);

		string result;
		result.reserve(200);
		for(int i=0; i<m_read[idx]->core.l_qseq; i++)
		{
			unsigned char ucBase = bam1_seqi(seq, i);
			char cBase;
			if(ucBase == 1)		result.push_back('A');
			else if(ucBase == 2)	result.push_back('C');
			else if(ucBase == 4)	result.push_back('G');
			else if(ucBase == 8)	result.push_back('T');
			else if(ucBase == 15)	result.push_back('N');
		}
		
		return result;
	}
/*
	//get sequence quality
	template <typename T>
	string GetSeqQualForRead(T idx){
		if(idx < 0 || idx >= m_ulCntRead){  
			cout << "ERROR: input wrong idx in GetSeqQualForRead()" << endl;
			exit(EXIT_FAILURE);
		} 
		string result(m_read[idx]->data + m_read[idx]->core.n_cigar*4 + m_read[idx]->core.l_qname 
										+ ((m_read[idx]->core.l_qseq+1)>>1));	
		return result;
	}
*/
	//get flag
	template <typename T>
	T GetFlagForRead(T idx){
		if(idx < 0 || idx >= m_ulCntRead){  
			cout << "ERROR: input wrong idx in GetFlagForRead()" << endl;
			exit(EXIT_FAILURE);
		} 
		return m_read[idx]->core.flag;
	}
	//get # of cigar
	template <typename T>
	T GetCigarNumForRead(T idx){
		if(idx < 0 || idx >= m_ulCntRead){  
			cout << "ERROR: input wrong idx in GetCigarNumForRead()" << endl;
			exit(EXIT_FAILURE);
		} 
		return m_read[idx]->core.n_cigar;
	}
	template <typename T1, typename T2>
	T1 GetCigarOp(T1 idx, T2 num){
		if(idx < 0 || idx >= m_ulCntRead){  
			cout << "ERROR: input wrong idx in GetCigarOp()" << endl;
			exit(EXIT_FAILURE);
		} 
		if(num < 0 || num >= m_read[idx]->core.n_cigar){  
			cout << "ERROR: input wrong idx in GetCigarOp()" << endl;
			exit(EXIT_FAILURE);
		} 
		uint32_t cigar;
		memcpy(&cigar, m_read[idx]->data + m_read[idx]->core.l_qname + 4*num, 4);
		return cigar & 0x0000000f;
	}
	template <typename T1, typename T2>
	T1 GetCigarLen(T1 idx, T2 num){
		if(idx < 0 || idx >= m_ulCntRead){  
			cout << "ERROR: input wrong idx in GetCigarLen()" << endl;
			exit(EXIT_FAILURE);
		} 
		if(num < 0 || num >= m_read[idx]->core.n_cigar){  
			cout << "ERROR: input wrong idx in GetCigarLen()" << endl;
			exit(EXIT_FAILURE);
		} 
		uint32_t cigar;
		memcpy(&cigar, m_read[idx]->data + m_read[idx]->core.l_qname + 4*num, 4);
		return cigar >> 4;
	}

	//get sequence length
	template <typename T>
	T GetSeqLenForRead(T idx){
		if(idx < 0 || idx >= m_ulCntRead){  
			cout << "ERROR: input wrong idx in GetSeqLenForRead()" << endl;
			exit(EXIT_FAILURE);
		} 
		return m_read[idx]->core.l_qseq;
	}

	template <typename T>
	T GetMTidForRead(T idx){
		if(idx < 0 || idx >= m_ulCntRead){  
			cout << "ERROR: input wrong idx in GetMTidForRead()" << endl;
			exit(EXIT_FAILURE);
		} 
		return m_read[idx]->core.mtid;
	}
	template <typename T>
	T GetMPosForRead(T idx){
		if(idx < 0 || idx >= m_ulCntRead){  
			cout << "ERROR: input wrong idx in GetMPosForRead()" << endl;
			exit(EXIT_FAILURE);
		} 
		return m_read[idx]->core.mpos;
	}
	template <typename T>
	T GetISizeForRead(T idx){
		if(idx < 0 || idx >= m_ulCntRead){  
			cout << "ERROR: input wrong idx in GetISizeForRead()" << endl;
			exit(EXIT_FAILURE);
		} 
		return m_read[idx]->core.isize;
	}	

	////////////////////////////////////////////////////////
	//
	//  destroy
	//
	////////////////////////////////////////////////////////
	void DestroyHeader();
	void DestroyAllRead();

private :	//minor member functions



	//read all reads
	bool ReadBamRead();
	//read reads which are aligned in "sRefName"
	bool ReadBamRead(const string &sRefName);
	


};








#endif
