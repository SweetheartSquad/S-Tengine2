#pragma once

#include "CMD_Parent.h"
#include "Node.h"

#include "NodeParent.h"
#include "NodeChild.h"
#include "ShiftKiddie.h"
#include "SceneRoot.h"

CMD_Parent::CMD_Parent(SceneRoot * _sceneRoot, NodeChild * _node, NodeParent * _parent) :
	node(_node),
	newParent(_parent),
	oldParent(nullptr),
	sceneRoot(_sceneRoot)
{
}

void CMD_Parent::execute(){
	if(node != nullptr){
		oldParent = node->parent;

		// If the new and old parent are the same, the command doesn't need to do anything
		if(oldParent != newParent){

			ShiftKiddie * sk = dynamic_cast<ShiftKiddie *>(node);
			if(sk != nullptr){
				oldPos = sk->getPos(false);
			}
		
			if(newParent != nullptr){
				newParent->addChild(node);
			}else if(sceneRoot != nullptr){
				sceneRoot->addChild(node);
			}else{
				// Error: no parent provided
			}

			if (oldParent != nullptr){
				index = oldParent->removeChild(node);
			}

			if(sk != nullptr){
				sk->setPos(oldPos, true);
			}
		}
	}
}

void CMD_Parent::unexecute(){
	if(oldParent != newParent){
		ShiftKiddie * sk = dynamic_cast<ShiftKiddie *>(node);
		if(sk != nullptr){
			oldPos = sk->getPos(false);
		}
		if(oldParent != nullptr){
			oldParent->children.insert(oldParent->children.begin() + index, node);
		}
		if(newParent != nullptr){
			newParent->children.pop_back();
		}else if(sceneRoot != nullptr){
			sceneRoot->children.pop_back();
		}
		if(node != nullptr){
			node->parent = oldParent;
		}
		if(sk != nullptr){
			sk->setPos(oldPos, true);
		}
	}
}

CMD_Parent::~CMD_Parent(void){
}