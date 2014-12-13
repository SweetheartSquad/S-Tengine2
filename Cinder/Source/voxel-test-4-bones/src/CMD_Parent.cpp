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
	sceneRoot(_sceneRoot),
	error(false)
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
				error = !newParent->addChild(node);
			}else if(sceneRoot != nullptr){
				error = !sceneRoot->addChild(node);
			}else{
				// Error: no parent provided
				error = true;
			}

			if(!error){
				if (oldParent != nullptr){
					index = oldParent->removeChild(node);
				}

				if(sk != nullptr){
					sk->setPos(oldPos, true);
				}
			}
		}else{
			// Error: Node is already the parent of child
			error = true;
		}
	}
}

void CMD_Parent::unexecute(){
	if(oldParent != newParent && !error){
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
}

CMD_Parent::~CMD_Parent(void){
}