#pragma once
#include <vector>
#include <map>

class Accelerometer;
class Arduino;

struct AccelerometerData{
	int x,y,z;
	unsigned long int id;
};

class AccelerometerParser{
public:
	Arduino * arduino;

	std::map<int, AccelerometerData> dataObjects;

	explicit AccelerometerParser(Arduino * _arduino);
	~AccelerometerParser();

	void parseData();
	void mapTo(int _id, Accelerometer * _accelerometer);
};