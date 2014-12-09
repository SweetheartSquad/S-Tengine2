#pragma once

#include "CMD_RotateSelectedTransformables.h"
#include "CMD_RotateTransformable.h"
#include "UI.h"
#include "NodeTransformable.h"
#include "Joint.h"

CMD_RotateSelectedTransformables::CMD_RotateSelectedTransformables(glm::quat _rotation, bool _relative, CoordinateSpace _space) :
	rotation(_rotation),
	relative(_relative),
	space(_space)
{
}

void CMD_RotateSelectedTransformables::execute(){
	if(firstRun){
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			NodeTransformable * nt = dynamic_cast<NodeTransformable *>(UI::selectedNodes.at(i));
			if(nt != nullptr){
				subCmdProc.executeCommand(new CMD_RotateTransformable(nt, rotation, relative, space));
			}else{
				// node doesn't have a transform
			}
		}
	}else{
		subCmdProc.redoAll();
	}
}

void CMD_RotateSelectedTransformables::unexecute(){
	subCmdProc.undoAll();
}

CMD_RotateSelectedTransformables::~CMD_RotateSelectedTransformables(void){}