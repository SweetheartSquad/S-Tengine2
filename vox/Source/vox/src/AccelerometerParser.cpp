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
	PurgeComm(hSerial, PURGE_RXCLEAR);
}

AccelerometerParser::~AccelerometerParser(){
}

void AccelerometerParser::addAccelerometer(Accelerometer* _accelerometer){
	accelerometers.push_back(_accelerometer);
}

void AccelerometerParser::update(Step* _step){
	Arduino::update(_step);
	if(IsConnected()) {
		char buffer[LINE_SIZE];
		if(firstPing) {
			 ClearCommError(this->hSerial, &this->errors, &this->status);
		}
		if(this->status.cbInQue == LINE_SIZE){
			int numRead = ReadData(buffer, LINE_SIZE);
			if(numRead > 0 && buffer[0] == ':'){
				if(numRead >= LINE_SIZE){
					firstPing = false;
					for(unsigned long int i = 0; i < accelerometers.size(); ++i){
						char * src = buffer + (i*9) + 1;
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
			}else {
				PurgeComm(hSerial, PURGE_RXCLEAR);
			}
		}else {
			PurgeComm(hSerial, PURGE_RXCLEAR);	
		}
		char ping[1] = {'1'};
		WriteData(ping, 1);
	}		
}