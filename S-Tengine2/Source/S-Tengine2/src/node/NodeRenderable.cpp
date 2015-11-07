#pragma once

#include "node/NodeRenderable.h"

NodeRenderable::NodeRenderable() :
	visible(true)
{
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