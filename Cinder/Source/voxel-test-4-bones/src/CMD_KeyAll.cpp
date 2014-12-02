#pragma once

#include "CMD_KeyAll.h"

#include <cinder/app/AppBasic.h>

#include "NodeAnimatable.h"
#include "Transform.h"
#include "CMD_KeyProperty.h"

#include "Step.h"

CMD_KeyAll::CMD_KeyAll(float _time) :
	time(_time)
{
}

void CMD_KeyAll::execute(){
	ci::app::console() << "execute CMD_KeyAll" << std::endl;
	// Add keyframe to node's animation objects
	if(subCommands.size() == 0){

		//Loop through selected nodes
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			NodeAnimatable * _node = dynamic_cast<NodeAnimatable *>(UI::selectedNodes.at(i));
			if(_node != NULL){
				//translateX
				subCommands.push_back(new CMD_KeyProperty(_node->translateX,time,time,_node->transform->translationVector.x,UI::interpolation));
				//translateY
				subCommands.push_back(new CMD_KeyProperty(_node->translateY,time,time,_node->transform->translationVector.y,UI::interpolation));
				//translateZ
				subCommands.push_back(new CMD_KeyProperty(_node->translateZ,time,time,_node->transform->translationVector.z,UI::interpolation));
				//rotateX
				subCommands.push_back(new CMD_KeyProperty(_node->rotateX,time,time,_node->transform->orientation.x,UI::interpolation));
				//rotateY
				subCommands.push_back(new CMD_KeyProperty(_node->rotateY,time,time,_node->transform->orientation.y,UI::interpolation));
				//rotateZ
				subCommands.push_back(new CMD_KeyProperty(_node->rotateZ,time,time,_node->transform->orientation.z,UI::interpolation));
				//rotateW
				subCommands.push_back(new CMD_KeyProperty(_node->rotateW,time,time,_node->transform->orientation.w,UI::interpolation));
				//scaleX
				subCommands.push_back(new CMD_KeyProperty(_node->scaleX,time,time,_node->transform->scaleVector.x,UI::interpolation));
				//scaleY
				subCommands.push_back(new CMD_KeyProperty(_node->scaleY,time,time,_node->transform->scaleVector.y,UI::interpolation));
				//scaleZ
				subCommands.push_back(new CMD_KeyProperty(_node->scaleZ,time,time,_node->transform->scaleVector.z,UI::interpolation));
			}
		}
	}

	for (unsigned long int i = 0; i < subCommands.size(); ++i){
		subCommands.at(i)->execute();
	}
	
	/*for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
		NodeAnimatable * _node = dynamic_cast<NodeAnimatable *>(UI::selectedNodes.at(i));
		Step s;
		_node->update(&s);
	}*/
}

void CMD_KeyAll::unexecute(){
	for(unsigned long int i = subCommands.size(); i > 0; --i){
		subCommands.at(i-1)->unexecute();
	}
}



CMD_KeyAll::~CMD_KeyAll(){
}