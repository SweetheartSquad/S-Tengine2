#pragma once

#include "AccelerometerParser.h"
#include "Accelerometer.h"

#include <StringUtils.h>
#include <Arduino.h>

#include <iostream>

AccelerometerParser::AccelerometerParser(std::string portName):
	Arduino(portName),
	forced(false)
{
}

AccelerometerParser::~AccelerometerParser(){
}

void AccelerometerParser::addAccelerometer(Accelerometer* _accelerometer){
	accelerometers.push_back(_accelerometer);
}

void AccelerometerParser::update(Step* _step){
	Arduino::update(_step);

	int count = 0;
	bool loop = true;
	//Zero out the x,y,z data
	for(Accelerometer * acc : accelerometers) {
		acc->x = 0;
		acc->y = 0;
		acc->z = 0;
	}
	do{
		if(IsConnected()) {
#define SIZE_THING 37*3
			char buffer[SIZE_THING];
			for(unsigned long int i = 0; i < SIZE_THING; ++i){
				buffer[i] = '\0';
			}
			bool test;

			ReadDataUntil(':', &test);

			int numRead = ReadData(buffer, SIZE_THING-1);
			if(numRead > 0){
				int colon = 0;
				for(unsigned long int i = 0; i < numRead; ++i){
					if(buffer[i] == ':'){
						colon = i+1;
						break;
					}
				}
			
				/*if(numRead < 38){
					buffer[numRead] = '\0';
				}*/
				//std::cout << numRead << ":\t" << buffer << std::endl;
				if(numRead-colon >= 36){
					for(unsigned long int i = 0; i < accelerometers.size(); ++i){
						char * src = buffer+colon + (i*9);
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

						accelerometers.at(i)->x += atoi(x);
						accelerometers.at(i)->y += atoi(y);
						accelerometers.at(i)->z += atoi(z);
					}

					/*std::vector<std::string> dataPacks;// =  StringUtils::split(data, ';');
					dataPacks.push_back(data.substr(0,9));
					dataPacks.push_back(data.substr(9,18));
					dataPacks.push_back(data.substr(18,27));
					dataPacks.push_back(data.substr(27,36));
					for(unsigned long int i = 0; i < dataPacks.size(); ++i){
						if(i >= accelerometers.size()){
							break;
						}
						std::string dataPack = dataPacks.at(i);
						//std::vector<std::string> vals = StringUtils::split(dataPack, ',');
						/*if(vals.size() >= 3){
							accelerometers.at(i)->x += atoi(vals.at(0).c_str());
							accelerometers.at(i)->y += atoi(vals.at(1).c_str());
							accelerometers.at(i)->z += atoi(vals.at(2).c_str());
						}*/
						/*
						if(dataPack.size() == 9){
							accelerometers.at(i)->x += atoi(dataPack.substr(0, 3).c_str());
							accelerometers.at(i)->y += atoi(dataPack.substr(3, 6).c_str());
							accelerometers.at(i)->z += atoi(dataPack.substr(6, 9).c_str());
						}
						else{
							loop = false;
						}
					}*/
					++count;
				}else{
					loop = false;
				}
			}else{
				loop = false;
			}
		}else{
			loop = false;
		}
	}while(loop);

	if(count != 0){
		for(unsigned long int i = 0; i < accelerometers.size(); ++i){
			accelerometers.at(i)->x /= count;
			accelerometers.at(i)->y /= count;
			accelerometers.at(i)->z /= count;
			accelerometers.at(i)->update(_step);
		}
	}
	//std::cout << accelerometers.at(0)->x << " " << accelerometers.at(0)->y << " " << accelerometers.at(0)->z << std::endl;
}