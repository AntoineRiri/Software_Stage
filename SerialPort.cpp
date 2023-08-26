#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/throw_exception.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <iomanip>
#include "SerialPort.h"
#include <stdexcept>
#include <widemath.h>
#include <NIDAQmx.h>
#include <excpt.h>
#include <boost/system/system_error.hpp>
#include "errorHandler.h"








void SerialPort::open()
{
    std::cout << "Oppenning Serial Port " << std::endl;
    std::cout << "\"" << nameofPort() << "\"..." << std::endl;
    try {
        serial.open(portName);
        serial.set_option(boost::asio::serial_port_base::baud_rate(baudRate));
        serial.set_option(boost::asio::serial_port_base::character_size(8));
        serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
        serial.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
        serial.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
        std::cout << "Port \"" << nameofPort() << "\" connected.\n" << std::endl;
    }
    catch (const boost::system::system_error& error) {
        const boost::system::error_code& errorCode = error.code();
        str errorLabel = "Le port could not be open : " + str(errorCode.message());
        throw Excep(errorLabel, errorCode.value());
    }
    
};
void SerialPort::close()
{
    serial.close();
};
bool SerialPort::writeData(const pckt bytesToSend)
{   
    int isSent = boost::asio::write(serial, boost::asio::buffer(bytesToSend));
    return isSent;
};
pckt SerialPort::readData()
{
    size_t bufferSize = 100;
    pckt buffer(bufferSize);
    boost::system::error_code error;
    size_t bytesRead = serial.read_some(boost::asio::buffer(buffer), error);
    if (error) {
        throw std::runtime_error("Erreur lors de la lecture des données du port série : " + error.message());
    }
    buffer.resize(bytesRead);
    return buffer;
};
str SerialPort::nameofPort() {
    return portName;
}





void SerialPort::initSerialPort() {
    int i = 0;
    portName = parameters[i++];
    baudRate = std::stoi(parameters[i++]);
}
