#pragma once

#include "OrthographicCamera.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "System.h"
#include "Transform.h"

OrthographicCamera::OrthographicCamera(float _left, float _right, float _bottom, float _top, float _near, float _far):
	Camera(),
	left(_left),
	right(_right),
	bottom(_bottom),
	top(_top),
	near(_near),
	far(_far)
{
	transform->translate(-5.f, 0.f, 0.f);
	transform->translate(0.f, 3.f, 0.f);

	Dimension screenDimensions = vox::getScreenDimensions();
}

OrthographicCamera::~OrthographicCamera(){
}

glm::mat4 OrthographicCamera::getViewMatrix(){
	return glm::lookAt(
		transform->translationVector,	// Camera is here
		transform->translationVector + forwardVectorRotated, // and looks here : at the same position, plus "direction"
		upVectorRotated				// Head is up (set to 0,-1,0 to look upside-down)
		);
}

glm::mat4 OrthographicCamera::getProjectionMatrix(){
	//
	return glm::ortho<float>(left,right,bottom,top,near,far);
}