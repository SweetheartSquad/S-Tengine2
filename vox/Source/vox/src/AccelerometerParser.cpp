#pragma once

#include "AccelerometerParser.h"
#include "Accelerometer.h"

#include <StringUtils.h>
#include <Arduino.h>

#include <iostream>

AccelerometerParser::AccelerometerParser(std::string portName):
	Arduino(portName),
	forced(false),
	firstPing(true)
{
	//Make sure our buffer is clear
    PurgeComm(hSerial, PURGE_RXABORT); 
	PurgeComm(hSerial, PURGE_TXABORT);
	PurgeComm(hSerial, PURGE_RXCLEAR);
	PurgeComm(hSerial, PURGE_TXCLEAR);
}

AccelerometerParser::~AccelerometerParser(){
	while(accelerometers.size() > 0){
		delete accelerometers.back();
		accelerometers.pop_back();
	}
}

Accelerometer * AccelerometerParser::addAccelerometer(){
	accelerometers.push_back(new Accelerometer(this));
	return accelerometers.back();
}

void AccelerometerParser::update(Step* _step){
	Arduino::update(_step);
	if(IsConnected()) {
		if(firstPing){
			ClearCommError(this->hSerial, &this->errors, &this->status);
			firstPing = false;
			char ping[1] = {'1'};
			WriteData(ping, 1);
		}
		ClearCommError(this->hSerial, &this->errors, &this->status);
		int inQ = status.cbInQue;
		if(inQ >= LINE_SIZE){
			char * buffer = new char[inQ]();
			int numRead = ReadData(buffer, inQ);
			accumlator += buffer;
			std::string src = getLatestData(accumlator);
			if(src.size() >= LINE_SIZE - 1){
				accumlator.clear();
				for(unsigned long int i = 0; i < accelerometers.size(); ++i){
					char x[4];
					char y[4];
					char z[4];
						
					x[0] = src[0];
					x[1] = src[1];
					x[2] = src[2];
					x[3] = '\0';
						
					y[0] = src[3];
					y[1] = src[4];
					y[2] = src[5];
					y[3] = '\0';
						
					z[0] = src[6];
					z[1] = src[7];
					z[2] = src[8];
					z[3] = '\0';

					accelerometers.at(i)->lx = accelerometers.at(i)->x;
					accelerometers.at(i)->ly = accelerometers.at(i)->y;
					accelerometers.at(i)->lz = accelerometers.at(i)->z;
					accelerometers.at(i)->x = atoi(x);
					accelerometers.at(i)->y = atoi(y);
					accelerometers.at(i)->z = atoi(z);
					accelerometers.at(i)->update(_step);
				}
			}
		}
	}		
}

std::string AccelerometerParser::getLatestData(std::string _acc){
	for(signed long int i = _acc.size() - 1; i >= 0; --i) {
		if(_acc.at(i) == ':') {
			if(_acc.size() - i >= LINE_SIZE) {
				return _acc.substr(i + 1, LINE_SIZE - 1);
			}
		}
	}
	return "";
}