#pragma once

#include "MeshEntity.h"

#include "MeshInterface.h"
#include "shader/Shader.h"

#include "RenderOptions.h"
#include "MatrixStack.h"

#include <algorithm>

MeshEntity::MeshEntity(MeshInterface * _mesh, Shader * _shader) :
	Entity(),
	NodeRenderable(),
	mesh(_mesh),
	shader(_shader)
{
	++mesh->referenceCount;
	childTransform->addChild(mesh, false);
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
	if(_renderOptions->overrideShader == nullptr){
		_renderOptions->shader = shader;
	}else{
		_renderOptions->shader = _renderOptions->overrideShader;
	}
	Entity::render(_matrixStack, _renderOptions);
}

void MeshEntity::update(Step * _step){
	Entity::update(_step);
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
	}else{
		shader = nullptr;
	}
}

Shader * MeshEntity::getShader(){
	return shader;
}

void MeshEntity::setShaderOnChildren(Shader * _shader){
	for(NodeChild * child : childTransform->children){
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
	for(auto c : childTransform->children){
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

/*void MeshEntity::freezeTransformation(){
	glm::mat4 m = parent->getModelMatrix();

	for(auto & v : mesh->vertices){
		glm::vec4 newV(v.x, v.y, v.z, 0);
		newV = m * newV;
		v.x = newV.x;
		v.y = newV.y;
		v.z = newV.z;
	}
	mesh->dirty = true;
	parent->reset();
}*/