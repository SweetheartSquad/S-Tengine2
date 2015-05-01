#pragma once

#include <Camera.h>
#include <Mouse.h>
#include <Keyboard.h>
#include <System.h>
#include <Transform.h>

Camera::Camera() :
	NodeTransformable(new Transform()),
	forwardVectorLocal(1.f, 0.f, 0.f),
	forwardVectorRotated(1.f, 0.f, 0.f),
	upVectorLocal(0.f, 1.f, 0.f),
	upVectorRotated(0.f, 1.f, 0.f),
	rightVectorLocal(0.f, 0.f, 1.f),
	rightVectorRotated(0.f, 0.f, 1.f),
	fieldOfView(60.0f),
	pitch(0.0f),
	yaw(0.f),
	nearClip(1.f),
	farClip(100.f)
{
	transform->translate(-5.f, 0.f, 0.f);
	transform->translate(0.f, 3.f, 0.f);
}

Camera::~Camera(){
}

glm::vec3 Camera::worldToScreen(glm::vec3 _coords, glm::uvec2 _screen){
	glm::vec4 newPos(_coords, 1);
	newPos = getProjectionMatrix() * getViewMatrix() * newPos;
	
	glm::uvec2 screenDimensions = vox::getScreenDimensions();
	return glm::vec3(
		_screen.x * (1 - newPos.x/newPos.w)*0.5f,
		_screen.y * (newPos.y/newPos.w + 1)*0.5f,
		newPos.z
	);
}

glm::quat Camera::calcOrientation(){
	glm::quat res(1.f, 0.f, 0.f, 0.f);
	res = glm::rotate(res, yaw, upVectorLocal);
	res = glm::rotate(res, pitch, rightVectorLocal);
	return res;
}