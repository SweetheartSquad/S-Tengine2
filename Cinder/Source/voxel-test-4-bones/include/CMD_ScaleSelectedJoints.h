#pragma once

#include <vector>

#include <cinder/Vector.h>

#include "Command.h"

class CMD_ScaleSelectedJoints : public Command{
public:
	// Relative move will move by _v, non-relative move will move to _v
	CMD_ScaleSelectedJoints(ci::Vec3d _v);
	~CMD_ScaleSelectedJoints(void);

	void execute();
	void unexecute();
private:
	ci::Vec3d v;
};