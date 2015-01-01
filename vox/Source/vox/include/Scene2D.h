#pragma once 

#include "Scene.h"

class Scene2D : public Scene{
public:
	explicit Scene2D(Game * _game);
	virtual ~Scene2D();

	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override; 
};