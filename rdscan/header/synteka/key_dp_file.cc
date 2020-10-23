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
*  FILE NAME : key_dp_file.cc
*  LAST MODIFIED DATE : 2016-02-16
*
*  AUTHOR : Sunho Lee
*  E-MAIL : shlee@syntekabio.com
*
*  DESCRIPTION : source file for library of keydp file
*
**************************************************************************/

#include "key_dp_file.h"

const unsigned long INITIAL_SIZE_OF_POSITION_IN_KEY_FILE = 100000;
using namespace std;

//parse .key .dp file (1_12341234 A C 50 2 -> 1 12341234 A C 50 2)
bool CKEY_DP_FILE::ParseKeyDpFormat(string &sRefName, unsigned long &ulPos, char &cRef, char &cAlt, 
			unsigned long &ulRefDp, unsigned long &ulAltDp, const string &sLine, const string &sKeyDpFileName)
{	
	//parse a key dp format
	unsigned long nMarkerUnderBar, nMarkerBlank;
	nMarkerUnderBar = sLine.find_first_of("_");
	nMarkerBlank = sLine.find_first_of(" ");

	if((nMarkerUnderBar == string::npos) || nMarkerBlank == string::npos)
	{
		cout << sKeyDpFileName;
		throw " file format is not correct";
	}

	//read a reference name
	sRefName = sLine.substr(0, nMarkerUnderBar);

	//read a position 
	char *pcEnd;
	ulPos = strtol(sLine.substr(nMarkerUnderBar+1, nMarkerBlank).c_str(), &pcEnd, 10);

	//read a Ref/Alt
	cRef = sLine[nMarkerBlank+1];
	cAlt = sLine[nMarkerBlank+3];

	
	//read a Ref/Alt Depth
	string sDepthLine = sLine.substr(nMarkerBlank+5);
	nMarkerBlank = sDepthLine.find_first_of(" ");

	ulRefDp = strtol(sDepthLine.substr(0, nMarkerBlank).c_str(), &pcEnd, 10);
	ulAltDp = strtol(sDepthLine.substr(nMarkerBlank+1).c_str(), &pcEnd, 10);

	
	return true;
}

bool CKEY_DP_FILE::ReadKeyDpFile(const string &sSampleName)
{
	try{
		//open a key file 
		ifstream fin;
		string sKeyDpFileName;
		sKeyDpFileName = sSampleName;
		fin.open(sKeyDpFileName.c_str());
		if(!fin.is_open())
		{
			cout << sKeyDpFileName;
			throw " File does not exists";
		}

		//reserve a memory
		m_sRefName.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);
		m_ulPos.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);
		m_cRef.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);
		m_cAlt.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);
		m_ulRefDp.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);
		m_ulAltDp.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);


		//read a key file
		string sLine;
		string sRefName;
		unsigned long ulPos;
		char cRef, cAlt;
		unsigned long ulRefDp, ulAltDp;
		while(1)
		{	
			//read a line in keydp file
			if(!getline(fin, sLine) || sLine.size() <= 0)	break;

			//parsing .key file
			ParseKeyDpFormat(sRefName, ulPos, cRef, cAlt, ulRefDp, ulAltDp, sLine, sKeyDpFileName);
			
			//store pos, ref, and alt in a memory
			m_sRefName.push_back(sRefName);
			m_ulPos.push_back(ulPos);
			m_cRef.push_back(cRef);
			m_cAlt.push_back(cAlt);
			m_ulRefDp.push_back(ulRefDp);
			m_ulAltDp.push_back(ulAltDp);
		
		//	int j=m_ulRefDp.size()-1;
		//	cout << m_sRefName[j] << " " << m_ulPos[j] << " " << m_cRef[j] << " " << m_cAlt[j]<< " " << m_ulRefDp[j] << " " << m_ulAltDp[j] << endl;
		}
		fin.close();

		//calculate # of positions
		m_ulCntPos = m_cAlt.size();
	}
	catch (const char *s)
	{
		cout << s << endl;
		exit(EXIT_FAILURE);
	}

	return true;
}

//TODO : optimize (using binary search)
bool CKEY_DP_FILE::FindIdxFromPosRange(long &lStartIdx, long &lEndIdx, 
			const string &sRefName,	const unsigned long &ulStartPos, const unsigned long &ulEndPos)
{
	//initialize
	lStartIdx = -1;
	lEndIdx = -1;

	//find start idx
	for(unsigned long i=0; i<m_ulCntPos; i++)
	{
		//equal reference sequence
		if(sRefName == m_sRefName[i])
		{
			if(ulStartPos <= m_ulPos[i])
			{
				lStartIdx = i;	
				break;
			}
		}
	}
	if(lStartIdx == -1)	return false;

	//find end idx
	for(unsigned long i=m_ulCntPos-1; i>=0; i--)
	{
		//equal chromosome
		if(sRefName == m_sRefName[i])
		{
			if(ulEndPos >= m_ulPos[i])
			{
				lEndIdx = i;
				break;
			}
		}
	}
	if(lEndIdx == -1)	return false;

	if(lStartIdx > lEndIdx)	return false;
	
	return true;
}

char CKEY_DP_FILE::GetRef(const long &lIdx)
{
	if(lIdx < 0 || lIdx >= (long)m_ulCntPos)	cout << "ERROR: input wrong idx in GetRef()" << endl;
	return m_cRef[lIdx];
}

char CKEY_DP_FILE::GetAlt(const long &lIdx)
{
	if(lIdx < 0 || lIdx >= (long)m_ulCntPos)	cout << "ERROR: input wrong idx in GetAlt()" << endl;
	return m_cAlt[lIdx];
}

unsigned long CKEY_DP_FILE::GetPosition(const long &lIdx)
{
	if(lIdx < 0 || lIdx >= (long)m_ulCntPos)	cout << "ERROR: input wrong idx in GetPosition()" << endl;
	return m_ulPos[lIdx];
}

string CKEY_DP_FILE::GetRefName(const long &lIdx)
{
	if(lIdx < 0 || lIdx >= (long)m_ulCntPos)	cout << "ERROR: input wrong idx in GetPosition()" << endl;
	return m_sRefName[lIdx];
}

unsigned long CKEY_DP_FILE::GetRefDp(const unsigned long lIdx)
{
	if(lIdx < 0 || lIdx >= m_ulCntPos)      cout << "ERROR: input wrong idx in GetRefDp()" << endl;
	return m_ulRefDp[lIdx];
}

unsigned long CKEY_DP_FILE::GetAltDp(const long &lIdx)
{
	if(lIdx < 0 || lIdx >= (long)m_ulCntPos)      cout << "ERROR: input wrong idx in GetAltDp()" << endl;
	return m_ulAltDp[lIdx];
}






