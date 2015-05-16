# pragma once

#include "node/Node.h"

namespace vox{
	class MatrixStack;
};
class RenderOptions;

class NodeRenderable abstract : public virtual Node{
protected:
	// if false, calls to render will be ignored
	bool visible;
public:
	NodeRenderable();
	void setVisible(bool _visible);
	bool isVisible();
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) = 0;
};