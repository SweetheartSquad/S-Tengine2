#pragma once 

#include "Scene.h"

class Scene2D : public Scene{
public:
	explicit Scene2D(Game * _game);
	virtual ~Scene2D();

	virtual void update(Step * _step) override;
	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override; 
};