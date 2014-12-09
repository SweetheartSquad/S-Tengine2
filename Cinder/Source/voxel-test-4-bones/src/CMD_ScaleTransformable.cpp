#pragma once

#include "CMD_ScaleTransformable.h"

#include "NodeTransformable.h"

CMD_ScaleTransformable::CMD_ScaleTransformable(NodeTransformable * _node, ci::Vec3f _scale, bool _relative, CoordinateSpace _space) :
	node(_node),
	scale(_scale),
	relative(_relative),
	space(_space)
{
}

void CMD_ScaleTransformable::execute(){
	if(node != nullptr){
		if(firstRun){
			oldScale = node->transform->scaleVector;
		}
		
		if(relative){
			glm::vec4 v2(scale.x, scale.y, scale.z, 0);
			NodeHierarchical * nh = dynamic_cast<NodeHierarchical *>(node);
			switch(space){
			case WORLD:

				if(nh != nullptr){
					std::vector<glm::mat4> modelMatrixStack;
					NodeParent * parent = nh->parent;
					while(parent != nullptr){
						modelMatrixStack.push_back(dynamic_cast<NodeTransformable *>(parent)->transform->getOrientationMatrix());
						parent = dynamic_cast<NodeHierarchical *>(parent)->parent;
					}

					glm::mat4 modelMatrix(1);
					for(unsigned long int i = modelMatrixStack.size(); i > 0; --i){
						modelMatrix = modelMatrix * modelMatrixStack.at(i-1);
					}
					v2 = modelMatrix * v2;
				}
				
				node->transform->scale(v2.x, v2.y, v2.z);
				break;

			case OBJECT:
				node->transform->scale(scale.x, scale.y, scale.z);
				break;
			}
		}else{
			node->transform->scaleVector = glm::vec3(scale.x, scale.y, scale.z);
		}
	}else{
		// Error: no node provided
	}
}

void CMD_ScaleTransformable::unexecute(){
	if(node != nullptr){
		node->transform->scaleVector = oldScale;
	}else{
		// Error: no node provided
	}
}

CMD_ScaleTransformable::~CMD_ScaleTransformable(void){}