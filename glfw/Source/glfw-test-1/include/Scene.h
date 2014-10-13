#pragma once

#include <iostream>
#include "Entity.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Camera.h"
#include "Light.h"

class Scene{
public:
	Scene(void);
	virtual ~Scene(void);

	/** Reference to mouse singleton */
	Mouse * mouse;
	/** Reference to keyboard singleton */
	Keyboard * keyboard;

	/** Reference to this scene's camera */
	Camera * camera;
	/** Reference to a list of references to entitites included in this scene */
	std::vector<Entity *> children;
	std::vector<Light *> lights;

	/** Calls update on the attached camera */
	virtual void update(void);
	/** Tells the RenderSystem to render the attached children to the vox::currentContext using the camera's view-projection matrix */
	virtual void render();
	/** Adds a reference to an entity to the attached list of children */
	void addChild(Entity * child);
	void toggleFullScreen();
};
