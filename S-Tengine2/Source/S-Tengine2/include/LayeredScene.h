#pragma once

#include "Scene.h"
#include <UILayer.h>

class LayeredScene : public Scene{
public:
	explicit LayeredScene(Game * _game, unsigned long int _numLayers);

	unsigned long int numLayers;
	std::vector<Transform *> layers;
	std::vector<bool> depthEnabled;
	UILayer uiLayer;
	
	/** Calls update on the attached camera */
	virtual void update(Step * _step) override;
	/** Tells the RenderSystem to render the attached children to the vox::currentContext using the camera's view-projection matrix */
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	/** In addition to removing from child list, also searches each layer individually and removes as well */
	void removeChild(NodeChild * _child);
	
	/** Adds _child to uiLayer and the child list */
	void addUIChild(NodeUI * _child);
	/** Removes every instance of _child from the uiLayer and the child list */
	void removeUIChild(NodeUI * _child);

	/** Adds an entity to the specified layer */
	void addChild(NodeChild * _child, unsigned long int _layer, bool underNewTransform = true);
};