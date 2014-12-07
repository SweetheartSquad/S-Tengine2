#pragma once

#include <vector>
#include <cinder\Rect.h>
#include <cinder\Vector.h>

#include "NodeRenderable.h"
#include "NodeParent.h"

class ToolSet;
class Step;

class ToolBar : public NodeRenderable, public NodeParent{
public:
	ci::Vec2i pos;
	
	void addToolset(ToolSet * _toolset);

	ToolBar(ci::Vec2i _pos);
	~ToolBar();
	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;	
};