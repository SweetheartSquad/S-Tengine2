#pragma once

#include <vector>
#include <cinder\Rect.h>
#include <cinder\Vector.h>

#include "NodeRenderable.h"

class ToolSet;
class Step;

class ToolBar : public NodeRenderable{
public:
	std::vector<ToolSet *> toolsets;
	
	ci::Vec2i pos;

	ToolBar(ci::Vec2i _pos);
	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;	
};