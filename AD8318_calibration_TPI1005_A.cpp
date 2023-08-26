#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <iomanip>
#include "SerialPort.h"
#include "DAQ.h"
#include "TPI.h"
#include <NIDAQmx.h>
#include "errorHandler.h"
#include <sstream>
#include "CSVtoParam.h"
#include "MATLABSession.h"
#include <engine.h>
#include <mat.h>
#include <cmath>





int main() {
	
	
	CSVParam parameters;
	if(parameters.askforplots()) {
		return 0;
	}
	DAQ daqmx(parameters);
	system("pause");
	daqmx.acquire();
	//daqmx.createCSVFile();

	

	return 0;
}






