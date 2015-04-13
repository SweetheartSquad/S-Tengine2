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

	PuppetCharacter * const getPuppetCharacter();
	void setPuppetCharacter(PuppetCharacter * _puppet);
	void setPuppetCharacter(PuppetCharacter * _puppet, signed long int _id);
	void unassign();
	PuppetController(Accelerometer * _accelerometer);
	~PuppetController();

	void update(Step* _step) override;

	static void reset();

};