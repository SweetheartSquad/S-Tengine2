#pragma once

#include <node/NodeUpdatable.h>

class Accelerometer;
class PuppetCharacter;

class PuppetController : public virtual NodeUpdatable{
public:
	
	Accelerometer * accelerometer;
	PuppetCharacter * puppetCharacter;

	//float trendY;

	PuppetController(Accelerometer * _accelerometer, PuppetCharacter * _puppetCharacter);
	~PuppetController();

	void update(Step* _step) override;

};