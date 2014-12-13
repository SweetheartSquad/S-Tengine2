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

bool CMD_Parent::execute(){
	if(node != nullptr){
		oldParent = node->parent;

		// If the new and old parent are the same, the command doesn't need to do anything
		if(oldParent != newParent){

			ShiftKiddie * sk = dynamic_cast<ShiftKiddie *>(node);
			if(sk != nullptr){
				oldPos = sk->getPos(false);
			}
			
			if(newParent != nullptr){
				if(!newParent->addChild(node)){
					error("Node could not be parented");
					return false;
				}
			}else if(sceneRoot != nullptr){
				if(!sceneRoot->addChild(node)){
					error("Node could not be parented");
					return false;
				}
			}else{
				warn("No parent provided; command aborted");
				return false;
			}

			if (oldParent != nullptr){
				index = oldParent->removeChild(node);
			}

			if(sk != nullptr){
				sk->setPos(oldPos, true);
			}
		}else{
			warn("Node is already the parent of child; command aborted");
			return false;
		}
	}else{
		error("Node is null");
		return false;
	}
	return true;
}

bool CMD_Parent::unexecute(){
	if(oldParent != newParent){
		ShiftKiddie * sk = dynamic_cast<ShiftKiddie *>(node);
		if(sk != nullptr){
			oldPos = sk->getPos(false);
		}
		if(oldParent != nullptr){
			oldParent->removeChild(node);
		}
		if(newParent != nullptr){
			newParent->removeChild(node);
		}else if(sceneRoot != nullptr){
			sceneRoot->removeChild(node);
		}
		if(node != nullptr){
			node->parent = oldParent;
		}
		if(sk != nullptr){
			sk->setPos(oldPos, true);
		}
	}
	return true;
}

CMD_Parent::~CMD_Parent(void){
}