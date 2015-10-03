#pragma once

#include <node/NodeUpdatable.h>

class MousePerspectiveCamera;
class Keyboard;

class MouseCameraController : public NodeUpdatable {
public:	

	MousePerspectiveCamera * target;

	unsigned int forwardKey;
	unsigned int backwardsKey;
	unsigned int leftKey;
	unsigned int rightKey;

	float speed;

	MouseCameraController(MousePerspectiveCamera * _target = nullptr);

	virtual void update(Step * _step) override;

	void attach(MousePerspectiveCamera * _target);
	void setKeys(unsigned int _forwardKey, unsigned int _backwardsKey, unsigned int _leftKey, unsigned int _rightKey);

	void setSpeed(float _speed);

private:
	Keyboard * keyboard;
};