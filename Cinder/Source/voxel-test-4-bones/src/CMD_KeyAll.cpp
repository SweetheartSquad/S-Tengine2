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

bool CMD_KeyAll::execute(){
	ci::app::console() << "execute CMD_KeyAll" << std::endl;
	// Add keyframe to node's animation objects
	if(firstRun){
		//translateX
		subCmdProc.executeCommand(new CMD_KeyProperty(node->translateX,time,time,node->transform->translationVector.x,static_cast<Easing::Type>(UI::interpolationValue)));
		//translateY
		subCmdProc.executeCommand(new CMD_KeyProperty(node->translateY,time,time,node->transform->translationVector.y,static_cast<Easing::Type>(UI::interpolationValue)));
		//translateZ
		subCmdProc.executeCommand(new CMD_KeyProperty(node->translateZ,time,time,node->transform->translationVector.z,static_cast<Easing::Type>(UI::interpolationValue)));
		//rotateX
		subCmdProc.executeCommand(new CMD_KeyProperty(node->rotateX,time,time,node->transform->orientation.x,static_cast<Easing::Type>(UI::interpolationValue)));
		//rotateY
		subCmdProc.executeCommand(new CMD_KeyProperty(node->rotateY,time,time,node->transform->orientation.y,static_cast<Easing::Type>(UI::interpolationValue)));
		//rotateZ
		subCmdProc.executeCommand(new CMD_KeyProperty(node->rotateZ,time,time,node->transform->orientation.z,static_cast<Easing::Type>(UI::interpolationValue)));
		//rotateW
		subCmdProc.executeCommand(new CMD_KeyProperty(node->rotateW,time,time,node->transform->orientation.w,static_cast<Easing::Type>(UI::interpolationValue)));
		//scaleX
		subCmdProc.executeCommand(new CMD_KeyProperty(node->scaleX,time,time,node->transform->scaleVector.x,static_cast<Easing::Type>(UI::interpolationValue)));
		//scaleY
		subCmdProc.executeCommand(new CMD_KeyProperty(node->scaleY,time,time,node->transform->scaleVector.y,static_cast<Easing::Type>(UI::interpolationValue)));
		//scaleZ
		subCmdProc.executeCommand(new CMD_KeyProperty(node->scaleZ,time,time,node->transform->scaleVector.z,static_cast<Easing::Type>(UI::interpolationValue)));
	}else{
		subCmdProc.redoAll();
	}
	
	return true;
}

bool CMD_KeyAll::unexecute(){
	subCmdProc.undoAll();
	return true;
}

CMD_KeyAll::~CMD_KeyAll(){
}