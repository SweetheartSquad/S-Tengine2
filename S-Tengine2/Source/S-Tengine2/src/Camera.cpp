#pragma once

#include <Camera.h>
#include <Mouse.h>
#include <Keyboard.h>
#include <Transform.h>

Camera::Camera() :
	forwardVectorLocal(1.f, 0.f, 0.f),
	forwardVectorRotated(1.f, 0.f, 0.f),
	upVectorLocal(0.f, 1.f, 0.f),
	upVectorRotated(0.f, 1.f, 0.f),
	rightVectorLocal(0.f, 0.f, 1.f),
	rightVectorRotated(0.f, 0.f, 1.f),
	fieldOfView(60.0f),
	pitch(0.0f),
	yaw(0.f),
	roll(0.f),
	nearClip(1.f),
	farClip(100.f),
	lookFromSpot(0),
	lookAtSpot(0),
	lookAtOffset(0)
{
}

Camera::~Camera(){
}

void Camera::update(Step * _step){
	// update the childTransform based on the pitch, roll, and yaw
	setOrientation(calcOrientation());
	// update the rotation and position vectors based on the childTransform
	rotateVectors(childTransform->getOrientationQuat());
	lookFromSpot = childTransform->getWorldPos();
	lookAtSpot = lookFromSpot+forwardVectorRotated;

	Entity::update(_step);
}

void Camera::setOrientation(glm::quat _orientation){
	childTransform->setOrientation(_orientation);
}

glm::vec3 Camera::worldToScreen(glm::vec3 _coords, glm::uvec2 _screen) const{
	glm::vec4 newPos(_coords, 1);
	newPos = getProjectionMatrix() * getViewMatrix() * newPos;
	
	return glm::vec3(
		_screen.x * (newPos.x/newPos.w + 1)*0.5f,
		_screen.y * (newPos.y/newPos.w + 1)*0.5f,
		newPos.z
	);
}

glm::vec3 Camera::screenToWorld(glm::vec3 _screenCoords) const{
	_screenCoords = glm::min(glm::vec3(1), _screenCoords);
	_screenCoords = glm::max(glm::vec3(0), _screenCoords);
	glm::vec4 t = glm::inverse(getProjectionMatrix() * getViewMatrix()) * glm::vec4((_screenCoords - glm::vec3(0.5f))*2.f, 1);
    return glm::vec3(t)/t.w;
}

glm::quat Camera::calcOrientation() const{
	glm::quat res(1.f, 0.f, 0.f, 0.f);
	res = glm::rotate(res, yaw, upVectorLocal);
	res = glm::rotate(res, pitch, rightVectorLocal);
	res = glm::rotate(res, roll, forwardVectorLocal);
	return res;
}

void Camera::rotateVectors(glm::quat _orientation){
	forwardVectorRotated   = _orientation * forwardVectorLocal;
	rightVectorRotated	   = _orientation * rightVectorLocal;
	upVectorRotated		   = _orientation * upVectorLocal;
}

glm::mat4 Camera::getViewMatrix() const{
	return glm::lookAt(
		lookFromSpot,	// Camera is here
		lookAtSpot + lookAtOffset,			// and looks here : at the same position, plus "direction"
		upVectorRotated						// Head is up (set to 0,-1,0 to look upside-down)
	);
}