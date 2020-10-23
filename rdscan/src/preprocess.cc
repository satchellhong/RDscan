#include "../header/rdscan.h"
#include "../header/bio_files.h"

using namespace std;
pthread_mutex_t mutex_temp = PTHREAD_MUTEX_INITIALIZER;


CRD::CRD(int nStatus, string sBamFile, string sBamFileN, string sRefFile, string sInputFile, int nCntThread, bool bIsDebug)
{
	m_nStatus = nStatus;
	m_sBamFile = sBamFile;
	m_sBamFileN = sBamFileN;
	m_sRefFile = sRefFile;
	m_sInputFile = sInputFile;
	m_nCntThread = nCntThread;
	m_bIsDebug = bIsDebug;
}



bool CRD::ReadInput()
{
	if(m_nStatus == 1)		ReadVcf();
	if(m_nStatus == 2)		ReadAdiscan();
	if(m_nStatus == 3)		ReadVarscan();

	//read reference file
	m_FaFile.Init();
	m_FaFile.ReadFaFile(m_sRefFile);

	if(m_bIsDebug)		cout << "DONE: ReadInput()" << endl;

	return true;
}



bool CRD::ReadVcf()
{
	ifstream fin;
	fin.open(m_sInputFile.c_str());
	if(!fin.is_open())		throw std::logic_error("Input file does not exists");

	string sLine;
	char *pcEnd;
	vector<string> vsWord;
	while(getline(fin, sLine) && sLine.size() > 0)
	{
		if(sLine.size() >= 2 && sLine[0] == '#' && sLine[1] == '#')			m_vcf.vsHeader.push_back(sLine);
		else if(sLine.size() >= 2 && sLine[0] == '#' && sLine[1] != '#')	m_vcf.sFormat = sLine;
		else
		{
			Parsing(vsWord, sLine, "\t");
			m_vcf.vsChr.push_back(vsWord[0]);
			m_vcf.vnPos.push_back(strtol(vsWord[1].c_str(), &pcEnd, 10));
			m_vcf.vsId.push_back(vsWord[2]);
			m_vcf.vsRef.push_back(vsWord[3]);
			m_vcf.vsAlt.push_back(vsWord[4]);
			m_vcf.vsQual.push_back(vsWord[5]);
			m_vcf.vsFilter.push_back(vsWord[6]);
			m_vcf.vsInfo.push_back(vsWord[7]);
			m_vcf.vsEtc.push_back("");
			if(vsWord.size() > 8)					m_vcf.vsEtc[m_vcf.vsEtc.size()-1] += vsWord[8];
			for(int i=9; i<vsWord.size(); i++)		m_vcf.vsEtc[m_vcf.vsEtc.size()-1] += "\t" + vsWord[i];

			m_rdscan.vdCorr.push_back(0);
			m_rdscan.vdPval.push_back(1);
			m_rdscan.vdVafT.push_back(0);
			m_rdscan.vdVafN.push_back(0);
			m_rdscan.vnVarT.push_back(0);
			m_rdscan.vnVarN.push_back(0);
		}
//		if(m_rdscan.vdCorr.size() >= 10000)		break;
	}
	return true;
}

bool CRD::ReadAdiscan()
{
	return true;
}
bool CRD::ReadVarscan()
{
	ifstream fin;
	fin.open(m_sInputFile.c_str());
	if(!fin.is_open())		throw std::logic_error("Input file does not exists");

	string sLine;
	char *pcEnd;
	vector<string> vsWord;
	while(getline(fin, sLine) && sLine.size() > 0)
	{
		Parsing(vsWord, sLine, "\t");
		if(vsWord[0] == "chrom")	m_varscan.sFormat = sLine;
		else
		{
			m_varscan.vsChr.push_back(vsWord[0]);
			m_varscan.vnPos.push_back(strtol(vsWord[1].c_str(), &pcEnd, 10));
			
			vector<string> vsT;
			Parsing(vsT, vsWord[3], "/");
			if(vsT.size() > 1 || vsWord[12] != "Somatic")
			{
				m_varscan.vsRef.push_back("N");
				m_varscan.vsAlt.push_back("N");
				m_varscan.vsStatus.push_back("None");
				m_varscan.vsAll.push_back(sLine);
			}
			else
			{
				string sRef = vsWord[2];
				string sAlt="";
				if(vsWord[3][0] == '-')
				{
					sRef += vsWord[3].substr(1);
					sAlt = vsWord[2];
				}
				else if(vsWord[3][0] == '+')	sAlt = sRef + vsWord[3].substr(1);
				else							sAlt = vsWord[3];
				m_varscan.vsRef.push_back(sRef);
				m_varscan.vsAlt.push_back(sAlt);
				m_varscan.vsStatus.push_back("Somatic");
				m_varscan.vsAll.push_back(sLine);
			}
			m_rdscan.vdCorr.push_back(0);
			m_rdscan.vdPval.push_back(1);
			m_rdscan.vdVafT.push_back(0);
			m_rdscan.vdVafN.push_back(0);
			m_rdscan.vnVarT.push_back(0);
			m_rdscan.vnVarN.push_back(0);
		}
		//if(m_rdscan.vdCorr.size() >= 10000)		break;
	}

	return true;
}


bool CRD::Parsing(vector<string> &vsWord, string sLine, string sSep)
{
	char *pcEnd;
	unsigned long ulMarker;
	vsWord.clear();
	while(1)
	{
		ulMarker = sLine.find_first_of(sSep);
		vsWord.push_back(sLine.substr(0, ulMarker));

		if(ulMarker > sLine.size()) break;
		sLine = sLine.substr(ulMarker+1);
	}
	return true;
}






