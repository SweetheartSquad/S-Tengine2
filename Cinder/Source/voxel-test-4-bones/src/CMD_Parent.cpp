#pragma once

#include "CMD_Parent.h"
#include "UI.h"
#include "Node.h"

#include "NodeParent.h"
#include "NodeChild.h"
#include "ShiftKiddie.h"

CMD_Parent::CMD_Parent(NodeChild * _node, NodeParent * _parent) :
	node(_node),
	newParent(_parent),
	oldParent(nullptr)
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
		
			node->parent = newParent;

			if(newParent != nullptr){
				newParent->children.push_back(node);
			}

			if (oldParent != nullptr){
				for(unsigned long int i = 0; i < oldParent->children.size(); ++i){
					if(node == oldParent->children.at(i)){
						index = i;
						oldParent->children.erase(oldParent->children.begin() + index);
						break;
					}
				}
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