#pragma once

#include "Commands/CMD_ScaleSelectedTransformables.h"
#include "Commands/CMD_ScaleTransformable.h"

#include "UI.h"
#include "node/Node.h"
#include "node/NodeTransformable.h"

CMD_ScaleSelectedTransformables::CMD_ScaleSelectedTransformables(ci::Vec3f _scale, bool _relative, CoordinateSpace _space) :
	scale(_scale),
	relative(_relative),
	space(_space)
{
}

bool CMD_ScaleSelectedTransformables::execute(){
	if(firstRun){
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			NodeTransformable * nt = dynamic_cast<NodeTransformable *>(UI::selectedNodes.at(i));
			if(nt != nullptr){
				subCmdProc.executeCommand(new CMD_ScaleTransformable(nt, scale, relative, space));
			}else{
				warn("Node ignored (not transformable)");
			}
		}
	}else{
		subCmdProc.redoAll();
	}
	return true;
}

bool CMD_ScaleSelectedTransformables::unexecute(){
	subCmdProc.undoAll();
	return true;
}

CMD_ScaleSelectedTransformables::~CMD_ScaleSelectedTransformables(void){}
