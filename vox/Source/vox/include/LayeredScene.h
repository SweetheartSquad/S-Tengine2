#pragma once

#include "Scene.h"

class LayeredScene : public Scene{
public:
	explicit LayeredScene(Game * _game, unsigned long int _numLayers);

	unsigned long int numLayers;
	std::vector<std::vector<Entity *> *> layers;
	
	/** Calls update on the attached camera */
	virtual void update(Step * _step);
	/** Tells the RenderSystem to render the attached children to the vox::currentContext using the camera's view-projection matrix */
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	/** DOESN'T DO ANYTHING! */
	void addChild(Entity * _child) override;

	/** Adds an entity to the specified layer */
	void addChild(Entity * _child, unsigned long int _layer);
};