#pragma once

#include <vector>
#include <cinder\Area.h>

#include "NodeRenderable.h"

class ToolButton;
class Step;

class ToolSet : public NodeRenderable{
public:
	ToolSet(ci::Area _iconSize);
	~ToolSet();

	ci::Area iconSize;
	std::vector<ToolButton *> buttons;

	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;

	void addButton(ToolButton * _button);

};