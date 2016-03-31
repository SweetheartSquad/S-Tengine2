#pragma once

#include "node/NodeRenderable.h"

NodeRenderable::NodeRenderable() :
	visible(true)
{
	nodeType |= NodeType::kNODE_RENDERABLE;
}

void NodeRenderable::setVisible(bool _visible){
	visible = _visible;
}
bool NodeRenderable::isVisible(){
	return visible;
}

/*void NodeRenderable::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
}
*/