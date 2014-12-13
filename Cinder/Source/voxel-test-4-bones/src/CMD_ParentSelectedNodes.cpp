#pragma once

#include "CMD_ParentSelectedNodes.h"
#include "CMD_Parent.h"

#include "UI.h"
#include "NodeHierarchical.h"
#include "SceneRoot.h"
#include "Joint.h"

CMD_ParentSelectedNodes::CMD_ParentSelectedNodes(SceneRoot * _sceneRoot, NodeParent * _parent) :
	sceneRoot(_sceneRoot),
	parent(_parent)
{
}

void CMD_ParentSelectedNodes::execute(){
	if (!executed){
		
		if(firstRun){
			if(UI::selectedNodes.size() > 0){
				for (unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
					if(parent != UI::selectedNodes.at(i)){
						NodeChild * n = dynamic_cast<NodeChild *>(UI::selectedNodes.at(i));
						if (n != nullptr){
							// add subCommand
					        subCmdProc.executeCommand(new CMD_Parent(sceneRoot, n, parent));
						}
					}
				}
			}
		}else{
			subCmdProc.redoAll();
		}
	}
}

void CMD_ParentSelectedNodes::unexecute(){
	subCmdProc.undoAll();
}


CMD_ParentSelectedNodes::~CMD_ParentSelectedNodes(){
}