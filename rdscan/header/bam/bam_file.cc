

#include "bam_file.h"

bool CBAM_FILE::ReadBamFile(const string &sBamFileName)
{
	try
	{
		//open bam file
		bamFile finBamFile = bam_open(sBamFileName.c_str(), "r");
		if(finBamFile == NULL)
		{
			cout << sBamFileName << endl;
			throw "ERROR: cannot open the bam file";
		}
		//Read bam header
		m_header = bam_header_read(finBamFile);
		if(!m_header)
		{
			throw "ERROR: fail to read the header of bam file";
		}

		//Read all reads
		while(1)
		{
			//read one read info
			bam1_t *tempRead = new bam1_t;
			tempRead->data = new uint8_t[SIZE_OF_READ_INFO];
			int nRet = bam_read1(finBamFile, tempRead);
			
			if(nRet < -1)		throw "ERROR: Bam file is truncated";
			else if(nRet == -1)
			{	
				delete [] tempRead->data;
				delete tempRead;
				break;		//file end
			}
			else					//correct reading 
			{
				//store the read in a memory
				m_read.push_back(tempRead);
				m_ulCntRead++;
			}
		}

		//close bam file
		bam_close(finBamFile);
	}
	catch (bad_alloc &ba)
	{
		cout << ba.what() << endl;
		exit(EXIT_FAILURE);
	}
	catch (const char *s)
	{
		cout << s << endl;
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		cout << "ERROR: Unexpected Exception" << endl;
		exit(EXIT_FAILURE);
	}

	return true;
}

void CBAM_FILE::DestroyAllRead()
{
	for(unsigned int i=0; i<m_read.size(); i++)
	{
		delete [] m_read[i]->data;
		delete m_read[i];
	}
}

void CBAM_FILE::DestroyHeader()
{
	bam_header_destroy(m_header);
}



