#include "util.h"


bool CPARSER::Parse(string sSentence, vector<char> vcMarker)
{
	string sWord = "";
	for(unsigned long i=0; i<sSentence.size(); i++)
	{
		unsigned long j;
		for(j=0; j<vcMarker.size(); j++)
		{
			if(sSentence[i] == vcMarker[j])	break;
		}

		if(j == vcMarker.size())	sWord.push_back(sSentence[i]);
		else if(sWord != "")	//if Marker and seq are same
		{
			this->m_vsWord.push_back(sWord);
			sWord = "";
		}
	}
	if(sWord != "")		this->m_vsWord.push_back(sWord);

	return true;
}

string CPARSER::GetWord(unsigned long ulIdx)
{
	string sWord = "";
	if(ulIdx >= this->m_vsWord.size() || ulIdx < 0)		return sWord;
	else							return this->m_vsWord[ulIdx];
}






