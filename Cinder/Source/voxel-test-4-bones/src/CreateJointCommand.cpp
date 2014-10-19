#pragma once

#include "CreateJointCommand.h"
#include "UI.h"
#include "Node.h"
#include "Joint.h"

CreateJointCommand::CreateJointCommand(std::vector<Joint *> &joints, ci::Vec3d pos, Joint * parent)
{
	this->joints = &joints;
	this->pos = pos;
	this->parent = parent;
}

void CreateJointCommand::execute(){
	Joint * j;
	if(parent != nullptr){
		j = new Joint(parent);
		j->parent = parent;
		parent->building = false;
		parent->children.push_back(j);
	}else{	
		j = new Joint();
	}
	j->setPos(pos);
	UI::selectedNode = j;

	//ci::app::console() << "execute()" << std::endl;
	joints->push_back(j);
}

void CreateJointCommand::unexecute(){
	//ci::app::console() << "unexecute()" << std::endl;
	joints->pop_back(); //probably not the best way, but assuming last element of current joint vector is the newly added joint
}

CreateJointCommand::~CreateJointCommand(void){
}
