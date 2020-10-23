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
*  FILE NAME : key_dp_file.h
*  LAST MODIFIED DATE : 2016-01-30
*
*  AUTHOR : Sunho Lee
*  E-MAIL : shlee@syntekabio.com
*
*  DESCRIPTION : header file for library of reading .key, .dp files

**************************************************************************/

#ifndef KEY_DP_FILE_H_
#define KEY_DP_FILE_H_

#include <vector>
#include <exception>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <new>
#include <string>
#include <stdio.h>
#include <string.h>
#include <assert.h>

using namespace std;


class CKEY_FILE
{
public :	//member variables
	vector<string> m_sRefName;				//[1-22, x=23, y=24]
	vector<unsigned long> m_ulPos;
	vector<char> m_cRef;
	vector<char> m_cAlt;
	unsigned long m_ulCntPos;


public:		//major member functions
	//////////////////////////////////////////////////////
	// input : sample name (ex:/twork11/shlee/chr6)
	//////////////////////////////////////////////////////
	bool ReadKeyFile(const string &sSampleName);

	/////////////////////////////////////////////////////
	// input : reference sequence name
	// 	   start/end position
	// output : index range corresponding to the position range
	/////////////////////////////////////////////////////
	bool FindIdxFromPosRange(long &lStartIdx, long &lEndIdx, 
			const string &sRefName, const unsigned long &ulStartPos, const unsigned long &ulEndPos);
	
	///////////////////////////////////////////////////////
	// input : index
	// output : reference character of the index
	///////////////////////////////////////////////////////
	char GetRef(const long &lIdx);
	
	//////////////////////////////////////////////////////
	// input : index
	// output : alternative character of the index
	//////////////////////////////////////////////////////
	char GetAlt(const long &lIdx);

	/////////////////////////////////////////////////////
	// input : index
	// output : position of the index
	////////////////////////////////////////////////////
	unsigned long GetPosition(const long &lIdx);

	/////////////////////////////////////////////////////
	// input : index
	// output : Reference name of the index
	////////////////////////////////////////////////////
	string GetRefName(const long &lIdx);


private:	//private member functions
	//ReadKeyFile() related function
	bool ParseKeyFormat(string &sRefName, unsigned long &ulPos, char &cRef, char &cAlt, 
								const string &sLine, const string &sKeyFileName);

};


class CDP_FILE
{
public :	//member variables
	vector<unsigned char> m_ucChr;
	vector<unsigned long> m_ulPos;
	vector<unsigned long> m_ulRefDp;
	vector<unsigned long> m_ulAltDp;
	unsigned long m_ulCntPos;


public :	//major member functions
	//////////////////////////////////////////////////////
	// input : sample name (ex:/twork11/shlee/chr6)
	//////////////////////////////////////////////////////
	bool ReadDpFile(const string &sSampleName);

	//////////////////////////////////////////////////////
	// input : index
	// output : reference depth of the index
	// //////////////////////////////////////////////////
	unsigned long GetRefDp(const long &lIdx);

	/////////////////////////////////////////////////////
	// input : index
	// output : alternative depth of the index
	/////////////////////////////////////////////////////
	unsigned long GetAltDp(const long &lIdx);



private :	//private member functions
	//ReadDpFile() related function
	bool ParseDpFormat(unsigned long &ulRefDp, unsigned long &ulAltDp, const string &sLine, const string &sDpFileName);

};



class CKEY_DP_FILE
{
private :	//member variables
	vector<string> m_sRefName;				
	vector<unsigned long> m_ulPos;
	vector<char> m_cRef;
	vector<char> m_cAlt;
	vector<unsigned long> m_ulRefDp;
	vector<unsigned long> m_ulAltDp;
	unsigned long m_ulCntPos;


public:		//major member functions
	//////////////////////////////////////////////////////
	// input : sample name (ex:/twork11/shlee/chr6)
	//////////////////////////////////////////////////////
	bool ReadKeyDpFile(const string &sSampleName);

	/////////////////////////////////////////////////////
	// input : reference sequence name
	// 	   start/end position
	// output : index range corresponding to the position range
	/////////////////////////////////////////////////////
	bool FindIdxFromPosRange(long &lStartIdx, long &lEndIdx, 
			const string &sRefName, const unsigned long &ulStartPos, const unsigned long &ulEndPos);
	
	///////////////////////////////////////////////////////
	// input : index
	// output : reference character of the index
	///////////////////////////////////////////////////////
	char GetRef(const long &lIdx);
	
	//////////////////////////////////////////////////////
	// input : index
	// output : alternative character of the index
	//////////////////////////////////////////////////////
	char GetAlt(const long &lIdx);

	/////////////////////////////////////////////////////
	// input : index
	// output : position of the index
	////////////////////////////////////////////////////
	unsigned long GetPosition(const long &lIdx);

	/////////////////////////////////////////////////////
	// input : index
	// output : Reference name of the index
	////////////////////////////////////////////////////
	string GetRefName(const long &lIdx);

	//////////////////////////////////////////////////////
	// input : index
	// output : reference depth of the index
	// //////////////////////////////////////////////////
	unsigned long GetRefDp(const unsigned long lIdx);

	/////////////////////////////////////////////////////
	// input : index
	// output : alternative depth of the index
	/////////////////////////////////////////////////////
	unsigned long GetAltDp(const long &lIdx);

	inline unsigned long GetPosCnt(){	return m_ulCntPos;	}

private:	//private member functions
	//ReadKeyFile() related function
	bool ParseKeyDpFormat(string &sRefName, unsigned long &ulPos, char &cRef, char &cAlt, 
		unsigned long &ulRefDp, unsigned long &ulAltDp, const string &sLine, const string &sKeyFileName);

};



#endif

