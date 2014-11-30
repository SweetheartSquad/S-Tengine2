#pragma once

#include <vector>

#include <cinder/Vector.h>

#include "Command.h"

class CMD_MoveSelectedJoints : public Command{
public:
	enum MovementMode{
		OBJECT,
		WORLD
	};
	// Relative move will move by _v, non-relative move will move to _v
	// Mode can be OBJECT or WORLD transform
	CMD_MoveSelectedJoints(ci::Vec3d _v, bool _relative, MovementMode _mode);
	~CMD_MoveSelectedJoints(void);

	void execute();
	void unexecute();
private:
	ci::Vec3d v;
	bool relative;
	MovementMode mode;
};

