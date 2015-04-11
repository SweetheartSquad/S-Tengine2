#pragma once

#include <Sprite.h>

class ScoreIndicator : public Sprite{
public:
	ScoreIndicator(unsigned long int _id);
	void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;
	unsigned long int id;
};