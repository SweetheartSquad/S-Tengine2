#pragma once

#include "Commands/CMD_TranslateTransformable.h"
#include "node/NodeTransformable.h"
#include "node/ShiftKiddie.h"

CMD_TranslateTransformable::CMD_TranslateTransformable(NodeTransformable * _node, ci::Vec3d _v, bool _relative, CoordinateSpace _space) :
	node(_node),
	v(_v),
	relative(_relative),
	space(_space)
{
}

bool CMD_TranslateTransformable::execute(){
	if(node != nullptr){
		if(firstRun){
			oldPos = node->transform->translationVector;
		}
		ShiftKiddie * sk = dynamic_cast<ShiftKiddie *>(node);
		if(sk != nullptr){
			if(relative){
				switch(space){
				case kWORLD:
					sk->setPos(sk->getPos(false) + glm::vec3(v.x, v.y, v.z), true);
					break;
				case kOBJECT:
					sk->setPos(sk->getPos(true) + glm::vec3(v.x, v.y, v.z), false);
					break;
				}
			}else{
				sk->setPos(glm::vec3(v.x, v.y, v.z));
			}
		}else{
			// no parent transforms to worry about
			if(relative){
				glm::vec4 newPos(v.x, v.y, v.z, 1.f);
				switch(space){
				case kWORLD:
					newPos = glm::inverse(node->transform->getOrientationMatrix() * node->transform->getScaleMatrix()) * newPos;
					node->transform->translate(newPos.x, newPos.y, newPos.z);
					break;

				case kOBJECT:
					node->transform->translate(v.x, v.y, v.z);
					break;
				}
			}else{
				node->transform->translationVector = glm::vec3(v.x, v.y, v.z);
			}
		}
	}else{
		// Error: node not provided
	}
	return true;
}

bool CMD_TranslateTransformable::unexecute(){
	if(node != nullptr){
		node->transform->translationVector = oldPos;
	}else{
		// Error: No node provided
	}
	return true;
}

CMD_TranslateTransformable::~CMD_TranslateTransformable(void){}
