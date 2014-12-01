#pragma once

#include <vector>
#include <cinder\Rect.h>

#include "NodeRenderable.h"
#include "NodeUpdatable.h"

class ToolButton;
class Step;

class ToolSet : public NodeRenderable, public NodeUpdatable{
public:
	ToolSet(ci::Rectf _iconSize);

	ci::Rectf iconSize;
	std::vector<ToolButton *> buttons;

	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;
	void update(Step * _step) override;

	void addButton(ToolButton * _button);

};