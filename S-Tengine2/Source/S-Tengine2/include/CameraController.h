#pragma once

#include <node/NodeChild.h>
#include <node/NodeUpdatable.h>

class Camera;
class Keyboard;
class Mouse;

// A controller class which maps mouse movement to camera rotation, and four keyboard keys to camera movement
// Movement and rotation are both enabled by default, but it is possible to disable one or both of them at runtime
class CameraController : public virtual NodeUpdatable, public virtual NodeChild {
public:	
	// whether or not the movement keys will actually affect the target camera's position
	bool movementEnabled;
	// whether or not mouse movement will actually affect the target camera's pitch/yaw
	bool rotationEnabled;

	// the camera which will be affected by this controller
	Camera * target;

	// GLFW key codes for movement
	unsigned int forwardKey;
	unsigned int backwardsKey;
	unsigned int leftKey;
	unsigned int rightKey;

	// the camera moves based on its direction vectors * movementSpeed
	// default is 1.f
	float movementSpeed;
	// the camera rotates based on the difference in mouse position * rotationSpeed
	// default is 0.05f
	float rotationSpeed;

	CameraController(Camera * _target = nullptr);

	virtual void update(Step * _step) override;

	// attaches the controller to the _target camera
	// NOTE: controller may only have one target, so attaching to nullptr can be used to detach from currents
	void attach(Camera * _target);
	void setKeys(unsigned int _forwardKey, unsigned int _backwardsKey, unsigned int _leftKey, unsigned int _rightKey);
	
	// sets lastMouseX and lastMouseY to the current mouse position
	// this is called once per update automatically in order to track the difference between frames
	// this can be called manually in order to "reset" the controller and prevent movement for the current update
	void alignMouse();
private:
	// reference to the keyboard singleton
	const Keyboard * const keyboard;
	// reference to the mouse singleton
	const Mouse * const mouse;
	/** The X position of the mouse from the last update loop*/
	double lastMouseX;
	/** The Y position of the mouse from the last update loop*/
	double lastMouseY;
};