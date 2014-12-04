#pragma once

#include <vector>

#include <glm/gtx/quaternion.hpp>

#include "Command.h"
#include "Transform.h"

class CMD_RotateSelectedTransformable : public Command{
public:
	// Relative will rotate BY _rotation, non-relative will rotate TO _rotation
	// Local = true: OBJECT
	// Local = false: WORLD
	CMD_RotateSelectedTransformable(glm::quat _rotation, bool _relative, CoordinateSpace _space);
	~CMD_RotateSelectedTransformable(void);

	void execute();
	void unexecute();
private:
	// Original orientations of selected joints
	std::vector<glm::quat> orientations;
	// Rotation to be applied to selected joints
	glm::quat rotation;
	bool relative;
	CoordinateSpace space;
};

