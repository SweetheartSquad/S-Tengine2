#pragma once

#include <vector>

#include <Entity.h>

class Camera;

namespace sweet{
	class MatrixStack;
}

class RenderOptions;
class Light;
class DepthMapShader;
class ComponentShaderBase;
class RenderSurface;
class Keyboard;
class StandardFrameBuffer;
class DepthFrameBuffer;
class Mouse;
class Game;
class Entity;
class BlurShader;
class NormalsShaderComponent;

class Scene : public Entity{
public:

	//StandardFrameBuffer	* depthBuffer;
	//StandardFrameBuffer	* normalBuffer;
	//StandardFrameBuffer * shadowBuffer;
	//ComponentShaderBase	* depthShader;
	//BlurShader			* shadowShader;
	//ComponentShaderBase * normalsShader;
	//RenderSurface       * shadowSurface;
	
	// Reference to the game instance 
	Game * game;
	
	/** Reference to mouse singleton */
	Mouse * const mouse;
	
	/** Reference to keyboard singleton */
	Keyboard * const keyboard;
	
	/** Reference to this scene's active camera */
	Camera * activeCamera;
	
	// List of cameras for the scene
	std::vector<Camera *> cameras;
	// removes _camera from the list of cameras
	void removeCamera(Camera * _camera);

	/** The lights for this scene **/
	std::vector<Light *> lights;
	// removes _light from the list of lights
	void removeLight(Light * _light);

	explicit Scene(Game * _game);
	
	virtual ~Scene();
	
	/** 
	* Calls update on the attached camera
	*/
	virtual void update(Step * _step) override;
	
	/** 
	* Tells the RenderSystem to render the attached children to 
	* the sweet::currentContext using the camera's view-projection matrix 
	*/
	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	
	//virtual void renderShadows(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions);

	/**
	* Renders the children of the scene to its depth buffer
	* Sets the override shader to an instance of depthMapShader in order 
	* to render the depth properly
	*
	* The currently bound frame buffer will be set back to 0 at the end of this function
	*/
	//virtual void renderDepth(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions);

	/**
	* Convenience fuction for rendering the current depth buffer to the provided surface
	*
	* NOTE - The scene is not rendered to the depth buffer as part of this
	*         function. This can be done using renderDepth
	*
	* @param _renderSurface  The surface that the depth buffer will be rendered to
	*/
	//void renderDepthBufferToSurface(RenderSurface * _renderSurface);
	
	/**
	* Renders the children of the scene to its normals buffer
	* Sets the override shader to an instance of normalsShader in order 
	* to render the normals properly
	*
	* The currently bound frame buffer will be set back to 0 at the end of this function
	*/
	//virtual void renderNormals(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions);

	/**
	* Convenience fuction for rendering the current normals buffer to the provided surface
	*
	* NOTE - The scene is not rendered to the depth buffer as part of this
	*         function. This can be done using renderDepth
	*
	* @param _renderSurface  The surface that the normal buffer will be rendered to
	*/
	//void renderNormalBufferToSurface(RenderSurface * _renderSurface);


	virtual void load() override;

	virtual void unload() override;


protected:
	// sets activeCamera to the next camera in the camera stack
	// if activeCamera is the last camera, sets it to the first camera
	// returns the new activeCamera
	// (this is intended for debugging purposes)
	Camera * cycleCamera();
};
