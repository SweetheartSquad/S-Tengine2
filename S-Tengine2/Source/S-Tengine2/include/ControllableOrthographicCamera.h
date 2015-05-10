#pragma once

#include "OrthographicCamera.h"

class ControllableOrthographicCamera : public OrthographicCamera{
public:

	Transform * target;
	float deadZone;

	ControllableOrthographicCamera(float left, float right, float bottom, float top, float near, float far);
	~ControllableOrthographicCamera();

	void update(Step* _step) override;
	void follow(Transform * _target);
};