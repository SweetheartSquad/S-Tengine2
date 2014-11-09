# pragma once

#include "Node.h"
#include <MatrixStack.h>
#include "RenderOptions.h"

class NodeRenderable : public virtual Node{
public:

	NodeRenderable();
	virtual ~NodeRenderable();

	virtual void render(MatrixStack * _matrixStack, RenderOptions * _renderStack) = 0;
};