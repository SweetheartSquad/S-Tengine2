#pragma once

#include <vector>

#include <cinder\Vector.h>

class Node;
class UI{
public:
	static std::vector<Node *> selectedNodes;
	static ci::Vec3d handlePos;

	static void updateHandlePos();
};