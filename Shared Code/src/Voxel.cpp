#pragma once

#include "Voxel.h"
#include "Joint.h"
#include "Transform.h"
#include "CinderRenderOptions.h"
#include "MatrixStack.h"

void Voxel::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	CinderRenderOptions * cro = ((CinderRenderOptions *)_renderStack);
	gl::pushModelView();
	_matrixStack->pushMatrix();
		gl::setMatrices(*cro->ciCam);

		glm::vec4 voxelPos(getPos().x, getPos().y, getPos().z, 1);
					
		glm::vec4 pos = _matrixStack->getCurrentMatrix() * voxelPos;
		
		float resolution = cro->voxelPreviewResolution;
		if(cro->voxelPreviewMode){
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
					
		glUniformMatrix4fv(cro->ciShader->getUniformLocation("modelMatrix"), 1, GL_FALSE, &_matrixStack->currentModelMatrix[0][0]);
		cro->ciShader->uniform("pickingColor", Color::hex(pickingColor));
					
		if(cro->voxelPreviewMode){
			gl::pushMatrices();
			_matrixStack->pushMatrix();
				gl::scale(resolution*2.f, resolution*2.f, resolution*2.f);
				_matrixStack->scale(glm::scale(glm::vec3(resolution*2.f, resolution*2.f, resolution*2.f)));
				glUniformMatrix4fv(cro->ciShader->getUniformLocation("modelMatrix"), 1, GL_FALSE, &_matrixStack->currentModelMatrix[0][0]);
				gl::draw(*cro->cube);
			gl::popMatrices();
			_matrixStack->popMatrix();
		}else{
			gl::pushMatrices();
			_matrixStack->pushMatrix();
				gl::scale(cro->voxelSphereRadius,cro->voxelSphereRadius,cro->voxelSphereRadius);
				_matrixStack->scale(glm::scale(glm::vec3(cro->voxelSphereRadius, cro->voxelSphereRadius, cro->voxelSphereRadius)));
				glUniformMatrix4fv(cro->ciShader->getUniformLocation("modelMatrix"), 1, GL_FALSE, &_matrixStack->currentModelMatrix[0][0]);
				gl::draw(*cro->sphere);
			gl::popMatrices();
			_matrixStack->popMatrix();
		}
	gl::popModelView();
	_matrixStack->popMatrix();
}

Voxel::Voxel(ci::Vec3f _pos, Joint * _parent, bool _convertToRelative) :
	NodeRenderable(),
	NodeSelectable(),
	NodeTransformable(new Transform()),
	NodeChild(_parent)
{
    setPos(glm::vec3(_pos.x, _pos.y, _pos.z), _convertToRelative);
}

Voxel::~Voxel(){
	delete transform;
}