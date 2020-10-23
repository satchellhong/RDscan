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
*  FILE NAME : key_file.cc
*  LAST MODIFIED DATE : 2016-01-30
*
*  AUTHOR : Sunho Lee
*  E-MAIL : shlee@syntekabio.com
*
*  DESCRIPTION : source file for library of key file
*
**************************************************************************/

#include "key_dp_file.h"

const unsigned long INITIAL_SIZE_OF_POSITION_IN_KEY_FILE = 100000;
using namespace std;

//parse .key file (1_12341234 A C -> 1 12341234 A C)
bool CKEY_FILE::ParseKeyFormat(string &sRefName, unsigned long &ulPos, char &cRef, char &cAlt, const string &sLine, const string &sKeyFileName)
{	
	//parse a key format
	unsigned long nMarkerUnderBar, nMarkerBlank;
	nMarkerUnderBar = sLine.find_first_of("_");
	nMarkerBlank = sLine.find_first_of(" ");

	if((nMarkerUnderBar == string::npos) || nMarkerBlank == string::npos)
	{
		cout << sKeyFileName;
		throw " file format is not correct";
	}

	//read a chromosome numb
	sRefName = sLine.substr(0, nMarkerUnderBar);

	//read a position 
	char *pcEnd;
	ulPos = strtol(sLine.substr(nMarkerUnderBar+1, nMarkerBlank).c_str(), &pcEnd, 10);

	//read a Ref/Alt
	if(sLine.size() != nMarkerBlank+4)
	{
		cout << sKeyFileName;
		throw " file format is not correct";
	}
	cRef = sLine[nMarkerBlank+1];
	cAlt = sLine[nMarkerBlank+3];

	return true;
}

bool CKEY_FILE::ReadKeyFile(const string &sSampleName)
{
	try{
		//open a key file 
		ifstream fin;
		string sKeyFileName;
		sKeyFileName = sSampleName;
		fin.open(sKeyFileName.c_str());
		if(!fin.is_open())
		{
			cout << sKeyFileName;
			throw " File does not exists";
		}

		//reserve a memory
		m_sRefName.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);
		m_ulPos.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);
		m_cRef.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);
		m_cAlt.reserve(INITIAL_SIZE_OF_POSITION_IN_KEY_FILE);

		//read a key file
		string sLine;
		string sRefName;
		unsigned long ulPos;
		char cRef, cAlt;
		while(1)
		{	
			//read a line in .key file
			if(!getline(fin, sLine) || sLine.size() <= 0)	break;

			//parsing .key file
			ParseKeyFormat(sRefName, ulPos, cRef, cAlt, sLine, sKeyFileName);
			
			//store pos, ref, and alt in a memory
			m_sRefName.push_back(sRefName);
			m_ulPos.push_back(ulPos);
			m_cRef.push_back(cRef);
			m_cAlt.push_back(cAlt);
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
bool CKEY_FILE::FindIdxFromPosRange(long &lStartIdx, long &lEndIdx, 
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

char CKEY_FILE::GetRef(const long &lIdx)
{
	if(lIdx < 0 || lIdx >= (long)m_ulCntPos)	cout << "ERROR: input wrong idx in GetRef()" << endl;
	return m_cRef[lIdx];
}

char CKEY_FILE::GetAlt(const long &lIdx)
{
	if(lIdx < 0 || lIdx >= (long)m_ulCntPos)	cout << "ERROR: input wrong idx in GetAlt()" << endl;
	return m_cAlt[lIdx];
}

unsigned long CKEY_FILE::GetPosition(const long &lIdx)
{
	if(lIdx < 0 || lIdx >= (long)m_ulCntPos)	cout << "ERROR: input wrong idx in GetPosition()" << endl;
	return m_ulPos[lIdx];
}

string CKEY_FILE::GetRefName(const long &lIdx)
{
	if(lIdx < 0 || lIdx >= (long)m_ulCntPos)	cout << "ERROR: input wrong idx in GetPosition()" << endl;
	return m_sRefName[lIdx];
}







