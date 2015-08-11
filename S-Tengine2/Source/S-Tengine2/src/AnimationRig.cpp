#pragma once

#include <AnimationRig.h>
#include <shader\ComponentShaderBase.h>
#include <shader\ShaderComponentTexture.h>
#include <shader\ShaderComponentMVP.h>
#include <MeshFactory.h>
#include <MeshInterface.h>


void AnimationJoint::setAngle(float _angle){
	// keep angle within the range [-180, 180]
	_angle = fmod(_angle, (2.f * glm::pi<float>()));
	if(_angle > glm::pi<float>()){
		_angle -= (2.f * glm::pi<float>());
	}else if(_angle < -glm::pi<float>()){
		_angle += (2.f * glm::pi<float>());
	} 

	// keep angle within joint's specified limits (upper limit takes precedence)
	if(limitsEnabled){
		if(_angle > angleLimitUpper){
			_angle = angleLimitUpper;
		}else if(_angle < angleLimitLower){
			_angle = angleLimitLower;
		}
	}

	angle = _angle;
}

float AnimationJoint::getAngle(){
	return angle;
}


void AnimationJoint::setAngleLimits(float _lower, float _upper){
	limitsEnabled = true;
	angleLimitLower = _lower;
	angleLimitUpper = _upper;
}

void AnimationJoint::addJoint(AnimationJoint * _j){
	childTransform->addChild(_j);
	outJoints.push_back(_j);
	
	lineVis->mesh->pushVert(Vertex(0,0,0));
	lineVis->mesh->pushVert(Vertex(_j->pos.x,_j->pos.y,0));

	pointVis->mesh->pushVert(Vertex(_j->pos.x,_j->pos.y,0));
}

AnimationJoint::AnimationJoint(glm::vec2 _pos) :
	pos(_pos),
	angle(0),
	angleLimitUpper(0),
	angleLimitLower(0),
	limitsEnabled(false)
{
	ComponentShaderBase * shader = new ComponentShaderBase(true);
	shader->addComponent(new ShaderComponentMVP(shader));
	shader->addComponent(new ShaderComponentTexture(shader));
	shader->compileShader();
	shader->load();
	
	lineVis = new MeshEntity(new MeshInterface(GL_LINES, GL_STATIC_DRAW));
	pointVis = new MeshEntity(new MeshInterface(GL_POINTS, GL_STATIC_DRAW));
	lineVis->setShader(shader, true);
	pointVis->setShader(shader, true);
	
	childTransform->addChild(lineVis);
	childTransform->addChild(pointVis);

	pointVis->mesh->pushVert(Vertex(0,0,0));
	
	pointVis->setVisible(false);
	lineVis->setVisible(false);
}

void AnimationJoint::update(Step * _step){
	// update transform
	childTransform->translate(glm::vec3(pos, 0), false);
	childTransform->setOrientation(glm::quat(glm::vec3( 0, 0, getAngle() )));

	// update bone/joint display
	if(pointVis->isVisible()){
		for(unsigned long int i = 0; i < outJoints.size(); ++i){
			pointVis->mesh->vertices.at(i+1).x = outJoints.at(i)->pos.x;
			pointVis->mesh->vertices.at(i+1).y = outJoints.at(i)->pos.y;
		}
		pointVis->mesh->dirty = true;
	}
	if(lineVis->isVisible()){
		for(unsigned long int i = 0; i < outJoints.size(); ++i){
			lineVis->mesh->vertices.at(i*2+1).x = outJoints.at(i)->pos.x;
			lineVis->mesh->vertices.at(i*2+1).y = outJoints.at(i)->pos.y;
		}
		lineVis->mesh->dirty = true;
	}

	Entity::update(_step);
}

void AnimationJoint::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	float prevLineWidth, prevPointSize;
	glGetFloatv(GL_LINE_WIDTH, &prevLineWidth);
	glGetFloatv(GL_POINT_SIZE, &prevPointSize);

	glLineWidth(5);
	glPointSize(8);

	Entity::render(_matrixStack, _renderOptions);

	glLineWidth(prevLineWidth);
	glPointSize(prevPointSize);
}