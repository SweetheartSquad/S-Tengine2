# pragma once

#include "Node.h"
#include <MatrixStack.h>
#include <RenderStack.h>

class NodeRenderable : public virtual Node{
public:

	NodeRenderable();
	virtual ~NodeRenderable();

	virtual void render(MatrixStack * _matrixStack, RenderStack * _renderStack) = 0;
};