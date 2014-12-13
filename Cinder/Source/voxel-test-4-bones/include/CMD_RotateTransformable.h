#pragma once

#include <vector>

#include <glm/gtx/quaternion.hpp>

#include "Command.h"
#include "Transform.h"

class NodeTransformable;

class CMD_RotateTransformable : public Command{
public:
	// Relative will rotate BY _rotation, non-relative will rotate TO _rotation
	CMD_RotateTransformable(NodeTransformable * _node, glm::quat _rotation, bool _relative, CoordinateSpace _space);
	~CMD_RotateTransformable(void);

	void execute();
	void unexecute();
private:
	NodeTransformable * node;
	// Original orientations of selected joints
	glm::quat oldOrientation;
	// Rotation to be applied to selected joints
	glm::quat rotation;
	bool relative;
	CoordinateSpace space;
};

