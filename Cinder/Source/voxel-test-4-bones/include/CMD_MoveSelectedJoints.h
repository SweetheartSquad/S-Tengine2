#pragma once

#include <vector>

#include <cinder/Vector.h>

#include "Command.h"

class CMD_MoveSelectedJoints : public Command{
public:
	// Relative move will move by _v, non-relative move will move to _v
	CMD_MoveSelectedJoints(ci::Vec3d _v, bool _relative);
	~CMD_MoveSelectedJoints(void);

	void execute();
	void unexecute();
private:
	ci::Vec3d v;
	bool relative;
};

