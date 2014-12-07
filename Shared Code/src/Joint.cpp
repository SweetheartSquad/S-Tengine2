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
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{
	init();
}

Joint::Joint(NodeParent * _parent) : 
	NodeTransformable(new Transform()),
	NodeChild(_parent)
{
	init();
	parent = _parent;
	while(_parent != nullptr){
		NodeHierarchical * t = dynamic_cast<NodeHierarchical *>(_parent);
		if (t != NULL){
			_parent = t->parent;
			depth += 1;
		}else{
			break;
		}
	}
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
			gl::rotate(Quatd(transform->orientation.w,
								transform->orientation.x,
								transform->orientation.y,
								transform->orientation.z));
			gl::scale(transform->scaleVector.x, transform->scaleVector.y, transform->scaleVector.z);
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

					gl::translate(pos.x, pos.y, pos.z);
					_matrixStack->translate(glm::translate(glm::vec3(pos.x, pos.y, pos.z)));
					
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
					glm::quat tempOrientation;
					tempOrientation.x = boneDir.v.x;
					tempOrientation.y = boneDir.v.y;
					tempOrientation.z = boneDir.v.z;
					tempOrientation.w = boneDir.w;
					_matrixStack->rotate(glm::toMat4(tempOrientation));
			
					glUniformMatrix4fv(r->ciShader->getUniformLocation("modelMatrix"), 1, GL_FALSE, &_matrixStack->currentModelMatrix[0][0]);
					gl::drawSolidTriangle(Vec2f(0.05f, 0.f), Vec2f(-0.05f, 0.f), Vec2f(0.f, cinderTrans.length()));

					gl::rotate(Vec3f(0.f, 90.f, 0.f));

					glm::quat tempOrientation2(90.f, 0.f, 1.f, 0.f);
					_matrixStack->rotate(glm::toMat4(tempOrientation2));
			
					glUniformMatrix4fv(r->ciShader->getUniformLocation("modelMatrix"), 1, GL_FALSE, &_matrixStack->currentModelMatrix[0][0]);
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