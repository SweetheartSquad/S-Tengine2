#pragma once

#include <iostream>
#include "Entity.h"
#include "RenderSystem.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Camera.h"

class Scene{
public:
	Scene(void);
	virtual ~Scene(void);
	
	Mouse * mouse;			// Reference to mouse singleton
	Keyboard * keyboard;	// Reference to keyboard singleton

	Camera * camera;					// Reference to this scene's camera
	std::vector<Entity *> * children;	// Reference to a list of references to entitites included in this scene

	// Calls update on the attached camera
	virtual void update(void);
	// Tells the RenderSystem to render the attached children to the vox::currentContext using the camera's view-projection matrix
	virtual void draw();
	// Adds a reference to an entity to the attached list of children
	void addChild(Entity * child);
	void toggleFullScreen();
};
