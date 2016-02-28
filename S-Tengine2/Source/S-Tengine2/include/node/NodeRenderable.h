# pragma once

#include "node/Node.h"

namespace sweet{
	class MatrixStack;
};
class RenderOptions;

class NodeRenderable abstract : public virtual Node{
protected:
	// if false, calls to render will be ignored
	bool visible;
public:
	NodeRenderable();
	virtual void setVisible(bool _visible);
	bool isVisible();
	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions) = 0;
};