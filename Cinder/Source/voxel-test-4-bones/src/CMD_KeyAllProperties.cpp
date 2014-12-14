#pragma once

#include "CMD_KeyAllProperties.h"

#include <cinder/app/AppBasic.h>

#include "NodeAnimatable.h"
#include "Transform.h"
#include "CMD_KeyProperty.h"

#include "Step.h"

CMD_KeyAllProperties::CMD_KeyAllProperties(NodeAnimatable * _node, float _time) :
	node(_node),
	time(_time)
{
}

bool CMD_KeyAllProperties::execute(){
	ci::app::console() << "execute CMD_KeyAll" << std::endl;
	// Add keyframe to node's animation objects
	if(firstRun){
		//translateX
		subCmdProc.executeCommand(new CMD_KeyProperty<float>(node->translateX,time,time,node->transform->translationVector.x,static_cast<Easing::Type>(UI::interpolationValue)));
		//translateY
		subCmdProc.executeCommand(new CMD_KeyProperty<float>(node->translateY,time,time,node->transform->translationVector.y,static_cast<Easing::Type>(UI::interpolationValue)));
		//translateZ
		subCmdProc.executeCommand(new CMD_KeyProperty<float>(node->translateZ,time,time,node->transform->translationVector.z,static_cast<Easing::Type>(UI::interpolationValue)));
		//rotate
		subCmdProc.executeCommand(new CMD_KeyProperty<glm::quat>(node->rotate,time,time,node->transform->orientation,static_cast<Easing::Type>(UI::interpolationValue)));
		//scaleX
		subCmdProc.executeCommand(new CMD_KeyProperty<float>(node->scaleX,time,time,node->transform->scaleVector.x,static_cast<Easing::Type>(UI::interpolationValue)));
		//scaleY
		subCmdProc.executeCommand(new CMD_KeyProperty<float>(node->scaleY,time,time,node->transform->scaleVector.y,static_cast<Easing::Type>(UI::interpolationValue)));
		//scaleZ
		subCmdProc.executeCommand(new CMD_KeyProperty<float>(node->scaleZ,time,time,node->transform->scaleVector.z,static_cast<Easing::Type>(UI::interpolationValue)));
	}else{
		subCmdProc.redoAll();
	}
	
	return true;
}

bool CMD_KeyAllProperties::unexecute(){
	subCmdProc.undoAll();
	return true;
}

CMD_KeyAllProperties::~CMD_KeyAllProperties(){
}