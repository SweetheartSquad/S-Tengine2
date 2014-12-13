#pragma once

#include <iostream>
#include "Entity.h"

class Light;
class DepthMapShader;
class BaseComponentShader;
class RenderSurface;
class PerspectiveCamera;
class Keyboard;
class StandardFrameBuffer;
class DepthFrameBuffer;
class Mouse;
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
	PerspectiveCamera * camera;
	/** Reference to a list of references to entitites included in this scene */
	RenderOptions       * renderOptions;
	StandardFrameBuffer	* depthBuffer;
	StandardFrameBuffer * shadowBuffer;
	DepthMapShader		* depthShader;
	RenderSurface       * shadowSurface;
	std::vector<Entity *> children;
	/** The lights for this scene **/
	std::vector<Light *> lights;
	/**The default matrix stack for the scene*/
	vox::MatrixStack * matrixStack;
	/** Calls update on the attached camera */
	virtual void update(void);
	/** Tells the RenderSystem to render the attached children to the vox::currentContext using the camera's view-projection matrix */
	virtual void render();
	/** Adds a reference to an entity to the attached list of children */
	void addChild(Entity * child);
	/** Toggles fullscreen on and off */
	void toggleFullScreen();
	/**Called when the openGL context is changed. eg. Entering full screen*/
	virtual void onContextChange();

	virtual void renderShadows();

	virtual void setViewport(float _x, float _y, float _width, float _height);
	float viewPortX, viewPortY, viewPortWidth, viewPortHeight;
};
