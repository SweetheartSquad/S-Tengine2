#pragma once

#include "OrthographicCamera.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Transform.h"

OrthographicCamera::OrthographicCamera(float _left, float _right, float _bottom, float _top, float _near, float _far) :
	left(_left),
	top(_top),
	bottom(_bottom),
	right(_right)
{
	yaw = -90;

	nearClip = _near;
	farClip = _far;

	// update immediately in order to set the orientations and stuff
	update(nullptr);
}

glm::mat4 OrthographicCamera::getProjectionMatrix() const{
	return glm::ortho<float>(-left, -right, bottom, top, nearClip, farClip);
}


void OrthographicCamera::resize(float _left, float _right, float _bottom, float _top){
	left = _left;
	right = _right;
	bottom = _bottom;
	top = _top;
}

float OrthographicCamera::getWidth() const{
	return right - left;
}

float OrthographicCamera::getHeight() const{
	return top - bottom;
}