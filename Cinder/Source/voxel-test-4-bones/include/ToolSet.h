#pragma once

#include <vector>
#include <cinder\Rect.h>

#include "NodeRenderable.h"

class ToolButton;
class Step;

class ToolSet : public NodeRenderable{
public:
	ToolSet(ci::Rectf _iconSize);

	ci::Rectf iconSize;
	std::vector<ToolButton *> buttons;

	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;

	void addButton(ToolButton * _button);

};