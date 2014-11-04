#pragma once

#include "CMD_KeyAll.h"

#include "NodeAnimatable.h"
#include "Keyframe.h"
#include "Transform.h"

CMD_KeyAll::CMD_KeyAll(float _time) :
	time(_time)
{
	
}

void CMD_KeyAll::execute(){
	int prev;
	for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
		for(Keyframe k : dynamic_cast<NodeAnimatable *>(UI::selectedNodes.at(i))->translateX.keyframes){
			if(k.time != time){

			}
		}
	}
}

void CMD_KeyAll::unexecute(){
	
}

CMD_KeyAll::~CMD_KeyAll(){
}