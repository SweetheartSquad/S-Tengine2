#pragma once

#include <iostream>
#include "Entity.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Camera.h"
#include "Light.h"
#include "FrameBufferInterface.h"

//Forward declaration since Game has many scenes and Sene has a reference back to game
class Game;

class Scene{
public:
	explicit Scene(Game * _game);
	virtual ~Scene(void);
	Game * game;
	/** Reference to mouse singleton */
	Mouse * mouse;
	/** Reference to keyboard singleton */
	Keyboard * keyboard;
	/** Reference to this scene's camera */
	Camera * camera;
	/** Reference to a list of references to entitites included in this scene */
	std::vector<Entity *> children;
	/** The lights for this scene **/
	std::vector<Light *> lights;
	/** Calls update on the attached camera */
	virtual void update(void);
	/** Tells the RenderSystem to render the attached children to the vox::currentContext using the camera's view-projection matrix */
	virtual void render();
	/** Adds a reference to an entity to the attached list of children */
	void addChild(Entity * child);
	/** Toggles fullscreen on and off */
	void toggleFullScreen();

	FrameBufferInterface frameBuffer;
};
