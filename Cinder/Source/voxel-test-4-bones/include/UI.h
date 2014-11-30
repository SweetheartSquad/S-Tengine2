#pragma once

#include <vector>
#include "Easing.h"

#include <cinder\Vector.h>

class Node;

class UI{
public:
	static std::vector<Node *> selectedNodes;

	static float time;
	static float stepScale;
	static Easing::Type interpolation;
	static ci::Vec3d handlePos;
	static ci::Vec3d displayHandlePos;

	static void updateHandlePos(bool _displayOnly);
};