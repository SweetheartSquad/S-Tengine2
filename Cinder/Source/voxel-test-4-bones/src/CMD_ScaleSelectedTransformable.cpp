#pragma once

#include "CMD_ScaleSelectedTransformable.h"

#include "UI.h"
#include "Node.h"
#include "NodeTransformable.h"

CMD_ScaleSelectedTransformable::CMD_ScaleSelectedTransformable(ci::Vec3f _v, CoordinateSpace _space) :
	v(_v),
	space(_space)
{
}

void CMD_ScaleSelectedTransformable::execute(){
	switch(space){
	case WORLD:
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			NodeTransformable * j = dynamic_cast<NodeTransformable *>(UI::selectedNodes.at(i));
			if(j != NULL){
				glm::vec4 v2(v.x, v.y, v.z, 0);

				NodeHierarchical * nh = dynamic_cast<NodeHierarchical *>(j);
				if(nh != NULL){
					std::vector<glm::mat4> modelMatrixStack;
					NodeParent * parent = nh->parent;
					while(parent != NULL){
						modelMatrixStack.push_back(dynamic_cast<NodeTransformable *>(parent)->transform->getOrientationMatrix());
						parent = dynamic_cast<NodeHierarchical *>(parent)->parent;
					}

					glm::mat4 modelMatrix(1);
					for(unsigned long int i = modelMatrixStack.size(); i > 0; --i){
						modelMatrix = modelMatrix * modelMatrixStack.at(i-1);
					}
					v2 = modelMatrix * v2;
				}
				
				j->transform->scale(v2.x, v2.y, v2.z);
			}
		}
		break;
	case OBJECT:
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			NodeTransformable * j = dynamic_cast<NodeTransformable *>(UI::selectedNodes.at(i));
			if(j != NULL){
				j->transform->scale(v.x, v.y, v.z);
			}
		}
		break;
	}
}

void CMD_ScaleSelectedTransformable::unexecute(){
	switch(space){
	case WORLD:

		break;
	case OBJECT:
		for(unsigned long int i = UI::selectedNodes.size(); i > 0; -- i){
			NodeTransformable * j = dynamic_cast<NodeTransformable *>(UI::selectedNodes.at(i-1));
			if(j != NULL){
				j->transform->scale(1.f/v.x, 1.f/v.y, 1.f/v.z);
			}
		}
		break;
	}
}

CMD_ScaleSelectedTransformable::~CMD_ScaleSelectedTransformable(void){}
