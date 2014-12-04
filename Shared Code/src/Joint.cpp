#pragma once

#include <cinder\app\App.h>
#include <cinder\Camera.h>

#include "Joint.h"
#include "Transform.h"

#include "MatrixStack.h"
#include "CinderRenderOptions.h"
#include "Step.h"

unsigned long int Joint::nextId = 0;

void Joint::init(){
	depth = 0;
	parent = nullptr;
	id = nextId;
	nextId += 1;
}

Joint::Joint() : 
	NodeAnimatable(new Transform()),
	NodeHierarchical(nullptr),
	NodeSelectable()
{
	init();
}

Joint::Joint(NodeHierarchical * _parent) : 
	//NodeTransformable(new Transform()),
	NodeAnimatable(new Transform()),
	NodeHierarchical(_parent),
	NodeSelectable()
{
	init();
	parent = _parent;
	while(_parent != nullptr){
		_parent = _parent->parent;
		depth += 1;
	}
}

void Joint::setPos(Vec3d _pos, bool _convertToRelative){
	glm::vec4 newPos(_pos.x, _pos.y, _pos.z, 1);
	if(_convertToRelative){
		NodeHierarchical * _parent = parent;
		std::vector<glm::mat4> modelMatrixStack;
		while(_parent != nullptr){
			modelMatrixStack.push_back(dynamic_cast<NodeTransformable *>(_parent)->transform->getModelMatrix());
			_parent = _parent->parent;
		}

		glm::mat4 modelMatrix(1);
		for(unsigned long int i = modelMatrixStack.size(); i > 0; --i){
			modelMatrix = modelMatrix * modelMatrixStack.at(i-1);
		}
		newPos = glm::inverse(modelMatrix) * newPos;
	}
	transform->translationVector = glm::vec3(newPos.x, newPos.y, newPos.z);
}

Vec3d Joint::getPos(bool _relative){
	glm::vec4 res(transform->translationVector.x, transform->translationVector.y, transform->translationVector.z, 1);
	if(!_relative){
		NodeHierarchical * _parent = parent;
		std::vector<glm::mat4> modelMatrixStack;
		while(_parent != nullptr){
			modelMatrixStack.push_back(dynamic_cast<NodeTransformable *>(_parent)->transform->getModelMatrix());
			_parent = _parent->parent;
		}
		
		glm::mat4 modelMatrix(1);
		for(unsigned long int i = modelMatrixStack.size(); i > 0; --i){
			modelMatrix = modelMatrix * modelMatrixStack.at(i-1);
		}
		res = modelMatrix * res;
	}
	return Vec3d(res.x, res.y, res.z);
}

Joint::~Joint(){
	delete transform;
}

void Joint::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	CinderRenderOptions * r = (CinderRenderOptions *)_renderStack;
	if(r->ciShader != nullptr){
		//gl::enableWireframe();
		r->ciShader->uniform("pickingColor", Color::hex(pickingColor));
		//colour
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
			//vox::translate(transform->getModelMatrix());

			//gl::pushMatrices(); 
			//gl::popMatrices();
	
			gl::rotate(Quatd(transform->orientation.w,
								transform->orientation.x,
								transform->orientation.y,
								transform->orientation.z));
			if(transform->scaleVector.x != transform->scaleVector.y || transform->scaleVector.x != transform->scaleVector.z || transform->scaleVector.z != transform->scaleVector.y){
				std::cout<< "test" << std::endl;

			}
			gl::scale(transform->scaleVector.x, transform->scaleVector.y, transform->scaleVector.z);
			//vox::rotate(transform->getOrientationMatrix());
			_matrixStack->applyMatrix(transform->getModelMatrix());

			glUniformMatrix4fv(r->ciShader->getUniformLocation("modelMatrix"), 1, GL_FALSE, &_matrixStack->currentModelMatrix[0][0]);
			gl::drawSphere(Vec3f(0.f, 0.f, 0.f), 0.05f);

			//draw voxels
			float resolution = ((CinderRenderOptions *)_renderStack)->voxelPreviewResolution;
			for(unsigned long int i = 0; i < voxels.size(); ++i){
				gl::pushModelView();
				_matrixStack->pushMatrix();
					gl::setMatrices(*r->ciCam);

					glm::vec4 voxelPos(voxels.at(i)->pos.x, voxels.at(i)->pos.y, voxels.at(i)->pos.z, 1);
					
					glm::vec4 pos = _matrixStack->getCurrentMatrix() * voxelPos;


					if(((CinderRenderOptions *)_renderStack)->voxelPreviewMode){
						// Snap to grid
						glm::vec4 posDif(
							fmod(pos.x, resolution),
							fmod(pos.y, resolution),
							fmod(pos.z, resolution),
							0);
						pos -= posDif;
					}
					
					for(unsigned long int x = 0; x < 4; ++x){
						for(unsigned long int y = 0; y < 4; ++y){
							_matrixStack->currentModelMatrix[x][y] = x == y ? 1.f : 0.f;
						}
					}

					Transform t;
					t.translate(pos.x, pos.y, pos.z);
					gl::translate(pos.x, pos.y, pos.z);
					_matrixStack->translate(t.getTranslationMatrix());
					
					glUniformMatrix4fv(r->ciShader->getUniformLocation("modelMatrix"), 1, GL_FALSE, &_matrixStack->currentModelMatrix[0][0]);
					r->ciShader->uniform("pickingColor", Color::hex(voxels.at(i)->pickingColor));
					
					if(((CinderRenderOptions *)_renderStack)->voxelPreviewMode){
						gl::drawCube(Vec3f(0.f, 0.f, 0.f), Vec3f(resolution*2,resolution*2,resolution*2));
					}else{
						gl::drawSphere(Vec3f(0.f, 0.f, 0.f), ((CinderRenderOptions *)_renderStack)->voxelSphereRadius);
					}
				gl::popModelView();
				_matrixStack->popMatrix();
			}
		
			//draw bones
			gl::color(color);
			r->ciShader->uniform("pickingColor", Color::hex(pickingColor));
			for(NodeHierarchical * child : children){
				Vec3f cinderTrans(
					dynamic_cast<NodeTransformable *>(child)->transform->translationVector.x,
					dynamic_cast<NodeTransformable *>(child)->transform->translationVector.y,
					dynamic_cast<NodeTransformable *>(child)->transform->translationVector.z
				);
				Quatf boneDir(Vec3f(0.0, 1.0, 0.0), cinderTrans);
				
				gl::pushMatrices();
				_matrixStack->pushMatrix();
					gl::rotate(boneDir);
					Transform temp;
					temp.orientation.x = boneDir.v.x;
					temp.orientation.y = boneDir.v.y;
					temp.orientation.z = boneDir.v.z;
					temp.orientation.w = boneDir.w;
					_matrixStack->rotate(temp.getOrientationMatrix());
			
					glUniformMatrix4fv(r->ciShader->getUniformLocation("modelMatrix"), 1, GL_FALSE, &_matrixStack->currentModelMatrix[0][0]);
					gl::drawSolidTriangle(Vec2f(0.05f, 0.f), Vec2f(-0.05f, 0.f), Vec2f(0.f, cinderTrans.length()));

					gl::rotate(Vec3f(0.f, 90.f, 0.f));
					Transform temp2;
					temp2.rotate(90, 0, 1, 0, OBJECT);
					_matrixStack->rotate(temp2.getOrientationMatrix());
			
					glUniformMatrix4fv(r->ciShader->getUniformLocation("modelMatrix"), 1, GL_FALSE, &_matrixStack->currentModelMatrix[0][0]);
					gl::drawSolidTriangle(Vec2f(0.05f, 0.f), Vec2f(-0.05f, 0.f), Vec2f(0.f, cinderTrans.length()));

				gl::popMatrices();
				_matrixStack->popMatrix();
			}

			// Draw child joints
			for(NodeHierarchical * child : children){
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

	for(NodeHierarchical * child : children){
		dynamic_cast<Joint *>(child)->update(_step);
	}
}