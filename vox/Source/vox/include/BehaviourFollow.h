#pragma once

#include <Behaviour.h>
#include <node/NodeUpdatable.h>

#include <vector>

class PuppetCharacter;

template <class Target>
class BehaviourFollow : public Behaviour {
public:
	std::vector<Target *> targets;
	
	void evaluateBeginContact(b2Fixture * _target) override;
	void evaluateEndContact(b2Fixture * _target) override;
	void update(Step * _step) override;

	BehaviourFollow(PuppetCharacter * _source, float _radius, PuppetGame::BOX2D_CATEGORY _filter);
};



template <class Target>
BehaviourFollow<Target>::BehaviourFollow(PuppetCharacter * _source, float _radius, PuppetGame::BOX2D_CATEGORY _filter) :
	Behaviour(_source, _radius, _filter)
{

}

#include <Box2D\Box2D.h>

template <class Target>
void BehaviourFollow<Target>::evaluateBeginContact(b2Fixture * _target){
	Behaviour::evaluateBeginContact(_target);
	targets.push_back(static_cast<Target *>(_target->GetUserData()));
}
template <class Target>
void BehaviourFollow<Target>::evaluateEndContact(b2Fixture * _target){
	Behaviour::evaluateEndContact(_target);
	for(unsigned long int i = targets.size(); i > 0; --i){
		if(targets.at(i-1) == _target->GetUserData()){
			targets.erase (targets.begin()+(i-1));
			//break;
		}
	}
	if(targets.size() == 0){
		active = false;
	}
}

#include <PuppetCharacter.h>

template <class Target>
void BehaviourFollow<Target>::update(Step * _step){
	Target * closest = nullptr;
	float closestDist = 99999999.f;
	glm::vec3 closestDir(0,0,0);
	for(Target * t : targets){
		glm::vec3 dir = source->getPos(false) - t->getPos(false);
		float distance = dir.x * dir.x + dir.y * dir.y;
		if(distance < closestDist){
			closestDist = distance;
			closest = t;
			closestDir = dir;
		}
	}

	if(closest != nullptr){
		closestDir = glm::normalize(closestDir);
		float speed = 0.f;
		std::cout << closestDir.x << " " << closestDir.y << std::endl;
		//source->torso->applyLinearImpulseRight(closestDir.x*speed);
		//source->torso->applyLinearImpulseUp(closestDir.y*speed);
	}
}