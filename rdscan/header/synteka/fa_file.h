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
*  FILE NAME : fa_file.h
*  LAST MODIFIED DATE : 2016-03-29
*
*  AUTHOR : Sunho Lee
*  E-MAIL : shlee@syntekabio.com
*
*  DESCRIPTION : header file for library of reading .fa files

**************************************************************************/

#ifndef FA_FILE_H_
#define FA_FILE_H_

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


class CFA_FILE
{
public :	//member variables
	vector<string> m_sHeader;
	vector<string> m_sRefSeq;

	unsigned long m_ulCntRef;


public:		//major member functions

	bool Init();

	bool WriteFaFile(const string &sFileName);

	//////////////////////////////////////////////////////
	// input : sample name (ex:/twork11/shlee/chr6)
	//////////////////////////////////////////////////////
	bool ReadFaFile(const string &sFileName);

	///////////////////////////////////////////////////////
	// input : index
	// output : reference character of the index
	///////////////////////////////////////////////////////
	bool GetSeq(string &, const string &sRefName, const long &lSPos, const long &lEPos);
		
	/////////////////////////////////////////////////////
	// input : reference name
	// output : Reference name of the index
	////////////////////////////////////////////////////
	long GetRefIdx(const string &sRefName);
	
	/////////////////////////////////////////////////////
	// input : index
	// output : Reference name of the index
	////////////////////////////////////////////////////
	string GetRefName(const long &lIdx);

	long GetCntRef();



private:	//private member functions

};



#endif

