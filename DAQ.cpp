#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <NIDAQmx.h>
#include "DAQ.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include "errorHandler.h"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <boost/filesystem.hpp>
#include "TPI.h"
#include <locale>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>



void DAQ::configClkTimingFinite(TaskHandle task_) {
	try {
		if ((error = (DAQmxCfgSampClkTiming(task_, "", sampleRate, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, nbrOfPoints))) < 0) {
			throw Excep("The timing configuration could not be set.\n", error);
		}

	}
	catch (Excep& exception) {
		std::cerr << "ERROR : \n\t- Status Code : \t" << exception.getLevel() << "\n\t- Error label : \t" << exception.what() << std::endl;
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		printf("DAQmx Error: %s\n", errBuff);
	}
}
void DAQ::getData(TaskHandle task_) {
	try {
		if ((error = (DAQmxReadAnalogF64(task_, -1, 10.0, DAQmx_Val_GroupByScanNumber, data, 16000, &read, NULL))) < 0) {
			throw Excep("No data could be acquired.", error);
		}
	}
	catch (Excep& exception) {
		std::cerr << "ERROR : \n\t- Status Code : \t" << exception.getLevel() << "\n\t- Error label : \t" << exception.what() << std::endl;
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		printf("DAQmx Error: %s\n", errBuff);
	}
}
void DAQ::clear(int n) {

	if (n == 1) {
		DAQmxStopTask(taskHandle1);
		DAQmxClearTask(taskHandle1);
	}
	else if (n == 2) {
		DAQmxStopTask(taskHandle2);
		DAQmxClearTask(taskHandle2);
	}
	else {
		DAQmxStopTask(taskHandle3);
		DAQmxClearTask(taskHandle3);
	}
}
void DAQ::configChan(TaskHandle task_, const char* physicalChan_) {
	try {
		if ((error = (DAQmxCreateAIVoltageChan(task_, physicalChan_, "", DAQmx_Val_Diff, min, max, DAQmx_Val_Volts, NULL))) < 0) {
			throw Excep("The channel could not be created. Verify that the N.I. Unit is pluged in.\n", error);
		}

	}
	catch (Excep& exception) {
		std::cerr << "ERROR : \n\t- Status Code : \t" << exception.getLevel() << "\n\t- Error label : \t" << exception.what() << std::endl;
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		printf("DAQmx Error: %s\n", errBuff);
	}
}
void DAQ::createTask(TaskHandle* task_) {
	try {
		if ((error = (DAQmxCreateTask("", task_))) < 0) {
			throw Excep("Task could not be created.\n", error);
		}

	}
	catch (Excep& exception) {
		std::cerr << "ERROR : \n\t- Status Code : \t" << exception.getLevel() << "\n\t- Error label : \t" << exception.what() << std::endl;
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		printf("DAQmx Error: %s\n", errBuff);
	}
}
void DAQ::startTask(TaskHandle task_) {
	try {
		if ((error = (DAQmxStartTask(task_))) < 0) {
			throw Excep("The timing configuration could not start.\n", error);
		}

	}
	catch (Excep& exception) {
		std::cerr << "ERROR : \n\t- Status Code : \t" << exception.getLevel() << "\n\t- Error label : \t" << exception.what() << std::endl;
		DAQmxGetExtendedErrorInfo(errBuff, 2048);
		printf("DAQmx Error: %s\n", errBuff);
	}
}
void DAQ::pickUpData() {
	defineChan();
	if (DICO) {


		getVal(1);
		addDataToplotData(1);

		getVal(1);
		addDataToplotData(2);



		if (DICOtrans) {
			getVal(3);
			addDataToplotData(3);
			clear(3);
		}
	}
	else {

		getVal(1);
		addDataToplotData(1);
	}
	(*unitPt).incrementLevel();
	std::cout << "\n\n";

}
void DAQ::meanData() {
	/*Use this function to make an average on all the data acquired and read in data*/
	float64 sum = 0;
	int inc = 1;
	int i = 0;
	for (int i = 0; i < read; i++) {
		sum += data[i];
	}
	mean = sum / read;
	std::cout << mean << std::endl;
}
void DAQ::addDataToplotData(int n) {
	/*Fill the buffer plotData to make the link between acquisition and CSV file*/
	meanData();
	if (n == 1) {
		plotMeanData1.push_back(mean);
	}
	else if (n == 2) {
		plotMeanData2.push_back(mean);
	}
	else {
		plotMeanData3.push_back(mean);
	}
	std::cout << "\t- Cap" << n << " :  " << mean << " V\n";
}
void DAQ::createCSVFile() {
	str folder = "Plots";
	folder = (PATH / folder).string();
	str fileName;
	if (!DICO) {
		folder += "\\Calibration\\Calibration";
		folder.push_back(calibLabel);
		if (attenuator > 0) {
			folder += "Attenuated";
			char* att = new char[4];
			sprintf(att, "%ddB_", attenuator);
			fileName = folder + "\\calibrationAttenuated";
			fileName += att;
		}
		else {
			fileName = folder + "\\calibration";
		}
	}
	else {
		folder += "\\DICO\\DICO";
		char* dico = new char[1];
		sprintf(dico, "%d", DICONbr);
		folder += dico;
		if (DICOcirc == 0) {
			folder += "\\Open";
		}
		else if (DICOcirc == 1) {
			folder += "\\Load";
		}
		else {
			folder += "\\RFmeter";
		}
		fileName = folder + "\\calibDICO";
	}
	//std::cout << "je suis la : " << fileName << std::endl;
	if (!(boost::filesystem::exists(folder))) {
		boost::filesystem::create_directory(folder);
		std::cout << "Folder created : " << folder << std::endl;
	}
	// Insert time stamp
	const boost::posix_time::ptime time =
		boost::posix_time::microsec_clock::universal_time();

	// Compute  time in a custom format
	std::stringstream stream;
	boost::posix_time::time_facet* facet = new boost::posix_time::time_facet();
	facet->format("%Y%m%d_%H%M%SUTC");
	stream.imbue(std::locale(std::locale::classic(), facet));
	stream << time;
	fileName += stream.str();
	fileName += ".csv";
	std::ofstream file(fileName);

	for (int i = 0; i < nbrOfValue; i += increment) {
		file << i + startLevel;
		file << ",";
		file << plotMeanData1[i];
		if (DICO) {
			file << ',';
			file << plotMeanData2[i];
			if (DICOtrans) {
				file << ',';
				file << plotMeanData3[i];
			}
		}
		file << std::endl;
	}
	std::cout << "CSV file created : " << fileName << "\n" << std::endl;
	file.close();
}
void DAQ::acquire() {
	if (mode == 1) {
		calibrate();
	}
	else {
		pickUpValues();
	}


}
void DAQ::pickUpCont() {
	if (DICO) {

		getVal(1);
		meanData();
		c1 = mean;

		getVal(2);
		meanData();
		c2 = mean;


		if (DICOtrans) {
			getVal(3);
			meanData();
			c3 = mean;
		}
	}
	else {

		getVal(1);
		meanData();
		c1 = mean;
	}
	dispValues();

}
void DAQ::getVal(int n) {
	TaskHandle* task;
	if (n == 1) { task = &taskHandle1; }
	else if (n == 2) { task = &taskHandle2; }
	else { task = &taskHandle3; }
	createTask(task);
	configChan(*task, ptChan1);
	configClkTimingFinite(*task);
	startTask(*task);
	getData(*task);
	clear(n);
}
void DAQ::defineChan() {
	/*To start give the physical channels involved in this task.
	We use from the channel 0 to 15 (3 inputs per channel : "GND"  "-"  "+") --> we use diffential measurements
	between '-' & '+' --> see the terminalConfig parameter set to DAQmx_Val_diif in our case
	for j < 8 : physicalChan_j = "Dev1/aij"
	for j > 7 physicalChan_j = "Dev1/ai(j+8)"      see the instrument input mapping             */
	int calibNum;
	int calibRec;
	if (DICO) {
		calibNum = 2 * (DICONbr - 1);
		calibRec = calibNum + 16;
		if (calibNum >= 8) {
			calibNum += 8;
			calibRec = calibNum - 16;
		}
		sprintf(ptChan1, "Dev1/ai%d", calibNum);
		sprintf(ptChan2, "Dev1/ai%d", calibNum + 1);
		if (DICOtrans) { sprintf(ptChan3, "Dev1/ai%d", calibRec); }

	}

	else {
		calibNum = int(calibLabel) - 65;
		if (calibNum >= 8) { calibNum += 1; }
		sprintf(ptChan1, "Dev1/ai%d", calibNum);
	}
}
void DAQ::pickUpValues() {
	defineChan();
	std::cout << "Current output voltage :\n";

	while (1) {
		pickUpCont();
	}
}
void DAQ::dispValues() {
	std::cout << "\rCap1 : " << c1 << " V\tCap2 : " << c2 << " V\tCap3 : " << c3 << " V";
}
void DAQ::calibrate() {

	while (!((*unitPt).endlevIsReached())) {
		std::cout << "\nCurrent Level :" << (*unitPt).currentLev() << " dBm" << std::endl;
		pickUpData();
		
	}
}
void DAQ::printDAQRecap() {
	std::cout << "*********************************************************************************************************************" << std::endl;
	std::cout << "*************************************  Connecting DAQmx National Instrument  ****************************************" << std::endl;
	std::cout << "*********************************************************************************************************************\n\n" << std::endl;
	std::cout << "NIDAQmx is connected" << std::endl;
	if (!DICO) {
		std::cout << "Calibration of the board :\t\t\t" << calibLabel << std::endl;
		std::cout << "\t - Number of points per value :\t\t" << nbrOfPoints << std::endl;
		std::cout << "\t - Sample Rate (Hz) :\t\t\t" << sampleRate << std::endl;
		std::cout << "\t - Attenuator (dB) :\t\t\t" << attenuator << "\n" << std::endl;
	}
	else {
		std::cout << "Calibration of the boards on DICO nbr :\t\t" << DICONbr << std::endl;
		std::cout << "\t - DICO circuit :\t\t\t" << DICOcirc << std::endl;
		std::cout << "\t - Number of points per value :\t\t" << nbrOfPoints << std::endl;
		std::cout << "\t - Sample Rate (Hz) :\t\t\t" << sampleRate << std::endl;
	}

}
void DAQ::initDaq() {
	if (mode == 1) {
		unitPt = new TPI(Param);
	}
	increment = std::stoi(parameters[4]);
	int i = 7;
	DICO = (std::stoi(parameters[i++]) == 1);
	DICONbr = std::stoi(parameters[i++]);
	DICOcirc = std::stoi(parameters[i++]);
	DICOtrans = (DICOcirc > 1);
	startLevel = std::stoi(parameters[i++]);
	endLevel = std::stoi(parameters[i++]);
	calibLabel = parameters[i++][0];
	sampleRate = std::stod(parameters[i++]);
	nbrOfPoints = std::stoi(parameters[i++]);
	min = std::stod(parameters[i++]);
	max = std::stod(parameters[i++]);
	attenuator = std::stoi(parameters[i++]);
	nbrOfValue = abs(endLevel - startLevel + 1);
	if (attenuator > 0) {
		endLevel = 10;
	}
}

