#pragma once

#include "PerspectiveCamera.h"
#include <CameraController.h>

// Just a standard perspective camera which is constructed with a CameraController as a child node
class MousePerspectiveCamera : public PerspectiveCamera {
public:
	CameraController * const controller;

	MousePerspectiveCamera();
};