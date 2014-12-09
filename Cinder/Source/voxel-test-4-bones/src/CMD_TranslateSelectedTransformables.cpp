#pragma once

#include "CMD_TranslateSelectedTransformables.h"
#include "CMD_TranslateTransformable.h"
#include "UI.h"
#include "Node.h"
#include "NodeTransformable.h"

CMD_TranslateSelectedTransformables::CMD_TranslateSelectedTransformables(ci::Vec3d _v, bool _relative, CoordinateSpace _space) :
	v(_v),
	relative(_relative),
	space(_space)
{
}

void CMD_TranslateSelectedTransformables::execute(){
	if(firstRun){
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			NodeTransformable * nt = dynamic_cast<NodeTransformable *>(UI::selectedNodes.at(i));
			if(nt != nullptr){
				subCmdProc.executeCommand(new CMD_TranslateTransformable(nt, v, relative, space));
			}else{
				// node doesn't have a transform
			}
		}
	}else{
		subCmdProc.redoAll();
	}
}

void CMD_TranslateSelectedTransformables::unexecute(){
	subCmdProc.undoAll();
}

CMD_TranslateSelectedTransformables::~CMD_TranslateSelectedTransformables(void){}
