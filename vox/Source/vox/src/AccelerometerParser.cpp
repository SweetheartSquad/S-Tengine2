#pragma once

#include "AccelerometerParser.h"

#include <StringUtils.h>
#include <Arduino.h>

#include <iostream>

AccelerometerParser::AccelerometerParser(Arduino* _arduino):
	arduino(_arduino)
{
}

AccelerometerParser::~AccelerometerParser(){
}

void AccelerometerParser::parseData(){
	int x = 0;
	int y = 0;
	int z = 0;
	int count = 0;
	bool loop = true;
	bool forced = false;

	do{
		if(arduino->IsConnected()) {
			std::string data = arduino->ReadDataUntil('\n', &forced);
			auto vals =  StringUtils::split(data, ' ');
			if(vals.size() >= 3){
				x += atoi(vals.at(0).c_str());
				y += atoi(vals.at(1).c_str());
				z += atoi(vals.at(2).c_str());
				++count;
				if(forced){
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
		x /= count;
		y /= count;
		z /= count;
		std::cout << x << "\t" << y << "\t" << z << std::endl << std::endl;
	}
}

void AccelerometerParser::mapTo(int _id, Accelerometer* _accelerometer){
}