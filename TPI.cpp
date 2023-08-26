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
#include "TPI.h"
#include <stdexcept>
#include <widemath.h>
#include <NIDAQmx.h>
#include <excpt.h>
#include <boost/system/system_error.hpp>
#include "errorHandler.h"




void TPI::printPacket(pckt packet)
{
    int a = 0;
    for (const uint8_t byte : packet) {

        if (static_cast<int>(byte) == 0xAA && a != 0) {
            std::cout << std::dec << std::endl;
            printf("Other packet received : ");
        }
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
        a++;
    }
    std::cout << std::dec << std::endl;
};
void TPI::printLastCommunication() {
    printf("Communication Number : %d.\n", comNbr);
    printf("Packet Sent :    ");
    printPacket(sentPacket);
    printf("Packet Read :    ");
    printPacket(readPacket);
    printf("\n");
};
void TPI::sendPacket(pckt bytesToSend, int p = 0) {
    pckt CompletePacket;
    CompletePacket.push_back(0xAA);
    CompletePacket.push_back(0x55);
    int checkSum = 0;
    for (const uint8_t byte : bytesToSend) {
        checkSum += byte;
        CompletePacket.push_back(byte);
    }
    CompletePacket.push_back(~checkSum);
    if (!TPIPort.writeData(CompletePacket)) {
        throw Excep("Packet not well.");
    }
    sentPacket = CompletePacket;
    readPacket = TPIPort.readData();
    comNbr += 1;
    if (p != 0) {
        printLastCommunication();
    }
};
str TPI::nameofTPIPort() {
    return TPIPort.nameofPort();
}
void TPI::printModeRecap() {
    std::cout << "Mode :\t\t\t\t\t\t" << Mode << std::endl;
    if (Mode == "Square Wave") {
        std::cout << " -  Period (Square Modulation)\t\t\t" << TIME_UP + TIME_DOWN << std::endl;
        std::cout << " -  Duty cycle :\t\t\t\t" << 100 * float(TIME_UP) / (float(TIME_UP) + float(TIME_DOWN)) << "\%" << std::endl;
    }
    std::cout << "Frequency :\t\t\t\t\t" << frequency << " kHz" << std::endl;
    std::cout << "Start Level :\t\t\t\t\t" << startLevel << std::endl;
    std::cout << "Increment :\t\t\t\t\t" << increment << "\n\n" << std::endl;
}
void TPI::RFOn() {
    pckt vect;
    vect.push_back(0x00);
    vect.push_back(0x03);
    vect.push_back(0x08);
    vect.push_back(0x0B);
    vect.push_back(0x01);
    sendPacket(vect);
};
void TPI::RFOff() {
    pckt vect;
    vect.push_back(0x00);
    vect.push_back(0x03);
    vect.push_back(0x08);
    vect.push_back(0x0B);
    vect.push_back(0x00);
    sendPacket(vect);
};
void TPI::levelIncrement(char m = 'r') {
    pckt vect;
    if (m == 'w') {
        vect.push_back(0x00);
        vect.push_back(0x03);
        vect.push_back(0x08);
        vect.push_back(0x1C);
        vect.push_back(increment);
        sendPacket(vect);
    }
    else {
        vect.push_back(0x00);
        vect.push_back(0x02);
        vect.push_back(0x07);
        vect.push_back(0x1C);
        sendPacket(vect);
    }
};
void TPI::Frequency(char m = 'r') {
    if (frequency < 35000 || frequency > 4400000) {
        //throw Excep("Frequency could not be set --> has to be set between 35 Mhz \& 4.4 GHz.");
    }
    pckt vect;
    if (m == 'r') {
        vect.push_back(0x00);
        vect.push_back(0x02);
        vect.push_back(0x07);
        vect.push_back(0x09);
        sendPacket(vect);
    }
    else {
        uint8_t b1 = frequency >> 0;
        uint8_t b2 = frequency >> 8;
        uint8_t b3 = frequency >> 16;
        uint8_t b4 = frequency >> 24;
        vect.push_back(0x00);
        vect.push_back(0x06);
        vect.push_back(0x08);
        vect.push_back(0x09);
        vect.push_back(b1);
        vect.push_back(b2);
        vect.push_back(b3);
        vect.push_back(b4);
        sendPacket(vect);
    }

};
void TPI::setRFdBmOutput(int level) {
    if (level < -90 || level > 0) {
        throw Excep("The RF level out of range : the level has to be set between -90 and 0 dBm.", 0);
    }
    pckt vect;
    vect.push_back(0x00);
    vect.push_back(0x03);
    vect.push_back(0x08);
    vect.push_back(0x0A);
    vect.push_back(level);
    sendPacket(vect);
    currentLevel = level;
};
void TPI::enable() {
    pckt vect;
    vect.push_back(0x00);
    vect.push_back(0x02);
    vect.push_back(0x08);
    vect.push_back(0x01);
    sendPacket(vect);
};
void TPI::SquareWaveParam(char m = 'r') {
    if (TIME_UP < 10 || TIME_UP > 16383 || TIME_DOWN < 10 || TIME_DOWN > 16383) {
        throw Excep("ValueEror --> In square wave mode TIME_UP and TIME_DOWN have to be set between 10 and 16383 ms.\n");
    }

    pckt vect;

    if (m == 'w') {
        squareWaveParam[6] = TIME_UP >> 0;
        squareWaveParam[7] = TIME_UP >> 8;
        squareWaveParam[8] = TIME_DOWN >> 0;
        squareWaveParam[9] = TIME_DOWN >> 8;
        squareWaveParam[10] = 1;
        vect = squareWaveParam;
        vect[4] = 0x08;
        vect.erase(vect.begin());
        vect.erase(vect.begin());
        vect.pop_back();
        sendPacket(vect);
    }
    else {
        printf("Square Wave Parameters : ");
        printPacket(squareWaveParam);
    }
};
void TPI::setSquareWaveMode() {
    pckt vect;
    vect.push_back(0x00);
    vect.push_back(0x04);
    vect.push_back(0x08);
    vect.push_back(0x08);
    vect.push_back(0x01);
    vect.push_back(0x00);
    sendPacket(vect);
    pckt mvect;
    mvect.push_back(0x00);
    mvect.push_back(0x02);
    mvect.push_back(0x07);
    mvect.push_back(0x26);
    sendPacket(mvect);
    squareWaveParam = readPacket;
    SquareWaveParam('w');
};
void TPI::setGeneratorMode() {
    pckt vect;
    vect.push_back(0x00);
    vect.push_back(0x04);
    vect.push_back(0x08);
    vect.push_back(0x08);
    vect.push_back(0x00);
    vect.push_back(0x00);
    sendPacket(vect);

}
void TPI::setTPIMode() {
    Frequency('w');
    if (Mode == "Generator") {
        setGeneratorMode();
    }
    else if (Mode == "Square Wave") {
        setSquareWaveMode();
    }
    else {
        throw Excep("Mode not defined.\n", 0);
    }
}
void TPI::setTPIUp() {
    enable();
    setTPIMode();
    setRFdBmOutput(startLevel);
    Frequency('w');
    levelIncrement('w');
    printModeRecap();
}
int TPI::currentLev() {
    return currentLevel;
}
void TPI::incrementLevel() {
    pckt vect;
    vect.push_back(0x00);
    vect.push_back(0x03);
    vect.push_back(0x08);
    vect.push_back(0x22);
    vect.push_back(0x00);
    RFOff();
    sendPacket(vect);
    currentLevel++;
    RFOn();
}
pckt TPI::lastSentPckt() {
    return sentPacket;
};
pckt TPI::lastReceivedPckt() {
    return readPacket;
};
void TPI::endCom() {
    RFOff();
    TPIPort.close();
}




void TPI::initTPI() {
    int i = 2;
    frequency = uint32_t(std::stoi(parameters[i++]));
    Mode = parameters[i++];
    increment = std::stoi(parameters[i++]);
    TIME_UP = std::stoi(parameters[i++]);
    TIME_DOWN = std::stoi(parameters[i++]);
    DICO = (std::stoi(parameters[i++]) == 1);
    i++;
    i++;
    startLevel = std::stoi(parameters[i++]);
    endLevel = std::stoi(parameters[i++]);
    if (std::stoi(parameters[17]) > 0) {
        endLevel = 10;
    }
}

bool TPI::endlevIsReached() {
    return(currentLevel > endLevel);
}



