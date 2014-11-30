#pragma once

#include "CMD_ParentSelectedNodes.h"
#include "CMD_Parent.h"

#include "UI.h"
#include "NodeHierarchical.h"

CMD_ParentSelectedNodes::CMD_ParentSelectedNodes()
{
}

void CMD_ParentSelectedNodes::execute(){

	if (!executed){
		if (UI::selectedNodes.size() > 1){

			// set last element as the parent
			NodeHierarchical * parent = dynamic_cast<NodeHierarchical *>(UI::selectedNodes.back());
			// TODO: check that it can be a parent?

			for (unsigned long int i = 0; i < UI::selectedNodes.size() - 1; ++i){
				NodeHierarchical * n = dynamic_cast<NodeHierarchical *>(UI::selectedNodes.at(i));
				if (n != NULL){
					// check that it isn't already a child of the new parent
					bool isChild = false;
					for (unsigned long int i = 0; i < parent->children.size(); ++i){
						if (n == parent->children.at(i)){
							isChild = true;
						}
					}

					if (!isChild){
						// add subCommand
						subCommands.push_back(new CMD_Parent(n, parent));
					}
				}
			}
		}
	}


	
}

void CMD_ParentSelectedNodes::unexecute(){

}


CMD_ParentSelectedNodes::~CMD_ParentSelectedNodes()
{
}