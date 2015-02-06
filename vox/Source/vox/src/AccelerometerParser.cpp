#pragma once

#include "AccelerometerParser.h"
#include "Accelerometer.h"

#include <StringUtils.h>
#include <Arduino.h>

#include <iostream>

AccelerometerParser::AccelerometerParser(std::string portName):
	Arduino(portName)
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
	bool forced = false;
	do{
		if(IsConnected()) {
			std::string data = ReadDataUntil('\n', &forced);
			std::vector<std::string> dataPacks =  StringUtils::split(data, ';');
			for(unsigned long int i = 0; i < dataPacks.size(); ++i){
				if(i >= accelerometers.size()){
					break;
				}
				std::string dataPack = dataPacks.at(i);
				accelerometers.at(i)->x = 0;
				accelerometers.at(i)->y = 0;
				accelerometers.at(i)->z = 0;
				std::vector<std::string> vals = StringUtils::split(dataPack, ',');
				if(vals.size() >= 3){
					accelerometers.at(i)->x += atoi(vals.at(0).c_str());
					accelerometers.at(i)->y += atoi(vals.at(1).c_str());
					accelerometers.at(i)->z += atoi(vals.at(2).c_str());
				}else{
					loop = false;
				}
			}
			++count;
			
			if(forced){
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
			std::cout << i << ":\t" << accelerometers.at(i)->x << "\t" << accelerometers.at(i)->y << "\t" << accelerometers.at(i)->z << std::endl;
		}
	}
	std::cout << std::endl;
}