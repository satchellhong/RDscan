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
*  FILE NAME : dp_file.cc
*  LAST MODIFIED DATE : 2016-01-30
*
*  AUTHOR : Sunho Lee
*  E-MAIL : shlee@syntekabio.com
*
*  DESCRIPTION : source file for library of dp file
*
**************************************************************************/

#include "key_dp_file.h"

const unsigned long INITIAL_SIZE_OF_POSITION_IN_DP_FILE = 100000;



bool CDP_FILE::ParseDpFormat(unsigned long &ulRefDp, unsigned long &ulAltDp, const string &sLine, const string &sDpFileName)
{
	//parse .dp format
	unsigned long ulMarkerBlank = sLine.find_first_of(" ");
	if(ulMarkerBlank == string::npos)
	{
		cout << sDpFileName;
		throw " file format is not correct";
	}

	//read a Ref/Alt depth
	char *pcEnd;
	ulRefDp = strtol(sLine.substr(0, ulMarkerBlank).c_str(), &pcEnd, 10);
	ulAltDp = strtol(sLine.substr(ulMarkerBlank+1).c_str(), &pcEnd, 10);

	return true;
}

bool CDP_FILE::ReadDpFile(const string &sSampleName)
{
	try
	{
		//open .dp files
		ifstream fin;
		string sDpFileName;
		sDpFileName = sSampleName;
		fin.open(sDpFileName.c_str());
		if(!fin.is_open())
		{
			cout << sDpFileName;
			throw " file does not exists";
		}

		//reserve a memory
		m_ulRefDp.reserve(INITIAL_SIZE_OF_POSITION_IN_DP_FILE);
		m_ulAltDp.reserve(INITIAL_SIZE_OF_POSITION_IN_DP_FILE);

		//read a .dp file
		string sLine;
		unsigned long ulRefDp, ulAltDp;
		while(1)
		{
			//read a line in .dp file
			if(!getline(fin, sLine) || sLine.size() <= 0) 	break;

			//parse .dp format
			ParseDpFormat(ulRefDp, ulAltDp, sLine, sDpFileName);

			//store ref/alt depth in a memory
			m_ulRefDp.push_back(ulRefDp);
			m_ulAltDp.push_back(ulAltDp);
		}
		//close .dp files
		fin.close();

		//calculate # of positions
		m_ulCntPos = m_ulRefDp.size();
	}
	catch (const char *s)
	{
		cout << s << endl;
		exit(EXIT_FAILURE);
	}

	return true;
}

unsigned long CDP_FILE::GetRefDp(const long &lIdx)
{
	if(lIdx < 0 || lIdx >= (long)m_ulCntPos)      cout << "ERROR: input wrong idx in GetRefDp()" << endl;
	return m_ulRefDp[lIdx];
}

unsigned long CDP_FILE::GetAltDp(const long &lIdx)
{
	if(lIdx < 0 || lIdx >= (long)m_ulCntPos)      cout << "ERROR: input wrong idx in GetAltDp()" << endl;
	return m_ulAltDp[lIdx];
}









