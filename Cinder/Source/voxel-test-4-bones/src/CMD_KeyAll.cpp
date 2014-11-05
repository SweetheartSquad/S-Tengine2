#pragma once

#include "CMD_KeyAll.h"

#include "NodeAnimatable.h"
#include "Keyframe.h"
#include "Transform.h"
#include "CMD_KeyProperty.h"

CMD_KeyAll::CMD_KeyAll(float _time) :
	time(_time)
{
}

void CMD_KeyAll::execute(){

	// Add keyframe to node's animation objects
	if(subCommands.size() == 0){

		int prev;
		//Loop through selected nodes
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			NodeAnimatable * _node = dynamic_cast<NodeAnimatable *>(UI::selectedNodes.at(i));

			//translateX
			subCommands.push_back(new CMD_KeyProperty(&_node->translateX,time,_node->transform->translationVector.x,UI::interpolation));
			//translateY
			subCommands.push_back(new CMD_KeyProperty(&_node->translateY,time,_node->transform->translationVector.y,UI::interpolation));
			//translateZ
			subCommands.push_back(new CMD_KeyProperty(&_node->translateZ,time,_node->transform->translationVector.z,UI::interpolation));
			//rotateX
			subCommands.push_back(new CMD_KeyProperty(&_node->rotateX,time,_node->transform->orientation.x,UI::interpolation));
			//rotateY
			subCommands.push_back(new CMD_KeyProperty(&_node->rotateY,time,_node->transform->orientation.y,UI::interpolation));
			//rotateZ
			subCommands.push_back(new CMD_KeyProperty(&_node->rotateZ,time,_node->transform->orientation.z,UI::interpolation));
			//rotateW
			subCommands.push_back(new CMD_KeyProperty(&_node->rotateW,time,_node->transform->orientation.w,UI::interpolation));
			//scaleX
			subCommands.push_back(new CMD_KeyProperty(&_node->scaleX,time,_node->transform->scaleVector.x,UI::interpolation));
			//scaleY
			subCommands.push_back(new CMD_KeyProperty(&_node->scaleY,time,_node->transform->scaleVector.y,UI::interpolation));
			//scaleZ
			subCommands.push_back(new CMD_KeyProperty(&_node->scaleZ,time,_node->transform->scaleVector.z,UI::interpolation));
		}
	}
	subCommands.at(0)->execute();

	
}

void CMD_KeyAll::unexecute(){
	
}



CMD_KeyAll::~CMD_KeyAll(){
}