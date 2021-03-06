#pragma once

#define ARDUINO_WAIT_TIME 2000

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <node/NodeUpdatable.h>

/***************************************************************
*
* Taken from http://playground.arduino.cc/Interfacing/CPPWindows
*
****************************************************************/
class Arduino : public NodeUpdatable
{
  public:
        //Serial comm handler
        HANDLE hSerial;
        //Connection status
        bool connected;
        //Get various information about the connection
        COMSTAT status;
        //Keep track of last error
        DWORD errors;

        //Initialize Serial communication with the given COM port
        Arduino(std::string portName);
        //Close the connection
        //NOTA: for some reason you can't connect again before exiting
        //the program and running it again
        ~Arduino();
        //Read data in a buffer, if nbChar is greater than the
        //maximum number of bytes available, it will return only the
        //bytes available. The function return -1 when nothing could
        //be read, the number of bytes actually read.
        int ReadData(char *buffer, unsigned int nbChar);

		std::string ReadDataUntil(char _until, bool * _forced);
        //Writes data from a buffer through the Serial connection
        //return true on success.
        bool WriteData(const char * buffer, unsigned int nbChar);
        //Check if we are actually connected
        bool IsConnected();

		void connect(std::string _portName);

		virtual void update(Step * _step) override;
		
};

