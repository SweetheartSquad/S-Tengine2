#include "MousePerspectiveCamera.h"
#include "Mouse.h"
#include "Transform.h"

MousePerspectiveCamera::MousePerspectiveCamera() :
	controller(new CameraController(this))
{
	childTransform->addChild(controller);
}