#pragma once

#include "Commands/CMD_ClearAllProperties.h"

#include <cinder/app/AppBasic.h>

#include "node/NodeAnimatable.h"
#include "Transform.h"
#include "Commands/CMD_ClearProperty.h"

#include "Step.h"

CMD_ClearAllProperties::CMD_ClearAllProperties(NodeAnimatable * _node, float _time) :
	node(_node),
	time(_time)
{
}

bool CMD_ClearAllProperties::execute(){
	ci::app::console() << "execute CMD_KeyAll" << std::endl;
	// Clear keyframe from node's animation objects
	if(firstRun){
		//translateX
		subCmdProc.executeCommand(new CMD_ClearProperty<float>(node->translateX,time,time));
		//translateY
		subCmdProc.executeCommand(new CMD_ClearProperty<float>(node->translateY,time,time));
		//translateZ
		subCmdProc.executeCommand(new CMD_ClearProperty<float>(node->translateZ,time,time));
		//rotate
		subCmdProc.executeCommand(new CMD_ClearProperty<glm::quat>(node->rotate,time,time));
		//scaleX
		subCmdProc.executeCommand(new CMD_ClearProperty<float>(node->scaleX,time,time));
		//scaleY
		subCmdProc.executeCommand(new CMD_ClearProperty<float>(node->scaleY,time,time));
		//scaleZ
		subCmdProc.executeCommand(new CMD_ClearProperty<float>(node->scaleZ,time,time));
	}else{
		subCmdProc.redoAll();
	}
	
	return true;
}

bool CMD_ClearAllProperties::unexecute(){
	subCmdProc.undoAll();
	return true;
}

CMD_ClearAllProperties::~CMD_ClearAllProperties(){
}