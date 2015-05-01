# pragma once

#include "node/Node.h"

namespace vox{
class MatrixStack;
};
class RenderOptions;

class NodeRenderable abstract : public virtual Node{
public:
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) = 0;
};