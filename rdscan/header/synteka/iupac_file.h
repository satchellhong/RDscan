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
*  FILE NAME : iupac_file.h
*  LAST MODIFIED DATE : 2016-04-21
*
*  AUTHOR : Sunho Lee
*  E-MAIL : shlee@syntekabio.com
*
*  DESCRIPTION : header file for library of reading .iupac files

**************************************************************************/

#ifndef IUPAC_FILE_H_
#define IUPAC_FILE_H_

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


class CIUPAC_FILE
{
public :	//member variables
	vector<string> m_sChr;			
	vector<unsigned long> m_ulPos;
	vector<unsigned long> m_ulRefDp;
	vector<unsigned long> m_ulAltDp;
	vector<char> m_cRef;
	vector<char> m_cAlt;

	vector<double> m_dAdiscore;
	vector<unsigned long> m_ulGt;
	vector<char> m_cIupac;


public:		//major member functions
	//////////////////////////////////////////////////////
	// input : sample name (ex:/twork11/shlee/chr6)
	//////////////////////////////////////////////////////
	bool ReadIupacFile(const string &sSampleName);


private:	//private member functions
	//ReadKeyFile() related function
	bool ParseIupacFormat(string &sChr, unsigned long &ulPos, unsigned long &ulRefDp, unsigned long &ulAltDp,
			char &cRef, char &cAlt, double &dAdiscore, unsigned long &ulGt, char &cIupac,
			const string &sLine, const string &sIupacFileName);

};



#endif

