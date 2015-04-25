#pragma once

#include "OrthographicCamera.h"
#include "node/NodeAnimatable.h"

class ControllableOrthographicCamera : public OrthographicCamera
{
public:

	NodeTransformable * target;
	float deadZone;

	ControllableOrthographicCamera(float left, float right, float bottom, float top, float near, float far);
	~ControllableOrthographicCamera();

	void update(Step* _step) override;
	void follow(NodeTransformable * _nodeTransformable);
};