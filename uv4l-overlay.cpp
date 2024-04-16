/*
 * uv4l-overlay.cpp
 * To compile: g++ -Ofast -std=c++11 uv4l-overlay.cpp -o uv4l-overlay
 * To run: ./uv4l-overlay
 *
 * This program updates the json file used by uv4l raspicam for text overlay.
 * Text overlay must be enabled and the json text file must be identified when
 * starting uv4l.
*/

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include <string>
#include <fstream>
#include <iostream>

int main(int argc, char **argv) {
        std::cout << "Press Ctrl-C to terminate\n";

        char vidDevPath[12] = "/dev/video0";

        // Temperature Log Variables
        char tempReadPath[40] = "temp-readings.txt";
        char tempReadRaw[20];
        std::string tempReadings;
        std::ifstream tempReadFile;

        // Text Overlay Variables
        time_t rawTime;
        char currentTime[10];
        char currentDate[10];
        std::string textOverlay;

        // JSON File Variables
        char jsonPath[100] = "/tmp/text.json";
        std::ofstream jsonFile;

        for (;; sleep(5)) {
                // Opening Video Device
                auto vidDev = open(vidDevPath, O_RDWR | O_NONBLOCK);
                if (vidDev == -1) {
                        std::cerr << "Failed to open " << vidDevPath << '\n';
                        continue;
                }

                // Checking if text overlay is supported
                struct v4l2_queryctrl queryctrl{};
                struct v4l2_control ctrl{0, 1};

                for (queryctrl.id = V4L2_CID_PRIVATE_BASE;; ++queryctrl.id) {
                        if (!ioctl(vidDev, VIDIOC_QUERYCTRL, &queryctrl)) {
                                const std::string name{std::begin(queryctrl.name), std::end(queryctrl.name)};

                                if (name.find("overlay") != std::string::npos) {
                                        ctrl.id = queryctrl.id;
                                        break;
                                }
                        } else {
                                break;
                        }
                }

                if (!ctrl.id) {
                        std::cerr << "Text overlay not supported\n";
                        return EXIT_FAILURE;
                }

                do {
                        // Reading Temperature Log
                        tempReadFile.open(tempReadPath, std::ifstream::in | std::ifstream::binary);
                        if(tempReadFile.is_open()) {
                                getline(tempReadFile, tempReadings);
                        } else {
                                //std::cerr << "Failed to open " << tempReadPath << '\n';
                        }
                        tempReadFile.close();

                        // Getting Current Date and Time
                        time(&rawTime);
                        strftime(currentTime, 9, "%T", localtime(&rawTime));
                        strftime(currentDate, 30, "%d/%m/%y", localtime(&rawTime));

                        textOverlay = std::string() + "Nestcam " + currentTime +  " " + currentDate + "   " + tempReadings;

                        // Font Options: simplex, plain, duplex, complex, triplex, complex_small, script_simplex, script_complex, italic

                        // Writing to Json File
                        jsonFile.open(jsonPath, std::ofstream::out | std::ofstream::binary);
                        jsonFile << "[\n  {\n";
                        // Adds background colour to text
                        //jsonFile << "    \"font\": \"simplex\",\n";
                        //jsonFile << "    \"scale\": 82,\n";
                        //jsonFile << "    \"thickness\": 57,\n";
                        //jsonFile << "    \"red\": 0,\n";
                        //jsonFile << "    \"green\": 0,\n";
                        //jsonFile << "    \"blue\": 0,\n";
                        //jsonFile << "    \"line_type\": 1,\n";
                        //jsonFile << "    \"text_line\": \"-\",\n";
                        //jsonFile << "    \"x\": -350,\n";
                        //jsonFile << "    \"y\": 767\n";
                        //jsonFile << "  },\n  {\n";
                        jsonFile << "    \"font\": \"simplex\",\n";
                        jsonFile << "    \"scale\": 1.4,\n";
                        jsonFile << "    \"thickness\": 2,\n";
                        jsonFile << "    \"red\": 200,\n";
                        jsonFile << "    \"green\": 200,\n";
                        jsonFile << "    \"blue\": 200,\n";
                        jsonFile << "    \"line_type\": 8,\n";
                        jsonFile << "    \"text_line\": \"" << textOverlay << "\",\n";
                        jsonFile << "    \"x\": 20,\n";
                        jsonFile << "    \"y\": 41\n";
                        jsonFile << "  }\n]\n";
                        jsonFile.close();

                        if (ioctl(vidDev, VIDIOC_S_CTRL, &ctrl) == -1) {
                                std::cerr << "Failed to notify the driver\n";
                                break;
                        }
                        usleep(1000000);
                } while (true);
                close(vidDev);
                std::cout << "Retrying to open in 5 seconds...\n";
        }
}