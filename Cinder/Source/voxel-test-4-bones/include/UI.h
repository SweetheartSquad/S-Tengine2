#pragma once

#include <vector>
#include "Easing.h"

class Node;
class Transform;
class NodeTransformable;

class UI{
public:
	static std::vector<Node *> selectedNodes;

	static struct frame_type {
		Transform * transform;
	} properties;

	static float time;
	static Easing::Type interpolation;

	static void setPropertyWindow();
};