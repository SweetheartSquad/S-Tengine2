#pragma once

#include <NodeUI.h>

class Layout abstract : public NodeUI {
public:

	Layout(BulletWorld * _world, Scene * _scene);

	virtual Transform * addChild(NodeUI * _uiElement) = 0;
};