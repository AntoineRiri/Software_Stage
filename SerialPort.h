#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <iomanip>
#include "errorHandler.h"

/***********************************************************************************************************************

SerialPort : class aimed at handling the USB port and make the link between the software and the TPI Unit by reading and 
sending datas. 

************************************************************************************************************************/

typedef std::vector<uint8_t> pckt;
typedef std::string str;


class SerialPort {

private:
    boost::asio::io_service io;
    boost::asio::serial_port serial;
    str portName;
    int baudRate;
    std::vector<str> parameters;

public:

    SerialPort(std::vector<str> parameters_) : io(), serial(io), parameters(parameters_)
    {
        initSerialPort();
    }
    void initSerialPort();
    void open();
    void close();
    bool writeData(const pckt bytesToSend);
    pckt readData();
    str nameofPort();
};


