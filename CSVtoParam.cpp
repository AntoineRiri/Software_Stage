#include "CSVtoParam.h"
#include "MATLABSession.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include <cstdlib>

void CSVParam::getParam() {
    currentPATH = boost::filesystem::current_path();
    std::ifstream file("setUp.csv");
    str line;
    while (std::getline(file, line)) {
        parameters.push_back(splitLine(line)[1]);
    }
    file.close();
    return;
}

std::vector<str> CSVParam::vector() {
    return parameters;
}

bool CSVParam::askforplots() {
    int resp;
    do {
        std::cout << "Select the mode :\n\n\t- 1. Plot calibrations on MATLAB\n\t- 2. Start a calibration according to the parameters in setUp.csv\n\t- 3. Display real-time voltage values\n\nANS  :  ";
        std::cin >> resp;
    } while (!((resp == 1) || (resp == 2) || (resp == 3)));

    switch (resp) {
    case 1:
        startMATLAB();
        return true;
    case 2:
        std::cout << "\n\t--> Sarting Calibration..." << std::endl;
        mode = 1;
        break;
    case 3:
        std::cout << "\n\t--> Sarting the real-time monitor" << std::endl;
        mode = 2;
        break;
    }
    return false;
}

boost::filesystem::path CSVParam::PATH() {
    return currentPATH;
}

int CSVParam::modeNbr() {
    return mode;
}

void CSVParam::startMATLAB() {
    std::cout << "\n\t--> Starting to plot acquired data in MATLAB..." << std::endl;
    MATengine Pt(currentPATH);
    Pt.exe();
}