#pragma once

#include <vector>

#include <cinder/Vector.h>
#include <glm\glm.hpp>

#include "Command.h"
#include "Transform.h"

class NodeTransformable;

class CMD_TranslateTransformable : public Command{
public:
	// Relative move will move _node BY _v, non-relative move will move _node TO _v
	CMD_TranslateTransformable(NodeTransformable * _node, ci::Vec3d _v, bool _relative, CoordinateSpace _space);
	~CMD_TranslateTransformable(void);

	bool execute();
	bool unexecute();
private:
	NodeTransformable * node;
	ci::Vec3d v;
	glm::vec3 oldPos;
	bool relative;
	CoordinateSpace space;
};

