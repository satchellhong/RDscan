/**************************************************************************
*
*	Syntekabio, inc.
*
*  Copyright(c) 2016 Syntekabio, inc., 187, Techro 2-Gil, 
*  Yuseong-Gu, Daejeon, Korea.
*  All rights are reserved. No part of this work covered by the copyright
*  may be reproduced, stored in retrieval systems, in any form or by any
*  means, electronics, mechanical, photocopying, recording or otherwise,
*  without the prior permission of Syntekabio.
*
*  FILE NAME : trace_syn.h
*  LAST MODIFIED DATE : 2016-01-27
*
*  AUTHOR : Sunho Lee
*  E-MAIL : shlee@syntekabio.com
*
*  DESCRIPTION : library for log_trace, time_trace and var_trace
*	You should define below variables when you use trace library
*		1. USE_LOG_TRACE
*		2. USE_TIME_TRACE
*		3. USE_VAR_TRACE
*	You should define below static variables when you use log_trace
*		1. int CLOGTRACE::nLevel = 0;
*		2. std::string CLOGTRACE::LOG_TRACE_PATH = "xxxx.txt";
*	You should define below static variables when you use time_trace
*		1. struct timeval CTIMETRACE::timeStart[10];
*		2. struct timeval CTIMETRACE::timeEnd[10];
*		3. double CTIMETRACE::timeExecution[10] = {0, };
*
**************************************************************************/


#ifndef TRACE_SYN_H_
#define TRACE_SYN_H_

#define USE_VAR_TRACE
#define USE_TIME_TRACE
#define USE_LOG_TRACE


#include <string>
#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <fstream>


class CLOGTRACE
{
public:
	CLOGTRACE(const std::string file, const std::string func, const int line)
	: sFile(file), sFunc(func), sLine(line)
	{
		nLevel++;
		timer = time(NULL);
		t = localtime(&timer);
		fout.open(LOG_TRACE_PATH.c_str(), std::ofstream::app);
		if(nLevel == 1)         fout << std::endl;
		fout << "[" << t->tm_year+1900 << "-" << t->tm_mon+1 << "-" << t->tm_mday << " ";
		fout << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "]\t";
		for(int i=1; i<nLevel; i++)     fout << "\t";
		fout << "<=== " << sLine << ":" << sFunc << "():" << sFile << std::endl;
		fout.close();
	}

	~CLOGTRACE()
	{
		timer = time(NULL);
		t = localtime(&timer);
		fout.open(LOG_TRACE_PATH.c_str(), std::ofstream::app);
		fout << "[" << t->tm_year+1900 << "-" << t->tm_mon+1 << "-" << t->tm_mday << " ";
		fout << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "]\t";
		for(int i=1; i<nLevel; i++)     fout << "\t";
		fout << "===> " << sLine << ":" << sFunc << "():" << sFile << std::endl;
		fout.close();
		nLevel--;
	}

private:
	static int nLevel;
	static std::string LOG_TRACE_PATH;
	
	const std::string sFile, sFunc;
	const int sLine;

	struct tm *t;
	time_t timer;
	std::ofstream fout;
};
	

class CTIMETRACE
{
public:
	static void TimeTraceStart(const int nNum);
	static void TimeTraceEnd(const int nNum);
	static void TimeTraceReport(const int nNum, const std::string sName, 
						const std::string sFile, const std::string sFunc);
private:
	static struct timeval timeStart[10];
	static struct timeval timeEnd[10];
	static double timeExecution[10];
};



//define LOG_TRACE
#ifdef USE_LOG_TRACE
	#define LOG_TRACE_SCOPE		CLOGTRACE LOGTRACE(__FILE__,__func__,__LINE__);
#else
	#define LOG_TRACE_SCOPE
#endif

//define TIME_TRACE
#ifdef USE_TIME_TRACE
	#define TIME_TRACE_START	CTIMETRACE::TimeTraceStart(0);
	#define TIME_TRACE_END		CTIMETRACE::TimeTraceEnd(0);
	#define TIME_TRACE_REPORT(X)	CTIMETRACE::TimeTraceReport(0,X,__FILE__,__func__);
	#define TIME_TRACE_START_1	CTIMETRACE::TimeTraceStart(1);
	#define TIME_TRACE_END_1	CTIMETRACE::TimeTraceEnd(1);
	#define TIME_TRACE_REPORT_1(X)	CTIMETRACE::TimeTraceReport(1,X,__FILE__,__func__);
	#define TIME_TRACE_START_2	CTIMETRACE::TimeTraceStart(2);
	#define TIME_TRACE_END_2	CTIMETRACE::TimeTraceEnd(2);
	#define TIME_TRACE_REPORT_2(X)	CTIMETRACE::TimeTraceReport(2,X,__FILE__,__func__);
	#define TIME_TRACE_START_3	CTIMETRACE::TimeTraceStart(3);
	#define TIME_TRACE_END_3	CTIMETRACE::TimeTraceEnd(3);
	#define TIME_TRACE_REPORT_3(X)	CTIMETRACE::TimeTraceReport(3,X,__FILE__,__func__);
	#define TIME_TRACE_START_4	CTIMETRACE::TimeTraceStart(4);
	#define TIME_TRACE_END_4	CTIMETRACE::TimeTraceEnd(4);
	#define TIME_TRACE_REPORT_4(X)	CTIMETRACE::TimeTraceReport(4,X,__FILE__,__func__);
	#define TIME_TRACE_START_5	CTIMETRACE::TimeTraceStart(5);
	#define TIME_TRACE_END_5	CTIMETRACE::TimeTraceEnd(5);
	#define TIME_TRACE_REPORT_5(X)	CTIMETRACE::TimeTraceReport(5,X,__FILE__,__func__);
	#define TIME_TRACE_START_6	CTIMETRACE::TimeTraceStart(6);
	#define TIME_TRACE_END_6	CTIMETRACE::TimeTraceEnd(6);
	#define TIME_TRACE_REPORT_6(X)	CTIMETRACE::TimeTraceReport(6,X,__FILE__,__func__);
	#define TIME_TRACE_START_7	CTIMETRACE::TimeTraceStart(7);
	#define TIME_TRACE_END_7	CTIMETRACE::TimeTraceEnd(7);
	#define TIME_TRACE_REPORT_7(X)	CTIMETRACE::TimeTraceReport(7,X,__FILE__,__func__);
	#define TIME_TRACE_START_8	CTIMETRACE::TimeTraceStart(8);
	#define TIME_TRACE_END_8	CTIMETRACE::TimeTraceEnd(8);
	#define TIME_TRACE_REPORT_8(X)	CTIMETRACE::TimeTraceReport(8,X,__FILE__,__func__);
	#define TIME_TRACE_START_9	CTIMETRACE::TimeTraceStart(9);
	#define TIME_TRACE_END_9	CTIMETRACE::TimeTraceEnd(9);
	#define TIME_TRACE_REPORT_9(X)	CTIMETRACE::TimeTraceReport(9,X,__FILE__,__func__);
#else
	#define TIME_TRACE_START
	#define TIME_TRACE_END
	#define TIME_TRACE_REPORT
	#define TIME_TRACE_START_1
	#define TIME_TRACE_END_1
	#define TIME_TRACE_REPORT_1
	#define TIME_TRACE_START_2
	#define TIME_TRACE_END_2
	#define TIME_TRACE_REPORT_2
	#define TIME_TRACE_START_3
	#define TIME_TRACE_END_3
	#define TIME_TRACE_REPORT_3
	#define TIME_TRACE_START_4
	#define TIME_TRACE_END_4
	#define TIME_TRACE_REPORT_4
	#define TIME_TRACE_START_5
	#define TIME_TRACE_END_5
	#define TIME_TRACE_REPORT_5
	#define TIME_TRACE_START_6
	#define TIME_TRACE_END_6
	#define TIME_TRACE_REPORT_6
	#define TIME_TRACE_START_7
	#define TIME_TRACE_END_7
	#define TIME_TRACE_REPORT_7
	#define TIME_TRACE_START_8
	#define TIME_TRACE_END_8
	#define TIME_TRACE_REPORT_8
	#define TIME_TRACE_START_9
	#define TIME_TRACE_END_9
	#define TIME_TRACE_REPORT_9
#endif

//define VAR_TRACE
#ifdef USE_VAR_TRACE
	#define VAR_TRACE(_x, args...)	printf("[%s:%s():%d]\t"_x"\n", __FILE__,__func__,__LINE__,##args)
#else
	#define VAR_TRACE(_x, args...)
#endif

#endif
