#include "../header/rdscan.h"
#include <math.h>


pthread_mutex_t mutex_filter = PTHREAD_MUTEX_INITIALIZER;

using namespace std;



bool CRD::CalcDist()
{
	//parallel process
	vector<DIST_THREAD> vDistThread;
	int nCntAll = m_rdscan.vdCorr.size();
	int nCntSub	= 0;
	if(m_nCntThread > 1)	nCntSub = nCntAll/(m_nCntThread-1);
	else					
	{
		nCntSub = nCntAll;
		m_nCntThread = 1;
	}

	for(int i=0; i<m_nCntThread; i++)
	{   
		int nSIdx = nCntSub*i;
		int nEIdx = nCntSub*(i+1) - 1;
		if(i == m_nCntThread-2)			nEIdx = nCntSub*i + nCntSub*4/5 -1;
		if(i == m_nCntThread-1 && m_nCntThread != 1)         nSIdx = nCntSub*(i-1) + nCntSub*4/5;
		if(i == m_nCntThread-1)         nEIdx = nCntAll-1;

		DIST_THREAD DistThread;
		DistThread.RD = this;
		DistThread.nSIdx = nSIdx;
		DistThread.nEIdx = nEIdx;

		vDistThread.push_back(DistThread);
	}
	//create thread
	pthread_t* thread_handles;
	thread_handles = new pthread_t[m_nCntThread];
	for(int i=0; i<m_nCntThread; i++)	pthread_create(&thread_handles[i], NULL, AlleleDist_helper, (void*)&vDistThread[i]);
	for(int i=0; i<m_nCntThread; i++)	pthread_join(thread_handles[i], NULL);
	free(thread_handles);

	if(m_bIsDebug)		cout << "DONE: CalcDist()" << endl;

	return true;
}

void* CRD::AlleleDist(int nSIdx, int nEIdx)
{

	//open bam
	bamFile finBam, finBamN;
	finBam = bam_open(m_sBamFile.c_str(), "r");
	if(finBam == NULL)	throw std::logic_error("ERROR: Can not open .bam file");

	bam_header_t *bamHeader, *bamHeaderN;
	bamHeader = bam_header_read(finBam);
	if(!bamHeader)       throw std::logic_error("ERROR: Fail to read the header of a bam file");

	bam_index_t *bamIndex, *bamIndexN;
	bamIndex = bam_index_load(m_sBamFile.c_str());
	if(!bamIndex)      throw std::logic_error("ERROR: Fail to read .bai file");

	if(m_sBamFileN != "")
	{
		finBamN = bam_open(m_sBamFileN.c_str(), "r");
		if(finBamN == NULL)	throw std::logic_error("ERROR: Can not open .bam file");

		bamHeaderN = bam_header_read(finBamN);
		if(!bamHeaderN)       throw std::logic_error("ERROR: Fail to read the header of a bam file");

		bamIndexN = bam_index_load(m_sBamFileN.c_str());
		if(!bamIndexN)      throw std::logic_error("ERROR: Fail to read .bai file");
	}


	//calc dist
	for(int i=nSIdx; i<=nEIdx; i++)
	{
		if(m_bIsDebug && i%1000==0)	cout << nSIdx << "-" << nEIdx << "\t" << (double)(i-nSIdx)/(double)(nEIdx-nSIdx)*100 << "%" << endl;
		///////////////// get variants info. //////////////////////////
		int nChr, nPos;
		string sChr, sRef, sAlt, sAlt1="", sAlt2="";
		if(m_nStatus == 1)				// vcf
		{
			sChr = m_vcf.vsChr[i];
			nPos = m_vcf.vnPos[i];
			sRef = m_vcf.vsRef[i];
			sAlt = m_vcf.vsAlt[i];
			vector<string> vsWord;
			Parsing(vsWord, sAlt, ",");
			if(vsWord.size() > 1)
			{
				sAlt1 = vsWord[0];
				sAlt2 = vsWord[1];
				sAlt = sAlt1;
				if(sRef.size() == 1 && sAlt1.size() < sAlt2.size())		sAlt = sAlt2;
				if(sRef.size() > 1 && sAlt1.size() > sAlt2.size())		sAlt = sAlt2;
			}
		}
		else if(m_nStatus == 2)			// adiscan
		{}
		else if(m_nStatus == 3)			// varscan
		{
			if(m_varscan.vsStatus[i] != "Somatic")		
			{
				m_rdscan.vdCorr[i] = -1;
				continue;
			}
			sChr = m_varscan.vsChr[i];
			nPos = m_varscan.vnPos[i];
			sRef = m_varscan.vsRef[i];
			sAlt = m_varscan.vsAlt[i];
		}

		nChr = ConvertChrToTid(sChr, bamHeader);
		if(nChr == -1)
		{
			cout << sChr << "(" << nChr << ")" << ":" << nPos << "\t" << sRef << "\t" << sAlt << endl;
			throw std::logic_error("Not available Chr");
		}
		
		// Check Repeat
		int nRepeatCnt = 1;
		int nRepeatLastLen = 0;
		CheckRepeat(sRef, sAlt, sChr, nPos, nRepeatCnt, nRepeatLastLen);
	

		///////////////// calc allele distribution ////////////////////
		
		int nPivotSPos = -1;
		int nPivotSPosN = -1;
		vector<int> vnNorSPos, vnNorEPos, vnVarSPos, vnVarEPos;
		vector<int> vnNorSPosN, vnNorEPosN, vnVarSPosN, vnVarEPosN;
		vector<string> vsNorSeq, vsVarSeq;
		vector<string> vsNorSeqN, vsVarSeqN;
		
		
		AlleleCount(bamIndex, finBam, nChr, nPos, sRef, sAlt, nRepeatCnt, nRepeatLastLen, nPivotSPos, vnNorSPos, vnNorEPos, vnVarSPos, vnVarEPos, vsNorSeq, vsVarSeq);
		if(m_sBamFileN != "")	AlleleCount(bamIndexN, finBamN, nChr, nPos, sRef, sAlt, nRepeatCnt, nRepeatLastLen, nPivotSPosN, vnNorSPosN, vnNorEPosN, vnVarSPosN, vnVarEPosN, vsNorSeqN, vsVarSeqN);
	
		if(m_bIsDebug)	cout << sChr << ":" << nPos << "\t" << vnVarSPos.size() << "/" << vnNorSPos.size()+vnVarSPos.size();
		if(m_bIsDebug && m_sBamFileN != "")	cout << "\t" << vnVarSPosN.size() << "/" << vnNorSPosN.size()+vnVarSPosN.size();
		if(m_bIsDebug)	cout << endl;



		///////////////// filter read ////////////////////////////////
		int nSumNor,nSumVar,nSumNorN,nSumVarN;
		int nCntNor, nCntVar, nCntNorN, nCntVarN;

		FilterRead(sChr,vnNorSPos,vnNorEPos,vnVarSPos,vnVarEPos,vsNorSeq, vsVarSeq, nSumNor, nSumVar, nCntNor, nCntVar);
		if(m_sBamFileN != "")	FilterRead(sChr,vnNorSPosN,vnNorEPosN,vnVarSPosN,vnVarEPosN,vsNorSeqN,vsVarSeqN,nSumNorN,nSumVarN,nCntNorN,nCntVarN);


		double dAveVarReadLen = (double)nSumVar/(double)nCntVar;
		double dAveNorReadLen = (double)nSumNor/(double)nCntNor;
		
		double dAveVarReadLenN, dAveNorReadLenN;
		if(m_sBamFileN != "")	dAveVarReadLenN = (double)nSumVarN/(double)nCntVarN;
		if(m_sBamFileN != "")	dAveNorReadLenN = (double)nSumNorN/(double)nCntNorN;
		
		
		if(m_bIsDebug)	cout << sChr << ":" << nPos << "\t" << vnVarSPos.size() << "/" << vnNorSPos.size()+vnVarSPos.size() <<endl;
		


		/////////////////// calculate depth //////////////////////////
		int anAllDp[1000] = {0,};
		int anVarDp[1000] = {0,};
		int anAllDpN[1000] = {0,};
		int anVarDpN[1000] = {0,};
		
		for(int j=0; j<vnNorSPos.size(); j++)
		{
			for(int k=vnNorSPos[j]-nPivotSPos; k<=vnNorEPos[j]-nPivotSPos; k++)		anAllDp[k]++;
		}
		for(int j=0; j<vnVarSPos.size(); j++)
		{
			for(int k=vnVarSPos[j]-nPivotSPos; k<=vnVarEPos[j]-nPivotSPos; k++)		anVarDp[k]++;
			for(int k=vnVarSPos[j]-nPivotSPos; k<=vnVarEPos[j]-nPivotSPos; k++)		anAllDp[k]++;
		}

		if(m_sBamFileN != "")
		{
			for(int j=0; j<vnNorSPosN.size(); j++)
			{
				for(int k=vnNorSPosN[j]-nPivotSPosN; k<=vnNorEPosN[j]-nPivotSPosN; k++)		anAllDpN[k]++;
			}
			for(int j=0; j<vnVarSPosN.size(); j++)
			{
				for(int k=vnVarSPosN[j]-nPivotSPosN; k<=vnVarEPosN[j]-nPivotSPosN; k++)		anVarDpN[k]++;
				for(int k=vnVarSPosN[j]-nPivotSPosN; k<=vnVarEPosN[j]-nPivotSPosN; k++)		anAllDpN[k]++;
			}
		}

			

		///////////////// calc correlation between all & variants ////////////////////
		double dCorr, dPval;
		double dCorrN=0, dPvalN=0;
		double dSignificant = 1;
		double dTVaf = 0, dNVaf = 0;
		
		// calc correlation
		CalcPearsonCorr(anAllDp, anVarDp, dCorr, dPval);
		double dCoef = 1;
//		if(dTVaf < 0.1)		dCoef = CalcCoef(sChr, vsSeq, vnVarSPos, vnVarEPos);		//calc coef
		m_rdscan.vdCorr[i] = dCoef * dCorr;
		

		if(m_bIsDebug)	cout << "SCORE:" << "\t" << m_rdscan.vdCorr[i] << endl;

		////////////////// minus control corr ////////////////////////////
		if(vnNorSPos.size()+vnVarSPos.size() != 0)	
			dTVaf = (double)max(0,(int)vnVarSPos.size()) / ((double)vnNorSPos.size()+(double)vnVarSPos.size());

		
		if(m_sBamFileN != "")
		{
			CalcPearsonCorr(anAllDpN, anVarDpN, dCorrN, dPvalN);
			if(vnNorSPosN.size()+vnVarSPosN.size() != 0)	
				dNVaf = (double)max(0,(int)vnVarSPosN.size()) / ((double)vnNorSPosN.size()+(double)vnVarSPosN.size());
			if(dTVaf != 0)							dSignificant = dNVaf/dTVaf;

			dCoef = 1;
//			if(dNVaf < 0.1)		dCoef = CalcCoef(sChr, vsSeqN, vnVarSPosN, vnVarEPosN);			
			m_rdscan.vdCorr[i] = max(0.0, m_rdscan.vdCorr[i] - dSignificant*dCoef*dCorrN);
		}
		


		if(dTVaf < 0.2)		m_rdscan.vdCorr[i] = m_rdscan.vdCorr[i] * 0.1 * min((int)vnVarSPos.size(),10);
		
		m_rdscan.vdCorr[i] = min(1.0, m_rdscan.vdCorr[i]);
		m_rdscan.vdCorr[i] = max(0.0, m_rdscan.vdCorr[i]);

		
		m_rdscan.vdVafT[i] = dTVaf;
		m_rdscan.vdVafN[i] = dNVaf;
		m_rdscan.vnVarT[i] = vnVarSPos.size();
		m_rdscan.vnVarN[i] = vnVarSPosN.size();
		
	}
	bam_close(finBam);
	if(m_sBamFileN != "")		bam_close(finBamN);
	
	if(m_bIsDebug) 	cout << "\t\t\t\t\tEND: " << nSIdx << "-" << nEIdx << endl;

}


bool CRD::FilterRead(string sChr, vector<int> &vnNorSPos, vector<int> &vnNorEPos, 
		vector<int> &vnVarSPos, vector<int> &vnVarEPos,
		vector<string> &vsNorSeq, vector<string> &vsVarSeq, int &nSumNor, int &nSumVar, int &nCntNor, int &nCntVar)
{
	vector<int> vnNorSPosT = vnNorSPos;
	vector<int> vnNorEPosT = vnNorEPos;
	vector<int> vnVarSPosT = vnVarSPos;
	vector<int> vnVarEPosT = vnVarEPos;
	vector<string> vsNorSeqT = vsNorSeq;
	vector<string> vsVarSeqT = vsVarSeq;
	vnNorSPos.clear();
	vnNorEPos.clear();
	vnVarSPos.clear();
	vnVarEPos.clear();
	vsNorSeq.clear();
	vsVarSeq.clear();
	
	//calculate midium read length
	vector<int> vnReadLen;
	int nMidLen = 0;
	for(int i=0; i<vnNorSPosT.size(); i++)		vnReadLen.push_back(vnNorEPosT[i]-vnNorSPosT[i]+1);
	for(int i=0; i<vnVarSPosT.size(); i++)		vnReadLen.push_back(vnVarEPosT[i]-vnVarSPosT[i]+1);
	sort(vnReadLen.begin(), vnReadLen.end());
	if(vnReadLen.size() != 0)					nMidLen = vnReadLen[vnReadLen.size()/2];
	
	//calculate # of mutations
	int nNorMidMutation = 0;
	int nVarMidMutation = 0;
	int nPivotSPos = -1;
	int nPivotEPos = -1;
	for(int i=0; i<vnNorSPosT.size(); i++)
	{
		if(nPivotSPos == -1 || vnNorSPos[i] < nPivotSPos)	nPivotSPos = vnNorSPos[i];
		if(nPivotEPos == -1 || vnNorEPos[i] > nPivotEPos)	nPivotEPos = vnNorEPos[i];
	}
	for(int i=0; i<vnVarSPosT.size(); i++)
	{
		if(nPivotSPos == -1 || vnVarSPos[i] < nPivotSPos)	nPivotSPos = vnVarSPos[i];
		if(nPivotEPos == -1 || vnVarEPos[i] > nPivotEPos)	nPivotEPos = vnVarEPos[i];
	}

	string sRefSeq;
	if(!m_FaFile.GetSeq(sRefSeq, sChr, nPivotSPos, nPivotEPos))		return -1;
	
	vector<int> vnNorMismatch;
	vector<int> vnVarMismatch;
	
	for(int i=0; i<vnNorSPosT.size(); i++)
	{
		int nCntMismatch = 0;
		for(int j=vnNorSPosT[i]; j<=vnNorEPosT[i]; j++)
			if(vsNorSeqT[i][j-vnNorSPosT[i]] != sRefSeq[j-nPivotSPos])		nCntMismatch++;
		vnNorMismatch.push_back(nCntMismatch);
	}
	for(int i=0; i<vnVarSPosT.size(); i++)
	{
		int nCntMismatch = 0;
		for(int j=vnVarSPosT[i]; j<=vnVarEPosT[i]; j++)
			if(vsVarSeqT[i][j-vnVarSPosT[i]] != sRefSeq[j-nPivotSPos])		nCntMismatch++;
		vnVarMismatch.push_back(nCntMismatch);
	}
	int nMidMutation = 5;
	sort(vnVarMismatch.begin(), vnVarMismatch.end());
	sort(vnNorMismatch.begin(), vnNorMismatch.end());
	if(vnNorMismatch.size() > vnVarMismatch.size())		nMidMutation = vnNorMismatch[vnNorMismatch.size()/2];
	else												nMidMutation = vnVarMismatch[vnVarMismatch.size()/2];

	
	
	// read filtering
	int pnVarA[500] = {0,};
	int pnVarC[500] = {0,}; 
	int pnVarG[500] = {0,}; 
	int pnVarT[500] = {0,}; 
	int pnVarAll[500] = {0,};
	int pnNorA[500] = {0,};
	int pnNorC[500] = {0,}; 
	int pnNorG[500] = {0,}; 
	int pnNorT[500] = {0,}; 
	int pnNorAll[500] = {0,};


	for(int i=0; i<vnVarSPosT.size(); i++)
	{
		for(int j=vnVarSPosT[i]; j<=vnVarEPosT[i]; j++)
		{
			int nPos = j-nPivotSPos;
			if(nPos > 500)	continue;
			if(vsVarSeqT[i][j-vnVarSPosT[i]] == 'A')		pnVarA[nPos]++;
			if(vsVarSeqT[i][j-vnVarSPosT[i]] == 'C')		pnVarC[nPos]++;
			if(vsVarSeqT[i][j-vnVarSPosT[i]] == 'G')		pnVarG[nPos]++;
			if(vsVarSeqT[i][j-vnVarSPosT[i]] == 'T')		pnVarT[nPos]++;
			pnVarAll[nPos]++;
		}
	}
	for(int i=0; i<vnNorSPosT.size(); i++)
	{
		for(int j=vnNorSPosT[i]; j<=vnNorEPosT[i]; j++)
		{
			int nPos = j-nPivotSPos;
			if(nPos > 500)	continue;
			if(vsNorSeqT[i][j-vnNorSPosT[i]] == 'A')		pnNorA[nPos]++;
			if(vsNorSeqT[i][j-vnNorSPosT[i]] == 'C')		pnNorC[nPos]++;
			if(vsNorSeqT[i][j-vnNorSPosT[i]] == 'G')		pnNorG[nPos]++;
			if(vsNorSeqT[i][j-vnNorSPosT[i]] == 'T')		pnNorT[nPos]++;
			pnNorAll[nPos]++;
		}
	}

	for(int i=0; i<vnNorSPosT.size(); i++)
	{
		if(vnNorEPosT[i]-vnNorSPosT[i]+1 < nMidLen*0.8)		continue;
		int nCntMismatch = 0;
		for(int j=vnNorSPosT[i]; j<=vnNorEPosT[i]; j++)
		{
			if(j-vnNorSPosT[i] >= vsNorSeqT[i].size())	break;
			if(vsNorSeqT[i][j-vnNorSPosT[i]] == 'D')	continue;
			if(vsNorSeqT[i][j-vnNorSPosT[i]] != sRefSeq[j-nPivotSPos])		nCntMismatch++;
		}
		if(nCntMismatch > min(nMidMutation*2, 6))	continue;

		vnNorSPos.push_back(vnNorSPosT[i]);
		vnNorEPos.push_back(vnNorEPosT[i]);
		vsNorSeq.push_back(vsNorSeqT[i]);
	}
	for(int i=0; i<vnVarSPosT.size(); i++)
	{
	//	cout << vnVarSPosT[i] << "-" << vnVarEPosT[i] << "\t" << vsVarSeqT[i].size() << endl;	
		
		if(vnVarEPosT[i]-vnVarSPosT[i]+1 < nMidLen*0.8)		continue;
		int nCntMismatch = 0;
		for(int j=vnVarSPosT[i]; j<=vnVarEPosT[i]; j++)
		{
			if(j-vnVarSPosT[i] >= vsVarSeqT[i].size())	break;
			if(vsVarSeqT[i][j-vnVarSPosT[i]] == 'D')	continue;
			
			int nPos = j-nPivotSPos;
			if(nPos > 500)		continue;
			if(pnVarAll[nPos] < 4)								continue;
			
			double dVarVaf = 0;
			double dNorVaf = 0;
			if(pnVarAll[nPos] > 0)
			{
				if(vsVarSeqT[i][j-vnVarSPosT[i]] == 'A')		 dVarVaf = (double)pnVarA[nPos]/(double)pnVarAll[nPos];
				if(vsVarSeqT[i][j-vnVarSPosT[i]] == 'C')		 dVarVaf = (double)pnVarC[nPos]/(double)pnVarAll[nPos];
				if(vsVarSeqT[i][j-vnVarSPosT[i]] == 'G')		 dVarVaf = (double)pnVarG[nPos]/(double)pnVarAll[nPos];
				if(vsVarSeqT[i][j-vnVarSPosT[i]] == 'T')		 dVarVaf = (double)pnVarT[nPos]/(double)pnVarAll[nPos];
			}
			if(pnNorAll[nPos] > 0)
			{
				if(vsVarSeqT[i][j-vnVarSPosT[i]] == 'A')		 dNorVaf = (double)pnNorA[nPos]/(double)pnNorAll[nPos];
				if(vsVarSeqT[i][j-vnVarSPosT[i]] == 'C')		 dNorVaf = (double)pnNorC[nPos]/(double)pnNorAll[nPos];
				if(vsVarSeqT[i][j-vnVarSPosT[i]] == 'G')		 dNorVaf = (double)pnNorG[nPos]/(double)pnNorAll[nPos];
				if(vsVarSeqT[i][j-vnVarSPosT[i]] == 'T')		 dNorVaf = (double)pnNorT[nPos]/(double)pnNorAll[nPos];
			}

			if(dVarVaf >= 0.75 && dNorVaf <= 0.25)		continue;

			if(vsVarSeq[i][j-vnVarSPosT[i]] != sRefSeq[nPos])		nCntMismatch++;
		}
		//cout << nCntMismatch << endl;
		if(nCntMismatch > min(nMidMutation*2 + 1, 6))	continue;
		
		vnVarSPos.push_back(vnVarSPosT[i]);
		vnVarEPos.push_back(vnVarEPosT[i]);
		vsVarSeq.push_back(vsVarSeqT[i]);
	}

	// calculate read info.
	nSumNor = 0;
	nSumVar = 0;
	for(int i=0; i<vnNorSPosT.size(); i++)		nSumNor += vnNorEPosT[i]-vnNorSPosT[i]+1;
	for(int i=0; i<vnVarSPosT.size(); i++)		nSumVar += vnVarEPosT[i]-vnVarSPosT[i]+1;
	nCntNor = vnNorSPosT.size();
	nCntVar = vnVarSPosT.size();
	return true;
}

/*
double CRD::CalcCoef(string sChr, vector<string> &vsSeq, vector<int> &vnSPos, vector<int> &vnEPos)
{
	double dCoef = 0;

	int nPivotSPos = -1;
	int nPivotEPos = -1;
	for(int i=0; i<vnSPos.size(); i++)
	{
		if(nPivotSPos == -1 || vnSPos[i] < nPivotSPos)	nPivotSPos = vnSPos[i];
		if(nPivotEPos == -1 || vnEPos[i] > nPivotEPos)	nPivotEPos = vnEPos[i];
	}

	if(nPivotSPos == -1 || nPivotEPos == -1)	return dCoef;
	if(vnSPos.size() <= 1)						return dCoef;

	string sRefSeq;
	if(!m_FaFile.GetSeq(sRefSeq, sChr, nPivotSPos, nPivotEPos))		return -1;
	
	int pnA[500] = {0,};
	int pnC[500] = {0,}; 
	int pnG[500] = {0,}; 
	int pnT[500] = {0,}; 
	int pnAll[500] = {0,};

	for(int i=0; i<vnSPos.size(); i++)
	{
		for(int j=vnSPos[i]; j<=vnEPos[i]; j++)
		{
			int nPos = j-nPivotSPos;
			if(vsSeq[i][j-vnSPos[i]] == 'A')		pnA[nPos]++;
			if(vsSeq[i][j-vnSPos[i]] == 'C')		pnC[nPos]++;
			if(vsSeq[i][j-vnSPos[i]] == 'G')		pnG[nPos]++;
			if(vsSeq[i][j-vnSPos[i]] == 'T')		pnT[nPos]++;
			pnAll[nPos]++;
		}
	}
	
	for(int i=0; i<vnSPos.size(); i++)
	{
		double dReadQual = 1;
		for(int j=vnSPos[i]; j<=vnEPos[i]; j++)
		{
			int nPos = j-nPivotSPos;
			if(vsSeq[i][j-vnSPos[i]] == sRefSeq[nPos])		continue;
			if(pnAll[nPos] < 4)								continue;

			if(vsSeq[i][j-vnSPos[i]] == 'A')		dReadQual *= 0.5 + (double)pnA[nPos]/(double)pnAll[nPos];
			if(vsSeq[i][j-vnSPos[i]] == 'C')		dReadQual *= 0.5 + (double)pnC[nPos]/(double)pnAll[nPos];
			if(vsSeq[i][j-vnSPos[i]] == 'G')		dReadQual *= 0.5 + (double)pnG[nPos]/(double)pnAll[nPos];
			if(vsSeq[i][j-vnSPos[i]] == 'T')		dReadQual *= 0.5 + (double)pnT[nPos]/(double)pnAll[nPos];
		}
		dReadQual /= (double)vnSPos.size();
		dCoef += dReadQual;
	}
	if(dCoef > 2)	dCoef = 2;
	return dCoef;
}*/


bool CRD::AlleleCount(bam_index_t *bamIndex, bamFile &finBam, 
		int nChr, int nPos, string sRef, string sAlt, int nRepeatCnt, int nRepeatLastLen, int &nPivotSPos, 
		vector<int> &vnNorSPos, vector<int> &vnNorEPos, vector<int> &vnVarSPos, vector<int> &vnVarEPos, 
		vector<string> &vsNorSeq, vector<string> &vsVarSeq)
{

	//CASE : seek start position of aligned reads in the bound
	bam_iter_t bamIter;
	bam1_t *b;
	b = bam_init1();
	bamIter = bam_iter_query(bamIndex, nChr, nPos-1, nPos);

	int nRet;

	int nCntVar = 0;
	int nCntNor = 0;
	while((nRet = bam_iter_read(finBam, bamIter, b)) >= 0)
	{
		//calc all depth
		int nSPos, nEPos;
		string sSeq;
		bool bIsVar = GetReadAlign(nSPos, nEPos, sSeq, nPos, sRef, sAlt, b);
		
		if(bIsVar)	nCntVar++;
		else		nCntNor++;
		

		//modify Pos depending Indel
		int nPosEdit = sRef.size()-sAlt.size();
		if(bIsVar)		nEPos -= nPosEdit;

		if(nSPos >= nPos-abs(nPosEdit))										continue;
//		if(nEPos <= nPos+abs(nPosEdit))										continue;
		if(nEPos <= nPos+abs(nPosEdit)*(nRepeatCnt)+nRepeatLastLen)			continue;


		if(nPivotSPos == -1 || (nPivotSPos != -1 && nPivotSPos > nSPos))	nPivotSPos = nSPos;		

		//calc variants depth
		if(bIsVar)
		{
			vnVarSPos.push_back(nSPos);
			vnVarEPos.push_back(nEPos);
			vsVarSeq.push_back(sSeq);
		}
		else
		{
			vnNorSPos.push_back(nSPos);
			vnNorEPos.push_back(nEPos);
			vsNorSeq.push_back(sSeq);
		}
	}

	bam_iter_destroy(bamIter);
	bam_destroy1(b);


	return true;
}


bool CRD::CheckRepeat(string sRef, string sAlt, string sChr, int nPos, int &nRepeatCnt, int &nRepeatLastLen)
{
	if(sRef.size() == sAlt.size())
	{
		nRepeatCnt = 1;
		nRepeatLastLen = 0;
	}
	else if(sRef.size() > sAlt.size())		//deletion
	{
		int nSizeDiff = sRef.size()-sAlt.size();
		string sOrigin = "";
		bool bIsGetRefSeq = m_FaFile.GetSeq(sOrigin, sChr, nPos+1, nPos+nSizeDiff);

		nRepeatCnt = 1;
		nRepeatLastLen = 0;
		while(1)
		{
			nRepeatLastLen = 0;
			bool bIsConsecutiveRepeat = true;
			string sNextSeq = "";
			bIsGetRefSeq = m_FaFile.GetSeq(sNextSeq, sChr, nPos+1+nSizeDiff*nRepeatCnt, nPos+nSizeDiff*(nRepeatCnt+1));
			if(bIsGetRefSeq == false)	break;
			int nDiff = 0;
			int nSame = 0;

			for(int i=0; i<nSizeDiff; i++)
			{
				if(sOrigin[i] == sNextSeq[i])	nSame++;
				else							nDiff++;
				
				if(bIsConsecutiveRepeat)	
				{
					if(sOrigin[i] == sNextSeq[i])	nRepeatLastLen++;
					else							bIsConsecutiveRepeat = false;
				}
			}
			if((double)nDiff/(double)(nSame+nDiff) > 0.33)		break;
			nRepeatCnt++;
		}
	}
	else if(sRef.size() < sAlt.size())		//insertion
	{
		int nSizeDiff = sAlt.size() - sRef.size();
		string sOrigin = sAlt.substr(sRef.size());
		
		nRepeatCnt = 1;
		nRepeatLastLen = 0;
		while(1)
		{
			nRepeatLastLen = 0;
			bool bIsConsecutiveRepeat = true;
			string sNextSeq = "";
			bool bIsGetRefSeq = m_FaFile.GetSeq(sNextSeq, sChr, nPos+1+nSizeDiff*nRepeatCnt, nPos+nSizeDiff*(nRepeatCnt+1));
			if(bIsGetRefSeq == false)	break;
			int nDiff = 0;
			int nSame = 0;

			for(int i=0; i<nSizeDiff; i++)
			{
				if(sOrigin[i] == sNextSeq[i])	nSame++;
				else							nDiff++;
				
				if(bIsConsecutiveRepeat)	
				{
					if(sOrigin[i] == sNextSeq[i])	nRepeatLastLen++;
					else							bIsConsecutiveRepeat = false;
				}
			}
			if((double)nDiff/(double)(nSame+nDiff) > 0.33)		break;
			nRepeatCnt++;
		}
	}
	return true;
}

bool CRD::CalcPearsonCorr(int *pnAllDp, int *pnVarDp, double &dCorr, double &dPval)
{
	int nEPosT = 1000;
	for(int i=1000; i>=0; i--)
	{
		if(pnAllDp[i] != 0)	
		{
			nEPosT = i;
			break;
		}
//		cout << pnAllDp[i] << "\t";
	}
//	cout << endl;
//	for(int j=0; j<nEPosT; j++)		cout << pnVarDp[j] << "\t";
//	cout << endl;


	double *pdX = new double[nEPosT];
	double *pdY = new double[nEPosT];
	for(int i=0; i<nEPosT; i++)
	{
		pdX[i] = (double)pnAllDp[i];
		pdY[i] = (double)pnVarDp[i];
	}


	alglib::real_1d_array x;
	x.setcontent(nEPosT, pdX);
	alglib::real_1d_array y;
	y.setcontent(nEPosT, pdY);
	
	delete [] pdX;
	delete [] pdY;

	double dPearsonCorr = alglib::pearsoncorr2(x,y);
	alglib::ae_int_t nSize = nEPosT;
	double a, b, c;
	alglib::pearsoncorrelationsignificance(dPearsonCorr, nSize, a,b,c);
	

	dCorr = dPearsonCorr*dPearsonCorr;
	if(dPearsonCorr < 0)	dCorr = 0;
	if(dCorr < 0)	dCorr = 0;
	if(dCorr > 1)	dCorr = 1;
	dPval = a;
	return true;
}



bool CRD::GetReadAlign(int &nSPos, int &nEPos, string &sSeq, int nPos, string sRef, string sAlt, bam1_t *b)
{
	bool bIsVar = false;
	int nCigar[1000];
	char cSeq[1000];
	
	memcpy(nCigar, b->data+b->core.l_qname, b->core.n_cigar*4);
	memcpy(cSeq, b->data + b->core.l_qname + b->core.n_cigar*4, (b->core.l_qseq+1)/2);



	//resolve cigar
	int nRefPos = b->core.pos;
	int nSeqPos = 0;

	if(b->core.n_cigar == 1) 	// just one operation, save a loop
	{
		int op = nCigar[0] & 0x0000000f;
		int l = nCigar[0] >> 4;
		nSPos = nRefPos + 1;
		nEPos = nRefPos + l;
		
		if(op == BAM_CMATCH || op == BAM_CEQUAL || op == BAM_CDIFF)
		{
			int nVarPosInSeq = nPos - (nRefPos+1);
			unsigned char base = bam1_seqi(cSeq, nVarPosInSeq); // base
			if(sRef.size() == 1 && sAlt.size() == 1)
			{			
				if((base == 1 && sAlt == "A") || (base == 2 && sAlt == "C"))	bIsVar = true;
				if((base == 4 && sAlt == "G") || (base == 8 && sAlt == "T"))	bIsVar = true;
			}
			
			//make tail seq
			for(int i=0; i<l; i++)		
			{	
				base = bam1_seqi(cSeq, i); // base

				if(base == 1)		sSeq.push_back('A');
				if(base == 2)		sSeq.push_back('C');
				if(base == 4)		sSeq.push_back('G');
				if(base == 8)		sSeq.push_back('T');
				if(base == 15)		sSeq.push_back('N');
			}
		}
	}
	else
	{
		nSPos = nRefPos+1;
		nEPos = nRefPos;	

		for(int i=0; i<b->core.n_cigar; i++)
		{ 
			int op = nCigar[i] & 0x0000000f;
			int l = nCigar[i] >> 4;
		
			if(op == BAM_CMATCH || op == BAM_CEQUAL || op == BAM_CDIFF)
			{
				unsigned char base;
				int nVarPosInSeq = nSeqPos + (nPos - (nRefPos+1));
				
				if(nRefPos+1 <= nPos && nPos <= nRefPos+l)
				{
					base = bam1_seqi(cSeq, nVarPosInSeq); // base

					if(sRef.size() == 1 && sAlt.size() == 1)
					{
						if((base == 1 && sAlt == "A") || (base == 2 && sAlt == "C"))	bIsVar = true;
						if((base == 4 && sAlt == "G") || (base == 8 && sAlt == "T"))	bIsVar = true;
					}
				}
				
				//make tail seq
				for(int j=0; j<l; j++)		
				{	
					base = bam1_seqi(cSeq, nSeqPos+j);
					if(base == 1)		sSeq.push_back('A');
					if(base == 2)		sSeq.push_back('C');
					if(base == 4)		sSeq.push_back('G');
					if(base == 8)		sSeq.push_back('T');
					if(base == 15)		sSeq.push_back('N');
				}
				
				nRefPos += l;
				nSeqPos += l;
				nEPos += l;
			}
			else if(op == BAM_CDEL)
			{
				int nVarPosInSeq = nSeqPos + (nPos - (nRefPos+1));
				
				if(nRefPos+1 == nPos+1 && l == sRef.size()-sAlt.size())
				{
					unsigned char base = bam1_seqi(cSeq, nSeqPos-1); // base
					if((base == 1 && sAlt == "A") || (base == 2 && sAlt == "C"))	bIsVar = true;
					if((base == 4 && sAlt == "G") || (base == 8 && sAlt == "T"))	bIsVar = true;
				}
				
				for(int j=0; j<l; j++)		sSeq.push_back('D');
				
				nRefPos += l;
				nEPos += l;
			}
			else if(op == BAM_CREF_SKIP)
			{
			//	cout << "R" << l << endl;
				fprintf( stderr, "N" );
				fprintf( stderr, "(%d,%d): ", nRefPos, nSeqPos);
				nRefPos += l;
				nSeqPos += l;
				nEPos += l;
			}
			else if(op == BAM_CINS)
			{
			//	cout << "I" << l << endl;
				if(nRefPos+1 == nPos+1 && l == sAlt.size()-sRef.size())
				{
					//make insertion seq
					string sSeqTemp = "";
					for(int m=0; m<l; m++)
					{
						unsigned char base = bam1_seqi(cSeq, nSeqPos+m); //base
						switch(base)
						{
							case 1: sSeqTemp.push_back('A');    break;
							case 2: sSeqTemp.push_back('C');    break;
							case 4: sSeqTemp.push_back('G');    break;
							case 8: sSeqTemp.push_back('T');    break;
							case 15:sSeqTemp.push_back('N');    break;
							default:                break;
						}
					}

					if(sSeqTemp == sAlt.substr(1))
					{
						unsigned char base = bam1_seqi(cSeq, nSeqPos-1); // base
						if((base == 1 && sRef == "A") || (base == 2 && sRef == "C"))	bIsVar = true;
						if((base == 4 && sRef == "G") || (base == 8 && sRef == "T"))	bIsVar = true;
		
				//		cout << nPos << "\t" << nRefPos << "\t" << nSeqPos << "\t" << bIsVar << endl;
					}
				}
				nSeqPos += l;
			}
			else if(op == BAM_CSOFT_CLIP)
			{
				//cout << "S" << l << endl;
				nSeqPos += l;	
			}
			else if(op == BAM_CHARD_CLIP){}
		}
	}
	return bIsVar;
}


int CRD::ConvertChrToTid(string sChr, bam_header_t* bamHeader)
{
	int nId = -1;
	for(int i=0; i<(int)bamHeader->n_targets; i++)
	{
		if(strcmp(sChr.c_str(), bamHeader->target_name[i]) == 0)		
		{
			nId=i;
			break;
		}
	}
	return nId;
}




















