#pragma once

#include "CMD_ScaleSelectedTransformables.h"
#include "CMD_ScaleTransformable.h"

#include "UI.h"
#include "Node.h"
#include "NodeTransformable.h"

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
				// node doesn't have a transform
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
