/*
 * temp-logger.cpp
 * To compile: g++ -Ofast -std=c++11 temp-logger.cpp -o temp-logger
 * To run: ./temp-logger
 *
 * This program updates a text file with temperature readings from
 * two temperature sensors (inside and outside).
*/

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <unistd.h>

int main(int argc,  char **argv) {
        std::cout << "Press Ctrl-C to terminate\n";

        // Temperature Sensor Variables
        // Inside Sensor
        char tempInPath[45] = "/sys/bus/w1/devices/28-1b19d443d7bd/w1_slave";
        char tempInRaw[10];
        char tempIn[20];
        std::ifstream tempInSen;

        // Outside Sensor
        char tempOutPath[45] = "/sys/bus/w1/devices/28-04fdd443ac37/w1_slave";
        char tempOutRaw[10];
        char tempOut[20];
        std::ifstream tempOutSen;

        // Temperature Log Variables
        std::string  tempReadings;
        char tempReadPath[18] = "temp-readings.txt";
        std::ofstream tempReadFile;

        do {
                // Reading Inside Temperature Sensor
                tempInSen.open(tempInPath, std::ifstream::in |  std::ifstream::binary);
                if(tempInSen.is_open()) {
                        tempInSen.seekg(69, std::ios::beg);
                        tempInSen.read(tempInRaw, 10);
                } else {
                        std::cerr << "Failed to open Inside Temperature Sensor (28-00000d891b40)";
                        break;
                }
                tempInSen.close();

                // Converting to Celcius
                float tempInC = std::stof(tempInRaw);
                tempInC = tempInC / 1000;
                sprintf(tempIn, "Inside: %.1fC", tempInC);

                // Reading Outside Temperature Sensor
                tempOutSen.open(tempOutPath, std::ifstream::in | std::ifstream::binary);
                if(tempOutSen.is_open()) {
                        tempOutSen.seekg(69, std::ios::beg);
                        tempOutSen.read(tempOutRaw, 10);
                } else {
                        std::cerr << "Failed to open Outside Temperature Sensor (28-00000d886df7)";
                        break;
                }
                tempOutSen.close();

                // Converting to Celcius
                float tempOutC = std::stof(tempOutRaw);
                tempOutC = tempOutC / 1000;
                sprintf(tempOut, "Outside: %.1fC", tempOutC)

                tempReadings = std::string() + tempIn +  " " + tempOut;

                // Logging to Temperature Readings File
                tempReadFile.open(tempReadPath, std::ofstream::out | std::ofstream::binary);
                tempReadFile << tempReadings;
                tempReadFile.close();

                usleep(1000000);
        } while (true);
}
