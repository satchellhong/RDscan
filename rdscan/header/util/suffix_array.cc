#include "./suffix_array.h"

using namespace std;

// find the suffix array SA of s[0..n-1] in {1..k}^n
// require s[n]=s[n+1]=s[n+2]=0, n>=2
void CSUFFIX_ARRAY::suffix_array(int *s, int *SA, int n, int k) {
	
	
	int n0=(n+2)/3, n1=(n+1)/3, n2=n/3, n02=n0+n2; 
	int* s12  = new int[n02 + 3];  s12[n02]= s12[n02+1]= s12[n02+2]=0; 
	int* SA12 = new int[n02 + 3]; SA12[n02]=SA12[n02+1]=SA12[n02+2]=0;
	int* s0   = new int[n0];
	int* SA0  = new int[n0];

	// generate positions of mod 1 and mod  2 suffixes
	// the "+(n0-n1)" adds a dummy mod 1 suffix if n%3 == 1
	for (int i=0, j=0;  i < n+(n0-n1);  i++) if (i%3 != 0) s12[j++] = i;

	// lsb radix sort the mod 1 and mod 2 triples
	radixPass(s12 , SA12, s+2, n02, k);
	radixPass(SA12, s12 , s+1, n02, k);  
	radixPass(s12 , SA12, s  , n02, k);
	// find lexicographic names of triples
	int name = 0, c0 = -1, c1 = -1, c2 = -1;
	for (int i = 0;  i < n02;  i++) {
		if (s[SA12[i]] != c0 || s[SA12[i]+1] != c1 || s[SA12[i]+2] != c2) { 
			name++;  c0 = s[SA12[i]];  c1 = s[SA12[i]+1];  c2 = s[SA12[i]+2];
		}
		if (SA12[i] % 3 == 1) { s12[SA12[i]/3]      = name; } // left half
		else                  { s12[SA12[i]/3 + n0] = name; } // right half
	}

	// recurse if names are not yet unique
	if (name < n02) {
		suffix_array(s12, SA12, n02, name);
		// store unique names in s12 using the suffix array 
		for (int i = 0;  i < n02;  i++) s12[SA12[i]] = i + 1;
	} else // generate the suffix array of s12 directly
		for (int i = 0;  i < n02;  i++) SA12[s12[i] - 1] = i; 

	// stably sort the mod 0 suffixes from SA12 by their first character
	for (int i=0, j=0;  i < n02;  i++) if (SA12[i] < n0) s0[j++] = 3*SA12[i];
	radixPass(s0, SA0, s, n0, k);

	// merge sorted SA0 suffixes and sorted SA12 suffixes
	for (int p=0,  t=n0-n1,  k=0;  k < n;  k++) {
#define GetI() (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2)
		int i = GetI(); // pos of current offset 12 suffix
		int j = SA0[p]; // pos of current offset 0  suffix
		if (SA12[t] < n0 ? 
				leq(s[i],       s12[SA12[t] + n0], s[j],       s12[j/3]) :
				leq(s[i],s[i+1],s12[SA12[t]-n0+1], s[j],s[j+1],s12[j/3+n0]))
		{ // suffix from SA12 is smaller
			SA[k] = i;  t++;
			if (t == n02) { // done --- only SA0 suffixes left
				for (k++;  p < n0;  p++, k++) SA[k] = SA0[p];
			}
		} else { 
			SA[k] = j;  p++; 
			if (p == n0)  { // done --- only SA12 suffixes left
				for (k++;  t < n02;  t++, k++) SA[k] = GetI(); 
			}
		}  
	} 

	delete [] s12; 
	delete [] SA12; 
	delete [] SA0; 
	delete [] s0; 
}



void CSUFFIX_ARRAY::Output()
{
	for (int i = 0;  i < (int)m_vS.size();  i++) {
		cout << m_vS[i] << " " ;
	}
	cout << endl;
	for(int i=0; i<(int)m_vSA.size(); i++)
	{
		cout << m_vSA[i] << " ";
	}
	cout << endl;
}

// inputs from {1,..,k}^n for 1 <= n <= nmax
bool CSUFFIX_ARRAY::ConstructSA(vector<int> vnS, int k) {
	int n = vnS.size();
	int *s = new int[n+3];
	int *SA = new int[n+3];
	for (int i=0; i<n;  i++)
	{
		s[i] = vnS[i]; 
		SA[i] = 1;
	}
	s[n] = s[n+1] = s[n+2] = SA[n] = SA[n+1] = SA[n+2] = 0;

	suffix_array(s, SA, n, k);
	
	for(int i=0; i<n; i++)
	{
		m_vS.push_back(s[i]);
		m_vSA.push_back(SA[i]);
	}
	m_sCharSize = k;

	delete [] s;
	delete [] SA;
	return true;
}


int CSUFFIX_ARRAY::Find(vector<int> vnKey)
{
	//binary search
	return BinarySearch(0, m_vSA.size()-1, vnKey);
}

//return = idx, -1:fail
int CSUFFIX_ARRAY::BinarySearch(int nStartPos, int nEndPos, vector<int> vnKey)
{
	if(nEndPos < nStartPos)		return -1;

	int nResult = -1;
	int nHalfPos = (nStartPos+nEndPos)/2;
	int nHalfIdx = m_vSA[nHalfPos];
	
	//whether vnKey and the suffix are same?
	int nFlag=0;	//-1: Go lower bound, 0:same, 1:Go upper bound
	
	for(int i=0; i<(int)vnKey.size(); i++)
	{
		if(nHalfIdx+i >= (int)m_vS.size())	//Go upper bound
		{
			nFlag = 1;
			break;
		}

		if(m_vS[nHalfIdx+i] == vnKey[i]){}
		else if(m_vS[nHalfIdx+i] < vnKey[i])	//Go upper bound
		{
			nFlag = 1;
			break;	
		}
		else if(m_vS[nHalfIdx+i] > vnKey[i])	//Go lower bound
		{
			nFlag = -1;
			break;
		}
	}

	
	//found
	if(nFlag == 0)		nResult = nHalfIdx;
	else if(nFlag == -1)	nResult = BinarySearch(nStartPos, nHalfPos-1, vnKey);
	else if(nFlag == 1)	nResult = BinarySearch(nHalfPos+1, nEndPos, vnKey);

	return nResult;
}
/*
int main(int argv, char *argc[])
{
	vector<int> vnS;
	vnS.push_back(2);
	vnS.push_back(1);
	vnS.push_back(4);
	vnS.push_back(4);
	vnS.push_back(1);
	vnS.push_back(4);
	vnS.push_back(4);
	vnS.push_back(1);
	vnS.push_back(3);
	vnS.push_back(3);
	vnS.push_back(1);

	CSUFFIX_ARRAY SUFFIX_ARRAY;
	SUFFIX_ARRAY.ConstructSA(vnS, 4);
	SUFFIX_ARRAY.Output();

	vector<int> vnKey;
	vnKey.push_back(4);
	vnKey.push_back(1);
	vnKey.push_back(3);
	vnKey.push_back(2);
	

	cout << SUFFIX_ARRAY.Find(vnKey) << endl;

	return 0;
}
*/
