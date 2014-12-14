#pragma once

#include <vector>
#include <cinder\Area.h>

#include "node/NodeRenderable.h"
#include "node/NodeHierarchical.h"

class ToolButton;
class Step;

class ToolSet : public NodeRenderable, public NodeHierarchical{
public:
	ToolSet();
	~ToolSet();

	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;

	void addButton(ToolButton * _button);

	unsigned long int getHeight();
	unsigned long int getWidth();
};