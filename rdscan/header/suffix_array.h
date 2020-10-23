#ifndef SUFFIX_ARRAY_H
#define SUFFIX_ARRAY_H

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>

using namespace std;


class CSUFFIX_ARRAY
{
private:
	vector<int> m_vS;
	vector<int> m_vSA;
	int m_sCharSize;

public:

	// find the suffix array SA of s[0..n-1] in {1..k}^n
	bool ConstructSA(vector<int> vnS, int k);
	void Output();
	
	//find substring == (vector<int>), then return start idx, -1:fail
	int Find(vector<int> vnKey);

private:
	int BinarySearch(int nStartPos, int nEndPos, vector<int> vnKey);
	void suffix_array(int *s, int *SA, int n, int k);
	
	inline bool leq(int a1, int a2,   int b1, int b2) 
	{ // lexic. order for pairs
		return(a1 < b1 || (a1 == b1 && a2 <= b2)); 
	}                                                   // and triples
	
	inline bool leq(int a1, int a2, int a3,   int b1, int b2, int b3) 
	{
		return(a1 < b1 || (a1 == b1 && leq(a2,a3, b2,b3))); 
	}

	// stably sort a[0..n-1] to b[0..n-1] with keys in 0..K from r
	static void radixPass(int* a, int* b, int* r, int n, int K)  
	{ // count occurrences
		int* c = new int[K + 1];                          // counter array
		for (int i = 0;  i <= K;  i++) c[i] = 0;         // reset counters
		for (int i = 0;  i < n;  i++) c[r[a[i]]]++;    // count occurences
		for (int i = 0, sum = 0;  i <= K;  i++) { // exclusive prefix sums
			int t = c[i];  
			c[i] = sum;  
			sum += t;
		}   
		for (int i = 0;  i < n;  i++) b[c[r[a[i]]]++] = a[i];      // sort
		delete [] c;
	}
};








#endif
