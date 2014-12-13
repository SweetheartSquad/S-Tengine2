#pragma once

#include <cinder\app\App.h>
#include <cinder\Camera.h>

#include "Joint.h"
#include "Transform.h"

#include "MatrixStack.h"
#include "CinderRenderOptions.h"
#include "Step.h"

unsigned long int Joint::nextId = 0;

Joint::Joint() : 
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	id(nextId)
{
	nextId += 1;
}

Joint::~Joint(){
	delete transform;
	transform = nullptr;

	while(voxels.size() > 0){
		delete voxels.back();
		voxels.pop_back();
	}
}

void Joint::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	CinderRenderOptions * cro = (CinderRenderOptions *)_renderStack;
	if(cro->ciShader != nullptr){
		//gl::enableWireframe();
		cro->ciShader->uniform("pickingColor", Color::hex(pickingColor));
		//colour
		float depth = calculateDepth();
		ColorA color(
			1 - (float)depth / 28.f,
			0.25f + (depth <= 14 ? (float)depth / 14.f : (float)(14-depth) / 14.f),
			(float)depth / 28.f
		);

		if (depth > 28){
			color.set(1, 1, 1, 1);
		}

		gl::color(color);

		//draw joint
		gl::pushModelView();
		_matrixStack->pushMatrix();
			gl::translate(transform->translationVector.x,
							transform->translationVector.y,
							transform->translationVector.z);
			gl::rotate(Quatd(transform->orientation.w,
								transform->orientation.x,
								transform->orientation.y,
								transform->orientation.z));
			gl::scale(transform->scaleVector.x, transform->scaleVector.y, transform->scaleVector.z);
			_matrixStack->applyMatrix(transform->getModelMatrix());

			glUniformMatrix4fv(cro->ciShader->getUniformLocation("modelMatrix"), 1, GL_FALSE, &_matrixStack->currentModelMatrix[0][0]);
			gl::drawSphere(Vec3f(0.f, 0.f, 0.f), 0.05f);

			//draw voxels
            if(!cro->viewJointsOnly){
			    float resolution = cro->voxelPreviewResolution;
			    for(unsigned long int i = 0; i < voxels.size(); ++i){
					voxels.at(i)->render(_matrixStack, _renderStack);
			    }
			}
		
			//draw bones
			gl::color(color);
			cro->ciShader->uniform("pickingColor", Color::hex(pickingColor));
			for(NodeChild * child : children){
				Vec3d cinderTrans(
					dynamic_cast<NodeTransformable *>(child)->transform->translationVector.x,
					dynamic_cast<NodeTransformable *>(child)->transform->translationVector.y,
					dynamic_cast<NodeTransformable *>(child)->transform->translationVector.z
				);
				Quatf boneDir(Vec3f(0.0, 1.0, 0.0), cinderTrans);
				
				gl::pushMatrices();
				_matrixStack->pushMatrix();
					gl::rotate(boneDir);
					_matrixStack->rotate(glm::toMat4(glm::quat(boneDir.w, boneDir.v.x, boneDir.v.y, boneDir.v.z)));
			
					glUniformMatrix4fv(cro->ciShader->getUniformLocation("modelMatrix"), 1, GL_FALSE, &_matrixStack->currentModelMatrix[0][0]);
					gl::drawSolidTriangle(Vec2f(0.05f, 0.f), Vec2f(-0.05f, 0.f), Vec2f(0.f, cinderTrans.length()));

					gl::rotate(Vec3f(0.f, 90.f, 0.f));
					_matrixStack->rotate(glm::toMat4(glm::quat(glm::vec3(0.f, 90.f, 0.f))));
			
					glUniformMatrix4fv(cro->ciShader->getUniformLocation("modelMatrix"), 1, GL_FALSE, &_matrixStack->currentModelMatrix[0][0]);
					gl::drawSolidTriangle(Vec2f(0.05f, 0.f), Vec2f(-0.05f, 0.f), Vec2f(0.f, cinderTrans.length()));

				gl::popMatrices();
				_matrixStack->popMatrix();
			}

			// Draw child joints
			for(NodeChild * child : children){
				dynamic_cast<Joint *>(child)->render(_matrixStack, _renderStack);
			}
		gl::popModelView();
		_matrixStack->popMatrix();
		
		//gl::disableWireframe();
	}else{
		throw "no shader attached";
	}
}

void Joint::update(Step * _step){
	NodeAnimatable::update(_step);

	for(NodeChild * child : children){
		dynamic_cast<NodeUpdatable *>(child)->update(_step);
	}
}

bool Joint::addChild(NodeChild * _child){
    if(dynamic_cast<Voxel *>(_child) != nullptr){
        // Remove the first instance of the child in the current list of voxels
        for (unsigned long int i = 0; i < voxels.size(); ++i){
		    if (_child == voxels.at(i)){
			    voxels.erase(voxels.begin() + i);
			    break;
		    }
	    }    
        // Add the child to the list of voxels and set it's parent to this
	    voxels.push_back(dynamic_cast<Voxel *>(_child));
	    _child->parent = this;
	    return true;
	}else{
        // Remove the first instance of the child in the current list of children
	    for (unsigned long int i = 0; i < children.size(); ++i){
		    if (_child == children.at(i)){
			    children.erase(children.begin() + i);
			    break;
		    }
	    }
        // Add the child to the list of children and set it's parent to this
	    children.push_back(_child);
	    _child->parent = this;
	    return true;
	}
}

void Joint::addChildAtIndex(NodeChild * _child, int _index){
    if(dynamic_cast<Voxel *>(_child) != nullptr){
        voxels.insert(voxels.begin() + _index, dynamic_cast<Voxel *>(_child));
	}else{
	    children.insert(children.begin() + _index, _child);
	}
}

unsigned long int Joint::removeChild(NodeChild * _child){
    if(dynamic_cast<Voxel *>(_child) != nullptr){
        for(unsigned long int i = 0; i < voxels.size(); ++i){
		    if(_child == voxels.at(i)){
			    voxels.erase(voxels.begin() + i);
			    return i;
		    }
	    }
	    return -1;
	}else{
        for(unsigned long int i = 0; i < children.size(); ++i){
		    if(_child == children.at(i)){
			    children.erase(children.begin() + i);
			    return i;
		    }
	    }
	    return -1;
	}
	
}