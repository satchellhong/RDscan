#include "list_file.h"


using namespace std;

bool CLIST_FILE::ReadListFile(const string &sListFileName, const string sAddWord)
{
	if(sListFileName.size() == 0){                
		throw std::logic_error("you should input a list file name");
	}   
	//store List file name
	m_sListFileName = sListFileName;

	ifstream fin;   
	fin.open(m_sListFileName.c_str());  

	if(!fin.is_open())
	{   
		throw std::logic_error("List File does not exists");
	}   

	string sLine;         
	char *pcEnd;
	while(getline(fin, sLine) && sLine.size() > 0)
	{   
		sLine = sLine + sAddWord;
		unsigned long ulChr;

		unsigned long ulMarker = sLine.find_first_of("\t ");
		string sChr = sLine.substr(0, ulMarker);
		if((sChr[0]=='c' || sChr[0]=='C') && (sChr[1]=='h' || sChr[1]=='H') && (sChr[2]=='r' || sChr[2]=='R'))
		{
			sChr = sChr.substr(3);
		}

		if(sChr == "X")		ulChr = 23;
		else if(sChr == "Y")	ulChr = 24;
		else if(sChr == "MT" || sChr == "M")	ulChr = 25;
		else if(sChr.size() > 2)	continue;
		else			ulChr = strtol(sChr.c_str(), &pcEnd, 10);

		sLine = sLine.substr(ulMarker+1);
		m_vulStartPos[ulChr-1].push_back(strtol(sLine.c_str(), &pcEnd, 10));

		ulMarker = sLine.find_first_of("\t ");
		sLine = sLine.substr(ulMarker+1);
		m_vulEndPos[ulChr-1].push_back(strtol(sLine.c_str(), &pcEnd, 10));

		ulMarker = sLine.find_first_of("\t ");
		sLine = sLine.substr(ulMarker+1);
		
		ulMarker = sLine.find_first_of("\t ");
		m_vsGene[ulChr-1].push_back(sLine.substr(0, ulMarker));
		sLine = sLine.substr(ulMarker+1);

		ulMarker = sLine.find_first_of("\t ");
		m_vsExon[ulChr-1].push_back(sLine.substr(0, ulMarker));
		
	}   

	fin.close();    

	return true;
}


bool CLIST_FILE::SortList()
{
	for(unsigned int i=0; i<25; i++)
	{
		for(int j=0; j<(int)m_vulStartPos[i].size()-1; j++)
		{
			for(int k=0; k<(int)m_vulStartPos[i].size()-1; k++)
			{
				if(m_vulStartPos[i][k] > m_vulStartPos[i][k+1])
				{
					unsigned long ulTempStartPos = m_vulStartPos[i][k];
					unsigned long ulTempEndPos = m_vulEndPos[i][k];
					string sTempGene = m_vsGene[i][k];
					string sTempExon = m_vsExon[i][k];

					m_vulStartPos[i][k] = m_vulStartPos[i][k+1];
					m_vulEndPos[i][k] = m_vulEndPos[i][k+1];
					m_vsGene[i][k] = m_vsGene[i][k+1];
					m_vsExon[i][k] = m_vsExon[i][k+1];


					m_vulStartPos[i][k+1] = ulTempStartPos;
					m_vulEndPos[i][k+1] = ulTempEndPos;
					m_vsGene[i][k+1] = sTempGene;
					m_vsExon[i][k+1] = sTempExon;

				}
			}
		}

	}

	return true;
}




