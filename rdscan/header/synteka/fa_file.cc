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
*  FILE NAME : fa_file.cc
*  LAST MODIFIED DATE : 2016-03-29
*
*  AUTHOR : Sunho Lee
*  E-MAIL : shlee@syntekabio.com
*
*  DESCRIPTION : source file for library of .fa file
*
**************************************************************************/

#include "fa_file.h"

#define CNT_REF 100

using namespace std;

bool CFA_FILE::Init()
{
	m_ulCntRef = 0;
	return true;
}

bool CFA_FILE::WriteFaFile(const string &sFileName)
{
	try{
		//open a .fa file 
		ofstream fout;
		fout.open(sFileName.c_str());
		if(!fout.is_open())
		{
			cout << sFileName;
			throw " File does not exists";
		}

		//write a .fa file
		string sLine;
		for(long i=0; i<(long)m_ulCntRef; i++)
		{	
			//write header
			fout << m_sHeader[i] << endl;
			
			//write seq
			long j=0;
			for(j=0; j<(long)m_sRefSeq[i].size(); j++)
			{
				fout << m_sRefSeq[i][j];
				if((j+1)%60 == 0)		fout << endl;
			}
			if(j%60 != 0)	fout << endl;


		}


		fout.close();
	}
	catch (const char *s)
	{
		cout << s << endl;
		exit(EXIT_FAILURE);
	}

	return true;

}

bool CFA_FILE::ReadFaFile(const string &sFileName)
{
	try{
		//open a .fa file 
		ifstream fin;
		fin.open(sFileName.c_str());
		if(!fin.is_open())
		{
			cout << sFileName;
			throw " File does not exists";
		}

		//reserve a memory
		m_sHeader.reserve(CNT_REF);
		m_sRefSeq.reserve(CNT_REF);
		
		//read a .fa file
		string sLine;
		while(1)
		{	
			//read a line in .fa file
			if(!getline(fin, sLine) || sLine.size() <= 0)	break;

			if(sLine[0] == '>')
			{
				//store header of .fa file
				m_sHeader.push_back(sLine);
				m_sRefSeq.push_back("");
				m_sRefSeq[m_sRefSeq.size()-1].reserve(50000000);
				m_ulCntRef++;
			}
			else	m_sRefSeq[m_sRefSeq.size()-1] += sLine;

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

bool CFA_FILE::GetSeq(string &sSeq, const string &sRefName, const long &lSPos, const long &lEPos)
{
	sSeq = "";
	long lRefIdx = this->GetRefIdx(sRefName);
	if(lRefIdx == -1)
	{
		return false;
	}
	if(lSPos <= 0 ||  lEPos > (long)m_sRefSeq[lRefIdx].size())
	{
		return false;
	}
	for(int i=lSPos; i<=lEPos; i++) 	sSeq.push_back(m_sRefSeq[lRefIdx][i-1]);

	return true;
}

long CFA_FILE::GetRefIdx(const string &sRefName)
{
	long lIdx=-1;
	
	string sChr = sRefName;
	for(unsigned long i=0; i<m_sHeader.size(); i++)
	{
		if(sChr == this->GetRefName(i))
		{
			lIdx = i;
			break;
		}
	}

	return lIdx;
}

string CFA_FILE::GetRefName(const long &lIdx)
{
	string sRefName;

	if(lIdx < 0 || lIdx >= (long)m_ulCntRef)	throw "ERROR: input wrong idx in GetRefName()";

	unsigned long i=1;
	while(i < m_sHeader[lIdx].size())
	{
		if(m_sHeader[lIdx][i] == ' ' || m_sHeader[lIdx][i] == '\t'
			|| m_sHeader[lIdx][i] == '\n' )		break;
		if(m_sHeader[lIdx][i] != '>') 		sRefName.push_back(m_sHeader[lIdx][i]);
		i++;
	}
	return sRefName;
}


long CFA_FILE::GetCntRef()
{
	return (long)m_ulCntRef;
}



