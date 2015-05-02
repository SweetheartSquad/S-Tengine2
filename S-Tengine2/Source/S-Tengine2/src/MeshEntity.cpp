#pragma once

#include "MeshEntity.h"

#include "MeshInterface.h"
#include "shader/Shader.h"

#include "RenderOptions.h"
#include "MatrixStack.h"

#include <algorithm>

MeshEntity::MeshEntity(MeshInterface * _mesh, Transform * _transform, Shader * _shader):
	Entity(_transform),
	NodeTransformable(_transform),
	NodeChild(nullptr),
	NodeRenderable(),
	mesh(_mesh),
	shader(_shader)
{
	if(mesh != nullptr){
		++mesh->referenceCount;
	}
	if(shader != nullptr){
		++shader->referenceCount;
	}
}

MeshEntity::~MeshEntity(void){
	if(mesh != nullptr){
		mesh->decrementAndDelete();
		mesh = nullptr;
	}
	if(shader != nullptr){
		shader->decrementAndDelete();		
		shader = nullptr;
	}
}

void MeshEntity::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	//push transform
	if(_matrixStack != nullptr && _renderOptions != nullptr){
		_matrixStack->pushMatrix();
		_matrixStack->applyMatrix(transform->getModelMatrix());
	
		if(_renderOptions->overrideShader == nullptr){
			_renderOptions->shader = shader;
		}else{
			_renderOptions->shader = _renderOptions->overrideShader;
		}
		if(mesh != nullptr){
			mesh->render(_matrixStack, _renderOptions);
		}
		Entity::render(_matrixStack, _renderOptions);
		//pop transform
		_matrixStack->popMatrix();
	}
}

void MeshEntity::update(Step * _step){
	Entity::update(_step);
}

void MeshEntity::removeChildAtIndex(int _index){
	NodeHierarchical::removeChildAtIndex(_index);
}

void MeshEntity::setShader(Shader * _shader, bool _configureDefaultAttributes){
	if(_shader != nullptr){
		if(_shader->isCompiled){
			if(shader != _shader){
				if(shader != nullptr){
					shader->decrementAndDelete();
				}
				shader = _shader;
				++shader->referenceCount;
			}
			if(_configureDefaultAttributes){
				if(mesh != nullptr){
					load();
				}
			}
		}else{
			throw "shader not compiled; cannot setShader";
		}
	}
}

Shader* MeshEntity::getShader(){
	return shader;
}

void MeshEntity::setShaderOnChildren(Shader * _shader){
	for(NodeChild * child : children){
		MeshEntity * me = dynamic_cast<MeshEntity*>(child);
		if(me != nullptr){
			me->setShaderOnChildren(_shader);
		}
	}
	setShader(_shader, true);
}

void MeshEntity::unload(){
	if(loaded){
		if(shader != nullptr){
			shader->unload();
		}

		if(mesh != nullptr){
			mesh->unload();
		}
	}
	Entity::unload();
}

void MeshEntity::load(){
	if(!loaded){
		if(mesh != nullptr){
			mesh->load();
		}
	
		if(shader != nullptr){
			shader->load();
			if(mesh != nullptr){
				mesh->configureDefaultVertexAttributes(shader);
			}
		}
	}
	Entity::load();
}

vox::Box MeshEntity::calcOverallBoundingBox(){
	vox::Box res = mesh->calcBoundingBox();
	for(auto c : children){
		MeshEntity * me = dynamic_cast<MeshEntity *>(c);
		if(me != nullptr){
			vox::Box t = me->calcOverallBoundingBox();
			res.x = std::min(res.x, t.x);
			res.y = std::min(res.y, t.y);
			res.z = std::min(res.z, t.z);
			res.width = std::max(res.x + res.width, t.x+t.width) - res.x;
			res.height = std::max(res.y + res.height, t.y+t.height) - res.y;
			res.depth = std::max(res.z + res.depth, t.z+t.depth) - res.z;
		}
	}
	return res;
}

void MeshEntity::freezeTransformation(){
	glm::mat4 m = transform->getModelMatrix();

	for(auto & v : mesh->vertices){
		glm::vec4 newV(v.x, v.y, v.z, 0);
		newV = m * newV;
		v.x = newV.x;
		v.y = newV.y;
		v.z = newV.z;
	}
	mesh->dirty = true;
	transform->reset();
}