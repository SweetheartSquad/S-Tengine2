#pragma once

#include "CMD_ParentSelectedNodes.h"
#include "CMD_Parent.h"

#include "UI.h"
#include "NodeHierarchical.h"

CMD_ParentSelectedNodes::CMD_ParentSelectedNodes(NodeParent * _parent) :
	parent(_parent)
{
}

void CMD_ParentSelectedNodes::execute(){
	if (!executed){
		
		if(subCommands.size() == 0){
			if(UI::selectedNodes.size() > 0){
				for (unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
					if(parent != UI::selectedNodes.at(i)){
						NodeChild * n = dynamic_cast<NodeChild *>(UI::selectedNodes.at(i));
						if (n != nullptr){
							// add subCommand
							subCommands.push_back(new CMD_Parent(n, parent));
						}
					}
				}
			}
		}

		for(unsigned long int i = 0; i < subCommands.size(); ++i){
			subCommands.at(i)->execute();
			subCommands.at(i)->executed = true;
		}
	}
}

void CMD_ParentSelectedNodes::unexecute(){
	for(unsigned long int i = subCommands.size(); i > 0; --i){
		subCommands.at(i-1)->unexecute();
		subCommands.at(i-1)->executed = false;
	}
}


CMD_ParentSelectedNodes::~CMD_ParentSelectedNodes(){
}