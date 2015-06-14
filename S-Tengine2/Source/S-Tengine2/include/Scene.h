#pragma once

#include <vector>

#include <Entity.h>

class Camera;

namespace vox{
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

class Scene : public Entity{
public:
	explicit Scene(Game * _game);
	virtual ~Scene(void);
	Game * game;
	/** Reference to mouse singleton */
	Mouse * mouse;
	/** Reference to keyboard singleton */
	Keyboard * keyboard;
	/** Reference to this scene's active camera */
	Camera * activeCamera;
	std::vector<Camera *> cameras;

	StandardFrameBuffer	* depthBuffer;
	StandardFrameBuffer * shadowBuffer;
	DepthMapShader		* depthShader;
	BlurShader			* shadowShader;
	RenderSurface       * shadowSurface;

	/** The lights for this scene **/
	std::vector<Light *> lights;
	/** Calls update on the attached camera */
	virtual void update(Step * _step) override;
	/** Tells the RenderSystem to render the attached children to the vox::currentContext using the camera's view-projection matrix */
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	
	virtual void renderShadows(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions);

	void clear();

	float clearColor[4];

	virtual void load() override;
	virtual void unload() override;
};
