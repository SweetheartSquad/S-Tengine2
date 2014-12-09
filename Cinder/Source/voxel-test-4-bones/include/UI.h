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
	static int interpolationValue;
	static std::vector<std::string> interpolationNames;

	static ci::Vec3d handlePos;
	static ci::Vec3d displayHandlePos;

	static void updateHandlePos(bool _displayOnly);

	static std::vector<std::string> getInterpolationNames();
};