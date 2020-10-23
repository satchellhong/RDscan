
#ifndef LIST_FILE_H
#define LIST_FILE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdexcept>

using namespace std;

class CLIST_FILE
{
public :
	string m_sListFileName;
	
	vector<unsigned long> m_vulStartPos[25];
	vector<unsigned long> m_vulEndPos[25];
	vector<string> m_vsGene[25];
	vector<string> m_vsExon[25];
public :
	CLIST_FILE()
	{
	}

	//read (content in list file + sAddWord)
	bool ReadListFile(const string &sListFileName, const string sAddWord = "");

	bool SortList();


};







#endif
