#pragma once

#include "CMD_TranslateTransformable.h"
#include "Node.h"
#include "Joint.h"
#include "ShiftKiddie.h"

CMD_TranslateTransformable::CMD_TranslateTransformable(NodeTransformable * _node, ci::Vec3d _v, bool _relative, CoordinateSpace _space) :
	node(_node),
	v(_v),
	relative(_relative),
	space(_space)
{
}

void CMD_TranslateTransformable::execute(){
	if(node != nullptr){
		if(firstRun){
			oldPos = node->transform->translationVector;
		}
		ShiftKiddie * sk = dynamic_cast<ShiftKiddie *>(node);
		if(sk != nullptr){
			if(relative){
				switch(space){
				case WORLD:
					sk->setPos(sk->getPos(false) + glm::vec3(v.x, v.y, v.z), true);
					break;
				case OBJECT:
					sk->setPos(sk->getPos(true) + glm::vec3(v.x, v.y, v.z), false);
					break;
				}
			}else{
				sk->setPos(glm::vec3(v.x, v.y, v.z));
			}
		}else{
			// no parent transforms to worry about
			if(relative){
				switch(space){
				case WORLD:
					// doesn't take into account node's orientation/scale?
					node->transform->translationVector += glm::vec3(v.x, v.y, v.z);
					break;

				case OBJECT:
					node->transform->translationVector += glm::vec3(v.x, v.y, v.z);
					break;
				}
			}else{
				node->transform->translationVector = glm::vec3(v.x, v.y, v.z);
			}
		}
	}else{
		// Error: node not provided
	}
}

void CMD_TranslateTransformable::unexecute(){
	if(node != nullptr){
		node->transform->translationVector = oldPos;
	}else{
		// Error: No node provided
	}
}

CMD_TranslateTransformable::~CMD_TranslateTransformable(void){}
