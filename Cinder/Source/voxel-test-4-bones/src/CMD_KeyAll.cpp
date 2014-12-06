#pragma once

#include "CMD_KeyAll.h"

#include <cinder/app/AppBasic.h>

#include "NodeAnimatable.h"
#include "Transform.h"
#include "CMD_KeyProperty.h"

#include "Step.h"

CMD_KeyAll::CMD_KeyAll(NodeAnimatable * _node, float _time) :
	node(_node),
	time(_time)
{
}

void CMD_KeyAll::execute(){
	ci::app::console() << "execute CMD_KeyAll" << std::endl;
	// Add keyframe to node's animation objects
	if(subCommands.size() == 0){
		//translateX
		subCommands.push_back(new CMD_KeyProperty(node->translateX,time,time,node->transform->translationVector.x,UI::interpolation));
		//translateY
		subCommands.push_back(new CMD_KeyProperty(node->translateY,time,time,node->transform->translationVector.y,UI::interpolation));
		//translateZ
		subCommands.push_back(new CMD_KeyProperty(node->translateZ,time,time,node->transform->translationVector.z,UI::interpolation));
		//rotateX
		subCommands.push_back(new CMD_KeyProperty(node->rotateX,time,time,node->transform->orientation.x,UI::interpolation));
		//rotateY
		subCommands.push_back(new CMD_KeyProperty(node->rotateY,time,time,node->transform->orientation.y,UI::interpolation));
		//rotateZ
		subCommands.push_back(new CMD_KeyProperty(node->rotateZ,time,time,node->transform->orientation.z,UI::interpolation));
		//rotateW
		subCommands.push_back(new CMD_KeyProperty(node->rotateW,time,time,node->transform->orientation.w,UI::interpolation));
		//scaleX
		subCommands.push_back(new CMD_KeyProperty(node->scaleX,time,time,node->transform->scaleVector.x,UI::interpolation));
		//scaleY
		subCommands.push_back(new CMD_KeyProperty(node->scaleY,time,time,node->transform->scaleVector.y,UI::interpolation));
		//scaleZ
		subCommands.push_back(new CMD_KeyProperty(node->scaleZ,time,time,node->transform->scaleVector.z,UI::interpolation));
	}

	for (unsigned long int i = 0; i < subCommands.size(); ++i){
		subCommands.at(i)->execute();
	}
	
	/*for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
		NodeAnimatable * node = dynamic_cast<NodeAnimatable *>(UI::selectedNodes.at(i));
		Step s;
		node->update(&s);
	}*/
}

void CMD_KeyAll::unexecute(){
	for(unsigned long int i = subCommands.size(); i > 0; --i){
		subCommands.at(i-1)->unexecute();
	}
}



CMD_KeyAll::~CMD_KeyAll(){
}