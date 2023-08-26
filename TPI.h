#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <iomanip>
#include "errorHandler.h"
#include "SerialPort.h"
#include "CSVtoParam.h"
#include "CSVtoParam.h"


/***********************************************************************************************************************

TPI : class representing the TPI Unit aimed at handling the Unit Commands.
Methods use TPIPort an instance of SerialPort to exchange datas from the TPI Unit.

************************************************************************************************************************/


typedef std::vector<uint8_t> pckt;
typedef std::string str;


class TPI {
private:
    SerialPort TPIPort;
    pckt sentPacket;
    pckt readPacket;
    std::string LastCommunication;
    std::string Mode;
    int comNbr;
    pckt squareWaveParam;
    int TIME_UP;
    int TIME_DOWN;
    uint32_t frequency;
    int increment;
    bool DICO;
    int startLevel;
    int endLevel;
    int currentLevel;
    std::vector<str> parameters;

public:

    TPI(CSVParam Param_) 
        : parameters(Param_.vector()), comNbr(0), TPIPort(Param_.vector()) {
        initTPI();
        std::cout << "\n***********************************************************************************************************************" << std::endl;
        std::cout << "*******************************************  Connecting Serial TPI Port  **********************************************" << std::endl;
        std::cout << "***********************************************************************************************************************\n" << std::endl;
        try {
            TPIPort.open();
        }
        catch (Excep& exception) {
            std::cerr << "ERROR : \n\t- error level : \t" << exception.getLevel() << "\n\t- error label : \t" << exception.what() << "\n" << std::endl;
            exception.~Excep();
        }
        try {
            setTPIUp();
        }
        catch (Excep& exception) {
            std::cerr << "ERROR : " << exception.what() << std::endl;
            exception.~Excep();
        }

    }

    void initTPI();
    void printPacket(pckt packet);
    void printLastCommunication();
    void sendPacket(pckt bytes, int p);
    str nameofTPIPort();
    void printModeRecap();
    void RFOn();
    void RFOff();
    void levelIncrement(char m);
    void Frequency(char m);
    void setRFdBmOutput(int level);
    void enable();
    void SquareWaveParam(char m);
    void setSquareWaveMode();
    void setGeneratorMode();
    void setTPIMode();
    void setTPIUp();
    void incrementLevel();
    pckt lastSentPckt();
    pckt lastReceivedPckt();
    void endCom();
    int currentLev();
    bool endlevIsReached();
    
};