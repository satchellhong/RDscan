#include "../header/rdscan.h"


using namespace std;


bool CRD::Report()
{
	if(m_nStatus == 1)      ReportVcf();
	//if(m_nStatus == 2)      ReportAdiscan();
	else if(m_nStatus == 3)      ReportVarscan();


	return true;
}

bool CRD::ReportVcf()
{
	//header
	string sPrevStatus = "";
	for(int i=0; i<m_vcf.vsHeader.size(); i++)
	{
		string sCurrStatus = m_vcf.vsHeader[i].substr(0,6);
		if(sPrevStatus == "##FILT" && sCurrStatus != sPrevStatus)
		{
			cout << "##FILTER=<ID=LowRDscore,Description=\"Read Distribution (RD) score of a variant is below threshold\">" << endl;
		}
		if(sPrevStatus == "##INFO" && sCurrStatus != sPrevStatus)
		{
			cout << "##INFO=<ID=RDscore,Description=\"Read Distribution (RD) score of a variant\">" << endl;
		}
		cout << m_vcf.vsHeader[i] << endl;
		sPrevStatus = sCurrStatus;
	}
	cout << m_vcf.sFormat << endl;

	//variants
	for(int i=0; i<m_vcf.vsChr.size(); i++)
	{
	/*
		if(m_rdscan.vdCorr[i] == -1)
		{
			if(m_vcf.vsFilter[i] == "PASS")		m_vcf.vsFilter[i] = "NONE";
			else								m_vcf.vsFilter[i] += ";NONE";
		}
		else if(m_rdscan.vdCorr[i] < 0.25)
		{
			if(m_vcf.vsFilter[i] == "PASS")		m_vcf.vsFilter[i] = "LowRDscore";
			else								m_vcf.vsFilter[i] += ";LowRDscore";
		}
		*/
		
		cout << m_vcf.vsChr[i] << "\t" << m_vcf.vnPos[i] << "\t" << m_vcf.vsId[i] << "\t";
		cout << m_vcf.vsRef[i] << "\t" << m_vcf.vsAlt[i] << "\t" << m_vcf.vsQual[i] << "\t";
		cout << m_vcf.vsFilter[i] << "\t" << m_vcf.vsInfo[i] << ";RDscore=" << m_rdscan.vdCorr[i] << "\t"; 
		
		cout << m_vcf.vsEtc[i];
		cout << endl;

	}

	return true;
}

bool CRD::ReportVarscan()
{
	//header
	cout << m_varscan.sFormat << "\tRDscore\tFilter" << endl;
	
	//variants
	for(int i=0; i<m_varscan.vsChr.size(); i++)
	{
		//cout << m_rdscan.vdCorr[i] << "\t";

		cout << m_varscan.vsAll[i] << "\t" << m_rdscan.vdCorr[i] << "\t";
	//	if(m_rdscan.vdCorr[i] == -1)		cout << "NONE";
	//	else if(m_rdscan.vdCorr[i] < 0.25)	cout << "LowRDscore";
	//	else								cout << "PASS";

		cout << endl;	
	}
	return true;
}


