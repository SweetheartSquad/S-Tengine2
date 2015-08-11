#pragma once

#include <IkChain.h>
#include <MeshInterface.h>

#ifndef EPSILON
#define EPSILON 0.000001f
#endif

IkJoint_CCD_World::IkJoint_CCD_World(AnimationJoint * _copy) :
	pos(_copy->pos),
	angle(_copy->getAngle()),
	angleSin(sin(angle)),
	angleCos(cos(angle))
{

}

void IkJoint_CCD_World::setAngle(float _angle){	
	angle = _angle;
	angleCos = cos(angle);
	angleSin = sin(angle);
}

float IkJoint_CCD_World::getAngle(){
	return angle;
}

IkChain_CCD::IkChain_CCD(glm::vec2 _pos) :
	AnimationJoint(_pos),
	iterations(1),
	target(0,0),
	targetTolerance(EPSILON)
{
	addJointToChain(this);

	// add an extra verts for the target
	pointVis->mesh->pushVert(Vertex(0,0,0));
	lineVis->mesh->pushVert(Vertex(0,0,0));
	lineVis->mesh->pushVert(Vertex(0,0,0));
}

void IkChain_CCD::update(Step * _step){
	// perform IK iterations
	unsigned long int i = 0;
	while(++i <= iterations && iterate());

	// update target display
	glm::vec4 v = glm::inverse(childTransform->getModelMatrix()) * glm::vec4(target.x, target.y, 0, 1);
	
	if(pointVis->isVisible()){
		pointVis->mesh->vertices.back().x = v.x;
		pointVis->mesh->vertices.back().y = v.y;
		pointVis->mesh->vertices.back().green = 0;
		pointVis->mesh->vertices.back().blue = 0;
	}
	
	if(lineVis->isVisible()){
		lineVis->mesh->vertices.at(lineVis->mesh->vertices.size()-2).x = 0;
		lineVis->mesh->vertices.at(lineVis->mesh->vertices.size()-2).y = 0;
		lineVis->mesh->vertices.at(lineVis->mesh->vertices.size()-2).green = 0;
		lineVis->mesh->vertices.at(lineVis->mesh->vertices.size()-2).blue = 0;
		lineVis->mesh->vertices.back().x = v.x;
		lineVis->mesh->vertices.back().y = v.y;
		lineVis->mesh->vertices.back().green = 0;
		lineVis->mesh->vertices.back().blue = 0;
	}

	// transformation data gets applied in individual joint updates as the step progresses through the hierarchy
	AnimationJoint::update(_step);
}

void IkChain_CCD::addJointToChain(AnimationJoint * _j){
	if(jointsLocal.size() > 0){
		jointsLocal.back()->addJoint(_j);
	}
	jointsLocal.push_back(_j);

	// make a new world joint to correspond with the new local joint
	jointsWorld.push_back(new IkJoint_CCD_World(_j));
}

float IkChain_CCD::getChainLength(){
	float res = 0;
	for(auto b : jointsLocal){
		res += glm::length(b->pos);
	}
	return res;
}





/******************************************************************************

	CCD algorithm implementation adapted for use with our engine from sample code provided here: http://www.ryanjuckett.com/programming/cyclic-coordinate-descent-in-2d/
	Original license follows:

  Copyright (c) 2008-2009 Ryan Juckett
  http://www.ryanjuckett.com/
  
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.
  
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  
  3. This notice may not be removed or altered from any source
     distribution.
******************************************************************************/

bool IkChain_CCD::iterate(){
	// whether we've modified the bones in this iteration
	bool modifiedBones = false;

	// UPDATE WORLD-SPACE BONES
	jointsWorld.front()->pos = jointsLocal.front()->pos;
	jointsWorld.front()->setAngle(jointsLocal.front()->getAngle());

	for(unsigned long int boneIdx = 1; boneIdx < jointsLocal.size(); ++boneIdx ){
		IkJoint_CCD_World * prevWorldBone = jointsWorld.at(boneIdx-1);
		AnimationJoint * curLocalBone = jointsLocal.at(boneIdx);
  
		IkJoint_CCD_World * newWorldBone = jointsWorld.at(boneIdx);
		newWorldBone->pos = prevWorldBone->pos;
		newWorldBone->pos.x += prevWorldBone->angleCos*curLocalBone->pos.x - prevWorldBone->angleSin*curLocalBone->pos.y;
		newWorldBone->pos.y += prevWorldBone->angleSin*curLocalBone->pos.x + prevWorldBone->angleCos*curLocalBone->pos.y;

		newWorldBone->setAngle(prevWorldBone->getAngle() + curLocalBone->getAngle());
	}
  
	// current chain end position
	glm::vec2 chainEnd = jointsWorld.back()->pos;
  
	// PERFORM CCD
    for(int boneIdx = jointsLocal.size()-2; boneIdx >= 0; --boneIdx){
		// vector from the current bone to the end effector position
		glm::vec2 curToEnd = chainEnd - jointsWorld.at(boneIdx)->pos;
		float curToEndMag = glm::length(curToEnd);
  
		// vector from the current bone to the target position
		glm::vec2 curToTarget = target - jointsWorld.at(boneIdx)->pos;
		float curToTargetMag = glm::length(curToTarget);
  
		// calculate rotation to place the end effector on the line from the current joint position to the target postion
		float cosRotAng, sinRotAng;
		float endTargetMag = curToEndMag*curToTargetMag;
		if(endTargetMag > EPSILON){
			cosRotAng = (curToEnd.x*curToTarget.x + curToEnd.y*curToTarget.y) / endTargetMag;
			sinRotAng = (curToEnd.x*curToTarget.y - curToEnd.y*curToTarget.x) / endTargetMag;
		}else{
			cosRotAng = 1;
			sinRotAng = 0;
		}
  
		// calculated rotation 
		float rotAng = acos( glm::max(-1.f, glm::min(1.f, cosRotAng) ) ); // manually clamp the cosine to [-1,1] when computing the angle b/c of possible floating point error
		if(sinRotAng < 0.0){
			rotAng = -rotAng;
		}

		// rotate the current bone by the calculated angle
		jointsLocal.at(boneIdx)->setAngle(jointsLocal.at(boneIdx)->getAngle() + rotAng);

		// keep the chain end up-to-date by rotating it to the new position
		chainEnd.x = jointsWorld.at(boneIdx)->pos.x + cosRotAng*curToEnd.x - sinRotAng*curToEnd.y;
		chainEnd.y = jointsWorld.at(boneIdx)->pos.y + sinRotAng*curToEnd.x + cosRotAng*curToEnd.y;
  
		// check for termination
		glm::vec2 endToTarget = target-chainEnd;
		if(glm::length2(endToTarget) <= targetTolerance){
			// found valid solution; return early
			return false;
		}
  
		// track if the arc length that we moved the end effector was a nontrivial distance
		if(!modifiedBones && abs(rotAng)*curToEndMag > EPSILON){
			modifiedBones = true;
		}
	}

	// if the bones have moved, a solution hasn't been found yet
	// if they haven't moved, there's no point in iterating further b/c we're stuck at an invalid solution
	return modifiedBones;
}