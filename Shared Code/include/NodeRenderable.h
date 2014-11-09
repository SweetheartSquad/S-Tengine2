# pragma once

#include "Node.h"

class MatrixStack;
class RenderOptions;

class NodeRenderable abstract : public virtual Node{
public:
	virtual void render(MatrixStack * _matrixStack, RenderOptions * _renderStack) = 0;
};