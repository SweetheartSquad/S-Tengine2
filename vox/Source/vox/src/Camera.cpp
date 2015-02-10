#pragma once

#include "Camera.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "System.h"
#include "Transform.h"

Camera::Camera():
	NodeTransformable(new Transform()),
	forwardVectorLocal(1.f, 0.f, 0.f),
	forwardVectorRotated(1.f, 0.f, 0.f),
	upVectorLocal(0.f, 1.f, 0.f),
	upVectorRotated(0.f, 1.f, 0.f),
	rightVectorLocal(0.f, 0.f, 1.f),
	rightVectorRotated(0.f, 0.f, 1.f),
	fieldOfView(60.0f),
	pitch(0.0f),
	yaw(-30.f),
	NodeAnimatable(),
	nearClip(1.f),
	farClip(100.f)
{
	transform->translate(-5.f, 0.f, 0.f);
	transform->translate(0.f, 3.f, 0.f);

	Dimension screenDimensions = vox::getScreenDimensions();
}

Camera::~Camera(){
}