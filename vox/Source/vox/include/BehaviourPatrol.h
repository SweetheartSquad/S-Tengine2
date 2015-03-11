#pragma once

#include <Behaviour.h>

#include <vector>
#include <glm\glm.hpp>

class PuppetCharacter;
class b2Fixture;
class Box2DSuperSprite;

class BehaviourPatrol : public Behaviour {
public:
	std::vector<glm::vec3> wayPoints;
	bool forward;
	glm::vec3 * nextWayPoint;
	
	//void evaluateBeginContact(b2Fixture * _target) override;
	//void evaluateEndContact(b2Fixture * _target) override;
	void update(Step * _step) override;

	BehaviourPatrol(glm::vec3 _startPoint, glm::vec3 _endPoint, PuppetCharacter * _source, float _radius, PuppetGame::BOX2D_CATEGORY _filter);
	void addWayPoint(glm::vec3 _wayPoint);
	void addWayPoint(glm::vec3 _wayPoint, unsigned long int _index);
};