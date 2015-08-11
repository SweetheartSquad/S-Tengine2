#pragma once

#include <Entity.h>
#include <AnimationRig.h>

class IkJoint_CCD_World : public Entity{
public:
	IkJoint_CCD_World(AnimationJoint * _copy);

	 // position in world space
	glm::vec2 pos;
	 // cosing of angle in world space
	float angleCos;
	 // sing of angle in world space
	float angleSin;

	// sets the angle and stores the sin and cos for later use
	void setAngle(float _angle);
	float getAngle();

private:
	 // angle in world space
	float angle;
};

class IkChain_CCD : public AnimationJoint{
public:
	// target position for end of IK chain
	glm::vec2 target;
	// square of distance between end of chain and target which is considered a success
	float targetTolerance;
	// maximum number of iterations to run per update loop
	unsigned long int iterations;
	// list of joints contributing to this IK chain
	std::vector<AnimationJoint *> jointsLocal;
	
	
	explicit IkChain_CCD(glm::vec2 _pos);

	// adds _j as a child of the last joint in this IK chain
	// adds _j to the IK joint list of this chain
	// (this will create a new bone at the end of this IK chain)
	void addJointToChain(AnimationJoint * _j);

	// returns the sum of the length of the bones in this IK chain
	float getChainLength();
	
	virtual void update(Step * _step) override;
private:
	// list of world-space joints corresponding to the joints in this IK chain
	std::vector<IkJoint_CCD_World *> jointsWorld;

	// run an iteration of the CCD algorithm
	// returns whether we should keep iterating:
	//	false if we found a solution or if we didn't change anything
	//	true if we are still searching for a solution
	bool iterate();
};