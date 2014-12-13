#pragma once

#include <vector>

#include <cinder/Vector.h>

#include "Command.h"
#include "Transform.h"

class NodeTransformable;

class CMD_ScaleTransformable : public Command{
public:
	// Relative will scale BY _v, non-relative will scale TO _v
	CMD_ScaleTransformable(NodeTransformable * _node, ci::Vec3f _scale, bool _relative, CoordinateSpace _space);
	~CMD_ScaleTransformable(void);

	void execute();
	void unexecute();
private:
	NodeTransformable * node;
	ci::Vec3f scale;
	bool relative;
	CoordinateSpace space;

	glm::vec3 oldScale;
};