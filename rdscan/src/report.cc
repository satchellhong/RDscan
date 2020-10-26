#include "../header/rdscan.h"


using namespace std;


bool CRD::Report()
{
	if(m_nStatus == 1)      ReportVcf();

	return true;
}

bool CRD::ReportVcf()
{
	this->SetStartTime();
	this->ViewStatus(0, m_vcf.vsChr.size(),"Generate a vcf file with RDscore");
	
	ofstream fout;
	fout.open(m_sOutputFile.c_str());
	//header
	string sPrevStatus = "";
	for(int i=0; i<m_vcf.vsHeader.size(); i++)
	{
		string sCurrStatus = m_vcf.vsHeader[i].substr(0,6);
		if(sPrevStatus == "##INFO" && sCurrStatus != sPrevStatus)
		{
			fout << "##INFO=<ID=RDscore,Type=Float,Number=1,Description=\"Read Distribution (RD) score of a variant\">" << endl;
		}
		fout << m_vcf.vsHeader[i] << endl;
		sPrevStatus = sCurrStatus;
	}
	fout << m_vcf.sFormat << endl;
	
	//variants
	for(int i=0; i<m_vcf.vsChr.size(); i++)
	{
		double dRDscore = m_rdscan.vdCorr[i];
		if(dRDscore > 0.8)		dRDscore = 0.8;
		fout << m_vcf.vsChr[i] << "\t" << m_vcf.vnPos[i] << "\t" << m_vcf.vsId[i] << "\t";
		fout << m_vcf.vsRef[i] << "\t" << m_vcf.vsAlt[i] << "\t" << m_vcf.vsQual[i] << "\t";
		fout << m_vcf.vsFilter[i] << "\t" << m_vcf.vsInfo[i] << ";RDscore=" << dRDscore << "\t"; 
		
		fout << m_vcf.vsEtc[i];
		fout << endl;

		if(i+1 == m_vcf.vsChr.size() || i%(m_vcf.vsChr.size()/100)==0)
			this->ViewStatus(i+1, m_vcf.vsChr.size(),"Generate a vcf file with RDscore");

	}
	fout.close();
	cout << endl << endl;

	return true;
}

