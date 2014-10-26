#pragma once

#include "CMD_RotateSelectedJoints.h"
#include "UI.h"
#include "Node.h"
#include "Joint.h"

CMD_RotateSelectedJoints::CMD_RotateSelectedJoints(ci::Vec3d _v, bool _relative) :
	v(_v),
	relative(_relative)
{
}

void CMD_RotateSelectedJoints::execute(){
	//get angle

}

void CMD_RotateSelectedJoints::unexecute(){
	
}

CMD_RotateSelectedJoints::~CMD_RotateSelectedJoints(void){}
