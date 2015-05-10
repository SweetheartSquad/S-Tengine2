#pragma once

#include "OrthographicCamera.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "System.h"
#include "Transform.h"

OrthographicCamera::OrthographicCamera(float _left, float _right, float _bottom, float _top, float _near, float _far) :
	left(_left),
	top(_top),
	bottom(_bottom),
	right(_right)
{
	forwardVectorRotated = glm::vec3(0.f, 0.f, 1.f);

	nearClip = _near;
	farClip = _far;
}

OrthographicCamera::~OrthographicCamera(){
}

glm::mat4 OrthographicCamera::getViewMatrix(){
	return glm::lookAt(
		parents.at(0)->getTranslationVector(),							// Camera is here
		parents.at(0)->getTranslationVector() + forwardVectorRotated,	// and looks here : at the same position, plus "direction"
		upVectorRotated												// Head is up (set to 0,-1,0 to look upside-down)
	);
}

glm::mat4 OrthographicCamera::getProjectionMatrix(){
	return glm::ortho<float>(left, right, bottom, top, nearClip, farClip);
}