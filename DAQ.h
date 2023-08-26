#pragma once
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <NIDAQmx.h>
#include "errorHandler.h"
#include "TPI.h"
#include "CSVtoParam.h"

/***********************************************************************************************************************

DAQ : class aimed at handling reads from the N.I.and creating a plot CSV file 

************************************************************************************************************************/

class DAQ {
private:
	int32 error;

	int mode;

	TPI* unitPt;

	TaskHandle taskHandle1;
	TaskHandle taskHandle2;
	TaskHandle taskHandle3;

	int32 read;
	float64 data[16000];

	char errBuff[2048];
	float64 min;
	float64 max;
	int startLevel;
	int endLevel;
	float64 sampleRate;
	int nbrOfPoints;
	int nbrOfValue;

	char* ptChan1 = new char[10];
	char* ptChan2 = new char[10];
	char* ptChan3 = new char[10];

	std::vector<float64> plotMeanData1;
	std::vector<float64> plotMeanData2;
	std::vector<float64> plotMeanData3;
	float64 mean;
	float64 c1; float64 c2; float64 c3;

	char calibLabel;
	bool DICO;
	int DICONbr;
	int DICOcirc;
	bool DICOtrans;
	int attenuator;

	int increment;
	std::vector<str> parameters;
	boost::filesystem::path PATH;
	CSVParam Param;

public:
	DAQ(CSVParam Param_) : error(0), Param(Param_), PATH(Param_.PATH()), attenuator(0) {
		parameters = Param.vector();
		mode = Param.modeNbr();
		initDaq();
		printDAQRecap();

	}

	void pickUpCont();
	void createTask(TaskHandle* task_);
	void configClkTimingFinite(TaskHandle task_);
	void configClkTimingCont(TaskHandle task_);
	void configChan(TaskHandle task_, const char* physicalChan_);
	void startTask(TaskHandle task_);
	void getData(TaskHandle task_);
	void pickUpData();
	void clear(int n);
	void meanData();
	void addDataToplotData(int n);
	void createCSVFile();
	void acquire();
	void printDAQRecap();
	void initDaq();
	void calibrate();
	void dispValues();
	void pickUpValues();
	void getVal(int n);
	void defineChan();
};