#pragma once

#include <vector>

#include <glm/gtx/quaternion.hpp>

#include "Command.h"

class CMD_RotateSelectedJoints : public Command{
public:
	// Relative move will rotate BY _rotation, non-relative rotate TO _rotation
	CMD_RotateSelectedJoints(glm::quat _rotation, bool _relative);
	~CMD_RotateSelectedJoints(void);

	void execute();
	void unexecute();
private:
	// Original orientations of selected joints
	std::vector<glm::quat> orientations;
	// Rotation to be applied to selected joints
	glm::quat rotation;
	bool relative;
};

