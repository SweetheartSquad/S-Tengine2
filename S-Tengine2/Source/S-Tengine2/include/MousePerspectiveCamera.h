#pragma once

#include "PerspectiveCamera.h"

class Mouse;

class MousePerspectiveCamera : public PerspectiveCamera {
public:

	/** Reference to the mouse singleton */
	Mouse * mouse;

	/** Movement speed multiplier */
	float speed;
	/** Mouse speed multiplier */
	float mouseSpeed;

	MousePerspectiveCamera();
	~MousePerspectiveCamera();
	
	virtual void update(Step * _step) override;

private :
	/** The X position of the mouse from the last update loop*/
	double lastMouseX;
	/** The Y position of the mouse from the last update loop*/
	double lastMouseY;

};