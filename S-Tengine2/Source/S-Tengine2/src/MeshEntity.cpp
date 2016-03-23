#pragma once

#include "MeshEntity.h"

#include "MeshInterface.h"
#include "shader/Shader.h"

#include "RenderOptions.h"
#include "MatrixStack.h"
#include <Box.h>

#include <algorithm>

MeshEntity::MeshEntity(MeshInterface * const _mesh, Shader * _shader, bool _configureDefaultVertexAttributes) :
	NodeShadable(_shader),
	mesh(_mesh),
	meshTransform(childTransform->addChild(mesh, true))
{
	if(shader != nullptr && _configureDefaultVertexAttributes) {
		if(!mesh->loaded || mesh->dirty) {
			mesh->load();
		}
		if(!shader->loaded || shader->isDirty()) {
			shader->load();
		}
		mesh->configureDefaultVertexAttributes(_shader);
		++shader->referenceCount;
	}
	++mesh->referenceCount;
}

MeshEntity::~MeshEntity(void){
	/*if(mesh != nullptr){
		mesh->decrementAndDelete();
		mesh = nullptr;
	}*/
}

void MeshEntity::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	// don't bother doing any work if we aren't rendering anyway
	if(!isVisible()){
		return;
	}

	Shader * prev = _renderOptions->shader;
	NodeShadable::applyShader(_renderOptions);
	Entity::render(_matrixStack, _renderOptions);
	_renderOptions->shader = prev;
}

void MeshEntity::setShader(Shader * _shader, bool _configureDefaultAttributes){
	if(shader != nullptr) {
		--shader->referenceCount;
	}
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

Shader * MeshEntity::getShader() const{
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

/*sweet::Box MeshEntity::calcOverallBoundingBox(){
	sweet::Box res = mesh->calcBoundingBox();
	for(auto c : childTransform->children){
		MeshEntity * me = dynamic_cast<MeshEntity *>(c);
		if(me != nullptr){
			res = sweet::Box::bound(res, me->calcOverallBoundingBox());
		}
	}
	return res;
}*/

void MeshEntity::freezeTransformation(){
	mesh->applyTransformation(meshTransform);
	meshTransform->reset();
	mesh->applyTransformation(childTransform);
	childTransform->reset();
}