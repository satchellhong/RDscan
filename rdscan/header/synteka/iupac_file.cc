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
*  FILE NAME : iupac_file.cc
*  LAST MODIFIED DATE : 2016-04-21
*
*  AUTHOR : Sunho Lee
*  E-MAIL : shlee@syntekabio.com
*
*  DESCRIPTION : source file for library of iupac file
*
**************************************************************************/

#include "iupac_file.h"

const unsigned long INITIAL_SIZE_OF_POSITION_IN_KEY_FILE = 100000;
using namespace std;

//parse .iupac file (chr 1_12341234 15 0 A C | 9.0 | 1 | A)
bool CIUPAC_FILE::ParseIupacFormat(string &sChr, unsigned long &ulPos, unsigned long &ulRefDp, unsigned long &ulAltDp, 
				char &cRef, char &cAlt, double &dAdiscore, unsigned long &ulGt, char &cIupac,    
				const string &sLine, const string &sIupacFileName)
{	
	//parse a iupac format
	unsigned long nMarkerUnderBar, nMarkerBlank;
	nMarkerBlank = sLine.find_first_of(" ");

	string sTemp = sLine.substr(nMarkerBlank+1);

	nMarkerUnderBar = sTemp.find_first_of("_");

	//read a chromosome numb
	sChr = sTemp.substr(0, nMarkerUnderBar);

	//read a position 
	char *pcEnd;
	ulPos = strtol(sTemp.substr(nMarkerUnderBar+1).c_str(), &pcEnd, 10);

	ulRefDp = strtol(pcEnd, &pcEnd, 10);
	ulAltDp = strtol(pcEnd, &pcEnd, 10);

	unsigned long nMarkerSeq = sTemp.find_first_of("ACGTN-");
	sTemp = sTemp.substr(nMarkerSeq);

	cRef = sTemp[0];
	cAlt = sTemp[3];

	unsigned long nMarkerI = sTemp.find_first_of("|");
	sTemp = sTemp.substr(nMarkerI+1);

	dAdiscore = strtod(sTemp.c_str(), &pcEnd);
	nMarkerI = sTemp.find_first_of("|");
	sTemp = sTemp.substr(nMarkerI+1);

	ulGt = strtol(sTemp.c_str(), &pcEnd, 10);

	nMarkerI = sTemp.find_first_of("|");
	sTemp = sTemp.substr(nMarkerI+1);
	cIupac = sTemp[1];


	

	return true;
}

bool CIUPAC_FILE::ReadIupacFile(const string &sSampleName)
{
	try{
		//open a iupac file 
		ifstream fin;
		string sIupacFileName;
		sIupacFileName = sSampleName;
		fin.open(sIupacFileName.c_str());
		if(!fin.is_open())
		{
			cout << sIupacFileName;
			throw " File does not exists";
		}

		//reserve a memory
		m_sChr.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);
		m_ulRefDp.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);
		m_ulAltDp.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);
		m_dAdiscore.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);
		m_cIupac.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);
		m_ulGt.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);
		m_ulPos.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);
		m_cRef.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);
		m_cAlt.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);

		//read a iupac file
		string sLine;

		string sChr;
		unsigned long ulPos, ulRefDp, ulAltDp;
		char cRef, cAlt;
		double dAdiscore;
		unsigned long ulGt;
		char cIupac;

		while(1)
		{	
			//read a line in .key file
			if(!getline(fin, sLine) || sLine.size() <= 0)	break;

			//parsing .key file
			ParseIupacFormat(sChr,ulPos,ulRefDp,ulAltDp,cRef,cAlt,dAdiscore, ulGt, cIupac, sLine, sIupacFileName);
			
			//store pos, ref, and alt in a memory
			m_sChr.push_back(sChr);
			m_ulPos.push_back(ulPos);
			m_ulRefDp.push_back(ulRefDp);
			m_ulAltDp.push_back(ulAltDp);
			m_cRef.push_back(cRef);
			m_cAlt.push_back(cAlt);
			m_dAdiscore.push_back(dAdiscore);
			m_ulGt.push_back(ulGt);
			m_cIupac.push_back(cIupac);
		}
		fin.close();
	}
	catch (const char *s)
	{
		cout << s << endl;
		exit(EXIT_FAILURE);
	}

	return true;
}



