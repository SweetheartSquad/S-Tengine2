#pragma once

#include "CMD_ScaleSelectedJoints.h"

#include "UI.h"
#include "Node.h"
#include "Joint.h"
#include "Transform.h"

CMD_ScaleSelectedJoints::CMD_ScaleSelectedJoints(ci::Vec3d _v) :
	v(_v)
{
}

void CMD_ScaleSelectedJoints::execute(){
	for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
		dynamic_cast<Joint *>(UI::selectedNodes.at(i))->transform->scale(v.x, v.y, v.z);
	}
}

void CMD_ScaleSelectedJoints::unexecute(){
	for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
		dynamic_cast<Joint *>(UI::selectedNodes.at(i))->transform->scale(1/v.x, 1/v.y, 1/v.z);
	}
}

CMD_ScaleSelectedJoints::~CMD_ScaleSelectedJoints(void){}
