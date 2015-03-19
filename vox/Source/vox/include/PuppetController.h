#pragma once

#include <node/NodeUpdatable.h>
#include <vector>

class Accelerometer;
class PuppetCharacter;

class PuppetController : public virtual NodeUpdatable{
private:
	static std::vector<bool> idsAvailable;
	Accelerometer * accelerometer;
	PuppetCharacter * puppetCharacter;
public:


	void setPuppetCharacter(PuppetCharacter * _puppet);
	void unassign();
	PuppetController(Accelerometer * _accelerometer);
	~PuppetController();

	void update(Step* _step) override;

};