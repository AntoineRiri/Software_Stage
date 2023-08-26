#pragma once

#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <boost/filesystem.hpp>

typedef std::string str;

class CSVParam {
private:

	int nbrOfParam;
	std::vector<str> parameters;
    boost::filesystem::path currentPATH;
    int mode;
 
public :

    CSVParam() {
        getParam();
    }
    inline std::vector<str> splitLine(const str& line) {
        std::vector<str> result;
        std::stringstream ss(line);
        str cell;

        while (std::getline(ss, cell, ',')) {
            result.push_back(cell);
        }

        return result;
    }
    void getParam();
    std::vector<str> vector();
    bool askforplots();
    boost::filesystem::path PATH();
    int modeNbr();
    void startMATLAB();
};